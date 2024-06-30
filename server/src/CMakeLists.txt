#####################################################################
# Creating Server Executable target
#####################################################################
cmake_minimum_required(VERSION 3.17)

set(project PowderpunkServer)
set(exe powderpunk_server)

project(${project})

# Generate JSON file for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output binary files in specific dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set std C++ version for the code
# C++ 14 is required for Telnet++
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED TRUE)
set (CMAKE_CXX_EXTENSIONS FALSE)

add_executable(${exe} main.cpp)

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
add_subdirectory(DataStructures)
add_subdirectory(GameServer)
add_subdirectory(Logger)
add_subdirectory(LoginManager)
add_subdirectory(TextInterface)
add_subdirectory(Utils)

#####################################################################
# Adding local libs 
#####################################################################
# Link it to libs
target_link_libraries(powderpunk_server
    PRIVATE 
        Boost::thread
        GameServer
        Logger
        TelnetInterface
        Utils
)

