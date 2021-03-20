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
#include <QtGui/qbitmap.h>
#include <QtGui/qicon.h>

#if QT_VERSION < 0x050000
#include <QtGui/qlayout.h>
#include <QtGui/qmenu.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qtreeview.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qmenu.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qtreeview.h>
#endif

#include "ZSDataGUI/ZSDataSetWdgt.h"
#include "ZSDataGUI/ZSDataSetModel.h"
//#include "ZSDataGUI/ZSDataSetModelNode.h"
//#include "ZSDataGUI/ZSDataSetNodeContentWdgt.h"
//#include "ZSDataGUI/ZSDataSetSchemaDlg.h"
//#include "ZSDataGUI/ZSDataParameterSchemaDlg.h"
//#include "ZSDataGUI/ZSDataTableSchemaDlg.h"
#include "ZSData/ZSDatabase.h"
//#include "ZSData/ZSDatabaseClient.h"
//#include "ZSData/ZSDataEnumeration.h"
#include "ZSData/ZSDataSet.h"
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
class CWdgtDataSet : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtDataSet::CWdgtDataSet( const QString& i_strDSName, QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_strDSName(i_strDSName),
    m_pDS(nullptr),
    m_pDb(nullptr),
    m_pDbClt(nullptr),
    m_pDSModel(nullptr),
    //m_pDSModelNodeSelected(nullptr),
    m_pLyt(nullptr),
    m_pSplitter(nullptr),
    m_pWdgtTreeView(nullptr),
    m_pLytTreeView(nullptr),
    m_pLytToolBarEdit(nullptr),
    m_pBtnAddGroup(nullptr),
    m_pBtnAddEnumeration(nullptr),
    m_pBtnAddTable(nullptr),
    m_pBtnAddParameter(nullptr),
    m_pBtnDelete(nullptr),
    m_pBtnEdit(nullptr),
    m_pTreeView(nullptr),
    //m_pWdgtNodeContent(nullptr),
    m_pMnuContextEdit(nullptr),
    //m_pDlgDSEditSchema(nullptr),
    //m_pDlgDPEditSchema(nullptr),
    //m_pDlgDTEditSchema(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(m_strDSName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(nameSpace(), className(), objectName());

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "DS: " + i_strDSName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strAddTrcInfo );

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    init();

} // ctor

////------------------------------------------------------------------------------
//CWdgtDataSet::CWdgtDataSet( CDataSet* i_pDS, QWidget* i_pWdgtParent ) :
////------------------------------------------------------------------------------
//    QWidget(i_pWdgtParent),
//    m_strDSName(),
//    m_pDS(i_pDS),
//    m_pDb(nullptr),
//    m_pDbClt(nullptr),
//    m_pDSModel(nullptr),
//    //m_pDSModelNodeSelected(nullptr),
//    m_pLyt(nullptr),
//    m_pSplitter(nullptr),
//    m_pWdgtTreeView(nullptr),
//    m_pLytTreeView(nullptr),
//    m_pLytToolBarEdit(nullptr),
//    m_pBtnAddGroup(nullptr),
//    m_pBtnAddEnumeration(nullptr),
//    m_pBtnAddTable(nullptr),
//    m_pBtnAddParameter(nullptr),
//    m_pBtnDelete(nullptr),
//    m_pBtnEdit(nullptr),
//    m_pTreeView(nullptr),
//    m_pWdgtNodeContent(nullptr),
//    m_pMnuContextEdit(nullptr),
//    m_pDlgDSEditSchema(nullptr),
//    m_pDlgDPEditSchema(nullptr),
//    m_pDlgDTEditSchema(nullptr),
//    m_pTrcAdminObj(nullptr)
//{
//    if( m_pDS != nullptr )
//    {
//        m_strDSName = m_pDS->getKey();
//    }
//
//    setObjectName(m_strDSName);
//
//    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(nameSpace(), className(), objectName());
//
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = "DS: " + QString( i_pDS == nullptr ? "nullptr" : i_pDS->objectName() );
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "ctor",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    m_pLyt = new QVBoxLayout();
//    setLayout(m_pLyt);
//
//    init();
//
//} // ctor
//
////------------------------------------------------------------------------------
//CWdgtDataSet::CWdgtDataSet( CDb* i_pDb, QWidget* i_pWdgtParent ) :
////------------------------------------------------------------------------------
//    QWidget(i_pWdgtParent),
//    m_strDSName(),
//    m_pDS(nullptr),
//    m_pDb(i_pDb),
//    m_pDbClt(nullptr),
//    m_pDSModel(nullptr),
//    m_pDSModelNodeSelected(nullptr),
//    m_pLyt(nullptr),
//    m_pSplitter(nullptr),
//    m_pWdgtTreeView(nullptr),
//    m_pLytTreeView(nullptr),
//    m_pLytToolBarEdit(nullptr),
//    m_pBtnAddGroup(nullptr),
//    m_pBtnAddEnumeration(nullptr),
//    m_pBtnAddTable(nullptr),
//    m_pBtnAddParameter(nullptr),
//    m_pBtnDelete(nullptr),
//    m_pBtnEdit(nullptr),
//    m_pTreeView(nullptr),
//    m_pWdgtNodeContent(nullptr),
//    m_pMnuContextEdit(nullptr),
//    m_pDlgDSEditSchema(nullptr),
//    m_pDlgDPEditSchema(nullptr),
//    m_pDlgDTEditSchema(nullptr),
//    m_pTrcAdminObj(nullptr)
//{
//    if( m_pDb != nullptr )
//    {
//        m_pDS = m_pDb->getDataSet();
//    }
//
//    if( m_pDS != nullptr )
//    {
//        m_strDSName = m_pDS->getKey();
//    }
//
//    setObjectName(m_strDSName);
//
//    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(nameSpace(), className(), objectName());
//
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = "Db: " + QString( i_pDb == nullptr ? "nullptr" : i_pDb->objectName() );
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "ctor",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    m_pLyt = new QVBoxLayout();
//    setLayout(m_pLyt);
//
//    init();
//
//} // ctor
//
////------------------------------------------------------------------------------
//CWdgtDataSet::CWdgtDataSet( CDbClient* i_pDbClt, QWidget* i_pWdgtParent ) :
////------------------------------------------------------------------------------
//    QWidget(i_pWdgtParent),
//    m_strDSName(),
//    m_pDS(nullptr),
//    m_pDb(nullptr),
//    m_pDbClt(i_pDbClt),
//    m_pDSModel(nullptr),
//    m_pDSModelNodeSelected(nullptr),
//    m_pLyt(nullptr),
//    m_pSplitter(nullptr),
//    m_pWdgtTreeView(nullptr),
//    m_pLytTreeView(nullptr),
//    m_pLytToolBarEdit(nullptr),
//    m_pBtnAddGroup(nullptr),
//    m_pBtnAddEnumeration(nullptr),
//    m_pBtnAddTable(nullptr),
//    m_pBtnAddParameter(nullptr),
//    m_pBtnDelete(nullptr),
//    m_pBtnEdit(nullptr),
//    m_pTreeView(nullptr),
//    m_pWdgtNodeContent(nullptr),
//    m_pMnuContextEdit(nullptr),
//    m_pDlgDSEditSchema(nullptr),
//    m_pDlgDPEditSchema(nullptr),
//    m_pDlgDTEditSchema(nullptr),
//    m_pTrcAdminObj(nullptr)
//{
//    if( m_pDbClt != nullptr )
//    {
//        m_pDS = m_pDbClt->getDataSet();
//    }
//
//    if( m_pDS != nullptr )
//    {
//        m_strDSName = m_pDS->getKey();
//    }
//
//    setObjectName(m_strDSName);
//
//    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(nameSpace(), className(), objectName());
//
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = "DbClt: " + QString( i_pDbClt == nullptr ? "nullptr" : i_pDbClt->objectName() );
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "ctor",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    m_pLyt = new QVBoxLayout();
//    setLayout(m_pLyt);
//
//    init();
//
//} // ctor

