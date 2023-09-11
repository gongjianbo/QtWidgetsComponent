QT += core
QT += gui
QT += widgets

CONFIG += c++17
CONFIG += utf8_source

DEFINES += QT_DEPRECATED_WARNINGS
# disables all the APIs deprecated before Qt 6.0.0
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

TEMPLATE = lib
TARGET = CuteComponent
DESTDIR = $$PWD/../bin
DEFINES += CuteComponent_Library

HEADERS += \
    CuteComponent.h \
    CuteComponentExport.h

SOURCES += \
    CuteComponent.cpp

include($$PWD/ToolTip/ToolTip.pri)
INCLUDEPATH += $$PWD/ToolTip

include($$PWD/Popup/Popup.pri)
INCLUDEPATH += $$PWD/Popup
