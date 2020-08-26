cmake_minimum_required(VERSION 2.8)
project(smallsh C)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=gnu99 -Wall -pedantic")

set(CMAKE_C_STANDARD 99)

add_executable(smallsh smallsh.c)