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

#include "ZSSysGUI/ZSSysErrLogModel.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtGui/qguiapplication.h>
#include <QtGui/qfontmetrics.h>

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CModelErrLog : public QAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

 QHash<int, QByteArray> CModelErrLog::s_clm2Name {
    { CModelErrLog::EColumnSeverityImageUrl, "SeverityImageUrl"},
    { CModelErrLog::EColumnSeverityIcon, "SeverityIcon"},
    { CModelErrLog::EColumnSeverity, "Severity"},
    { CModelErrLog::EColumnResultNumber, "ResultNumber"},
    { CModelErrLog::EColumnResult, "Result"},
    { CModelErrLog::EColumnDate, "Date"},
    { CModelErrLog::EColumnTime, "Time"},
    { CModelErrLog::EColumnOccurrences, "Occurences"},
    { CModelErrLog::EColumnSource, "Source"},
    { CModelErrLog::EColumnAddInfo, "AddInfo"},
    { CModelErrLog::EColumnProposal, "Proposal"}
};

//------------------------------------------------------------------------------
QString CModelErrLog::column2Str(EColumn i_clm)
//------------------------------------------------------------------------------
{
    return s_clm2Name.value(i_clm, "? (" + QByteArray::number(i_clm) + ")");
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelErrLog::CModelErrLog( CErrLog* i_pErrLog ) :
//------------------------------------------------------------------------------
    QAbstractItemModel(),
    m_pErrLog(i_pErrLog),
    m_ararpEntries(QVector<QList<SErrLogEntry*>>(EResultSeverityCount)),
    m_roleNames(),
    m_roleValues(),
    m_ariClmWidths(QVector<int>(EColumnCount)),
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjNoisyMethods(nullptr)
{
    fillRoleNames();

    setObjectName(i_pErrLog->objectName());

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName(), objectName());
    m_pTrcAdminObjNoisyMethods = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::NoisyMethods", objectName());

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ErrLog: " + i_pErrLog->objectName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strMthInArgs */ strMthInArgs );

    CErrLogLocker errLogLocker(m_pErrLog);

    SErrLogEntry* pErrLogEntry = nullptr;
    int           iRowIdx;

    for( iRowIdx = 0; iRowIdx < m_pErrLog->getEntryCount(); iRowIdx++ )
    {
        pErrLogEntry = m_pErrLog->getEntry(iRowIdx);

        onEntryAdded(pErrLogEntry->m_errResultInfo);
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pErrLog,
        /* szSignal     */ SIGNAL(entryAdded(const ZS::System::SErrResultInfo&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEntryAdded(const ZS::System::SErrResultInfo&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pErrLog,
        /* szSignal     */ SIGNAL(entryChanged(const ZS::System::SErrResultInfo&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEntryChanged(const ZS::System::SErrResultInfo&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pErrLog,
        /* szSignal     */ SIGNAL(entryRemoved(const ZS::System::SErrResultInfo&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEntryRemoved(const ZS::System::SErrResultInfo&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CModelErrLog::~CModelErrLog()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strMthInArgs */ "" );

    int iSeverity;
    int iRowIdx;

    for( iSeverity = m_ararpEntries.count()-1; iSeverity >= 0; iSeverity-- )
    {
        if( m_ararpEntries[iSeverity].count() > 0 )
        {
            beginRemoveRows( QModelIndex(), 0, m_ararpEntries[iSeverity].count()-1 );

            for( iRowIdx = m_ararpEntries[iSeverity].count()-1; iRowIdx >= 0; iRowIdx-- )
            {
                delete m_ararpEntries[iSeverity][iRowIdx];
                m_ararpEntries[iSeverity][iRowIdx] = nullptr;
            }
            endRemoveRows();
        }
    }

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjNoisyMethods);
    m_pTrcAdminObjNoisyMethods = nullptr;

    m_pErrLog = nullptr;

} // dtor

/*==============================================================================
public: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CModelErrLog::role2Str(int i_iRole) const
//------------------------------------------------------------------------------
{
    return m_roleNames.value(i_iRole, "? (" + QByteArray::number(i_iRole) + ")");
}

//------------------------------------------------------------------------------
int CModelErrLog::byteArr2Role(const QByteArray& i_byteArrRole) const
//------------------------------------------------------------------------------
{
    return m_roleValues.value(i_byteArrRole, Qt::DisplayRole);
}

//------------------------------------------------------------------------------
int CModelErrLog::column2Role(EColumn i_clm) const
//------------------------------------------------------------------------------
{
    return static_cast<int>(ERole::FirstDataColumnRole) + i_clm;
}

//------------------------------------------------------------------------------
CModelErrLog::EColumn CModelErrLog::role2Column(int i_iRole) const
//------------------------------------------------------------------------------
{
    if( i_iRole >= static_cast<int>(ERole::FirstDataColumnRole)
        && i_iRole < (static_cast<int>(ERole::FirstDataColumnRole) + EColumnCount))
    {
        return static_cast<EColumn>(i_iRole - static_cast<int>(ERole::FirstDataColumnRole));
    }
    return EColumnUndefined;
}

//------------------------------------------------------------------------------
QString CModelErrLog::modelIndex2Str( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString str;

    if( !i_modelIdx.isValid() )
    {
        str = "Invalid";
    }
    else
    {
        str = "Row: " + QString::number(i_modelIdx.row());
        str += ", Clm: " + QString::number(i_modelIdx.column());
        str += " (" + column2Str(static_cast<EColumn>(i_modelIdx.column())) + ")";
        // Endless recursion if called by ::data method.
        //if( i_modelIdx.data().canConvert(QVariant::String) )
        //{
        //    str += ", Data: " + i_modelIdx.data().toString();
        //}
    }
    return str;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelErrLog::clear()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "clear",
        /* strMthInArgs */ "" );

    CErrLogLocker errLogLocker(m_pErrLog);

    beginResetModel();

    int iSeverity;
    int iRowIdx;

    for( iSeverity = m_ararpEntries.count()-1; iSeverity >= 0; iSeverity-- )
    {
        if( m_ararpEntries[iSeverity].count() > 0 )
        {
            for( iRowIdx = m_ararpEntries[iSeverity].count()-1; iRowIdx >= 0; iRowIdx-- )
            {
                delete m_ararpEntries[iSeverity][iRowIdx];
                m_ararpEntries[iSeverity][iRowIdx] = nullptr;
            }
            m_ararpEntries[iSeverity].clear();
        }
    }

    QObject::disconnect(
        /* pObjSender   */ m_pErrLog,
        /* szSignal     */ SIGNAL(entryRemoved(const ZS::System::SErrResultInfo&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEntryRemoved(const ZS::System::SErrResultInfo&)) );

    m_pErrLog->clear();

    if( !QObject::connect(
        /* pObjSender   */ m_pErrLog,
        /* szSignal     */ SIGNAL(entryRemoved(const ZS::System::SErrResultInfo&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEntryRemoved(const ZS::System::SErrResultInfo&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    endResetModel();
}

//------------------------------------------------------------------------------
void CModelErrLog::removeEntries( const QModelIndexList& i_modelIdxList )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdxList[" + QString::number(i_modelIdxList.count()) + "]";
        if( i_modelIdxList.count() > 0 ) {
            strMthInArgs += "(";
            for( const QModelIndex& modelIdx : i_modelIdxList ) {
                if( !strMthInArgs.endsWith("(")) {
                    strMthInArgs += ", ";
                }
                strMthInArgs += "{" + modelIndex2Str(modelIdx) + "}";
            }
            strMthInArgs += ")";
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "removeEntries",
        /* strMthInArgs */ strMthInArgs );

    if( i_modelIdxList.count() > 0 )
    {
        CErrLogLocker errLogLocker(m_pErrLog);

        // The entries must be removed starting with the highest row index.
        // Otherwise we would remove entries within the list and the row index
        // of the list entries changes.
        QModelIndexList modelIdxListSorted = i_modelIdxList;
        std::sort( modelIdxListSorted.begin(), modelIdxListSorted.end() );
        for( int idx = modelIdxListSorted.count()-1; idx >= 0; --idx )
        {
            const QModelIndex modelIdx = modelIdxListSorted[idx];
            removeEntry(modelIdx.row());
        }
    }

} // removeEntries

//------------------------------------------------------------------------------
void CModelErrLog::removeEntries( const QVariantList& i_arRowIdxs )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "RowIdxList[" + QString::number(i_arRowIdxs.count()) + "]";
        if( i_arRowIdxs.count() > 0 ) {
            strMthInArgs += "(";
            for( const QVariant& val : i_arRowIdxs ) {
                if( !strMthInArgs.endsWith("(")) {
                    strMthInArgs += ", ";
                }
                strMthInArgs += val.toString();
            }
            strMthInArgs += ")";
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "removeEntries",
        /* strMthInArgs */ strMthInArgs );

    if( i_arRowIdxs.count() > 0 )
    {
        CErrLogLocker errLogLocker(m_pErrLog);

        // The entries must be removed starting with the highest row index.
        // Otherwise we would remove entries within the list and the row index
        // of the list entries changes.
        QList<int> arRowIdxsSorted;
        for( const QVariant& val : i_arRowIdxs )
        {
            int iRowIdx = val.toInt();
            arRowIdxsSorted.insert(0, iRowIdx);
        }
        for( int iRowIdx : arRowIdxsSorted )
        {
            removeEntry(iRowIdx);
        }
    }

} // removeEntries

//------------------------------------------------------------------------------
void CModelErrLog::removeEntry( int i_iRowIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "RowIdx: " + QString::number(i_iRowIdx);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "removeEntry",
        /* strMthInArgs */ strMthInArgs );

    CErrLogLocker errLogLocker(m_pErrLog);

    EResultSeverity severity = EResultSeverityUndefined;
    int iRowIdxSeveritySection = -1;

    SErrLogEntry* pModelEntry = getEntry(i_iRowIdx, &severity, &iRowIdxSeveritySection);

    if( pModelEntry != nullptr )
    {
        SErrResultInfo errResultInfo = pModelEntry->m_errResultInfo;

        beginRemoveRows(QModelIndex(), i_iRowIdx, i_iRowIdx);
        m_ararpEntries[severity].removeAt(iRowIdxSeveritySection);
        delete pModelEntry;
        pModelEntry = nullptr;
        endRemoveRows();

        QObject::disconnect(
            /* pObjSender   */ m_pErrLog,
            /* szSignal     */ SIGNAL(entryRemoved(const ZS::System::SErrResultInfo&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEntryRemoved(const ZS::System::SErrResultInfo&)) );

        m_pErrLog->removeEntry(errResultInfo);

        if( !QObject::connect(
            /* pObjSender   */ m_pErrLog,
            /* szSignal     */ SIGNAL(entryRemoved(const ZS::System::SErrResultInfo&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEntryRemoved(const ZS::System::SErrResultInfo&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CModelErrLog::onEntryAdded( const ZS::System::SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_errResultInfo.toString(2);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEntryAdded",
        /* strMthInArgs */ strMthInArgs );

    CErrLogLocker errLogLocker(m_pErrLog);

    SErrLogEntry* pErrLogEntry = m_pErrLog->findEntry(i_errResultInfo);

    // The row state of the entries in the main list and in the view list
    // are set to "Detached" on creating a new entry. The row state in 
    // the main list remains detached and will not be considered anymore.
    // But during the next update timeout processing for all "Detached"
    // entries in the view list the views indicating the content of the
    // model will be informed via the "begin" and "endInsertRows" methods
    // that a new entry has been added to the model. Without this "delay"
    // too many error messages in a short time may block the GUI main thread.

    if( pErrLogEntry != nullptr )
    {
        EResultSeverity severity = i_errResultInfo.getSeverity();
        int iRowIdx = 0;

        for( int iSeverity = m_ararpEntries.count()-1; iSeverity >= severity; --iSeverity )
        {
            iRowIdx += m_ararpEntries[iSeverity].count();
        }

        // Copy the "main" model entry into the model:
        beginInsertRows(QModelIndex(), iRowIdx, iRowIdx);
        SErrLogEntry* pModelEntry = new SErrLogEntry(*pErrLogEntry);
        m_ararpEntries[severity].append(pModelEntry);
        endInsertRows();
    }

} // onEntryAdded

//------------------------------------------------------------------------------
void CModelErrLog::onEntryChanged( const ZS::System::SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_errResultInfo.toString(2);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEntryChanged",
        /* strMthInArgs */ strMthInArgs );

    CErrLogLocker errLogLocker(m_pErrLog);

    int iRowIdx = -1;
    SErrLogEntry* pModelEntry = findEntry(i_errResultInfo, &iRowIdx);
    SErrLogEntry* pErrLogEntry = m_pErrLog->findEntry(i_errResultInfo, &iRowIdx);

    if( pModelEntry != nullptr )
    {
        if( pErrLogEntry != nullptr )
        {
            *pModelEntry = *pErrLogEntry;
        }
        else
        {
            pModelEntry->m_errResultInfo = i_errResultInfo;
        }
        emit dataChanged(index(iRowIdx,0), index(iRowIdx,EColumnCount-1));
    }

} // onEntryChanged

//------------------------------------------------------------------------------
void CModelErrLog::onEntryRemoved( const ZS::System::SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_errResultInfo.toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEntryRemoved",
        /* strMthInArgs */ strMthInArgs );

    CErrLogLocker errLogLocker(m_pErrLog);

    int iRowIdx = -1;
    int iRowIdxSeveritySection = -1;
    SErrLogEntry* pModelEntry = findEntry(i_errResultInfo, &iRowIdx, &iRowIdxSeveritySection);

    if( pModelEntry != nullptr )
    {
        EResultSeverity severity = i_errResultInfo.getSeverity();
        beginRemoveRows(QModelIndex(), iRowIdx, iRowIdx);
        m_ararpEntries[severity].removeAt(iRowIdxSeveritySection);
        delete pModelEntry;
        pModelEntry = nullptr;
        endRemoveRows();
    }

} // onEntryRemoved

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelErrLog::columnWidthByColumn(int i_iClm, int i_iFontPixelSize) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Clm: " + column2Str(static_cast<EColumn>(i_iClm));
        strMthInArgs += ", FontPixelSize: " + QString::number(i_iFontPixelSize);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "columnWidthByColumn",
        /* strMthInArgs */ strMthInArgs );

    int iClmWidth = 1;

    const int iClmSpacing = 10;

    if( i_iClm >= 0 && i_iClm < m_ariClmWidths.count() )
    {
        CErrLogLocker errLogLocker(m_pErrLog);

        iClmWidth = m_ariClmWidths[i_iClm];

        if( iClmWidth == 0)
        {
            QFont fnt = QGuiApplication::font();
            if( i_iFontPixelSize > 0) {
                fnt.setPixelSize(i_iFontPixelSize);
            }

            QFontMetrics fntMetrics = QFontMetrics(fnt);
            QString strClmHeader = headerData(i_iClm, Qt::Horizontal).toString();
            iClmWidth = fntMetrics.horizontalAdvance(strClmHeader + QLatin1String(" ^")) + 8;

            for( int iRowIdx = 0; iRowIdx < rowCount(); ++iRowIdx )
            {
                QString strType = "string";
                QVariant varData = data(index(iRowIdx, i_iClm), static_cast<int>(ERole::Type));
                if( varData.canConvert(QVariant::String) )
                {
                    strType = varData.toString();
                }
                if( strType == "string" || strType == "int" )
                {
                    varData = data(index(iRowIdx, i_iClm), Qt::DisplayRole);
                    if( varData.canConvert(QVariant::String) )
                    {
                        QString strCellData = varData.toString();
                        iClmWidth = qMax(iClmWidth, fntMetrics.horizontalAdvance(strCellData));
                    }
                }
                else if( strType == "imageUrl" || strType == "icon" )
                {
                    varData = data(index(iRowIdx, i_iClm), Qt::DisplayRole);
                    if( varData.canConvert(QVariant::String) )
                    {
                        QString strCellData = varData.toString();
                        QPixmap pixmap(strCellData);
                        iClmWidth = qMax(iClmWidth, pixmap.width());
                    }
                }
            }
            m_ariClmWidths[i_iClm] = iClmWidth + 2*iClmSpacing;
        }
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(iClmWidth);
    }

    return iClmWidth;

} // columnWidthByColumn

//------------------------------------------------------------------------------
int CModelErrLog::columnWidthByRole(const QByteArray& i_byteArrRole, int i_iFontPixelSize) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Role: " + i_byteArrRole;
        strMthInArgs += ", FontPixelSize: " + QString::number(i_iFontPixelSize);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "columnWidthByRole",
        /* strMthInArgs */ strMthInArgs );

    int iClmWidth = 1;

    int iRole = byteArr2Role(i_byteArrRole);
    EColumn clm = role2Column(iRole);

    if(clm != EColumnUndefined)
    {
        iClmWidth = columnWidthByColumn(static_cast<int>(clm), i_iFontPixelSize);
    }
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(iClmWidth);
    }
    return iClmWidth;
}

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
QHash<int, QByteArray> CModelErrLog::roleNames() const
//------------------------------------------------------------------------------
{
    return m_roleNames;
}

/*==============================================================================
protected: // must overridables of base class QAbstractTableModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelErrLog::rowCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Parent {" + modelIndex2Str(i_modelIdxParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjNoisyMethods,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "rowCount",
        /* strMthInArgs */ strMthInArgs );

    CErrLogLocker errLogLocker(m_pErrLog);

    int iRowCount = 0;

    for( int iSeverity = 0; iSeverity < m_ararpEntries.count(); ++iSeverity )
    {
        iRowCount += m_ararpEntries[iSeverity].count();
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(iRowCount);
    }
    return iRowCount;

} // rowCount

//------------------------------------------------------------------------------
int CModelErrLog::columnCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Parent {" + modelIndex2Str(i_modelIdxParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjNoisyMethods,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "columnCount",
        /* strMthInArgs */ strMthInArgs );

    int iClmCount = EColumnCount;

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(iClmCount);
    }
    return iClmCount;
}

//------------------------------------------------------------------------------
QVariant CModelErrLog::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    EColumn clm = static_cast<EColumn>(i_modelIdx.column());
    int iRole = i_iRole;

    if( i_iRole >= static_cast<int>(ERole::FirstDataColumnRole) )
    {
        clm = role2Column(i_iRole);
        iRole = Qt::DisplayRole;
    }

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdx {" + modelIndex2Str(i_modelIdx) + "}";
        strMthInArgs += ", Role: " + QString::number(i_iRole) + " (" + role2Str(i_iRole) + ")";

        if( i_iRole >= static_cast<int>(ERole::FirstDataColumnRole) )
        {
            strMthInArgs += ", {-> Clm: " + QString::number(static_cast<int>(clm)) + " (" + column2Str(clm) + ")";
            strMthInArgs += ", Role: " + QString::number(static_cast<int>(iRole)) + " (" + role2Str(iRole) + ")}";
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "data",
        /* strMthInArgs */ strMthInArgs );

    static QSet<int> s_rolesHandled = {
        Qt::DisplayRole,
        Qt::DecorationRole,
        Qt::ToolTipRole,
        static_cast<int>(ERole::Sort),
        static_cast<int>(ERole::ImageUrl),
        static_cast<int>(ERole::Type)
    };

    QVariant varData;

    if( !i_modelIdx.isValid() )
    {
        if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
        {
            mthTracer.setMethodReturn(varData.toString());
        }
        return varData;
    }

    CErrLogLocker errLogLocker(m_pErrLog);

    int iRow = i_modelIdx.row();

    if( iRow < 0 || iRow >= rowCount() )
    {
        if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
        {
            mthTracer.setMethodReturn(varData.toString());
        }
        return varData;
    }

    const SErrLogEntry* pErrLogEntry = m_pErrLog->getEntry(iRow);

    if( pErrLogEntry == nullptr )
    {
        if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
        {
            mthTracer.setMethodReturn(varData.toString());
        }
        return varData;
    }

    if( iRole == Qt::ToolTipRole )
    {
        QString strTmp = pErrLogEntry->m_strProposal;
        strTmp = encodeForHtml(strTmp);
        varData = strTmp;
    }
    else if( s_rolesHandled.contains(iRole) )
    {
        switch( clm )
        {
            case EColumnSeverityImageUrl:
            {
                if( iRole == static_cast<int>(ERole::Type) )
                {
                    varData = QLatin1String("imageUrl");
                }
                else if( iRole == Qt::DisplayRole || iRole == Qt::DecorationRole )
                {
                    QString strUrl = getErrImageUrl(pErrLogEntry->m_errResultInfo.getSeverity());
                    if( strUrl.startsWith(":") ) {
                        strUrl.insert(0, "qrc");
                    }
                    else if( !strUrl.startsWith("qrc:") ) {
                        strUrl.insert(0, "qrc:");
                    }
                    varData = strUrl;
                }
                break;
            }
            case EColumnSeverityIcon:
            {
                if( iRole == static_cast<int>(ERole::Type) )
                {
                    varData = QLatin1String("icon");
                }
                else if( iRole == Qt::DisplayRole )
                {
                }
                else if( i_iRole == Qt::DecorationRole )
                {
                    varData = getErrIcon(pErrLogEntry->m_errResultInfo.getSeverity());
                }
                break;
            }
            case EColumnSeverity:
            {
                if( iRole == static_cast<int>(ERole::Type) )
                {
                    varData = QLatin1String("string");
                }
                else if( iRole == Qt::DisplayRole )
                {
                    varData = resultSeverity2Str(pErrLogEntry->m_errResultInfo.getSeverity());
                }
                break;
            }
            case EColumnResultNumber:
            {
                if( iRole == static_cast<int>(ERole::Type) )
                {
                    varData = QLatin1String("int");
                }
                else if( iRole == Qt::DisplayRole )
                {
                    varData = QString::number(pErrLogEntry->m_errResultInfo.getResult());
                }
                break;
            }
            case EColumnResult:
            {
                if( iRole == static_cast<int>(ERole::Type) )
                {
                    varData = QLatin1String("string");
                }
                else if( iRole == Qt::DisplayRole )
                {
                    varData = pErrLogEntry->m_errResultInfo.getResultStr();
                }
                break;
            }
            case EColumnDate:
            {
                if( iRole == static_cast<int>(ERole::Type) )
                {
                    varData = QLatin1String("string");
                }
                else if( iRole == Qt::DisplayRole )
                {
                    varData = pErrLogEntry->m_dateTime.toString("yyyy-MM-dd");
                }
                break;
            }
            case EColumnTime:
            {
                if( iRole == static_cast<int>(ERole::Type) )
                {
                    varData = QLatin1String("string");
                }
                else if( iRole == Qt::DisplayRole )
                {
                    varData = pErrLogEntry->m_dateTime.toString("hh:mm:ss");
                }
                break;
            }
            case EColumnOccurrences:
            {
                if( iRole == static_cast<int>(ERole::Type) )
                {
                    varData = QLatin1String("int");
                }
                else if( iRole == Qt::DisplayRole )
                {
                    varData = pErrLogEntry->m_iOccurrences;
                }
                break;
            }
            case EColumnSource:
            {
                if( iRole == static_cast<int>(ERole::Type) )
                {
                    varData = QLatin1String("string");
                }
                else if( iRole == Qt::DisplayRole )
                {
                    QString strTmp = pErrLogEntry->m_errResultInfo.getErrSource().toString();
                    strTmp = decodeFromHtml(strTmp);
                    varData = strTmp;
                }
                break;
            }
            case EColumnAddInfo:
            {
                if( iRole == static_cast<int>(ERole::Type) )
                {
                    varData = QLatin1String("string");
                }
                else if( iRole == Qt::DisplayRole )
                {
                    QString strTmp = pErrLogEntry->m_errResultInfo.getAddErrInfoDscr();
                    strTmp = decodeFromHtml(strTmp);
                    varData = strTmp;
                }
                break;
            }
            case EColumnProposal:
            {
                if( iRole == static_cast<int>(ERole::Type) )
                {
                    varData = QLatin1String("string");
                }
                else if( iRole == Qt::DisplayRole )
                {
                    QString strTmp = pErrLogEntry->m_strProposal;
                    strTmp = decodeFromHtml(strTmp);
                    varData = strTmp;
                }
                break;
            }
            default:
            {
                break;
            }
        }
    } // if( s_rolesHandled.contains(iRole) )

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(varData.toString());
    }
    return varData;

} // data

//------------------------------------------------------------------------------
QModelIndex CModelErrLog::index( int i_iRow, int i_iClm, const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs  = "Row: " + QString::number(i_iRow);
        strMthInArgs += ", Clm: " + column2Str(static_cast<EColumn>(i_iClm));
        strMthInArgs += ", Parent {" + modelIndex2Str(i_modelIdxParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjNoisyMethods,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "index",
        /* strMthInArgs */ strMthInArgs );

    CErrLogLocker errLogLocker(m_pErrLog);

    const SErrLogEntry* pErrLogEntry = m_pErrLog->getEntry(i_iRow);

    QModelIndex modelIdx = createIndex(i_iRow, i_iClm, const_cast<SErrLogEntry*>(pErrLogEntry));

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(modelIndex2Str(modelIdx));
    }

    return modelIdx;

} // index

//------------------------------------------------------------------------------
QModelIndex CModelErrLog::parent( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "{" + modelIndex2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjNoisyMethods,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "parent",
        /* strMthInArgs */ strMthInArgs );

    QModelIndex modelIdx;

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(modelIndex2Str(modelIdx));
    }

    return modelIdx;
}

