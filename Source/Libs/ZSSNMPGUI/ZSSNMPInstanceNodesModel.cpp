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
#include "ZSSNMP/Include/ZSSNMPInstanceNodesModel.h"
#include "ZSSNMP/Include/ZSSNMPInstanceNode.h"
#include "ZSSNMP/Include/ZSSNMPOidNode.h"
#include "ZSSys/Include/ZSSysErrResult.h"
#include "ZSSys/Include/ZSSysException.h"
#include "ZSSys/Include/ZSSysMemLeakDump.h"

using namespace ZS::SNMP;


/*******************************************************************************
class CModelInstanceNodes : public QAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelInstanceNodes::CModelInstanceNodes( CExplorer* i_pExplorer ) :
//------------------------------------------------------------------------------
    QAbstractTableModel(),
    m_pExplorer(i_pExplorer),
    m_pOidNode(NULL)
{
} // ctor

//------------------------------------------------------------------------------
CModelInstanceNodes::~CModelInstanceNodes()
//------------------------------------------------------------------------------
{
    clear();

    m_pExplorer = NULL;
    m_pOidNode = NULL;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelInstanceNodes::clear()
//------------------------------------------------------------------------------
{
    if( rowCount() > 0 )
    {
        beginRemoveRows(QModelIndex(),0,rowCount()-1);
        endRemoveRows();
    }

    if( m_pOidNode != NULL )
    {
        CInstanceNode* pInstanceNode;
        unsigned int   udxRow;

        QObject::disconnect(
            /* pObjSender   */ m_pOidNode,
            /* szSignal     */ SIGNAL( instanceNodeAdded(COidNode*,CInstanceNode*) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onNodeAdded(COidNode*,CInstanceNode*) ) );
        QObject::disconnect(
            /* pObjSender   */ m_pOidNode,
            /* szSignal     */ SIGNAL( removingInstanceNode(COidNode*,CInstanceNode*) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onNodeRemoving(COidNode*,CInstanceNode*) ) );
        QObject::disconnect(
            /* pObjSender   */ m_pOidNode,
            /* szSignal     */ SIGNAL( removingInstanceNode(COidNode*,int) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onNodeRemoving(COidNode*,int) ) );

        for( udxRow = 0; udxRow < m_pOidNode->getInstanceNodesCount(); udxRow++ )
        {
            pInstanceNode = m_pOidNode->getInstanceNode(udxRow);

            if( pInstanceNode != NULL )
            {
                QObject::disconnect(
                    /* pObjSender   */ pInstanceNode,
                    /* szSignal     */ SIGNAL( nodeChanged(CInstanceNode*) ),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT( onNodeChanged(CInstanceNode*) ) );
            }
        }
    }

    m_pOidNode = NULL;

    reset();

} // clear

