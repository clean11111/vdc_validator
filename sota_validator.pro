#-------------------------------------------------
#
# Project created by QtCreator 2018-08-14T15:08:35
#
#-------------------------------------------------

QT       += core gui widgets websockets network
CONFIG += c++11
TARGET = sota_validator
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        jsoncpp.cpp\
        main.cpp \
        mainwindow.cpp

HEADERS += \
        json.h \
        mainwindow.h

OTHER_FILES += json/*

DISTFILES += \
    json/hmi_info_IP20.json \
    json/hmi_info_array.json
