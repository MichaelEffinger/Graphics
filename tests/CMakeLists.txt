#Welcome to the debug CMakeLists
add_executable(ComputerGraphics_Tests
        tests/VectorN_test.cpp
)

target_compile_definitions(ComputerGraphics_Tests PRIVATE NDEBUG)

target_link_libraries(ComputerGraphics_Tests PRIVATE Catch2::Catch2WithMain ComputerGraphics)
target_compile_features(ComputerGraphics_Tests PRIVATE cxx_std_26)

# Enable testing & integrate Catch2 with CTest
enable_testing()
include(CTest)
#include(Catch) REDUNDANT! ...maybe... I'm getting conflicting reports...
catch_discover_tests(ComputerGraphics_Tests)
