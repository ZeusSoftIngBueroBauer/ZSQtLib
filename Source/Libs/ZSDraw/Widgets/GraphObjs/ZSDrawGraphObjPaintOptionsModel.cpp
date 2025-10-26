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

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjPaintOptionsModel.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
//#include "ZSSysGUI/ZSSysComboBoxItemDelegate.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysRefCountGuard.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

//#include <QtCore/qset.h>

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Draw;


/*******************************************************************************
class CModelGraphObjPaintOptions : public QAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
QString CModelGraphObjPaintOptions::column2Str(EColumn i_enumVal)
//------------------------------------------------------------------------------
{
    static const QHash<EColumn, QByteArray> s_hshEnumVal2Str {
        { EColumnName, "Option"},
        { EColumnIsSet, "Set"}
    };
    return s_hshEnumVal2Str.value(i_enumVal, "? (" + QByteArray::number(i_enumVal) + ")");
}

//------------------------------------------------------------------------------
QString CModelGraphObjPaintOptions::paintOptionName2Str(EPaintOptionName i_enumVal)
//------------------------------------------------------------------------------
{
    static const QHash<EPaintOptionName, QByteArray> s_hshEnumVal2Str {
        { EPaintOptionNameDrawBoundingRect, "Draw Bounding Rect"},
        { EPaintOptionNameDrawShapePath, "Draw Shape Path"}
    };
    return s_hshEnumVal2Str.value(i_enumVal, "? (" + QByteArray::number(i_enumVal) + ")");
}

/*==============================================================================
protected: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Fills the label struct with the information retrieved from the graphical object.
*/
CModelGraphObjPaintOptions::SPaintOption CModelGraphObjPaintOptions::SPaintOption::fromGraphObj(
    CGraphObj* i_pGraphObj, EPaintOptionName i_eOptionName, int i_iRowIdx)
//------------------------------------------------------------------------------
{
    SPaintOption labelSettings;
    labelSettings.m_eOptionName = i_eOptionName;
    labelSettings.m_iRowIdx = i_iRowIdx;
    if (i_eOptionName == EPaintOptionNameDrawBoundingRect) {
        labelSettings.m_bIsSet = i_pGraphObj->isPaintBoundingRectSet();
    }
    else if (i_eOptionName == EPaintOptionNameDrawShapePath) {
        labelSettings.m_bIsSet = i_pGraphObj->isPaintShapePathSet();
    }
    return labelSettings;
}

//------------------------------------------------------------------------------
CModelGraphObjPaintOptions::SPaintOption::SPaintOption(
    EPaintOptionName i_eOptionName, int i_iRowIdx, bool i_bIsSet) :
//------------------------------------------------------------------------------
    m_eOptionName(i_eOptionName), m_iRowIdx(i_iRowIdx), m_bIsSet(i_bIsSet)
{
}

//------------------------------------------------------------------------------
bool CModelGraphObjPaintOptions::SPaintOption::operator == (const SPaintOption& i_other) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;
    if (m_eOptionName != i_other.m_eOptionName) {
        bEqual = false;
    }
    else if (m_iRowIdx != i_other.m_iRowIdx) {
        bEqual = false;
    }
    else if (m_bIsSet != i_other.m_bIsSet) {
        bEqual = false;
    }
    return bEqual;
}

//------------------------------------------------------------------------------
bool CModelGraphObjPaintOptions::SPaintOption::operator != (const SPaintOption& i_other) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelGraphObjPaintOptions::CModelGraphObjPaintOptions(
    CDrawingScene* i_pDrawingScene,
    const QString& i_strNameSpace,
    const QString& i_strGraphObjType,
    const QString& i_strObjName,
    QObject*       i_pObjParent ) :
