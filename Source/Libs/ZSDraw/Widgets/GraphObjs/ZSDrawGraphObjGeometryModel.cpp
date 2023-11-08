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

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjGeometryModel.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSSysGUI/ZSSysComboBoxItemDelegate.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysRefCountGuard.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtCore/qset.h>

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::PhysVal;
using namespace ZS::Draw;


/*******************************************************************************
class ZSDRAWDLL_API CModelGraphObjGeometry : publicQAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
QString CModelGraphObjGeometry::column2Str(int i_clm)
//------------------------------------------------------------------------------
{
    static QHash<int, QByteArray> s_clm2Name {
        { EColumnName, "Name"},
        { EColumnXVal, "XVal"},
        { EColumnYVal, "YVal"},
        { EColumnShowVals, "ShowVals"},
        { EColumnShowLine, "ShowLine"}
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

//------------------------------------------------------------------------------
/*! @brief Fills the label struct with the information retrieved from the graphical object.
*/
CModelGraphObjGeometry::SDataRow CModelGraphObjGeometry::SDataRow::fromGraphObj(
    CGraphObj* i_pGraphObj, const QString& i_strValueName, int i_iRowIdx)
//------------------------------------------------------------------------------
{
    SDataRow dataRow;
    dataRow.m_strValueName = i_strValueName;
    dataRow.m_iRowIdx = i_iRowIdx;
    dataRow.m_physValX = i_pGraphObj->getXValue(i_strValueName);
    dataRow.m_physValY = i_pGraphObj->getYValue(i_strValueName);
    dataRow.m_bVisible = i_pGraphObj->isValueLabelAnchorLineVisible(i_strValueName);
    dataRow.m_bLineVisible = i_pGraphObj->isValueLabelAnchorLineVisible(i_strValueName);
    return dataRow;
}

//------------------------------------------------------------------------------
CModelGraphObjGeometry::SDataRow::SDataRow() :
//------------------------------------------------------------------------------
    m_strValueName(), m_iRowIdx(-1),
    m_physValX(), m_physValY(),
    m_bVisible(false), m_bLineVisible(false)
{
}

//------------------------------------------------------------------------------
CModelGraphObjGeometry::SDataRow::SDataRow(
    const QString& i_strValueName, int i_iRowIdx,
    const CPhysVal& i_physValX, const CPhysVal& i_physValY,
    bool i_bVisible, bool i_bLineVisible) :
//------------------------------------------------------------------------------
    m_strValueName(), m_iRowIdx(-1),
    m_physValX(), m_physValY(),
    m_bVisible(false), m_bLineVisible(false)
{
}

//------------------------------------------------------------------------------
bool CModelGraphObjGeometry::SDataRow::operator == (const SDataRow& i_other) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;
    if (m_strValueName != i_other.m_strValueName) {
        bEqual = false;
    }
    else if (m_iRowIdx != i_other.m_iRowIdx) {
        bEqual = false;
    }
    else if (m_physValX != i_other.m_physValX) {
        bEqual = false;
    }
    else if (m_physValY != i_other.m_physValY) {
        bEqual = false;
    }
    else if (m_bVisible != i_other.m_bVisible) {
        bEqual = false;
    }
    else if (m_bLineVisible != i_other.m_bLineVisible) {
        bEqual = false;
    }
    return bEqual;
}

//------------------------------------------------------------------------------
bool CModelGraphObjGeometry::SDataRow::operator != (const SDataRow& i_other) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelGraphObjGeometry::CModelGraphObjGeometry(
    CDrawingScene* i_pDrawingScene,
    const QString& i_strNameSpace,
    const QString& i_strGraphObjType,
    const QString& i_strObjName,
    QObject*       i_pObjParent ) :
