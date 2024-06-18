#ifndef TASKSIMPLESENDRECEIVE_H
#define TASKSIMPLESENDRECEIVE_H

#include <tasktemplate.h>
#include <QTcpSocket>
#include <memory>

class TaskSimpleSendReceive : public TaskTemplate
{
    Q_OBJECT
public:
    static TaskTemplatePtr create(std::shared_ptr<QTcpSocket> socket, QString cmd, int timeout, std::function<void()> additionalErrorHandler = []{});
    TaskSimpleSendReceive(std::shared_ptr<QTcpSocket> socket, QString cmd);
    void start() override;
private slots:
    void onReadyRead();
private:
    void decodeResponse();
    std::shared_ptr<QTcpSocket> m_socket;
    QString m_cmd;
    QString m_response;
};

#endif // TASKSIMPLESENDRECEIVE_H
