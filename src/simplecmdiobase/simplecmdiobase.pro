TARGET = QtSimpleCmdIO
QT = core network

load(qt_module)

include($$PWD/simplecmdiobase-lib.pri)

exists(../../localpaths.user.pri) {
    include(../../localpaths.user.pri)
}
