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

#include <QtCore/qtimer.h>

#include "ZSSysGUI/ZSSysRequestExecTreeModel.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysRequestExecTree.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
struct SRequestExecTreeModelNode
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
SRequestExecTreeModelNode::SRequestExecTreeModelNode() :
//------------------------------------------------------------------------------
    m_pReqExecTree(nullptr),
    m_pParentNode(nullptr),
    m_arpChildNodes(),
    // Data copied from request item (to keep content of request item after request item has been deleted)
    m_reqDscr()
{
} // ctor

//------------------------------------------------------------------------------
SRequestExecTreeModelNode::SRequestExecTreeModelNode( CRequestExecTree* i_pReqExecTree ) :
//------------------------------------------------------------------------------
    m_pReqExecTree(i_pReqExecTree),
    m_pParentNode(nullptr),
    m_arpChildNodes(),
    // Data copied from request item (to keep content of request item after request item has been deleted)
    m_reqDscr()
{
    m_reqDscr.m_strRequest = "Requests";

} // ctor

//------------------------------------------------------------------------------
SRequestExecTreeModelNode::SRequestExecTreeModelNode( SRequestDscr* i_pReqDscr ) :
//------------------------------------------------------------------------------
    m_pReqExecTree(nullptr),
    m_pParentNode(nullptr),
    m_arpChildNodes(),
    // Data copied from request item (to keep content of request item after request item has been deleted)
    m_reqDscr(*i_pReqDscr)
{
} // ctor

//------------------------------------------------------------------------------
SRequestExecTreeModelNode::~SRequestExecTreeModelNode()
//------------------------------------------------------------------------------
{
    m_pReqExecTree = nullptr;
    m_pParentNode = nullptr;

} // dtor

