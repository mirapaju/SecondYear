cmake_minimum_required(VERSION 3.26)
project(lab12)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab12 main.cpp
        MenuItem.h
        Menu.cpp
        Menu.h
        IntEdit.cpp
        IntEdit.h
        MenuItem.cpp)
