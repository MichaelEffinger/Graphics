########################################################
# Evil CMake Helper file, because--in this house--we DO NOT COUPLE NOR CLOUD in this house. #
########################################################



#Makes a project use C++26.
#MSVC is not cooperating, so I made this.
# @param target_name the name of the target to be set to C++26 (privately).
function(ES_enable_CXX26_for_project target_name)
    if(MSVC)
        target_compile_options(${target_name} PRIVATE "/std:c++latest") #MSVC does not want to cooperate with the '26 times, man.
    else ()
        target_compile_features(${target_name} PRIVATE cxx_std_26) #purportedly better than setting CXX_STANDARD as 26, because this instead enforces a minimum version
    endif ()
endfunction()