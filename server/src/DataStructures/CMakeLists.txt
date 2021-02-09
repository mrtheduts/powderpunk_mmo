#####################################################################
# Creating DataStructures library target
#####################################################################

cmake_minimum_required(VERSION 3.17)

set(project DataStructures)

project(${project} LANGUAGES CXX)

set(DATASTRUCTURES_PUBLIC_HEADERS
        include/DataStructures/user_command.h
)

set(DATASTRUCTURES_PUBLIC_SOURCE
)

set(DATASTRUCTURES_PRIVATE_HEADERS
        src/user_command.h
)

set(DATASTRUCTURES_PRIVATE_SOURCE
        src/user_command.c
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

