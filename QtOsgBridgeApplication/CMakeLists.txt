begin_project(QtOsgBridgeApplication EXECUTABLE OPTIONAL)

enable_automoc()

require_library(Qt6 MODULES Core Gui Widgets OpenGL OpenGLWidgets)
require_library(OpenSceneGraph MODULES osg osgViewer osgUtil osgGA osgDB osgText OpenThreads)
require_library(osgPPU)
#require_library(OpenGL)

require_project(osgHelper PATH osgHelper)
require_project(QtOsgBridge)

add_source_directory(src)
