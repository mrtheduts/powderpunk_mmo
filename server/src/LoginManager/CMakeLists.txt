#####################################################################
# Creating Login Manager library target
#####################################################################
cmake_minimum_required(VERSION 3.17)

set(project LoginManager)

project(${project})

set(LOGINMANAGER_PUBLIC_HEADERS
    include/LoginManager/login_manager.h
)

set(LOGINMANAGER_PUBLIC_SOURCE
)

set(LOGINMANAGER_PRIVATE_HEADERS
        src/login_manager.h
)

set(LOGINMANAGER_PRIVATE_SOURCE
        src/login_manager.cpp
)

add_library(${project} 
        ${LOGINMANAGER_PUBLIC_HEADERS}
        ${LOGINMANAGER_PUBLIC_SOURCE}
        ${LOGINMANAGER_PRIVATE_HEADERS}
        ${LOGINMANAGER_PRIVATE_SOURCE}
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

#####################################################################
# Building local libs 
#####################################################################
# add_subdirectory(UserPool)

#####################################################################
# Adding local libs 
#####################################################################
find_library(DataStructures SHARED IMPORTED)
find_library(Logger SHARED IMPORTED)
find_library(Utils SHARED IMPORTED)

# Link it to libs
target_link_libraries(${project}
    PRIVATE 
        Boost::fiber
        Boost::thread
        Boost::log
        DataStructures
        Logger
        Utils
)

