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

#include "App.h"

#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
#include "ZSSys/ZSSysTrcServer.h"
#include "ZSSys/ZSSysVersion.h"

#include "ZSSys/ZSSysMemLeakDump.h"


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
#pragma message(__FILE__ ": Linking against = " ZSIPCLIB)
#pragma comment(lib, ZSIPCLIB)
#pragma message(__FILE__ ": Linking against = " ZSIPCGUILIB)
#pragma comment(lib, ZSIPCGUILIB)
#pragma message(__FILE__ ": Linking against = " ZSIPCTRACELIB)
#pragma comment(lib, ZSIPCTRACELIB)
#pragma message(__FILE__ ": Linking against = " ZSIPCTRACEGUILIB)
#pragma comment(lib, ZSIPCTRACEGUILIB)
#pragma message(__FILE__ ": Linking against = " ZSPHYSVALLIB)
#pragma comment(lib, ZSPHYSVALLIB)
#pragma message(__FILE__ ": Linking against = " ZSPHYSVALGUILIB)
#pragma comment(lib, ZSPHYSVALGUILIB)
#pragma message(__FILE__ ": Linking against = " ZSPHYSSIZESGEOMETRYLIB)
#pragma comment(lib, ZSPHYSSIZESGEOMETRYLIB)
#pragma message(__FILE__ ": Linking against = " ZSDRAWLIB)
#pragma comment(lib, ZSDRAWLIB)

#endif // #ifdef USE_PRAGMA_COMMENT_LIB_INCLUDE_IN_MAIN_MODULES

#endif // #ifdef _WINDOWS


/*******************************************************************************
Entry point for the DLL application.
*******************************************************************************/

//------------------------------------------------------------------------------
int main( int argc, char* argv[] )
//------------------------------------------------------------------------------
{
    int iAppResult = 0;

    // Initialize static objects of dlls (where necessary)
    //----------------------------------------------------

    ZS::PhysVal::createUnitsPool();

    ZS::PhysVal::Geometry::createPhysSizes();

    // Create and start application
    //-----------------------------

    ZS::Apps::Products::Draw::CApplication* pApp;

    try
    {
        pApp = new ZS::Apps::Products::Draw::CApplication(
            /* argc                  */ argc,
            /* argv                  */ argv,
            /* strOrganizationName   */ "ZeusSoft",
            /* strOrganizationDomain */ "ZeusSoft.de",
            /* strAppName            */ "ZSAppDraw",
            /* strAppNameWindowTitle */ "Draw" );
    }
    catch(...)
    {
        return -1;
    }

    iAppResult = pApp->exec();

    // Delete application
    //-------------------

    //try
    //{
    //    delete pApp;
    //}
    //catch(...)
    //{
    //}
    //pApp = nullptr;

    // Delete trace server
    //--------------------

    // Please note that the trace server must be deleted after the application has been
    // destroyed and the main event loop has been quit. There might still be events (messages)
    // pending while the application is quit whose dtor will be called. If the message dtor
    // wants to output traces the server must still be alive. Otherwise the trace admin
    // objects referenced by the message dtor would have already been destroyed before the dtor
    // of the event (message) is called.

    //ZS::Trace::CTrcServer* pTrcServer = ZS::Trace::CTrcServer::GetInstance();

    //if( pTrcServer != nullptr )
    //{
    //    try
    //    {
    //        ZS::Trace::CTrcServer::DestroyInstance(pTrcServer);
    //    }
    //    catch(...)
    //    {
    //    }
    //    pTrcServer = nullptr;
    //}

    // Remove static objects of dlls (where necessary)
    //------------------------------------------------

    ZS::PhysVal::Geometry::deletePhysSizes();

    ZS::PhysVal::deleteUnitsPool();

    delete pApp;
    pApp = nullptr;

    #ifdef _WINDOWS
    #ifdef _DEBUG
    _CrtDoForAllClientObjects( ZS::dumpClientHook, nullptr );
    #endif
    #endif

    return iAppResult;

} // main
