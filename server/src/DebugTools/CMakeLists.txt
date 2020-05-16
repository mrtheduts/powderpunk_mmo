#####################################################################
# Creating Debug library target
#####################################################################
add_library(DebugTools INTERFACE)
# Adding properties of Debug target
target_include_directories(DebugTools
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/DebugTools
)

