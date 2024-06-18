#include "taskopenconnection.h"
#include <taskdecoratortimeout.h>

TaskTemplatePtr TaskOpenConnection::create(std::shared_ptr<QTcpSocket> socket, QString ipAddress, int port, int timeout, std::function<void ()> additionalErrorHandler)
{
    return TaskDecoratorTimeout::wrapTimeout(timeout,
                                             std::make_unique<TaskOpenConnection>(socket, ipAddress, port),
                                             additionalErrorHandler);
}

TaskOpenConnection::TaskOpenConnection(std::shared_ptr<QTcpSocket> socket, QString ipAddress, int port) :
    m_socket(socket),
    m_ipAddress(ipAddress),
    m_port(port)
{
}

void TaskOpenConnection::start()
{
    connect(m_socket.get(), &QTcpSocket::connected,
            this, [this](){ emit sigFinish(true, getTaskId()); });
    connect(m_socket.get(), &QTcpSocket::errorOccurred,
            this, [this](){ emit sigFinish(false, getTaskId()); });
    m_socket->connectToHost(m_ipAddress, m_port);
}
