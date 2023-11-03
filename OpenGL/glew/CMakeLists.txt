cmake_minimum_required(VERSION 3.14)

project(glew)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

add_library(glew src/glew.c)