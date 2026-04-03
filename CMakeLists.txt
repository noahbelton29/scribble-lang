cmake_minimum_required(VERSION 4.1)
project(scribble)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API OFF)

include_directories(${CMAKE_SOURCE_DIR}/external/pegtl/include)

add_executable(scribble
    src/main.cpp
    src/lexer.cpp
    src/token_printer.cpp
)