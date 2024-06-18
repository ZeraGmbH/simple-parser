#include "testcmdiohandler.h"

void TestCmdIoHandler::add(bool cmdOk, QString cmdInfoReturned)
{
    m_commandInfos.append({cmdOk, cmdInfoReturned});
}

void TestCmdIoHandler::StartCmd(SimpleCmdData *pCmd, QVariantList params)
{
    Q_UNUSED(pCmd)
    Q_UNUSED(params)
    const CommandReturnInfo &info = m_commandInfos[pCmd->GetCmdID()];
    emit OperationFinish(!info.ok, info.cmdInfoReturned);
}
