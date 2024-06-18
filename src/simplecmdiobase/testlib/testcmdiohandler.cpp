#include "testcmdiohandler.h"

void TestCmdIoHandler::StartCmd(SimpleCmdData *pCmd, QVariantList params)
{
    Q_UNUSED(pCmd)
    Q_UNUSED(params)
    emit OperationFinish(false, "");
}
