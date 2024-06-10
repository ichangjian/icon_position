# cmake_minimum_required(VERSION 3.10)
# project(TARGET_POSITION)

# # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
message("===============${CMAKE_EXPORT_COMPILE_COMMANDS}")



SET(PADDLE_LIB "" CACHE PATH "Location of libraries")

if(NOT DEFINED PADDLE_LIB)
  message(FATAL_ERROR "please set PADDLE_LIB with -DPADDLE_LIB=/path/paddle/lib")
endif()



find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})



include_directories("${PADDLE_LIB}/paddle/include")
link_directories("${PADDLE_LIB}/paddle/lib")


include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories("${CMAKE_SOURCE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/include")


link_directories("${PADDLE_LIB}/paddle/lib")



  include_directories("${PADDLE_LIB}/third_party/install/mklml/include")

    set(MATH_LIB ${PADDLE_LIB}/third_party/install/mklml/lib/mklml.lib
            ${PADDLE_LIB}/third_party/install/mklml/lib/libiomp5md.lib)

  set(MKLDNN_PATH "${PADDLE_LIB}/third_party/install/mkldnn")
  include_directories("${MKLDNN_PATH}/include")
  set(MKLDNN_LIB ${MKLDNN_PATH}/lib/mkldnn.lib)


# Note: libpaddle_inference_api.so/a must put before libpaddle_inference.so/a
set(DEPS
${PADDLE_LIB}/paddle/lib/paddle_inference.lib)

set(DEPS ${DEPS}
${MATH_LIB} ${MKLDNN_LIB})



set(DEPS ${DEPS} ${OpenCV_LIBS})


AUX_SOURCE_DIRECTORY(./src SRCS)
# add_library(ocr_paddle ${SRCS}  target_position_ocrpp.cpp)
add_library(ocr_paddle SHARED ${SRCS} target_position_ocrpp.cpp )
# add_executable(ocr_paddle  ${SRCS}  )
target_link_libraries(ocr_paddle ${DEPS})

# add_custom_command(TARGET ocr_paddle POST_BUILD
# COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PADDLE_LIB}/third_party/install/mklml/lib/libiomp5md.dll ./libiomp5md.dll
# COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PADDLE_LIB}/third_party/install/mkldnn/lib/mkldnn.dll ./mkldnn.dll
# COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PADDLE_LIB}/third_party/install/mklml/lib/libiomp5md.dll ./release/libiomp5md.dll
# COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PADDLE_LIB}/third_party/install/mkldnn/lib/mkldnn.dll ./release/mkldnn.dll
# )
