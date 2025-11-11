#Welcome to the debug CMakeLists
add_executable(ComputerGraphics_Tests
        VectorN_test.cpp
        PositionN_test.cpp
        Random_test.cpp
)

target_compile_definitions(ComputerGraphics_Tests PRIVATE NDEBUG)

target_link_libraries(ComputerGraphics_Tests PRIVATE Catch2::Catch2WithMain)
ES_enable_CXX26_for_project(ComputerGraphics_Tests)

# Enable testing & integrate Catch2 with CTest
enable_testing() #THIS is the redundant one...
include(CTest)
include(Catch) #NOT REDUNDANT!
catch_discover_tests(ComputerGraphics_Tests)
