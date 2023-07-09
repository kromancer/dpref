add_executable(SpMV main.cpp SpMV.cpp)

add_executable(SpMVFirstSlicing main.cpp SpMVFirstSlicing.cpp)
target_link_libraries(SpMVFirstSlicing PRIVATE Queue)

add_executable(SpMVSecondSlicing main.cpp SpMVSecondSlicing.cpp)
target_link_libraries(SpMVSecondSlicing PRIVATE Queue)