//------------------------------------------------------------------------------
    QAbstractTableModel(i_pObjParent),
    m_pDrawingScene(i_pDrawingScene),
    m_strKeyInTree(),
    m_pGraphObj(nullptr),
    m_arDataRows(),
    m_bContentChanged(false),
    m_iContentChangedSignalBlockedCounter(0),
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjNoisyMethods(nullptr)
{
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        i_strNameSpace + "::" + i_strGraphObjType, ClassName(), i_strObjName);
    m_pTrcAdminObjNoisyMethods = CTrcServer::GetTraceAdminObj(
        i_strNameSpace + "::" + i_strGraphObjType, ClassName() + "::NoisyMethods", i_strObjName);
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "DrawingScene: " + QString(i_pDrawingScene == nullptr ? "nullptr" : i_pDrawingScene->objectName());
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );

} // ctor

//------------------------------------------------------------------------------
CModelGraphObjGeometry::~CModelGraphObjGeometry()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ "" );

    if( m_pTrcAdminObj != nullptr ) {
        mthTracer.onAdminObjAboutToBeReleased();
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    }
    if( m_pTrcAdminObjNoisyMethods != nullptr ) {
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjNoisyMethods);
    }

    m_pDrawingScene = nullptr;
    //m_strKeyInTree;
    m_pGraphObj = nullptr;
    //m_arDataRows;
    m_bContentChanged = false;
    m_iContentChangedSignalBlockedCounter = 0;
    m_pTrcAdminObj = nullptr;
    m_pTrcAdminObjNoisyMethods = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CModelGraphObjGeometry::setKeyInTree(const QString& i_strKeyInTree)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strKeyInTree;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setKeyInTree",
        /* strMethodInArgs    */ strMthInArgs );

    bool bObjectChanged = false;

    if (m_strKeyInTree != i_strKeyInTree) {
        bObjectChanged = true;

        if (m_pGraphObj != nullptr) {
            QObject::disconnect(
                m_pGraphObj, &CGraphObj::geometryChanged,
                this, &CModelGraphObjGeometry::onGraphObjGeometryChanged);
            QObject::disconnect(
                m_pGraphObj, &CGraphObj::aboutToBeDestroyed,
                this, &CModelGraphObjGeometry::onGraphObjAboutToBeDestroyed);
        }

        m_strKeyInTree = i_strKeyInTree;

        if (m_strKeyInTree.isEmpty()) {
            m_pGraphObj = nullptr;
        }
        else {
            m_pGraphObj = m_pDrawingScene->findGraphObj(i_strKeyInTree);
        }

        if (m_pGraphObj != nullptr) {
            QObject::connect(
                m_pGraphObj, &CGraphObj::geometryChanged,
                this, &CModelGraphObjGeometry::onGraphObjGeometryChanged);
            QObject::connect(
                m_pGraphObj, &CGraphObj::aboutToBeDestroyed,
                this, &CModelGraphObjGeometry::onGraphObjAboutToBeDestroyed);
        }

        // Fill the content of the edit controls.
        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

            fillModel();
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bObjectChanged);
    }
    return bObjectChanged;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Checks whether the user has entered meaningful data.

    @return true, if at least one data row has an erronous setting that
            cannot be applied to the graphical object, false otherwise.
*/
bool CModelGraphObjGeometry::hasErrors() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hasErrors",
        /* strAddInfo   */ "" );

    bool bHasErrors = false;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHasErrors);
    }
    return bHasErrors;
}

//------------------------------------------------------------------------------
/*! @brief Checks whether the user has changed settings which haven't been
           applied yet to the graphical object.

    @return true, if at least one data row has a different setting than the
            grahical object which need to be applied to take effect.
            false, if the settings shown are the current settings of the
            graphical object.
*/
bool CModelGraphObjGeometry::hasChanges() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hasChanges",
        /* strAddInfo   */ "" );

    bool bHasChanges = false;
    if (m_pGraphObj != nullptr) {
        QList<SDataRow> arDataRows = getDataRows(m_pGraphObj);
        bHasChanges = (arDataRows != m_arDataRows);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHasChanges);
    }
    return bHasChanges;
}

