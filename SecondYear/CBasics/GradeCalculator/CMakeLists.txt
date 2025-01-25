cmake_minimum_required(VERSION 3.26)
project(Project C)

set(CMAKE_C_STANDARD 11)

add_executable(Project main.c
        read_input.c
        grade_calculator.h
        grade_operations.c
        format_data.c
        format_strings.h)
