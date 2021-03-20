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

#ifndef ZSData_DatabaseClientGatewayThread_h
#define ZSData_DatabaseClientGatewayThread_h

#include <QtCore/QThread>

#include "ZSData/ZSDataCommon.h"

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}

namespace Data
{
class CDbClient;
class CDbClientGateway;

//******************************************************************************
class ZSDATADLL_API CDbClientGatewayThread : public QThread
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Data::DbClient"; }
    static QString ClassName() { return "CDbClientGatewayThread"; }
public: // ctors and dtor
    CDbClientGatewayThread( CDbClient* i_pClient );
    virtual ~CDbClientGatewayThread();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    CDbClientGateway* getGateway() { return m_pGateway; }
public: // overridables
    virtual void start( qint64 i_iReqId );
protected: // hiding public overridables of base class QThread
    virtual void start();
public: // must overridables (overridables of base class QThread)
    virtual void run();
protected: // instance members
    CDbClient*               m_pClient;
    CDbClientGateway*        m_pGateway;
    qint64                   m_iReqIdStart;
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CDbClientGatewayThread

} // namespace Data

} // namespace ZS

#endif // #ifndef ZSData_DatabaseClientGatewayThread_h
