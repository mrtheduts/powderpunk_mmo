#####################################################################
# Creating HeaderOnly library target
#####################################################################

cmake_minimum_required(VERSION 3.17)

set(project Utils)
project(${project} LANGUAGES CXX)

#####################################################################
# Adding header-only files
#####################################################################

# Adding properties of DebugTools target ############################
set(DEBUGTOOLS_PUBLIC_HEADER_FILES
        include/Utils/DebugTools/assert_debug_print.h
)


#####################################################################
# Creating Utils library
#####################################################################
add_library(${project}
    SHARED
        ${DEBUGTOOLS_PUBLIC_HEADER_FILES}
)

target_include_directories(${project}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_options(${project} PRIVATE -Werror -Wall -Wpedantic -Wextra)

target_compile_features(${project} PRIVATE cxx_std_14)
set_target_properties(${project} PROPERTIES LINKER_LANGUAGE CXX)

