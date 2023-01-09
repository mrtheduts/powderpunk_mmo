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
        include/Utils/DebugTools/assert_debug_print.hpp
)

# Adding properties of BasicConnection target #######################
set(BASICCONNECTION_PUBLIC_HEADER_FILES
        include/Utils/BasicConnection/basic_connection.hpp
)

# Adding properties of BasicServer target ###########################
set(BASICSERVER_PUBLIC_HEADER_FILES
        include/Utils/BasicServer/basic_server.hpp
)

# Adding properties of TSQueue target ###############################
set(TSQUEUE_PUBLIC_HEADER_FILES
    include/Utils/TSStructures/ts_queue.hpp
    include/Utils/TSStructures/ts_map.hpp
)

#####################################################################
# Creating Utils library
#####################################################################
add_library(${project}
    SHARED
        ${DEBUGTOOLS_PUBLIC_HEADER_FILES}
        ${BASICCONNECTION_PUBLIC_HEADER_FILES}
        ${BASICSERVER_PUBLIC_HEADER_FILES}
        ${TSQUEUE_PUBLIC_HEADER_FILES}
)

target_include_directories(${project}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

#####################################################################
# Adding external libs
#####################################################################
set(BOOST_ROOT ${CMAKE_SOURCE_DIR}/external/boost_install)
set(BOOST_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/external/boost_install/include)
set(BOOST_LIBRARY_DIRS ${CMAKE_SOURCE_DIR}/external/boost_install/lib)

find_package(Boost COMPONENTS thread system REQUIRED)
include_directories(${BOOST_INCLUDE_DIRS})

find_library(DataStructures SHARED IMPORTED)

# Link it to libs
target_link_libraries(${project}
    PUBLIC
        Boost::thread
        Boost::system
        DataStructures
)


target_compile_options(${project} PRIVATE -Werror -Wall -Wpedantic -Wextra)
target_compile_features(${project} PRIVATE cxx_std_14)

set_target_properties(${project} PROPERTIES LINKER_LANGUAGE CXX)

