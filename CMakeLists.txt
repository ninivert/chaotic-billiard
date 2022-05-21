cmake_minimum_required (VERSION 3.5)

project(chaotic_billiard)

add_compile_options(-Wall -Wextra -O3)

add_subdirectory(physics)
add_subdirectory(gui)
