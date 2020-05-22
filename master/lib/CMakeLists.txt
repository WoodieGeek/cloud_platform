cmake_minimum_required(VERSION 3.10)

project(master_server_lib)

set(SOURCE_FILES server.cpp bfs.cpp instances.cpp graph.cpp server_options.cpp)

set(HEADER_FILES server.h bfs.h instances.h task.h graph.h server_options.h)

source_group("sources" FILES ${SOURCE_FILES})
source_group("headers" FILES ${HEADER_FILES})

add_library(master_server_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
