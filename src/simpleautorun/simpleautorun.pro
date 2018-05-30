QT += core
QT += network
QT -= gui
QT += simplecmdiobase

TARGET = simpleautorun
CONFIG += console
CONFIG -= app_bundle
# for old compilers
CONFIG += c++11

TEMPLATE = app

SOURCES += \
    main.cpp \
    cmdparserfile.cpp \
    cmdhandlerfile.cpp

HEADERS += \
    cmdparserfile.h \
    cmdhandlerfile.h

target.path = /usr/bin
INSTALLS += target
