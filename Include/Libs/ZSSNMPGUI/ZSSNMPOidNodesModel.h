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

#ifndef ZSSNMPOidNodesModel_h
#define ZSSNMPOidNodesModel_h

#include <QtCore/qabstractitemmodel.h>
#include "ZSSNMP/Include/ZSSNMPDllMain.h"
#include "ZSSNMP/Include/ZSSNMPOid.h"

namespace ZS
{
namespace SNMP
{
class CExplorer;
class COidNode;
class CInstanceNode;

//******************************************************************************
class ZSSNMPDLL_API CModelOidNodes : public QAbstractItemModel
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    typedef enum
    {
        EColumnName = 0,
        EColumnCount
    }   EColumn;
public: // ctors and dtor
    CModelOidNodes( CExplorer* m_pExplorer );
    virtual ~CModelOidNodes();
public: // instance methods
    void clear( COidNode* i_pOidNode = NULL );
    COidNode* getRootNode();
    COidNode* findOidNode( COid& i_oid );
    COidNode* findOidNode( const QString& i_strOid );
    COidNode* findOidNode( COidNode* i_pOidNodeParent, TSubId i_uSubId );
    COidNode* addOidNode( COidNode* i_pOidNodeParent, TSubId i_uSubId );
    CInstanceNode* findInstanceNode( const QString& i_strOid );
    CInstanceNode* findInstanceNode( COidNode* i_pOidNodeParent, TSubId i_uSubId );
    CInstanceNode* addInstanceNode( COidNode* i_pOidNodeParent, TSubId i_uSubId );
public: // must overridables of base class QAbstractItemModel
    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const;
    virtual QModelIndex index( int i_iRow, int i_iCol, const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QModelIndex parent( const QModelIndex& i_modelIdx ) const;
public: // overridables of base class QAbstractItemModel
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const;
private: // copy ctor not allowed
    CModelOidNodes( const CModelOidNodes& );
private: // assignment operator not allowed
    void operator = ( const CModelOidNodes& );
protected: // instance members
    CExplorer* m_pExplorer;
    COidNode*  m_pOidNodeRoot;

}; // class CModelOidNodes

} // namespace SNMP

} // namespace ZS

#endif // #ifndef ZSSNMPOidNodesModel_h
