/*******************************************************************************

Copyright 2004 - 2009 by    ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSApp_h
#define ZSApp_h

#include <QtGui/qapplication.h>
#include "ZSSNMP/Include/ZSSNMPExplorer.h"
#include "ZSSNMP/Include/ZSSNMPCltStdPC.h"

class QLabel;
class QSettings;
class QStatusBar;
class QTimer;

namespace ZS
{
struct SErrResultInfo;
class CModelErrLog;

namespace Trace
{
class CTrcAdminObj;
class CTrcServer;
}
namespace SNMP
{
class CMainWindow;

//******************************************************************************
class CApplication : public QApplication
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    typedef enum {
        EStateIdle     = 0,
        EStateStartup  = 1,
        EStateShutdown = 2,
        EStateRunning  = 3,
        EStateCount
    }   EState;
    static QString state2Str( int i_iState );
public: // class methods
    static CApplication* instance();
public: // ctors and dtor
    CApplication(
        int            i_argc,
        char*          i_argv[],
        const QString& i_strOrganizationName,
        const QString& i_strOrganizationDomain,
        const QString& i_strAppName );
    virtual ~CApplication();
public: // instance methods to trigger system startup and shutdown
    void startup();
    void shutdown();
public: // overridables of base class QCoreApplication
    virtual bool notify( QObject* i_pObjReceiver, QEvent* i_pEv );
protected slots: // instance methods of system shutdown
    void onLastWindowClosed();
protected: // instance members
    Trace::CTrcServer*   m_pTrcServer;
    bool                 m_bTrcServerCreatedByMe;
    Trace::CTrcAdminObj* m_pTrcAdminObj;
    QSettings*           m_pSettingsFile;
    bool                 m_bSettingsFileCreatedByMe;
    ZS::CModelErrLog*    m_pModelErrLog;
    bool                 m_bModelErrLogCreatedByMe;
    CExplorer*           m_pExplorer;
    CCltStdPC*           m_pCltStdPC;
    CMainWindow*         m_pMainWindow;
    QStatusBar*          m_pStatusBar;

}; // class CApplication

} // namespace SNMP

} // namespace ZS

#endif // #ifndef ZSApp_h
