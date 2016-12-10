cmake_minimum_required(VERSION 3.6)

project(TinyStringBuilderTests)

set(CMAKE_CXX_STANDARD 11)

include_directories(
    /
    tests
)

set(SOURCE_FILES

        #CStringBuilder.hpp
        tests/catch.cpp
        tests/catch.hpp
        tests/CStringBuilderIntTest.cpp
        tests/CStringBuilderFloatTest.cpp
        tests/Benchmark.cpp
        )

add_executable(TinyStringBuilderTests ${SOURCE_FILES} CStringBuilder.hpp)