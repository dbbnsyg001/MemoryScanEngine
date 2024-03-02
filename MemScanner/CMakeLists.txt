#----------Base info----------
include(Function.cmake)
cmake_minimum_required(VERSION 3.9)
project(MemScanner)
#----------Visual studio special----------
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

#----------x64 or x86----------
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    SET(PLATFORM_CPU "x64")
else()
    SET(PLATFORM_CPU "x86")
endif()
MESSAGE(STATUS "Build type: ${PLATFORM_CPU}")

#----------redirection output---------
set(OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/output/${PLATFORM_CPU})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})

#---------Visual studio settings---------
set(CMAKE_CXX_STANDARD    11) 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP") 
set(CMAKE_CXX_STANDARD_LIBRARIES "%(AdditionalDependencies)")
#release to generate debugging information
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi /Od")
#set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
#set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
#set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")

#---------add build directory and file---------
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/DataDefine)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Scan)

file(GLOB TARGET_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.*)
file(GLOB TARGET_SRC ${TARGET_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/DataDefine/*.*)
file(GLOB TARGET_SRC ${TARGET_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/Common/*.*)
file(GLOB TARGET_SRC ${TARGET_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/Scan/*.*)
#----------Visual studio special filter----------
source_group_by_dir(TARGET_SRC)

#----------build result----------
add_executable(${PROJECT_NAME} ${TARGET_SRC})