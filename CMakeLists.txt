cmake_minimum_required(VERSION 3.14)
include_directories("/usr/include/postgresql/server/")
project(hamming C)

set(CMAKE_C_STANDARD 99)

find_package(PostgreSQL REQUIRED)
add_library(hamming SHARED hamming.c)
target_compile_options(
        hamming
        PRIVATE
        -Ofast
        -march=native
)