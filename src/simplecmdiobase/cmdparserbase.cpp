#include "cmdparserbase.h"
#include "cmdhandlerbase.h"
#include <QCoreApplication>

SimpleCmdData::SimpleCmdData(QString strDisplay, CmdParamTypeIdList ListParams, int CmdID, bool bLogCmd, QString strHelpAdd) :
    m_strDisplay(strDisplay), m_ListParams(ListParams), m_CmdID(CmdID), m_bLogCmd(bLogCmd), m_strHelpAdd(strHelpAdd)
{
}

int SimpleCmdData::GetCmdID()
{
    return m_CmdID;
}

QString SimpleCmdData::GetDisplayStr()
{
    return m_strDisplay;
}

bool QSimpleCmdParserBase::m_bLogCmdGlobal = false;

QSimpleCmdParserBase::QSimpleCmdParserBase(QObject *parent) :
    QObject(parent), m_pCurrCmd(Q_NULLPTR), m_pCookie(Q_NULLPTR), m_pCmdHandler(Q_NULLPTR), m_bLogCmd(m_bLogCmdGlobal)
{
}

void QSimpleCmdParserBase::SetCmdHandler(QSimpleCmdHandlerBase* pCmdHandler)
{
    m_pCmdHandler = pCmdHandler;
    connect(m_pCmdHandler, &QSimpleCmdHandlerBase::OperationFinish, this, &QSimpleCmdParserBase::OnOperationFinish, Qt::QueuedConnection);
}

void QSimpleCmdParserBase::SetGlobalOutOfOrderMessage(QString strMessage)
{
    m_strGlobalOutOfOrderMessage = strMessage;
}

bool QSimpleCmdParserBase::GetCmdLog()
{
    return m_bLogCmdGlobal && m_bLogCmd;
}

void QSimpleCmdParserBase::SetCmdLogGlobal(bool bLogCmdGlobal)
{
    m_bLogCmdGlobal = bLogCmdGlobal;
}


/**
  @b Setup command info lists required for proper parsing
  @param strCmd [in] Command name
  @param paramList [in] list of expected parameter types
  @param iCmdID [in] ID of decoded command
  */
void QSimpleCmdParserBase::AddCmdInfo(QString strCmd, CmdParamTypeIdList paramList, int iCmdID, bool bLogCmd, QString strHelpAdd)
{
    QString strCmdUpp = strCmd.toUpper();
    if(strCmdUpp == QLatin1String("HELP"))
        qFatal("Cmd \"%s\" is not allowed!", qPrintable(strCmd));
    if(m_cmdList.contains(strCmdUpp))
        qFatal("Cmd \"%s\" already inserted!", qPrintable(strCmd));
    else
    {
        m_cmdList[strCmdUpp] = new SimpleCmdData(
                    strCmd,
                    paramList,
                    iCmdID,
                    bLogCmd,
                    strHelpAdd);
    }
}

const QString QSimpleCmdParserBase::PlausiCheck(SimpleCmdData *pCmd, const QVariantList& params)
{
    Q_UNUSED( pCmd )
    Q_UNUSED( params )
    return QString();
}

/**
  @b Parse and start a command. If parsing succeeds our handler's StartCmd is called
  @param strCmd [in] Command received from whatever interface
  @param pCookie [in] Generic pointer wich will be part of the
  @returns (Error)Message - if empty we expended end on signal CmdFinish
  */
