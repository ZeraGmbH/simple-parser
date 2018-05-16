#include "cmdhandlerbase.h"

#define STR(s) _STR(s)
#define _STR(s) #s

SimpleCmdHandlerBase::SimpleCmdHandlerBase(QObject *parent) :
    QObject(parent)
{
}
