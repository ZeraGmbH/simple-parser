INCLUDEPATH += $$PWD

PUBLIC_HEADERS += \
    $$PWD/cmdhandlerbase.h \
    $$PWD/cmdparserbase.h

SOURCES += \
    $$PWD/cmdparserbase.cpp \
    $$PWD/cmdhandlerbase.cpp

HEADERS += $$PUBLIC_HEADERS

#QMAKE_CXXFLAGS += -O0
