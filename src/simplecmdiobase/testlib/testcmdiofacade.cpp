#include "testcmdiofacade.h"

TestCmdIoFacade::TestCmdIoFacade(int port)
{
    m_parser.SetCmdHandler(&m_handler);
    m_server.Setup(port, &m_parser);
}

void TestCmdIoFacade::addCmd(QString cmd, CmdParamTypeIdList params, bool cmdOk, QString cmdInfoReturned)
{
    m_parser.add(cmd, params, cmdOk, cmdInfoReturned);
}
