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

#include <QtCore/qglobal.h>

#include <ObjBase.h>

#include "App.h"

#include "ZSPhysSizes/Kinematics/ZSPhysSizesKinematicsDllMain.h"
#include "ZSPhysVal/ZSPhysValDllMain.h"
#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysVersion.h"

#include "ZSSys/ZSSysMemLeakDump.h"


/*******************************************************************************
Libraries depending on build configuration and used Qt version
*******************************************************************************/

#ifdef _WINDOWS

#ifdef USE_PRAGMA_COMMENT_LIB_INCLUDE_IN_MAIN_MODULES

#pragma comment( lib, QTMAINLIB )
#pragma comment( lib, QTCORELIB )
#pragma comment( lib, QTNETWORKLIB )
#pragma comment( lib, QTXMLLIB )
#pragma comment( lib, QTGUILIB )
#if QT_VERSION >= 0x050000 && QT_VERSION < 0x060000
#pragma comment( lib, QTWIDGETSLIB )
#endif

#endif // #ifdef USE_PRAGMA_COMMENT_LIB_INCLUDE_IN_MAIN_MODULES

#endif // #ifdef _WINDOWS


/*******************************************************************************
Entry point for the application.
*******************************************************************************/

using namespace ZS::Apps::Products::PCResourceMonitor;

//------------------------------------------------------------------------------
int main( int argc, char* argv[] )
//------------------------------------------------------------------------------
{
    int iAppResult;

    #ifdef _WINDOWS

    HRESULT hResCoInitialize = MAKE_HRESULT(SEVERITY_ERROR,0,S_FALSE);
    HRESULT hResCoInitializeSecurity = MAKE_HRESULT(SEVERITY_ERROR,0,S_FALSE);

    hResCoInitialize = CoInitializeEx(nullptr,COINIT_APARTMENTTHREADED);

    if( SUCCEEDED(hResCoInitialize) )
    {
        hResCoInitializeSecurity = CoInitializeSecurity(
            /* pSecDesc       */ nullptr,
            /* cAuthSvc       */ -1,
            /* asAuthSvc      */ nullptr,
            /* pReserved1     */ nullptr,
            /* dwAuthnLevel   */ RPC_C_AUTHN_LEVEL_PKT,
            /* dwImpLevel     */ RPC_C_IMP_LEVEL_IMPERSONATE,
            /* pAuthList      */ nullptr,
            /* dwCapabilities */ EOAC_NONE,
            /* pReserved3     */ nullptr );
    }
    #endif // #ifdef _WINDOWS

    ZS::PhysVal::createUnitsPool();

    ZS::PhysVal::Kinematics::createPhysSizes();

    CApplication* pApp = new CApplication(
        /* argc                  */ argc,
        /* argv                  */ argv,
        /* strOrganizationName   */ "ZeusSoft",
        /* strOrganizationDomain */ "ZeusSoft.de",
        /* strAppName            */ "ZSAppPCResourceMonitor",
        /* strWindowTitle        */ "PC Resource Monitor" );

    iAppResult = pApp->exec();

    delete pApp;
    pApp = nullptr;

    ZS::PhysVal::Kinematics::deletePhysSizes();

    ZS::PhysVal::deleteUnitsPool();

    #ifdef _WINDOWS
    if( SUCCEEDED(hResCoInitialize) )
    {
        CoUninitialize();
    }
    #endif // #ifdef _WINDOWS

    #ifdef _DEBUG
    _CrtDoForAllClientObjects( ZS::dumpClientHook, nullptr );
    #endif

    return iAppResult;

} // main
