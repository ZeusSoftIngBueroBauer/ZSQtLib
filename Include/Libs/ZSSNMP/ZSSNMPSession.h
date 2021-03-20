/*******************************************************************************

Copyright 2004 - 2010 by    ZeusSoft, Ing. Buero Bauer
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

--------------------------------------------------------------------------------

$Date: $

$Revision: $

$Author: $

$History: $

*******************************************************************************/

#ifndef ZSSNMPSession_h
#define ZSSNMPSession_h

#include <QtCore/qobject.h>
#include <QtCore/qthread.h>
#include "ZSSNMP/Include/ZSSNMPDllMain.h"
#include "ZSSNMP/Include/ZSSNMPExplorer.h"

struct snmp_session;

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}
namespace SNMP
{
class CSession;

//******************************************************************************
class ZSSNMPDLL_API CSessionThread : public QThread
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CSessionThread(
        const QString& i_strObjNameSession,
        CExplorer*     i_pExplorer,
        CModelErrLog*  i_pModelErrLog );
    virtual ~CSessionThread();
signals:
    void sessionCreated( QObject* i_pSessionThread, QObject* i_pSession );
public: // overridables of base class QThread
    virtual void run();
protected: // instance members
    CModelErrLog*        m_pModelErrLog;
    QString              m_strObjName;
    QString              m_strObjNameSession;
    Trace::CTrcAdminObj* m_pTrcAdminObj;
    CExplorer*           m_pExplorer;
    CSession*            m_pSession;

}; // class CSessionThread

//******************************************************************************
class ZSSNMPDLL_API CSession : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CSession(
        const QString&  i_strObjName,
        CExplorer*      i_pExplorer,
        CSessionThread* i_pSessionThread,
        CModelErrLog*   i_pModelErrLog );
    virtual ~CSession();
protected: // overridables of inherited class QObject
    virtual bool event( QEvent* i_pEv );
protected: // instance members
    CModelErrLog*        m_pModelErrLog;
    QString              m_strObjName;
    Trace::CTrcAdminObj* m_pTrcAdminObj;
    CExplorer*           m_pExplorer;
    CSessionThread*      m_pSessionThread;
    SHostSettings        m_hostSettings;
    snmp_session*        m_pSession;

}; // class CSession

} // namespace SNMP

} // namespace ZS

#endif // #ifndef ZSSNMPSession_h
