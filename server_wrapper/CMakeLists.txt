cmake_minimum_required(VERSION 3.10)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost COMPONENTS chrono filesystem REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} ../library lib)

link_directories(../library)
add_subdirectory(../library build)

link_directories(lib)
add_subdirectory(lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -pthread")

add_executable(main main.cpp)

target_link_libraries(main ${Boost_LIBRARIES} server_wrapper_lib server_library nlohmann_json::nlohmann_json)