/*==============================================================================
protected: // overridables of base class QAbstractTableModel
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CModelErrLog::headerData(
    int             i_iSection,
    Qt::Orientation i_orientation,
    int             i_iRole ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Section: " + column2Str(static_cast<EColumn>(i_iSection));
        strMthInArgs += ", Orientation: " + qOrientation2Str(i_orientation);
        strMthInArgs += ", Role: " + role2Str(i_iRole);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "headerData",
        /* strMthInArgs */ strMthInArgs );

    QVariant varData;

    if( i_orientation == Qt::Horizontal )
    {
        switch( i_iSection )
        {
            case EColumnSeverityImageUrl:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Severity Image Url";
                }
                break;
            }
            case EColumnSeverityIcon:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Severity";
                }
                break;
            }
            case EColumnSeverity:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Severity";
                }
                break;
            }
            case EColumnResultNumber:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Result Nr.";
                }
                break;
            }
            case EColumnResult:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Result";
                }
                break;
            }
            case EColumnDate:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Date";
                }
                break;
            }
            case EColumnTime:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Time";
                }
                break;
            }
            case EColumnOccurrences:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Occurrences";
                }
                break;
            }
            case EColumnSource:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Source";
                }
                break;
            }
            case EColumnAddInfo:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Additional Info";
                }
                break;
            }
            case EColumnProposal:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Proposal";
                }
                break;
            }
            default:
            {
                break;
            }
        }
    }
    else if( i_orientation == Qt::Vertical )
    {
        if( i_iRole == Qt::DisplayRole )
        {
            varData = i_iSection;
        }
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(varData.toString());
    }
    return varData;

} // headerData

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Searches the error log entry for the given error result info.

    The search pattern includes the result code, the result severity,
    the error source and the additional error info.

    @param i_errResultInfo [in] Error result info.

    @param o_piRowIdx [out]
        Overall index of the entry in the list of error log entries.

    @param o_piRowIdxSeveritySection [out]
        Index of the entry in the list of error log entries of the severity.

    @return Pointer to error log entry or nullptr, if the corresponding entry
            could not be found.
