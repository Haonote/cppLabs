cmake_minimum_required(VERSION 3.15)
project(SmallVectorPractice LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -g)

add_executable(testsmallvector test_smallvector.cpp)

target_include_directories(testsmallvector PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})