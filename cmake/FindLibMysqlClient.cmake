# - Try to find LibMysqlClient headers and libraries.
#
# Usage of this module as follows:
#
#     find_package(LibMysqlClient)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
#  LibMysqlClient_ROOT_DIR  Set this variable to the root installation of
#                           LibMysqlClient if the module has problems finding
#                           the proper installation path.
#
# Variables defined by this module:
#
#  LibMysqlClient_FOUND              System has UUID libs/headers
#  LibMysqlClient_LIBRARIES          The LUID libraries
#  LibMysqlClient_INCLUDE_DIR        The location of UUID headers

find_path(LibMysqlClient_ROOT_DIR
    NAMES include/mysql/mysql.h
    PATHS ${CNSTREAM_ROOT_PATH}/3rdparty/mysqlclient
	NO_DEFAULT_PATH
)

find_path(LibMysqlClient_INCLUDE_DIR
		NAMES mysql/mysql.h
		PATHS ${LibMysqlClient_ROOT_DIR}/include
		NO_DEFAULT_PATH
		)

find_library(LibMysqlClient_LIBRARIES
		NAMES mysqlclient
		PATHS ${LibMysqlClient_ROOT_DIR}/lib/linux_lib
		NO_DEFAULT_PATH
		)


if (LibMysqlClient_LIBRARIES)
    set(LibMysqlClient_FOUND TRUE)
endif ()
