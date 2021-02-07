#####################################################################
# Creating TextInputTranslate library target
#####################################################################

cmake_minimum_required(VERSION 3.17)

set(project TextInputTranslator)
project(${project} LANGUAGES CXX)

set(TEXTINPUTTRANSLATOR_PUBLIC_HEADERS
        include/TextInputTranslator/text_input_translator.h
)

set(TEXTINPUTTRANSLATOR_PUBLIC_SOURCE
)

set(TEXTINPUTTRANSLATOR_PRIVATE_HEADERS
)

set(TEXTINPUTTRANSLATOR_PRIVATE_SOURCE
)

add_library(TextInputTranslator 
        ${TEXTINPUTTRANSLATOR_PUBLIC_HEADERS}
        ${TEXTINPUTTRANSLATOR_PUBLIC_SOURCE}
        ${TEXTINPUTTRANSLATOR_PRIVATE_HEADERS}
        ${TEXTINPUTTRANSLATOR_PRIVATE_SOURCE}
)

#####################################################################
# Adding libs 
#####################################################################
# Link it to libs
target_link_libraries(${project}
    PUBLIC 
        Utils
)

target_include_directories(${project}
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src
)

target_compile_options(${project} PRIVATE -Werror -Wall -Wpedantic -Wextra)

target_compile_features(${project} PRIVATE cxx_std_14)
set_target_properties(${project} PROPERTIES LINKER_LANGUAGE CXX)