//------------------------------------------------------------------------------
void CModelInstanceNodes::setOidNode( COidNode* i_pOidNode )
//------------------------------------------------------------------------------
{
    if( m_pOidNode != i_pOidNode )
    {
        CInstanceNode* pInstanceNode;
        unsigned int   udxRow;

        if( m_pOidNode != NULL )
        {
            if( rowCount() > 0 )
            {
                beginRemoveRows(QModelIndex(),0,rowCount()-1);
                endRemoveRows();
            }

            QObject::disconnect(
                /* pObjSender   */ m_pOidNode,
                /* szSignal     */ SIGNAL( instanceNodeAdded(COidNode*,CInstanceNode*) ),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT( onNodeAdded(COidNode*,CInstanceNode*) ) );
            QObject::disconnect(
                /* pObjSender   */ m_pOidNode,
                /* szSignal     */ SIGNAL( removingInstanceNode(COidNode*,CInstanceNode*) ),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT( onNodeRemoving(COidNode*,CInstanceNode*) ) );
            QObject::disconnect(
                /* pObjSender   */ m_pOidNode,
                /* szSignal     */ SIGNAL( removingInstanceNode(COidNode*,int) ),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT( onNodeRemoving(COidNode*,int) ) );

            for( udxRow = 0; udxRow < m_pOidNode->getInstanceNodesCount(); udxRow++ )
            {
                pInstanceNode = m_pOidNode->getInstanceNode(udxRow);

                if( pInstanceNode != NULL )
                {
                    QObject::disconnect(
                        /* pObjSender   */ pInstanceNode,
                        /* szSignal     */ SIGNAL( nodeChanged(CInstanceNode*) ),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT( onNodeChanged(CInstanceNode*) ) );
                }
            }
        }

        m_pOidNode = i_pOidNode;

        if( m_pOidNode != NULL )
        {
            if( m_pOidNode->getInstanceNodesCount() > 0 )
            {
                beginInsertRows(QModelIndex(),0,m_pOidNode->getInstanceNodesCount()-1);
                endInsertRows();
            }

            if( !QObject::connect(
                /* pObjSender   */ m_pOidNode,
                /* szSignal     */ SIGNAL( instanceNodeAdded(COidNode*,CInstanceNode*) ),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT( onNodeAdded(COidNode*,CInstanceNode*) ) ) )
            {
                throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
            }
            if( !QObject::connect(
                /* pObjSender   */ m_pOidNode,
                /* szSignal     */ SIGNAL( removingInstanceNode(COidNode*,CInstanceNode*) ),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT( onNodeRemoving(COidNode*,CInstanceNode*) ) ) )
            {
                throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
            }
            if( !QObject::connect(
                /* pObjSender   */ m_pOidNode,
                /* szSignal     */ SIGNAL( removingInstanceNode(COidNode*,int) ),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT( onNodeRemoving(COidNode*,int) ) ) )
            {
                throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
            }

            for( udxRow = 0; udxRow < m_pOidNode->getInstanceNodesCount(); udxRow++ )
            {
                pInstanceNode = m_pOidNode->getInstanceNode(udxRow);

                if( pInstanceNode != NULL )
                {
                    if( !QObject::connect(
                        /* pObjSender   */ pInstanceNode,
                        /* szSignal     */ SIGNAL( nodeChanged(CInstanceNode*) ),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT( onNodeChanged(CInstanceNode*) ) ) )
                    {
                        throw ZS::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
                    }
                }
            }
        }
    }

} // setOidNode

//------------------------------------------------------------------------------
CInstanceNode* CModelInstanceNodes::getNode( int i_iRow )
//------------------------------------------------------------------------------
{
    CInstanceNode* pInstanceNode = NULL;

    if( i_iRow >= 0 && i_iRow < rowCount() )
    {
        pInstanceNode = m_pOidNode->getInstanceNode(i_iRow);
    }
    return pInstanceNode;

} // getNode

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CModelInstanceNodes::onNodeAdded( COidNode* i_pOidNode, CInstanceNode* i_pInstanceNode )
//------------------------------------------------------------------------------
{
    unsigned int udxRow;

    for( udxRow = 0; udxRow < i_pOidNode->getInstanceNodesCount(); udxRow++ )
    {
        if( i_pOidNode->getInstanceNode(udxRow) == i_pInstanceNode )
        {
            break;
        }
    }
    if( udxRow >= i_pOidNode->getInstanceNodesCount() )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultObjNotInList,QString::number(udxRow));
    }

    beginInsertRows(QModelIndex(),udxRow,udxRow);
    endInsertRows();

    if( !QObject::connect(
        /* pObjSender   */ i_pInstanceNode,
        /* szSignal     */ SIGNAL( nodeChanged(CInstanceNode*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onNodeChanged(CInstanceNode*) ) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }

} // onNodeAdded

//------------------------------------------------------------------------------
void CModelInstanceNodes::onNodeRemoving( COidNode* i_pOidNode, CInstanceNode* i_pInstanceNode )
//------------------------------------------------------------------------------
{
    unsigned int udxRow;

    for( udxRow = 0; udxRow < i_pOidNode->getInstanceNodesCount(); udxRow++ )
    {
        if( i_pOidNode->getInstanceNode(udxRow) == i_pInstanceNode )
        {
            break;
        }
    }
    if( udxRow >= i_pOidNode->getInstanceNodesCount() )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultObjNotInList,QString::number(udxRow));
    }

    beginRemoveRows(QModelIndex(),udxRow,udxRow);
    endRemoveRows();

    QObject::disconnect(
        /* pObjSender   */ i_pInstanceNode,
        /* szSignal     */ SIGNAL( nodeChanged(CInstanceNode*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onNodeChanged(CInstanceNode*) ) );

} // onNodeRemoving

