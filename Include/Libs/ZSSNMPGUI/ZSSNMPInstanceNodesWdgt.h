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

$Revision: 4 $

$Author: Christian Bauer $

$History: ZSSNMPInstanceNodesWdgt.h $
 * 
 * *****************  Version 4  *****************
 * User: Christian Bauer Date: 22.01.10   Time: 22:11
 * Updated in $/ZSSNMP/Include

*******************************************************************************/

#ifndef ZSSNMPInstanceNodesWdgt_h
#define ZSSNMPInstanceNodesWdgt_h

#ifdef NO_GUI
#error Module may not be used in this configuration.
#endif

#include <QtGui/qwidget.h>
#include "ZSSNMP/Include/ZSSNMPDllMain.h"

class QModelIndex;
class QTableView;
class QVBoxLayout;

namespace ZS
{
namespace SNMP
{
class COidNode;
class CInstanceNode;
class CModelInstanceNodes;

//******************************************************************************
class ZSSNMPDLL_API CWdgtInstanceNodes : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtInstanceNodes( CModelInstanceNodes* i_pModel, QWidget* i_pWdgtParent = NULL );
    virtual ~CWdgtInstanceNodes();
signals:
    void nodeSelected( CInstanceNode* i_pInstanceNode );
protected slots: // connected to the signals of the table view
    void onTableViewClicked( const QModelIndex& i_modelIdx );
    void onTableViewSelectionModelCurrentChanged( const QModelIndex& i_modelIdxCurr, const QModelIndex& i_modelIdxPrev );
protected slots: // connected to the signals of the model
    void onModelDataChanged( const QModelIndex& i_modelIdxTopLeft, const QModelIndex& i_modelIdxBottomRight );
    void onModelRowsInserted( const QModelIndex& i_modelIdxParent, int i_iRowStart, int i_iRowEnd );
    void onModelRowsRemoved( const QModelIndex& i_modelIdxParent, int i_iRowStart, int i_iRowEnd );
protected: // instance members
    CModelInstanceNodes* m_pModelInstanceNodes;
    QVBoxLayout*         m_pLytMain;
    QTableView*          m_pTableView;

}; // class CWdgtInstanceNodes

} // namespace SNMP

} // namespace ZS

#endif // #ifndef ZSSNMPInstanceNodesWdgt_h
