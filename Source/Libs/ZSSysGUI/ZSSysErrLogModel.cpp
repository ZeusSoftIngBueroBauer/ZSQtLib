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

#include "ZSSysGUI/ZSSysErrLogModel.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtGui/qguiapplication.h>
#include <QtGui/qfontmetrics.h>
#include <QtQml/qqmlapplicationengine.h>

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/******************************************************************************/
class CInitModuleSysErrLogModel
/* Please note:
   The class name should be unique for the whole system. Otherwise the compiler
   may be confused and using a CInitModule class from other modules to create
   the static InitModule instance.
*******************************************************************************/
{
public: // ctor
    CInitModuleSysErrLogModel()
    {
        qmlRegisterType<CModelErrLog>("ZSSysGUI", 1, 0, "ModelErrLog");
    }
};

static CInitModuleSysErrLogModel s_initModule;


/*******************************************************************************
class CModelErrLog : public QAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

QHash<int, QByteArray> CModelErrLog::s_roleNames;
QHash<QByteArray, int> CModelErrLog::s_roleValues;

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
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CModelErrLog::EColumn CModelErrLog::role2Column(int i_iRole)
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
QString CModelErrLog::role2Str(int i_iRole)
//------------------------------------------------------------------------------
{
    return s_roleNames.value(i_iRole, "? (" + QByteArray::number(i_iRole) + ")");
}

//------------------------------------------------------------------------------
int CModelErrLog::byteArr2Role(const QByteArray& i_byteArrRole)
//------------------------------------------------------------------------------
{
    return s_roleValues.value(i_byteArrRole, Qt::DisplayRole);
}

//------------------------------------------------------------------------------
int CModelErrLog::column2Role(EColumn i_clm)
//------------------------------------------------------------------------------
{
    return static_cast<int>(ERole::FirstDataColumnRole) + i_clm;
}

//------------------------------------------------------------------------------
QString CModelErrLog::modelIdx2Str(
    const QModelIndex& i_modelIdx,
    int i_iRole,
    bool i_bIncludeId )
//------------------------------------------------------------------------------
{
    QString str;

    if( !i_modelIdx.isValid() ) {
        str = "Invalid";
    } else {
        str = "Row: " + QString::number(i_modelIdx.row());
        if( i_iRole >= static_cast<int>(CModelErrLog::ERole::FirstDataColumnRole) ) {
            str += ", Clm: " + QString::number(i_modelIdx.column());
        } else if ((i_modelIdx.column() >= 0) && (i_modelIdx.column() < EColumnCount)) {
            str += ", Clm: " + column2Str(static_cast<EColumn>(i_modelIdx.column()));
        } else {
            str += ", Clm: " + QString::number(i_modelIdx.column());
        }
        if( i_bIncludeId ) {
            str += ", Id: " + QString::number(i_modelIdx.internalId()) + " (" + pointer2Str(i_modelIdx.internalPointer()) + ")";
        }
    }
    return str;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelErrLog::CModelErrLog( QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    CModelErrLog(nullptr, i_pObjParent)
{
} // ctor

//------------------------------------------------------------------------------
CModelErrLog::CModelErrLog( CErrLog* i_pErrLog, QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    QAbstractItemModel(),
    m_pErrLog(nullptr),
    m_ararpEntries(QVector<QList<SErrLogEntry*>>(EResultSeverityCount)),
    m_ariClmWidths(QVector<int>(EColumnCount)),
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjNoisyMethods(nullptr)
{
    fillRoleNames();

    setObjectName( QString(i_pErrLog == nullptr ? "ZSErrLog" : i_pErrLog->objectName()) );

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName(), objectName());
    m_pTrcAdminObjNoisyMethods = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::NoisyMethods", objectName());

    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ErrLog: " + i_pErrLog->objectName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strMthInArgs */ strMthInArgs );

    if( i_pErrLog != nullptr )
    {
        setErrLog(i_pErrLog);
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

    for( int iSeverity = m_ararpEntries.count()-1; iSeverity >= 0; iSeverity-- )
    {
        if( m_ararpEntries[iSeverity].count() > 0 )
        {
            beginRemoveRows( QModelIndex(), 0, m_ararpEntries[iSeverity].count()-1 );

            for( int iRowIdx = m_ararpEntries[iSeverity].count()-1; iRowIdx >= 0; iRowIdx-- )
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

//------------------------------------------------------------------------------
/*! Sets the index tree whose content should be exposed by the model to views.

    @param i_pIdxTree
        Pointer to index tree to be used by model.
        The argument is of type QObject so that it can also be invoked by QML.
*/
void CModelErrLog::setErrLog( QObject* i_pErrLog )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        if( m_pErrLog == nullptr ) {
            strMthInArgs = "ErrLog: " + QString(i_pErrLog == nullptr ? "nullptr" : i_pErrLog->objectName());
        } else {
            strMthInArgs  = "ErrLogOrig: " + QString(m_pErrLog == nullptr ? "nullptr" : m_pErrLog->objectName());
            strMthInArgs += ", ErrLogNew: " + QString(i_pErrLog == nullptr ? "nullptr" : i_pErrLog->objectName());
        }
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setErrLog",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_pErrLog != i_pErrLog )
    {
        if( m_pErrLog != nullptr )
        {
            QObject::disconnect(
                m_pErrLog, &CErrLog::entryAdded,
                this, &CModelErrLog::onEntryAdded);
            QObject::disconnect(
                m_pErrLog, &CErrLog::entryChanged,
                this, &CModelErrLog::onEntryChanged);
            QObject::disconnect(
                m_pErrLog, &CErrLog::entryRemoved,
                this, &CModelErrLog::onEntryRemoved);

            for( int iSeverity = m_ararpEntries.count()-1; iSeverity >= 0; iSeverity-- )
            {
                if( m_ararpEntries[iSeverity].count() > 0 )
                {
                    beginRemoveRows( QModelIndex(), 0, m_ararpEntries[iSeverity].count()-1 );

                    for( int iRowIdx = m_ararpEntries[iSeverity].count()-1; iRowIdx >= 0; iRowIdx-- )
                    {
                        delete m_ararpEntries[iSeverity][iRowIdx];
                        m_ararpEntries[iSeverity][iRowIdx] = nullptr;
                    }
                    endRemoveRows();
                }
            }
        }

        m_pErrLog = dynamic_cast<CErrLog*>(i_pErrLog);

        if( m_pErrLog == nullptr )
        {
            setObjectName("ZSErrLog");
        }
        else
        {
            CErrLogLocker errLogLocker(m_pErrLog);

            setObjectName(m_pErrLog->objectName());

            SErrLogEntry* pErrLogEntry = nullptr;
            int           iRowIdx;

            for( iRowIdx = 0; iRowIdx < m_pErrLog->getEntryCount(); iRowIdx++ )
            {
                pErrLogEntry = m_pErrLog->getEntry(iRowIdx);

                onEntryAdded(pErrLogEntry->m_errResultInfo);
            }

            QObject::connect(
                m_pErrLog, &CErrLog::entryAdded,
                this, &CModelErrLog::onEntryAdded);
            QObject::connect(
                m_pErrLog, &CErrLog::entryChanged,
                this, &CModelErrLog::onEntryChanged);
            QObject::connect(
                m_pErrLog, &CErrLog::entryRemoved,
                this, &CModelErrLog::onEntryRemoved);
        }

        emit errLogChanged(m_pErrLog);
    }
} // setErrLog

//------------------------------------------------------------------------------
/*! Returns the pointer to the error log instance (which might be null).

    @param Pointer to error log instance. The type is of QObject so that
           it can also be accessed by QML.

    @note If you access the error log instance and its entries you must lock
          and unlock the error log instance.
*/
QObject* CModelErrLog::errLog()
//------------------------------------------------------------------------------
{
    return m_pErrLog;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the number of error log entries for the given severity.

    @param i_strSeverity [in]
        Severity for which the number of log entries should be returned.
        By passing Undefined or Count for the severity the overall number of
        entries will be returned.

    @return Number of error log entries for the desired severity.

    @note If you use this method to loop through the error log entries don't forget
          to lock the instance before and unlocking it again afterwards.
*/
int CModelErrLog::getEntryCount( const QString& i_strSeverity ) const
//------------------------------------------------------------------------------
{
    int iSeverity = str2ResultSeverity(i_strSeverity);
    int iCount = 0;

    int iSeverityMin = iSeverity;
    int iSeverityMax = iSeverity;

    if( iSeverity == EResultSeverityUndefined || iSeverity == EResultSeverityCount )
    {
        iSeverityMin = 0;
        iSeverityMax = EResultSeverityCount-1;
    }

    for( iSeverity = iSeverityMin; iSeverity <= iSeverityMax; iSeverity++ )
    {
        iCount += m_ararpEntries[iSeverity].count();
    }

    return iCount;
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

    if( rowCount() > 0 )
    {
        CErrLogLocker errLogLocker(m_pErrLog);

        beginResetModel();

        for( int iSeverity = m_ararpEntries.count()-1; iSeverity >= 0; iSeverity-- )
        {
            if( m_ararpEntries[iSeverity].count() > 0 )
            {
                for( int iRowIdx = m_ararpEntries[iSeverity].count()-1; iRowIdx >= 0; iRowIdx-- )
                {
                    delete m_ararpEntries[iSeverity][iRowIdx];
                    m_ararpEntries[iSeverity][iRowIdx] = nullptr;
                }
                m_ararpEntries[iSeverity].clear();
            }
        }

        QObject::disconnect(
            m_pErrLog, &CErrLog::entryRemoved,
            this, &CModelErrLog::onEntryRemoved);

        m_pErrLog->clear();

        QObject::connect(
            m_pErrLog, &CErrLog::entryRemoved,
            this, &CModelErrLog::onEntryRemoved);

        endResetModel();

        emit countChanged();
    }
} // clear

//------------------------------------------------------------------------------
void CModelErrLog::removeEntries( const QModelIndexList& i_modelIdxList )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdxList[" + QString::number(i_modelIdxList.count()) + "]";
        if( i_modelIdxList.count() > 0 ) {
            strMthInArgs += "(";
            for( const QModelIndex& modelIdx : i_modelIdxList ) {
                if( !strMthInArgs.endsWith("(")) {
                    strMthInArgs += ", ";
                }
                strMthInArgs += "{" + modelIdx2Str(modelIdx) + "}";
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

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
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

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
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
            m_pErrLog, &CErrLog::entryRemoved,
            this, &CModelErrLog::onEntryRemoved);

        m_pErrLog->removeEntry(errResultInfo);

        QObject::connect(
            m_pErrLog, &CErrLog::entryRemoved,
            this, &CModelErrLog::onEntryRemoved);
        emit countChanged();
    }
} // removeEntry

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CModelErrLog::onEntryAdded( const ZS::System::SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
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

        emit countChanged();
    }
} // onEntryAdded

//------------------------------------------------------------------------------
void CModelErrLog::onEntryChanged( const ZS::System::SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
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

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
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

        emit countChanged();
    }
} // onEntryRemoved

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CModelErrLog::columnWidthByColumn(int i_iClm, int i_iFontPixelSize) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
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
        iClmWidth = m_ariClmWidths[i_iClm];

        if( iClmWidth == 0)
        {
            QFont fnt = QGuiApplication::font();
            if( i_iFontPixelSize > 0) {
                fnt.setPixelSize(i_iFontPixelSize);
            }

            QFontMetrics fntMetrics = QFontMetrics(fnt);
            QString strClmHeader = headerData(i_iClm, Qt::Horizontal).toString();
            #if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
            iClmWidth = fntMetrics.horizontalAdvance(strClmHeader + QLatin1String(" ^")) + 8;
            #else
            iClmWidth = fntMetrics.boundingRect(strClmHeader + QLatin1String(" ^")).width() + 8;
            #endif

            for( int iRowIdx = 0; iRowIdx < rowCount(); ++iRowIdx )
            {
                QString strType = "string";
                QVariant varData = data(index(iRowIdx, i_iClm), static_cast<int>(ERole::Type));
                #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                if( varData.canConvert(QVariant::String) )
                #else
                // static_cast to avoid deprecation warning
                if( varData.canConvert(static_cast<QMetaType>(QMetaType::QString)) )
                #endif
                {
                    strType = varData.toString();
                }
                if( strType == "string" || strType == "int" )
                {
                    varData = data(index(iRowIdx, i_iClm), Qt::DisplayRole);
                    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    if( varData.canConvert(QVariant::String) )
                    #else
                    // static_cast to avoid deprecation warning
                    if( varData.canConvert(static_cast<QMetaType>(QMetaType::QString)) )
                    #endif
                    {
                        QString strCellData = varData.toString();
                        #if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
                        iClmWidth = qMax(iClmWidth, fntMetrics.horizontalAdvance(strCellData));
                        #else
                        iClmWidth = qMax(iClmWidth, fntMetrics.boundingRect(strCellData).width());
                        #endif
                    }
                }
                else if( strType == "imageUrl" || strType == "icon" )
                {
                    varData = data(index(iRowIdx, i_iClm), Qt::DisplayRole);
                    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    if( varData.canConvert(QVariant::String) )
                    #else
                    // static_cast to avoid deprecation warning
                    if( varData.canConvert(static_cast<QMetaType>(QMetaType::QString)) )
                    #endif
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

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
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

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
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
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
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
    return s_roleNames;
}

/*==============================================================================
protected: // must overridables of base class QAbstractTableModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelErrLog::rowCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Parent {" + modelIdx2Str(i_modelIdxParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjNoisyMethods,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "rowCount",
        /* strMthInArgs */ strMthInArgs );

    int iRowCount = 0;

    for( int iSeverity = 0; iSeverity < m_ararpEntries.count(); ++iSeverity )
    {
        iRowCount += m_ararpEntries[iSeverity].count();
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
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

    if( areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Parent {" + modelIdx2Str(i_modelIdxParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjNoisyMethods,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "columnCount",
        /* strMthInArgs */ strMthInArgs );

    int iClmCount = EColumnCount;

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
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

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdx {" + modelIdx2Str(i_modelIdx) + "}";
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
        if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
        {
            mthTracer.setMethodReturn(varData.toString());
        }
        return varData;
    }

    int iRow = i_modelIdx.row();

    if( iRow < 0 || iRow >= rowCount() )
    {
        if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
        {
            mthTracer.setMethodReturn(varData.toString());
        }
        return varData;
    }

    const SErrLogEntry* pModelEntry = getEntry(iRow);

    if( pModelEntry == nullptr )
    {
        if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
        {
            mthTracer.setMethodReturn(varData.toString());
        }
        return varData;
    }

    if( iRole == Qt::ToolTipRole )
    {
        QString strTmp = pModelEntry->m_strProposal;
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
                    QString strUrl = getErrImageUrl(pModelEntry->m_errResultInfo.getSeverity());
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
                    varData = getErrIcon(pModelEntry->m_errResultInfo.getSeverity());
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
                    varData = resultSeverity2Str(pModelEntry->m_errResultInfo.getSeverity());
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
                    varData = QString::number(pModelEntry->m_errResultInfo.getResult());
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
                    varData = pModelEntry->m_errResultInfo.getResultStr();
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
                    varData = pModelEntry->m_dateTime.toString("yyyy-MM-dd");
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
                    varData = pModelEntry->m_dateTime.toString("hh:mm:ss");
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
                    varData = pModelEntry->m_iOccurrences;
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
                    QString strTmp = pModelEntry->m_errResultInfo.getErrSource().toString();
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
                    QString strTmp = pModelEntry->m_errResultInfo.getAddErrInfoDscr();
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
                    QString strTmp = pModelEntry->m_strProposal;
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

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
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

    if( areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs  = "Row: " + QString::number(i_iRow);
        strMthInArgs += ", Clm: " + column2Str(static_cast<EColumn>(i_iClm));
        strMthInArgs += ", Parent {" + modelIdx2Str(i_modelIdxParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjNoisyMethods,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "index",
        /* strMthInArgs */ strMthInArgs );

    const SErrLogEntry* pModelEntry = getEntry(i_iRow);

    QModelIndex modelIdx = createIndex(i_iRow, i_iClm, const_cast<SErrLogEntry*>(pModelEntry));

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
    {
        mthTracer.setMethodReturn(modelIdx2Str(modelIdx));
    }

    return modelIdx;

} // index

//------------------------------------------------------------------------------
QModelIndex CModelErrLog::parent( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "{" + modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjNoisyMethods,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "parent",
        /* strMthInArgs */ strMthInArgs );

    QModelIndex modelIdx;

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
    {
        mthTracer.setMethodReturn(modelIdx2Str(modelIdx));
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

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
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

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
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
    if( s_roleNames.isEmpty() )
    {
        s_roleNames = QAbstractItemModel::roleNames();

        s_roleNames[static_cast<int>(ERole::Sort)] = "sort";
        s_roleNames[static_cast<int>(ERole::ImageUrl)] = "imageUrl";
        s_roleNames[static_cast<int>(ERole::Type)] = "type";

        for( int clm = 0; clm < EColumnCount; ++clm)
        {
            int role = column2Role(static_cast<EColumn>(clm));
            s_roleNames[role] = s_clm2Name[clm];
        }

        for( int iRole : s_roleNames.keys() )
        {
            const QByteArray& byteArrRole = s_roleNames.value(iRole);
            s_roleValues[byteArrRole] = iRole;
        }
    }
}
