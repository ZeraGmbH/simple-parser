TARGET = QtSimpleCmdIO
QT = core network

# for old compilers
CONFIG += c++11

load(qt_module)

include($$PWD/simplecmdiobase-lib.pri)

exists(../../localpaths.user.pri) {
    include(../../localpaths.user.pri)
}
