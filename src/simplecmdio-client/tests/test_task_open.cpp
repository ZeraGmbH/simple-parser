#include "test_task_open.h"
#include "taskopenconnection.h"
#include <timemachinefortest.h>
#include <timerfactoryqtfortest.h>
#include <QTcpServer>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_task_open)

constexpr int testTimeout = 2000;
constexpr int testPort = 4712;

void test_task_open::init()
{
    TimerFactoryQtForTest::enableTest();
    TimeMachineForTest::reset();
}

void test_task_open::openFailNoServerLocal()
{
    TaskTemplatePtr task = TaskOpenConnection::create(std::make_shared<QTcpSocket>(), "127.0.0.1", testPort, testTimeout);
    QSignalSpy spy(task.get(), &TaskOpenConnection::sigFinish);
    task->start();
    TimeMachineForTest::getInstance()->processTimers(testTimeout);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][0], false);
}

void test_task_open::openFailNoServerForeign()
{
    TaskTemplatePtr task = TaskOpenConnection::create(std::make_shared<QTcpSocket>(), "192.168.6.3", testPort, testTimeout);
    QSignalSpy spy(task.get(), &TaskOpenConnection::sigFinish);
    task->start();
    TimeMachineForTest::getInstance()->processTimers(testTimeout);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][0], false);
}

void test_task_open::openSuccess()
{
    QTcpServer server;
    server.listen(QHostAddress::LocalHost, testPort);
    TimeMachineObject::feedEventLoop();

    TaskTemplatePtr task = TaskOpenConnection::create(std::make_shared<QTcpSocket>(), "127.0.0.1", testPort, testTimeout);
    QSignalSpy spy(task.get(), &TaskOpenConnection::sigFinish);
    task->start();
    TimeMachineForTest::getInstance()->processTimers(testTimeout);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][0], true);
}