*/
SErrLogEntry* CModelErrLog::findEntry(
    const SErrResultInfo& i_errResultInfo,
    int* o_piRowIdx,
    int* o_piRowIdxSeveritySection )
//------------------------------------------------------------------------------
{
    EResultSeverity severity = i_errResultInfo.getSeverity();

    int iRowIdx = 0;
    int iRowIdxSeveritySection = 0;

    SErrLogEntry* pModelEntry = nullptr;
    SErrLogEntry* pModelEntryTmp;

    if( o_piRowIdx != nullptr )
    {
        // Counting rows starts at highest severity (Critical).
        for( int iSeverity = EResultSeverityCount-1; iSeverity > static_cast<int>(severity); --iSeverity )
        {
            iRowIdx += m_ararpEntries[iSeverity].count();
        }
    }
    for( iRowIdxSeveritySection = 0; iRowIdxSeveritySection < m_ararpEntries[severity].count(); iRowIdxSeveritySection++ )
    {
        pModelEntryTmp = m_ararpEntries[severity][iRowIdxSeveritySection];

        if( pModelEntryTmp != nullptr )
        {
            if( pModelEntryTmp->m_errResultInfo == i_errResultInfo )
            {
                pModelEntry = pModelEntryTmp;
                iRowIdx += iRowIdxSeveritySection;
                break;
            }
        }
    }
    if( o_piRowIdx != nullptr )
    {
        if( pModelEntry == nullptr ) {
            *o_piRowIdx = -1;
        } else {
            *o_piRowIdx = iRowIdx;
        }
    }
    if( o_piRowIdxSeveritySection != nullptr )
    {
        if( pModelEntry == nullptr ) {
            *o_piRowIdxSeveritySection = -1;
        } else {
            *o_piRowIdxSeveritySection = iRowIdxSeveritySection;
        }
    }
    return pModelEntry;

} // findEntry

