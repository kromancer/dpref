add_executable(MG main.cpp MG.cpp Utils.cpp smooth.cpp)

add_executable(MGFirstSlicing main.cpp MGFirstSlicing.cpp Utils.cpp smoothFirstSlicing.cpp)
target_link_libraries(MGFirstSlicing PRIVATE Queue)