const QString QSimpleCmdParserBase::ParseAndStartCmd(QString strCmd, QIODevice *pCookie)
{
    QString strRet;
    QStringList strCmdAndParamList = strCmd.split(QLatin1String(","));
    m_bLogCmd = true;
    if(strCmdAndParamList.count() > 0)
    {
        QString strCmdUpp = strCmdAndParamList[0].toUpper().trimmed();
        // Special Help command available for all parsers
        if(strCmdUpp == QLatin1String("HELP"))
        {
            QString strDisplay = QLatin1String("HELP");
            // Help does no accept params
            if(strCmdAndParamList.count() == 1)
            {
                // Title
                strRet += QLatin1String("--------------------------------------------------------------------------------\n");
                strRet += QString(QLatin1String("Name: %1\n\n")).arg(GetParserName());
                strRet += QLatin1String("--------------------------------------------------------------------------------\n");
                // Sort commands
                QMap<QString, SimpleCmdData*> cmdListSorted;
                for(QHash<QString, SimpleCmdData*>::iterator iterCmd = m_cmdList.begin();
                    iterCmd!=m_cmdList.end(); iterCmd++)
                    cmdListSorted[iterCmd.key()] = iterCmd.value();
                // Loop all commands
                for(QMap<QString, SimpleCmdData*>::iterator iterCmdSorted = cmdListSorted.begin();
                    iterCmdSorted!=cmdListSorted.end(); iterCmdSorted++)
                {
                    SimpleCmdData *cmdData = iterCmdSorted.value();
                    strRet += QString(QLatin1String("Cmd: %1")).arg(cmdData->GetDisplayStr());
                    CmdParamTypeIdList::iterator iterParam = cmdData->m_ListParams.begin();
                    while(iterParam != cmdData->m_ListParams.end())
                    {
                        switch(*iterParam)
                        {
                        case PARAM_TYPE_BOOL:
                            strRet += QLatin1String(",bool");
                            break;
                        case PARAM_TYPE_INT:
                            strRet += QLatin1String(",int");
                            break;
                        case PARAM_TYPE_FLOAT:
                            strRet += QLatin1String(",float");
                            break;
                        case PARAM_TYPE_STRING:
                            strRet += QLatin1String(",string");
                            break;
                        }
                        iterParam++;
                    }
                    strRet += QLatin1String("\n");
                    if(!cmdData->m_strHelpAdd.isEmpty())
                    {
                        QString strIndent = QLatin1String("  ");
                        QString strHelpAdd = cmdData->m_strHelpAdd;
                        strHelpAdd = strHelpAdd.replace(QLatin1String("\n"), QLatin1String("\n")+strIndent);
                        strRet += strIndent + QString(QLatin1String("%1\n")).arg(strHelpAdd);
                    }
                    strRet += QLatin1String("--------------------------------------------------------------------------------\n");
                }
            }
            else
                strRet = FormatErrorMsg(strDisplay, QLatin1String("Wrong number of parameters"));
        }
        else if(m_cmdList.contains(strCmdUpp))
        {
            SimpleCmdData *cmdData = m_cmdList[strCmdUpp];
            // Check parameters
            if(strCmdAndParamList.count() == cmdData->m_ListParams.count() + 1)
            {
                // First trim numeric
                for(int iParam=0; iParam<cmdData->m_ListParams.count();iParam++)
                {
                    if( cmdData->m_ListParams[iParam] == PARAM_TYPE_BOOL ||
                        cmdData->m_ListParams[iParam] == PARAM_TYPE_INT ||
                        cmdData->m_ListParams[iParam] == PARAM_TYPE_FLOAT )
                        strCmdAndParamList[iParam+1] = strCmdAndParamList[iParam+1].trimmed();
                }
                QVariantList params;
                bool bParamFormatOK = true;
                for(int iParam=0; iParam<cmdData->m_ListParams.count();iParam++)
                {
                    QVariant param;
                    QString strParam = strCmdAndParamList[iParam+1];
                    switch(cmdData->m_ListParams[iParam])
                    {
                    case PARAM_TYPE_BOOL:
                        if(strParam == QLatin1String("0"))
                            param = QVariant(false);
                        else if(strParam == QLatin1String("1"))
                            param = QVariant(true);
                        else
                            bParamFormatOK = false;
                        break;
                    case PARAM_TYPE_INT:
                    {
                        int iVal = strParam.toInt(&bParamFormatOK);
                        param = QVariant(iVal);
                        break;
                    }
                    case PARAM_TYPE_FLOAT:
                    {
                        double dblVal = strParam.toDouble(&bParamFormatOK);
                        param = QVariant(dblVal);
                        break;
                    }
                    case PARAM_TYPE_STRING:
                        param = QVariant(strParam);
                        break;
                    }
                    params.append(param);
                }

                if(bParamFormatOK)
                {
                    // Are we busy?
                    if(m_pCurrCmd != Q_NULLPTR)
                        strRet = FormatErrorMsg(cmdData->m_strDisplay, "Cannot start while command \"" + m_pCurrCmd->m_strDisplay +"\"is busy!");
                    else
                    {
                        strRet = PlausiCheck(cmdData, params);
                        if(strRet.isEmpty())
                        {
                            // Command itself is OK - is there a global bad (e.g device not open)?
                            if(m_strGlobalOutOfOrderMessage.isEmpty())
                            {
                                m_pCurrCmd = cmdData;
                                m_bLogCmd = m_pCurrCmd->m_bLogCmd;
                                m_pCookie = pCookie;
                                m_pCmdHandler->StartCmd(cmdData, params);
                            }
                            else
                                strRet = FormatErrorMsg(cmdData->m_strDisplay, m_strGlobalOutOfOrderMessage);
                        }
                    }
                }
                else
                    strRet = FormatErrorMsg(cmdData->m_strDisplay , QLatin1String("Wrong parameter format"));
            }
            else
                strRet = FormatErrorMsg(cmdData->m_strDisplay , QLatin1String("Wrong number of parameters"));
        }
        // command not found
        else
            strRet = FormatErrorMsg(strCmdUpp, QLatin1String("Unknown command: \"") + strCmd +QLatin1String("\""));
    }
    return strRet;
}