//------------------------------------------------------------------------------
/*! @brief This method is called by acceptChanges to apply the settings from
           the model at the graphical object.

    The ContentChangedSignalBlocked counter has been incrememented by the
    acceptChanges method as on changing properties of the graphical object
    the on<Properties>Changed slots are called as reentries when applying the
    changes at the graphical object. The on<Properties>Changed slots should
    not call fillModel when applying the changes at the graphical object
    as that would overwrite current settings with old setting.
*/
void CModelGraphObjGeometry::applySettings()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "applySettings",
        /* strAddInfo   */ "" );

    if (m_pGraphObj != nullptr && !hasErrors())
    {
        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

            for (const SDataRow& dataRow : m_arDataRows) {
                m_pGraphObj->setXValue(dataRow.m_strValueName, dataRow.m_physValX);
                m_pGraphObj->setYValue(dataRow.m_strValueName, dataRow.m_physValY);
                dataRow.m_bVisible ?
                    m_pGraphObj->showValueLabel(dataRow.m_strValueName) :
                    m_pGraphObj->hideValueLabel(dataRow.m_strValueName);
                dataRow.m_bLineVisible ?
                    m_pGraphObj->showValueLabelAnchorLine(dataRow.m_strValueName) :
                    m_pGraphObj->hideValueLabelAnchorLine(dataRow.m_strValueName);
            }
        }
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelGraphObjGeometry::clearModel()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "clearModel",
        /* strMethodInArgs    */ "" );

    if (m_arDataRows.size() > 0) {
        _beginRemoveRows(QModelIndex(), 0, m_arDataRows.size()-1);
        m_arDataRows.clear();
        _endRemoveRows();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CModelGraphObjGeometry::fillModel()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "fillModel",
        /* strMethodInArgs    */ "" );

    if (m_arDataRows.size() > 0) {
        clearModel();
    }
    if (m_pGraphObj != nullptr) {
        m_arDataRows = getDataRows(m_pGraphObj);
        if (m_arDataRows.size() > 0) {
            _beginInsertRows(QModelIndex(), 0, m_arDataRows.size()-1);
            _endInsertRows();
            emit_contentChanged();
        }
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the row index for the given, current label name.

    @param [in] i_strName Name to be checked.

    @return Row index of the label with the given current name, -1 otherwise.
*/
int CModelGraphObjGeometry::getValueRowIndex(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    int iRowIdx = -1;
    for (const SDataRow& dataRow : m_arDataRows) {
        if (i_strName == dataRow.m_strValueName) {
            iRowIdx = dataRow.m_iRowIdx;
            break;
        }
    }
    return iRowIdx;
}

//------------------------------------------------------------------------------
/*! @brief Returns a list with the current names of the labels.
*/
QStringList CModelGraphObjGeometry::valueNames() const
//------------------------------------------------------------------------------
{
    QStringList strlstNames;
    for (const SDataRow& dataRow : m_arDataRows) {
        strlstNames.append(dataRow.m_strValueName);
    }
    return strlstNames;
}

/*==============================================================================
public: // must overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelGraphObjGeometry::rowCount(const QModelIndex& i_modelIdxParent) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdxParent {" + qModelIndex2Str(i_modelIdxParent) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "rowCount",
        /* strMethodInArgs    */ strMthInArgs );

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(m_arDataRows.size());
    }
    return m_arDataRows.size();
}

//------------------------------------------------------------------------------
int CModelGraphObjGeometry::columnCount(const QModelIndex& i_modelIdxParent) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdxParent {" + qModelIndex2Str(i_modelIdxParent) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "columnCount",
        /* strMethodInArgs    */ strMthInArgs );

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(EColumnCount);
    }
    return EColumnCount;
}

