#-------------------------------------------------
#
# Project created by QtCreator 2014-12-19T13:10:39
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_NuFitsFileTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_NuFitsFileTest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

include(../test.pri)
