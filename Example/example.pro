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
    $$PWD/MainWindow.cpp \
    PopupDemo.cpp \
    ToolTipDemo.cpp

HEADERS += \
    $$PWD/MainWindow.h \
    PopupDemo.h \
    ToolTipDemo.h

FORMS += \
    $$PWD/MainWindow.ui \
    PopupDemo.ui \
    ToolTipDemo.ui

unix|win32: LIBS += -L$$PWD/../bin -lCuteComponent
INCLUDEPATH += $$PWD/../CuteComponent
DEPENDPATH += $$PWD/../CuteComponent

RESOURCES += \
    img.qrc
