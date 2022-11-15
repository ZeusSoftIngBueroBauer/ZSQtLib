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

#include "App.h"

#include "ZSSys/ZSSysVersion.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::Apps::Products::IpcServer;

/*******************************************************************************
Entry point for the application.
*******************************************************************************/

//------------------------------------------------------------------------------
int main(int argc, char *argv[])
//------------------------------------------------------------------------------
{
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    #endif

    int iAppResult = 0;

    // Set the OpenGL type before instantiating the application.
    // In this example, we're forcing use of ANGLE.
    // If started from Qt Creator OpenGLES cannot be used for some graphic cards.
    // The usage of OpenGLES should therefore be disabled by program argument.
    // Setting to OpenGL is also a workaround for a deadlock on restarting the
    // application on a secondary screen.
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);

    CApplication* pApp = new CApplication(
        /* argc                  */ argc,
        /* argv                  */ argv,
        /* strOrganizationName   */ "ZeusSoft",
        /* strOrganizationDomain */ "ZeusSoft.de",
        /* strAppName            */ "ZSAppIpcServer",
        /* strAppVersion         */ ZS::System::c_strSysVersionNr,
        /* strWindowTitle        */ "Ipc Server" );

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
}
