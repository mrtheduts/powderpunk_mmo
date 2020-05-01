#####################################################################
# Creating Window library target
#####################################################################
add_library(Window window.cpp)
# Adding properties of Window target
target_include_directories(Window
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Window
)
target_compile_options(Window PRIVATE -Werror -Wall)
# Searches for Qt5
find_package(Qt5 COMPONENTS Widgets REQUIRED)
# Link it to Window lib
target_link_libraries(Window
    PUBLIC
        Qt5::Widgets
)

