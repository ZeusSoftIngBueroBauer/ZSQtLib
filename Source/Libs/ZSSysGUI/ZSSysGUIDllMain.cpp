/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#ifdef _WINDOWS
#include <windows.h>
#endif

#include <QtCore/qglobal.h>
#include <QtCore/qmetatype.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qapplication.h>
#include <QtGui/qmainwindow.h>
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qmainwindow.h>
#endif

#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysVersion.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
Libraries depending on build configuration and used Qt version
*******************************************************************************/

#ifdef _WINDOWS

#ifdef USE_PRAGMA_COMMENT_LIB_INCLUDE_IN_MAIN_MODULES

#pragma message(__FILE__ ": Linking against = " QTCORELIB)
#pragma comment(lib, QTCORELIB)
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

#endif // #ifdef USE_PRAGMA_COMMENT_LIB_INCLUDE_IN_MAIN_MODULES

#endif // #ifdef _WINDOWS


/*******************************************************************************
Entry point for the DLL application.
*******************************************************************************/

#ifdef _WINDOWS
//------------------------------------------------------------------------------
BOOL APIENTRY DllMain(
    HANDLE /*i_hModule*/,
    DWORD  i_ul_reason_for_call,
    LPVOID /*i_lpReserved*/ )
//------------------------------------------------------------------------------
{
    switch( i_ul_reason_for_call )
    {
        case DLL_PROCESS_ATTACH:
        {
            break;
        }
        case DLL_THREAD_ATTACH:
        {
            break;
        }
        case DLL_THREAD_DETACH:
        {
            break;
        }
        case DLL_PROCESS_DETACH:
        {
            break;
        }
        default:
        {
            break;
        }
    }
    return TRUE;

} // DllMain
#endif


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

/*==============================================================================
Enum EViewMode
==============================================================================*/

const ZS::System::SEnumEntry s_arEnumStrViewModes[] =
{                                        // IdxName,     Symbol, Text,         SCIPShort, SCPILong,
    /* 0 */ SEnumEntry( EViewModeTableView, "TableView", "Tbl",  "Table View", "TABL",    "TABLeview" ),
    /* 1 */ SEnumEntry( EViewModeTreeView,  "TreeView",  "Tre",  "Tree View",  "TREE",    "TREEview"  )
};

//------------------------------------------------------------------------------
QString ZS::System::GUI::viewMode2Str( int i_iViewMode, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrViewModes, _ZSArrLen(s_arEnumStrViewModes), i_iViewMode, i_alias);
}

//------------------------------------------------------------------------------
EViewMode ZS::System::GUI::str2ViewMode( const QString& i_str, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    EViewMode viewMode = EViewModeUndefined;
    int iViewMode = SEnumEntry::str2Enumerator(s_arEnumStrViewModes, _ZSArrLen(s_arEnumStrViewModes), i_str, i_alias, Qt::CaseInsensitive);
    if( iViewMode >= 0 && iViewMode < _ZSArrLen(s_arEnumStrViewModes) )
    {
        viewMode = static_cast<EViewMode>(iViewMode);
    }
    return viewMode;

} // str2ViewMode



/*******************************************************************************
global auxiliary methods
*******************************************************************************/

/*==============================================================================
Auxiliary Methods
==============================================================================*/

//------------------------------------------------------------------------------
QMainWindow* ZS::System::GUI::getMainWindowOfApp()
//------------------------------------------------------------------------------
{
    QMainWindow* pMainWindow = nullptr;

    QApplication* pApp = dynamic_cast<QApplication*>(QApplication::instance());

    foreach( QWidget* pWdgt, pApp->topLevelWidgets() )
    {
        QMainWindow* pMainWindowTmp = dynamic_cast<QMainWindow*>(pWdgt);

        if( pMainWindowTmp != nullptr )
        {
            pMainWindow = pMainWindowTmp;
            break;
        }
    }
    return pMainWindow;

} // getMainWindowOfApp

//------------------------------------------------------------------------------
QString ZS::System::GUI::getMainWindowTitle()
//------------------------------------------------------------------------------
{
    QString strTitle = QCoreApplication::applicationName();

    QMainWindow* pMainWindow = getMainWindowOfApp();

    if( pMainWindow != nullptr )
    {
        strTitle = pMainWindow->windowTitle();
    }
    return strTitle;

} // getMainWindowTitle

/*==============================================================================
Error Pixmaps
==============================================================================*/

bool     s_bErrIconPixmapsCreated = false;
QPixmap* s_arpErrIconPixmaps[EResultSeverityCount];

static void createErrIconPixmaps();

