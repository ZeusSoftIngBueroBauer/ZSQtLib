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

#ifndef ZSSysGUI_DllMain_h
#define ZSSysGUI_DllMain_h

#include "ZSSys/ZSSysDllMain.h"

#include <QtGui/qbitmap.h>
#include <QtGui/qpixmap.h>
#include <QtGui/qicon.h>

#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrCode.h"

class QMainWindow;

// specific export defines
#ifdef ZSSYSGUIDLL_EXPORTS
    #define ZSSYSGUIDLL_API __API_EXPORT
#else
    #define ZSSYSGUIDLL_API __API_IMPORT
#endif

namespace ZS
{
namespace System
{
namespace GUI
{
/*******************************************************************************
global type definitions and constants
*******************************************************************************/

//==============================================================================
enum EViewMode
//==============================================================================
{
    EViewModeTableView = 0,
    EViewModeTreeView  = 1,
    EViewModeCount,
    EViewModeUndefined
};

ZSSYSGUIDLL_API QString viewMode2Str( int i_direction, ZS::System::EEnumEntryAliasStr i_alias = ZS::System::EEnumEntryAliasStrName );
ZSSYSGUIDLL_API EViewMode str2ViewMode( const QString& i_str, ZS::System::EEnumEntryAliasStr i_alias = ZS::System::EEnumEntryAliasStrName );

/*******************************************************************************
global auxiliary methods
*******************************************************************************/

ZSSYSGUIDLL_API QMainWindow* getMainWindowOfApp();
ZSSYSGUIDLL_API QString getMainWindowTitle();
ZSSYSGUIDLL_API QPixmap getErrPixmap( EResultSeverity i_severity, const QSize& i_sz = QSize(24,24) );
ZSSYSGUIDLL_API QIcon getErrIcon( EResultSeverity i_severity );
ZSSYSGUIDLL_API QPixmap mode2Pixmap( int i_iMode, int i_iSize = 16 ); // Range 16, 24, 32

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_DllMain_h
