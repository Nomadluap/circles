include(../defaults.pri)

TARGET = circles
TEMPLATE = lib
CONFIG += shared

SOURCES += \
    graph/Edge.cpp \
    graph/Graph.cpp \
    packing/Circle.cpp \
    packing/EuclidCircle.cpp \
    packing/EuclidPacking.cpp \
    packing/HyperCircle.cpp \
    packing/Packing.cpp \
    packing/HyperPacking.cpp \
    ui/PackingView.cpp \
    packing/PackingCoordinate.cpp \
    ui/GraphicCircle.cpp

HEADERS += \
    graph/Edge.hpp \
    graph/Graph.hpp \
    packing/Circle.hpp \
    packing/EuclidCircle.hpp \
    packing/EuclidPacking.hpp \
    packing/HyperCircle.hpp \
    packing/Packing.hpp \
    packing/HyperPacking.hpp \
    ui/PackingView.hpp \
    packing/PackingCoordinate.hpp \
    graph/Triangle.hpp \
    ui/GraphicCircle.hpp

DISTFILES +=
