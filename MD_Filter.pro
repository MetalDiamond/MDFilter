#-------------------------------------------------
#
# Project created by QtCreator 2015-09-29T14:11:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MD_Filter
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    ker.cpp \
    pgmp5.cpp \
    abstractimage.cpp \
    slidedialog.cpp

HEADERS  += mainwindow.h \
    ker.h \
    pgmp5.h \
    abstractimage.h \
    slidedialog.h

FORMS    += mainwindow.ui \
    slidedialog.ui
