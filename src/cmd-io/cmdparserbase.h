#ifndef CmdParserBase_H
#define CmdParserBase_H

#include <QIODevice>
#include <QVariant>
#include <QTimer>

#define END_STR '\n'
#define END_IGNORE '\r'

// forwards
class CmdParserBase;
class CmdHandlerBase;

// data types available for params
enum CmdParamTypeIds
{
    PARAM_TYPE_BOOL = 0,
    PARAM_TYPE_INT,
    PARAM_TYPE_FLOAT,
    PARAM_TYPE_STRING
};

typedef QList<enum CmdParamTypeIds> CmdParamTypeIdList;

// All devices handle GetStatus in the same manner so define param here globally
enum enStatusQueries
{
    STATUS_QUERY_ERROR = 0,
    STATUS_QUERY_WARNING,

    STATUS_QUERY_COUNT
};


// command parsing/handling data
class CmdData
{
public:
    CmdData(QString strDisplay, CmdParamTypeIdList ListParams, int CmdID, bool bLogCmd);
    int GetCmdID();
    QString GetDisplayStr();
private:
    QString m_strDisplay;
    CmdParamTypeIdList m_ListParams;
    int m_CmdID;
    bool m_bLogCmd;
    friend class CmdParserBase;
};


class CmdParserBase : public QObject
{
    Q_OBJECT
public:
    explicit CmdParserBase(QObject *parent = 0);
    void AddCmdInfo(QString strCmd, CmdParamTypeIdList paramList, int iCmdID, bool bLogCmd=true); /* setup commands */
    const QString ParseAndStartCmd(QString strCmd, QIODevice *pCookie); /* return non-empty string if finished */
    void SetCmdHandler(CmdHandlerBase* pCmdHandler);
    void SetGlobalOutOfOrderMessage(QString strMessage, bool bKillAfterNotification);
    virtual QString GetParserName() { return QLatin1String("unknown"); }
    void SetListenPort(quint16 ui16IPPort);
    quint16 GetListenPort();
    // One for all
    static void SetCmdLog(bool bFullLog);
    bool GetCmdLog();
signals:
    void CmdFinish(QString strCmdResponse, QIODevice *pCookie); /* external response on command */
public slots:
    void OnOperationFinish(bool bError, QString strReturn);         /* connected to CmdHandlerBase::OperationFinish */
protected:
    virtual const QString PlausiCheck(CmdData *pCmd, const QVariantList &params);   /* return non-empty string for error */
    virtual const QString PlausiGetStatusCmd(CmdData *pCmd, const QVariantList &params);           /* common implementation */
    const QString FormatErrorMsg(QString strCmd, QString strErrDescription);
    bool isValidHexValue(QString strParam, int iMaxLen);

private:
    void OnKillTimer();

    QHash<QString, CmdData*> m_cmdList;         /* CmdName hashes param information */
    const CmdData *m_pCurrCmd;                  /* used for command response and busy indicator */
    QIODevice* m_pCookie;                       /* Identifies Client Socket */
    CmdHandlerBase* m_pCmdHandler;              /* The work is done in the handler */
    QString m_strGlobalOutOfOrderMessage;       /* If this string is not empty, all commands will fail with this message */
    bool m_bKillAfterNotification;              /* If m_strGlobalOutOfOrderMessage was send - exit appliation */
    quint16 m_ui16IPPort;                       /* Keep port number for logging */

    bool m_bLogCmd;
    static bool m_bLogCmdGlobal;

    QTimer m_KillTimer;
};

#endif // CmdParserBase_H
