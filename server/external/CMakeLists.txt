#####################################################################
# Building External Projects
#####################################################################
#add_subdirectory(boost EXCLUDE_FROM_ALL)
#add_library(Boost SHARED IMPORTED)
#set_target_properties(Boost
    #PROPERTIES
        #IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/boost/stage/lib"
#)
#target_include_directories(Boost
    #INTERFACE
        #$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/boost>  
        #$<INSTALL_INTERFACE:boost>
#)

set(BOOST_ROOT ${CMAKE_CURRENT_LIST_DIR}/boost_install)
set(BOOST_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/boost_install/include)
set(BOOST_LIBRARY_DIRS ${CMAKE_CURRENT_LIST_DIR}/boost_install/lib)

add_subdirectory(gsl-lite)

configure_file(CMakeLists.txt.in ${CMAKE_CURRENT_SOURCE_DIR}/telnetpp/CMakeLists.txt COPYONLY)
add_subdirectory(telnetpp)

