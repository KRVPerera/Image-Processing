######################################################################
# Automatically generated by qmake (3.0) ????? ??? 16 14:57:45 2015
######################################################################

TEMPLATE = app
TARGET = imageviewer
INCLUDEPATH += .

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += core gui xml printsupport

# Input
HEADERS += catch.hpp imageviewer.h pixel.h qcustomplot.h \
    contraststretchingdialog.h
SOURCES += imageviewer.cpp main.cpp pixel.cpp qcustomplot.cpp \
    contraststretchingdialog.cpp

FORMS += \
    contraststretchingdialog.ui
