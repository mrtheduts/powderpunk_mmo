#####################################################################
# Creating Server Executable target
#####################################################################

cmake_minimum_required(VERSION 3.17)
project(PowderpunkServerBin)

# Generate JSON file for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output binary files in specific dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(powderpunk_server main.cpp)
# Adding properties of GUI target
target_include_directories(powderpunk_server
    PRIVATE
        ${Powderpunk_Server_SOURCE_DIR}/src
)

target_compile_options(powderpunk_server PRIVATE -Werror -Wall -Wpedantic -Wextra)

#####################################################################
# Building local libs 
#####################################################################
add_subdirectory(DebugTools)
add_subdirectory(TextInterface)


#####################################################################
# Adding local libs 
#####################################################################
find_library(DebugTools REQUIRED)
find_library(TextInterface REQUIRED)

#####################################################################
# Adding external libs 
#####################################################################
find_library(gsl-lite REQUIRED)

set(BOOST_ROOT ${CMAKE_SOURCE_DIR}/external/boost_install)
set(BOOST_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/external/boost_install/include)
set(BOOST_LIBRARY_DIRS ${CMAKE_SOURCE_DIR}/external/boost_install/lib)

find_package(Boost REQUIRED)
include_directories(${BOOST_INCLUDE_DIRS})

find_library(telnetpp REQUIRED)

# Link it to libs
target_link_libraries(powderpunk_server
    PRIVATE 
        DebugTools
        TextInterface
        gsl-lite
        telnetpp
)

