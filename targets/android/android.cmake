# Android native ES2
message("*** Using Android ES2")
set(DEQP_TARGET_NAME	"Android")
set(DEQP_SUPPORT_GLES1	ON)
set(DEQP_SUPPORT_GLES2	ON)

if (DE_ANDROID_API GREATER 8)
	set(DEQP_SUPPORT_EGL	ON)
	set(DEQP_SUPPORT_GLES3	ON) # \note GLES3 support requires eglGetProcAddress()
else ()
	set(DEQP_SUPPORT_EGL	OFF)
endif ()

set(DEQP_SUPPORT_OPENCL	OFF)

# GLESv2 lib
find_library(GLES2_LIBRARY GLESv2 PATHS /usr/lib)
set(DEQP_GLES2_LIBRARIES ${GLES2_LIBRARY})

# GLESv1 lib
find_library(GLES1_LIBRARY GLESv1_CM PATHS /usr/lib)
set(DEQP_GLES1_LIBRARIES ${GLES1_LIBRARY})

# EGL lib
if (DEQP_SUPPORT_EGL)
	find_library(EGL_LIBRARY EGL PATHS /usr/lib)
	set(DEQP_EGL_LIBRARIES ${EGL_LIBRARY})
endif ()

# OpenCL - dynamic loading
if (DEQP_SUPPORT_OPENCL)
	include_directories(framework/opencl/inc)

	add_subdirectory(wrappers/opencl_dynamic)
	set(DEQP_OPENCL_LIBRARIES OpenCL)
endif ()

# Platform libs
find_library(LOG_LIBRARY NAMES log PATHS /usr/lib)
set(DEQP_PLATFORM_LIBRARIES ${DEQP_PLATFORM_LIBRARIES} ${LOG_LIBRARY})

if (DE_ANDROID_API GREATER 8)
	# libandroid for NativeActivity APIs
	find_library(ANDROID_LIBRARY NAMES android PATHS /usr/lib)
	set(DEQP_PLATFORM_LIBRARIES ${DEQP_PLATFORM_LIBRARIES} ${ANDROID_LIBRARY})
endif ()

# Android uses customized execserver
include_directories(execserver)
set(DEQP_PLATFORM_LIBRARIES xscore ${DEQP_PLATFORM_LIBRARIES})
