cmake_minimum_required(VERSION 3.22.1)

project(serverEx)

add_executable(serverEx serverEx.cpp)

#target_link_libraries(muhderp mearlymath)
target_link_libraries(serverEx server)
