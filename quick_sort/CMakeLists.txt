cmake_minimum_required(VERSION 3.10)

project(quick_sort)

add_executable(quick_sort src/main.cpp)

target_include_directories(quick_sort PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src  # Для QuickSort.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../dynamic_array/src  # Для Array.h
)