//------------------------------------------------------------------------------
    QAbstractTableModel(i_pObjParent),
    m_pDrawingScene(i_pDrawingScene)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        i_strNameSpace + "::" + i_strGraphObjType, ClassName(), objectName());
    m_pTrcAdminObjNoisyMethods = CTrcServer::GetTraceAdminObj(
        i_strNameSpace + "::" + i_strGraphObjType, ClassName() + "::NoisyMethods", objectName());
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
CModelGraphObjPaintOptions::~CModelGraphObjPaintOptions()
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
    //m_arPaintOptions;
    m_bContentChanged = false;
    m_iContentChangedSignalBlockedCounter = 0;
    m_pTrcAdminObj = nullptr;
    m_pTrcAdminObjNoisyMethods = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CModelGraphObjPaintOptions::setKeyInTree(const QString& i_strKeyInTree)
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
                m_pGraphObj, &CGraphObj::optionPaintBoundingRectChanged,
                this, &CModelGraphObjPaintOptions::onGraphObjOptionPaintBoundingRectChanged);
            QObject::disconnect(
                m_pGraphObj, &CGraphObj::optionPaintShapePathChanged,
                this, &CModelGraphObjPaintOptions::onGraphObjOptionPaintShapePathChanged);
            QObject::disconnect(
                m_pGraphObj, &CGraphObj::aboutToBeDestroyed,
                this, &CModelGraphObjPaintOptions::onGraphObjAboutToBeDestroyed);
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
                m_pGraphObj, &CGraphObj::optionPaintBoundingRectChanged,
                this, &CModelGraphObjPaintOptions::onGraphObjOptionPaintBoundingRectChanged);
            QObject::connect(
                m_pGraphObj, &CGraphObj::optionPaintShapePathChanged,
                this, &CModelGraphObjPaintOptions::onGraphObjOptionPaintShapePathChanged);
            QObject::connect(
                m_pGraphObj, &CGraphObj::aboutToBeDestroyed,
                this, &CModelGraphObjPaintOptions::onGraphObjAboutToBeDestroyed);
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
bool CModelGraphObjPaintOptions::hasErrors() const
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
bool CModelGraphObjPaintOptions::hasChanges() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hasChanges",
        /* strAddInfo   */ "" );

    bool bHasChanges = false;
    if (m_pGraphObj != nullptr) {
        QList<SPaintOption> arPaintOptions = getPaintOptions(m_pGraphObj);
        bHasChanges = (arPaintOptions != m_arPaintOptions);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHasChanges);
    }
    return bHasChanges;
}

//------------------------------------------------------------------------------
/*! @brief This method is called by acceptChanges to apply the settings from
           the model at the graphical object.

    The values from the model are applied one after another at the graphical object.

    For each changed property the method "onGraphObjChanged" is called as an reentry.
    "onGraphObjChanged" is also used to fill the model with the current values of the
    graphical object if a new graphical object has been set or if the settings got to be reset.

    To avoid that "onGraphObjChanged" overwrites settings in the model which haven't been
    applied yet the m_iContentChangedSignalBlockedCounter is incremented before applying
    the changes from the model.
*/
void CModelGraphObjPaintOptions::acceptChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "acceptChanges",
        /* strAddInfo   */ "" );

    if (m_pGraphObj != nullptr && !hasErrors())
    {
        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

            for (const SPaintOption& paintOption : m_arPaintOptions) {
                if (paintOption.m_eOptionName == EPaintOptionNameDrawBoundingRect) {
                    m_pGraphObj->setPaintBoundingRect(paintOption.m_bIsSet);
                }
                else if (paintOption.m_eOptionName == EPaintOptionNameDrawShapePath) {
                    m_pGraphObj->setPaintShapePath(paintOption.m_bIsSet);
                }
            }
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Resets the current settings of the edit controls to the current values
           of the graphical object.

    Resetting is done be invoking "onGraphObjChanged", which fills the edit
    controls with the current property values of the graphical object.
*/
void CModelGraphObjPaintOptions::rejectChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "rejectChanges",
        /* strAddInfo   */ "" );

    // "fillModel" is used to fill the model with the current settings of the
    // graphical object. To avoid that the signal "contentChanged" is emitted
    // for each value of the graphical object set in the model the
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
        emit_contentChanged();
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CModelGraphObjPaintOptions::getOptionRowIndex(EPaintOptionName i_eOptionName) const
//------------------------------------------------------------------------------
{
    int iRowIdx = -1;
    for (const SPaintOption& paintOption : m_arPaintOptions) {
        if (i_eOptionName == paintOption.m_eOptionName) {
            iRowIdx = paintOption.m_iRowIdx;
            break;
        }
    }
    return iRowIdx;
}

