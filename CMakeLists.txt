cmake_minimum_required(VERSION 4.1)
project(scribble)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API OFF)

include_directories(
    ${CMAKE_SOURCE_DIR}/src/lexer
    ${CMAKE_SOURCE_DIR}/src/parser
    ${CMAKE_SOURCE_DIR}/src/interpreter
    ${CMAKE_SOURCE_DIR}/src/utils
)

add_executable(scribble
    src/main.cpp
    src/lexer/lexer.cpp
    src/parser/parser.cpp
    src/interpreter/interpreter.cpp
    src/utils/ast_printer.cpp
    src/utils/token_printer.cpp
    src/utils/file_utils.cpp
)