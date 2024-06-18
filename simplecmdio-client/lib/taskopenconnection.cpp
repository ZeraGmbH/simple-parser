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
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    connect<void(QTcpSocket::*)(QAbstractSocket::SocketError)>(m_socket.get(), &QTcpSocket::error, this, [this](QAbstractSocket::SocketError socketError) {
        Q_UNUSED(socketError)
        emit sigFinish(false, getTaskId());
    });
#else
    connect(m_socket.get(), &QTcpSocket::errorOccurred,
            this, [this](){ emit sigFinish(false, getTaskId()); });
#endif
    m_socket->connectToHost(m_ipAddress, m_port);
}
