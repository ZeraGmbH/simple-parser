#ifndef SIMPLECMDCLIENT_H
#define SIMPLECMDCLIENT_H

#include <QObject>

class QTcpSocket;
class SimpleCmdParserSocketBase;
class QIODevice;

class SimpleCmdClient: public QObject
{
    Q_OBJECT
public:
    SimpleCmdClient(QObject *parent, QTcpSocket* pSocket, SimpleCmdParserSocketBase* pCmdParser);

public slots:
    void OnReceive();
    void OnCmdFinish(QString strCmdResponse, QIODevice *pCookie);

private:
    QTcpSocket* m_pSocket;
    SimpleCmdParserSocketBase* m_pCmdParser;
    QString m_strCmdBuffer;
};

#endif // SIMPLECMDCLIENT_H
