cmake_minimum_required(VERSION 3.10)
project(EmployeeProject)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(MainApp main.cpp)
add_executable(Creator creator.cpp)
add_executable(Reporter reporter.cpp)

option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/release-1.11.0.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(UnitTests UnitTests.cpp creator.cpp reporter.cpp)
    target_compile_definitions(UnitTests PRIVATE UNIT_TESTS)
    target_link_libraries(UnitTests PRIVATE gtest_main)
    target_include_directories(UnitTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    include(GoogleTest)
    gtest_discover_tests(UnitTests)
endif()