//------------------------------------------------------------------------------
QPixmap ZS::System::GUI::getErrPixmap( EResultSeverity i_severity, const QSize& i_sz )
//------------------------------------------------------------------------------
{
    QPixmap pxm;

    if( !s_bErrIconPixmapsCreated )
    {
        createErrIconPixmaps();
    }
    if( s_arpErrIconPixmaps[i_severity] != nullptr )
    {
        pxm = *s_arpErrIconPixmaps[i_severity];
    }

    pxm = pxm.scaled(i_sz.width(), i_sz.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    pxm.setMask(pxm.createHeuristicMask());

    return pxm;

} // getErrPixmap

//------------------------------------------------------------------------------
QIcon ZS::System::GUI::getErrIcon( EResultSeverity i_severity )
//------------------------------------------------------------------------------
{
    QIcon icon;

    if( !s_bErrIconPixmapsCreated )
    {
        createErrIconPixmaps();
    }
    if( i_severity >= 0 && i_severity < EResultSeverityCount )
    {
        if( s_arpErrIconPixmaps[i_severity] != nullptr )
        {
            icon.addPixmap(*s_arpErrIconPixmaps[i_severity]);
        }
    }
    else
    {
        if( s_arpErrIconPixmaps[EResultSeverityCritical] != nullptr )
        {
            icon.addPixmap(*s_arpErrIconPixmaps[EResultSeverityCritical]);
        }
    }
    return icon;

} // getErrIcon

//------------------------------------------------------------------------------
static void createErrIconPixmaps()
//------------------------------------------------------------------------------
{
    if( !s_bErrIconPixmapsCreated )
    {
        // Must construct a QApplication before a QPaintDevice ..
        if( QCoreApplication::instance() != nullptr )
        {
            memset( s_arpErrIconPixmaps, 0x00, sizeof(s_arpErrIconPixmaps) );

            int      iSeverity;
            QString  strSeverity;
            QString  strFileName;
            QPixmap* pPixmap;

            for( iSeverity = 0; iSeverity < EResultSeverityCount; iSeverity++ )
            {
                strSeverity = ZS::System::resultSeverity2Str(static_cast<EResultSeverity>(iSeverity));

                strFileName = ":/ZS/Result/ResultSeverity" + strSeverity;

                #ifdef _WINDOWS
                #pragma push_macro("_SMSYSDBGNEW_CLIENT_BLOCK_SUBTYPE")
                #pragma warning( disable : 4005 )
                #define _SMSYSDBGNEW_CLIENT_BLOCK_SUBTYPE 0
                #endif

                pPixmap = new QPixmap(strFileName+".bmp");

                #ifdef _WINDOWS
                #pragma warning( default : 4005 )
                #pragma pop_macro("_SMSYSDBGNEW_CLIENT_BLOCK_SUBTYPE")
                #endif

                //QSize sizePxm = pPixmap->size();

                pPixmap->setMask(pPixmap->createHeuristicMask());

                s_arpErrIconPixmaps[iSeverity] = pPixmap;
            }

            s_bErrIconPixmapsCreated = true;
        }
    }

} // createErrIconPixmaps

////------------------------------------------------------------------------------
//static void deleteErrIconPixmaps()
////------------------------------------------------------------------------------
//{
//    if( s_bErrIconPixmapsCreated )
//    {
//        int iSeverity;
//
//        for( iSeverity = 0; iSeverity < EResultSeverityCount; iSeverity++ )
//        {
//            delete s_arpErrIconPixmaps[iSeverity];
//            s_arpErrIconPixmaps[iSeverity] = nullptr;
//        }
//        s_bErrIconPixmapsCreated = false;
//    }
//
//} // deleteErrIconPixmaps


/*==============================================================================
Enum EMode
==============================================================================*/

//------------------------------------------------------------------------------
QPixmap ZS::System::GUI::mode2Pixmap( int i_iMode, int i_iSize )
//------------------------------------------------------------------------------
{
    QPixmap pxm;
    QString strSize = QString::number(i_iSize) + "x" + QString::number(i_iSize);

    if( i_iMode == static_cast<int>(EMode::Edit) )
    {
        pxm = QPixmap( ":/ZS/App/ModeEdit" + strSize + ".bmp" );
        pxm.setMask(pxm.createHeuristicMask());
    }
    else if( i_iMode == static_cast<int>(EMode::Simulation) )
    {
        pxm = QPixmap( ":/ZS/App/ModeSimulation" + strSize + ".bmp" );
        pxm.setMask(pxm.createHeuristicMask());
    }
    return pxm;

} // mode2Pixmap


