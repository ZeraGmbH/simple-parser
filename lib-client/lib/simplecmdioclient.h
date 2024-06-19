#ifndef SIMPLECMDIOCLIENT_H
#define SIMPLECMDIOCLIENT_H

#include <QObject>

class SimpleCmdIoClient : public QObject
{
    Q_OBJECT
public:
    explicit SimpleCmdIoClient(int portNo);
    void startCmd(QString cmd);
signals:
    void sigCmdFinish(bool ok);
private:
    int m_portNo;
};

#endif // SIMPLECMDIOCLIENT_H
