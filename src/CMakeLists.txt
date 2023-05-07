cmake_minimum_required(VERSION 3.24)

project(Base)

set(BASE_NAME_LOWER_CASE "base")
set(BASE_NAME_UPPER_CASE "Base")
set(BASE_BUNDLE_ID_PREFIX "cn.gz.ch")

set(BASE_STRING_LIB "")
string(CONCAT BASE_STRING_LIB ${BASE_NAME_LOWER_CASE} "_string")

set(BASE_STORAGE_MEMORY_LIB "")
string(CONCAT BASE_STORAGE_MEMORY_LIB ${BASE_NAME_LOWER_CASE} "_storage_memory")

set(BASE_STORAGE_DATABASE_LIB "")
string(CONCAT BASE_STORAGE_DATABASE_LIB ${BASE_NAME_LOWER_CASE} "_storage_database")

set(BASE_BUNDLE_ID "")
string(CONCAT BASE_BUNDLE_ID ${BASE_BUNDLE_ID_PREFIX} ".base")

file(GLOB
     base_source_cc
     *.cc)

file(GLOB
     base_source_h
     *.h)

include_directories(./)
include_directories(../thirdparty)

add_subdirectory(storage/memory)
add_subdirectory(string)
add_subdirectory(./../thirdparty/jansson jansson)

add_library(${PROJECT_NAME} SHARED ${base_source_h} ${base_source_cc})
add_dependencies(${PROJECT_NAME} 
                    ${BASE_STRING_LIB} 
                    ${BASE_STORAGE_MEMORY_LIB})

target_link_libraries(${PROJECT_NAME} 
                         ${BASE_STRING_LIB} 
                         ${BASE_STORAGE_MEMORY_LIB}
                         jansson)

apple_framework_generate(${PROJECT_NAME} ${BASE_BUNDLE_ID} ${PROJECT_NAME} "")
set_xcode_property(${PROJECT_NAME} PUBLIC_HEADER "${base_source_h}")

set_target_folder_if_exist(${BASE_STRING_LIB} ${PROJECT_NAME})
set_target_folder_if_exist(${BASE_STORAGE_MEMORY_LIB} ${PROJECT_NAME})

if(${CMAKE_VERSION} VERSION_GREATER "3.16.0")
    target_precompile_headers(${PROJECT_NAME} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${BASE_ROOT}/pch.h>")
endif()
