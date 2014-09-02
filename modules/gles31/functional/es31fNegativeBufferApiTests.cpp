/*-------------------------------------------------------------------------
 * drawElements Quality Program OpenGL ES 3.1 Module
 * -------------------------------------------------
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
 * \brief Negative Buffer API tests.
 *//*--------------------------------------------------------------------*/

#include "es31fNegativeBufferApiTests.hpp"

#include "gluCallLogWrapper.hpp"

#include "glwDefs.hpp"
#include "glwEnums.hpp"

namespace deqp
{
namespace gles31
{
namespace Functional
{
namespace NegativeTestShared
{

using tcu::TestLog;
using glu::CallLogWrapper;
using namespace glw;

// Buffers
void bind_buffer (NegativeTestContext& ctx)
{
	ctx.beginSection("GL_INVALID_ENUM is generated if target is not one of the allowable values.");
	ctx.glBindBuffer(-1, 0);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.endSection();
}

void delete_buffers (NegativeTestContext& ctx)
{
	ctx.beginSection("GL_INVALID_VALUE is generated if n is negative.");
	ctx.glDeleteBuffers(-1, 0);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.endSection();
}

void gen_buffers (NegativeTestContext& ctx)
{
	ctx.beginSection("GL_INVALID_VALUE is generated if n is negative.");
	ctx.glGenBuffers(-1, 0);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.endSection();
}

void buffer_data (NegativeTestContext& ctx)
{
	GLuint buffer = 0x1234;
	ctx.glGenBuffers(1, &buffer);
	ctx.glBindBuffer(GL_ARRAY_BUFFER, buffer);

	ctx.beginSection("GL_INVALID_ENUM is generated if target is not GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER.");
	ctx.glBufferData(-1, 0, NULL, GL_STREAM_DRAW);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_ENUM is generated if usage is not GL_STREAM_DRAW, GL_STATIC_DRAW, or GL_DYNAMIC_DRAW.");
	ctx.glBufferData(GL_ARRAY_BUFFER, 0, NULL, -1);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if size is negative.");
	ctx.glBufferData(GL_ARRAY_BUFFER, -1, NULL, GL_STREAM_DRAW);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if the reserved buffer object name 0 is bound to target.");
	ctx.glBindBuffer(GL_ARRAY_BUFFER, 0);
	ctx.glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STREAM_DRAW);
	ctx.expectError(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.glDeleteBuffers(1, &buffer);
}

void buffer_sub_data (NegativeTestContext& ctx)
{
	GLuint buffer = 0x1234;
	ctx.glGenBuffers(1, &buffer);
	ctx.glBindBuffer(GL_ARRAY_BUFFER, buffer);
	ctx.glBufferData(GL_ARRAY_BUFFER, 10, 0, GL_STREAM_DRAW);

	ctx.beginSection("GL_INVALID_ENUM is generated if target is not GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER.");
	ctx.glBufferSubData(-1, 1, 1, 0);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if the reserved buffer object name 0 is bound to target.");
	ctx.glBindBuffer(GL_ARRAY_BUFFER, 0);
	ctx.glBufferSubData(GL_ARRAY_BUFFER, 1, 1, 0);
	ctx.expectError(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if the buffer object being updated is mapped.");
	ctx.glBindBuffer(GL_ARRAY_BUFFER, buffer);
	ctx.glMapBufferRange(GL_ARRAY_BUFFER, 0, 5, GL_MAP_READ_BIT);
	ctx.expectError(GL_NO_ERROR);
	ctx.glBufferSubData(GL_ARRAY_BUFFER, 1, 1, 0);
	ctx.expectError(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.glDeleteBuffers(1, &buffer);
}

void buffer_sub_data_size_offset (NegativeTestContext& ctx)
{
	GLuint buffer = 0x1234;
	ctx.glGenBuffers(1, &buffer);
	ctx.glBindBuffer(GL_ARRAY_BUFFER, buffer);
	ctx.glBufferData(GL_ARRAY_BUFFER, 10, 0, GL_STREAM_DRAW);

	ctx.beginSection("GL_INVALID_VALUE is generated if offset or size is negative, or if together they define a region of memory that extends beyond the buffer object's allocated data store.");
	ctx.glBufferSubData(GL_ARRAY_BUFFER, -1, 1, 0);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.glBufferSubData(GL_ARRAY_BUFFER, -1, -1, 0);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.glBufferSubData(GL_ARRAY_BUFFER, 1, -1, 0);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.glBufferSubData(GL_ARRAY_BUFFER, 15, 1, 0);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.glBufferSubData(GL_ARRAY_BUFFER, 1, 15, 0);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.glBufferSubData(GL_ARRAY_BUFFER, 8, 8, 0);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.glDeleteBuffers(1, &buffer);
}

void clear (NegativeTestContext& ctx)
{
	ctx.beginSection("GL_INVALID_VALUE is generated if any bit other than the three defined bits is set in mask.");
	ctx.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	ctx.expectError(GL_NO_ERROR);
	ctx.glClear(0x00000200);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.glClear(0x00001000);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.glClear(0x00000010);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.endSection();
}

void read_pixels (NegativeTestContext& ctx)
{
	std::vector<GLubyte> ubyteData(4);

	ctx.beginSection("GL_INVALID_OPERATION is generated if the combination of format and type is unsupported.");
	ctx.glReadPixels(0, 0, 1, 1, GL_LUMINANCE_ALPHA, GL_UNSIGNED_SHORT_4_4_4_4, &ubyteData[0]);
	ctx.expectError(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if either width or height is negative.");
	ctx.glReadPixels(0, 0, -1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &ubyteData[0]);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.glReadPixels(0, 0, 1, -1, GL_RGBA, GL_UNSIGNED_BYTE, &ubyteData[0]);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.glReadPixels(0, 0, -1, -1, GL_RGBA, GL_UNSIGNED_BYTE, &ubyteData[0]);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_FRAMEBUFFER_OPERATION is generated if the currently bound framebuffer is not framebuffer complete.");
	GLuint fbo = 0x1234;
	ctx.glGenFramebuffers(1, &fbo);
	ctx.glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	ctx.glCheckFramebufferStatus(GL_FRAMEBUFFER);
	ctx.glReadPixels(0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &ubyteData[0]);
	ctx.expectError(GL_INVALID_FRAMEBUFFER_OPERATION);
	ctx.endSection();

	ctx.glDeleteFramebuffers(1, &fbo);
}

void read_pixels_format_mismatch (NegativeTestContext& ctx)
{
	std::vector<GLubyte> ubyteData(4);
	std::vector<GLushort> ushortData(4);

	ctx.beginSection("Unsupported combinations of format and type will generate an INVALID_OPERATION error.");
	ctx.glReadPixels(0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_SHORT_5_6_5, &ushortData[0]);
	ctx.expectError(GL_INVALID_OPERATION);
	ctx.glReadPixels(0, 0, 1, 1, GL_ALPHA, GL_UNSIGNED_SHORT_5_6_5, &ushortData[0]);
	ctx.expectError(GL_INVALID_OPERATION);
	ctx.glReadPixels(0, 0, 1, 1, GL_RGB, GL_UNSIGNED_SHORT_4_4_4_4, &ushortData[0]);
	ctx.expectError(GL_INVALID_OPERATION);
	ctx.glReadPixels(0, 0, 1, 1, GL_ALPHA, GL_UNSIGNED_SHORT_4_4_4_4, &ushortData[0]);
	ctx.expectError(GL_INVALID_OPERATION);
	ctx.glReadPixels(0, 0, 1, 1, GL_RGB, GL_UNSIGNED_SHORT_5_5_5_1, &ushortData[0]);
	ctx.expectError(GL_INVALID_OPERATION);
	ctx.glReadPixels(0, 0, 1, 1, GL_ALPHA, GL_UNSIGNED_SHORT_5_5_5_1, &ushortData[0]);
	ctx.expectError(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.beginSection("GL_RGBA/GL_UNSIGNED_BYTE is always accepted and the other acceptable pair can be discovered by querying GL_IMPLEMENTATION_COLOR_READ_FORMAT and GL_IMPLEMENTATION_COLOR_READ_TYPE.");
	ctx.glReadPixels(0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &ubyteData[0]);
	ctx.expectError(GL_NO_ERROR);
	GLint readFormat = 0x1234;
	GLint readType = 0x1234;
	ctx.glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_FORMAT, &readFormat);
	ctx.glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_TYPE, &readType);
	ctx.glReadPixels(0, 0, 1, 1, readFormat, readType, &ubyteData[0]);
	ctx.expectError(GL_NO_ERROR);
	ctx.endSection();
}

void read_pixels_fbo_format_mismatch (NegativeTestContext& ctx)
{
	std::vector<GLubyte>	ubyteData(4);
	std::vector<float>		floatData(4);
	deUint32				fbo = 0x1234;
	deUint32				texture = 0x1234;

	ctx.glGenTextures			(1, &texture);
	ctx.glBindTexture			(GL_TEXTURE_2D, texture);
	ctx.glTexImage2D			(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	ctx.glGenFramebuffers		(1, &fbo);
	ctx.glBindFramebuffer		(GL_FRAMEBUFFER, fbo);
	ctx.glFramebufferTexture2D	(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	ctx.expectError				(GL_NO_ERROR);

	ctx.beginSection("GL_INVALID_OPERATION is generated if currently bound framebuffer format is incompatible with format and type.");

	ctx.glTexImage2D			(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	ctx.glFramebufferTexture2D	(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	ctx.glCheckFramebufferStatus(GL_FRAMEBUFFER);
	ctx.expectError				(GL_NO_ERROR);
	ctx.glReadPixels			(0, 0, 1, 1, GL_RGBA, GL_FLOAT, &floatData[0]);
	ctx.expectError				(GL_INVALID_OPERATION);

	ctx.glTexImage2D			(GL_TEXTURE_2D, 0, GL_RGBA32I, 32, 32, 0, GL_RGBA_INTEGER, GL_INT, NULL);
	ctx.glFramebufferTexture2D	(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	ctx.glCheckFramebufferStatus(GL_FRAMEBUFFER);
	ctx.expectError				(GL_NO_ERROR);
	ctx.glReadPixels			(0, 0, 1, 1, GL_RGBA, GL_FLOAT, &floatData[0]);
	ctx.expectError				(GL_INVALID_OPERATION);

	ctx.glTexImage2D			(GL_TEXTURE_2D, 0, GL_RGBA32UI, 32, 32, 0, GL_RGBA_INTEGER, GL_UNSIGNED_INT, NULL);
	ctx.glFramebufferTexture2D	(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	ctx.glCheckFramebufferStatus(GL_FRAMEBUFFER);
	ctx.expectError				(GL_NO_ERROR);
	ctx.glReadPixels			(0, 0, 1, 1, GL_RGBA, GL_FLOAT, &floatData[0]);
	ctx.expectError				(GL_INVALID_OPERATION);

	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if GL_READ_FRAMEBUFFER_BINDING is non-zero, the read framebuffer is complete, and the value of GL_SAMPLE_BUFFERS for the read framebuffer is greater than zero.");

	int			binding			= -1;
	int			sampleBuffers = 0x1234;
	deUint32	rbo = 0x1234;

	ctx.glGenRenderbuffers(1, &rbo);
	ctx.glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	ctx.glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA8, 32, 32);
	ctx.glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo);

	ctx.glGetIntegerv			(GL_READ_FRAMEBUFFER_BINDING, &binding);
	ctx.getLog() << TestLog::Message << "// GL_READ_FRAMEBUFFER_BINDING: " << binding << TestLog::EndMessage;
	ctx.glCheckFramebufferStatus(GL_FRAMEBUFFER);
	ctx.glGetIntegerv			(GL_SAMPLE_BUFFERS, &sampleBuffers);
	ctx.getLog() << TestLog::Message << "// GL_SAMPLE_BUFFERS: " << sampleBuffers << TestLog::EndMessage;
	ctx.expectError				(GL_NO_ERROR);

	if (binding == 0 || sampleBuffers <= 0)
	{
		ctx.getLog() << TestLog::Message << "// ERROR: expected GL_READ_FRAMEBUFFER_BINDING to be non-zero and GL_SAMPLE_BUFFERS to be greater than zero" << TestLog::EndMessage;
		ctx.fail("Got invalid value");
	}
	else
	{
		ctx.glReadPixels	(0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &ubyteData[0]);
		ctx.expectError		(GL_INVALID_OPERATION);
	}

	ctx.endSection();

	ctx.glBindRenderbuffer		(GL_RENDERBUFFER, 0);
	ctx.glBindTexture			(GL_TEXTURE_2D, 0);
	ctx.glBindFramebuffer		(GL_FRAMEBUFFER, 0);
	ctx.glDeleteFramebuffers	(1, &fbo);
	ctx.glDeleteTextures		(1, &texture);
	ctx.glDeleteRenderbuffers	(1, &rbo);
}

void bind_buffer_range (NegativeTestContext& ctx)
{
	deUint32 bufU = 0x1234;
	ctx.glGenBuffers(1, &bufU);
	ctx.glBindBuffer(GL_UNIFORM_BUFFER, bufU);
	ctx.glBufferData(GL_UNIFORM_BUFFER, 16, NULL, GL_STREAM_DRAW);

	deUint32 bufTF = 0x1234;
	ctx.glGenBuffers(1, &bufTF);
	ctx.glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, bufTF);
	ctx.glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, 16, NULL, GL_STREAM_DRAW);

	ctx.beginSection("GL_INVALID_ENUM is generated if target is not GL_TRANSFORM_FEEDBACK_BUFFER or GL_UNIFORM_BUFFER.");
	ctx.glBindBufferRange(GL_ARRAY_BUFFER, 0, bufU, 0, 4);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if target is GL_TRANSFORM_FEEDBACK_BUFFER and index is greater than or equal to GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS.");
	int maxTFSize = 0x1234;
	ctx.glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS, &maxTFSize);
	ctx.glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, maxTFSize, bufTF, 0, 4);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if target is GL_UNIFORM_BUFFER and index is greater than or equal to GL_MAX_UNIFORM_BUFFER_BINDINGS.");
	int maxUSize = 0x1234;
	ctx.glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxUSize);
	ctx.glBindBufferRange(GL_UNIFORM_BUFFER, maxUSize, bufU, 0, 4);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if size is less than or equal to zero.");
	ctx.glBindBufferRange(GL_UNIFORM_BUFFER, 0, bufU, 0, -1);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.glBindBufferRange(GL_UNIFORM_BUFFER, 0, bufU, 0, 0);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if target is GL_TRANSFORM_FEEDBACK_BUFFER and size or offset are not multiples of 4.");
	ctx.glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, 0, bufTF, 4, 5);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, 0, bufTF, 5, 4);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, 0, bufTF, 5, 7);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if target is GL_UNIFORM_BUFFER and offset is not a multiple of GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT.");
	int alignment = 0x1234;
	ctx.glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &alignment);
	ctx.glBindBufferRange(GL_UNIFORM_BUFFER, 0, bufU, alignment+1, 4);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.glDeleteBuffers(1, &bufU);
	ctx.glDeleteBuffers(1, &bufTF);
}

void bind_buffer_base (NegativeTestContext& ctx)
{
	deUint32 bufU = 0x1234;
	ctx.glGenBuffers(1, &bufU);
	ctx.glBindBuffer(GL_UNIFORM_BUFFER, bufU);
	ctx.glBufferData(GL_UNIFORM_BUFFER, 16, NULL, GL_STREAM_DRAW);

	deUint32 bufTF = 0x1234;
	ctx.glGenBuffers(1, &bufTF);
	ctx.glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, bufTF);
	ctx.glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, 16, NULL, GL_STREAM_DRAW);
	ctx.expectError(GL_NO_ERROR);

	ctx.beginSection("GL_INVALID_ENUM is generated if target is not GL_TRANSFORM_FEEDBACK_BUFFER or GL_UNIFORM_BUFFER.");
	ctx.glBindBufferBase(-1, 0, bufU);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.glBindBufferBase(GL_ARRAY_BUFFER, 0, bufU);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if target is GL_UNIFORM_BUFFER and index is greater than or equal to GL_MAX_UNIFORM_BUFFER_BINDINGS.");
	int maxUSize = 0x1234;
	ctx.glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxUSize);
	ctx.glBindBufferBase(GL_UNIFORM_BUFFER, maxUSize, bufU);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if target is GL_TRANSFORM_FEEDBACK_BUFFER andindex is greater than or equal to GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS.");
	int maxTFSize = 0x1234;
	ctx.glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS, &maxTFSize);
	ctx.glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, maxTFSize, bufTF);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.glDeleteBuffers(1, &bufU);
	ctx.glDeleteBuffers(1, &bufTF);
}

