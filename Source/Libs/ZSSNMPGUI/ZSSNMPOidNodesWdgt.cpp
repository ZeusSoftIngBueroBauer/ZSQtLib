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

$Date: $

$Revision: $

$Author: $

$History: $

*******************************************************************************/

#include <QtGui/qlayout.h>
#include <QtGui/qpainter.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qtreeview.h>
#include "ZSSNMP/Include/ZSSNMPOidNodesWdgt.h"
#include "ZSSNMP/Include/ZSSNMPOidNodesModel.h"
#include "ZSSNMP/Include/ZSSNMPExplorer.h"
#include "ZSSys/Include/ZSSysErrResult.h"
#include "ZSSys/Include/ZSSysException.h"
#include "ZSSys/Include/ZSSysMemLeakDump.h"

using namespace ZS::SNMP;


/*******************************************************************************
class CWdgtOidNodes : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtOidNodes::CWdgtOidNodes( CModelOidNodes* i_pModel, QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pModelOidNodes(i_pModel),
    m_pLytMain(NULL),
    m_pTreeView(NULL)
{
    setObjectName("WdgtOidNodes");

    m_pLytMain = new QVBoxLayout();
    #if QT_VERSION >= 0x040500
    m_pLytMain->setContentsMargins(0,0,0,0);
    #endif
    setLayout(m_pLytMain);

    // Tree view
    //----------

    m_pTreeView = new QTreeView(this);
    m_pLytMain->addWidget(m_pTreeView);

    m_pTreeView->setModel(m_pModelOidNodes);
    m_pTreeView->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_pTreeView->setSelectionMode(QAbstractItemView::SingleSelection);

    if( !QObject::connect(
        /* pObjSender   */ m_pTreeView,
        /* szSignal     */ SIGNAL( clicked(const QModelIndex&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onTreeViewClicked(const QModelIndex&) ) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTreeView,
        /* szSignal     */ SIGNAL( expanded(const QModelIndex&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onTreeViewExpanded(const QModelIndex&) ) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTreeView->selectionModel(),
        /* szSignal     */ SIGNAL( currentChanged(const QModelIndex&, const QModelIndex&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onTreeViewSelectionModelCurrentChanged(const QModelIndex&, const QModelIndex&) ) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

} // ctor

//------------------------------------------------------------------------------
CWdgtOidNodes::~CWdgtOidNodes()
//------------------------------------------------------------------------------
{
    m_pModelOidNodes = NULL;
    m_pLytMain = NULL;
    m_pTreeView = NULL;

} // dtor

/*==============================================================================
protected: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtOidNodes::showEvent( QShowEvent* i_pEv )
//------------------------------------------------------------------------------
{
    int iClm;

    for( iClm = 0; iClm < CModelOidNodes::EColumnCount; iClm++ )
    {
        m_pTreeView->resizeColumnToContents(iClm);
    }

    QWidget::showEvent(i_pEv);

} // showEvent

/*==============================================================================
protected slots: // connected to the signals of the tree view
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtOidNodes::onTreeViewClicked( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    COidNode* pOidNode = NULL;

    if( i_modelIdx.isValid() && i_modelIdx.internalPointer() != NULL )
    {
        pOidNode = static_cast<COidNode*>(i_modelIdx.internalPointer());
    }
    emit nodeSelected(pOidNode);

} // onTreeViewClicked

//------------------------------------------------------------------------------
void CWdgtOidNodes::onTreeViewExpanded( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    if( i_modelIdx.isValid() )
    {
        m_pTreeView->resizeColumnToContents(i_modelIdx.column());
    }

} // onTreeViewExpanded

//------------------------------------------------------------------------------
void CWdgtOidNodes::onTreeViewSelectionModelCurrentChanged(
    const QModelIndex& i_modelIdxCurr,
    const QModelIndex& /*i_modelIdxPrev*/ )
//------------------------------------------------------------------------------
{
    COidNode* pOidNode = NULL;

    if( i_modelIdxCurr.isValid() && i_modelIdxCurr.internalPointer() != NULL )
    {
        pOidNode = static_cast<COidNode*>(i_modelIdxCurr.internalPointer());
    }
    emit nodeSelected(pOidNode);

} // onTreeViewSelectionModelCurrentChanged
