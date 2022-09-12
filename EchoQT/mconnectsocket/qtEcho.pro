#-------------------------------------------------
#
# Project created by QtCreator 2022-07-01T10:41:30
#
#-------------------------------------------------

QT       += core
QT 	 += network
QT       -= gui

TARGET = qtEcho
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    mconnectsokcet.cpp \
    serverbase.cpp

HEADERS += \
    serverbase.h \
    mconnectsokcet.h \
    serverbase.h
