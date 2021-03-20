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

#ifndef ZSSysGUI_ErrLogModel_h
#define ZSSysGUI_ErrLogModel_h

#include <QtCore/qabstractitemmodel.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qcoreevent.h>
#include <QtCore/qmutex.h>

#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSSys/ZSSysErrResult.h"

namespace ZS
{
namespace System
{
class CErrLog;
struct SErrLogEntry;

namespace GUI
{
//******************************************************************************
class ZSSYSGUIDLL_API CModelErrLog : public QAbstractItemModel // QAbstractTableModel
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    enum EColumn {
        EColumnRowIdx         =  0,
        EColumnSeverityIcon   =  1,
        EColumnDate           =  2,
        EColumnTime           =  3,
        EColumnSeverity       =  4,
        EColumnResult         =  5,
        EColumnResultStr      =  6,
        EColumnOccurrences    =  7,
        EColumnSource         =  8,
        EColumnAddInfo        =  9,
        EColumnProposal       = 10,
        EColumnCount
    };
public: // ctors and dtor
    CModelErrLog( CErrLog* i_pErrLog );
    virtual ~CModelErrLog();
public: // instance methods
    SErrLogEntry* findEntry( const ZS::System::SErrResultInfo& i_errResultInfo );
    void removeEntries( const QModelIndexList& i_modelIdxList );
    void clear();
protected slots:
    void onEntryAdded( const ZS::System::SErrResultInfo& i_errResultInfo );   // called on adding an entry to the main model
    void onEntryChanged( const ZS::System::SErrResultInfo& i_errResultInfo ); // called on changing an entry in the main model
    void onEntryRemoved( const ZS::System::SErrResultInfo& i_errResultInfo );
protected: // instance methods
    SErrLogEntry* getEntry( int i_iRowIdx, EResultSeverity i_severity = EResultSeverityUndefined );
    void removeEntry( int i_iRowIdx, EResultSeverity i_severity = EResultSeverityUndefined );
protected: // must overridables of base class QAbstractItemModel
    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const;
    virtual QModelIndex index( int i_iRow, int i_iCol, const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QModelIndex parent( const QModelIndex& i_modelIdx ) const;
protected: // overridables of base class QAbstractItemModel
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole ) const;
protected: // instance members
    // Please note that entries may be added from within different thread contexts
    // to the error log object and that for this the list of entries of the error
    // log object is protected by a mutex and entries will be "immediately" entered.
    // The models entry list is used by the views allocated to the model and will be
    // accessed by the abstract item model methods. Those methods must be called from
    // within the GUI's main thread and the list may only be modified from within
    // the GUI's main thread (the signals of the error log object have to be queued
    // before calling the slots of the model).
    CErrLog*             m_pErrLog;
    QList<SErrLogEntry*> m_ararpEntries[EResultSeverityCount];

}; // class CModelErrLog

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_ErrLogModel_h
