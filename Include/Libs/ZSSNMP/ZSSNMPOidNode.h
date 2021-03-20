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

#ifndef ZSSNMPOidNode_h
#define ZSSNMPOidNode_h

#include <QtCore/qobject.h>
#include <QtCore/qabstractitemmodel.h>
#include "ZSSNMP/Include/ZSSNMPDllMain.h"
#include "ZSSNMP/Include/ZSSNMPOid.h"

namespace ZS
{
namespace SNMP
{
class CModelOidNodes;
class CInstanceNode;

//******************************************************************************
class ZSSNMPDLL_API COidNode : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    COidNode( TSubId i_uSubId, CModelOidNodes* i_pModel );
    virtual ~COidNode();
signals:
    void childOidNodeAdded( COidNode* i_pOidNode, COidNode* i_pOidNodeChild );
    void removingChildOidNode( COidNode* i_pOidNode, int i_iRowIdx );
    void instanceNodeAdded( COidNode* i_pOidNode, CInstanceNode* i_pInstanceNode );
    void removingInstanceNode( COidNode* i_pOidNode, CInstanceNode* i_pInstanceNode );
    void removingInstanceNode( COidNode* i_pOidNode, int i_iRowIdx );
    void nodeChanged( COidNode* i_pOidNode );
public: // instance methods
    void setSubId( TSubId i_uSubId );
    void setSubId( const QString& i_strSubId );
    TSubId getSubId() const;
    QString getOidNumeric( bool i_bFullPath = false );
    QString getOidTextual( bool i_bFullPath = false );
    SOid getOidStruct(); // always the full path
    COid* getOidClass();
    void setParentNode( COidNode* i_pOidNode );
    COidNode* parentNode();
    QString getParentOidNumeric() const;
    QString getParentOidTextual() const;
    void setRowIdx( int i_iRowIdx );
    int getRowIdx() const;
    void setModelIdx( const QModelIndex& i_modelIdx );
    QModelIndex getModelIdx() const;
    unsigned int getChildOidNodesCount() const;
    virtual int addChildOidNode( COidNode* i_pOidNodeChild ); // Does not only add the node as a child but also emits the signal "childAdded" and sets the parent of the newly created node.
    void removeChildOidNode( COidNode* i_pOidNodeChild );
    void removeChildOidNode( int i_iRowIdx );
    COidNode* getChildOidNode( int i_iRowIdx ) const;
    COidNode* findChildOidNode( TSubId i_uSubId ) const;
    COidNode* findChildOidNode( const QString& i_strSubId ) const;
    unsigned int getInstanceNodesCount() const;
    virtual int addInstanceNode( CInstanceNode* i_pInstanceNode ); // Does not only add the node as a child but also emits the signal "childAdded" and sets the parent of the newly created node.
    void removeInstanceNode( CInstanceNode* i_pInstanceNode );
    void removeInstanceNode( int i_iRowIdx );
    CInstanceNode* getInstanceNode( int i_iRowIdx ) const;
    CInstanceNode* findInstanceNode( TSubId i_uInstanceNumber ) const;
protected: // instance members
    TSubId          m_uSubId;
    QString         m_strSubId;
    COid*           m_pOid;
    CModelOidNodes* m_pModelOidNodes;
    COidNode*       m_pOidNodeParent;
    int             m_iRowIdx;
    QModelIndex     m_modelIdx;
    unsigned int    m_uChildOidNodesCount;
    unsigned int    m_uChildOidNodesArrLen;
    COidNode**      m_arpChildOidNodes;
    unsigned int    m_uInstanceNodesCount;
    unsigned int    m_uInstanceNodesArrLen;
    CInstanceNode** m_arpInstanceNodes;

}; // class COidNode

} // namespace SNMP

} // namespace ZS

#endif // #ifndef ZSSNMPOidNode_h