/*==============================================================================
public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
double SRequestExecTreeModelNode::getDurationInSec() const
//------------------------------------------------------------------------------
{
    double fDuration_s = 0.0;

    if( m_reqDscr.m_iProgress_perCent >= 100 )
    {
        fDuration_s = m_reqDscr.m_fTimeEnd_s - m_reqDscr.m_fTimeStart_s;
    }
    else
    {
        fDuration_s = ZS::System::Time::getProcTimeInSec() - m_reqDscr.m_fTimeStart_s;
    }
    return fDuration_s;

} // getDurationInSec

//------------------------------------------------------------------------------
QString SRequestExecTreeModelNode::getDurationAsStrInBestUnit() const
//------------------------------------------------------------------------------
{
    QString strDuration;
    double  fDuration_s = getDurationInSec();

    if( fabs(fDuration_s) <= 1.0e-6 )
    {
        strDuration = QString::number(fDuration_s*1.0e9,'f',3) + " ns";
    }
    else if( fabs(fDuration_s) <= 1.0e-3 )
    {
        strDuration = QString::number(fDuration_s*1.0e6,'f',3) + " " + QString::fromLatin1("µ") + "s";
    }
    else if( fabs(fDuration_s) <= 1.0 )
    {
        strDuration = QString::number(fDuration_s*1.0e3,'f',3) + " ms";
    }
    else if( fabs(fDuration_s) <= 1.0e3 )
    {
        strDuration = QString::number(fDuration_s,'f',3) + " s";
    }
    else
    {
        strDuration = QString::number(fDuration_s,'f',3) + " s";
    }
    return strDuration;

} // getDurationAsStrInBestUnit


/*******************************************************************************
class CModelRequestExecTree : public QAbstractItemModel
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelRequestExecTree::CModelRequestExecTree( CRequestExecTree* i_pReqExecTree, QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    QAbstractItemModel(i_pObjParent),
    m_pReqExecTree(i_pReqExecTree),
    m_pRootEntry(nullptr),
    m_hshRequests(),
    m_pTmrRefresh(nullptr)
{
    // Use queued connection for signal/slots of this model.
    // Requests might be created and deleted in different threads.
    // E.g. data might be sent through the trace server from within any thread.
    // Synchronous requests will be set at the request queue to be deleted later
    // on setting a request to be in progress. If the request is created in the
    // gateway thread the "requestAdded" signal will be queued. If the next request
    // is started before receiving the corresponding "requestAdded" slot message the
    // request will be deleted before the "requestAdded" slot message is handled in the
    // main thread. In this case the descriptor would remain endlessly in the model.
    // So we queue both - requests created within the main thread and requests
    // created in other threads hoping that always the "requestAdded" signal
    // is received before the "requestChanged" and "requestRemoved" signals.
    if( !QObject::connect(
        /* pObjSender   */ m_pReqExecTree,
        /* szSignal     */ SIGNAL(requestAdded(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestAdded(ZS::System::SRequestDscr)),
        /* cnctType     */ Qt::QueuedConnection ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pReqExecTree,
        /* szSignal     */ SIGNAL(requestRemoved(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestRemoved(ZS::System::SRequestDscr)),
        /* cnctType     */ Qt::QueuedConnection ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pReqExecTree,
        /* szSignal     */ SIGNAL(requestChanged(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)),
        /* cnctType     */ Qt::QueuedConnection ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    beginInsertRows( QModelIndex(), 0, 0 );

    m_pRootEntry = new SRequestExecTreeModelNode(m_pReqExecTree);

    endInsertRows();

    SRequestDscr* pReqDscrTop;
    SRequestDscr* pReqDscr;

    m_pReqExecTree->lock();

    QHash<qint64,SRequestDscr*>           hshRequests = m_pReqExecTree->getRequests();
    QHash<qint64,SRequestDscr*>::iterator itHshRequests;

    for( itHshRequests = hshRequests.begin(); itHshRequests != hshRequests.end(); itHshRequests++ )
    {
        pReqDscr = itHshRequests.value();

        if( pReqDscr != nullptr )
        {
            pReqDscrTop = m_pReqExecTree->findParentRequestOf(pReqDscr->m_iId,true);

            if( pReqDscrTop == nullptr )
            {
                addNode( m_pRootEntry, pReqDscr );
            }
            else // if( pReqTop != nullptr )
            {
                addNode( m_pRootEntry, pReqDscrTop );
            }
        }
    }

    m_pReqExecTree->unlock();

    m_pTmrRefresh = new QTimer(this);

    if( !QObject::connect(
        /* pObjSender   */ m_pTmrRefresh,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTmrRefreshTimeout()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pTmrRefresh->start(1000);

} // ctor

//------------------------------------------------------------------------------
CModelRequestExecTree::~CModelRequestExecTree()
//------------------------------------------------------------------------------
{
    clearNode(m_pRootEntry);

    beginRemoveRows( QModelIndex(), 0, 0 );

    try
    {
        delete m_pRootEntry;
    }
    catch(...)
    {
    }
    m_pRootEntry = nullptr;

    endRemoveRows();

    m_pReqExecTree = nullptr;
    m_pTmrRefresh = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelRequestExecTree::clear()
//------------------------------------------------------------------------------
{
    clearNode(m_pRootEntry);
}

//------------------------------------------------------------------------------
void CModelRequestExecTree::removeEntry( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    if( i_modelIdx.isValid() )
    {
        m_pReqExecTree->lock();

        QObject::disconnect(
            /* pObjSender   */ m_pReqExecTree,
            /* szSignal     */ SIGNAL(requestRemoved(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestRemoved(ZS::System::SRequestDscr)) );

        SRequestExecTreeModelNode* pNode;
        qint64                     iReqId;
        QModelIndex                modelIdxParent;

        modelIdxParent = parent(i_modelIdx);

        pNode = static_cast<SRequestExecTreeModelNode*>(i_modelIdx.internalPointer());

        if( pNode != m_pRootEntry )
        {
            iReqId = pNode->m_reqDscr.m_iId;

            m_pReqExecTree->removeRequest(iReqId);

            clearNode(pNode);

            int iRow = pNode->m_pParentNode->m_arpChildNodes.indexOf(pNode);

            beginRemoveRows( modelIdxParent, iRow, iRow );

            if( m_hshRequests.contains(iReqId) )
            {
                m_hshRequests.remove(iReqId);
            }

            pNode->m_pParentNode->m_arpChildNodes.removeOne(pNode);

            if( pNode->m_pParentNode != m_pRootEntry )
            {
                pNode->m_pParentNode->m_reqDscr.m_ariChildIds.removeOne(pNode->m_reqDscr.m_iId);
            }

            delete pNode;
            pNode = nullptr;

            endRemoveRows();

            if( !QObject::connect(
                /* pObjSender   */ m_pReqExecTree,
                /* szSignal     */ SIGNAL(requestRemoved(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onRequestRemoved(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

        } // if( pNode != m_pRootEntry )

        m_pReqExecTree->unlock();

    } // if( i_modelIdx.isValid() )

} // removeEntry

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CModelRequestExecTree::onRequestAdded( ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    m_pReqExecTree->lock();

    SRequestExecTreeModelNode* pNodeParent = nullptr;

    SRequestDscr* pReqDscrParent = m_pReqExecTree->findParentRequestOf(i_reqDscr.m_iId);

    if( pReqDscrParent != nullptr && m_hshRequests.contains(pReqDscrParent->m_iId) )
    {
        pNodeParent = m_hshRequests[pReqDscrParent->m_iId];
    }
    else
    {
        pNodeParent = m_pRootEntry;
    }

    addNode( pNodeParent, &i_reqDscr );

    m_pReqExecTree->unlock();

} // onRequestAdded

//------------------------------------------------------------------------------
void CModelRequestExecTree::onRequestRemoved( ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    if( m_hshRequests.contains(i_reqDscr.m_iId) )
    {
        SRequestExecTreeModelNode* pNode = m_hshRequests[i_reqDscr.m_iId];

        clearNode(pNode);

        int iRow = 0;

        if( pNode->m_pParentNode != nullptr )
        {
            iRow = pNode->m_pParentNode->m_arpChildNodes.indexOf(pNode);
        }

        if( iRow >= 0 )
        {
            QModelIndex modelIdx = createIndex(iRow,0,pNode);

            QModelIndex modelIdxParent = parent(modelIdx);

            beginRemoveRows( modelIdxParent, iRow, iRow );

            m_hshRequests.remove(i_reqDscr.m_iId);

            if( pNode->m_pParentNode != nullptr )
            {
                pNode->m_pParentNode->m_arpChildNodes.removeOne(pNode);
                pNode->m_pParentNode->m_reqDscr.m_ariChildIds.removeOne(pNode->m_reqDscr.m_iId);
            }

            delete pNode;
            pNode = nullptr;

            endRemoveRows();

        } // if( iRow >= 0 )
    } // if( m_hshRequests.contains(i_reqDscr->m_iId) )

} // onRequestRemoved

//------------------------------------------------------------------------------
void CModelRequestExecTree::onRequestChanged( ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    if( m_hshRequests.contains(i_reqDscr.m_iId) )
    {
        SRequestExecTreeModelNode* pNode = m_hshRequests[i_reqDscr.m_iId];

        int iRow = 0;

        if( pNode->m_pParentNode != nullptr )
        {
            iRow = pNode->m_pParentNode->m_arpChildNodes.indexOf(pNode);
        }

        QModelIndex modelIdxTL = createIndex(iRow,0,pNode);
        QModelIndex modelIdxBR = createIndex(iRow,EColumnCount-1,pNode);

        pNode->m_reqDscr = i_reqDscr;

        emit dataChanged( modelIdxTL, modelIdxBR );

    } // if( m_hshRequests.contains(i_reqDscr.m_iId) )

} // onRequestChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CModelRequestExecTree::onTmrRefreshTimeout()
//------------------------------------------------------------------------------
{
    updateNode(m_pRootEntry);
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelRequestExecTree::addNode( SRequestExecTreeModelNode* i_pNodeParent, SRequestDscr* i_pReqDscr )
//------------------------------------------------------------------------------
{
    SRequestExecTreeModelNode* pNodeParent = i_pNodeParent;
    SRequestExecTreeModelNode* pNode;
    int                        iRow;

    if( pNodeParent == nullptr )
    {
        pNodeParent = m_pRootEntry;
    }

    if( m_hshRequests.contains(i_pReqDscr->m_iId) )
    {
        pNode = m_hshRequests[i_pReqDscr->m_iId];
    }
    else // if( !m_hshRequests.contains(i_pReqDscr->m_iId) )
    {
        pNode = new SRequestExecTreeModelNode(i_pReqDscr);

        iRow = pNodeParent->m_arpChildNodes.size();
        pNodeParent->m_arpChildNodes.append(pNode);
        pNodeParent->m_reqDscr.m_ariChildIds.append(pNode->m_reqDscr.m_iId);
        pNode->m_pParentNode = pNodeParent;

        QModelIndex modelIdx = createIndex( iRow, 0, pNode );

        QModelIndex modelIdxParent = parent(modelIdx);

        beginInsertRows( modelIdxParent, iRow, iRow );

        m_hshRequests[i_pReqDscr->m_iId] = pNode;

        endInsertRows();

    } // if( !m_hshRequests.contains(i_pReqDscr->m_iId) )

    // If the added node already has children they will also be added to the model.
    //-----------------------------------------------------------------------------

    SRequestDscr* pReqDscr = m_pReqExecTree->findRequest(i_pReqDscr->m_iId);

    if( pReqDscr != nullptr )
    {
        SRequestDscr* pReqDscrChild;
        int           idxChild;

        for( idxChild = 0; idxChild < pReqDscr->m_ariChildIds.size(); idxChild++ )
        {
            pReqDscrChild = m_pReqExecTree->findRequest(pReqDscr->m_ariChildIds[idxChild]);

            if( pReqDscrChild != nullptr )
            {
                addNode(pNode,pReqDscrChild);
            }
        }
    }

} // addNode

//------------------------------------------------------------------------------
void CModelRequestExecTree::clearNode( SRequestExecTreeModelNode* i_pNode )
//------------------------------------------------------------------------------
{
    if( i_pNode->m_arpChildNodes.size() > 0 )
    {
        int iRow = 0;

        if( i_pNode->m_pParentNode != nullptr )
        {
            iRow = i_pNode->m_pParentNode->m_arpChildNodes.indexOf(i_pNode);
        }

        QModelIndex modelIdx = createIndex( iRow, 0, i_pNode );

        SRequestExecTreeModelNode* pNodeChild;

        for( iRow = i_pNode->m_arpChildNodes.size()-1; iRow >= 0; iRow-- )
        {
            pNodeChild = i_pNode->m_arpChildNodes[iRow];

            clearNode(pNodeChild);

            beginRemoveRows( modelIdx, iRow, iRow );

            if( m_hshRequests.contains(pNodeChild->m_reqDscr.m_iId) )
            {
                m_hshRequests.remove(pNodeChild->m_reqDscr.m_iId);
            }

            i_pNode->m_arpChildNodes.removeLast();

            i_pNode->m_reqDscr.m_ariChildIds.removeOne(pNodeChild->m_reqDscr.m_iId);

            delete pNodeChild;
            pNodeChild = nullptr;

            endRemoveRows();
        }
    } // if( i_pNode->m_arpChildNodes.size() > 0 )

} // clearNode

//------------------------------------------------------------------------------
void CModelRequestExecTree::updateNode( SRequestExecTreeModelNode* i_pNode )
//------------------------------------------------------------------------------
{
    int iRow;

    if( i_pNode->m_reqDscr.m_objState == EObjState::Created )
    {
        if( i_pNode->m_reqDscr.m_iProgress_perCent < 100 )
        {
            int iRow = 0;

            if( i_pNode->m_pParentNode != nullptr )
            {
                iRow = i_pNode->m_pParentNode->m_arpChildNodes.indexOf(i_pNode);
            }

            QModelIndex modelIdxTL = createIndex(iRow,0,i_pNode);
            QModelIndex modelIdxBR = createIndex(iRow,EColumnCount-1,i_pNode);

            emit dataChanged(modelIdxTL,modelIdxBR);
        }
    }

    SRequestExecTreeModelNode* pNodeChild;

    for( iRow = 0; iRow < i_pNode->m_arpChildNodes.size(); iRow++ )
    {
        pNodeChild = i_pNode->m_arpChildNodes[iRow];

        updateNode(pNodeChild);
    }

} // updateNode

/*==============================================================================
public: // must overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelRequestExecTree::rowCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    int iRowCount = 0;

    if( !i_modelIdxParent.isValid() )
    {
        iRowCount = 1;
    }
    else
    {
        SRequestExecTreeModelNode* pNodeParent = static_cast<SRequestExecTreeModelNode*>(i_modelIdxParent.internalPointer());

        if( pNodeParent != nullptr )
        {
            iRowCount = pNodeParent->m_arpChildNodes.size();
        }
    }

    return iRowCount;

} // rowCount

//------------------------------------------------------------------------------
int CModelRequestExecTree::columnCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return EColumnCount;
}

//------------------------------------------------------------------------------
QVariant CModelRequestExecTree::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    if( !i_modelIdx.isValid() )
    {
        return varData;
    }

    SRequestExecTreeModelNode* pNode = static_cast<SRequestExecTreeModelNode*>(i_modelIdx.internalPointer());

    if( pNode == nullptr )
    {
        return varData;
    }

    switch( i_modelIdx.column() )
    {
        case EColumnRequestName:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                QString strNodeName;

                if( !pNode->m_reqDscr.m_strCreatorObjName.isEmpty() )
                {
                    strNodeName = pNode->m_reqDscr.m_strCreatorObjName + ".";
                }
                else if( !pNode->m_reqDscr.m_strCreatorClassName.isEmpty() )
                {
                    strNodeName = pNode->m_reqDscr.m_strCreatorClassName + ".";
                }
                varData = strNodeName + pNode->m_reqDscr.m_strRequest;
            }
            break;
        }
        case EColumnRequest:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( pNode != m_pRootEntry )
                {
                    varData = QString::number(pNode->m_reqDscr.m_request);
                }
            }
            break;
        }
        case EColumnId:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( pNode != m_pRootEntry )
                {
                    varData = QString::number(pNode->m_reqDscr.m_iId);
                }
            }
            break;
        }
        case EColumnParentId:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( pNode != m_pRootEntry )
                {
                    if( pNode->m_reqDscr.m_iParentId >= 0 )
                    {
                        varData = QString::number(pNode->m_reqDscr.m_iParentId);
                    }
                    else
                    {
                        varData = "-";
                    }
                }
            }
            break;
        }
        case EColumnCreaterObjName:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( pNode != m_pRootEntry )
                {
                    varData = pNode->m_reqDscr.m_strCreatorObjName;
                }
            }
            break;
        }
        case EColumnCreatorNameSpace:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( pNode != m_pRootEntry )
                {
                    varData = pNode->m_reqDscr.m_strCreatorNameSpace;
                }
            }
            break;
        }
        case EColumnCreatorClassName:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( pNode != m_pRootEntry )
                {
                    varData = pNode->m_reqDscr.m_strCreatorClassName;
                }
            }
            break;
        }
        case EColumnCreatorThreadName:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( pNode != m_pRootEntry )
                {
                    varData = pNode->m_reqDscr.m_strCreatorThreadName;
                }
            }
            break;
        }
        case EColumnProgressInPerCent:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( pNode != m_pRootEntry )
                {
                    varData = QString::number(pNode->m_reqDscr.m_iProgress_perCent);
                }
            }
            break;
        }
        case EColumnErrResultInfo:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( pNode != m_pRootEntry )
                {
                    varData = pNode->m_reqDscr.m_errResultInfo.getResultStr();
                }
            }
            else if( i_iRole == Qt::ToolTipRole )
            {
                if( pNode != m_pRootEntry )
                {
                    if( pNode->m_reqDscr.m_errResultInfo.isErrorResult() )
                    {
                        QString strErrInfo = pNode->m_reqDscr.m_errResultInfo.getErrSource().toString();

                        if( !pNode->m_reqDscr.m_errResultInfo.getAddErrInfoDscr().isEmpty() )
                        {
                            strErrInfo += ", " + pNode->m_reqDscr.m_errResultInfo.getAddErrInfoDscr();
                        }
                        if( !strErrInfo.isEmpty() )
                        {
                            varData = strErrInfo;
                        }
                    }
                }
            }
            else if( i_iRole == Qt::DecorationRole )
            {
                if( pNode != m_pRootEntry )
                {
                    varData = getErrIcon(pNode->m_reqDscr.m_errResultInfo.getSeverity());
                }
            }
            break;
        }
        case EColumnIsBlocking:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( pNode != m_pRootEntry )
                {
                    varData = bool2Str(pNode->m_reqDscr.m_bIsBlocking);
                }
            }
            break;
        }
        case EColumnTimeout:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( pNode != m_pRootEntry )
                {
                    double  fTimeout_s = pNode->m_reqDscr.m_iTimeout_ms/1000.0;
                    QString strTimeout = QString::number(fTimeout_s,'f',3) + " s";

                    if( pNode->m_reqDscr.m_bIsBlocking )
                    {
                        if( fTimeout_s <= 0.0 )
                        {
                            varData = "Infinite (" + strTimeout + ")";
                        }
                        else
                        {
                            varData = strTimeout;
                        }
                    }
                    else
                    {
                        if( fTimeout_s < 0.0 )
                        {
                            varData = "Invalid (" + strTimeout + ")";
                        }
                        else
                        {
                            varData = strTimeout;
                        }
                    }
                }
            }
            break;
        }
        case EColumnTimeStart:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( pNode != m_pRootEntry )
                {
                    varData = QString::number(pNode->m_reqDscr.m_fTimeStart_s,'f',3) + " s";
                }
            }
            break;
        }
        case EColumnTimeEnd:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( pNode != m_pRootEntry )
                {
                    varData = QString::number(pNode->m_reqDscr.m_fTimeEnd_s,'f',3) + " s";
                }
            }
            break;
        }
        case EColumnTimeDuration:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( pNode != m_pRootEntry )
                {
                    varData = pNode->getDurationAsStrInBestUnit();
                }
            }
            break;
        }
        case EColumnObjState:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( pNode != m_pRootEntry )
                {
                    varData = CEnumObjState::toString(pNode->m_reqDscr.m_objState);
                }
            }
            break;
        }
        case EColumnDescription:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( pNode != m_pRootEntry )
                {
                    varData = pNode->m_reqDscr.m_strDscr;
                }
            }
            break;
        }
        case EColumnToolTip:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( pNode != m_pRootEntry )
                {
                    varData = pNode->m_reqDscr.m_strToolTip;
                }
            }
            break;
        }
        default:
        {
            break;
        }
    }
    return varData;

} // data

//------------------------------------------------------------------------------
QModelIndex CModelRequestExecTree::index( int i_iRow, int i_iCol, const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    SRequestExecTreeModelNode* pNodeParent = nullptr;
    SRequestExecTreeModelNode* pNode;
    QModelIndex                modelIdx;

    if( i_iRow >= 0 )
    {
        if( !i_modelIdxParent.isValid() )
        {
            if( i_iRow == 0 )
            {
                modelIdx = createIndex(i_iRow,i_iCol,m_pRootEntry);
            }
        }
        else
        {
            pNodeParent = static_cast<SRequestExecTreeModelNode*>(i_modelIdxParent.internalPointer());

            if( pNodeParent->m_arpChildNodes.size() > 0 && i_iRow < pNodeParent->m_arpChildNodes.size() )
            {
                pNode = pNodeParent->m_arpChildNodes[i_iRow];
                modelIdx = createIndex(i_iRow,i_iCol,pNode);
            }
        }
    }
    return modelIdx;

} // index

//------------------------------------------------------------------------------
QModelIndex CModelRequestExecTree::parent( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QModelIndex modelIdxParent;

    if( i_modelIdx.isValid() )
    {
        SRequestExecTreeModelNode* pNode = static_cast<SRequestExecTreeModelNode*>(i_modelIdx.internalPointer());
        SRequestExecTreeModelNode* pNodeParent;

        if( pNode != nullptr )
        {
            pNodeParent = pNode->m_pParentNode;

            if( pNodeParent != nullptr )
            {
                int iRow = 0;

                SRequestExecTreeModelNode* pNodeParentParent = pNodeParent->m_pParentNode;

                if( pNodeParentParent != nullptr )
                {
                    iRow = pNodeParentParent->m_arpChildNodes.indexOf(pNodeParent);
                }
                modelIdxParent = createIndex( iRow, 0, pNodeParent );
            }
        }
    } // if( i_modelIdx.isValid() )

    return modelIdxParent;

} // parent

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CModelRequestExecTree::headerData(
    int             i_iSection,
    Qt::Orientation i_orientation,
    int             i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    if( i_orientation == Qt::Horizontal )
    {
        switch( i_iSection )
        {
            case EColumnRequestName:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Request Name";
                }
                break;
            }
            case EColumnRequest:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Request";
                }
                break;
            }
            case EColumnId:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "ID";
                }
                break;
            }
            case EColumnParentId:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Parent-ID";
                }
                break;
            }
            case EColumnCreatorNameSpace:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Name Space";
                }
                break;
            }
            case EColumnCreatorClassName:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Class Name";
                }
                break;
            }
            case EColumnCreatorThreadName:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Thread Name";
                }
                break;
            }
            case EColumnCreaterObjName:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Object Name";
                }
                break;
            }
            case EColumnProgressInPerCent:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Progress/%";
                }
                break;
            }
            case EColumnErrResultInfo:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Result Info";
                }
                break;
            }
            case EColumnIsBlocking:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Blocking";
                }
                break;
            }
            case EColumnTimeout:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Timeout";
                }
                break;
            }
            case EColumnTimeStart:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Started";
                }
                break;
            }
            case EColumnTimeEnd:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Ended";
                }
                break;
            }
            case EColumnTimeDuration:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Duration";
                }
                break;
            }
            case EColumnObjState:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Object State";
                }
                break;
            }
            case EColumnDescription:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Description";
                }
                break;
            }
            case EColumnToolTip:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Tool Tip";
                }
                break;
            }
            default:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "?";
                }
                else if( i_iRole == Qt::SizeHintRole )
                {
                    //varData = QSize(10,16);
                }
                break;
            }
        }
    }
    else // if( i_orientation == Qt::Vertical )
    {
    }
    return varData;

} // headerData
