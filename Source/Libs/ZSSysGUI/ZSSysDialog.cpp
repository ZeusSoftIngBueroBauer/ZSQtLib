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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qsettings.h>

#include "ZSSysGUI/ZSSysDialog.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CDialog : public QDialog
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

// Key is string concatenated by NameSpace::ClassName::ObjName
QHash<QString, CDialog*> CDialog::s_hshpDlgs;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns a pointer to the dialog whose unique key is defined by
           the NameSpace, ClassName and objectName.

    @param [in] i_strNameSpace
        Name space of the class derived from CDialog.
    @param [in] i_strClassName
        Class name of the class derived from CDialog.
    @param [in] i_strObjName
        Object name of the instance to be created.

    @return If a dialog with the with the given name space, class name and
            object name derived from CDialog is already existing the pointer
            to this dialog instance is returned.
            nullptr otherwise.
*/
CDialog* CDialog::GetInstance(
    const QString& i_strNameSpace,
    const QString& i_strClassName,
    const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    QString strKey = buildPathStr("::", i_strNameSpace, i_strClassName, i_strObjName);
    return s_hshpDlgs.value(strKey, nullptr);
}

//------------------------------------------------------------------------------
/*! @brief Destroys the given dialog and removes the dialog key
           (comprised of NameSpace::ClassName::ObjName) from the container
           with all existing dialogs derived from CDialog.

    @param [in] i_strNameSpace
        Name space of the class derived from CDialog.
    @param [in] i_strClassName
        Class name of the class derived from CDialog.
    @param [in] i_strObjName
        Object name of the instance to be created.

    @note If you call "exec" on the dialog the dialog will be deleted by the
          Qt framework on leaving the "exec" function. Don't call "DestroyInstance"
          afterwards. Instead you need to create the dialog again after calling
          "exec" if you want ot use it again.
*/
void CDialog::DestroyInstance(
    const QString& i_strNameSpace,
    const QString& i_strClassName,
    const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    QString strKey = buildPathStr("::", i_strNameSpace, i_strClassName, i_strObjName);
    CDialog* pDlg = s_hshpDlgs.value(strKey, nullptr);
    if (pDlg == nullptr) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, strKey);
    }
    // The dtor of the dialog removes the instance from the hash.
    delete pDlg;
    pDlg = nullptr;
}

//------------------------------------------------------------------------------
/*! @brief Destroys the given dialog and removes the dialog key
           (comprised of NameSpace::ClassName::ObjName) from the container
           with all existing dialogs derived from CDialog.

    @param i_pDlg Pointer to dialog to be destroyed.

    @note If you call "exec" on the dialog the dialog will be deleted by the
          Qt framework on leaving the "exec" function. Don't call "DestroyInstance"
          afterwards. Instead you need to create the dialog again after calling
          "exec" if you want ot use it again.
*/
void CDialog::DestroyInstance( CDialog* i_pDlg )
//------------------------------------------------------------------------------
{
    // The dtor of the dialog removes the instance from the hash.
    delete i_pDlg;
    i_pDlg = nullptr;
}

//------------------------------------------------------------------------------
/*! @brief Destroys all instances and removes their keys
           (comprised of NameSpace::ClassName::ObjName) from the container
           with all existing dialogs derived from CDialog.

    This method is useful on closing (terminating) an application.
*/
void CDialog::DestroyAllInstances()
//------------------------------------------------------------------------------
{
    while (!s_hshpDlgs.isEmpty()) {
        QHash<QString,CDialog*>::iterator itDlgs = s_hshpDlgs.begin();
        CDialog* pDlg = itDlgs.value();
        // The dtor of the dialog removes the instance from the hash.
        delete pDlg;
        pDlg = nullptr;
    }
    s_hshpDlgs.clear();
}

//------------------------------------------------------------------------------
/*! @brief Hides all instances derived from CDialog.

    This method is useful on closing (terminating) an application.
*/
void CDialog::HideAllInstances()
//------------------------------------------------------------------------------
{
    QHash<QString,CDialog*>::iterator itDlgs;
    for (itDlgs = s_hshpDlgs.begin(); itDlgs != s_hshpDlgs.end(); itDlgs++) {
        CDialog* pDlg = itDlgs.value();
        pDlg->hide();
    }
}

