#-------------------------------------------------
#
# Project created by QtCreator 2014-01-14T20:37:46
#
#-------------------------------------------------

QT       += core gui concurrent

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bezier
TEMPLATE = app


SOURCES += main.cpp\
    controlitem.cpp \
    graphicsbezieritem.cpp \
    mainwindow.cpp

HEADERS  += \
    graphicsbezieritem.h \
    mainwindow.h

FORMS    += \
    mainwindow.ui
