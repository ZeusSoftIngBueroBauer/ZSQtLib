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

$History: ZSSNMPInstanceNodesWdgt.cpp $
 * 
 * *****************  Version 3  *****************
 * User: Christian Bauer Date: 22.01.10   Time: 22:11
 * Updated in $/ZSSNMP/Source

*******************************************************************************/

#include <QtGui/qlayout.h>
#include <QtGui/qheaderview.h>
#include <QtGui/qpainter.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qtableview.h>
#include "ZSSNMP/Include/ZSSNMPInstanceNodesWdgt.h"
#include "ZSSNMP/Include/ZSSNMPInstanceNodesModel.h"
#include "ZSSNMP/Include/ZSSNMPExplorer.h"
#include "ZSSys/Include/ZSSysErrResult.h"
#include "ZSSys/Include/ZSSysException.h"
#include "ZSSys/Include/ZSSysMemLeakDump.h"

using namespace ZS::SNMP;


/*******************************************************************************
class CWdgtInstanceNodes : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtInstanceNodes::CWdgtInstanceNodes( CModelInstanceNodes* i_pModel, QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pModelInstanceNodes(i_pModel),
    m_pLytMain(NULL),
    m_pTableView(NULL)
{
    setObjectName("WdgtKeyInstanceNodes");

    m_pLytMain = new QVBoxLayout();
    #if QT_VERSION >= 0x040500
    m_pLytMain->setContentsMargins(0,0,0,0);
    #endif
    setLayout(m_pLytMain);

    // Table view with model
    //----------------------

    m_pTableView = new QTableView(this);
    m_pLytMain->addWidget(m_pTableView);

    m_pTableView->setModel(m_pModelInstanceNodes);
    m_pTableView->verticalHeader()->hide();
    m_pTableView->horizontalHeader()->show();
    m_pTableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_pTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pTableView->setTextElideMode(Qt::ElideRight);
    m_pTableView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    //m_pTableView->setSpan(0,CModelInstanceNodes::EColumnInstanceNumber,1,1);
    //m_pTableView->setSpan(0,CModelInstanceNodes::EColumnValueType,1,1);
    //m_pTableView->setSpan(0,CModelInstanceNodes::EColumnValue,1,1);

    if( !QObject::connect(
        /* pObjSender   */ m_pTableView,
        /* szSignal     */ SIGNAL( clicked(const QModelIndex&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onTableViewClicked(const QModelIndex&) ) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTableView->selectionModel(),
        /* szSignal     */ SIGNAL( currentChanged(const QModelIndex&, const QModelIndex&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onTableViewSelectionModelCurrentChanged(const QModelIndex&, const QModelIndex&) ) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }
    if( !connect(
        /* pObjSender   */ m_pModelInstanceNodes,
        /* szSignal     */ SIGNAL( dataChanged(const QModelIndex&,const QModelIndex&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onModelDataChanged(const QModelIndex&,const QModelIndex&) ) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }
    if( !connect(
        /* pObjSender   */ m_pModelInstanceNodes,
        /* szSignal     */ SIGNAL( rowsInserted(const QModelIndex&,int,int) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onModelRowsInserted(const QModelIndex&,int,int) ) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }
    if( !connect(
        /* pObjSender   */ m_pModelInstanceNodes,
        /* szSignal     */ SIGNAL( rowsRemoved(const QModelIndex&,int,int) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onModelRowsRemoved(const QModelIndex&,int,int) ) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }

} // ctor

//------------------------------------------------------------------------------
CWdgtInstanceNodes::~CWdgtInstanceNodes()
//------------------------------------------------------------------------------
{
    m_pModelInstanceNodes = NULL;
    m_pLytMain = NULL;
    m_pTableView = NULL;

} // dtor

/*==============================================================================
protected slots: // connected to the signals of the table view
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtInstanceNodes::onTableViewClicked( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    CInstanceNode* pInstanceNode = NULL;

    if( i_modelIdx.isValid() )
    {
        int iRow = i_modelIdx.row();

        const CModelInstanceNodes* pCModel = dynamic_cast<const CModelInstanceNodes*>(i_modelIdx.model());
        CModelInstanceNodes*       pVModel = const_cast<CModelInstanceNodes*>(pCModel);

        if( pCModel != NULL )
        {
            pInstanceNode = pVModel->getNode(iRow);
        }
    }
    emit nodeSelected(pInstanceNode);

} // onTableViewClicked

//------------------------------------------------------------------------------
void CWdgtInstanceNodes::onTableViewSelectionModelCurrentChanged(
    const QModelIndex& i_modelIdxCurr,
    const QModelIndex& /*i_modelIdxPrev*/ )
//------------------------------------------------------------------------------
{
    CInstanceNode* pInstanceNode = NULL;

    if( i_modelIdxCurr.isValid() )
    {
        int iRow = i_modelIdxCurr.row();

        const CModelInstanceNodes* pCModel = dynamic_cast<const CModelInstanceNodes*>(i_modelIdxCurr.model());
        CModelInstanceNodes*       pVModel = const_cast<CModelInstanceNodes*>(pCModel);

        if( pCModel != NULL )
        {
            pInstanceNode = pVModel->getNode(iRow);
        }
    }
    emit nodeSelected(pInstanceNode);

} // onTableViewSelectionModelCurrentChanged

/*==============================================================================
protected slots: // connected to the signals of the model
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtInstanceNodes::onModelDataChanged( const QModelIndex& /*i_modelIdxTopLeft*/, const QModelIndex& /*i_modelIdxBottomRight*/ )
//------------------------------------------------------------------------------
{
    //m_pTableView->resizeRowsToContents();
    //m_pTableView->resizeColumnsToContents();
}

//------------------------------------------------------------------------------
void CWdgtInstanceNodes::onModelRowsInserted( const QModelIndex& /*i_modelIdxParent*/, int /*i_iRowStart*/, int /*i_iRowEnd*/ )
//------------------------------------------------------------------------------
{
    //m_pTableView->resizeRowsToContents();
    m_pTableView->resizeColumnsToContents();
}

//------------------------------------------------------------------------------
void CWdgtInstanceNodes::onModelRowsRemoved( const QModelIndex& /*i_modelIdxParent*/, int /*i_iRowStart*/, int /*i_iRowEnd*/ )
//------------------------------------------------------------------------------
{
    //m_pTableView->resizeRowsToContents();
    //m_pTableView->resizeColumnsToContents();
}
