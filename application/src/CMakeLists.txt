add_subdirectory(
    GUI
    Debugtools
)

#####################################################################
# Creating GUI Client Executable target
#####################################################################
add_executable(powderpunk_gui_client main_app_gui.cpp)
# Adding properties of GUI target
target_include_directories(powderpunk_gui_client
    PRIVATE
        ${Powderpunk_Clients_SOURCE_DIR}/src
)
target_compile_options(powderpunk_gui_client PRIVATE -Werror -Wall)
# Link it to libs
target_link_libraries(powderpunk_gui_client
    PRIVATE
        GUI
)

