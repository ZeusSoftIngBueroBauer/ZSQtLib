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

#include <QtCore/qmutex.h>
#include <QtCore/qfile.h>
#include <QtCore/qthread.h>
#include <QtCore/qtextstream.h>
#include <QtGui/qbitmap.h>
#include <QtGui/qicon.h>

#include "ZSSysGUI/ZSSysTrcAdminObjIdxTreeModel.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntries.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysTrcAdminObjIdxTree.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Trace;
using namespace ZS::Trace::GUI;


/*******************************************************************************
class CModelIdxTreeTrcAdminObjs : public ZS::Trace::CModelObjPool
*******************************************************************************/

/*==============================================================================
protected: // class members (hiding class members of base class CModelIdxTree)
==============================================================================*/

bool CModelIdxTreeTrcAdminObjs::s_bIconsCreated = false;

QPixmap* CModelIdxTreeTrcAdminObjs::s_pPxmBranchEntryNormalOff;
QPixmap* CModelIdxTreeTrcAdminObjs::s_pPxmBranchEntrySelectedOff;
QPixmap* CModelIdxTreeTrcAdminObjs::s_pPxmLeaveEntryNormalOff;
QPixmap* CModelIdxTreeTrcAdminObjs::s_pPxmLeaveEntrySelectedOff;
QIcon*   CModelIdxTreeTrcAdminObjs::s_pIconBranch;
QIcon*   CModelIdxTreeTrcAdminObjs::s_pIconLeave;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QIcon CModelIdxTreeTrcAdminObjs::GetIcon( EIdxTreeEntryType i_entryType )
//------------------------------------------------------------------------------
{
    QIcon icon;

    if( !s_bIconsCreated )
    {
        s_pPxmBranchEntryNormalOff = new QPixmap(":/ZS/TreeView/IdxTreeTrcAdminObjsBranchEntryNormalOff.bmp");
        s_pPxmBranchEntryNormalOff->setMask(s_pPxmBranchEntryNormalOff->createHeuristicMask());

        s_pPxmBranchEntrySelectedOff = new QPixmap(":/ZS/TreeView/IdxTreeTrcAdminObjsBranchEntrySelectedOff.bmp");
        s_pPxmBranchEntrySelectedOff->setMask(s_pPxmBranchEntrySelectedOff->createHeuristicMask());

        s_pPxmLeaveEntryNormalOff = new QPixmap(":/ZS/TreeView/IdxTreeTrcAdminObjsLeaveEntryNormalOff.bmp");
        s_pPxmLeaveEntryNormalOff->setMask(s_pPxmLeaveEntryNormalOff->createHeuristicMask());

        s_pPxmLeaveEntrySelectedOff = new QPixmap(":/ZS/TreeView/IdxTreeTrcAdminObjsLeaveEntrySelectedOff.bmp");
        s_pPxmLeaveEntrySelectedOff->setMask(s_pPxmLeaveEntrySelectedOff->createHeuristicMask());

        s_pIconBranch = new QIcon();

        s_pIconBranch->addPixmap(*s_pPxmBranchEntryNormalOff,   QIcon::Normal,   QIcon::Off);
        s_pIconBranch->addPixmap(*s_pPxmBranchEntrySelectedOff, QIcon::Selected, QIcon::Off);
        s_pIconBranch->addPixmap(*s_pPxmBranchEntryNormalOff,   QIcon::Normal,   QIcon::On);
        s_pIconBranch->addPixmap(*s_pPxmBranchEntrySelectedOff, QIcon::Selected, QIcon::On);

        s_pIconLeave = new QIcon();

        s_pIconLeave->addPixmap(*s_pPxmLeaveEntryNormalOff,   QIcon::Normal,   QIcon::Off);
        s_pIconLeave->addPixmap(*s_pPxmLeaveEntrySelectedOff, QIcon::Selected, QIcon::Off);
        s_pIconLeave->addPixmap(*s_pPxmLeaveEntryNormalOff,   QIcon::Normal,   QIcon::On);
        s_pIconLeave->addPixmap(*s_pPxmLeaveEntrySelectedOff, QIcon::Selected, QIcon::On);

        s_bIconsCreated = true;
    }

    switch( i_entryType )
    {
        case EIdxTreeEntryType::Root:
        case EIdxTreeEntryType::Branch:
        {
            if( s_pIconBranch != nullptr )
            {
                icon = *s_pIconBranch;
            }
            break;
        }
        case EIdxTreeEntryType::Leave:
        {
            if( s_pIconLeave != nullptr )
            {
                icon = *s_pIconLeave;
            }
            break;
        }
        default:
        {
            break;
        }
    }

    return icon;

} // GetIcon

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelIdxTreeTrcAdminObjs::CModelIdxTreeTrcAdminObjs(
    CIdxTreeTrcAdminObjs* i_pTrcAdminObjIdxTree,
    QObject*              i_pObjParent,
    int                   i_iTrcDetailLevel ) :
