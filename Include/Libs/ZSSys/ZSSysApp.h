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

#ifndef ZSSys_App_h
#define ZSSys_App_h

#include <QtCore/qcoreapplication.h>

#include "ZSSys/ZSSysDllMain.h"

namespace ZS
{
namespace System
{
class CErrLog;

/*******************************************************************************
Application
*******************************************************************************/

ZSSYSDLL_API void parseAppArgs(
    int&         i_argc,
    char*        i_argv[],
    QStringList& i_strListPar,
    QStringList& i_strListVal );

// TODO: Define template class CZSApp with inline method "qcoreapplication_notify"

// Range of IniFileScope: ["AppDir", "User", "System"]
ZSSYSDLL_API QString getAppConfigDir( const QString& i_strIniFileScope );
ZSSYSDLL_API QString getAppLogDir( const QString& i_strIniFileScope );
ZSSYSDLL_API QString getAppDataDir( const QString& i_strIniFileScope );


//******************************************************************************
class ZSSYSDLL_API CCoreApp : public QCoreApplication
//******************************************************************************
{
public: // ctors and dtor
    CCoreApp( int& i_argc, char* i_argv[] );
    ~CCoreApp();
protected: // overridables of base class QCoreApplication
    virtual bool notify( QObject* i_pObjRcv, QEvent* i_pEv );

}; // class CCoreApp

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_App_h
