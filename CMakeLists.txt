cmake_minimum_required(VERSION 3.4)

set(CMAKE_SCRIPTS_DIRECTORY "" CACHE STRING "The CMake scripts directory")
if(EXISTS ${CMAKE_SCRIPTS_DIRECTORY}/scripts.cmake)
  include(${CMAKE_SCRIPTS_DIRECTORY}/scripts.cmake)
else()
  message(FATAL_ERROR "Please specify CMAKE_SCRIPTS_DIRECTORY")
  return()
endif()

option(QT_USE_VERSION_5 "Use Qt version 5" ON)

project(QtOsgBridge)

add_subdirectory(QtOsgBridge)
add_subdirectory(QtOsgBridgeApplication)

make_projects()

if (MSVC)
  osg_copy_additional_binaries(zlib)
  qt_copy_plugins(platforms/qwindows styles/qwindowsvistastyle)
endif()
