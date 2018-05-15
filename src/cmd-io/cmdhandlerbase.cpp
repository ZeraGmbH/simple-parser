#include "cmdhandlerbase.h"

#define STR(s) _STR(s)
#define _STR(s) #s

CmdHandlerBase::CmdHandlerBase(QObject *parent) : QObject(parent)
{
}
