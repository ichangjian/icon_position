cmake_minimum_required(VERSION 3.0)
project(ICON_POSITION)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
# 如果想要生成的可执行文件拥有符号表，可以gdb调试，就直接加上这句
# add_definitions("-Wall -g")
add_definitions(-DWINDOWS)
find_package(OpenCV)

# 规定.h头文件路径
include_directories(${PROJECT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS})


add_library(icon_position SHARED icon_position_sdk.cpp icon_position_template.cpp)
target_link_libraries(icon_position ${OpenCV_LIBS})

# 生成可执行文件
add_executable(demo demo.cpp )
target_link_libraries(demo icon_position  ${OpenCV_LIBS})