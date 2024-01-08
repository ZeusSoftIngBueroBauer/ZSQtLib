/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
                         Gewerbepark 28
                         D-83670 Bad Heilbrunn
                         Tel: 0049 8046 9488
                         www.zeussoft.de
                         E-Mail: mailbox@zeussoft.de

--------------------------------------------------------------------------------

Content: This file is part of the ZSQtLib.

*******************************************************************************/

#include "ZSSysGUI/ZSSysTrcAdminObjEditFilterExpressionsModel.h"
#include "ZSSysGUI/ZSSysComboBoxItemDelegate.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysRefCountGuard.h"
#include "ZSSys/ZSSysTrcAdminObjIdxTree.h"
#include "ZSSys/ZSSysTrcAdminObj.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CModelTrcAdminObjEditFilterExpressions : public QAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
QString CModelTrcAdminObjEditFilterExpressions::column2Str(int i_clm)
//------------------------------------------------------------------------------
{
    static QHash<int, QByteArray> s_clm2Name {
        { EColumnSelected, "Selected"},
        { EColumnFilterExpressionType, "Type"},
        { EColumnFilterExpression, "Expression"},
        { EColumnError, "Error"}
    };

    QString str;
    if (i_clm >= 0 && i_clm < EColumnCount) {
        str = s_clm2Name.value(i_clm);
    }
    else {
        str = s_clm2Name.value(i_clm, "? (" + QByteArray::number(i_clm) + ")");
    }
    return str;
}

/*==============================================================================
protected: // type definitions and constants
==============================================================================*/

/* struct SFilterExpression
==============================================================================*/

//------------------------------------------------------------------------------
CModelTrcAdminObjEditFilterExpressions::SFilterExpression::SFilterExpression() :
//------------------------------------------------------------------------------
    m_bSelected(false),
    m_filterType(EMethodTraceFilterExpressionType::Include),
    m_strFilter(),
    m_errResultInfo()
{
}

//------------------------------------------------------------------------------
CModelTrcAdminObjEditFilterExpressions::SFilterExpression::SFilterExpression(
    EMethodTraceFilterExpressionType i_filterType, const QString& i_strFilter) :
//------------------------------------------------------------------------------
    m_bSelected(false),
    m_filterType(i_filterType),
    m_strFilter(i_strFilter),
    m_errResultInfo()
{
}

//------------------------------------------------------------------------------
bool CModelTrcAdminObjEditFilterExpressions::SFilterExpression::operator == (const SFilterExpression& i_other) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;
    if (m_bSelected != i_other.m_bSelected) {
        bEqual = false;
    }
    else if (m_filterType != i_other.m_filterType) {
        bEqual = false;
    }
    else if (m_strFilter != i_other.m_strFilter) {
        bEqual = false;
    }
    else if (m_errResultInfo != i_other.m_errResultInfo) {
        bEqual = false;
    }
    return bEqual;
}

//------------------------------------------------------------------------------
bool CModelTrcAdminObjEditFilterExpressions::SFilterExpression::operator != (const SFilterExpression& i_other) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelTrcAdminObjEditFilterExpressions::CModelTrcAdminObjEditFilterExpressions(
    CIdxTreeTrcAdminObjs* i_pIdxTree, QObject* i_pObjParent) :
//------------------------------------------------------------------------------
    QAbstractTableModel(i_pObjParent),
    m_pIdxTree(i_pIdxTree),
    m_strKeyInTree(),
    m_eFilter(EMethodTraceFilterProperty::Undefined),
    m_arFilterExpressions(),
    m_bContentChanged(false),
    m_iContentChangedSignalBlockedCounter(0)
{
    setObjectName("TrcAdminObjEditFilterExpression");
}

