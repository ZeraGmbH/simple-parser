#ifndef CMDPARSEREXAMPLE_H
#define CMDPARSEREXAMPLE_H

// From outside include
// #include <QSimpleCmdParserSocketBase>

// Inside include
#include "cmdparserbase.h"

enum CmdExampleCmdIds
{
    CMD_EXAMPLE_MSG,
    CMD_EXAMPLE_DELAY
};

class CmdParserExample : public QSimpleCmdParserSocketBase
{
    Q_OBJECT
public:
    CmdParserExample(QObject *parent = Q_NULLPTR);
    virtual QString GetParserName() { return QLatin1String("ExampleParser"); }

public slots:
    void OnApplyPressed();

protected:
    virtual const QString PlausiCheck(SimpleCmdData *pCmd, const QVariantList &params);

};

#endif // CMDPARSEREXAMPLE_H
