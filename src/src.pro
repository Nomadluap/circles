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
    ui/dualpackingview.cpp

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
    ui/dualpackingview.hpp

DISTFILES +=

FORMS += \
    ui/dualpackingview.ui
