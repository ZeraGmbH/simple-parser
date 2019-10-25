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
    if(pSocket == m_pSocket) {
        QString ReadData = QString::fromUtf8(m_pSocket->readAll());
        ReadData = ReadData.replace(END_IGNORE, QStringLiteral());
        m_strCmdBuffer += ReadData;
        int iLFPos;
        while((iLFPos=m_strCmdBuffer.indexOf(END_STR)) >= 0) {
            // Extract one command
            QString strCmd = m_strCmdBuffer.left(iLFPos);
            m_strCmdBuffer = m_strCmdBuffer.right(m_strCmdBuffer.length() - iLFPos - 1);
            // Log
            bool bLog = m_pCmdParser->GetCmdLog();
            if(bLog) {
                qInfo("%s/%i: %s", qPrintable(m_pCmdParser->GetParserName()), m_pCmdParser->GetListenPort(), qPrintable(strCmd));
            }
            // Give it to the parser
            QString strRet = m_pCmdParser->ParseAndStartCmd(strCmd, m_pSocket);
            // Immediate response?
            if(!strRet.isEmpty()) {
                QString dataResponse = strRet + END_STR;
                if(bLog) {
                    qInfo("%s/%i: %s", qPrintable(m_pCmdParser->GetParserName()), m_pCmdParser->GetListenPort(), qPrintable(dataResponse));
                }
                m_pSocket->write(dataResponse.toUtf8());
            }
        }
    }
}

void QSimpleCmdClient::OnCmdFinish(QString strCmdResponse, QIODevice *pCookie)
{
    if(pCookie == m_pSocket) {
        if(!strCmdResponse.isEmpty()) {
            QString dataResponse = strCmdResponse + END_STR;
            if(m_pCmdParser->GetCmdLog()) {
                qInfo("%s/%i: %s", qPrintable(m_pCmdParser->GetParserName()), m_pCmdParser->GetListenPort(), qPrintable(dataResponse));
            }
            m_pSocket->write(dataResponse.toUtf8());
        }
    }
}
