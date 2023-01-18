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

#ifndef ZSSysGUI_EditStringValueDlg_h
#define ZSSysGUI_EditStringValueDlg_h

#include "ZSSysGUI/ZSSysDialog.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qspinbox.h>
#else
#include <QtWidgets/qspinbox.h>
#endif

class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QLineEdit;
class QPushButton;

namespace ZS
{
namespace System
{
namespace GUI
{
//******************************************************************************
class ZSSYSGUIDLL_API CDlgEditStringValue : public CDialog
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CDlgEditStringValue"; }
public: // class methods
    static CDlgEditStringValue* CreateInstance(
        const QString&  i_strDlgTitle,
        const QString&  i_strObjName,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags() );
    static CDlgEditStringValue* GetInstance( const QString& i_strObjName );
protected: // ctor
    CDlgEditStringValue(
        const QString&  i_strDlgTitle,
        const QString&  i_strObjName,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags());
signals:
    void applied();
    //void accepted(); inherited from QDialog
    //void rejected(); inherited from QDialog
public: // dtor
    virtual ~CDlgEditStringValue();
public: // overridables of base class ZS::System::GUI::CDialog
    virtual QString nameSpace() override { return NameSpace(); }
    virtual QString className() override { return ClassName(); }
public: // overridables of base class QDialog
   virtual int exec() override;
public: // instance methods
    void setValueName( const QString& i_strName );
    QString getValueName() const;
    void setValue( const QString& i_strVal );
    QString getValue() const;
    void setDescription( const QString& i_strDescription );
protected slots:
    void onBtnApplyClicked( bool i_bChecked );
    void onBtnOkClicked( bool i_bChecked );
    void onBtnCancelClicked( bool i_bChecked );
protected: // instance members
    QVBoxLayout*   m_pLyt;
    QHBoxLayout*   m_pLytValue;
    QLabel*        m_pLblValue;
    QLineEdit*     m_pEdtValue;
    QString        m_strValOrig;
    QVBoxLayout*   m_pLytDescription;
    QLabel*        m_pLblDescription;
    QHBoxLayout*   m_pLytBtns;
    QPushButton*   m_pBtnApply;
    QPushButton*   m_pBtnOk;
    QPushButton*   m_pBtnCancel;

}; // class CDlgEditStringValue

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_EditStringValueDlg_h
