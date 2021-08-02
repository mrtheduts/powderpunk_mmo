#####################################################################
# Creating TextToCommand library target
#####################################################################

cmake_minimum_required(VERSION 3.17)

set(project TextToCommand)

project(${project} LANGUAGES CXX)

set(TEXTTOCOMMAND_PUBLIC_HEADERS
        include/TextToCommand/text_to_command.h
)

set(TEXTTOCOMMAND_PUBLIC_SOURCE
)

set(TEXTTOCOMMAND_PRIVATE_HEADERS
        src/text_to_command.h
)

set(TEXTTOCOMMAND_PRIVATE_SOURCE
        src/text_to_command.cpp
)

add_library(${project} 
        ${TEXTTOCOMMAND_PUBLIC_HEADERS}
        ${TEXTTOCOMMAND_PUBLIC_SOURCE}
        ${TEXTTOCOMMAND_PRIVATE_HEADERS}
        ${TEXTTOCOMMAND_PRIVATE_SOURCE}
)

target_include_directories(${project}
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src
)

#####################################################################
# Adding external libs 
#####################################################################
set(BOOST_ROOT ${CMAKE_SOURCE_DIR}/external/boost_install)
set(BOOST_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/external/boost_install/include)
set(BOOST_LIBRARY_DIRS ${CMAKE_SOURCE_DIR}/external/boost_install/lib)

find_package(Boost COMPONENTS fiber thread log REQUIRED)
include_directories(${BOOST_INCLUDE_DIRS})

find_library(DataStructures SHARED IMPORTED)
find_library(Utils SHARED IMPORTED)

# Link it to libs
target_link_libraries(${project}
    PUBLIC 
        Boost::thread
        Boost::log
        DataStructures
        Utils
)

