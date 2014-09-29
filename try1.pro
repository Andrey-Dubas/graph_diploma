#-------------------------------------------------
#
# Project created by QtCreator 2014-09-10T16:09:37
#
#-------------------------------------------------

QT       += core gui opengl

CONFIG += qt warn_on qt3d

TARGET = try1
TEMPLATE = app


SOURCES += main.cpp\
        window.cpp \
    glwidget.cpp

HEADERS  += window.h \
    glwidget.h

FORMS    += window.ui

RESOURCES += \
    try1.qrc
