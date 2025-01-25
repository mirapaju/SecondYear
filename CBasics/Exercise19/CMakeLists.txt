cmake_minimum_required(VERSION 3.26)
project(Exercise19 C)

set(CMAKE_C_STANDARD 11)

add_executable(Exercise19 main.c
        menu.h
        menu_handling.c
        reading.c
        compare.c)
