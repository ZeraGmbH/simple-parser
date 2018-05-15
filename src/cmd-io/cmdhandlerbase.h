#ifndef CMDHANDLERBASE_H
#define CMDHANDLERBASE_H

#include "cmdparserbase.h"
#include <QObject>

// class CmdHandlerBase
class CmdHandlerBase : public QObject
{
    Q_OBJECT
public:
    explicit CmdHandlerBase(QObject *parent = 0);
    virtual void StartCmd(CmdData *pCmd, QVariantList params) = 0;
signals:
    void OperationFinish(bool bError, QString strMessage);

public slots:
};

#endif // CMDHANDLERBASE_H
