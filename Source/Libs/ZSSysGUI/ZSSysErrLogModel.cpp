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

#include <stdlib.h>

#include <QtCore/qcoreapplication.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qdir.h>
#include <QtCore/qfile.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qsettings.h>
#include <QtCore/qtextstream.h>
#include <QtCore/qthread.h>
#include <QtCore/qtimer.h>

#include "ZSSysGUI/ZSSysErrLogModel.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTime.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CModelErrLog : public QAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelErrLog::CModelErrLog( CErrLog* i_pErrLog ) :
//------------------------------------------------------------------------------
    QAbstractItemModel(),
    //QAbstractTableModel(),
    m_pErrLog(i_pErrLog)
    //m_ararpEntries[EResultSeverityCount]
{
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
    int iSeverity;
    int iRowIdx;

    for( iSeverity = EResultSeverityCount-1; iSeverity >= 0; iSeverity-- )
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

    m_pErrLog = nullptr;

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

        qSort( modelIdxListSorted.begin(), modelIdxListSorted.end() );

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
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CModelErrLog::onEntryAdded( const ZS::System::SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
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
protected: // must overridables of base class QAbstractTableModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelErrLog::rowCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    int iRowCount = 0;

    int iSeverity;

    for( iSeverity = EResultSeverityCount-1; iSeverity >= 0; iSeverity-- )
    {
        iRowCount += m_ararpEntries[iSeverity].count();
    }

    return iRowCount;

} // rowCount

//------------------------------------------------------------------------------
int CModelErrLog::columnCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return EColumnCount;
}

//------------------------------------------------------------------------------
QVariant CModelErrLog::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    if( !i_modelIdx.isValid() )
    {
        return varData;
    }

    const SErrLogEntry* pEntryView = nullptr;

    int iRow = i_modelIdx.row();

    if( iRow < 0 || iRow >= rowCount() )
    {
        return varData;
    }

    CModelErrLog* pVThis = const_cast<CModelErrLog*>(this);

    pEntryView = const_cast<const SErrLogEntry*>(pVThis->getEntry(iRow));

    if( pEntryView == nullptr )
    {
        return varData;
    }

    if( i_iRole == Qt::ToolTipRole )
    {
        QString strTmp = pEntryView->m_strProposal;
        strTmp.replace("&lt;","<");
        strTmp.replace("&gt;",">");
        varData = strTmp;
    }
    else if( i_iRole == Qt::DecorationRole )
    {
        if( i_modelIdx.column() == EColumnSeverityIcon )
        {
            varData = getErrIcon(pEntryView->m_errResultInfo.getSeverity());
        }
    }
    else if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
    {
        switch( i_modelIdx.column() )
        {
            case EColumnRowIdx:
            {
                varData = pEntryView->m_iRowIdx;
                break;
            }
            case EColumnDate:
            {
                varData = pEntryView->m_dateTime.toString("yyyy-MM-dd");
                break;
            }
            case EColumnTime:
            {
                varData = pEntryView->m_dateTime.toString("hh:mm:ss");
                break;
            }
            case EColumnSeverity:
            {
                varData = resultSeverity2Str(pEntryView->m_errResultInfo.getSeverity());
                break;
            }
            case EColumnResult:
            {
                varData = QString::number(pEntryView->m_errResultInfo.getResult());
                break;
            }
            case EColumnResultStr:
            {
                varData = pEntryView->m_errResultInfo.getResultStr();
                break;
            }
            case EColumnOccurrences:
            {
                varData = pEntryView->m_iOccurrences;
                break;
            }
            case EColumnSource:
            {
                QString strTmp = pEntryView->m_errResultInfo.getErrSource().toString();
                strTmp.replace("&lt;","<");
                strTmp.replace("&gt;",">");
                varData = strTmp;
                break;
            }
            case EColumnAddInfo:
            {
                QString strTmp = pEntryView->m_errResultInfo.getAddErrInfoDscr();
                strTmp.replace("&lt;","<");
                strTmp.replace("&gt;",">");
                varData = strTmp;
                break;
            }
            case EColumnProposal:
            {
                QString strTmp = pEntryView->m_strProposal;
                strTmp.replace("&lt;","<");
                strTmp.replace("&gt;",">");
                varData = strTmp;
                break;
            }
            default:
            {
                break;
            }
        }
    } // if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )

    return varData;

} // data

//------------------------------------------------------------------------------
QModelIndex CModelErrLog::index( int i_iRow, int i_iCol, const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    QModelIndex modelIdx;

    const SErrLogEntry* pEntryView = nullptr;

    CModelErrLog* pVThis = const_cast<CModelErrLog*>(this);

    pEntryView = const_cast<const SErrLogEntry*>(pVThis->getEntry(i_iRow));

    modelIdx = createIndex(i_iRow, i_iCol, const_cast<SErrLogEntry*>(pEntryView));

    return modelIdx;

} // index

//------------------------------------------------------------------------------
QModelIndex CModelErrLog::parent( const QModelIndex& /*i_modelIdx*/ ) const
//------------------------------------------------------------------------------
{
    return QModelIndex();
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
            case EColumnSeverityIcon:
            {
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
            case EColumnSeverity:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Severity";
                }
                break;
            }
            case EColumnResult:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Result Nr.";
                }
                break;
            }
            case EColumnResultStr:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Result String";
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
    return varData;

} // headerData
