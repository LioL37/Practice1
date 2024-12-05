cmake_minimum_required(VERSION 3.10)
project(CompleteBinaryTreeProject)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Добавляем Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
FetchContent_MakeAvailable(googletest)

# Добавляем тесты
add_executable(CompleteBinaryTree_tests test_CompleteBinaryTree.cpp)
target_include_directories(CompleteBinaryTree_tests PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(CompleteBinaryTree_tests gtest gtest_main)

# Включаем поддержку gcov
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

# Добавляем цель для запуска тестов и генерации отчета
add_custom_target(run_tests
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target CompleteBinaryTree_tests
    COMMAND ${CMAKE_BINARY_DIR}/CompleteBinaryTree_tests
    COMMAND lcov --capture --directory ${CMAKE_BINARY_DIR} --output-file coverage.info --ignore-errors inconsistent
    COMMAND genhtml coverage.info --output-directory coverage_report
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Добавляем цель run_tests в список целей, которые должны быть собраны
add_dependencies(run_tests CompleteBinaryTree_tests)