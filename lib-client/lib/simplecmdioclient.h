#ifndef SIMPLECMDIOCLIENT_H
#define SIMPLECMDIOCLIENT_H

#include <taskcontainersequence.h>
#include <QObject>
#include <QTcpSocket>

class SimpleCmdIoClient : public QObject
{
    Q_OBJECT
public:
    explicit SimpleCmdIoClient(QString ip, int portNo, int timeout);
    void startCmd(QString cmd);
signals:
    void sigCmdFinish(bool ok);
private:
    QString m_ip;
    int m_portNo;
    int m_timeout;
    std::unique_ptr<TaskContainerInterface> m_tasks;
    std::shared_ptr<QTcpSocket> m_socket;
};

#endif // SIMPLECMDIOCLIENT_H
