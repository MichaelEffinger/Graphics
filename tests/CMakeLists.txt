# Welcome to the debug CMakeLists
add_executable(ComputerGraphics_Tests ../ES_math.ixx
        VectorN_test.cpp
        VectorN_constexpr_test.cpp
        Math_test.cpp
        ES_test_util.hpp
        PointN_test.cpp
        PointN_constexpr_test.cpp
        VectorH_test.cpp
        VectorH_constexpr_test.cpp
        Color_test.cpp
        Matrix_test.cpp
        Matrix_constexpr_test.cpp
        Affine3_test.cpp
        Transform_test.cpp
        Transform_constexpr_test.cpp
        EulerAngles_test.cpp
        Quaternion_test.cpp
        Quaternion_constexpr_test.cpp
#        Random_test.cpp takes too long!
       ES_math_module_test.cpp
        ../ES_debug.ixx
)

if(NOT MSVC) #more MSVC cutouts!
target_sources(ComputerGraphics_Tests
  PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS
      ${CMAKE_SOURCE_DIR}      # the common root, e.g. /home/Michael/Graphics
    FILES
        ${CMAKE_SOURCE_DIR}/ES_math.ixx
        ${CMAKE_SOURCE_DIR}/ES_debug.ixx
)
endif ()

target_compile_definitions(ComputerGraphics_Tests PRIVATE NDEBUG)
target_link_libraries(ComputerGraphics_Tests PRIVATE Catch2::Catch2WithMain)
ES_enable_CXX26_for_project(ComputerGraphics_Tests)

enable_testing()
include(CTest)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(ComputerGraphics_Tests
        DISCOVERY_MODE PRE_TEST #works better with modules, allegedly.
)