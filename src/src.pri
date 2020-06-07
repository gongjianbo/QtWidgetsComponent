QT  += core gui widgets

CONFIG += c++11 utf8_source

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/mainwindow.cpp

HEADERS += \
    $$PWD/mainwindow.h

FORMS += \
    $$PWD/mainwindow.ui

include($$PWD/tooltip/tooltip.pri)
INCLUDEPATH+=$$PWD/tooltip
