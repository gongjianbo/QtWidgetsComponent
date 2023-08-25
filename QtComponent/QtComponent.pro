QT += core
QT += gui
QT += widgets

CONFIG += c++17
CONFIG += utf8_source

DEFINES += QT_DEPRECATED_WARNINGS
# disables all the APIs deprecated before Qt 6.0.0
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

TEMPLATE = lib
TARGET = QtComponent
DESTDIR = $$PWD/../bin
DEFINES += QtComponent_Library

HEADERS += \
    QtComponent.h \
    QtComponentExport.h

SOURCES += \
    QtComponent.cpp
