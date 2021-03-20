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

#ifndef ZSRemCmd_Client_h
#define ZSRemCmd_Client_h

#include "ZSRemCmd/ZSRemCmdDllMain.h"
#include "ZSIpc/ZSIpcClient.h"


namespace ZS
{
namespace System
{
class CErrLog;
class CRequest;
}
namespace Ipc
{
struct SClientHostSettings;
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
class ZSREMCMDDLL_API CRemCmdClient : public ZS::Ipc::CClient
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CRemCmdClient( const QString& i_strObjName );
    virtual ~CRemCmdClient();
signals:
    void commandSent( QObject* i_pRemCmdClient, const QString& i_strCmd, const QByteArray& i_byteArrArgs );
protected: // instance methods (belonging to ctor)
    void createRemoteCommands();
public: // instance methods
    CRemCmdObjPool* getObjPool() { return m_pRemCmdObjPool; }
    CRemCmdParser* getParser() { return m_pRemCmdParser; }
public: // instance methods
    CRemCmdSubSysNode* getRemCmdSubSysNode( const QString& i_strNode );
public: // instance methods
    CRemCmdObj* getRemCmdObj( const QString& i_strCmd );
    CRemCmdObj* getRemCmdObj( int i_iObjId );
public: // instance methods
    ZS::System::SErrResultInfo parseData( const QByteArray& i_byteArrData, QString* o_pstrCmd, bool* o_pbIsQuery, QByteArray* o_pByteArrArgs ) const;
public: // instance methods to send commands to the connected server
    ZS::System::CRequest* sendCommand( const QString& i_strCmd, const QByteArray& i_byteArrArgs );
    ZS::System::CRequest* sendCommand( CRemCmdObj* i_pRemCmdObj, const QByteArray& i_byteArrArgs );
protected: // overridables of base class Ipc::CClient
    void onReceivedData( const QByteArray& i_byteArr );
protected slots: // connected to the signals of the Ipc::Client (thats me)
    void onIpcClientConnected( QObject* i_pIpcClient );
    void onIpcClientDisconnected( QObject* i_pIpcClient );
protected slots:
    void onRemCmdCommCatDestroyed( QObject* i_pRemCmdObj );
protected: // instance members
    CRemCmdParser*       m_pRemCmdParser;
    CRemCmdObjPool*      m_pRemCmdObjPool;
    CRemCmdObj*          m_pRemCmdCommCat;
    Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CRemCmdClient

} // namespace RemCmd

} // namespace ZS

#endif // #ifndef ZSRemCmd_Client_h
