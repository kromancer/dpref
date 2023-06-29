cmake_minimum_required(VERSION 3.22)

project(dpref LANGUAGES CXX)

add_compile_options("-std=c++2b" "-ggdb3" "-O0")

# Static linkage simplifies things when running on gem5
# Assumes that when compiling on Linux, we are compiling for running on gem5
add_link_options("$<$<PLATFORM_ID:Linux>:-static>")

add_subdirectory(Queue)
add_subdirectory(Vertex)
add_subdirectory(SpMV)
add_subdirectory(BFS)

