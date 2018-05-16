#ifndef SIMPLESERVERCMD_H
#define SIMPLESERVERCMD_H

#include <QTcpServer>
#include <QTcpSocket>
#include "cmdclient.h"

class SimpleCmdParserSocketBase;

class SimpleCmdServer: public QObject
{
    Q_OBJECT
public:
    SimpleCmdServer(QObject *parent = 0);
    bool Setup(quint16 ui16IPPort, SimpleCmdParserSocketBase *pParser);

public slots:
    void OnClientNew();
    void OnClientDisconnect();

private:
    QTcpServer m_Server;
    QHash<QTcpSocket*, SimpleCmdClient*> m_ClientHash;
    SimpleCmdParserSocketBase *m_pParser;
};


#endif // SIMPLESERVERCMD_H
