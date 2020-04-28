cmake_minimum_required(VERSION 3.13)
project(hamming C)

set(CMAKE_C_STANDARD 99)

find_package(PostgreSQL REQUIRED)

include_directories(${PostgreSQL_INCLUDE_DIRS})

add_library(hamming SHARED hamming.c)
target_compile_options(
        hamming
        PRIVATE
        -Ofast
        -march=native
)