/*==============================================================================
public: // must overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelGraphObjPaintOptions::rowCount(const QModelIndex& i_modelIdxParent) const
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
        mthTracer.setMethodReturn(static_cast<int>(m_arPaintOptions.size()));
    }
    return m_arPaintOptions.size();
}

//------------------------------------------------------------------------------
int CModelGraphObjPaintOptions::columnCount(const QModelIndex& i_modelIdxParent) const
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
QVariant CModelGraphObjPaintOptions::data(const QModelIndex& i_modelIdx, int i_iRole) const
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
        if ((iRow >= 0) && (iRow < m_arPaintOptions.size())) {
            const SPaintOption& paintOption = m_arPaintOptions[iRow];
            switch (iClm) {
                case EColumnName: {
                    if (i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole) {
                        varData = paintOptionName2Str(paintOption.m_eOptionName);
                    }
                    break;
                }
                case EColumnIsSet: {
                    if (i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole) {
                        varData = paintOption.m_bIsSet;
                    }
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
bool CModelGraphObjPaintOptions::setData(
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
        if ((iRow >= 0) && (iRow < m_arPaintOptions.size())) {
            SPaintOption paintOption = m_arPaintOptions[iRow];
            switch (iClm) {
                case EColumnName: {
                    break;
                }
                case EColumnIsSet: {
                    if (i_iRole == Qt::EditRole) {
                        paintOption.m_bIsSet = i_varData.toBool();
                        bDataSet = true;
                    }
                    break;
                }
            }
            if (m_arPaintOptions[iRow] != paintOption) {
                m_arPaintOptions[iRow] = paintOption;
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
QVariant CModelGraphObjPaintOptions::headerData(int i_iSection, Qt::Orientation i_orientation, int i_iRole) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Section: " + column2Str(static_cast<EColumn>(i_iSection)) + "}" +
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
        if (i_iRole == Qt::DisplayRole) {
            varData = column2Str(static_cast<EColumn>(i_iSection));
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(varData.toString());
    }
    return varData;
}

//------------------------------------------------------------------------------
Qt::ItemFlags CModelGraphObjPaintOptions::flags(const QModelIndex& i_modelIdx) const
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
        if ((iRow >= 0) && (iRow < m_arPaintOptions.size())) {
            SPaintOption labelSettings = m_arPaintOptions[iRow];
            switch (i_modelIdx.column()) {
                case EColumnName: {
                    break;
                }
                case EColumnIsSet: {
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
void CModelGraphObjPaintOptions::onGraphObjOptionPaintBoundingRectChanged(
    CGraphObj* i_pGraphObj, bool i_bOptionSet)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pGraphObj->keyInTree() + ", " + bool2Str(i_bOptionSet);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onGraphObjOptionPaintBoundingRectChanged",
        /* strMethodInArgs    */ strMthInArgs );

    // When applying the changes from the model by invoking "applySettings"
    // the ContentChangedSignalBlockedCounter is incremented to avoid that the
    // "onGraphObj<Signal>" slots overwrite settings in the model which haven't been
    // applied yet.
    if (m_iContentChangedSignalBlockedCounter == 0)
    {
        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

            int iRow = getOptionRowIndex(EPaintOptionNameDrawBoundingRect);
            if (iRow >= 0 && iRow < m_arPaintOptions.size()) {
                if (m_arPaintOptions[iRow].m_bIsSet != i_bOptionSet) {
                    m_arPaintOptions[iRow].m_bIsSet = i_bOptionSet;
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
        }
    }
}

