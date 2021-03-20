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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qstringlist.h>
#include "ZSSNMP/Include/ZSSNMPOidNode.h"
#include "ZSSNMP/Include/ZSSNMPInstanceNode.h"
#include "ZSSys/Include/ZSSysAux.h"
#include "ZSSys/Include/ZSSysErrResult.h"
#include "ZSSys/Include/ZSSysException.h"
#include "ZSSys/Include/ZSSysMemLeakDump.h"

using namespace ZS::SNMP;


/*******************************************************************************
class COidNode : public QObject
*******************************************************************************/

/*==============================================================================
protected: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
COidNode::COidNode( TSubId i_uSubId, CModelOidNodes* i_pModel ) :
//------------------------------------------------------------------------------
    QObject(),
    m_uSubId(i_uSubId),
    m_strSubId(QString::number(i_uSubId)),
    m_pOid(NULL),
    m_pModelOidNodes(i_pModel),
    m_pOidNodeParent(NULL),
    m_iRowIdx(-1),
    m_modelIdx(),
    m_uChildOidNodesCount(0),
    m_uChildOidNodesArrLen(0),
    m_arpChildOidNodes(NULL),
    m_uInstanceNodesCount(0),
    m_uInstanceNodesArrLen(0),
    m_arpInstanceNodes(NULL)
{
} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
COidNode::~COidNode()
//------------------------------------------------------------------------------
{
    m_pModelOidNodes = NULL;
    m_pOidNodeParent = NULL;

    try
    {
        delete m_pOid;
    }
    catch(...)
    {
    }
    m_pOid = NULL;

    try
    {
        delete [] m_arpChildOidNodes;
    }
    catch(...)
    {
    }
    m_arpChildOidNodes = NULL;
    m_uChildOidNodesCount = 0;
    m_uChildOidNodesArrLen = 0;

    try
    {
        delete [] m_arpInstanceNodes;
    }
    catch(...)
    {
    }
    m_arpInstanceNodes = NULL;
    m_uInstanceNodesCount = 0;
    m_uInstanceNodesArrLen = 0;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void COidNode::setSubId( TSubId i_uSubId )
//------------------------------------------------------------------------------
{
    if( m_uSubId != i_uSubId )
    {
        m_uSubId = i_uSubId;
        #ifdef _WINDOWS
        #pragma message(__TODO__"check if the id could be converted into textual form")
        #endif
        m_strSubId = "";
        emit nodeChanged(this);
    }

} // setSubId

//------------------------------------------------------------------------------
void COidNode::setSubId( const QString& i_strSubId )
//------------------------------------------------------------------------------
{
    if( m_strSubId != i_strSubId )
    {
        #ifdef _WINDOWS
        #pragma message(__TODO__"extract last sub key and detect whether type of key is numeric or textual")
        #endif
        m_strSubId = i_strSubId;
        //m_strKeyTextual = i_strKey; only if it could be converted
        emit nodeChanged(this);
    }

} // setSubId

//------------------------------------------------------------------------------
TSubId COidNode::getSubId() const
//------------------------------------------------------------------------------
{
    return m_uSubId;
}

//------------------------------------------------------------------------------
QString COidNode::getOidNumeric( bool i_bFullPath )
//------------------------------------------------------------------------------
{
    QString strOid = QString::number(m_uSubId);

    if( m_pOid == NULL && i_bFullPath )
    {
        COidNode* pOidNodeParent = m_pOidNodeParent;

        if( !strOid.startsWith(".") )
        {
            strOid.insert(0,".");
        }
        while( pOidNodeParent != NULL )
        {
            strOid.insert(0,pOidNodeParent->getOidNumeric(false));

            if( !strOid.startsWith(".") )
            {
                strOid.insert(0,".");
            }
            pOidNodeParent = pOidNodeParent->m_pOidNodeParent;
        }
        m_pOid = new COid(strOid);
    }
    return strOid;

} // getOidNumeric

//------------------------------------------------------------------------------
QString COidNode::getOidTextual( bool i_bFullPath )
//------------------------------------------------------------------------------
{
    QString strOid = getOidNumeric(true);

    // The oid instance will be created on calling "getOidNumeric" with "fullPath" set to true.
    strOid = m_pOid->toString(EOidFormatFull);

    if( !i_bFullPath )
    {
        QStringList strListSubIds = strOid.split(".",QString::SkipEmptyParts);

        if( strListSubIds.count() <= 0 )
        {
            strOid = "";
        }
        else
        {
            strOid = strListSubIds[strListSubIds.count()-1];
        }
    }
    return strOid;

} // getOidTextual

//------------------------------------------------------------------------------
SOid COidNode::getOidStruct()
//------------------------------------------------------------------------------
{
    if( m_pOid == NULL )
    {
        // The oid instance will be created on calling "getOidNumeric" with "fullPath" set to true.
        getOidNumeric(true);
    }
    return m_pOid->toOidStruct();

} // getOidStruct

//------------------------------------------------------------------------------
COid* COidNode::getOidClass()
//------------------------------------------------------------------------------
{
    if( m_pOid == NULL )
    {
        // The oid instance will be created on calling "getOidNumeric" with "fullPath" set to true.
        getOidNumeric(true);
    }
    return m_pOid;

} // getOidClass

//------------------------------------------------------------------------------
void COidNode::setParentNode( COidNode* i_pOidNodeParent )
//------------------------------------------------------------------------------
{
    m_pOidNodeParent = i_pOidNodeParent;
}

//------------------------------------------------------------------------------
COidNode* COidNode::parentNode()
//------------------------------------------------------------------------------
{
    return m_pOidNodeParent;
}

//------------------------------------------------------------------------------
QString COidNode::getParentOidNumeric() const
//------------------------------------------------------------------------------
{
    QString strParentOid;

    if( m_pOidNodeParent != NULL )
    {
        strParentOid = m_pOidNodeParent->getOidNumeric();
    }
    return strParentOid;
}

//------------------------------------------------------------------------------
QString COidNode::getParentOidTextual() const
//------------------------------------------------------------------------------
{
    QString strParentOid;

    if( m_pOidNodeParent != NULL )
    {
        strParentOid = m_pOidNodeParent->getOidTextual();
    }
    return strParentOid;
}

//------------------------------------------------------------------------------
void COidNode::setRowIdx( int i_iRowIdx )
//------------------------------------------------------------------------------
{
    m_iRowIdx = i_iRowIdx;
}

//------------------------------------------------------------------------------
int COidNode::getRowIdx() const
//------------------------------------------------------------------------------
{
    return m_iRowIdx;
}

//------------------------------------------------------------------------------
void COidNode::setModelIdx( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    m_modelIdx = i_modelIdx;
}

//------------------------------------------------------------------------------
QModelIndex COidNode::getModelIdx() const
//------------------------------------------------------------------------------
{
    return m_modelIdx;
}

//------------------------------------------------------------------------------
unsigned int COidNode::getChildOidNodesCount() const
//------------------------------------------------------------------------------
{
    return m_uChildOidNodesCount;
}

//------------------------------------------------------------------------------
int COidNode::addChildOidNode( COidNode* i_pOidNodeChild )
//------------------------------------------------------------------------------
{
    if( i_pOidNodeChild == NULL )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultArgOutOfRange);
    }
    if( m_pModelOidNodes == NULL )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalProgramError);
    }
    if( i_pOidNodeChild->parentNode() != NULL )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultObjAlreadyInList);
    }

    int iRowIdxChild = -1;

    // Resize or newly allocate the array:
    if( m_arpChildOidNodes == NULL )
    {
        m_uChildOidNodesArrLen = ZS::_NewArrLen(m_uChildOidNodesArrLen,2,10);
        m_uChildOidNodesCount = 0;
        m_arpChildOidNodes = new COidNode*[m_uChildOidNodesArrLen];
        memset( m_arpChildOidNodes, 0x00, m_uChildOidNodesArrLen*sizeof(COidNode*) );
    }
    else if( m_uChildOidNodesCount == m_uChildOidNodesArrLen )
    {
        COidNode**   arpChilds = m_arpChildOidNodes;
        unsigned int uChildsCount = m_uChildOidNodesCount;

        m_uChildOidNodesArrLen = ZS::_NewArrLen(m_uChildOidNodesArrLen,2,10);
        m_arpChildOidNodes = new COidNode*[m_uChildOidNodesArrLen];
        memset( m_arpChildOidNodes, 0x00, m_uChildOidNodesArrLen*sizeof(COidNode*) );

        for( iRowIdxChild = 0; iRowIdxChild < static_cast<int>(uChildsCount); iRowIdxChild++ )
        {
            m_arpChildOidNodes[iRowIdxChild] = arpChilds[iRowIdxChild];
        }
        delete [] arpChilds;
        arpChilds = NULL;
    }

    // Add the entry to the list:
    iRowIdxChild = static_cast<int>(m_uChildOidNodesCount);
    m_arpChildOidNodes[m_uChildOidNodesCount] = i_pOidNodeChild;
    m_uChildOidNodesCount++;

    i_pOidNodeChild->setParentNode(this);
    i_pOidNodeChild->setRowIdx(iRowIdxChild);

    emit childOidNodeAdded(this,i_pOidNodeChild);

    return iRowIdxChild;

} // addChildOidNode

//------------------------------------------------------------------------------
void COidNode::removeChildOidNode( COidNode* i_pOidNodeChild )
//------------------------------------------------------------------------------
{
    if( i_pOidNodeChild == NULL )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultArgOutOfRange);
    }
    if( m_pModelOidNodes == NULL )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalProgramError);
    }
    if( i_pOidNodeChild->parentNode() == NULL )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultObjNotInList);
    }

    removeChildOidNode( i_pOidNodeChild->getRowIdx() );

} // removeChildOidNode

//------------------------------------------------------------------------------
void COidNode::removeChildOidNode( int i_iRowIdx )
//------------------------------------------------------------------------------
{
    if( m_uChildOidNodesCount == 0 )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalProgramError);
    }
    if( i_iRowIdx < 0 || i_iRowIdx >= static_cast<int>(m_uChildOidNodesCount) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultArgOutOfRange);
    }
    if( m_pModelOidNodes == NULL )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalProgramError);
    }

    emit removingChildOidNode(this,i_iRowIdx);

    m_arpChildOidNodes[i_iRowIdx] = NULL;

    int iRowIdxChild;

    if( m_uChildOidNodesCount > 1 )
    {
        for( iRowIdxChild = i_iRowIdx; iRowIdxChild < static_cast<int>(m_uChildOidNodesCount)-1; iRowIdxChild++ )
        {
            m_arpChildOidNodes[iRowIdxChild] = m_arpChildOidNodes[iRowIdxChild+1];
        }
        m_arpChildOidNodes[m_uChildOidNodesCount-1] = NULL;
    }
    m_uChildOidNodesCount--;

} // removeChildOidNode

//------------------------------------------------------------------------------
COidNode* COidNode::getChildOidNode( int i_iRowIdx ) const
//------------------------------------------------------------------------------
{
    if( i_iRowIdx < 0 || i_iRowIdx >= static_cast<int>(m_uChildOidNodesCount) )
    {
        return NULL;
    }
    return m_arpChildOidNodes[i_iRowIdx];

} // getChildOidNode

//------------------------------------------------------------------------------
COidNode* COidNode::findChildOidNode( TSubId i_uSubId ) const
//------------------------------------------------------------------------------
{
    COidNode* pOidNodeChild = NULL;
    int       iRowIdxChild;

    for( iRowIdxChild = 0; iRowIdxChild < static_cast<int>(m_uChildOidNodesCount); iRowIdxChild++ )
    {
        pOidNodeChild = m_arpChildOidNodes[iRowIdxChild];

        if( pOidNodeChild != NULL )
        {
            if( pOidNodeChild->getSubId() == i_uSubId )
            {
                break;
            }
        }
    }
    if( iRowIdxChild >= static_cast<int>(m_uChildOidNodesCount) )
    {
        pOidNodeChild = NULL;
    }
    return pOidNodeChild;

} // findChildOidNode

//------------------------------------------------------------------------------
COidNode* COidNode::findChildOidNode( const QString& i_strSubId ) const
//------------------------------------------------------------------------------
{
    #ifdef _WINDOWS
    #pragma message(__TODO__"extract last sub key and detect whether type of key is numeric or textual")
    #endif

    COidNode* pOidNodeChild = NULL;
    int       iRowIdxChild;

    for( iRowIdxChild = 0; iRowIdxChild < static_cast<int>(m_uChildOidNodesCount); iRowIdxChild++ )
    {
        pOidNodeChild = m_arpChildOidNodes[iRowIdxChild];

        if( pOidNodeChild != NULL )
        {
            if( pOidNodeChild->getOidNumeric() == i_strSubId )
            {
                break;
            }
        }
    }
    if( iRowIdxChild >= static_cast<int>(m_uChildOidNodesCount) )
    {
        pOidNodeChild = NULL;
    }
    return pOidNodeChild;

} // findChildOidNode

//------------------------------------------------------------------------------
unsigned int COidNode::getInstanceNodesCount() const
//------------------------------------------------------------------------------
{
    return m_uInstanceNodesCount;
}

//------------------------------------------------------------------------------
int COidNode::addInstanceNode( CInstanceNode* i_pInstanceNode )
//------------------------------------------------------------------------------
{
    if( i_pInstanceNode == NULL )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultArgOutOfRange);
    }
    if( m_pModelOidNodes == NULL )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalProgramError);
    }
    if( i_pInstanceNode->parentNode() != NULL )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultObjAlreadyInList);
    }

    int iRowIdxChild = -1;

    // Resize or newly allocate the array:
    if( m_arpInstanceNodes == NULL )
    {
        m_uInstanceNodesArrLen = ZS::_NewArrLen(m_uInstanceNodesArrLen,2,10);
        m_uInstanceNodesCount = 0;
        m_arpInstanceNodes = new CInstanceNode*[m_uInstanceNodesArrLen];
        memset( m_arpInstanceNodes, 0x00, m_uInstanceNodesArrLen*sizeof(CInstanceNode*) );
    }
    else if( m_uInstanceNodesCount == m_uInstanceNodesArrLen )
    {
        CInstanceNode** arpInstances    = m_arpInstanceNodes;
        unsigned int       uInstancesCount = m_uInstanceNodesCount;

        m_uInstanceNodesArrLen = ZS::_NewArrLen(m_uInstanceNodesArrLen,2,10);
        m_arpInstanceNodes = new CInstanceNode*[m_uInstanceNodesArrLen];
        memset( m_arpInstanceNodes, 0x00, m_uInstanceNodesArrLen*sizeof(CInstanceNode*) );

        for( iRowIdxChild = 0; iRowIdxChild < static_cast<int>(uInstancesCount); iRowIdxChild++ )
        {
            m_arpInstanceNodes[iRowIdxChild] = arpInstances[iRowIdxChild];
        }
        delete [] arpInstances;
        arpInstances = NULL;
    }

    // Add the entry to the list:
    iRowIdxChild = static_cast<int>(m_uInstanceNodesCount);
    m_arpInstanceNodes[m_uInstanceNodesCount] = i_pInstanceNode;
    m_uInstanceNodesCount++;

    i_pInstanceNode->setParentNode(this);
    i_pInstanceNode->setRowIdx(iRowIdxChild);

    emit instanceNodeAdded(this,i_pInstanceNode);

    return iRowIdxChild;

} // addInstanceNode

//------------------------------------------------------------------------------
void COidNode::removeInstanceNode( CInstanceNode* i_pInstanceNode )
//------------------------------------------------------------------------------
{
    if( i_pInstanceNode == NULL )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultArgOutOfRange);
    }
    if( m_pModelOidNodes == NULL )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalProgramError);
    }
    if( i_pInstanceNode->parentNode() == NULL )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultObjNotInList);
    }

    removeInstanceNode( i_pInstanceNode->getRowIdx() );

} // removeInstanceNode

//------------------------------------------------------------------------------
void COidNode::removeInstanceNode( int i_iRowIdx )
//------------------------------------------------------------------------------
{
    if( m_uInstanceNodesCount == 0 )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalProgramError);
    }
    if( i_iRowIdx < 0 || i_iRowIdx >= static_cast<int>(m_uInstanceNodesCount) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultArgOutOfRange);
    }
    if( m_pModelOidNodes == NULL )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalProgramError);
    }

    if( m_arpInstanceNodes[i_iRowIdx] != NULL )
    {
        emit removingInstanceNode(this,m_arpInstanceNodes[i_iRowIdx]);
    }
    else
    {
        emit removingInstanceNode(this,i_iRowIdx);
    }

    m_arpInstanceNodes[i_iRowIdx] = NULL;

    int iRowIdxChild;

    if( m_uInstanceNodesCount > 1 )
    {
        for( iRowIdxChild = i_iRowIdx; iRowIdxChild < static_cast<int>(m_uInstanceNodesCount)-1; iRowIdxChild++ )
        {
            m_arpInstanceNodes[iRowIdxChild] = m_arpInstanceNodes[iRowIdxChild+1];
        }
        m_arpInstanceNodes[m_uInstanceNodesCount-1] = NULL;
    }
    m_uInstanceNodesCount--;

} // removeInstanceNode

//------------------------------------------------------------------------------
CInstanceNode* COidNode::getInstanceNode( int i_iRowIdx ) const
//------------------------------------------------------------------------------
{
    if( i_iRowIdx < 0 || i_iRowIdx >= static_cast<int>(m_uInstanceNodesCount) )
    {
        return NULL;
    }
    return m_arpInstanceNodes[i_iRowIdx];

} // getInstanceNode

//------------------------------------------------------------------------------
CInstanceNode* COidNode::findInstanceNode( TSubId i_uInstanceNumber ) const
//------------------------------------------------------------------------------
{
    CInstanceNode* pInstanceNode = NULL;
    int               iRowIdxChild;

    for( iRowIdxChild = 0; iRowIdxChild < static_cast<int>(m_uInstanceNodesCount); iRowIdxChild++ )
    {
        pInstanceNode = m_arpInstanceNodes[iRowIdxChild];

        if( pInstanceNode != NULL )
        {
            if( pInstanceNode->getInstanceNumber() == i_uInstanceNumber )
            {
                break;
            }
        }
    }
    if( iRowIdxChild >= static_cast<int>(m_uInstanceNodesCount) )
    {
        pInstanceNode = NULL;
    }
    return pInstanceNode;

} // findInstanceNode
