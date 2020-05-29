#include "cmdparserbase.h"
#include "cmdhandlerbase.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QRegularExpression>

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
    QObject(parent), m_pCurrCmd(nullptr), m_pCookie(nullptr), m_pCmdHandler(nullptr), m_bLogCmd(m_bLogCmdGlobal)
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
    if(strCmdUpp == QStringLiteral("HELP")) {
        qFatal("Cmd \"%s\" is not allowed!", qPrintable(strCmd));
    }
    if(m_cmdList.contains(strCmdUpp)) {
        qFatal("Cmd \"%s\" already inserted!", qPrintable(strCmd));
    }
    else {
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
    QStringList strCmdAndParamList = strCmd.split(',');
    m_bLogCmd = true;
    if(strCmdAndParamList.count() > 0) {
        QString strCmdUpp = strCmdAndParamList[0].toUpper().trimmed();
        // Special Help command available for all parsers
        if(strCmdUpp == QStringLiteral("HELP") || strCmdUpp == QStringLiteral("?")) {
            QString strDisplay = QStringLiteral("HELP");
            // Help does no accept params
            if(strCmdAndParamList.count() == 1) {
                // Title
                strRet += QStringLiteral("--------------------------------------------------------------------------------\n");
                strRet += QStringLiteral("Name: %1\n\n").arg(GetParserName());
                strRet += QStringLiteral("--------------------------------------------------------------------------------\n");
                // Macros
                strRet += QStringLiteral("Parameter macros:\n\n");
                strRet += QStringLiteral("@date_time(QDateTime-format-string): expands to formatted system date/time\n");
                strRet += QStringLiteral("@ascii_to_hex(string): expands to hexadecimal representation of `string`\n");
                strRet += QStringLiteral("@hex_to_ascii(hex_string): expands to`hex_string` to ASCII\n");
                strRet += QStringLiteral("@num_to_hex(integer;min_num_hex_digits): expands `integer` to hexadecimal\n");
                strRet += QStringLiteral("\n");
                strRet += QStringLiteral("--------------------------------------------------------------------------------\n");

                strRet += QStringLiteral("Commands:\n");
                strRet += QStringLiteral("--------------------------------------------------------------------------------\n");
                // Sort commands
                QMap<QString, SimpleCmdData*> cmdListSorted;
                for(QHash<QString, SimpleCmdData*>::iterator iterCmd = m_cmdList.begin();
                        iterCmd!=m_cmdList.end(); iterCmd++) {
                    cmdListSorted[iterCmd.key()] = iterCmd.value();
                }
                // Loop all commands
                for(QMap<QString, SimpleCmdData*>::iterator iterCmdSorted = cmdListSorted.begin();
                        iterCmdSorted!=cmdListSorted.end(); iterCmdSorted++) {
                    SimpleCmdData *cmdData = iterCmdSorted.value();
                    strRet += QStringLiteral("Cmd: %1").arg(cmdData->GetDisplayStr());
                    CmdParamTypeIdList::iterator iterParam = cmdData->m_ListParams.begin();
                    while(iterParam != cmdData->m_ListParams.end()) {
                        switch(*iterParam) {
                        case PARAM_TYPE_BOOL:
                            strRet += QStringLiteral(",bool");
                            break;
                        case PARAM_TYPE_INT:
                            strRet += QStringLiteral(",int");
                            break;
                        case PARAM_TYPE_FLOAT:
                            strRet += QStringLiteral(",float");
                            break;
                        case PARAM_TYPE_STRING:
                            strRet += QStringLiteral(",string");
                            break;
                        }
                        iterParam++;
                    }
                    strRet += QStringLiteral("\n");
                    if(!cmdData->m_strHelpAdd.isEmpty()) {
                        QString strIndent = QStringLiteral("  ");
                        QString strHelpAdd = cmdData->m_strHelpAdd;
                        strHelpAdd = strHelpAdd.replace(QStringLiteral("\n"), QStringLiteral("\n")+strIndent);
                        strRet += strIndent + QStringLiteral("%1\n").arg(strHelpAdd);
                    }
                    strRet += QStringLiteral("--------------------------------------------------------------------------------\n");
                }
            }
            else {
                strRet = FormatErrorMsg(strDisplay, QStringLiteral("Wrong number of parameters"));
            }
        }
        else if(m_cmdList.contains(strCmdUpp)) {
            SimpleCmdData *cmdData = m_cmdList[strCmdUpp];
            // Check parameters
            if(strCmdAndParamList.count() == cmdData->m_ListParams.count() + 1) {
                // expand macros
                QString strMacroError;
                for(int iParam=0; iParam<cmdData->m_ListParams.count();iParam++) {
                    strMacroError += ExpandMacro(strCmdAndParamList[iParam+1]);
                }
                // early out in case of macro error(s)
                if(!strMacroError.isEmpty()) {
                    return FormatErrorMsg(cmdData->m_strDisplay, strMacroError.right(strMacroError.size()-3) /* remove lead ' / ' */);
                }

                // trim numeric params
                for(int iParam=0; iParam<cmdData->m_ListParams.count();iParam++) {
                    if( cmdData->m_ListParams[iParam] == PARAM_TYPE_BOOL ||
                            cmdData->m_ListParams[iParam] == PARAM_TYPE_INT ||
                            cmdData->m_ListParams[iParam] == PARAM_TYPE_FLOAT )
                        strCmdAndParamList[iParam+1] = strCmdAndParamList[iParam+1].trimmed();
                }
                // check parameter format
                bool bParamFormatOK = true;
                QVariantList params;
                for(int iParam=0; iParam<cmdData->m_ListParams.count();iParam++) {
                    QVariant param;
                    QString strParam = strCmdAndParamList[iParam+1];
                    switch(cmdData->m_ListParams[iParam]) {
                    case PARAM_TYPE_BOOL:
                        if(strParam == QStringLiteral("0"))
                            param = QVariant(false);
                        else if(strParam == QStringLiteral("1"))
                            param = QVariant(true);
                        else
                            bParamFormatOK = false;
                        break;
                    case PARAM_TYPE_INT: {
                        int iVal = strParam.toInt(&bParamFormatOK);
                        param = QVariant(iVal);
                        break;
                    }
                    case PARAM_TYPE_FLOAT: {
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

                if(bParamFormatOK) {
                    // Are we busy?
                    if(m_pCurrCmd != nullptr) {
                        strRet = FormatErrorMsg(cmdData->m_strDisplay, "Cannot start while command \"" + m_pCurrCmd->m_strDisplay +"\"is busy!");
                    }
                    else {
                        strRet = PlausiCheck(cmdData, params);
                        if(strRet.isEmpty()) {
                            // Command itself is OK - is there a global bad (e.g device not open)?
                            if(m_strGlobalOutOfOrderMessage.isEmpty()) {
                                m_pCurrCmd = cmdData;
                                m_bLogCmd = m_pCurrCmd->m_bLogCmd;
                                m_pCookie = pCookie;
                                m_pCmdHandler->StartCmd(cmdData, params);
                            }
                            else {
                                strRet = FormatErrorMsg(cmdData->m_strDisplay, m_strGlobalOutOfOrderMessage);
                            }
                        }
                    }
                }
                else {
                    strRet = FormatErrorMsg(cmdData->m_strDisplay , QStringLiteral("Wrong parameter format"));
                }
            }
            else {
                strRet = FormatErrorMsg(cmdData->m_strDisplay , QStringLiteral("Wrong number of parameters"));
            }
        }
        // command not found
        else {
            strRet = FormatErrorMsg(strCmdUpp, QStringLiteral("Unknown command: \"") + strCmd + QStringLiteral("\""));
        }
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
    if(m_pCurrCmd) {
        strEmit += m_pCurrCmd->m_strDisplay;
    }
    if(!bError) {
        strEmit += QStringLiteral(",OK");
    }
    else {
        strEmit += QStringLiteral(",ERROR");
    }
    if(!strReturn.isEmpty()) {
        strEmit += QStringLiteral(",") + strReturn;
    }

    QIODevice* pCookie = m_pCookie;
    m_pCurrCmd = nullptr;
    m_pCookie = nullptr;
    emit CmdFinish(strEmit, pCookie);
}

/**
  @b Build error message
  @param strCmd [in] Name of command
  @param strErrDescription [in] Human readable error description
  */
const QString QSimpleCmdParserBase::FormatErrorMsg(QString strCmd, QString strErrDescription)
{
    return strCmd + QStringLiteral(",ERROR: ") + strErrDescription;
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
    if((strParam.length() % 2) || (iMaxLen > 0 && strParam.length() > iMaxLen) || strParam.length() <= 0) {
        bValidHexValue = false;
    }
    // correct hex
    else {
        for(int iByte=0; iByte<strParam.length()/2 && bValidHexValue; iByte++) {
            strParam.mid(iByte*2, 2).toInt(&bValidHexValue, 16);
        }
    }
    return bValidHexValue;
}

/**
 * @b Expand macros starting with @
 * @param strParam [in/out] parameter to expand
 * @return in case error: Helpful message
 */
QString QSimpleCmdParserBase::ExpandMacro(QString &strParam)
{
    // Macro expansion. When adding further macros don't forget to
    // * adjust help text above
    // * README.md

    // expand from right to left
    int lastEt = strParam.size();
    int iMacroStartPos, iMacroLabelEndPos, iParamsEndPos;

    QString strKnownMacro;
    QString strError;
    while((lastEt = strParam.lastIndexOf('@', lastEt-1)) >= 0) {
        QString strBefore;
        QString strParamConverted;
        QString strAfter;
        bool bExpandedOne = false;

        // common
        strBefore = strParam.left(lastEt);
        iParamsEndPos = strParam.indexOf(')', lastEt);
        if(iParamsEndPos > 0) {
            strAfter = strParam.mid(iParamsEndPos+1);
        }

        // @date_time
        if(!bExpandedOne) {
            strKnownMacro = QStringLiteral("@date_time(");
            iMacroStartPos = strParam.indexOf(strKnownMacro, lastEt);
            if(iMacroStartPos == lastEt) {
                iMacroLabelEndPos = iMacroStartPos+strKnownMacro.size();
                // param
                if(iParamsEndPos > 0) {
                    strParamConverted = QDateTime::currentDateTime().toString(strParam.mid(iMacroLabelEndPos, iParamsEndPos-iMacroLabelEndPos));
                }
                else {
                    strError += QStringLiteral(" / @date_time: ')' missing");
                }
                bExpandedOne = true;
            }
        }
        // @ascii_to_hex
        if(!bExpandedOne) {
            strKnownMacro = QStringLiteral("@ascii_to_hex(");
            iMacroStartPos = strParam.indexOf(strKnownMacro, lastEt);
            if(iMacroStartPos == lastEt) {
                iMacroLabelEndPos = iMacroStartPos+strKnownMacro.size();
                // param
                if(iParamsEndPos > 0) {
                    QByteArray macroParam = strParam.mid(iMacroLabelEndPos, iParamsEndPos-iMacroLabelEndPos).toLatin1();
                    strParamConverted = QString().fromLatin1(macroParam.toHex());
                }
                else {
                    strError += QStringLiteral(" / @ascii_to_hex: ')' missing");
                }
                bExpandedOne = true;
            }
        }
        // @hex_to_ascii
        if(!bExpandedOne) {
            strKnownMacro = QStringLiteral("@hex_to_ascii(");
            iMacroStartPos = strParam.indexOf(strKnownMacro, lastEt);
            if(iMacroStartPos == lastEt) {
                iMacroLabelEndPos = iMacroStartPos+strKnownMacro.size();
                // param
                if(iParamsEndPos > 0) {
                    QString strMacroParam = strParam.mid(iMacroLabelEndPos, iParamsEndPos-iMacroLabelEndPos);
                    QRegularExpression hexMatcher(QStringLiteral("^[a-fA-F0-9]+$"));
                    QRegularExpressionMatch match = hexMatcher.match(strMacroParam);
                    if (match.hasMatch()) {
                        strParamConverted = QString().fromLatin1(QByteArray::fromHex(strMacroParam.toLatin1()));
                    }
                    else {
                        strError += QStringLiteral(" / @hex_to_ascii: '%1' is not valid hex").arg(strMacroParam);
                    }
                }
                else {
                    strError += QStringLiteral(" / @hex_to_ascii: ')' missing");
                }
                bExpandedOne = true;
            }
        }
        // @num_to_hex
        if(!bExpandedOne) {
            strKnownMacro = QStringLiteral("@num_to_hex(");
            iMacroStartPos = strParam.indexOf(strKnownMacro, lastEt);
            if(iMacroStartPos == lastEt) {
                iMacroLabelEndPos = iMacroStartPos+strKnownMacro.size();
                // params
                if(iParamsEndPos > 0) {
                    QStringList params = strParam.mid(iMacroLabelEndPos, iParamsEndPos-iMacroLabelEndPos).split(';');
                    if(params.size() == 2) {
                        // value
                        bool bValidVal, bValidLen;
                        strParamConverted = QStringLiteral("%1").arg(params[0].toInt(&bValidVal, 10), 0, 16);
                        int iMinDigits = params[1].toInt(&bValidLen);
                        if(bValidVal && bValidLen) {
                            // leading 0
                            if(iMinDigits>strParamConverted.size()) {
                                strParamConverted = QString(iMinDigits-strParamConverted.size(), '0') + strParamConverted;
                            }
                        }
                        else {
                            if(!bValidVal) {
                                strError += QStringLiteral(" / @num_to_hex: '%1' is not a valid number").arg(params[0]);
                            }
                            if(!bValidLen) {
                                strError += QStringLiteral(" / @num_to_hex: '%1' is not a valid length").arg(params[1]);
                            }
                        }
                    }
                    else {
                        strError += QStringLiteral(" / @num_to_hex: expects two parametes");
                    }
                }
                else {
                    strError += QStringLiteral(" / @num_to_hex: ')' missing");
                }
                bExpandedOne = true;
            }
        }
        if(bExpandedOne) {
            strParam = strBefore + strParamConverted + strAfter;
            lastEt = strParam.size();
            continue;
        }
    }
    return strError;
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
    data = data.replace(const_cast<char*>("\\0"),2,const_cast<char*>("\0"),1);
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
