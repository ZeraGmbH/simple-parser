#ifndef TEST_SIMPLECMDIOCLIENT_H
#define TEST_SIMPLECMDIOCLIENT_H

#include "testcmdiocompleteserver.h"

#include <QObject>
#include <memory>

class test_simplecmdioclient : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();
    void cmdInvalidServer();
    void cmdInvalid();
    void cmdOk();
    void cmdTransparentHelp();

private:
    void setupServer();
    std::unique_ptr<TestCmdIoCompleteServer> m_simpleServer;
};

#endif // TEST_SIMPLECMDIOCLIENT_H
