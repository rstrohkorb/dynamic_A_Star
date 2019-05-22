TARGET=das
# where to put the .o files
OBJECTS_DIR=obj
# core Qt Libs to use add more here if needed.
QT+=gui opengl core
# where to put moc auto generated files
MOC_DIR=moc
# on a mac we don't create a .app bundle file ( for ease of multiplatform use)
CONFIG-=app_bundle

SOURCES+=src/main.cpp \
         src/NGLScene.cpp \
         src/Graph.cpp \
         src/NGLSceneMouseControls.cpp \
         src/MainWindow.cpp \
         src/ColorTeapot.cpp

HEADERS+= include/NGLScene.h \
          include/WindowParams.h \
          include/Graph.h \
          include/MainWindow.h \
          include/ColorTeapot.h \
          include/teapot.h

OTHER_FILES+= shaders/*.glsl

FORMS+= ui/MainWindow.ui

INCLUDEPATH+= include

cache()

# were are going to default to a console app
CONFIG += console

# Following code written by Jon Macey
include($(HOME)/NGL/UseNGL.pri)
