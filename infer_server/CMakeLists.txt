cmake_minimum_required(VERSION 3.5)

# ---[ nvis project
project(nvis C CXX)

#######################################################################
# -- Build Flags

option(NVIS_RECORD_PERF "Enable record performance" OFF)

#######################################################################
set(NVIS_VERSION_MAJOR 1)
set(NVIS_VERSION_MINOR 0)
set(NVIS_VERSION_PATCH 0)
set(NVIS_VERSION "${NVIS_VERSION_MAJOR}.${NVIS_VERSION_MINOR}.${NVIS_VERSION_PATCH}")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG -rdynamic -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")

# set runtime path
set(CNSTREAM_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(CMAKE_MODULE_PATH "${CNSTREAM_ROOT_PATH}/cmake")

# ---[ Compute
find_package(LibCompute)
if(NOT LibCompute_FOUND)
   message(FATAL_ERROR "LibCompute not found!")
endif()


set(3RDPARTY_LIBS "")





# ---[ Google-glog
find_package(Glog)
if(NOT Glog_FOUND)
   message(FATAL_ERROR "Glog not found!")
endif()

# ---[ Google-gflags
find_package(GFlags)
if(NOT GFlags_FOUND)
   message(FATAL_ERROR "GFlags not found!")
endif()

file(GLOB_RECURSE contrib_srcs ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE infer_server_srcs ${CMAKE_CURRENT_SOURCE_DIR}/src/core/*.cpp)

set(LIBRARY_OUTPUT_PATH ${CNSTREAM_ROOT_PATH}/lib)
message(STATUS "@@@@@@@@@@@ Target : nvis")
add_library(nvis SHARED ${infer_server_srcs} ${contrib_srcs})

if (NVIS_RECORD_PERF)
  target_compile_definitions(D PRIVATE NVIS_RECORD_PERF)
endif()

target_include_directories(nvis PRIVATE
                           ${LibCompute_INCLUDE_DIRS}
                           ${CMAKE_CURRENT_SOURCE_DIR}/include
                           ${CMAKE_CURRENT_SOURCE_DIR}/src
                           ${GLOG_INCLUDE_DIRS}
                           ${GFLAGS_INCLUDE_DIRS}
                           )
       
target_link_libraries(nvis PRIVATE ${3RDPARTY_LIBS} ${LibCompute_LIBRARIES} ${GLOG_LIBRARIES} )
message(STATUS "FFmpeg libraries: ${FFMPEG_LIBRARIES}")

set_target_properties(nvis PROPERTIES VERSION ${NVIS_VERSION})
set_target_properties(nvis PROPERTIES SOVERSION ${NVIS_VERSION_MAJOR})
