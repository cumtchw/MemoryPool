# - Try to find LibRDKafka headers and libraries.
#
# Usage of this module as follows:
#
#     find_package(LibRDKafka)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
#  LibRDKafka_ROOT_DIR  Set this variable to the root installation of
#                    LibRDKafka if the module has problems finding
#                    the proper installation path.
#
# Variables defined by this module:
#
#  LibRDKAFKA_FOUND              System has LibRDKafka libs/headers
#  LibRDKafka_LIBRARIES          The LibRDKafka libraries
#  LibRDKafka_INCLUDE_DIRS        The location of LibRDKafka headers

find_path(LibRDKafka_ROOT_DIR
    NAMES include/librdkafka/rdkafka.h
    PATHS ${CNSTREAM_ROOT_PATH}/3rdparty/librdkafka
)

find_path(LibRDKafka_INCLUDE_DIRS
    NAMES librdkafka/rdkafka.h
    PATHS ${LibRDKafka_ROOT_DIR}/include
)

find_library(LibRDKafka_C_LIBRARIES
	NAMES rdkafka
	PATHS ${LibRDKafka_ROOT_DIR}/lib/linux_lib
)

if (LibRDKafka_C_LIBRARIES)
    set(LibRDKAFKA_FOUND TRUE)
endif ()
