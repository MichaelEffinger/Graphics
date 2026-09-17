########################################################
# Evil CMake Helper file, because--in this house--we DO NOT COUPLE NOR CLOUD in this house. #
########################################################



#Makes a project use C++26.
#MSVC is not cooperating, so I made this.
# @param target_name the name of the target to be set to C++26 (privately).
function(ES_enable_CXX26_for_project target_name)
#    if(MSVC)
#        target_compile_options(${target_name} PRIVATE "/std:c++latest" "/W4") #MSVC does not want to cooperate with the '26 times, man.
#    else ()
        target_compile_features(${target_name} PRIVATE cxx_std_23) #purportedly better than setting CXX_STANDARD as 26, because this instead enforces a minimum version
#    endif ()
endfunction()

#[[
Creates a macro named WE_HAVE_IMPORT_STD with the value one or zero for the given target.
0 means absolutely not.
1 means maybe, hopefully, but still is not a guarantee in and of itself.
]]
function(ES_create_WE_HAVE_IMPORT_STD_macro_for_project target_name)

    #that'll be the day...
    set(we_have_converted_all_modules_to_use_the_guards FALSE)

    if(we_have_converted_all_modules_to_use_the_guards AND CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(we_might 1)
    else ()
        set(we_might 0)
    endif ()

    target_compile_definitions(${target_name} PRIVATE
            WE_HAVE_IMPORT_STD=${we_might}
            WE_LACK_IMPORT_STD=$<IF:$<BOOL:${well_do_we}>,0,1>
    )
endfunction()

#I'll chalk up the difference between hooking and creating a macro to whether or not it actually stores a usable value, where hooking does.
function(ES_hook_ES_GAME_ROOT_DIR_macro_for_debug_builds target_name)
    target_compile_definitions(${target_name} PRIVATE
            ES_GAME_ROOT_DIR="${CMAKE_SOURCE_DIR}"
    )
endfunction()