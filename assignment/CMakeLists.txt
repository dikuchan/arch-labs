cmake_minimum_required(VERSION 3.15)

project(assignment LANGUAGES C)
set_property(DIRECTORY PROPERTY C_STANDARD 99)
find_package(OpenMP REQUIRED)

include_directories(include)
include_directories(source)

add_executable(assignment main.c)
target_compile_options(assignment PRIVATE -Werror -Wall -Wextra)
target_link_libraries(assignment PRIVATE OpenMP::OpenMP_C)

add_compile_definitions(WRITE)
add_compile_definitions(INLINE)
