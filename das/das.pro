TARGET=das
SOURCES+=src/main.cpp \
         src/NGLScene.cpp \
         src/Graph.cpp

HEADERS+= include/NGLScene.h \
          include/WindowParams.h \
          include/Graph.h

OTHER_FILES+= shaders/*.glsl

INCLUDEPATH+= include

cache()

# Following code written by Jon Macey
include($(HOME)/NGL/UseNGL.pri)
