cmake_minimum_required(VERSION 3.13)
project(lab3)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab3 main.cpp
        NetworkServer.cpp
        NetworkServer.h
        Symbol.cpp
        Symbol.h
        SharedEditor.cpp
        SharedEditor.h
        Message.cpp
        Message.h)