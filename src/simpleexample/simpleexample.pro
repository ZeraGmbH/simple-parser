QT += core gui widgets network

CONFIG += console

# for old compilers
CONFIG += c++11

# Use lib outside here
#QT += simplecmdiobase

TARGET = simpleexample
TEMPLATE = app

SOURCES += \
    main.cpp \
    cmdparserexample.cpp \
    cmdhandlerexample.cpp

HEADERS += \
    cmdparserexample.h \
    cmdhandlerexample.h

# Not necessary when using lib
INCLUDEPATH +=  ../simplecmdiobase
LIBS += -L../../lib -lQt5SimpleCmdIO

exists(../../localpaths.user.pri) {
    include(../../localpaths.user.pri)
}
