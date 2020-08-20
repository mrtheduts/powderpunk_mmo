#####################################################################
# Creating Server Executable target
#####################################################################

cmake_minimum_required(VERSION 3.17)
project(PowderpunkServerBin)

# Generate JSON file for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output binary files in specific dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set std C++ version for the code
# C++ 14 is required for Telnet++
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED TRUE)
set (CMAKE_CXX_EXTENSIONS FALSE)

add_executable(powderpunk_server main.cpp)
# Adding properties of GUI target
target_include_directories(powderpunk_server
    PRIVATE
        ${Powderpunk_Server_SOURCE_DIR}/src
)

# -Werror was removed because of Telnet++
add_compile_options("-Wall" "-Wpedantic" "-Wextra")

#####################################################################
# Adding external libs 
#####################################################################

set(BOOST_ROOT ${CMAKE_SOURCE_DIR}/external/boost_install)
set(BOOST_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/external/boost_install/include)
set(BOOST_LIBRARY_DIRS ${CMAKE_SOURCE_DIR}/external/boost_install/lib)

find_package(Boost COMPONENTS thread REQUIRED)
include_directories(${BOOST_INCLUDE_DIRS})

#####################################################################
# Building local libs 
#####################################################################
add_subdirectory(DebugTools)
add_subdirectory(TelnetInterface)


#####################################################################
# Adding local libs 
#####################################################################
find_library(TelnetInterface SHARED IMPORTED)
find_library(DebugTools SHARED IMPORTED)

# Link it to libs
target_link_libraries(powderpunk_server
    PRIVATE 
        Boost::thread
        DebugTools
        TelnetInterface
)

