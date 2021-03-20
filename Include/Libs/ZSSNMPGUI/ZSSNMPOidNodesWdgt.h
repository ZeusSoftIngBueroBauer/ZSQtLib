/*******************************************************************************

Copyright 2004 - 2009 by    ZeusSoft, Ing. Buero Bauer
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

$Date: 22.01.10 22:11 $

$Revision: 3 $

$Author: Christian Bauer $

$History: ZSSNMPOidNodesWdgt.h $
 * 
 * *****************  Version 3  *****************
 * User: Christian Bauer Date: 22.01.10   Time: 22:11
 * Updated in $/ZSSNMP/Include

*******************************************************************************/

#ifndef ZSSNMPOidNodesWdgt_h
#define ZSSNMPOidNodesWdgt_h

#ifdef NO_GUI
#error Module may not be used in this configuration.
#endif

#include <QtGui/qwidget.h>
#include "ZSSNMP/Include/ZSSNMPDllMain.h"

class QModelIndex;
class QTreeView;
class QVBoxLayout;

namespace ZS
{
namespace SNMP
{
class COidNode;
class CModelOidNodes;

//******************************************************************************
class ZSSNMPDLL_API CWdgtOidNodes : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtOidNodes( CModelOidNodes* i_pModel, QWidget* i_pWdgtParent = NULL );
    virtual ~CWdgtOidNodes();
signals:
    void nodeSelected( COidNode* i_pOidNode );
protected: // overridables of base class QWidget
    virtual void showEvent( QShowEvent* i_pEv );
protected slots: // connected to the signals of the tree view
    void onTreeViewExpanded( const QModelIndex& i_modelIdx );
    void onTreeViewClicked( const QModelIndex& i_modelIdx );
    void onTreeViewSelectionModelCurrentChanged( const QModelIndex& i_modelIdxCurr, const QModelIndex& i_modelIdxPrev );
protected: // instance members
    CModelOidNodes* m_pModelOidNodes;
    QVBoxLayout*    m_pLytMain;
    QTreeView*      m_pTreeView;

}; // class CWdgtOidNodes

} // namespace SNMP

} // namespace ZS

#endif // #ifndef ZSSNMPOidNodesWdgt_h
