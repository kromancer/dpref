add_executable(BFS main.cpp BFS.cpp)

add_executable(BFSFirstSlicing main.cpp BFSFirstSlicing.cpp)
target_link_libraries(BFSFirstSlicing PRIVATE Queue)
