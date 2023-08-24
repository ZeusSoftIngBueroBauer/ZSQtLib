/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
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

#include "ZSPhysValGUI/ZSPhysUnitFctConvertInternalModel.h"
#include "ZSPhysVal/ZSPhysTreeEntryPhysUnit.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;


/*******************************************************************************
class CModelUnitFctConvertInternal : public QAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelUnitFctConvertInternal::CModelUnitFctConvertInternal( QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    QAbstractTableModel(i_pObjParent),
    m_strKeyInTreeOfRootEntry(),
    m_pPhysUnit(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
CModelUnitFctConvertInternal::~CModelUnitFctConvertInternal()
//------------------------------------------------------------------------------
{
    //m_strKeyInTreeOfRootEntry;
    m_pPhysUnit = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelUnitFctConvertInternal::setKeyInTreeOfRootEntry( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    if( m_strKeyInTreeOfRootEntry.compare(i_strKeyInTree) != 0 )
    {
        if( rowCount() > 0 )
        {
            beginRemoveRows(QModelIndex(), 0, rowCount()-1);
            endRemoveRows();
        }

        m_pPhysUnit = nullptr;

        CIdxTreeEntry* pTreeEntry = CIdxTreeUnits::GetInstance()->findEntry(i_strKeyInTree);

        if( pTreeEntry != nullptr && pTreeEntry->isLeave() )
        {
            m_pPhysUnit = dynamic_cast<CUnitsTreeEntryPhysUnit*>(pTreeEntry);
        }

        if( rowCount() > 0 )
        {
            beginInsertRows(QModelIndex(), 0, rowCount()-1);
            endInsertRows();
        }
    } // if( m_strKeyInTreeOfRootEntry.compare(i_strKeyInTree) != 0 )
} // setKeyInTreeOfRootEntry

/*==============================================================================
public: // must overridables of base class QAbstractTableModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelUnitFctConvertInternal::rowCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    int iRowCount = 0;

    if( m_pPhysUnit != nullptr )
    {
        iRowCount = m_pPhysUnit->getFctConvertsInternalCount();
    }
    return iRowCount;
}

//------------------------------------------------------------------------------
int CModelUnitFctConvertInternal::columnCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return EColumnCount;
}

//------------------------------------------------------------------------------
QVariant CModelUnitFctConvertInternal::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    int iRow = i_modelIdx.row();
    int iCol = i_modelIdx.column();
    SFctConvert* pfctConvert = nullptr;
    CUnitsTreeEntryPhysUnit* pPhysUnitDst = nullptr;

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
QVariant CModelUnitFctConvertInternal::headerData(
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
