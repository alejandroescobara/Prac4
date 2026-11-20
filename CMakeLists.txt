cmake_minimum_required(VERSION 3.20)
project(p4 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(p4
    src/main.cpp
    src/lib.cpp
    src/Network.cpp
    src/Router.cpp

    include/lib.hpp
    include/Network.hpp
    include/Router.hpp
)

target_include_directories(p4 PRIVATE include)
