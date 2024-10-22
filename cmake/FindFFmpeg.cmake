# ==============================================
# Try to find FFmpeg libraries:
# - avcodec
# - avformat
# - avdevice
# - avutil
# - swscale
# - avfilter
#
# FFMPEG_FOUND - system has FFmpeg
# FFMPEG_INCLUDE_DIRS - the FFmpeg inc directory
# FFMPEG_LIBRARIES - Link these to use FFmpeg
# ==============================================
# Notice: this original script is from internet.

if (FFMPEG_LIBRARIES AND FFMPEG_INCLUDE_DIRS)
    # in cache already
    set(FFMPEG_FOUND TRUE)
else (FFMPEG_LIBRARIES AND FFMPEG_INCLUDE_DIRS)

   	find_path(LibFFMPEG_ROOT_DIR
		NAMES include/libavcodec/avcodec.h
		PATHS ${CNSTREAM_ROOT_PATH}/3rdparty/ffmpeg
		NO_DEFAULT_PATH
	)	
   
    find_path(LibFFMEPG_INCLUDE_DIR
        NAMES libavcodec/avcodec.h
        PATHS ${LibFFMPEG_ROOT_DIR}/include
        NO_DEFAULT_PATH
    )

    find_library(FFMPEG_LIBAVCODEC
            NAMES avcodec
            PATHS ${CNSTREAM_ROOT_PATH}/3rdparty/ffmpeg/lib/linux_lib
            NO_DEFAULT_PATH
    )
  
    find_library(
            FFMPEG_LIBAVFORMAT
            NAMES avformat
            PATHS ${CNSTREAM_ROOT_PATH}/3rdparty/ffmpeg/lib/linux_lib
            NO_DEFAULT_PATH
    )
  
    find_library(
            FFMPEG_LIBSWRESAMPLE
            NAMES swresample
            PATHS ${CNSTREAM_ROOT_PATH}/3rdparty/ffmpeg/lib/linux_lib
            NO_DEFAULT_PATH
    )
 
    find_library(
            FFMPEG_LIBAVUTIL
            NAMES avutil
            PATHS ${CNSTREAM_ROOT_PATH}/3rdparty/ffmpeg/lib/linux_lib
            NO_DEFAULT_PATH
    )

    find_library(
            FFMPEG_LIBSWSCALE
            NAMES swscale
            PATHS ${CNSTREAM_ROOT_PATH}/3rdparty/ffmpeg/lib/linux_lib
            NO_DEFAULT_PATH
    )

    find_library(
            FFMPEG_LIBAVFILTER
            NAMES avfilter
            PATHS ${CNSTREAM_ROOT_PATH}/3rdparty/ffmpeg/lib/linux_lib
            NO_DEFAULT_PATH
    )
    find_library(
            FFMPEG_LIBAVDEVICE
            NAMES avdevice
            PATHS ${CNSTREAM_ROOT_PATH}/3rdparty/ffmpeg/lib/linux_lib
            NO_DEFAULT_PATH
    )
  if (WITH_FFMPEG_AVDEVICE AND NOT FFMPEG_LIBAVDEVICE)
    message(FATAL_ERROR "Not find FFmpeg LIBAVDEVICE ")
  endif ()
  if (FFMPEG_LIBAVCODEC AND FFMPEG_LIBAVFORMAT AND FFMPEG_LIBAVUTIL AND FFMPEG_LIBSWSCALE AND FFMPEG_LIBSWRESAMPLE AND (FFMPEG_LIBAVDEVICE OR NOT WITH_FFMPEG_AVDEVICE))
        set(FFMPEG_FOUND TRUE)
  endif ()

    if (FFMPEG_FOUND)
        set(FFMPEG_INCLUDE_DIRS ${LibFFMEPG_INCLUDE_DIR})
        set(FFMPEG_LIBRARIES
                ${FFMPEG_LIBAVCODEC}
                ${FFMPEG_LIBAVFORMAT}
                ${FFMPEG_LIBAVUTIL}
                ${FFMPEG_LIBSWSCALE}
                ${FFMPEG_LIBSWRESAMPLE})
        if (WITH_FFMPEG_AVDEVICE)
          list(APPEND FFMPEG_LIBRARIES ${FFMPEG_LIBAVDEVICE})
        endif (WITH_FFMPEG_AVDEVICE)
    else (FFMPEG_FOUND)
        message(FATAL_ERROR "Could not find FFmpeg libraries!")
    endif (FFMPEG_FOUND)

endif (FFMPEG_LIBRARIES AND FFMPEG_INCLUDE_DIRS)
