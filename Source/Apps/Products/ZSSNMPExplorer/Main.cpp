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

#include <QtCore/qglobal.h>
#include "Include/ZSApp.h"
#include "ZSSys/Include/ZSSysVersion.h"
#include "ZSSys/Include/ZSSysMemLeakDump.h"


/*******************************************************************************
Libraries depending on build configuration and used Qt version (Qt-Isar-Dll's)
*******************************************************************************/

#ifdef _WINDOWS

    #if QT_VERSION >= 0x040700
        #ifdef _DEBUG
            #pragma comment(lib, "qtmainIsard")
            #pragma comment(lib, "QtCoreIsard4")
            #pragma comment(lib, "QtNetworkIsard4")
            #pragma comment(lib, "QtXmlIsard4")
            #pragma comment(lib, "QtGuiIsard4")
        #else
            #pragma comment(lib, "qtmainIsar")
            #pragma comment(lib, "QtCoreIsar4")
            #pragma comment(lib, "QtNetworkIsar4")
            #pragma comment(lib, "QtXmlIsar4")
            #pragma comment(lib, "QtGuiIsar4")
        #endif
    #else
        #ifdef _DEBUG
            #pragma comment(lib, "qtmaind")
            #pragma comment(lib, "QtCored4")
            #pragma comment(lib, "QtNetworkd4")
            #pragma comment(lib, "QtXmld4")
            #pragma comment(lib, "QtGuid4")
        #else
            #pragma comment(lib, "qtmain")
            #pragma comment(lib, "QtCore4")
            #pragma comment(lib, "QtNetwork4")
            #pragma comment(lib, "QtXml4")
            #pragma comment(lib, "QtGui4")
        #endif
    #endif

    // ZSQtLib-Dll's
    //--------------------------------------------------------------------------

    #ifdef _DEBUG
        #pragma comment(lib, "ZSSysd")
        #pragma comment(lib, "ZSIpcd")
        #pragma comment(lib, "ZSTraced")
        #pragma comment(lib, "ZSSNMPd")
    #else
        #pragma comment(lib, "ZSSys")
        #pragma comment(lib, "ZSIpc")
        #pragma comment(lib, "ZSTrace")
        #pragma comment(lib, "ZSSNMP")
    #endif

#endif // #ifdef _WINDOWS


/*******************************************************************************
Entry point for the application.
*******************************************************************************/

//------------------------------------------------------------------------------
int main( int argc, char* argv[] )
//------------------------------------------------------------------------------
{
    int iAppResult;

    ZS::SNMP::CApplication* pApp = new ZS::SNMP::CApplication(
        /* argc                  */ argc,
        /* argv                  */ argv,
        /* strOrganizationName   */ "ZeusSoft",
        /* strOrganizationDomain */ "ZeusSoft.de",
        /* strAppName            */ "SNMP-Explorer" );
    iAppResult = pApp->exec();

    delete pApp;
    pApp = NULL;

    #ifdef _WINDOWS
    #ifdef _DEBUG
    _CrtDoForAllClientObjects( ZS::System::dumpClientHook, NULL );
    #endif
    #endif

    return iAppResult;

} // main
