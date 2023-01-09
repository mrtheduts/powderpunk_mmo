#####################################################################
# Creating DataStructures library target
#####################################################################

cmake_minimum_required(VERSION 3.17)

set(project DataStructures)

project(${project} LANGUAGES CXX)

set(DATASTRUCTURES_PUBLIC_HEADERS
        include/DataStructures/user_command.hpp
        include/DataStructures/user.hpp
)

set(DATASTRUCTURES_PUBLIC_SOURCE
)

set(DATASTRUCTURES_PRIVATE_HEADERS
    src/Commands/user_command.hpp
    src/User/user.hpp
)

set(DATASTRUCTURES_PRIVATE_SOURCE
    src/Commands/user_command.cpp
    src/User/user.cpp
)

add_library(${project} 
        ${DATASTRUCTURES_PUBLIC_HEADERS}
        ${DATASTRUCTURES_PUBLIC_SOURCE}
        ${DATASTRUCTURES_PRIVATE_HEADERS}
        ${DATASTRUCTURES_PRIVATE_SOURCE}
)

target_include_directories(${project}
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src
)