//------------------------------------------------------------------------------
QVariant CModelGraphObjGeometry::data(const QModelIndex& i_modelIdx, int i_iRole) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + qModelIndex2Str(i_modelIdx) + "}" +
                       ", Role: " + QString::number(i_iRole) + " (" + qItemDataRole2Str(i_iRole) + ")";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "data",
        /* strMethodInArgs    */ strMthInArgs );

    QVariant varData;

    if (m_pGraphObj != nullptr && i_modelIdx.isValid()) {
        int iRow = i_modelIdx.row();
        int iClm = i_modelIdx.column();
        if ((iRow >= 0) && (iRow < m_arDataRows.size())) {
            const SDataRow& dataRow = m_arDataRows[iRow];
            switch (iClm) {
                case EColumnName: {
                    if (i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole) {
                        varData = dataRow.m_strValueName;
                    }
                    break;
                }
                case EColumnXVal: {
                    if (i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole) {
                        varData = dataRow.m_physValX.toString();
                    }
                    break;
                }
                case EColumnYVal: {
                    if (i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole) {
                        varData = dataRow.m_physValY.toString();
                    }
                    break;
                }
                case EColumnShowVals: {
                    if (i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole) {
                        varData = dataRow.m_bVisible;
                    }
                    else if (i_iRole == Qt::CheckStateRole) {
                        varData = dataRow.m_bVisible ? Qt::Checked : Qt::Unchecked;
                    }
                    else if (i_iRole == Qt::TextAlignmentRole) {
                        varData = static_cast<int>(Qt::AlignHCenter | Qt::AlignVCenter);
                    }
                    break;
                }
                case EColumnShowLine: {
                    if (i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole) {
                        varData = dataRow.m_bLineVisible;
                    }
                    else if (i_iRole == Qt::CheckStateRole) {
                        varData = dataRow.m_bLineVisible ? Qt::Checked : Qt::Unchecked;
                    }
                    else if (i_iRole == Qt::TextAlignmentRole) {
                        varData = static_cast<int>(Qt::AlignHCenter | Qt::AlignVCenter);
                    }
                    break;
                }
                default: {
                    break;
                }
            }
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(varData.toString());
    }
    return varData;
}

//------------------------------------------------------------------------------
bool CModelGraphObjGeometry::setData(
    const QModelIndex& i_modelIdx, const QVariant& i_varData, int i_iRole)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + qModelIndex2Str(i_modelIdx) + "}" +
                       ", Data: " + i_varData.toString() +
                       ", Role: " + QString::number(i_iRole) + " (" + qItemDataRole2Str(i_iRole) + ")";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setData",
        /* strMethodInArgs    */ strMthInArgs );

    bool bDataSet = false;

    if (m_pGraphObj != nullptr && i_modelIdx.isValid()) {
        int iRow = i_modelIdx.row();
        int iClm = i_modelIdx.column();
        if ((iRow >= 0) && (iRow < m_arDataRows.size())) {
            SDataRow dataRow = m_arDataRows[iRow];
            switch (iClm) {
                case EColumnName: {
                    break;
                }
                case EColumnXVal: {
                    if (i_iRole == Qt::EditRole) {
                        dataRow.m_physValX = i_varData.toString();
                        bDataSet = true;
                    }
                    break;
                }
                case EColumnYVal: {
                    if (i_iRole == Qt::EditRole) {
                        dataRow.m_physValY = i_varData.toString();
                        bDataSet = true;
                    }
                    break;
                }
                case EColumnShowVals: {
                    if (i_iRole == Qt::EditRole) {
                        dataRow.m_bVisible = i_varData.toBool();
                        bDataSet = true;
                    }
                    break;
                }
                case EColumnShowLine: {
                    if (i_iRole == Qt::EditRole) {
                        dataRow.m_bLineVisible = i_varData.toBool();
                        bDataSet = true;
                    }
                    break;
                }
                default: {
                    break;
                }
            }
            if (m_arDataRows[iRow] != dataRow) {
                m_arDataRows[iRow] = dataRow;
                if (m_iContentChangedSignalBlockedCounter > 0) {
                    m_bContentChanged = true;
                }
                else {
                    emit_contentChanged();
                }
            }
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bDataSet);
    }
    return bDataSet;
}

