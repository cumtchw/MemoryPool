# - Try to find LibOpenssl headers and libraries.
#
# Usage of this module as follows:
#
#     find_package(LibOpenssl)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
# Variables defined by this module:
#
#  LibOpenssl_FOUND              System has LibOpenssl libs/headers
#  LibOpenssl_INCLUDE_DIRS        The location of LibOpenssl headers
#  LibOpenssl_LIBRARIES          The LibOpenssl libraries

find_path(LibOpenssl_INCLUDE_DIRS
    NAMES openssl/opensslv.h
    PATHS ${CNSTREAM_ROOT_PATH}/3rdparty/openssl/include
	NO_DEFAULT_PATH
)


find_library(LibOpenssl_LIBRARIES
	NAMES ssl
	PATHS ${CNSTREAM_ROOT_PATH}/3rdparty/openssl/lib/linux_lib
	NO_DEFAULT_PATH
)

find_library(LibCrypto_LIBRARIES
	NAMES crypto
	PATHS ${CNSTREAM_ROOT_PATH}/3rdparty/openssl/lib/linux_lib
	NO_DEFAULT_PATH
)

if (LibOpenssl_LIBRARIES)
	list(APPEND LibOpenssl_LIBRARIES ${LibCrypto_LIBRARIES})
    set(LibOpenssl_FOUND TRUE)
endif ()
