cmake_minimum_required(VERSION 3.25)
project(ComputerGraphics LANGUAGES CXX)

# Main executable
add_executable(ComputerGraphics "ComputerGraphics.cpp" "ComputerGraphics.h")
set_property(TARGET ComputerGraphics PROPERTY CXX_STANDARD 26)
set_property(TARGET ComputerGraphics PROPERTY CXX_STANDARD_REQUIRED ON)

find_package(Catch2 CONFIG REQUIRED)

add_executable(ComputerGraphics_Tests
    tests/VectorN_test.cpp
)

target_link_libraries(ComputerGraphics_Tests PRIVATE Catch2::Catch2WithMain)
set_property(TARGET ComputerGraphics_Tests PROPERTY CXX_STANDARD 26)
set_property(TARGET ComputerGraphics_Tests PROPERTY CXX_STANDARD_REQUIRED ON)

# Enable testing & integrate Catch2 with CTest
enable_testing()
include(CTest)
include(Catch)
catch_discover_tests(ComputerGraphics_Tests)
