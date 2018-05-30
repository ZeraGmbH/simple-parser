#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QObject>
#include <QTimer>
#include "cmdparserfile.h"
#include "cmdhandlerfile.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("fmtautorun");
    parser.addHelpOption();

    // option execution file
    QCommandLineOption optExecFile(QStringList() << "x" << "exec-file", "fill path execution file", "path");
    optExecFile.setDefaultValue("/opt/zera/fmt/autoexec");
    parser.addOption(optExecFile);

    parser.process(a);
    QString strExecFile = parser.value(optExecFile);
    if(strExecFile.isEmpty())
        parser.showHelp(-1);

    CmdParserFile parserFile;
    CmdHandlerFile handlerFile;
    parserFile.SetCmdHandler(&handlerFile);

    // Ensure event loop up
    QTimer::singleShot(300,[&]
                       ()

    {
        QObject::connect(&parserFile, &CmdParserFile::idle, &a, &QCoreApplication::quit );

        parserFile.StartFileExecution(strExecFile, &handlerFile);
    });

    return a.exec();
}
