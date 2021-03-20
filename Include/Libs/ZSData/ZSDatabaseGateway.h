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

#ifndef ZSData_DatabaseGateway_h
#define ZSData_DatabaseGateway_h

#include <QtCore/qobject.h>

#include "ZSData/ZSDataCommon.h"

namespace ZS
{
namespace System
{
class CMsgCon;
}
namespace Trace
{
class CTrcAdminObj;
}

namespace Data
{
class CDb;
class CDbGatewayThread;

//******************************************************************************
class ZSDATADLL_API CDbGateway : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Data::Db"; }
    static QString ClassName() { return "CDbGateway"; }
public: // type definitions and constants
    enum ERequest {
        ERequestNone               = 0,
        ERequestStartup            = 1,
        ERequestShutdown           = 2,
        ERequestUpdateHostSettings = 3,
        ERequestCount,
        ERequestUndefined
    };
    static QString Request2Str( int i_iRequest );
public: // ctors and dtor
    CDbGateway(
        EProtocolType         i_protocolType,
        ZS::Ipc::ESocketType  i_socketType,
        CDb*                  i_pDb,
        CDbGatewayThread*     i_pGatewayThread );
    virtual ~CDbGateway();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    EProtocolType getProtocolType() const { return m_protocolType; }
    ZS::Ipc::ESocketType getSocketType() const { return m_hostSettings.m_socketType; }
    ZS::Ipc::SServerHostSettings getHostSettings() const { return m_hostSettings; }
public: // overridables
    virtual void abortRequestInProgress();  // immediately aborts the request in progress
public: // overridables
    virtual void addTrcMsgLogObject( QObject* i_pObj );
    virtual void removeTrcMsgLogObject( QObject* i_pObj );
protected: // must overridables (overridables of base class QObject)
    // Please note that the method cannot be declared as pure virtual.
    // On creating a QTimer object in the ctor of the client gateway
    // the event method of this gateway instance is called - which is
    // not existing and the application crashes (pure virtual function called).
    virtual bool event( QEvent* i_pMsg );
protected: // instance members
    CDb*                         m_pDb;
    CDbGatewayThread*            m_pGatewayThread;
    EProtocolType                m_protocolType;
    ZS::Ipc::SServerHostSettings m_hostSettings;
    ERequest                     m_requestInProgress;
    qint64                       m_iReqIdInProgress;
    qint64                       m_iMsgIdReqInProgress;
    ZS::System::CMsgCon*         m_pMsgCon;
    QList<QObject*>              m_arpTrcMsgLogObjects;
    ZS::Trace::CTrcAdminObj*     m_pTrcAdminObj;

}; // class CDbGateway

} // namespace Data

} // namespace ZS

#endif // #ifndef ZSData_DatabaseGateway_h
