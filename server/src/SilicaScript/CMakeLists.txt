#####################################################################
# Creating CommandInterpreter library target
#####################################################################

cmake_minimum_required(VERSION 3.17)

set(project CommandInterpreter)

project(${project} LANGUAGES CXX)

set(COMMANDINTERPRETER_PUBLIC_HEADERS
)

set(COMMANDINTERPRETER_PUBLIC_SOURCE
)

set(COMMANDINTERPRETER_PRIVATE_HEADERS
)

set(COMMANDINTERPRETER_PRIVATE_SOURCE
)

add_library(${project} 
        ${COMMANDINTERPRETER_PUBLIC_HEADERS}
        ${COMMANDINTERPRETER_PUBLIC_SOURCE}
        ${COMMANDINTERPRETER_PRIVATE_HEADERS}
        ${COMMANDINTERPRETER_PRIVATE_SOURCE}
)

target_include_directories(${project}
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src
)