//------------------------------------------------------------------------------
CWdgtDataSet::~CWdgtDataSet()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    QSettings settings;

    settings.setValue( objectName()+"/Geometry", saveGeometry() );

    QList<int> listSizes = m_pSplitter->sizes();

    for( int idx = 0; idx < listSizes.count(); idx++ )
    {
        settings.setValue( objectName() + "/SplitterWidth" + QString::number(idx), listSizes[idx] );
    }

    if( m_pTreeView != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pTreeView,
            /* szSignal     */ SIGNAL( expanded(const QModelIndex&) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onTreeViewExpanded(const QModelIndex&) ) );

        QObject::disconnect(
            /* pObjSender   */ m_pTreeView,
            /* szSignal     */ SIGNAL( doubleClicked(const QModelIndex&) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onTreeViewDoubleClicked(const QModelIndex&) ) );

        QItemSelectionModel* pSelModel = m_pTreeView->selectionModel();

        if( pSelModel != nullptr )
        {
            QObject::disconnect(
                /* pObjSender   */ pSelModel,
                /* szSignal     */ SIGNAL( currentChanged(const QModelIndex&,const QModelIndex&) ),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT( onTreeViewCurrentChanged(const QModelIndex&,const QModelIndex&) ) );
        }
    } // if( m_pTreeView != nullptr )

    //try
    //{
    //    delete m_pDlgDSEditSchema;
    //}
    //catch(...)
    //{
    //}
    //m_pDlgDSEditSchema = nullptr;

    //try
    //{
    //    delete m_pDlgDPEditSchema;
    //}
    //catch(...)
    //{
    //}
    //m_pDlgDPEditSchema = nullptr;

    //try
    //{
    //    delete m_pDlgDTEditSchema;
    //}
    //catch(...)
    //{
    //}
    //m_pDlgDTEditSchema = nullptr;

    try
    {
        delete m_pDSModel;
    }
    catch(...)
    {
    }
    m_pDSModel = nullptr;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pDS = nullptr;
    m_pDbClt = nullptr;
    m_pDSModel = nullptr;
    //m_pDSModelNodeSelected = nullptr;
    m_pLyt = nullptr;
    m_pSplitter = nullptr;
    m_pWdgtTreeView = nullptr;
    m_pLytTreeView = nullptr;
    m_pLytToolBarEdit = nullptr;
    m_pBtnAddGroup = nullptr;
    m_pBtnAddEnumeration = nullptr;
    m_pBtnAddTable = nullptr;
    m_pBtnAddParameter = nullptr;
    m_pBtnDelete = nullptr;
    m_pBtnEdit = nullptr;
    m_pTreeView = nullptr;
    //m_pWdgtNodeContent = nullptr;
    //m_pDlgDSEditSchema = nullptr;
    //m_pDlgDPEditSchema = nullptr;
    //m_pDlgDTEditSchema = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDataSet::init()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "init",
        /* strAddInfo   */ "" );

    bool bFirstCall = false;

    if( m_pSplitter == nullptr )
    {
        bFirstCall = true;

        m_pSplitter = new QSplitter(Qt::Horizontal);
        m_pSplitter->setChildrenCollapsible(false);
        m_pLyt->addWidget(m_pSplitter);

        // Tree view
        //============

        m_pWdgtTreeView = new QWidget();
        m_pLytTreeView = new QVBoxLayout();
        m_pLytTreeView->setContentsMargins(0,0,0,0); // left, top, right, bottom
        m_pWdgtTreeView->setLayout(m_pLytTreeView);
        m_pSplitter->addWidget(m_pWdgtTreeView);

        // <ToolBar> Edit
        //---------------

        m_pLytToolBarEdit = new QHBoxLayout();
        m_pLytTreeView->addLayout(m_pLytToolBarEdit);

        // <Button> Add Group
        //-------------------

        QIcon   iconAddGroup;
        QPixmap pxmAddGroup(":/ZS/Data/Menu/AddGroup");

        pxmAddGroup.setMask(pxmAddGroup.createHeuristicMask());

        iconAddGroup.addPixmap(pxmAddGroup);

        m_pBtnAddGroup = new QPushButton();
        m_pBtnAddGroup->setIcon(iconAddGroup);
        m_pBtnAddGroup->setEnabled(false);
        m_pBtnAddGroup->setToolTip("Add Group");

        m_pLytToolBarEdit->addWidget(m_pBtnAddGroup);

        if( !QObject::connect(
            /* pObjSender   */ m_pBtnAddGroup,
            /* szSignal     */ SIGNAL(clicked(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onBtnAddGroupClicked(bool)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        // <Button> Add Enumeration
        //-------------------------

        QIcon   iconAddEnumeration;
        QPixmap pxmAddEnumeration(":/ZS/Data/Menu/AddEnumeration");

        pxmAddEnumeration.setMask(pxmAddEnumeration.createHeuristicMask());

        iconAddEnumeration.addPixmap(pxmAddEnumeration);

        m_pBtnAddEnumeration = new QPushButton();
        m_pBtnAddEnumeration->setIcon(iconAddEnumeration);
        m_pBtnAddEnumeration->setEnabled(false);
        m_pBtnAddEnumeration->setToolTip( "Add Enumeration" );
        m_pLytToolBarEdit->addWidget(m_pBtnAddEnumeration);

        if( !QObject::connect(
            /* pObjSender   */ m_pBtnAddEnumeration,
            /* szSignal     */ SIGNAL(clicked(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onBtnAddEnumerationClicked(bool)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        // <Button> Add Table
        //-------------------

        QIcon   iconAddTable;
        QPixmap pxmAddTable(":/ZS/Data/Menu/AddTable");

        pxmAddTable.setMask(pxmAddTable.createHeuristicMask());

        iconAddTable.addPixmap(pxmAddTable);

        m_pBtnAddTable = new QPushButton();
        m_pBtnAddTable->setIcon(iconAddTable);
        m_pBtnAddTable->setEnabled(false);
        m_pBtnAddTable->setToolTip( "Add Table" );
        m_pLytToolBarEdit->addWidget(m_pBtnAddTable);

        if( !QObject::connect(
            /* pObjSender   */ m_pBtnAddTable,
            /* szSignal     */ SIGNAL(clicked(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onBtnAddTableClicked(bool)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        // <Button> Add Parameter
        //-----------------------

        QIcon   iconAddParameter;
        QPixmap pxmAddParameter(":/ZS/Data/Menu/AddParameter");

        pxmAddParameter.setMask(pxmAddParameter.createHeuristicMask());

        iconAddParameter.addPixmap(pxmAddParameter);

        m_pBtnAddParameter = new QPushButton();
        m_pBtnAddParameter->setIcon(iconAddParameter);
        m_pBtnAddParameter->setEnabled(false);
        m_pBtnAddParameter->setToolTip( "Add Parameter" );
        m_pLytToolBarEdit->addWidget(m_pBtnAddParameter);

        if( !QObject::connect(
            /* pObjSender   */ m_pBtnAddParameter,
            /* szSignal     */ SIGNAL(clicked(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onBtnAddParameterClicked(bool)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        // <Button> Delete
        //----------------

        QIcon   iconDelete;
        QPixmap pxmDelete(":/ZS/Button/ButtonDelete16x16.bmp");

        pxmDelete.setMask(pxmDelete.createHeuristicMask());

        iconDelete.addPixmap(pxmDelete);

        m_pBtnDelete = new QPushButton();
        m_pBtnDelete->setIcon(iconDelete);
        m_pBtnDelete->setEnabled(false);
        m_pLytToolBarEdit->addWidget(m_pBtnDelete);

        if( !QObject::connect(
            /* pObjSender   */ m_pBtnDelete,
            /* szSignal     */ SIGNAL(clicked(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onBtnDeleteClicked(bool)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        // <Button> Edit
        //--------------

        QIcon   iconEdit;
        QPixmap pxmEdit(":/ZS/Menu/MenuFileEdit16x16.bmp");

        pxmEdit.setMask(pxmEdit.createHeuristicMask());

        iconEdit.addPixmap(pxmEdit);

        m_pBtnEdit = new QPushButton();
        m_pBtnEdit->setIcon(iconEdit);
        m_pBtnEdit->setEnabled(false);
        m_pLytToolBarEdit->addWidget(m_pBtnEdit);

        if( !QObject::connect(
            /* pObjSender   */ m_pBtnEdit,
            /* szSignal     */ SIGNAL(clicked(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onBtnEditClicked(bool)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        // <Stretch> on right side of buttons
        //------------------------------------

        m_pLytToolBarEdit->addStretch();

        // <TreeView> Data Set Model
        //--------------------------

        m_pTreeView = new QTreeView(this);
        m_pLytTreeView->addWidget(m_pTreeView);

        m_pTreeView->setContextMenuPolicy(Qt::NoContextMenu);
        m_pTreeView->setSelectionBehavior(QAbstractItemView::SelectItems);
        m_pTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
        m_pTreeView->setDragEnabled(true);
        m_pTreeView->setAcceptDrops(true);
        m_pTreeView->setDropIndicatorShown(true);
        m_pTreeView->setDragDropMode(QAbstractItemView::InternalMove);

        if( !QObject::connect(
            /* pObjSender   */ m_pTreeView,
            /* szSignal     */ SIGNAL( expanded(const QModelIndex&) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onTreeViewExpanded(const QModelIndex&) ) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        if( !QObject::connect(
            /* pObjSender   */ m_pTreeView,
            /* szSignal     */ SIGNAL( doubleClicked(const QModelIndex&) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onTreeViewDoubleClicked(const QModelIndex&) ) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

    } // if( m_pSplitter == nullptr )

    // <TreeView> Data Set Model
    //--------------------------

    delete m_pDSModel;
    m_pDSModel = nullptr;

    if( m_pDS != nullptr )
    {
        m_pDSModel = new CDataSetModel( m_pDS->getName(), m_pDS );

        m_pTreeView->setModel(m_pDSModel);

        if( !QObject::connect(
            /* pObjSender   */ m_pTreeView->selectionModel(),
            /* szSignal     */ SIGNAL( currentChanged(const QModelIndex&,const QModelIndex&) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onTreeViewCurrentChanged(const QModelIndex&,const QModelIndex&) ) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

    } // if( m_pDS != nullptr )

    // Content of selected node
    //=========================

    //delete m_pWdgtNodeContent;
    //m_pWdgtNodeContent = nullptr;

    //if( m_pDS != nullptr )
    //{
    //    if( m_pDb != nullptr )
    //    {
    //        m_pWdgtNodeContent = new CWdgtDataSetNodeContent(m_pDb);
    //    }
    //    else if( m_pDbClt != nullptr )
    //    {
    //        m_pWdgtNodeContent = new CWdgtDataSetNodeContent(m_pDbClt);
    //    }
    //    else if( m_pDS != nullptr )
    //    {
    //        m_pWdgtNodeContent = new CWdgtDataSetNodeContent(m_pDS);
    //    }
    //    else
    //    {
    //        m_pWdgtNodeContent = new CWdgtDataSetNodeContent(m_strDSName);
    //    }

    //    m_pSplitter->addWidget(m_pWdgtNodeContent);

    //} // if( m_pDS != nullptr )

    // Restore geometry of widget
    //===========================

    if( bFirstCall )
    {
        QSettings settings;

        restoreGeometry( settings.value(objectName()+"/Geometry").toByteArray() );

        QList<int> listSizes = m_pSplitter->sizes();

        for( int idx = 0; idx < listSizes.count(); idx++ )
        {
            listSizes[idx] = settings.value( objectName() + "/SplitterWidth" + QString::number(idx), 250 ).toInt();
        }
        m_pSplitter->setSizes(listSizes);

    } // if( bFirstCall )

} // init

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDataSet::setDataSet( CDataSet* i_pDS )
//------------------------------------------------------------------------------
{
    if( m_pDS != i_pDS )
    {
        m_pDS = i_pDS;
        m_pDb = nullptr;
        m_pDbClt = nullptr;

        init();

    } // if( m_pDS != i_pDS )

} // setDataSet

////------------------------------------------------------------------------------
//void CWdgtDataSet::setDb( CDb* i_pDb )
////------------------------------------------------------------------------------
//{
//    if( m_pDb != i_pDb )
//    {
//        m_pDS = nullptr;
//        m_pDb = i_pDb;
//        m_pDbClt = nullptr;
//
//        if( m_pDb != nullptr )
//        {
//            m_pDS = m_pDb->getDataSet();
//        }
//
//        init();
//
//    } // if( m_pDb != i_pDb )
//
//} // setDb

////------------------------------------------------------------------------------
//void CWdgtDataSet::setDbClient( CDbClient* i_pDbClt )
////------------------------------------------------------------------------------
//{
//    if( m_pDbClt != i_pDbClt )
//    {
//        m_pDS = nullptr;
//        m_pDb = nullptr;
//        m_pDbClt = i_pDbClt;
//
//        if( m_pDbClt != nullptr )
//        {
//            m_pDS = m_pDbClt->getDataSet();
//        }
//
//        init();
//
//    } // if( m_pDbClt != i_pDbClt )
//
//} // setDbClient

/*==============================================================================
protected slots: // connected to the signals of the edit buttons
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDataSet::onBtnAddGroupClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnAddGroupClicked",
        /* strAddInfo   */ "" );

    //if( m_pDSModelNodeSelected != nullptr )
    //{
    //    EDataSetObjType dsObjType = m_pDSModelNodeSelected->getObjType();

    //    if( dsObjType == EDataSetObjTypeDataSet || dsObjType == EDataSetObjTypeGroup )
    //    {
    //        CDataSetObjGroup* pDSObjGroup = dynamic_cast<CDataSetObjGroup*>(m_pDSModelNodeSelected->getDataSetObject());

    //        if( pDSObjGroup != nullptr )
    //        {
    //            CDataSet* pDS = pDSObjGroup->getDataSet();

    //            if( pDS != nullptr )
    //            {
    //                QString strName = pDSObjGroup->getUniqueChildObjName(EDataSetObjTypeGroup);
    //                pDSObjGroup->addChildGroup(strName);
    //            }
    //        }
    //    } // if( dsObjType == EDataSetObjTypeDataSet || dsObjType == EDataSetObjTypeGroup )
    //} // if( m_pDSModelNodeSelected != nullptr )

} // onBtnAddGroupClicked

//------------------------------------------------------------------------------
void CWdgtDataSet::onBtnAddEnumerationClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnAddEnumerationClicked",
        /* strAddInfo   */ "" );

    //if( m_pDSModelNodeSelected != nullptr )
    //{
    //    EDataSetObjType dsObjType = m_pDSModelNodeSelected->getObjType();

    //    if( dsObjType == EDataSetObjTypeDataSet || dsObjType == EDataSetObjTypeGroup )
    //    {
    //        CDataSetObjGroup* pDSObjGroup = dynamic_cast<CDataSetObjGroup*>(m_pDSModelNodeSelected->getDataSetObject());

    //        if( pDSObjGroup != nullptr )
    //        {
    //            CDataSet* pDS = pDSObjGroup->getDataSet();

    //            if( pDS != nullptr )
    //            {
    //                QString strName = pDSObjGroup->getUniqueChildObjName(EDataSetObjTypeEnumeration);
    //                pDSObjGroup->addChildEnumeration(strName);
    //            }
    //        }
    //    } // if( dsObjType == EDataSetObjTypeDataSet || dsObjType == EDataSetObjTypeGroup )
    //} // if( m_pDSModelNodeSelected != nullptr )

} // onBtnAddEnumerationClicked

//------------------------------------------------------------------------------
void CWdgtDataSet::onBtnAddTableClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnAddTableClicked",
        /* strAddInfo   */ "" );

    //if( m_pDSModelNodeSelected != nullptr )
    //{
    //    EDataSetObjType dsObjType = m_pDSModelNodeSelected->getObjType();

    //    if( dsObjType == EDataSetObjTypeDataSet || dsObjType == EDataSetObjTypeGroup )
    //    {
    //        CDataSetObjGroup* pDSObjGroup = dynamic_cast<CDataSetObjGroup*>(m_pDSModelNodeSelected->getDataSetObject());

    //        if( pDSObjGroup != nullptr )
    //        {
    //            CDataSet* pDS = pDSObjGroup->getDataSet();

    //            if( pDS != nullptr )
    //            {
    //                QString strName = pDSObjGroup->getUniqueChildObjName(EDataSetObjTypeTable);
    //                pDSObjGroup->addChildTable(strName);
    //            }
    //        }
    //    } // if( dsObjType == EDataSetObjTypeDataSet || dsObjType == EDataSetObjTypeGroup )
    //} // if( m_pDSModelNodeSelected != nullptr )

} // onBtnAddTableClicked

//------------------------------------------------------------------------------
void CWdgtDataSet::onBtnAddParameterClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnAddParameterClicked",
        /* strAddInfo   */ "" );

    //if( m_pDSModelNodeSelected != nullptr )
    //{
    //    EDataSetObjType dsObjType = m_pDSModelNodeSelected->getObjType();

    //    if( dsObjType == EDataSetObjTypeDataSet || dsObjType == EDataSetObjTypeGroup )
    //    {
    //        CDataSetObjGroup* pDSObjGroup = dynamic_cast<CDataSetObjGroup*>(m_pDSModelNodeSelected->getDataSetObject());

    //        if( pDSObjGroup != nullptr )
    //        {
    //            CDataSet* pDS = pDSObjGroup->getDataSet();

    //            if( pDS != nullptr )
    //            {
    //                QString strName = pDSObjGroup->getUniqueChildObjName(EDataSetObjTypeParameter);
    //                pDSObjGroup->addChildParameter(strName,EDimensionType::SingleValue);
    //            }
    //        }
    //    } // if( dsObjType == EDataSetObjTypeDataSet || dsObjType == EDataSetObjTypeGroup )
    //} // if( m_pDSModelNodeSelected != nullptr )

} // onBtnAddParameterClicked

//------------------------------------------------------------------------------
void CWdgtDataSet::onBtnDeleteClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnDeleteClicked",
        /* strAddInfo   */ "" );

    //if( m_pDSModelNodeSelected != nullptr )
    //{
    //    switch( m_pDSModelNodeSelected->getObjType() )
    //    {
    //        case EDataSetObjTypeDataSet:
    //        {
    //            CDataSet* pDS = dynamic_cast<CDataSet*>(m_pDSModelNodeSelected->getDataSetObject());
    //            if( pDS != nullptr && pDS->isDeletable() )
    //            {
    //                delete pDS;
    //                pDS = nullptr;
    //            }
    //            break;
    //        }
    //        case EDataSetObjTypeGroup:
    //        {
    //            CDataSetObjGroup* pDSObjGroup = dynamic_cast<CDataSetObjGroup*>(m_pDSModelNodeSelected->getDataSetObject());
    //            if( pDSObjGroup != nullptr && pDSObjGroup->isDeletable() )
    //            {
    //                delete pDSObjGroup;
    //                pDSObjGroup = nullptr;
    //            }
    //            break;
    //        }
    //        case EDataSetObjTypeEnumeration:
    //        {
    //            CDataEnumeration* pDE = dynamic_cast<CDataEnumeration*>(m_pDSModelNodeSelected->getDataSetObject());
    //            if( pDE != nullptr && pDE->isDeletable() )
    //            {
    //                delete pDE;
    //                pDE = nullptr;
    //            }
    //            break;
    //        }
    //        case EDataSetObjTypeParameter:
    //        {
    //            CDataParameter* pDP = dynamic_cast<CDataParameter*>(m_pDSModelNodeSelected->getDataSetObject());
    //            if( pDP != nullptr && pDP->isDeletable() )
    //            {
    //                delete pDP;
    //                pDP = nullptr;
    //            }
    //            break;
    //        }
    //        case EDataSetObjTypeTable:
    //        {
    //            CDataTable* pDT = dynamic_cast<CDataTable*>(m_pDSModelNodeSelected->getDataSetObject());
    //            if( pDT != nullptr && pDT->isDeletable() )
    //            {
    //                delete pDT;
    //                pDT = nullptr;
    //            }
    //            break;
    //        }
    //        default:
    //        {
    //            break;
    //        }
    //    } // switch( m_pDSModelNodeSelected->getObjType() )

    //} // if( m_pDSModelNodeSelected != nullptr )

} // onBtnDeleteClicked

//------------------------------------------------------------------------------
void CWdgtDataSet::onBtnEditClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnEditClicked",
        /* strAddInfo   */ "" );

    //if( m_pDSModelNodeSelected != nullptr )
    //{
    //    switch( m_pDSModelNodeSelected->getObjType() )
    //    {
    //        case EDataSetObjTypeDataSet:
    //        {
    //            if( m_pDlgDSEditSchema == nullptr )
    //            {
    //                if( m_pDb != nullptr )
    //                {
    //                    m_pDlgDSEditSchema = new CDlgDataSetSchema( m_pDb, EMode::Edit );
    //                }
    //                else if( m_pDbClt != nullptr )
    //                {
    //                    m_pDlgDSEditSchema = new CDlgDataSetSchema( m_pDbClt, EMode::Edit );
    //                }
    //                else if( m_pDS != nullptr )
    //                {
    //                    m_pDlgDSEditSchema = new CDlgDataSetSchema( m_pDS, EMode::Edit );
    //                }
    //                else
    //                {
    //                    m_pDlgDSEditSchema = new CDlgDataSetSchema( m_strDSName, EMode::Edit );
    //                }
    //            }
    //            m_pDlgDSEditSchema->exec();
    //            break;
    //        }
    //        case EDataSetObjTypeGroup:
    //        {
    //            break;
    //        }
    //        case EDataSetObjTypeEnumeration:
    //        {
    //            break;
    //        }
    //        case EDataSetObjTypeParameter:
    //        {
    //            if( m_pDlgDPEditSchema == nullptr )
    //            {
    //                if( m_pDb != nullptr )
    //                {
    //                    m_pDlgDPEditSchema = new CDlgDataParameterSchema( m_pDb, EMode::Edit );
    //                }
    //                else if( m_pDbClt != nullptr )
    //                {
    //                    m_pDlgDPEditSchema = new CDlgDataParameterSchema( m_pDbClt, EMode::Edit );
    //                }
    //                else if( m_pDS != nullptr )
    //                {
    //                    m_pDlgDPEditSchema = new CDlgDataParameterSchema( m_pDS, EMode::Edit );
    //                }
    //                else
    //                {
    //                    m_pDlgDPEditSchema = new CDlgDataParameterSchema( m_strDSName, EMode::Edit );
    //                }
    //            }
    //            m_pDlgDPEditSchema->setDataParameter(m_pDSModelNodeSelected->toParameter());
    //            m_pDlgDPEditSchema->exec();
    //            break;
    //        }
    //        case EDataSetObjTypeTable:
    //        {
    //            if( m_pDlgDTEditSchema == nullptr )
    //            {
    //                if( m_pDb != nullptr )
    //                {
    //                    m_pDlgDTEditSchema = new CDlgDataTableSchema( m_pDb, EMode::Edit );
    //                }
    //                else if( m_pDbClt != nullptr )
    //                {
    //                    m_pDlgDTEditSchema = new CDlgDataTableSchema( m_pDbClt, EMode::Edit );
    //                }
    //                else if( m_pDS != nullptr )
    //                {
    //                    m_pDlgDTEditSchema = new CDlgDataTableSchema( m_pDS, EMode::Edit );
    //                }
    //                else
    //                {
    //                    m_pDlgDTEditSchema = new CDlgDataTableSchema( m_strDSName, EMode::Edit );
    //                }
    //            }
    //            m_pDlgDTEditSchema->setDataTable(m_pDSModelNodeSelected->toTable());
    //            m_pDlgDTEditSchema->exec();
    //            break;
    //        }
    //        default:
    //        {
    //            break;
    //        }
    //    } // switch( m_pDSModelNodeSelected->getObjType() )
    //} // if( m_pDSModelNodeSelected != nullptr )

} // onBtnEditClicked

/*==============================================================================
protected slots: // connected to the signals of the tree view
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDataSet::onTreeViewContextMenuRequested( const QPoint& /*i_pt*/ )
//------------------------------------------------------------------------------
{
    if( m_pMnuContextEdit != nullptr )
    {
        m_pMnuContextEdit->exec(QCursor::pos());
    }

} // onTreeViewContextMenuRequested

//------------------------------------------------------------------------------
void CWdgtDataSet::onTreeViewExpanded( const QModelIndex& i_iModelIdx )
//------------------------------------------------------------------------------
{
    if( i_iModelIdx.isValid() )
    {
        m_pTreeView->resizeColumnToContents(i_iModelIdx.column());
    }

} // onTreeViewExpanded

//------------------------------------------------------------------------------
void CWdgtDataSet::onTreeViewDoubleClicked( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    if( i_modelIdx.isValid() && i_modelIdx.internalPointer() != nullptr )
    {
        //CDataSetModelNode* pDSNode = static_cast<CDataSetModelNode*>(i_modelIdx.internalPointer());

        //if( pDSNode != nullptr )
        //{
        //    //CDlgDataSetObjectSchema* pDlgDSObjSchema = CDlgDataSetObjectSchema::GetInstance();

        //    //if( pDlgDSObjSchema != nullptr )
        //    //{
        //    //    pDlgDSObjSchema->setDataSetObject( pDSNode->getDataSetObject() );
        //    //    pDlgDSObjSchema->show();
        //    //    pDlgDSObjSchema->activateWindow();
        //    //    pDlgDSObjSchema->raise();
        //    //}
        //}
    }

} // onTreeViewDoubleClicked

//------------------------------------------------------------------------------
void CWdgtDataSet::onTreeViewCurrentChanged(
    const QModelIndex& i_modelIdxCurr,
    const QModelIndex& /*i_modelIdxPrev*/ )
//------------------------------------------------------------------------------
{
    if( i_modelIdxCurr.isValid() && i_modelIdxCurr.internalPointer() != nullptr )
    {
        //CDataSetModelNode* pDSNode = static_cast<CDataSetModelNode*>(i_modelIdxCurr.internalPointer());
        //CDataSetObject*    pDSObj = pDSNode->getDataSetObject();

        //if( pDSNode != m_pDSModelNodeSelected )
        //{
        //    m_pDSModelNodeSelected = pDSNode;

        //    if( m_pDSModelNodeSelected == nullptr || pDSObj == nullptr )
        //    {
        //        if( m_pBtnAddGroup != nullptr )
        //        {
        //            m_pBtnAddGroup->setEnabled(false);
        //        }
        //        if( m_pBtnAddEnumeration != nullptr )
        //        {
        //            m_pBtnAddEnumeration->setEnabled(false);
        //        }
        //        if( m_pBtnAddTable != nullptr )
        //        {
        //            m_pBtnAddTable->setEnabled(false);
        //        }
        //        if( m_pBtnAddParameter != nullptr )
        //        {
        //            m_pBtnAddParameter->setEnabled(false);
        //        }
        //        if( m_pBtnDelete != nullptr )
        //        {
        //            m_pBtnDelete->setEnabled(false);
        //        }
        //        if( m_pBtnEdit != nullptr )
        //        {
        //            m_pBtnEdit->setEnabled(false);
        //        }

        //        if( m_pTreeView != nullptr && m_pTreeView->contextMenuPolicy() != Qt::NoContextMenu )
        //        {
        //            m_pTreeView->setContextMenuPolicy(Qt::NoContextMenu);

        //            QObject::disconnect(
        //                /* pObjSender   */ m_pTreeView,
        //                /* szSignal     */ SIGNAL(customContextMenuRequested(const QPoint&)),
        //                /* pObjReceiver */ this,
        //                /* szSlot       */ SLOT(onTreeViewContextMenuRequested(const QPoint&)) );
        //        }
        //    }

        //    else // if( m_pDSModelNodeSelected != nullptr && pDSObj != nullptr )
        //    {
        //        if( m_pWdgtNodeContent != nullptr )
        //        {
        //            m_pWdgtNodeContent->setNode(m_pDSModelNodeSelected);
        //        }

        //        switch( m_pDSModelNodeSelected->getObjType() )
        //        {
        //            case EDataSetObjTypeDataSet:
        //            case EDataSetObjTypeGroup:
        //            {
        //                if( m_pBtnAddGroup != nullptr )
        //                {
        //                    m_pBtnAddGroup->setEnabled(true);
        //                }
        //                if( m_pBtnAddEnumeration != nullptr )
        //                {
        //                    m_pBtnAddEnumeration->setEnabled(true);
        //                }
        //                if( m_pBtnAddTable != nullptr )
        //                {
        //                    m_pBtnAddTable->setEnabled(true);
        //                }
        //                if( m_pBtnAddParameter != nullptr )
        //                {
        //                    m_pBtnAddParameter->setEnabled(true);
        //                }
        //                if( m_pBtnDelete != nullptr )
        //                {
        //                    m_pBtnDelete->setToolTip( "Delete " + pDSObj->getObjTypeStr() );

        //                    if( pDSObj->isDeletable() )
        //                    {
        //                        m_pBtnDelete->setEnabled(true);
        //                    }
        //                    else
        //                    {
        //                        m_pBtnDelete->setEnabled(false);
        //                    }
        //                }
        //                if( m_pBtnEdit != nullptr )
        //                {
        //                    m_pBtnEdit->setToolTip("");
        //                    m_pBtnEdit->setEnabled(false);
        //                }
        //                break;
        //            } // case EDataSetObjTypeDataSet || EDataSetObjTypeGroup

        //            case EDataSetObjTypeEnumeration:
        //            case EDataSetObjTypeParameter:
        //            case EDataSetObjTypeTable:
        //            {
        //                if( m_pBtnAddGroup != nullptr )
        //                {
        //                    m_pBtnAddGroup->setEnabled(false);
        //                }
        //                if( m_pBtnAddEnumeration != nullptr )
        //                {
        //                    m_pBtnAddEnumeration->setEnabled(false);
        //                }
        //                if( m_pBtnAddTable != nullptr )
        //                {
        //                    m_pBtnAddTable->setEnabled(false);
        //                }
        //                if( m_pBtnAddParameter != nullptr )
        //                {
        //                    m_pBtnAddParameter->setEnabled(false);
        //                }
        //                if( m_pBtnDelete != nullptr )
        //                {
        //                    m_pBtnDelete->setToolTip( "Delete " + pDSObj->getObjTypeStr() );

        //                    if( pDSObj->isDeletable() )
        //                    {
        //                        m_pBtnDelete->setEnabled(true);
        //                    }
        //                    else
        //                    {
        //                        m_pBtnDelete->setEnabled(false);
        //                    }
        //                }
        //                if( m_pBtnEdit != nullptr )
        //                {
        //                    m_pBtnEdit->setToolTip( "Edit "+ pDSObj->getObjTypeStr() );
        //                    m_pBtnEdit->setEnabled(true);
        //                }
        //                break;
        //            } // case EDataSetObjTypeEnumeration || EDataSetObjTypeParameter || EDataSetObjTypeTable

        //            default:
        //            {
        //                if( m_pBtnAddGroup != nullptr )
        //                {
        //                    m_pBtnAddGroup->setEnabled(false);
        //                }
        //                if( m_pBtnAddEnumeration != nullptr )
        //                {
        //                    m_pBtnAddEnumeration->setEnabled(false);
        //                }
        //                if( m_pBtnAddTable != nullptr )
        //                {
        //                    m_pBtnAddTable->setEnabled(false);
        //                }
        //                if( m_pBtnAddParameter != nullptr )
        //                {
        //                    m_pBtnAddParameter->setEnabled(false);
        //                }
        //                if( m_pBtnDelete != nullptr )
        //                {
        //                    m_pBtnDelete->setToolTip("");
        //                    m_pBtnDelete->setEnabled(false);
        //                }
        //                if( m_pBtnEdit != nullptr )
        //                {
        //                    m_pBtnEdit->setToolTip("");
        //                    m_pBtnEdit->setEnabled(false);
        //                }

        //                if( m_pTreeView != nullptr && m_pTreeView->contextMenuPolicy() != Qt::NoContextMenu )
        //                {
        //                    m_pTreeView->setContextMenuPolicy(Qt::NoContextMenu);

        //                    QObject::disconnect(
        //                        /* pObjSender   */ m_pTreeView,
        //                        /* szSignal     */ SIGNAL(customContextMenuRequested(const QPoint&)),
        //                        /* pObjReceiver */ this,
        //                        /* szSlot       */ SLOT(onTreeViewContextMenuRequested(const QPoint&)) );
        //                }
        //                break;
        //            }
        //        }

        //        if( m_pTreeView != nullptr && m_pTreeView->contextMenuPolicy() == Qt::NoContextMenu )
        //        {
        //            m_pTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

        //            if( !QObject::connect(
        //                /* pObjSender   */ m_pTreeView,
        //                /* szSignal     */ SIGNAL(customContextMenuRequested(const QPoint&)),
        //                /* pObjReceiver */ this,
        //                /* szSlot       */ SLOT(onTreeViewContextMenuRequested(const QPoint&)) ) )
        //            {
        //                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        //            }
        //        }

        //    } // if( m_pDSModelNodeSelected != nullptr )

        //    emit selectedModelNodeChanged(m_pDSModelNodeSelected);

        //} // if( pDSNode != m_pDSModelNodeSelected )
    } // if( i_modelIdxCurr.isValid() && i_modelIdxCurr.internalPointer() != nullptr )

} // onTreeViewCurrentChanged
