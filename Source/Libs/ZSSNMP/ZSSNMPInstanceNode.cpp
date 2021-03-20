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
#include "ZSSNMP/Include/ZSSNMPInstanceNode.h"
#include "ZSSNMP/Include/ZSSNMPOidNode.h"
#include "ZSSys/Include/ZSSysAux.h"
#include "ZSSys/Include/ZSSysErrResult.h"
#include "ZSSys/Include/ZSSysException.h"
#include "ZSSys/Include/ZSSysMemLeakDump.h"

using namespace ZS::SNMP;


/*******************************************************************************
class CInstanceNode : public QObject
*******************************************************************************/

/*==============================================================================
protected: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
CInstanceNode::CInstanceNode( TSubId i_uInstanceNumber, CModelOidNodes* i_pModel ) :
//------------------------------------------------------------------------------
    QObject(),
    m_uInstanceNumber(i_uInstanceNumber),
    m_pValue(NULL),
    m_dateTimeValueUpdate(),
    m_pModelOidNodes(i_pModel),
    m_pOidNodeParent(NULL),
    m_iRowIdx(-1)
{
} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CInstanceNode::~CInstanceNode()
//------------------------------------------------------------------------------
{
    try
    {
        delete m_pValue;
    }
    catch(...)
    {
    }
    m_pValue = NULL;
    m_pModelOidNodes = NULL;
    m_pOidNodeParent = NULL;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
TSubId CInstanceNode::getInstanceNumber() const
//------------------------------------------------------------------------------
{
    return m_uInstanceNumber;
}

//------------------------------------------------------------------------------
QString CInstanceNode::getOidNumeric( bool i_bFullPath )
//------------------------------------------------------------------------------
{
    QString strOid;

    if( m_pOidNodeParent != NULL && i_bFullPath )
    {
        strOid = m_pOidNodeParent->getOidNumeric(i_bFullPath);
    }
    strOid += "." + QString::number(m_uInstanceNumber);

    return strOid;

} // getOidNumeric

//------------------------------------------------------------------------------
QString CInstanceNode::getOidTextual( bool i_bFullPath )
//------------------------------------------------------------------------------
{
    QString strOid;

    if( m_pOidNodeParent != NULL && i_bFullPath )
    {
        strOid = m_pOidNodeParent->getOidTextual(i_bFullPath);
    }
    strOid += "." + QString::number(m_uInstanceNumber);

    return strOid;

} // getOidTextual

//------------------------------------------------------------------------------
SOid CInstanceNode::getOidStruct()
//------------------------------------------------------------------------------
{
    SOid soid;

    if( m_pOidNodeParent != NULL )
    {
        soid = m_pOidNodeParent->getOidStruct();
    }
    if( soid.m_iSubIdsCount < soid.m_iSubIdsArrLen )
    {
        soid.m_aruSubIds[soid.m_iSubIdsCount] = m_uInstanceNumber;
        soid.m_iSubIdsCount++;
    }
    return soid;

} // getOidStruct

//------------------------------------------------------------------------------
COid* CInstanceNode::getOidClass()
//------------------------------------------------------------------------------
{
    COid* pOid = NULL;

    if( m_pOidNodeParent != NULL )
    {
        pOid = new COid( getOidNumeric(true) );
    }
    return pOid;

} // getOidClass

//------------------------------------------------------------------------------
void CInstanceNode::setValue( CValue* i_pValue, bool i_bTakeOwnerShip )
//------------------------------------------------------------------------------
{
    m_dateTimeValueUpdate = QDateTime::currentDateTime();

    if( m_pValue != NULL )
    {
        delete m_pValue;
        m_pValue = NULL;
    }
    if( i_pValue != NULL )
    {
        if( i_bTakeOwnerShip )
        {
            m_pValue = i_pValue;
        }
        else
        {
            m_pValue = new CValue(*i_pValue);
        }
    }
    emit nodeChanged(this);

} // setValue

//------------------------------------------------------------------------------
CValue* CInstanceNode::getValue()
//------------------------------------------------------------------------------
{
    return m_pValue;
}

//------------------------------------------------------------------------------
QDateTime CInstanceNode::getValueUpdateTime() const
//------------------------------------------------------------------------------
{
    return m_dateTimeValueUpdate;
}

//------------------------------------------------------------------------------
void CInstanceNode::setParentNode( COidNode* i_pOidNodeParent )
//------------------------------------------------------------------------------
{
    m_pOidNodeParent = i_pOidNodeParent;
}

//------------------------------------------------------------------------------
COidNode* CInstanceNode::parentNode()
//------------------------------------------------------------------------------
{
    return m_pOidNodeParent;
}

//------------------------------------------------------------------------------
QString CInstanceNode::getParentNodeOidNumeric( bool i_bFullPath ) const
//------------------------------------------------------------------------------
{
    QString strParentNodeKey;

    if( m_pOidNodeParent != NULL )
    {
        strParentNodeKey = m_pOidNodeParent->getOidNumeric(i_bFullPath);
    }
    return strParentNodeKey;
}

//------------------------------------------------------------------------------
QString CInstanceNode::getParentNodeOidTextual( bool i_bFullPath ) const
//------------------------------------------------------------------------------
{
    QString strParentNodeKey;

    if( m_pOidNodeParent != NULL )
    {
        strParentNodeKey = m_pOidNodeParent->getOidTextual(i_bFullPath);
    }
    return strParentNodeKey;
}

//------------------------------------------------------------------------------
void CInstanceNode::setRowIdx( int i_iRowIdx )
//------------------------------------------------------------------------------
{
    m_iRowIdx = i_iRowIdx;
}

//------------------------------------------------------------------------------
int CInstanceNode::getRowIdx() const
//------------------------------------------------------------------------------
{
    return m_iRowIdx;
}
