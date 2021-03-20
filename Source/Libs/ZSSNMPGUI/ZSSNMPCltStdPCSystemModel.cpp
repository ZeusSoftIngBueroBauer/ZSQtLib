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
#include "ZSSNMP/Include/ZSSNMPCltStdPCSystemModel.h"
#include "ZSSys/Include/ZSSysErrResult.h"
#include "ZSSys/Include/ZSSysException.h"
#include "ZSSys/Include/ZSSysMemLeakDump.h"


using namespace ZS::SNMP;


/*******************************************************************************
class CModelSystem : public QAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelSystem::CModelSystem( CCltStdPC* i_pCltStdPC ) :
//------------------------------------------------------------------------------
    QAbstractTableModel(),
    m_pCltStdPC(i_pCltStdPC),
    m_ptrListValues()
{
} // ctor

//------------------------------------------------------------------------------
CModelSystem::~CModelSystem()
//------------------------------------------------------------------------------
{
    clear();

    m_pCltStdPC = NULL;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelSystem::clear()
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
int CModelSystem::addValues( SSystemValues* i_pValues, bool i_bTakeOwnerShip )
//------------------------------------------------------------------------------
{
    int            iRow = m_ptrListValues.count();
    SSystemValues* pValues = i_pValues;

    beginInsertRows( QModelIndex(), iRow, iRow );

    if( !i_bTakeOwnerShip )
    {
        pValues = new SSystemValues(*i_pValues);
    }
    m_ptrListValues.append(pValues);

    endInsertRows();

    return iRow;

} // addValues

//------------------------------------------------------------------------------
void CModelSystem::setValues( int i_iRow, SSystemValues* i_pValues, bool i_bTakeOwnerShip )
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
                SSystemValues* pValues = m_ptrListValues[i_iRow];

                if( pValues == NULL )
                {
                    pValues = new SSystemValues(*i_pValues);
                }
                *pValues = *i_pValues;
            }
        }
        emit dataChanged( index(i_iRow,0), index(i_iRow,EColumnCount-1) );
    }

} // setValue( SSystemValues* )

//------------------------------------------------------------------------------
void CModelSystem::setValue( int i_iRow, const QDateTime& i_dateTime )
//------------------------------------------------------------------------------
{
    SSystemValues* pValues = NULL;

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
void CModelSystem::setValue( int i_iRow, int i_iClm, const COid& i_oid )
//------------------------------------------------------------------------------
{
    SSystemValues* pValues = NULL;

    if( i_iRow >= 0 && i_iRow < rowCount() && i_iClm >= 0 && i_iClm < EColumnCount )
    {
        pValues = m_ptrListValues[i_iRow];
    }
    if( pValues != NULL )
    {
        switch( i_iClm )
        {
            case EColumnSysDescrOid:
            {
                pValues->m_oidSysDescr = i_oid;
                break;
            }
            case EColumnSysNameOid:
            {
                pValues->m_oidSysName = i_oid;
                break;
            }
            case EColumnUpdateTime:
            case EColumnSysDescrValue:
            case EColumnSysNameValue:
            default:
            {
                break;
            }
        }
        emit dataChanged( index(i_iRow,i_iClm), index(i_iRow,i_iClm) );
    }

} // setValue( const COid& )

//------------------------------------------------------------------------------
void CModelSystem::setValue( int i_iRow, int i_iClm, const QString& i_strValue )
//------------------------------------------------------------------------------
{
    SSystemValues* pValues = NULL;

    if( i_iRow >= 0 && i_iRow < rowCount() && i_iClm >= 0 && i_iClm < EColumnCount )
    {
        pValues = m_ptrListValues[i_iRow];
    }
    if( pValues != NULL )
    {
        switch( i_iClm )
        {
            case EColumnSysDescrValue:
            {
                pValues->m_strSysDescrValue = i_strValue;
                break;
            }
            case EColumnSysNameValue:
            {
                pValues->m_strSysNameValue = i_strValue;
                break;
            }
            case EColumnUpdateTime:
            case EColumnSysDescrOid:
            case EColumnSysNameOid:
            default:
            {
                break;
            }
        }
        emit dataChanged( index(i_iRow,i_iClm), index(i_iRow,i_iClm) );
    }

} // setValue( const QString& )

//------------------------------------------------------------------------------
SSystemValues* CModelSystem::getValues( int i_iRow )
//------------------------------------------------------------------------------
{
    SSystemValues* pValues = NULL;

    if( i_iRow >= 0 && i_iRow < rowCount() )
    {
        pValues = m_ptrListValues[i_iRow];
    }
    return pValues;

} // getValues

//------------------------------------------------------------------------------
QDateTime CModelSystem::getValueDateTime( int i_iRow ) const
//------------------------------------------------------------------------------
{
    QDateTime      dateTime;
    SSystemValues* pValues = NULL;

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
COid* CModelSystem::getValueOid( int i_iRow, int i_iClm )
//------------------------------------------------------------------------------
{
    COid*          pOid = NULL;
    SSystemValues* pValues = NULL;

    if( i_iRow >= 0 && i_iRow < rowCount() && i_iClm >= 0 && i_iClm < EColumnCount )
    {
        pValues = m_ptrListValues[i_iRow];
    }
    if( pValues != NULL )
    {
        switch( i_iClm )
        {
            case EColumnSysDescrOid:
            {
                pOid = &pValues->m_oidSysDescr;
                break;
            }
            case EColumnSysNameOid:
            {
                pOid = &pValues->m_oidSysName;
                break;
            }
            case EColumnUpdateTime:
            case EColumnSysDescrValue:
            case EColumnSysNameValue:
            default:
            {
                break;
            }
        }
    }
    return pOid;

} // getOid

//------------------------------------------------------------------------------
QString CModelSystem::getValueStr( int i_iRow, int i_iClm ) const
//------------------------------------------------------------------------------
{
    QString        strValue;
    SSystemValues* pValues = NULL;

    if( i_iRow >= 0 && i_iRow < rowCount() && i_iClm >= 0 && i_iClm < EColumnCount )
    {
        pValues = m_ptrListValues[i_iRow];
    }
    if( pValues != NULL )
    {
        switch( i_iClm )
        {
            case EColumnSysDescrValue:
            {
                strValue = pValues->m_strSysDescrValue;
                break;
            }
            case EColumnSysNameValue:
            {
                strValue = pValues->m_strSysNameValue;
                break;
            }
            case EColumnUpdateTime:
            case EColumnSysDescrOid:
            case EColumnSysNameOid:
            default:
            {
                break;
            }
        }
    }
    return strValue;

} // getValueStr

/*==============================================================================
public: // must overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelSystem::rowCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return m_ptrListValues.count();
}

//------------------------------------------------------------------------------
int CModelSystem::columnCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return EColumnCount;
}

//------------------------------------------------------------------------------
QVariant CModelSystem::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    if( !i_modelIdx.isValid() )
    {
        return varData;
    }

    int iRow = i_modelIdx.row();

    const SSystemValues* pCValues = NULL;

    if( iRow >= 0 && iRow < rowCount() )
    {
        pCValues = m_ptrListValues[iRow];
    }
    if( pCValues == NULL )
    {
        return varData;
    }

    SSystemValues* pVValues = const_cast<SSystemValues*>(pCValues);

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
        case EColumnSysDescrOid:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = pVValues->m_oidSysDescr.toString();
            }
            break;
        }
        case EColumnSysDescrValue:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = pVValues->m_strSysDescrValue;
            }
            break;
        }
        case EColumnSysNameOid:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = pVValues->m_oidSysName.toString();
            }
            break;
        }
        case EColumnSysNameValue:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = pVValues->m_strSysNameValue;
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
QVariant CModelSystem::headerData(
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
            case EColumnSysDescrOid:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = "System Descr. OID";
                }
                break;
            }
            case EColumnSysDescrValue:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = "System Descr.";
                }
                break;
            }
            case EColumnSysNameOid:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = "System Name OID";
                }
                break;
            }
            case EColumnSysNameValue:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = "System Name";
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
