/*******************************************************************************

Copyright 2004 - 2010 by    ZeusSoft, Ing. Buero Bauer
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

--------------------------------------------------------------------------------

$Date: $

$Revision: $

$Author: $

$History: $

*******************************************************************************/

#include <QtCore/qdatetime.h>
#include <QtCore/qfile.h>
#include <QtCore/qtextstream.h>
#include <QtCore/qthread.h>
#include <QtXml/qdom.h>
#include "ZSSNMP/Include/ZSSNMPCltStdPCStoragesModel.h"
#include "ZSSys/Include/ZSSysErrResult.h"
#include "ZSSys/Include/ZSSysException.h"
#include "ZSSys/Include/ZSSysMemLeakDump.h"


using namespace ZS::SNMP;


/*******************************************************************************
class CModelStorages : public QAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelStorages::CModelStorages( CCltStdPC* i_pCltStdPC ) :
//------------------------------------------------------------------------------
    QAbstractTableModel(),
    m_pCltStdPC(i_pCltStdPC),
    m_ptrListValues()
{
} // ctor

//------------------------------------------------------------------------------
CModelStorages::~CModelStorages()
//------------------------------------------------------------------------------
{
    clear();

    m_pCltStdPC = NULL;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelStorages::clear()
//------------------------------------------------------------------------------
{
    if( rowCount() > 0 )
    {
        beginRemoveRows(QModelIndex(),0,rowCount()-1);

        int idxRow;

        for( idxRow = rowCount()-1; idxRow >= 0; idxRow-- )
        {
            try
            {
                delete m_ptrListValues[idxRow];
            }
            catch(...)
            {
            }
            m_ptrListValues[idxRow] = NULL;
        }
        m_ptrListValues.clear();

        endRemoveRows();
    }

    reset();

} // clear

//------------------------------------------------------------------------------
int CModelStorages::addValues( SStorageValues* i_pValues, bool i_bTakeOwnerShip )
//------------------------------------------------------------------------------
{
    int             iRow = m_ptrListValues.count();
    SStorageValues* pValues = i_pValues;

    beginInsertRows( QModelIndex(), iRow, iRow );

    if( !i_bTakeOwnerShip )
    {
        pValues = new SStorageValues(*i_pValues);
    }
    m_ptrListValues.append(pValues);

    endInsertRows();

    return iRow;

} // addValues

//------------------------------------------------------------------------------
void CModelStorages::setValues( int i_iRow, SStorageValues* i_pValues, bool i_bTakeOwnerShip )
//------------------------------------------------------------------------------
{
    if( i_iRow >= 0 && i_iRow < rowCount() )
    {
        if( i_bTakeOwnerShip )
        {
            if( i_pValues != m_ptrListValues[i_iRow] )
            {
                try
                {
                    delete m_ptrListValues[i_iRow];
                }
                catch(...)
                {
                }
                m_ptrListValues[i_iRow] = i_pValues;
            }
        }
        else
        {
            if( i_pValues == NULL )
            {
                try
                {
                    delete m_ptrListValues[i_iRow];
                }
                catch(...)
                {
                }
                m_ptrListValues[i_iRow] = i_pValues;
            }
            else // if( i_pValues != NULL )
            {
                SStorageValues* pValues = m_ptrListValues[i_iRow];

                if( pValues == NULL )
                {
                    pValues = new SStorageValues(*i_pValues);
                }
                *pValues = *i_pValues;
            }
        }
        emit dataChanged( index(i_iRow,0), index(i_iRow,EColumnCount-1) );
    }

} // setValue( SStorageValues* )

//------------------------------------------------------------------------------
void CModelStorages::setValue( int i_iRow, const QDateTime& i_dateTime )
//------------------------------------------------------------------------------
{
    SStorageValues* pValues = NULL;

    if( i_iRow >= 0 && i_iRow < rowCount() )
    {
        pValues = m_ptrListValues[i_iRow];
    }
    if( pValues != NULL )
    {
        pValues->m_dateTimeUpdate = i_dateTime;
        emit dataChanged( index(i_iRow,EColumnUpdateTime), index(i_iRow,EColumnUpdateTime) );
    }

} // setValue( QDateTime )

//------------------------------------------------------------------------------
void CModelStorages::setValue( int i_iRow, int i_iClm, const COid& i_oid )
//------------------------------------------------------------------------------
{
    SStorageValues* pValues = NULL;

    if( i_iRow >= 0 && i_iRow < rowCount() && i_iClm >= 0 && i_iClm < EColumnCount )
    {
        pValues = m_ptrListValues[i_iRow];
    }
    if( pValues != NULL )
    {
        switch( i_iClm )
        {
            case EColumnTypeOid:
            {
                pValues->m_oidType = i_oid;
                break;
            }
            case EColumnDescrOid:
            {
                pValues->m_oidDescr = i_oid;
                break;
            }
            case EColumnAllocationUnitsOid:
            {
                pValues->m_oidAllocationUnits = i_oid;
                break;
            }
            case EColumnSizeOid:
            {
                pValues->m_oidSize = i_oid;
                break;
            }
            case EColumnUsedOid:
            {
                pValues->m_oidUsed = i_oid;
                break;
            }
            case EColumnInstNr:
            case EColumnTypeValue:
            case EColumnStorageType:
            case EColumnDescrValue:
            case EColumnAllocationUnitsValue:
            case EColumnSizeValue:
            case EColumnUsedValue:
            default:
            {
                break;
            }
        }
        emit dataChanged( index(i_iRow,i_iClm), index(i_iRow,i_iClm) );
    }

} // setValue( const COid& )

//------------------------------------------------------------------------------
void CModelStorages::setValue( int i_iClm, int i_iRow, const QString& i_strValue )
//------------------------------------------------------------------------------
{
    SStorageValues* pValues = NULL;

    if( i_iRow >= 0 && i_iRow < rowCount() && i_iClm >= 0 && i_iClm < EColumnCount )
    {
        pValues = m_ptrListValues[i_iRow];
    }
    if( pValues != NULL )
    {
        switch( i_iClm )
        {
            case EColumnTypeValue:
            {
                pValues->m_strTypeValue = i_strValue;
                break;
            }
            case EColumnDescrValue:
            {
                pValues->m_strDescrValue = i_strValue;
                break;
            }
            case EColumnInstNr:
            case EColumnTypeOid:
            case EColumnDescrOid:
            case EColumnAllocationUnitsOid:
            case EColumnSizeOid:
            case EColumnUsedOid:
            case EColumnStorageType:
            case EColumnAllocationUnitsValue:
            case EColumnSizeValue:
            case EColumnUsedValue:
            default:
            {
                break;
            }
        }
        emit dataChanged( index(i_iRow,i_iClm), index(i_iRow,i_iClm) );
    }

} // setValue( const QString& )

//------------------------------------------------------------------------------
void CModelStorages::setValue( int i_iClm, int i_iRow, int i_iValue )
//------------------------------------------------------------------------------
{
    SStorageValues* pValues = NULL;

    if( i_iRow >= 0 && i_iRow < rowCount() && i_iClm >= 0 && i_iClm < EColumnCount )
    {
        pValues = m_ptrListValues[i_iRow];
    }
    if( pValues != NULL )
    {
        switch( i_iClm )
        {
            case EColumnInstNr:
            {
                pValues->m_iInstNr = i_iValue;
                break;
            }
            case EColumnStorageType:
            {
                if( i_iValue >= 0 && i_iValue < EStorageTypeCount )
                {
                    pValues->m_storageType = static_cast<EStorageType>(i_iValue);
                }
                else
                {
                    pValues->m_storageType = EStorageTypeUndefined;
                }
                break;
            }
            case EColumnAllocationUnitsValue:
            {
                pValues->m_iAllocationUnits_byte = i_iValue;
                break;
            }
            case EColumnSizeValue:
            {
                pValues->m_iSize_AllocationUnits = i_iValue;
                break;
            }
            case EColumnUsedValue:
            {
                pValues->m_iUsed_AllocationUnits = i_iValue;
                break;
            }
            case EColumnTypeOid:
            case EColumnDescrOid:
            case EColumnAllocationUnitsOid:
            case EColumnSizeOid:
            case EColumnUsedOid:
            case EColumnTypeValue:
            case EColumnDescrValue:
            default:
            {
                break;
            }
        }
        emit dataChanged( index(i_iRow,i_iClm), index(i_iRow,i_iClm) );
    }

} // setValue( int )

//------------------------------------------------------------------------------
SStorageValues* CModelStorages::getValues( int i_iRow )
//------------------------------------------------------------------------------
{
    SStorageValues* pValues = NULL;

    if( i_iRow >= 0 && i_iRow < rowCount() )
    {
        pValues = m_ptrListValues[i_iRow];
    }
    return pValues;

} // getValues

//------------------------------------------------------------------------------
QDateTime CModelStorages::getValueDateTime( int i_iRow ) const
//------------------------------------------------------------------------------
{
    QDateTime      dateTime;
    SStorageValues* pValues = NULL;

    if( i_iRow >= 0 && i_iRow < rowCount() )
    {
        pValues = m_ptrListValues[i_iRow];
    }
    if( pValues != NULL )
    {
        dateTime = pValues->m_dateTimeUpdate;
    }
    return dateTime;

} // getUpdateTime

//------------------------------------------------------------------------------
COid* CModelStorages::getValueOid( int i_iClm, int i_iRow )
//------------------------------------------------------------------------------
{
    COid*          pOid = NULL;
    SStorageValues* pValues = NULL;

    if( i_iRow >= 0 && i_iRow < rowCount() && i_iClm >= 0 && i_iClm < EColumnCount )
    {
        pValues = m_ptrListValues[i_iRow];
    }
    if( pValues != NULL )
    {
        switch( i_iClm )
        {
            case EColumnTypeOid:
            {
                pOid = &pValues->m_oidType;
                break;
            }
            case EColumnDescrOid:
            {
                pOid = &pValues->m_oidDescr;
                break;
            }
            case EColumnAllocationUnitsOid:
            {
                pOid = &pValues->m_oidAllocationUnits;
                break;
            }
            case EColumnSizeOid:
            {
                pOid = &pValues->m_oidSize;
                break;
            }
            case EColumnUsedOid:
            {
                pOid = &pValues->m_oidUsed;
                break;
            }
            case EColumnInstNr:
            case EColumnTypeValue:
            case EColumnDescrValue:
            case EColumnAllocationUnitsValue:
            case EColumnSizeValue:
            case EColumnUsedValue:
            default:
            {
                break;
            }
        }
    }
    return pOid;

} // getOid

//------------------------------------------------------------------------------
QString CModelStorages::getValueStr( int i_iClm, int i_iRow ) const
//------------------------------------------------------------------------------
{
    QString        strValue;
    SStorageValues* pValues = NULL;

    if( i_iRow >= 0 && i_iRow < rowCount() && i_iClm >= 0 && i_iClm < EColumnCount )
    {
        pValues = m_ptrListValues[i_iRow];
    }
    if( pValues != NULL )
    {
        switch( i_iClm )
        {
            case EColumnTypeValue:
            {
                strValue = pValues->m_strTypeValue;
                break;
            }
            case EColumnStorageType:
            {
                strValue = storageType2Str(pValues->m_storageType);
                break;
            }
            case EColumnDescrValue:
            {
                strValue = pValues->m_strDescrValue;
                break;
            }
            case EColumnInstNr:
            case EColumnTypeOid:
            case EColumnDescrOid:
            case EColumnAllocationUnitsOid:
            case EColumnSizeOid:
            case EColumnUsedOid:
            case EColumnAllocationUnitsValue:
            case EColumnSizeValue:
            case EColumnUsedValue:
            default:
            {
                break;
            }
        }
    }
    return strValue;

} // getValueStr

//------------------------------------------------------------------------------
int CModelStorages::getValueInt( int i_iClm, int i_iRow ) const
//------------------------------------------------------------------------------
{
    int             iValue = 0;
    SStorageValues* pValues = NULL;

    if( i_iRow >= 0 && i_iRow < rowCount() && i_iClm >= 0 && i_iClm < EColumnCount )
    {
        pValues = m_ptrListValues[i_iRow];
    }
    if( pValues != NULL )
    {
        switch( i_iClm )
        {
            case EColumnInstNr:
            {
                iValue = pValues->m_iInstNr;
                break;
            }
            case EColumnStorageType:
            {
                iValue = pValues->m_storageType;
                break;
            }
            case EColumnAllocationUnitsValue:
            {
                iValue = pValues->m_iAllocationUnits_byte;
                break;
            }
            case EColumnSizeValue:
            {
                iValue = pValues->m_iSize_AllocationUnits;
                break;
            }
            case EColumnUsedValue:
            {
                iValue = pValues->m_iUsed_AllocationUnits;
                break;
            }
            case EColumnTypeOid:
            case EColumnDescrOid:
            case EColumnAllocationUnitsOid:
            case EColumnSizeOid:
            case EColumnUsedOid:
            case EColumnTypeValue:
            case EColumnDescrValue:
            default:
            {
                break;
            }
        }
    }
    return iValue;

} // getValueInt

/*==============================================================================
public: // must overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelStorages::rowCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return m_ptrListValues.count();
}

//------------------------------------------------------------------------------
int CModelStorages::columnCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return EColumnCount;
}

//------------------------------------------------------------------------------
QVariant CModelStorages::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    if( !i_modelIdx.isValid() )
    {
        return varData;
    }

    int iRow = i_modelIdx.row();

    const SStorageValues* pCValues = NULL;

    if( iRow >= 0 && iRow < rowCount() )
    {
        pCValues = m_ptrListValues[iRow];
    }
    if( pCValues == NULL )
    {
        return varData;
    }

    SStorageValues* pVValues = const_cast<SStorageValues*>(pCValues);

    switch( i_modelIdx.column() )
    {
        case EColumnUpdateTime:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = pVValues->m_dateTimeUpdate.toString("yyyy-MM-dd hh:mm:ss:zzz");
            }
            break;
        }
        case EColumnInstNr:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = pVValues->m_iInstNr;
            }
            break;
        }
        case EColumnTypeOid:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = pVValues->m_oidType.toString();
            }
            break;
        }
        case EColumnTypeValue:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = pVValues->m_strTypeValue;
            }
            break;
        }
        case EColumnStorageType:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = storageType2Str(pVValues->m_storageType);
            }
            break;
        }
        case EColumnDescrOid:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = pVValues->m_oidDescr.toString();
            }
            break;
        }
        case EColumnDescrValue:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = pVValues->m_strDescrValue;
            }
            break;
        }
        case EColumnAllocationUnitsOid:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = pVValues->m_oidAllocationUnits.toString();
            }
            break;
        }
        case EColumnAllocationUnitsValue:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = pVValues->m_iAllocationUnits_byte;
            }
            break;
        }
        case EColumnSizeOid:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = pVValues->m_oidSize.toString();
            }
            break;
        }
        case EColumnSizeValue:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = pVValues->m_iSize_AllocationUnits;
            }
            break;
        }
        case EColumnUsedOid:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = pVValues->m_oidUsed.toString();
            }
            break;
        }
        case EColumnUsedValue:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = pVValues->m_iUsed_AllocationUnits;
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

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CModelStorages::headerData(
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
            case EColumnUpdateTime:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Update Time";
                }
                break;
            }
            case EColumnInstNr:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = "Instance";
                }
                break;
            }
            case EColumnTypeOid:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = "Type OID";
                }
                break;
            }
            case EColumnTypeValue:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = "Type";
                }
                break;
            }
            case EColumnStorageType:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = "Storage Type";
                }
                break;
            }
            case EColumnDescrOid:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = "Description OID";
                }
                break;
            }
            case EColumnDescrValue:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = "Description";
                }
                break;
            }
            case EColumnAllocationUnitsOid:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = "Allocation Units OID";
                }
                break;
            }
            case EColumnAllocationUnitsValue:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = "Allocation Units";
                }
                break;
            }
            case EColumnSizeOid:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = "Size OID";
                }
                break;
            }
            case EColumnSizeValue:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = "Size / Allocation Units";
                }
                break;
            }
            case EColumnUsedOid:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = "Used OID";
                }
                break;
            }
            case EColumnUsedValue:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = "Used / Allocation Units";
                }
                break;
            }
            default:
            {
                break;
            }
        }
    }
    return varData;

} // headerData
