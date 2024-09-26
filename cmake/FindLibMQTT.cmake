# - Try to find MQTT headers and libraries.
#
# Usage of this module as follows:
#
#     find_package(LibMQTT)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
#  LibMQTT_ROOT_DIR  Set this variable to the root installation of
#                    LibMQTT if the module has problems finding
#                    the proper installation path.
#
# Variables defined by this module:
#
#  LibMQTT_FOUND              System has LibMQTT libs/headers
#  LibMQTT_LIBRARIES          The LibMQTT libraries
#  LibMQTT_INCLUDE_DIRS        The location of LibMQTT headers

find_path(LibMQTT_ROOT_DIR
    NAMES include/mqtt/MQTTClient.h
    PATHS ${CNSTREAM_ROOT_PATH}/3rdparty/mqtt
)

find_path(LibMQTT_INCLUDE_DIRS
    NAMES mqtt/MQTTClient.h
    PATHS ${LibMQTT_ROOT_DIR}/include
)

find_library(LibMQTT_LIBRARIES
	NAMES paho-mqttpp3
	PATHS ${LibMQTT_ROOT_DIR}/lib/linux_lib
)

find_library(LibMQTT3A_LIBRARIES
	NAMES paho-mqtt3a
	PATHS ${LibMQTT_ROOT_DIR}/lib/linux_lib
)

find_library(LibMQTT3AS_LIBRARIES
	NAMES paho-mqtt3as
	PATHS ${LibMQTT_ROOT_DIR}/lib/linux_lib
)

if (LibMQTT_LIBRARIES)
	list(APPEND LibMQTT_LIBRARIES ${LibMQTT3A_LIBRARIES} ${LibMQTT3AS_LIBRARIES})
    set(LibMQTT_FOUND TRUE)
endif ()