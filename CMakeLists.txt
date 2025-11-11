cmake_minimum_required(VERSION 3.25)

include(CMake/ES_Util.cmake) #yeah, we've includes in CMake now, buckle up.



project(ComputerGraphics LANGUAGES CXX)


#SETTINGS!!!
  #Testing
option(BUILD_TESTING "Build the testing suite" ON)
option(FETCH_CATCH2 "Download Catch2 if not found" OFF)


# Main executable
add_executable(ComputerGraphics "ComputerGraphics.cpp" "ComputerGraphics.h")

ES_enable_CXX26_for_project(ComputerGraphics)


#############
# Neat testing block #
#############
block()
    if(NOT BUILD_TESTING)
        message(NOTICE "Skipping the Catch2 test suite...")
        return() #this actually takes us out of the block, instead of prematurely terminating before reaching whatever may lie below!
    endif()

    find_package(Catch2 CONFIG QUIET) #only once we have the build testing go ahead, do we actually need to see if we've it.

    if(NOT Catch2_FOUND AND FETCH_CATCH2) #it is imperative we go get it!
        message(NOTICE "Fetching Catch2 hasn't been implemented yet! Tee hee.")
    endif()

    if(Catch2_FOUND)
        message(STATUS "Building the Catch2 testing suite!")
        add_subdirectory(tests) #and then this sees the CMakeLists INSIDE of the test folder!
        return()
    endif()

    message(WARNING "Catch2 testing suite was requested to be built, yet Catch2 itself was never found nor fetched.")

endblock()
