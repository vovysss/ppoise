cmake_minimum_required(VERSION 3.14)
project(VectorLab CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Основное интерактивное приложение
add_executable(vector_app
    main.cpp
    vector.cpp
    vector.h
)

# 2. Подключение Google Test через FetchContent (скачает автоматически при первой сборке)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

# Для корректной сборки под Windows / MSVC (если используется)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# 3. Исполняемый файл с юнит-тестами
add_executable(vector_tests
    test_vector.cpp
    vector.cpp
    vector.h
)

# Подключаем библиотеку Google Test к тестам
target_link_libraries(vector_tests PRIVATE gtest_main)

# Интеграция с системой тестирования CMake (ctest)
include(GoogleTest)
gtest_discover_tests(vector_tests)