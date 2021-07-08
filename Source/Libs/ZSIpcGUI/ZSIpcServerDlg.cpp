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

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qlayout.h>
#else
#include <QtWidgets/qlayout.h>
#endif

#include "ZSIpcGUI/ZSIpcServerDlg.h"
#include "ZSIpcGUI/ZSIpcServerWdgt.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Ipc;
using namespace ZS::Ipc::GUI;


/*******************************************************************************
class CDlgIpcServer : public QDialog
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CDlgIpcServer* CDlgIpcServer::CreateInstance(
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

    return new CDlgIpcServer(
        /* strObjName   */ i_strObjName,
        /* strDlgTitle  */ i_strDlgTitle,
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags );

} // CreateInstance

//------------------------------------------------------------------------------
CDlgIpcServer* CDlgIpcServer::GetInstance( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CDlgIpcServer*>(CDialog::GetInstance(NameSpace(), ClassName(), i_strObjName));
}

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgIpcServer::CDlgIpcServer(
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
    m_pLyt(nullptr),
    m_pWdgtIpcServer(nullptr)
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    setWindowTitle(i_strDlgTitle);

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    m_pWdgtIpcServer = new CWdgtIpcServer(objectName());
    m_pLyt->addWidget(m_pWdgtIpcServer);

    if( !QObject::connect(
        /* pObjSender   */ m_pWdgtIpcServer,
        /* szSignal     */ SIGNAL(accepted()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onSettingsAccepted()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pWdgtIpcServer,
        /* szSignal     */ SIGNAL(rejected()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onSettingsRejected()) ) )
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

} // ctor

//------------------------------------------------------------------------------
CDlgIpcServer::~CDlgIpcServer()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pLyt = nullptr;
    m_pWdgtIpcServer = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgIpcServer::setServer( CServer* i_pServer )
//------------------------------------------------------------------------------
{
    bool bIsTrcServer = false;

    // Cannot trace methods of trace server as this would result in endless recursion loops.
    if( i_pServer != nullptr && i_pServer->parent() != nullptr )
    {
        // The trace server hosts the IPC server as an instance member.
        // On creating the Ipc server instance the trace server sets itself as
        // the parent object of the Ipc server (see ctor of class ZS::Trace::CTrcServer).
        CTrcServer* pTrcServer = dynamic_cast<CTrcServer*>(i_pServer->parent());
        if( pTrcServer != nullptr )
        {
            bIsTrcServer = true;
        }
    }

    if( m_pTrcAdminObj != nullptr )
    {
        if( bIsTrcServer )
        {
            m_pTrcAdminObj->setEnabled(EEnabled::No);

            CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
            m_pTrcAdminObj = nullptr;
        }
    }
    else // if( m_pTrcAdminObj == nullptr )
    {
        if( !bIsTrcServer )
        {
            m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Ipc::GUI", "CDlgIpcServer", objectName());
        }
    }

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "Server: " + QString( i_pServer == nullptr ? "nullptr" : i_pServer->objectName() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setServer",
        /* strAddInfo   */ strMthInArgs );

    m_pWdgtIpcServer->setServer(i_pServer);

} // setServer

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgIpcServer::setServerObjectNameVisible( bool i_bVisible )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = bool2Str(i_bVisible);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setServerObjectNameVisible",
        /* strAddInfo   */ strAddTrcInfo );

    m_pWdgtIpcServer->setServerObjectNameVisible(i_bVisible);

} // setServerObjectNameVisible

//------------------------------------------------------------------------------
bool CDlgIpcServer::isServerObjectNameVisible() const
//------------------------------------------------------------------------------
{
    return m_pWdgtIpcServer->isServerObjectNameVisible();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgIpcServer::setProtocolTypeImage( const QPixmap& i_pxm )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setProtocolTypeImage",
        /* strAddInfo   */ strAddTrcInfo );

    m_pWdgtIpcServer->setProtocolTypeImage(i_pxm);

} // setProtocolTypeImage

//------------------------------------------------------------------------------
void CDlgIpcServer::setProtocolTypeImageVisible( bool i_bVisible )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = bool2Str(i_bVisible);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setProtocolTypeImageVisible",
        /* strAddInfo   */ strAddTrcInfo );

    m_pWdgtIpcServer->setProtocolTypeImageVisible(i_bVisible);

} // setProtocolTypeImageVisible

//------------------------------------------------------------------------------
bool CDlgIpcServer::isProtocolTypeImageVisible() const
//------------------------------------------------------------------------------
{
    return m_pWdgtIpcServer->isProtocolTypeImageVisible();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgIpcServer::onSettingsAccepted()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onSettingsAccepted",
        /* strAddInfo   */ "" );

    hide();

} // onSettingsAccepted

//------------------------------------------------------------------------------
void CDlgIpcServer::onSettingsRejected()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onSettingsRejected",
        /* strAddInfo   */ "" );

    hide();

} // onSettingsRejected

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgIpcServer::onWdgtIpcServerDetailsVisibilityChanged( bool i_bDetailsVisible )
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

} // onWdgtIpcServerDetailsVisibilityChanged
