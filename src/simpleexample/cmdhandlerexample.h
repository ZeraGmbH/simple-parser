#ifndef CMDHANDLEREXAMPLE_H
#define CMDHANDLEREXAMPLE_H

// From outside include
// #include <QSimpleCmdHandlerBase>

// Inside include
#include "cmdhandlerbase.h"

class CmdHandlerExample : public QSimpleCmdHandlerBase
{
    Q_OBJECT
public:
    CmdHandlerExample(QObject *parent = Q_NULLPTR);
    virtual void StartCmd(SimpleCmdData *pCmd, QVariantList params);

signals:
    void ApplyPressed();

private slots:
    void OnDelayTimer();

private:
    QTimer m_DelayTimer;
    int m_iDelayRequested;
};

#endif // CMDHANDLEREXAMPLE_H
