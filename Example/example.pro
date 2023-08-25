QT += core
QT += gui
QT += widgets

CONFIG += c++17
CONFIG += utf8_source

DEFINES += QT_DEPRECATED_WARNINGS
# disables all the APIs deprecated before Qt 6.0.0
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

TEMPLATE = app
TARGET = ComponentExample
DESTDIR = $$PWD/../bin

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/MainWindow.cpp

HEADERS += \
    $$PWD/MainWindow.h

FORMS += \
    $$PWD/MainWindow.ui

unix|win32: LIBS += -L$$PWD/../bin -lQtComponent
INCLUDEPATH += $$PWD/../QtComponent
DEPENDPATH += $$PWD/../QtComponent
