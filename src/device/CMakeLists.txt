project(${BASE_DEVICE})

file(GLOB
     base_device_source
     *.cc
     *.h)

if(OSX OR IOS)
    file(GLOB
         base_device_source_mm
         *.mm
         *.m)
    list(APPEND base_device_source ${base_device_source_mm})
 endif()

add_library(${PROJECT_NAME} STATIC ${base_device_source})

if(${CMAKE_VERSION} VERSION_GREATER "3.16.0")
    target_precompile_headers(${PROJECT_NAME} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${BASE_ROOT}/pch.h>")
endif()