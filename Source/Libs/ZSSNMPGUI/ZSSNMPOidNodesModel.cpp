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

$Date: 10.01.10 19:19 $

$Revision: 7 $

$Author: Christian Bauer $

$History: ZSSNMPOidNodesModel.cpp $
 * 
 * *****************  Version 7  *****************
 * User: Christian Bauer Date: 10.01.10   Time: 19:19
 * Updated in $/ZSSNMP/Source

*******************************************************************************/

#include <QtCore/qdatetime.h>
#include <QtCore/qfile.h>
#include <QtCore/qtextstream.h>
#include <QtCore/qthread.h>
#include <QtXml/qdom.h>
#include "ZSSNMP/Include/ZSSNMPOidNodesModel.h"
#include "ZSSNMP/Include/ZSSNMPOidNode.h"
#include "ZSSNMP/Include/ZSSNMPExplorer.h"
#include "ZSSNMP/Include/ZSSNMPInstanceNode.h"
#include "ZSSys/Include/ZSSysAux.h"
#include "ZSSys/Include/ZSSysErrLogModel.h"
#include "ZSSys/Include/ZSSysErrResult.h"
#include "ZSSys/Include/ZSSysException.h"
#include "ZSSys/Include/ZSSysMemLeakDump.h"

using namespace ZS::SNMP;


