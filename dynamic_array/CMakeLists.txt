cmake_minimum_required(VERSION 3.10)

project(array)

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
add_executable(array src/main.cpp)

# Тесты
add_executable(array_tests src/test_array.cpp)
target_link_libraries(array_tests PRIVATE gtest_main gmock)

# Включение директив для тестов
target_include_directories(array_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Запуск тестов через CTest
enable_testing()
add_test(NAME array_tests COMMAND array_tests)