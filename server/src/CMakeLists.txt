#####################################################################
# Creating Server Executable target
#####################################################################

cmake_minimum_required(VERSION 3.17)
project(PowderpunkServerBin)

# Generate JSON file for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output binary files in specific dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set std c++ version for the code
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED TRUE)
set (CMAKE_CXX_EXTENSIONS FALSE)

add_executable(powderpunk_server main.cpp)
# Adding properties of GUI target
target_include_directories(powderpunk_server
    PRIVATE
        ${Powderpunk_Server_SOURCE_DIR}/src
)

# Possible solution for pthread problem:
# https://stackoverflow.com/questions/34143265/undefined-reference-to-symbol-pthread-createglibc-2-2-5
# With a mix of in the target_link_libraries:
# https://stackoverflow.com/questions/50545214/adding-compiler-flags-to-cmakelists-txt
#set(CMAKE_THREAD_LIBS_INIT "-lpthread")
#set(CMAKE_HAVE_THREADS_LIBRARY 1)
#set(CMAKE_USE_WIN32_THREADS_INIT 0)
#set(CMAKE_USE_PTHREADS_INIT 1)
#set(THREADS_PREFER_PTHREAD_FLAG ON)

add_compile_options("-Werror" "-Wall" "-Wpedantic" "-Wextra")

#####################################################################
# Adding external libs 
#####################################################################

set(BOOST_ROOT ${CMAKE_SOURCE_DIR}/external/boost_install)
set(BOOST_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/external/boost_install/include)
set(BOOST_LIBRARY_DIRS ${CMAKE_SOURCE_DIR}/external/boost_install/lib)

find_package(Boost REQUIRED)
include_directories(${BOOST_INCLUDE_DIRS})

#####################################################################
# Building local libs 
#####################################################################
add_subdirectory(DebugTools)
add_subdirectory(TelnetInterface)


#####################################################################
# Adding local libs 
#####################################################################
find_library(TelnetInterface SHARED IMPORTED)
find_library(DebugTools SHARED IMPORTED)

# Link it to libs
target_link_libraries(powderpunk_server
    PRIVATE 
        pthread
        DebugTools
        TelnetInterface
)

