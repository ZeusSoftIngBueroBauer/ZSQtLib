/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer, Germany
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

#include <QtCore/qglobal.h>

#include "App.h"

#include "ZSSys/ZSSysVersion.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::Apps::Test::IpcTrace;


/*******************************************************************************
Libraries depending on build configuration and used Qt version
*******************************************************************************/

#ifdef _WINDOWS

#ifdef USE_PRAGMA_COMMENT_LIB_INCLUDE_IN_MAIN_MODULES

#pragma message(__FILE__ ": Linking against = " QTMAINLIB)
#pragma comment( lib, QTMAINLIB )
#pragma message(__FILE__ ": Linking against = " QTCORELIB)
#pragma comment(lib, QTCORELIB)
#pragma message(__FILE__ ": Linking against = " QTNETWORKLIB)
#pragma comment(lib, QTNETWORKLIB)
#pragma message(__FILE__ ": Linking against = " QTXMLLIB)
#pragma comment(lib, QTXMLLIB)
#pragma message(__FILE__ ": Linking against = " QTGUILIB)
#pragma comment( lib, QTGUILIB )
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#pragma message(__FILE__ ": Linking against = " QTWIDGETSLIB)
#pragma comment( lib, QTWIDGETSLIB )
#endif
#pragma message(__FILE__ ": Linking against = " ZSSYSLIB)
#pragma comment(lib, ZSSYSLIB)
#pragma message(__FILE__ ": Linking against = " ZSSYSGUILIB)
#pragma comment(lib, ZSSYSGUILIB)
#pragma message(__FILE__ ": Linking against = " ZSIPCLIB)
#pragma comment(lib, ZSIPCLIB)
#pragma message(__FILE__ ": Linking against = " ZSIPCGUILIB)
#pragma comment(lib, ZSIPCGUILIB)
#pragma message(__FILE__ ": Linking against = " ZSIPCTRACELIB)
#pragma comment(lib, ZSIPCTRACELIB)
#pragma message(__FILE__ ": Linking against = " ZSIPCTRACEGUILIB)
#pragma comment(lib, ZSIPCTRACEGUILIB)
#pragma message(__FILE__ ": Linking against = " ZSTESTLIB)
#pragma comment(lib, ZSTESTLIB)
#pragma message(__FILE__ ": Linking against = " ZSTESTGUILIB)
#pragma comment(lib, ZSTESTGUILIB)

#endif // #ifdef USE_PRAGMA_COMMENT_LIB_INCLUDE_IN_MAIN_MODULES

#endif // #ifdef _WINDOWS


/*******************************************************************************
Entry point for the application.
*******************************************************************************/

//------------------------------------------------------------------------------
int main( int argc, char* argv[] )
//------------------------------------------------------------------------------
{
    int iAppResult = 0;

    CApplication* pApp = new CApplication(
        /* argc                  */ argc,
        /* argv                  */ argv,
        /* strOrganizationName   */ "ZeusSoft",
        /* strOrganizationDomain */ "ZeusSoft.de",
        /* strAppName            */ "ZSAppTestIpcTrace",
        /* strAppNameWindowTitle */ "ZSApp-Test-IpcTrace (" + ZS::System::c_strSysVersionNr + ")" );

    iAppResult = pApp->exec();

    try
    {
        delete pApp;
    }
    catch(...)
    {
    }
    pApp = nullptr;

    #ifdef _WINDOWS
    #ifdef _DEBUG
    _CrtDoForAllClientObjects( ZS::dumpClientHook, nullptr );
    #endif
    #endif

    return iAppResult;

} // main
