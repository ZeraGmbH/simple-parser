#include "commonhelpers.h"
#include <QDateTime>
#include <QDebug>

bool gbColourOutput = false;

void LogMsg(QString strMsg, t_LogColours colour)
{
    QDateTime now = QDateTime::currentDateTime();
    QString strOut = QString("[%1]: %2")
            .arg(now.toString("yyyy-MM-dd HH:mm:ss:zzz"))
            .arg(strMsg);
    QString colourPre, colourPost;
    if(gbColourOutput)
    {
        switch(colour)
        {
        case LOG_COLOUR_NONE:
            break;
        case LOG_COLOUR_RED:
            colourPre = QString("\033[0;31m");
            colourPost = QString("\033[0;0m");
            break;
        case LOG_COLOUR_GREEN:
            colourPre = QString("\033[0;32m");
            colourPost = QString("\033[0;0m");
            break;
        }
    }
    QTextStream qout(stdout);
    qout << colourPre << strOut << colourPost << QString("\n");
}
