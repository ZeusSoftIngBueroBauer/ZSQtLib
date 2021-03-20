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

$Revision: 3 $

$Author: Christian Bauer $

$History: ZSSNMPCltStdPCWdgt.cpp $
 * 
 * *****************  Version 3  *****************
 * User: Christian Bauer Date: 22.01.10   Time: 22:11
 * Updated in $/ZSSNMPCltStdPC/Source

*******************************************************************************/

#include <QtCore/qabstractitemmodel.h>
#include <QtCore/qsettings.h>
#include <QtGui/qapplication.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qgroupbox.h>
#include <QtGui/qheaderview.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpainter.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qtableview.h>
#include <QtGui/qtextedit.h>
#include <QtGui/qvalidator.h>
#include "ZSSNMP/Include/ZSSNMPCltStdPCWdgt.h"
#include "ZSSNMP/Include/ZSSNMPCltStdPCProcessorsModel.h"
#include "ZSSNMP/Include/ZSSNMPCltStdPCStoragesModel.h"
#include "ZSSNMP/Include/ZSSNMPCltStdPCSystemModel.h"
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
class CWdgtCltStdPC : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtCltStdPC::CWdgtCltStdPC(
    CCltStdPC*     i_pCltStdPC,
    QSettings*     i_pSettingsFile,
    const QString& i_strSettingsKey,
    QWidget*       i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pCltStdPC(i_pCltStdPC),
    m_pExplorer(i_pCltStdPC->getExplorer()),
    m_pSettingsFile(i_pSettingsFile),
    m_strSettingsKey(i_strSettingsKey),
    m_pLytMain(NULL),
    m_autoUpdateIntervals(),
    m_pGrpAutoUpdateIntervals(NULL),
    m_pLytGrpAutoUpdateIntervals(NULL),
    m_pBtnAutoUpdateStart(NULL),
    m_bAutoUpdateStarted(false),
    m_pLblAutoOpenSessionInterval_ms(NULL),
    m_pEdtAutoOpenSessionInterval_ms(NULL),
    m_pLblAutoUpdateIntervalAllValues_ms(NULL),
    m_pEdtAutoUpdateIntervalAllValues_ms(NULL),
    m_pLblAutoUpdateIntervalProcessorValues_ms(NULL),
    m_pEdtAutoUpdateIntervalProcessorValues_ms(NULL),
    m_pLblAutoUpdateIntervalStorageValues_ms(NULL),
    m_pEdtAutoUpdateIntervalStorageValues_ms(NULL),
    m_pBtnAutoUpdateIntervalsEdit(NULL),
    m_bEditingAutoUpdateIntervals(false),
    m_oids(),
    m_pGrpOids(NULL),
    m_pLytGrpOids(NULL),
    m_pBtnOidsEdit(NULL),
    m_bEditingOids(false),
    m_pLblOidSystem(NULL),
    m_pEdtOidSystem(NULL),
    m_pLblOidSysDescr(NULL),
    m_pEdtOidSysDescr(NULL),
    m_pLblOidSysName(NULL),
    m_pEdtOidSysName(NULL),
    m_pLblOidHrProcessorLoad(NULL),
    m_pEdtOidHrProcessorLoad(NULL),
    m_pLblOidHrStorageType(NULL),
    m_pEdtOidHrStorageType(NULL),
    m_pLblOidHrStorageDescr(NULL),
    m_pEdtOidHrStorageDescr(NULL),
    m_pLblOidHrStorageAllocationUnits(NULL),
    m_pEdtOidHrStorageAllocationUnits(NULL),
    m_pLblOidHrStorageSize(NULL),
    m_pEdtOidHrStorageSize(NULL),
    m_pLblOidHrStorageUsed(NULL),
    m_pEdtOidHrStorageUsed(NULL),
    m_pSplitterValueTables(NULL),
    m_pGrpSystem(NULL),
    m_pLytGrpSystem(NULL),
    m_pTableViewSystem(NULL),
    m_pGrpProcessors(NULL),
    m_pLytGrpProcessors(NULL),
    m_pTableViewProcessors(NULL),
    m_pGrpStorages(NULL),
    m_pLytGrpStorages(NULL),
    m_pTableViewStorages(NULL),
    m_pTrcAdminObj(NULL)
{
    int iRow;
    int iClm;

    setObjectName("WdgtCltStdPC");

    if( m_strSettingsKey.isEmpty() )
    {
        m_strSettingsKey = objectName();
    }

    m_pTrcAdminObj = CTrcServer::getTrcAdminObj(
        /* traceType         */ ETraceTypeMth,
        /* strClassNameSpace */ "ZS::SNMP",
        /* strClassName      */ "CWdgtCltStdPC",
        /* strObjNameSpace   */ "",
        /* strObjName        */ m_pExplorer->getName() );

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "ctor",
        /* strAddTrcInfo */ "" );

    m_pLytMain = new QVBoxLayout();
    setLayout(m_pLytMain);

    // <GroupBox> Auto Update Intervals
    //=================================

    m_autoUpdateIntervals = m_pCltStdPC->getAutoUpdateIntervals();

    m_pGrpAutoUpdateIntervals = new QGroupBox("Auto Update Intervals");
    m_pLytGrpAutoUpdateIntervals = new QGridLayout();
    m_pGrpAutoUpdateIntervals->setLayout(m_pLytGrpAutoUpdateIntervals);
    m_pLytMain->addWidget(m_pGrpAutoUpdateIntervals,0);

    // Line 1 (there is only one row)
    //-------------------------------

    iRow = 0;
    iClm = 0;

    // <Button> Auto Update Start
    //--------------------------

    m_pBtnAutoUpdateStart = new QPushButton("Start");
    m_pBtnAutoUpdateStart->setFixedWidth(80);
    m_pLytGrpAutoUpdateIntervals->addWidget(m_pBtnAutoUpdateStart,iRow,iClm++);

    if( !connect(
        /* pObjSender   */ m_pBtnAutoUpdateStart,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnUpdateStartClicked(bool)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }

    // <LineEdit> Open Session
    //------------------------

    m_pLblAutoOpenSessionInterval_ms = new QLabel("Open Session:");
    m_pLytGrpAutoUpdateIntervals->addWidget(m_pLblAutoOpenSessionInterval_ms,iRow,iClm++);
    m_pEdtAutoOpenSessionInterval_ms = new QLineEdit(QString::number(m_autoUpdateIntervals.m_iOpenSession_ms));
    m_pEdtAutoOpenSessionInterval_ms->setValidator( new QIntValidator(0,360000,this) );
    m_pEdtAutoOpenSessionInterval_ms->setEnabled(false);
    m_pLytGrpAutoUpdateIntervals->addWidget(m_pEdtAutoOpenSessionInterval_ms,iRow,iClm++);
    m_pLytGrpAutoUpdateIntervals->setColumnMinimumWidth(iClm++,10);

    // <LineEdit> All Values
    //----------------------

    m_pLblAutoUpdateIntervalAllValues_ms = new QLabel("All Values:");
    m_pLytGrpAutoUpdateIntervals->addWidget(m_pLblAutoUpdateIntervalAllValues_ms,iRow,iClm++);
    m_pEdtAutoUpdateIntervalAllValues_ms = new QLineEdit(QString::number(m_autoUpdateIntervals.m_iAllValues_ms));
    m_pEdtAutoUpdateIntervalAllValues_ms->setValidator( new QIntValidator(0,360000,this) );
    m_pEdtAutoUpdateIntervalAllValues_ms->setEnabled(false);
    m_pLytGrpAutoUpdateIntervals->addWidget(m_pEdtAutoUpdateIntervalAllValues_ms,iRow,iClm++);
    m_pLytGrpAutoUpdateIntervals->setColumnMinimumWidth(iClm++,10);

    // <LineEdit> Processor Values
    //----------------------------

    m_pLblAutoUpdateIntervalProcessorValues_ms = new QLabel("Processor Values:");
    m_pLytGrpAutoUpdateIntervals->addWidget(m_pLblAutoUpdateIntervalProcessorValues_ms,iRow,iClm++);
    m_pEdtAutoUpdateIntervalProcessorValues_ms = new QLineEdit(QString::number(m_autoUpdateIntervals.m_iProcessorValues_ms));
    m_pEdtAutoUpdateIntervalProcessorValues_ms->setValidator( new QIntValidator(0,360000,this) );
    m_pEdtAutoUpdateIntervalProcessorValues_ms->setEnabled(false);
    m_pLytGrpAutoUpdateIntervals->addWidget(m_pEdtAutoUpdateIntervalProcessorValues_ms,iRow,iClm++);
    m_pLytGrpAutoUpdateIntervals->setColumnMinimumWidth(iClm++,10);

    // <LineEdit> Storage Values
    //--------------------------

    m_pLblAutoUpdateIntervalStorageValues_ms = new QLabel("Storage Values:");
    m_pLytGrpAutoUpdateIntervals->addWidget(m_pLblAutoUpdateIntervalStorageValues_ms,iRow,iClm++);
    m_pEdtAutoUpdateIntervalStorageValues_ms = new QLineEdit(QString::number(m_autoUpdateIntervals.m_iStorageValues_ms));
    m_pEdtAutoUpdateIntervalStorageValues_ms->setValidator( new QIntValidator(0,360000,this) );
    m_pEdtAutoUpdateIntervalStorageValues_ms->setEnabled(false);
    m_pLytGrpAutoUpdateIntervals->addWidget(m_pEdtAutoUpdateIntervalStorageValues_ms,iRow,iClm++);
    m_pLytGrpAutoUpdateIntervals->setColumnMinimumWidth(iClm++,10);

    // <Button> Edit Settings
    //------------------------

    m_pBtnAutoUpdateIntervalsEdit = new QPushButton("Edit Intervals");
    m_pBtnAutoUpdateIntervalsEdit->setFixedWidth(120);
    m_pLytGrpAutoUpdateIntervals->addWidget(m_pBtnAutoUpdateIntervalsEdit,iRow,iClm++);

    if( !connect(
        /* pObjSender   */ m_pBtnAutoUpdateIntervalsEdit,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnUpdateIntervalsEditClicked(bool)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }

    // <GroupBox> Oids
    //================

    m_oids = m_pCltStdPC->getOids();

    m_pGrpOids = new QGroupBox("Oids");
    m_pLytGrpOids = new QGridLayout();
    m_pGrpOids->setLayout(m_pLytGrpOids);
    m_pLytMain->addWidget(m_pGrpOids,0);

    // Columns 0 and 1
    //----------------

    // (one column for the labels and one column for the edit fields)

    iRow = 0;
    iClm = 0;

    // <LineEdit> Oid System
    //----------------------

    m_pLblOidSystem = new QLabel("system:");
    m_pLytGrpOids->addWidget(m_pLblOidSystem,iRow,iClm);
    m_pEdtOidSystem = new QLineEdit( m_oids.m_oidSystem.toString() );
    m_pEdtOidSystem->setEnabled(false);
    m_pEdtOidSystem->setMinimumWidth(120);
    m_pLytGrpOids->addWidget(m_pEdtOidSystem,iRow,iClm+1);
    iRow++;

    // <LineEdit> Oid SysDescr
    //------------------------

    m_pLblOidSysDescr = new QLabel("sysDescr:");
    m_pLytGrpOids->addWidget(m_pLblOidSysDescr,iRow,iClm);
    m_pEdtOidSysDescr = new QLineEdit( m_oids.m_oidSysDescr.toString() );
    m_pEdtOidSysDescr->setEnabled(false);
    m_pEdtOidSysDescr->setMinimumWidth(120);
    m_pLytGrpOids->addWidget(m_pEdtOidSysDescr,iRow,iClm+1);
    iRow++;

    // <LineEdit> Oid SysName
    //------------------------

    m_pLblOidSysName = new QLabel("sysName:");
    m_pLytGrpOids->addWidget(m_pLblOidSysName,iRow,iClm);
    m_pEdtOidSysName = new QLineEdit( m_oids.m_oidSysName.toString() );
    m_pEdtOidSysName->setEnabled(false);
    m_pEdtOidSysName->setMinimumWidth(120);
    m_pLytGrpOids->addWidget(m_pEdtOidSysName,iRow,iClm+1);
    iRow++;

    // Column 2
    //---------

    // (space between column 1 and column 3)

    iRow = 0;
    iClm = 2;

    m_pLytGrpOids->setColumnMinimumWidth(iClm,10);

    // Columns 3 and 4
    //----------------

    // (one column for the labels and one column for the edit fields)

    iRow = 0;
    iClm = 3;

    // <LineEdit> Oid HrProcessorLoad
    //--------------------------------

    m_pLblOidHrProcessorLoad = new QLabel("hrProcessorLoad:");
    m_pLytGrpOids->addWidget(m_pLblOidHrProcessorLoad,iRow,iClm);
    m_pEdtOidHrProcessorLoad = new QLineEdit( m_oids.m_oidHrProcessorLoad.toString() );
    m_pEdtOidHrProcessorLoad->setEnabled(false);
    m_pEdtOidHrProcessorLoad->setMinimumWidth(120);
    m_pLytGrpOids->addWidget(m_pEdtOidHrProcessorLoad,iRow,iClm+1);
    iRow++;

    // <LineEdit> Oid HrStorageType
    //-----------------------------

    m_pLblOidHrStorageType = new QLabel("hrStorageType:");
    m_pLytGrpOids->addWidget(m_pLblOidHrStorageType,iRow,iClm);
    m_pEdtOidHrStorageType = new QLineEdit( m_oids.m_oidHrStorageType.toString() );
    m_pEdtOidHrStorageType->setEnabled(false);
    m_pEdtOidHrStorageType->setMinimumWidth(120);
    m_pLytGrpOids->addWidget(m_pEdtOidHrStorageType,iRow,iClm+1);
    iRow++;

    // <LineEdit> Oid HrStorageDescr
    //------------------------------

    m_pLblOidHrStorageDescr = new QLabel("hrStorageDescr:");
    m_pLytGrpOids->addWidget(m_pLblOidHrStorageDescr,iRow,iClm);
    m_pEdtOidHrStorageDescr = new QLineEdit( m_oids.m_oidHrStorageDescr.toString() );
    m_pEdtOidHrStorageDescr->setEnabled(false);
    m_pEdtOidHrStorageDescr->setMinimumWidth(120);
    m_pLytGrpOids->addWidget(m_pEdtOidHrStorageDescr,iRow,iClm+1);
    iRow++;

    // <LineEdit> Oid HrStorageAllocationUnits
    //----------------------------------------

    m_pLblOidHrStorageAllocationUnits = new QLabel("hrStorageAllocationUnits:");
    m_pLytGrpOids->addWidget(m_pLblOidHrStorageAllocationUnits,iRow,iClm);
    m_pEdtOidHrStorageAllocationUnits = new QLineEdit( m_oids.m_oidHrStorageAllocationUnits.toString() );
    m_pEdtOidHrStorageAllocationUnits->setEnabled(false);
    m_pEdtOidHrStorageAllocationUnits->setMinimumWidth(120);
    m_pLytGrpOids->addWidget(m_pEdtOidHrStorageAllocationUnits,iRow,iClm+1);
    iRow++;

    // Column 5
    //---------

    // (space between column 4 and column 6)

    iRow = 0;
    iClm = 5;

    m_pLytGrpOids->setColumnMinimumWidth(iClm,10);

    // Columns 6 and 7
    //----------------

    // (one column for the labels and one column for the edit fields)

    iRow = 0;
    iClm = 6;

    // <Button> Edit Oids
    //------------------------

    m_pBtnOidsEdit = new QPushButton("Edit Oids");
    m_pBtnOidsEdit->setFixedWidth(120);
    m_pLytGrpOids->addWidget(m_pBtnOidsEdit,iRow,iClm+1,Qt::AlignRight);
    iRow++;

    if( !connect(
        /* pObjSender   */ m_pBtnOidsEdit,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnOidsEditClicked(bool)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }

    // <LineEdit> Oid HrStorageSize
    //-----------------------------

    m_pLblOidHrStorageSize = new QLabel("hrStorageSize:");
    m_pLytGrpOids->addWidget(m_pLblOidHrStorageSize,iRow,iClm);
    m_pEdtOidHrStorageSize = new QLineEdit( m_oids.m_oidHrStorageSize.toString() );
    m_pEdtOidHrStorageSize->setEnabled(false);
    m_pEdtOidHrStorageSize->setMinimumWidth(120);
    m_pLytGrpOids->addWidget(m_pEdtOidHrStorageSize,iRow,iClm+1);
    iRow++;

    // <LineEdit> Oid HrStorageUsed
    //-----------------------------

    m_pLblOidHrStorageUsed = new QLabel("hrStorageUsed:");
    m_pLytGrpOids->addWidget(m_pLblOidHrStorageUsed,iRow,iClm);
    m_pEdtOidHrStorageUsed = new QLineEdit( m_oids.m_oidHrStorageUsed.toString() );
    m_pEdtOidHrStorageUsed->setEnabled(false);
    m_pEdtOidHrStorageUsed->setMinimumWidth(120);
    m_pLytGrpOids->addWidget(m_pEdtOidHrStorageUsed,iRow,iClm+1);
    iRow++;

    // <Splitter> Table Values
    //========================

    m_pSplitterValueTables = new QSplitter(this);
    m_pSplitterValueTables->setOrientation(Qt::Vertical);
    m_pSplitterValueTables->setChildrenCollapsible(false);
    m_pLytMain->addWidget(m_pSplitterValueTables);

    // <GroupBox> System
    //==================

    //m_pGrpSystem = new QGroupBox("System");
    //m_pLytGrpSystem = new QVBoxLayout();
    //m_pGrpSystem->setLayout(m_pLytGrpSystem);
    //m_pSplitterValueTables->addWidget(m_pGrpSystem);

    m_pTableViewSystem = new QTableView(this);
    //m_pLytGrpSystem->addWidget(m_pTableViewSystem);
    m_pSplitterValueTables->addWidget(m_pTableViewSystem);

    QFont fntTableView = m_pTableViewSystem->font();
    fntTableView.setPointSize(8);

    m_pTableViewSystem->setModel( m_pCltStdPC->getModelSystem() );
    m_pTableViewSystem->setFont(fntTableView);
    m_pTableViewSystem->setShowGrid(false);
    m_pTableViewSystem->setAlternatingRowColors(true);
    m_pTableViewSystem->verticalHeader()->hide();
    m_pTableViewSystem->horizontalHeader()->show();
    m_pTableViewSystem->setTextElideMode(Qt::ElideRight);
    m_pTableViewSystem->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_pTableViewSystem->hideColumn(CModelSystem::EColumnSysDescrOid);
    m_pTableViewSystem->hideColumn(CModelSystem::EColumnSysNameOid);
    m_pTableViewSystem->resizeColumnsToContents();
    m_pTableViewSystem->resizeRowsToContents();

    // <GroupBox> Processors
    //======================

    //m_pGrpProcessors = new QGroupBox("Processors");
    //m_pLytGrpProcessors = new QVBoxLayout();
    //m_pGrpProcessors->setLayout(m_pLytGrpProcessors);
    //m_pSplitterValueTables->addWidget(m_pGrpProcessors);

    m_pTableViewProcessors = new QTableView(this);
    //m_pLytGrpProcessors->addWidget(m_pTableViewProcessors);
    m_pSplitterValueTables->addWidget(m_pTableViewProcessors);

    m_pTableViewProcessors->setModel( m_pCltStdPC->getModelProcessors() );
    m_pTableViewProcessors->setFont(fntTableView);
    m_pTableViewProcessors->setShowGrid(false);
    m_pTableViewProcessors->setAlternatingRowColors(true);
    m_pTableViewProcessors->verticalHeader()->hide();
    m_pTableViewProcessors->horizontalHeader()->show();
    m_pTableViewProcessors->setTextElideMode(Qt::ElideRight);
    m_pTableViewProcessors->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_pTableViewProcessors->hideColumn(CModelProcessors::EColumnLoadOid);
    m_pTableViewProcessors->resizeColumnsToContents();
    m_pTableViewProcessors->resizeRowsToContents();

    // <GroupBox> Storages
    //====================

    //m_pGrpStorages = new QGroupBox("Storages");
    //m_pLytGrpStorages = new QVBoxLayout();
    //m_pGrpStorages->setLayout(m_pLytGrpStorages);
    //m_pSplitterValueTables->addWidget(m_pGrpStorages);

    m_pTableViewStorages = new QTableView(this);
    //m_pLytGrpStorages->addWidget(m_pTableViewStorages);
    m_pSplitterValueTables->addWidget(m_pTableViewStorages);

    m_pTableViewStorages->setModel( m_pCltStdPC->getModelStorages() );
    m_pTableViewStorages->setFont(fntTableView);
    m_pTableViewStorages->setShowGrid(false);
    m_pTableViewStorages->setAlternatingRowColors(true);
    m_pTableViewStorages->verticalHeader()->hide();
    m_pTableViewStorages->horizontalHeader()->show();
    m_pTableViewStorages->setTextElideMode(Qt::ElideRight);
    m_pTableViewStorages->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_pTableViewStorages->hideColumn(CModelStorages::EColumnTypeOid);
    m_pTableViewStorages->hideColumn(CModelStorages::EColumnTypeValue);
    m_pTableViewStorages->hideColumn(CModelStorages::EColumnDescrOid);
    m_pTableViewStorages->hideColumn(CModelStorages::EColumnAllocationUnitsOid);
    m_pTableViewStorages->hideColumn(CModelStorages::EColumnSizeOid);
    m_pTableViewStorages->hideColumn(CModelStorages::EColumnUsedOid);
    m_pTableViewStorages->resizeColumnsToContents();
    m_pTableViewStorages->resizeRowsToContents();

    // Stretch at bottom of main layout
    //=================================

    m_pLytMain->addStretch();

    // Restore splitter position
    //==========================

    if( m_pSettingsFile != NULL )
    {
        QList<int> listSizes = m_pSplitterValueTables->sizes();

        for( int idx = 0; idx < listSizes.count(); idx++ )
        {
            listSizes[idx] = m_pSettingsFile->value( m_strSettingsKey + "/SplitterValueTablesHeight" + QString::number(idx), 50 ).toInt();
        }
        m_pSplitterValueTables->setSizes(listSizes);
    }

    // Keeping indicated session parameters up to date ..
    //===================================================

    if( !connect(
        /* pObjSender   */ m_pCltStdPC,
        /* szSignal     */ SIGNAL(autoUpdateIntervalsChanged(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCltStdPCUpdateIntervalsChanged(QObject*)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }
    if( !connect(
        /* pObjSender   */ m_pCltStdPC,
        /* szSignal     */ SIGNAL(oidsChanged(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCltStdPCOidsChanged(QObject*)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }
    if( !connect(
        /* pObjSender   */ m_pCltStdPC,
        /* szSignal     */ SIGNAL(stateChanged(QObject*,int,int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCltStdPCStateChanged(QObject*,int,int)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }
    if( !connect(
        /* pObjSender   */ m_pCltStdPC,
        /* szSignal     */ SIGNAL(receivedSystemValues(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCltStdPCReceivedSystemValues(QObject*)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }
    if( !connect(
        /* pObjSender   */ m_pCltStdPC,
        /* szSignal     */ SIGNAL(receivedProcessorValues(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCltStdPCReceivedProcessorValues(QObject*)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }
    if( !connect(
        /* pObjSender   */ m_pCltStdPC,
        /* szSignal     */ SIGNAL(receivedStorageValues(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCltStdPCReceivedStorageValues(QObject*)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }

} // ctor

//------------------------------------------------------------------------------
CWdgtCltStdPC::~CWdgtCltStdPC()
//------------------------------------------------------------------------------
{
    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "dtor",
        /* strAddTrcInfo */ "" );

    if( m_pSettingsFile != NULL )
    {
        QList<int> listSizes = m_pSplitterValueTables->sizes();

        for( int idx = 0; idx < listSizes.count(); idx++ )
        {
            m_pSettingsFile->setValue( m_strSettingsKey + "/SplitterValueTablesHeight" + QString::number(idx), listSizes[idx] );
        }
    }

    m_pCltStdPC = NULL;
    m_pExplorer = NULL;
    m_pLytMain = NULL;
    m_pGrpAutoUpdateIntervals = NULL;
    m_pLytGrpAutoUpdateIntervals = NULL;
    m_pBtnAutoUpdateStart = NULL;
    m_pLblAutoOpenSessionInterval_ms = NULL;
    m_pEdtAutoOpenSessionInterval_ms = NULL;
    m_pLblAutoUpdateIntervalAllValues_ms = NULL;
    m_pEdtAutoUpdateIntervalAllValues_ms = NULL;
    m_pLblAutoUpdateIntervalProcessorValues_ms = NULL;
    m_pEdtAutoUpdateIntervalProcessorValues_ms = NULL;
    m_pLblAutoUpdateIntervalStorageValues_ms = NULL;
    m_pEdtAutoUpdateIntervalStorageValues_ms = NULL;
    m_pBtnAutoUpdateIntervalsEdit = NULL;
    m_pGrpOids = NULL;
    m_pLytGrpOids = NULL;
    m_pBtnOidsEdit = NULL;
    m_pLblOidSystem = NULL;
    m_pEdtOidSystem = NULL;
    m_pLblOidSysDescr = NULL;
    m_pEdtOidSysDescr = NULL;
    m_pLblOidSysName = NULL;
    m_pEdtOidSysName = NULL;
    m_pLblOidHrProcessorLoad = NULL;
    m_pEdtOidHrProcessorLoad = NULL;
    m_pLblOidHrStorageType = NULL;
    m_pEdtOidHrStorageType = NULL;
    m_pLblOidHrStorageDescr = NULL;
    m_pEdtOidHrStorageDescr = NULL;
    m_pLblOidHrStorageAllocationUnits = NULL;
    m_pEdtOidHrStorageAllocationUnits = NULL;
    m_pLblOidHrStorageSize = NULL;
    m_pEdtOidHrStorageSize = NULL;
    m_pLblOidHrStorageUsed = NULL;
    m_pEdtOidHrStorageUsed = NULL;
    m_pSplitterValueTables = NULL;
    m_pGrpSystem = NULL;
    m_pLytGrpSystem = NULL;
    m_pGrpProcessors = NULL;
    m_pLytGrpProcessors = NULL;
    m_pGrpSystem = NULL;
    m_pLytGrpSystem = NULL;
    m_pTableViewSystem = NULL;
    m_pGrpProcessors = NULL;
    m_pLytGrpProcessors = NULL;
    m_pTableViewProcessors = NULL;
    m_pGrpStorages = NULL;
    m_pLytGrpStorages = NULL;
    m_pTableViewStorages = NULL;

    CTrcServer::releaseTrcAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = NULL;

} // dtor

/*==============================================================================
protected slots: // connected to the signals of my user controls
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtCltStdPC::onBtnUpdateStartClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo += "StdPCCltState:" + CCltStdPC::state2Str(m_pCltStdPC->getCurrentState());
        strAddTrcInfo += ", StdPCCltRequestInProgress:" + CCltStdPC::request2Str(m_pCltStdPC->getRequestInProgress());
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "onBtnSessionStartClicked",
        /* strAddTrcInfo */ strAddTrcInfo );

    if( !m_bAutoUpdateStarted )
    {
        if( m_autoUpdateIntervals.m_iOpenSession_ms == 0
         && m_autoUpdateIntervals.m_iAllValues_ms == 0
         && m_autoUpdateIntervals.m_iProcessorValues_ms == 0
         && m_autoUpdateIntervals.m_iStorageValues_ms == 0 )
        {
            m_pCltStdPC->updateAllValues();
        }
        else
        {
            m_bAutoUpdateStarted = true;
            m_pBtnAutoUpdateStart->setText("Stop");
            m_pCltStdPC->start();
        }
    }
    else
    {
        m_bAutoUpdateStarted = false;
        m_pBtnAutoUpdateStart->setText("Start");
        m_pCltStdPC->stop();
    }

} // onBtnUpdateStartClicked

//------------------------------------------------------------------------------
void CWdgtCltStdPC::onBtnUpdateIntervalsEditClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo += "StdPCCltState:" + CCltStdPC::state2Str(m_pCltStdPC->getCurrentState());
        strAddTrcInfo += ", StdPCCltRequestInProgress:" + CCltStdPC::request2Str(m_pCltStdPC->getRequestInProgress());
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "onBtnUpdateIntervalsEditClicked",
        /* strAddTrcInfo */ strAddTrcInfo );

    if( !m_bEditingAutoUpdateIntervals )
    {
        m_pEdtAutoOpenSessionInterval_ms->setEnabled(true);
        m_pEdtAutoUpdateIntervalAllValues_ms->setEnabled(true);
        m_pEdtAutoUpdateIntervalProcessorValues_ms->setEnabled(true);
        m_pEdtAutoUpdateIntervalStorageValues_ms->setEnabled(true);

        m_bEditingAutoUpdateIntervals = true;
        m_pBtnAutoUpdateIntervalsEdit->setText("Accept ...");

        if( m_autoUpdateIntervals.m_iOpenSession_ms == 0
         && m_autoUpdateIntervals.m_iAllValues_ms == 0
         && m_autoUpdateIntervals.m_iProcessorValues_ms == 0
         && m_autoUpdateIntervals.m_iStorageValues_ms == 0 )
        {
            if( m_bAutoUpdateStarted )
            {
                m_bAutoUpdateStarted = false;
                m_pBtnAutoUpdateStart->setText("Start");
                m_pCltStdPC->stop();
            }
        }
    }
    else
    {
        bool bSettingsChanged = false;

        if( m_autoUpdateIntervals.m_iOpenSession_ms != m_pEdtAutoOpenSessionInterval_ms->text().toInt() )
        {
            m_autoUpdateIntervals.m_iOpenSession_ms = m_pEdtAutoOpenSessionInterval_ms->text().toInt();
            bSettingsChanged = true;
        }
        if( m_autoUpdateIntervals.m_iAllValues_ms != m_pEdtAutoUpdateIntervalAllValues_ms->text().toInt() )
        {
            m_autoUpdateIntervals.m_iAllValues_ms = m_pEdtAutoUpdateIntervalAllValues_ms->text().toInt();
            bSettingsChanged = true;
        }
        if( m_autoUpdateIntervals.m_iProcessorValues_ms != m_pEdtAutoUpdateIntervalProcessorValues_ms->text().toInt() )
        {
            m_autoUpdateIntervals.m_iProcessorValues_ms = m_pEdtAutoUpdateIntervalProcessorValues_ms->text().toInt();
            bSettingsChanged = true;
        }
        if( m_autoUpdateIntervals.m_iStorageValues_ms != m_pEdtAutoUpdateIntervalStorageValues_ms->text().toInt() )
        {
            m_autoUpdateIntervals.m_iStorageValues_ms = m_pEdtAutoUpdateIntervalStorageValues_ms->text().toInt();
            bSettingsChanged = true;
        }
        if( bSettingsChanged )
        {
            disconnect(
                /* pObjSender   */ m_pCltStdPC,
                /* szSignal     */ SIGNAL(autoUpdateIntervalsChanged(QObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onCltStdPCUpdateIntervalsChanged(QObject*)) );

            m_pCltStdPC->setAutoUpdateIntervals(m_autoUpdateIntervals);

            connect(
                /* pObjSender   */ m_pCltStdPC,
                /* szSignal     */ SIGNAL(autoUpdateIntervalsChanged(QObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onCltStdPCUpdateIntervalsChanged(QObject*)) );
        }

        m_pEdtAutoOpenSessionInterval_ms->setEnabled(false);
        m_pEdtAutoUpdateIntervalAllValues_ms->setEnabled(false);
        m_pEdtAutoUpdateIntervalProcessorValues_ms->setEnabled(false);
        m_pEdtAutoUpdateIntervalStorageValues_ms->setEnabled(false);

        m_bEditingAutoUpdateIntervals = false;
        m_pBtnAutoUpdateIntervalsEdit->setText("Edit Intervals");
    }

} // onBtnUpdateIntervalsEditClicked

//------------------------------------------------------------------------------
void CWdgtCltStdPC::onBtnOidsEditClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo += "StdPCCltState:" + CCltStdPC::state2Str(m_pCltStdPC->getCurrentState());
        strAddTrcInfo += ", StdPCCltRequestInProgress:" + CCltStdPC::request2Str(m_pCltStdPC->getRequestInProgress());
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "onBtnOidsEditClicked",
        /* strAddTrcInfo */ strAddTrcInfo );

    if( !m_bEditingOids )
    {
        m_pEdtOidSysDescr->setEnabled(true);
        m_pEdtOidSysName->setEnabled(true);
        m_pEdtOidHrProcessorLoad->setEnabled(true);
        m_pEdtOidHrStorageType->setEnabled(true);
        m_pEdtOidHrStorageDescr->setEnabled(true);
        m_pEdtOidHrStorageAllocationUnits->setEnabled(true);
        m_pEdtOidHrStorageSize->setEnabled(true);
        m_pEdtOidHrStorageUsed->setEnabled(true);

        m_bEditingOids = true;
        m_pBtnOidsEdit->setText("Accept ...");
    }
    else
    {
        bool bSettingsChanged = false;

        COid oidSysDescr( m_pEdtOidSysDescr->text() );
        COid oidSysName( m_pEdtOidSysName->text() );
        COid oidProcessorLoad( m_pEdtOidHrProcessorLoad->text() );
        COid oidHrStorageType( m_pEdtOidHrStorageType->text() );
        COid oidHrStorageDescr( m_pEdtOidHrStorageDescr->text() );
        COid oidHrStorageAllocationUnits( m_pEdtOidHrStorageAllocationUnits->text() );
        COid oidHrStorageSize( m_pEdtOidHrStorageSize->text() );
        COid oidHrStorageUsed( m_pEdtOidHrStorageUsed->text() );

        if( m_oids.m_oidSystem != m_pEdtOidSystem->text() )
        {
            m_oids.m_oidSystem = m_pEdtOidSystem->text();
            bSettingsChanged = true;
        }
        if( m_oids.m_oidSysDescr != m_pEdtOidSysDescr->text() )
        {
            m_oids.m_oidSysDescr = m_pEdtOidSysDescr->text();
            bSettingsChanged = true;
        }
        if( m_oids.m_oidSysName != m_pEdtOidSysName->text() )
        {
            m_oids.m_oidSysName = m_pEdtOidSysName->text();
            bSettingsChanged = true;
        }
        if( m_oids.m_oidHrProcessorLoad != m_pEdtOidHrProcessorLoad->text() )
        {
            m_oids.m_oidHrProcessorLoad = m_pEdtOidHrProcessorLoad->text();
            bSettingsChanged = true;
        }
        if( m_oids.m_oidHrStorageType != m_pEdtOidHrStorageType->text() )
        {
            m_oids.m_oidHrStorageType = m_pEdtOidHrStorageType->text();
            bSettingsChanged = true;
        }
        if( m_oids.m_oidHrStorageDescr != m_pEdtOidHrStorageDescr->text() )
        {
            m_oids.m_oidHrStorageDescr = m_pEdtOidHrStorageDescr->text();
            bSettingsChanged = true;
        }
        if( m_oids.m_oidHrStorageAllocationUnits != m_pEdtOidHrStorageAllocationUnits->text() )
        {
            m_oids.m_oidHrStorageAllocationUnits = m_pEdtOidHrStorageAllocationUnits->text();
            bSettingsChanged = true;
        }
        if( m_oids.m_oidHrStorageSize != m_pEdtOidHrStorageSize->text() )
        {
            m_oids.m_oidHrStorageSize = m_pEdtOidHrStorageSize->text();
            bSettingsChanged = true;
        }
        if( m_oids.m_oidHrStorageUsed != m_pEdtOidHrStorageUsed->text() )
        {
            m_oids.m_oidHrStorageUsed = m_pEdtOidHrStorageUsed->text();
            bSettingsChanged = true;
        }
        if( bSettingsChanged )
        {
            disconnect(
                /* pObjSender   */ m_pCltStdPC,
                /* szSignal     */ SIGNAL(oidsSettingsChanged(QObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onOidsSettingsChanged(QObject*)) );

            m_pCltStdPC->setOids(m_oids);

            connect(
                /* pObjSender   */ m_pCltStdPC,
                /* szSignal     */ SIGNAL(oidsSettingsChanged(QObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onOidsSettingsChanged(QObject*)) );
        }

        m_pEdtOidSysDescr->setEnabled(false);
        m_pEdtOidSysName->setEnabled(false);
        m_pEdtOidHrProcessorLoad->setEnabled(false);
        m_pEdtOidHrStorageType->setEnabled(false);
        m_pEdtOidHrStorageDescr->setEnabled(false);
        m_pEdtOidHrStorageAllocationUnits->setEnabled(false);
        m_pEdtOidHrStorageSize->setEnabled(false);
        m_pEdtOidHrStorageUsed->setEnabled(false);

        m_bEditingOids = false;
        m_pBtnOidsEdit->setText("Edit Oids");
    }

} // onBtnOidsEditClicked

/*==============================================================================
protected slots: // connected to the signals of the explorer
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtCltStdPC::onCltStdPCUpdateIntervalsChanged( QObject* /*i_pCltStdPC*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo += "StdPCCltState:" + CCltStdPC::state2Str(m_pCltStdPC->getCurrentState());
        strAddTrcInfo += ", StdPCCltRequestInProgress:" + CCltStdPC::request2Str(m_pCltStdPC->getRequestInProgress());
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "onCltStdPCUpdateIntervalsChanged",
        /* strAddTrcInfo */ strAddTrcInfo );

    m_autoUpdateIntervals = m_pCltStdPC->getAutoUpdateIntervals();

    m_pEdtAutoOpenSessionInterval_ms->setText(QString::number(m_autoUpdateIntervals.m_iOpenSession_ms));
    m_pEdtAutoUpdateIntervalAllValues_ms->setText(QString::number(m_autoUpdateIntervals.m_iAllValues_ms));
    m_pEdtAutoUpdateIntervalProcessorValues_ms->setText(QString::number(m_autoUpdateIntervals.m_iProcessorValues_ms));
    m_pEdtAutoUpdateIntervalStorageValues_ms->setText(QString::number(m_autoUpdateIntervals.m_iStorageValues_ms));

} // onCltStdPCUpdateIntervalsChanged

//------------------------------------------------------------------------------
void CWdgtCltStdPC::onCltStdPCOidsChanged( QObject* /*i_pCltStdPC*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo += "StdPCCltState:" + CCltStdPC::state2Str(m_pCltStdPC->getCurrentState());
        strAddTrcInfo += ", StdPCCltRequestInProgress:" + CCltStdPC::request2Str(m_pCltStdPC->getRequestInProgress());
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "onCltStdPCOidsChanged",
        /* strAddTrcInfo */ strAddTrcInfo );

    m_oids = m_pCltStdPC->getOids();

    m_pEdtOidSystem->setText( m_oids.m_oidSystem.toString() );
    m_pEdtOidSysDescr->setText( m_oids.m_oidSysDescr.toString() );
    m_pEdtOidSysName->setText( m_oids.m_oidSysName.toString() );
    m_pEdtOidHrProcessorLoad->setText( m_oids.m_oidHrProcessorLoad.toString() );
    m_pEdtOidHrStorageType->setText( m_oids.m_oidHrStorageType.toString() );
    m_pEdtOidHrStorageDescr->setText( m_oids.m_oidHrStorageDescr.toString() );
    m_pEdtOidHrStorageAllocationUnits->setText( m_oids.m_oidHrStorageAllocationUnits.toString() );
    m_pEdtOidHrStorageSize->setText( m_oids.m_oidHrStorageSize.toString() );
    m_pEdtOidHrStorageUsed->setText( m_oids.m_oidHrStorageUsed.toString() );

} // onCltStdPCOidsChanged

//------------------------------------------------------------------------------
void CWdgtCltStdPC::onCltStdPCStateChanged( QObject*, int i_iState, int /*i_iRequestInProgress*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo += "StdPCCltState:" + CCltStdPC::state2Str(m_pCltStdPC->getCurrentState());
        strAddTrcInfo += ", StdPCCltRequestInProgress:" + CCltStdPC::request2Str(m_pCltStdPC->getRequestInProgress());
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "onCltStdPCStateChanged",
        /* strAddTrcInfo */ strAddTrcInfo );

    if( m_pBtnAutoUpdateStart != NULL )
    {
        if( i_iState == CCltStdPC::EStateStartedSessionClosed || i_iState == CCltStdPC::EStateStartedSessionOpened )
        {
            m_pBtnAutoUpdateStart->setText("Stop");
            m_bAutoUpdateStarted = true;
        }
        else // if( i_iState == CCltStdPC::EStateStopped )
        {
            m_pBtnAutoUpdateStart->setText("Start");
            m_bAutoUpdateStarted = false;
        }
    }

    //if( m_pProgressBar != NULL )
    //{
    //    if( i_iRequestInProgress == CCltStdPC::ERequestNone )
    //    {
    //        m_pProgressBar->stop();
    //        m_pProgressBar->reset();
    //    }
    //    else
    //    {
    //        if( m_pCltStdPC->getRequestTimeoutInMs(static_cast<CCltStdPC::ERequest>(i_iRequestInProgress)) > 0 )
    //        {
    //            m_pProgressBar->setDurationInMs( m_pCltStdPC->getRequestTimeoutInMs(static_cast<CCltStdPC::ERequest>(i_iRequestInProgress)) );
    //            m_pProgressBar->start();
    //        }
    //        else if( m_pCltStdPC->getLastRecentRequestDurationInMs(static_cast<CCltStdPC::ERequest>(i_iRequestInProgress)) > 5000 )
    //        {
    //            m_pProgressBar->setDurationInMs( m_pCltStdPC->getLastRecentRequestDurationInMs(static_cast<CCltStdPC::ERequest>(i_iRequestInProgress)) );
    //            m_pProgressBar->start();
    //        }
    //        else
    //        {
    //            m_pProgressBar->setDurationInMs(5000);
    //            m_pProgressBar->start();
    //        }
    //    }
    //}

} // onCltStdPCStateChanged

//------------------------------------------------------------------------------
void CWdgtCltStdPC::onCltStdPCReceivedSystemValues( QObject* )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo += "StdPCCltState:" + CCltStdPC::state2Str(m_pCltStdPC->getCurrentState());
        strAddTrcInfo += ", StdPCCltRequestInProgress:" + CCltStdPC::request2Str(m_pCltStdPC->getRequestInProgress());
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "onCltStdPCReceivedSystemValues",
        /* strAddTrcInfo */ strAddTrcInfo );

    m_pTableViewSystem->resizeColumnsToContents();
    m_pTableViewSystem->resizeRowsToContents();

} // onCltStdPCReceivedSystemValues

//------------------------------------------------------------------------------
void CWdgtCltStdPC::onCltStdPCReceivedProcessorValues( QObject* )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo += "StdPCCltState:" + CCltStdPC::state2Str(m_pCltStdPC->getCurrentState());
        strAddTrcInfo += ", StdPCCltRequestInProgress:" + CCltStdPC::request2Str(m_pCltStdPC->getRequestInProgress());
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "onCltStdPCReceivedProcessorValues",
        /* strAddTrcInfo */ strAddTrcInfo );

    m_pTableViewProcessors->resizeColumnsToContents();
    m_pTableViewProcessors->resizeRowsToContents();

} // onCltStdPCReceivedProcessorValues

//------------------------------------------------------------------------------
void CWdgtCltStdPC::onCltStdPCReceivedStorageValues( QObject* )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo += "StdPCCltState:" + CCltStdPC::state2Str(m_pCltStdPC->getCurrentState());
        strAddTrcInfo += ", StdPCCltRequestInProgress:" + CCltStdPC::request2Str(m_pCltStdPC->getRequestInProgress());
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "onCltStdPCReceivedStorageValues",
        /* strAddTrcInfo */ strAddTrcInfo );

    m_pTableViewStorages->resizeColumnsToContents();
    m_pTableViewStorages->resizeRowsToContents();

} // onCltStdPCReceivedStorageValues
