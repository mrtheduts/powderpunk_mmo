#####################################################################
# Creating Game Server library target
#####################################################################
cmake_minimum_required(VERSION 3.17)

set(project GameServer)

project(${project})

set(GAMESERVER_PUBLIC_HEADERS
        include/GameServer/game_server.h
)

set(GAMESERVER_PUBLIC_SOURCE
)

set(GAMESERVER_PRIVATE_HEADERS
        src/game_server.h
)

set(GAMESERVER_PRIVATE_SOURCE
        src/game_server.cpp
)

add_library(${project} 
        ${GAMESERVER_PUBLIC_HEADERS}
        ${GAMESERVER_PUBLIC_SOURCE}
        ${GAMESERVER_PRIVATE_HEADERS}
        ${GAMESERVER_PRIVATE_SOURCE}
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

find_package(Boost COMPONENTS fiber thread REQUIRED)
include_directories(${BOOST_INCLUDE_DIRS})

#####################################################################
# Building local libs 
#####################################################################
# add_subdirectory(UserPool)

#####################################################################
# Adding local libs 
#####################################################################
find_library(DataStructures SHARED IMPORTED)
find_library(Logger SHARED IMPORTED)
find_library(LoginManager SHARED IMPORTED)
find_library(TelnetInterface SHARED IMPORTED)
find_library(Utils SHARED IMPORTED)

# Link it to libs
target_link_libraries(${project}
    PRIVATE 
        Boost::fiber
        Boost::thread
        DataStructures
        LoginManager
        Logger
        TelnetInterface
        Utils
)

