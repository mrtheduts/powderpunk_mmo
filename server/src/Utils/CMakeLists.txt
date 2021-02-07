#####################################################################
# Creating HeaderOnly library target
#####################################################################

cmake_minimum_required(VERSION 3.17)

set(project Utils)
project(${project} LANGUAGES CXX)

#####################################################################
# Adding header-only files as components
#####################################################################

# Adding properties of DebugTools target ############################
set(DEBUGTOOLS_PUBLIC_HEADER_FILES
        include/Utils/DebugTools/assert_debug_print.h
)

# Adding properties of DataStructures target ########################
set(DATASTRUCTURES_PUBLIC_HEADER_FILES
        include/Utils/DataStructures/user_command.h
)

#####################################################################
# Creating Utils library
#####################################################################
add_library(${project}
    SHARED
        ${DATASTRUCTURES_PUBLIC_HEADER_FILES}
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