//------------------------------------------------------------------------------
void CModelInstanceNodes::onNodeRemoving( COidNode* i_pOidNode, int i_iRowIdx )
//------------------------------------------------------------------------------
{
    CInstanceNode* pInstanceNode = NULL;

    if( i_iRowIdx >= 0 && i_iRowIdx < static_cast<int>(i_pOidNode->getInstanceNodesCount()) )
    {
        pInstanceNode = i_pOidNode->getInstanceNode(i_iRowIdx);
    }

    beginRemoveRows(QModelIndex(),i_iRowIdx,i_iRowIdx);
    endRemoveRows();

    if( pInstanceNode != NULL )
    {
        QObject::disconnect(
            /* pObjSender   */ pInstanceNode,
            /* szSignal     */ SIGNAL( nodeChanged(CInstanceNode*) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onNodeChanged(CInstanceNode*) ) );
    }

} // onNodeRemoving

//------------------------------------------------------------------------------
void CModelInstanceNodes::onNodeChanged( CInstanceNode* i_pInstanceNode )
//------------------------------------------------------------------------------
{
    if( i_pInstanceNode == NULL )
    {
        return;
    }

    QModelIndex modelIdxStart = index(
        /* iRow           */ i_pInstanceNode->getRowIdx(),
        /* iColumn        */ 0,
        /* modelIdxParent */ QModelIndex() );
    QModelIndex modelIdxEnd = index(
        /* iRow           */ i_pInstanceNode->getRowIdx(),
        /* iColumn        */ EColumnCount-1,
        /* modelIdxParent */ QModelIndex() );
    emit dataChanged(modelIdxStart,modelIdxEnd);

} // onNodeChanged

/*==============================================================================
public: // must overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelInstanceNodes::rowCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    int iRowCount = 0;

    if( m_pOidNode != NULL )
    {
        iRowCount = m_pOidNode->getInstanceNodesCount();
    }
    return iRowCount;

} // rowCount

//------------------------------------------------------------------------------
int CModelInstanceNodes::columnCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return EColumnCount;
}

//------------------------------------------------------------------------------
QVariant CModelInstanceNodes::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    if( !i_modelIdx.isValid() )
    {
        return varData;
    }
    if( m_pOidNode == NULL )
    {
        return varData;
    }

    CInstanceNode* pInstanceNode = m_pOidNode->getInstanceNode(i_modelIdx.row());

    if( pInstanceNode == NULL )
    {
        return varData;
    }

    CValue* pValue = pInstanceNode->getValue();

    switch( i_modelIdx.column() )
    {
        case EColumnInstanceNumber:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = QString::number( pInstanceNode->getInstanceNumber() );
            }
            break;
        }
        case EColumnPDUDataType:
        {
            if( pValue != NULL )
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = pduDataType2Str( pValue->getPDUDataType() );
                }
            }
            break;
        }
        case EColumnValueType:
        {
            if( pValue != NULL )
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = valueType2Str( pValue->getValueType() );
                }
            }
            break;
        }
        case EColumnValueUpdateTime:
        {
            if( pValue != NULL )
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = pInstanceNode->getValueUpdateTime().toString("yyyy-MM-dd hh:mm:ss:zzz" );
                }
            }
            break;
        }
        case EColumnValue:
        {
            if( pValue != NULL )
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = pValue->getValueString();
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

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CModelInstanceNodes::headerData(
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
            case EColumnInstanceNumber:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Id";
                }
                break;
            }
            case EColumnPDUDataType:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "PDU Data Type";
                }
                break;
            }
            case EColumnValueType:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Value Type";
                }
                break;
            }
            case EColumnValueUpdateTime:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Update Time";
                }
                break;
            }
            case EColumnValue:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Value";
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
