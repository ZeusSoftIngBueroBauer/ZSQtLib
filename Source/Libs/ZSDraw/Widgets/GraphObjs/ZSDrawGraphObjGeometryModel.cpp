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
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLine.h"
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
CModelGraphObjGeometry::SLabelSettings CModelGraphObjGeometry::SLabelSettings::fromGraphObj(
    CGraphObj* i_pGraphObj, const QString& i_strValueName, int i_iRowIdx)
//------------------------------------------------------------------------------
{
    SLabelSettings labelSettings;
    labelSettings.m_strValueName = i_strValueName;
    labelSettings.m_iRowIdx = i_iRowIdx;
    labelSettings.m_bVisible = i_pGraphObj->isValueLabelVisible(i_strValueName);
    labelSettings.m_bLineVisible = i_pGraphObj->isValueLabelAnchorLineVisible(i_strValueName);
    return labelSettings;
}

//------------------------------------------------------------------------------
CModelGraphObjGeometry::SLabelSettings::SLabelSettings() :
//------------------------------------------------------------------------------
    m_strValueName(), m_iRowIdx(-1),
    m_bVisible(false), m_bLineVisible(false)
{
}

//------------------------------------------------------------------------------
CModelGraphObjGeometry::SLabelSettings::SLabelSettings(
    const QString& i_strValueName, int i_iRowIdx,
    const CPhysVal& i_physValX, const CPhysVal& i_physValY,
    bool i_bVisible, bool i_bLineVisible) :
//------------------------------------------------------------------------------
    m_strValueName(), m_iRowIdx(-1),
    m_bVisible(false), m_bLineVisible(false)
{
}

//------------------------------------------------------------------------------
bool CModelGraphObjGeometry::SLabelSettings::operator == (const SLabelSettings& i_other) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;
    if (m_strValueName != i_other.m_strValueName) {
        bEqual = false;
    }
    else if (m_iRowIdx != i_other.m_iRowIdx) {
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
bool CModelGraphObjGeometry::SLabelSettings::operator != (const SLabelSettings& i_other) const
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
    const ZS::System::CEnumScaleDimensionUnit& i_eDimensionUnit,
    QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    QAbstractTableModel(i_pObjParent),
    m_pDrawingScene(i_pDrawingScene),
    m_eDimensionUnit(i_eDimensionUnit),
    m_font(),
    m_fontMetrics(m_font),
    m_strKeyInTree(),
    m_pGraphObj(nullptr),
    m_physValLine(),
    m_arLabelSettings(),
    m_strXYValSizeHint("1024 px +-"),
    m_sizeXYValSizeHint(),
    m_bContentChanged(false),
    m_iContentChangedSignalBlockedCounter(0),
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjNoisyMethods(nullptr)
{
    setObjectName(i_strObjName + m_eDimensionUnit.toString());

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
    m_eDimensionUnit = static_cast<EScaleDimensionUnit>(0);
    //m_font;
    //m_fontMetrics;
    //m_strKeyInTree;
    m_pGraphObj = nullptr;
    //m_physValLine;
    //m_arLabelSettings;
    //m_strXYValSizeHint;
    //m_sizeXYValSizeHint;
    m_bContentChanged = false;
    m_iContentChangedSignalBlockedCounter = 0;
    m_pTrcAdminObj = nullptr;
    m_pTrcAdminObjNoisyMethods = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the font to be used for the sizeHint role.
*/
void CModelGraphObjGeometry::setFont(const QFont& i_font)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_font.family();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setFont",
        /* strMethodInArgs    */ strMthInArgs );

    m_font = i_font;
    m_fontMetrics = QFontMetrics(m_font);

    updateXYValueSizeHint();
}

