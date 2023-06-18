project(${BASE_ANALYTICS})

file(GLOB
     base_analytics_source
     *.cc
     *.h)

add_library(${PROJECT_NAME} STATIC ${base_analytics_source})

if(${CMAKE_VERSION} VERSION_GREATER "3.16.0")
    target_precompile_headers(${PROJECT_NAME} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${BASE_ROOT}/pch.h>")
endif()