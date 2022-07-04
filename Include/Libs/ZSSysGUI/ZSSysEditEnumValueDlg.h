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

#ifndef ZSSysGUI_EditEnumValueDlg_h
#define ZSSysGUI_EditEnumValueDlg_h

#include "ZSSysGUI/ZSSysDialog.h"

class QComboBox;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QPushButton;

namespace ZS
{
namespace System
{
namespace GUI
{
//******************************************************************************
class ZSSYSGUIDLL_API CDlgEditEnumValue : public CDialog
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CDlgEditEnumValue"; }
public: // class methods
    static CDlgEditEnumValue* CreateInstance(
        const QString&  i_strDlgTitle,
        const QString&  i_strObjName,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags() );
    static CDlgEditEnumValue* GetInstance( const QString& i_strObjName );
protected: // ctor
    CDlgEditEnumValue(
        const QString&  i_strDlgTitle,
        const QString&  i_strObjName,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags());
signals:
    void applied();
    //void accepted(); inherited from QDialog
    //void rejected(); inherited from QDialog
public: // dtor
    virtual ~CDlgEditEnumValue();
public: // overridables of base class ZS::System::GUI::CDialog
    virtual QString nameSpace() override { return NameSpace(); }
    virtual QString className() override { return ClassName(); }
public: // overridables of base class QDialog
   virtual int exec() override;
public: // instance methods
    void setValueName( const QString& i_strName );
    QString getValueName() const;
    void setComboItems( const QVector<SEnumEntry>& i_arEnumEntries, int i_idxAliasToBeIndicated = EEnumEntryAliasStrName );
    void setEnumerator( int i_iEnumerator );
    void setEnumerator( const QString& i_strEnumerator );
    int getEnumerator() const;
    QString getEnumeratorName( int i_idxAlias = EEnumEntryAliasStrName ) const;
    void setDescription( const QString& i_strDescription );
protected slots:
    void onBtnApplyClicked( bool i_bChecked );
    void onBtnOkClicked( bool i_bChecked );
    void onBtnCancelClicked( bool i_bChecked );
protected: // instance members
    QVBoxLayout*        m_pLyt;
    QHBoxLayout*        m_pLytValue;
    QLabel*             m_pLblValue;
    QVector<SEnumEntry> m_arEnumEntries;
    QComboBox*          m_pCmbValue;
    int                 m_iValOrig;
    QVBoxLayout*        m_pLytDescription;
    QLabel*             m_pLblDescription;
    QHBoxLayout*        m_pLytBtns;
    QPushButton*        m_pBtnApply;
    QPushButton*        m_pBtnOk;
    QPushButton*        m_pBtnCancel;

}; // class CDlgEditEnumValue

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_EditEnumValueDlg_h
