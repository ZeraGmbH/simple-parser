#include "cmdparserexample.h"
#include "cmdhandlerexample.h"
#include <QMessageBox>

CmdHandlerExample::CmdHandlerExample(QObject *parent) : QSimpleCmdHandlerBase(parent)
{
    m_DelayTimer.setSingleShot(true);
    connect(&m_DelayTimer, &QTimer::timeout, this, &CmdHandlerExample::OnDelayTimer);
}

void CmdHandlerExample::StartCmd(SimpleCmdData *pCmd, QVariantList params)
{
    switch(static_cast<enum CmdExampleCmdIds>(pCmd->GetCmdID())) {
        case CMD_EXAMPLE_MSG: {
            QMessageBox::StandardButton selection = QMessageBox::information(
                nullptr,
                QStringLiteral("Example message command received"),
                QStringLiteral("Param received: %1\n\n"
                                      "OK: Immediate out / no data return\n"
                                      "Apply: SetGlobalOutOfOrderMessage -> error out further\n"
                                      "Cancel: Immediate out with error message").arg(params[0].toString()),
                QMessageBox::Ok | QMessageBox::Apply | QMessageBox::Cancel);
            switch(selection) {
            case QMessageBox::Ok:
                // Immediate command return without further data
                emit OperationFinish(false, QString());
                break;
            case QMessageBox::Apply:
                // Immediate command return without further data
                emit OperationFinish(false, QStringLiteral("SetGlobalOutOfOrderMessage active"));
                // Let our parser deny all further commands
                emit ApplyPressed();
                break;
            case QMessageBox::Cancel:
                // Immediate command return with error & message
                emit OperationFinish(true,QStringLiteral("Cancel pressed"));
                break;
            default:
                break;
            }
            break;
        }
        case CMD_EXAMPLE_DELAY:
            // keep param for command return
            m_iDelayRequested = params[0].toInt();
            m_DelayTimer.start(1000*m_iDelayRequested);
            break;
        case CMD_EXAMPLE_HELLO_WORLD_RETURN:
            emit OperationFinish(false, QStringLiteral("Hello World"));
            break;
    }
}

void CmdHandlerExample::OnDelayTimer()
{
    // over and out
    emit OperationFinish(false, QStringLiteral("%1").arg(m_iDelayRequested));
}