/*==============================================================================
protected: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates an instance of a dialog.

    This method is protected as instances derived from CDialog may only be created
    using the CreateInstance methods.

    The dialog enteres itself into the hash of dialogs.
    The key into this hash is the concatenated string of the name space and
    class name as passed to the constructor. This must be the name space and
    class name of the derived class.

    In addition the passed object name is appended to the name space and class name.

    @param [in] i_strNameSpace
        Name space of the class derived from CDialog.
    @param [in] i_strClassName
        Class name of the class derived from CDialog.
    @param [in] i_strObjName
        Object name of the instance to be created.

    @return If a dialog with the with the given name space, class name and
            object name derived from CDialog is already existing the pointer
            to this dialog instance is returned.
            nullptr otherwise.
*/
CDialog::CDialog(
    const QString&  i_strDlgTitle,
    const QString&  i_strNameSpace,
    const QString&  i_strClassName,
    const QString&  i_strObjName,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wFlags ) :
//------------------------------------------------------------------------------
    QDialog(i_pWdgtParent, i_wFlags),
    m_strNameSpace(i_strNameSpace),
    m_strClassName(i_strClassName),
    m_strObjName(i_strObjName),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        m_strNameSpace, m_strClassName, objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "CDialog::ctor",
        /* strAddInfo   */ "" );

    if (i_strDlgTitle.isEmpty()) {
        setWindowTitle( ZS::System::GUI::getMainWindowTitle() + ": " + i_strObjName );
    }
    else {
        setWindowTitle(i_strDlgTitle);
    }

    QString strKey = buildPathStr("::", m_strNameSpace, m_strClassName, m_strObjName);
    s_hshpDlgs[strKey] = this;

    QSettings settings;
    restoreGeometry( settings.value(strKey + "/Geometry").toByteArray() );

} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Destroys the dialog and removes the dialogs key
           (comprised of NameSpace::ClassName::ObjName) from the container
           with all existing dialogs derived from CDialog.
*/
CDialog::~CDialog()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "CDialog::dtor",
        /* strAddInfo   */ "" );

    QRect rct = geometry();

    QString strKey = buildPathStr("::", m_strNameSpace, m_strClassName, objectName());

    QSettings settings;
    settings.setValue( strKey + "/Geometry", saveGeometry() );

    if( s_hshpDlgs.contains(strKey) ) {
        s_hshpDlgs.remove(strKey);
    }

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    //m_strNameSpace;
    //m_strClassName;
    //m_strObjName;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Shows the dialog and restores the previously used geometry.
*/
void CDialog::show()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "CDialog::show",
        /* strAddInfo   */ "" );

    QString strKey = buildPathStr("::", m_strNameSpace, m_strClassName, objectName());
    QSettings settings;
    restoreGeometry( settings.value(strKey + "/Geometry").toByteArray() );
    QDialog::show();
}

//------------------------------------------------------------------------------
/*! @brief Hides the dialog and saves the currently used geometry.
*/
void CDialog::hide()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "CDialog::hide",
        /* strAddInfo   */ "" );

    QRect rct = geometry();

    QString strKey = buildPathStr("::", m_strNameSpace, m_strClassName, objectName());
    QSettings settings;
    settings.setValue( strKey + "/Geometry", saveGeometry() );
    QDialog::hide();
}

/*==============================================================================
protected: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Reimplemented method called on closing the dialog to save the currently used geometry.

    @param i_pEv Pointer to the Qt Event.
*/
void CDialog::closeEvent( QCloseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "CDialog::closeEvent",
        /* strAddInfo   */ "" );

    QRect rct = geometry();

    QString strKey = buildPathStr("::", m_strNameSpace, m_strClassName, objectName());
    QSettings settings;
    settings.setValue( strKey + "/Geometry", saveGeometry() );
    QDialog::closeEvent(i_pEv);
}