/**
  @b Fire final success notification here
  @param strReturn [in] In case the transaction reads data it is set here as ','-separated else error message
  */
void QSimpleCmdParserBase::OnOperationFinish(bool bError, QString strReturn)
{
    QString strEmit;
    if(m_pCurrCmd)
        strEmit += m_pCurrCmd->m_strDisplay;
    if(!bError)
        strEmit += QString(QLatin1String(",OK"));
    else
        strEmit += QString(QLatin1String(",ERROR"));
    if(!strReturn.isEmpty())
        strEmit += QString(QLatin1String(",")) + strReturn;

    QIODevice* pCookie = m_pCookie;
    m_pCurrCmd = Q_NULLPTR;
    m_pCookie = Q_NULLPTR;
    emit CmdFinish(strEmit, pCookie);
}

/**
  @b Build error message
  @param strCmd [in] Name of command
  @param strErrDescription [in] Human readable error description
  */
const QString QSimpleCmdParserBase::FormatErrorMsg(QString strCmd, QString strErrDescription)
{
    return strCmd + QLatin1String(",ERROR: ") + strErrDescription;
}


/**
  @b Check valid Hex value
  @param strParam [in] Hex address value
  @param iMaxLen [in] maximum string length of value
  @returns true if valid
  */
bool QSimpleCmdParserBase::isValidHexValue(QString strParam, int iMaxLen)
{
    bool bValidHexValue = true;
    // length
    if((strParam.length() % 2) || (iMaxLen > 0 && strParam.length() > iMaxLen) || strParam.length() <= 0)
        bValidHexValue = false;
    // correct hex
    else
    {
        for(int iByte=0; iByte<strParam.length()/2 && bValidHexValue; iByte++)
            strParam.mid(iByte*2, 2).toInt(&bValidHexValue, 16);
    }
    return bValidHexValue;
}

/**
  @b Convert ASCII to binary by replacing escape sequences
  @param strParam [in] ASCII Param
  @returns binary data
  */
QByteArray QSimpleCmdParserBase::BinaryFromAscii(QString strParam)
{
    QByteArray data = strParam.toLatin1();
    data = data.replace("\\\\","\\");
    data = data.replace("\\n","\n");
    data = data.replace("\\r","\r");
    data = data.replace("\\t","\t");
    data = data.replace((char*)"\\0",2,(char*)"\0",1);
    data = data.replace("\\;",",");
    return data;
}

/**
  @b Help string for escape sequences and specials
  @param strParam [in] ASCII Param
  @returns Help string
  */
QString QSimpleCmdParserBase::BinaryConversionHelpString()
{
    return QLatin1String("Escapes: \'\\\\\' \'\\n\' \'\\r\' \'\\t\' Specials: \'\\0\' -> 0x00 \'\\;\' -> \',\'");
}

QSimpleCmdParserSocketBase::QSimpleCmdParserSocketBase(QObject *parent) :
    QSimpleCmdParserBase(parent), m_ui16IPPort(0)
{
}


void QSimpleCmdParserSocketBase::SetListenPort(quint16 ui16IPPort)
{
    m_ui16IPPort = ui16IPPort;
}

quint16 QSimpleCmdParserSocketBase::GetListenPort()
{
    return m_ui16IPPort;
}
