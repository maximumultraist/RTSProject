cmake_minimum_required(VERSION 3.9)
project(Project01)

set(CMAKE_CXX_STANDARD 11)

add_executable(Project01 main.cpp cmake-build-debug/Buffer.h BufferType1.cpp BufferType1.h)