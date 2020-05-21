#####################################################################
# Powderpunk_Server Project
#####################################################################
cmake_minimum_required(VERSION 3.17)

# Project Name
project(
    PowderpunkServer VERSION 0.0.0 
    DESCRIPTION "Server for the Powderpunk MMO"
    LANGUAGES CXX
)

# Specifying C++ version
#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED True)

# The Boost C++ Libraries were successfully built!

#The following directory should be added to compiler include paths:

    #/home/mrtheduts/Git/my_stuff/powderpunk_mmo/server/external/boost

#The following directory should be added to linker library paths:

    #/home/mrtheduts/Git/my_stuff/powderpunk_mmo/server/external/boost/stage/lib


#####################################################################
# Preparing external libraries
#####################################################################
add_subdirectory(external)

#####################################################################
# Compiling source code 
#####################################################################
add_subdirectory(src)

