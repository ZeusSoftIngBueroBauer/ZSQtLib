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

#include <QtCore/qdatetime.h>
#include <QtCore/qsettings.h>
#include <QtCore/qthread.h>
#include <QtCore/qtimer.h>
#include <QtGui/qvalidator.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qapplication.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qtextedit.h>
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qtextedit.h>
#endif

#include "ZSIpcGUI/ZSIpcTrcMsgLogWdgt.h"
#include "ZSIpc/ZSIpcClient.h"
#include "ZSIpc/ZSIpcServer.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMsg.h"
#include "ZSSys/ZSSysTime.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Ipc;
using namespace ZS::Ipc::GUI;


/*******************************************************************************
class CWdgtTrcMsgLog : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtTrcMsgLog::CWdgtTrcMsgLog(
    const STrcMsgLogSettings& i_settings,
    QWidget*                  i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pClient(nullptr),
    m_pServer(nullptr),
    m_settings(i_settings),
    m_pLyt(nullptr),
    m_pLytLineEditSettings(nullptr),
    m_pLblEnabled(nullptr),
    m_pChkEnabled(nullptr),
    m_pLblMaxItemCount(nullptr),
    m_pEdtMaxItemCount(nullptr),
    m_pBtnClear(nullptr),
    m_uLogItemCount(0),
    m_bLogIsFull(false),
    m_pEdtLog(nullptr)
{
    QFont fntListWdgt(
        /* strFamily  */ "Courier New",
        /* iPointSize */ 8,
        /* iWeight    */ -1,
        /* bItalic    */ false );

    m_pLyt = new QVBoxLayout();
    m_pLyt->setContentsMargins(0,0,0,0);
    setLayout(m_pLyt);

    int cxBtnWidth = 80;

    const int c_arcxGridLytClmWidth[] = {
        /*  0 */ 44,         // Label Enabled       (fixed width)
        /*  1 */ 20,         // Check Box Enabled   (fixed width)
        /*  2 */  5,         // Space
        /*  3 */ 60,         // Label Max Items       (fixed width)
        /*  4 */ 80,         // Edit field Max Items  (fixed width)
        /*  5 */  5,         // Space
        /*  6 */ cxBtnWidth, // Button Reset (fixed width)
        /*  7 */ cxBtnWidth, // Button Apply (fixed width)
        /*  8 */ cxBtnWidth, // Button Clear (fixed width)
        /*  9 */  5          // Not Used
    };

    int iClm;

    // Line 1
    //-------

    iClm = 0;

    m_pLytLineEditSettings = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytLineEditSettings);

    // <CheckBox> Message Log Enabled
    //-------------------------------

    m_pLblEnabled = new QLabel("Enabled:");
    m_pLblEnabled->setFixedWidth(c_arcxGridLytClmWidth[iClm]);
    m_pLytLineEditSettings->addWidget(m_pLblEnabled); iClm++;
    m_pChkEnabled = new QCheckBox();
    m_pChkEnabled->setChecked(m_settings.m_bEnabled);
    m_pChkEnabled->setFixedWidth(c_arcxGridLytClmWidth[iClm]);
    m_pLytLineEditSettings->addWidget(m_pChkEnabled); iClm++;
    m_pLytLineEditSettings->addSpacing(c_arcxGridLytClmWidth[iClm]); iClm++;

    if( !QObject::connect(
        /* pObjSender   */ m_pChkEnabled,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkEnabledToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <LineEdit> Maximum Number of Message Log Entries
    //-------------------------------------------------

    m_pLblMaxItemCount = new QLabel("Max Items:");
    m_pLblMaxItemCount->setFixedWidth(c_arcxGridLytClmWidth[iClm]);
    m_pLytLineEditSettings->addWidget(m_pLblMaxItemCount); iClm++;
    m_pEdtMaxItemCount = new QLineEdit( QString::number(m_settings.m_uMaxItems) );
    m_pEdtMaxItemCount->setValidator( new QIntValidator(0,99999,this) );
    m_pEdtMaxItemCount->setFixedWidth(c_arcxGridLytClmWidth[iClm]);
    m_pLytLineEditSettings->addWidget(m_pEdtMaxItemCount); iClm++;
    m_pLytLineEditSettings->addSpacing(c_arcxGridLytClmWidth[iClm]); iClm++;

    if( !QObject::connect(
        /* pObjSender   */ m_pEdtMaxItemCount,
        /* szSignal     */ SIGNAL(editingFinished()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtMaxItemCountEditingFinished()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Clear Log Widget
    //--------------------------

    m_pBtnClear = new QPushButton(tr("Clear"));
    m_pBtnClear->setFixedWidth(c_arcxGridLytClmWidth[iClm]);
    m_pLytLineEditSettings->addWidget(m_pBtnClear); iClm++;

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnClear,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnClearClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <EditField> Client Message Log Window
    //--------------------------------------

    m_pEdtLog = new QTextEdit(this);
    m_pEdtLog->setFont(fntListWdgt);
    m_pEdtLog->setLineWrapMode(QTextEdit::NoWrap);
    m_pLyt->addWidget(m_pEdtLog);

    m_pLytLineEditSettings->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWdgtTrcMsgLog::~CWdgtTrcMsgLog()
//------------------------------------------------------------------------------
{
    if( m_pClient != nullptr )
    {
        m_pClient->removeTrcMsgLogObject(this);
    }
    if( m_pServer != nullptr )
    {
        m_pServer->removeTrcMsgLogObject(this);
    }

    m_pClient = nullptr;
    m_pServer = nullptr;
    //m_settings;
    m_pLyt = nullptr;
    m_pLytLineEditSettings = nullptr;
    m_pLblEnabled = nullptr;
    m_pChkEnabled = nullptr;
    m_pLblMaxItemCount = nullptr;
    m_pEdtMaxItemCount = nullptr;
    m_pBtnClear = nullptr;
    m_uLogItemCount = 0;
    m_bLogIsFull = false;
    m_pEdtLog = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTrcMsgLog::setClient( CClient* i_pClient )
//------------------------------------------------------------------------------
{
    if( m_pClient != i_pClient )
    {
        if( m_pClient != nullptr )
        {
            m_pClient->removeTrcMsgLogObject(this);
        }

        m_pClient = i_pClient;

        if( m_pClient != nullptr )
        {
            m_pClient->addTrcMsgLogObject(this);
        }
    } // if( m_pClient != i_pClient )

} // setClient

//------------------------------------------------------------------------------
void CWdgtTrcMsgLog::setServer( CServer* i_pServer )
//------------------------------------------------------------------------------
{
    if( m_pServer != i_pServer )
    {
        if( m_pServer != nullptr )
        {
            m_pServer->removeTrcMsgLogObject(this);
        }

        m_pServer = i_pServer;

        if( m_pServer != nullptr )
        {
            m_pServer->addTrcMsgLogObject(this);
        }
    } // if( m_pServer != i_pServer )

} // setServer

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTrcMsgLog::setSettings( const STrcMsgLogSettings& i_settings )
//------------------------------------------------------------------------------
{
    m_settings = i_settings;

    if( m_pChkEnabled != nullptr )
    {
        if( m_settings.m_bEnabled )
        {
            m_pChkEnabled->setCheckState(Qt::Checked);
        }
        else
        {
            m_pChkEnabled->setCheckState(Qt::Unchecked);
        }
    }

    if( m_pEdtMaxItemCount != nullptr )
    {
        m_pEdtMaxItemCount->setText( QString::number(m_settings.m_uMaxItems) );
    }

} // setSettings

/*==============================================================================
public: // instance methods which may be called from any but not just the GUIMainThread
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTrcMsgLog::addItem( ETransmitDir i_transmitDir, bool i_bBold, const QString& i_strMsg )
//------------------------------------------------------------------------------
{
    if( m_settings.m_bEnabled )
    {
        CMsgReqAddLogItem* pMsg = new CMsgReqAddLogItem(
            /* pObjSender   */ this,
            /* pObjReceiver */ this,
            /* transmitDir  */ i_transmitDir,
            /* bBold        */ i_bBold,
            /* strMsg       */ i_strMsg );
        POST_OR_DELETE_MESSAGE(pMsg);
        pMsg = nullptr;
    }

} // addItem

/*==============================================================================
public: // overridables of inherited class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtTrcMsgLog::event( QEvent* i_pMsg )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    if( i_pMsg->type() >= QEvent::User )
    {
        bEventHandled = true;

        QString strAddTrcInfo;
        QString strAddErrInfo;

        CMsg* pMsg = nullptr;

        try
        {
            pMsg = dynamic_cast<CMsg*>(i_pMsg);
        }
        catch(...)
        {
            pMsg = nullptr;
        }

        if( pMsg != nullptr )
        {
            if( pMsg->isBaseMsgType() && pMsg->getMsgType() == EBaseMsgTypeReqAddLogItem )
            {
                CMsgReqAddLogItem* pMsg = dynamic_cast<CMsgReqAddLogItem*>(i_pMsg);

                if( m_pEdtLog != nullptr && m_settings.m_bEnabled )
                {
                    if( m_uLogItemCount >= m_settings.m_uMaxItems )
                    {
                        if( !m_bLogIsFull )
                        {
                            m_bLogIsFull = true;
                            m_pEdtLog->append("---------- MAXIMUM NUMBER OF ENTRIES REACHED -----------");
                        }
                    }
                    else // if( m_uLogItemCount < m_settings.m_uMaxItems )
                    {
                        QString strItem;
                        QString strTime = pMsg->getDateTimeUtc().toLocalTime().toString("hh:mm:ss:zzz");
                        QString strSysTimeFill("            ");
                        QString strSysTime = QString::number(pMsg->getSystemTimeInSec(),'f',6);

                        if( strSysTime.length() < 12 )
                        {
                            strSysTime.insert(0,strSysTimeFill.data(),12-strSysTime.length());
                            strSysTime.replace(" ","&nbsp;");
                        }
                        strItem = strTime + " (" + strSysTime + "): " + pMsg->getMsg();
                        strItem = encodeForHtml(strItem);

                        m_uLogItemCount++;

                        if( pMsg->isBold() )
                        {
                            m_pEdtLog->append("<FONT color=black><b>"+strItem+"</b></FONT>");
                        }
                        else
                        {
                            m_pEdtLog->append("<FONT color=black>"+strItem+"</FONT>");
                        }
                    } // if( m_uLogItemCount < m_settings.m_uMaxItems )
                } // if( m_pEdtLog != nullptr )
            } // if( pMsg->type() == ESystemMsgTypeLog )
        } // if( pMsg != nullptr )
    } // if( i_pMsg->type() >= QEvent::User )

    if( !bEventHandled )
    {
        bEventHandled = QWidget::event(i_pMsg);
    }
    return bEventHandled;

} // event

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTrcMsgLog::onChkEnabledToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    m_settings.m_bEnabled = i_bChecked;
}

//------------------------------------------------------------------------------
void CWdgtTrcMsgLog::onEdtMaxItemCountEditingFinished()
//------------------------------------------------------------------------------
{
    m_settings.m_uMaxItems = m_pEdtMaxItemCount->text().toUInt();
}

//------------------------------------------------------------------------------
void CWdgtTrcMsgLog::onBtnClearClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( m_pEdtLog != nullptr )
    {
        m_pEdtLog->clear();
        m_uLogItemCount = 0;
        m_bLogIsFull = false;
    }

} // onBtnClearClicked