//------------------------------------------------------------------------------
CModelTrcAdminObjEditFilterExpressions::~CModelTrcAdminObjEditFilterExpressions()
//------------------------------------------------------------------------------
{
    m_pIdxTree = nullptr;
    //m_strKeyInTree;
    m_eFilter = static_cast<EMethodTraceFilterProperty>(0);
    //m_arFilterExpressions;
    m_bContentChanged = false;
    m_iContentChangedSignalBlockedCounter = 0;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelTrcAdminObjEditFilterExpressions::setKeyEntryToEdit(
    const QString& i_strKeyInTree, EMethodTraceFilterProperty i_filter)
//------------------------------------------------------------------------------
{
    if (m_strKeyInTree != i_strKeyInTree || m_eFilter != i_filter) {
        disconnectFromTraceAdminObjFilterChangedSignal();
        m_strKeyInTree = i_strKeyInTree;
        m_eFilter = i_filter;
        fillModel();
        connectWithTraceAdminObjFilterChangedSignal();
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Checks whether the user has entered meaningful data.

    @return true, if at least one data row has an erronous setting that
            cannot be applied, false otherwise.
*/
bool CModelTrcAdminObjEditFilterExpressions::hasErrors() const
//------------------------------------------------------------------------------
{
    bool bHasErrors = false;
    for (const SFilterExpression& filterExpression : m_arFilterExpressions) {
        if (isErrorResult(filterExpression.m_errResultInfo.getResult())) {
            bHasErrors = true;
            break;
        }
    }
    return bHasErrors;
}

//------------------------------------------------------------------------------
/*! @brief Checks whether the user has changed settings which haven't been
           applied yet to the trace admin object.

    @return true, if at least one data row has a different setting than the
            trace admin object which need to be applied to take effect.
            false, if the settings shown are the current settings of the
            trace admin object.
*/
bool CModelTrcAdminObjEditFilterExpressions::hasChanges() const
//------------------------------------------------------------------------------
{
    bool bHasChanges = false;
    CIdxTreeEntry* pTreeEntry = m_pIdxTree->findEntry(m_strKeyInTree);
    if (pTreeEntry != nullptr) {
        if (pTreeEntry->isLeave()) {
            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);
            if (pTrcAdminObj != nullptr) {
                QList<SFilterExpression> arFilterExpressions = getFilterExpressions(pTrcAdminObj);
                bHasChanges = (arFilterExpressions != m_arFilterExpressions);
            }
        }
        else {
            bHasChanges = (m_arFilterExpressions.size() > 0);
        }
    }
    return bHasChanges;
}

//------------------------------------------------------------------------------
/*! @brief This method is called to apply the settings from the model at the
           trace admin object.

    The values from the model are applied one after another at the trace admin object.

    For each changed property the method "onTrcAdminObjChanged" is called as an reentry.
    "onTrcAdminObjChanged" is also used to fill the model with the current values of the
    trace admin object if a new trace admin object has been set or if the settings got to be reset.

    To avoid that "onTrcAdminObjChanged" overwrites settings in the model which haven't been
    applied yet the m_iContentChangedSignalBlockedCounter is incremented before applying
    the changes from the model.
*/
void CModelTrcAdminObjEditFilterExpressions::acceptChanges()
//------------------------------------------------------------------------------
{
    bool bHasChanges = false;
    CIdxTreeEntry* pTreeEntry = m_pIdxTree->findEntry(m_strKeyInTree);
    if (pTreeEntry != nullptr) {
        if (pTreeEntry->isLeave()) {
            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);
            if (pTrcAdminObj != nullptr && !hasErrors()) {
                CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
                QString strFilter = toFilterExpression(m_arFilterExpressions);
                if (m_eFilter == EMethodTraceFilterProperty::ObjectName) {
                    pTrcAdminObj->setObjectNameFilter(strFilter);
                }
                else if (m_eFilter == EMethodTraceFilterProperty::MethodName) {
                    pTrcAdminObj->setMethodNameFilter(strFilter);
                }
                else if (m_eFilter == EMethodTraceFilterProperty::TraceData) {
                    pTrcAdminObj->setTraceDataFilter(strFilter);
                }
            }
        }
        else {
            CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
            QString strFilter = toFilterExpression(m_arFilterExpressions);
            if (m_eFilter == EMethodTraceFilterProperty::ObjectName) {
                m_pIdxTree->setObjectNameFilter(pTreeEntry, strFilter);
            }
            else if (m_eFilter == EMethodTraceFilterProperty::MethodName) {
                m_pIdxTree->setMethodNameFilter(pTreeEntry, strFilter);
            }
            else if (m_eFilter == EMethodTraceFilterProperty::TraceData) {
                m_pIdxTree->setTraceDataFilter(pTreeEntry, strFilter);
            }
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Resets the current settings of the edit controls to the current values
           of the trace admin object.

    Resetting is done be invoking "onTrcAdminObjChanged", which fills the edit
    controls with the current property values of the trace admin object.
*/
void CModelTrcAdminObjEditFilterExpressions::rejectChanges()
//------------------------------------------------------------------------------
{

    // "fillModel" is used to fill the model with the current settings of the
    // trace admin object. To avoid that the signal "contentChanged" is emitted
    // for each value of the trace admin object set in the model the
    // ContentChangedSignalBlockedCounter is incremented. After fillModel has
    // been executed the contentChanged flag is checked and the contentChanged
    // signal is emitted if necessary.

    {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

        // Fill the content of the edit controls.
        fillModel();
    }

    // If the "contentChanged" signal is no longer blocked and the content of
    // properties widget has been changed ...
    if (m_iContentChangedSignalBlockedCounter == 0 && m_bContentChanged) {
        // .. emit the contentChanged signal and update the enabled state
        // of the Apply and Reset buttons.
        emit contentChanged();
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief 
*/
void CModelTrcAdminObjEditFilterExpressions::addFilterExpression()
//------------------------------------------------------------------------------
{
    int iRow = rowCount();
    beginInsertRows(QModelIndex(), iRow, iRow);
    m_arFilterExpressions.append(SFilterExpression());
    endInsertRows();
    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        emit contentChanged();
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns a list with the current names of the selected labels.
*/
QList<int> CModelTrcAdminObjEditFilterExpressions::selectedRows() const
//------------------------------------------------------------------------------
{
    QList<int> ariSelectedRows;
    for (int iRow = 0; iRow < rowCount(); ++iRow) {
        const SFilterExpression& filterExpression = m_arFilterExpressions[iRow];
        if (filterExpression.m_bSelected) {
            ariSelectedRows.append(iRow);
        }
    }
    return ariSelectedRows;
}

//------------------------------------------------------------------------------
/*! @brief Removes all selected labels from the model.

    @note The method does not implicitely call removeLabel to remove a single
          label on purpose so that the contentChanged signal is emitted just once.
*/
void CModelTrcAdminObjEditFilterExpressions::removeSelectedRows()
//------------------------------------------------------------------------------
{
    QList<int> ariSelectedRows = selectedRows();
    if (ariSelectedRows.size() > 0) {
        for (int iRow = ariSelectedRows.size()-1; iRow >= 0; --iRow) {
            beginRemoveRows(QModelIndex(), iRow, iRow);
            m_arFilterExpressions.removeAt(iRow);
            endRemoveRows();
        }
    }
}

/*==============================================================================
public: // must overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelTrcAdminObjEditFilterExpressions::rowCount(const QModelIndex& i_modelIdxParent) const
//------------------------------------------------------------------------------
{
    return m_arFilterExpressions.size();
}

//------------------------------------------------------------------------------
int CModelTrcAdminObjEditFilterExpressions::columnCount(const QModelIndex& i_modelIdxParent) const
//------------------------------------------------------------------------------
{
    return EColumnCount;
}

//------------------------------------------------------------------------------
QVariant CModelTrcAdminObjEditFilterExpressions::data(const QModelIndex& i_modelIdx, int i_iRole) const
//------------------------------------------------------------------------------
{
    QVariant varData;
    if (i_modelIdx.isValid()) {
        int iRow = i_modelIdx.row();
        int iClm = i_modelIdx.column();
        if ((iRow >= 0) && (iRow < rowCount())) {
            const SFilterExpression& filterExpression = m_arFilterExpressions[iRow];
            switch (iClm) {
                case EColumnSelected: {
                    if (i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole) {
                        varData = filterExpression.m_bSelected;
                    }
                    else if (i_iRole == Qt::CheckStateRole) {
                        varData = filterExpression.m_bSelected ? Qt::Checked : Qt::Unchecked;
                    }
                    else if (i_iRole == Qt::TextAlignmentRole) {
                        varData = static_cast<int>(Qt::AlignHCenter | Qt::AlignVCenter);
                    }
                    break;
                }
                case EColumnFilterExpressionType: {
                    if (i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole) {
                        varData = CEnumMethodTraceFilterExpressionType(filterExpression.m_filterType).toString();
                    }
                    else if (i_iRole == Qt::AccessibleTextRole) {
                        QList<SComboBoxItem> arItems;
                        for (CEnumMethodTraceFilterExpressionType eType = 0; eType < CEnumMethodTraceFilterExpressionType::count(); ++eType) {
                            if (eType != EMethodTraceFilterExpressionType::Undefined) {
                                arItems.append(SComboBoxItem(eType.toString()));
                            }
                        }
                        varData.setValue(arItems);
                    }
                    break;
                }
                case EColumnFilterExpression: {
                    if (i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole) {
                        varData = filterExpression.m_strFilter;
                    }
                    break;
                }
                case EColumnError: {
                    if (i_iRole == Qt::DecorationRole) {
                        if (isErrorResult(filterExpression.m_errResultInfo.getResult())) {
                            varData = getErrIcon(filterExpression.m_errResultInfo.getSeverity());
                        }
                    }
                    else if (i_iRole == Qt::ToolTipRole) {
                        QString strResult = filterExpression.m_errResultInfo.getResultStr();
                        QString strAddInfo = filterExpression.m_errResultInfo.getAddErrInfoDscr();
                        if (!strResult.isEmpty() && !strAddInfo.isEmpty()) {
                            varData = strResult + " (" + strAddInfo + ")";
                        }
                        else if (!strResult.isEmpty()) {
                            varData = strResult;
                        }
                        else if (!strAddInfo.isEmpty()) {
                            varData = strAddInfo;
                        }
                    }
                    break;
                }
                default: {
                    break;
                }
            }
        }
    }
    return varData;
}

//------------------------------------------------------------------------------
bool CModelTrcAdminObjEditFilterExpressions::setData(
    const QModelIndex& i_modelIdx, const QVariant& i_varData, int i_iRole)
//------------------------------------------------------------------------------
{
    bool bDataSet = false;

    if (i_modelIdx.isValid()) {
        int iRow = i_modelIdx.row();
        int iClm = i_modelIdx.column();
        if ((iRow >= 0) && (iRow < rowCount())) {
            SFilterExpression filterExpression = m_arFilterExpressions[iRow];
            switch (iClm) {
                case EColumnSelected: {
                    if (i_iRole == Qt::EditRole) {
                        filterExpression.m_bSelected = i_varData.toBool();
                        bDataSet = true;
                    }
                    break;
                }
                case EColumnFilterExpressionType: {
                    if (i_iRole == Qt::EditRole) {
                        QString strData = i_varData.toString();
                        bool bOk = false;
                        CEnumMethodTraceFilterExpressionType eType = CEnumMethodTraceFilterExpressionType::fromString(strData, &bOk);
                        if (bOk) {
                            filterExpression.m_filterType = eType.enumerator();
                        }
                        bDataSet = true;
                    }
                    break;
                }
                case EColumnFilterExpression: {
                    if (i_iRole == Qt::EditRole) {
                        QString strFilterNew = i_varData.toString();
                        if (filterExpression.m_strFilter != strFilterNew) {
                            filterExpression.m_errResultInfo = ErrResultSuccess;
                            #pragma message(__TODO__"Check filter expression")
                            filterExpression.m_strFilter = strFilterNew;
                        }
                        bDataSet = true;
                    }
                    break;
                }
                case EColumnError: {
                    break;
                }
                default: {
                    break;
                }
            }
            if (m_arFilterExpressions[iRow] != filterExpression) {
                m_arFilterExpressions[iRow] = filterExpression;
                if (m_iContentChangedSignalBlockedCounter > 0) {
                    m_bContentChanged = true;
                }
                else {
                    emit contentChanged();
                }
            }
        }
    }
    return bDataSet;
}

//------------------------------------------------------------------------------
QVariant CModelTrcAdminObjEditFilterExpressions::headerData(int i_iSection, Qt::Orientation i_orientation, int i_iRole) const
//------------------------------------------------------------------------------
{
    QVariant varData;
    if (i_orientation == Qt::Horizontal && i_iRole == Qt::DisplayRole) {
        varData = column2Str(i_iSection);
    }
    return varData;
}

//------------------------------------------------------------------------------
Qt::ItemFlags CModelTrcAdminObjEditFilterExpressions::flags(const QModelIndex& i_modelIdx) const
//------------------------------------------------------------------------------
{
    // The base class implementation returns a combination of flags that enables
    // the item (ItemIsEnabled) and allows it to be selected (ItemIsSelectable).
    Qt::ItemFlags uFlags = uFlags = QAbstractItemModel::flags(i_modelIdx);
    if (i_modelIdx.isValid()) {
        //CTrcAdminObj* pTrcAdminObj = nullptr;
        CIdxTreeEntry* pTreeEntry = m_pIdxTree->findEntry(m_strKeyInTree);
        if (pTreeEntry != nullptr) {
            //if (pTreeEntry->isLeave()) {
            //    pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);
            //}
        }
        //if (pTrcAdminObj != nullptr) {
        if (pTreeEntry != nullptr) {
            int iRow = i_modelIdx.row();
            int iClm = i_modelIdx.column();
            if ((iRow >= 0) && (iRow < rowCount())) {
                switch (i_modelIdx.column()) {
                    case EColumnSelected: {
                        uFlags = uFlags | Qt::ItemIsUserCheckable;
                        break;
                    }
                    case EColumnFilterExpressionType: {
                        uFlags = uFlags | Qt::ItemIsEditable;
                        break;
                    }
                    case EColumnFilterExpression: {
                        uFlags = uFlags | Qt::ItemIsEditable;
                        break;
                    }
                    default: {
                        break;
                    }
                }
            }
        }
    }
    return uFlags;
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CModelTrcAdminObjEditFilterExpressions::onTraceAdminObjFilterChanged(const QString& i_strFilter)
//------------------------------------------------------------------------------
{
    // When applying the changes from the model by invoking "applySettings"
    // the ContentChangedSignalBlockedCounter is incremented to avoid that the
    // "onGraphObj<Signal>" slots overwrite settings in the model which haven't been
    // applied yet.
    if (m_iContentChangedSignalBlockedCounter == 0)
    {
        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
            fillModel();
        }

        // If the "contentChanged" signal is no longer blocked and the content of
        // properties widget has been changed ...
        if (m_iContentChangedSignalBlockedCounter == 0 && m_bContentChanged) {
            // .. emit the contentChanged signal and update the enabled state
            // of the Apply and Reset buttons.
            emit contentChanged();
        }
    }
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelTrcAdminObjEditFilterExpressions::clearModel()
//------------------------------------------------------------------------------
{
    if (m_arFilterExpressions.size() > 0) {
        beginRemoveRows(QModelIndex(), 0, m_arFilterExpressions.size()-1);
        m_arFilterExpressions.clear();
        endRemoveRows();
        if (m_iContentChangedSignalBlockedCounter > 0) {
            m_bContentChanged = true;
        }
        else {
            emit contentChanged();
        }
    }
}

//------------------------------------------------------------------------------
void CModelTrcAdminObjEditFilterExpressions::fillModel()
//------------------------------------------------------------------------------
{
    if (m_arFilterExpressions.size() > 0) {
        clearModel();
    }
    CTrcAdminObj* pTrcAdminObj = nullptr;
    CIdxTreeEntry* pTreeEntry = m_pIdxTree->findEntry(m_strKeyInTree);
    if (pTreeEntry != nullptr) {
        if (pTreeEntry->isLeave()) {
            pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);
        }
    }
    if (pTrcAdminObj != nullptr) {
        m_arFilterExpressions = getFilterExpressions(pTrcAdminObj);
        if (m_arFilterExpressions.size() > 0) {
            beginInsertRows(QModelIndex(), 0, m_arFilterExpressions.size()-1);
            endInsertRows();
            if (m_iContentChangedSignalBlockedCounter > 0) {
                m_bContentChanged = true;
            }
            else {
                emit contentChanged();
            }
        }
    }
}

//------------------------------------------------------------------------------
void CModelTrcAdminObjEditFilterExpressions::connectWithTraceAdminObjFilterChangedSignal()
//------------------------------------------------------------------------------
{
    CTrcAdminObj* pTrcAdminObj = nullptr;
    CIdxTreeEntry* pTreeEntry = m_pIdxTree->findEntry(m_strKeyInTree);
    if (pTreeEntry != nullptr) {
        if (pTreeEntry->isLeave()) {
            pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);
        }
    }
    if (pTrcAdminObj != nullptr) {
        if (m_eFilter == EMethodTraceFilterProperty::ObjectName) {
            QObject::connect(
                pTrcAdminObj, &CTrcAdminObj::objectNameFilterChanged,
                this, &CModelTrcAdminObjEditFilterExpressions::onTraceAdminObjFilterChanged);
        }
        else if (m_eFilter == EMethodTraceFilterProperty::MethodName) {
            QObject::connect(
                pTrcAdminObj, &CTrcAdminObj::methodNameFilterChanged,
                this, &CModelTrcAdminObjEditFilterExpressions::onTraceAdminObjFilterChanged);
        }
        else if (m_eFilter == EMethodTraceFilterProperty::TraceData) {
            QObject::connect(
                pTrcAdminObj, &CTrcAdminObj::traceDataFilterChanged,
                this, &CModelTrcAdminObjEditFilterExpressions::onTraceAdminObjFilterChanged);
        }
    }
}

//------------------------------------------------------------------------------
void CModelTrcAdminObjEditFilterExpressions::disconnectFromTraceAdminObjFilterChangedSignal()
//------------------------------------------------------------------------------
{
    CTrcAdminObj* pTrcAdminObj = nullptr;
    CIdxTreeEntry* pTreeEntry = m_pIdxTree->findEntry(m_strKeyInTree);
    if (pTreeEntry != nullptr) {
        if (pTreeEntry->isLeave()) {
            pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);
        }
    }
    if (pTrcAdminObj != nullptr) {
        if (m_eFilter == EMethodTraceFilterProperty::ObjectName) {
            QObject::disconnect(
                pTrcAdminObj, &CTrcAdminObj::objectNameFilterChanged,
                this, &CModelTrcAdminObjEditFilterExpressions::onTraceAdminObjFilterChanged);
        }
        else if (m_eFilter == EMethodTraceFilterProperty::MethodName) {
            QObject::disconnect(
                pTrcAdminObj, &CTrcAdminObj::methodNameFilterChanged,
                this, &CModelTrcAdminObjEditFilterExpressions::onTraceAdminObjFilterChanged);
        }
        else if (m_eFilter == EMethodTraceFilterProperty::TraceData) {
            QObject::disconnect(
                pTrcAdminObj, &CTrcAdminObj::traceDataFilterChanged,
                this, &CModelTrcAdminObjEditFilterExpressions::onTraceAdminObjFilterChanged);
        }
    }
}

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QList<CModelTrcAdminObjEditFilterExpressions::SFilterExpression>
    CModelTrcAdminObjEditFilterExpressions::getFilterExpressions(CTrcAdminObj* i_pTrcAdminObj) const
//------------------------------------------------------------------------------
{
    QList<SFilterExpression> arFilterExpressions;
    QString strFilter;
    if (m_eFilter == EMethodTraceFilterProperty::ObjectName) {
        strFilter = i_pTrcAdminObj->getObjectNameFilter();
    }
    else if (m_eFilter == EMethodTraceFilterProperty::MethodName) {
        strFilter = i_pTrcAdminObj->getMethodNameFilter();
    }
    else if (m_eFilter == EMethodTraceFilterProperty::TraceData) {
        strFilter = i_pTrcAdminObj->getTraceDataFilter();
    }
    QStringList strlstInclude;
    QStringList strlstExclude;
    SErrResultInfo errResultInfo = splitMethodTraceFilterExpressionString(strFilter, strlstInclude, strlstExclude);
    if (!errResultInfo.isErrorResult()) {
        for (const QString& strExpr : strlstInclude) {
            arFilterExpressions.append(SFilterExpression(EMethodTraceFilterExpressionType::Include, strExpr));
        }
        for (const QString& strExpr : strlstExclude) {
            arFilterExpressions.append(SFilterExpression(EMethodTraceFilterExpressionType::Exclude, strExpr));
        }
    }
    return arFilterExpressions;
}

//------------------------------------------------------------------------------
QString CModelTrcAdminObjEditFilterExpressions::toFilterExpression(const QList<SFilterExpression>& i_arFilterExpressions) const
//------------------------------------------------------------------------------
{
    QStringList strlstInclude;
    QStringList strlstExclude;
    for (const SFilterExpression& filterExpr : i_arFilterExpressions) {
        if (!filterExpr.m_errResultInfo.isErrorResult()) {
            if (filterExpr.m_filterType == EMethodTraceFilterExpressionType::Include) {
                strlstInclude.append(filterExpr.m_strFilter);
            }
            else if (filterExpr.m_filterType == EMethodTraceFilterExpressionType::Exclude) {
                strlstExclude.append(filterExpr.m_strFilter);
            }
        }
    }
    return joinMethodTraceFilterExpressionStrings(strlstInclude, strlstExclude);
}
