# ==============================================
# Try to find Opencv libraries:
# - opencv_core
# - opencv_highgui
# - opencv_imgproc
# - opencv_imgcodecs
# - opencv_video
# - opencv_videoio
# - opencv_features2d
# - opencv_flann
# - opencv_calib3d
#
#  LibOPENCV_ROOT_DIR  Set this variable to the root installation of
#                    LibActiveMQ if the module has problems finding
#                    the proper installation path.
#
# OpenCV_FOUND - system has Opencv
# OpenCV_INCLUDE_DIRS - the Opencv inc directory
# OpenCV_LIBS - Link these to use Opencv
# ==============================================
# Notice: this original script is from internet.

if (OpenCV_LIBS AND OpenCV_INCLUDE_DIRS)
    # in cache already
    set(OpenCV_FOUND TRUE)
else (OpenCV_LIBS AND OpenCV_INCLUDE_DIRS)

	find_path(LibOPENCV_ROOT_DIR
		NAMES include/opencv2/opencv.hpp
		PATHS ${CNSTREAM_ROOT_PATH}/3rdparty/opencv
		NO_DEFAULT_PATH
	)	
		
	find_path(LibOPENCV_INCLUDE_DIR
		NAMES opencv2/opencv.hpp
		PATHS ${LibOPENCV_ROOT_DIR}/include
		NO_DEFAULT_PATH
	)

	find_library(OPENCV_LIBCORE
		NAMES opencv_core
		PATHS ${LibOPENCV_ROOT_DIR}/lib/linux_lib
		NO_DEFAULT_PATH
	)

	find_library(OPENCV_LIBHIGHGUI
		NAMES opencv_highgui
		PATHS ${LibOPENCV_ROOT_DIR}/lib/linux_lib
		NO_DEFAULT_PATH
	)
   
   	find_library(OPENCV_LIBIMGPROC
		NAMES opencv_imgproc
		PATHS ${LibOPENCV_ROOT_DIR}/lib/linux_lib
		NO_DEFAULT_PATH
	)
	
	find_library(OPENCV_LIBIMGCODECS
		NAMES opencv_imgcodecs
		PATHS ${LibOPENCV_ROOT_DIR}/lib/linux_lib
		NO_DEFAULT_PATH
	)
	
	find_library(OPENCV_LIBVIDEO
		NAMES opencv_video
		PATHS ${LibOPENCV_ROOT_DIR}/lib/linux_lib
		NO_DEFAULT_PATH
	)
	
	find_library(OPENCV_LIBVIDEOIO
		NAMES opencv_videoio
		PATHS ${LibOPENCV_ROOT_DIR}/lib/linux_lib
		NO_DEFAULT_PATH
	)	
	
	find_library(OPENCV_LIBFEATURES2D
		NAMES opencv_features2d
		PATHS ${LibOPENCV_ROOT_DIR}/lib/linux_lib
		NO_DEFAULT_PATH
	)
	
	find_library(OPENCV_LIBFLANN
		NAMES opencv_flann
		PATHS ${LibOPENCV_ROOT_DIR}/lib/linux_lib
		NO_DEFAULT_PATH
	)
	
	find_library(OPENCV_LIBCALIB3D
		NAMES opencv_calib3d
		PATHS ${LibOPENCV_ROOT_DIR}/lib/linux_lib
		NO_DEFAULT_PATH
	)
	
	find_library(OPENCV_LIBDNN
		NAMES opencv_dnn
		PATHS ${LibOPENCV_ROOT_DIR}/lib/linux_lib
		NO_DEFAULT_PATH
	)

	if (OPENCV_LIBCORE AND OPENCV_LIBHIGHGUI AND OPENCV_LIBIMGPROC AND OPENCV_LIBIMGCODECS AND OPENCV_LIBVIDEO AND OPENCV_LIBVIDEOIO AND OPENCV_LIBFEATURES2D AND OPENCV_LIBFLANN AND OPENCV_LIBCALIB3D AND OPENCV_LIBDNN)
        set(OpenCV_FOUND TRUE)
	endif ()
	
    if (OpenCV_FOUND)
        set(OpenCV_INCLUDE_DIRS ${LibOPENCV_INCLUDE_DIR})
        set(OpenCV_LIBS
                ${OPENCV_LIBCORE}
                ${OPENCV_LIBHIGHGUI}
                ${OPENCV_LIBIMGPROC}
                ${OPENCV_LIBIMGCODECS}
                ${OPENCV_LIBVIDEO}
				${OPENCV_LIBVIDEOIO}
				${OPENCV_LIBFEATURES2D}
				${OPENCV_LIBFLANN}
				${OPENCV_LIBCALIB3D}
				${OPENCV_LIBDNN})
    else (OpenCV_FOUND)
        message(FATAL_ERROR "Could not find Opencv libraries!")
    endif (OpenCV_FOUND)

endif (OpenCV_LIBS AND OpenCV_INCLUDE_DIRS)
