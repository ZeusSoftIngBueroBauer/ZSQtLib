/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
                         Gewerbepark 28
                         D-83670 Bad Heilbrunn
                         Tel: 0049 8046 9488
                         www.zeussoft.de
                         E-Mail: mailbox@zeussoft.de

--------------------------------------------------------------------------------

Content: This file is part of the ZSQtLib.

*******************************************************************************/

#if 0

#include <QtGui/QBitmap>

#include "ZSDraw/ZSDrawGraphicsItemsModel.h"
#include "ZSDraw/ZSDrawGraphObj.h"
#include "ZSDraw/ZSDrawGraphObjModelEntry.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSDraw/ZSDrawObjFactory.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;


/*******************************************************************************
class CGraphicsItemsModel : public QAbstractItemModel
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphicsItemsModel::CGraphicsItemsModel( CDrawingScene* i_pDrawingScene, QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    QAbstractItemModel(i_pObjParent),
    m_iconNameSpaceEntry(),
    m_pDrawingScene(i_pDrawingScene),
    m_pRootEntry(nullptr),
    m_dctpGraphObjs(),
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjNoisyMethods(nullptr)
{
    setObjectName( m_pDrawingScene->objectName() + "GraphicsItemsModel" );

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName(), objectName());
    m_pTrcAdminObjNoisyMethods = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::NoisyMethods", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    QPixmap pxm(":/ZS/ObjPoolExplorer/ObjPoolEntryTypeNameSpaceNormalOff.bmp");
    pxm.setMask(pxm.createHeuristicMask());

    m_iconNameSpaceEntry.addPixmap(pxm);

    m_pRootEntry = new CGraphObjModelEntry(
        /* pModel          */ this,
        /* pDrawingScene   */ m_pDrawingScene,
        /* graphObjType    */ EGraphObjTypeUndefined,
        /* strGraphObjName */ "Drawing Scene",
        /* strGraphObjId   */ "" );

    m_pRootEntry->setRowId(0);

    if( !QObject::connect(
        /* pObjSender   */ m_pDrawingScene,
        /* szSignal     */ SIGNAL(graphObjCreated(ZS::Draw::CGraphObj*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDrawingSceneGraphObjCreated(ZS::Draw::CGraphObj*)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pDrawingScene,
        /* szSignal     */ SIGNAL(graphObjDestroying(const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDrawingSceneGraphObjDestroying(const QString&)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pDrawingScene,
        /* szSignal     */ SIGNAL(graphObjIdChanged(const QString&,const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDrawingSceneGraphObjIdChanged(const QString&,const QString&)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pDrawingScene,
        /* szSignal     */ SIGNAL(graphObjNameChanged(const QString&,const QString&,const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDrawingSceneGraphObjNameChanged(const QString&,const QString&,const QString&)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

} // ctor

//------------------------------------------------------------------------------
CGraphicsItemsModel::~CGraphicsItemsModel()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    clear();

    beginRemoveRows( QModelIndex(), m_pRootEntry->getRowId(), m_pRootEntry->getRowId() );

    try {
        delete m_pRootEntry;
    } catch(...) {
    }
    m_pRootEntry = nullptr;

    endRemoveRows();

    //m_iconNameSpaceEntry;
    m_pDrawingScene = nullptr;
    m_pRootEntry = nullptr;
    m_dctpGraphObjs.clear();

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjNoisyMethods);
    m_pTrcAdminObjNoisyMethods = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphicsItemsModel::clear()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "clear",
        /* strAddInfo   */ "" );

    // The root entry will not be deleted.
    if( m_pRootEntry->getChildCount() > 0 )
    {
        CGraphObjModelEntry* pModelEntryChild;
        int                  iRow;

        beginRemoveRows( m_pRootEntry->getModelIdx(), 0, m_pRootEntry->getChildCount()-1 );

        for( iRow = m_pRootEntry->getChildCount()-1; iRow >= 0; iRow-- )
        {
            pModelEntryChild = m_pRootEntry->getChildEntry(iRow);

            clearModelEntry(pModelEntryChild);

            // After the childs of the child have been removed the child may be removed.
            m_pRootEntry->removeChildEntry(iRow);

            delete pModelEntryChild;
            pModelEntryChild = nullptr;
        }

        endRemoveRows();

    } // if( m_pRootEntry->getChildCount() > 0 )

} // clear

//------------------------------------------------------------------------------
void CGraphicsItemsModel::update()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "update",
        /* strAddInfo   */ "" );

    clear();

    QList<QGraphicsItem*> arpGraphicsItems;
    QGraphicsItem*        pGraphicsItem;
    CGraphObj*            pGraphObj;
    CGraphObjModelEntry*  pModelEntry;
    int                   idxGraphObj;

    // Also includes selection points of selected objects and the selection rectangle:
    arpGraphicsItems = m_pDrawingScene->items();

    // Count number of graphical objects without selection points, labels and selection rectangle:
    int iGraphObjCount = 0;
    if( arpGraphicsItems.size() > 0 )
    {
        for( idxGraphObj = 0; idxGraphObj < arpGraphicsItems.size(); idxGraphObj++ )
        {
            pGraphicsItem = arpGraphicsItems[idxGraphObj];
            pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

            if( pGraphObj != nullptr && !pGraphObj->isSelectionPoint() && !pGraphObj->isLabel() )
            {
                iGraphObjCount++;
            }
        }
    }

    if( iGraphObjCount > 0 )
    {
        beginInsertRows( m_pRootEntry->getModelIdx(), 0, iGraphObjCount-1 );

        for( idxGraphObj = 0; idxGraphObj < arpGraphicsItems.size(); idxGraphObj++ )
        {
            pGraphicsItem = arpGraphicsItems[idxGraphObj];
            pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

            if( pGraphObj != nullptr && !pGraphObj->isSelectionPoint() && !pGraphObj->isLabel() )
            {
                pModelEntry = new CGraphObjModelEntry(
                    /* pModel          */ this,
                    /* pDrawingScene   */ m_pDrawingScene,
                    /* graphObjType    */ pGraphObj->type(),
                    /* strGraphObjName */ pGraphObj->name(),
                    /* strGraphObjId   */ pGraphObj->keyInTree() );

                m_pRootEntry->appendChildEntry(pModelEntry);

                m_dctpGraphObjs.insert( pModelEntry->getGraphObjId(), pModelEntry );
            }
        }

        endInsertRows();

    } // if( arpGraphicsItems.size() > 0 )

} // update

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjModelEntry* CGraphicsItemsModel::getModelEntry( CGraphObj* i_pGraphObj ) const
//------------------------------------------------------------------------------
{
    CGraphObjModelEntry* pModelEntry = nullptr;

    if( i_pGraphObj != nullptr )
    {
        QString strObjId = i_pGraphObj->keyInTree();

        if( m_dctpGraphObjs.contains(strObjId) )
        {
            pModelEntry = m_dctpGraphObjs[strObjId];
        }
    }
    return pModelEntry;

} // getModelEntry

//------------------------------------------------------------------------------
CGraphObjModelEntry* CGraphicsItemsModel::getModelEntry( QGraphicsItem* i_pGraphicsItem ) const
//------------------------------------------------------------------------------
{
    CGraphObjModelEntry* pModelEntry = nullptr;

    if( i_pGraphicsItem != nullptr )
    {
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);

        if( pGraphObj != nullptr )
        {
            QString strObjId = pGraphObj->keyInTree();

            if( m_dctpGraphObjs.contains(strObjId) )
            {
                pModelEntry = m_dctpGraphObjs[strObjId];
            }
        }
    }
    return pModelEntry;

} // getModelEntry

//------------------------------------------------------------------------------
CGraphObjModelEntry* CGraphicsItemsModel::getModelEntry( const QString& i_strObjId ) const
//------------------------------------------------------------------------------
{
    CGraphObjModelEntry* pModelEntry = nullptr;

    if( m_dctpGraphObjs.contains(i_strObjId) )
    {
        pModelEntry = m_dctpGraphObjs[i_strObjId];
    }
    return pModelEntry;

} // getModelEntry

//------------------------------------------------------------------------------
QModelIndex CGraphicsItemsModel::getModelIndex( CGraphObj* i_pGraphObj ) const
//------------------------------------------------------------------------------
{
    QModelIndex modelIdx;

    CGraphObjModelEntry* pModelEntry = getModelEntry(i_pGraphObj);

    if( pModelEntry != nullptr )
    {
        modelIdx = pModelEntry->getModelIdx();
    }
    return modelIdx;

} // getModelEntry

//------------------------------------------------------------------------------
QModelIndex CGraphicsItemsModel::getModelIndex( QGraphicsItem* i_pGraphicsItem ) const
//------------------------------------------------------------------------------
{
    QModelIndex modelIdx;

    CGraphObjModelEntry* pModelEntry = getModelEntry(i_pGraphicsItem);

    if( pModelEntry != nullptr )
    {
        modelIdx = pModelEntry->getModelIdx();
    }
    return modelIdx;

} // getModelEntry

//------------------------------------------------------------------------------
QModelIndex CGraphicsItemsModel::getModelIndex( const QString& i_strObjId ) const
//------------------------------------------------------------------------------
{
    QModelIndex modelIdx;

    CGraphObjModelEntry* pModelEntry = getModelEntry(i_strObjId);

    if( pModelEntry != nullptr )
    {
        modelIdx = pModelEntry->getModelIdx();
    }
    return modelIdx;

} // getModelEntry

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphicsItemsModel::clearModelEntry( CGraphObjModelEntry* i_pModelEntry )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        if( i_pModelEntry == nullptr )
        {
            strAddTrcInfo = "ModelEntry:nullptr";
        }
        else
        {
            strAddTrcInfo = "ModelEntry:" + i_pModelEntry->getGraphObjId();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "clearModelEntry",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_pModelEntry->getChildCount() > 0 )
    {
        CGraphObjModelEntry* pModelEntryChild;
        int                  iRow;

        beginRemoveRows( i_pModelEntry->getModelIdx(), 0, i_pModelEntry->getChildCount()-1 );

        for( iRow = i_pModelEntry->getChildCount()-1; iRow >= 0; iRow-- )
        {
            pModelEntryChild = i_pModelEntry->getChildEntry(iRow);

            clearModelEntry(pModelEntryChild);

            // After the childs of the child have been removed the child may be removed.
            i_pModelEntry->removeChildEntry(iRow);

            m_dctpGraphObjs.remove( i_pModelEntry->getGraphObjId() );

            delete pModelEntryChild;
            pModelEntryChild = nullptr;
        }

        endRemoveRows();

    } // if( i_pTreeEntry->getChildCount() > 0 )

} // clearTreeEntry

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphicsItemsModel::onDrawingSceneGraphObjCreated( ZS::Draw::CGraphObj* i_pGraphObj )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        if( i_pGraphObj == nullptr )
        {
            strAddTrcInfo = "GraphObj:nullptr";
        }
        else
        {
            strAddTrcInfo = "GraphObj:" + i_pGraphObj->keyInTree();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingSceneGraphObjCreated",
        /* strAddInfo   */ strAddTrcInfo );

    if( !i_pGraphObj->isSelectionPoint() && !i_pGraphObj->isLabel() )
    {
        beginInsertRows( m_pRootEntry->getModelIdx(), m_pRootEntry->getChildCount(), m_pRootEntry->getChildCount() );

        CGraphObjModelEntry* pModelEntry = new CGraphObjModelEntry(
            /* pModel          */ this,
            /* pDrawingScene   */ m_pDrawingScene,
            /* graphObjType    */ i_pGraphObj->type(),
            /* strGraphObjName */ i_pGraphObj->name(),
            /* strGraphObjId   */ i_pGraphObj->keyInTree() );

        m_pRootEntry->appendChildEntry(pModelEntry);

        m_dctpGraphObjs.insert( pModelEntry->getGraphObjId(), pModelEntry );

        endInsertRows();
    }

} // onDrawingSceneGraphObjCreated

//------------------------------------------------------------------------------
void CGraphicsItemsModel::onDrawingSceneGraphObjDestroying( const QString& i_strObjId )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = "ObjId:" + i_strObjId;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingSceneGraphObjDestroying",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_dctpGraphObjs.contains(i_strObjId) )
    {
        CGraphObjModelEntry* pModelEntry = m_dctpGraphObjs[i_strObjId];

        if( pModelEntry != nullptr )
        {
            clearModelEntry(pModelEntry);

            QModelIndex modelIdx = pModelEntry->getModelIdx();

            beginRemoveRows( pModelEntry->getParentModelIdx(), modelIdx.row(), modelIdx.row() );

            m_dctpGraphObjs.remove( pModelEntry->getGraphObjId() );

            CGraphObjModelEntry* pModelEntryParent = pModelEntry->getParentEntry();

            // After the childs of the child have been removed the child may be removed.
            if( pModelEntryParent != nullptr )
            {
                pModelEntryParent->removeChildEntry(pModelEntry->getRowId());
            }

            delete pModelEntry;
            pModelEntry = nullptr;

            endRemoveRows();

        } // if( pModelEntry != nullptr )

    } // if( m_dctpGraphObjs.contains(i_strObjId) )

} // onDrawingSceneGraphObjDestroying

