#ifndef SIMPLECMDPARSERBASE_H
#define SIMPLECMDPARSERBASE_H

#include <QIODevice>
#include <QVariant>
#include <QTimer>

#define END_STR '\n'
#define END_IGNORE '\r'

#include "simplecmdiobase_global.h"

// forwards
class QSimpleCmdParserBase;
class QSimpleCmdHandlerBase;

// data types available for params
enum CmdParamTypeIds
{
    PARAM_TYPE_BOOL = 0,
    PARAM_TYPE_INT,
    PARAM_TYPE_FLOAT,
    PARAM_TYPE_STRING
};

typedef QList<enum CmdParamTypeIds> CmdParamTypeIdList;

// command parsing/handling data
class SIMPLE_CMDIO_EXPORT SimpleCmdData
{
public:
    SimpleCmdData(QString strDisplay, CmdParamTypeIdList ListParams, int CmdID, bool bLogCmd, QString strHelpAdd);
    int GetCmdID();
    QString GetDisplayStr();
private:
    QString m_strDisplay;
    CmdParamTypeIdList m_ListParams;
    int m_CmdID;
    bool m_bLogCmd;
    QString m_strHelpAdd;
    friend class QSimpleCmdParserBase;
};


class SIMPLE_CMDIO_EXPORT QSimpleCmdParserBase : public QObject
{
    Q_OBJECT
public:
    explicit QSimpleCmdParserBase(QObject *parent = nullptr);
    void AddCmdInfo(QString strCmd, CmdParamTypeIdList paramList, int iCmdID, bool bLogCmd=true, QString strHelpAdd = QString()); /* setup commands */
    const QString ParseAndStartCmd(QString strCmd, QIODevice *pCookie); /* return non-empty string if finished */
    void SetCmdHandler(QSimpleCmdHandlerBase* pCmdHandler);
    void SetGlobalOutOfOrderMessage(QString strMessage);
    virtual QString GetParserName() { return QLatin1String("unknown"); }
    static QByteArray BinaryFromAscii(QString strParam);
    static QString BinaryConversionHelpString();
    // One for all
    static void SetCmdLogGlobal(bool bLogCmdGlobal);                /* Shut up all logging */
    bool GetCmdLog();                                               /* Global enable && per command enable log */
signals:
    void CmdFinish(QString strCmdResponse, QIODevice *pCookie);     /* external response on command */
    void GlobalOutOfOrder();                                        /* external out of order notification */
public slots:
    void OnOperationFinish(bool bError, QString strReturn);         /* connected to QSimpleCmdHandlerBase::OperationFinish */
protected:
    virtual const QString PlausiCheck(SimpleCmdData *pCmd, const QVariantList &params);   /* return non-empty string for error */
    const QString FormatErrorMsg(QString strCmd, QString strErrDescription);
    bool isValidHexValue(QString strParam, int iMaxLen);

private:
    QHash<QString, SimpleCmdData*> m_cmdList;         /* CmdName hashes param information */
    const SimpleCmdData *m_pCurrCmd;                  /* used for command response and busy indicator */
    QIODevice* m_pCookie;                       /* Identifies Client Socket */
    QSimpleCmdHandlerBase* m_pCmdHandler;        /* The work is done in the handler */
    QString m_strGlobalOutOfOrderMessage;       /* If this string is not empty, all commands will fail with this message */
    bool m_bNotifyAfterGlobalOutOfOrder;        /* If m_strGlobalOutOfOrderMessage was send - exit appliation */

    bool m_bLogCmd;                             /* Working variable to ensure command parse errors are logged - even if commands don't want log */
    static bool m_bLogCmdGlobal;                /* Set this to false for shutup all parsers */
};

// class QSimpleCmdParserSocketBase
class SIMPLE_CMDIO_EXPORT QSimpleCmdParserSocketBase : public QSimpleCmdParserBase
{
    Q_OBJECT
public:
    explicit QSimpleCmdParserSocketBase(QObject *parent = nullptr);
    void SetListenPort(quint16 ui16IPPort);
    quint16 GetListenPort();
private:
    quint16 m_ui16IPPort;
};


#endif // SIMPLECMDPARSERBASE_H
