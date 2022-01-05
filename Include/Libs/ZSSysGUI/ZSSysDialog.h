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

#ifndef ZSSysGUI_Dialog_h
#define ZSSysGUI_Dialog_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qdialog.h>
#else
#include <QtWidgets/qdialog.h>
#endif

#include "ZSSysGUI/ZSSysGUIDllMain.h"

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}

namespace System
{
namespace GUI
{
//******************************************************************************
class ZSSYSGUIDLL_API CDialog : public QDialog
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CDialog"; }
public: // class methods (CreateInstance must be implemented by the derived class)
    //static CDialog* CreateInstance(
    //    const QString&  i_strNameSpace,
    //    const QString&  i_strClassName,
    //    const QString&  i_strObjName,
    //    const QString&  i_strDlgTitle,
    //    QWidget*        i_pWdgtParent = nullptr,
    //    Qt::WindowFlags i_wFlags = Qt::WindowFlags() );
public: // class methods
    static CDialog* GetInstance( const QString& i_strNameSpace, const QString& i_strClassName, const QString& i_strObjName );
    static void DestroyInstance( const QString& i_strNameSpace, const QString& i_strClassName, const QString& i_strObjName );
    static void DestroyInstance( CDialog* i_pDlg );
    static void DestroyAllInstances();
    static void HideAllInstances();
protected: // ctors
    CDialog(
        const QString&  i_strNameSpace,
        const QString&  i_strClassName,
        const QString&  i_strObjName,
        const QString&  i_strDlgTitle,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags());
public: // dtor
    virtual ~CDialog();
public: // overridables
    virtual QString nameSpace() { return NameSpace(); }
    virtual QString className() { return ClassName(); }
public: // instance methods
    void show();
    void hide();
protected: // overridables of base class QWidget
    virtual void closeEvent( QCloseEvent* i_pEv ) override;
protected: // class members
    static QHash<QString,CDialog*> s_hshpDlgs; // Key is string concatenated by NameSpace::ClassName::ObjName
protected: // instance members
    QString              m_strNameSpace;
    QString              m_strClassName;
    Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CDialog

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_Dialog_h
