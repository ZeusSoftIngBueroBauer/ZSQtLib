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
#include "ZSSNMP/Include/ZSSNMPCltStdPCProcessorsModel.h"
#include "ZSSys/Include/ZSSysErrResult.h"
#include "ZSSys/Include/ZSSysException.h"
#include "ZSSys/Include/ZSSysMemLeakDump.h"


using namespace ZS::SNMP;


/*******************************************************************************
class CModelProcessors : public QAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelProcessors::CModelProcessors( CCltStdPC* i_pCltStdPC ) :
//------------------------------------------------------------------------------
    QAbstractTableModel(),
    m_pCltStdPC(i_pCltStdPC),
    m_ptrListValues()
{
} // ctor

//------------------------------------------------------------------------------
CModelProcessors::~CModelProcessors()
//------------------------------------------------------------------------------
{
    clear();

    m_pCltStdPC = NULL;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelProcessors::clear()
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
int CModelProcessors::addValues( SProcessorValues* i_pValues, bool i_bTakeOwnerShip )
//------------------------------------------------------------------------------
{
    int               iRow = m_ptrListValues.count();
    SProcessorValues* pValues = i_pValues;

    beginInsertRows( QModelIndex(), iRow, iRow );

    if( !i_bTakeOwnerShip )
    {
        pValues = new SProcessorValues(*i_pValues);
    }
    m_ptrListValues.append(pValues);

    endInsertRows();

    return iRow;

} // addValues

//------------------------------------------------------------------------------
void CModelProcessors::setValues( int i_iRow, SProcessorValues* i_pValues, bool i_bTakeOwnerShip )
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
                SProcessorValues* pValues = m_ptrListValues[i_iRow];

                if( pValues == NULL )
                {
                    pValues = new SProcessorValues(*i_pValues);
                }
                *pValues = *i_pValues;
            }
        }
        emit dataChanged( index(i_iRow,0), index(i_iRow,EColumnCount-1) );
    }

} // setValue( SProcessorValues* )

//------------------------------------------------------------------------------
void CModelProcessors::setValue( int i_iRow, const QDateTime& i_dateTime )
//------------------------------------------------------------------------------
{
    SProcessorValues* pValues = NULL;

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
void CModelProcessors::setValue( int i_iRow, int i_iClm, const COid& i_oid )
//------------------------------------------------------------------------------
{
    SProcessorValues* pValues = NULL;

    if( i_iRow >= 0 && i_iRow < rowCount() && i_iClm >= 0 && i_iClm < EColumnCount )
    {
        pValues = m_ptrListValues[i_iRow];
    }
    if( pValues != NULL )
    {
        switch( i_iClm )
        {
            case EColumnLoadOid:
            {
                pValues->m_oidLoad = i_oid;
                break;
            }
            case EColumnInstNr:
            case EColumnUpdateTime:
            case EColumnLoadValue:
            default:
            {
                break;
            }
        }
        emit dataChanged( index(i_iRow,i_iClm), index(i_iRow,i_iClm) );
    }

} // setValue( const COid& )

//------------------------------------------------------------------------------
void CModelProcessors::setValue( int i_iClm, int i_iRow, int i_iValue )
//------------------------------------------------------------------------------
{
    SProcessorValues* pValues = NULL;

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
            case EColumnLoadValue:
            {
                pValues->m_iLoad_PerCent = i_iValue;
                break;
            }
            case EColumnUpdateTime:
            case EColumnLoadOid:
            default:
            {
                break;
            }
        }
        emit dataChanged( index(i_iRow,i_iClm), index(i_iRow,i_iClm) );
    }

} // setValue( int )

//------------------------------------------------------------------------------
SProcessorValues* CModelProcessors::getValues( int i_iRow )
//------------------------------------------------------------------------------
{
    SProcessorValues* pValues = NULL;

    if( i_iRow >= 0 && i_iRow < rowCount() )
    {
        pValues = m_ptrListValues[i_iRow];
    }
    return pValues;

} // getValues

//------------------------------------------------------------------------------
QDateTime CModelProcessors::getValueDateTime( int i_iRow ) const
//------------------------------------------------------------------------------
{
    QDateTime      dateTime;
    SProcessorValues* pValues = NULL;

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
COid* CModelProcessors::getValueOid( int i_iRow, int i_iClm )
//------------------------------------------------------------------------------
{
    COid*             pOid = NULL;
    SProcessorValues* pValues = NULL;

    if( i_iRow >= 0 && i_iRow < rowCount() && i_iClm >= 0 && i_iClm < EColumnCount )
    {
        pValues = m_ptrListValues[i_iRow];
    }
    if( pValues != NULL )
    {
        switch( i_iClm )
        {
            case EColumnLoadOid:
            {
                pOid = &pValues->m_oidLoad;
                break;
            }
            case EColumnUpdateTime:
            case EColumnInstNr:
            case EColumnLoadValue:
            default:
            {
                break;
            }
        }
    }
    return pOid;

} // getOid

//------------------------------------------------------------------------------
int CModelProcessors::getValueInt( int i_iRow, int i_iClm ) const
//------------------------------------------------------------------------------
{
    int               iValue = 0;
    SProcessorValues* pValues = NULL;

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
            case EColumnLoadValue:
            {
                iValue = pValues->m_iLoad_PerCent;
                break;
            }
            case EColumnUpdateTime:
            case EColumnLoadOid:
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
int CModelProcessors::rowCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return m_ptrListValues.count();
}

//------------------------------------------------------------------------------
int CModelProcessors::columnCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return EColumnCount;
}

//------------------------------------------------------------------------------
QVariant CModelProcessors::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    if( !i_modelIdx.isValid() )
    {
        return varData;
    }

    int iRow = i_modelIdx.row();

    const SProcessorValues* pCValues = NULL;

    if( iRow >= 0 && iRow < rowCount() )
    {
        pCValues = m_ptrListValues[iRow];
    }
    if( pCValues == NULL )
    {
        return varData;
    }

    SProcessorValues* pVValues = const_cast<SProcessorValues*>(pCValues);

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
        case EColumnLoadOid:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = pVValues->m_oidLoad.toString();
            }
            break;
        }
        case EColumnLoadValue:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = pVValues->m_iLoad_PerCent;
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
QVariant CModelProcessors::headerData(
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
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Instance";
                }
                break;
            }
            case EColumnLoadOid:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Processor Load OID";
                }
                break;
            }
            case EColumnLoadValue:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Processor Load";
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
