#####################################################################
# Powderpunk_Server Project
#####################################################################
cmake_minimum_required(VERSION 3.17)

# Project Name
project(
    Powderpunk_Server VERSION 0.0.0 
    DESCRIPTION "Server for the Powderpunk MUD"
    LANGUAGES CXX
)

# Specifying C++ version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Generate JSON file for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output binary files in specific dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#####################################################################
# Preparing external libraries
#####################################################################
add_subdirectory(external)

#####################################################################
# Compiling source code 
#####################################################################
add_subdirectory(src)

