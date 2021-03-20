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

#include "App.h"

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysVersion.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Apps::Test::IpcTraceDllIfQtApp;


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
#if QT_VERSION >= 0x050000 && QT_VERSION < 0x060000
#pragma message(__FILE__ ": Linking against = " QTWIDGETSLIB)
#pragma comment( lib, QTWIDGETSLIB )
#endif
#pragma message(__FILE__ ": Linking against = " ZSSYSLIB)
#pragma comment(lib, ZSSYSLIB)
#pragma message(__FILE__ ": Linking against = " ZSSYSGUILIB)
#pragma comment(lib, ZSSYSGUILIB)
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
        /* strAppName            */ "ZSAppTestIpcTraceDllIfQtApp",
        /* strAppNameWindowTitle */ "ZSApp-Test-TestIpcTraceDllIfQtApp (" + ZS::System::c_strSysVersionNr + ")" );

    iAppResult = pApp->exec();

    delete pApp;
    pApp = nullptr;

    #ifdef _WINDOWS
    #ifdef _DEBUG
    try
    {
        _CrtDoForAllClientObjects( ZS::dumpClientHook, nullptr );
    }
    catch(...)
    {
    }
    #endif
    #endif

    return iAppResult;

} // main
