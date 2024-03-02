#----------Base info----------
include(Function.cmake)
cmake_minimum_required(VERSION 3.9)
project(ARM64Scanner)
#----------Visual studio special----------
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
#----------x64 or x86----------
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    SET(PLATFORM_CPU "x64")
else()
    SET(PLATFORM_CPU "x86")
endif()
MESSAGE(STATUS "Build type: ${PLATFORM_CPU}")

# Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#---------add build directory and file---------
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/DataDefine)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/UIModule)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/UIModule/BaseUI)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Resources)
#
file(GLOB TARGET_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.*)
file(GLOB TARGET_SRC ${TARGET_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/DataDefine/*.*)
file(GLOB TARGET_SRC ${TARGET_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/UIModule/*.*)
file(GLOB TARGET_SRC ${TARGET_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/UIModule/BaseUI/*.*)
file(GLOB TARGET_SRC ${TARGET_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/Resources/*.*)

#----------redirection output---------
set(OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/output/${PLATFORM_CPU})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})

#---------Visual studio settings---------
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP") 
#release to generate debugging information
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi /Od")
#set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
#set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
#set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")

#----------Exclud qt moc/qrc/ui_xxx.cpp----------
foreach(file ${SRC_FILE})
    list(APPEND TARGET_SRC ${file})
endforeach(file ${SRC_FILE})
list(FILTER TARGET_SRC EXCLUDE REGEX "moc_|qrc_|ui_")

source_group_by_dir(TARGET_SRC)

#----------Set qt module----------
set(QT_MODULES Core Gui Widgets Xml)
find_package(Qt5 COMPONENTS ${QT_MODULES} REQUIRED)

#----------build result----------
add_executable(${PROJECT_NAME} ${TARGET_SRC})

#----------Link qt module----------
foreach(MODULE ${QT_MODULES})
    target_link_libraries(${PROJECT_NAME} Qt5::${MODULE})
endforeach(MODULE ${QT_MODULES})


