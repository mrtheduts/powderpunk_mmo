#####################################################################
# Creating TextInterface library target
#####################################################################

cmake_minimum_required(VERSION 3.17)
project(textinterface LANGUAGES CXX)

set(TEXTINTERFACE_PUBLIC_HEADERS
        include/TextInterface/text_interface.h
)

set(TEXTINTERFACE_PUBLIC_SOURCE
        src/text_interface.cpp
)

add_library(TextInterface 
        ${TEXTINTERFACE_PUBLIC_HEADERS}
        ${TEXTINTERFACE_PUBLIC_SOURCE}
)

target_include_directories(TextInterface
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/src
)

target_compile_options(TextInterface PRIVATE -Werror -Wall -Wpedantic -Wextra)

target_compile_features(TextInterface PRIVATE cxx_std_20)

