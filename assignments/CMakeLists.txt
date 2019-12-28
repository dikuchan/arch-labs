cmake_minimum_required(VERSION 3.15)

project(assignment LANGUAGES C)

find_package(OpenMP REQUIRED)

add_executable(minmax minmax.c)
add_executable(multiplication multiplication.c)
add_executable(vector vector.c)
add_executable(pattern pattern.c)

target_compile_options(minmax PRIVATE -Werror -Wall -Wextra)
target_compile_options(multiplication PRIVATE -Werror -Wall -Wextra)
target_compile_options(vector PRIVATE -Werror -Wall -Wextra)
target_compile_options(pattern PRIVATE -Werror -Wall -Wextra)

target_link_libraries(minmax PRIVATE OpenMP::OpenMP_C)
target_link_libraries(multiplication PRIVATE OpenMP::OpenMP_C)
target_link_libraries(vector PRIVATE OpenMP::OpenMP_C)
target_link_libraries(pattern PRIVATE OpenMP::OpenMP_C)

set_property(DIRECTORY PROPERTY C_STANDARD 99)

add_compile_definitions(WRITE)
add_compile_definitions(INLINE)
