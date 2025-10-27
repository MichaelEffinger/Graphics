cmake_minimum_required(VERSION 3.25)

project(ComputerGraphics)

# Enable Hot Reload for MSVC compilers if supported (Windows only)
if (MSVC)
  if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
  endif()
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
          "$<IF:$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>,EditAndContinue,ProgramDatabase>")
endif()

# Add source files
add_executable(ComputerGraphics "ComputerGraphics.cpp" "ComputerGraphics.h")

# Yummy C++26...
set_property(TARGET ComputerGraphics PROPERTY CXX_STANDARD 26)


# Example: You can add platform-specific includes or libraries here
if (WIN32)
  # Windows-specific settings (if any)
  # target_link_libraries(ComputerGraphics some_windows_lib)
elseif(UNIX)
  # Linux-specific settings (if any)
  # target_link_libraries(ComputerGraphics some_linux_lib)
endif()

# TODO: Add tests and install targets if needed.
