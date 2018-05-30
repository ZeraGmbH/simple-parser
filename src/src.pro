TEMPLATE = subdirs

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
