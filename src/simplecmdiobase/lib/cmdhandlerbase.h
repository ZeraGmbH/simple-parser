#ifndef CMDHANDLERBASE_H
#define CMDHANDLERBASE_H

#include "cmdparserbase.h"
#include <QObject>
#include "simplecmdiobase_export.h"

// class QSimpleCmdHandlerBase
class SIMPLECMDIOBASE_EXPORT QSimpleCmdHandlerBase : public QObject
{
    Q_OBJECT
public:
    explicit QSimpleCmdHandlerBase(QObject *parent = nullptr);
    virtual void StartCmd(SimpleCmdData *pCmd, QVariantList params) = 0;
signals:
    void OperationFinish(bool bError, QString strMessage);

public slots:
};

#endif // CMDHANDLERBASE_H
