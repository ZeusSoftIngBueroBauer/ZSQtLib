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

#ifndef ZSSysGUI_LoggerIdxTreeDlg_h
#define ZSSysGUI_LoggerIdxTreeDlg_h

#include <QtCore/qglobal.h>

#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSSysGUI/ZSSysDialog.h"
#include "ZSSys/ZSSysAux.h"

class QVBoxLayout;

namespace ZS
{
namespace System
{
class CIdxTreeLoggers;

namespace GUI
{
class CWdgtIdxTreeLoggers;

//******************************************************************************
class ZSSYSGUIDLL_API CDlgIdxTreeLoggers : public CDialog
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CDlgIdxTreeLoggers"; }
public: // class CDlgTrcServer
    static CDlgIdxTreeLoggers* CreateInstance(
        const QString& i_strDlgTitle,
        CIdxTreeLoggers* i_pIdxTree,
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags(),
        ZS::System::EMethodTraceDetailLevel i_eTrcDetailLevel = ZS::System::EMethodTraceDetailLevel::None,
        ZS::System::EMethodTraceDetailLevel i_eTrcDetailLevelNoisyMethods = ZS::System::EMethodTraceDetailLevel::None );
    static CDlgIdxTreeLoggers* GetInstance( const QString& i_strObjName );
protected: // ctor
    CDlgIdxTreeLoggers(
        const QString& i_strDlgTitle,
        CIdxTreeLoggers* i_pIdxTree,
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags(),
        ZS::System::EMethodTraceDetailLevel i_eTrcDetailLevel = ZS::System::EMethodTraceDetailLevel::None,
        ZS::System::EMethodTraceDetailLevel i_eTrcDetailLevelNoisyMethods = ZS::System::EMethodTraceDetailLevel::None );
public: // dtor
    virtual ~CDlgIdxTreeLoggers();
protected: // instance members
    CIdxTreeLoggers*     m_pIdxTree;
    QVBoxLayout*         m_pLyt;
    CWdgtIdxTreeLoggers* m_pWdgtIdxTree;

}; // class CDlgIdxTreeLoggers

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_LoggerIdxTreeDlg_h
