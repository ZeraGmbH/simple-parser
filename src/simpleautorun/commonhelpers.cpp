#include "commonhelpers.h"
#include <QDateTime>
#include <QDebug>

void LogMsg(QString strMsg)
{
    QDateTime now = QDateTime::currentDateTime();
    QString strOut = QString("[%1]: %2")
            .arg(now.toString("yyyy-MM-dd HH:mm:ss:zzz"))
            .arg(strMsg);
    qInfo(qPrintable(strOut));
}
