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

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjLabelsModel.h"
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
using namespace ZS::Draw;


/*******************************************************************************
class ZSDRAWDLL_API CModelGraphObjLabels : public ZS::System::GUI::CModelIdxTree
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
QString CModelGraphObjLabels::column2Str(int i_clm)
//------------------------------------------------------------------------------
{
    static QHash<int, QByteArray> s_clm2Name {
        { EColumnSelected, "Selected"},
        { EColumnName, "Name"},
        { EColumnText, "Text"},
        { EColumnVisible, "Visible"},
        { EColumnAnchor, "Anchor"},
        { EColumnAnchorLineVisible, "AnchorLineVisible"},
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

//------------------------------------------------------------------------------
CModelGraphObjLabels::SLabelSettings CModelGraphObjLabels::SLabelSettings::fromGraphObj(
    CGraphObj* i_pGraphObj, const QString& i_strLabelName, int i_iRowIdx)
//------------------------------------------------------------------------------
{
    SLabelSettings labelSettings;
    labelSettings.m_bSelected = false;
    labelSettings.m_strName = i_strLabelName;
    labelSettings.m_iRowIdx = i_iRowIdx;
    labelSettings.m_bIsPredefinedLabelName = i_pGraphObj->isPredefinedLabelName(i_strLabelName);
    labelSettings.m_strText = i_pGraphObj->labelText(i_strLabelName);
    labelSettings.m_selPt = i_pGraphObj->labelAnchorPoint(i_strLabelName);
    labelSettings.m_bVisible = i_pGraphObj->isLabelVisible(i_strLabelName);
    labelSettings.m_bAnchorLineVisible = i_pGraphObj->isLabelAnchorLineVisible(i_strLabelName);
    labelSettings.m_errResultInfo = SErrResultInfo();
    return labelSettings;
}

//------------------------------------------------------------------------------
bool CModelGraphObjLabels::SLabelSettings::operator == (const SLabelSettings& i_other) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;
    if (m_bSelected != i_other.m_bSelected) {
        bEqual = false;
    }
    else if (m_strName != i_other.m_strName) {
        bEqual = false;
    }
    else if (m_iRowIdx != i_other.m_iRowIdx) {
        bEqual = false;
    }
    else if (m_bIsPredefinedLabelName != i_other.m_bIsPredefinedLabelName) {
        bEqual = false;
    }
    else if (m_strText != i_other.m_strText) {
        bEqual = false;
    }
    else if (m_selPt != i_other.m_selPt) {
        bEqual = false;
    }
    else if (m_bVisible != i_other.m_bVisible) {
        bEqual = false;
    }
    else if (m_bAnchorLineVisible != i_other.m_bAnchorLineVisible) {
        bEqual = false;
    }
    else if (m_errResultInfo != i_other.m_errResultInfo) {
        bEqual = false;
    }
    return bEqual;
}

//------------------------------------------------------------------------------
bool CModelGraphObjLabels::SLabelSettings::operator != (const SLabelSettings& i_other) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelGraphObjLabels::CModelGraphObjLabels(
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
    m_arLabelSettings(),
    m_hshName2RowIdx(),
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
CModelGraphObjLabels::~CModelGraphObjLabels()
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
    //m_arLabelSettings;
    //m_hshName2RowIdx;
    m_bContentChanged = false;
    m_iContentChangedSignalBlockedCounter = 0;
    m_pTrcAdminObj = nullptr;
    m_pTrcAdminObjNoisyMethods = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CModelGraphObjLabels::setKeyInTree(const QString& i_strKeyInTree)
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
                m_pGraphObj, &CGraphObj::labelAdded,
                this, &CModelGraphObjLabels::onGraphObjLabelAdded);
            QObject::disconnect(
                m_pGraphObj, &CGraphObj::labelRemoved,
                this, &CModelGraphObjLabels::onGraphObjLabelRemoved);
            QObject::disconnect(
                m_pGraphObj, &CGraphObj::labelRenamed,
                this, &CModelGraphObjLabels::onGraphObjLabelRenamed);
            QObject::disconnect(
                m_pGraphObj, &CGraphObj::labelChanged,
                this, &CModelGraphObjLabels::onGraphObjLabelChanged);
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
                m_pGraphObj, &CGraphObj::labelAdded,
                this, &CModelGraphObjLabels::onGraphObjLabelAdded);
            QObject::connect(
                m_pGraphObj, &CGraphObj::labelRemoved,
                this, &CModelGraphObjLabels::onGraphObjLabelRemoved);
            QObject::connect(
                m_pGraphObj, &CGraphObj::labelRenamed,
                this, &CModelGraphObjLabels::onGraphObjLabelRenamed);
            QObject::connect(
                m_pGraphObj, &CGraphObj::labelChanged,
                this, &CModelGraphObjLabels::onGraphObjLabelChanged);
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
bool CModelGraphObjLabels::hasErrors() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hasErrors",
        /* strAddInfo   */ "" );

    bool bHasErrors = false;
    for (const SLabelSettings& labelSettings : m_arLabelSettings) {
        if (isErrorResult(labelSettings.m_errResultInfo.getResult())) {
            bHasErrors = true;
            break;
        }
    }
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
bool CModelGraphObjLabels::hasChanges() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hasChanges",
        /* strAddInfo   */ "" );

    bool bHasChanges = false;
    if (m_pGraphObj != nullptr) {
        QList<SLabelSettings> arLabelSettings = getLabelSettings(m_pGraphObj);
        bHasChanges = (arLabelSettings != m_arLabelSettings);
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
void CModelGraphObjLabels::applySettings()
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

            // First rename labels which have been renamed.
            QStringList strlstLabelNames = m_hshName2RowIdx.keys();
            for (const QString& strName : strlstLabelNames) {
                int iRow = m_hshName2RowIdx.value(strName);
                const SLabelSettings& labelSettings = m_arLabelSettings[iRow];
                if (strName != labelSettings.m_strName) {
                    m_pGraphObj->renameLabel(strName, labelSettings.m_strName);
                    m_hshName2RowIdx.remove(strName);
                    m_hshName2RowIdx.insert(labelSettings.m_strName, iRow);
                }
            }
            // Next remove labels which have been removed.
            strlstLabelNames = m_pGraphObj->getLabelNames();
            for (const QString& strName : strlstLabelNames) {
                if (!m_hshName2RowIdx.contains(strName)) {
                    m_pGraphObj->removeLabel(strName);
                }
            }
            // Last add or change labels which have been added or changed.
            for (const SLabelSettings& labelSettings : m_arLabelSettings) {
                const QString& strName = labelSettings.m_strName;
                if (!m_pGraphObj->isLabelAdded(strName)) {
                    m_pGraphObj->addLabel(strName);
                }
                m_pGraphObj->setLabelText(strName, labelSettings.m_strText);
                m_pGraphObj->setLabelAnchorPoint(strName, labelSettings.m_selPt);
                labelSettings.m_bVisible ?
                    m_pGraphObj->showLabel(strName) : m_pGraphObj->hideLabel(strName);
                labelSettings.m_bAnchorLineVisible ?
                    m_pGraphObj->showLabelAnchorLine(strName) : m_pGraphObj->hideLabelAnchorLine(strName);
            }
        }
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelGraphObjLabels::clearModel()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "clearModel",
        /* strMethodInArgs    */ "" );

    if (m_arLabelSettings.size() > 0) {
        _beginRemoveRows(QModelIndex(), 0, m_arLabelSettings.size()-1);
        m_arLabelSettings.clear();
        m_hshName2RowIdx.clear();
        _endRemoveRows();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CModelGraphObjLabels::fillModel()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "fillModel",
        /* strMethodInArgs    */ "" );

    if (m_arLabelSettings.size() > 0) {
        clearModel();
    }
    if (m_pGraphObj != nullptr) {
        m_arLabelSettings = getLabelSettings(m_pGraphObj);
        if (m_arLabelSettings.size() > 0) {
            _beginInsertRows(QModelIndex(), 0, m_arLabelSettings.size()-1);
            for (int iRow = 0; iRow < m_arLabelSettings.size(); ++iRow) {
                const SLabelSettings& labelSettings = m_arLabelSettings[iRow];
                m_hshName2RowIdx.insert(labelSettings.m_strName, iRow);
            }
            _endInsertRows();
            emit_contentChanged();
        }
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the list of all existing label names.

    @return List of existing label names.
*/
QString CModelGraphObjLabels::findUniqueLabelName(const QString& i_strPrefix) const
//------------------------------------------------------------------------------
{
    QString strName = i_strPrefix;
    if (strName.isEmpty()) {
        strName = "Label";
    }
    strName += QString::number(m_hshName2RowIdx.size());
    if (m_hshName2RowIdx.contains(strName)) {
        strName = i_strPrefix;
        if (strName.isEmpty()) {
            strName = "Label";
        }
        int iRow = 0;
        QString strNameTmp = strName + QString::number(iRow);
        while (m_hshName2RowIdx.contains(strNameTmp)) {
            ++iRow;
            strNameTmp = strName + QString::number(iRow);
        }
        strName = strNameTmp;
    }
    return strName;
}

//------------------------------------------------------------------------------
/*! @brief Adds a label to the model and returns the name of the label.

    The label is not added to the graphical object but only to the internal list
    of labels within the model. To add the label to the graphical object
    applySettings need to be called.

    @return Name of the label.
*/
QString CModelGraphObjLabels::addLabel()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "addLabel",
        /* strMethodInArgs    */ "" );

    QString strName = findUniqueLabelName("Label");
    ESelectionPoint selPt = ESelectionPoint::None;
    QList<ESelectionPoint> arSelPts = m_pGraphObj->getPossibleLabelAnchorPoints(strName);
    if (arSelPts.size() > 0) {
        selPt = arSelPts[0];
    }
    int iRow = m_arLabelSettings.size();
    _beginInsertRows(QModelIndex(), iRow, iRow);
    m_arLabelSettings.append(SLabelSettings(strName, iRow, false, strName, selPt, false, false));
    m_hshName2RowIdx.insert(strName, iRow);
    _endInsertRows();
    emit_contentChanged();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(strName);
    }
    return strName;
}

