#ifndef TESTCMDIOPARSER_H
#define TESTCMDIOPARSER_H

#include <QSimpleCmdParserSocketBase>
#include <QList>

class TestCmdIoParser : public QSimpleCmdParserSocketBase
{
    Q_OBJECT
public:
    explicit TestCmdIoParser(QObject *parent = nullptr);
    void add(QString cmd, CmdParamTypeIdList params, bool cmdOk, QString cmdInfoReturned);
private:
    QString GetParserName() override;
    const QString PlausiCheck(SimpleCmdData *pCmd, const QVariantList &params) override;
    struct CommandReturnInfo
    {
        bool ok;
        QString cmdInfoReturned;
    };
    QList<CommandReturnInfo> m_commandInfos;
};

#endif // TESTCMDIOPARSER_H
