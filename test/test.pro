TARGET=test
SOURCES+= main.cpp \
          ../das/src/Graph.cpp \
          ../das/src/ColorTeapot.cpp
#          ../clothSim/src/Cloth.cpp \

LIBS+= -lgtest
INCLUDEPATH+= ../das/include

# Following code written by Jon Macey
include($$(HOME)/NGL/UseNGL.pri)
