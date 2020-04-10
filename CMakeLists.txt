cmake_minimum_required(VERSION 3.14)
include_directories("/usr/include/postgresql/server/")
project(hamming C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

find_package(PostgreSQL REQUIRED)
add_library(hamming SHARED hamming.c)