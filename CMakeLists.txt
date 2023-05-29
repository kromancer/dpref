cmake_minimum_required(VERSION 3.22)

project(dpref LANGUAGES CXX)

add_compile_options("-ggdb3" "-O0")

add_executable(BFS main.cpp BFS.cpp)
add_executable(BFSFirstSlicing main.cpp BFSFirstSlicing.cpp)

