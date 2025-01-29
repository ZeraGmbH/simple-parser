#include "simplecmdioclient.h"
#include "taskopenconnection.h"
#include "tasksimplesendreceive.h"
#include "tasksimplesendreceivetransparent.h"
#include "tasklambdarunner.h"

SimpleCmdIoClient::SimpleCmdIoClient(QString ip, int portNo, int timeout) :
    m_ip(ip),
    m_portNo(portNo),
    m_timeout(timeout)
{
}

void SimpleCmdIoClient::startCmd(QString cmd)
{
    prepareIoTask();
    addOpenConnectionTask();
    addSendReceiveTask(TaskSimpleSendReceive::create(m_socket, cmd, m_timeout));
    addCloseConnectionTask();
    m_tasks->start();
}

void SimpleCmdIoClient::startCmdTransparent(const QString &cmd, std::shared_ptr<QString> response)
{
    prepareIoTask();
    addOpenConnectionTask();
    addSendReceiveTask(TaskSimpleSendReceiveTransparent::create(m_socket, cmd, response, m_timeout));
    addCloseConnectionTask();
    m_tasks->start();
}

void SimpleCmdIoClient::prepareIoTask()
{
    m_socket = std::make_shared<QTcpSocket>();
    m_tasks = TaskContainerSequence::create();
}

void SimpleCmdIoClient::addOpenConnectionTask()
{
    TaskTemplatePtr openConnection = TaskOpenConnection::create(m_socket, m_ip, m_portNo, m_timeout);
    m_tasks->addSub(std::move(openConnection));
}

void SimpleCmdIoClient::addSendReceiveTask(TaskTemplatePtr task)
{
    m_tasks->addSub(std::move(task));
    connect(m_tasks.get(), &TaskContainerSequence::sigFinish,
            this, &SimpleCmdIoClient::sigCmdFinish);
}

void SimpleCmdIoClient::addCloseConnectionTask()
{
    TaskTemplatePtr closeConnection = TaskLambdaRunner::create([this]() {
        m_socket->close();
        return true;
    });
    m_tasks->addSub(std::move(closeConnection));
}
