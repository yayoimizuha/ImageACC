cmake_minimum_required(VERSION 3.24)
project(ImageACC)

add_subdirectory(BitmapPlusPlus)
add_subdirectory(argh)
set(CMAKE_CXX_STANDARD 17)

add_executable(ImageACC main.cpp)