void clear_bufferiv (NegativeTestContext& ctx)
{
	std::vector<int>		data(32*32);
	deUint32				fbo = 0x1234;
	deUint32				texture = 0x1234;

	ctx.glGenTextures			(1, &texture);
	ctx.glBindTexture			(GL_TEXTURE_2D, texture);
	ctx.glTexImage2D			(GL_TEXTURE_2D, 0, GL_RGBA32I, 32, 32, 0, GL_RGBA_INTEGER, GL_INT, NULL);
	ctx.glGenFramebuffers		(1, &fbo);
	ctx.glBindFramebuffer		(GL_FRAMEBUFFER, fbo);
	ctx.glFramebufferTexture2D	(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	ctx.glCheckFramebufferStatus(GL_FRAMEBUFFER);
	ctx.expectError				(GL_NO_ERROR);

	ctx.beginSection("GL_INVALID_ENUM is generated if buffer is not an accepted value.");
	ctx.glClearBufferiv(-1, 0, &data[0]);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.glClearBufferiv(GL_FRAMEBUFFER, 0, &data[0]);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if buffer is GL_COLOR, GL_FRONT, GL_BACK, GL_LEFT, GL_RIGHT, or GL_FRONT_AND_BACK and drawBuffer is greater than or equal to GL_MAX_DRAW_BUFFERS.");
	int maxDrawBuffers = 0x1234;
	ctx.glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuffers);
	ctx.glClearBufferiv(GL_COLOR, maxDrawBuffers, &data[0]);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_ENUM is generated if buffer is GL_DEPTH or GL_DEPTH_STENCIL.");
	ctx.glClearBufferiv(GL_DEPTH, 1, &data[0]);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.glClearBufferiv(GL_DEPTH_STENCIL, 1, &data[0]);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if buffer is GL_STENCIL and drawBuffer is not zero.");
	ctx.glClearBufferiv(GL_STENCIL, 1, &data[0]);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.glDeleteFramebuffers(1, &fbo);
	ctx.glDeleteTextures(1, &texture);
}

