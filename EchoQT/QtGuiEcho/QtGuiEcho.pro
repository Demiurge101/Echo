#-------------------------------------------------
#
# Project created by QtCreator 2022-09-14T09:51:53
#
#-------------------------------------------------

QT       += core gui
QT	 += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtGuiEcho
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mconnectsocket.cpp \
    serverbase.cpp \
    windowconnect.cpp

HEADERS  += mainwindow.h \
    mconnectsocket.h \
    serverbase.h \
    windowconnect.h

FORMS    += mainwindow.ui \
    windowconnect.ui
