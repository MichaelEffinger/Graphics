cmake_minimum_required(VERSION 3.30)

include(CMake/ES_Util.cmake)
include(FetchContent)

project(ComputerGraphics LANGUAGES C CXX)

# SETTINGS!!!
  # Testing
option(BUILD_TESTING "Build the testing suite" ON)
option(BUILD_ENGINE "Build the actual game engine itself" ON)
option(FETCH_CATCH2 "Download Catch2 if not found" ON)
option(FETCH_GLFW "Download GLFW if not found" ON)
set(DESIRED_BACKEND "WebGPU" CACHE STRING "The backend the engine shall use.")
set_property(CACHE DESIRED_BACKEND PROPERTY STRINGS "WebGPU" "Raylib")

if(BUILD_ENGINE)
    add_executable(ComputerGraphics)
    ES_enable_CXX26_for_project(ComputerGraphics)
endif()

#########################################
# Neat testing set up function FOR FREESTANDING ES library things #
#########################################
function(testing_set_up)
    if(NOT BUILD_TESTING)
        message(NOTICE "Skipping the Catch2 test suite...")
        return()
    endif()

    find_package(Catch2 CONFIG QUIET)

    if(NOT Catch2_FOUND AND FETCH_CATCH2)
        message(NOTICE "Catch2 not found locally, fetching...")
        FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG        v3.15.0
        )
        FetchContent_MakeAvailable(Catch2)
        set(Catch2_FOUND TRUE)
    endif()

    if(Catch2_FOUND)
        message(NOTICE "Building the Catch2 testing suite!")
        add_subdirectory(tests)
        return()
    endif()

    message(WARNING "Catch2 testing suite was requested to be built, yet Catch2 itself was never found nor fetched.")

endfunction()
testing_set_up()

###############
# Backend Finder 3000 #
###############
function(graphics_and_window_set_up)
    if(DESIRED_BACKEND STREQUAL "Raylib")
        message("Chosen window library: Raylib")
        message("Chosen backend: Raylib")
        find_package(raylib CONFIG REQUIRED)

        find_path(raylib_cpp_INCLUDE_DIR
                NAMES raylib-cpp.hpp
                PATHS ${CMAKE_PREFIX_PATH}
                PATH_SUFFIXES include
        )
        target_link_libraries(ComputerGraphics PRIVATE raylib
                $<$<BOOL:${WIN32}>:winmm gdi32>
        )
        message(FATAL_ERROR "Hey! Don't use raylib now! Clear the CMake cache and build again if you cannot make it use GLFW3")
    endif()

    # GLFW path
    message("Chosen window library: GLFW3")

    find_package(glfw3 QUIET)

    if(NOT glfw3_FOUND AND FETCH_GLFW)
        message(NOTICE "GLFW3 not found locally, fetching...")
        FetchContent_Declare(
            glfw
            GIT_REPOSITORY https://github.com/glfw/glfw.git
            GIT_TAG        3.5.1
        )
        # These suppress GLFW building its own tests/docs/examples
        set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(glfw)
        set(glfw3_FOUND TRUE)
    endif()

    if(NOT glfw3_FOUND)
        if(BUILD_TESTING)
            message(WARNING "Engine requested to be built, with a backend that needed GLFW3, yet GLFW3 was not found!\nThe engine will not be built, yet the tests still will.\nTry acquiring GLFW3, setting FETCH_GLFW=ON, or building with -DBUILD_ENGINE=OFF.")
        else()
            message(FATAL_ERROR "GLFW3 not found and fetching is disabled. Set FETCH_GLFW=ON or install GLFW3.")
        endif()
        return()
    endif()

    # Only add sources once we know GLFW is available
    target_sources(ComputerGraphics PUBLIC main_webgpu.cpp)
    target_link_libraries(ComputerGraphics PRIVATE glfw)
    target_compile_definitions(ComputerGraphics PRIVATE ES_GLFW)
    message("Successfully found and linked GLFW3!")

endfunction()

if(BUILD_ENGINE)
    message("Building the engine now!")
    graphics_and_window_set_up()
else()
    message("Skipping building the engine...")
endif()