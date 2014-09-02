/*-------------------------------------------------------------------------
 * drawElements Quality Program OpenGL ES Utilities
 * ------------------------------------------------
 *
 * Copyright 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *//*!
 * \file
 * \brief OpenGL ES context wrapper that uses FBO as default framebuffer.
 *//*--------------------------------------------------------------------*/

#include "gluFboRenderContext.hpp"
#include "gluContextFactory.hpp"
#include "gluRenderConfig.hpp"
#include "glwEnums.hpp"
#include "glwFunctions.hpp"
#include "tcuCommandLine.hpp"
#include "gluTextureUtil.hpp"
#include "tcuTextureUtil.hpp"

#include <sstream>

namespace glu
{

static tcu::PixelFormat getPixelFormat (deUint32 colorFormat)
{
	const tcu::IVec4 bits = tcu::getTextureFormatBitDepth(glu::mapGLInternalFormat(colorFormat));
	return tcu::PixelFormat(bits[0], bits[1], bits[2], bits[3]);
}

static void getDepthStencilBits (deUint32 depthStencilFormat, int* depthBits, int* stencilBits)
{
	const tcu::IVec4 bits = tcu::getTextureFormatBitDepth(glu::mapGLInternalFormat(depthStencilFormat));
	*depthBits		= bits[0];
	*stencilBits	= bits[3];
}

deUint32 chooseColorFormat (const glu::RenderConfig& config)
{
	static const deUint32 s_formats[] =
	{
		GL_RGBA8,
		GL_RGB8,
		GL_RG8,
		GL_R8,
		GL_RGBA4,
		GL_RGB5_A1,
		GL_RGB565,
		GL_RGB5
	};

	for (int fmtNdx = 0; fmtNdx < DE_LENGTH_OF_ARRAY(s_formats); fmtNdx++)
	{
		const deUint32		format	= s_formats[fmtNdx];
		const tcu::IVec4	bits	= tcu::getTextureFormatBitDepth(glu::mapGLInternalFormat(format));

		if (config.redBits != glu::RenderConfig::DONT_CARE &&
			config.redBits != bits[0])
			continue;

		if (config.greenBits != glu::RenderConfig::DONT_CARE &&
			config.greenBits != bits[1])
			continue;

		if (config.blueBits != glu::RenderConfig::DONT_CARE &&
			config.blueBits != bits[2])
			continue;

		if (config.alphaBits != glu::RenderConfig::DONT_CARE &&
			config.alphaBits != bits[3])
			continue;

		return format;
	}

	return 0;
}

deUint32 chooseDepthStencilFormat (const glu::RenderConfig& config)
{
	static const deUint32 s_formats[] =
	{
		GL_DEPTH32F_STENCIL8,
		GL_DEPTH24_STENCIL8,
		GL_DEPTH_COMPONENT32F,
		GL_DEPTH_COMPONENT24,
		GL_DEPTH_COMPONENT16,
		GL_STENCIL_INDEX8
	};

	for (int fmtNdx = 0; fmtNdx < DE_LENGTH_OF_ARRAY(s_formats); fmtNdx++)
	{
		const deUint32		format	= s_formats[fmtNdx];
		const tcu::IVec4	bits	= tcu::getTextureFormatBitDepth(glu::mapGLInternalFormat(format));

		if (config.depthBits != glu::RenderConfig::DONT_CARE &&
			config.depthBits != bits[0])
			continue;

		if (config.stencilBits != glu::RenderConfig::DONT_CARE &&
			config.stencilBits != bits[3])
			continue;

		return format;
	}

	return 0;
}

FboRenderContext::FboRenderContext (RenderContext* context, const RenderConfig& config)
	: m_context				(context)
	, m_framebuffer			(0)
	, m_colorBuffer			(0)
	, m_depthStencilBuffer	(0)
	, m_renderTarget		()
{
	try
	{
		createFramebuffer(config);
	}
	catch (...)
	{
		destroyFramebuffer();
		throw;
	}
}

FboRenderContext::FboRenderContext (const ContextFactory& factory, const RenderConfig& config, const tcu::CommandLine& cmdLine)
	: m_context				(DE_NULL)
	, m_framebuffer			(0)
	, m_colorBuffer			(0)
	, m_depthStencilBuffer	(0)
	, m_renderTarget		()
{
	try
	{
		RenderConfig nativeRenderConfig;
		nativeRenderConfig.type				= config.type;
		nativeRenderConfig.windowVisibility	= config.windowVisibility;
		// \note All other properties are defaults, mostly DONT_CARE
		m_context = factory.createContext(nativeRenderConfig, cmdLine);
		createFramebuffer(config);
	}
	catch (...)
	{
		delete m_context;
		throw;
	}
}

FboRenderContext::~FboRenderContext (void)
{
	// \todo [2013-04-08 pyry] Do we want to destry FBO before destroying context?
	delete m_context;
}

void FboRenderContext::postIterate (void)
{
	// \todo [2012-11-27 pyry] Blit to default framebuffer in ES3?
	m_context->getFunctions().finish();
}

void FboRenderContext::createFramebuffer (const RenderConfig& config)
{
	DE_ASSERT(m_framebuffer == 0 && m_colorBuffer == 0 && m_depthStencilBuffer == 0);

	const glw::Functions&	gl					= m_context->getFunctions();
	const deUint32			colorFormat			= chooseColorFormat(config);
	const deUint32			depthStencilFormat	= chooseDepthStencilFormat(config);
	int						width				= config.width;
	int						height				= config.height;
	tcu::PixelFormat		pixelFormat;
	int						depthBits			= 0;
	int						stencilBits			= 0;

	if (config.numSamples > 0 && !gl.renderbufferStorageMultisample)
		throw tcu::NotSupportedError("Multisample FBO is not supported");

	if (colorFormat == 0)
		throw tcu::NotSupportedError("Unsupported color attachment format");

	if (width == glu::RenderConfig::DONT_CARE || height == glu::RenderConfig::DONT_CARE)
	{
		int maxSize = 0;
		gl.getIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxSize);

		width	= (width	== glu::RenderConfig::DONT_CARE) ? maxSize : width;
		height	= (height	== glu::RenderConfig::DONT_CARE) ? maxSize : height;
	}

