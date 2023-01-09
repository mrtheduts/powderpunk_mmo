#####################################################################
# Creating Game Server library target
#####################################################################
cmake_minimum_required(VERSION 3.17)

set(project Logger)

project(${project})

set(LOGGER_PUBLIC_HEADERS
    include/Logger/logger.hpp
)

set(LOGGER_PUBLIC_SOURCE
)

set(LOGGER_PRIVATE_HEADERS
        src/logger.hpp
)

set(LOGGER_PRIVATE_SOURCE
        src/logger.cpp
)

add_library(${project} 
        ${LOGGER_PUBLIC_HEADERS}
        ${LOGGER_PUBLIC_SOURCE}
        ${LOGGER_PRIVATE_HEADERS}
        ${LOGGER_PRIVATE_SOURCE}
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

find_package(Boost COMPONENTS log_setup log REQUIRED)

include_directories(${BOOST_INCLUDE_DIRS})

#####################################################################
# Adding local libs 
#####################################################################

# Link it to libs
target_link_libraries(${project}
    PRIVATE 
        Boost::log_setup
        Boost::log
)

