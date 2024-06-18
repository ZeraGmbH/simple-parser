#ifndef CMDHANDLERBASE_H
#define CMDHANDLERBASE_H

#include "cmdparserbase.h"
#include "simplecmdiobase_export.h"

class SIMPLECMDIOBASE_EXPORT QSimpleCmdHandlerBase : public QObject
{
    Q_OBJECT
public:
    explicit QSimpleCmdHandlerBase(QObject *parent = nullptr);
    virtual void StartCmd(SimpleCmdData *pCmd, QVariantList params) = 0;
signals:
    void OperationFinish(bool bError, QString strMessage);
};

#endif // CMDHANDLERBASE_H
