cmake_minimum_required(VERSION 3.24)

project(Base)

set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
set(CMAKE_OSX_SYSROOT "iphoneos")
set(CMAKE_CXX_STANDARD 17)

set(BASE_NAME_LOWER_CASE "base")
set(BASE_NAME_UPPER_CASE "Base")
set(BASE_BUNDLE_ID_PREFIX "cn.gz.ch")

set(BASE_BUNDLE_ID "")
string(CONCAT BASE_BUNDLE_ID ${BASE_BUNDLE_ID_PREFIX} ".base")

set(BASE_STRING_LIB "")
string(CONCAT BASE_STRING_LIB ${BASE_NAME_LOWER_CASE} "_string")

set(BASE_STORAGE_MEMORY_LIB "")
string(CONCAT BASE_STORAGE_MEMORY_LIB ${BASE_NAME_LOWER_CASE} "_storage_memory")

set(BASE_STORAGE_DATABASE_LIB "")
string(CONCAT BASE_STORAGE_DATABASE_LIB ${BASE_NAME_LOWER_CASE} "_storage_database")

set(BASE_JSON "")
string(CONCAT BASE_JSON ${BASE_NAME_LOWER_CASE} "_json")

set(BASE_THREAD "")
string(CONCAT BASE_THREAD ${BASE_NAME_LOWER_CASE} "_thread")

set(BASE_MMAP "")
string(CONCAT BASE_MMAP ${BASE_NAME_LOWER_CASE} "_mmap")

set(BASE_LOG "")
string(CONCAT BASE_LOG ${BASE_NAME_LOWER_CASE} "_log")

set(BASE_DEVICE "")
string(CONCAT BASE_DEVICE ${BASE_NAME_LOWER_CASE} "_device")

set(BASE_PLATFORM "")
string(CONCAT BASE_PLATFORM ${BASE_NAME_LOWER_CASE} "_platform")

file(GLOB
     base_source_cc
     *.cc)

file(GLOB
     base_source_h
     *.h)

if(OSX OR IOS)
    file(GLOB
         base_source_mm
         *.mm
         *m)
    list(APPEND base_source_cc ${base_source_mm})
 endif()

if(IOS)
    set(PLATFORM iOS)
    add_definitions(-D OS_IOS)
elseif(OSX)
    set(PLATFORM Mac)
    add_definitions(-D OS_MAX)
elseif(ANDROID)
    set(PLATFORM Android)
    add_definitions(-D OS_ANDROID)
endif()

include_directories(./)
include_directories(../thirdparty)

add_subdirectory(storage/memory)
add_subdirectory(storage/database)
add_subdirectory(string)
add_subdirectory(json)
add_subdirectory(thread)
add_subdirectory(mmap)
add_subdirectory(log)
add_subdirectory(device)
add_subdirectory(platform)
#thirdparty
add_subdirectory(./../thirdparty/jansson jansson)

find_library(UIKIT UIKit)
find_library(FOUNDATION Foundation)
find_library(MOBILECORESERVICES MobileCoreServices)
find_library(CFNETWORK CFNetwork)
find_library(SYSTEMCONFIGURATION SystemConfiguration)

add_library(${PROJECT_NAME} SHARED ${base_source_h} ${base_source_cc})
add_dependencies(${PROJECT_NAME} 
                    ${BASE_STRING_LIB} 
                    ${BASE_STORAGE_MEMORY_LIB}
                    ${BASE_JSON}
                    ${BASE_THREAD}
                    ${BASE_MMAP}
                    ${BASE_LOG}
                    ${BASE_DEVICE}
                    ${BASE_PLATFORM}
                    jansson)
                    

target_link_libraries(${PROJECT_NAME} 
                         ${BASE_STRING_LIB} 
                         ${BASE_STORAGE_MEMORY_LIB}
                         ${BASE_JSON}
                         ${BASE_THREAD}
                         ${BASE_MMAP}
                         ${BASE_LOG}
                         ${BASE_DEVICE}
                         ${BASE_PLATFORM}
                         ${UIKIT} 
                         ${FOUNDATION}
                         jansson)

apple_framework_generate(${PROJECT_NAME} ${BASE_BUNDLE_ID} ${PROJECT_NAME} "")
set_xcode_property(${PROJECT_NAME} PUBLIC_HEADER "${base_source_h}")

set_target_folder_if_exist(${BASE_STRING_LIB} ${PROJECT_NAME})
set_target_folder_if_exist(${BASE_STORAGE_MEMORY_LIB} ${PROJECT_NAME})
set_target_folder_if_exist(${BASE_JSON} ${PROJECT_NAME})
set_target_folder_if_exist(${BASE_THREAD} ${PROJECT_NAME})
set_target_folder_if_exist(${BASE_MMAP} ${PROJECT_NAME})
set_target_folder_if_exist(${BASE_LOG} ${PROJECT_NAME})
set_target_folder_if_exist(${BASE_DEVICE} ${PROJECT_NAME})
set_target_folder_if_exist(${BASE_PLATFORM} ${PROJECT_NAME})

if(${CMAKE_VERSION} VERSION_GREATER "3.16.0")
    target_precompile_headers(${PROJECT_NAME} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${BASE_ROOT}/pch.h>")
endif()
