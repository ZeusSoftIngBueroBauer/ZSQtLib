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

    return new CDlgIpcServer(
        /* strDlgTitle  */ i_strDlgTitle,
        /* strObjName   */ i_strObjName,
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
    m_pLyt(nullptr),
    m_pWdgtIpcServer(nullptr)
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    setWindowTitle(i_strDlgTitle);

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    m_pWdgtIpcServer = new CWdgtIpcServer(objectName());
    m_pLyt->addWidget(m_pWdgtIpcServer);

    QObject::connect(
        m_pWdgtIpcServer, &CWdgtIpcServer::accepted,
        this, &CDlgIpcServer::onSettingsAccepted);
    QObject::connect(
        m_pWdgtIpcServer, &CWdgtIpcServer::rejected,
        this, &CDlgIpcServer::onSettingsRejected);
    QObject::connect(
        m_pWdgtIpcServer, &CWdgtIpcServer::detailsVisibilityChanged,
        this, &CDlgIpcServer::onWdgtIpcServerDetailsVisibilityChanged);

} // ctor

//------------------------------------------------------------------------------
CDlgIpcServer::~CDlgIpcServer()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    mthTracer.onAdminObjAboutToBeReleased();

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
            m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
        }
    }

    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "Server: " + QString( i_pServer == nullptr ? "nullptr" : i_pServer->objectName() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setServer",
        /* strAddInfo   */ strMthInArgs );

    m_pWdgtIpcServer->setServer(i_pServer);

} // setServer

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgIpcServer::setProtocolTypeImage( const QPixmap& i_pxm )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setProtocolTypeImage",
        /* strAddInfo   */ strAddTrcInfo );

    m_pWdgtIpcServer->setProtocolTypeImage(i_pxm);

} // setProtocolTypeImage

//------------------------------------------------------------------------------
void CDlgIpcServer::setProtocolTypeImageVisible( bool i_bVisible )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strAddTrcInfo = bool2Str(i_bVisible);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
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
