#ifndef TEST_TASK_SIMPLEIO_H
#define TEST_TASK_SIMPLEIO_H

#include "testcmdiofacade.h"
#include <QObject>
#include <memory>

// This test tests simplecmdio-testlib too which was created along with
// SimpleCmdIoClient. Maybe we think about a more simple server test
// implementation once SimpleCmdIoClient has matured...

class test_task_simpleio : public QObject
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
    std::unique_ptr<TestCmdIoFacade> m_simpleServer;
    std::shared_ptr<QTcpSocket> m_clientSocket;
};

#endif // TEST_TASK_SIMPLEIO_H
