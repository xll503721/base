project(${BASE_STORAGE_DATABASE_LIB})

file(GLOB
     base_storage_database_source
     *.cc
     *.h)

add_library(${PROJECT_NAME} STATIC ${base_storage_database_source})

if(${CMAKE_VERSION} VERSION_GREATER "3.16.0")
    target_precompile_headers(${PROJECT_NAME} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${BASE_ROOT}/pch.h>")
endif()