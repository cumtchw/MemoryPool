# - Try to find Eigen headers and libraries.
#
# Usage of this module as follows:
#
#     find_package(Eigen)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
#  Eigen_ROOT_DIR  Set this variable to the root installation of
#                    Eigen if the module has problems finding
#                    the proper installation path.
#                    the proper installation path.
#
# Variables defined by this module:
#
#  Eigen_FOUND              System has Eigen libs/headers
#  Eigen_INCLUDE_DIRS        The location of Eigen headers

find_path(Eigen_ROOT_DIR
    NAMES include/Eigen/Eigen
    PATHS ${CNSTREAM_ROOT_PATH}/3rdparty/eigen
	NO_DEFAULT_PATH
)

find_path(Eigen_INCLUDE_DIRS
    NAMES Eigen/Eigen
    PATHS ${Eigen_ROOT_DIR}/include
	NO_DEFAULT_PATH
)

if (Eigen_INCLUDE_DIRS)
    set(Eigen_FOUND TRUE)
endif ()