void clear_bufferuiv (NegativeTestContext& ctx)
{
	std::vector<deUint32>	data(32*32);
	deUint32				fbo = 0x1234;
	deUint32				texture = 0x1234;

	ctx.glGenTextures			(1, &texture);
	ctx.glBindTexture			(GL_TEXTURE_2D, texture);
	ctx.glTexImage2D			(GL_TEXTURE_2D, 0, GL_RGBA32UI, 32, 32, 0, GL_RGBA_INTEGER, GL_UNSIGNED_INT, NULL);
	ctx.glGenFramebuffers		(1, &fbo);
	ctx.glBindFramebuffer		(GL_FRAMEBUFFER, fbo);
	ctx.glFramebufferTexture2D	(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	ctx.glCheckFramebufferStatus(GL_FRAMEBUFFER);
	ctx.expectError				(GL_NO_ERROR);

	ctx.beginSection("GL_INVALID_ENUM is generated if buffer is not an accepted value.");
	ctx.glClearBufferuiv(-1, 0, &data[0]);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.glClearBufferuiv(GL_FRAMEBUFFER, 0, &data[0]);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if buffer is GL_COLOR, GL_FRONT, GL_BACK, GL_LEFT, GL_RIGHT, or GL_FRONT_AND_BACK and drawBuffer is greater than or equal to GL_MAX_DRAW_BUFFERS.");
	int maxDrawBuffers = 0x1234;
	ctx.glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuffers);
	ctx.glClearBufferuiv(GL_COLOR, maxDrawBuffers, &data[0]);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_ENUM is generated if buffer is GL_DEPTH, GL_STENCIL or GL_DEPTH_STENCIL.");
	ctx.glClearBufferuiv(GL_DEPTH, 1, &data[0]);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.glClearBufferuiv(GL_STENCIL, 1, &data[0]);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.glClearBufferuiv(GL_DEPTH_STENCIL, 1, &data[0]);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.endSection();

	ctx.glDeleteFramebuffers(1, &fbo);
	ctx.glDeleteTextures(1, &texture);
}

void clear_bufferfv (NegativeTestContext& ctx)
{
	std::vector<float>		data(32*32);
	deUint32				fbo = 0x1234;
	deUint32				texture = 0x1234;

	ctx.glGenTextures			(1, &texture);
	ctx.glBindTexture			(GL_TEXTURE_2D, texture);
	ctx.glTexImage2D			(GL_TEXTURE_2D, 0, GL_RGBA32F, 32, 32, 0, GL_RGBA, GL_FLOAT, NULL);
	ctx.glGenFramebuffers		(1, &fbo);
	ctx.glBindFramebuffer		(GL_FRAMEBUFFER, fbo);
	ctx.glFramebufferTexture2D	(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	ctx.glCheckFramebufferStatus(GL_FRAMEBUFFER);
	ctx.expectError				(GL_NO_ERROR);

	ctx.beginSection("GL_INVALID_ENUM is generated if buffer is not an accepted value.");
	ctx.glClearBufferfv(-1, 0, &data[0]);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.glClearBufferfv(GL_FRAMEBUFFER, 0, &data[0]);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if buffer is GL_COLOR, GL_FRONT, GL_BACK, GL_LEFT, GL_RIGHT, or GL_FRONT_AND_BACK and drawBuffer is greater than or equal to GL_MAX_DRAW_BUFFERS.");
	int maxDrawBuffers = 0x1234;
	ctx.glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuffers);
	ctx.glClearBufferfv(GL_COLOR, maxDrawBuffers, &data[0]);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_ENUM is generated if buffer is GL_STENCIL or GL_DEPTH_STENCIL.");
	ctx.glClearBufferfv(GL_STENCIL, 1, &data[0]);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.glClearBufferfv(GL_DEPTH_STENCIL, 1, &data[0]);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if buffer is GL_DEPTH and drawBuffer is not zero.");
	ctx.glClearBufferfv(GL_DEPTH, 1, &data[0]);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.glDeleteFramebuffers(1, &fbo);
	ctx.glDeleteTextures(1, &texture);
}

void clear_bufferfi (NegativeTestContext& ctx)
{
	ctx.beginSection("GL_INVALID_ENUM is generated if buffer is not an accepted value.");
	ctx.glClearBufferfi(-1, 0, 1.0f, 1);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.glClearBufferfi(GL_FRAMEBUFFER, 0, 1.0f, 1);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_ENUM is generated if buffer is not GL_DEPTH_STENCIL.");
	ctx.glClearBufferfi(GL_DEPTH, 0, 1.0f, 1);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.glClearBufferfi(GL_STENCIL, 0, 1.0f, 1);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.glClearBufferfi(GL_COLOR, 0, 1.0f, 1);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if buffer is GL_DEPTH_STENCIL and drawBuffer is not zero.");
	ctx.glClearBufferfi(GL_DEPTH_STENCIL, 1, 1.0f, 1);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.endSection();
}

