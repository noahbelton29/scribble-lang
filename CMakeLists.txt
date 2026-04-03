cmake_minimum_required(VERSION 4.1)
project(scribble)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API OFF)

add_executable(scribble
    src/main.cpp
    src/lexer.cpp
    src/token_printer.cpp
    src/parser.cpp
    src/ast_printer.cpp
    src/interpreter.cpp
    src/file_utils.cpp
)