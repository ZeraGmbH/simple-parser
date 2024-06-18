#include "cmdparserfile.h"
#include "cmdhandlerfile.h"
#include "commonhelpers.h"
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QObject>
#include <QTimer>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("fmtautorun");
    parser.addHelpOption();

    // option execution file
    QCommandLineOption optExecFile(QStringList() << "x" << "exec-file", "fill path execution file", "path");
    parser.addOption(optExecFile);

    // option colour
    QCommandLineOption optColour(QStringList() << "c" << "colour", "coloured output 0/1", "colour");
    parser.addOption(optColour);

    parser.process(a);
    QString strExecFile = parser.value(optExecFile);
    if(strExecFile.isEmpty())
        parser.showHelp(-1);

    QString strColour = parser.value(optColour);
    if(!strColour.isEmpty()) {
        if(strColour!=QStringLiteral("1")) {
            parser.showHelp(-1);
        }
        else {
            gbColourOutput = true;
        }
    }

    CmdParserFile parserFile;
    CmdHandlerFile handlerFile;
    parserFile.SetCmdHandler(&handlerFile);

    // Ensure event loop up
    QTimer::singleShot(300, &parserFile, [&]
                       () {
        QObject::connect(&parserFile, &CmdParserFile::done, &a, &QCoreApplication::exit );
        QObject::connect(&handlerFile, &CmdHandlerFile::kill, &a, &QCoreApplication::exit );

        parserFile.StartFileExecution(strExecFile, &handlerFile);
    });

    return a.exec();
}
