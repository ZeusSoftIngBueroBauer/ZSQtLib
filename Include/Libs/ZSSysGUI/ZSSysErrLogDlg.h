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

#ifndef ZSSysGUI_ErrLogDlg_h
#define ZSSysGUI_ErrLogDlg_h

#include "ZSSysGUI/ZSSysDialog.h"
#include "ZSSysGUI/ZSSysErrLogModel.h"
#include "ZSSysGUI/ZSSysGUIDllMain.h"

class QVBoxLayout;

namespace ZS
{
namespace System
{
namespace GUI
{
class CWdgtErrLog;

//******************************************************************************
class ZSSYSGUIDLL_API CDlgErrLog : public CDialog
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CDlgErrLog"; }
public: // class methods
    static CDlgErrLog* CreateInstance(
        const QString&  i_strObjName,
        const QString&  i_strDlgTitle,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags() );
    static CDlgErrLog* GetInstance( const QString& i_strObjName );
protected: // ctor
    CDlgErrLog(
        const QString&  i_strObjName,
        const QString&  i_strDlgTitle,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags());
public: // dtor
    virtual ~CDlgErrLog();
public: // overridables of base class ZS::System::GUI::CDialog
    virtual QString nameSpace() override { return NameSpace(); }
    virtual QString className() override { return ClassName(); }
public: // instance methods
    void showColumn( CModelErrLog::EColumn i_clm );
    void hideColumn( CModelErrLog::EColumn i_clm );
protected: // instance members
    QVBoxLayout* m_pLyt;
    CWdgtErrLog* m_pWdgtErrLog;

}; // class CDlgErrLog

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_ErrLogDlg_h
