INCLUDEPATH += $$PWD

PUBLIC_HEADERS += \
    $$PWD/simple_cmdio_global.h \
    $$PWD/cmdhandlerbase.h \
    $$PWD/cmdparserbase.h \
    $$PWD/cmdclient.h \
    $$PWD/cmdserver.h \

SOURCES += \
    $$PWD/cmdparserbase.cpp \
    $$PWD/cmdhandlerbase.cpp \
    $$PWD/cmdclient.cpp \
    $$PWD/cmdserver.cpp

HEADERS += $$PUBLIC_HEADERS
