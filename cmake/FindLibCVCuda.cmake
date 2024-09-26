# - Try to find CVCuda headers and libraries.
#
# Usage of this module as follows:
#
#     find_package(LibCVCuda)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
#  LibCVCuda_ROOT_DIR  Set this variable to the root installation of
#                    LibCVCuda if the module has problems finding
#                    the proper installation path.
#
# Variables defined by this module:
#
#  LibCVCuda_FOUND              System has LibCVCuda libs/headers
#  LibCVCuda_LIBRARIES          The LibCVCuda libraries
#  LibCVCuda_INCLUDE_DIRS        The location of LibCVCuda headers

find_path(LibCVCuda_ROOT_DIR
    NAMES include/cvcuda/Version.h
    PATHS ${CNSTREAM_ROOT_PATH}/3rdparty/jetson/cvcuda
    NO_DEFAULT_PATH
)

find_path(LibCVCuda_INCLUDE_DIRS
    NAMES cvcuda/Version.h
    PATHS ${LibCVCuda_ROOT_DIR}/include
    NO_DEFAULT_PATH
)

find_library(LibCVCuda_LIBRARIES
	NAMES cvcuda
	PATHS ${LibCVCuda_ROOT_DIR}/lib/linux_lib
    NO_DEFAULT_PATH
)

find_library(LibNVCV_Types_LIBRARIES
	NAMES nvcv_types
	PATHS ${LibCVCuda_ROOT_DIR}/lib/linux_lib
    NO_DEFAULT_PATH
)

if (LibCVCuda_LIBRARIES)
	list(APPEND LibCVCuda_LIBRARIES ${LibNVCV_Types_LIBRARIES})
    set(LibCVCuda_FOUND TRUE)
endif ()