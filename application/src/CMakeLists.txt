add_subdirectory(Window)

#####################################################################
# Creating Executable target 
#####################################################################
add_executable(Application main.cpp)
# Adding properties of Application target
target_include_directories(Application
    PRIVATE
        ${pwdpnkmmoapp_SOURCE_DIR}/src
)
target_compile_options(Application PRIVATE -Werror -Wall)
# Link libs to it
target_link_libraries(Application
    PRIVATE
        Window
)
