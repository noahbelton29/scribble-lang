cmake_minimum_required(VERSION 4.1)
project(scribble)

set(CMAKE_CXX_STANDARD 20)

add_executable(scribble src/main.cpp src/lexer.cpp src/lexer.h)