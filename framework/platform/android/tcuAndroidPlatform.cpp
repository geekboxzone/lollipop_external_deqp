/*-------------------------------------------------------------------------
 * drawElements Quality Program Tester Core
 * ----------------------------------------
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
 * \brief Android EGL platform.
 *//*--------------------------------------------------------------------*/

#include "tcuAndroidPlatform.hpp"
#include "gluRenderContext.hpp"
#include "egluNativeDisplay.hpp"
#include "egluNativeWindow.hpp"
#include "egluGLContextFactory.hpp"
#include "egluUtil.hpp"

namespace tcu
{
namespace Android
{

static const eglu::NativeDisplay::Capability	DISPLAY_CAPABILITIES	= eglu::NativeDisplay::CAPABILITY_GET_DISPLAY_LEGACY;
static const eglu::NativeWindow::Capability		WINDOW_CAPABILITIES		= (eglu::NativeWindow::Capability)(eglu::NativeWindow::CAPABILITY_CREATE_SURFACE_LEGACY |
																										   eglu::NativeWindow::CAPABILITY_SET_SURFACE_SIZE |
																										   eglu::NativeWindow::CAPABILITY_GET_SCREEN_SIZE);

class NativeDisplay : public eglu::NativeDisplay
{
public:
									NativeDisplay			(void) : eglu::NativeDisplay(DISPLAY_CAPABILITIES) {}
	virtual							~NativeDisplay			(void) {}

	virtual EGLNativeDisplayType	getLegacyNative			(void) { return EGL_DEFAULT_DISPLAY; }
};

class NativeDisplayFactory : public eglu::NativeDisplayFactory
{
public:
									NativeDisplayFactory	(Window& window);
									~NativeDisplayFactory	(void) {}

	virtual eglu::NativeDisplay*	createDisplay			(const EGLAttrib* attribList) const;
};

class NativeWindow : public eglu::NativeWindow
{
public:
									NativeWindow			(Window& window, int width, int height, int32_t format);
	virtual							~NativeWindow			(void);

	virtual EGLNativeWindowType		getLegacyNative			(void)			{ return m_window.getNativeWindow();	}
	IVec2							getScreenSize			(void) const	{ return m_window.getSize();			}

	void							setSurfaceSize			(IVec2 size);

	virtual void					processEvents			(void);

private:
	Window&							m_window;
	int32_t							m_format;
};

class NativeWindowFactory : public eglu::NativeWindowFactory
{
public:
									NativeWindowFactory		(Window& window);
									~NativeWindowFactory	(void);

	virtual eglu::NativeWindow*		createWindow			(eglu::NativeDisplay* nativeDisplay, const eglu::WindowParams& params) const;
	virtual eglu::NativeWindow*		createWindow			(eglu::NativeDisplay* nativeDisplay, EGLDisplay display, EGLConfig config, const EGLAttrib* attribList, const eglu::WindowParams& params) const;

private:
	Window&							m_window;
};

// NativeWindow

NativeWindow::NativeWindow (Window& window, int width, int height, int32_t format)
	: eglu::NativeWindow	(WINDOW_CAPABILITIES)
	, m_window				(window)
	, m_format				(format)
{
	// Try to acquire window.
	if (!m_window.tryAcquire())
		throw ResourceError("Native window is not available", "", __FILE__, __LINE__);

	// Set up buffers.
	setSurfaceSize(IVec2(width, height));
}

NativeWindow::~NativeWindow (void)
{
	m_window.release();
}

void NativeWindow::processEvents (void)
{
}

void NativeWindow::setSurfaceSize (tcu::IVec2 size)
{
	m_window.setBuffersGeometry(size.x() != eglu::WindowParams::SIZE_DONT_CARE ? size.x() : 0,
								size.y() != eglu::WindowParams::SIZE_DONT_CARE ? size.y() : 0,
								m_format);
}

// NativeWindowFactory

NativeWindowFactory::NativeWindowFactory (Window& window)
	: eglu::NativeWindowFactory	("default", "Default display", WINDOW_CAPABILITIES)
	, m_window					(window)
{
}

NativeWindowFactory::~NativeWindowFactory (void)
{
}

eglu::NativeWindow* NativeWindowFactory::createWindow (eglu::NativeDisplay* nativeDisplay, const eglu::WindowParams& params) const
{
	DE_UNREF(nativeDisplay);
	return new NativeWindow(m_window, params.width, params.height, WINDOW_FORMAT_RGBA_8888);
}

eglu::NativeWindow* NativeWindowFactory::createWindow (eglu::NativeDisplay* nativeDisplay, EGLDisplay display, EGLConfig config, const EGLAttrib* attribList, const eglu::WindowParams& params) const
{
	const int32_t format = (int32_t)eglu::getConfigAttribInt(display, config, EGL_NATIVE_VISUAL_ID);
	DE_UNREF(nativeDisplay && attribList);
	return new NativeWindow(m_window, params.width, params.height, format);
}

// NativeDisplayFactory

NativeDisplayFactory::NativeDisplayFactory (Window& window)
	: eglu::NativeDisplayFactory("default", "Default display", DISPLAY_CAPABILITIES)
{
	m_nativeWindowRegistry.registerFactory(new NativeWindowFactory(window));
}

eglu::NativeDisplay* NativeDisplayFactory::createDisplay (const EGLAttrib* attribList) const
{
	DE_UNREF(attribList);
	return new NativeDisplay();
}

// Platform

Platform::Platform (ANativeWindow* window)
	: m_window(window)
{
	m_nativeDisplayFactoryRegistry.registerFactory(new NativeDisplayFactory(m_window));
	m_contextFactoryRegistry.registerFactory(new eglu::GLContextFactory(m_nativeDisplayFactoryRegistry));
}

Platform::~Platform (void)
{
}

} // Android
} // tcu
