add_subdirectory(GUI)

#####################################################################
# Creating Executable target
#####################################################################
add_executable(Application_GUI main.cpp)
# Adding properties of GUI target
target_include_directories(Application_GUI
    PRIVATE
    ${Powderpunk_GUI_Client_SOURCE_DIR}/src
)
target_compile_options(Application_GUI PRIVATE -Werror -Wall)
# Link it to  GUI lib
target_link_libraries(Application_GUI
    PRIVATE
        GUI
)
