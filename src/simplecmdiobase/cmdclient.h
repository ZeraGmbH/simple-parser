#ifndef SIMPLECMDCLIENT_H
#define SIMPLECMDCLIENT_H

#include <QObject>
#include "simplecmdiobase_global.h"

class QTcpSocket;
class QSimpleCmdParserSocketBase;
class QIODevice;

class SIMPLE_CMDIO_EXPORT QSimpleCmdClient: public QObject
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
