cmake_minimum_required(VERSION 3.9)
project(RTSProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(Project01 main.cpp BufferType1.cpp BufferType1.h BufferType2.cpp BufferType2.h)