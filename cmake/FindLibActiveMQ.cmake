# - Try to find ActiveMQ headers and libraries.
#
# Usage of this module as follows:
#
#     find_package(LibActiveMQ)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
#  LibActiveMQ_ROOT_DIR  Set this variable to the root installation of
#                    LibActiveMQ if the module has problems finding
#                    the proper installation path.
#
# Variables defined by this module:
#
#  LibActiveMQ_FOUND              System has LibActiveMQ libs/headers
#  LibActiveMQ_LIBRARIES          The LibActiveMQ libraries
#  LibActiveMQ_INCLUDE_DIRS        The location of LibActiveMQ headers

find_path(LibActiveMQ_ROOT_DIR
    NAMES include/activemq/library/ActiveMQCPP.h
    PATHS ${CNSTREAM_ROOT_PATH}/3rdparty/activemq
)

find_path(LibActiveMQ_INCLUDE_DIRS
    NAMES activemq/library/ActiveMQCPP.h
    PATHS ${LibActiveMQ_ROOT_DIR}/include
)

find_path(LibAPR_INCLUDE_DIRS
    NAMES apr.h
    PATHS ${LibActiveMQ_ROOT_DIR}/include/apr-1
)

# ---[ activemq-cpp
find_library(LibActiveMQ_CPP_LIBRARIES
    NAMES activemq-cpp
	PATHS ${LibActiveMQ_ROOT_DIR}/lib/linux_lib
)

# ---[ apr-1
find_library(LibApr_C_LIBRARIES
    NAMES apr-1
	PATHS ${LibActiveMQ_ROOT_DIR}/lib/linux_lib
	NO_DEFAULT_PATH 
)

if (LibActiveMQ_CPP_LIBRARIES AND LibApr_C_LIBRARIES)
	list(APPEND LibActiveMQ_INCLUDE_DIR ${LibAPR_INCLUDE_DIR})
    set(LibActiveMQ_FOUND TRUE)
endif ()