#include "testcmdioparser.h"

void TestCmdIoParser::add(QString cmd, CmdParamTypeIdList params, bool cmdOk, QString cmdInfoReturned)
{
    m_commandInfos.append({ cmdOk, cmdInfoReturned });
    AddCmdInfo(cmd, params, m_commandInfos.count()-1);
}

QString TestCmdIoParser::GetParserName()
{
    return "TestCmdIoParser";
}

const QString TestCmdIoParser::PlausiCheck(SimpleCmdData *pCmd, const QVariantList &params)
{
    Q_UNUSED(params)
    QString ret;
    const CommandReturnInfo &info = m_commandInfos[pCmd->GetCmdID()];
    if(!info.ok)
        ret = FormatErrorMsg(pCmd->GetDisplayStr(), info.cmdInfoReturned);
    return ret;
}
