add_executable(MG main.cpp MG.cpp Utils.cpp)

add_executable(MGFirstSlicing main.cpp MGFirstSlicing.cpp Utils.cpp)
target_link_libraries(MGFirstSlicing PRIVATE Queue)