void copy_buffer_sub_data (NegativeTestContext& ctx)
{
	deUint32				buf[2];
	std::vector<float>		data(32*32);

	ctx.glGenBuffers			(2, buf);
	ctx.glBindBuffer			(GL_COPY_READ_BUFFER, buf[0]);
	ctx.glBufferData			(GL_COPY_READ_BUFFER, 32, &data[0], GL_DYNAMIC_COPY);
	ctx.glBindBuffer			(GL_COPY_WRITE_BUFFER, buf[1]);
	ctx.glBufferData			(GL_COPY_WRITE_BUFFER, 32, &data[0], GL_DYNAMIC_COPY);
	ctx.expectError				(GL_NO_ERROR);

	ctx.beginSection("GL_INVALID_VALUE is generated if any of readoffset, writeoffset or size is negative.");
	ctx.glCopyBufferSubData		(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, -4);
	ctx.expectError				(GL_INVALID_VALUE);
	ctx.glCopyBufferSubData		(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, -1, 0, 4);
	ctx.expectError				(GL_INVALID_VALUE);
	ctx.glCopyBufferSubData		(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, -1, 4);
	ctx.expectError				(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if readoffset + size exceeds the size of the buffer object bound to readtarget.");
	ctx.glCopyBufferSubData		(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, 36);
	ctx.expectError				(GL_INVALID_VALUE);
	ctx.glCopyBufferSubData		(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 24, 0, 16);
	ctx.expectError				(GL_INVALID_VALUE);
	ctx.glCopyBufferSubData		(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 36, 0, 4);
	ctx.expectError				(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if writeoffset + size exceeds the size of the buffer object bound to writetarget.");
	ctx.glCopyBufferSubData		(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, 36);
	ctx.expectError				(GL_INVALID_VALUE);
	ctx.glCopyBufferSubData		(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 24, 16);
	ctx.expectError				(GL_INVALID_VALUE);
	ctx.glCopyBufferSubData		(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 36, 4);
	ctx.expectError				(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if the same buffer object is bound to both readtarget and writetarget and the ranges [readoffset, readoffset + size) and [writeoffset, writeoffset + size) overlap.");
	ctx.glBindBuffer			(GL_COPY_WRITE_BUFFER, buf[0]);
	ctx.glCopyBufferSubData		(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 16, 4);
	ctx.expectError				(GL_NO_ERROR);
	ctx.glCopyBufferSubData		(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, 4);
	ctx.expectError				(GL_INVALID_VALUE);
	ctx.glCopyBufferSubData		(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 16, 18);
	ctx.expectError				(GL_INVALID_VALUE);
	ctx.glBindBuffer			(GL_COPY_WRITE_BUFFER, buf[1]);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if zero is bound to readtarget or writetarget.");
	ctx.glBindBuffer			(GL_COPY_READ_BUFFER, 0);
	ctx.glCopyBufferSubData		(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, 16);
	ctx.expectError				(GL_INVALID_OPERATION);

	ctx.glBindBuffer			(GL_COPY_READ_BUFFER, buf[0]);
	ctx.glBindBuffer			(GL_COPY_WRITE_BUFFER, 0);
	ctx.glCopyBufferSubData		(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, 16);
	ctx.expectError				(GL_INVALID_OPERATION);

	ctx.glBindBuffer			(GL_COPY_WRITE_BUFFER, buf[1]);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if the buffer object bound to either readtarget or writetarget is mapped.");
	ctx.glMapBufferRange		(GL_COPY_READ_BUFFER, 0, 4, GL_MAP_READ_BIT);
	ctx.glCopyBufferSubData		(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, 16);
	ctx.expectError				(GL_INVALID_OPERATION);
	ctx.glUnmapBuffer			(GL_COPY_READ_BUFFER);

	ctx.glMapBufferRange		(GL_COPY_WRITE_BUFFER, 0, 4, GL_MAP_READ_BIT);
	ctx.glCopyBufferSubData		(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, 16);
	ctx.expectError				(GL_INVALID_OPERATION);
	ctx.glUnmapBuffer			(GL_COPY_WRITE_BUFFER);
	ctx.endSection();

	ctx.glDeleteBuffers(2, buf);
}

void draw_buffers (NegativeTestContext& ctx)
{
	deUint32				fbo = 0x1234;
	deUint32				texture = 0x1234;
	int						maxDrawBuffers = 0x1234;
	ctx.glGetIntegerv		(GL_MAX_DRAW_BUFFERS, &maxDrawBuffers);
	std::vector<deUint32>	values(maxDrawBuffers+1);
	values[0]				= GL_NONE;
	values[1]				= GL_BACK;
	values[2]				= GL_COLOR_ATTACHMENT0;
	values[3]				= GL_DEPTH_ATTACHMENT;
	std::vector<GLfloat>	data(32*32);

	ctx.glGenTextures			(1, &texture);
	ctx.glBindTexture			(GL_TEXTURE_2D, texture);
	ctx.glTexImage2D			(GL_TEXTURE_2D, 0, GL_RGBA8, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	ctx.glGenFramebuffers		(1, &fbo);
	ctx.glBindFramebuffer		(GL_FRAMEBUFFER, fbo);
	ctx.glFramebufferTexture2D	(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	ctx.glCheckFramebufferStatus(GL_FRAMEBUFFER);
	ctx.expectError				(GL_NO_ERROR);

	ctx.beginSection("GL_INVALID_ENUM is generated if one of the values in bufs is not an accepted value.");
	ctx.glDrawBuffers			(2, &values[2]);
	ctx.expectError				(GL_INVALID_ENUM);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if the GL is bound to the default framebuffer and n is not 1.");
	ctx.glBindFramebuffer		(GL_FRAMEBUFFER, 0);
	ctx.glDrawBuffers			(2, &values[0]);
	ctx.expectError				(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if the GL is bound to the default framebuffer and the value in bufs is one of the GL_COLOR_ATTACHMENTn tokens.");
	ctx.glBindFramebuffer		(GL_FRAMEBUFFER, 0);
	ctx.glDrawBuffers			(1, &values[2]);
	ctx.expectError				(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if the GL is bound to a framebuffer object and the ith buffer listed in bufs is anything other than GL_NONE or GL_COLOR_ATTACHMENTSi.");
	ctx.glBindFramebuffer		(GL_FRAMEBUFFER, fbo);
	ctx.glDrawBuffers			(1, &values[1]);
	ctx.expectError				(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if n is less than 0 or greater than GL_MAX_DRAW_BUFFERS.");
	ctx.glDrawBuffers			(-1, &values[1]);
	ctx.expectError				(GL_INVALID_VALUE);
	ctx.glDrawBuffers			(maxDrawBuffers+1, &values[0]);
	ctx.expectError				(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.glDeleteTextures(1, &texture);
	ctx.glDeleteFramebuffers(1, &fbo);
}

void flush_mapped_buffer_range (NegativeTestContext& ctx)
{
	deUint32				buf = 0x1234;
	std::vector<GLfloat>	data(32);

	ctx.glGenBuffers			(1, &buf);
	ctx.glBindBuffer			(GL_ARRAY_BUFFER, buf);
	ctx.glBufferData			(GL_ARRAY_BUFFER, 32, &data[0], GL_STATIC_READ);
	ctx.glMapBufferRange		(GL_ARRAY_BUFFER, 0, 16, GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);
	ctx.expectError				(GL_NO_ERROR);

	ctx.beginSection("GL_INVALID_VALUE is generated if offset or length is negative, or if offset + length exceeds the size of the mapping.");
	ctx.glFlushMappedBufferRange(GL_ARRAY_BUFFER, -1, 1);
	ctx.expectError				(GL_INVALID_VALUE);
	ctx.glFlushMappedBufferRange(GL_ARRAY_BUFFER, 0, -1);
	ctx.expectError				(GL_INVALID_VALUE);
	ctx.glFlushMappedBufferRange(GL_ARRAY_BUFFER, 12, 8);
	ctx.expectError				(GL_INVALID_VALUE);
	ctx.glFlushMappedBufferRange(GL_ARRAY_BUFFER, 24, 4);
	ctx.expectError				(GL_INVALID_VALUE);
	ctx.glFlushMappedBufferRange(GL_ARRAY_BUFFER, 0, 24);
	ctx.expectError				(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if zero is bound to target.");
	ctx.glBindBuffer			(GL_ARRAY_BUFFER, 0);
	ctx.glFlushMappedBufferRange(GL_ARRAY_BUFFER, 0, 8);
	ctx.expectError				(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if the buffer bound to target is not mapped, or is mapped without the GL_MAP_FLUSH_EXPLICIT flag.");
	ctx.glBindBuffer			(GL_ARRAY_BUFFER, buf);
	ctx.glUnmapBuffer			(GL_ARRAY_BUFFER);
	ctx.glFlushMappedBufferRange(GL_ARRAY_BUFFER, 0, 8);
	ctx.expectError				(GL_INVALID_OPERATION);
	ctx.glMapBufferRange		(GL_ARRAY_BUFFER, 0, 16, GL_MAP_WRITE_BIT);
	ctx.glFlushMappedBufferRange(GL_ARRAY_BUFFER, 0, 8);
	ctx.expectError				(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.glUnmapBuffer			(GL_ARRAY_BUFFER);
	ctx.glDeleteBuffers			(1, &buf);
}

void map_buffer_range (NegativeTestContext& ctx)
{
	deUint32				buf = 0x1234;
	std::vector<GLfloat>	data(32);

	ctx.glGenBuffers			(1, &buf);
	ctx.glBindBuffer			(GL_ARRAY_BUFFER, buf);
	ctx.glBufferData			(GL_ARRAY_BUFFER, 32, &data[0], GL_DYNAMIC_COPY);
	ctx.expectError				(GL_NO_ERROR);

	ctx.beginSection("GL_INVALID_VALUE is generated if either of offset or length is negative.");
	ctx.glMapBufferRange		(GL_ARRAY_BUFFER, -1, 1, GL_MAP_READ_BIT);
	ctx.expectError				(GL_INVALID_VALUE);

	ctx.glMapBufferRange		(GL_ARRAY_BUFFER, 1, -1, GL_MAP_READ_BIT);
	ctx.expectError				(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if offset + length is greater than the value of GL_BUFFER_SIZE.");
	ctx.glMapBufferRange		(GL_ARRAY_BUFFER, 0, 33, GL_MAP_READ_BIT);
	ctx.expectError				(GL_INVALID_VALUE);

	ctx.glMapBufferRange		(GL_ARRAY_BUFFER, 32, 1, GL_MAP_READ_BIT);
	ctx.expectError				(GL_INVALID_VALUE);

	ctx.glMapBufferRange		(GL_ARRAY_BUFFER, 16, 17, GL_MAP_READ_BIT);
	ctx.expectError				(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if access has any bits set other than those accepted.");
	ctx.glMapBufferRange		(GL_ARRAY_BUFFER, 0, 16, GL_MAP_READ_BIT | 0x1000);
	ctx.expectError				(GL_INVALID_VALUE);

	ctx.glMapBufferRange		(GL_ARRAY_BUFFER, 0, 16, GL_MAP_WRITE_BIT | 0x1000);
	ctx.expectError				(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if the buffer is already in a mapped state.");
	ctx.glMapBufferRange		(GL_ARRAY_BUFFER, 0, 16, GL_MAP_WRITE_BIT);
	ctx.expectError				(GL_NO_ERROR);
	ctx.glMapBufferRange		(GL_ARRAY_BUFFER, 16, 8, GL_MAP_READ_BIT);
	ctx.expectError				(GL_INVALID_OPERATION);
	ctx.glUnmapBuffer			(GL_ARRAY_BUFFER);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if neither GL_MAP_READ_BIT or GL_MAP_WRITE_BIT is set.");
	ctx.glMapBufferRange		(GL_ARRAY_BUFFER, 0, 16, GL_MAP_INVALIDATE_RANGE_BIT);
	ctx.expectError				(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if ");
	ctx.glMapBufferRange		(GL_ARRAY_BUFFER, 0, 16, GL_MAP_INVALIDATE_RANGE_BIT);
	ctx.expectError				(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if GL_MAP_READ_BIT is set and any of GL_MAP_INVALIDATE_RANGE_BIT, GL_MAP_INVALIDATE_BUFFER_BIT, or GL_MAP_UNSYNCHRONIZED_BIT is set.");
	ctx.glMapBufferRange		(GL_ARRAY_BUFFER, 0, 16, GL_MAP_READ_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
	ctx.expectError				(GL_INVALID_OPERATION);

	ctx.glMapBufferRange		(GL_ARRAY_BUFFER, 0, 16, GL_MAP_READ_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	ctx.expectError				(GL_INVALID_OPERATION);

	ctx.glMapBufferRange		(GL_ARRAY_BUFFER, 0, 16, GL_MAP_READ_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	ctx.expectError				(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if GL_MAP_FLUSH_EXPLICIT_BIT is set and GL_MAP_WRITE_BIT is not set.");
	ctx.glMapBufferRange		(GL_ARRAY_BUFFER, 0, 16, GL_MAP_READ_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);
	ctx.expectError				(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.glDeleteBuffers			(1, &buf);
}

void read_buffer (NegativeTestContext& ctx)
{
	deUint32				fbo = 0x1234;
	deUint32				texture = 0x1234;
	int						maxColorAttachments = 0x1234;

	ctx.glGetIntegerv			(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);
	ctx.glGenTextures			(1, &texture);
	ctx.glBindTexture			(GL_TEXTURE_2D, texture);
	ctx.glTexImage2D			(GL_TEXTURE_2D, 0, GL_RGBA8, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	ctx.glGenFramebuffers		(1, &fbo);
	ctx.glBindFramebuffer		(GL_FRAMEBUFFER, fbo);
	ctx.glFramebufferTexture2D	(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	ctx.glCheckFramebufferStatus(GL_FRAMEBUFFER);
	ctx.expectError				(GL_NO_ERROR);

	ctx.beginSection("GL_INVALID_ENUM is generated if mode is not GL_BACK, GL_NONE, or GL_COLOR_ATTACHMENTi, where i is less than GL_MAX_COLOR_ATTACHMENTS.");
	ctx.glReadBuffer			(-1);
	ctx.expectError				(GL_INVALID_ENUM);
	ctx.glReadBuffer			(GL_FRAMEBUFFER);
	ctx.expectError				(GL_INVALID_ENUM);
	ctx.glReadBuffer			(GL_COLOR_ATTACHMENT0 + maxColorAttachments);
	ctx.expectError				(GL_INVALID_ENUM);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if the current framebuffer is the default framebuffer and mode is not GL_NONE or GL_BACK.");
	ctx.glBindFramebuffer		(GL_FRAMEBUFFER, 0);
	ctx.glReadBuffer			(GL_COLOR_ATTACHMENT0);
	ctx.expectError				(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if the current framebuffer is a named framebuffer and mode is not GL_NONE or GL_COLOR_ATTACHMENTi.");
	ctx.glBindFramebuffer		(GL_FRAMEBUFFER, fbo);
	ctx.glReadBuffer			(GL_BACK);
	ctx.expectError				(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.glDeleteTextures(1, &texture);
	ctx.glDeleteFramebuffers(1, &fbo);
}

void unmap_buffer (NegativeTestContext& ctx)
{
	deUint32				buf = 0x1234;
	std::vector<GLfloat>	data(32);

	ctx.glGenBuffers			(1, &buf);
	ctx.glBindBuffer			(GL_ARRAY_BUFFER, buf);
	ctx.glBufferData			(GL_ARRAY_BUFFER, 32, &data[0], GL_DYNAMIC_COPY);
	ctx.expectError				(GL_NO_ERROR);

	ctx.beginSection("GL_INVALID_OPERATION is generated if the buffer data store is already in an unmapped state.");
	ctx.glUnmapBuffer			(GL_ARRAY_BUFFER);
	ctx.expectError				(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.glDeleteBuffers			(1, &buf);
}
// Framebuffer Objects

void bind_framebuffer (NegativeTestContext& ctx)
{
	ctx.beginSection("GL_INVALID_ENUM is generated if target is not GL_FRAMEBUFFER.");
	ctx.glBindFramebuffer(-1, 0);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.glBindFramebuffer(GL_RENDERBUFFER, 0);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.endSection();
}

void bind_renderbuffer (NegativeTestContext& ctx)
{
	ctx.beginSection("GL_INVALID_ENUM is generated if target is not GL_RENDERBUFFER.");
	ctx.glBindRenderbuffer(-1, 0);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.glBindRenderbuffer(GL_FRAMEBUFFER, 0);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.endSection();
}

void check_framebuffer_status (NegativeTestContext& ctx)
{
	ctx.beginSection("GL_INVALID_ENUM is generated if target is not GL_FRAMEBUFFER.");
	ctx.glCheckFramebufferStatus(-1);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.glCheckFramebufferStatus(GL_RENDERBUFFER);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.endSection();
}

void gen_framebuffers (NegativeTestContext& ctx)
{
	ctx.beginSection("GL_INVALID_VALUE is generated if n is negative.");
	ctx.glGenFramebuffers(-1, 0);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.endSection();
}

void gen_renderbuffers (NegativeTestContext& ctx)
{
	ctx.beginSection("GL_INVALID_VALUE is generated if n is negative.");
	ctx.glGenRenderbuffers(-1, 0);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.endSection();
}

void delete_framebuffers (NegativeTestContext& ctx)
{
	ctx.beginSection("GL_INVALID_VALUE is generated if n is negative.");
	ctx.glDeleteFramebuffers(-1, 0);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.endSection();
}

void delete_renderbuffers (NegativeTestContext& ctx)
{;
	ctx.beginSection("GL_INVALID_VALUE is generated if n is negative.");
	ctx.glDeleteRenderbuffers(-1, 0);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.endSection();
}

void framebuffer_renderbuffer (NegativeTestContext& ctx)
{
	GLuint fbo = 0x1234;
	GLuint rbo = 0x1234;
	ctx.glGenFramebuffers(1, &fbo);
	ctx.glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	ctx.glGenRenderbuffers(1, &rbo);

	ctx.beginSection("GL_INVALID_ENUM is generated if target is not one of the accepted tokens.");
	ctx.glFramebufferRenderbuffer(-1, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, 0);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_ENUM is generated if renderbuffertarget is not GL_RENDERBUFFER.");
	ctx.glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	ctx.glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, -1, rbo);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.glBindRenderbuffer(GL_RENDERBUFFER, 0);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if renderbuffer is neither 0 nor the name of an existing renderbuffer object.");
	ctx.glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, -1);
	ctx.expectError(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if zero is bound to target.");
	ctx.glBindFramebuffer(GL_FRAMEBUFFER, 0);
	ctx.glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, 0);
	ctx.expectError(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.glDeleteRenderbuffers(1, &rbo);
	ctx.glDeleteFramebuffers(1, &fbo);
}

void framebuffer_texture2d (NegativeTestContext& ctx)
{
	GLuint fbo = 0x1234;
	GLuint tex2D = 0x1234;
	GLuint texCube = 0x1234;
	GLint maxTexSize = 0x1234;
	GLint maxTexCubeSize = 0x1234;

	ctx.glGenFramebuffers(1, &fbo);
	ctx.glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	ctx.glGenTextures(1, &tex2D);
	ctx.glBindTexture(GL_TEXTURE_2D, tex2D);
	ctx.glGenTextures(1, &texCube);
	ctx.glBindTexture(GL_TEXTURE_CUBE_MAP, texCube);
	ctx.glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
	ctx.glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &maxTexCubeSize);
	ctx.expectError(GL_NO_ERROR);

	ctx.beginSection("GL_INVALID_ENUM is generated if target is not one of the accepted tokens.");
	ctx.glFramebufferTexture2D(-1, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_ENUM is generated if textarget is not an accepted texture target.");
	ctx.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, -1, tex2D, 0);
	ctx.expectError(GL_INVALID_ENUM);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if level is less than 0 or larger than log_2 of maximum texture size.");
	int maxSize = 0x1234;
	ctx.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex2D, -1);
	ctx.expectError(GL_INVALID_VALUE);
	maxSize = deLog2Floor32(maxTexSize) + 1;
	ctx.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex2D, maxSize);
	ctx.expectError(GL_INVALID_VALUE);
	maxSize = deLog2Floor32(maxTexSize) + 1;
	ctx.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, texCube, maxSize);
	ctx.expectError(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if texture is neither 0 nor the name of an existing texture object.");
	ctx.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, -1, 0);
	ctx.expectError(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if textarget and texture are not compatible.");
	ctx.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, tex2D, 0);
	ctx.expectError(GL_INVALID_OPERATION);
	ctx.glDeleteTextures(1, &tex2D);

	ctx.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texCube, 0);
	ctx.expectError(GL_INVALID_OPERATION);
	ctx.glDeleteTextures(1, &texCube);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if zero is bound to target.");
	ctx.glBindFramebuffer(GL_FRAMEBUFFER, 0);
	ctx.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
	ctx.expectError(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.glDeleteFramebuffers(1, &fbo);
}

void renderbuffer_storage (NegativeTestContext& ctx)
{
	deUint32					rbo = 0x1234;
	ctx.glGenRenderbuffers		(1, &rbo);
	ctx.glBindRenderbuffer		(GL_RENDERBUFFER, rbo);

	ctx.beginSection("GL_INVALID_ENUM is generated if target is not GL_RENDERBUFFER.");
	ctx.glRenderbufferStorage	(-1, GL_RGBA4, 1, 1);
	ctx.expectError				(GL_INVALID_ENUM);
	ctx.glRenderbufferStorage	(GL_FRAMEBUFFER, GL_RGBA4, 1, 1);
	ctx.expectError				(GL_INVALID_ENUM);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_ENUM is generated if internalformat is not a color-renderable, depth-renderable, or stencil-renderable format.");
	ctx.glRenderbufferStorage	(GL_RENDERBUFFER, -1, 1, 1);
	ctx.expectError				(GL_INVALID_ENUM);
	ctx.glRenderbufferStorage	(GL_RENDERBUFFER, GL_RGB16F, 1, 1);
	ctx.expectError				(GL_INVALID_ENUM);
	ctx.glRenderbufferStorage	(GL_RENDERBUFFER, GL_RGBA8_SNORM, 1, 1);
	ctx.expectError				(GL_INVALID_ENUM);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if width or height is less than zero.");
	ctx.glRenderbufferStorage	(GL_RENDERBUFFER, GL_RGBA4, -1, 1);
	ctx.expectError				(GL_INVALID_VALUE);
	ctx.glRenderbufferStorage	(GL_RENDERBUFFER, GL_RGBA4, 1, -1);
	ctx.expectError				(GL_INVALID_VALUE);
	ctx.glRenderbufferStorage	(GL_RENDERBUFFER, GL_RGBA4, -1, -1);
	ctx.expectError				(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if width or height is greater than GL_MAX_RENDERBUFFER_SIZE.");
	GLint maxSize = 0x1234;
	ctx.glGetIntegerv			(GL_MAX_RENDERBUFFER_SIZE, &maxSize);
	ctx.glRenderbufferStorage	(GL_RENDERBUFFER, GL_RGBA4, 1, maxSize+1);
	ctx.expectError				(GL_INVALID_VALUE);
	ctx.glRenderbufferStorage	(GL_RENDERBUFFER, GL_RGBA4, maxSize+1, 1);
	ctx.expectError				(GL_INVALID_VALUE);
	ctx.glRenderbufferStorage	(GL_RENDERBUFFER, GL_RGBA4, maxSize+1, maxSize+1);
	ctx.expectError				(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.glDeleteRenderbuffers(1, &rbo);
}

void blit_framebuffer (NegativeTestContext& ctx)
{
	deUint32					fbo[2];
	deUint32					rbo[2];
	deUint32					texture[2];

	ctx.glGenFramebuffers		(2, fbo);
	ctx.glGenTextures			(2, texture);
	ctx.glGenRenderbuffers		(2, rbo);

	ctx.glBindTexture			(GL_TEXTURE_2D, texture[0]);
	ctx.glBindRenderbuffer		(GL_RENDERBUFFER, rbo[0]);
	ctx.glBindFramebuffer		(GL_READ_FRAMEBUFFER, fbo[0]);

	ctx.glTexImage2D			(GL_TEXTURE_2D, 0, GL_RGBA8, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	ctx.glRenderbufferStorage	(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 32, 32);
	ctx.glFramebufferTexture2D	(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture[0], 0);
	ctx.glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo[0]);
	ctx.glCheckFramebufferStatus(GL_READ_FRAMEBUFFER);

	ctx.glBindTexture			(GL_TEXTURE_2D, texture[1]);
	ctx.glBindRenderbuffer		(GL_RENDERBUFFER, rbo[1]);
	ctx.glBindFramebuffer		(GL_DRAW_FRAMEBUFFER, fbo[1]);

	ctx.glTexImage2D			(GL_TEXTURE_2D, 0, GL_RGBA8, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	ctx.glRenderbufferStorage	(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 32, 32);
	ctx.glFramebufferTexture2D	(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture[1], 0);
	ctx.glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo[1]);
	ctx.glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	ctx.expectError				(GL_NO_ERROR);

	ctx.beginSection("GL_INVALID_OPERATION is generated if mask contains any of the GL_DEPTH_BUFFER_BIT or GL_STENCIL_BUFFER_BIT and filter is not GL_NEAREST.");
	ctx.glBlitFramebuffer		(0, 0, 16, 16, 0, 0, 16, 16, GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_LINEAR);
	ctx.expectError				(GL_INVALID_OPERATION);
	ctx.glBlitFramebuffer		(0, 0, 16, 16, 0, 0, 16, 16, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_LINEAR);
	ctx.expectError				(GL_INVALID_OPERATION);
	ctx.glBlitFramebuffer		(0, 0, 16, 16, 0, 0, 16, 16, GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_LINEAR);
	ctx.expectError				(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if mask contains GL_COLOR_BUFFER_BIT and read buffer format is incompatible with draw buffer format.");
	ctx.glBindTexture			(GL_TEXTURE_2D, texture[0]);

	ctx.glTexImage2D			(GL_TEXTURE_2D, 0, GL_RGBA32UI, 32, 32, 0, GL_RGBA_INTEGER, GL_UNSIGNED_INT, NULL);
	ctx.glFramebufferTexture2D	(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture[0], 0);
	ctx.getLog() << TestLog::Message << "// Read buffer: GL_RGBA32UI, draw buffer: GL_RGBA" << TestLog::EndMessage;
	ctx.glBlitFramebuffer		(0, 0, 16, 16, 0, 0, 16, 16, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	ctx.expectError				(GL_INVALID_OPERATION);

	ctx.glTexImage2D			(GL_TEXTURE_2D, 0, GL_RGBA32I, 32, 32, 0, GL_RGBA_INTEGER, GL_INT, NULL);
	ctx.glFramebufferTexture2D	(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture[0], 0);
	ctx.getLog() << TestLog::Message << "// Read buffer: GL_RGBA32I, draw buffer: GL_RGBA" << TestLog::EndMessage;
	ctx.glBlitFramebuffer		(0, 0, 16, 16, 0, 0, 16, 16, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	ctx.expectError				(GL_INVALID_OPERATION);

	ctx.glTexImage2D			(GL_TEXTURE_2D, 0, GL_RGBA8, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	ctx.glFramebufferTexture2D	(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture[0], 0);
	ctx.glBindTexture			(GL_TEXTURE_2D, texture[1]);
	ctx.glTexImage2D			(GL_TEXTURE_2D, 0, GL_RGBA32I, 32, 32, 0, GL_RGBA_INTEGER, GL_INT, NULL);
	ctx.glFramebufferTexture2D	(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture[1], 0);
	ctx.getLog() << TestLog::Message << "// Read buffer: GL_RGBA8, draw buffer: GL_RGBA32I" << TestLog::EndMessage;
	ctx.glBlitFramebuffer		(0, 0, 16, 16, 0, 0, 16, 16, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	ctx.expectError				(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if filter is GL_LINEAR and the read buffer contains integer data.");
	ctx.glBindTexture			(GL_TEXTURE_2D, texture[0]);
	ctx.glTexImage2D			(GL_TEXTURE_2D, 0, GL_RGBA32UI, 32, 32, 0, GL_RGBA_INTEGER, GL_UNSIGNED_INT, NULL);
	ctx.glFramebufferTexture2D	(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture[0], 0);
	ctx.glTexImage2D			(GL_TEXTURE_2D, 0, GL_RGBA8, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	ctx.glFramebufferTexture2D	(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture[1], 0);
	ctx.getLog() << TestLog::Message << "// Read buffer: GL_RGBA32I, draw buffer: GL_RGBA8" << TestLog::EndMessage;
	ctx.glBlitFramebuffer		(0, 0, 16, 16, 0, 0, 16, 16, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	ctx.expectError				(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if mask contains GL_DEPTH_BUFFER_BIT or GL_STENCIL_BUFFER_BIT and the source and destination depth and stencil formats do not match.");
	ctx.glBindRenderbuffer		(GL_RENDERBUFFER, rbo[0]);
	ctx.glRenderbufferStorage	(GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, 32, 32);
	ctx.glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo[0]);
	ctx.glBlitFramebuffer		(0, 0, 16, 16, 0, 0, 16, 16, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	ctx.expectError				(GL_INVALID_OPERATION);
	ctx.glBlitFramebuffer		(0, 0, 16, 16, 0, 0, 16, 16, GL_STENCIL_BUFFER_BIT, GL_NEAREST);
	ctx.expectError				(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.glBindFramebuffer		(GL_FRAMEBUFFER, 0);
	ctx.glDeleteFramebuffers	(2, fbo);
	ctx.glDeleteTextures		(2, texture);
	ctx.glDeleteRenderbuffers	(2, rbo);
}

void blit_framebuffer_multisample (NegativeTestContext& ctx)
{
	deUint32							fbo[2];
	deUint32							rbo[2];

	ctx.glGenFramebuffers				(2, fbo);
	ctx.glGenRenderbuffers				(2, rbo);

	ctx.glBindRenderbuffer				(GL_RENDERBUFFER, rbo[0]);
	ctx.glBindFramebuffer				(GL_READ_FRAMEBUFFER, fbo[0]);
	ctx.glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA8, 32, 32);
	ctx.glFramebufferRenderbuffer		(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo[0]);
	ctx.glCheckFramebufferStatus		(GL_READ_FRAMEBUFFER);

	ctx.glBindRenderbuffer				(GL_RENDERBUFFER, rbo[1]);
	ctx.glBindFramebuffer				(GL_DRAW_FRAMEBUFFER, fbo[1]);

	ctx.expectError						(GL_NO_ERROR);

	ctx.beginSection("GL_INVALID_OPERATION is generated if the value of GL_SAMPLE_BUFFERS for the draw buffer is greater than zero.");
	ctx.glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA8, 32, 32);
	ctx.glFramebufferRenderbuffer		(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo[1]);
	ctx.glBlitFramebuffer				(0, 0, 16, 16, 0, 0, 16, 16, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	ctx.expectError						(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if GL_SAMPLE_BUFFERS for the read buffer is greater than zero and the formats of draw and read buffers are not identical.");
	ctx.glRenderbufferStorage			(GL_RENDERBUFFER, GL_RGBA4, 32, 32);
	ctx.glFramebufferRenderbuffer		(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo[1]);
	ctx.glBlitFramebuffer				(0, 0, 16, 16, 0, 0, 16, 16, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	ctx.expectError						(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if GL_SAMPLE_BUFFERS for the read buffer is greater than zero and the source and destination rectangles are not defined with the same (X0, Y0) and (X1, Y1) bounds.");
	ctx.glRenderbufferStorage			(GL_RENDERBUFFER, GL_RGBA8, 32, 32);
	ctx.glFramebufferRenderbuffer		(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo[1]);
	ctx.glBlitFramebuffer				(0, 0, 16, 16, 2, 2, 18, 18, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	ctx.expectError						(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.glBindFramebuffer				(GL_FRAMEBUFFER, 0);
	ctx.glDeleteRenderbuffers			(2, rbo);
	ctx.glDeleteFramebuffers			(2, fbo);
}

void framebuffer_texture_layer (NegativeTestContext& ctx)
{
	deUint32						fbo = 0x1234;
	deUint32						tex3D;
	deUint32						tex2DArray;
	deUint32						tex2D;

	ctx.glGenFramebuffers			(1, &fbo);
	ctx.glGenTextures				(1, &tex3D);
	ctx.glGenTextures				(1, &tex2DArray);
	ctx.glGenTextures				(1, &tex2D);
	ctx.glBindFramebuffer			(GL_FRAMEBUFFER, fbo);

	ctx.glBindTexture				(GL_TEXTURE_3D, tex3D);
	ctx.glTexImage3D				(GL_TEXTURE_3D, 0, GL_RGBA, 4, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	ctx.glBindTexture				(GL_TEXTURE_2D_ARRAY, tex2DArray);
	ctx.glTexImage3D				(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, 4, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	ctx.glBindTexture				(GL_TEXTURE_2D, tex2D);
	ctx.glTexImage2D				(GL_TEXTURE_2D, 0, GL_RGBA, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	ctx.expectError					(GL_NO_ERROR);

	ctx.beginSection("GL_INVALID_ENUM is generated if target is not one of the accepted tokens.");
	ctx.glFramebufferTextureLayer	(-1, GL_COLOR_ATTACHMENT0, tex3D, 0, 1);
	ctx.expectError					(GL_INVALID_ENUM);
	ctx.glFramebufferTextureLayer	(GL_RENDERBUFFER, GL_COLOR_ATTACHMENT0, tex3D, 0, 1);
	ctx.expectError					(GL_INVALID_ENUM);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_ENUM is generated if attachment is not one of the accepted tokens.");
	ctx.glFramebufferTextureLayer	(GL_FRAMEBUFFER, -1, tex3D, 0, 1);
	ctx.expectError					(GL_INVALID_ENUM);
	ctx.glFramebufferTextureLayer	(GL_FRAMEBUFFER, GL_BACK, tex3D, 0, 1);
	ctx.expectError					(GL_INVALID_ENUM);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if texture is non-zero and not the name of a 3D texture or 2D array texture.");
	ctx.glFramebufferTextureLayer	(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, -1, 0, 0);
	ctx.expectError					(GL_INVALID_OPERATION);
	ctx.glFramebufferTextureLayer	(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex2D, 0, 0);
	ctx.expectError					(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if texture is not zero and layer is negative.");
	ctx.glFramebufferTextureLayer	(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex3D, 0, -1);
	ctx.expectError					(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if texture is not zero and layer is greater than GL_MAX_3D_TEXTURE_SIZE-1 for a 3D texture.");
	int							max3DTexSize = 0x1234;
	ctx.glGetIntegerv				(GL_MAX_3D_TEXTURE_SIZE, &max3DTexSize);
	ctx.glFramebufferTextureLayer	(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex3D, 0, max3DTexSize);
	ctx.expectError					(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if texture is not zero and layer is greater than GL_MAX_ARRAY_TEXTURE_LAYERS-1 for a 2D array texture.");
	int							maxArrayTexLayers = 0x1234;
	ctx.glGetIntegerv				(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxArrayTexLayers);
	ctx.glFramebufferTextureLayer	(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex2DArray, 0, maxArrayTexLayers);
	ctx.expectError					(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if zero is bound to target.");
	ctx.glBindFramebuffer			(GL_FRAMEBUFFER, 0);
	ctx.glFramebufferTextureLayer	(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex3D, 0, 1);
	ctx.expectError					(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.glDeleteTextures		(1, &tex3D);
	ctx.glDeleteTextures		(1, &tex2DArray);
	ctx.glDeleteTextures		(1, &tex2D);
	ctx.glDeleteFramebuffers	(1, &fbo);
}

void invalidate_framebuffer (NegativeTestContext& ctx)
{
	deUint32					fbo = 0x1234;
	deUint32					texture = 0x1234;
	deUint32					attachments[2];
	int							maxColorAttachments = 0x1234;
	ctx.glGetIntegerv				(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);
	attachments[0]				= GL_COLOR_ATTACHMENT0;
	attachments[1]				= GL_COLOR_ATTACHMENT0 + maxColorAttachments;

	ctx.glGenFramebuffers			(1, &fbo);
	ctx.glGenTextures				(1, &texture);
	ctx.glBindFramebuffer			(GL_FRAMEBUFFER, fbo);
	ctx.glBindTexture				(GL_TEXTURE_2D, texture);
	ctx.glTexImage2D				(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	ctx.glFramebufferTexture2D		(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	ctx.glCheckFramebufferStatus	(GL_FRAMEBUFFER);
	ctx.expectError					(GL_NO_ERROR);

	ctx.beginSection("GL_INVALID_ENUM is generated if target is not GL_FRAMEBUFFER, GL_READ_FRAMEBUFFER or GL_DRAW_FRAMEBUFFER.");
	ctx.glInvalidateFramebuffer		(-1, 1, &attachments[0]);
	ctx.expectError					(GL_INVALID_ENUM);
	ctx.glInvalidateFramebuffer		(GL_BACK, 1, &attachments[0]);
	ctx.expectError					(GL_INVALID_ENUM);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if attachments contains GL_COLOR_ATTACHMENTm and m is greater than or equal to the value of GL_MAX_COLOR_ATTACHMENTS.");
	ctx.glInvalidateFramebuffer		(GL_FRAMEBUFFER, 1, &attachments[1]);
	ctx.expectError					(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.glDeleteTextures		(1, &texture);
	ctx.glDeleteFramebuffers	(1, &fbo);
}

void invalidate_sub_framebuffer (NegativeTestContext& ctx)
{
	deUint32					fbo = 0x1234;
	deUint32					texture = 0x1234;
	deUint32					attachments[2];
	int							maxColorAttachments = 0x1234;
	ctx.glGetIntegerv				(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);
	attachments[0]				= GL_COLOR_ATTACHMENT0;
	attachments[1]				= GL_COLOR_ATTACHMENT0 + maxColorAttachments;

	ctx.glGenFramebuffers			(1, &fbo);
	ctx.glGenTextures				(1, &texture);
	ctx.glBindFramebuffer			(GL_FRAMEBUFFER, fbo);
	ctx.glBindTexture				(GL_TEXTURE_2D, texture);
	ctx.glTexImage2D				(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	ctx.glFramebufferTexture2D		(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	ctx.glCheckFramebufferStatus	(GL_FRAMEBUFFER);
	ctx.expectError					(GL_NO_ERROR);

	ctx.beginSection("GL_INVALID_ENUM is generated if target is not GL_FRAMEBUFFER, GL_READ_FRAMEBUFFER or GL_DRAW_FRAMEBUFFER.");
	ctx.glInvalidateSubFramebuffer	(-1, 1, &attachments[0], 0, 0, 16, 16);
	ctx.expectError					(GL_INVALID_ENUM);
	ctx.glInvalidateSubFramebuffer	(GL_BACK, 1, &attachments[0], 0, 0, 16, 16);
	ctx.expectError					(GL_INVALID_ENUM);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if attachments contains GL_COLOR_ATTACHMENTm and m is greater than or equal to the value of GL_MAX_COLOR_ATTACHMENTS.");
	ctx.glInvalidateSubFramebuffer	(GL_FRAMEBUFFER, 1, &attachments[1], 0, 0, 16, 16);
	ctx.expectError					(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.glDeleteTextures		(1, &texture);
	ctx.glDeleteFramebuffers	(1, &fbo);
}

void renderbuffer_storage_multisample (NegativeTestContext& ctx)
{
	deUint32							rbo = 0x1234;
	int									maxSamplesSupportedRGBA4 = -1;

	ctx.glGetInternalformativ				(GL_RENDERBUFFER, GL_RGBA4, GL_SAMPLES, 1, &maxSamplesSupportedRGBA4);
	ctx.glGenRenderbuffers					(1, &rbo);
	ctx.glBindRenderbuffer					(GL_RENDERBUFFER, rbo);

	ctx.beginSection("GL_INVALID_ENUM is generated if target is not GL_RENDERBUFFER.");
	ctx.glRenderbufferStorageMultisample	(-1, 2, GL_RGBA4, 1, 1);
	ctx.expectError							(GL_INVALID_ENUM);
	ctx.glRenderbufferStorageMultisample	(GL_FRAMEBUFFER, 2, GL_RGBA4, 1, 1);
	ctx.expectError							(GL_INVALID_ENUM);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if samples is greater than the maximum number of samples supported for internalformat.");
	ctx.glRenderbufferStorageMultisample	(GL_RENDERBUFFER, maxSamplesSupportedRGBA4+1, GL_RGBA4, 1, 1);
	ctx.expectError							(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_ENUM is generated if internalformat is not a color-renderable, depth-renderable, or stencil-renderable format.");
	ctx.glRenderbufferStorageMultisample	(GL_RENDERBUFFER, 2, -1, 1, 1);
	ctx.expectError							(GL_INVALID_ENUM);
	ctx.glRenderbufferStorageMultisample	(GL_RENDERBUFFER, 2, GL_RGB16F, 1, 1);
	ctx.expectError							(GL_INVALID_ENUM);
	ctx.glRenderbufferStorageMultisample	(GL_RENDERBUFFER, 2, GL_RGBA8_SNORM, 1, 1);
	ctx.expectError							(GL_INVALID_ENUM);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_OPERATION is generated if internalformat is a signed or unsigned integer format and samples is greater than 0.");
	ctx.glRenderbufferStorageMultisample	(GL_RENDERBUFFER, 1, GL_RGBA8UI, 1, 1);
	ctx.expectError							(GL_INVALID_OPERATION);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if width or height is less than zero.");
	ctx.glRenderbufferStorageMultisample	(GL_RENDERBUFFER, 2, GL_RGBA4, -1, 1);
	ctx.expectError							(GL_INVALID_VALUE);
	ctx.glRenderbufferStorageMultisample	(GL_RENDERBUFFER, 2, GL_RGBA4, 1, -1);
	ctx.expectError							(GL_INVALID_VALUE);
	ctx.glRenderbufferStorageMultisample	(GL_RENDERBUFFER, 2, GL_RGBA4, -1, -1);
	ctx.expectError							(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.beginSection("GL_INVALID_VALUE is generated if width or height is greater than GL_MAX_RENDERBUFFER_SIZE.");
	GLint maxSize = 0x1234;
	ctx.glGetIntegerv						(GL_MAX_RENDERBUFFER_SIZE, &maxSize);
	ctx.glRenderbufferStorageMultisample	(GL_RENDERBUFFER, 4, GL_RGBA4, 1, maxSize+1);
	ctx.expectError							(GL_INVALID_VALUE);
	ctx.glRenderbufferStorageMultisample	(GL_RENDERBUFFER, 4, GL_RGBA4, maxSize+1, 1);
	ctx.expectError							(GL_INVALID_VALUE);
	ctx.glRenderbufferStorageMultisample	(GL_RENDERBUFFER, 4, GL_RGBA4, maxSize+1, maxSize+1);
	ctx.expectError							(GL_INVALID_VALUE);
	ctx.endSection();

	ctx.glDeleteRenderbuffers(1, &rbo);
}

std::vector<FunctionContainer> getNegativeBufferApiTestFunctions ()
{
	FunctionContainer funcs[] =
	{
		{bind_buffer,						"bind_buffer",						"Invalid glBindBuffer() usage"					  },
		{delete_buffers,					"delete_buffers",					"Invalid glDeleteBuffers() usage"				  },
		{gen_buffers,						"gen_buffers",						"Invalid glGenBuffers() usage"					  },
		{buffer_data,						"buffer_data",						"Invalid glBufferData() usage"					  },
		{buffer_sub_data,					"buffer_sub_data",					"Invalid glBufferSubData() usage"				  },
		{buffer_sub_data_size_offset,		"buffer_sub_data_size_offset",		"Invalid glBufferSubData() usage"				  },
		{clear,								"clear",							"Invalid glClear() usage"						  },
		{read_pixels,						"read_pixels",						"Invalid glReadPixels() usage"					  },
		{read_pixels_format_mismatch,		"read_pixels_format_mismatch",		"Invalid glReadPixels() usage"					  },
		{read_pixels_fbo_format_mismatch,	"read_pixels_fbo_format_mismatch",	"Invalid glReadPixels() usage"					  },
		{bind_buffer_range,					"bind_buffer_range",				"Invalid glBindBufferRange() usage"				  },
		{bind_buffer_base,					"bind_buffer_base",					"Invalid glBindBufferBase() usage"				  },
		{clear_bufferiv,					"clear_bufferiv",					"Invalid glClearBufferiv() usage"				  },
		{clear_bufferuiv,					"clear_bufferuiv",					"Invalid glClearBufferuiv() usage"				  },
		{clear_bufferfv,					"clear_bufferfv",					"Invalid glClearBufferfv() usage"				  },
		{clear_bufferfi,					"clear_bufferfi",					"Invalid glClearBufferfi() usage"				  },
		{copy_buffer_sub_data,				"copy_buffer_sub_data",				"Invalid glCopyBufferSubData() usage"			  },
		{draw_buffers,						"draw_buffers",						"Invalid glDrawBuffers() usage"					  },
		{flush_mapped_buffer_range,			"flush_mapped_buffer_range",		"Invalid glFlushMappedBufferRange() usage"		  },
		{map_buffer_range,					"map_buffer_range",					"Invalid glMapBufferRange() usage"				  },
		{read_buffer,						"read_buffer",						"Invalid glReadBuffer() usage"					  },
		{unmap_buffer,						"unmap_buffer",						"Invalid glUnmapBuffer() usage"					  },
		{bind_framebuffer,					"bind_framebuffer",					"Invalid glBindFramebuffer() usage"				  },
		{bind_renderbuffer,					"bind_renderbuffer",				"Invalid glBindRenderbuffer() usage"			  },
		{check_framebuffer_status,			"check_framebuffer_status",			"Invalid glCheckFramebufferStatus() usage"		  },
		{gen_framebuffers,					"gen_framebuffers",					"Invalid glGenFramebuffers() usage"				  },
		{gen_renderbuffers,					"gen_renderbuffers",				"Invalid glGenRenderbuffers() usage"			  },
		{delete_framebuffers,				"delete_framebuffers",				"Invalid glDeleteFramebuffers() usage"			  },
		{delete_renderbuffers,				"delete_renderbuffers",				"Invalid glDeleteRenderbuffers() usage"			  },
		{framebuffer_renderbuffer,			"framebuffer_renderbuffer",			"Invalid glFramebufferRenderbuffer() usage"		  },
		{framebuffer_texture2d,				"framebuffer_texture2d",			"Invalid glFramebufferTexture2D() usage"		  },
		{renderbuffer_storage,				"renderbuffer_storage",				"Invalid glRenderbufferStorage() usage"			  },
		{blit_framebuffer,					"blit_framebuffer",					"Invalid glBlitFramebuffer() usage"				  },
		{blit_framebuffer_multisample,		"blit_framebuffer_multisample",		"Invalid glBlitFramebuffer() usage"				  },
		{framebuffer_texture_layer,			"framebuffer_texture_layer",		"Invalid glFramebufferTextureLayer() usage"		  },
		{invalidate_framebuffer,			"invalidate_framebuffer",			"Invalid glInvalidateFramebuffer() usage"		  },
		{invalidate_sub_framebuffer,		"invalidate_sub_framebuffer",		"Invalid glInvalidateSubFramebuffer() usage"	  },
		{renderbuffer_storage_multisample,	"renderbuffer_storage_multisample",	"Invalid glRenderbufferStorageMultisample() usage"},
	};

	return std::vector<FunctionContainer>(DE_ARRAY_BEGIN(funcs), DE_ARRAY_END(funcs));
}

} // NegativeTestShared
} // Functional
} // gles31
} // deqp
