cmake_minimum_required(VERSION 3.0)
project(TARGET_POSITION)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
# 如果想要生成的可执行文件拥有符号表，可以gdb调试，就直接加上这句
# add_definitions("-Wall -g")
add_definitions(-DWINDOWS)
find_package(OpenCV)

# 规定.h头文件路径
include_directories(${PROJECT_SOURCE_DIR}/src
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