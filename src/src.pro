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
    packing/PackingCoordinate.cpp \
    view/GraphicCircle.cpp \
    view/PackingView.cpp \
    gui/dualpackingview.cpp \
    view/HyperColor.cpp \
    gui/mainwindow.cpp \
    graph/Triangle.cpp

HEADERS += \
    graph/Edge.hpp \
    graph/Graph.hpp \
    packing/Circle.hpp \
    packing/EuclidCircle.hpp \
    packing/EuclidPacking.hpp \
    packing/HyperCircle.hpp \
    packing/Packing.hpp \
    packing/HyperPacking.hpp \
    packing/PackingCoordinate.hpp \
    graph/Triangle.hpp \
    view/GraphicCircle.hpp \
    view/PackingView.hpp \
    gui/dualpackingview.hpp \
    graph/Node.hpp \
    view/HyperColor.hpp \
    gui/mainwindow.hpp

DISTFILES +=

FORMS += \
    gui/dualpackingview.ui \
    gui/mainwindow.ui
