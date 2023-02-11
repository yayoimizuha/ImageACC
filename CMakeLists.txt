cmake_minimum_required(VERSION 3.24)
project(ImageACC)

add_subdirectory(BitmapPlusPlus)
add_subdirectory(argh)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP REQUIRED)
if(OpenMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

add_executable(ImageACC main.cpp)
