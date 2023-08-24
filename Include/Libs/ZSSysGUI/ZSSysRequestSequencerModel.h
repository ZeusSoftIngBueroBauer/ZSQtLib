/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSSysGUI_RequestSequencerModel_h
#define ZSSysGUI_RequestSequencerModel_h

#include <QtCore/qabstractitemmodel.h>

#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSSys/ZSSysRequestSequencer.h"

class QMutex;
class QTimer;

namespace ZS
{
namespace System
{
namespace GUI
{
//******************************************************************************
struct ZSSYSGUIDLL_API SRequestSeqEntryModelNode
//******************************************************************************
{
public: // ctors and dtor
    SRequestSeqEntryModelNode();
    ~SRequestSeqEntryModelNode();
public: // struct members
    CRequestSequencer*                m_pReqSeq; // used by the root entry
    SRequestSeqEntryModelNode*        m_pParentNode;
    QList<SRequestSeqEntryModelNode*> m_arpChildNodes;
    SRequestSeqEntry                  m_reqSeqEntry;

}; // struct SRequestSeqEntryModelNode

//******************************************************************************
class ZSSYSGUIDLL_API CModelRequestSequencer : public QAbstractItemModel
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    enum EColumn {
        EColumnRequestName           =  0,
        EColumnRequest               =  1,
        EColumnId                    =  2,
        EColumnParentId              =  3,
        EColumnPredecessorId         =  4,
        EColumnCreaterObjName        =  5,
        EColumnCreatorNameSpace =  6,
        EColumnCreatorClassName      =  7,
        EColumnCreatorThreadName     =  8,
        EColumnThreadName            =  9,
        EColumnObjFctExecute         = 10,
        EColumnFctExecute            = 11,
        EColumnProgressInPerCent     = 12,
        EColumnErrResultInfo         = 13,
        EColumnIsBlocking            = 14,
        EColumnTimeout               = 15,
        EColumnTimeStart             = 16,
        EColumnTimeEnd               = 17,
        EColumnTimeDuration          = 18,
        EColumnObjState              = 19,
        EColumnDescription           = 20,
        EColumnToolTip               = 21,
        EColumnCount
    };
public: // class methods
    static QIcon GetIcon( ERequestEntryType i_entryType );
public: // ctors and dtor
    CModelRequestSequencer( QObject* i_pObjParent = nullptr );
    virtual ~CModelRequestSequencer();
public: // instance methods
    void setSequencer( CRequestSequencer* i_pReqSeq );
public: // instance methods
    void clear();
    void removeEntry( const QModelIndex& i_modelIdx );
protected slots:
    // As the sequencer may not live in the GUI Main Thread the signals will be queued.
    // The request sequence entry may already be deleted if the correspondign slot is executed.
    // This is why hte sequence entry are transferred by value and not by reference.
    void onRequestAdded( qint64 i_iReqId );
    void onRequestRemoved( qint64 i_iReqId );
    void onRequestChanged( qint64 i_iReqIdOld, qint64 i_iReqIdNew );
protected: // struct methods
    double getDurationInSec( SRequestSeqEntryModelNode* i_pNode ) const;
    QString getDurationAsStrInBestUnit( SRequestSeqEntryModelNode* i_pNode ) const;
protected slots:
    void onTmrRefreshTimeout();
protected: // instance methods
    void addNode( SRequestSeqEntryModelNode* i_pNodeParent, SRequestSeqEntry* i_pReqSeqEntry );
    void clearNode( SRequestSeqEntryModelNode* i_pNode );
    void updateNode( SRequestSeqEntryModelNode* i_pNode );
public: // must overridables of base class QAbstractItemModel
    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const;
    virtual QModelIndex index( int i_iRow, int i_iCol, const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QModelIndex parent( const QModelIndex& i_modelIdx ) const;
public: // overridables of base class QAbstractItemModel
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const;
protected: // instance members
    CRequestSequencer*                       m_pReqSeq; // The sequencer may not live in the GUI Main Thread.
    SRequestSeqEntryModelNode*               m_pRootEntry;
    QHash<qint64,SRequestSeqEntryModelNode*> m_hshRequests;
    QTimer*                                  m_pTmrRefresh;

}; // class CModelRequestSequencer

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_RequestSequencerModel_h
