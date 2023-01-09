/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer, Germany
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

#include <QtCore/qglobal.h>

#include "UnitModels.h"

#include "ZSPhysVal/ZSPhysSizesIdxTree.h"
#include "ZSPhysVal/ZSPhysUnitsRatio.h"
#include "ZSPhysVal/ZSPhysUnitsDataQuantity.h"
#include "ZSSysGUI/ZSSysIdxTreeModel.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::System::GUI;
using namespace ZS::Apps::Test::PhysVal;

/*******************************************************************************
struct SUnitsModelNode
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
SUnitsModelNode::SUnitsModelNode() :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_modelIdx(),
    m_pParentNode(nullptr),
    m_arpChildNodes()
{
} // ctor

//------------------------------------------------------------------------------
SUnitsModelNode::SUnitsModelNode( ZS::PhysVal::CUnitGrp* i_pUnitGrp ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pUnitGrp),
    m_pUnit(nullptr),
    m_modelIdx(),
    m_pParentNode(nullptr),
    m_arpChildNodes()
{
} // ctor

//------------------------------------------------------------------------------
SUnitsModelNode::SUnitsModelNode( ZS::PhysVal::CUnit* i_pUnit ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(i_pUnit),
    m_modelIdx(),
    m_pParentNode(nullptr),
    m_arpChildNodes()
{
} // ctor

//------------------------------------------------------------------------------
SUnitsModelNode::~SUnitsModelNode()
//------------------------------------------------------------------------------
{
    m_pUnitGrp = nullptr;
    m_pUnit = nullptr;
    m_pParentNode = nullptr;
    m_arpChildNodes.clear();

} // dtor


/*******************************************************************************
class CUnitsModel : public QAbstractItemModel
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CUnitsModel::CUnitsModel( QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    QAbstractItemModel(i_pObjParent),
    m_pRootNode(nullptr)
{
    m_pRootNode = new SUnitsModelNode();
    m_pRootNode->m_modelIdx = createIndex(0,0,m_pRootNode);

    for( int idxClassType = 0; idxClassType < CEnumUnitClassType::count(); idxClassType++ )
    {
        EUnitClassType classType = static_cast<EUnitClassType>(idxClassType);

        //CUnitGrp* pUnitGrp = CIdxTreePhysSizes::GetUnitClassTypeGroup(classType);

        //if( pUnitGrp->getChildUnitGrpCount() > 0 || pUnitGrp->getUnitCount() > 0 )
        //{
        //    addNode(m_pRootNode,pUnitGrp);
        //}
    }
} // ctor

//------------------------------------------------------------------------------
CUnitsModel::~CUnitsModel()
//------------------------------------------------------------------------------
{
    clear(m_pRootNode);

    try
    {
        delete m_pRootNode;
    }
    catch(...)
    {
    }
    m_pRootNode = nullptr;

} // dtor

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CUnitsModel::clear( SUnitsModelNode* i_pModelNode )
//------------------------------------------------------------------------------
{
    if( i_pModelNode->m_arpChildNodes.size() > 0 )
    {
        SUnitsModelNode* pChildNode;
        int              idxChild;

        for( idxChild = i_pModelNode->m_arpChildNodes.size()-1; idxChild >= 0; idxChild-- )
        {
            pChildNode = i_pModelNode->m_arpChildNodes[idxChild];
            i_pModelNode->m_arpChildNodes[idxChild] = nullptr;

            clear(pChildNode);

            delete pChildNode;
            pChildNode = nullptr;
        }
    }

} // clear

//------------------------------------------------------------------------------
void CUnitsModel::addNode( SUnitsModelNode* i_pParentNode, CUnitGrp* i_pUnitGrp )
//------------------------------------------------------------------------------
{
    SUnitsModelNode* pNodeUnitGrp = new SUnitsModelNode(i_pUnitGrp);

    i_pParentNode->m_arpChildNodes.append(pNodeUnitGrp);

    pNodeUnitGrp->m_pParentNode = i_pParentNode;
    pNodeUnitGrp->m_modelIdx = createIndex(i_pParentNode->m_arpChildNodes.size()-1,0,pNodeUnitGrp);

    if( i_pUnitGrp->count() > 0 )
    {
        CUnitGrp* pUnitGrp;
        int       idxUnitGrp;

        for( idxUnitGrp = 0; idxUnitGrp < i_pUnitGrp->count(); idxUnitGrp++ )
        {
            pUnitGrp = dynamic_cast<CUnitGrp*>(i_pUnitGrp->at(idxUnitGrp));

            addNode(pNodeUnitGrp,pUnitGrp);
        }
    }
    else // if( pUnitGrp->getUnitCount() > 0 )
    {
        SUnitsModelNode* pNodeUnit;
        CUnit*           pUnit;
        int              idxUnit;

        for( idxUnit = 0; idxUnit < i_pUnitGrp->count(); idxUnit++ )
        {
            pUnit = dynamic_cast<CUnit*>(i_pUnitGrp->at(idxUnit));

            pNodeUnit = new SUnitsModelNode(pUnit);

            pNodeUnitGrp->m_arpChildNodes.append(pNodeUnit);

            pNodeUnit->m_pParentNode = pNodeUnitGrp;
            pNodeUnit->m_modelIdx = createIndex(pNodeUnitGrp->m_arpChildNodes.size()-1,0,pNodeUnit);
        }
    }

} // addNode

/*==============================================================================
public: // must overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CUnitsModel::rowCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    int iRowCount = 0;

    if( !i_modelIdxParent.isValid() )
    {
        iRowCount = 1; // root node
    }
    else
    {
        SUnitsModelNode* pParentNode = static_cast<SUnitsModelNode*>(i_modelIdxParent.internalPointer());

        if( pParentNode == nullptr )
        {
            throw CException(__FILE__,__LINE__,EResultInternalProgramError);
        }
        iRowCount = pParentNode->m_arpChildNodes.size();
    }
    return iRowCount;

} // rowCount

//------------------------------------------------------------------------------
int CUnitsModel::columnCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return EColumnCount;
}

//------------------------------------------------------------------------------
QVariant CUnitsModel::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    if( !i_modelIdx.isValid() )
    {
        return varData;
    }

    SUnitsModelNode* pNode = static_cast<SUnitsModelNode*>(i_modelIdx.internalPointer());

    if( pNode == nullptr )
    {
        return varData;
    }

    switch( i_modelIdx.column() )
    {
        case EColumnNodeName:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( pNode->m_pUnit != nullptr )
                {
                    varData = pNode->m_pUnit->name();
                }
                else if( pNode->m_pUnitGrp != nullptr )
                {
                    varData = pNode->m_pUnitGrp->name();
                }
                else
                {
                    varData = "Units";
                }
            }
            else if( i_iRole == Qt::DecorationRole )
            {
                if( pNode->m_pUnit != nullptr )
                {
                    varData = CModelIdxTree::getIcon(EIdxTreeEntryType::Leave);
                }
                else if( pNode->m_pUnitGrp != nullptr )
                {
                    varData = CModelIdxTree::getIcon(EIdxTreeEntryType::Branch);
                }
                else
                {
                    varData = CModelIdxTree::getIcon(EIdxTreeEntryType::Root);
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
QModelIndex CUnitsModel::index( int i_iRow, int /*i_iCol*/, const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    QModelIndex modelIdx;

    if( i_iRow >= 0 )
    {
        if( !i_modelIdxParent.isValid() )
        {
            if( i_iRow == 0 )
            {
                modelIdx = m_pRootNode->m_modelIdx;
            }
        }
        else
        {
            SUnitsModelNode* pParentNode = static_cast<SUnitsModelNode*>(i_modelIdxParent.internalPointer());

            if( pParentNode->m_arpChildNodes.size() > 0 && i_iRow < pParentNode->m_arpChildNodes.size() )
            {
                SUnitsModelNode* pNode = pParentNode->m_arpChildNodes[i_iRow];
                modelIdx = pNode->m_modelIdx;
            }
        }
    }
    return modelIdx;

} // index