//------------------------------------------------------------------------------
/*! @brief Returns the font used for the sizeHint role.
*/
QFont CModelGraphObjGeometry::font() const
//------------------------------------------------------------------------------
{
    return m_font;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Selects the object to be indicated and edited in the widget.

    "fillModel" is called to fill the model with the settings of the graphical object.

    @param i_strKeyInTree [in]
        Unique key of the graphical object.
*/
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
                m_pDrawingScene, &CDrawingScene::drawingSizeChanged,
                this, &CModelGraphObjGeometry::onDrawingSceneDrawingSizeChanged);
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
                m_pDrawingScene, &CDrawingScene::drawingSizeChanged,
                this, &CModelGraphObjGeometry::onDrawingSceneDrawingSizeChanged);
            QObject::connect(
                m_pGraphObj, &CGraphObj::geometryChanged,
                this, &CModelGraphObjGeometry::onGraphObjGeometryChanged);
            QObject::connect(
                m_pGraphObj, &CGraphObj::aboutToBeDestroyed,
                this, &CModelGraphObjGeometry::onGraphObjAboutToBeDestroyed);
        }

        updateXYValueSizeHint();

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

//------------------------------------------------------------------------------
/*! @brief Returns the unique key of the graphical object currently shown in
           the widget.
*/
QString CModelGraphObjGeometry::getKeyInTree() const
//------------------------------------------------------------------------------
{
    return m_strKeyInTree;
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
    CGraphObjLine* pGraphObjLine = nullptr;
    if (m_pGraphObj != nullptr) {
        pGraphObjLine = dynamic_cast<CGraphObjLine*>(m_pGraphObj);
    }
    if (pGraphObjLine != nullptr) {
        const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
        CPhysValLine physValLine = pGraphObjLine->getLine(drawingSize.unit());
        bHasChanges = (m_physValLine != physValLine);
        if (!bHasChanges) {
            QList<SLabelSettings> arLabelSettings = getLabelSettings(m_pGraphObj);
            bHasChanges = (arLabelSettings != m_arLabelSettings);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHasChanges);
    }
    return bHasChanges;
}