//------------------------------------------------------------------------------
    CModelIdxTree(
        /* pObjPool        */ i_pTrcAdminObjIdxTree,
        /* pObjParent      */ i_pObjParent,
        /* iTrcDetailLevel */ i_iTrcDetailLevel )
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "IdxTree: " + QString(i_pTrcAdminObjIdxTree == nullptr ? "nullptr" : i_pTrcAdminObjIdxTree->objectName());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );

} // ctor

//------------------------------------------------------------------------------
CModelIdxTreeTrcAdminObjs::~CModelIdxTreeTrcAdminObjs()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ strMthInArgs );

    // Please note that instance counter will be decremented by the base class CModelIdxTree.
    // If the instance counter is 1 it will be decremented to 0.
    if( s_iInstCount == 1 )
    {
        delete s_pIconBranch;
        s_pIconBranch = NULL;
        delete s_pIconLeave;
        s_pIconLeave = NULL;

        delete s_pPxmBranchEntryNormalOff;
        s_pPxmBranchEntryNormalOff = NULL;
        delete s_pPxmBranchEntrySelectedOff;
        s_pPxmBranchEntrySelectedOff = NULL;
        delete s_pPxmLeaveEntryNormalOff;
        s_pPxmLeaveEntryNormalOff = NULL;
        delete s_pPxmLeaveEntrySelectedOff;
        s_pPxmLeaveEntrySelectedOff = NULL;

        s_bIconsCreated = false;
    }
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CIdxTreeTrcAdminObjs* CModelIdxTreeTrcAdminObjs::idxTree()
//------------------------------------------------------------------------------
{
    return dynamic_cast<CIdxTreeTrcAdminObjs*>(m_pIdxTree);
}

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelIdxTreeTrcAdminObjs::columnCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs = "ModelIdxParent {" + ModelIdx2Str(i_modelIdxParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "columnCount",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        mthTracer.setMethodReturn(EColumnCount);
    }

    return EColumnCount;

} // columnCount

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CModelIdxTreeTrcAdminObjs::headerData(
    int             i_iSection,
    Qt::Orientation i_orientation,
    int             i_iRole ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs = "Section: " + QString::number(i_iSection);
        strMthInArgs += ", Orientation: " + qOrientation2Str(i_orientation);
        strMthInArgs += ", Role: " + qItemDataRole2Str(i_iRole);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "headerData",
        /* strMethodInArgs    */ strMthInArgs );

    QVariant varData;

    if( i_orientation == Qt::Horizontal )
    {
        if( i_iSection >= 0 && i_iSection < CModelIdxTree::EColumnCount )
        {
            varData = CModelIdxTree::headerData(i_iSection, i_orientation, i_iRole);
        }
        else if( i_iSection >= CModelIdxTree::EColumnCount )
        {
            switch( i_iSection )
            {
                case EColumnObjAddress:
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        varData = "ObjAddress";
                    }
                    break;
                }
                case EColumnRefCount:
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        varData = "RefCount";
                    }
                    break;
                }
                case EColumnNameSpace:
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        varData = "NameSpace";
                    }
                    break;
                }
                case EColumnClassName:
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        varData = "ClassName";
                    }
                    break;
                }
                case EColumnObjName:
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        varData = "ObjName";
                    }
                    break;
                }
                case EColumnObjThreadName:
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        varData = "Thread";
                    }
                    break;
                }
                case EColumnEnabled:
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        varData = "Enabled";
                    }
                    break;
                }
                case EColumnDetailLevel:
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        varData = "DetailLevel";
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
            } // switch( i_iSection )
        } // if( i_iSection >= CModelIdxTree::EColumnCount )
    } // if( i_orientation == Qt::Horizontal )

    else // if( i_orientation == Qt::Vertical )
    {
    }

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        mthTracer.setMethodReturn(varData.toString());
    }

    return varData;

} // headerData

