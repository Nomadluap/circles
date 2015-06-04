#-------------------------------------------------
#
# Project created by QtCreator 2015-06-02T11:05:30
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Circles
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Node.cpp \
    Packing.cpp \
    Circle.cpp \
    EuclideanCircle.cpp \
    PFile.cpp

HEADERS  += mainwindow.h \
    Node.hpp \
    Packing.hpp \
    Circle.hpp \
    EuclideanCircle.hpp \
    PFile.hpp

FORMS    += mainwindow.ui

DISTFILES += \
    molecule.p
