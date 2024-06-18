#ifndef TESTCMDIOHANDLER_H
#define TESTCMDIOHANDLER_H

#include <QSimpleCmdHandlerBase>

class TestCmdIoHandler : public QSimpleCmdHandlerBase
{
    Q_OBJECT
public:
    void add(bool cmdOk, QString cmdInfoReturned);
private:
    void StartCmd(SimpleCmdData *pCmd, QVariantList params) override;
    struct CommandReturnInfo
    {
        bool ok;
        QString cmdInfoReturned;
    };
    QList<CommandReturnInfo> m_commandInfos;
};

#endif // TESTCMDIOHANDLER_H
