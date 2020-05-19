#####################################################################
# Building External Projects
#####################################################################
add_subdirectory(gsl-lite EXCLUDE_FROM_ALL)

configure_file(CMakeLists.txt.in ${CMAKE_CURRENT_SOURCE_DIR}/telnetpp/CMakeLists.txt COPYONLY)
add_subdirectory(telnetpp EXCLUDE_FROM_ALL)

