set(PROJECT_NAME QtOsgBridge)

add_source_directory(QTOSGBRIDGE_SOURCE src)
add_source_directory(QTOSGBRIDGE_SOURCE include)

set(QTOSGBRIDGE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME} ${QTOSGBRIDGE_SOURCE})

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")
target_compile_options(${PROJECT_NAME} PRIVATE "-fpermissive")

# set include directories
target_include_directories(${PROJECT_NAME} PUBLIC
  ${QTOSGBRIDGE_INCLUDE_DIR}
  ${OSG_INCLUDE_DIR}
  ${OSGPPU_INCLUDE_DIR})
  
# make variable globally accessable
set(QTOSGBRIDGE_INCLUDE_DIR ${QTOSGBRIDGE_INCLUDE_DIR} PARENT_SCOPE)