//------------------------------------------------------------------------------
void CGraphicsItemsModel::onDrawingSceneGraphObjIdChanged(
    const QString& i_strObjIdOld,
    const QString& i_strObjIdNew )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo  = "ObjIdOld:" + i_strObjIdOld;
        strAddTrcInfo += ", ObjIdNew:" + i_strObjIdNew;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingSceneGraphObjIdChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_dctpGraphObjs.contains(i_strObjIdOld) )
    {
        CGraphObjModelEntry* pModelEntry = m_dctpGraphObjs[i_strObjIdOld];
        CGraphObj*           pGraphObj = m_pDrawingScene->findGraphObj(i_strObjIdNew);

        if( pModelEntry != nullptr )
        {
            m_dctpGraphObjs.remove( pModelEntry->getGraphObjId() );

            pModelEntry->setGraphObjId(i_strObjIdNew);

            if( pGraphObj != nullptr )
            {
                pModelEntry->setGraphObjName( pGraphObj->name() );
            }

            m_dctpGraphObjs.insert( pModelEntry->getGraphObjId(), pModelEntry );

            QModelIndex modelIdx   = pModelEntry->getModelIdx();
            QModelIndex modelIdxLT = index( modelIdx.row(), 0, pModelEntry->getParentModelIdx() );
            QModelIndex modelIdxRB = index( modelIdx.row(), 0, pModelEntry->getParentModelIdx() );

            emit dataChanged( modelIdxLT, modelIdxRB );

        } // if( pModelEntry != nullptr )

    } // if( m_dctpGraphObjs.contains(i_strObjId) )

} // onDrawingSceneGraphObjIdChanged

