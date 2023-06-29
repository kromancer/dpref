add_executable(BFS main.cpp BFS.cpp)
target_link_libraries(BFS PRIVATE Vertex)

add_executable(BFSFirstSlicing main.cpp BFSFirstSlicing.cpp)
target_link_libraries(BFSFirstSlicing PRIVATE Vertex Queue)
