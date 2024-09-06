cmake_minimum_required(VERSION 3.15)

if (APPLE)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "" FORCE)
message("-- Compiling fat Binaries on macos")
endif()

PROJECT(HELLEBORE
LANGUAGES CXX
VERSION 1.0.1
)

# for clang-tidy(this enable to find system header files).
if(CMAKE_EXPORT_COMPILE_COMMANDS)
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

# Enable JUCE. Do not use find_package to prevent from mix up with one globally installed.
add_subdirectory(lib/JUCE)

add_subdirectory(src)