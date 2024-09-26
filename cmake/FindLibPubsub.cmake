# - Try to find LibPubsub headers and libraries.
#
# Usage of this module as follows:
#
#     find_package(LibPubsub)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
#  LibPubsub_ROOT_DIR  Set this variable to the root installation of
#                    LibPubsub if the module has problems finding
#                    the proper installation path.
#
# Variables defined by this module:
#
#  LibPubsub_FOUND              System has LibPubsub libs/headers
#  LibPubsub_LIBRARIES          The LibPubsub libraries
#  LibPubsub_INCLUDE_DIRS        The location of LibPubsub headers

find_path(LibPubsub_ROOT_DIR
    NAMES include/tcp_pubsub/tcp_pubsub_version.h
    PATHS ${CNSTREAM_ROOT_PATH}/3rdparty/tcp_pubsub
)

find_path(LibPubsub_INCLUDE_DIRS
    NAMES tcp_pubsub/tcp_pubsub_version.h
    PATHS ${LibPubsub_ROOT_DIR}/include
)

find_library(LibPubsub_LIBRARIES
	NAMES tcp_pubsub
	PATHS ${LibPubsub_ROOT_DIR}/lib/linux_lib
)

if (LibPubsub_LIBRARIES)
    set(LibPubsub_FOUND TRUE)
endif ()
