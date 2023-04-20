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

#ifndef ZSSysGUI_LastUsedFileDlg_h
#define ZSSysGUI_LastUsedFileDlg_h

#include "ZSSysGUI/ZSSysDialog.h"
#include "ZSSys/ZSSysApp.h"

class QLabel;
class QPushButton;
class QTreeWidget;
class QTreeWidgetItem;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
namespace GUI
{
//******************************************************************************
/*! @brief Dialog providing a tree view to select one of the last used files.

    The list of last used files is retrieved from QSettings using the applications
    registry path and the key "LastUsedFiles".

    The last used file may be chosen be double clicking on the correspondig item
    in the tree widget. In this case the return code of the dialogs "exec" function
    is QDialog::Accepted.

    In addition the user may press the "Open ..." button to select any file using
    a file dialog. If a file has been chosen the return code from the dialogs "exec"
    function is also QDialog::Accepted.

    The selected file may be retrieved by invoking the class method "getSelectedFile".

    If the user don't want to continue with an already existing file he may press
    the "New ..." button and the return code of the dialogs "exec" function is
    QDialog::Rejected.

    @Note If the "exec" function returns the dialog instance has already been
          destroyed. For this reason the selected file is kept as a class member
          so it can be retrieved also if the dialogs destructor has been called.

    @Note This dialog class can only be used as a singleton for now.
          If more than one instance of the class should be created at the same
          time the class member "s_selectedFile" must be kept for each instance
          separately using e.g. a hash and a unique name for the dialog instance.
*/
class ZSSYSGUIDLL_API CDlgOpenLastUsedFile : public CDialog
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CDlgOpenLastUsedFile"; }
public: // class methods
    static CDlgOpenLastUsedFile* CreateInstance(
        const QString&  i_strDlgTitle,
        const QString&  i_strObjName = "OpenLastUsedFile",
        int             i_iLastUsedFilesCountMax = 10,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags() );
    static CDlgOpenLastUsedFile* GetInstance( const QString& i_strObjName = "OpenLastUsedFile" );
public: // class methods
    static SLastUsedFile getSelectedFile();
protected: // ctor
    CDlgOpenLastUsedFile(
        const QString&  i_strDlgTitle,
        const QString&  i_strObjName,
        int             i_iLastUsedFilesCountMax,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags());
public: // dtor
    virtual ~CDlgOpenLastUsedFile();
public: // overridables of base class ZS::System::GUI::CDialog
    virtual QString nameSpace() override { return NameSpace(); }
    virtual QString className() override { return ClassName(); }
protected: // instance methods
    void updateTreeWdgtLastUsedFiles();
    void updateButtonFileOpenLastUsedDir();
protected slots:
    void onTreeWidgetItemClicked(QTreeWidgetItem* i_pItem, int i_iColumn);
    void onTreeWidgetItemDoubleClicked(QTreeWidgetItem* i_pItem, int i_iColumn);
    void onBtnFileOpenUserDocDirClicked(bool i_bChecked = false);
    void onBtnFileOpenLastUsedDirClicked(bool i_bChecked = false);
    void onBtnFileNewClicked(bool i_bChecked = false);
protected: // class members
    static SLastUsedFile s_selectedFile;
protected: // instance members
    QVBoxLayout*  m_pLyt;
    QHBoxLayout*  m_pLytHeadline;
    QLabel*       m_pLblHeadline;
    QTreeWidget*  m_pTreeWdgtLastUsedFiles;
    QHBoxLayout*  m_pLytButtons;
    QPushButton*  m_pBtnFileOpenUserDocDir;
    QPushButton*  m_pBtnFileOpenLastUsedDir;
    QPushButton*  m_pBtnFileNew;
    QList<SLastUsedFile> m_arLastUsedFiles;

}; // class CDlgOpenLastUsedFile

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_LastUsedFileDlg_h
