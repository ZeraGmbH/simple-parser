#ifndef SIMPLECMDPARSERBASE_H
#define SIMPLECMDPARSERBASE_H

#include <QIODevice>
#include <QVariant>
#include <QTimer>

#define END_STR QLatin1String("\n")
#define END_IGNORE QLatin1String("\r")

// forwards
class SimpleCmdParserBase;
class SimpleCmdHandlerBase;

// data types available for params
enum CmdParamTypeIds
{
    PARAM_TYPE_BOOL = 0,
    PARAM_TYPE_INT,
    PARAM_TYPE_FLOAT,
    PARAM_TYPE_STRING
};

typedef QList<enum CmdParamTypeIds> CmdParamTypeIdList_t;

// command parsing/handling data
class SimpleCmdData
{
public:
    SimpleCmdData(QString strDisplay, CmdParamTypeIdList_t ListParams, int CmdID, bool bLogCmd);
    int GetCmdID();
    QString GetDisplayStr();
private:
    QString m_strDisplay;
    CmdParamTypeIdList_t m_ListParams;
    int m_CmdID;
    bool m_bLogCmd;
    friend class SimpleCmdParserBase;
};


class SimpleCmdParserBase : public QObject
{
    Q_OBJECT
public:
    explicit SimpleCmdParserBase(QObject *parent = 0);
    void AddCmdInfo(QString strCmd, CmdParamTypeIdList_t paramList, int iCmdID, bool bLogCmd=true); /* setup commands */
    const QString ParseAndStartCmd(QString strCmd, QIODevice *pCookie); /* return non-empty string if finished */
    void SetCmdHandler(SimpleCmdHandlerBase* pCmdHandler);
    void SetGlobalOutOfOrderMessage(QString strMessage);
    virtual QString GetParserName() { return QLatin1String("unknown"); }
    // One for all
    static void SetCmdLogGlobal(bool bLogCmdGlobal);                /* Shut up all logging */
    bool GetCmdLog();                                               /* Global enable && per command enable log */
signals:
    void CmdFinish(QString strCmdResponse, QIODevice *pCookie);     /* external response on command */
    void GlobalOutOfOrder();                                        /* external out of order notification */
public slots:
    void OnOperationFinish(bool bError, QString strReturn);         /* connected to SimpleCmdHandlerBase::OperationFinish */
protected:
    virtual const QString PlausiCheck(SimpleCmdData *pCmd, const QVariantList &params);   /* return non-empty string for error */
    const QString FormatErrorMsg(QString strCmd, QString strErrDescription);
    bool isValidHexValue(QString strParam, int iMaxLen);

private:
    QHash<QString, SimpleCmdData*> m_cmdList;         /* CmdName hashes param information */
    const SimpleCmdData *m_pCurrCmd;                  /* used for command response and busy indicator */
    QIODevice* m_pCookie;                       /* Identifies Client Socket */
    SimpleCmdHandlerBase* m_pCmdHandler;        /* The work is done in the handler */
    QString m_strGlobalOutOfOrderMessage;       /* If this string is not empty, all commands will fail with this message */
    bool m_bNotifyAfterGlobalOutOfOrder;        /* If m_strGlobalOutOfOrderMessage was send - exit appliation */

    bool m_bLogCmd;                             /* Working variable to ensure command parse errors are logged - even if commands don't want log */
    static bool m_bLogCmdGlobal;                /* Set this to false for shutup all parsers */
};

// class SimpleCmdParserSocketBase
class SimpleCmdParserSocketBase : public SimpleCmdParserBase
{
    Q_OBJECT
public:
    SimpleCmdParserSocketBase();
    void SetListenPort(quint16 ui16IPPort);
    quint16 GetListenPort();
private:
    quint16 m_ui16IPPort;
};


#endif // SIMPLECMDPARSERBASE_H
