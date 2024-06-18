#ifndef TESTCMDIOHANDLER_H
#define TESTCMDIOHANDLER_H

#include <QSimpleCmdHandlerBase>

class TestCmdIoHandler : public QSimpleCmdHandlerBase
{
    Q_OBJECT
public:
    void StartCmd(SimpleCmdData *pCmd, QVariantList params) override;
};

#endif // TESTCMDIOHANDLER_H
