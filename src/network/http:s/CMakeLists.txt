project(${BASE_NETWROK_HTTPS})

file(GLOB
     base_network_http_s_source
     *.cc
     *.h)

if(OSX OR IOS)
    file(GLOB
          base_network_http_s_source_mm
         *.mm
         *.m)
    list(APPEND base_network_http_s_source ${base_network_http_s_source_mm})
 endif()

add_library(${PROJECT_NAME} STATIC ${base_network_http_s_source})

if(${CMAKE_VERSION} VERSION_GREATER "3.16.0")
    target_precompile_headers(${PROJECT_NAME} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${BASE_ROOT}/pch.h>")
endif()