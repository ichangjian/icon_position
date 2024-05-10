
find_package(OpenCV)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
    ${PROJECT_SOURCE_DIR}/common
    ${OpenCV_INCLUDE_DIRS})


add_library(template_opencv SHARED target_position_template.cpp)
target_link_libraries(template_opencv ${OpenCV_LIBS})