	{
		pixelFormat = getPixelFormat(colorFormat);

		gl.genRenderbuffers(1, &m_colorBuffer);
		gl.bindRenderbuffer(GL_RENDERBUFFER, m_colorBuffer);

		if (config.numSamples > 0)
			gl.renderbufferStorageMultisample(GL_RENDERBUFFER, config.numSamples, colorFormat, width, height);
		else
			gl.renderbufferStorage(GL_RENDERBUFFER, colorFormat, width, height);

		gl.bindRenderbuffer(GL_RENDERBUFFER, 0);
		GLU_EXPECT_NO_ERROR(gl.getError(), "Creating color renderbuffer");
	}

	if (depthStencilFormat != GL_NONE)
	{
		getDepthStencilBits(depthStencilFormat, &depthBits, &stencilBits);

		gl.genRenderbuffers(1, &m_depthStencilBuffer);
		gl.bindRenderbuffer(GL_RENDERBUFFER, m_depthStencilBuffer);

		if (config.numSamples > 0)
			gl.renderbufferStorageMultisample(GL_RENDERBUFFER, config.numSamples, depthStencilFormat, width, height);
		else
			gl.renderbufferStorage(GL_RENDERBUFFER, depthStencilFormat, width, height);

		gl.bindRenderbuffer(GL_RENDERBUFFER, 0);
		GLU_EXPECT_NO_ERROR(gl.getError(), "Creating depth / stencil renderbuffer");
	}

	gl.genFramebuffers(1, &m_framebuffer);
	gl.bindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

	if (m_colorBuffer)
		gl.framebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorBuffer);

	if (m_depthStencilBuffer)
	{
		if (depthBits > 0)
			gl.framebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilBuffer);

		if (stencilBits > 0)
			gl.framebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilBuffer);
	}

	GLU_EXPECT_NO_ERROR(gl.getError(), "Creating framebuffer");

	if (gl.checkFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw tcu::NotSupportedError("Framebuffer is not complete");

	// Set up correct viewport for first test case.
	gl.viewport(0, 0, width, height);

	m_renderTarget = tcu::RenderTarget(width, height, pixelFormat, depthBits, stencilBits, config.numSamples);
}

void FboRenderContext::destroyFramebuffer (void)
{
	const glw::Functions& gl = m_context->getFunctions();

	if (m_framebuffer)
	{
		gl.deleteFramebuffers(1, &m_framebuffer);
		m_framebuffer = 0;
	}

	if (m_depthStencilBuffer)
	{
		gl.deleteRenderbuffers(1, &m_depthStencilBuffer);
		m_depthStencilBuffer = 0;
	}

	if (m_colorBuffer)
	{
		gl.deleteRenderbuffers(1, &m_colorBuffer);
		m_colorBuffer = 0;
	}
}

} // glu
