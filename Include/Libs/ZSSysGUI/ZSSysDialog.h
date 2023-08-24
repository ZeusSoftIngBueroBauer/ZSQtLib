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
namespace System
{
class CTrcAdminObj;

namespace GUI
{
//******************************************************************************
/*! @brief Base class for dialogs which should only be created once.

    Dialogs may be opened via menu items, status bars or tool bars or by any
    other user action.

    If a dialog has already been opened via the menu bar by clicking on the
    action item on the tool bar not a new instance of a dialog should be created
    and shown, instead the already existing dialog should be put into front.

    For this the base dialog class keeps a map of all created dialogs derived
    from CDialog. The key into this map is the NameSpace, ClassName and objectName
    of the Dialog created.

    With the class method GetInstance it could be checked whether a dialog of
    the class and the given object instance is already existing.

    If the GetInstance method returns a valid pointer the show method of the
    returned dialog instance puts the dialog into front.

    If a dialog instance is not yet existing the CreateInstance method will
    create the dialog, enter the dialog with the given NameSpace, ClassName
    and objectName as key into the map of dialogs.

    @note If you call "exec" on the dialog the dialog will be deleted by the
          Qt framework on leaving the "exec" function and you cannot retrieve
          any further information from the dialog afterwards.
*/
class ZSSYSGUIDLL_API CDialog : public QDialog
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CDialog"; }
public: // class methods (CreateInstance must be implemented by the derived class)
    //static CDialog* CreateInstance(
    //    const QString&  i_strDlgTitle,
    //    const QString&  i_strNameSpace,
    //    const QString&  i_strClassName,
    //    const QString&  i_strObjName,
    //    QWidget*        i_pWdgtParent = nullptr,
    //    Qt::WindowFlags i_wFlags = Qt::WindowFlags() );
public: // class methods
    static CDialog* GetInstance(
        const QString& i_strNameSpace, const QString& i_strClassName, const QString& i_strObjName );
    static void DestroyInstance(
        const QString& i_strNameSpace, const QString& i_strClassName, const QString& i_strObjName );
    static void DestroyInstance( CDialog* i_pDlg );
    static void DestroyAllInstances();
    static void HideAllInstances();
protected: // ctors
    CDialog(
        const QString&  i_strDlgTitle,
        const QString&  i_strNameSpace,
        const QString&  i_strClassName,
        const QString&  i_strObjName,
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
    /*!< Hash with pointers to the already created dialogs derived from
         the CDialog base class. The key is a string concatenated as
         "NameSpace::ClassName::objectName" of the created dialog instance. */
    static QHash<QString, CDialog*> s_hshpDlgs;
protected: // instance members
    /*!< Name space of the derived dialog class. */
    QString m_strNameSpace;
    /*!< Class name of the derived dialog class. */
    QString m_strClassName;
    /*!< Object name of this instance. The object name may differ from the
         object name of the QObject class. The key of the dialog in the hash
         of dialogs is a concatenated string of the name space, class name and
         object name passed to the constructor. If the object name is later on
         changed via "QObject::setObjectName" the original key must not be changed
         so that on destroying the dialog the destructor can remove the dialog
         instance again from the hash of dialogs. */
    QString m_strObjName;
    /*!< Trace admin object for method tracing. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CDialog

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_Dialog_h
