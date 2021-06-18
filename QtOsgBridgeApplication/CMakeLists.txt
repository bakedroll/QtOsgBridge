set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(PROJECT_NAME QtOsgBridgeApplication)

add_source_directory(QTOSGBRIDGEAPPLICATION_SOURCE src)

# add executable OnePercent
add_executable(${PROJECT_NAME}
  ${QTOSGBRIDGEAPPLICATION_SOURCE})

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")

if (WIN32)
  if(NOT BUILD_ENABLE_CONSOLE)
    set_property(TARGET ${PROJECT_NAME} PROPERTY WIN32_EXECUTABLE true)
  endif()
endif()

source_group(GeneratedFiles FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_autogen/moc_compilation.cpp)

# specify needed osg libraries
set(OSG_LIBRARIES_TMP "osg osgViewer osgDB osgGA osgUtil osgText OpenThreads")

get_libraries(OSG_LIBRARIES ${OSG_LIBRARY_RELEASE} "" ${OSG_LIBRARIES_TMP})
get_libraries(OSG_LIBRARIES_DEBUG ${OSG_LIBRARY_DEBUG} ${OSG_LIBRARY_SUFFIX} ${OSG_LIBRARIES_TMP})

# set include directories
target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${QTOSGBRIDGE_INCLUDE_DIR}
  ${OSG_INCLUDE_DIR}
  ${OSGPPU_INCLUDE_DIR}
  ${OSGHELPER_INCLUDE_DIR})

foreach(module ${QT_MODULES})
  target_include_directories(${PROJECT_NAME} PUBLIC ${Qt${QT_VERSION}${module}_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION}::${module})
endforeach()

target_link_libraries(${PROJECT_NAME} osgHelper QtOsgBridge ${OPENGL_gl_LIBRARY})

target_link_libraries(${PROJECT_NAME}
  optimized ${OSGPPU_LIBRARY_RELEASE} ${LUA_LIBRARY})
  
target_link_libraries(${PROJECT_NAME}
  debug ${OSGPPU_LIBRARY_DEBUG} ${LUA_LIB_DEBUG})

if (MSVC)
  foreach(lib ${OSG_LIBRARIES})
    target_link_libraries(${PROJECT_NAME}
      optimized ${lib})
  endforeach()

  foreach(lib ${OSG_LIBRARIES_DEBUG})
    target_link_libraries(${PROJECT_NAME}
      debug ${lib})
  endforeach()
else()
  target_link_libraries(${PROJECT_NAME} ${OPENSCENEGRAPH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
endif()

set(CMAKE_INCLUDE_CURRENT_DIR OFF)
set(CMAKE_AUTOMOC OFF)