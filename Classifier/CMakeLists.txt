cmake_minimum_required(VERSION 3.5)

project(Classif)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(OpenGL_GL_PREFERENCE GLVND)

add_executable(Classif class.cpp)
