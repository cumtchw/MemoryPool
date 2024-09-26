# - Try to find Glog headers and libraries.
#
# Usage of this module as follows:
#
#     find_package(LibGlog)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
#  LibGlog_ROOT_DIR  Set this variable to the root installation of
#                    LibGlog if the module has problems finding
#                    the proper installation path.
#
# Variables defined by this module:
#
#  LibGlog_FOUND              System has LibGlog libs/headers
#  GLOG_LIBRARIES          The LibGlog libraries
#  GLOG_INCLUDE_DIRS        The location of LibGlog headers

find_path(LibGlog_ROOT_DIR
    NAMES include/glog/logging.h
    PATHS ${CNSTREAM_ROOT_PATH}/3rdparty/glog
	NO_DEFAULT_PATH
)

find_path(GLOG_INCLUDE_DIRS
    NAMES glog/logging.h
    PATHS ${LibGlog_ROOT_DIR}/include
	NO_DEFAULT_PATH
)

find_library(GLOG_LIBRARIES
	NAMES glog
	PATHS ${LibGlog_ROOT_DIR}/lib/linux_lib
	NO_DEFAULT_PATH
)

if (GLOG_LIBRARIES)
    set(Glog_FOUND TRUE)
endif ()