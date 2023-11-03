cmake_minimum_required(VERSION 3.0)

project(huffman)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
	huffman
	Source/Main.cpp
	Source/Huffman.cpp
)
