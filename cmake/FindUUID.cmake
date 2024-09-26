# - Try to find UUID headers and libraries.
#
# Usage of this module as follows:
#
#     find_package(UUID)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
#  UUID_ROOT_DIR  Set this variable to the root installation of
#                    UUID if the module has problems finding
#                    the proper installation path.
#
# Variables defined by this module:
#
#  UUID_FOUND              System has UUID libs/headers
#  UUID_LIBRARIES          The LUID libraries
#  UUID_INCLUDE_DIRS        The location of UUID headers

find_path(UUID_ROOT_DIR
    NAMES include/uuid/uuid.h
    PATHS ${CNSTREAM_ROOT_PATH}/3rdparty/uuid
	NO_DEFAULT_PATH
)

find_path(UUID_INCLUDE_DIRS
    NAMES uuid/uuid.h
    PATHS ${UUID_ROOT_DIR}/include
	NO_DEFAULT_PATH
)

find_library(UUID_LIBRARIES
	NAMES uuid
	PATHS ${UUID_ROOT_DIR}/lib/linux_lib
	NO_DEFAULT_PATH
)

if (UUID_LIBRARIES)
    set(UUID_FOUND TRUE)
endif ()
