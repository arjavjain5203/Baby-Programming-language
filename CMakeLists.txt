cmake_minimum_required(VERSION 3.20.3) # min version required to run the compiler

project(Baby) # name of the project

set(CMAKE_CXX_STANDARD 20) # set the C++ standard to use

add_executable(baby ../src/main.cpp) # create executable from source
