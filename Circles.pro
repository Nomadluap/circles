#-------------------------------------------------
#
# Project created by QtCreator 2015-06-02T11:05:30
#
#-------------------------------------------------

QT       += core gui widgets opengl
QMAKE_CXXFLAGS += -std=c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Circles
TEMPLATE = app

SOURCES += main.cpp\
    Node.cpp \
    graphics/Packing.cpp \
    graphics/Circle.cpp \
    PFile.cpp \
    graphics/Connector.cpp \
    graphics/SelectionPacking.cpp \
    graphics/Boundary.cpp \
    ui/PackingView.cpp \
    ui/PropertyWindow.cpp \
    ui/ShapeSelector.cpp \
    graphics/ShapeSelectorGraphicsView.cpp \
    graphics/SelectionVertex.cpp \
    ui/MainWindow.cpp

HEADERS  += \
    Node.hpp \
    graphics/Packing.hpp \
    graphics/Circle.hpp \
    PFile.hpp \
    graphics/Connector.hpp \
    graphics/SelectionPacking.hpp \
    graphics/Boundary.hpp \
    ui/PackingView.hpp \
    ui/PropertyWindow.hpp \
    ui/ShapeSelector.hpp \
    graphics/ShapeSelectorGraphicsView.hpp \
    graphics/SelectionVertex.hpp \
    ui/MainWindow.hpp

FORMS    += \
    ui/PackingView.ui \
    ui/PropertyWindow.ui \
    ui/ShapeSelector.ui \
    ui/MainWindow.ui


