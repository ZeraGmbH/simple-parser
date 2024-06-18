#include "testcmdioparser.h"

TestCmdIoParser::TestCmdIoParser(QObject *parent)
    : QSimpleCmdParserSocketBase{parent}
{
}

void TestCmdIoParser::add(QString cmd, CmdParamTypeIdList params, QString paramOkErrorString)
{
    m_paramOkErrorStrings.append(paramOkErrorString);
    AddCmdInfo(cmd, params, m_paramOkErrorStrings.count()-1);
}

QString TestCmdIoParser::GetParserName()
{
    return "TestCmdIoParser";
}

const QString TestCmdIoParser::PlausiCheck(SimpleCmdData *pCmd, const QVariantList &params)
{
    Q_UNUSED(params)
    QString paramOkErrorString = m_paramOkErrorStrings[pCmd->GetCmdID()];
    return paramOkErrorString;
}
