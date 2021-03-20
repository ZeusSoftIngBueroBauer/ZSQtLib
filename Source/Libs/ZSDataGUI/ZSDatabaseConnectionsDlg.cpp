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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qsettings.h>

#if QT_VERSION < 0x050000
#include <QtGui/qlayout.h>
#else
#include <QtWidgets/qlayout.h>
#endif

#include "ZSDataGUI/ZSDatabaseConnectionsDlg.h"
#include "ZSDataGUI/ZSDatabaseConnectionsWdgt.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Data;
using namespace ZS::Data::GUI;


/*******************************************************************************
class CDlgDbConnections : public ZS::System::GUI::CDialog
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CDlgDbConnections* CDlgDbConnections::CreateInstance(
    const QString&  i_strObjName,
    const QString&  i_strDlgTitle,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wFlags )
//------------------------------------------------------------------------------
{
    if( CDialog::GetInstance(NameSpace(), ClassName(), i_strObjName) != nullptr )
    {
        QString strKey = buildPathStr("::", NameSpace(), ClassName(), i_strObjName);
        throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, strKey);
    }

    return new CDlgDbConnections(
        /* strObjName   */ i_strObjName,
        /* strDlgTitle  */ i_strDlgTitle,
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags );

} // CreateInstance

//------------------------------------------------------------------------------
CDlgDbConnections* CDlgDbConnections::GetInstance( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CDlgDbConnections*>(CDialog::GetInstance(NameSpace(), ClassName(), i_strObjName));
}

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgDbConnections::CDlgDbConnections(
    const QString&  i_strObjName,
    const QString&  i_strDlgTitle,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wFlags ) :
//------------------------------------------------------------------------------
    CDialog(
        /* strNameSpace */ NameSpace(),
        /* strClassName */ ClassName(),
        /* strObjName   */ i_strObjName,
        /* strDlgTitle  */ i_strDlgTitle,
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags ),
    m_pDb(nullptr),
    m_pLyt(nullptr),
    m_pWdgtCncts(nullptr)
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    m_pWdgtCncts = new CWdgtDbConnections();
    m_pLyt->addWidget(m_pWdgtCncts);

    if( !QObject::connect(
        /* pObjSender   */ m_pWdgtCncts,
        /* szSignal     */ SIGNAL(accepted()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onWdgtCnctsAccepted()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pWdgtCncts,
        /* szSignal     */ SIGNAL(rejected()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onWdgtCnctsRejected()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pWdgtCncts,
        /* szSignal     */ SIGNAL(detailsVisibilityChanged(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onWdgtCnctsDetailsVisibilityChanged(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgDbConnections::~CDlgDbConnections()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_pDb = nullptr;
    m_pLyt = nullptr;
    m_pWdgtCncts = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgDbConnections::setDb( CDb* i_pDb )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Db: " + QString( i_pDb == nullptr ? "nullptr" : i_pDb->objectName() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setDb",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pDb != i_pDb )
    {
        if( m_pDb != nullptr )
        {
        } // if( m_pDb != nullptr )

        m_pDb = i_pDb;

        m_pWdgtCncts->setDb(m_pDb);

        if( m_pDb != nullptr )
        {
        } // if( m_pDb != nullptr )
    } // if( m_pDb != i_pDb )

} // setDb

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgDbConnections::show()
//------------------------------------------------------------------------------
{
    // Restore position and size of dialog
    //====================================

    QSettings settings;

    restoreGeometry( settings.value(objectName()+"/Geometry").toByteArray() );

    // Call base method to show the dialog
    //------------------------------------

    QDialog::show();

} // show

//------------------------------------------------------------------------------
void CDlgDbConnections::hide()
//------------------------------------------------------------------------------
{
    // Save position and size of dialog
    //------------------------------------

    QSettings settings;

    settings.setValue( objectName()+"/Geometry", saveGeometry() );

    // Call base method to hide the dialog
    //------------------------------------

    QDialog::hide();

} // hide

/*==============================================================================
protected: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgDbConnections::closeEvent( QCloseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    // Save position and size of dialog
    //=================================

    QSettings settings;

    settings.setValue( objectName()+"/Geometry", saveGeometry() );

    // Call base method to hide the dialog
    //------------------------------------

    QDialog::closeEvent(i_pEv);

} // closeEvent

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgDbConnections::onWdgtCnctsAccepted()
//------------------------------------------------------------------------------
{
    hide();
}

//------------------------------------------------------------------------------
void CDlgDbConnections::onWdgtCnctsRejected()
//------------------------------------------------------------------------------
{
    hide();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgDbConnections::onWdgtCnctsDetailsVisibilityChanged( bool /*i_bDetailsVisible*/ )
//------------------------------------------------------------------------------
{
    QSize sizeWdgtProposed(280,120);
    QSize sizeWdgt = sizeHint();

    if( sizeWdgt.width() < sizeWdgtProposed.width() )
    {
        sizeWdgt.setWidth( sizeWdgtProposed.width() );
    }
    if( sizeWdgt.height() < sizeWdgtProposed.height() )
    {
        sizeWdgt.setHeight( sizeWdgtProposed.height() );
    }

    resize(sizeWdgt);

    adjustSize();

} // onWdgtCnctsDetailsVisibilityChanged
