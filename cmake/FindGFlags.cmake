# - Try to find GFlags headers and libraries.
#
# Usage of this module as follows:
#
#     find_package(LibGflags)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
#  LibGFlags_ROOT_DIR  Set this variable to the root installation of
#                    LibGflags if the module has problems finding
#                    the proper installation path.
#
# Variables defined by this module:
#
#  LibGflags_FOUND              System has LibGflags libs/headers
#  GFLAGS_LIBRARIES          The LibGFlags libraries
#  GFLAGS_INCLUDE_DIRS        The location of LibGflags headers

find_path(LibGflags_ROOT_DIR
    NAMES include/gflags/gflags.h
    PATHS ${CNSTREAM_ROOT_PATH}/3rdparty/gflags
)

find_path(GFLAGS_INCLUDE_DIRS
    NAMES gflags/gflags.h
    PATHS ${LibGflags_ROOT_DIR}/include
)

find_library(GFLAGS_LIBRARIES
	NAMES gflags
	PATHS ${LibGflags_ROOT_DIR}/lib/linux_lib
)

if (GFLAGS_LIBRARIES)
    set(GFlags_FOUND TRUE)
endif ()