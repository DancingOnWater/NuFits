#-------------------------------------------------
#
# Project created by QtCreator 2014-12-15T13:49:01
#
#-------------------------------------------------


QMAKE_CXXFLAGS += -std=gnu++14

QT       -= core gui

TARGET = cpp
TEMPLATE = lib

DEFINES += CPP_LIBRARY

SOURCES += \
    NuFitsFile.cpp \
    NufFitsHeadRecord.cpp \
    NuFitsDescriptor.cpp

HEADERS +=\
    NuFitsFile.h \
    NuFitsLiteralPrivate.h \
    NufFitsHeadRecord.h \
    NuFitsDescriptor.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