//------------------------------------------------------------------------------
void CGraphicsItemsModel::onDrawingSceneGraphObjNameChanged(
    const QString& i_strObjId,
    const QString& i_strObjNameOld,
    const QString& i_strObjNameNew )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo  = "ObjId:" + i_strObjId;
        strAddTrcInfo += ", ObjNameOld:" + i_strObjNameOld;
        strAddTrcInfo += ", ObjNameNew:" + i_strObjNameNew;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingSceneGraphObjNameChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_dctpGraphObjs.contains(i_strObjId) )
    {
        CGraphObjModelEntry* pModelEntry = m_dctpGraphObjs[i_strObjId];
        CGraphObj*           pGraphObj = m_pDrawingScene->findGraphObj(i_strObjId);

        if( pModelEntry != nullptr && pGraphObj != nullptr )
        {
            pModelEntry->setGraphObjName( pGraphObj->name() );

            QModelIndex modelIdx   = pModelEntry->getModelIdx();
            QModelIndex modelIdxLT = index( modelIdx.row(), 0, pModelEntry->getParentModelIdx() );
            QModelIndex modelIdxRB = index( modelIdx.row(), 0, pModelEntry->getParentModelIdx() );

            emit dataChanged( modelIdxLT, modelIdxRB );

        } // if( pModelEntry != nullptr && pGraphObj != nullptr )

    } // if( m_dctpGraphObjs.contains(i_strObjId) )

} // onDrawingSceneGraphObjNameChanged

