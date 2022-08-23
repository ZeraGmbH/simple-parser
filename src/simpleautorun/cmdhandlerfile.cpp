#include <QTimer>
#include <QEventLoop>
#include <QStringList>
#include <QFile>
#include "commonhelpers.h"
#include "cmdhandlerfile.h"
#include "cmdparserfile.h"

CmdHandlerFile::CmdHandlerFile(QObject *parent) : QSimpleCmdHandlerBase(parent)
{
    m_pCurrSocket = nullptr;
    m_bStopOnExternalError = false;
}

void CmdHandlerFile::StartCmd(SimpleCmdData *pCmd, QVariantList params)
{
    switch(pCmd->GetCmdID()) {
        case CMD_FILE_OPEN_IPPORT: {
            QTcpSocket *pNewSocket;
            pNewSocket = new QTcpSocket(this);
            pNewSocket->connectToHost(params[0].toString(), static_cast<quint16>(params[1].toUInt()));
            if(!pNewSocket->waitForConnected()) {
                delete pNewSocket;
                QString strError = QStringLiteral("IP connection %i:%2 could not be opened!").arg(params[0].toString()).arg(params[1].toInt());
                emit OperationFinish(true, strError);
            }
            else {
                LogMsg(QStringLiteral("IP connection %1:%2 was opened successfully!")
                       .arg(params[0].toString())
                       .arg(params[1].toInt()),LOG_COLOUR_GREEN);
                m_listSockets.append(pNewSocket);
                SelectSocket(pNewSocket);
                emit OperationFinish(false, "");
            }
            break;
        }
        case CMD_FILE_SELECT_IPCONNECTION: {
            int iConnectionNo = params[0].toInt();
            if(iConnectionNo < m_listSockets.count()) {
                SelectSocket(m_listSockets[iConnectionNo]);
                LogMsg(QStringLiteral("IP connection %1 was selected successfully!")
                       .arg(iConnectionNo),LOG_COLOUR_GREEN);
                emit OperationFinish(false, "");
            }
            else {
                emit OperationFinish(true, QStringLiteral("IP connection %1 was not opened!").arg(iConnectionNo));
            }
            break;
        }
        case CMD_FILE_WAIT_MS: {
            // Wait
            QTimer timer;
            timer.setSingleShot(true);
            QEventLoop loop;
            QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
            timer.start(params[0].toInt());
            loop.exec();
            emit OperationFinish(false, "");
            break;
        }
        case CMD_FILE_SET_TAG:
            // +1: Do not repeat tag set on every run
            m_MapTags[params[0].toString().toUpper()] = m_iterCurrLine+1;
            emit OperationFinish(false, "");
            break;
        case CMD_FILE_LOOP_TAG: {
            // check if target tag was set
            QString strLoopTarget = params[0].toString().toUpper();
            if(m_MapTags.contains(strLoopTarget)) {
                QStringList::iterator iterTarget = m_MapTags[strLoopTarget];
                // loop already active
                if(m_MapActiveLoops.contains(m_iterCurrLine)) {
                    // endless
                    if(m_MapActiveLoops[m_iterCurrLine] == 0) {
                        m_iterNextLine = iterTarget;
                    }
                    else {
                        m_MapActiveLoops[m_iterCurrLine]--;
                        if(m_MapActiveLoops[m_iterCurrLine] == 0) {
                            // remove loop
                            m_MapActiveLoops.remove(m_iterCurrLine);
                            LogMsg(QStringLiteral("All loops for target %1 performed")
                                   .arg(params[0].toString()));
                        }
                        else {
                            // next loop
                            m_iterNextLine = iterTarget;
                        }
                    }
                }
                // new loop
                else {
                    m_MapActiveLoops[m_iterCurrLine] = params[1].toInt();
                    m_iterNextLine = iterTarget;
                }
                // finish here
                emit OperationFinish(false, "");
            }
            else {
                QString strError = QStringLiteral("Could not find loop target \"%1\"!").arg(params[0].toString());
                emit OperationFinish(true, strError);
            }
            break;
        }
        case CMD_FILE_SET_ON_ERROR_STOP:
            m_bStopOnExternalError = params[0].toBool();
            emit OperationFinish(false, "");
            break;
        case CMD_FILE_CHECK_LAST_RESPONSE:
        case CMD_FILE_CHECK_LAST_RESPONSE_WITH_FILE:
        {
            QString strError;
            if(m_strLastReceivedExternal.isEmpty()) {
                strError = QStringLiteral("There is no last command response");
            }
            else
            {
                if(m_strLastReceivedExternal.count(',') >= 2) {
                    QStringList list = m_strLastReceivedExternal.split(',');
                    if(list[1].contains(QStringLiteral("ERROR"), Qt::CaseInsensitive)) {
                        strError = QStringLiteral("Last command returned an error");
                    }
                    else {
                        QString strExpected;
                        bool bIgnoreCase = params[1].toBool();

                        if (pCmd->GetCmdID() == CMD_FILE_CHECK_LAST_RESPONSE_WITH_FILE) {

                            QFile executeFile(params[0].toString());
                            if(executeFile.open(QIODevice::ReadOnly)) {
                                QTextStream textStream(&executeFile);
                                strExpected = textStream.readAll();
                                strExpected = strExpected.replace("\n", " ");
                            }
                            executeFile.close();
                        }
                        else {
                            strExpected = params[0].toString();
                        }

                        QString strReceived = list[2];
                        if(bIgnoreCase) {
                            strExpected = strExpected.toUpper();
                            strReceived = strReceived.toUpper();
                        }
                        if(strExpected != strReceived) {
                            strError = QStringLiteral("Last command check failed expected '%1' / received '%2'").arg(strExpected, strReceived);
                        }
                    }
                }
                else {
                    strError = QStringLiteral("Last command did not return data");
                }
            }
            if(!strError.isEmpty()) {
                // It is an internal command but stop goes with extenal error setting
                if(!m_bStopOnExternalError) {
                    emit OperationFinish(true, strError);
                }
                else {
                    LogMsg(QStringLiteral("Abort on check fail: %1")
                           .arg(strError), LOG_COLOUR_RED);
                    emit kill(-1);
                }
            }
            else {
                emit OperationFinish(false, "");
            }
            break;
        }
        case CMD_FILE_MESSAGE:
            LogMsg(QStringLiteral("%1 <Press RETURN>").arg(params[0].toString()), LOG_COLOUR_BLUE);
            while(getchar()!='\n') {}
            emit OperationFinish(false, "");
            break;
    }
}

