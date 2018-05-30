#include <QTimer>
#include <QEventLoop>
#include <QStringList>
#include "cmdhandlerfile.h"
#include "cmdparserfile.h"

CmdHandlerFile::CmdHandlerFile(QObject *parent) : QSimpleCmdHandlerBase(parent)
{
    m_pCurrSocket = NULL;
    m_bStopOnExternalError = false;
}

void CmdHandlerFile::StartCmd(SimpleCmdData *pCmd, QVariantList params)
{
    switch(pCmd->GetCmdID())
    {
        case CMD_FILE_OPEN_IPPORT:
        {
            QTcpSocket *pNewSocket;
            pNewSocket = new QTcpSocket(this);
            pNewSocket->connectToHost(params[0].toString(), params[1].toInt());
            if(!pNewSocket->waitForConnected())
            {
                delete pNewSocket;
                QString strError;
                strError.sprintf("IP connection %s:%i could not be opened!", qPrintable(params[0].toString()), params[1].toInt());
                emit OperationFinish(true, strError);
            }
            else
            {
                qInfo("IP connection %s:%i was opened successfully!", qPrintable(params[0].toString()), params[1].toInt());
                m_listSockets.append(pNewSocket);
                SelectSocket(pNewSocket);
                emit OperationFinish(false, "");
            }
            break;
        }
        case CMD_FILE_SELECT_IPCONNECTION:
        {
            int iConnectionNo = params[0].toInt();
            if(iConnectionNo < m_listSockets.count())
            {
                SelectSocket(m_listSockets[iConnectionNo]);
                qInfo("IP connection %i was selected successfully!", iConnectionNo);
                emit OperationFinish(false, "");
            }
            else
            {
                QString strError;
                strError.sprintf("IP connection %i was not opened!", iConnectionNo);
                emit OperationFinish(true, strError);
            }
            break;
        }
        case CMD_FILE_WAIT_MS:
        {
            // Wait
            QTimer timer;
            timer.setSingleShot(true);
            QEventLoop loop;
            QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
            timer.start(params[0].toInt());
            loop.exec();
            //qInfo("Waited %ims", params[0].toInt());
            emit OperationFinish(false, "");
            break;
        }
        case CMD_FILE_SET_TAG:
            m_MapTags[params[0].toString().toUpper()] = m_iterCurrLine;
            emit OperationFinish(false, "");
            break;
        case CMD_FILE_LOOP_TAG:
        {
            // check if target tag was set
            QString strLoopTarget = params[0].toString().toUpper();
            if(m_MapTags.contains(strLoopTarget))
            {
                QStringList::iterator iterTarget = m_MapTags[strLoopTarget];
                // loop already active
                if(m_MapActiveLoops.contains(m_iterCurrLine))
                {
                    // endless
                    if(m_MapActiveLoops[m_iterCurrLine] == 0)
                        m_iterNextLine = iterTarget;
                    else
                    {
                        m_MapActiveLoops[m_iterCurrLine]--;
                        if(m_MapActiveLoops[m_iterCurrLine] == 0)
                        {
                            // remove loop
                            m_MapActiveLoops.remove(m_iterCurrLine);
                            qInfo("All loops for target %s performd", qPrintable(params[0].toString()));
                        }
                        else
                            // next loop
                            m_iterNextLine = iterTarget;
                    }
                }
                // new loop
                else
                {
                    m_MapActiveLoops[m_iterCurrLine] = params[1].toInt();
                    m_iterNextLine = iterTarget;
                }
                // finish here
                emit OperationFinish(false, "");
            }
            else
            {
                QString strError;
                strError.sprintf("Could not find loop target \"%s\"!", qPrintable(params[0].toString()));
                emit OperationFinish(true, strError);
            }
            break;
        }
        case CMD_FILE_SET_ON_ERROR_STOP:
            m_bStopOnExternalError = params[0].toBool();
            emit OperationFinish(false, "");
            break;
    }
}

void CmdHandlerFile::SendRemoteCmd(QByteArray Cmd)
{
    if(m_pCurrSocket)
        m_pCurrSocket->write(Cmd + END_STR);
    else
        qFatal("No connection to server established!");

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
    if(m_pCurrSocket)
        disconnect(m_pCurrSocket, &QTcpSocket::readyRead, this, &CmdHandlerFile::OnReceive);
    m_pCurrSocket = pSocket;
    connect(m_pCurrSocket, &QTcpSocket::readyRead, this, &CmdHandlerFile::OnReceive);
    connect(m_pCurrSocket, &QTcpSocket::disconnected, this, &CmdHandlerFile::OnDisconnect);
}


void CmdHandlerFile::OnReceive()
{
    if(m_pCurrSocket)
    {
        QString ReadData = m_pCurrSocket->readAll();
        qInfo(qPrintable(ReadData));
        if(!m_bStopOnExternalError || !ReadData.toUpper().contains(",ERROR"))
            emit cmdFinish();
        else
            qFatal("Abort on external error!");
    }
}

void CmdHandlerFile::OnDisconnect()
{
    qFatal("Server closed connection unexpected!");
}
