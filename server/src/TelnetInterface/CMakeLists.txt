#####################################################################
# Creating TelnetInterface library target
#####################################################################

cmake_minimum_required(VERSION 3.17)
project(telnetinterface LANGUAGES CXX)

set(TELNETINTERFACE_PUBLIC_HEADERS
        include/TelnetInterface/telnet_interface.h
)

set(TELNETINTERFACE_PUBLIC_SOURCE
)

set(TELNETINTERFACE_PRIVATE_HEADERS
        src/telnet_connection.h
        src/telnet_server.h
)

set(TELNETINTERFACE_PRIVATE_SOURCE
        src/telnet_connection.cpp
        src/telnet_server.cpp
)

add_library(TelnetInterface 
        ${TELNETINTERFACE_PUBLIC_HEADERS}
        ${TELNETINTERFACE_PUBLIC_SOURCE}
        ${TELNETINTERFACE_PRIVATE_HEADERS}
        ${TELNETINTERFACE_PRIVATE_SOURCE}
)

#####################################################################
# Adding external libs 
#####################################################################
set(BOOST_ROOT ${CMAKE_SOURCE_DIR}/external/boost_install)
set(BOOST_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/external/boost_install/include)
set(BOOST_LIBRARY_DIRS ${CMAKE_SOURCE_DIR}/external/boost_install/lib)

find_package(Boost REQUIRED)
include_directories(${BOOST_INCLUDE_DIRS})

find_library(telnetpp SHARED IMPORTED)

# Link it to libs
target_link_libraries(TelnetInterface
    PUBLIC 
		telnetpp
        Boost::boost
)

target_include_directories(TelnetInterface
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src
)

