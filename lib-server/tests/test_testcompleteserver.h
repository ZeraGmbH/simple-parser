#ifndef TEST_TASK_SIMPLEIO_H
#define TEST_TASK_SIMPLEIO_H

#include "testcmdiocompleteserver.h"
#include <memory>

class test_testcompleteserver : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();
    void failInvalidSocket();
    void failInvalidCommand();
    void passCommand();
    void failParser();
    void failHandler();
private:
    void setupServer();
    void setupOpenClient();
    std::unique_ptr<TestCmdIoCompleteServer> m_simpleServer;
    std::shared_ptr<QTcpSocket> m_clientSocket;
};

#endif // TEST_TASK_SIMPLEIO_H
