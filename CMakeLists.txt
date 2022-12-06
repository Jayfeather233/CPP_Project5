cmake_minimum_required(VERSION 3.12)

add_compile_options(-O3)

project(mat)

ADD_EXECUTABLE(mat test.cpp)

find_package(OpenMP)
if(OpenMP_C_FOUND)
   target_link_libraries(mat PUBLIC OpenMP::OpenMP_C)
endif()