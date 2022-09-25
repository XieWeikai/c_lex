cmake_minimum_required(VERSION 3.23)
project(c_lex C)

set(CMAKE_C_STANDARD 99)

aux_source_directory(. SRC)

add_executable(c_lex ${SRC})
