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

#include <QtCore/qtimer.h>

#include "ZSSysGUI/ZSSysRequestSequencerModel.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
struct SRequestSeqEntryModelNode
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
SRequestSeqEntryModelNode::SRequestSeqEntryModelNode() :
//------------------------------------------------------------------------------
    m_pReqSeq(nullptr),
    m_pParentNode(nullptr),
    m_arpChildNodes(),
    m_reqSeqEntry()
{
} // ctor

//------------------------------------------------------------------------------
SRequestSeqEntryModelNode::~SRequestSeqEntryModelNode()
//------------------------------------------------------------------------------
{
    m_pReqSeq = nullptr;
    m_pParentNode = nullptr;

} // dtor


/*******************************************************************************
class CModelRequestSequencer : public QAbstractItemModel
*******************************************************************************/

// Should be protected class members.
// But python bindings generated with shiboken will not be compilable then.

static int /*CModelRequestSequencer::*/s_iInstCount = 0;

static bool /*CModelRequestSequencer::*/s_bIconsCreated = false;

static QPixmap* /*CModelRequestSequencer::*/s_pPxmTypeGroupNodeNormalOff = nullptr;
static QPixmap* /*CModelRequestSequencer::*/s_pPxmTypeGroupNodeSelectedOff = nullptr;
static QIcon*   /*CModelRequestSequencer::*/s_pIconTypeGroupNode = nullptr;
static QPixmap* /*CModelRequestSequencer::*/s_pPxmTypeActiveNodeNormalOff = nullptr;
static QPixmap* /*CModelRequestSequencer::*/s_pPxmTypeActiveNodeSelectedOff = nullptr;
static QIcon*   /*CModelRequestSequencer::*/s_pIconTypeActiveNode = nullptr;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QIcon CModelRequestSequencer::GetIcon( ERequestEntryType i_entryType )
//------------------------------------------------------------------------------
{
    QIcon icon;

    if( !s_bIconsCreated )
    {
        QSize sizePxm;

        s_pIconTypeGroupNode = new QIcon();
        s_pIconTypeActiveNode = new QIcon();

        s_pPxmTypeGroupNodeNormalOff = new QPixmap(":/ZS/TreeView/Requests/TreeViewRequestsBranchEntryNormalOff.bmp");
        s_pPxmTypeGroupNodeNormalOff->setMask(s_pPxmTypeGroupNodeNormalOff->createHeuristicMask());

        s_pPxmTypeGroupNodeSelectedOff = new QPixmap(":/ZS/TreeView/Requests/TreeViewRequestsBranchEntrySelectedOff.bmp");
        s_pPxmTypeGroupNodeSelectedOff->setMask(s_pPxmTypeGroupNodeSelectedOff->createHeuristicMask());

        s_pPxmTypeActiveNodeNormalOff = new QPixmap(":/ZS/TreeView/Requests/TreeViewRequestsLeaveEntryNormalOff.bmp");
        s_pPxmTypeActiveNodeNormalOff->setMask(s_pPxmTypeActiveNodeNormalOff->createHeuristicMask());

        s_pPxmTypeActiveNodeSelectedOff = new QPixmap(":/ZS/TreeView/Requests/TreeViewRequestsLeaveEntrySelectedOff.bmp");
        s_pPxmTypeActiveNodeSelectedOff->setMask(s_pPxmTypeActiveNodeSelectedOff->createHeuristicMask());

        sizePxm = s_pPxmTypeGroupNodeNormalOff->size();
        sizePxm = s_pPxmTypeGroupNodeSelectedOff->size();

        s_pIconTypeGroupNode->addPixmap(*s_pPxmTypeGroupNodeNormalOff,QIcon::Normal,QIcon::Off);
        s_pIconTypeGroupNode->addPixmap(*s_pPxmTypeGroupNodeSelectedOff,QIcon::Selected,QIcon::Off);
        s_pIconTypeGroupNode->addPixmap(*s_pPxmTypeGroupNodeNormalOff,QIcon::Normal,QIcon::On);
        s_pIconTypeGroupNode->addPixmap(*s_pPxmTypeGroupNodeSelectedOff,QIcon::Selected,QIcon::On);

        sizePxm = s_pPxmTypeActiveNodeNormalOff->size();
        sizePxm = s_pPxmTypeActiveNodeSelectedOff->size();

        s_pIconTypeActiveNode->addPixmap(*s_pPxmTypeActiveNodeNormalOff,QIcon::Normal,QIcon::Off);
        s_pIconTypeActiveNode->addPixmap(*s_pPxmTypeActiveNodeSelectedOff,QIcon::Selected,QIcon::Off);
        s_pIconTypeActiveNode->addPixmap(*s_pPxmTypeActiveNodeNormalOff,QIcon::Normal,QIcon::On);
        s_pIconTypeActiveNode->addPixmap(*s_pPxmTypeActiveNodeSelectedOff,QIcon::Selected,QIcon::On);

        s_bIconsCreated = true;
    }

    switch( i_entryType )
    {
        case ERequestEntryTypeGroupNode:
        {
            if( s_pIconTypeGroupNode != nullptr )
            {
                icon = *s_pIconTypeGroupNode;
            }
            break;
        }
        case ERequestEntryTypeActiveNode:
        {
            if( s_pIconTypeActiveNode != nullptr )
            {
                icon = *s_pIconTypeActiveNode;
            }
            break;
        }
        default:
        {
            break;
        }
    }

    return icon;

} // getIcon

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelRequestSequencer::CModelRequestSequencer( QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    QAbstractItemModel(i_pObjParent),
    m_pReqSeq(nullptr),
    m_pRootEntry(nullptr),
    m_hshRequests(),
    m_pTmrRefresh(nullptr)
{
    s_iInstCount++;

    beginInsertRows(QModelIndex(), 0, 0);

    m_pRootEntry = new SRequestSeqEntryModelNode();

    m_pRootEntry->m_pReqSeq = nullptr;
    m_pRootEntry->m_reqSeqEntry.m_reqEntryType = ERequestEntryTypeGroupNode;

    //m_hshRequests[-1] = pNode;

    endInsertRows();

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
CModelRequestSequencer::~CModelRequestSequencer()
//------------------------------------------------------------------------------
{
    s_iInstCount--;

    if( s_iInstCount == 0 )
    {
        delete s_pPxmTypeGroupNodeNormalOff;
        s_pPxmTypeGroupNodeNormalOff = nullptr;
        delete s_pPxmTypeGroupNodeSelectedOff;
        s_pPxmTypeGroupNodeSelectedOff = nullptr;
        delete s_pIconTypeGroupNode;
        s_pIconTypeGroupNode = nullptr;
        delete s_pPxmTypeActiveNodeNormalOff;
        s_pPxmTypeActiveNodeNormalOff = nullptr;
        delete s_pPxmTypeActiveNodeSelectedOff;
        s_pPxmTypeActiveNodeSelectedOff = nullptr;
        delete s_pIconTypeActiveNode;
        s_pIconTypeActiveNode = nullptr;

        s_bIconsCreated = false;
    }

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

    m_pReqSeq = nullptr;
    m_pTmrRefresh = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelRequestSequencer::setSequencer( CRequestSequencer* i_pReqSeq )
//------------------------------------------------------------------------------
{
    if( m_pReqSeq != i_pReqSeq )
    {
        if( m_pReqSeq != nullptr )
        {
            m_pTmrRefresh->stop();

            QObject::disconnect(
                /* pObjSender   */ m_pReqSeq,
                /* szSignal     */ SIGNAL(requestAdded(qint64)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onRequestAdded(qint64)) );
            QObject::disconnect(
                /* pObjSender   */ m_pReqSeq,
                /* szSignal     */ SIGNAL(requestRemoved(qint64)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onRequestRemoved(qint64)) );
            QObject::disconnect(
                /* pObjSender   */ m_pReqSeq,
                /* szSignal     */ SIGNAL(requestChanged(qint64,qint64)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onRequestChanged(qint64,qint64)) );

            clear();

        } // if( m_pReqSeq != nullptr )

        m_pReqSeq = i_pReqSeq;

        if( m_pReqSeq != nullptr )
        {
            if( !QObject::connect(
                /* pObjSender   */ m_pReqSeq,
                /* szSignal     */ SIGNAL(requestAdded(qint64)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onRequestAdded(qint64)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
            if( !QObject::connect(
                /* pObjSender   */ m_pReqSeq,
                /* szSignal     */ SIGNAL(requestRemoved(qint64)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onRequestRemoved(qint64)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
            if( !QObject::connect(
                /* pObjSender   */ m_pReqSeq,
                /* szSignal     */ SIGNAL(requestChanged(qint64,qint64)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onRequestChanged(qint64,qint64)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            QModelIndex modelIdxTL = createIndex(0, 0, m_pRootEntry);
            QModelIndex modelIdxBR = createIndex(0, EColumnCount-1, m_pRootEntry);

            m_pRootEntry->m_pReqSeq = m_pReqSeq;

            emit dataChanged(modelIdxTL, modelIdxBR);

            m_pReqSeq->lock();

            QList<qint64> ariReqIds = m_pReqSeq->getRequestIds();

            SRequestSeqEntry* pReqSeqEntry;
            qint64            iReqId;
            int               idxReq;

            for( idxReq = 0; idxReq < ariReqIds.size(); idxReq++ )
            {
                iReqId = ariReqIds[idxReq];
                pReqSeqEntry = m_pReqSeq->getRequestSeqEntry(iReqId);
                addNode(m_pRootEntry, pReqSeqEntry);
            }

            m_pReqSeq->unlock();

            m_pTmrRefresh->start(1000);

        } // if( m_pReqSeq != nullptr )
    } // if( m_pReqSeq != i_pReqSeq )

} // setSequencer

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelRequestSequencer::clear()
//------------------------------------------------------------------------------
{
    clearNode(m_pRootEntry);
}

//------------------------------------------------------------------------------
void CModelRequestSequencer::removeEntry( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    if( i_modelIdx.isValid() )
    {
        m_pReqSeq->lock();

        QObject::disconnect(
            /* pObjSender   */ m_pReqSeq,
            /* szSignal     */ SIGNAL(requestRemoved(qint64)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestRemoved(qint64)) );

        SRequestSeqEntryModelNode* pNode;
        qint64                     iReqId;
        QModelIndex                modelIdxParent;

        modelIdxParent = parent(i_modelIdx);

        pNode = static_cast<SRequestSeqEntryModelNode*>(i_modelIdx.internalPointer());

        if( pNode != m_pRootEntry )
        {
            iReqId = pNode->m_reqSeqEntry.m_reqDscr.m_iId;

            m_pReqSeq->removeRequest(iReqId);

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
                pNode->m_pParentNode->m_reqSeqEntry.m_reqDscr.m_ariChildIds.removeOne(pNode->m_reqSeqEntry.m_reqDscr.m_iId);
            }

            delete pNode;
            pNode = nullptr;

            endRemoveRows();

            if( !QObject::connect(
                /* pObjSender   */ m_pReqSeq,
                /* szSignal     */ SIGNAL(requestRemoved(qint64)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onRequestRemoved(qint64)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

        } // if( pNode != m_pRootEntry )

        m_pReqSeq->unlock();

    } // if( i_modelIdx.isValid() )

} // removeEntry

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CModelRequestSequencer::onRequestAdded( qint64 i_iReqId )
//------------------------------------------------------------------------------
{
    m_pReqSeq->lock();

    SRequestSeqEntry* pReqSeqEntry = m_pReqSeq->getRequestSeqEntry(i_iReqId);

    if( pReqSeqEntry != nullptr )
    {
        SRequestSeqEntryModelNode* pNodeParent = nullptr;

        if( pReqSeqEntry->m_reqDscr.m_iParentId < 0 )
        {
            pNodeParent = m_pRootEntry;
        }
        else if( m_hshRequests.contains(pReqSeqEntry->m_reqDscr.m_iParentId) )
        {
            pNodeParent = m_hshRequests[pReqSeqEntry->m_reqDscr.m_iParentId];
        }

        if( pNodeParent != nullptr )
        {
            addNode(pNodeParent, pReqSeqEntry);
        }
    } // if( pReqSeqEntry != nullptr )

    m_pReqSeq->unlock();

} // onRequestAdded

//------------------------------------------------------------------------------
void CModelRequestSequencer::onRequestRemoved( qint64 i_iReqId )
//------------------------------------------------------------------------------
{
    SRequestSeqEntryModelNode* pNode = nullptr;

    if( m_hshRequests.contains(i_iReqId) )
    {
        pNode = m_hshRequests[i_iReqId];
    }

    if( pNode != nullptr && pNode->m_pParentNode != nullptr )
    {
        clearNode(pNode);

        int iRow = pNode->m_pParentNode->m_arpChildNodes.indexOf(pNode);

        if( iRow >= 0 )
        {
            QModelIndex modelIdx = createIndex(iRow, 0, pNode);

            QModelIndex modelIdxParent = parent(modelIdx);

            beginRemoveRows( modelIdxParent, iRow, iRow );

            m_hshRequests.remove(pNode->m_reqSeqEntry.m_reqDscr.m_iId);

            pNode->m_pParentNode->m_arpChildNodes.removeOne(pNode);

            if( pNode->m_pParentNode != m_pRootEntry )
            {
                pNode->m_pParentNode->m_reqSeqEntry.m_reqDscr.m_ariChildIds.removeOne(pNode->m_reqSeqEntry.m_reqDscr.m_iId);
            }

            delete pNode;
            pNode = nullptr;

            endRemoveRows();

        } // if( iRow >= 0 )
    } // if( pNode != nullptr && pNode->m_pParentNode != nullptr )

} // onRequestRemoved

//------------------------------------------------------------------------------
void CModelRequestSequencer::onRequestChanged( qint64 i_iReqIdOld, qint64 i_iReqIdNew )
//------------------------------------------------------------------------------
{
    m_pReqSeq->lock();

    SRequestSeqEntryModelNode* pNode = nullptr;

    if( m_hshRequests.contains(i_iReqIdOld) )
    {
        pNode = m_hshRequests[i_iReqIdOld];
    }

    if( pNode != nullptr && pNode->m_pParentNode != nullptr )
    {
        SRequestSeqEntry* pReqSeqEntry = m_pReqSeq->getRequestSeqEntry(i_iReqIdNew); // may already be nullptr

        int iRow = pNode->m_pParentNode->m_arpChildNodes.indexOf(pNode);

        QModelIndex modelIdxTL = createIndex(iRow, 0, pNode);
        QModelIndex modelIdxBR = createIndex(iRow, EColumnCount-1, pNode);

        if( pReqSeqEntry == nullptr )
        {
            pNode->m_reqSeqEntry.m_reqDscr.m_objState = EObjState::Destroyed;
        }
        else
        {
            pNode->m_reqSeqEntry.m_reqDscr = pReqSeqEntry->m_reqDscr;

            pNode->m_reqSeqEntry.m_iReqIdPredecessor = pReqSeqEntry->m_iReqIdPredecessor;
            pNode->m_reqSeqEntry.m_pObjFctExecute    = pReqSeqEntry->m_pObjFctExecute;
            pNode->m_reqSeqEntry.m_fctExecuteType1   = pReqSeqEntry->m_fctExecuteType1;
            pNode->m_reqSeqEntry.m_fctExecuteType2   = pReqSeqEntry->m_fctExecuteType2;
        }

        if( i_iReqIdOld != i_iReqIdNew )
        {
            m_hshRequests.remove(i_iReqIdOld);
            m_hshRequests[i_iReqIdNew] = pNode;

            if( pNode->m_pParentNode != m_pRootEntry )
            {
                pNode->m_pParentNode->m_reqSeqEntry.m_reqDscr.m_ariChildIds[iRow] = i_iReqIdNew;
            }
        }

        emit dataChanged(modelIdxTL, modelIdxBR);

    } // if( pNode != nullptr && pNode->m_pParentNode != nullptr )

    m_pReqSeq->unlock();

} // onRequestChanged

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
double CModelRequestSequencer::getDurationInSec( SRequestSeqEntryModelNode* i_pNode ) const
//------------------------------------------------------------------------------
{
    double fDuration_s = 0.0;

    qint64 iReqId = i_pNode->m_reqSeqEntry.m_reqDscr.m_iId;

    if( m_hshRequests.contains(iReqId) )
    {
        if( i_pNode->m_reqSeqEntry.m_reqDscr.m_iProgress_perCent >= 100 )
        {
            fDuration_s = i_pNode->m_reqSeqEntry.m_reqDscr.m_fTimeEnd_s - i_pNode->m_reqSeqEntry.m_reqDscr.m_fTimeStart_s;
        }
        else
        {
            fDuration_s = ZS::System::Time::getProcTimeInSec() - i_pNode->m_reqSeqEntry.m_reqDscr.m_fTimeStart_s;
        }
    }
    return fDuration_s;

} // getDurationInSec

//------------------------------------------------------------------------------
QString CModelRequestSequencer::getDurationAsStrInBestUnit( SRequestSeqEntryModelNode* i_pNode ) const
//------------------------------------------------------------------------------
{
    QString strDuration;
    double  fDuration_s = getDurationInSec(i_pNode);

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

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CModelRequestSequencer::onTmrRefreshTimeout()
//------------------------------------------------------------------------------
{
    updateNode(m_pRootEntry);
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelRequestSequencer::addNode( SRequestSeqEntryModelNode* i_pNodeParent, SRequestSeqEntry* i_pReqSeqEntry )
//------------------------------------------------------------------------------
{
    SRequestSeqEntryModelNode* pNodeParent = i_pNodeParent;
    SRequestSeqEntryModelNode* pNode = nullptr;
    int                        iRow;

    if( pNodeParent == nullptr )
    {
        pNodeParent = m_pRootEntry;
    }

    if( m_hshRequests.contains(i_pReqSeqEntry->m_reqDscr.m_iId) )
    {
        pNode = m_hshRequests[i_pReqSeqEntry->m_reqDscr.m_iId];
    }
    else // if( !m_hshRequests.contains(i_pReqSeqEntry->m_iId) )
    {
        pNode = new SRequestSeqEntryModelNode();

        pNode->m_reqSeqEntry.m_reqEntryType      = i_pReqSeqEntry->m_reqEntryType;
        pNode->m_reqSeqEntry.m_reqDscr           = i_pReqSeqEntry->m_reqDscr;
        pNode->m_reqSeqEntry.m_iReqIdPredecessor = i_pReqSeqEntry->m_iReqIdPredecessor;
        pNode->m_reqSeqEntry.m_pObjFctExecute    = i_pReqSeqEntry->m_pObjFctExecute;
        pNode->m_reqSeqEntry.m_fctExecuteType1   = i_pReqSeqEntry->m_fctExecuteType1;
        pNode->m_reqSeqEntry.m_fctExecuteType2   = i_pReqSeqEntry->m_fctExecuteType2;

        iRow = pNodeParent->m_arpChildNodes.size();
        pNodeParent->m_arpChildNodes.append(pNode);
        pNodeParent->m_reqSeqEntry.m_reqDscr.m_ariChildIds.append(i_pReqSeqEntry->m_reqDscr.m_iId);
        pNode->m_pParentNode = pNodeParent;

        QModelIndex modelIdx = createIndex(iRow, 0, pNode);

        QModelIndex modelIdxParent = parent(modelIdx);

        beginInsertRows(modelIdxParent, iRow, iRow);

        m_hshRequests[i_pReqSeqEntry->m_reqDscr.m_iId] = pNode;

        endInsertRows();

    } // if( !m_hshRequests.contains(i_pReqSeqEntry->m_iId) )

    // If the added node already has children they will also be added to the model.
    //-----------------------------------------------------------------------------

    SRequestSeqEntry* pReqSeqEntry;
    qint64            iReqId;

    for( iRow = 0; iRow < i_pReqSeqEntry->m_reqDscr.m_ariChildIds.size(); iRow++ )
    {
        iReqId = i_pReqSeqEntry->m_reqDscr.m_ariChildIds[iRow];
        pReqSeqEntry = m_pReqSeq->getRequestSeqEntry(iReqId);
        addNode( pNode, pReqSeqEntry );
    }

} // addNode

//------------------------------------------------------------------------------
void CModelRequestSequencer::clearNode( SRequestSeqEntryModelNode* i_pNode )
//------------------------------------------------------------------------------
{
    if( i_pNode->m_arpChildNodes.size() > 0 )
    {
        int iRow = 0;

        if( i_pNode->m_pParentNode != nullptr )
        {
            iRow = i_pNode->m_pParentNode->m_arpChildNodes.indexOf(i_pNode);
        }

        QModelIndex modelIdx = createIndex(iRow, 0, i_pNode);

        SRequestSeqEntryModelNode* pNodeChild;

        for( iRow = i_pNode->m_arpChildNodes.size()-1; iRow >= 0; iRow-- )
        {
            pNodeChild = i_pNode->m_arpChildNodes[iRow];

            clearNode(pNodeChild);

            beginRemoveRows( modelIdx, iRow, iRow );

            m_hshRequests.remove(pNodeChild->m_reqSeqEntry.m_reqDscr.m_iId);

            i_pNode->m_arpChildNodes.removeLast();

            if( i_pNode->m_pParentNode != m_pRootEntry )
            {
                i_pNode->m_reqSeqEntry.m_reqDscr.m_ariChildIds.removeOne(pNodeChild->m_reqSeqEntry.m_reqDscr.m_iId);
            }

            delete pNodeChild;
            pNodeChild = nullptr;

            endRemoveRows();
        }
    } // if( i_pNode->m_arpChildNodes.size() > 0 )

} // clearNode

//------------------------------------------------------------------------------
void CModelRequestSequencer::updateNode( SRequestSeqEntryModelNode* i_pNode )
//------------------------------------------------------------------------------
{
    int iRow;

    if( i_pNode->m_reqSeqEntry.m_reqDscr.m_objState == EObjState::Created )
    {
        if( i_pNode->m_reqSeqEntry.m_reqDscr.m_iProgress_perCent < 100 )
        {
            int iRow = 0;

            if( i_pNode->m_pParentNode != nullptr )
            {
                iRow = i_pNode->m_pParentNode->m_arpChildNodes.indexOf(i_pNode);
            }

            QModelIndex modelIdxTL = createIndex(iRow, 0, i_pNode);
            QModelIndex modelIdxBR = createIndex(iRow, EColumnCount-1, i_pNode);

            emit dataChanged(modelIdxTL,modelIdxBR);
        }
    }

    SRequestSeqEntryModelNode* pNodeChild;

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
int CModelRequestSequencer::rowCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    int iRowCount = 0;

    if( !i_modelIdxParent.isValid() )
    {
        iRowCount = 1;
    }
    else
    {
        SRequestSeqEntryModelNode* pNodeParent = static_cast<SRequestSeqEntryModelNode*>(i_modelIdxParent.internalPointer());

        if( pNodeParent != nullptr )
        {
            iRowCount = pNodeParent->m_arpChildNodes.size();
        }
    }

    return iRowCount;

} // rowCount

//------------------------------------------------------------------------------
int CModelRequestSequencer::columnCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return EColumnCount;
}

//------------------------------------------------------------------------------
QVariant CModelRequestSequencer::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    if( !i_modelIdx.isValid() )
    {
        return varData;
    }

    SRequestSeqEntryModelNode* pNode = static_cast<SRequestSeqEntryModelNode*>(i_modelIdx.internalPointer());

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

                if( pNode == m_pRootEntry )
                {
                    strNodeName = pNode->m_pReqSeq->objectName();
                }
                else if( !pNode->m_reqSeqEntry.m_reqDscr.m_strCreatorObjName.isEmpty() )
                {
                    strNodeName = pNode->m_reqSeqEntry.m_reqDscr.m_strCreatorObjName + ".";
                }
                else if( !pNode->m_reqSeqEntry.m_reqDscr.m_strCreatorClassName.isEmpty() )
                {
                    strNodeName = pNode->m_reqSeqEntry.m_reqDscr.m_strCreatorClassName + ".";
                }
                varData = strNodeName + pNode->m_reqSeqEntry.m_reqDscr.m_strRequest;
            }
            else if( i_iRole == Qt::DecorationRole )
            {
                varData = GetIcon(pNode->m_reqSeqEntry.m_reqEntryType);
            }
            break;
        }
        case EColumnRequest:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( pNode != m_pRootEntry )
                {
                    varData = QString::number(pNode->m_reqSeqEntry.m_reqDscr.m_request);
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
                    varData = QString::number(pNode->m_reqSeqEntry.m_reqDscr.m_iId);
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
                    if( pNode->m_reqSeqEntry.m_reqDscr.m_iParentId >= 0 )
                    {
                        varData = QString::number(pNode->m_reqSeqEntry.m_reqDscr.m_iParentId);
                    }
                    else
                    {
                        varData = "-";
                    }
                }
            }
            break;
        }
        case EColumnPredecessorId:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( pNode != m_pRootEntry )
                {
                    if( pNode->m_reqSeqEntry.m_iReqIdPredecessor >= 0 )
                    {
                        varData = QString::number(pNode->m_reqSeqEntry.m_iReqIdPredecessor);
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
                    varData = pNode->m_reqSeqEntry.m_reqDscr.m_strCreatorObjName;
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
                    varData = pNode->m_reqSeqEntry.m_reqDscr.m_strCreatorNameSpace;
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
                    varData = pNode->m_reqSeqEntry.m_reqDscr.m_strCreatorClassName;
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
                    varData = pNode->m_reqSeqEntry.m_reqDscr.m_strCreatorThreadName;
                }
            }
            break;
        }
        case EColumnThreadName:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( pNode != m_pRootEntry )
                {
                    varData = pNode->m_reqSeqEntry.m_reqDscr.m_strThreadName;
                }
            }
            break;
        }
        case EColumnObjFctExecute:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( pNode != m_pRootEntry )
                {
                    if( pNode->m_reqSeqEntry.m_pObjFctExecute != nullptr )
                    {
                        varData = pNode->m_reqSeqEntry.m_pObjFctExecute->objectName();
                    }
                    else
                    {
                        varData = "(null)";
                    }
                }
            }
            break;
        }
        case EColumnFctExecute:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( pNode != m_pRootEntry )
                {
                    if( pNode->m_reqSeqEntry.m_fctExecuteType1 != nullptr )
                    {
                        varData = "FctType1( QObject*, qint64 )";
                    }
                    else if( pNode->m_reqSeqEntry.m_fctExecuteType2 != nullptr )
                    {
                        varData = "FctType2( QObject*, SRequestDscr* )";
                    }
                    else
                    {
                        varData = "-";
                    }
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
                    varData = QString::number(pNode->m_reqSeqEntry.m_reqDscr.m_iProgress_perCent);
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
                    QString strErrInfo = pNode->m_reqSeqEntry.m_reqDscr.m_errResultInfo.getResultStr();

                    if( pNode->m_reqSeqEntry.m_reqDscr.m_errResultInfo.isErrorResult() )
                    {
                        strErrInfo += " (" + pNode->m_reqSeqEntry.m_reqDscr.m_errResultInfo.getSeverityStr();
                        strErrInfo += ", " + pNode->m_reqSeqEntry.m_reqDscr.m_errResultInfo.getErrSource().toString();
                        strErrInfo += ", " + pNode->m_reqSeqEntry.m_reqDscr.m_errResultInfo.getAddErrInfoDscr() + " )";
                    }
                    varData = strErrInfo;
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
                    if( pNode->m_reqSeqEntry.m_reqDscr.m_objState == EObjState::Detached )
                    {
                        varData = "-";
                    }
                    else
                    {
                        varData = bool2Str(pNode->m_reqSeqEntry.m_reqDscr.m_bIsBlocking);
                    }
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
                    if( pNode->m_reqSeqEntry.m_reqDscr.m_objState == EObjState::Detached )
                    {
                        varData = "-";
                    }
                    else
                    {
                        double fTimeout_s = pNode->m_reqSeqEntry.m_reqDscr.m_iTimeout_ms/1000.0;
                        varData = QString::number(fTimeout_s,'f',3) + " s";
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
                    if( pNode->m_reqSeqEntry.m_reqDscr.m_objState == EObjState::Detached )
                    {
                        varData = "-";
                    }
                    else
                    {
                        varData = QString::number(pNode->m_reqSeqEntry.m_reqDscr.m_fTimeStart_s,'f',3) + " s";
                    }
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
                    if( pNode->m_reqSeqEntry.m_reqDscr.m_objState == EObjState::Detached )
                    {
                        varData = "-";
                    }
                    else
                    {
                        varData = QString::number(pNode->m_reqSeqEntry.m_reqDscr.m_fTimeEnd_s,'f',3) + " s";
                    }
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
                    if( pNode->m_reqSeqEntry.m_reqDscr.m_objState == EObjState::Detached )
                    {
                        varData = "-";
                    }
                    else
                    {
                        varData = getDurationAsStrInBestUnit(pNode);
                    }
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
                    varData = CEnumObjState::toString(pNode->m_reqSeqEntry.m_reqDscr.m_objState);
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
                    varData = pNode->m_reqSeqEntry.m_reqDscr.m_strDscr;
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
                    varData = pNode->m_reqSeqEntry.m_reqDscr.m_strToolTip;
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
QModelIndex CModelRequestSequencer::index( int i_iRow, int i_iCol, const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    SRequestSeqEntryModelNode* pNodeParent = nullptr;
    SRequestSeqEntryModelNode* pNode;
    QModelIndex                modelIdx;

    if( i_iRow >= 0 )
    {
        if( !i_modelIdxParent.isValid() )
        {
            if( i_iRow == 0 )
            {
                modelIdx = createIndex(i_iRow, i_iCol, m_pRootEntry);
            }
        }
        else
        {
            pNodeParent = static_cast<SRequestSeqEntryModelNode*>(i_modelIdxParent.internalPointer());

            if( pNodeParent->m_arpChildNodes.size() > 0 && i_iRow < pNodeParent->m_arpChildNodes.size() )
            {
                pNode = pNodeParent->m_arpChildNodes[i_iRow];
                modelIdx = createIndex(i_iRow, i_iCol, pNode);
            }
        }
    }
    return modelIdx;

} // index

//------------------------------------------------------------------------------
QModelIndex CModelRequestSequencer::parent( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QModelIndex modelIdxParent;

    if( i_modelIdx.isValid() )
    {
        SRequestSeqEntryModelNode* pNode = static_cast<SRequestSeqEntryModelNode*>(i_modelIdx.internalPointer());
        SRequestSeqEntryModelNode* pNodeParent;

        if( pNode != nullptr )
        {
            pNodeParent = pNode->m_pParentNode;

            if( pNodeParent != nullptr )
            {
                int iRow = 0;

                if( pNodeParent != m_pRootEntry )
                {
                    SRequestSeqEntryModelNode* pNodeParentParent = pNodeParent->m_pParentNode;

                    if( pNodeParentParent != nullptr )
                    {
                        iRow = pNodeParentParent->m_arpChildNodes.indexOf(pNodeParent);
                    }
                }
                modelIdxParent = createIndex(iRow, 0, pNodeParent);
            }
        }
    } // if( i_modelIdx.isValid() )

    return modelIdxParent;

} // parent

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CModelRequestSequencer::headerData(
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
            case EColumnPredecessorId:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Predecessor-ID";
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
                    varData = "Creator Thread Name";
                }
                break;
            }
            case EColumnThreadName:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Request Thread Name";
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
            case EColumnObjFctExecute:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Executer";
                }
                break;
            }
            case EColumnFctExecute:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "FctExecute";
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
