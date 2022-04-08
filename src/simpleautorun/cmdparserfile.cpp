#include <QFile>
#include <QTextStream>
#include "commonhelpers.h"
#include "cmdparserfile.h"

CmdParserFile::CmdParserFile(QObject *parent) : QSimpleCmdParserBase(parent)
{
    m_pCmdHandlerFile = nullptr;
    /* OpenIPPort: Open a IP port to fmt service */
    /* Parameters: strIP, iPort */
    /* Result: None */
    AddCmdInfo("OpenIPPort",
               CmdParamTypeIdList() << PARAM_TYPE_STRING << PARAM_TYPE_INT,
               CMD_FILE_OPEN_IPPORT);

    /* SelectIPConnection: Select IP as current (Opened IP connections are counted zero based) */
    /* Parameters: iNumCOnnection */
    /* Result: None */
    AddCmdInfo("SelectIPConnection",
               CmdParamTypeIdList() << PARAM_TYPE_INT,
               CMD_FILE_SELECT_IPCONNECTION);

    /* WaitMs: Wait for n ms */
    /* Parameters: iMs */
    /* Result: None */
    AddCmdInfo("WaitMs",
               CmdParamTypeIdList() << PARAM_TYPE_INT,
               CMD_FILE_WAIT_MS);

    /* SetTag: Set a tag for loops */
    /* Parameters: strTagID */
    /* Result: None */
    AddCmdInfo("SetTag",
               CmdParamTypeIdList() << PARAM_TYPE_STRING,
               CMD_FILE_SET_TAG);

    /* LoopTag: Loop to tag */
    /* Parameters: strTagID, iCount */
    /* Result: None */
    AddCmdInfo("LoopTag",
               CmdParamTypeIdList() << PARAM_TYPE_STRING << PARAM_TYPE_INT,
               CMD_FILE_LOOP_TAG);

    /* SetOnErrorStop: Set behaviour in case of error on external commands */
    /* Parameters: bStop */
    /* Result: None */
    AddCmdInfo("SetOnErrorStop",
               CmdParamTypeIdList() << PARAM_TYPE_BOOL,
               CMD_FILE_SET_ON_ERROR_STOP);

    /* CheckLastResponse: Check data received from last response */
    /* Parameters: strExpectedResponse, bIgnoreCase */
    /* Result: None */
    AddCmdInfo("CheckLastResponse",
               CmdParamTypeIdList() << PARAM_TYPE_STRING << PARAM_TYPE_BOOL,
               CMD_FILE_CHECK_LAST_RESPONSE);

    /* MsgAndWait: Output message and wait for user to press return */
    /* Parameters: strMessage */
    /* Result: None */
    AddCmdInfo("MsgAndWait",
               CmdParamTypeIdList() << PARAM_TYPE_STRING,
               CMD_FILE_MESSAGE);

    connect(this, &CmdParserFile::next, this, &CmdParserFile::OnExecNext);
    connect(this, &CmdParserFile::CmdFinish, this, &CmdParserFile::OnFileCmdFinish );

}

void CmdParserFile::StartFileExecution(QString strFileName, CmdHandlerFile *pCmdHandlerFile)
{
    if(m_pCmdHandlerFile) {
        disconnect(m_pCmdHandlerFile, &CmdHandlerFile::cmdFinish, this, &CmdParserFile::OnExecNext);
    }
    m_pCmdHandlerFile = pCmdHandlerFile;
    connect(m_pCmdHandlerFile, &CmdHandlerFile::cmdFinish, this, &CmdParserFile::OnExecNext);

    QFile executeFile(strFileName);
    if(executeFile.open(QIODevice::ReadOnly)) {
        m_strCmdList.clear();
        QTextStream textStream(&executeFile);
        while (!textStream.atEnd()) {
            QString strLine = textStream.readLine();
            if(!strLine.isEmpty())
                m_strCmdList.append(strLine);
        }
        executeFile.close();
        m_iterCurrLine = m_strCmdList.begin();
        emit next();
    }
    else {
        LogMsg(QStringLiteral("Execution file %1 could not be opened!")
               .arg(strFileName), LOG_COLOUR_RED);
        emit done(-1);
    }
}

