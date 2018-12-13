cmake_minimum_required(VERSION 3.2)
PROJECT(demo)
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

find_package(Caffe)
include_directories(${Caffe_INCLUDE_DIRS})
add_definitions(${Caffe_DEFINITIONS})

#message(${Caffe_INCLUDE_DIRS})
include_directories(
    code/include
)

#add_library(${PROJECT_NAME}
#    code/src/demo.cpp
#    
#)

add_executable(${PROJECT_NAME}
    code/src/demo.cpp
    code/src/detector.cpp
    code/src/judge_state.cpp
    code/src/locking_id.cpp
)
target_link_libraries(${PROJECT_NAME} ${Caffe_LIBRARIES})