//------------------------------------------------------------------------------
/*! @brief This method is called to apply the changes made in the model to the graphical object.

    The values from the model are applied one after another at the graphical object.

    For each changed property the method "onGraphObjChanged" is called as an reentry.
    "onGraphObjChanged" is also used to fill the model with the current values of the
    graphical object if a new graphical object has been set or if the settings got to be reset.

    To avoid that "onGraphObjChanged" overwrites settings in the model which haven't been
    applied yet the m_iContentChangedSignalBlockedCounter is incremented before applying
    the changes from the model.
*/
void CModelGraphObjGeometry::acceptChanges()
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

            CGraphObjLine* pGraphObjLine = nullptr;
            if (m_pGraphObj != nullptr) {
                pGraphObjLine = dynamic_cast<CGraphObjLine*>(m_pGraphObj);
            }
            if (pGraphObjLine != nullptr) {
                pGraphObjLine->setLine(m_physValLine);
                for (const SLabelSettings& labelSettings : m_arLabelSettings) {
                    labelSettings.m_bVisible ?
                        m_pGraphObj->showValueLabel(labelSettings.m_strValueName) :
                        m_pGraphObj->hideValueLabel(labelSettings.m_strValueName);
                    labelSettings.m_bLineVisible ?
                        m_pGraphObj->showValueLabelAnchorLine(labelSettings.m_strValueName) :
                        m_pGraphObj->hideValueLabelAnchorLine(labelSettings.m_strValueName);
                }
            }
        }

        // If the "contentChanged" signal is no longer blocked and the content of
        // properties widget has been changed ...
        if (m_iContentChangedSignalBlockedCounter == 0 && m_bContentChanged) {
            // .. emit the contentChanged signal.
            emit_contentChanged();
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Resets the current settings of the edit controls to the current values
           of the graphical object.

    Resetting is done be invoking "onGraphObjChanged", which fills the edit
    controls with the current property values of the graphical object.
*/
void CModelGraphObjGeometry::rejectChanges()
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
/*! @brief Returns the row index for the given, current label name.

    @param [in] i_strName Name to be checked.

    @return Row index of the label with the given current name, -1 otherwise.
*/
int CModelGraphObjGeometry::getValueRowIndex(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    int iRowIdx = -1;
    for (const SLabelSettings& labelSettings : m_arLabelSettings) {
        if (i_strName == labelSettings.m_strValueName) {
            iRowIdx = labelSettings.m_iRowIdx;
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
    for (const SLabelSettings& labelSettings : m_arLabelSettings) {
        strlstNames.append(labelSettings.m_strValueName);
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
        mthTracer.setMethodReturn(m_arLabelSettings.size());
    }
    return m_arLabelSettings.size();
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
        if ((iRow >= 0) && (iRow < m_arLabelSettings.size())) {
            const SLabelSettings& labelSettings = m_arLabelSettings[iRow];
            switch (iClm) {
                case EColumnName: {
                    if (i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole) {
                        varData = labelSettings.m_strValueName;
                    }
                    else if (i_iRole == Qt::SizeHintRole) {
                        varData = m_fontMetrics.boundingRect(QString(labelSettings.m_strValueName + "OO000")).size();
                    }
                    break;
                }
                case EColumnXVal: {
                    if (i_iRole == Qt::DisplayRole) {
                        if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameP1) {
                            varData = m_physValLine.p1().x().toString();
                        }
                        else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameP2) {
                            varData = m_physValLine.p2().x().toString();
                        }
                        else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameCenter) {
                            varData = m_physValLine.center().x().toString();
                        }
                        else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameSize) {
                            varData = m_physValLine.width().toString();
                        }
                        else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameLength) {
                            varData = m_physValLine.length().toString();
                        }
                        else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameAngle) {
                            varData = m_physValLine.angle().toString();
                        }
                    }
                    else if (i_iRole == Qt::EditRole) {
                        if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameP1) {
                            varData = QVariant::fromValue(m_physValLine.p1().x());
                        }
                        else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameP2) {
                            varData = QVariant::fromValue(m_physValLine.p2().x());
                        }
                        else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameCenter) {
                            varData = QVariant::fromValue(m_physValLine.center().x());
                        }
                        else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameSize) {
                            varData = QVariant::fromValue(m_physValLine.width());
                        }
                        else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameLength) {
                            varData = QVariant::fromValue(m_physValLine.length());
                        }
                        else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameAngle) {
                            varData = QVariant::fromValue(m_physValLine.angle());
                        }
                    }
                    else if (i_iRole == Qt::TextAlignmentRole) {
                        varData = static_cast<int>(Qt::AlignRight|Qt::AlignVCenter);
                    }
                    else if (i_iRole == Qt::SizeHintRole) {
                        varData = m_sizeXYValSizeHint;
                    }
                    else if (i_iRole == ERoleMinimumValue) {
                        if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameAngle) {
                            varData = 360.0;
                        }
                        else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameSize) {
                            if (m_eDimensionUnit == EScaleDimensionUnit::Metric) {
                                varData = -m_pDrawingScene->drawingSize().metricImageWidth().getVal();
                            }
                            else {
                                varData = -m_pDrawingScene->drawingSize().imageWidthInPixels();
                            }
                        }
                        else {
                            if (m_eDimensionUnit == EScaleDimensionUnit::Metric) {
                                varData = 0.0;
                            }
                            else {
                                varData = 0;
                            }
                        }
                    }
                    else if (i_iRole == ERoleMaximumValue) {
                        if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameAngle) {
                            varData = 360.0;
                        }
                        else {
                            if (m_eDimensionUnit == EScaleDimensionUnit::Metric) {
                                varData = m_pDrawingScene->drawingSize().metricImageWidth().getVal();
                            }
                            else {
                                varData = m_pDrawingScene->drawingSize().imageWidthInPixels();
                            }
                        }
                    }
                    break;
                }
                case EColumnYVal: {
                    if (i_iRole == Qt::DisplayRole) {
                        if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameP1) {
                            varData = m_physValLine.p1().y().toString();
                        }
                        else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameP2) {
                            varData = m_physValLine.p2().y().toString();
                        }
                        else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameCenter) {
                            varData = m_physValLine.center().y().toString();
                        }
                        else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameSize) {
                            varData = m_physValLine.height().toString();
                        }
                    }
                    else if (i_iRole == Qt::EditRole) {
                        if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameP1) {
                            varData = QVariant::fromValue(m_physValLine.p1().y());
                        }
                        else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameP2) {
                            varData = QVariant::fromValue(m_physValLine.p2().y());
                        }
                        else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameCenter) {
                            varData = QVariant::fromValue(m_physValLine.center().y());
                        }
                        else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameSize) {
                            varData = QVariant::fromValue(m_physValLine.height());
                        }
                    }
                    else if (i_iRole == Qt::TextAlignmentRole) {
                        varData = static_cast<int>(Qt::AlignRight|Qt::AlignVCenter);
                    }
                    else if (i_iRole == Qt::SizeHintRole) {
                        varData = m_sizeXYValSizeHint;
                    }
                    else if (i_iRole == ERoleMinimumValue) {
                        if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameSize) {
                            if (m_eDimensionUnit == EScaleDimensionUnit::Metric) {
                                varData = -m_pDrawingScene->drawingSize().metricImageHeight().getVal();
                            }
                            else {
                                varData = -m_pDrawingScene->drawingSize().imageHeightInPixels();
                            }
                        }
                        else {
                            if (m_eDimensionUnit == EScaleDimensionUnit::Metric) {
                                varData = 0.0;
                            }
                            else {
                                varData = 0;
                            }
                        }
                    }
                    else if (i_iRole == ERoleMaximumValue) {
                        if (m_eDimensionUnit == EScaleDimensionUnit::Metric) {
                            varData = m_pDrawingScene->drawingSize().metricImageHeight().getVal();
                        }
                        else {
                            varData = m_pDrawingScene->drawingSize().imageHeightInPixels();
                        }
                    }
                    break;
                }
                case EColumnShowVals: {
                    if (i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole) {
                        varData = labelSettings.m_bVisible;
                    }
                    else if (i_iRole == Qt::CheckStateRole) {
                        varData = labelSettings.m_bVisible ? Qt::Checked : Qt::Unchecked;
                    }
                    else if (i_iRole == Qt::TextAlignmentRole) {
                        varData = static_cast<int>(Qt::AlignHCenter|Qt::AlignVCenter);
                    }
                    break;
                }
                case EColumnShowLine: {
                    if (i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole) {
                        varData = labelSettings.m_bLineVisible;
                    }
                    else if (i_iRole == Qt::CheckStateRole) {
                        varData = labelSettings.m_bLineVisible ? Qt::Checked : Qt::Unchecked;
                    }
                    else if (i_iRole == Qt::TextAlignmentRole) {
                        varData = static_cast<int>(Qt::AlignHCenter|Qt::AlignVCenter);
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
        QString strMthRet;
        if (i_iRole == Qt::EditRole && (i_modelIdx.column() == EColumnXVal || i_modelIdx.column() == EColumnYVal)) {
            strMthRet = varData.value<CPhysVal>().toString();
        }
        else {
            strMthRet = varData.toString();
        }
        mthTracer.setMethodReturn(strMthRet);
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
                       ", Role: " + QString::number(i_iRole) + " (" + qItemDataRole2Str(i_iRole) + ")";
        if (i_iRole == Qt::EditRole && (i_modelIdx.column() == EColumnXVal || i_modelIdx.column() == EColumnYVal)) {
            strMthInArgs += ", Data: " + i_varData.value<CPhysVal>().toString();
        }
        else {
            strMthInArgs += ", Data: " + i_varData.toString();
        }
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
            QVector<bool> arbColumnsChanged(EColumnCount, false);
            SLabelSettings labelSettings = m_arLabelSettings[iRow];
            CPhysValLine physValLine = m_physValLine;
            switch (iClm) {
                case EColumnName: {
                    break;
                }
                case EColumnXVal: {
                    if (i_iRole == Qt::EditRole) {
                        if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameP1) {
                            CPhysValPoint physValP1 = physValLine.p1();
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValP1.setX(physVal);
                            physValLine.setP1(physValP1);
                            arbColumnsChanged[EColumnXVal] = true;
                            arbColumnsChanged[EColumnYVal] = true;
                        }
                        else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameP2) {
                            CPhysValPoint physValP2 = physValLine.p2();
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValP2.setX(physVal);
                            physValLine.setP2(physValP2);
                            arbColumnsChanged[EColumnXVal] = true;
                            arbColumnsChanged[EColumnYVal] = true;
                        }
                        else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameCenter) {
                            CPhysValPoint physValPtCenter = physValLine.center();
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValPtCenter.setX(physVal);
                            physValLine.setCenter(physValPtCenter);
                            arbColumnsChanged[EColumnXVal] = true;
                            arbColumnsChanged[EColumnYVal] = true;
                        }
                        else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameSize) {
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValLine.setWidth(physVal);
                            arbColumnsChanged[EColumnXVal] = true;
                            arbColumnsChanged[EColumnYVal] = true;
                        }
                        else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameLength) {
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValLine.setLength(physVal);
                            arbColumnsChanged[EColumnXVal] = true;
                        }
                        else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameAngle) {
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValLine.setAngle(physVal);
                            arbColumnsChanged[EColumnXVal] = true;
                        }
                        bDataSet = true;
                    }
                    break;
                }
                case EColumnYVal: {
                    if (i_iRole == Qt::EditRole) {
                        if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameP1) {
                            CPhysValPoint physValP1 = physValLine.p1();
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValP1.setY(physVal);
                            physValLine.setP1(physValP1);
                            arbColumnsChanged[EColumnXVal] = true;
                            arbColumnsChanged[EColumnYVal] = true;
                        }
                        else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameP2) {
                            CPhysValPoint physValP2 = physValLine.p2();
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValP2.setY(physVal);
                            physValLine.setP2(physValP2);
                            arbColumnsChanged[EColumnXVal] = true;
                            arbColumnsChanged[EColumnYVal] = true;
                        }
                        else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameCenter) {
                            CPhysValPoint physValPtCenter = physValLine.center();
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValPtCenter.setY(physVal);
                            physValLine.setCenter(physValPtCenter);
                            arbColumnsChanged[EColumnXVal] = true;
                            arbColumnsChanged[EColumnYVal] = true;
                        }
                        else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameSize) {
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValLine.setHeight(physVal);
                            arbColumnsChanged[EColumnXVal] = true;
                            arbColumnsChanged[EColumnYVal] = true;
                        }
                        bDataSet = true;
                    }
                    break;
                }
                case EColumnShowVals: {
                    if (i_iRole == Qt::EditRole) {
                        labelSettings.m_bVisible = i_varData.toBool();
                        bDataSet = true;
                    }
                    break;
                }
                case EColumnShowLine: {
                    if (i_iRole == Qt::EditRole) {
                        labelSettings.m_bLineVisible = i_varData.toBool();
                        bDataSet = true;
                    }
                    break;
                }
                default: {
                    break;
                }
            }
            bool bContentChanged = false;
            if (m_physValLine != physValLine) {
                m_physValLine = physValLine;
                bContentChanged = true;
            }
            if (m_arLabelSettings[iRow] != labelSettings) {
                m_arLabelSettings[iRow] = labelSettings;
                bContentChanged = true;
            }
            if (bContentChanged) {
                for (int idxRow = 0; idxRow < m_arLabelSettings.size(); ++idxRow) {
                    for (int idxClm = 0; idxClm < arbColumnsChanged.size(); ++idxClm) {
                        if (arbColumnsChanged[idxClm]) {
                            if (idxClm != iClm || idxRow != iRow) {
                                QModelIndex modelIdxTL = index(idxRow, idxClm);
                                QModelIndex modelIdxBR = index(idxRow, idxClm);
                                emit_dataChanged(modelIdxTL, modelIdxBR);
                            }
                        }
                    }
                }
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
        if ((iRow >= 0) && (iRow < m_arLabelSettings.size())) {
            SLabelSettings labelSettings = m_arLabelSettings[iRow];
            switch (i_modelIdx.column()) {
                case EColumnName: {
                    uFlags = uFlags & ~Qt::ItemIsSelectable;
                    break;
                }
                case EColumnXVal: {
                    if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameP1) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameP2) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameCenter) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameSize) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameLength) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameAngle) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    break;
                }
                case EColumnYVal: {
                    if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameP1) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameP2) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameCenter) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameSize) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameLength) {
                        uFlags = uFlags & ~Qt::ItemIsSelectable;
                    }
                    else if (labelSettings.m_strValueName == CGraphObjLine::c_strValueNameAngle) {
                        uFlags = uFlags & ~Qt::ItemIsSelectable;
                    }
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
/*! @brief Slot method connected to the drawingSizeChanged signal of the drawingScene.

*/
void CModelGraphObjGeometry::onDrawingSceneDrawingSizeChanged(const CDrawingSize& i_drawingSize)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_drawingSize.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingSceneDrawingSizeChanged",
        /* strAddInfo   */ strMthInArgs );

    updateXYValueSizeHint();
}

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

            CGraphObjLine* pGraphObjLine = nullptr;
            if (m_pGraphObj != nullptr) {
                pGraphObjLine = dynamic_cast<CGraphObjLine*>(m_pGraphObj);
            }
            if (pGraphObjLine != nullptr) {
                QList<SLabelSettings> arLabelSettings = getLabelSettings(m_pGraphObj);
                if (arLabelSettings.size() != m_arLabelSettings.size()) {
                    throw CException(__FILE__, __LINE__, EResultInternalProgramError);
                }
                bool bContentChanged = false;
                QVector<bool> arbRowsChanged(arLabelSettings.size(), false);
                for (int iRow = 0; iRow < arLabelSettings.size(); ++iRow) {
                    if (arLabelSettings[iRow] != m_arLabelSettings[iRow]) {
                        m_arLabelSettings[iRow] = arLabelSettings[iRow];
                        arbRowsChanged[iRow] = true;
                        bContentChanged = true;
                    }
                }
                const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
                CPhysValLine physValLine = pGraphObjLine->getLine(drawingSize.unit());
                if (m_eDimensionUnit == EScaleDimensionUnit::Pixels) {
                    if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Metric) {
                        // No simple unit conversion is possible here. The Y Scale Axis may
                        // be oriented from top to bottom or bottom to top.
                        // To get the correct scene coordinates we must let the drawing scene
                        // convert the coordinates into pixel values.
                        physValLine = m_pDrawingScene->convert(physValLine, Units.Length.px);
                    }
                }
                if (physValLine != m_physValLine) {
                    m_physValLine = physValLine;
                    arbRowsChanged.fill(true);
                    bContentChanged = true;
                }
                if (bContentChanged) {
                    for (int iRow = 0; iRow < arbRowsChanged.size(); ++iRow) {
                        QModelIndex modelIdxTL = index(iRow, 0);
                        QModelIndex modelIdxBR = index(iRow, EColumnCount-1);
                        emit_dataChanged(modelIdxTL, modelIdxBR);
                    }
                    if (m_iContentChangedSignalBlockedCounter > 0) {
                        m_bContentChanged = true;
                    }
                    else {
                        emit_contentChanged();
                    }
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
void CModelGraphObjGeometry::clearModel()
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
void CModelGraphObjGeometry::fillModel()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "fillModel",
        /* strMethodInArgs    */ "" );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();

    CGraphObjLine* pGraphObjLine = nullptr;
    if (m_pGraphObj != nullptr) {
        pGraphObjLine = dynamic_cast<CGraphObjLine*>(m_pGraphObj);
    }

    if (pGraphObjLine == nullptr) {
        m_physValLine = CPhysValLine(*m_pDrawingScene);
    }
    else {
        m_physValLine = pGraphObjLine->getLine(drawingSize.unit());
    }
    if (m_eDimensionUnit == EScaleDimensionUnit::Pixels) {
        if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Metric) {
            // No simple unit conversion is possible here. The Y Scale Axis may
            // be oriented from top to bottom or bottom to top.
            // To get the correct scene coordinates we must let the drawing scene
            // convert the coordinates into pixel values.
            m_physValLine = m_pDrawingScene->convert(m_physValLine, Units.Length.px);
        }
    }

    if (m_arLabelSettings.size() > 0) {
        clearModel();
    }
    if (m_pGraphObj != nullptr) {
        m_arLabelSettings = getLabelSettings(m_pGraphObj);
        if (m_arLabelSettings.size() > 0) {
            _beginInsertRows(QModelIndex(), 0, m_arLabelSettings.size()-1);
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
QList<CModelGraphObjGeometry::SLabelSettings> CModelGraphObjGeometry::getLabelSettings(CGraphObj* i_pGraphObj) const
//------------------------------------------------------------------------------
{
    QList<SLabelSettings> arLabelSettings;
    CUnit unit = m_pDrawingScene->drawingSize().unit();
    if (m_eDimensionUnit == EScaleDimensionUnit::Pixels) {
        unit = Units.Length.px;
    }
    if (i_pGraphObj != nullptr) {
        QStringList strlstValueNames = i_pGraphObj->getValueNames();
        if (strlstValueNames.size() > 0) {
            for (const QString& strName : strlstValueNames) {
                arLabelSettings.append(
                    SLabelSettings::fromGraphObj(i_pGraphObj, strName, arLabelSettings.size()));
            }
        }
    }
    return arLabelSettings;
}

//------------------------------------------------------------------------------
void CModelGraphObjGeometry::updateXYValueSizeHint()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "updateXYValueSizeHint",
        /* strMethodInArgs    */ "" );

    CPhysVal physValWidth;
    CPhysVal physValHeight;
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    if (m_eDimensionUnit == EScaleDimensionUnit::Metric) {
        physValWidth = drawingSize.metricImageWidth();
        physValHeight = drawingSize.metricImageHeight();
    }
    else {
        physValWidth = CPhysVal(drawingSize.imageWidthInPixels(), Units.Length.px, drawingSize.imageCoorsResolutionInPx());
        physValHeight = CPhysVal(drawingSize.imageHeightInPixels(), Units.Length.px, drawingSize.imageCoorsResolutionInPx());
    }
    CPhysVal physValMax = physValWidth > physValHeight ? physValWidth : physValHeight;
    m_strXYValSizeHint = physValMax.toString();
    if (m_eDimensionUnit == EScaleDimensionUnit::Pixels) {
        // For length and angle one decimal point is used.
        m_strXYValSizeHint += ".0";
    }

    QSize size = m_fontMetrics.boundingRect(m_strXYValSizeHint).size();
    size.setWidth(size.width() + 40); // Add additional space for spin box arrows
    if (m_sizeXYValSizeHint != size) {
        m_sizeXYValSizeHint = size;
        QModelIndex modelIdxTL = index(0, EColumnXVal);
        QModelIndex modelIdxBR = index(m_arLabelSettings.size()-1, EColumnYVal);
        emit_dataChanged(modelIdxTL, modelIdxBR);
    }
}

/*==============================================================================
protected: // instance methods (tracing emitting signals)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Emits the contentChanged signal and resets the content changed flags.
*/
void CModelGraphObjGeometry::emit_contentChanged()
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
