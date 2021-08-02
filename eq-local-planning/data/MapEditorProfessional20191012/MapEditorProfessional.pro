#-------------------------------------------------
#
# Project created by QtCreator 2018-05-15T16:14:09
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MapEditorProfessional
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    commonUsed/database.cpp \
    commonUsed/hserialport.cpp \
    commonUsed/pubfunc.cpp \
    commonUsed/qdisplaywidget1.cpp \
    commonUsed/qdisplaywidget2.cpp \
    commonUsed/tdinsdecode.cpp \
    ins_novatel_pp7d_decode.cpp \
    geographicLib_source/Geocentric.cpp \
    geographicLib_source/Geodesic.cpp \
    geographicLib_source/GeodesicLine.cpp \
    geographicLib_source/LocalCartesian.cpp \
    geographicLib_source/Math.cpp

HEADERS += \
        mainwindow.h \
    commonUsed/database.h \
    commonUsed/GL8_structures.h \
    commonUsed/hserialport.h \
    commonUsed/pubclass.h \
    commonUsed/pubfunc.h \
    commonUsed/qdisplaywidget1.h \
    commonUsed/qdisplaywidget2.h \
    commonUsed/tdinsdecode.h \
    commonUsed/typedefs.h \
    ins_novatel_pp7d_decode.h \
    geographicLib_header/Config.h \
    geographicLib_header/Constants.hpp \
    geographicLib_header/Geocentric.hpp \
    geographicLib_header/Geodesic.hpp \
    geographicLib_header/GeodesicLine.hpp \
    geographicLib_header/LocalCartesian.hpp \
    geographicLib_header/Math.hpp

FORMS += \
        mainwindow.ui
