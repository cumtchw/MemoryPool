# - Try to find Freetype headers and libraries.
#
# Usage of this module as follows:
#
#     find_package(LibFreetype)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
#  LibFreetype_ROOT_DIR  Set this variable to the root installation of
#                    LibFreetype if the module has problems finding
#                    the proper installation path.
#
# Variables defined by this module:
#
#  LibFreetype_FOUND              System has LibFreetype libs/headers
#  FREETYPE_LIBRARIES          The LibFreetype libraries
#  FREETYPE_INCLUDE_DIRS        The location of LibFreetype headers

find_path(LibFreetype_ROOT_DIR
    NAMES include/freetype2/ft2build.h
    PATHS ${CNSTREAM_ROOT_PATH}/3rdparty/freetype
	NO_DEFAULT_PATH
)

find_path(FREETYPE_INCLUDE_DIRS
    NAMES ft2build.h
    PATHS ${LibFreetype_ROOT_DIR}/include/freetype2
	NO_DEFAULT_PATH
)

find_library(FREETYPE_LIBRARIES
	NAMES freetype
	PATHS ${LibFreetype_ROOT_DIR}/lib/linux_lib
	NO_DEFAULT_PATH
)

if (FREETYPE_LIBRARIES)
    set(FREETYPE_FOUND TRUE)
endif ()