/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

*******************************************************************************/

#ifndef ZSSysGUI_RequestExecTreeModel_h
#define ZSSysGUI_RequestExecTreeModel_h

#include <QtCore/qabstractitemmodel.h>

#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysRequest.h"

class QMutex;
class QTimer;

namespace ZS
{
namespace System
{
class CRequest;
class CRequestExecTree;

namespace GUI
{
//******************************************************************************
struct ZSSYSGUIDLL_API SRequestExecTreeModelNode
//******************************************************************************
{
public: // ctors and dtor
    SRequestExecTreeModelNode();
    SRequestExecTreeModelNode( CRequestExecTree* i_pReqExecTree );
    SRequestExecTreeModelNode( SRequestDscr* i_pReqDscr );
    ~SRequestExecTreeModelNode();
public: // struct methods
    double getDurationInSec() const;
    QString getDurationAsStrInBestUnit() const;
public: // struct members
    CRequestExecTree*                 m_pReqExecTree; // used by the root entry
    SRequestExecTreeModelNode*        m_pParentNode;
    QList<SRequestExecTreeModelNode*> m_arpChildNodes;
    // Data copied from request item (to keep content of request item after request item has been deleted)
    SRequestDscr                      m_reqDscr;

}; // struct SRequestExecTreeModelNode

//******************************************************************************
class ZSSYSGUIDLL_API CModelRequestExecTree : public QAbstractItemModel
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    enum EColumn {
        EColumnRequestName           =  0,
        EColumnRequest               =  1,
        EColumnId                    =  2,
        EColumnParentId              =  3,
        EColumnCreaterObjName        =  4,
        EColumnCreatorNameSpace =  5,
        EColumnCreatorClassName      =  6,
        EColumnCreatorThreadName     =  7,
        EColumnProgressInPerCent     =  8,
        EColumnErrResultInfo         =  9,
        EColumnIsBlocking            = 10,
        EColumnTimeout               = 11,
        EColumnTimeStart             = 12,
        EColumnTimeEnd               = 13,
        EColumnTimeDuration          = 14,
        EColumnObjState              = 15,
        EColumnDescription           = 16,
        EColumnToolTip               = 17,
        EColumnCount
    };
public: // ctors and dtor
    CModelRequestExecTree( CRequestExecTree* i_pReqExecTree, QObject* i_pObjParent = nullptr );
    virtual ~CModelRequestExecTree();
public: // instance methods
    void clear();
    void removeEntry( const QModelIndex& i_modelIdx );
protected slots:
    void onRequestAdded( ZS::System::SRequestDscr i_reqDscr );
    void onRequestRemoved( ZS::System::SRequestDscr i_reqDscr );
    void onRequestChanged( ZS::System::SRequestDscr i_reqDscr );
protected slots:
    void onTmrRefreshTimeout();
protected: // instance methods
    void addNode( SRequestExecTreeModelNode* i_pNodeParent, ZS::System::SRequestDscr* i_pReqDscr );
    void clearNode( SRequestExecTreeModelNode* i_pNode );
    void updateNode( SRequestExecTreeModelNode* i_pNode );
public: // must overridables of base class QAbstractItemModel
    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const;
    virtual QModelIndex index( int i_iRow, int i_iCol, const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QModelIndex parent( const QModelIndex& i_modelIdx ) const;
public: // overridables of base class QAbstractItemModel
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const;
protected: // instance members
    CRequestExecTree*                        m_pReqExecTree;
    SRequestExecTreeModelNode*               m_pRootEntry;
    QHash<qint64,SRequestExecTreeModelNode*> m_hshRequests;
    QTimer*                                  m_pTmrRefresh;

}; // class CModelRequestExecTree

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_RequestExecTreeModel_h
