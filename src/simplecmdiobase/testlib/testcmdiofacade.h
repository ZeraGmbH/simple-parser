#ifndef TESTCMDIOFACADE_H
#define TESTCMDIOFACADE_H

#include "cmdserver.h"
#include "testcmdiohandler.h"
#include "testcmdioparser.h"
#include <QObject>

class TestCmdIoFacade : public QObject
{
    Q_OBJECT
public:
    TestCmdIoFacade(int port);
    void addCmd(QString cmd, CmdParamTypeIdList params, QString paramOkErrorString);
private:
    TestCmdIoParser m_parser;
    TestCmdIoHandler m_handler;
    QSimpleCmdServer m_server;
};

#endif // TESTCMDIOFACADE_H
