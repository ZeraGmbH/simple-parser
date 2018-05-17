#include <QTcpSocket>
#include "cmdclient.h"
#include "cmdparserbase.h"

QSimpleCmdClient::QSimpleCmdClient(QObject *parent, QTcpSocket* pSocket, QSimpleCmdParserSocketBase *pCmdParser) :
    QObject(parent), m_pSocket(pSocket), m_pCmdParser(pCmdParser)
{
    connect(m_pSocket, &QTcpSocket::readyRead, this, &QSimpleCmdClient::OnReceive);
    connect(m_pCmdParser, &QSimpleCmdParserSocketBase::CmdFinish, this, &QSimpleCmdClient::OnCmdFinish);
}

void QSimpleCmdClient::OnReceive()
{
    QTcpSocket *pSocket = qobject_cast<QTcpSocket*>(sender());
    if(pSocket == m_pSocket)
    {
        QString ReadData = QLatin1String(m_pSocket->readAll());
        ReadData = ReadData.replace(END_IGNORE, QLatin1String(""));
        m_strCmdBuffer += ReadData;
        int iLFPos;
        while((iLFPos=m_strCmdBuffer.indexOf(END_STR)) >= 0)
        {
            // Extract one command
            QString strCmd = m_strCmdBuffer.left(iLFPos);
            m_strCmdBuffer = m_strCmdBuffer.right(m_strCmdBuffer.length() - iLFPos - 1);
            // Give it to the parser
            QString strRet = m_pCmdParser->ParseAndStartCmd(strCmd, m_pSocket);
            // Log
            bool bLog = m_pCmdParser->GetCmdLog();
            if(bLog)
                qInfo("%s/%i: %s", qPrintable(m_pCmdParser->GetParserName()), m_pCmdParser->GetListenPort(), qPrintable(strCmd));
            // Immediate response?
            if(!strRet.isEmpty())
            {
                QByteArray dataResponse = (strRet + END_STR).toLatin1();
                if(bLog)
                    qInfo("%s/%i: %s", qPrintable(m_pCmdParser->GetParserName()), m_pCmdParser->GetListenPort(), qPrintable(dataResponse));
                m_pSocket->write(dataResponse);
            }
        }
    }
}

void QSimpleCmdClient::OnCmdFinish(QString strCmdResponse, QIODevice *pCookie)
{
    if(pCookie == m_pSocket)
    {
        if(!strCmdResponse.isEmpty())
        {
            QByteArray dataResponse = (strCmdResponse + END_STR).toLatin1();
            if(m_pCmdParser->GetCmdLog())
                qInfo("%s/%i: %s", qPrintable(m_pCmdParser->GetParserName()), m_pCmdParser->GetListenPort(), qPrintable(dataResponse));
            m_pSocket->write(dataResponse);
        }
    }
}
