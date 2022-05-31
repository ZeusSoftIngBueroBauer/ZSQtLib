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

#include "ZSIpcTraceGUI/ZSIpcTrcClientDlg.h"
#include "ZSIpcTraceGUI/ZSIpcTrcSettingsWdgt.h"
#include "ZSIpcGUI/ZSIpcClientWdgt.h"
#include "ZSSysGUI/ZSSysTrcAdminObjIdxTreeWdgt.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Ipc::GUI;
using namespace ZS::Trace;
using namespace ZS::Trace::GUI;


/*******************************************************************************
class CDlgTrcClient : public ZS::System::GUI::CDialog
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CDlgTrcClient* CDlgTrcClient::CreateInstance(
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

    return new CDlgTrcClient(
        /* strDlgTitle  */ i_strDlgTitle,
        /* strObjName   */ i_strObjName,
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags );

} // CreateInstance

//------------------------------------------------------------------------------
CDlgTrcClient* CDlgTrcClient::GetInstance( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CDlgTrcClient*>(CDialog::GetInstance(NameSpace(), ClassName(), i_strObjName));
}

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgTrcClient::CDlgTrcClient(
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
    m_pTrcClient(nullptr),
    m_pLyt(nullptr),
    m_pTabWidget(nullptr),
    m_pWdgtIpcClient(nullptr),
    m_pWdgtTrcSettings(nullptr)
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    m_pTabWidget = new QTabWidget();
    m_pLyt->addWidget(m_pTabWidget);

    m_pWdgtIpcClient = new ZS::Ipc::GUI::CWdgtIpcClient(objectName());
    m_pTabWidget->addTab( m_pWdgtIpcClient, "Connection Settings" );

    if( !QObject::connect(
        /* pObjSender   */ m_pWdgtIpcClient,
        /* szSignal     */ SIGNAL(accepted()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(hide()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pWdgtIpcClient,
        /* szSignal     */ SIGNAL(rejected()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(hide()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pWdgtIpcClient,
        /* szSignal     */ SIGNAL(detailsVisibilityChanged(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onWdgtIpcClientDetailsVisibilityChanged(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pWdgtTrcSettings = new CWdgtTrcSettings();
    m_pTabWidget->addTab( m_pWdgtTrcSettings, "Trace Settings" );

    if( !QObject::connect(
        /* pObjSender   */ m_pWdgtTrcSettings,
        /* szSignal     */ SIGNAL(accepted()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(hide()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pWdgtTrcSettings,
        /* szSignal     */ SIGNAL(rejected()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(hide()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CDlgTrcClient::~CDlgTrcClient()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_pTrcClient = nullptr;
    m_pLyt = nullptr;
    m_pTabWidget = nullptr;
    m_pWdgtIpcClient = nullptr;
    m_pWdgtTrcSettings = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgTrcClient::setClient( CIpcTrcClient* i_pTrcClient )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "Client: " + QString( i_pTrcClient == nullptr ? "nullptr" : i_pTrcClient->objectName() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setClient",
        /* strAddInfo   */ strMthInArgs );

    m_pWdgtIpcClient->setClient(i_pTrcClient);
    m_pWdgtTrcSettings->setClient(i_pTrcClient);

} // setClient

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgTrcClient::onWdgtIpcClientDetailsVisibilityChanged( bool i_bDetailsVisible )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = bool2Str(i_bDetailsVisible);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
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

} // onWdgtIpcClientDetailsVisibilityChanged
