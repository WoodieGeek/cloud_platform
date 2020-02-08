cmake_minimum_required(VERSION 3.10)

project(library)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -pthread") 

set(SOURCE_LIB server.cpp session.cpp request_parser.cpp request_parser.h resolver.cpp reply.cpp)

add_library(library STATIC ${SOURCE_LIB})
