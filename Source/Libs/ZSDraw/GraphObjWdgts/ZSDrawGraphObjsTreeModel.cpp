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
#include <QtGui/QPainter>

#include "ZSDraw/GraphObjWdgts/ZSDrawGraphObjsTreeModel.h"
#include "ZSDraw/GraphObjs/ZSDrawGraphObj.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/GraphObjFactories/ZSDrawObjFactory.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Draw;


/*******************************************************************************
class ZSDRAWDLL_API CModelIdxTreeGraphObjs : public ZS::System::GUI::CModelIdxTree
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelIdxTreeGraphObjs::CModelIdxTreeGraphObjs(
    CDrawingScene* i_pDrawingScene,
    QObject*       i_pObjParent ) :
//------------------------------------------------------------------------------
    CModelIdxTree(i_pDrawingScene->getGraphObjsIdxTree(), i_pObjParent),
    m_pDrawingScene(i_pDrawingScene),
    m_iconRootEntry(),
    m_iconSelectionPointEntry(),
    m_iconLabelEntry(),
    m_iconBranchEntry(),
    m_iconLeaveEntry()
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    QPixmap pxmRootEntryNormalOff(":/ZS/TreeView/FileExplorer/TreeViewFileExplorerBranchEntryNormalOff.bmp");
    pxmRootEntryNormalOff.setMask(pxmRootEntryNormalOff.createHeuristicMask());

    QPixmap pxmRootEntryNormalOn(":/ZS/TreeView/FileExplorer/TreeViewFileExplorerBranchEntryNormalOn.bmp");
    pxmRootEntryNormalOn.setMask(pxmRootEntryNormalOn.createHeuristicMask());

    QPixmap pxmRootEntryDisabledOff(":/ZS/TreeView/FileExplorer/TreeViewFileExplorerBranchEntryDisabledOff.bmp");
    pxmRootEntryDisabledOff.setMask(pxmRootEntryDisabledOff.createHeuristicMask());

    QPixmap pxmRootEntryDisabledOn(":/ZS/TreeView/FileExplorer/TreeViewFileExplorerBranchEntryDisabledOn.bmp");
    pxmRootEntryDisabledOn.setMask(pxmRootEntryDisabledOn.createHeuristicMask());

    QPixmap pxmRootEntryActiveOff(":/ZS/TreeView/FileExplorer/TreeViewFileExplorerBranchEntryActiveOff.bmp");
    pxmRootEntryActiveOff.setMask(pxmRootEntryActiveOff.createHeuristicMask());

    QPixmap pxmRootEntryActiveOn(":/ZS/TreeView/FileExplorer/TreeViewFileExplorerBranchEntryActiveOn.bmp");
    pxmRootEntryActiveOn.setMask(pxmRootEntryActiveOn.createHeuristicMask());

    QPixmap pxmRootEntrySelectedOff(":/ZS/TreeView/FileExplorer/TreeViewFileExplorerBranchEntrySelectedOff.bmp");
    pxmRootEntrySelectedOff.setMask(pxmRootEntrySelectedOff.createHeuristicMask());

    QPixmap pxmRootEntrySelectedOn(":/ZS/TreeView/FileExplorer/TreeViewFileExplorerBranchEntrySelectedOn.bmp");
    pxmRootEntrySelectedOn.setMask(pxmRootEntrySelectedOn.createHeuristicMask());

    m_iconRootEntry.addPixmap(pxmRootEntryNormalOff, QIcon::Normal, QIcon::Off);
    m_iconRootEntry.addPixmap(pxmRootEntryNormalOn, QIcon::Normal, QIcon::On);
    m_iconRootEntry.addPixmap(pxmRootEntryActiveOff, QIcon::Active, QIcon::Off);
    m_iconRootEntry.addPixmap(pxmRootEntryActiveOn, QIcon::Active, QIcon::On);
    m_iconRootEntry.addPixmap(pxmRootEntryDisabledOff, QIcon::Disabled, QIcon::Off);
    m_iconRootEntry.addPixmap(pxmRootEntryDisabledOn, QIcon::Disabled, QIcon::On);
    m_iconRootEntry.addPixmap(pxmRootEntrySelectedOff, QIcon::Selected, QIcon::Off);
    m_iconRootEntry.addPixmap(pxmRootEntrySelectedOn, QIcon::Selected, QIcon::On);

    QPixmap pxmSelectionPointNormalOff(":/ZS/Draw/DrawSelectionPoint16x16.bmp");
    pxmSelectionPointNormalOff.setMask(pxmSelectionPointNormalOff.createHeuristicMask());

    QPixmap pxmSelectionPointEntrySelectedOff(":/ZS/Draw/DrawSelectionPoint16x16.bmp");
    pxmSelectionPointEntrySelectedOff.setMask(pxmSelectionPointEntrySelectedOff.createHeuristicMask());

    m_iconSelectionPointEntry.addPixmap(pxmSelectionPointNormalOff, QIcon::Normal, QIcon::Off);
    m_iconSelectionPointEntry.addPixmap(pxmSelectionPointEntrySelectedOff, QIcon::Selected, QIcon::Off);

    QPixmap pxmLabelNormalOff(":/ZS/Draw/DrawLabel16x16.bmp");
    pxmLabelNormalOff.setMask(pxmLabelNormalOff.createHeuristicMask());

    QPixmap pxmLabelEntrySelectedOff(":/ZS/Draw/DrawLabel16x16.bmp");
    pxmLabelEntrySelectedOff.setMask(pxmLabelEntrySelectedOff.createHeuristicMask());

    m_iconLabelEntry.addPixmap(pxmLabelNormalOff, QIcon::Normal, QIcon::Off);
    m_iconLabelEntry.addPixmap(pxmLabelEntrySelectedOff, QIcon::Selected, QIcon::Off);

    QPixmap pxmBranchNormalOff(":/ZS/TreeView/TreeViewBranchEntry.bmp");
    pxmBranchNormalOff.setMask(pxmBranchNormalOff.createHeuristicMask());

    QPixmap pxmBranchEntrySelectedOff(":/ZS/TreeView/TreeViewBranchEntry.bmp");
    pxmBranchEntrySelectedOff.setMask(pxmBranchEntrySelectedOff.createHeuristicMask());

    m_iconBranchEntry.addPixmap(pxmBranchNormalOff, QIcon::Normal, QIcon::Off);
    m_iconBranchEntry.addPixmap(pxmBranchEntrySelectedOff, QIcon::Selected, QIcon::Off);

    QPixmap pxmLeaveNormalOff(":/ZS/TreeView/TreeViewLeaveEntry.bmp");
    pxmLeaveNormalOff.setMask(pxmLeaveNormalOff.createHeuristicMask());

    QPixmap pxmLeaveEntrySelectedOff(":/ZS/TreeView/TreeViewLeaveEntry.bmp");
    pxmLeaveEntrySelectedOff.setMask(pxmLeaveEntrySelectedOff.createHeuristicMask());

    m_iconLeaveEntry.addPixmap(pxmLeaveNormalOff, QIcon::Normal, QIcon::Off);
    m_iconLeaveEntry.addPixmap(pxmLeaveEntrySelectedOff, QIcon::Selected, QIcon::Off);

} // ctor

//------------------------------------------------------------------------------
CModelIdxTreeGraphObjs::~CModelIdxTreeGraphObjs()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_pDrawingScene = nullptr;
    //m_iconRootEntry;
    //m_iconSelectionPointEntry;
    //m_iconLabelEntry;
    //m_iconBranchEntry;
    //m_iconLeaveEntry;

} // dtor

/*==============================================================================
public: // must overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CModelIdxTreeGraphObjs::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if(m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "ModelIdx {" + modelIdx2Str(i_modelIdx) + "}";
        strMthInArgs += ", Role: " + qItemDataRole2Str(i_iRole);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjNoisyMethods,
        /* iFilterLevel */ EMethodTraceDetailLevel::EnterLeave,
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
        pIdxTreeEntry = m_pIdxTree->findEntry(pModelTreeEntry->keyInTree());
    }

    if( pIdxTreeEntry != nullptr )
    {
        QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(pIdxTreeEntry);
        CGraphObj*     pGraphObj     = dynamic_cast<CGraphObj*>(pIdxTreeEntry);

        if( i_modelIdx.column() == EColumnTreeEntryName )
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
                else if( pGraphObj != nullptr && pGraphicsItem != nullptr )
                {
                    CObjFactory* pObjFactory = nullptr;

                    pObjFactory = CObjFactory::FindObjFactory(pGraphObj->getFactoryGroupName(), pGraphObj->typeAsString());

                    if( pObjFactory != nullptr )
                    {
                        QPixmap pxm = pObjFactory->getToolIconPixmap();
                        if( pGraphicsItem->isSelected() )
                        {
                            QPainter painter(&pxm);
                            painter.setPen(Qt::red);
                            painter.setBrush(Qt::white);
                            QSize size = pxm.size();
                            painter.drawEllipse(0, size.height()/2-3, 6, 6);
                        }
                        varData = pxm;
                    }
                    else if( pGraphObj->isSelectionPoint() )
                    {
                        varData = m_iconSelectionPointEntry;
                    }
                    else if( pGraphObj->isLabel() )
                    {
                        varData = m_iconLabelEntry;
                    }
                    else if( pIdxTreeEntry->isBranch() )
                    {
                        varData = m_iconBranchEntry;
                    }
                    else
                    {
                        varData = m_iconLeaveEntry;
                    }
                    #pragma message(__TODO__"Small icons for states")
                    if( pGraphicsItem->isActive() ) {
                    }
                    if( pGraphicsItem->isEnabled() ) {
                    }
                    if( pGraphicsItem->isSelected() ) {
                    }
                } // if( pGraphObj != nullptr && pGraphicsItem != nullptr )
            } // if( i_iRole == Qt::DecorationRole )
        } // if( i_modelIdx.column() == EColumnTreeEntryName )

        else {
            varData = CModelIdxTree::data(i_modelIdx, i_iRole);
        }
    } // if( pIdxTreeEntry != nullptr )

    return varData;

} // data
