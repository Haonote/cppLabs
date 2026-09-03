cmake_minimum_required(VERSION 3.15)
project(SmallVectorPractice LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -g)

add_executable(01lab 01_references_and_value_categories.cpp)
add_executable(02lab 02_class_raii_and_explicit.cpp)
add_executable(03lab 03_rule_of_three_deep_copy.cpp)
add_executable(04lab 04_rule_of_five_and_move.cpp)
add_executable(05lab 05_polymorphism_and_enums.cpp)
add_executable(06lab 06_smart_pointers_and_ownership.cpp)
add_executable(07lab 07_string_view_and_zero_copy.cpp)
    