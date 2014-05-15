#-------------------------------------------------
#
# Project created by QtCreator 2014-04-06T02:09:17
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = someGuiTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    profilewindow.cpp \
    peopledialog.cpp

HEADERS  += mainwindow.h \
    profilewindow.h

FORMS    += mainwindow.ui \
    registration.ui \
    profilewindow.ui
