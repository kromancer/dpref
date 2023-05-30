add_executable(SpMV main.cpp SpMV.cpp)

add_executable(SpMVFirstSlicing main.cpp SpMVFirstSlicing.cpp)
target_link_libraries(SpMVFirstSlicing PRIVATE Queue)
