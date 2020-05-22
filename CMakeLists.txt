cmake_minimum_required(VERSION 3.15)
project(processor)

set(CMAKE_CXX_STANDARD 14)

add_executable(processorasm Assembler.cpp general.h registers.h )
add_executable(processorcpu CPU.cpp )
add_executable(processordisasm DisAssembler.cpp )