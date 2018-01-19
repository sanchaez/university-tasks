#-------------------------------------------------
#
# Project created by QtCreator 2016-12-12T16:01:49
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SimplePaint
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    scribblearea.cpp \
    scribblescrollarea.cpp

HEADERS  += mainwindow.h \
    scribblearea.h \
    scribblescrollarea.h

FORMS    += mainwindow.ui

RESOURCES += \
    icons.qrc