/*==============================================================================
public: // must overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CGraphicsItemsModel::rowCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = "ModelIdxParent:" + QString::number(i_modelIdxParent.row()) + "," + QString::number(i_modelIdxParent.column());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjNoisyMethods,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "rowCount",
        /* strAddInfo   */ strAddTrcInfo );

    int iRowCount = 0;

    if( !i_modelIdxParent.isValid() )
    {
        iRowCount = 1;
    }
    else
    {
        CGraphObjModelEntry* pModelEntryParent = static_cast<CGraphObjModelEntry*>(i_modelIdxParent.internalPointer());

        if( pModelEntryParent == nullptr )
        {
            throw CException( __FILE__, __LINE__, EResultInternalProgramError, "i_modelIdxParent.internalPointer() == nullptr" );
        }
        iRowCount = pModelEntryParent->getChildCount();
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strAddTrcInfo = "RowCount:" + QString::number(iRowCount);
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return iRowCount;

} // rowCount

//------------------------------------------------------------------------------
int CGraphicsItemsModel::columnCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return EColumnCount;
}

//------------------------------------------------------------------------------
QVariant CGraphicsItemsModel::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    if( !i_modelIdx.isValid() )
    {
        return varData;
    }

    CGraphObjModelEntry* pModelEntry = static_cast<CGraphObjModelEntry*>(i_modelIdx.internalPointer());

    if( pModelEntry == nullptr )
    {
        return varData;
    }

    QGraphicsItem* pGraphicsItem = pModelEntry->getGraphicsItem();
    CGraphObj*     pGraphObj = pModelEntry->getGraphObj();

    switch( i_modelIdx.column() )
    {
        case EColumnGraphObjName:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( pModelEntry == m_pRootEntry )
                {
                    varData = "Graphics Items";
                }
                else
                {
                    if( pGraphObj != nullptr )
                    {
                        varData = pGraphObj->path();
                    }
                    else if( pGraphicsItem != nullptr )
                    {
                        varData = graphObjType2Str(pGraphicsItem->type());
                    }
                }
            }
            else if( i_iRole == Qt::DecorationRole )
            {
                if( pModelEntry == m_pRootEntry )
                {
                    varData = m_iconNameSpaceEntry;
                }
                else
                {
                    CObjFactory* pObjFactory = nullptr;

                    if( pGraphObj != nullptr )
                    {
                        pObjFactory = CObjFactory::FindObjFactory(
                            /* strNameSpace */ pGraphObj->nameSpace(),
                            /* strClassName */ pGraphObj->className() );
                    }
                    if( pObjFactory != nullptr )
                    {
                        varData = pObjFactory->getToolIconPixmap();
                    }
                    else
                    {
                        varData = m_iconNameSpaceEntry;
                    }
                }
            }
            break;
        }
        case EColumnGraphObjId:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( pModelEntry == m_pRootEntry )
                {
                }
                else
                {
                    if( pGraphObj != nullptr )
                    {
                        varData = pGraphObj->keyInTree();
                    }
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
QModelIndex CGraphicsItemsModel::index( int i_iRow, int i_iCol, const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    CGraphObjModelEntry* pModelEntryParent = nullptr;
    CGraphObjModelEntry* pModelEntry;
    QModelIndex          modelIdx;

    if( i_iRow >= 0 )
    {
        if( !i_modelIdxParent.isValid() )
        {
            if( i_iRow == 0 )
            {
                modelIdx = createIndex( i_iRow, i_iCol, m_pRootEntry );
            }
        }
        else
        {
            pModelEntryParent = static_cast<CGraphObjModelEntry*>(i_modelIdxParent.internalPointer());

            if( pModelEntryParent->getChildCount() > 0 && i_iRow < static_cast<int>(pModelEntryParent->getChildCount()) )
            {
                pModelEntry = pModelEntryParent->getChildEntry(i_iRow);
                modelIdx = createIndex(i_iRow,i_iCol,pModelEntry);
            }
        }
    }

    return modelIdx;

} // index

//------------------------------------------------------------------------------
QModelIndex CGraphicsItemsModel::parent( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QModelIndex modelIdx;

    if( i_modelIdx.isValid() )
    {
        CGraphObjModelEntry* pModelEntry = static_cast<CGraphObjModelEntry*>(i_modelIdx.internalPointer());
        CGraphObjModelEntry* pModelEntryParent;

        if( pModelEntry == nullptr )
        {
            throw CException (__FILE__, __LINE__, EResultInternalProgramError, "i_modelIdx.internalPointer() = nullptr" );
        }

        pModelEntryParent = pModelEntry->getParentEntry();

        if( pModelEntryParent != nullptr )
        {
            modelIdx = createIndex( pModelEntryParent->getRowId(), 0, pModelEntryParent );
        }
    }

    return modelIdx;

} // parent

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CGraphicsItemsModel::headerData(
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
            case EColumnGraphObjName:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Name";
                }
                else if( i_iRole == Qt::SizeHintRole )
                {
                    //varData = QSize(10,16);
                }
                break;
            }
            case EColumnGraphObjId:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Id";
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

#endif // #if 0
