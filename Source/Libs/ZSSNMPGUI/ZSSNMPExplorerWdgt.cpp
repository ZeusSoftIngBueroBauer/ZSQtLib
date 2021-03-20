/*******************************************************************************

Copyright 2004 - 2009 by    ZeusSoft, Ing. Buero Bauer
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

--------------------------------------------------------------------------------

$Date: 22.01.10 22:11 $

$Revision: 9 $

$Author: Christian Bauer $

$History: ZSSNMPExplorerWdgt.cpp $
 * 
 * *****************  Version 9  *****************
 * User: Christian Bauer Date: 22.01.10   Time: 22:11
 * Updated in $/ZSSNMP/Source
 * 
 * *****************  Version 8  *****************
 * User: Christian Bauer Date: 10.01.10   Time: 19:19
 * Updated in $/ZSSNMP/Source

*******************************************************************************/

#include <QtCore/qabstractitemmodel.h>
#include <QtCore/qsettings.h>
#include <QtGui/qapplication.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qgroupbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpainter.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qtextedit.h>
#include <QtGui/qvalidator.h>
#include "ZSSNMP/Include/ZSSNMPExplorerWdgt.h"
#include "ZSSNMP/Include/ZSSNMPExplorer.h"
#include "ZSSNMP/Include/ZSSNMPOidNode.h"
#include "ZSSNMP/Include/ZSSNMPOidNodesModel.h"
#include "ZSSNMP/Include/ZSSNMPOidNodesWdgt.h"
#include "ZSSNMP/Include/ZSSNMPInstanceNode.h"
#include "ZSSNMP/Include/ZSSNMPInstanceNodesModel.h"
#include "ZSSNMP/Include/ZSSNMPInstanceNodesWdgt.h"
#include "ZSSNMP/Include/ZSSNMPMsg.h"
#include "ZSTrace/Include/ZSTrcAdminObj.h"
#include "ZSTrace/Include/ZSTrcMethod.h"
#include "ZSTrace/Include/ZSTrcServer.h"
#include "ZSSys/Include/ZSSysErrResult.h"
#include "ZSSys/Include/ZSSysException.h"
#include "ZSSys/Include/ZSSysProgressBar.h"
#include "ZSSys/Include/ZSSysTime.h"
#include "ZSSys/Include/ZSSysMemLeakDump.h"

using namespace ZS::SNMP;
using namespace ZS::Trace;


