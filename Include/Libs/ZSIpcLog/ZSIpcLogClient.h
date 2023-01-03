/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
                         Gewerbepark 28
                         D-83670 Bad Heilbrunn
                         Tel: 0049 8046 9488
                         www.zeussoft.de
                         E-Mail: mailbox@zeussoft.de

--------------------------------------------------------------------------------

Content: This file is part of the ZSQtLib.

This file may be used with no license restrictions for your needs. But it is not
allowed to resell any modules of the ZSQtLib veiling the original developer of
the modules. Therefore the copyright link to ZeusSoft, Ing. Buero Bauer must not
be removed from the header of the source code modules.

ZeusSoft, Ing. Buero Bauer provides the source code as is without any guarantee
that the code is written without faults.

ZeusSoft, Ing. Buero Bauer does not assume any liability for any damages which
may result in using the software modules.

*******************************************************************************/

#ifndef ZSIpcLog_Client_h
#define ZSIpcLog_Client_h

#include "ZSIpcLog/ZSIpcLogDllMain.h"
#include "ZSIpc/ZSIpcClient.h"
#include "ZSSys/ZSSysLogServer.h"

class QMutex;

namespace ZS
{
namespace System
{
class CIdxTreeLoggers;
class CLogger;
}
namespace Ipc
{
struct SSocketDscr;
}

namespace Log
{
//******************************************************************************
class ZSIPCLOGDLL_API CIpcLogClient : public ZS::Ipc::CClient
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CIpcLogClient(const QString& i_strName);
    virtual ~CIpcLogClient();
signals: // on receiving log data
    void logSettingsChanged( QObject* i_pTrcClient );
    void logDataReceived( QObject* i_pTrcClient, const QString& i_str );
    void loggerInserted( QObject* i_pTrcClient, const QString& i_strKeyInTree );
    void loggerChanged( QObject* i_pTrcClient, const QString& i_strKeyInTree );
public: // instance methods
    ZS::System::CIdxTreeLoggers* getLoggersIdxTree() { return m_pLoggersIdxTree; }
public: // overridables of the remote connection
    virtual ZS::System::CRequest* connect_( int i_iTimeout_ms = 0, bool i_bWait = false, qint64 i_iReqIdParent = -1 ) override;
public: // instance methods to read remote application settings
    QString getRemoteApplicationName() const;
    QString getRemoteServerName() const;
public: // instance methods to set and read log settings of the server
    ZS::System::SLogServerSettings getLogSettings() const;
    void setLogSettings( const ZS::System::SLogServerSettings& i_settings );
protected: // instance methods to send index tree entries to the connected server
    void sendBranch(
        ZS::System::MsgProtocol::TSystemMsgType  i_systemMsgType,
        ZS::System::MsgProtocol::TCommand        i_cmd,
        ZS::System::CIdxTreeEntry*               i_pBranch,
        ZS::System::EEnabled                     i_enabled,
        ZS::System::ELogDetailLevel              i_eDetailLevel,
        const QString&                           i_strDataFilter );
    void sendLeave(
        ZS::System::MsgProtocol::TSystemMsgType i_systemMsgType,
        ZS::System::MsgProtocol::TCommand       i_cmd,
        ZS::System::CLogger*                    i_pLogger );
protected: // overridables of base class CClient
    virtual void onReceivedData( const QByteArray& i_byteArr ) override;
protected slots: // connected to the signals of the IPC client
    void onIpcClientConnected( QObject* i_pClient );
    void onIpcClientDisconnected( QObject* i_pClient );
protected slots: // connected to the slots of the loggers idx tree
    void onLoggersIdxTreeEntryChanged( const QString& i_strKeyInTree );
protected: // instance members
    /*!< When connecting to the log server the name of the application is
         sent together with other settings by the log server to the client. */
    QString m_strRemoteApplicationName;
    /*!< When connecting to the log server the name of the log server is
         sent together with other settings by the log server to the client. */
    QString m_strRemoteServerName;
    /*!< When connecting to the log server the settings of the log server are
         sent from the server to the client. */
    ZS::System::SLogServerSettings m_logServerSettings;
    /*!< The received loggers are inserted into this index tree. */
    ZS::System::CIdxTreeLoggers* m_pLoggersIdxTree;
    /*!< This flag is set to true if the client receives data and onReceivedData is in progress updating
         the clients data with the settings read from the remote application. If the settings are updated
         the signal logSettingsReceived is not emitted before all settings have been applied. If this
         flag is set the client knows that the settings are changed by the server and that the
         data must not be send back to the remote server. */
    bool m_bOnReceivedDataUpdateInProcess;

}; // class CIpcLogClient

} // namespace Log

} // namespace ZS

#endif // #ifndef ZSIpcLog_Client_h
