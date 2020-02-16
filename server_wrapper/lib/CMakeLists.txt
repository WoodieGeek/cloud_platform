cmake_minimum_required(VERSION 3.10)

project(server_wrapper_lib)

set(SOURCE_FILES server.cpp subprocess.cpp)

set(HEADER_FILES server.h subprocess.h)

source_group("sources" FILES ${SOURCE_FILES})
source_group("headers" FILES ${HEADER_FILES})

add_library(server_wrapper_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
