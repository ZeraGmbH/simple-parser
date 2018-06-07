QT += core
QT += network
QT -= gui

TARGET = simpleautorun
CONFIG += console
CONFIG -= app_bundle
# for old compilers
CONFIG += c++11

TEMPLATE = app

SOURCES += \
    main.cpp \
    cmdparserfile.cpp \
    cmdhandlerfile.cpp \
    commonhelpers.cpp

HEADERS += \
    cmdparserfile.h \
    cmdhandlerfile.h \
    commonhelpers.h

target.path = /usr/bin
INSTALLS += target

INCLUDEPATH +=  ../simplecmdiobase
LIBS += -L../../lib -lQt5SimpleCmdIO

exists(../../localpaths.user.pri) {
    include(../../localpaths.user.pri)
}
