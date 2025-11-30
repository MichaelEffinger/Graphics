cmake_minimum_required(VERSION 3.25)

include(CMake/ES_Util.cmake) #yeah, we've includes in CMake now, buckle up.



project(ComputerGraphics LANGUAGES CXX)


#SETTINGS!!!
  #Testing
option(BUILD_TESTING "Build the testing suite" ON)
option(BUILD_ENGINE "Build the actual game engine itself" ON) #maybe... if you only wanted tests?
option(FETCH_CATCH2 "Download Catch2 if not found" OFF)
set(DESIRED_BACKEND "WebGPU" CACHE STRING "The backend the engine shall use.") #sets the default funny backend
set_property(CACHE DESIRED_BACKEND PROPERTY STRINGS "WebGPU" "Raylib") #this just gives the CMake GUI a dropdown... allegedly. Does not work as intended

if(BUILD_ENGINE)
# Main executable
add_executable(ComputerGraphics)

ES_enable_CXX26_for_project(ComputerGraphics)
endif ()

#########################################
# Neat testing set up function FOR FREESTANDING ES library things #
#########################################
function(testing_set_up)
    if(NOT BUILD_TESTING)
        message(NOTICE "Skipping the Catch2 test suite...")
        return() #this actually takes us out of the function (blocks do NOT work as I thought... OOPSIE), instead of prematurely terminating before reaching whatever may lie below!
    endif()

    find_package(Catch2 CONFIG QUIET) #only once we have the build testing go ahead, do we actually need to see if we've it.

    if(NOT Catch2_FOUND AND FETCH_CATCH2) #it is imperative we go get it!
        message(WARNING "Fetching Catch2 hasn't been implemented yet! Tee hee.")
    endif()

    if(Catch2_FOUND)
        message(NOTICE "Building the Catch2 testing suite!")
        add_subdirectory(tests) #and then this sees the CMakeLists INSIDE of the test folder!
        return()
    endif()

    message(WARNING "Catch2 testing suite was requested to be built, yet Catch2 itself was never found nor fetched.")

endfunction()
testing_set_up() #immediately invoke the function... god I wish I could just stick a pair of parenthesis at the end of the declaration like an instant lambda...

###############
# Backend Finder 3000 #
###############
function(graphics_and_window_set_up)
    if(DESIRED_BACKEND STREQUAL "Raylib") #should we need raylib we just grab it and go
        find_package(raylib CONFIG REQUIRED)

        find_path(raylib_cpp_INCLUDE_DIR
                NAMES raylib-cpp.hpp
                PATHS ${CMAKE_PREFIX_PATH}
                PATH_SUFFIXES include
        )
        target_link_libraries(ComputerGraphics PRIVATE raylib
                $<$<BOOL:${WIN32}>:winmm gdi32> #Raylib needs these... on Windows exclusively... thank goodness I am so handsome and smart I know how to use generator expressions.
        )
        message(FATAL_ERROR "Hey! Don't use this!")
#        target_sources(ComputerGraphics PUBLIC ComputerGraphics.cpp)
#        message("Successfully found and linked Raylib!")
#        target_compile_definitions(ComputerGraphics PRIVATE ES_RAYLIB) #a preprocessor macro to signal that we're using raylib
#        return() #We've raylib, we can exit now.
    endif ()
    #Now, if we're not using raylib, we must go on a GLFW adventure
    target_sources(ComputerGraphics PUBLIC main_webgpu.cpp)

    find_package(glfw3 REQUIRED)
    target_link_libraries(ComputerGraphics PRIVATE glfw)
    target_compile_definitions(ComputerGraphics PRIVATE ES_GLFW) #macro for GLFW
    message("Successfully found and linked GLFW3!")




endfunction()
if(BUILD_ENGINE)
    message("Building the engine now!")
    graphics_and_window_set_up()
else ()
    message("Skipping building the engine entirely!")
endif ()