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

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qlayout.h>
#include <QtGui/qtabwidget.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qtabwidget.h>
#endif

#include "ZSIpcTraceGUI/ZSIpcTrcServerDlg.h"
#include "ZSIpcTraceGUI/ZSIpcTrcSettingsWdgt.h"
#include "ZSIpcGUI/ZSIpcServerWdgt.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Ipc::GUI;
using namespace ZS::Trace;
using namespace ZS::Trace::GUI;


/*******************************************************************************
class CDlgTrcServer : public ZS::System::GUI::CDialog
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CDlgTrcServer* CDlgTrcServer::CreateInstance(
    const QString&  i_strDlgTitle,
    const QString&  i_strObjName,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wFlags )
//------------------------------------------------------------------------------
{
    if( CDialog::GetInstance(NameSpace(), ClassName(), i_strObjName) != nullptr )
    {
        QString strKey = buildPathStr("::", NameSpace(), ClassName(), i_strObjName);
        throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, strKey);
    }

    return new CDlgTrcServer(
        /* strDlgTitle  */ i_strDlgTitle,
        /* strObjName   */ i_strObjName,
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags );

} // CreateInstance

//------------------------------------------------------------------------------
CDlgTrcServer* CDlgTrcServer::GetInstance( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CDlgTrcServer*>(CDialog::GetInstance(NameSpace(), ClassName(), i_strObjName));
}

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgTrcServer::CDlgTrcServer(
    const QString&  i_strDlgTitle,
    const QString&  i_strObjName,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wFlags ) :
//------------------------------------------------------------------------------
    CDialog(
        /* strDlgTitle  */ i_strDlgTitle,
        /* strNameSpace */ NameSpace(),
        /* strClassName */ ClassName(),
        /* strObjName   */ i_strObjName,
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags ),
    m_pIpcTrcServer(nullptr),
    m_pLyt(nullptr),
    m_pTabWidget(nullptr),
    m_pWdgtIpcServer(nullptr),
    m_pWdgtTrcSettings(nullptr)
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    m_pTabWidget = new QTabWidget();
    m_pLyt->addWidget(m_pTabWidget);

    CIpcTrcServer* pTrcServer = CIpcTrcServer::GetInstance();

    // IPC Connection Settings
    //------------------------

    m_pWdgtIpcServer = new ZS::Ipc::GUI::CWdgtIpcServer(i_strObjName);
    m_pTabWidget->addTab(m_pWdgtIpcServer, "Connection Settings");

    if( !QObject::connect(
        /* pObjSender   */ m_pWdgtIpcServer,
        /* szSignal     */ SIGNAL(accepted()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onIpcServerSettingsAccepted()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pWdgtIpcServer,
        /* szSignal     */ SIGNAL(rejected()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onIpcServerSettingsRejected()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pWdgtIpcServer,
        /* szSignal     */ SIGNAL(detailsVisibilityChanged(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onWdgtIpcServerDetailsVisibilityChanged(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // Trace Settings
    //---------------

    m_pWdgtTrcSettings = new CWdgtTrcSettings();
    m_pTabWidget->addTab(m_pWdgtTrcSettings, "Trace Settings");

    if( !QObject::connect(
        /* pObjSender   */ m_pWdgtTrcSettings,
        /* szSignal     */ SIGNAL(accepted()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTrcSettingsAccepted()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pWdgtTrcSettings,
        /* szSignal     */ SIGNAL(rejected()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTrcSettingsRejected()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // Assign server to widgets if there is one already defined.
    //----------------------------------------------------------

    if( pTrcServer != nullptr)
    {
        m_pWdgtIpcServer->setServer(pTrcServer->getIpcServer());
        m_pWdgtTrcSettings->setServer(pTrcServer);
    }

} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgTrcServer::~CDlgTrcServer()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_pIpcTrcServer = nullptr;
    m_pLyt = nullptr;
    m_pTabWidget = nullptr;
    m_pWdgtIpcServer = nullptr;
    m_pWdgtTrcSettings = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgTrcServer::setServer( CIpcTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Server: " + QString( i_pTrcServer == nullptr ? "nullptr" : i_pTrcServer->objectName() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setServer",
        /* strAddInfo   */ strMthInArgs );

    m_pWdgtIpcServer->setServer(i_pTrcServer->getIpcServer());
    m_pWdgtTrcSettings->setServer(i_pTrcServer);

} // setServer

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgTrcServer::onIpcServerSettingsAccepted()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onIpcServerSettingsAccepted",
        /* strAddInfo   */ "" );

    if( m_pWdgtTrcSettings->hasChanges() )
    {
        m_pWdgtTrcSettings->applyChanges();
    }
    hide();
}

//------------------------------------------------------------------------------
void CDlgTrcServer::onIpcServerSettingsRejected()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onIpcServerSettingsRejected",
        /* strAddInfo   */ "" );

    hide();
}

//------------------------------------------------------------------------------
void CDlgTrcServer::onTrcSettingsAccepted()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onTrcSettingsAccepted",
        /* strAddInfo   */ "" );

    m_pWdgtIpcServer->applySettings();

    hide();
}

//------------------------------------------------------------------------------
void CDlgTrcServer::onTrcSettingsRejected()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onTrcSettingsRejected",
        /* strAddInfo   */ "" );

    hide();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgTrcServer::onWdgtIpcServerDetailsVisibilityChanged( bool i_bDetailsVisible )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = bool2Str(i_bDetailsVisible);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onWdgtIpcClientDetailsVisibilityChanged",
        /* strAddInfo   */ strMthInArgs );

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

} // onWdgtIpcServerDetailsVisibilityChanged
