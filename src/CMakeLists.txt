cmake_minimum_required(VERSION 3.24)

project(Base)


set(BASE_NAME_LOWER_CASE "base")
set(BASE_NAME_UPPER_CASE "Base")
set(BASE_BUNDLE_ID_PREFIX "cn.gz.ch")

set(BASE_STRING_LIB "")
string(CONCAT BASE_STRING_LIB ${BASE_NAME_LOWER_CASE} "_string")

set(BASE_BUNDLE_ID "")
string(CONCAT BASE_BUNDLE_ID ${BASE_BUNDLE_ID_PREFIX} ".base")

file(GLOB
     base_source_cc
     *.cc)

file(GLOB
     base_source_h
     *.h)

add_subdirectory(string)

add_library(${PROJECT_NAME} SHARED ${base_source_h} ${base_source_cc})
add_dependencies(${PROJECT_NAME} ${BASE_STRING_LIB})

target_link_libraries(${PROJECT_NAME} ${BASE_STRING_LIB})

apple_framework_generate(${PROJECT_NAME} ${BASE_BUNDLE_ID} ${PROJECT_NAME} "")
set_xcode_property(${PROJECT_NAME} PUBLIC_HEADER "${base_source_h}")

set_target_folder_if_exist(${BASE_STRING_LIB} ${PROJECT_NAME})