//------------------------------------------------------------------------------
/*! @brief Removes the label with the given name from the model.
*/
void CModelGraphObjLabels::removeLabel(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "removeLabel",
        /* strMethodInArgs    */ strMthInArgs );

    int iRow = m_hshName2RowIdx.value(i_strName, -1);
    if (iRow >= 0 && iRow < m_arLabelSettings.size()) {
        _beginRemoveRows(QModelIndex(), iRow, iRow);
        m_arLabelSettings.removeAt(iRow);
        m_hshName2RowIdx.remove(i_strName);
        for (; iRow < m_arLabelSettings.size(); iRow++) {
            const SLabelSettings labelSettings = m_arLabelSettings[iRow];
            m_arLabelSettings[iRow].m_iRowIdx = iRow;
            m_hshName2RowIdx[labelSettings.m_strName] = iRow;
        }
        _endRemoveRows();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns a list with the names of the selected labels.
*/
QStringList CModelGraphObjLabels::selectedLabels() const
//------------------------------------------------------------------------------
{
    QStringList strlstNames;
    for (const SLabelSettings& labelSettings : m_arLabelSettings) {
        if (labelSettings.m_bSelected) {
            strlstNames.append(labelSettings.m_strName);
        }
    }
    return strlstNames;
}

//------------------------------------------------------------------------------
/*! @brief Removes all selected labels from the model.

    @note The method does not implicitely call removeLabel to remove a single
          label on purpose so that the contentChanged signal is emitted just once.
*/
void CModelGraphObjLabels::removeSelectedLabels()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "removeSelectedLabels",
        /* strMethodInArgs    */ "" );

    if (m_arLabelSettings.size() > 0) {
        for (int iRow = m_arLabelSettings.size()-1; iRow >= 0; --iRow) {
            const SLabelSettings labelSettings = m_arLabelSettings[iRow];
            if (labelSettings.m_bSelected) {
                _beginRemoveRows(QModelIndex(), iRow, iRow);
                m_arLabelSettings.removeAt(iRow);
                m_hshName2RowIdx.remove(labelSettings.m_strName);
                for (int iRowTmp = iRow; iRowTmp < m_arLabelSettings.size(); iRowTmp++) {
                    const SLabelSettings labelSettings = m_arLabelSettings[iRowTmp];
                    m_arLabelSettings[iRowTmp].m_iRowIdx = iRowTmp;
                    m_hshName2RowIdx[labelSettings.m_strName] = iRowTmp;
                }
                _endRemoveRows();
            }
        }
        emit_contentChanged();
    }
}

