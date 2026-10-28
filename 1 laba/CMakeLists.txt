cmake_minimum_required(VERSION 3.14)
project(RectangleProject)

set(CMAKE_CXX_STANDARD 17)

# 1. Основное приложение с меню
add_executable(rectangle_app 
    main.cpp 
    rectangle.cpp
)

# 2. Включаем поддержку тестирования
enable_testing()

# Автоматически скачиваем и подключаем Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 3. Исполняемый файл для тестов
add_executable(rectangle_tests
    test_rectangle.cpp
    rectangle.cpp
)

# Подключаем библиотеку Google Test к нашим тестам
target_link_libraries(rectangle_tests GTest::gtest_main)

# Регистрируем тесты для работы с ctest
include(GoogleTest)
gtest_discover_tests(rectangle_tests)