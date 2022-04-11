cmake_minimum_required(VERSION 3.21)
project(data-processing-intermediate)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O2")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_executable(data-processing-intermediate main.cpp Tokenizer.cpp Tokenizer.h IterableStream.h Lexer.h DPI_Syntax.h Vm.h Math.h Translator.h Gc.h LexerC.cpp LexerC.h)
