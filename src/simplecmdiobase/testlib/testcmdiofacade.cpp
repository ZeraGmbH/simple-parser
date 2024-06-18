#include "testcmdiofacade.h"

TestCmdIoFacade::TestCmdIoFacade(int port)
{
    m_parser.SetCmdHandler(&m_handler);
    m_server.Setup(port, &m_parser);
}

void TestCmdIoFacade::addCmd(QString cmd, CmdParamTypeIdList params, CommandResultTypes cmdResultType, QString cmdInfoReturned)
{
    m_parser.add(cmd, params, cmdResultType != RESULT_ERROR_PARSER, cmdInfoReturned);
    m_handler.add(cmdResultType != RESULT_ERROR_HANDLER, cmdInfoReturned);
}
