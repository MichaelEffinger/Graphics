cmake_minimum_required(VERSION 3.10)

project(ComputerGraphics)

# Enable Hot Reload for MSVC compilers if supported (Windows only)
if (MSVC)
  if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
  endif()
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
    "$<IF:$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue,ProgramDatabase>")
endif()

# Add source files
add_executable(ComputerGraphics "ComputerGraphics.cpp" "ComputerGraphics.h")

# Set C++20 standard if supported
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ComputerGraphics PROPERTY CXX_STANDARD 20)
endif()

# Example: You can add platform-specific includes or libraries here
if (WIN32)
  # Windows-specific settings (if any)
  # target_link_libraries(ComputerGraphics some_windows_lib)
elseif(UNIX)
  # Linux-specific settings (if any)
  # target_link_libraries(ComputerGraphics some_linux_lib)
endif()

# TODO: Add tests and install targets if needed.
