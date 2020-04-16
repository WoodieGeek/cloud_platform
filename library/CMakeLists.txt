cmake_minimum_required(VERSION 3.10)

project(server_library)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -pthread") 

set(SOURCE_FILES server.cpp session.cpp request_parser.cpp resolver.cpp reply.cpp request.cpp client_session.cpp client.cpp
    reply_parser.cpp)

set(HEADER_FILES server.h session.h request_parser.h resolver.h reply.h request.h client_session.h client.h
    reply_parser.h)

source_group("sources" FILES ${SOURCE_FILES})
source_group("headers" FILES ${HEADER_FILES})

add_library(server_library STATIC ${SOURCE_FILES} ${HEADER_FILES})
