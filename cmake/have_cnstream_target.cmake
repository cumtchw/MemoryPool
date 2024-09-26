
macro(have_infer_server_target CNSTREAM_ROOT_PATH)
  if(TARGET nvis)
    set(HAVE_INFER_SERVER_TARGET TRUE)
  else()
    if(NOT EXISTS ${CNSTREAM_ROOT_PATH}/lib/libnvis.so)
      message(FATAL_ERROR "Build infer server first")
    endif()
    link_directories(${CNSTREAM_ROOT_PATH}/lib)
  endif()
  include_directories(${CNSTREAM_ROOT_PATH}/infer_server/include)
  include_directories(${CNSTREAM_ROOT_PATH}/infer_server/include/common)
endmacro()