//------------------------------------------------------------------------------
Qt::ItemFlags CModelIdxTreeTrcAdminObjs::flags( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs = "ModelIdx {" + ModelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "flags",
        /* strMethodInArgs    */ strMthInArgs );

    Qt::ItemFlags uFlags = QAbstractItemModel::flags(i_modelIdx);

    if( i_modelIdx.isValid() && i_modelIdx.column() == EColumnDetailLevel )
    {
        CModelAbstractTreeEntry* pModelTreeEntry = static_cast<CModelAbstractTreeEntry*>(i_modelIdx.internalPointer());

        CTrcAdminObj* pTrcAdminObj = nullptr;

        if( pModelTreeEntry != nullptr )
        {
            pTrcAdminObj = reinterpret_cast<CTrcAdminObj*>(pModelTreeEntry->treeEntry());

            if( pTrcAdminObj != nullptr )
            {
                uFlags |= Qt::ItemIsEditable;
            }
        }
    }

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        mthTracer.setMethodReturn(qItemFlags2Str(uFlags));
    }

    return uFlags;

} // flags

//------------------------------------------------------------------------------
QVariant CModelIdxTreeTrcAdminObjs::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs  = "ModelIdx {" + ModelIdx2Str(i_modelIdx) + "}";
        strMthInArgs += ", Role: " + qItemDataRole2Str(i_iRole);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "data",
        /* strMethodInArgs    */ strMthInArgs );

    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pIdxTree == nullptr ? nullptr : m_pIdxTree->mutex());

    QVariant varData;

    CModelAbstractTreeEntry* pModelTreeEntry = nullptr;

    if( i_modelIdx.isValid() )
    {
        pModelTreeEntry = static_cast<CModelAbstractTreeEntry*>(i_modelIdx.internalPointer());
    }

    if( pModelTreeEntry != nullptr )
    {
        CModelBranchTreeEntry* pModelBranch = nullptr;
        CModelLeaveTreeEntry*  pModelLeave  = nullptr;

        CTrcAdminObj* pTrcAdminObj = nullptr;

        if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Root || pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            pModelBranch = dynamic_cast<CModelBranchTreeEntry*>(pModelTreeEntry);
        }
        else if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Leave )
        {
            pModelLeave = dynamic_cast<CModelLeaveTreeEntry*>(pModelTreeEntry);
            pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pModelLeave->treeEntry());
        }

        switch( i_modelIdx.column() )
        {
            case EColumnTreeEntryName:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole || i_iRole == Qt::ToolTipRole || i_iRole == Qt::ForegroundRole)
                {
                    QString strKeyInTree = pModelTreeEntry->keyInTree();
                    QString strCalculatedKeyInTree  = pModelTreeEntry->getCalculatedKeyInTree();
                    QString strCalculatedKeyInModel = pModelTreeEntry->getCalculatedKeyInModel();

                    if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                    {
                        if( pModelTreeEntry == m_pModelRoot )
                        {
                            varData = pModelTreeEntry->name();
                        }
                        else if( strCalculatedKeyInModel != strCalculatedKeyInTree || strCalculatedKeyInModel != strKeyInTree )
                        {
                            varData = "!" + pModelTreeEntry->name();
                        }
                        else
                        {
                            varData = pModelTreeEntry->name();
                        }
                    }
                    else if( i_iRole == Qt::ToolTipRole )
                    {
                        QString strData;
                        strData  = "KeyInTree: " + strKeyInTree;
                        strData += "\nCalculatedKeyInTree: " + strCalculatedKeyInTree;
                        strData += "\nCalculatedKeyInModel: " + strCalculatedKeyInModel;
                        strData += "\nKeyInParentBranch: " + pModelTreeEntry->keyInParentBranch();
                        strData += "\nIdxInTree: " + QString::number(pModelTreeEntry->indexInTree());
                        strData += "\nIdxInParentBranch: " + QString::number(pModelTreeEntry->indexInParentBranch());
                        strData += "\nSelected: " + bool2Str(pModelTreeEntry->isSelected()) ;
                        if( pModelBranch != nullptr )
                        {
                            strData += "\nExpanded: " + bool2Str(pModelBranch->isExpanded());
                        }
                        varData = strData;
                    }
                    else if( i_iRole == Qt::ForegroundRole)
                    {
                        if( pModelTreeEntry != m_pModelRoot )
                        {
                            if( strCalculatedKeyInModel != strCalculatedKeyInTree || strCalculatedKeyInModel != strKeyInTree )
                            {
                                varData = QColor(Qt::red);
                            }
                        }
                    }
                }
                else if( i_iRole == Qt::DecorationRole )
                {
                    varData = GetIcon(pModelTreeEntry->entryType());
                }
                break;
            }
            case EColumnInternalId:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = QString::number(i_modelIdx.internalId()) + " (" + pointer2Str(i_modelIdx.internalPointer()) + ")";
                }
                break;
            }
            case EColumnTreeEntryType:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = pModelTreeEntry->entryType2Str();
                }
                break;
            }
            case EColumnIdxInTree:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = pModelTreeEntry->indexInTree();
                }
                break;
            }
            case EColumnIdxInParentBranch:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = pModelTreeEntry->indexInParentBranch();
                }
                break;
            }
            case EColumnKeyInTree:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = pModelTreeEntry->keyInTree();
                }
                break;
            }
            case EColumnKeyInParentBranch:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = pModelTreeEntry->keyInParentBranch();
                }
                break;
            }
            case EColumnObjAddress:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    if( pTrcAdminObj != nullptr )
                    {
                        if( sizeof(pTrcAdminObj) == sizeof(quint32) ) // 32 bit machines
                        {
                            quint32 ulAddr;
                            quint32* pulAddr = &ulAddr;
                            memcpy( pulAddr, &pTrcAdminObj, sizeof(pTrcAdminObj) );
                            varData = "0x" + QString::number(ulAddr,16);
                        }
                        else if( sizeof(pTrcAdminObj) == sizeof(quint64) ) // 64 bit machines
                        {
                            quint64 ulAddr;
                            quint64* pulAddr = &ulAddr;
                            memcpy( pulAddr, &pTrcAdminObj, sizeof(pTrcAdminObj) );
                            varData = "0x" + QString::number(ulAddr,16);
                        }
                    }
                }
                break;
            }
            case EColumnRefCount:
            {
                if( i_iRole == Qt::DisplayRole)
                {
                    if( pTrcAdminObj != nullptr )
                    {
                        varData = QString::number(pTrcAdminObj->getRefCount());
                    }
                }
                break;
            }
            case EColumnNameSpace:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    if( pTrcAdminObj != nullptr )
                    {
                        varData = pTrcAdminObj->getNameSpace();
                    }
                }
                break;
            }
            case EColumnClassName:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    if( pTrcAdminObj != nullptr )
                    {
                        varData = pTrcAdminObj->getClassName();
                    }
                }
                break;
            }
            case EColumnObjName:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    if( pTrcAdminObj != nullptr )
                    {
                        varData = pTrcAdminObj->getObjectName();
                    }
                }
                break;
            }
            case EColumnObjThreadName:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    if( pTrcAdminObj != nullptr )
                    {
                        varData = pTrcAdminObj->getObjectThreadName();
                    }
                }
                break;
            }
            case EColumnEnabled:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    if( pTrcAdminObj != nullptr )
                    {
                        varData = CEnumEnabled::toString(pTrcAdminObj->getEnabled());
                    }
                }
                break;
            }
            case EColumnDetailLevel:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    if( pTrcAdminObj != nullptr )
                    {
                        varData = pTrcAdminObj->getTraceDetailLevel();
                    }
                }
                break;
            }
            default:
            {
                break;
            }
        } // switch( i_modelIdx.column() )
    } // if( pModelTreeEntry != nullptr )

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        mthTracer.setMethodReturn(varData.toString());
    }

    return varData;

} // data

