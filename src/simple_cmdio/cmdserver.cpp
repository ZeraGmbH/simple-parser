#include "cmdserver.h"
#include "cmdparserbase.h"

SimpleCmdServer::SimpleCmdServer(QObject* parent) : QObject(parent)
{
    m_pParser = Q_NULLPTR;
    connect(&m_Server, &QTcpServer::newConnection, this, &SimpleCmdServer::OnClientNew);
}

bool SimpleCmdServer::Setup(quint16 ui16IPPort, SimpleCmdParserSocketBase *pParser)
{
    m_pParser = pParser;
    pParser->SetListenPort(ui16IPPort);
    return m_Server.listen(QHostAddress::Any, ui16IPPort);
}


void SimpleCmdServer::OnClientNew()
{
    QTcpSocket* pClientSocket = m_Server.nextPendingConnection();
    SimpleCmdClient* pClient = new SimpleCmdClient(this, pClientSocket, m_pParser);
    m_ClientHash[pClientSocket] = pClient;
    qInfo("Client connected to %s parser", qPrintable(m_pParser->GetParserName()));

    connect(pClientSocket, &QTcpSocket::disconnected, this, &SimpleCmdServer::OnClientDisconnect);
}

void SimpleCmdServer::OnClientDisconnect()
{
    QTcpSocket *pSocket = qobject_cast<QTcpSocket*>(sender());
    if(pSocket)
    {
        SimpleCmdClient* pClient = m_ClientHash.take(pSocket);
        qInfo("Client disconnected from %s parser", qPrintable(m_pParser->GetParserName()));
        delete pClient;
        pSocket->deleteLater();
    }
}

