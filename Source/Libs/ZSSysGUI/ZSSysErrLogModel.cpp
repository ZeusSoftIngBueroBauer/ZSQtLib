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
    { CModelErrLog::EColumnRowIdx, "RowIdx"},
    { CModelErrLog::EColumnSeverityRowIdx, "SeverityRowIdx"},
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
CModelErrLog::CModelErrLog( CErrLog* i_pErrLog, bool i_bUsedByQmlListModels ) :
//------------------------------------------------------------------------------
    QAbstractItemModel(),
    m_pErrLog(i_pErrLog),
    m_ararpEntries(QVector<QList<SErrLogEntry*>>(EResultSeverityCount)),
    m_bUsedByQmlListModels(i_bUsedByQmlListModels),
    m_roleNames(),
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
        strMthInArgs += ", UsedByQmlListModels: " + bool2Str(i_bUsedByQmlListModels);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strMthInArgs */ strMthInArgs );

    m_pErrLog->lock();

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

    m_pErrLog->unlock();

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
    m_bUsedByQmlListModels = false;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SErrLogEntry* CModelErrLog::findEntry( const ZS::System::SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    SErrLogEntry* pEntryView = nullptr;
    SErrLogEntry* pEntryViewTmp;
    int           iRowIdx;

    EResultSeverity severity = i_errResultInfo.getSeverity();

    for( iRowIdx = 0; iRowIdx < m_ararpEntries[severity].count(); iRowIdx++ )
    {
        pEntryViewTmp = m_ararpEntries[severity][iRowIdx];

        if( pEntryViewTmp != nullptr )
        {
            if( pEntryViewTmp->m_errResultInfo == i_errResultInfo )
            {
                pEntryView = pEntryViewTmp;
                break;
            }
        }
    }

    return pEntryView;

} // findEntry

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
        m_pErrLog->lock();

        QObject::disconnect(
            /* pObjSender   */ m_pErrLog,
            /* szSignal     */ SIGNAL(entryRemoved(const ZS::System::SErrResultInfo&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEntryRemoved(const ZS::System::SErrResultInfo&)) );

        QModelIndexList modelIdxListSorted = i_modelIdxList;
        QModelIndex     modelIdx;
        SErrLogEntry*   pEntryView;
        int             idxRow;

        std::sort( modelIdxListSorted.begin(), modelIdxListSorted.end() );

        //beginRemoveRows( QModelIndex(), modelIdxListSorted[0].row(), modelIdxListSorted[modelIdxListSorted.size()-1].row() );

        for( idxRow = modelIdxListSorted.count()-1; idxRow >= 0; idxRow-- )
        {
            modelIdx = modelIdxListSorted[idxRow];

            pEntryView = getEntry(modelIdx.row());

            m_pErrLog->removeEntry(pEntryView->m_errResultInfo);

            removeEntry(modelIdx.row());
        }

        //endRemoveRows();

        if( !QObject::connect(
            /* pObjSender   */ m_pErrLog,
            /* szSignal     */ SIGNAL(entryRemoved(const ZS::System::SErrResultInfo&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEntryRemoved(const ZS::System::SErrResultInfo&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        m_pErrLog->unlock();

    } // if( i_modelIdxList.count() > 0 )

} // removeEntries

//------------------------------------------------------------------------------
void CModelErrLog::clear()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "clear",
        /* strMthInArgs */ "" );

    m_pErrLog->lock();

    //QObject::disconnect(
    //    /* pObjSender   */ m_pErrLog,
    //    /* szSignal     */ SIGNAL(entryRemoved(const ZS::System::SErrResultInfo&)),
    //    /* pObjReceiver */ this,
    //    /* szSlot       */ SLOT(onEntryRemoved(const ZS::System::SErrResultInfo&)) );

    m_pErrLog->clear();

    //SErrLogEntry* pEntryView;

    //int iSeverity;
    //int idxRow;

    //for( iSeverity = EResultSeverityCount-1; iSeverity >= 0; iSeverity-- )
    //{
    //    if( m_ararpEntries[iSeverity].count() > 0 )
    //    {
    //        //beginRemoveRows( QModelIndex(), 0, m_ararpEntries[iSeverity].size()-1 );

    //        for( idxRow = m_ararpEntries[iSeverity].size()-1; idxRow >= 0; idxRow-- )
    //        {
    //            pEntryView = m_ararpEntries[iSeverity][idxRow];

    //            m_pErrLog->removeEntry(pEntryView->m_errResultInfo);

    //            removeEntry(idxRow, pEntryView->m_errResultInfo.getSeverity());
    //        }

    //        //endRemoveRows();

    //    } // if( m_ararpEntries[iSeverity].count() > 0 )
    //} // for( iSeverity = EResultSeverityCount-1; iSeverity >= 0; iSeverity-- )

    //if( !QObject::connect(
    //    /* pObjSender   */ m_pErrLog,
    //    /* szSignal     */ SIGNAL(entryRemoved(const ZS::System::SErrResultInfo&)),
    //    /* pObjReceiver */ this,
    //    /* szSlot       */ SLOT(onEntryRemoved(const ZS::System::SErrResultInfo&)) ) )
    //{
    //    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    //}

    m_pErrLog->unlock();

} // clear

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
int CModelErrLog::column2Role(EColumn i_clm) const
//------------------------------------------------------------------------------
{
    if( m_bUsedByQmlListModels )
    {
        return static_cast<int>(ERole::FirstDataColumnRole) + i_clm;
    }
    return -1;
}

//------------------------------------------------------------------------------
CModelErrLog::EColumn CModelErrLog::role2Column(int i_iRole) const
//------------------------------------------------------------------------------
{
    if( m_bUsedByQmlListModels )
    {
        if( i_iRole >= static_cast<int>(ERole::FirstDataColumnRole)
         && i_iRole < (static_cast<int>(ERole::FirstDataColumnRole) + EColumnCount))
        {
            return static_cast<EColumn>(i_iRole - Qt::UserRole);
        }
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

        if( !m_bUsedByQmlListModels )
        {
            str += ", Clm: " + column2Str(static_cast<EColumn>(i_modelIdx.column()));
        }
        // Endless recursion if called by ::data method.
        //if( i_modelIdx.data().canConvert(QVariant::String) )
        //{
        //    str += ", Data: " + i_modelIdx.data().toString();
        //}
    }
    return str;
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

    m_pErrLog->lock();

    // Search entry in "main" list.
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

        // Copy the "main" model entry into the "views" model:
        SErrLogEntry* pEntryView = new SErrLogEntry(*pErrLogEntry);

        // The models row index is different from the err logs row index:
        pEntryView->m_iRowIdx = m_ararpEntries[severity].count();

        beginInsertRows( QModelIndex(), pEntryView->m_iRowIdx, pEntryView->m_iRowIdx );
        m_ararpEntries[severity].append(pEntryView);
        endInsertRows();

        //emit entryAdded(pEntryView);
    }

    m_pErrLog->unlock();

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

    m_pErrLog->lock();

    // Search entry in "main" list.
    SErrLogEntry* pErrLogEntry = m_pErrLog->findEntry(i_errResultInfo);

    if( pErrLogEntry != nullptr )
    {
        // Search entry in "views" model.
        int           iRowIdx;
        SErrLogEntry* pEntryView = nullptr;
        SErrLogEntry* pEntryViewTmp;

        EResultSeverity severity = i_errResultInfo.getSeverity();

        for( iRowIdx = 0; iRowIdx < m_ararpEntries[severity].count(); iRowIdx++ )
        {
            pEntryViewTmp = m_ararpEntries[severity][iRowIdx];

            if( pEntryViewTmp != nullptr )
            {
                if( pEntryViewTmp->m_errResultInfo == i_errResultInfo )
                {
                    pEntryView = pEntryViewTmp;
                    break;
                }
            }
        }

        if( pEntryView != nullptr )
        {
            // Copy the "main" model entry into the "views" model. But keep
            // the models row index which may be different from the err logs row index.
            iRowIdx = pEntryView->m_iRowIdx;
            *pEntryView = *pErrLogEntry;
            pEntryView->m_iRowIdx = iRowIdx;

            emit dataChanged( index(pEntryView->m_iRowIdx,0), index(pEntryView->m_iRowIdx,EColumnCount-1) );
        }
    }

    m_pErrLog->unlock();

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

    m_pErrLog->lock();

    // Search entry in my view list.
    SErrLogEntry* pEntryView = findEntry(i_errResultInfo);

    if( pEntryView != nullptr )
    {
        EResultSeverity severity = i_errResultInfo.getSeverity();

        int iRowIdx = pEntryView->m_iRowIdx;

        delete pEntryView;
        pEntryView = nullptr;

        beginRemoveRows( QModelIndex(), iRowIdx, iRowIdx );
        m_ararpEntries[severity].removeAt(iRowIdx);
        endRemoveRows();

        for( ; iRowIdx < m_ararpEntries[severity].count(); iRowIdx++ )
        {
            pEntryView = m_ararpEntries[severity][iRowIdx];

            pEntryView->m_iRowIdx--;

            emit dataChanged( index(pEntryView->m_iRowIdx,0), index(pEntryView->m_iRowIdx,EColumnCount-1) );
        }
    }

    m_pErrLog->unlock();

} // onEntryRemoved

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SErrLogEntry* CModelErrLog::getEntry( int i_iRowIdx, EResultSeverity i_severity )
//------------------------------------------------------------------------------
{
    SErrLogEntry* pEntry = nullptr;

    if( i_severity == EResultSeverityUndefined || i_severity == EResultSeverityCount )
    {
        int iCount = 0;

        // In this case counting rows starts at highest severity (Critical).
        for( int iSeverity = EResultSeverityCount-1; iSeverity >= 0; iSeverity-- )
        {
            if( i_iRowIdx < (iCount + m_ararpEntries[iSeverity].count()) )
            {
                pEntry = m_ararpEntries[iSeverity][i_iRowIdx-iCount];
                break;
            }
            iCount += m_ararpEntries[iSeverity].count();
        }
    }
    else // if( i_severity >= 0 && i_severity < EResultSeverityCount )
    {
        if( i_iRowIdx >= 0 && i_iRowIdx < m_ararpEntries[i_severity].count() )
        {
            pEntry = m_ararpEntries[i_severity][i_iRowIdx];
        }
    }

    return pEntry;

} // getEntry

//------------------------------------------------------------------------------
void CModelErrLog::removeEntry( int i_iRowIdx, EResultSeverity i_severity )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "RowIdx: " + QString::number(i_iRowIdx);
        strMthInArgs += ", Severity: " + resultSeverity2Str(i_severity);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "removeEntry",
        /* strMthInArgs */ strMthInArgs );

    SErrLogEntry* pEntry = nullptr;

    if( i_severity == EResultSeverityUndefined || i_severity == EResultSeverityCount )
    {
        int iCount = 0;

        // In this case counting rows starts at highest severity (Critical).
        for( int iSeverity = EResultSeverityCount-1; iSeverity >= 0; iSeverity-- )
        {
            if( i_iRowIdx < (iCount + m_ararpEntries[iSeverity].count()) )
            {
                pEntry = m_ararpEntries[iSeverity][i_iRowIdx-iCount];

                if( pEntry != nullptr )
                {
                    SErrResultInfo errResultInfo = pEntry->m_errResultInfo;

                    int iRowIdx = pEntry->m_iRowIdx;

                    beginRemoveRows( QModelIndex(), iRowIdx, iRowIdx );

                    m_ararpEntries[iSeverity].removeAt(iRowIdx);

                    delete pEntry;
                    pEntry = nullptr;

                    for( ; iRowIdx < m_ararpEntries[iSeverity].count(); iRowIdx++ )
                    {
                        pEntry = m_ararpEntries[iSeverity][iRowIdx];

                        pEntry->m_iRowIdx--;
                    }

                    endRemoveRows();

                } // if( pEntry != nullptr )

                break;

            } // if( i_iRowIdx < (iCount + m_ararpEntries[iSeverity].count()) )

            iCount += m_ararpEntries[iSeverity].count();

        } // for( int iSeverity = EResultSeverityCount-1; iSeverity >= 0; iSeverity-- )
    } // if( i_severity == EResultSeverityUndefined || i_severity == EResultSeverityCount )

    else // if( i_severity >= 0 && i_severity < EResultSeverityCount )
    {
        if( i_iRowIdx >= 0 && i_iRowIdx < m_ararpEntries[i_severity].count() )
        {
            pEntry = m_ararpEntries[i_severity][i_iRowIdx];

            if( pEntry != nullptr )
            {
                SErrResultInfo errResultInfo = pEntry->m_errResultInfo;

                int iRowIdx = pEntry->m_iRowIdx;

                beginRemoveRows( QModelIndex(), iRowIdx, iRowIdx );

                m_ararpEntries[i_severity].removeAt(iRowIdx);

                delete pEntry;
                pEntry = nullptr;

                for( ; iRowIdx < m_ararpEntries[i_severity].count(); iRowIdx++ )
                {
                    pEntry = m_ararpEntries[i_severity][iRowIdx];

                    pEntry->m_iRowIdx--;
                }

                endRemoveRows();

            } // if( pEntry != nullptr )
        } // if( i_iRowIdx >= 0 && i_iRowIdx < m_ararpEntries[i_severity].count() )
    } // if( i_severity >= 0 && i_severity < EResultSeverityCount )

} // removeEntry

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelErrLog::columnWidth(int i_iClm, const QFont* i_pFont) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Clm: " + column2Str(static_cast<EColumn>(i_iClm));
        strMthInArgs += ", Font: " + QString(i_pFont == nullptr ? "null" : i_pFont->toString());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "columnWidth",
        /* strMthInArgs */ strMthInArgs );

    int iClmWidth = 1;

    if( i_iClm >= 0 && i_iClm < m_ariClmWidths.count() )
    {
        iClmWidth = m_ariClmWidths[i_iClm];

        if( iClmWidth == 0)
        {
            QFontMetrics fntMetrics =
                i_pFont == nullptr ? QFontMetrics(QGuiApplication::font()) : QFontMetrics(*i_pFont);
            QString strClmHeader = headerData(i_iClm, Qt::Horizontal).toString();
            iClmWidth = fntMetrics.horizontalAdvance(strClmHeader + QLatin1String(" ^")) + 8;

            for( int iSeverity = 0; iSeverity < m_ararpEntries.count(); ++iSeverity )
            {
                for( int iRow = 0; iRow < m_ararpEntries[iSeverity].count(); ++iRow )
                {
                    QString strType = "string";
                    QVariant varData = data(index(iRow, i_iClm), static_cast<int>(ERole::Type));
                    if( varData.canConvert(QVariant::String) )
                    {
                        strType = varData.toString();
                    }
                    if( strType == "string" || strType == "int" )
                    {
                        varData = data(index(iRow, i_iClm), Qt::DisplayRole);
                        if( varData.canConvert(QVariant::String) )
                        {
                            QString strCellData = varData.toString();
                            iClmWidth = qMax(iClmWidth, fntMetrics.horizontalAdvance(strCellData));
                        }
                    }
                    else if( strType == "imageUrl" || strType == "icon" )
                    {
                        varData = data(index(iRow, i_iClm), Qt::DisplayRole);
                        if( varData.canConvert(QVariant::String) )
                        {
                            QString strCellData = varData.toString();
                            QPixmap pixmap(strCellData);
                            iClmWidth = qMax(iClmWidth, pixmap.width());
                        }
                    }
                }
            }
            m_ariClmWidths[i_iClm] = iClmWidth;
        }
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(iClmWidth);
    }

    return iClmWidth;

} // columnWidth

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

    int iRowCount = 0;

    int iSeverity;

    for( iSeverity = EResultSeverityCount-1; iSeverity >= 0; iSeverity-- )
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

    if( m_bUsedByQmlListModels )
    {
        iClmCount = 1;
    }

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
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdx {" + modelIndex2Str(i_modelIdx) + "}";
        strMthInArgs += ", Role: " + role2Str(i_iRole);
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

    EColumn clm = static_cast<EColumn>(i_modelIdx.column());
    int iRole = i_iRole;

    if( m_bUsedByQmlListModels && i_iRole >= static_cast<int>(ERole::FirstDataColumnRole) )
    {
        clm = role2Column(i_iRole);
        iRole = Qt::DisplayRole;
    }

    const SErrLogEntry* pEntryView = nullptr;

    int iRow = i_modelIdx.row();

    if( iRow < 0 || iRow >= rowCount() )
    {
        if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
        {
            mthTracer.setMethodReturn(varData.toString());
        }
        return varData;
    }

    CModelErrLog* pVThis = const_cast<CModelErrLog*>(this);

    pEntryView = const_cast<const SErrLogEntry*>(pVThis->getEntry(iRow));

    if( pEntryView == nullptr )
    {
        if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
        {
            mthTracer.setMethodReturn(varData.toString());
        }
        return varData;
    }

    if( iRole == Qt::ToolTipRole )
    {
        QString strTmp = pEntryView->m_strProposal;
        strTmp = encodeForHtml(strTmp);
        varData = strTmp;
    }
    else if( s_rolesHandled.contains(iRole) )
    {
        switch( clm )
        {
            case EColumnRowIdx:
            {
                if( iRole == static_cast<int>(ERole::Type) )
                {
                    varData = QLatin1String("int");
                }
                else if( iRole == Qt::DisplayRole )
                {
                    varData = iRow;
                }
                break;
            }
            case EColumnSeverityRowIdx:
            {
                if( iRole == static_cast<int>(ERole::Type) )
                {
                    varData = QLatin1String("int");
                }
                else if( iRole == Qt::DisplayRole )
                {
                    varData = pEntryView->m_iRowIdx;
                }
                break;
            }
            case EColumnSeverityImageUrl:
            {
                if( iRole == static_cast<int>(ERole::Type) )
                {
                    varData = QLatin1String("imageUrl");
                }
                else if( iRole == Qt::DisplayRole )
                {
                    varData = getErrImageUrl(pEntryView->m_errResultInfo.getSeverity());
                }
                else if( iRole == Qt::DecorationRole )
                {
                    QString strUrl = getErrImageUrl(pEntryView->m_errResultInfo.getSeverity());
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
                    varData = getErrIcon(pEntryView->m_errResultInfo.getSeverity());
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
                    varData = resultSeverity2Str(pEntryView->m_errResultInfo.getSeverity());
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
                    varData = QString::number(pEntryView->m_errResultInfo.getResult());
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
                    varData = pEntryView->m_errResultInfo.getResultStr();
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
                    varData = pEntryView->m_dateTime.toString("yyyy-MM-dd");
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
                    varData = pEntryView->m_dateTime.toString("hh:mm:ss");
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
                    varData = pEntryView->m_iOccurrences;
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
                    QString strTmp = pEntryView->m_errResultInfo.getErrSource().toString();
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
                    QString strTmp = pEntryView->m_errResultInfo.getAddErrInfoDscr();
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
                    QString strTmp = pEntryView->m_strProposal;
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

    QModelIndex modelIdx;

    const SErrLogEntry* pEntryView = nullptr;

    CModelErrLog* pVThis = const_cast<CModelErrLog*>(this);

    pEntryView = const_cast<const SErrLogEntry*>(pVThis->getEntry(i_iRow));

    modelIdx = createIndex(i_iRow, i_iClm, const_cast<SErrLogEntry*>(pEntryView));

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
            case EColumnRowIdx:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Row";
                }
                break;
            }
            case EColumnSeverityRowIdx:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Severity\nRow Idx";
                }
                break;
            }
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
void CModelErrLog::fillRoleNames()
//------------------------------------------------------------------------------
{
    m_roleNames = QAbstractItemModel::roleNames();

    m_roleNames[static_cast<int>(ERole::Sort)] = "sort";
    m_roleNames[static_cast<int>(ERole::ImageUrl)] = "imageUrl";
    m_roleNames[static_cast<int>(ERole::Type)] = "type";

    if( m_bUsedByQmlListModels )
    {
        for( int clm = 0; clm < CModelErrLog::EColumnCount; ++clm)
        {
            int role = CModelErrLog::column2Role(static_cast<CModelErrLog::EColumn>(clm));
            m_roleNames[role] = s_clm2Name[clm];
        }
    }
}
