#ifndef SIMPLESERVERCMD_H
#define SIMPLESERVERCMD_H

#include <QTcpServer>
#include <QTcpSocket>
#include "cmdclient.h"
#include "simplecmdiobase_global.h"

class QSimpleCmdParserSocketBase;

class SIMPLE_CMDIO_EXPORT QSimpleCmdServer: public QObject
{
    Q_OBJECT
public:
    QSimpleCmdServer(QObject *parent = 0);
    bool Setup(quint16 ui16IPPort, QSimpleCmdParserSocketBase *pParser);

public slots:
    void OnClientNew();
    void OnClientDisconnect();

private:
    QTcpServer m_Server;
    QHash<QTcpSocket*, QSimpleCmdClient*> m_ClientHash;
    QSimpleCmdParserSocketBase *m_pParser;
};


#endif // SIMPLESERVERCMD_H
