cmake_minimum_required(VERSION 3.0)
project(TARGET_POSITION)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE "Release")

IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
    MESSAGE(STATUS "current platform: Linux ")
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "current platform: Windows")
    add_definitions("-DWINDOWS")
    option(CMAKE_USE_WIN32_THREADS_INIT "using WIN32 threads" ON)
ELSE()
    MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
ENDIF(CMAKE_SYSTEM_NAME MATCHES "Linux")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV)

# 规定.h头文件路径
include_directories(${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/common
    ${OpenCV_INCLUDE_DIRS})


add_library(target_position SHARED 
        sdk_interface/target_position_sdk.cpp
        src/target_position_template.cpp)
target_link_libraries(target_position ${OpenCV_LIBS})

# 生成可执行文件

include_directories(${PROJECT_SOURCE_DIR}/sdk_interface)

add_executable(demo demo.cpp )
target_link_libraries(demo target_position  ${OpenCV_LIBS})

add_executable(train train.cpp )
target_link_libraries(train target_position  ${OpenCV_LIBS})