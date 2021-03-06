# dEQP-EGL

set(DEQP_EGL_SRCS
	teglApiCase.cpp
	teglApiCase.hpp
	teglChooseConfigReference.cpp
	teglChooseConfigReference.hpp
	teglChooseConfigTests.cpp
	teglChooseConfigTests.hpp
	teglQueryConfigTests.cpp
	teglQueryConfigTests.hpp
	teglColorClearCase.cpp
	teglColorClearCase.hpp
	teglColorClearTests.cpp
	teglColorClearTests.hpp
	teglConfigList.cpp
	teglConfigList.hpp
	teglCreateContextTests.cpp
	teglCreateContextTests.hpp
	teglQueryContextTests.cpp
	teglQueryContextTests.hpp
	teglCreateSurfaceTests.cpp
	teglCreateSurfaceTests.hpp
	teglQuerySurfaceTests.cpp
	teglQuerySurfaceTests.hpp
	teglGetProcAddressTests.cpp
	teglGetProcAddressTests.hpp
	teglGLES1RenderUtil.cpp
	teglGLES1RenderUtil.hpp
	teglGLES2RenderUtil.cpp
	teglGLES2RenderUtil.hpp
	teglImageTests.cpp
	teglImageTests.hpp
	teglInfoTests.cpp
	teglInfoTests.hpp
	teglNegativeApiTests.cpp
	teglNegativeApiTests.hpp
	teglRenderCase.cpp
	teglRenderCase.hpp
	teglRenderTests.cpp
	teglRenderTests.hpp
	teglSimpleConfigCase.cpp
	teglSimpleConfigCase.hpp
	teglTestCase.cpp
	teglTestCase.hpp
	teglTestPackage.cpp
	teglTestPackage.hpp
	teglVGRenderUtil.cpp
	teglVGRenderUtil.hpp
	teglImageFormatTests.hpp
	teglImageFormatTests.cpp
	teglGLES2SharingTests.hpp
	teglGLES2SharingTests.cpp
	teglGLES2SharingThreadedTests.hpp
	teglGLES2SharingThreadedTests.cpp
	teglSyncTests.hpp
	teglSyncTests.cpp
	teglMultiThreadTests.hpp
	teglMultiThreadTests.cpp
	teglMemoryStressTests.hpp
	teglMemoryStressTests.cpp
	teglMakeCurrentPerfTests.hpp
	teglMakeCurrentPerfTests.cpp
	teglGLES2SharedRenderingPerfTests.hpp
	teglGLES2SharedRenderingPerfTests.cpp
	teglPreservingSwapTests.hpp
	teglPreservingSwapTests.cpp
	teglClientExtensionTests.hpp
	teglClientExtensionTests.cpp
	teglCreateContextExtTests.hpp
	teglCreateContextExtTests.cpp
	teglSurfacelessContextTests.hpp
	teglSurfacelessContextTests.cpp
	teglSwapBuffersTests.hpp
	teglSwapBuffersTests.cpp
	teglNativeColorMappingTests.hpp
	teglNativeColorMappingTests.cpp
	teglNativeCoordMappingTests.hpp
	teglNativeCoordMappingTests.cpp
	teglResizeTests.hpp
	teglResizeTests.cpp
	)

set(DEQP_EGL_LIBS
	tcutil
	eglutil
	referencerenderer
	${DEQP_EGL_LIBRARIES}
	)


if (DEQP_SUPPORT_GLES2)
	set(DEQP_EGL_LIBS ${DEQP_EGL_LIBS} glutil glutil-sglr ${DEQP_GLES2_LIBRARIES})
endif ()

if (DEQP_SUPPORT_GLES1)
	set(DEQP_EGL_LIBS ${DEQP_EGL_LIBS} ${DEQP_GLES1_LIBRARIES})
endif ()

if (DEQP_SUPPORT_VG)
	set(DEQP_EGL_LIBS ${DEQP_EGL_LIBS} ${DEQP_VG_LIBRARIES})
endif ()

add_deqp_module(deqp-egl "${DEQP_EGL_SRCS}" "${DEQP_EGL_LIBS}" teglTestPackageEntry.cpp)
