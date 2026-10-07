# sandbox/CMakeLists.txt
# Manual, run-it-and-look programs. Needs a display. Not part of CI.

set(ES_SANDBOX_MODULES
        ${CMAKE_SOURCE_DIR}/ES_math.ixx
        ${CMAKE_SOURCE_DIR}/ContainerN.cppm
        ${CMAKE_SOURCE_DIR}/ArithmeticOpsMixin.cppm
        ${CMAKE_SOURCE_DIR}/VectorN.cppm
        ${CMAKE_SOURCE_DIR}/VectorH.cppm
        ${CMAKE_SOURCE_DIR}/ES_concepts.cppm
        ${CMAKE_SOURCE_DIR}/PointN.cppm
        ${CMAKE_SOURCE_DIR}/Matrix.cppm
        ${CMAKE_SOURCE_DIR}/Transform.cppm
        ${CMAKE_SOURCE_DIR}/Quaternion.cppm
        ${CMAKE_SOURCE_DIR}/EulerAngles.cppm
        ${CMAKE_SOURCE_DIR}/ES_meta.cppm
        ${CMAKE_SOURCE_DIR}/ColorN.cppm
        ${CMAKE_SOURCE_DIR}/Angle.cppm
        ${CMAKE_SOURCE_DIR}/ES_except.ixx
        ${CMAKE_SOURCE_DIR}/ES_easy.ixx
        ${CMAKE_SOURCE_DIR}/ES_game.ixx
        ${CMAKE_SOURCE_DIR}/ES_flyweight.ixx
        ${CMAKE_SOURCE_DIR}/ES_cyclical_iterator.ixx
        ${CMAKE_SOURCE_DIR}/Size.cppm
        # new for windowing
        ${CMAKE_SOURCE_DIR}/Platform/WindowMode.cppm
        ${CMAKE_SOURCE_DIR}/Platform/GLFW.cppm
        ${CMAKE_SOURCE_DIR}/Platform/Window.cppm
)

# One executable per scenario: window_open.cpp, window_resize.cpp, ...
set(ES_SANDBOX_PROGRAMS
        window
)

find_package(glfw3 REQUIRED)

foreach(prog IN LISTS ES_SANDBOX_PROGRAMS)
    set(target ES_Sandbox_${prog})

    add_executable(${target} ${prog}.cpp)

    # Same Ninja cut-in as the tests: modules go in a CXX_MODULES file set.
    # Non-Ninja generators (MSVC) take them as plain sources.
    if(CMAKE_GENERATOR STREQUAL "Ninja")
        target_sources(${target}
                PUBLIC
                FILE_SET CXX_MODULES
                BASE_DIRS ${CMAKE_SOURCE_DIR}
                FILES ${ES_SANDBOX_MODULES}
        )
    else()
        target_sources(${target} PRIVATE ${ES_SANDBOX_MODULES})
    endif()

    target_link_libraries(${target} PRIVATE glfw)

    if (ES_USE_STDCXXEXP)
        target_link_libraries(${target} PRIVATE stdc++exp)
    endif()

    ES_enable_CXX26_for_project(${target})
    ES_create_WE_HAVE_IMPORT_STD_macro_for_project(${target})
    ES_hook_ES_GAME_ROOT_DIR_macro_for_debug_builds(${target})
endforeach()