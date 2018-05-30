#include "cmdparserexample.h"
#include "cmdhandlerexample.h"
#include "cmdserver.h"
#include <QApplication>
#include <QCommandLineParser>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);

    QCommandLineParser cmdLineParser;
    cmdLineParser.setApplicationDescription("simpleexample");
    cmdLineParser.addHelpOption();

    // command line parameters
    QCommandLineOption optPortNo(QStringList() << "p" << "port", "IP-Port server opens (default: 5000)", "ip-port");
    optPortNo.setDefaultValue("5000");
    cmdLineParser.addOption(optPortNo);
    QCommandLineOption optVerboseLevel(QStringList() << "v" << "verbose", "Verbose level (default: 0)", "verbose-level");
    optVerboseLevel.setDefaultValue("0");
    cmdLineParser.addOption(optVerboseLevel);

    // process command line
    cmdLineParser.process(a);
    QString strPorNo = cmdLineParser.value(optPortNo);
    if(strPorNo.isEmpty())
        cmdLineParser.showHelp(-1);
    bool bConversionOK = true;
    int iPortNo = strPorNo.toInt(&bConversionOK);
    if(!bConversionOK)
        cmdLineParser.showHelp(-1);
    QString strVerboseLevel = cmdLineParser.value(optVerboseLevel);
    int iVerboseLevel = 0;
    if(!strVerboseLevel.isEmpty())
    {
        iVerboseLevel = strVerboseLevel.toInt(&bConversionOK);
        if(!bConversionOK)
            cmdLineParser.showHelp(-1);
    }

    // parser/handler/server objects
    CmdParserExample parserExample;
    CmdHandlerExample handlerExample;
    parserExample.SetCmdHandler(&handlerExample);
    QObject::connect(&handlerExample, &CmdHandlerExample::ApplyPressed, &parserExample, &CmdParserExample::OnApplyPressed);

    // Log Level
    QSimpleCmdParserSocketBase::SetCmdLogGlobal(iVerboseLevel >= 1);

    // Server
    QSimpleCmdServer serverExample;
    if(serverExample.Setup(iPortNo, &parserExample))
    {
        qInfo("Opened Example I/O server on port %u", iPortNo);
        qInfo("To start commands open another shell and enter telnet localhost %u", iPortNo);
        qInfo("In telnet session enter 'help<CR>' to get information on commands available");
    }
    else
        qWarning("FPGA I/O server does not listen on %u", iPortNo);

    return a.exec();
}
