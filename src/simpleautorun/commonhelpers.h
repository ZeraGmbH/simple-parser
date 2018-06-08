#ifndef COMMONHELPERS_H
#define COMMONHELPERS_H

#include <QString>

extern bool gbColourOutput;

typedef enum {
    LOG_COLOUR_NONE = 0,
    LOG_COLOUR_RED,
    LOG_COLOUR_GREEN,
    } t_LogColours;

void LogMsg(QString strMsg, t_LogColours colour = LOG_COLOUR_NONE);

#endif // COMMONHELPERS_H
