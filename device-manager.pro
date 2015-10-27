#-------------------------------------------------
#
# Project created by QtCreator 2015-10-23T08:38:15
#
#-------------------------------------------------

QT       += core gui

CONFIG   += c++11

win32:LIBS += -luser32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = device-manager
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    enum-devices/device.cpp \
    enum-devices/devicegroup.cpp \
    enum-devices/util.cpp \
    devicemanagercontroller.cpp \
    enum-devices/deviceenumerator.cpp \
    collapse-widget/collapselistwidget.cpp \
    collapse-widget/collapsewidget.cpp

HEADERS  += mainwindow.h \
    enum-devices/device.h \
    enum-devices/util.h \
    enum-devices/devicegroup.h \
    devicemanagercontroller.h \
    enum-devices/deviceenumerator.h \
    collapse-widget/collapselistwidget.h \
    collapse-widget/collapsewidget.h

RESOURCES += \
    res/resource.qrc

DISTFILES += \
    res/stylesheet.qss
