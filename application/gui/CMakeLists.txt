cmake_minimum_required(VERSION 3.17)

# Project Name
project(Powderpunk_GUI_Client VERSION 0.0.0 LANGUAGES CXX)

# Specifying C++ version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Generate JSON file for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output binary files in specific dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add src directory
add_subdirectory(src)