//------------------------------------------------------------------------------
QVariant CModelGraphObjGeometry::headerData(int i_iSection, Qt::Orientation i_orientation, int i_iRole) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Section: " + column2Str(i_iSection) + "}" +
                       ", Orientation: " + qOrientation2Str(i_orientation) +
                       ", Role: " + QString::number(i_iRole) + " (" + qItemDataRole2Str(i_iRole) + ")";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "headerData",
        /* strMethodInArgs    */ strMthInArgs );

    QVariant varData;

    if (i_orientation == Qt::Horizontal) {
        switch (i_iSection) {
            case EColumnName: {
                if (i_iRole == Qt::DisplayRole) {
                    varData = "Name";
                }
                break;
            }
            case EColumnXVal: {
                if (i_iRole == Qt::DisplayRole) {
                    varData = "X";
                }
                break;
            }
            case EColumnYVal: {
                if (i_iRole == Qt::DisplayRole) {
                    varData = "Y";
                }
                break;
            }
            case EColumnShowVals: {
                if (i_iRole == Qt::DisplayRole) {
                    varData = "Show";
                }
                break;
            }
            case EColumnShowLine: {
                if (i_iRole == Qt::DisplayRole) {
                    varData = "Line";
                }
                break;
            }
            default: {
                break;
            }
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(varData.toString());
    }
    return varData;
}

//------------------------------------------------------------------------------
Qt::ItemFlags CModelGraphObjGeometry::flags(const QModelIndex& i_modelIdx) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + qModelIndex2Str(i_modelIdx) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "flags",
        /* strMethodInArgs    */ strMthInArgs );

    // The base class implementation returns a combination of flags that enables
    // the item (ItemIsEnabled) and allows it to be selected (ItemIsSelectable).
    Qt::ItemFlags uFlags = uFlags = QAbstractItemModel::flags(i_modelIdx);
    if (m_pGraphObj != nullptr && i_modelIdx.isValid()) {
        int iRow = i_modelIdx.row();
        int iClm = i_modelIdx.column();
        if ((iRow >= 0) && (iRow < m_arDataRows.size())) {
            SDataRow dataRow = m_arDataRows[iRow];
            switch (i_modelIdx.column()) {
                case EColumnName: {
                    break;
                }
                case EColumnXVal: {
                    uFlags = uFlags | Qt::ItemIsEditable;
                    break;
                }
                case EColumnYVal: {
                    uFlags = uFlags | Qt::ItemIsEditable;
                    break;
                }
                case EColumnShowVals: {
                    uFlags = uFlags | Qt::ItemIsUserCheckable;
                    break;
                }
                case EColumnShowLine: {
                    uFlags = uFlags | Qt::ItemIsUserCheckable;
                    break;
                }
                default: {
                    break;
                }
            }
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(qItemFlags2Str(uFlags));
    }
    return uFlags;
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CModelGraphObjGeometry::onGraphObjGeometryChanged(CGraphObj* i_pGraphObj)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pGraphObj->keyInTree();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onGraphObjGeometryChanged",
        /* strMethodInArgs    */ strMthInArgs );

    // When applying the changes from the model by invoking "applySettings"
    // the ContentChangedSignalBlockedCounter is incremented to avoid that the
    // "onGraphObj<Signal>" slots overwrite settings in the model which haven't been
    // applied yet.
    if (m_iContentChangedSignalBlockedCounter == 0)
    {
        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

            m_bContentChanged = true;
        }

        // If the "contentChanged" signal is no longer blocked and the content of
        // properties widget has been changed ...
        if (m_iContentChangedSignalBlockedCounter == 0 && m_bContentChanged) {
            // .. emit the contentChanged signal and update the enabled state
            // of the Apply and Reset buttons.
            emit_contentChanged();
            m_bContentChanged = false;
        }
    }
}

