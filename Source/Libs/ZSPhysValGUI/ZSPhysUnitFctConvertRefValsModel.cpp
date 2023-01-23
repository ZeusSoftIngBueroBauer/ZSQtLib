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

#include "ZSPhysValGUI/ZSPhysUnitFctConvertRefValsModel.h"
#include "ZSPhysVal/ZSPhysTreeEntryGrpPhysUnits.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;


/*******************************************************************************
class CModelUnitFctConvertRefVals : public QAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelUnitFctConvertRefVals::CModelUnitFctConvertRefVals( QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    QAbstractTableModel(i_pObjParent),
    m_strKeyInTreeOfRootEntry(),
    m_pUnitsGrp(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
CModelUnitFctConvertRefVals::~CModelUnitFctConvertRefVals()
//------------------------------------------------------------------------------
{
    //m_strKeyInTreeOfRootEntry;
    m_pUnitsGrp = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelUnitFctConvertRefVals::setKeyInTreeOfRootEntry( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    if( m_strKeyInTreeOfRootEntry.compare(i_strKeyInTree) != 0 )
    {
        if( rowCount() > 0 )
        {
            beginRemoveRows(QModelIndex(), 0, rowCount()-1);
            endRemoveRows();
        }

        m_pUnitsGrp = nullptr;

        CIdxTreeEntry* pTreeEntry = CIdxTreeUnits::GetInstance()->findEntry(i_strKeyInTree);

        if( pTreeEntry != nullptr && pTreeEntry->isBranch() )
        {
            m_pUnitsGrp = dynamic_cast<CUnitsTreeEntryGrpBase*>(pTreeEntry);
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
int CModelUnitFctConvertRefVals::rowCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    int iRowCount = 0;

    if( m_pUnitsGrp != nullptr )
    {
        iRowCount = m_pUnitsGrp->getReferenceValuesCount();
    }
    return iRowCount;
}

//------------------------------------------------------------------------------
int CModelUnitFctConvertRefVals::columnCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return EColumnCount;
}

//------------------------------------------------------------------------------
QVariant CModelUnitFctConvertRefVals::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    int iRow = i_modelIdx.row();
    int iCol = i_modelIdx.column();

    if( !i_modelIdx.isValid() )
    {
        return varData;
    }
    if( m_pUnitsGrp == nullptr )
    {
        return varData;
    }
    if( iRow < 0 || iRow >= static_cast<int>(m_pUnitsGrp->getReferenceValuesCount()) )
    {
        return varData;
    }
    if( iCol < 0 || iCol >= columnCount() )
    {
        return varData;
    }

    switch( i_modelIdx.column() )
    {
        case EColumnName:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = m_pUnitsGrp->getReferenceValueName(iRow);
            }
            break;
        }
        case EColumnValue:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = m_pUnitsGrp->getReferenceValue(iRow).toString();
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
QVariant CModelUnitFctConvertRefVals::headerData(
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
        case EColumnValue:
        {
            if( i_iRole == Qt::DisplayRole )
            {
                if( i_orientation == Qt::Horizontal )
                {
                    varData = "Value";
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
