/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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
class CTrcAdminObj;
class CIdxTreeTrcAdminObjs;

//******************************************************************************
class ZSIPCTRACEDLL_API CIpcTrcClient : public ZS::Ipc::CClient
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CIpcTrcClient( const QString& i_strName );
    virtual ~CIpcTrcClient();
signals: // on receiving trace data
    void traceSettingsChanged( QObject* i_pTrcClient );
    void traceDataReceived( QObject* i_pTrcClient, const QString& i_str );
    void traceAdminObjInserted( QObject* i_pTrcClient, const QString& i_strKeyInTree );
public: // instance methods
    CIdxTreeTrcAdminObjs* getTraceAdminObjIdxTree() { return m_pTrcAdminObjIdxTree; }
public: // instance methods to set and read trace settings of the server
    void setEnabled( bool i_bEnabled );
    bool isEnabled() const;
    void setNewTrcAdminObjsEnabledAsDefault( bool i_bEnabled );
    bool areNewTrcAdminObjsEnabledAsDefault() const;
    void setNewTrcAdminObjsDefaultDetailLevel( int i_iDetailLevel );
    int getNewTrcAdminObjsDefaultDetailLevel() const;
    void setCacheTrcDataIfNotConnected( bool i_bCacheData );
    bool getCacheTrcDataIfNotConnected() const;
    void setCacheTrcDataMaxArrLen( int i_iMaxArrLen );
    int getCacheTrcDataMaxArrLen() const;
    void setAdminObjFileAbsoluteFilePath( const QString& i_strAbsFilePath );
    QString getAdminObjFileAbsoluteFilePath() const;
    void setUseLocalTrcFile( bool i_bUse );
    bool isLocalTrcFileUsed() const;
    void setLocalTrcFileAbsoluteFilePath( const QString& i_strAbsFilePath );
    QString getLocalTrcFileAbsoluteFilePath() const;
    void setLocalTrcFileCloseFileAfterEachWrite( bool i_bCloseFile );
    bool getLocalTrcFileCloseFileAfterEachWrite() const;
protected: // instance methods to send admin objects to the connected server
    void sendAdminObj(
        ZS::System::MsgProtocol::TSystemMsgType i_systemMsgType,
        ZS::System::MsgProtocol::TCommand       i_cmd,
        CTrcAdminObj*                           i_pTrcAdminObj );
    void sendNameSpace(
        ZS::System::MsgProtocol::TSystemMsgType i_systemMsgType,
        ZS::System::MsgProtocol::TCommand       i_cmd,
        ZS::System::CBranchIdxTreeEntry*        i_pBranch,
        ZS::System::EEnabled                    i_enabled,
        int                                     i_iDetailLevel );
protected: // overridables of base class CClient
    virtual void onReceivedData( const QByteArray& i_byteArr );
protected slots: // connected to the signals of the IPC client
    void onIpcClientConnected( QObject* i_pClient );
    void onIpcClientDisconnected( QObject* i_pClient );
protected slots: // connected to the slots of the trace admin object pool model
    void onTrcAdminObjIdxTreeEntryChanged( ZS::System::CIdxTree* i_pIdxTree, ZS::System::CAbstractIdxTreeEntry* i_pTreeEntry );
protected: // instance methods
    void resetTrcAdminRefCounters( ZS::System::CBranchIdxTreeEntry* i_pBranch );
protected: // instance members
    STrcServerSettings    m_trcServerSettings;
    CIdxTreeTrcAdminObjs* m_pTrcAdminObjIdxTree;
    bool                  m_bOnReceivedDataUpdateInProcess;

}; // class CIpcTrcClient

} // namespace Trace

} // namespace ZS

#endif // #ifndef ZSIpcTrace_Client_h
