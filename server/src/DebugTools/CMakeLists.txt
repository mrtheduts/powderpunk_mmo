#####################################################################
# Creating Debug library target
#####################################################################

cmake_minimum_required(VERSION 3.17)
project(debugtools)

# Adding properties of Debug target
set(DebugTools_PUBLIC_HEADER_FILES
        include/DebugTools/assert_debug_print.h
)

add_library(DebugTools
        ${DebugTools_PUBLIC_HEADER_FILES}
)

target_include_directories(DebugTools
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src
)

target_compile_options(DebugTools PRIVATE -Werror -Wall -Wpedantic -Wextra)

target_compile_features(DebugTools PRIVATE cxx_std_20)
set_target_properties(DebugTools PROPERTIES LINKER_LANGUAGE CXX)
