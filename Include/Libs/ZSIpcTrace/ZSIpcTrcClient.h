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

#ifndef ZSIpcTrace_Client_h
#define ZSIpcTrace_Client_h

#include "ZSIpcTrace/ZSIpcTrcDllMain.h"
#include "ZSIpc/ZSIpcClient.h"
#include "ZSSys/ZSSysTrcServer.h"

class QMutex;

namespace ZS
{
namespace Ipc
{
struct SSocketDscr;
}

namespace Trace
{
class CIdxTreeTrcAdminObjs;
class CTrcAdminObj;
class CTrcMthFile;


//******************************************************************************
class ZSIPCTRACEDLL_API CIpcTrcClient : public ZS::Ipc::CClient
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CIpcTrcClient(
        const QString& i_strName,
        ETraceDetailLevelMethodCalls i_eTrcMthFileDetailLevel = ETraceDetailLevelMethodCalls::None,
        ETraceDetailLevelMethodCalls i_eTrcMthFileDetailLevelMutex = ETraceDetailLevelMethodCalls::None,
        ETraceDetailLevelMethodCalls i_eTrcMthFileDetailLevelGateway = ZS::Trace::ETraceDetailLevelMethodCalls::None );
    virtual ~CIpcTrcClient();
signals: // on receiving trace data
    void traceSettingsChanged( QObject* i_pTrcClient );
    void traceDataReceived( QObject* i_pTrcClient, const QString& i_str );
    void traceAdminObjInserted( QObject* i_pTrcClient, const QString& i_strKeyInTree );
public: // instance methods
    CIdxTreeTrcAdminObjs* getTraceAdminObjIdxTree() { return m_pTrcAdminObjIdxTree; }
public: // overridables of the remote connection
    virtual ZS::System::CRequest* connect_( int i_iTimeout_ms = 0, bool i_bWait = false, qint64 i_iReqIdParent = -1 ) override;
public: // instance methods to read remote application settings
    QString getRemoteApplicationName() const;
    QString getRemoteServerName() const;
public: // instance methods to set and read trace settings of the server
    STrcServerSettings getTraceSettings() const;
    void setTraceSettings( const STrcServerSettings& i_settings );
protected: // instance methods to send admin objects to the connected server
    void sendAdminObj(
        ZS::System::MsgProtocol::TSystemMsgType i_systemMsgType,
        ZS::System::MsgProtocol::TCommand       i_cmd,
        CTrcAdminObj*                           i_pTrcAdminObj );
    void sendNameSpace(
        ZS::System::MsgProtocol::TSystemMsgType i_systemMsgType,
        ZS::System::MsgProtocol::TCommand       i_cmd,
        ZS::System::CIdxTreeEntry*              i_pBranch,
        ZS::System::EEnabled                    i_enabled,
        ETraceDetailLevelMethodCalls            i_eDetailLevelMethodCalls,
        ETraceDetailLevelRuntimeInfo            i_eDetailLevelRuntimeInfo );
protected: // overridables of base class CClient
    virtual void onReceivedData( const QByteArray& i_byteArr ) override;
protected slots: // connected to the signals of the IPC client
    void onIpcClientConnected( QObject* i_pClient );
    void onIpcClientDisconnected( QObject* i_pClient );
protected slots: // connected to the slots of the trace admin object pool model
    void onTrcAdminObjIdxTreeEntryChanged( ZS::System::CIdxTree* i_pIdxTree, ZS::System::CIdxTreeEntry* i_pTreeEntry );
protected: // instance methods
    void resetTrcAdminRefCounters( ZS::System::CIdxTreeEntry* i_pBranch );
protected: // instance members
    /*!< When connecting to the trace server the name of the application is
         sent together with other settings by the trace server to the client. */
    QString m_strRemoteApplicationName;
    /*!< When connecting to the trace server the name of the trace server is
         sent together with other settings by the trace server to the client. */
    QString m_strRemoteServerName;
    /*!< When connecting to the trace server the settings are queried by the client
         from the trace server. The response is stored in this member. */
    STrcServerSettings m_trcServerSettings;
    /*!< The received trace admin objects are inserted into this index tree. */
    CIdxTreeTrcAdminObjs* m_pTrcAdminObjIdxTree;
    /*!< This flag is set to true if the client receives data and onReceivedData is in progress updating
         the clients data with the settings read from the remote application. If the settings are updated
         the signal traceSettingsReceived is not emitted before all settings have been applied. If this
         flag is set the client knows that the settings are changed by the server and that the
         data must not be send back to the remote server. */
    bool m_bOnReceivedDataUpdateInProcess;

}; // class CIpcTrcClient

} // namespace Trace

} // namespace ZS

#endif // #ifndef ZSIpcTrace_Client_h