//------------------------------------------------------------------------------
void CModelGraphObjPaintOptions::onGraphObjOptionPaintShapePathChanged(
    CGraphObj* i_pGraphObj, bool i_bOptionSet)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pGraphObj->keyInTree() + ", " + bool2Str(i_bOptionSet);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onGraphObjOptionPaintShapePathChanged",
        /* strMethodInArgs    */ strMthInArgs );

    // When applying the changes from the model by invoking "applySettings"
    // the ContentChangedSignalBlockedCounter is incremented to avoid that the
    // "onGraphObj<Signal>" slots overwrite settings in the model which haven't been
    // applied yet.
    if (m_iContentChangedSignalBlockedCounter == 0)
    {
        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

            int iRow = getOptionRowIndex(EPaintOptionNameDrawShapePath);
            if (iRow >= 0 && iRow < m_arPaintOptions.size()) {
                if (m_arPaintOptions[iRow].m_bIsSet != i_bOptionSet) {
                    m_arPaintOptions[iRow].m_bIsSet = i_bOptionSet;
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
        }
    }
}

//------------------------------------------------------------------------------
void CModelGraphObjPaintOptions::onGraphObjAboutToBeDestroyed(CGraphObj*)
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
void CModelGraphObjPaintOptions::clearModel()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "clearModel",
        /* strMethodInArgs    */ "" );

    if (m_arPaintOptions.size() > 0) {
        _beginRemoveRows(QModelIndex(), 0, m_arPaintOptions.size()-1);
        m_arPaintOptions.clear();
        _endRemoveRows();
        if (m_iContentChangedSignalBlockedCounter > 0) {
            m_bContentChanged = true;
        }
        else {
            emit_contentChanged();
        }
    }
}

//------------------------------------------------------------------------------
void CModelGraphObjPaintOptions::fillModel()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "fillModel",
        /* strMethodInArgs    */ "" );

    if (m_arPaintOptions.size() > 0) {
        clearModel();
    }
    if (m_pGraphObj != nullptr) {
        m_arPaintOptions = getPaintOptions(m_pGraphObj);
        if (m_arPaintOptions.size() > 0) {
            _beginInsertRows(QModelIndex(), 0, m_arPaintOptions.size()-1);
            _endInsertRows();
            if (m_iContentChangedSignalBlockedCounter > 0) {
                m_bContentChanged = true;
            }
            else {
                emit_contentChanged();
            }
        }
    }
}

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QList<CModelGraphObjPaintOptions::SPaintOption> CModelGraphObjPaintOptions::getPaintOptions(CGraphObj* i_pGraphObj) const
//------------------------------------------------------------------------------
{
    QList<SPaintOption> arPaintOptions;
    if (i_pGraphObj != nullptr) {
        for (int iEnumVal = 0; iEnumVal < EPaintOptionNameCount; ++iEnumVal) {
            arPaintOptions.append(
                SPaintOption::fromGraphObj(
                    i_pGraphObj, static_cast<EPaintOptionName>(iEnumVal), arPaintOptions.size()));
        }
    }
    return arPaintOptions;
}

/*==============================================================================
protected: // instance methods (tracing emitting signals)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Emits the contentChanged signal and resets the content changed flags.
*/
void CModelGraphObjPaintOptions::emit_contentChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_contentChanged",
        /* strAddInfo   */ "" );

    m_bContentChanged = false;
    emit contentChanged();
}

//------------------------------------------------------------------------------
void CModelGraphObjPaintOptions::_beginInsertRows(
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
void CModelGraphObjPaintOptions::_endInsertRows()
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
void CModelGraphObjPaintOptions::_beginRemoveRows(
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
void CModelGraphObjPaintOptions::_endRemoveRows()
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
void CModelGraphObjPaintOptions::emit_dataChanged(
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
