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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qsettings.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qlayout.h>
#include <QtGui/qtabwidget.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qtabwidget.h>
#endif

#include "ZSIpcLogGUI/ZSIpcLogServerDlg.h"
#include "ZSIpcLogGUI/ZSIpcLogSettingsWdgt.h"
#include "ZSIpcGUI/ZSIpcServerWdgt.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Ipc::GUI;
using namespace ZS::Log;
using namespace ZS::Log::GUI;


/*******************************************************************************
class CDlgLogServer : public ZS::System::GUI::CDialog
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CDlgLogServer* CDlgLogServer::CreateInstance(
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

    return new CDlgLogServer(
        /* strDlgTitle  */ i_strDlgTitle,
        /* strObjName   */ i_strObjName,
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags );

} // CreateInstance

//------------------------------------------------------------------------------
CDlgLogServer* CDlgLogServer::GetInstance( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CDlgLogServer*>(CDialog::GetInstance(NameSpace(), ClassName(), i_strObjName));
}

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgLogServer::CDlgLogServer(
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
    m_pIpcLogServer(nullptr),
    m_pLyt(nullptr),
    m_pTabWidget(nullptr),
    m_pWdgtIpcServer(nullptr),
    m_pWdgtLogSettings(nullptr)
{
    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    m_pTabWidget = new QTabWidget();
    m_pLyt->addWidget(m_pTabWidget);

    CIpcLogServer* pLogServer = CIpcLogServer::GetInstance();

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

    // Log Settings
    //-------------

    m_pWdgtLogSettings = new CWdgtLogSettings();
    m_pTabWidget->addTab(m_pWdgtLogSettings, "Log Settings");

    if( !QObject::connect(
        /* pObjSender   */ m_pWdgtLogSettings,
        /* szSignal     */ SIGNAL(accepted()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onLogSettingsAccepted()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pWdgtLogSettings,
        /* szSignal     */ SIGNAL(rejected()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onLogSettingsRejected()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // Assign server to widgets if there is one already defined.
    //----------------------------------------------------------

    if( pLogServer != nullptr)
    {
        m_pWdgtIpcServer->setServer(pLogServer->getIpcServer());
        m_pWdgtLogSettings->setServer(pLogServer);
    }

} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgLogServer::~CDlgLogServer()
//------------------------------------------------------------------------------
{
    m_pIpcLogServer = nullptr;
    m_pLyt = nullptr;
    m_pTabWidget = nullptr;
    m_pWdgtIpcServer = nullptr;
    m_pWdgtLogSettings = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgLogServer::setServer( CIpcLogServer* i_pLogServer )
//------------------------------------------------------------------------------
{
    m_pWdgtIpcServer->setServer(i_pLogServer->getIpcServer());
    m_pWdgtLogSettings->setServer(i_pLogServer);
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgLogServer::onIpcServerSettingsAccepted()
//------------------------------------------------------------------------------
{
    if( m_pWdgtLogSettings->hasChanges() )
    {
        m_pWdgtLogSettings->applyChanges();
    }
    hide();
}

//------------------------------------------------------------------------------
void CDlgLogServer::onIpcServerSettingsRejected()
//------------------------------------------------------------------------------
{
    hide();
}

//------------------------------------------------------------------------------
void CDlgLogServer::onLogSettingsAccepted()
//------------------------------------------------------------------------------
{
    m_pWdgtIpcServer->applySettings();

    hide();
}

//------------------------------------------------------------------------------
void CDlgLogServer::onLogSettingsRejected()
//------------------------------------------------------------------------------
{
    hide();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgLogServer::onWdgtIpcServerDetailsVisibilityChanged( bool i_bDetailsVisible )
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
}