//------------------------------------------------------------------------------
QModelIndex CUnitsModel::parent( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QModelIndex modelIdx;

    if( i_modelIdx.isValid() )
    {
        SUnitsModelNode* pNode = static_cast<SUnitsModelNode*>(i_modelIdx.internalPointer());
        SUnitsModelNode* pParentNode;

        if( pNode == nullptr )
        {
            throw CException(__FILE__,__LINE__,EResultInternalProgramError);
        }

        pParentNode = pNode->m_pParentNode;

        if( pParentNode != nullptr )
        {
            modelIdx = createIndex( pParentNode->m_modelIdx.row(), 0, pParentNode );
        }
    }
    return modelIdx;

} // parent

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CUnitsModel::headerData(
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
            case EColumnNodeName:
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


///*******************************************************************************
//class CModelUnitGrpSIBase : public QAbstractTableModel
//*******************************************************************************/
//
///*==============================================================================
//public: // ctors and dtor
//==============================================================================*/
//
////------------------------------------------------------------------------------
//CModelUnitGrpSIBase::CModelUnitGrpSIBase(
//    const QString& i_strObjName,
//    QObject*       i_pObjParent ) :
////------------------------------------------------------------------------------
//    QAbstractTableModel(i_pObjParent),
//    m_strObjName(i_strObjName),
//    m_pUnitGrp(nullptr)
//{
//} // ctor
//
////------------------------------------------------------------------------------
//CModelUnitGrpSIBase::~CModelUnitGrpSIBase()
////------------------------------------------------------------------------------
//{
//    m_pUnitGrp = nullptr;
//
//} // dtor
//
///*==============================================================================
//public: // instance methods
//==============================================================================*/
//
////------------------------------------------------------------------------------
//void CModelUnitGrpSIBase::setObjName( const QString& i_strObjName )
////------------------------------------------------------------------------------
//{
//    m_strObjName = i_strObjName;
//}
//
////------------------------------------------------------------------------------
//QString CModelUnitGrpSIBase::getObjName() const
////------------------------------------------------------------------------------
//{
//    return m_strObjName;
//}
//
////------------------------------------------------------------------------------
//unsigned int CModelUnitGrpSIBase::getUnitCount() const
////------------------------------------------------------------------------------
//{
//    unsigned int uUnitCount = 0;
//
//    if( m_pUnitGrp != nullptr )
//    {
//        uUnitCount = m_pUnitGrp->getUnitCount();
//    }
//    return uUnitCount;
//}
//
////------------------------------------------------------------------------------
//void CModelUnitGrpSIBase::setUnitGroup( CUnitGrp* i_pUnitGrp )
////------------------------------------------------------------------------------
//{
//    if( getUnitCount() > 0 )
//    {
//        beginRemoveRows(QModelIndex(),0,getUnitCount()-1);
//        endRemoveRows();
//    }
//
//    m_pUnitGrp = i_pUnitGrp;
//
//    if( getUnitCount() > 0 )
//    {
//        beginInsertRows(QModelIndex(),0,getUnitCount()-1);
//        endInsertRows();
//    }
//}
//
////------------------------------------------------------------------------------
//CUnitGrp* CModelUnitGrpSIBase::unitGroup()
////------------------------------------------------------------------------------
//{
//    return m_pUnitGrp;
//}
//
///*==============================================================================
//public: // must overridables of base class QAbstractTableModel
//==============================================================================*/
//
////------------------------------------------------------------------------------
//int CModelUnitGrpSIBase::rowCount( const QModelIndex& i_modelIdxParent ) const
////------------------------------------------------------------------------------
//{
//    return getUnitCount();
//}
//
////------------------------------------------------------------------------------
//int CModelUnitGrpSIBase::columnCount( const QModelIndex& i_modelIdxParent ) const
////------------------------------------------------------------------------------
//{
//    return EColumnCount;
//}
//
////------------------------------------------------------------------------------
//QVariant CModelUnitGrpSIBase::data( const QModelIndex& i_modelIdx, int i_iRole ) const
////------------------------------------------------------------------------------
//{
//    QVariant     varData;
//    int          iRow = i_modelIdx.row();
//    int          iCol = i_modelIdx.column();
//    CUnitSIBase* pUnit = nullptr;
//
//    if( !i_modelIdx.isValid() )
//    {
//        return varData;
//    }
//    if( iRow < 0 || iRow >= static_cast<int>(getUnitCount()) )
//    {
//        return varData;
//    }
//    if( iCol < 0 || iCol >= columnCount() )
//    {
//        return varData;
//    }
//    if( m_pUnitGrp == nullptr )
//    {
//        return varData;
//    }
//
//    pUnit = dynamic_cast<CUnitSIBase*>(m_pUnitGrp->unit(iRow));
//
//    if( pUnit == nullptr )
//    {
//        return varData;
//    }
//
//    switch( i_modelIdx.column() )
//    {
//        case EColumnId:
//        {
//            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
//            {
//                varData = pUnit->getId();
//            }
//            break;
//        }
//        case EColumnName:
//        {
//            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
//            {
//                varData = pUnit->getName();
//            }
//            break;
//        }
//        case EColumnSymbol:
//        {
//            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
//            {
//                varData = pUnit->symbol();
//            }
//            break;
//        }
//        case EColumnMeasure:
//        {
//            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
//            {
//                varData = pUnit->getMeasure();
//            }
//            break;
//        }
//        case EColumnDimension:
//        {
//            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
//            {
//                varData = pUnit->getDimension();
//            }
//            break;
//        }
//        default:
//        {
//            break;
//        }
//    }
//    return varData;
//
//} // data
//
////------------------------------------------------------------------------------
//QVariant CModelUnitGrpSIBase::headerData(
//    int             i_iSection,
//    Qt::Orientation i_orientation,
//    int             i_iRole ) const
////------------------------------------------------------------------------------
//{
//    QVariant varData;
//
//    switch( i_iSection )
//    {
//        case EColumnId:
//        {
//            if( i_iRole == Qt::DisplayRole )
//            {
//                if( i_orientation == Qt::Horizontal )
//                {
//                    varData = "Id";
//                }
//            }
//            break;
//        }
//        case EColumnName:
//        {
//            if( i_iRole == Qt::DisplayRole )
//            {
//                if( i_orientation == Qt::Horizontal )
//                {
//                    varData = "Name";
//                }
//            }
//            break;
//        }
//        case EColumnSymbol:
//        {
//            if( i_iRole == Qt::DisplayRole )
//            {
//                if( i_orientation == Qt::Horizontal )
//                {
//                    varData = "Symbol";
//                }
//            }
//            break;
//        }
//        case EColumnMeasure:
//        {
//            if( i_iRole == Qt::DisplayRole )
//            {
//                if( i_orientation == Qt::Horizontal )
//                {
//                    varData = "Measure";
//                }
//            }
//            break;
//        }
//        case EColumnDimension:
//        {
//            if( i_iRole == Qt::DisplayRole )
//            {
//                if( i_orientation == Qt::Horizontal )
//                {
//                    varData = "Dimension";
//                }
//            }
//            break;
//        }
//        default:
//        {
//            if( i_iRole == Qt::DisplayRole )
//            {
//                if( i_orientation == Qt::Horizontal )
//                {
//                    varData = "?";
//                }
//            }
//            break;
//        }
//    }
//    return varData;
//
//} // headerData


/*******************************************************************************
class CModelUnitGrpRatio : public QAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelUnitGrpRatio::CModelUnitGrpRatio(
    const QString& i_strObjName,
    QObject*       i_pObjParent ) :
//------------------------------------------------------------------------------
    QAbstractTableModel(i_pObjParent),
    m_strObjName(i_strObjName),
    m_pUnitGrp(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
CModelUnitGrpRatio::~CModelUnitGrpRatio()
//------------------------------------------------------------------------------
{
    m_pUnitGrp = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelUnitGrpRatio::setObjName( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    m_strObjName = i_strObjName;
}

//------------------------------------------------------------------------------
QString CModelUnitGrpRatio::getObjName() const
//------------------------------------------------------------------------------
{
    return m_strObjName;
}

//------------------------------------------------------------------------------
unsigned int CModelUnitGrpRatio::getUnitCount() const
//------------------------------------------------------------------------------
{
    unsigned int uUnitCount = 0;

    if( m_pUnitGrp != nullptr )
    {
        uUnitCount = m_pUnitGrp->count();
    }
    return uUnitCount;
}

//------------------------------------------------------------------------------
void CModelUnitGrpRatio::setUnitGroup( CUnitGrp* i_pUnitGrp )
//------------------------------------------------------------------------------
{
    if( getUnitCount() > 0 )
    {
        beginRemoveRows(QModelIndex(),0,getUnitCount()-1);
        endRemoveRows();
    }

    m_pUnitGrp = i_pUnitGrp;

    if( getUnitCount() > 0 )
    {
        beginInsertRows(QModelIndex(),0,getUnitCount()-1);
        endInsertRows();
    }
}

//------------------------------------------------------------------------------
CUnitGrp* CModelUnitGrpRatio::unitGroup()
//------------------------------------------------------------------------------
{
    return m_pUnitGrp;
}

/*==============================================================================
public: // must overridables of base class QAbstractTableModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelUnitGrpRatio::rowCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return getUnitCount();
}

//------------------------------------------------------------------------------
int CModelUnitGrpRatio::columnCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return EColumnCount;
}

//------------------------------------------------------------------------------
QVariant CModelUnitGrpRatio::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant     varData;
    int          iRow = i_modelIdx.row();
    int          iCol = i_modelIdx.column();
    CUnitRatio*  pUnit = nullptr;

    if( !i_modelIdx.isValid() )
    {
        return varData;
    }
    if( iRow < 0 || iRow >= static_cast<int>(getUnitCount()) )
    {
        return varData;
    }
    if( iCol < 0 || iCol >= columnCount() )
    {
        return varData;
    }
    if( m_pUnitGrp == nullptr )
    {
        return varData;
    }

    pUnit = dynamic_cast<CUnitRatio*>(m_pUnitGrp->at(iRow));

    if( pUnit == nullptr )
    {
        return varData;
    }

    switch( i_modelIdx.column() )
    {
        case EColumnName:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = pUnit->name();
            }
            break;
        }
        case EColumnSymbol:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = pUnit->symbol();
            }
            break;
        }
        case EColumnFactor:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = pUnit->getFactor();
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
QVariant CModelUnitGrpRatio::headerData(
    int             i_iSection,
    Qt::Orientation i_orientation,
    int             i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    switch( i_iSection )
    {
        case EColumnName:
        {
            if( i_iRole == Qt::DisplayRole )
            {
                if( i_orientation == Qt::Horizontal )
                {
                    varData = "Name";
                }
            }
            break;
        }
        case EColumnSymbol:
        {
            if( i_iRole == Qt::DisplayRole )
            {
                if( i_orientation == Qt::Horizontal )
                {
                    varData = "Symbol";
                }
            }
            break;
        }
        case EColumnFactor:
        {
            if( i_iRole == Qt::DisplayRole )
            {
                if( i_orientation == Qt::Horizontal )
                {
                    varData = "Factor";
                }
            }
            break;
        }
        default:
        {
            if( i_iRole == Qt::DisplayRole )
            {
                if( i_orientation == Qt::Horizontal )
                {
                    varData = "?";
                }
            }
            break;
        }
    }
    return varData;

} // headerData


/*******************************************************************************
class CModelUnitGrpDataQuantity : public QAbstractTableModel
*******************************************************************************/
#if 0

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelUnitGrpDataQuantity::CModelUnitGrpDataQuantity(
    const QString& i_strObjName,
    QObject*       i_pObjParent ) :
//------------------------------------------------------------------------------
    QAbstractTableModel(i_pObjParent),
    m_strObjName(i_strObjName),
    m_pUnitGrp(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
CModelUnitGrpDataQuantity::~CModelUnitGrpDataQuantity()
//------------------------------------------------------------------------------
{
    m_pUnitGrp = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelUnitGrpDataQuantity::setObjName( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    m_strObjName = i_strObjName;
}

//------------------------------------------------------------------------------
QString CModelUnitGrpDataQuantity::getObjName() const
//------------------------------------------------------------------------------
{
    return m_strObjName;
}

//------------------------------------------------------------------------------
unsigned int CModelUnitGrpDataQuantity::getUnitCount() const
//------------------------------------------------------------------------------
{
    unsigned int uUnitCount = 0;

    if( m_pUnitGrp != nullptr )
    {
        uUnitCount = m_pUnitGrp->count();
    }
    return uUnitCount;
}

//------------------------------------------------------------------------------
void CModelUnitGrpDataQuantity::setUnitGroup( CUnitGrp* i_pUnitGrp )
//------------------------------------------------------------------------------
{
    if( getUnitCount() > 0 )
    {
        beginRemoveRows(QModelIndex(),0,getUnitCount()-1);
        endRemoveRows();
    }

    m_pUnitGrp = i_pUnitGrp;

    if( getUnitCount() > 0 )
    {
        beginInsertRows(QModelIndex(),0,getUnitCount()-1);
        endInsertRows();
    }
}

//------------------------------------------------------------------------------
CUnitGrp* CModelUnitGrpDataQuantity::unitGroup()
//------------------------------------------------------------------------------
{
    return m_pUnitGrp;
}

/*==============================================================================
public: // must overridables of base class QAbstractTableModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelUnitGrpDataQuantity::rowCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return getUnitCount();
}

//------------------------------------------------------------------------------
int CModelUnitGrpDataQuantity::columnCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return EColumnCount;
}

//------------------------------------------------------------------------------
QVariant CModelUnitGrpDataQuantity::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant           varData;
    int                iRow = i_modelIdx.row();
    int                iCol = i_modelIdx.column();
    CUnitDataQuantity* pUnit = nullptr;

    if( !i_modelIdx.isValid() )
    {
        return varData;
    }
    if( iRow < 0 || iRow >= static_cast<int>(getUnitCount()) )
    {
        return varData;
    }
    if( iCol < 0 || iCol >= columnCount() )
    {
        return varData;
    }
    if( m_pUnitGrp == nullptr )
    {
        return varData;
    }

    pUnit = dynamic_cast<CUnitDataQuantity*>(m_pUnitGrp->at(iRow));

    if( pUnit == nullptr )
    {
        return varData;
    }

    switch( i_modelIdx.column() )
    {
        case EColumnName:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = pUnit->name();
            }
            break;
        }
        case EColumnSymbol:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = pUnit->symbol();
            }
            break;
        }
        case EColumnFactor:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = pUnit->getFactor();
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
QVariant CModelUnitGrpDataQuantity::headerData(
    int             i_iSection,
    Qt::Orientation i_orientation,
    int             i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    switch( i_iSection )
    {
        case EColumnName:
        {
            if( i_iRole == Qt::DisplayRole )
            {
                if( i_orientation == Qt::Horizontal )
                {
                    varData = "Name";
                }
            }
            break;
        }
        case EColumnSymbol:
        {
            if( i_iRole == Qt::DisplayRole )
            {
                if( i_orientation == Qt::Horizontal )
                {
                    varData = "Symbol";
                }
            }
            break;
        }
        case EColumnFactor:
        {
            if( i_iRole == Qt::DisplayRole )
            {
                if( i_orientation == Qt::Horizontal )
                {
                    varData = "Factor";
                }
            }
            break;
        }
        default:
        {
            if( i_iRole == Qt::DisplayRole )
            {
                if( i_orientation == Qt::Horizontal )
                {
                    varData = "?";
                }
            }
            break;
        }
    }
    return varData;

} // headerData

#endif


/*******************************************************************************
class CModelPhysSize : public QAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

/* enum ViewMode
==============================================================================*/

//------------------------------------------------------------------------------
static const ZS::System::SEnumEntry s_arEnumEntriesViewMode[CModelPhysSize::EViewModeCount] =
//------------------------------------------------------------------------------
{
    SEnumEntry( CModelPhysSize::EViewModeNormal,           "Normal"            ),
    SEnumEntry( CModelPhysSize::EViewModeContingencyTable, "Contingency Table" )
};

//------------------------------------------------------------------------------
QString CModelPhysSize::viewMode2Str( int i_iViewMode )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumEntriesViewMode,EViewModeCount,i_iViewMode);
}

//------------------------------------------------------------------------------
CModelPhysSize::EViewMode CModelPhysSize::str2ViewMode( const QString& i_str )
//------------------------------------------------------------------------------
{
    EViewMode viewMode = EViewModeUndefined;
    int iViewMode = SEnumEntry::str2Enumerator(s_arEnumEntriesViewMode,EViewModeCount,i_str);
    if( iViewMode >= 0 && iViewMode < EViewModeCount )
    {
        viewMode = static_cast<EViewMode>(iViewMode);
    }
    return viewMode;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelPhysSize::CModelPhysSize(
    const QString& i_strObjName,
    QObject*       i_pObjParent ) :
//------------------------------------------------------------------------------
    QAbstractTableModel(i_pObjParent),
    m_strObjName(i_strObjName),
    m_pUnitGrp(nullptr),
    m_viewMode(EViewModeNormal)
{
} // ctor

//------------------------------------------------------------------------------
CModelPhysSize::~CModelPhysSize()
//------------------------------------------------------------------------------
{
    m_pUnitGrp = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelPhysSize::setObjName( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    m_strObjName = i_strObjName;
}

//------------------------------------------------------------------------------
QString CModelPhysSize::getObjName() const
//------------------------------------------------------------------------------
{
    return m_strObjName;
}

//------------------------------------------------------------------------------
bool CModelPhysSize::setViewMode( EViewMode i_viewMode )
//------------------------------------------------------------------------------
{
    if( m_viewMode != i_viewMode )
    {
        beginResetModel();

        m_viewMode = i_viewMode;

        endResetModel();
    }

    return true;

} // setViewMode

//------------------------------------------------------------------------------
CModelPhysSize::EViewMode CModelPhysSize::getViewMode() const
//------------------------------------------------------------------------------
{
    return m_viewMode;
}

//------------------------------------------------------------------------------
unsigned int CModelPhysSize::getUnitCount() const
//------------------------------------------------------------------------------
{
    unsigned int uUnitCount = 0;

    if( m_pUnitGrp != nullptr )
    {
        uUnitCount = m_pUnitGrp->count();
    }
    return uUnitCount;
}

//------------------------------------------------------------------------------
void CModelPhysSize::setUnitGroup( CUnitGrp* i_pUnitGrp )
//------------------------------------------------------------------------------
{
    if( getUnitCount() > 0 )
    {
        beginRemoveRows(QModelIndex(),0,getUnitCount()-1);
        endRemoveRows();
    }

    if( m_viewMode == EViewModeContingencyTable )
    {
        beginResetModel();
    }

    m_pUnitGrp = i_pUnitGrp;

    if( getUnitCount() > 0 )
    {
        beginInsertRows(QModelIndex(),0,getUnitCount()-1);
        endInsertRows();
    }

    if( m_viewMode == EViewModeContingencyTable )
    {
        endResetModel();
    }

} // setUnitGroup

//------------------------------------------------------------------------------
CUnitGrp* CModelPhysSize::unitGroup()
//------------------------------------------------------------------------------
{
    return m_pUnitGrp;
}

/*==============================================================================
public: // must overridables of base class QAbstractTableModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelPhysSize::rowCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return getUnitCount();
}

//------------------------------------------------------------------------------
int CModelPhysSize::columnCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    int iColCount = 0;

    switch( m_viewMode )
    {
        case EViewModeNormal:
        {
            iColCount = EColumnCount;
            break;
        }
        case EViewModeContingencyTable:
        {
            iColCount = getUnitCount();
            break;
        }
        default:
        {
            break;
        }
    }
    return iColCount;
}

//------------------------------------------------------------------------------
QVariant CModelPhysSize::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant   varData;
    int        iRow = i_modelIdx.row();
    int        iCol = i_modelIdx.column();
    CPhysUnit* pUnit = nullptr;
    CPhysUnit* pUnitNextLower = nullptr;
    CPhysUnit* pUnitNextHigher = nullptr;

    if( !i_modelIdx.isValid() )
    {
        return varData;
    }
    if( iRow < 0 || iRow >= static_cast<int>(getUnitCount()) )
    {
        return varData;
    }
    if( iCol < 0 || iCol >= columnCount() )
    {
        return varData;
    }
    if( m_pUnitGrp == nullptr )
    {
        return varData;
    }

    pUnit = dynamic_cast<CPhysUnit*>(m_pUnitGrp->at(iRow));

    if( pUnit == nullptr )
    {
        return varData;
    }
    pUnitNextLower  = dynamic_cast<CPhysUnit*>(pUnit->nextLowerUnit());
    pUnitNextHigher = dynamic_cast<CPhysUnit*>(pUnit->nextHigherUnit());

    switch( m_viewMode )
    {
        case EViewModeNormal:
        {
            switch( i_modelIdx.column() )
            {
                case EColumnName:
                {
                    if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                    {
                        varData = pUnit->name();
                    }
                    break;
                }
                case EColumnSymbol:
                {
                    if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                    {
                        varData = pUnit->symbol();
                    }
                    break;
                }
                case EColumnNextLower:
                {
                    if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                    {
                        if( pUnitNextLower != nullptr )
                        {
                            varData = pUnitNextLower->name();
                        }
                    }
                    break;
                }
                case EColumnNextHigher:
                {
                    if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                    {
                        if( pUnitNextHigher != nullptr )
                        {
                            varData = pUnitNextHigher->name();
                        }
                    }
                    break;
                }
                case EColumnFctConvertIntoSIUnit:
                {
                    if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                    {
                        varData = pUnit->getFctConvertIntoSIUnitName();
                    }
                    break;
                }
                case EColumnFctConvertFromSIUnit:
                {
                    if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                    {
                        varData = pUnit->getFctConvertFromSIUnitName();
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        } // case EViewModeNormal:

        case EViewModeContingencyTable:
        {
            CPhysUnit* pUnitCol = nullptr;

            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                pUnitCol = dynamic_cast<CPhysUnit*>(m_pUnitGrp->at(iCol));
                if( pUnitCol == nullptr )
                {
                    return varData;
                }
                if( iCol == iRow )
                {
                    varData = QString("-");
                }
                else
                {
                    varData = pUnit->findFctConvertInternalName(pUnitCol);
                }
            }
            break;
        } // case EViewModeContingencyTable:

        default:
        {
            break;
        }
    } // switch( m_viewMode )

    return varData;

} // data

//------------------------------------------------------------------------------
QVariant CModelPhysSize::headerData(
    int             i_iSection,
    Qt::Orientation i_orientation,
    int             i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    switch( m_viewMode )
    {
        case EViewModeNormal:
        {
            switch( i_iSection )
            {
                case EColumnName:
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        if( i_orientation == Qt::Horizontal )
                        {
                            varData = "Name";
                        }
                    }
                    break;
                }
                case EColumnSymbol:
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        if( i_orientation == Qt::Horizontal )
                        {
                            varData = "Symbol";
                        }
                    }
                    break;
                }
                case EColumnNextLower:
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        if( i_orientation == Qt::Horizontal )
                        {
                            varData = "Next Lower";
                        }
                    }
                    break;
                }
                case EColumnNextHigher:
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        if( i_orientation == Qt::Horizontal )
                        {
                            varData = "Next Higher";
                        }
                    }
                    break;
                }
                case EColumnFctConvertIntoSIUnit:
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        if( i_orientation == Qt::Horizontal )
                        {
                            varData = "Conversion Into SI Unit";
                        }
                    }
                    break;
                }
                case EColumnFctConvertFromSIUnit:
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        if( i_orientation == Qt::Horizontal )
                        {
                            varData = "Conversion From SI Unit";
                        }
                    }
                    break;
                }
                default:
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        if( i_orientation == Qt::Horizontal )
                        {
                            varData = "?";
                        }
                    }
                    break;
                }
            }
            break;
        } // case EViewModeNormal:

        case EViewModeContingencyTable:
        {
            if( i_iRole == Qt::DisplayRole )
            {
                CPhysUnit* pUnit = nullptr;

                if( i_iSection < 0 || i_iSection >= static_cast<int>(getUnitCount()) )
                {
                    return varData;
                }
                if( m_pUnitGrp == nullptr )
                {
                    return varData;
                }
                pUnit = dynamic_cast<CPhysUnit*>(m_pUnitGrp->at(i_iSection));
                if( pUnit == nullptr )
                {
                    return varData;
                }
                varData = pUnit->symbol();
            }
            break;
        }
        default:
        {
            break;
        }
    } // switch( m_viewMode )

    return varData;

} // headerData


/*******************************************************************************
class CModelFctConvertInternal : public QAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelFctConvertInternal::CModelFctConvertInternal(
    const QString& i_strObjName,
    QObject*       i_pObjParent ) :
//------------------------------------------------------------------------------
    QAbstractTableModel(i_pObjParent),
    m_strObjName(i_strObjName),
    m_pPhysUnit(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
CModelFctConvertInternal::~CModelFctConvertInternal()
//------------------------------------------------------------------------------
{
    m_pPhysUnit = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelFctConvertInternal::setObjName( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    m_strObjName = i_strObjName;
}

//------------------------------------------------------------------------------
QString CModelFctConvertInternal::getObjName() const
//------------------------------------------------------------------------------
{
    return m_strObjName;
}

//------------------------------------------------------------------------------
unsigned int CModelFctConvertInternal::getFctConvertsCount() const
//------------------------------------------------------------------------------
{
    unsigned int uFctConvertsCount = 0;

    if( m_pPhysUnit != nullptr )
    {
        uFctConvertsCount = m_pPhysUnit->getFctConvertsInternalCount();
    }
    return uFctConvertsCount;
}

//------------------------------------------------------------------------------
void CModelFctConvertInternal::setPhysUnit( CPhysUnit* i_pPhysUnit )
//------------------------------------------------------------------------------
{
    if( getFctConvertsCount() > 0 )
    {
        beginRemoveRows(QModelIndex(),0,getFctConvertsCount()-1);
        endRemoveRows();
    }

    m_pPhysUnit = i_pPhysUnit;

    if( getFctConvertsCount() > 0 )
    {
        beginInsertRows(QModelIndex(),0,getFctConvertsCount()-1);
        endInsertRows();
    }

} // setPhysUnit

/*==============================================================================
public: // must overridables of base class QAbstractTableModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelFctConvertInternal::rowCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return getFctConvertsCount();
}

//------------------------------------------------------------------------------
int CModelFctConvertInternal::columnCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return EColumnCount;
}

//------------------------------------------------------------------------------
QVariant CModelFctConvertInternal::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant     varData;
    int          iRow = i_modelIdx.row();
    int          iCol = i_modelIdx.column();
    CFctConvert* pfctConvert = nullptr;
    CPhysUnit*   pPhysUnitDst = nullptr;

    if( !i_modelIdx.isValid() )
    {
        return varData;
    }
    if( m_pPhysUnit == nullptr )
    {
        return varData;
    }
    if( iRow < 0 || iRow >= static_cast<int>(m_pPhysUnit->getFctConvertsInternalCount()) )
    {
        return varData;
    }
    if( iCol < 0 || iCol >= columnCount() )
    {
        return varData;
    }
    pfctConvert = m_pPhysUnit->getFctConvertInternal(static_cast<unsigned int>(iRow));
    if( pfctConvert == nullptr )
    {
        return varData;
    }
    pPhysUnitDst = pfctConvert->m_pPhysUnitDst;
    if( pPhysUnitDst == nullptr )
    {
        return varData;
    }

    switch( i_modelIdx.column() )
    {
        case EColumnFctConvert:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = m_pPhysUnit->findFctConvertInternalName(pPhysUnitDst);
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
QVariant CModelFctConvertInternal::headerData(
    int             i_iSection,
    Qt::Orientation i_orientation,
    int             i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    switch( i_iSection )
    {
        case EColumnFctConvert:
        {
            if( i_iRole == Qt::DisplayRole )
            {
                if( i_orientation == Qt::Horizontal )
                {
                    varData = "Function";
                }
            }
            break;
        }
        default:
        {
            if( i_iRole == Qt::DisplayRole )
            {
                if( i_orientation == Qt::Horizontal )
                {
                    varData = "?";
                }
            }
            break;
        }
    }
    return varData;

} // headerData


/*******************************************************************************
class CModelFctConvertExternal : public QAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelFctConvertExternal::CModelFctConvertExternal(
    const QString& i_strObjName,
    QObject*       i_pObjParent ) :
//------------------------------------------------------------------------------
    QAbstractTableModel(i_pObjParent),
    m_strObjName(i_strObjName),
    m_pPhysUnit(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
CModelFctConvertExternal::~CModelFctConvertExternal()
//------------------------------------------------------------------------------
{
    m_pPhysUnit = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelFctConvertExternal::setObjName( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    m_strObjName = i_strObjName;
}

//------------------------------------------------------------------------------
QString CModelFctConvertExternal::getObjName() const
//------------------------------------------------------------------------------
{
    return m_strObjName;
}

//------------------------------------------------------------------------------
unsigned int CModelFctConvertExternal::getFctConvertsCount() const
//------------------------------------------------------------------------------
{
    unsigned int uFctConvertsCount = 0;

    if( m_pPhysUnit != nullptr )
    {
        uFctConvertsCount = m_pPhysUnit->getFctConvertsExternalCount();
    }
    return uFctConvertsCount;
}

//------------------------------------------------------------------------------
void CModelFctConvertExternal::setPhysUnit( CPhysUnit* i_pPhysUnit )
//------------------------------------------------------------------------------
{
    if( getFctConvertsCount() > 0 )
    {
        beginRemoveRows(QModelIndex(),0,getFctConvertsCount()-1);
        endRemoveRows();
    }

    m_pPhysUnit = i_pPhysUnit;

    if( getFctConvertsCount() > 0 )
    {
        beginInsertRows(QModelIndex(),0,getFctConvertsCount()-1);
        endInsertRows();
    }

} // setPhysUnit

/*==============================================================================
public: // must overridables of base class QAbstractTableModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelFctConvertExternal::rowCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return getFctConvertsCount();
}

//------------------------------------------------------------------------------
int CModelFctConvertExternal::columnCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return EColumnCount;
}

//------------------------------------------------------------------------------
QVariant CModelFctConvertExternal::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant     varData;
    int          iRow = i_modelIdx.row();
    int          iCol = i_modelIdx.column();
    CFctConvert* pfctConvert = nullptr;
    CPhysUnit*   pPhysUnitDst = nullptr;

    if( !i_modelIdx.isValid() )
    {
        return varData;
    }
    if( m_pPhysUnit == nullptr )
    {
        return varData;
    }
    if( iRow < 0 || iRow >= static_cast<int>(m_pPhysUnit->getFctConvertsExternalCount()) )
    {
        return varData;
    }
    if( iCol < 0 || iCol >= columnCount() )
    {
        return varData;
    }
    pfctConvert = m_pPhysUnit->getFctConvertExternal(static_cast<unsigned int>(iRow));
    if( pfctConvert == nullptr )
    {
        return varData;
    }
    pPhysUnitDst = pfctConvert->m_pPhysUnitDst;
    if( pPhysUnitDst == nullptr )
    {
        return varData;
    }

    switch( i_modelIdx.column() )
    {
        case EColumnUnitGrp:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = pPhysUnitDst->parentBranchKeyInTree();
            }
            break;
        }
        case EColumnFctConvert:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = m_pPhysUnit->findFctConvertExternalName(pPhysUnitDst);
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
QVariant CModelFctConvertExternal::headerData(
    int             i_iSection,
    Qt::Orientation i_orientation,
    int             i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    switch( i_iSection )
    {
        case EColumnUnitGrp:
        {
            if( i_iRole == Qt::DisplayRole )
            {
                if( i_orientation == Qt::Horizontal )
                {
                    varData = "Unit Group";
                }
            }
            break;
        }
        case EColumnFctConvert:
        {
            if( i_iRole == Qt::DisplayRole )
            {
                if( i_orientation == Qt::Horizontal )
                {
                    varData = "Function";
                }
            }
            break;
        }
        default:
        {
            if( i_iRole == Qt::DisplayRole )
            {
                if( i_orientation == Qt::Horizontal )
                {
                    varData = "?";
                }
            }
            break;
        }
    }
    return varData;

} // headerData
