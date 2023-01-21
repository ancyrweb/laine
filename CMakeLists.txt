cmake_minimum_required(VERSION 3.24)
project(Laine C)

set(CMAKE_C_STANDARD 17)

add_executable(Laine main.c src/compiler.c src/compiler.h src/core/scanner.c src/core/scanner.h src/core/token.c src/core/token.h src/core/parser.c src/core/parser.h src/core/memory.c src/core/memory.h)
