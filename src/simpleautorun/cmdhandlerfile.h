#ifndef CMDHANDLERFILE_H
#define CMDHANDLERFILE_H

#include "cmdhandlerbase.h"
#include <QList>
#include <QMap>
#include <QTcpSocket>

class CmdHandlerFile : public QSimpleCmdHandlerBase
{
    Q_OBJECT
public:
    explicit CmdHandlerFile(QObject *parent = 0);
    virtual void StartCmd(SimpleCmdData *pCmd, QVariantList params);
    void SendRemoteCmd(QByteArray Cmd);
    void SetCurrIterLine(QStringList::iterator iterCurrLine);
    QStringList::iterator GetNextIterLine();
signals:
    void cmdFinish();
public slots:

private:
    void SelectSocket(QTcpSocket *pSocket);
    void OnReceive();
    void OnDisconnect();

    QList<QTcpSocket*> m_listSockets;
    QTcpSocket *m_pCurrSocket;
    QStringList::iterator m_iterCurrLine;
    QStringList::iterator m_iterNextLine;
    QMap<QString, QStringList::iterator> m_MapTags;
    QMap<QStringList::iterator, int /*loop count*/> m_MapActiveLoops;
    bool m_bStopOnExternalError;
};

#endif // CMDHANDLERFILE_H