void CmdHandlerFile::SendRemoteCmd(QString Cmd)
{
    if(m_pCurrSocket) {
        LogMsg(QStringLiteral("--> ")+Cmd);
        // Avoid drop through recent command respones
        m_strLastReceivedExternal.clear();
        m_pCurrSocket->write((Cmd + END_STR).toUtf8());
    }
    else {
        LogMsg(QStringLiteral("No connection to server established!"), LOG_COLOUR_RED);
        emit kill(-1);
    }
}

void CmdHandlerFile::SetCurrIterLine(QStringList::iterator iterCurrLine)
{
    m_iterCurrLine = iterCurrLine;
    // default next
    m_iterNextLine = ++iterCurrLine;
}

QStringList::iterator CmdHandlerFile::GetNextIterLine()
{
    return m_iterNextLine;
}

void CmdHandlerFile::SelectSocket(QTcpSocket *pSocket)
{
    if(m_pCurrSocket) {
        disconnect(m_pCurrSocket, &QTcpSocket::readyRead, this, &CmdHandlerFile::OnReceive);
    }
    m_pCurrSocket = pSocket;
    connect(m_pCurrSocket, &QTcpSocket::readyRead, this, &CmdHandlerFile::OnReceive);
    connect(m_pCurrSocket, &QTcpSocket::disconnected, this, &CmdHandlerFile::OnDisconnect);
}


void CmdHandlerFile::OnReceive()
{
    if(m_pCurrSocket) {
        //If response is longer than 32768 bytes, then it is received in multiple packets
        //so, we use first parameter as the expected length of the response in bytes

        if (m_strLastReceivedExternal.isEmpty()) {
            expectedResponseLength = m_pCurrSocket->read(sizeof(qint64)).toLongLong();
        }
        m_strLastReceivedExternal.append(m_pCurrSocket->readAll());

        if(m_strLastReceivedExternal.length() == expectedResponseLength) {
            m_strLastReceivedExternal.replace("\n", "");
            bool bError = m_strLastReceivedExternal.contains(",ERROR", Qt::CaseInsensitive);
            LogMsg(QStringLiteral("<-- ")+m_strLastReceivedExternal, bError ? LOG_COLOUR_RED : LOG_COLOUR_GREEN);
            if(!m_bStopOnExternalError || !bError) {
                emit cmdFinish();
            }
            else {
                LogMsg(QStringLiteral("Abort on external error!"), LOG_COLOUR_RED);
                emit kill(-1);
            }
            expectedResponseLength = 0;
        }
    }
}

void CmdHandlerFile::OnDisconnect()
{
    qFatal("Server closed connection unexpected!");
}
