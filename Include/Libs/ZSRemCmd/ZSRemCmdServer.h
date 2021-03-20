/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer, Germany
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

#ifndef ZSRemCmd_Server_h
#define ZSRemCmd_Server_h

#include <QtCore/qthread.h>

#include "ZSRemCmd/ZSRemCmdDllMain.h"
#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcServer.h"

namespace ZS
{
namespace System
{
class CErrLog;
class CObjPoolListEntry;
}
namespace Ipc
{
struct SServerHostSettings;
struct STimerSettings;
}

namespace Trace
{
class CTrcAdminObj;
}

namespace RemCmd
{
class CRemCmdObj;
class CRemCmdSubSysNode;
class CRemCmdObjPool;
class CRemCmdParser;

//******************************************************************************
class ZSREMCMDDLL_API CRemCmdServer : public ZS::Ipc::CServer
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CRemCmdServer( const QString& i_strObjName );
    virtual ~CRemCmdServer();
signals:
    void dataSent( QObject* i_pRemCmdServer, int i_iSocketId, const QByteArray& i_byteArrData );
public: // instance methods
    CRemCmdObjPool* getObjPool() { return m_pRemCmdObjPool; }
    CRemCmdParser* getParser() { return m_pRemCmdParser; }
public: // instance methods
    CRemCmdSubSysNode* getRemCmdSubSysNode( const QString& i_strNode );
public: // instance methods
    CRemCmdObj* getRemCmdObj( const QString& i_strCmd );
    CRemCmdObj* getRemCmdObj( int i_iObjId );
public: // instance methods
    void addErrResultInfo( const ZS::System::SErrResultInfo& i_errResultInfo );
public: // instance methods
    ZS::System::SErrResultInfo parseData( const QByteArray& i_byteArrData, QString* o_pstrCmd, bool* o_pbIsQuery, QByteArray* o_pByteArrArgs ) const;
    ZS::System::SErrResultInfo executeCommand( const ZS::Ipc::SSocketDscr& i_socketDscr, const QString& i_strCmd, bool i_bIsQuery, const QByteArray& i_byteArrArgs );
protected: // overridables of base class ZS::Ipc::CServer
    virtual void onReceivedData( int i_iSocketId, const QByteArray& i_byteArr );
protected: // overridables of base class Ipc::CServer
    virtual bool event( QEvent* i_pEv );
protected slots: // connected to the signals of the remote command object pool
    void onObjNodeInserted( QObject* i_pObjPool, ZS::System::CObjPoolListEntry* i_pListEntry );
protected: // instance members
    CRemCmdParser*        m_pRemCmdParser;
    CRemCmdObjPool*       m_pRemCmdObjPool;
    Trace::CTrcAdminObj*  m_pTrcAdminObj;

}; // class CRemCmdServer

} // namespace RemCmd

} // namespace ZS

#endif // #ifndef ZSRemCmd_Server_h