/*******************************************************************************
class CModelOidNodes : public QAbstractItemModel
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelOidNodes::CModelOidNodes( CExplorer* i_pExplorer ) :
//------------------------------------------------------------------------------
    QAbstractItemModel(i_pExplorer),
    m_pExplorer(i_pExplorer),
    m_pOidNodeRoot(NULL)
{
    m_pOidNodeRoot = new COidNode(1,this);
    m_pOidNodeRoot->setRowIdx(0);
    m_pOidNodeRoot->setModelIdx( createIndex(0,0,m_pOidNodeRoot) );

    beginInsertRows(QModelIndex(),0,0);
    endInsertRows();

} // ctor

//------------------------------------------------------------------------------
CModelOidNodes::~CModelOidNodes()
//------------------------------------------------------------------------------
{
    try
    {
        clear(m_pOidNodeRoot);
    }
    catch(...)
    {
    }

    // The root entry has not be deleted by the clear method and must
    // be explicitly destroyed.
    try
    {
        delete m_pOidNodeRoot;
    }
    catch(...)
    {
    }
    m_pOidNodeRoot = NULL;

    m_pExplorer = NULL;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelOidNodes::clear( COidNode* i_pOidNode )
//------------------------------------------------------------------------------
{
    if( m_pOidNodeRoot == NULL && i_pOidNode == NULL )
    {
        return;
    }

    COidNode* pOidNode = i_pOidNode;

    if( pOidNode == NULL )
    {
        pOidNode = m_pOidNodeRoot;
    }

    // The root entry will not be deleted.
    if( pOidNode->getChildOidNodesCount() > 0 )
    {
        COidNode* pOidNodeChild;
        int       iOidNodeChildIdx;

        for( iOidNodeChildIdx = static_cast<int>(pOidNode->getChildOidNodesCount())-1; iOidNodeChildIdx >= 0; iOidNodeChildIdx-- )
        {
            pOidNodeChild = dynamic_cast<COidNode*>(pOidNode->getChildOidNode(iOidNodeChildIdx));

            clear(pOidNodeChild);

            // After the child nodes of the child node have been removed the child node may be removed and deleted.
            beginRemoveRows( pOidNode->getModelIdx(), iOidNodeChildIdx, iOidNodeChildIdx );
            pOidNode->removeChildOidNode(iOidNodeChildIdx);
            delete pOidNodeChild;
            pOidNodeChild = NULL;
            endRemoveRows();
        }
    }

    // After the child nodes of the child node have been removed the instances may be removed and deleted.
    if( pOidNode->getInstanceNodesCount() > 0 )
    {
        CInstanceNode* pInstanceNode;
        int            iInstanceNodeIdx;

        for( iInstanceNodeIdx = static_cast<int>(pOidNode->getInstanceNodesCount())-1; iInstanceNodeIdx >= 0; iInstanceNodeIdx-- )
        {
            pInstanceNode = dynamic_cast<CInstanceNode*>(pOidNode->getInstanceNode(iInstanceNodeIdx));
            pOidNode->removeInstanceNode(iInstanceNodeIdx);
            delete pInstanceNode;
            pInstanceNode = NULL;
        }
    }

    // The node has been cleared (all child nodes have been removed and deleted).
    // The node to be cleared will not be deleted.
    //reset();

} // clear

//------------------------------------------------------------------------------
COidNode* CModelOidNodes::getRootNode()
//------------------------------------------------------------------------------
{
    return m_pOidNodeRoot;
}

//------------------------------------------------------------------------------
COidNode* CModelOidNodes::findOidNode( COid& i_oid )
//------------------------------------------------------------------------------
{
    COidNode* pOidNode = NULL;

    int idxSubId;

    if( i_oid.getSubIdsCount() > 0 )
    {
        for( idxSubId = 0; idxSubId < i_oid.getSubIdsCount(); idxSubId++ )
        {
            pOidNode = findOidNode(pOidNode,i_oid.getSubId(idxSubId));

            if( pOidNode == NULL )
            {
                break;
            }
        }
    }
    return pOidNode;

} // findOidNode

//------------------------------------------------------------------------------
COidNode* CModelOidNodes::findOidNode( const QString& i_strOid )
//------------------------------------------------------------------------------
{
    COidNode* pOidNode = NULL;

    SOid soid(i_strOid);

    int idxSubId;

    if( soid.m_iSubIdsCount > 0 )
    {
        for( idxSubId = 0; idxSubId < soid.m_iSubIdsCount; idxSubId++ )
        {
            pOidNode = findOidNode(pOidNode,soid.m_aruSubIds[idxSubId]);

            if( pOidNode == NULL )
            {
                break;
            }
        }
    }
    return pOidNode;

} // findOidNode

//------------------------------------------------------------------------------
COidNode* CModelOidNodes::findOidNode( COidNode* i_pOidNodeParent, TSubId i_uSubId )
//------------------------------------------------------------------------------
{
    COidNode* pOidNode = NULL;

    if( i_pOidNodeParent == NULL )
    {
        if( m_pOidNodeRoot->getSubId() == i_uSubId )
        {
            pOidNode = m_pOidNodeRoot;
        }
    }
    else
    {
        pOidNode = i_pOidNodeParent->findChildOidNode(i_uSubId);
    }
    return pOidNode;

} // findOidNode

//------------------------------------------------------------------------------
COidNode* CModelOidNodes::addOidNode( COidNode* i_pOidNodeParent, TSubId i_uSubId )
//------------------------------------------------------------------------------
{
    if( i_pOidNodeParent == NULL )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultArgOutOfRange,"pOidNodeParent=NULL");
    }

    COidNode* pOidNode = findOidNode(i_pOidNodeParent,i_uSubId);

    if( pOidNode == NULL )
    {
        int iRowIdx;

        pOidNode = new COidNode(i_uSubId,this);
        iRowIdx = i_pOidNodeParent->addChildOidNode(pOidNode);
        pOidNode->setModelIdx( createIndex(iRowIdx,0,pOidNode) );

        beginInsertRows( i_pOidNodeParent->getModelIdx(), iRowIdx, iRowIdx );
        endInsertRows();
    }
    return pOidNode;

} // addOidNode

//------------------------------------------------------------------------------
CInstanceNode* CModelOidNodes::findInstanceNode( const QString& i_strOid )
//------------------------------------------------------------------------------
{
    COidNode*      pOidNode = NULL;
    CInstanceNode* pInstanceNode = NULL;
    bool           bValidParent = true;

    SOid soid(i_strOid);

    int idxSubId;

    if( soid.m_iSubIdsCount > 1 )
    {
        for( idxSubId = 0; idxSubId < soid.m_iSubIdsCount-1; idxSubId++ )
        {
            pOidNode = findOidNode(pOidNode,soid.m_aruSubIds[idxSubId]);

            if( pOidNode == NULL )
            {
                bValidParent = false;
                break;
            }
        }
    }
    if( bValidParent && soid.m_iSubIdsCount > 0 )
    {
        idxSubId = soid.m_iSubIdsCount-1;
        pInstanceNode = findInstanceNode(pOidNode,soid.m_aruSubIds[idxSubId]);
    }
    return pInstanceNode;

} // findInstanceNode

//------------------------------------------------------------------------------
CInstanceNode* CModelOidNodes::findInstanceNode( COidNode* i_pOidNodeParent, TSubId i_uSubId )
//------------------------------------------------------------------------------
{
    if( i_pOidNodeParent == NULL )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultArgOutOfRange,"pObjNodeParent=NULL");
    }
    return i_pOidNodeParent->findInstanceNode(i_uSubId);

} // findInstanceNode

//------------------------------------------------------------------------------
CInstanceNode* CModelOidNodes::addInstanceNode( COidNode* i_pOidNodeParent, TSubId i_uSubId )
//------------------------------------------------------------------------------
{
    if( i_pOidNodeParent == NULL )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultArgOutOfRange,"pOidNodeParent=NULL");
    }

    CInstanceNode* pInstanceNode = findInstanceNode(i_pOidNodeParent,i_uSubId);

    if( pInstanceNode == NULL )
    {
        int iRowIdx;

        pInstanceNode = new CInstanceNode(i_uSubId,this);
        iRowIdx = i_pOidNodeParent->addInstanceNode(pInstanceNode);
    }
    return pInstanceNode;

} // addInstanceNode

/*==============================================================================
public: // must overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelOidNodes::rowCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    COidNode* pOidNodeParent = NULL;
    int       iRowCount = 0;

    if( !i_modelIdxParent.isValid() )
    {
        iRowCount = 1; // root entry
    }
    else
    {
        pOidNodeParent = static_cast<COidNode*>(i_modelIdxParent.internalPointer());

        if( pOidNodeParent != NULL )
        {
            iRowCount = pOidNodeParent->getChildOidNodesCount();
        }
    }
    return iRowCount;

} // rowCount

//------------------------------------------------------------------------------
int CModelOidNodes::columnCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return EColumnCount;
}

//------------------------------------------------------------------------------
QVariant CModelOidNodes::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    if( !i_modelIdx.isValid() )
    {
        return varData;
    }

    COidNode* pOidNode = static_cast<COidNode*>(i_modelIdx.internalPointer());

    if( pOidNode == NULL )
    {
        return varData;
    }

    switch( i_modelIdx.column() )
    {
        case EColumnName:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                QString strOid = QString::number( pOidNode->getSubId() );

                if( !pOidNode->getOidTextual().isEmpty() )
                {
                    strOid += " (" + pOidNode->getOidTextual() + ")";
                }
                varData = strOid;
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
QModelIndex CModelOidNodes::index( int i_iRow, int i_iCol, const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    QModelIndex modelIdx;

    if( i_iRow >= 0 )
    {
        if( !i_modelIdxParent.isValid() )
        {
            if( i_iRow == 0 )
            {
                modelIdx = createIndex(i_iRow,i_iCol,m_pOidNodeRoot);
            }
        }
        else
        {
            COidNode* pOidNodeParent = static_cast<COidNode*>(i_modelIdxParent.internalPointer());

            if( pOidNodeParent->getChildOidNodesCount() > 0 && i_iRow < static_cast<int>(pOidNodeParent->getChildOidNodesCount()) )
            {
                modelIdx = createIndex(i_iRow,i_iCol,pOidNodeParent->getChildOidNode(i_iRow));
            }
        }
    }
    return modelIdx;

} // index

//------------------------------------------------------------------------------
QModelIndex CModelOidNodes::parent( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QModelIndex modelIdx;

    if( i_modelIdx.isValid() )
    {
        COidNode* pOidNode = static_cast<COidNode*>(i_modelIdx.internalPointer());
        COidNode* pOidNodeParent;

        if( pOidNode == NULL )
        {
            return modelIdx;
        }
        pOidNodeParent = dynamic_cast<COidNode*>(pOidNode->parentNode());
        if( pOidNodeParent != NULL )
        {
            modelIdx = createIndex( pOidNodeParent->getRowIdx(), 0, pOidNodeParent );
        }
    }
    return modelIdx;

} // parent

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CModelOidNodes::headerData(
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
            case EColumnName:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Name";
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
