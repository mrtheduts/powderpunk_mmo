#####################################################################
# Creating Debug library target
#####################################################################
add_library(Debugtools INTERFACE)
# Adding properties of Debug target
target_include_directories(Debugtools
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Debugtools
)

