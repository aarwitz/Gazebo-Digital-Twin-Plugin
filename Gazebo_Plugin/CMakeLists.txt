cmake_minimum_required(VERSION 3.5)

project(arduinoIMU LANGUAGES CXX)
find_package(gazebo REQUIRED)

include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fPIC")

add_library(arduinoIMU SHARED arduinoIMU.cc)
target_link_libraries(arduinoIMU ${GAZEBO_LIBRARIES} client)

