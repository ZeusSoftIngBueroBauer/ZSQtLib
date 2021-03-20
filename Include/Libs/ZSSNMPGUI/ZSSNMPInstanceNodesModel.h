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

#ifndef ZSSNMPInstanceNodesModel_h
#define ZSSNMPInstanceNodesModel_h

#include <QtCore/qabstractitemmodel.h>
#include "ZSSNMP/Include/ZSSNMPDllMain.h"

namespace ZS
{
namespace SNMP
{
class CExplorer;
class COidNode;
class CInstanceNode;

//******************************************************************************
class ZSSNMPDLL_API CModelInstanceNodes : public QAbstractTableModel
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    typedef enum
    {
        EColumnInstanceNumber  = 0,
        EColumnPDUDataType     = 1,
        EColumnValueType       = 2,
        EColumnValueUpdateTime = 3,
        EColumnValue           = 4,
        EColumnCount
    }   EColumn;
public: // ctors and dtor
    CModelInstanceNodes( CExplorer* i_pExplorer );
    virtual ~CModelInstanceNodes();
public: // instance methods
    void clear();
    void setOidNode( COidNode* i_pOidNode );
    CInstanceNode* getNode( int i_iRow );
protected slots:
    void onNodeAdded( COidNode* i_pOidNode, CInstanceNode* i_pInstanceNode );
    void onNodeRemoving( COidNode* i_pOidNode, CInstanceNode* i_pInstanceNode );
    void onNodeRemoving( COidNode* i_pOidNode, int i_iRowIdx );
    void onNodeChanged( CInstanceNode* i_pInstanceNode );
public: // must overridables of base class QAbstractItemModel
    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const;
public: // overridables of base class QAbstractItemModel
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const;
private: // copy ctor not allowed
    CModelInstanceNodes( const CModelInstanceNodes& );
private: // assignment operator not allowed
    void operator = ( const CModelInstanceNodes& );
protected: // instance members
    CExplorer* m_pExplorer;
    COidNode*  m_pOidNode;

}; // class CModelInstanceNodes

} // namespace SNMP

} // namespace ZS

#endif // #ifndef ZSSNMPInstanceNodesModel_h
