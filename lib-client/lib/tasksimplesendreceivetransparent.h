#ifndef TASKSIMPLESENDRECEIVETRANSPARENT_H
#define TASKSIMPLESENDRECEIVETRANSPARENT_H

#include <tasktemplate.h>
#include <QTcpSocket>

class TaskSimpleSendReceiveTransparent : public TaskTemplate
{
public:
    static TaskTemplatePtr create(std::shared_ptr<QTcpSocket> socket,
                                  const QString &cmd,
                                  std::shared_ptr<QString> response,
                                  int timeout,
                                  std::function<void()> additionalErrorHandler = []{});
    TaskSimpleSendReceiveTransparent(std::shared_ptr<QTcpSocket> socket, const QString &cmd, std::shared_ptr<QString> response);
    void start() override;
private slots:
    void onReadyRead();
private:
    std::shared_ptr<QTcpSocket> m_socket;
    QString m_cmd;
    std::shared_ptr<QString> m_serverResponse;
};

#endif // TASKSIMPLESENDRECEIVETRANSPARENT_H
