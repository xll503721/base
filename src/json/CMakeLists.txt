project(${BASE_STORAGE_JSON})

file(GLOB
     base_json_source
     *.cc
     *.h)

include_directories(../../thirdparty/jansson/src)

add_library(${PROJECT_NAME} STATIC ${base_json_source})

if(${CMAKE_VERSION} VERSION_GREATER "3.16.0")
    target_precompile_headers(${PROJECT_NAME} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${BASE_ROOT}/pch.h>")
endif()