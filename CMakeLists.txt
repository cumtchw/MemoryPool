# ----------------------------------------------------------------------------
#  Root CMake file for nvis
# ----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.5)

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "Please create a separate directory for build files.")
endif()

# ---[ nvis project
project(nvis C CXX)
set(CMAKE_VERBOSE_MAKEFILE OFF)

#Determin target architecture
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/TargetArchitecture.cmake)
target_architecture(TARGET_ARCH)
message("####### Target Architecture is ${TARGET_ARCH}")

#configure lib for different architecture
execute_process(COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/tools/config_lib.sh ${TARGET_ARCH})

# ---[ Options
option(build_infer_server "build infer server" ON)
option(build_app "build app programs" ON)

# To use sanitizers, the version of GCC is required to be no less than 4.9
option(SANITIZE_MEMORY "Enable MemorySanitizer for sanitized targets." OFF)
option(SANITIZE_ADDRESS "Enable AddressSanitizer for sanitized targets." OFF)
option(SANITIZE_THREAD "Enable ThreadSanitizer for sanitized targets." OFF)
option(SANITIZE_UNDEFINED "Enable UndefinedBehaviorSanitizer for sanitized targets." OFF)

# ---[ Build Flags
if(USE_libstdcpp)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
  message("-- Warning: forcing libstdc++ (controlled by USE_libstdcpp option in cmake)")
endif()

# ---[ install
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install)
message("####### CMAKE_INSTALL_PREFIX is ${CMAKE_INSTALL_PREFIX}")

# ---[ build

if(build_infer_server)
  add_subdirectory(infer_server)
endif()

if(build_app)
  add_subdirectory(app)
endif()

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/ DESTINATION lib)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/gflags/lib/linux_lib/     DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glog/lib/linux_lib/       DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/opencv/lib/linux_lib/     DESTINATION lib)