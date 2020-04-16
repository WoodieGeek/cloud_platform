cmake_minimum_required(VERSION 3.10)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost COMPONENTS chrono filesystem REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread -lpqxx -lpq -std=c++1z -g" )

include_directories(${Boost_INCLUDE_DIRS} ../library lib)

link_directories(../library)
add_subdirectory(../library build)

link_directories(lib)
add_subdirectory(lib)


set(PQXX /usr/local/include/pqxx)
find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

add_executable(main main.cpp)

target_link_libraries(main ${Boost_LIBRARIES} master_server_lib server_library nlohmann_json::nlohmann_json ${PQXX_LIB} ${PQ_LIB})
