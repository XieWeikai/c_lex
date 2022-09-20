cmake_minimum_required(VERSION 3.23)
project(lex C)

set(CMAKE_C_STANDARD 99)

add_executable(lex main.c global.c lex.yy.c)
