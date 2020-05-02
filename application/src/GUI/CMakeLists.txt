#####################################################################
# Creating Window library target
#####################################################################
add_library(GUI gui.cpp splitter.cpp)
# Adding properties of GUI target
target_include_directories(GUI
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/GUI
)
target_compile_options(GUI PRIVATE -Werror -Wall)
# Searches for Qt5
find_package(Qt5 COMPONENTS Widgets REQUIRED)
# Link it to Window lib
target_link_libraries(GUI
    PUBLIC
        Qt5::Widgets
)