//------------------------------------------------------------------------------
void CModelGraphObjGeometry::onGraphObjAboutToBeDestroyed(CGraphObj*)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onGraphObjAboutToBeDestroyed",
        /* strAddInfo   */ "" );

    m_pGraphObj = nullptr;

    clearModel();
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QList<CModelGraphObjGeometry::SDataRow> CModelGraphObjGeometry::getDataRows(CGraphObj* i_pGraphObj) const
//------------------------------------------------------------------------------
{
    QList<SDataRow> arDataRows;
    if (i_pGraphObj != nullptr) {
        QStringList strlstValueNames = i_pGraphObj->getValueNames();
        if (strlstValueNames.size() > 0) {
            for (const QString& strName : strlstValueNames) {
                if (i_pGraphObj->isLabelAdded(strName)) {
                    arDataRows.append(
                        SDataRow::fromGraphObj(i_pGraphObj, strName, arDataRows.size()));
                }
            }
        }
    }
    return arDataRows;
}

/*==============================================================================
protected: // instance methods (tracing emitting signals)
==============================================================================*/

//------------------------------------------------------------------------------
void CModelGraphObjGeometry::emit_contentChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_contentChanged",
        /* strAddInfo   */ "" );

    emit contentChanged();
}

//------------------------------------------------------------------------------
void CModelGraphObjGeometry::_beginInsertRows(
    const QModelIndex& i_modelIdxParent, int i_iRowFirst, int i_iRowLast)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent {" + qModelIndex2Str(i_modelIdxParent) + "}" +
                       ", First: " + QString::number(i_iRowFirst) +
                       ", Last: " + QString::number(i_iRowLast);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "beginInsertRows",
        /* strAddInfo   */ strMthInArgs );

    beginInsertRows(i_modelIdxParent, i_iRowFirst, i_iRowLast);
}

//------------------------------------------------------------------------------
void CModelGraphObjGeometry::_endInsertRows()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "endInsertRows",
        /* strAddInfo   */ "" );

    endInsertRows();
}

//------------------------------------------------------------------------------
void CModelGraphObjGeometry::_beginRemoveRows(
    const QModelIndex& i_modelIdxParent, int i_iRowFirst, int i_iRowLast)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent {" + qModelIndex2Str(i_modelIdxParent) + "}" +
                       ", First: " + QString::number(i_iRowFirst) +
                       ", Last: " + QString::number(i_iRowLast);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "beginRemoveRows",
        /* strAddInfo   */ strMthInArgs );

    beginRemoveRows(i_modelIdxParent, i_iRowFirst, i_iRowLast);
}

//------------------------------------------------------------------------------
void CModelGraphObjGeometry::_endRemoveRows()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "endRemoveRows",
        /* strAddInfo   */ "" );

    endRemoveRows();
}

//------------------------------------------------------------------------------
void CModelGraphObjGeometry::emit_dataChanged(
    const QModelIndex& i_modelIdxTL,
    const QModelIndex& i_modelIdxBR,
    const QVector<int>& i_ariRoles)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "TL {" + qModelIndex2Str(i_modelIdxTL) + "}" +
                       ", BR {" + qModelIndex2Str(i_modelIdxBR) + "}" +
                       ", Roles [" + QString::number(i_ariRoles.size()) + "]";
        if (i_ariRoles.size() > 0) {
            strMthInArgs += "(";
            for (int idxRole = 0; idxRole < i_ariRoles.size(); idxRole++) {
                if (idxRole > 0) strMthInArgs += ", ";
                strMthInArgs += QString::number(idxRole) + ": " + qItemDataRole2Str(i_ariRoles[idxRole]);
            }
            strMthInArgs += ")";
        }
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_dataChanged",
        /* strAddInfo   */ strMthInArgs );

    emit dataChanged(i_modelIdxTL, i_modelIdxBR, i_ariRoles);
}
