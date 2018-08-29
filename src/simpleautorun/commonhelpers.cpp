#include "commonhelpers.h"
#include <QDateTime>
#include <QDebug>

bool gbColourOutput = false;

void LogMsg(QString strMsg, t_LogColours colour)
{
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
        case LOG_COLOUR_BLUE:
            colourPre = QString("\033[0;34m");
            colourPost = QString("\033[0;0m");
            break;
        }
    }
    QDateTime now = QDateTime::currentDateTime();
    QString strOut = QString("[%1]: %2%3%4")
            .arg(now.toString("yyyy-MM-dd HH:mm:ss:zzz"))
            .arg(colourPre)
            .arg(strMsg)
            .arg(colourPost);

    QTextStream qout(stdout);
    qout << strOut << QString("\n");
}