//------------------------------------------------------------------------------
SErrLogEntry* CModelErrLog::getEntry(
    int i_iRowIdx,
    EResultSeverity* o_pSeverity,
    int* o_piRowIdxSeveritySection ) const
//------------------------------------------------------------------------------
{
    SErrLogEntry* pModelEntry = nullptr;

    EResultSeverity severity = EResultSeverityUndefined;
    int iRowIdxSeveritySection = 0;

    int iFirstRowIdxSeveritySection = 0;

    // Counting rows starts at highest severity (Critical).
    for( int iSeverity = EResultSeverityCount-1; iSeverity >= 0; iSeverity-- )
    {
        if( i_iRowIdx < (iFirstRowIdxSeveritySection + m_ararpEntries[iSeverity].count()) )
        {
            iRowIdxSeveritySection = i_iRowIdx-iFirstRowIdxSeveritySection;
            pModelEntry = m_ararpEntries[iSeverity][iRowIdxSeveritySection];
            severity = static_cast<EResultSeverity>(iSeverity);
            break;
        }
        iFirstRowIdxSeveritySection += m_ararpEntries[iSeverity].count();
    }
    if( o_pSeverity != nullptr )
    {
        if( pModelEntry == nullptr ) {
            *o_pSeverity = EResultSeverityUndefined;
        } else {
            *o_pSeverity = severity;
        }
    }
    if( o_piRowIdxSeveritySection != nullptr )
    {
        if( pModelEntry == nullptr ) {
            *o_piRowIdxSeveritySection = -1;
        } else {
            *o_piRowIdxSeveritySection = iRowIdxSeveritySection;
        }
    }
    return pModelEntry;

} // getEntry

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelErrLog::fillRoleNames()
//------------------------------------------------------------------------------
{
    m_roleNames = QAbstractItemModel::roleNames();

    m_roleNames[static_cast<int>(ERole::Sort)] = "sort";
    m_roleNames[static_cast<int>(ERole::ImageUrl)] = "imageUrl";
    m_roleNames[static_cast<int>(ERole::Type)] = "type";

    for( int clm = 0; clm < EColumnCount; ++clm)
    {
        int role = column2Role(static_cast<CModelErrLog::EColumn>(clm));
        m_roleNames[role] = s_clm2Name[clm];
    }

    for( int iRole : m_roleNames.keys() )
    {
        const QByteArray& byteArrRole = m_roleNames.value(iRole);
        m_roleValues[byteArrRole] = iRole;
    }
}