/*******************************************************************************
class CWdgtExplorerSession : public QWidget
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
const ZS::SEnumEntry s_arEnumStrStates[CWdgtExplorerSession::EStateCount] =
//------------------------------------------------------------------------------
{
    ZS::SEnumEntry( CWdgtExplorerSession::EStateNotEditing, "Not Editing" ),
    ZS::SEnumEntry( CWdgtExplorerSession::EStateEditing,    "Editing"     )
};

//------------------------------------------------------------------------------
QString CWdgtExplorerSession::state2Str( int i_iState )
//------------------------------------------------------------------------------
{
    return ZS::SEnumEntry::val2Str(s_arEnumStrStates,EStateCount,i_iState);
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtExplorerSession::CWdgtExplorerSession(
    CExplorer*     i_pExplorer,
    QSettings*     i_pSettingsFile,
    const QString& i_strSettingsKey,
    QWidget*       i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pExplorer(i_pExplorer),
    m_pSettingsFile(i_pSettingsFile),
    m_strSettingsKey(i_strSettingsKey),
    m_hostSettings(),
    m_state(EStateNotEditing),
    m_requestResult(),
    m_pLyt(NULL),
    m_pLblHostName(NULL),
    m_pEdtHostName(NULL),
    m_pLblHostPort(NULL),
    m_pEdtHostPort(NULL),
    m_pLblHostCommunityStr(NULL),
    m_pEdtHostCommunityStr(NULL),
    m_pBtnHostEdit(NULL),
    m_bEditingHost(false),
    m_pLblSessionState(NULL),
    m_pEdtSessionState(NULL),
    m_pLblRequestInProgress(NULL),
    m_pEdtRequestInProgress(NULL),
    m_pLblProgressBar(NULL),
    m_pProgressBar(NULL),
    m_pBtnSessionStart(NULL),
    m_pTrcAdminObj(NULL)
{
    setObjectName( "WdgtExplorerSession" + m_pExplorer->objectName() );

    if( m_strSettingsKey.isEmpty() )
    {
        m_strSettingsKey = objectName();
    }

    m_pTrcAdminObj = CTrcServer::getTrcAdminObj(
        /* traceType         */ ETraceTypeMth,
        /* strClassNameSpace */ "ZS::SNMP",
        /* strClassName      */ "CWdgtExplorerSession",
        /* strObjNameSpace   */ "",
        /* strObjName        */ m_pExplorer->getName() );

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "ctor",
        /* strAddTrcInfo */ "" );

    // <GroupBox> Session
    //===================

    int iRow;
    int iClm;
    int iClmCount;

    int cxLblWidthClm1 =  58;
    int cxLblWidthClm2 = 106;
    int cxLblWidthClm3 =  88;

    m_hostSettings = m_pExplorer->getHostSettings();

    m_pLyt = new QGridLayout();
    setLayout(m_pLyt);

    // Line 1
    //--------

    iRow = 0;
    iClm = 0;

    // <LineEdit> Host Name
    //---------------------

    m_pLblHostName = new QLabel("Host Name:");
    m_pLblHostName->setFixedWidth(cxLblWidthClm1);
    m_pLyt->addWidget(m_pLblHostName,iRow,iClm++);
    m_pEdtHostName = new QLineEdit(m_hostSettings.m_strName);
    m_pEdtHostName->setEnabled(false);
    m_pLyt->addWidget(m_pEdtHostName,iRow,iClm++);
    m_pLyt->setColumnMinimumWidth(iClm++,10);

    // <LineEdit> Host Port
    //----------------------

    m_pLblHostPort = new QLabel("Host Port:");
    m_pLblHostPort->setFixedWidth(cxLblWidthClm2);
    m_pLyt->addWidget(m_pLblHostPort,iRow,iClm++);
    m_pEdtHostPort = new QLineEdit(QString::number(m_hostSettings.m_uPort));
    m_pEdtHostPort->setEnabled(false);
    m_pEdtHostPort->setValidator( new QIntValidator(1,65535,this) );
    m_pLyt->addWidget(m_pEdtHostPort,iRow,iClm++);
    m_pLyt->setColumnMinimumWidth(iClm++,10);

    // <LineEdit> Community String
    //-----------------------------

    m_pLblHostCommunityStr = new QLabel("Community String:");
    m_pLblHostCommunityStr->setFixedWidth(cxLblWidthClm3);
    m_pLyt->addWidget(m_pLblHostCommunityStr,iRow,iClm++);
    m_pEdtHostCommunityStr = new QLineEdit(m_hostSettings.m_strCommunity);
    m_pEdtHostCommunityStr->setEnabled(false);
    m_pLyt->addWidget(m_pEdtHostCommunityStr,iRow,iClm++);
    m_pLyt->setColumnMinimumWidth(iClm++,10);

    // <Button> Edit Settings
    //------------------------

    m_pBtnHostEdit = new QPushButton("Edit Session");
    m_pBtnHostEdit->setFixedWidth(120);
    m_pLyt->addWidget(m_pBtnHostEdit,iRow,iClm++);

    if( !connect(
        /* pObjSender   */ m_pBtnHostEdit,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnHostEditClicked(bool)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }

    // Line 2
    //--------

    iClmCount = iClm;
    iRow = 1;
    iClm = 0;

    // <Label> Session State
    //----------------------

    m_pLblSessionState = new QLabel("State:");
    m_pLblSessionState->setFixedWidth(cxLblWidthClm1);
    m_pLyt->addWidget(m_pLblSessionState,iRow,iClm++);
    m_pEdtSessionState = new QLineEdit("Closed");
    m_pEdtSessionState->setEnabled(false);
    m_pLyt->addWidget(m_pEdtSessionState,iRow,iClm++);
    m_pLyt->setColumnMinimumWidth(iClm++,10);

    // <Label> Request In Progress
    //----------------------------

    m_pLblRequestInProgress = new QLabel("Request In Progress:");
    m_pLblRequestInProgress->setFixedWidth(cxLblWidthClm2);
    m_pLyt->addWidget(m_pLblRequestInProgress,iRow,iClm++);
    m_pEdtRequestInProgress = new QLineEdit("None");
    m_pEdtRequestInProgress->setEnabled(false);
    m_pLyt->addWidget(m_pEdtRequestInProgress,iRow,iClm++);
    m_pLyt->setColumnMinimumWidth(iClm++,10);

    // <ProgressBar>
    //--------------

    m_pLblProgressBar = new QLabel("Progress:");
    m_pLblProgressBar->setFixedWidth(cxLblWidthClm3);
    m_pLyt->addWidget(m_pLblProgressBar,iRow,iClm++);
    m_pProgressBar = new CProgressBar(this);
    m_pLyt->addWidget(m_pProgressBar,iRow,iClm++);
    m_pLyt->setColumnMinimumWidth(iClm++,10);

    // <Button> Start/Stop Session
    //----------------------------

    m_pBtnSessionStart = new QPushButton("Start Session");
    m_pBtnSessionStart->setFixedWidth(120);
    m_pLyt->addWidget(m_pBtnSessionStart,iRow,iClm++);

    if( !connect(
        /* pObjSender   */ m_pBtnSessionStart,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnSessionStartClicked(bool)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }

    // Keeping indicated session parameters up to date ..
    //===================================================

    if( !connect(
        /* pObjSender   */ m_pExplorer,
        /* szSignal     */ SIGNAL(hostSettingsChanged(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onExplorerHostSettingsChanged(QObject*)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }
    if( !connect(
        /* pObjSender   */ m_pExplorer,
        /* szSignal     */ SIGNAL(stateChanged(QObject*,int,int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onExplorerStateChanged(QObject*,int,int)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }
    if( !connect(
        /* pObjSender   */ m_pExplorer,
        /* szSignal     */ SIGNAL(requestExecuted(QObject*,int,const ZS::SErrResultInfo&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onExplorerRequestExecuted(QObject*,int,const ZS::SErrResultInfo&)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }

} // ctor

//------------------------------------------------------------------------------
CWdgtExplorerSession::~CWdgtExplorerSession()
//------------------------------------------------------------------------------
{
    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "dtor",
        /* strAddTrcInfo */ "" );

    m_pExplorer = NULL;
    m_pLyt = NULL;
    m_pLblHostName = NULL;
    m_pEdtHostName = NULL;
    m_pLblHostPort = NULL;
    m_pEdtHostPort = NULL;
    m_pLblHostCommunityStr = NULL;
    m_pEdtHostCommunityStr = NULL;
    m_pBtnHostEdit = NULL;
    m_pLblSessionState = NULL;
    m_pEdtSessionState = NULL;
    m_pLblRequestInProgress = NULL;
    m_pEdtRequestInProgress = NULL;
    m_pLblProgressBar = NULL;
    m_pProgressBar = NULL;
    m_pBtnSessionStart = NULL;

    CTrcServer::releaseTrcAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = NULL;

} // dtor

/*==============================================================================
protected slots: // connected to the signals of my user controls
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtExplorerSession::onBtnHostEditClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo = "State:" + state2Str(m_state);
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "onBtnHostEditClicked",
        /* strAddTrcInfo */ strAddTrcInfo );

    if( m_state == EStateNotEditing )
    {
        m_pEdtHostName->setEnabled(true);
        m_pEdtHostCommunityStr->setEnabled(true);

        m_pBtnSessionStart->setEnabled(false);

        m_state = EStateEditing;
        m_pBtnHostEdit->setText("Accept ...");

        emit stateChanged(this,m_state);
    }

    else // if( m_state == EStateEditing )
    {
        SRequestResult requestResult;
        bool           bSettingsChanged = false;

        if( m_hostSettings.m_strName != m_pEdtHostName->text() )
        {
            m_hostSettings.m_strName = m_pEdtHostName->text();
            bSettingsChanged = true;
        }
        if( m_hostSettings.m_strCommunity != m_pEdtHostCommunityStr->text() )
        {
            m_hostSettings.m_strCommunity = m_pEdtHostCommunityStr->text();
            bSettingsChanged = true;
        }

        if( bSettingsChanged )
        {
            disconnect(
                /* pObjSender   */ m_pExplorer,
                /* szSignal     */ SIGNAL(hostSettingsChanged(QObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onExplorerHostSettingsChanged(QObject*)) );

            requestResult = m_pExplorer->setHostSettings(m_hostSettings);

            connect(
                /* pObjSender   */ m_pExplorer,
                /* szSignal     */ SIGNAL(hostSettingsChanged(QObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onExplorerHostSettingsChanged(QObject*)) );
        }

        if( requestResult.isAsynchronousRequest() )
        {
            m_requestResult = requestResult;
        }

        m_pEdtHostName->setEnabled(false);
        m_pEdtHostCommunityStr->setEnabled(false);

        m_pBtnSessionStart->setEnabled(true);

        m_state = EStateNotEditing;
        m_pBtnHostEdit->setText("Edit Session");

        emit stateChanged(this,m_state);

    } // if( m_state == EStateEditing )

} // onBtnHostEditClicked

//------------------------------------------------------------------------------
void CWdgtExplorerSession::onBtnSessionStartClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo = "State:" + state2Str(m_state);
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "onBtnSessionStartClicked",
        /* strAddTrcInfo */ strAddTrcInfo );

    if( m_state == EStateNotEditing )
    {
        SRequestResult requestResult;
        int            iDuration_ms = 5000;

        if( m_pBtnSessionStart->text().compare("Start Session") == 0 )
        {
            requestResult = m_pExplorer->openSession();

            if( m_pExplorer->getRequestTimeoutInMs(CExplorer::ERequestOpenSession) > 0 )
            {
                iDuration_ms = m_pExplorer->getRequestTimeoutInMs(CExplorer::ERequestOpenSession);
            }
            else if( m_pExplorer->getLastRecentRequestDurationInMs(CExplorer::ERequestOpenSession) > 5000 )
            {
                iDuration_ms = m_pExplorer->getLastRecentRequestDurationInMs(CExplorer::ERequestOpenSession);
            }
            m_pBtnSessionStart->setText("Stop Session");
        }

        else // if( m_pBtnSessionStart->text().compare("Stop Session") == 0 )
        {
            requestResult = m_pExplorer->closeSession();

            if( m_pExplorer->getRequestTimeoutInMs(CExplorer::ERequestCloseSession) > 0 )
            {
                iDuration_ms = m_pExplorer->getRequestTimeoutInMs(CExplorer::ERequestCloseSession);
            }
            else if( m_pExplorer->getLastRecentRequestDurationInMs(CExplorer::ERequestCloseSession) > 5000 )
            {
                iDuration_ms = m_pExplorer->getLastRecentRequestDurationInMs(CExplorer::ERequestCloseSession);
            }
            m_pBtnSessionStart->setText("Start Session");
        }

        if( requestResult.isAsynchronousRequest() )
        {
            m_requestResult = requestResult;

            m_pBtnSessionStart->setEnabled(false);

            if( iDuration_ms > 0 )
            {
                m_pProgressBar->setDurationInMs(iDuration_ms);
                m_pProgressBar->start();
            }
        }

    } // if( m_state == EStateNotEditing )

} // onBtnSessionStartClicked

/*==============================================================================
protected slots: // connected to the signals of the explorer
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtExplorerSession::onExplorerHostSettingsChanged( QObject* /*i_pExplorer*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", HostName:" + m_hostSettings.m_strName;
        strAddTrcInfo += ", HostPort:" + QString::number(m_hostSettings.m_uPort);
        strAddTrcInfo += ", Community:" + m_hostSettings.m_strCommunity;
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "onExplorerHostSettingsChanged",
        /* strAddTrcInfo */ strAddTrcInfo );

    SHostSettings hostSettingsCurr = m_pExplorer->getHostSettings();

    if( hostSettingsCurr != m_hostSettings )
    {
        m_hostSettings = hostSettingsCurr;

        m_pEdtHostName->setText(m_hostSettings.m_strName);
        m_pEdtHostPort->setText(QString::number(m_hostSettings.m_uPort));
        m_pEdtHostCommunityStr->setText(m_hostSettings.m_strCommunity);
    }

} // onExplorerHostSettingsChanged

//------------------------------------------------------------------------------
void CWdgtExplorerSession::onExplorerStateChanged( QObject*, int i_iState, int i_iRequestInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(i_iState);
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(i_iRequestInProgress);
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "onExplorerStateChanged",
        /* strAddTrcInfo */ strAddTrcInfo );

    if( i_iState == CExplorer::EStateSessionOpened )
    {
        m_pEdtSessionState->setStyleSheet("QLineEdit { color: green; }");
        m_pBtnSessionStart->setText("Stop Session");

        if( m_state == EStateNotEditing )
        {
            m_pBtnSessionStart->setEnabled(true);
        }
    }
    else if( i_iState == CExplorer::EStateSessionClosed )
    {
        m_pEdtSessionState->setStyleSheet("QLineEdit { color: red; }");
        m_pBtnSessionStart->setText("Start Session");

        if( m_state == EStateNotEditing )
        {
            m_pBtnSessionStart->setEnabled(true);
        }
    }
    else
    {
        m_pEdtSessionState->setStyleSheet("QLineEdit { color: gray; }");
        m_pBtnSessionStart->setEnabled(false);
    }

    m_pEdtSessionState->setText( CExplorer::state2Str(i_iState) );
    m_pEdtRequestInProgress->setText( CExplorer::request2Str(i_iRequestInProgress) );

} // onExplorerStateChanged

//------------------------------------------------------------------------------
void CWdgtExplorerSession::onExplorerRequestExecuted( QObject*, int i_iRequestId, const ZS::SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
        strAddTrcInfo += ", RequestIdWaitingFor:" + QString::number(m_requestResult.m_iPendingRequestId);
        strAddTrcInfo += ", RequestIdReceived:" + QString::number(i_iRequestId);
        strAddTrcInfo += ", Result:" + i_errResultInfo.getResultStr();
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "onExplorerRequestExecuted",
        /* strAddTrcInfo */ strAddTrcInfo );

    if( i_iRequestId == m_requestResult.m_iPendingRequestId )
    {
        // If we received the answer to our open/close session request (upon pressing the start/stop button) ..
        if( m_state == EStateNotEditing )
        {
            m_pBtnSessionStart->setEnabled(true);
        }
        // If we received the answer to our changeHostSettings request (upon pressing the edit/accept button) ..
        else // if( m_state == EStateEditing )
        {
            m_pBtnSessionStart->setEnabled(false);

            m_state = EStateNotEditing;

            emit stateChanged(this,m_state);
        }

        m_pProgressBar->stop();
        m_pProgressBar->reset();
    }

} // onExplorerRequestExecuted



/*******************************************************************************
class CWdgtExplorer : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtExplorer::CWdgtExplorer(
    CExplorer*     i_pExplorer,
    QSettings*     i_pSettingsFile,
    const QString& i_strSettingsKey,
    QWidget*       i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pExplorer(i_pExplorer),
    m_pSettingsFile(i_pSettingsFile),
    m_strSettingsKey(i_strSettingsKey),
    m_requestResult(),
    m_pLytMain(NULL),
    m_pGrpSession(NULL),
    m_pLytGrpSession(NULL),
    m_pWdgtSession(NULL),
    m_pGrpOid(NULL),
    m_pLytGrpOid(NULL),
    m_pLytOidInput(NULL),
    m_pLblOidInput(NULL),
    m_pEdtOidInput(NULL),
    m_pLytOidNumericLine(NULL),
    m_pLytOidNumeric(NULL),
    m_pLblOidNumeric(NULL),
    m_pEdtOidNumeric(NULL),
    m_pBtnOidNumericCopy2Input(NULL),
    m_pLytOidTextualLine(NULL),
    m_pLytOidTextual(NULL),
    m_pLblOidTextual(NULL),
    m_pEdtOidTextual(NULL),
    m_pBtnOidTextualCopy2Input(NULL),
    m_pLytOidModuleLine(NULL),
    m_pLytOidModule(NULL),
    m_pLblOidModule(NULL),
    m_pEdtOidModule(NULL),
    m_pBtnOidModuleCopy2Input(NULL),
    m_pGrpCommands(NULL),
    m_pLytGrpCommands(NULL),
    m_pBtnGet(NULL),
    m_pBtnGetNext(NULL),
    m_pBtnGetBulk(NULL),
    m_pBtnWalk(NULL),
    m_pBtnAbort(NULL),
    m_pSplitterOidNodesAndLog(NULL),
    m_pSplitterOidNodes(NULL),
    m_pWdgtOidNodes(NULL),
    m_pWdgtInstanceNodes(NULL),
    m_pGrpLog(NULL),
    m_pLytGrpLog(NULL),
    m_pWdgtLogHeadline(NULL),
    m_pLytLogHeadline(NULL),
    m_pLblLogEnable(NULL),
    m_pChkLogEnable(NULL),
    m_pLblLogEnableHint(NULL),
    m_pEdtLog(NULL),
    m_pTrcAdminObj(NULL)
{
    setObjectName( "WdgtExplorer" + m_pExplorer->objectName() );

    if( m_strSettingsKey.isEmpty() )
    {
        m_strSettingsKey = objectName();
    }

    m_pTrcAdminObj = CTrcServer::getTrcAdminObj(
        /* traceType         */ ETraceTypeMth,
        /* strClassNameSpace */ "ZS::SNMP",
        /* strClassName      */ "CWdgtExplorer",
        /* strObjNameSpace   */ "",
        /* strObjName        */ m_pExplorer->getName() );

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "ctor",
        /* strAddTrcInfo */ "" );

    m_pLytMain = new QVBoxLayout();
    setLayout(m_pLytMain);

    // <GroupBox> Session
    //===================

    m_pGrpSession = new QGroupBox("Session");
    m_pLytGrpSession = new QVBoxLayout();
    m_pGrpSession->setLayout(m_pLytGrpSession);
    m_pLytMain->addWidget(m_pGrpSession,0);

    m_pWdgtSession = new CWdgtExplorerSession(
        /* pExplorer      */ m_pExplorer,
        /* pSettingsFile  */ m_pSettingsFile,
        /* strSettingsKey */ "Session",
        /* pWdgtParent    */ NULL );
    m_pLytGrpSession->addWidget(m_pWdgtSession);

    if( !connect(
        /* pObjSender   */ m_pWdgtSession,
        /* szSignal     */ SIGNAL(stateChanged(QObject*,int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onWdgtSessionStateChanged(QObject*,int)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }

    // <GroupBox> Oid
    //================

    int cxLblWidth = 80;

    m_pGrpOid = new QGroupBox("Oid");
    m_pLytGrpOid = new QVBoxLayout();
    m_pGrpOid->setLayout(m_pLytGrpOid);
    m_pLytMain->addWidget(m_pGrpOid,0);

    // <LineEdit> Oid Input
    //----------------------

    m_pLytOidInput = new QHBoxLayout();
    m_pLytGrpOid->addLayout(m_pLytOidInput);
    m_pLblOidInput = new QLabel("Input:");
    m_pLblOidInput->setFixedWidth(cxLblWidth);
    m_pLytOidInput->addWidget(m_pLblOidInput);
    m_pEdtOidInput = new QLineEdit("");
    m_pEdtOidInput->setMinimumWidth(120);
    m_pLytOidInput->addWidget(m_pEdtOidInput);

    // <Line> Oid Numeric
    //-------------------

    m_pLytOidNumericLine = new QHBoxLayout();
    m_pLytGrpOid->addLayout(m_pLytOidNumericLine);

    // <Label> Oid Numeric
    //----------------------

    m_pLytOidNumeric = new QHBoxLayout();
    m_pLytOidNumericLine->addLayout(m_pLytOidNumeric);
    m_pLblOidNumeric = new QLabel("Numeric:");
    m_pLblOidNumeric->setFixedWidth(cxLblWidth);
    m_pLytOidNumeric->addWidget(m_pLblOidNumeric);
    m_pEdtOidNumeric = new QLineEdit("");
    m_pEdtOidNumeric->setMinimumWidth(120);
    m_pEdtOidNumeric->setEnabled(false);
    m_pLytOidNumeric->addWidget(m_pEdtOidNumeric);

    // <Button> Copy Oid Numeric to Input
    //-----------------------------------

    m_pBtnOidNumericCopy2Input = new QPushButton("Copy To Input");
    m_pBtnOidNumericCopy2Input->setMinimumWidth(80);
    m_pLytOidNumericLine->addWidget(m_pBtnOidNumericCopy2Input);

    if( !connect(
        /* pObjSender   */ m_pBtnOidNumericCopy2Input,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnOidNumericCopy2InputClicked(bool)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }

    // <Line> Oid Textual
    //-------------------

    m_pLytOidTextualLine = new QHBoxLayout();
    m_pLytGrpOid->addLayout(m_pLytOidTextualLine);

    // <Label> Oid Textual
    //----------------------

    m_pLytOidTextual = new QHBoxLayout();
    m_pLytOidTextualLine->addLayout(m_pLytOidTextual);
    m_pLblOidTextual = new QLabel("Textual:");
    m_pLblOidTextual->setFixedWidth(cxLblWidth);
    m_pLytOidTextual->addWidget(m_pLblOidTextual);
    m_pEdtOidTextual = new QLineEdit("");
    m_pEdtOidTextual->setMinimumWidth(120);
    m_pEdtOidTextual->setEnabled(false);
    m_pLytOidTextual->addWidget(m_pEdtOidTextual);

    // <Button> Copy Oid Textual to Input
    //-----------------------------------

    m_pBtnOidTextualCopy2Input = new QPushButton("Copy To Input");
    m_pBtnOidTextualCopy2Input->setMinimumWidth(80);
    m_pLytOidTextualLine->addWidget(m_pBtnOidTextualCopy2Input);

    if( !connect(
        /* pObjSender   */ m_pBtnOidTextualCopy2Input,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnOidTextualCopy2InputClicked(bool)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }

    // <Line> Oid Module
    //-------------------

    m_pLytOidModuleLine = new QHBoxLayout();
    m_pLytGrpOid->addLayout(m_pLytOidModuleLine);

    // <Label> Oid Module
    //----------------------

    m_pLytOidModule = new QHBoxLayout();
    m_pLytOidModuleLine->addLayout(m_pLytOidModule);
    m_pLblOidModule = new QLabel("Module:");
    m_pLblOidModule->setFixedWidth(cxLblWidth);
    m_pLytOidModule->addWidget(m_pLblOidModule);
    m_pEdtOidModule = new QLineEdit("");
    m_pEdtOidModule->setMinimumWidth(120);
    m_pEdtOidModule->setEnabled(false);
    m_pLytOidModule->addWidget(m_pEdtOidModule);

    // <Button> Copy Oid Module to Input
    //-----------------------------------

    m_pBtnOidModuleCopy2Input = new QPushButton("Copy To Input");
    m_pBtnOidModuleCopy2Input->setMinimumWidth(80);
    m_pLytOidModuleLine->addWidget(m_pBtnOidModuleCopy2Input);

    if( !connect(
        /* pObjSender   */ m_pBtnOidModuleCopy2Input,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnOidModuleCopy2InputClicked(bool)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }

    // <GroupBox> Command Buttons
    //===========================

    m_pGrpCommands = new QGroupBox("Commands");
    m_pLytGrpCommands = new QHBoxLayout();
    m_pGrpCommands->setLayout(m_pLytGrpCommands);
    m_pLytMain->addWidget(m_pGrpCommands,0);

    // <Button> GET
    //-------------

    m_pBtnGet = new QPushButton("GET");
    m_pBtnGet->setMinimumWidth(80);
    m_pLytGrpCommands->addWidget(m_pBtnGet);
    m_pLytGrpCommands->addSpacing(10);

    if( !connect(
        /* pObjSender   */ m_pBtnGet,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnCommandGetClicked(bool)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }

    // <Button> GETNEXT
    //-----------------

    m_pBtnGetNext = new QPushButton("GETNEXT");
    m_pBtnGetNext->setMinimumWidth(80);
    m_pLytGrpCommands->addWidget(m_pBtnGetNext);
    m_pLytGrpCommands->addSpacing(10);

    if( !connect(
        /* pObjSender   */ m_pBtnGetNext,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnCommandGetNextClicked(bool)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }

    // <Button> GETBULK
    //-----------------

    m_pBtnGetBulk = new QPushButton("GETBULK");
    m_pBtnGetBulk->setMinimumWidth(80);
    m_pLytGrpCommands->addWidget(m_pBtnGetBulk);
    m_pLytGrpCommands->addSpacing(10);

    if( !connect(
        /* pObjSender   */ m_pBtnGetBulk,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnCommandGetBulkClicked(bool)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }

    // <Button> WALK
    //-----------------

    m_pBtnWalk = new QPushButton("WALK");
    m_pBtnWalk->setMinimumWidth(80);
    m_pLytGrpCommands->addWidget(m_pBtnWalk);
    m_pLytGrpCommands->addSpacing(10);

    if( !connect(
        /* pObjSender   */ m_pBtnWalk,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnCommandWalkClicked(bool)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }

    // <Button> Abort
    //-----------------

    m_pBtnAbort = new QPushButton("Abort");
    m_pBtnAbort->setEnabled(false);
    m_pBtnAbort->setMinimumWidth(80);
    m_pLytGrpCommands->addWidget(m_pBtnAbort);
    m_pLytGrpCommands->addSpacing(10);

    if( !connect(
        /* pObjSender   */ m_pBtnAbort,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnCommandAbortClicked(bool)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }

    // <Splitter> (vertical) Key Nodes and Log Window
    //===============================================

    m_pSplitterOidNodesAndLog = new QSplitter(Qt::Vertical);
    m_pSplitterOidNodesAndLog->setChildrenCollapsible(false);
    m_pLytMain->addWidget(m_pSplitterOidNodesAndLog,1);

    // <Splitter> (horizontal) Key Nodes Explorer
    //===========================================

    m_pSplitterOidNodes = new QSplitter(Qt::Horizontal);
    m_pSplitterOidNodes->setChildrenCollapsible(false);
    m_pSplitterOidNodesAndLog->addWidget(m_pSplitterOidNodes);

    // <TreeView> Key Nodes
    //---------------------

    m_pWdgtOidNodes = new CWdgtOidNodes(m_pExplorer->getModelOidNodes(),this);
    m_pSplitterOidNodes->addWidget(m_pWdgtOidNodes);

    if( !QObject::connect(
        /* pObjSender   */ m_pWdgtOidNodes,
        /* szSignal     */ SIGNAL( nodeSelected(COidNode*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onNodeSelected(COidNode*) ) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <TableView> Instances
    //----------------------

    m_pWdgtInstanceNodes = new CWdgtInstanceNodes(m_pExplorer->getModelInstanceNodes(),this);
    m_pSplitterOidNodes->addWidget(m_pWdgtInstanceNodes);

    if( !QObject::connect(
        /* pObjSender   */ m_pWdgtInstanceNodes,
        /* szSignal     */ SIGNAL( nodeSelected(CInstanceNode*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onNodeSelected(CInstanceNode*) ) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <ListWidget> Logging data transfer
    //===========================================

    m_pGrpLog = new QGroupBox("Data Transfer Logging");
    m_pLytGrpLog = new QVBoxLayout();
    m_pGrpLog->setLayout(m_pLytGrpLog);
    m_pSplitterOidNodesAndLog->addWidget(m_pGrpLog);

    m_pWdgtLogHeadline = new QWidget();
    m_pLytLogHeadline = new QHBoxLayout();
    m_pWdgtLogHeadline->setLayout(m_pLytLogHeadline);
    m_pLytGrpLog->addWidget(m_pWdgtLogHeadline,0,Qt::AlignTop);

    m_pLblLogEnable = new QLabel("Enable Logging:");
    m_pLytLogHeadline->addWidget(m_pLblLogEnable);
    m_pChkLogEnable = new QCheckBox();
    m_pChkLogEnable->setChecked(false);
    m_pLytLogHeadline->addWidget(m_pChkLogEnable);
    m_pLytLogHeadline->addSpacing(10);

    m_pLblLogEnableHint = new QLabel("(enable logging notably slows down a WALK request)");
    m_pLytLogHeadline->addWidget(m_pLblLogEnableHint);
    m_pLytLogHeadline->addStretch();

    m_pEdtLog = new QTextEdit(this);
    m_pLytGrpLog->addWidget(m_pEdtLog,1);
    QFont fntListWdgt("Courier New",8);
    m_pEdtLog->setFont(fntListWdgt);

    if( !connect(
        /* pObjSender   */ m_pChkLogEnable,
        /* szSignal     */ SIGNAL(stateChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkLogEnableStateChanged(int)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }

    // Stretch at bottom of main layout
    //=================================

    m_pLytMain->addStretch();

    // Restore splitter position
    //==========================

    if( m_pSettingsFile != NULL )
    {
        QList<int> listSizes;
        int        idx;

        listSizes = m_pSplitterOidNodesAndLog->sizes();
        for( idx = 0; idx < listSizes.count(); idx++ )
        {
            listSizes[idx] = m_pSettingsFile->value( m_strSettingsKey + "/SplitterOidNodesAndLogHeight" + QString::number(idx), 50 ).toInt();
        }
        m_pSplitterOidNodesAndLog->setSizes(listSizes);

        listSizes = m_pSplitterOidNodes->sizes();
        for( idx = 0; idx < listSizes.count(); idx++ )
        {
            listSizes[idx] = m_pSettingsFile->value( m_strSettingsKey + "/SplitterOidNodesWidth" + QString::number(idx), 50 ).toInt();
        }
        m_pSplitterOidNodes->setSizes(listSizes);
    }

    // Keeping indicated session parameters up to date ..
    //===================================================

    if( !connect(
        /* pObjSender   */ m_pExplorer,
        /* szSignal     */ SIGNAL(stateChanged(QObject*,int,int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onExplorerStateChanged(QObject*,int,int)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }
    if( !connect(
        /* pObjSender   */ m_pExplorer,
        /* szSignal     */ SIGNAL(requestExecuted(QObject*,int,const ZS::SErrResultInfo&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onExplorerRequestExecuted(QObject*,int,const ZS::SErrResultInfo&)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }
    if( !connect(
        /* pObjSender   */ m_pExplorer,
        /* szSignal     */ SIGNAL(sendingMessage(QObject*,ZS::CMsg*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onExplorerSendingMessage(QObject*,ZS::CMsg*)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }
    if( !connect(
        /* pObjSender   */ m_pExplorer,
        /* szSignal     */ SIGNAL(receivedMessage(QObject*,ZS::CMsg*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onExplorerReceivedMessage(QObject*,ZS::CMsg*)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }

} // ctor

//------------------------------------------------------------------------------
CWdgtExplorer::~CWdgtExplorer()
//------------------------------------------------------------------------------
{
    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "dtor",
        /* strAddTrcInfo */ "" );

    if( m_pSettingsFile != NULL )
    {
        QList<int> listSizes;
        int        idx;

        listSizes = m_pSplitterOidNodesAndLog->sizes();
        for( idx = 0; idx < listSizes.count(); idx++ )
        {
            m_pSettingsFile->setValue( m_strSettingsKey + "/SplitterOidNodesAndLogHeight" + QString::number(idx), listSizes[idx] );
        }

        listSizes = m_pSplitterOidNodes->sizes();
        for( idx = 0; idx < listSizes.count(); idx++ )
        {
            m_pSettingsFile->setValue( m_strSettingsKey + "/SplitterOidNodesWidth" + QString::number(idx), listSizes[idx] );
        }
    }

    m_pExplorer = NULL;
    m_pLytMain = NULL;
    m_pGrpSession = NULL;
    m_pLytGrpSession = NULL;
    m_pWdgtSession = NULL;
    m_pGrpOid = NULL;
    m_pLytGrpOid = NULL;
    m_pLytOidInput = NULL;
    m_pLblOidInput = NULL;
    m_pEdtOidInput = NULL;
    m_pLytOidNumericLine = NULL;
    m_pLytOidNumeric = NULL;
    m_pLblOidNumeric = NULL;
    m_pEdtOidNumeric = NULL;
    m_pBtnOidNumericCopy2Input = NULL;
    m_pLytOidTextualLine = NULL;
    m_pLytOidTextual = NULL;
    m_pLblOidTextual = NULL;
    m_pEdtOidTextual = NULL;
    m_pBtnOidTextualCopy2Input = NULL;
    m_pLytOidModuleLine = NULL;
    m_pLytOidModule = NULL;
    m_pLblOidModule = NULL;
    m_pEdtOidModule = NULL;
    m_pBtnOidModuleCopy2Input = NULL;
    m_pGrpCommands = NULL;
    m_pLytGrpCommands = NULL;
    m_pBtnGet = NULL;
    m_pBtnGetNext = NULL;
    m_pBtnGetBulk = NULL;
    m_pBtnWalk = NULL;
    m_pBtnAbort = NULL;
    m_pSplitterOidNodesAndLog = NULL;
    m_pSplitterOidNodes = NULL;
    m_pWdgtOidNodes = NULL;
    m_pWdgtInstanceNodes = NULL;
    m_pGrpLog = NULL;
    m_pLytGrpLog = NULL;
    m_pWdgtLogHeadline = NULL;
    m_pLytLogHeadline = NULL;
    m_pLblLogEnable = NULL;
    m_pChkLogEnable = NULL;
    m_pLblLogEnableHint = NULL;
    m_pEdtLog = NULL;

    CTrcServer::releaseTrcAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = NULL;

} // dtor

/*==============================================================================
protected slots: // connected to the signals of my user controls
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtExplorer::onWdgtSessionStateChanged( QObject* /*i_pWdgt*/, int i_iState )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo = "WdgtSessionState:" + CWdgtExplorerSession::state2Str(i_iState);
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "onWdgtSessionStateChanged",
        /* strAddTrcInfo */ strAddTrcInfo );

    if( i_iState == CWdgtExplorerSession::EStateEditing )
    {
        m_pEdtOidInput->setEnabled(false);
        m_pBtnOidNumericCopy2Input->setEnabled(false);
        m_pBtnOidTextualCopy2Input->setEnabled(false);
        m_pBtnOidModuleCopy2Input->setEnabled(false);
        m_pBtnGet->setEnabled(false);
        m_pBtnGetNext->setEnabled(false);
        m_pBtnGetBulk->setEnabled(false);
        m_pBtnWalk->setEnabled(false);
    }
    else
    {
        m_pEdtOidInput->setEnabled(true);
        m_pBtnOidNumericCopy2Input->setEnabled(true);
        m_pBtnOidTextualCopy2Input->setEnabled(true);
        m_pBtnOidModuleCopy2Input->setEnabled(true);
        m_pBtnGet->setEnabled(true);
        m_pBtnGetNext->setEnabled(true);
        m_pBtnGetBulk->setEnabled(true);
        m_pBtnWalk->setEnabled(true);
    }

} // onWdgtSessionStateChanged

//------------------------------------------------------------------------------
void CWdgtExplorer::onBtnOidNumericCopy2InputClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    m_pEdtOidInput->setText( m_pEdtOidNumeric->text() );
}

//------------------------------------------------------------------------------
void CWdgtExplorer::onBtnOidTextualCopy2InputClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    m_pEdtOidInput->setText( m_pEdtOidTextual->text() );
}

//------------------------------------------------------------------------------
void CWdgtExplorer::onBtnOidModuleCopy2InputClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    m_pEdtOidInput->setText( m_pEdtOidModule->text() );
}

//------------------------------------------------------------------------------
void CWdgtExplorer::onBtnCommandGetClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "onBtnCommandGetClicked",
        /* strAddTrcInfo */ "" );

    m_pEdtLog->clear();

    m_requestResult = m_pExplorer->requestValue(m_pEdtOidInput->text());

    if( m_requestResult.isAsynchronousRequest() )
    {
        m_pBtnGet->setEnabled(false);
        m_pBtnGetNext->setEnabled(false);
        m_pBtnGetBulk->setEnabled(false);
        m_pBtnWalk->setEnabled(false);
        m_pBtnAbort->setEnabled(true);

        //if( m_pExplorer->getRequestTimeoutInMs(CExplorer::ERequestGetValue) > 0 )
        //{
        //    m_pProgressBar->setDurationInMs( m_pExplorer->getRequestTimeoutInMs(CExplorer::ERequestGetValue) );
        //    m_pProgressBar->start();
        //}
        //else if( m_pExplorer->getLastRecentRequestDurationInMs(CExplorer::ERequestGetValue) > 5000 )
        //{
        //    m_pProgressBar->setDurationInMs( m_pExplorer->getLastRecentRequestDurationInMs(CExplorer::ERequestGetValue) );
        //    m_pProgressBar->start();
        //}
        //else
        //{
        //    m_pProgressBar->setDurationInMs(5000);
        //    m_pProgressBar->start();
        //}
    }

} // onBtnCommandGetClicked

//------------------------------------------------------------------------------
void CWdgtExplorer::onBtnCommandGetNextClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "onBtnCommandGetNextClicked",
        /* strAddTrcInfo */ "" );

    m_pEdtLog->clear();

    m_requestResult = m_pExplorer->requestNextValue(m_pEdtOidInput->text());

    if( m_requestResult.isAsynchronousRequest() )
    {
        m_pBtnGet->setEnabled(false);
        m_pBtnGetNext->setEnabled(false);
        m_pBtnGetBulk->setEnabled(false);
        m_pBtnWalk->setEnabled(false);
        m_pBtnAbort->setEnabled(true);

        //if( m_pExplorer->getRequestTimeoutInMs(CExplorer::ERequestGetNextValue) > 0 )
        //{
        //    m_pProgressBar->setDurationInMs( m_pExplorer->getRequestTimeoutInMs(CExplorer::ERequestGetNextValue) );
        //    m_pProgressBar->start();
        //}
        //else if( m_pExplorer->getLastRecentRequestDurationInMs(CExplorer::ERequestGetNextValue) > 5000 )
        //{
        //    m_pProgressBar->setDurationInMs( m_pExplorer->getLastRecentRequestDurationInMs(CExplorer::ERequestGetNextValue) );
        //    m_pProgressBar->start();
        //}
        //else
        //{
        //    m_pProgressBar->setDurationInMs(5000);
        //    m_pProgressBar->start();
        //}
    }

} // onBtnCommandGetNextClicked

//------------------------------------------------------------------------------
void CWdgtExplorer::onBtnCommandGetBulkClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "onBtnCommandGetBulkClicked",
        /* strAddTrcInfo */ "" );

    m_pEdtLog->clear();

    m_requestResult = m_pExplorer->requestBulk(m_pEdtOidInput->text());

    if( m_requestResult.isAsynchronousRequest() )
    {
        m_pBtnGet->setEnabled(false);
        m_pBtnGetNext->setEnabled(false);
        m_pBtnGetBulk->setEnabled(false);
        m_pBtnWalk->setEnabled(false);
        m_pBtnAbort->setEnabled(true);

        //if( m_pExplorer->getRequestTimeoutInMs(CExplorer::ERequestGetBulk) > 0 )
        //{
        //    m_pProgressBar->setDurationInMs( m_pExplorer->getRequestTimeoutInMs(CExplorer::ERequestGetBulk) );
        //    m_pProgressBar->start();
        //}
        //else if( m_pExplorer->getLastRecentRequestDurationInMs(CExplorer::ERequestGetBulk) > 5000 )
        //{
        //    m_pProgressBar->setDurationInMs( m_pExplorer->getLastRecentRequestDurationInMs(CExplorer::ERequestGetBulk) );
        //    m_pProgressBar->start();
        //}
        //else
        //{
        //    m_pProgressBar->setDurationInMs(5000);
        //    m_pProgressBar->start();
        //}
    }

} // onBtnCommandGetBulkClicked

//------------------------------------------------------------------------------
void CWdgtExplorer::onBtnCommandWalkClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "onBtnCommandWalkClicked",
        /* strAddTrcInfo */ "" );

    m_pEdtLog->clear();

    m_requestResult = m_pExplorer->walk(m_pEdtOidInput->text());

    if( m_requestResult.isAsynchronousRequest() )
    {
        m_pBtnGet->setEnabled(false);
        m_pBtnGetNext->setEnabled(false);
        m_pBtnGetBulk->setEnabled(false);
        m_pBtnWalk->setEnabled(false);
        m_pBtnAbort->setEnabled(true);

        //if( m_pExplorer->getRequestTimeoutInMs(CExplorer::ERequestWalk) > 0 )
        //{
        //    m_pProgressBar->setDurationInMs( m_pExplorer->getRequestTimeoutInMs(CExplorer::ERequestWalk) );
        //    m_pProgressBar->start();
        //}
        //else if( m_pExplorer->getLastRecentRequestDurationInMs(CExplorer::ERequestWalk) > 5000 )
        //{
        //    m_pProgressBar->setDurationInMs( m_pExplorer->getLastRecentRequestDurationInMs(CExplorer::ERequestWalk) );
        //    m_pProgressBar->start();
        //}
        //else
        //{
        //    m_pProgressBar->setDurationInMs(60000);
        //    m_pProgressBar->start();
        //}
    }

} // onBtnCommandWalkClicked

//------------------------------------------------------------------------------
void CWdgtExplorer::onBtnCommandAbortClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "onBtnCommandAbortClicked",
        /* strAddTrcInfo */ "" );

    m_pBtnAbort->setEnabled(false);
    m_pBtnAbort->setText("Aborting ...");

    m_pExplorer->abortRequest(m_requestResult.m_iPendingRequestId);

    m_pBtnGet->setEnabled(true);
    m_pBtnGetNext->setEnabled(true);
    m_pBtnGetBulk->setEnabled(true);
    m_pBtnWalk->setEnabled(true);
    m_pBtnAbort->setEnabled(false);
    m_pBtnAbort->setText("Abort");

    //m_pProgressBar->stop();
    //m_pProgressBar->reset();

} // onBtnCommandAbortClicked

//------------------------------------------------------------------------------
void CWdgtExplorer::onChkLogEnableStateChanged( int i_iState )
//------------------------------------------------------------------------------
{
    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "onChkLogEnableStateChanged",
        /* strAddTrcInfo */ "" );

    switch( i_iState )
    {
        case Qt::PartiallyChecked:
        case Qt::Checked:
        {
            m_pLblLogEnable->setText("Disable Logging:");
            break;
        }
        case Qt::Unchecked:
        default:
        {
            m_pLblLogEnable->setText("Enable Logging:");
            break;
        }
    }

} // onChkLogEnableStateChanged

/*==============================================================================
protected slots: // connected to the signals of the explorer
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtExplorer::onExplorerStateChanged( QObject*, int i_iState, int i_iRequestInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "ExplorerState:" + CExplorer::state2Str(i_iState);
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(i_iRequestInProgress);
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "onExplorerStateChanged",
        /* strAddTrcInfo */ strAddTrcInfo );

} // onExplorerStateChanged

//------------------------------------------------------------------------------
void CWdgtExplorer::onExplorerRequestExecuted( QObject*, int i_iRequestId, const ZS::SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo += "ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
        strAddTrcInfo += ", RequestIdWaitingFor:" + QString::number(m_requestResult.m_iPendingRequestId);
        strAddTrcInfo += ", RequestIdReceived:" + QString::number(i_iRequestId);
        strAddTrcInfo += ", Result:" + i_errResultInfo.getResultStr();
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "onExplorerRequestExecuted",
        /* strAddTrcInfo */ strAddTrcInfo );

    if( i_iRequestId == m_requestResult.m_iPendingRequestId )
    {
        m_pEdtOidInput->setEnabled(true);
        m_pBtnOidNumericCopy2Input->setEnabled(true);
        m_pBtnOidTextualCopy2Input->setEnabled(true);
        m_pBtnOidModuleCopy2Input->setEnabled(true);
        m_pBtnGet->setEnabled(true);
        m_pBtnGetNext->setEnabled(true);
        m_pBtnGetBulk->setEnabled(true);
        m_pBtnWalk->setEnabled(true);
        m_pBtnAbort->setEnabled(false);
        m_pBtnAbort->setText("Abort");

        //m_pProgressBar->stop();
        //m_pProgressBar->reset();
    }

} // onExplorerRequestExecuted

//------------------------------------------------------------------------------
void CWdgtExplorer::onExplorerSendingMessage( QObject* /*i_pExplorer*/, ZS::CMsg* i_pMsg )
//------------------------------------------------------------------------------
{
    if( m_pChkLogEnable->isChecked() )
    {
        QString strMsgType = i_pMsg->msgTypeToStr();

        m_pEdtLog->append( "-> " + QString::number(ZS::System::Time::getProcTimeInSec(),'f',6) + " " + strMsgType + ": " );
        m_pEdtLog->append(i_pMsg->contentToStr());
    }

} // onExplorerSendingMessage

//------------------------------------------------------------------------------
void CWdgtExplorer::onExplorerReceivedMessage( QObject* /*i_pExplorer*/, ZS::CMsg* i_pMsg )
//------------------------------------------------------------------------------
{
    ZS::CMsgCon*       pMsgCon = dynamic_cast<ZS::CMsgCon*>(i_pMsg);
    ZS::SErrResultInfo errResultInfo;

    if( pMsgCon != NULL )
    {
        errResultInfo = pMsgCon->getErrResultInfo();
    }
    else
    {
        CMsgIndTimeout* pMsgInd = dynamic_cast<CMsgIndTimeout*>(i_pMsg);

        if( pMsgInd != NULL )
        {
            errResultInfo.setSeverity(ZS::EResultSeverityError);
            errResultInfo.setResult(ZS::EResultTimeout);
            errResultInfo.setResultStr(ZS::result2Str(ZS::EResultTimeout));
        }
    }

    if( m_pChkLogEnable->isChecked() || errResultInfo.getResult() != ZS::EResultSuccess )
    {
        QString strMsgType = i_pMsg->msgTypeToStr();

        m_pEdtLog->append( "<- " + QString::number(ZS::System::Time::getProcTimeInSec(),'f',6) + " " + strMsgType + ": " );
        m_pEdtLog->append(i_pMsg->contentToStr());
    }

    if( i_pMsg->getMsgType() == EMsgTypeConResponse )
    {
        if( m_pExplorer->getRequestInProgress() == CExplorer::ERequestGetNextValue
         || m_pExplorer->getRequestInProgress() == CExplorer::ERequestGetBulk )
        {
            CMsgConResponse* pMsgCon = dynamic_cast<CMsgConResponse*>(i_pMsg);

            if( pMsgCon != NULL && pMsgCon->getValueCount() > 0 )
            {
                int     idxLastValue = pMsgCon->getValueCount()-1;
                CValue* pValue = pMsgCon->getValue(idxLastValue,false);

                if( pValue != NULL )
                {
                    COid* pOid = pValue->getOid();

                    if( pOid != NULL )
                    {
                        m_pEdtOidInput->setText(pOid->toString(EOidFormatFull));
                        m_pEdtOidNumeric->setText(pOid->toString(EOidFormatNumeric));
                        m_pEdtOidTextual->setText(pOid->toString(EOidFormatFull));
                        m_pEdtOidModule->setText(pOid->toString(EOidFormatModule));
                    }
                }
            }
        }
    }

} // onExplorerReceivedMessage

/*==============================================================================
protected slots: // connected to the signals of the tree view
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtExplorer::onNodeSelected( COidNode* i_pOidNode )
//------------------------------------------------------------------------------
{
    if( m_pExplorer->getModelInstanceNodes() != NULL )
    {
        m_pExplorer->getModelInstanceNodes()->setOidNode(i_pOidNode);
    }

    if( i_pOidNode == NULL )
    {
        m_pEdtOidInput->setText("");
        m_pEdtOidNumeric->setText("");
        m_pEdtOidTextual->setText("");
        m_pEdtOidModule->setText("");
    }
    else
    {
        SOid soid = i_pOidNode->getOidStruct();

        m_pEdtOidInput->setText( soid.toString(EOidFormatFull) );
        m_pEdtOidNumeric->setText( soid.toString(EOidFormatNumeric) );
        m_pEdtOidTextual->setText( soid.toString(EOidFormatFull) );
        m_pEdtOidModule->setText( soid.toString(EOidFormatModule) );
    }

} // onNodeSelected

//------------------------------------------------------------------------------
void CWdgtExplorer::onNodeSelected( CInstanceNode* i_pInstanceNode )
//------------------------------------------------------------------------------
{
    if( i_pInstanceNode == NULL )
    {
        m_pEdtOidInput->setText("");
        m_pEdtOidNumeric->setText("");
        m_pEdtOidTextual->setText("");
        m_pEdtOidModule->setText("");
    }
    else
    {
        SOid soid = i_pInstanceNode->getOidStruct();

        m_pEdtOidInput->setText( soid.toString(EOidFormatFull) );
        m_pEdtOidNumeric->setText( soid.toString(EOidFormatNumeric) );
        m_pEdtOidTextual->setText( soid.toString(EOidFormatFull) );
        m_pEdtOidModule->setText( soid.toString(EOidFormatModule) );
    }

} // onNodeSelected