/*==============================================================================
public: // must overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelGraphObjLabels::rowCount(const QModelIndex& i_modelIdxParent) const
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
        mthTracer.setMethodReturn(m_arLabelSettings.size());
    }
    return m_arLabelSettings.size();
}

//------------------------------------------------------------------------------
int CModelGraphObjLabels::columnCount(const QModelIndex& i_modelIdxParent) const
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
QVariant CModelGraphObjLabels::data(const QModelIndex& i_modelIdx, int i_iRole) const
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
        if ((iRow >= 0) && (iRow < m_arLabelSettings.size())) {
            const SLabelSettings& labelSettings = m_arLabelSettings[iRow];
            switch (iClm) {
                case EColumnSelected: {
                    if (i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole) {
                        varData = labelSettings.m_bSelected;
                    }
                    break;
                }
                case EColumnName: {
                    if (i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole) {
                        varData = labelSettings.m_strName;
                    }
                    break;
                }
                case EColumnText: {
                    if (i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole) {
                        varData = labelSettings.m_strText;
                    }
                    break;
                }
                case EColumnVisible: {
                    if (i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole) {
                        varData = labelSettings.m_bVisible;
                    }
                    else if (i_iRole == Qt::CheckStateRole) {
                        varData = labelSettings.m_bVisible ? Qt::Checked : Qt::Unchecked;
                    }
                    else if (i_iRole == Qt::TextAlignmentRole) {
                        varData = static_cast<int>(Qt::AlignHCenter | Qt::AlignVCenter);
                    }
                    break;
                }
                case EColumnAnchor: {
                    if (i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole) {
                        if (m_pGraphObj->type() == EGraphObjTypeLine) {
                            if (labelSettings.m_selPt == ESelectionPoint::TopLeft) {
                                varData = "P1";
                            }
                            else if (labelSettings.m_selPt == ESelectionPoint::BottomRight) {
                                varData = "P2";
                            }
                            else {
                                varData = CEnumSelectionPoint(labelSettings.m_selPt).toString();
                            }
                        }
                        else {
                            varData = CEnumSelectionPoint(labelSettings.m_selPt).toString();
                        }
                    }
                    else if (i_iRole == Qt::AccessibleTextRole) {
                        QList<ESelectionPoint> arSelPts =
                            m_pGraphObj->getPossibleLabelAnchorPoints(labelSettings.m_strName);
                        QList<SComboBoxItem> arItems;
                        for (const ESelectionPoint& selPt : arSelPts) {
                            if (m_pGraphObj->type() == EGraphObjTypeLine) {
                                if (selPt == ESelectionPoint::TopLeft) {
                                    arItems.append(SComboBoxItem("P1"));
                                }
                                else if (selPt == ESelectionPoint::BottomRight) {
                                    arItems.append(SComboBoxItem("P2"));
                                }
                                else {
                                    arItems.append(SComboBoxItem(CEnumSelectionPoint(selPt).toString()));
                                }
                            }
                            else {
                                arItems.append(SComboBoxItem(CEnumSelectionPoint(selPt).toString()));
                            }
                        }
                        varData.setValue(arItems);
                    }
                    break;
                }
                case EColumnAnchorLineVisible: {
                    if (i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole) {
                        varData = labelSettings.m_bAnchorLineVisible;
                    }
                    else if (i_iRole == Qt::CheckStateRole) {
                        varData = labelSettings.m_bAnchorLineVisible ? Qt::Checked : Qt::Unchecked;
                    }
                    else if (i_iRole == Qt::TextAlignmentRole) {
                        varData = static_cast<int>(Qt::AlignHCenter | Qt::AlignVCenter);
                    }
                    break;
                }
                case EColumnError: {
                    if (i_iRole == Qt::DecorationRole) {
                        if (isErrorResult(labelSettings.m_errResultInfo.getResult())) {
                            varData = getErrIcon(labelSettings.m_errResultInfo.getSeverity());
                        }
                    }
                    else if (i_iRole == Qt::ToolTipRole) {
                        QString strResult = labelSettings.m_errResultInfo.getResultStr();
                        QString strAddInfo = labelSettings.m_errResultInfo.getAddErrInfoDscr();
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
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(varData.toString());
    }
    return varData;
}

//------------------------------------------------------------------------------
bool CModelGraphObjLabels::setData(
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
        if ((iRow >= 0) && (iRow < m_arLabelSettings.size())) {
            SLabelSettings labelSettings = m_arLabelSettings[iRow];
            switch (iClm) {
                case EColumnSelected: {
                    if (i_iRole == Qt::EditRole) {
                        if (!m_pGraphObj->isPredefinedLabelName(labelSettings.m_strName)) {
                            labelSettings.m_bSelected = i_varData.toBool();
                            bDataSet = true;
                        }
                    }
                    break;
                }
                case EColumnName: {
                    if (i_iRole == Qt::EditRole) {
                        QString strNameNew = i_varData.toString();
                        if (labelSettings.m_strName != strNameNew) {
                            if (m_pGraphObj->isPredefinedLabelName(strNameNew)) {
                                labelSettings.m_errResultInfo = SErrResultInfo(
                                    NameSpace(), ClassName(), objectName(), "setData",
                                    EResultInvalidObjName, EResultSeverityError,
                                    "User defined label names must be different from predefined label names");
                            }
                            else if (m_hshName2RowIdx.contains(strNameNew)) {
                                if (m_hshName2RowIdx.value(strNameNew) != labelSettings.m_iRowIdx) {
                                    labelSettings.m_errResultInfo = SErrResultInfo(
                                        NameSpace(), ClassName(), objectName(), "setData",
                                        EResultObjNameNotUnique, EResultSeverityError,
                                        "Label names must be unique");
                                }
                            }
                            else {
                                labelSettings.m_errResultInfo = ErrResultSuccess;
                            }
                            m_hshName2RowIdx.remove(labelSettings.m_strName);
                            labelSettings.m_strName = i_varData.toString();
                            m_hshName2RowIdx.insert(labelSettings.m_strName, labelSettings.m_iRowIdx);
                        }
                        bDataSet = true;
                    }
                    break;
                }
                case EColumnText: {
                    if (i_iRole == Qt::EditRole) {
                        labelSettings.m_strText = i_varData.toString();
                        bDataSet = true;
                    }
                    break;
                }
                case EColumnVisible: {
                    if (i_iRole == Qt::EditRole) {
                        labelSettings.m_bVisible = i_varData.toBool();
                        bDataSet = true;
                    }
                    break;
                }
                case EColumnAnchor: {
                    if (i_iRole == Qt::EditRole) {
                        if (m_pGraphObj->type() == EGraphObjTypeLine) {
                            if (i_varData.toString() == "P1") {
                                labelSettings.m_selPt = ESelectionPoint::TopLeft;
                            }
                            else if (i_varData.toString() == "P2") {
                                labelSettings.m_selPt = ESelectionPoint::BottomRight;
                            }
                            else {
                                labelSettings.m_selPt = CEnumSelectionPoint(i_varData.toString()).enumerator();
                            }
                        }
                        else {
                            labelSettings.m_selPt = CEnumSelectionPoint(i_varData.toString()).enumerator();
                        }
                        bDataSet = true;
                    }
                    break;
                }
                case EColumnAnchorLineVisible: {
                    if (i_iRole == Qt::EditRole) {
                        labelSettings.m_bAnchorLineVisible = i_varData.toBool();
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
            if (m_arLabelSettings[iRow] != labelSettings) {
                m_arLabelSettings[iRow] = labelSettings;
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
QVariant CModelGraphObjLabels::headerData(int i_iSection, Qt::Orientation i_orientation, int i_iRole) const
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
            case EColumnSelected: {
                if (i_iRole == Qt::DisplayRole) {
                    varData = "Select";
                }
                break;
            }
            case EColumnName: {
                if (i_iRole == Qt::DisplayRole) {
                    varData = "Key";
                }
                break;
            }
            case EColumnText: {
                if (i_iRole == Qt::DisplayRole) {
                    varData = "Text";
                }
                break;
            }
            case EColumnVisible: {
                if (i_iRole == Qt::DisplayRole) {
                    varData = "Visible";
                }
                break;
            }
            case EColumnAnchor: {
                if (i_iRole == Qt::DisplayRole) {
                    varData = "Anchored To";
                }
                break;
            }
            case EColumnAnchorLineVisible: {
                if (i_iRole == Qt::DisplayRole) {
                    varData = "Line";
                }
                break;
            }
            case EColumnError: {
                if (i_iRole == Qt::DisplayRole) {
                    varData = "Error";
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
Qt::ItemFlags CModelGraphObjLabels::flags(const QModelIndex& i_modelIdx) const
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
        if ((iRow >= 0) && (iRow < m_arLabelSettings.size())) {
            SLabelSettings labelSettings = m_arLabelSettings[iRow];
            switch (i_modelIdx.column()) {
                case EColumnSelected: {
                    if (!m_pGraphObj->isPredefinedLabelName(labelSettings.m_strName)) {
                        uFlags = uFlags | Qt::ItemIsUserCheckable;
                    }
                    break;
                }
                case EColumnName: {
                    if (!labelSettings.m_bIsPredefinedLabelName) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    break;
                }
                case EColumnText: {
                    uFlags = uFlags | Qt::ItemIsEditable;
                    break;
                }
                case EColumnVisible: {
                    uFlags = uFlags | Qt::ItemIsUserCheckable;
                    break;
                }
                case EColumnAnchor: {
                    uFlags = uFlags | Qt::ItemIsEditable;
                    break;
                }
                case EColumnAnchorLineVisible: {
                    uFlags = uFlags | Qt::ItemIsUserCheckable;
                    break;
                }
                case EColumnError: {
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
void CModelGraphObjLabels::onGraphObjLabelAdded(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onGraphObjLabelAdded",
        /* strMethodInArgs    */ strMthInArgs );

    // When applying the changes from the model by invoking "applySettings"
    // the ContentChangedSignalBlockedCounter is incremented to avoid that the
    // "onGraphObj<Signal>" slots overwrite settings in the model which haven't been
    // applied yet.
    if (m_iContentChangedSignalBlockedCounter == 0)
    {
        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

            if (!m_hshName2RowIdx.contains(i_strName)) {
                int iRow = m_arLabelSettings.size();
                _beginInsertRows(QModelIndex(), iRow, iRow);
                m_hshName2RowIdx.insert(i_strName, iRow);
                m_arLabelSettings.append(SLabelSettings::fromGraphObj(m_pGraphObj, i_strName, iRow));
                _endInsertRows();
                m_bContentChanged = true;
            }
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
void CModelGraphObjLabels::onGraphObjLabelRemoved(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onGraphObjLabelRemoved",
        /* strMethodInArgs    */ strMthInArgs );

    // When applying the changes from the model by invoking "applySettings"
    // the ContentChangedSignalBlockedCounter is incremented to avoid that the
    // "onGraphObj<Signal>" slots overwrite settings in the model which haven't been
    // applied yet.
    if (m_iContentChangedSignalBlockedCounter == 0)
    {
        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

            if (m_hshName2RowIdx.contains(i_strName)) {
                int iRow = m_hshName2RowIdx.value(i_strName);
                _beginRemoveRows(QModelIndex(), iRow, iRow);
                m_hshName2RowIdx.remove(i_strName);
                m_arLabelSettings.removeAt(iRow);
                for (; iRow < m_arLabelSettings.size(); iRow++) {
                    m_arLabelSettings[iRow].m_iRowIdx = iRow;
                    m_hshName2RowIdx[m_arLabelSettings[iRow].m_strName] = iRow;
                }
                _endRemoveRows();
                m_bContentChanged = true;
            }
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
void CModelGraphObjLabels::onGraphObjLabelRenamed(const QString& i_strName, const QString& i_strNameNew)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName + ", " + i_strNameNew;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onGraphObjLabelRenamed",
        /* strMethodInArgs    */ strMthInArgs );

    // When applying the changes from the model by invoking "applySettings"
    // the ContentChangedSignalBlockedCounter is incremented to avoid that the
    // "onGraphObj<Signal>" slots overwrite settings in the model which haven't been
    // applied yet.
    if (m_iContentChangedSignalBlockedCounter == 0)
    {
        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

            if (m_hshName2RowIdx.contains(i_strName)) {
                int iRow = m_hshName2RowIdx.value(i_strName);
                m_hshName2RowIdx.remove(i_strName);
                m_hshName2RowIdx.insert(i_strName, iRow);
                SLabelSettings labelSettings = SLabelSettings::fromGraphObj(m_pGraphObj, i_strNameNew, iRow);
                if (m_arLabelSettings[iRow] != labelSettings) {
                    m_arLabelSettings[iRow] = labelSettings;
                    QModelIndex modelIdxTL = index(iRow, 0);
                    QModelIndex modelIdxBR = index(iRow, EColumnCount-1);
                    emit_dataChanged(modelIdxTL, modelIdxBR);
                }
                m_bContentChanged = true;
            }
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
void CModelGraphObjLabels::onGraphObjLabelChanged(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onGraphObjLabelChanged",
        /* strMethodInArgs    */ strMthInArgs );

    // When applying the changes from the model by invoking "applySettings"
    // the ContentChangedSignalBlockedCounter is incremented to avoid that the
    // "onGraphObj<Signal>" slots overwrite settings in the model which haven't been
    // applied yet.
    if (m_iContentChangedSignalBlockedCounter == 0)
    {
        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

            if (m_hshName2RowIdx.contains(i_strName)) {
                int iRow = m_hshName2RowIdx.value(i_strName);
                SLabelSettings labelSettings = SLabelSettings::fromGraphObj(m_pGraphObj, i_strName, iRow);
                if (m_arLabelSettings[iRow] != labelSettings) {
                    m_arLabelSettings[iRow] = labelSettings;
                    QModelIndex modelIdxTL = index(iRow, 0);
                    QModelIndex modelIdxBR = index(iRow, EColumnCount-1);
                    emit_dataChanged(modelIdxTL, modelIdxBR);
                    m_bContentChanged = true;
                }
            }
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
void CModelGraphObjLabels::onGraphObjAboutToBeDestroyed(CGraphObj*)
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
QList<CModelGraphObjLabels::SLabelSettings> CModelGraphObjLabels::getLabelSettings(CGraphObj* i_pGraphObj) const
//------------------------------------------------------------------------------
{
    QList<SLabelSettings> arLabelSettings;
    if (i_pGraphObj != nullptr) {
        QStringList strlstLabelNames = i_pGraphObj->getLabelNames();
        if (strlstLabelNames.size() > 0) {
            // The predefined labels should be at the beginning of the table.
            // Insert those first if the corresponding label has been added.
            QStringList strlstPredefinedLabelNames = i_pGraphObj->getPredefinedLabelNames();
            QSet<QString> strlstLabelNamesAdded;
            for (const QString& strName : strlstPredefinedLabelNames) {
                if (i_pGraphObj->isLabelAdded(strName)) {
                    SLabelSettings labelSettings(
                        strName, arLabelSettings.size(), true,
                        i_pGraphObj->labelText(strName), i_pGraphObj->labelAnchorPoint(strName),
                        i_pGraphObj->isLabelVisible(strName), i_pGraphObj->isLabelAnchorLineVisible(strName));
                    arLabelSettings.append(std::move(labelSettings));
                    strlstLabelNamesAdded.insert(strName);
                }
            }
            // The user defined labels should follow the predefined labels.
            // Add those after the predefined labels.
            for (const QString& strName : strlstLabelNames) {
                if (i_pGraphObj->isLabelAdded(strName)) {
                    // If label has not already been added as a predefined label ...
                    if (!strlstLabelNamesAdded.contains(strName)) {
                        SLabelSettings labelSettings(
                            strName, arLabelSettings.size(), false,
                            i_pGraphObj->labelText(strName), i_pGraphObj->labelAnchorPoint(strName),
                            i_pGraphObj->isLabelVisible(strName), i_pGraphObj->isLabelAnchorLineVisible(strName));
                        arLabelSettings.append(std::move(labelSettings));
                        strlstLabelNamesAdded.insert(strName);
                    }
                }
            }
        }
    }
    return arLabelSettings;
}

////------------------------------------------------------------------------------
//bool CModelGraphObjLabels::changedNameIsUnique() const
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod    */ "changedNameIsUnique",
//        /* strAddInfo   */ "" );
//
//    bool bIsUnique = true;
//
//    //if (m_pGraphObj != nullptr) {
//    //    QString strParentBranchPath = m_pGraphObj->parentBranchPath();
//    //    QString strNewKeyInTree = m_pDrawingScene->getGraphObjsIdxTree()->buildKeyInTreeStr(
//    //        m_pGraphObj->entryTypeSymbol(), strParentBranchPath, m_pEdtName->text());
//    //    if (strNewKeyInTree != m_strKeyInTree) {
//    //        bIsUnique = (m_pDrawingScene->findGraphObj(strNewKeyInTree) == nullptr);
//    //    }
//    //}
//
//    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
//        mthTracer.setMethodReturn(bIsUnique);
//    }
//    return bIsUnique;
//}

/*==============================================================================
protected: // instance methods (tracing emitting signals)
==============================================================================*/

//------------------------------------------------------------------------------
void CModelGraphObjLabels::emit_contentChanged()
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
void CModelGraphObjLabels::_beginInsertRows(
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
void CModelGraphObjLabels::_endInsertRows()
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
void CModelGraphObjLabels::_beginRemoveRows(
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
void CModelGraphObjLabels::_endRemoveRows()
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
void CModelGraphObjLabels::emit_dataChanged(
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
