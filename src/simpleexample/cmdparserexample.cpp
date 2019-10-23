#include "cmdparserexample.h"

CmdParserExample::CmdParserExample(QObject *parent) : QSimpleCmdParserSocketBase(parent)
{
    /* ExampleMessage: Open a message box */
    /* Parameters: strMessage */
    /* Result: Empty / error on Cancel */
    AddCmdInfo("ExampleMessage",
               CmdParamTypeIdList()  << PARAM_TYPE_STRING,
               CMD_EXAMPLE_MSG,
               true
               // Just output default help
               );

    /* ExampleDelay: Delay */
    /* Parameters: iDelay <= 10 */
    /* Result: iDelay */
    AddCmdInfo("ExampleDelay",
               CmdParamTypeIdList() << PARAM_TYPE_INT,
               CMD_EXAMPLE_DELAY,
               true,
               // extend help command output
               QLatin1String("Parameter: iDelay [0;10]\nReturn: iDelay"));

    /* ExampleHelloWorld: Just return "Hello World" */
    /* Parameters: none */
    /* Result: "Hello World" */
    AddCmdInfo("ExampleHelloWorld",
               CmdParamTypeIdList(),
               CMD_EXAMPLE_HELLO_WORLD_RETURN,
               true,
               // extend help command output
               QLatin1String("Parameter: none\nReturn: \"Hello World\""));
}

void CmdParserExample::OnApplyPressed()
{
    SetGlobalOutOfOrderMessage(QLatin1String("Apply was pressed!"));
}

const QString CmdParserExample::PlausiCheck(SimpleCmdData *pCmd, const QVariantList &params)
{
    QString strRet, strErrInfo;
    switch(pCmd->GetCmdID())
    {
    case CMD_EXAMPLE_MSG:
    case CMD_EXAMPLE_HELLO_WORLD_RETURN:
        // No extra param validation
        break;
    case CMD_EXAMPLE_DELAY:
        if(params[0].toInt() < 0 || params[0].toInt() > 10)
            strErrInfo = QStringLiteral("Parameter out of limits: %1 allowed [0;10]").arg(params[0].toInt());
        break;
    }
    if(!strErrInfo.isEmpty())
        strRet = FormatErrorMsg(pCmd->GetDisplayStr(), strErrInfo);
    return strRet;
}
