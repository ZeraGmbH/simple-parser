#include "tasksimplesendreceivetransparent.h"
#include <taskdecoratortimeout.h>

#define END_STR '\n'

TaskTemplatePtr TaskSimpleSendReceiveTransparent::create(std::shared_ptr<QTcpSocket> socket,
                                                         const QString &cmd,
                                                         std::shared_ptr<QString> response,
                                                         int timeout,
                                                         std::function<void ()> additionalErrorHandler)
{
    return TaskDecoratorTimeout::wrapTimeout(timeout,
                                             std::make_unique<TaskSimpleSendReceiveTransparent>(socket, cmd, response),
                                             additionalErrorHandler);

}

TaskSimpleSendReceiveTransparent::TaskSimpleSendReceiveTransparent(std::shared_ptr<QTcpSocket> socket,
                                                                   const QString &cmd,
                                                                   std::shared_ptr<QString> response) :
    m_socket(socket),
    m_cmd(cmd),
    m_serverResponse(response)
{
}

void TaskSimpleSendReceiveTransparent::start()
{
    if(!m_socket->isOpen()) {
        finishTask(false);
        return;
    }
    m_socket->write(m_cmd.toLatin1() + END_STR);
    connect(m_socket.get(), &QTcpSocket::readyRead, this, &TaskSimpleSendReceiveTransparent::onReadyRead);
}

void TaskSimpleSendReceiveTransparent::onReadyRead()
{
    m_serverResponse->append(m_socket->readAll());
    if(m_serverResponse->endsWith(END_STR))
        finishTask(true);
}
