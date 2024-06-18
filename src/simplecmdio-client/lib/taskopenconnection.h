#ifndef TASKOPENCONNECTION_H
#define TASKOPENCONNECTION_H

#include <tasktemplate.h>
#include <QTcpSocket>
#include <memory>

class TaskOpenConnection : public TaskTemplate
{
    Q_OBJECT
public:
    static TaskTemplatePtr create(std::shared_ptr<QTcpSocket> socket, QString ipAddress, int port, int timeout, std::function<void()> additionalErrorHandler = []{});
    TaskOpenConnection(std::shared_ptr<QTcpSocket> socket, QString ipAddress, int port);
    virtual void start() override;
private:
    std::shared_ptr<QTcpSocket> m_socket;
    QString m_ipAddress;
    int m_port;
};

#endif // TASKOPENCONNECTION_H
