#####################################################################
# Building External Projects
#####################################################################
add_subdirectory(gsl-lite EXCLUDE_FROM_ALL)
configure_file(CMakeLists.txt.in ${CMAKE_CURRENT_SOURCE_DIR}/telnetpp/CMakeLists.txt COPYONLY)
add_subdirectory(telnetpp EXCLUDE_FROM_ALL)

#execute_process(COMMAND ${CMAKE_COMMAND} 
                    #-G "${CMAKE_GENERATOR}"
                    #-B ${CMAKE_CURRENT_BINARY_DIR}/gsl-lite-build/
                #RESULT_VARIABLE result
                #WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/gsl-lite
               #)

#if(result)
    #message(FATAL_ERROR "CMake step for gsl-lite failed: ${result}")
#endif()


#execute_process(COMMAND ${CMAKE_COMMAND}
                    #-G "${CMAKE_GENERATOR}"
                    #-B${CMAKE_CURRENT_BINARY_DIR}/telnetpp-build/
                #RESULT_VARIABLE result
                #WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/telnetpp
               #)
#if(result)
    #message(FATAL_ERROR "CMake step for telnetpp failed: ${result}")
#endif()

#execute_process(COMMAND ${CMAKE_COMMAND}
                    #-G "${CMAKE_GENERATOR}"
                    #-v
                    #-DCMAKE_BUILD_TYPE=Release
                    #-Dgsl-lite_DIR:PATH=${CMAKE_CURRENT_SOURCE_DIR}/gsl-lite/build
                    #--build .
                #RESULT_VARIABLE result
                #WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/telnetpp-build
               #)
#if(result)
  #message(FATAL_ERROR "CMake build for telnetpp failed: ${result}")
#endif()







#include(FetchContent)
#find_package(Git REQUIRED)

#set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external)

#####################################################################
# Creating gsl-lite library 
#####################################################################

#FetchContent_Declare(
    #gsl-lite
    #GIT_REPOSITORY    https://github.com/gsl-lite/gsl-lite.git
    #GIT_TAG           v0.36.0
    #TIMEOUT           10
    #SOURCE_DIR        ""
    #BINARY_DIR        ""
    #UPDATE_COMMAND    ""
    #CONFIGURE_COMMAND ""
    #BUILD_COMMAND     ""
    #INSTALL_COMMAND   ""
    #TEST_COMMAND      ""
    #LOG_DOWNLOAD      ON
#)
## Provide #include access to gsl-lite as <gsl/gsl-lite.hpp>: 
#FetchContent_GetProperties(gsl-lite)
#set(GSL_LITE_INCLUDE_DIR ${gsl-lite_SOURCE_DIR}/include CACHE INTERNAL "Include folder for gsl-lite")

#add_library(gsl INTERFACE)
#target_include_directories(gsl INTERFACE ${GSL_LITE_INCLUDE_DIR})

#configure_file(gsl-lite-download/CMakeLists.txt.in gsl-lite-download/CMakeLists.txt)

#execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  #RESULT_VARIABLE result
  #WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gsl-lite-download )
#if(result)
  #message(FATAL_ERROR "CMake step for gsl-lite-download failed: ${result}")
#endif()
#execute_process(COMMAND ${CMAKE_COMMAND} --build .
  #RESULT_VARIABLE result
  #WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gsl-lite-download )
#if(result)
  #message(FATAL_ERROR "Build step for gsl-lite-download failed: ${result}")
#endif()

#message(STATUS "Building gsl-lite-src")
#execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" ../gsl-lite-src
  #RESULT_VARIABLE result
  #WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gsl-lite-build )
#if(result)
  #message(FATAL_ERROR "CMake step for gsl-lite failed: ${result}")
#endif()
#execute_process(COMMAND ${CMAKE_COMMAND} --build .
  #RESULT_VARIABLE result
  #WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gsl-lite-build )
#if(result)
  #message(FATAL_ERROR "Build step for gsl-lite failed: ${result}")
