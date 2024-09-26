cmake_minimum_required(VERSION 3.5)

if(POLICY CMP0046)
  cmake_policy(SET CMP0046 NEW)
endif()
if(POLICY CMP0054)
  cmake_policy(SET CMP0054 NEW)
endif()

set(PROJECT_NAME app)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
if(NOT COMPILER_SUPPORTS_CXX17)
  message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++17 support. Please use a different C++ compiler.")
endif()

set(3RDPARTY_LIBS "")

if(USE_libstdcpp)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
  message("-- Warning: forcing libstdc++ (controlled by USE_libstdcpp option in cmake)")
endif()

set(CNSTREAM_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(APP_ROOT_PATH ${CNSTREAM_ROOT_PATH}/app)

# compile flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -O2")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG -rdynamic -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -D_REENTRANT -fPIC -Wall")

set(CMAKE_MODULE_PATH ${CNSTREAM_ROOT_PATH}/cmake/sanitizers ${CNSTREAM_ROOT_PATH}/cmake)
find_package(Sanitizers)

# ---[ Compute
find_package(LibCompute)
if(NOT LibCompute_FOUND)
   message(FATAL_ERROR "LibCompute not found!")
endif()

# ---[ opencv
set(OpenCV_FIND_QUIETLY true)
find_package(OpenCV REQUIRED core imgproc imgcodecs highgui video)
if(OpenCV_FOUND)
  include_directories(${OpenCV_INCLUDE_DIRS})
  list(APPEND 3RDPARTY_LIBS ${OpenCV_LIBS})
  message(STATUS "OpenCV_INCLUDE_DIRS include: ${OpenCV_INCLUDE_DIRS}")
  message(STATUS "OpenCV_INCLUDE_DIRS libraries: ${OpenCV_LIBS}")
else()
  message(FATAL_ERROR "OpenCV_INCLUDE_DIRS not found!")
endif()

# ---[ gflags
find_package(GFlags)
if(GFlags_FOUND)
  include_directories(${GFLAGS_INCLUDE_DIRS})
  list(APPEND 3RDPARTY_LIBS ${GFLAGS_LIBRARIES})
  message(STATUS "GFlags include: ${GFLAGS_INCLUDE_DIRS}")
  message(STATUS "GFlags libraries: ${GFLAGS_LIBRARIES}")
else() 
  message(FATAL_ERROR "GFlags not found!")
endif()

# ---[ glog
find_package(Glog)
if(Glog_FOUND)
  include_directories(${GLOG_INCLUDE_DIRS})
  list(APPEND 3RDPARTY_LIBS ${GLOG_LIBRARIES})
  message(STATUS "Glog include: ${GLOG_INCLUDE_DIRS}")
  message(STATUS "Glog libraries: ${GLOG_LIBRARIES}")
else() 
  message(FATAL_ERROR "Glog not found!")
endif()


# ---[ modules include dirs
include_directories(${LibCompute_INCLUDE_DIRS})

# ---[ sources
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} srcs)

# ---[ dependencies
set(APP_DEPENDENCIES "")
include(${CNSTREAM_ROOT_PATH}/cmake/have_cnstream_target.cmake)
have_infer_server_target(${CNSTREAM_ROOT_PATH})


if(HAVE_INFER_SERVER_TARGET)
  list(APPEND APP_DEPENDENCIES nvis)
endif()

# ---[ add targets
set(EXECUTABLE_OUTPUT_PATH ${APP_ROOT_PATH}/bin)
add_executable(${PROJECT_NAME} ${srcs})
add_sanitizers(${PROJECT_NAME})
if(NOT "${APP_DEPENDENCIES} " STREQUAL " ")
  add_dependencies(${PROJECT_NAME} ${APP_DEPENDENCIES})
endif()
target_link_libraries(${PROJECT_NAME}  nvis ${3RDPARTY_LIBS} pthread dl)

if(HAVE_MODULES_CONTRIB)
  set(CMAKE_EXE_LINKER_FLAGS "-Wl,--no-as-needed")
  target_link_libraries(${PROJECT_NAME} cnstream_contrib)
endif()

set_target_properties(${PROJECT_NAME}  PROPERTIES LINK_FLAGS "-Wl,--disable-new-dtags,-rpath,../lib:./lib")

# ---[ install
message("####### CMAKE_INSTALL_PREFIX is ${CMAKE_INSTALL_PREFIX}")
