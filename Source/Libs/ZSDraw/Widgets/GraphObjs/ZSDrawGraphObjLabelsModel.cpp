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
        { EColumnName, "Name"},
        { EColumnText, "Text"},
        { EColumnVisible, "Visible"},
        { EColumnAnchor, "Anchor"},
        { EColumnAnchorLineVisible, "AnchorLineVisible"}
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
void CModelGraphObjLabels::setGraphObj(CGraphObj* i_pGraphObj)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pGraphObj == nullptr ? "nullptr" : i_pGraphObj->path());
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setGraphObj",
        /* strMethodInArgs    */ strMthInArgs );

    if (m_pGraphObj != i_pGraphObj) {
        clearModel();
    }
    m_pGraphObj = i_pGraphObj;

    if (m_pGraphObj != nullptr) {
        fillModel();
    }
}

/*==============================================================================
public: // overridables
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
/*! @brief This method is called to apply the changes made in the model
           to the graphical object.

    The values from the model are applied one after another at the graphical object.

    For each changed property the method "onGraphObjChanged" is called as an
    reentry. "onGraphObjChanged" is also used to fill the model with the
    current property values of the graphical object if a new graphical object
    has been set or if the settings got to be reset.

    To avoid that "onGraphObjChanged" overwrites settings in the model which
    haven't been applied yet the m_iContentChangedSignalBlockedCounter is incremented
    before applying the changes from the model.
*/
void CModelGraphObjLabels::acceptChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "acceptChanges",
        /* strAddInfo   */ "" );

    if (!hasErrors())
    {
        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

            // Apply the settings from the model at the graphical object.
            applySettings();
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Resets the current settings of the model to the current values
           of the graphical object.

    Resetting is done be invoking "fillModel".
*/
void CModelGraphObjLabels::rejectChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "rejectChanges",
        /* strAddInfo   */ "" );

    // "fillEditControls" is used to fill the edit controls with the current settings
    // of the graphical object. To avoid that the signal "contentChanged" is emitted
    // for each property of the graphical object set at the edit controls the
    // ContentChangedSignalBlockedCounter is incremented. After fillEditControls has
    // been executed the contentChanged flag is checked and the contentChanged signal
    // is emitted if necessary.

    {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

        fillModel();
    }

    // If the "contentChanged" signal is no longer blocked and the content of
    // the model has been changed ...
    if (m_iContentChangedSignalBlockedCounter == 0 && m_bContentChanged) {
        // .. emit the contentChanged signal and update the enabled state
        // of the Apply and Reset buttons.
        emit_contentChanged();
        m_bContentChanged = false;
    }
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

    if (m_pGraphObj != nullptr) {
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
                        varData = CEnumSelectionPoint(labelSettings.m_selPt).toString();
                    }
                    else if (i_iRole == Qt::AccessibleTextRole) {
                        QList<ESelectionPoint> arSelPts =
                            m_pGraphObj->getPossibleLabelAnchorPoints(labelSettings.m_strName);
                        QList<SComboBoxItem> arItems;
                        for (const ESelectionPoint& selPt : arSelPts) {
                            arItems.append(SComboBoxItem(CEnumSelectionPoint(selPt).toString()));
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
                case EColumnName: {
                    if (i_iRole == Qt::EditRole) {
                        labelSettings.m_strName = i_varData.toString();
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
                        labelSettings.m_selPt = CEnumSelectionPoint(i_varData.toString()).enumerator();
                        bDataSet = true;
                    }
                    break;
                }
                case EColumnAnchorLineVisible: {
                    if (i_iRole == Qt::EditRole) {
                        labelSettings.m_bAnchorLineVisible = i_varData.toBool();
                    }
                    break;
                }
                default: {
                    break;
                }
            }
            m_arLabelSettings[iRow] = labelSettings;
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
    if (i_modelIdx.isValid()) {
        switch (i_modelIdx.column()) {
            case EColumnName: {
                break;
            }
            case EColumnText: {
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
            default: {
                break;
            }
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(qItemFlags2Str(uFlags));
    }
    return uFlags;
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelGraphObjLabels::clearModel()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "clearModel",
        /* strMethodInArgs    */ "" );

    if (m_arLabelSettings.size() > 0) {
        _beginRemoveRows(QModelIndex(), 0, m_arLabelSettings.size()-1);
        m_arLabelSettings.clear();
        m_hshName2RowIdx.clear();
        _endRemoveRows();
    }
}

//------------------------------------------------------------------------------
void CModelGraphObjLabels::fillModel()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
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
        }
    }
}

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

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CModelGraphObjLabels::onGraphObjLabelAdded(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onGraphObjLabelAdded",
        /* strMethodInArgs    */ strMthInArgs );

    // When applying the changes from the model controls by invoking "acceptChanges"
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
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onGraphObjLabelRemoved",
        /* strMethodInArgs    */ strMthInArgs );

    // When applying the changes from the model controls by invoking "acceptChanges"
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
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onGraphObjLabelChanged",
        /* strMethodInArgs    */ strMthInArgs );

    // When applying the changes from the model controls by invoking "acceptChanges"
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
            emit_contentChanged();
            m_bContentChanged = false;
        }
    }
}

//------------------------------------------------------------------------------
void CModelGraphObjLabels::onGraphObjAboutToDestroyed()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onGraphObjAboutToDestroyed",
        /* strAddInfo   */ "" );

    m_pGraphObj = nullptr;

    clearModel();
}

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
