cmake_minimum_required(VERSION 3.22)

project(dpref LANGUAGES CXX)

add_compile_options("-ggdb3" "-O0")

add_subdirectory(Queue)
add_subdirectory(SpMV)
add_subdirectory(BFS)

