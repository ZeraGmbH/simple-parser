#include "test_simplecmdioclient.h"
#include "simplecmdioclient.h"
#include <timemachinefortest.h>
#include <timerfactoryqtfortest.h>
#include <testloghelpers.h>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_simplecmdioclient)

constexpr int testTimeout = 2000;
constexpr int testPort = 4712;


void test_simplecmdioclient::init()
{
    TimerFactoryQtForTest::enableTest();
    TimeMachineForTest::reset();
}

void test_simplecmdioclient::cleanup()
{
    m_simpleServer = nullptr;
    TimeMachineObject::feedEventLoop();
}

void test_simplecmdioclient::cmdInvalidServer()
{
    SimpleCmdIoClient client("127.0.0.1", testPort, testTimeout);
    QSignalSpy spy(&client, &SimpleCmdIoClient::sigCmdFinish);
    client.startCmd("foo");

    TimeMachineForTest::getInstance()->processTimers(testTimeout);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][0], false);
}

void test_simplecmdioclient::cmdInvalid()
{
    setupServer();
    m_simpleServer->addCmd("foo", CmdParamTypeIdList(), TestCmdIoCompleteServer::RESULT_OK, "42");

    SimpleCmdIoClient client("127.0.0.1", testPort, testTimeout);
    QSignalSpy spy(&client, &SimpleCmdIoClient::sigCmdFinish);
    client.startCmd("bar");

    TimeMachineForTest::getInstance()->processTimers(testTimeout);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][0], false);
}

void test_simplecmdioclient::cmdOk()
{
    setupServer();
    QString testCmd = "TestCmd";
    m_simpleServer->addCmd(testCmd, CmdParamTypeIdList(), TestCmdIoCompleteServer::RESULT_OK, "42");

    SimpleCmdIoClient client("127.0.0.1", testPort, testTimeout);
    QSignalSpy spy(&client, &SimpleCmdIoClient::sigCmdFinish);
    client.startCmd(testCmd);

    TimeMachineForTest::getInstance()->processTimers(testTimeout);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][0], true);
}

void test_simplecmdioclient::cmdTransparentHelp()
{
    setupServer();
    QString testCmd = "TestCmd";
    m_simpleServer->addCmd(testCmd, CmdParamTypeIdList(), TestCmdIoCompleteServer::RESULT_OK, "42");

    SimpleCmdIoClient client("127.0.0.1", testPort, testTimeout);
    QSignalSpy spy(&client, &SimpleCmdIoClient::sigCmdFinish);
    std::shared_ptr<QString> serverResponse = std::make_shared<QString>();
    client.startCmdTransparent("?", serverResponse);

    TimeMachineForTest::getInstance()->processTimers(testTimeout);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][0], true);

    QString expected = TestLogHelpers::loadFile(":/help-response.txt");
    QVERIFY(!expected.isEmpty());
    QVERIFY(TestLogHelpers::compareAndLogOnDiff(expected, *serverResponse));
}

void test_simplecmdioclient::setupServer()
{
    m_simpleServer = std::make_unique<TestCmdIoCompleteServer>(testPort);
}