//------------------------------------------------------------------------------
bool CModelIdxTreeTrcAdminObjs::setData( const QModelIndex& i_modelIdx, const QVariant& i_varData, int i_iRole )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "ModelIdx {" + ModelIdx2Str(i_modelIdx) + "}";
        strMthInArgs += ", Data: " + i_varData.toString();
        strMthInArgs += ", Role: " + qItemDataRole2Str(i_iRole);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setData",
        /* strMethodInArgs    */ strMthInArgs );

    bool bOk = false;

    if( i_iRole == Qt::EditRole && i_varData.isValid() && i_modelIdx.isValid() && i_modelIdx.column() == EColumnDetailLevel )
    {
        CModelAbstractTreeEntry* pModelTreeEntry = nullptr;

        if( i_modelIdx.isValid() )
        {
            pModelTreeEntry = static_cast<CModelAbstractTreeEntry*>(i_modelIdx.internalPointer());
        }

        if( pModelTreeEntry != nullptr )
        {
            //CModelBranchTreeEntry* pModelBranch = nullptr;
            CModelLeaveTreeEntry*  pModelLeave  = nullptr;

            CTrcAdminObj* pTrcAdminObj = nullptr;

            if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Root || pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
            {
                //pModelBranch = dynamic_cast<CModelBranchTreeEntry*>(pModelTreeEntry);
            }
            else if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Leave )
            {
                pModelLeave = dynamic_cast<CModelLeaveTreeEntry*>(pModelTreeEntry);
                pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pModelLeave->treeEntry());
            }

            if( pTrcAdminObj != nullptr )
            {
                if( i_varData.canConvert(QVariant::Int) )
                {
                    pTrcAdminObj->setTraceDetailLevel(i_varData.toInt());
                }
            } // if( pTrcAdminObj != nullptr )
        } // if( pModelTreeEntry != nullptr )

        bOk = true;

    } // if( i_iRole == Qt::EditRole && i_varData.isValid() && i_modelIdx.isValid() && i_modelIdx.column() == EColumnDetailLevel )

    if( bOk )
    {
        // Please note that if the name has been changed there is no need to emit the
        // dataChanged signal here. The index tree will change the name of the node and
        // will emit the signals in the following order:
        // - treeEntryAboutToBeRenamed
        // - treeEntryKeyInTreeChanged
        // - treeEntryRenamed
        // This model is connected to the "treeEntryKeyInTreeChanged" signal
        // and emits the dataChanged signal for the changed tree node.
        // Please note that the index tree may be changed by anyone else
        // and not just be this model so this is the way it has to be.
        // But emitting the signal in case any other attribute has been changed
        // (e.g. the selection state) the data changed signal must be emitted.
        // So we do it here for all attributes.
        emit dataChanged(i_modelIdx, i_modelIdx);
    }

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(bool2Str(bOk));
    }

    return bOk;

} // setData
