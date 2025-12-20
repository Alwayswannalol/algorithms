cmake_minimum_required(VERSION 3.10)

project(quick_sort)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Основной исполняемый файл
add_executable(quick_sort src/main.cpp)

# Тесты
add_executable(comparison_sorts src/comparison_test.cpp)
add_executable(quick_sort_tests src/test_quicksort.cpp)
target_link_libraries(quick_sort_tests PRIVATE gtest_main gmock)

# Включение директив
target_include_directories(quick_sort PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../dynamic_array/src
)

target_include_directories(quick_sort_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../dynamic_array/src
)

# Запуск тестов через CTest
enable_testing()
add_test(NAME quick_sort_tests COMMAND quick_sort_tests)