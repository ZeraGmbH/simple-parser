#include "test_task_simpleio.h"
#include "tasksimplesendreceive.h"
#include "taskopenconnection.h"
#include <timemachinefortest.h>
#include <timerfactoryqtfortest.h>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_task_simpleio)

constexpr int testTimeout = 2000;
constexpr int testPort = 4712;

void test_task_simpleio::init()
{
    TimerFactoryQtForTest::enableTest();
    TimeMachineForTest::reset();
}

void test_task_simpleio::cleanup()
{
    m_clientSocket = nullptr;
    m_simpleServer = nullptr;
    TimeMachineObject::feedEventLoop();
}

void test_task_simpleio::failInvalidSocket()
{
    TaskTemplatePtr task = TaskSimpleSendReceive::create(std::make_shared<QTcpSocket>(), "FooCmd", testTimeout);
    QSignalSpy spy(task.get(), &TaskTemplate::sigFinish);
    task->start();
    TimeMachineObject::feedEventLoop();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][0], false);
}

void test_task_simpleio::failInvalidCommand()
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

void test_task_simpleio::passCommand()
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

void test_task_simpleio::failParser()
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

void test_task_simpleio::failHandler()
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

void test_task_simpleio::setupServer()
{
    m_simpleServer = std::make_unique<TestCmdIoCompleteServer>(testPort);

}

void test_task_simpleio::setupOpenClient()
{
    m_clientSocket = std::make_shared<QTcpSocket>();
    TaskTemplatePtr taskOpen = TaskOpenConnection::create(m_clientSocket, "127.0.0.1", testPort, testTimeout);
    taskOpen->start();
    TimeMachineObject::feedEventLoop();
    QVERIFY(m_clientSocket->isOpen());
}
