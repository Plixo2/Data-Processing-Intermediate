cmake_minimum_required(VERSION 3.21)
project(data-processing-intermediate)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O0")

add_executable(data-processing-intermediate main.cpp Tokenizer.cpp Tokenizer.h IterableStream.h Vm.h Math.h Translator.h Gc.h LexerC.cpp LexerC.h DPI_Syntax.h DPI_Syntax.cpp)
