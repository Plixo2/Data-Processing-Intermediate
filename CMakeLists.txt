cmake_minimum_required(VERSION 3.21)
project(data-processing-intermediate)

set(CMAKE_CXX_STANDARD 20)

add_executable(data-processing-intermediate main.cpp Tokenizer.cpp Tokenizer.h IterableStream.h Lexer.h DPI_Syntax.h)
