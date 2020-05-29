TEMPLATE = subdirs

# for old compilers
CONFIG += c++11

SUBDIRS = \
    simplecmdiobase \
    simpleautorun \
    simpleexample

simpleautorun.depends = simplecmdiobase
simpleexample.depends = simplecmdiobase

disableExample {
    message(Buld example disabled)
    SUBDIRS -= simpleexample
}