void CmdParserFile::OnExecNext()
{
    if(m_iterCurrLine != m_strCmdList.end()) {
        m_pCmdHandlerFile->SetCurrIterLine(m_iterCurrLine);
        QString strCurrLine = *m_iterCurrLine;
        // ignore comments
        int iPos = strCurrLine.indexOf("#");
        if(iPos >= 0) {
            strCurrLine = strCurrLine.remove(iPos, strCurrLine.length()).trimmed();
        }
        if(!strCurrLine.isEmpty()) {
            LogMsg(QStringLiteral("Starting %1...")
                   .arg(strCurrLine));
            QString strResponse = ParseAndStartCmd(strCurrLine, nullptr);
            // check commands returned immediately
            if(!strResponse.isEmpty()) {
                // Forward unkown to external if possible
                if(strResponse.toUpper().contains("UNKNOWN COMMAND")) {
                    m_pCmdHandlerFile->SendRemoteCmd(strCurrLine);
                }
                else if(!m_bAllowErrors) {
                    LogMsg(QStringLiteral("Unexpected return %1 on local command!")
                           .arg(strResponse), LOG_COLOUR_RED);
                    emit done(-1);
                }
            }
        }
        m_iterCurrLine = m_pCmdHandlerFile->GetNextIterLine();
        if(strCurrLine.isEmpty()) {
            emit next();
        }
    }
    else {
        // exit
        emit done(0);
    }
}

// If execution of file command was started, we arrive here
void CmdParserFile::OnFileCmdFinish(QString strCmdResponse, QIODevice *pCookie)
{
    Q_UNUSED(pCookie)
    bool bError = strCmdResponse.contains(",ERROR", Qt::CaseInsensitive);
    if(m_iCurrCmdID == CMD_FILE_CHECK_LAST_RESPONSE) {
        LogMsg(strCmdResponse, bError ? LOG_COLOUR_RED : LOG_COLOUR_GREEN);
    }
    else {
        LogMsg(strCmdResponse);
    }
    if(!m_bAllowErrors && bError) {
        LogMsg(QStringLiteral("Unexpected return %1 on local command!")
               .arg(strCmdResponse),LOG_COLOUR_RED);
        emit done(-1);
    }
    else {
        emit next();
    }
}

const QString CmdParserFile::PlausiCheck(SimpleCmdData *pCmd, const QVariantList &params)
{
    // Bail out by default in case internal command errors
    m_bAllowErrors = false;
    QString strRet;
    int iValue;
    m_iCurrCmdID = pCmd->GetCmdID();
    switch(m_iCurrCmdID) {
    case CMD_FILE_OPEN_IPPORT:
        iValue = params[1].toInt();
        if(iValue < 1 || iValue > 65535) {
            strRet = FormatErrorMsg(pCmd->GetDisplayStr(), QStringLiteral("Port of range: %1 [1-65535]").arg(iValue));
        }
        break;
    case CMD_FILE_SELECT_IPCONNECTION:
        iValue = params[0].toInt();
        if(iValue < 0) {
            strRet = FormatErrorMsg(pCmd->GetDisplayStr(), QStringLiteral("IP number connection must be >= 0"));
        }
        break;
    case CMD_FILE_WAIT_MS:
        iValue = params[0].toInt();
        if(iValue < 1 || iValue > 100000) {
            strRet = FormatErrorMsg(pCmd->GetDisplayStr(), QStringLiteral("Pause of range: %1 [1-100000]").arg(iValue));
        }
        break;
    case CMD_FILE_SET_TAG:
        break;
    case CMD_FILE_LOOP_TAG:
        iValue = params[1].toInt();
        if(iValue < 0 || iValue > 65535) {
            strRet = FormatErrorMsg(pCmd->GetDisplayStr(), QStringLiteral("Loop count out of range: %1 [0-65535]").arg(iValue));
        }
        break;
    case CMD_FILE_SET_ON_ERROR_STOP:
        break;
    case CMD_FILE_CHECK_LAST_RESPONSE:
        // Do not bail out in case check fails
        m_bAllowErrors = true;
        break;
    case CMD_FILE_MESSAGE:
        break;
    default:
        // Unhandled is no error -> cmd is passed to current IPport
        break;
    }
    return strRet;
}
