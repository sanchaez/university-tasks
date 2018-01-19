#-------------------------------------------------
#
# Project created by QtCreator 2016-10-23T02:44:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    CONFIG += c++11
} else {
    QMAKE_CXXFLAGS += -std=c++11
}

TARGET = lab1_cg
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    alghoritmscollection.cpp

HEADERS  += mainwindow.h \
    alghoritmscollection.h

FORMS    += mainwindow.ui
