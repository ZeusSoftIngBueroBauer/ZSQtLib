/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
                         Gewerbepark 28
                         D-83670 Bad Heilbrunn
                         Tel: 0049 8046 9488
                         www.zeussoft.de
                         E-Mail: mailbox@zeussoft.de

--------------------------------------------------------------------------------

Content: This file is part of the ZSQtLib.

*******************************************************************************/

#include <QtGui/QBitmap>

#include "ZSDraw/ZSDrawGraphicsItemsModel.h"
#include "ZSDraw/ZSDrawGraphObj.h"
#include "ZSDraw/ZSDrawObjFactory.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Draw;
using namespace ZS::Trace;


/*******************************************************************************
class ZSDRAWDLL_API CModelIdxTreeGraphicsItems : public ZS::System::GUI::CModelIdxTree
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelIdxTreeGraphicsItems::CModelIdxTreeGraphicsItems(
    CIdxTree* i_pIdxTree,
    QObject*  i_pObjParent ) :
//------------------------------------------------------------------------------
    CModelIdxTree(i_pIdxTree, i_pObjParent),
    m_iconRootEntry(),
    m_pTrcAdminObj(nullptr)
{
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    QPixmap pxm(":/ZS/IdxTreeExplorer/IdxTreeEntryTypeNameSpaceNormalOff.bmp");
    pxm.setMask(pxm.createHeuristicMask());

    m_iconRootEntry.addPixmap(pxm);

} // ctor

//------------------------------------------------------------------------------
CModelIdxTreeGraphicsItems::~CModelIdxTreeGraphicsItems()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    //m_iconRootEntry;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // must overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelIdxTreeGraphicsItems::columnCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if(m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelVerbose))
    {
        strMthInArgs = "ModelIdxParent {" + ModelIdx2Str(i_modelIdxParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelVerbose,
        /* strMethod    */ "columnCount",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelVerbose) )
    {
        mthTracer.setMethodReturn(EColumnCount);
    }

    return EColumnCount;

} // columnCount

//------------------------------------------------------------------------------
QVariant CModelIdxTreeGraphicsItems::headerData(
    int             i_iSection,
    Qt::Orientation i_orientation,
    int             i_iRole ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if(m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelVerbose))
    {
        strMthInArgs = "Section: " + QString::number(i_iSection);
        strMthInArgs += ", Orientation: " + qOrientation2Str(i_orientation);
        strMthInArgs += ", Role: " + qItemDataRole2Str(i_iRole);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelVerbose,
        /* strMethod    */ "headerData",
        /* strAddInfo   */ strMthInArgs );

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

//------------------------------------------------------------------------------
QVariant CModelIdxTreeGraphicsItems::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if(m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelVerbose))
    {
        strMthInArgs = "ModelIdx {" + ModelIdx2Str(i_modelIdx) + "}";
        strMthInArgs += ", Role: " + qItemDataRole2Str(i_iRole);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelVerbose,
        /* strMethod    */ "data",
        /* strAddInfo   */ strMthInArgs );

    QVariant varData;

    CModelIdxTreeEntry* pModelTreeEntry = nullptr;

    if( i_modelIdx.isValid() )
    {
        pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
    }

    CIdxTreeEntry* pIdxTreeEntry = nullptr;

    if( pModelTreeEntry != nullptr )
    {
        pIdxTreeEntry = dynamic_cast<CIdxTreeEntry*>(pModelTreeEntry->treeEntry());
    }

    if( pIdxTreeEntry != nullptr )
    {
        QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(pIdxTreeEntry);
        CGraphObj*     pGraphObj     = dynamic_cast<CGraphObj*>(pIdxTreeEntry);

        switch( i_modelIdx.column() )
        {
            case EColumnGraphObjName:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = pIdxTreeEntry->name();
                }
                else if( i_iRole == Qt::DecorationRole )
                {
                    if( pIdxTreeEntry->isRoot() )
                    {
                        varData = m_iconRootEntry;
                    }
                    else
                    {
                        CObjFactory* pObjFactory = nullptr;

                        if( pGraphObj != nullptr )
                        {
                            pObjFactory = CObjFactory::FindObjFactory(pGraphObj->getFactoryGroupName(), pGraphObj->getTypeAsString());
                        }
                        if( pObjFactory != nullptr )
                        {
                            varData = pObjFactory->getToolIcon();
                        }
                        else
                        {
                            varData = m_iconRootEntry;
                        }
                    }
                }
                break;
            }
            case EColumnGraphObjId:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = pIdxTreeEntry->keyInTree();
                }
                break;
            }
            default:
            {
                break;
            }
        } // switch( i_modelIdx.column() )
    } // if( pIdxTreeEntry != nullptr )

    return varData;

} // data
