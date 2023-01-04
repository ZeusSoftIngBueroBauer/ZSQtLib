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

#include <QtCore/qmutex.h>
#include <QtCore/qfile.h>
#include <QtCore/qthread.h>
#include <QtCore/qtextstream.h>
#include <QtGui/qbitmap.h>
#include <QtGui/qicon.h>

#include "ZSPhysValGUI/ZSPhysSizesIdxTreeModel.h"
#include "ZSPhysVal/ZSPhysSizesIdxTree.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSSys/ZSSysAux.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;


/*******************************************************************************
class CModelIdxTreePhysSizes : public ZS::System::GUI::CModelIdxTree
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelIdxTreePhysSizes::CModelIdxTreePhysSizes(
    CIdxTreePhysSizes* i_pTrcAdminObjIdxTree,
    QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    CModelIdxTree(i_pTrcAdminObjIdxTree, i_pObjParent)
{
} // ctor

//------------------------------------------------------------------------------
CModelIdxTreePhysSizes::~CModelIdxTreePhysSizes()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CIdxTreePhysSizes* CModelIdxTreePhysSizes::idxTree()
//------------------------------------------------------------------------------
{
    return dynamic_cast<CIdxTreePhysSizes*>(m_pIdxTree);
}

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelIdxTreePhysSizes::columnCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    return EColumnCount;
}

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CModelIdxTreePhysSizes::headerData(
    int             i_iSection,
    Qt::Orientation i_orientation,
    int             i_iRole ) const
//------------------------------------------------------------------------------
{
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
                case EColumn0:
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        varData = "0";
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

    return varData;

} // headerData

//------------------------------------------------------------------------------
QVariant CModelIdxTreePhysSizes::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    CModelIdxTreeEntry* pModelTreeEntry = nullptr;

    if( i_modelIdx.isValid() )
    {
        pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
    }

    if( pModelTreeEntry != nullptr )
    {
        CIdxTreeLocker idxTreeLocker(m_pIdxTree);

        if( pModelTreeEntry->isLeave() )
        {
        }

        switch( i_modelIdx.column() )
        {
            case EColumnTreeEntryName:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole || i_iRole == Qt::ToolTipRole || i_iRole == Qt::ForegroundRole)
                {
                    QString strKeyInTree = pModelTreeEntry->keyInTree();

                    if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                    {
                        varData = pModelTreeEntry->name();
                    }
                    else if( i_iRole == Qt::ToolTipRole )
                    {
                        QString strData;
                        strData  = "KeyInTree: " + strKeyInTree;
                        strData += "\nKeyInParentBranch: " + pModelTreeEntry->keyInParentBranch();
                        strData += "\nIdxInTree: " + QString::number(pModelTreeEntry->indexInTree());
                        strData += "\nIdxInParentBranch: " + QString::number(pModelTreeEntry->indexInParentBranch());
                        strData += "\nSelected: " + bool2Str(pModelTreeEntry->isSelected()) ;
                        if( pModelTreeEntry->isLeave() )
                        {
                            strData += "\nExpanded: " + bool2Str(pModelTreeEntry->isExpanded());
                        }
                        varData = strData;
                    }
                }
                else if( i_iRole == Qt::DecorationRole )
                {
                    varData = getIcon(pModelTreeEntry->entryType());
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
            case EColumn0:
            {
                if( i_iRole == Qt::DisplayRole)
                {
                }
                break;
            }
            default:
            {
                break;
            }
        } // switch( i_modelIdx.column() )
    } // if( pModelTreeEntry != nullptr )

    return varData;

} // data
