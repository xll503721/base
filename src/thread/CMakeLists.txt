project(${BASE_THREAD})

file(GLOB
     base_thread_source
     *.cc
     *.h)

if(OSX OR IOS)
    file(GLOB
         base_thread_source_mm
         *.mm)
    list(APPEND base_thread_source ${base_thread_source_mm})
 endif()

add_library(${PROJECT_NAME} STATIC ${base_thread_source})

if(${CMAKE_VERSION} VERSION_GREATER "3.16.0")
    target_precompile_headers(${PROJECT_NAME} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${BASE_ROOT}/pch.h>")
endif()