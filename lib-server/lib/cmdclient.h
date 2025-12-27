#ifndef SIMPLECMDCLIENT_H
#define SIMPLECMDCLIENT_H

#include "cmdparserbase.h"
#include <QObject>
#include <QTcpSocket>
#include <QIODevice>

class QSimpleCmdClient: public QObject
{
    Q_OBJECT
public:
    QSimpleCmdClient(QObject *parent, QTcpSocket* pSocket, QSimpleCmdParserSocketBase* pCmdParser);

public slots:
    void OnReceive();
    void OnCmdFinish(QString strCmdResponse, QIODevice *pCookie);

private:
    QTcpSocket* m_pSocket;
    QSimpleCmdParserSocketBase* m_pCmdParser;
    QString m_strCmdBuffer;
};

#endif // SIMPLECMDCLIENT_H
