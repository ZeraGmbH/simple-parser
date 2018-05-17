#include "cmdhandlerbase.h"

#define STR(s) _STR(s)
#define _STR(s) #s

QSimpleCmdHandlerBase::QSimpleCmdHandlerBase(QObject *parent) :
    QObject(parent)
{
}
