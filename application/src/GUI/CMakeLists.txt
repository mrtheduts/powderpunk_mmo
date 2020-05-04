#####################################################################
# Creating GUI library target
#####################################################################
add_library(GUI gui.cpp verticalbox.cpp)
# Adding properties of GUI target
target_include_directories(GUI
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/GUI
)
target_compile_options(GUI PRIVATE -Werror -Wall)
# Searches for Qt5
find_package(Qt5 COMPONENTS Widgets REQUIRED)
# Link it to Qt5 lib
target_link_libraries(GUI
    PUBLIC
        Qt5::Widgets
)

