cmake_minimum_required(VERSION 3.10)

project(library)

set(SOURCE_LIB server.cpp session.cpp)

add_library(library STATIC ${SOURCE_LIB})
