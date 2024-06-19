#include "simplecmdioclient.h"
#include "taskopenconnection.h"
#include "tasksimplesendreceive.h"
#include "tasklambdarunner.h"

SimpleCmdIoClient::SimpleCmdIoClient(QString ip, int portNo, int timeout) :
    m_ip(ip),
    m_portNo(portNo),
    m_timeout(timeout)
{
}

void SimpleCmdIoClient::startCmd(QString cmd)
{
    m_socket = std::make_shared<QTcpSocket>();
    m_tasks = TaskContainerSequence::create();

    TaskTemplatePtr openConnection = TaskOpenConnection::create(m_socket, m_ip, m_portNo, m_timeout);
    m_tasks->addSub(std::move(openConnection));

    TaskTemplatePtr sendCmd = TaskSimpleSendReceive::create(m_socket, cmd, m_timeout);
    m_tasks->addSub(std::move(sendCmd));

    TaskTemplatePtr closeConnection = TaskLambdaRunner::create([this]() {
        m_socket->close();
        return true;
    });
    m_tasks->addSub(std::move(closeConnection));

    connect(m_tasks.get(), &TaskContainerSequence::sigFinish,
            this, &SimpleCmdIoClient::sigCmdFinish);

    m_tasks->start();
}
