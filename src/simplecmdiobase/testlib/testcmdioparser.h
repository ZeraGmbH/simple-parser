#ifndef TESTCMDIOPARSER_H
#define TESTCMDIOPARSER_H

#include <QSimpleCmdParserSocketBase>

class TestCmdIoParser : public QSimpleCmdParserSocketBase
{
    Q_OBJECT
public:
    explicit TestCmdIoParser(QObject *parent = nullptr);
    void add(QString cmd, CmdParamTypeIdList params, QString paramOkErrorString);
private:
    QString GetParserName() override;
    const QString PlausiCheck(SimpleCmdData *pCmd, const QVariantList &params) override;
    QStringList m_paramOkErrorStrings;
};

#endif // TESTCMDIOPARSER_H
