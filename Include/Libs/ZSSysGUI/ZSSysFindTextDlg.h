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

#ifndef ZSSysGUI_FindTextDlg_h
#define ZSSysGUI_FindTextDlg_h

#include "ZSSysGUI/ZSSysDialog.h"

class QLabel;
class QLineEdit;
class QPushButton;
class QTextEdit;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
namespace GUI
{
//******************************************************************************
class ZSSYSGUIDLL_API CDlgFindText : public CDialog
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CDlgFindText"; }
public: // class methods
    static CDlgFindText* CreateInstance(
        const QString&  i_strDlgTitle,
        const QString&  i_strObjName,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags() );
    static CDlgFindText* GetInstance( const QString& i_strObjName );
protected: // ctor
    CDlgFindText(
        const QString&  i_strDlgTitle,
        const QString&  i_strObjName,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags());
public: // dtor
    virtual ~CDlgFindText();
public: // instance methods
    void setTextEdit( QTextEdit* i_pEdt );
public: // instance methods
    void findNext();
    void findPrev();
protected slots:
    void onBtnFindNextClicked( bool i_bChecked );
    void onBtnFindPrevClicked( bool i_bChecked );
protected: // instance members
    QTextEdit*   m_pEdt;
    QVBoxLayout* m_pLyt;
    QHBoxLayout* m_pLytFindText;
    QLineEdit*   m_pEdtFindText;
    QPushButton* m_pBtnFindNext;
    QPushButton* m_pBtnFindPrev;
    QHBoxLayout* m_pLytFindTextResult;
    QLabel*      m_pLblFindTextResult;

}; // class CDlgFindText

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_FindTextDlg_h
