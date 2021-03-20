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

#ifndef ZSData_DatabaseGatewayZSXML_h
#define ZSData_DatabaseGatewayZSXML_h

#include "ZSData/ZSDatabaseGateway.h"

namespace ZS
{
namespace Ipc
{
class CIpcServerWrapper;
class CIpcSocketWrapper;
}

namespace Data
{
//******************************************************************************
class ZSDATADLL_API CDbGatewayZSXML : public CDbGateway
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Data::Db"; }
    static QString ClassName() { return "CDbGatewayZSXML"; }
public: // ctors and dtor
    CDbGatewayZSXML(
        ZS::Ipc::ESocketType i_socketType,
        CDb*                 i_pDb,
        CDbGatewayThread*    i_pGatewayThread );
    virtual ~CDbGatewayZSXML();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
protected slots: // overridables
    virtual void onNewConnection( QObject* i_pServerWrapper );
protected slots: // must overridables of base class CSrvCltBaseGateway
    virtual void onDisconnected( QObject* i_pSocketWrapper );
    virtual void onReadyRead( QObject* i_pSocketWrapper );
    virtual void onError( QObject* i_pSocketWrapper, ZS::System::SErrResultInfo& i_errResultInfo );
public: // overridables of inherited class CDbGateway
    virtual bool event( QEvent* i_pMsg );
//protected: // overridables to parse and execute the incoming data stream
//    virtual ZS::System::SErrResultInfo onReceivedErr( const MsgProtocol::SMsgHeader* i_pMsgHdr );
//    virtual ZS::System::SErrResultInfo onReceivedConExecCmd( const MsgProtocol::SMsgHeader* i_pMsgHdr );
//protected: // overridables to parse and execute the incoming data stream
//    virtual ZS::System::SErrResultInfo onReceivedConExecCmdSelectSchema( const MsgProtocol::SCmdHeader* i_pCmdHdr );
//    virtual ZS::System::SErrResultInfo onReceivedConExecCmdUpdateSchema( const MsgProtocol::SCmdHeader* i_pCmdHdr );
//    virtual ZS::System::SErrResultInfo onReceivedConExecCmdSelect( const MsgProtocol::SCmdHeader* i_pCmdHdr );
//    virtual ZS::System::SErrResultInfo onReceivedConExecCmdUpdate( const MsgProtocol::SCmdHeader* i_pCmdHdr );
protected: // instance members
    ZS::Ipc::CIpcServerWrapper*          m_pIpcServerWrapper;
    QVector<ZS::Ipc::CIpcSocketWrapper*> m_arpIpcSocketWrappers;

}; // class CDbGatewayZSXML

} // namespace Data

} // namespace ZS

#define ZSData_DatabaseGatewayZSXML_h
#endif // #ifndef 
