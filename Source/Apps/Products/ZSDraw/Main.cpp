/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
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
#include "MainWindow.h"

#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysTrcServer.h"
#include "ZSSys/ZSSysVersion.h"
#include "ZSSysGUI/ZSSysLastUsedFileDlg.h"

#include "ZSSys/ZSSysMemLeakDump.h"


/*******************************************************************************
Entry point for the DLL application.
*******************************************************************************/

//------------------------------------------------------------------------------
int main( int i_argc, char* i_argv[] )
//------------------------------------------------------------------------------
{
    int iAppResult = 0;

    // Parse command arguments
    //------------------------

    QStringList strlstArgsPar;
    QStringList strlstArgsVal;

    ZS::System::parseAppArgs(i_argc, i_argv, strlstArgsPar, strlstArgsVal);

    bool bTest = false;
    QStringList strlstObjFactories;

    #if QT_VERSION >= 0x040501
    for( int idxArg = 0; idxArg < strlstArgsPar.length() && idxArg < strlstArgsVal.length(); idxArg++ )
    #else
    for( int idxArg = 0; idxArg < strListArgsPar.size() && idxArg < strListArgsVal.size(); idxArg++ )
    #endif
    {
        QString strArg = strlstArgsPar[idxArg];
        QString strVal = strlstArgsVal[idxArg];

        if( strArg.compare("Test",Qt::CaseInsensitive) == 0 )
        {
            bTest = true;
        }
        if( strArg.compare("DemoQtWidgets",Qt::CaseInsensitive) == 0 )
        {
            strlstObjFactories << ZS::Apps::Products::Draw::CMainWindow::c_strObjFactoryQtWidgets;
        }
        else if( strArg.compare("DemoElectricity",Qt::CaseInsensitive) == 0 )
        {
            strlstObjFactories << ZS::Apps::Products::Draw::CMainWindow::c_strObjFactoryElectricity;
        }
    }

    // Create and start application
    //-----------------------------

    ZS::Apps::Products::Draw::CApplication* pApp;

    try
    {
        pApp = new ZS::Apps::Products::Draw::CApplication(
            /* argc                  */ i_argc,
            /* argv                  */ i_argv,
            /* strOrganizationName   */ "ZeusSoft",
            /* strOrganizationDomain */ "ZeusSoft.de",
            /* strAppName            */ "ZSAppDraw" );
    }
    catch(...)
    {
        return -1;
    }

    // Open most recently used files
    //------------------------------

    QString strMainWindowTitle = "ZS Draw";

    ZS::System::SLastUsedFile lastUsedFile;
    int iDlgOpenLastUsedFileResult = QDialog::Rejected;

    if (!bTest)
    {
        ZS::System::GUI::CDlgOpenLastUsedFile* pDlg =
            ZS::System::GUI::CDlgOpenLastUsedFile::CreateInstance(strMainWindowTitle);
        pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
        pDlg->adjustSize();
        iDlgOpenLastUsedFileResult = pDlg->exec();
        pDlg = nullptr; // When leaving the exec function the dialog has already been destroyed by the Qt framework.

        if (iDlgOpenLastUsedFileResult == QDialog::Accepted)
        {
            lastUsedFile = ZS::System::GUI::CDlgOpenLastUsedFile::getSelectedFile();
        }
    }

    pApp->createAndShowMainWindow(strMainWindowTitle, lastUsedFile.m_strAbsFilePath, strlstObjFactories);

    iAppResult = pApp->exec();

    delete pApp;
    pApp = nullptr;

    #ifdef _WINDOWS
    #ifdef _DEBUG
    _CrtDoForAllClientObjects( ZS::dumpClientHook, nullptr );
    #endif
    #endif

    return iAppResult;

} // main
