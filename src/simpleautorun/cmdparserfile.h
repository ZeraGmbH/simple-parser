#ifndef CMDPARSERFILE_H
#define CMDPARSERFILE_H

#include "cmdparserbase.h"
#include "cmdhandlerfile.h"

enum CmdFILEIOCmdIds
{
    CMD_FILE_OPEN_IPPORT = 1,
    CMD_FILE_SELECT_IPCONNECTION,
    CMD_FILE_WAIT_MS,
    CMD_FILE_SET_TAG,
    CMD_FILE_LOOP_TAG,
    CMD_FILE_SET_ON_ERROR_STOP,
    CMD_FILE_CHECK_LAST_RESPONSE,
    CMD_FILE_MESSAGE,

};

class CmdParserFile : public QSimpleCmdParserBase
{
    Q_OBJECT
public:
    explicit CmdParserFile(QObject *parent = nullptr);
    virtual QString GetParserName() { return QStringLiteral("FILE"); }
    void StartFileExecution(QString strFileName, CmdHandlerFile *pCmdHandlerFile);

signals:
    void done(int exitCode);
    void next();

protected:
    void OnExecNext();
    void OnFileCmdFinish(QString strCmdResponse, QIODevice *pCookie);

    virtual const QString PlausiCheck(SimpleCmdData *pCmd, const QVariantList &params);

private:
    CmdHandlerFile *m_pCmdHandlerFile;

    QStringList m_strCmdList;
    QStringList::iterator m_iterCurrLine;
    bool m_bAllowErrors;
    int m_iCurrCmdID;
};

#endif // CMDPARSERFILE_H
