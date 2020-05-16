add_subdirectory(
    DebugTools
)

#####################################################################
# Creating Server Executable target
#####################################################################
add_executable(powderpunk_server main.cpp)
# Adding properties of GUI target
target_include_directories(powderpunk_server
    PRIVATE
    ${Powderpunk_Server_SOURCE_DIR}/src
)
target_compile_options(powderpunk_server PRIVATE -Werror -Wall -Wpedantic -Wextra)
# Link it to libs
target_link_libraries(powderpunk_server
    PRIVATE
        DebugTools
)

