/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSSys/ZSSysErrResult.h"

namespace ZS
{
namespace System
{
class CErrLog;
struct SErrLogEntry;
class CTrcAdminObj;

namespace GUI
{
//******************************************************************************
class ZSSYSGUIDLL_API CModelErrLog : public QAbstractItemModel
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CModelErrLog"; }
public: // type definitions and constants
    enum EColumn {
        EColumnSeverityImageUrl =  0,
        EColumnSeverityIcon     =  1,
        EColumnSeverity         =  2,
        EColumnResultNumber     =  3,
        EColumnResult           =  4,
        EColumnDate             =  5,
        EColumnTime             =  6,
        EColumnOccurrences      =  7,
        EColumnSource           =  8,
        EColumnAddInfo          =  9,
        EColumnProposal         = 10,
        EColumnCount,
        EColumnUndefined
    };
    static QString column2Str(EColumn i_clm);
public: // type definitions and constants
    enum class ERole {
        Sort = Qt::UserRole,
        ImageUrl,
        Type,
        FirstDataColumnRole
    };
    Q_ENUM(ERole);
public: // ctors and dtor
    CModelErrLog( CErrLog* i_pErrLog );
    virtual ~CModelErrLog();
public: // auxiliary instance methods
    QString role2Str(int i_iRole) const;
    int byteArr2Role(const QByteArray& i_byteArrRole) const;
    int column2Role(EColumn i_clm) const;
    EColumn role2Column(int i_iRole) const;
    QString modelIndex2Str( const QModelIndex& modelIdx ) const;
public: // instance methods
    Q_INVOKABLE void clear();
    Q_INVOKABLE void removeEntries( const QModelIndexList& i_modelIdxList );
    Q_INVOKABLE void removeEntries( const QVariantList& i_arRowIdxs );
    Q_INVOKABLE void removeEntry( int i_iRowIdx );
protected slots:
    void onEntryAdded( const ZS::System::SErrResultInfo& i_errResultInfo );
    void onEntryChanged( const ZS::System::SErrResultInfo& i_errResultInfo );
    void onEntryRemoved( const ZS::System::SErrResultInfo& i_errResultInfo );
public: // instance methods
    Q_INVOKABLE int columnWidthByColumn(int i_iClm, int i_iFontPixelSize = 0) const;
    Q_INVOKABLE int columnWidthByRole(const QByteArray& i_byteArrRole, int i_iFontPixelSize = 0) const;
public: // overridables of base class QAbstractItemModel
    virtual QHash<int, QByteArray> roleNames() const override;
public: // must overridables of base class QAbstractItemModel
    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const override;
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const override;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const override;
    virtual QModelIndex index( int i_iRow, int i_iClm, const QModelIndex& i_modelIdxParent = QModelIndex() ) const override;
    virtual QModelIndex parent( const QModelIndex& i_modelIdx ) const override;
protected: // overridables of base class QAbstractItemModel
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole ) const override;
protected: // auxiliary instance methods
    SErrLogEntry* findEntry( const SErrResultInfo& i_errResultInfo, int* o_piRowIdx = nullptr, int* o_piRowIdxSeveritySection = nullptr );
    SErrLogEntry* getEntry( int i_iRowIdx, EResultSeverity* o_pSeverity = nullptr, int* o_piRowIdxSeveritySection = nullptr ) const;
protected: // auxiliary instance methods
    void fillRoleNames();
protected: // class members
    static QHash<int, QByteArray> s_clm2Name;
protected: // instance members
    // Please note that entries may be added from within different thread contexts
    // to the error log object and that for this the list of entries of the error
    // log object is protected by a mutex and entries will be "immediately" entered.
    // The models entry list is used by the views allocated to the model and will be
    // accessed by the abstract item model methods. Those methods must be called from
    // within the GUI's main thread and the list may only be modified from within
    // the GUI's main thread (the signals of the error log object have to be queued
    // before calling the slots of the model).
    CErrLog*                      m_pErrLog;
    /*!< Need a copy of the err log model entries as entries may be added, changed
         or removed from different threads. When removing an entry the signal
         entryRemoved is emitted and may be queued. The model cannot access the
         removed entry as it is already deleted. But the model is able to find the
         entry in the internal list and can inform the views about the removed entry. */
    QVector<QList<SErrLogEntry*>> m_ararpEntries;
    QHash<int, QByteArray>        m_roleNames;
    QHash<QByteArray, int>        m_roleValues;
    mutable QVector<int>          m_ariClmWidths;
    ZS::System::CTrcAdminObj*     m_pTrcAdminObj;
    ZS::System::CTrcAdminObj*     m_pTrcAdminObjNoisyMethods;

}; // class CModelErrLog

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_ErrLogModel_h
