#-------------------------------------------------
#
# Project created by QtCreator 2018-05-15T11:31:55
#
#-------------------------------------------------


QT       += core gui network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test02
TEMPLATE = app

SOURCES += main.cpp\
        telemetryprinter.cpp

HEADERS  += telemetryprinter.h \
    standarttm.h

FORMS    += telemetryprinter.ui
