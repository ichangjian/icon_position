
find_package(OpenCV)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/template_opencv
    ${PROJECT_SOURCE_DIR}/common
    ${OpenCV_INCLUDE_DIRS})


add_library(target_position SHARED target_position_sdk.cpp)
target_link_libraries(target_position ${OpenCV_LIBS} template_opencv)
