# - Try to find Compute headers and libraries.
#
# Usage of this module as follows:
#
#     find_package(LibCompute)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
#  LibCompute_ROOT_DIR  Set this variable to the root installation of
#                    LibCompute if the module has problems finding
#                    the proper installation path.
#
# Variables defined by this module:
#
#  LibCompute_FOUND              System has LibCompute libs/headers
#  LibCompute_LIBRARIES          The LibCompute libraries
#  LibCompute_INCLUDE_DIRS        The location of LibCompute headers

find_path(LibCompute_ROOT_DIR
    NAMES include/export_funtions.h
    PATHS ${CNSTREAM_ROOT_PATH}/3rdparty/jetson/compute
)

find_path(LibCompute_INCLUDE_DIRS
    NAMES export_funtions.h
    PATHS ${LibCompute_ROOT_DIR}/include
)

find_path(LibCompute_CUDA_INCLUDE_DIRS
    NAMES cuda_runtime.h
    PATHS ${LibCompute_ROOT_DIR}/include/cuda
)

set(LibCompute_LIBRARIES "")
set(COMPUTE_LIBS cudart trteng_exp cuda_utils cfldwp2)
foreach(lib ${COMPUTE_LIBS})
  find_library(LibCompute_LIBRARIES_${lib}
    NAMES ${lib}
    PATHS ${LibCompute_ROOT_DIR}/lib/linux_lib
    NO_DEFAULT_PATH
  )
  
  if(LibCompute_LIBRARIES_${lib})
    list(APPEND LibCompute_LIBRARIES ${LibCompute_LIBRARIES_${lib}})
  endif()
endforeach()

if (LibCompute_INCLUDE_DIRS)
	list(APPEND LibCompute_INCLUDE_DIRS ${LibCompute_CUDA_INCLUDE_DIRS})
endif ()

if (LibCompute_LIBRARIES)
    set(LibCompute_FOUND TRUE)
endif ()