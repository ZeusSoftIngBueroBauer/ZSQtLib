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

#ifndef ZSSNMPInstanceNode_h
#define ZSSNMPInstanceNode_h

#include <QtCore/qobject.h>
#include <QtCore/qdatetime.h>
#include "ZSSNMP/Include/ZSSNMPDllMain.h"
#include "ZSSNMP/Include/ZSSNMPOid.h"

namespace ZS
{
namespace SNMP
{
class COidNode;
class CModelOidNodes;

//******************************************************************************
class ZSSNMPDLL_API CInstanceNode : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CInstanceNode( TSubId i_uInstanceNumber, CModelOidNodes* i_pModel );
    virtual ~CInstanceNode();
signals:
    void nodeChanged( CInstanceNode* i_pNode );
public: // instance methods
    TSubId getInstanceNumber() const;
    QString getOidNumeric( bool i_bFullPath = false );
    QString getOidTextual( bool i_bFullPath = false );
    SOid getOidStruct(); // always the full path
    COid* getOidClass();
    void setValue( CValue* i_pValue, bool i_bTakeOwnerShip = true );
    CValue* getValue();
    QDateTime getValueUpdateTime() const;
    void setParentNode( COidNode* i_pOidNode );
    COidNode* parentNode();
    QString getParentNodeOidNumeric( bool i_bFullPath = false ) const;
    QString getParentNodeOidTextual( bool i_bFullPath = false ) const;
    void setRowIdx( int i_iRowIdx );
    int getRowIdx() const;
protected: // instance members
    TSubId          m_uInstanceNumber;
    CValue*         m_pValue;
    QDateTime       m_dateTimeValueUpdate;
    CModelOidNodes* m_pModelOidNodes;
    COidNode*       m_pOidNodeParent;
    int             m_iRowIdx;

}; // class CInstanceNode

} // namespace SNMP

} // namespace ZS

#endif // #ifndef ZSSNMPKeyInstanceNode_h
