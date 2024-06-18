#include "test_testcompleteserver.h"
#include "tasksimplesendreceive.h"
#include "taskopenconnection.h"
#include <timemachinefortest.h>
#include <timerfactoryqtfortest.h>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_testcompleteserver)

constexpr int testTimeout = 2000;
constexpr int testPort = 4712;

void test_testcompleteserver::init()
{
    TimerFactoryQtForTest::enableTest();
    TimeMachineForTest::reset();
}

void test_testcompleteserver::cleanup()
{
    m_clientSocket = nullptr;
    m_simpleServer = nullptr;
    TimeMachineObject::feedEventLoop();
}

void test_testcompleteserver::failInvalidSocket()
{
    TaskTemplatePtr task = TaskSimpleSendReceive::create(std::make_shared<QTcpSocket>(), "FooCmd", testTimeout);
    QSignalSpy spy(task.get(), &TaskTemplate::sigFinish);
    task->start();
    TimeMachineObject::feedEventLoop();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][0], false);
}

void test_testcompleteserver::failInvalidCommand()
{
    setupServer();
    m_simpleServer->addCmd("TestCmd", CmdParamTypeIdList(), TestCmdIoCompleteServer::RESULT_OK, "");
    setupOpenClient();

    TaskTemplatePtr taskSendReceive = TaskSimpleSendReceive::create(m_clientSocket, "FooCmd", testTimeout);
    QSignalSpy spy(taskSendReceive.get(), &TaskSimpleSendReceive::sigFinish);
    taskSendReceive->start();
    TimeMachineObject::feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][0], false);
}

void test_testcompleteserver::passCommand()
{
    setupServer();
    m_simpleServer->addCmd("TestCmd", CmdParamTypeIdList(), TestCmdIoCompleteServer::RESULT_OK, "");
    setupOpenClient();

    TaskTemplatePtr taskSendReceive = TaskSimpleSendReceive::create(m_clientSocket, "TestCmd", testTimeout);
    QSignalSpy spy(taskSendReceive.get(), &TaskSimpleSendReceive::sigFinish);
    taskSendReceive->start();
    TimeMachineObject::feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][0], true);
}

void test_testcompleteserver::failParser()
{
    setupServer();
    m_simpleServer->addCmd("TestCmd", CmdParamTypeIdList(), TestCmdIoCompleteServer::RESULT_ERROR_PARSER, "42");
    setupOpenClient();

    TaskTemplatePtr taskSendReceive = TaskSimpleSendReceive::create(m_clientSocket, "TestCmd", testTimeout);
    QSignalSpy spy(taskSendReceive.get(), &TaskSimpleSendReceive::sigFinish);
    taskSendReceive->start();
    TimeMachineObject::feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][0], false);
}

void test_testcompleteserver::failHandler()
{
    setupServer();
    m_simpleServer->addCmd("TestCmd", CmdParamTypeIdList(), TestCmdIoCompleteServer::RESULT_ERROR_HANDLER, "42");
    setupOpenClient();

    TaskTemplatePtr taskSendReceive = TaskSimpleSendReceive::create(m_clientSocket, "TestCmd", testTimeout);
    QSignalSpy spy(taskSendReceive.get(), &TaskSimpleSendReceive::sigFinish);
    taskSendReceive->start();
    TimeMachineObject::feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][0], false);
}

void test_testcompleteserver::setupServer()
{
    m_simpleServer = std::make_unique<TestCmdIoCompleteServer>(testPort);

}

void test_testcompleteserver::setupOpenClient()
{
    m_clientSocket = std::make_shared<QTcpSocket>();
    TaskTemplatePtr taskOpen = TaskOpenConnection::create(m_clientSocket, "127.0.0.1", testPort, testTimeout);
    taskOpen->start();
    TimeMachineObject::feedEventLoop();
    QVERIFY(m_clientSocket->isOpen());
}
