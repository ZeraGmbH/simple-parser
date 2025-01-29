#include "tasksimplesendreceive.h"
#include <taskdecoratortimeout.h>

#define END_STR '\n'

TaskTemplatePtr TaskSimpleSendReceive::create(std::shared_ptr<QTcpSocket> socket, QString cmd, int timeout, std::function<void ()> additionalErrorHandler)
{
    return TaskDecoratorTimeout::wrapTimeout(timeout,
                                             std::make_unique<TaskSimpleSendReceive>(socket, cmd),
                                             additionalErrorHandler);
}

TaskSimpleSendReceive::TaskSimpleSendReceive(std::shared_ptr<QTcpSocket> socket, QString cmd) :
    m_socket(socket),
    m_cmd(cmd)
{
}

void TaskSimpleSendReceive::start()
{
    if(!m_socket->isOpen()) {
        finishTask(false);
        return;
    }
    m_socket->write(m_cmd.toLatin1() + END_STR);
    connect(m_socket.get(), &QTcpSocket::readyRead, this, &TaskSimpleSendReceive::onReadyRead);
}

void TaskSimpleSendReceive::onReadyRead()
{
    m_serverResponse.append(m_socket->readAll());
    if(m_serverResponse.endsWith(END_STR))
        decodeResponse();
}

void TaskSimpleSendReceive::decodeResponse()
{
    QString bareResponse = m_serverResponse.replace(END_STR, "");
    QStringList responseParts = bareResponse.split(",");
    if(responseParts.count() < 2)
        finishTask(false);

    else if(responseParts[1].toUpper().startsWith("ERROR"))
        finishTask(false);

    else if(responseParts[1].toUpper().startsWith(("OK")))
        finishTask(true);

    else
        finishTask(false);
}
