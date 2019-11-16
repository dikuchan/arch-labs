cmake_minimum_required(VERSION 3.15)

project(assignment)

add_executable(minmax minmax.c)
add_executable(multiplication multiplication.c)
add_executable(vector vector.c)

set_property(DIRECTORY PROPERTY C_STANDARD 99)

add_compile_definitions(WRITE)
add_compile_definitions(INLINE)

find_package(OpenMP)
if (OpenMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
endif()
