#ifndef TESTCMDIOCOMPLETESERVER_H
#define TESTCMDIOCOMPLETESERVER_H

#include "cmdserver.h"
#include "testcmdiohandler.h"
#include "testcmdioparser.h"
#include <QObject>

class TestCmdIoCompleteServer : public QObject
{
    Q_OBJECT
public:
    enum CommandResultTypes
    {
        RESULT_OK,
        RESULT_ERROR_PARSER,
        RESULT_ERROR_HANDLER
    };
    TestCmdIoCompleteServer(int port);
    void addCmd(QString cmd, CmdParamTypeIdList params, CommandResultTypes cmdResultType, QString cmdInfoReturned);
private:
    TestCmdIoParser m_parser;
    TestCmdIoHandler m_handler;
    QSimpleCmdServer m_server;
};

#endif // TESTCMDIOCOMPLETESERVER_H