#endif()

# Provide #include access to gsl-lite as <gsl/gsl-lite.hpp>: 
#ExternalProject_Get_Property( gsl_lite SOURCE_DIR )
#set( GSL_LITE_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/gsl-lite-src/include CACHE INTERNAL "Include folder for gsl-lite" )

#message(STATUS "GSL_LITE_INCLUDE_DIR: ${GSL_LITE_INCLUDE_DIR}")
#add_library( gsl-lite INTERFACE )
#target_include_directories( gsl-lite INTERFACE ${GSL_LITE_INCLUDE_DIR} )
### Prevent overriding the parent project's compiler/linker
## settings on Windows
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

## Add googletest directly to our build. This defines
## the gtest and gtest_main targets.
#add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 #${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 #EXCLUDE_FROM_ALL)

#add_library(gsl-lite INTERFACE)
#target_include_directories(gsl-lite INTERFACE ${GSL_LITE_INCLUDE_DIR})
#export(PACKAGE gsl-lite)

#####################################################################
# Creating googletest library
#####################################################################
# Download and unpack googletest at configure time
#configure_file(googletest-download/CMakeLists.txt.in googletest-download/CMakeLists.txt)
#execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  #RESULT_VARIABLE result
  #WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
#if(result)
  #message(FATAL_ERROR "CMake step for googletest failed: ${result}")
#endif()
#execute_process(COMMAND ${CMAKE_COMMAND} --build .
  #RESULT_VARIABLE result
  #WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
#if(result)
  #message(FATAL_ERROR "Build step for googletest failed: ${result}")
#endif()

## Prevent overriding the parent project's compiler/linker
## settings on Windows
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

## Add googletest directly to our build. This defines
## the gtest and gtest_main targets.
#add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 #${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 #EXCLUDE_FROM_ALL)

## The gtest/gtest_main targets carry header search path
## dependencies automatically when using CMake 2.8.11 or
## later. Otherwise we have to add them here ourselves.
#if (CMAKE_VERSION VERSION_LESS 2.8.11)
  #include_directories("${gtest_SOURCE_DIR}/include")
#endif()

#####################################################################
# Creating telnetpp library 
#####################################################################
# Download and unpack telnetpp at configure time
#configure_file(telnetpp-download/CMakeLists.txt.in telnetpp-download/CMakeLists.txt)

#execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  #RESULT_VARIABLE result
  #WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/telnetpp-download)
#if(result)
  #message(FATAL_ERROR "CMake step for telnetpp failed: ${result}")
#endif()
#execute_process(COMMAND ${CMAKE_COMMAND} --build .
  #RESULT_VARIABLE result
  #WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/telnetpp-download)
#if(result)
  #message(FATAL_ERROR "Build step for telnetpp failed: ${result}")
#endif()

## Prevent overriding the parent project's compiler/linker
## settings on Windows
#set(telnetpp_force_shared_crt ON CACHE BOOL "" FORCE)

#configure_file(${PROJECT_SOURCE_DIR}/external/telnetpp-download/telnetpp/CMakeLists.txt.in ${CMAKE_CURRENT_BINARY_DIR}/telnetpp-src/CMakeLists.txt)

#execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" -v -DCMAKE_BUILD_TYPE=Release -Dgsl-lite_DIR:PATH=${CMAKE_CURRENT_BINARY_DIR}/gsl-lite-build ${CMAKE_CURRENT_BINARY_DIR}/telnetpp-src
  #RESULT_VARIABLE result
  #WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/telnetpp-build)
#if(result)
  #message(FATAL_ERROR "CMake step for telnetpp failed: ${result}")
#endif()
#execute_process(COMMAND ${CMAKE_COMMAND} --build .
  #RESULT_VARIABLE result
  #WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/telnetpp-build)
#if(result)
  #message(FATAL_ERROR "Build step for telnetpp failed: ${result}")
#endif()

