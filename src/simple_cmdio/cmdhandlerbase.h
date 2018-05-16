#ifndef CMDHANDLERBASE_H
#define CMDHANDLERBASE_H

#include "cmdparserbase.h"
#include <QObject>

// class SimpleCmdHandlerBase
class SimpleCmdHandlerBase : public QObject
{
    Q_OBJECT
public:
    explicit SimpleCmdHandlerBase(QObject *parent = 0);
    virtual void StartCmd(SimpleCmdData *pCmd, QVariantList params) = 0;
signals:
    void OperationFinish(bool bError, QString strMessage);

public slots:
};

#endif // CMDHANDLERBASE_H
