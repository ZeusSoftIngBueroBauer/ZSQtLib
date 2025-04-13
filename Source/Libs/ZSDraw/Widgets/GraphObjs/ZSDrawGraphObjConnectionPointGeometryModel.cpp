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

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjConnectionPointGeometryModel.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjConnectionPoint.h"
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
class ZSDRAWDLL_API CModelGraphObjConnectionPointGeometry : publicQAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
QString CModelGraphObjConnectionPointGeometry::column2Str(int i_clm)
//------------------------------------------------------------------------------
{
    static QHash<int, QString> s_hshClm2Str {
        { EColumnName, "Name"},
        { EColumnXVal, "XVal"},
        { EColumnYVal, "YVal"},
        { EColumnShowVals, "ShowVals"},
        { EColumnShowLine, "ShowLine"}
    };
    return s_hshClm2Str.value(i_clm, "? (" + QString::number(i_clm) + ")");
}

//------------------------------------------------------------------------------
QString CModelGraphObjConnectionPointGeometry::itemDataRole2Str(int i_iRole)
//------------------------------------------------------------------------------
{
    static QHash<int, QString> s_hshRole2Str {
        { ERoleMinimumValue, "MinimumValue"},
        { ERoleMaximumValue, "MaximumValue"}
    };
    QString str;
    if (s_hshRole2Str.contains(i_iRole)) {
        str = s_hshRole2Str.value(i_iRole);
    }
    else {
        str = qItemDataRole2Str(i_iRole);
    }
    return str;
}

//------------------------------------------------------------------------------
QString CModelGraphObjConnectionPointGeometry::modelIndex2Str(const QModelIndex& i_modelIdx) const
//------------------------------------------------------------------------------
{
    QString str;
    if (!i_modelIdx.isValid()) {
        str = "Invalid";
    }
    else {
        if (i_modelIdx.row() < m_arLabelSettings.size()) {
            str = "Row: " + m_arLabelSettings[i_modelIdx.row()].m_strValueName;
        }
        else {
            str = "Row: " + QString::number(i_modelIdx.row());
        }
        str += ", Clm: " + column2Str(i_modelIdx.column());
    }
    return str;
}

/*==============================================================================
protected: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Fills the label struct with the information retrieved from the graphical object.
*/
CModelGraphObjConnectionPointGeometry::SLabelSettings CModelGraphObjConnectionPointGeometry::SLabelSettings::fromGraphObj(
    CGraphObj* i_pGraphObj, const QString& i_strValueName, int i_iRowIdx)
//------------------------------------------------------------------------------
{
    SLabelSettings labelSettings;
    // Width and Height are combined into one Row (Size with Width and Height)
    labelSettings.m_strValueName = i_strValueName;
    labelSettings.m_iRowIdx = i_iRowIdx;
    labelSettings.m_bVisible = i_pGraphObj->isGeometryLabelVisible(i_strValueName);
    labelSettings.m_bLineVisible = i_pGraphObj->isGeometryLabelAnchorLineVisible(i_strValueName);
    return labelSettings;
}

//------------------------------------------------------------------------------
CModelGraphObjConnectionPointGeometry::SLabelSettings::SLabelSettings() :
//------------------------------------------------------------------------------
    m_strValueName(), m_iRowIdx(-1),
    m_bVisible(false), m_bLineVisible(false)
{
}

//------------------------------------------------------------------------------
CModelGraphObjConnectionPointGeometry::SLabelSettings::SLabelSettings(
    const QString& i_strValueName, int i_iRowIdx,
    const CPhysVal& i_physValX, const CPhysVal& i_physValY,
    bool i_bVisible, bool i_bLineVisible) :
//------------------------------------------------------------------------------
    m_strValueName(), m_iRowIdx(-1),
    m_bVisible(false), m_bLineVisible(false)
{
}

//------------------------------------------------------------------------------
bool CModelGraphObjConnectionPointGeometry::SLabelSettings::operator == (const SLabelSettings& i_other) const
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
bool CModelGraphObjConnectionPointGeometry::SLabelSettings::operator != (const SLabelSettings& i_other) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Model for the geometry values of a line.

    @param [in] i_pDrawingScene
        Pointer to drawing scene.
    @param [in] i_strNameSpace
        Namespace to be used for the class in the trace admin object tree
        (e.g. "ZS::Draw::Widgets::GraphObjs").
    @param [in] i_strGraphObjType
        Graphical object type to be used for the class in the trace admin object tree.
        (e.g. "StandardShapes::Line").
    @param [in] i_strObjName
        Identifies the usage of the model (e.g. in a dialog or in the objects widget stack).
    @param [in] i_eDimensionUnit
        If not set (invalid) the values are indicated in the current dimension
        unit of the drawing scene.
        If set to Metric the values are always shown in metrics unit. If the drawing
        scene's unit is set to pixel the model cannot be used and the corresponding
        table view got to be hidden.
        If set to Pixels the values are always shown in pixel coordinates. If the drawing
        scene's unit is a metric unit the values will be converted to pixel values.
    @param [in] i_pObjParent
        Parent object.
*/
CModelGraphObjConnectionPointGeometry::CModelGraphObjConnectionPointGeometry(
    CDrawingScene* i_pDrawingScene,
    const QString& i_strNameSpace,
    const QString& i_strGraphObjType,
    const QString& i_strObjName,
    const CEnumScaleDimensionUnit& i_eDimensionUnit,
    QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    QAbstractTableModel(i_pObjParent),
    m_pDrawingScene(i_pDrawingScene),
    m_eDimensionUnit(i_eDimensionUnit),
    m_font(),
    m_fontMetrics(m_font),
    m_strKeyInTree(),
    m_pGraphObjConnectionPoint(nullptr),
    m_drawingSize(i_pDrawingScene->drawingSize()),
    m_physValRect(*i_pDrawingScene),
    m_arLabelSettings(),
    m_strXYValSizeHint("1024 px +-"),
    m_sizeXYValSizeHint(),
    m_bContentChanged(false),
    m_iContentChangedSignalBlockedCounter(0),
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjNoisyMethods(nullptr)
{
    setObjectName(i_strObjName + QString(m_eDimensionUnit.isValid() ? m_eDimensionUnit.toString() : ""));

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

    QObject::connect(
        m_pDrawingScene, &CDrawingScene::drawingSizeChanged,
        this, &CModelGraphObjConnectionPointGeometry::onDrawingSceneDrawingSizeChanged);

} // ctor

//------------------------------------------------------------------------------
CModelGraphObjConnectionPointGeometry::~CModelGraphObjConnectionPointGeometry()
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
    m_pGraphObjConnectionPoint = nullptr;
    //m_drawingSize;
    //m_physValRect;
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
void CModelGraphObjConnectionPointGeometry::setFont(const QFont& i_font)
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
QFont CModelGraphObjConnectionPointGeometry::font() const
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
bool CModelGraphObjConnectionPointGeometry::setKeyInTree(const QString& i_strKeyInTree)
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

        if (m_pGraphObjConnectionPoint != nullptr) {
            QObject::disconnect(
                m_pGraphObjConnectionPoint, &CGraphObj::geometryOnSceneChanged,
                this, &CModelGraphObjConnectionPointGeometry::onGraphObjGeometryOnSceneChanged);
            //QObject::disconnect(
            //    m_pGraphObjConnectionPoint, &CGraphObj::geometryValuesUnitChanged,
            //    this, &CModelGraphObjConnectionPointGeometry::onGraphObjGeometryValuesUnitChanged);
            QObject::disconnect(
                m_pGraphObjConnectionPoint, &CGraphObj::geometryLabelChanged,
                this, &CModelGraphObjConnectionPointGeometry::onGraphObjGeometryLabelChanged);
            QObject::disconnect(
                m_pGraphObjConnectionPoint, &CGraphObj::aboutToBeDestroyed,
                this, &CModelGraphObjConnectionPointGeometry::onGraphObjAboutToBeDestroyed);
        }

        m_strKeyInTree = i_strKeyInTree;

        if (m_strKeyInTree.isEmpty()) {
            m_pGraphObjConnectionPoint = nullptr;
        }
        else {
            m_pGraphObjConnectionPoint = dynamic_cast<CGraphObjConnectionPoint*>(m_pDrawingScene->findGraphObj(i_strKeyInTree));
        }

        if (m_pGraphObjConnectionPoint != nullptr) {
            QObject::connect(
                m_pGraphObjConnectionPoint, &CGraphObj::geometryOnSceneChanged,
                this, &CModelGraphObjConnectionPointGeometry::onGraphObjGeometryOnSceneChanged);
            //QObject::connect(
            //    m_pGraphObjConnectionPoint, &CGraphObj::geometryValuesUnitChanged,
            //    this, &CModelGraphObjConnectionPointGeometry::onGraphObjGeometryValuesUnitChanged);
            QObject::connect(
                m_pGraphObjConnectionPoint, &CGraphObj::geometryLabelChanged,
                this, &CModelGraphObjConnectionPointGeometry::onGraphObjGeometryLabelChanged);
            QObject::connect(
                m_pGraphObjConnectionPoint, &CGraphObj::aboutToBeDestroyed,
                this, &CModelGraphObjConnectionPointGeometry::onGraphObjAboutToBeDestroyed);
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
QString CModelGraphObjConnectionPointGeometry::getKeyInTree() const
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
bool CModelGraphObjConnectionPointGeometry::hasErrors() const
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
bool CModelGraphObjConnectionPointGeometry::hasChanges() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hasChanges",
        /* strAddInfo   */ "" );

    bool bHasChanges = false;
    if (m_pGraphObjConnectionPoint != nullptr) {
        const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
        CPhysValRect physValRect = m_pGraphObjConnectionPoint->getRect(drawingSize.unit());
        bHasChanges = (m_physValRect != physValRect);
        if (!bHasChanges) {
            QList<SLabelSettings> arLabelSettings = getLabelSettings(m_pGraphObjConnectionPoint);
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
void CModelGraphObjConnectionPointGeometry::acceptChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "acceptChanges",
        /* strAddInfo   */ "" );

    if (m_pGraphObjConnectionPoint != nullptr && !hasErrors())
    {
        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

            if (m_pGraphObjConnectionPoint != nullptr) {
                m_pGraphObjConnectionPoint->setRect(m_physValRect);
                for (const SLabelSettings& labelSettings : m_arLabelSettings) {
                    labelSettings.m_bVisible ?
                        m_pGraphObjConnectionPoint->showGeometryLabel(labelSettings.m_strValueName) :
                        m_pGraphObjConnectionPoint->hideGeometryLabel(labelSettings.m_strValueName);
                    labelSettings.m_bLineVisible ?
                        m_pGraphObjConnectionPoint->showGeometryLabelAnchorLine(labelSettings.m_strValueName) :
                        m_pGraphObjConnectionPoint->hideGeometryLabelAnchorLine(labelSettings.m_strValueName);
                }
                m_bContentChanged = true;
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
void CModelGraphObjConnectionPointGeometry::rejectChanges()
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
int CModelGraphObjConnectionPointGeometry::getLabelRowIndex(const QString& i_strName) const
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
QStringList CModelGraphObjConnectionPointGeometry::labelNames() const
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
int CModelGraphObjConnectionPointGeometry::rowCount(const QModelIndex& i_modelIdxParent) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdxParent {" + modelIndex2Str(i_modelIdxParent) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "rowCount",
        /* strMethodInArgs    */ strMthInArgs );

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(static_cast<int>(m_arLabelSettings.size()));
    }
    return m_arLabelSettings.size();
}

//------------------------------------------------------------------------------
int CModelGraphObjConnectionPointGeometry::columnCount(const QModelIndex& i_modelIdxParent) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdxParent {" + modelIndex2Str(i_modelIdxParent) + "}";
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
QVariant CModelGraphObjConnectionPointGeometry::data(const QModelIndex& i_modelIdx, int i_iRole) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + modelIndex2Str(i_modelIdx) + "}" +
                       ", Role: " + QString::number(i_iRole) + " (" + itemDataRole2Str(i_iRole) + ")";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "data",
        /* strMethodInArgs    */ strMthInArgs );

    QVariant varData;

    if (m_pGraphObjConnectionPoint != nullptr && i_modelIdx.isValid()) {
        int iRow = i_modelIdx.row();
        int iClm = i_modelIdx.column();
        if ((iRow >= 0) && (iRow < m_arLabelSettings.size())) {
            const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
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
                        if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameTopLeft) {
                            varData = m_physValRect.topLeft().x().toString();
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameTopRight) {
                            varData = m_physValRect.topRight().x().toString();
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameBottomRight) {
                            varData = m_physValRect.bottomRight().x().toString();
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameBottomLeft) {
                            varData = m_physValRect.bottomLeft().x().toString();
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameCenter) {
                            varData = m_physValRect.center().x().toString();
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameWidth) {
                            varData = m_physValRect.width().toString();
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameHeight) {
                            varData = m_physValRect.height().toString();
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameAngle) {
                            varData = m_physValRect.angle().toString();
                        }
                    }
                    else if (i_iRole == Qt::EditRole) {
                        if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameTopLeft) {
                            varData = QVariant::fromValue(m_physValRect.topLeft().x());
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameTopRight) {
                            varData = QVariant::fromValue(m_physValRect.topRight().x());
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameBottomRight) {
                            varData = QVariant::fromValue(m_physValRect.bottomRight().x());
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameBottomLeft) {
                            varData = QVariant::fromValue(m_physValRect.bottomLeft().x());
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameCenter) {
                            varData = QVariant::fromValue(m_physValRect.center().x());
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameWidth) {
                            varData = QVariant::fromValue(m_physValRect.width());
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameHeight) {
                            varData = QVariant::fromValue(m_physValRect.height());
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameAngle) {
                            varData = QVariant::fromValue(m_physValRect.angle());
                        }
                    }
                    else if (i_iRole == Qt::TextAlignmentRole) {
                        varData = static_cast<int>(Qt::AlignRight|Qt::AlignVCenter);
                    }
                    else if (i_iRole == Qt::SizeHintRole) {
                        varData = m_sizeXYValSizeHint;
                    }
                    else if (i_iRole == ERoleMinimumValue) {
                        if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameAngle) {
                            varData = 0.0;
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameWidth) {
                            if (m_eDimensionUnit == EScaleDimensionUnit::Metric) {
                                varData = -drawingSize.metricImageWidth().getVal();
                            }
                            else if (m_eDimensionUnit == EScaleDimensionUnit::Pixels) {
                                varData = -drawingSize.imageWidthInPixels();
                            }
                            // !m_eDimensionUnit.isValid()
                            else if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Metric) {
                                varData = -drawingSize.metricImageWidth().getVal();
                            }
                            else {
                                varData = -drawingSize.imageWidthInPixels();
                            }
                        }
                        if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameHeight) {
                            if (m_eDimensionUnit == EScaleDimensionUnit::Metric) {
                                varData = -drawingSize.metricImageHeight().getVal();
                            }
                            else if (m_eDimensionUnit == EScaleDimensionUnit::Pixels) {
                                varData = -drawingSize.imageHeightInPixels();
                            }
                            // !m_eDimensionUnit.isValid()
                            else if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Metric) {
                                varData = -drawingSize.metricImageHeight().getVal();
                            }
                            else {
                                varData = -drawingSize.imageHeightInPixels();
                            }
                        }
                        else {
                            if (m_eDimensionUnit == EScaleDimensionUnit::Metric) {
                                varData = 0.0;
                            }
                            else if (m_eDimensionUnit == EScaleDimensionUnit::Pixels) {
                                varData = 0;
                            }
                            // !m_eDimensionUnit.isValid()
                            else if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Metric) {
                                varData = 0.0;
                            }
                            else {
                                varData = 0;
                            }
                        }
                    }
                    else if (i_iRole == ERoleMaximumValue) {
                        if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameAngle) {
                            varData = 360.0;
                        }
                        else {
                            if (m_eDimensionUnit == EScaleDimensionUnit::Metric) {
                                varData = drawingSize.metricImageWidth().getVal();
                            }
                            else if (m_eDimensionUnit == EScaleDimensionUnit::Pixels) {
                                varData = drawingSize.imageWidthInPixels();
                            }
                            // !m_eDimensionUnit.isValid()
                            else if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Metric) {
                                varData = drawingSize.metricImageWidth().getVal();
                            }
                            else {
                                varData = drawingSize.imageWidthInPixels();
                            }
                        }
                    }
                    break;
                }
                case EColumnYVal: {
                    if (i_iRole == Qt::DisplayRole) {
                        if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameTopLeft) {
                            varData = m_physValRect.topLeft().y().toString();
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameTopRight) {
                            varData = m_physValRect.topRight().y().toString();
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameBottomRight) {
                            varData = m_physValRect.bottomRight().y().toString();
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameBottomLeft) {
                            varData = m_physValRect.bottomLeft().y().toString();
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameCenter) {
                            varData = m_physValRect.center().y().toString();
                        }
                    }
                    else if (i_iRole == Qt::EditRole) {
                        if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameTopLeft) {
                            varData = QVariant::fromValue(m_physValRect.topLeft().y());
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameTopRight) {
                            varData = QVariant::fromValue(m_physValRect.topRight().y());
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameBottomRight) {
                            varData = QVariant::fromValue(m_physValRect.bottomRight().y());
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameBottomLeft) {
                            varData = QVariant::fromValue(m_physValRect.bottomLeft().y());
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameCenter) {
                            varData = QVariant::fromValue(m_physValRect.center().y());
                        }
                    }
                    else if (i_iRole == Qt::TextAlignmentRole) {
                        varData = static_cast<int>(Qt::AlignRight|Qt::AlignVCenter);
                    }
                    else if (i_iRole == Qt::SizeHintRole) {
                        varData = m_sizeXYValSizeHint;
                    }
                    else if (i_iRole == ERoleMinimumValue) {
                        if (m_eDimensionUnit == EScaleDimensionUnit::Metric) {
                            varData = 0.0;
                        }
                        else if (m_eDimensionUnit == EScaleDimensionUnit::Pixels) {
                            varData = 0;
                        }
                        // !m_eDimensionUnit.isValid()
                        else if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Metric) {
                            varData = 0.0;
                        }
                        else {
                            varData = 0;
                        }
                    }
                    else if (i_iRole == ERoleMaximumValue) {
                        if (m_eDimensionUnit == EScaleDimensionUnit::Metric) {
                            varData = drawingSize.metricImageHeight().getVal();
                        }
                        else if (m_eDimensionUnit == EScaleDimensionUnit::Metric) {
                            varData = drawingSize.imageHeightInPixels();
                        }
                        // !m_eDimensionUnit.isValid()
                        else if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Metric) {
                            varData = drawingSize.metricImageHeight().getVal();
                        }
                        else {
                            varData = drawingSize.imageHeightInPixels();
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
bool CModelGraphObjConnectionPointGeometry::setData(
    const QModelIndex& i_modelIdx, const QVariant& i_varData, int i_iRole)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + modelIndex2Str(i_modelIdx) + "}" +
                       ", Role: " + itemDataRole2Str(i_iRole);
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

    if (m_pGraphObjConnectionPoint != nullptr && i_modelIdx.isValid()) {
        int iRow = i_modelIdx.row();
        int iClm = i_modelIdx.column();
        if ((iRow >= 0) && (iRow < m_arLabelSettings.size())) {
            QVector<bool> arbColumnsChanged(EColumnCount, false);
            SLabelSettings labelSettings = m_arLabelSettings[iRow];
            CPhysValRect physValRect = m_physValRect;
            switch (iClm) {
                case EColumnName: {
                    break;
                }
                case EColumnXVal: {
                    if (i_iRole == Qt::EditRole) {
                        if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameTopLeft) {
                            CPhysValPoint physValTopLeft = physValRect.topLeft();
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValTopLeft.setX(physVal);
                            physValRect.setTopLeft(physValTopLeft);
                            arbColumnsChanged[EColumnXVal] = true;
                            arbColumnsChanged[EColumnYVal] = true;
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameTopRight) {
                            CPhysValPoint physValTopRight = physValRect.topRight();
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValTopRight.setX(physVal);
                            physValRect.setTopRight(physValTopRight);
                            arbColumnsChanged[EColumnXVal] = true;
                            arbColumnsChanged[EColumnYVal] = true;
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameBottomRight) {
                            CPhysValPoint physValBottomRight = physValRect.bottomRight();
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValBottomRight.setX(physVal);
                            physValRect.setBottomRight(physValBottomRight);
                            arbColumnsChanged[EColumnXVal] = true;
                            arbColumnsChanged[EColumnYVal] = true;
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameBottomLeft) {
                            CPhysValPoint physValBottomLeft = physValRect.bottomLeft();
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValBottomLeft.setX(physVal);
                            physValRect.setBottomLeft(physValBottomLeft);
                            arbColumnsChanged[EColumnXVal] = true;
                            arbColumnsChanged[EColumnYVal] = true;
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameCenter) {
                            CPhysValPoint physValPtCenter = physValRect.center();
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValPtCenter.setX(physVal);
                            physValRect.setCenter(physValPtCenter);
                            arbColumnsChanged[EColumnXVal] = true;
                            arbColumnsChanged[EColumnYVal] = true;
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameWidth) {
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValRect.setWidth(physVal);
                            arbColumnsChanged[EColumnXVal] = true;
                            arbColumnsChanged[EColumnYVal] = true;
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameHeight) {
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValRect.setHeight(physVal);
                            arbColumnsChanged[EColumnXVal] = true;
                            arbColumnsChanged[EColumnYVal] = true;
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameAngle) {
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValRect.setAngle(physVal);
                            arbColumnsChanged[EColumnXVal] = true;
                            arbColumnsChanged[EColumnYVal] = true;
                        }
                        bDataSet = true;
                    }
                    break;
                }
                case EColumnYVal: {
                    if (i_iRole == Qt::EditRole) {
                        if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameTopLeft) {
                            CPhysValPoint physValTopLeft = physValRect.topLeft();
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValTopLeft.setY(physVal);
                            physValRect.setTopLeft(physValTopLeft);
                            arbColumnsChanged[EColumnXVal] = true;
                            arbColumnsChanged[EColumnYVal] = true;
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameTopRight) {
                            CPhysValPoint physValTopRight = physValRect.topRight();
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValTopRight.setY(physVal);
                            physValRect.setTopRight(physValTopRight);
                            arbColumnsChanged[EColumnXVal] = true;
                            arbColumnsChanged[EColumnYVal] = true;
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameBottomRight) {
                            CPhysValPoint physValBottomRight = physValRect.bottomRight();
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValBottomRight.setY(physVal);
                            physValRect.setBottomRight(physValBottomRight);
                            arbColumnsChanged[EColumnXVal] = true;
                            arbColumnsChanged[EColumnYVal] = true;
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameBottomLeft) {
                            CPhysValPoint physValBottomLeft = physValRect.bottomLeft();
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValBottomLeft.setY(physVal);
                            physValRect.setBottomLeft(physValBottomLeft);
                            arbColumnsChanged[EColumnXVal] = true;
                            arbColumnsChanged[EColumnYVal] = true;
                        }
                        else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameCenter) {
                            CPhysValPoint physValPtCenter = physValRect.center();
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValPtCenter.setY(physVal);
                            physValRect.setCenter(physValPtCenter);
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
            if (m_physValRect != physValRect) {
                m_physValRect = physValRect;
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
QVariant CModelGraphObjConnectionPointGeometry::headerData(int i_iSection, Qt::Orientation i_orientation, int i_iRole) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Section: " + column2Str(i_iSection) + "}" +
                       ", Orientation: " + qOrientation2Str(i_orientation) +
                       ", Role: " + itemDataRole2Str(i_iRole);
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
Qt::ItemFlags CModelGraphObjConnectionPointGeometry::flags(const QModelIndex& i_modelIdx) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + modelIndex2Str(i_modelIdx) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "flags",
        /* strMethodInArgs    */ strMthInArgs );

    // The base class implementation returns a combination of flags that enables
    // the item (ItemIsEnabled) and allows it to be selected (ItemIsSelectable).
    Qt::ItemFlags uFlags = uFlags = QAbstractItemModel::flags(i_modelIdx);
    if (m_pGraphObjConnectionPoint != nullptr && i_modelIdx.isValid()) {
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
                    if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameTopLeft) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameTopRight) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameBottomRight) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameBottomLeft) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameCenter) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameWidth) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameHeight) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameAngle) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    break;
                }
                case EColumnYVal: {
                    if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameTopLeft) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameTopRight) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameBottomRight) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameBottomLeft) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    else if (labelSettings.m_strValueName == CGraphObj::c_strGeometryLabelNameCenter) {
                        uFlags = uFlags | Qt::ItemIsEditable;
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
void CModelGraphObjConnectionPointGeometry::onDrawingSceneDrawingSizeChanged(const CDrawingSize& i_drawingSize)
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

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    if (m_drawingSize != drawingSize) {
        m_drawingSize = drawingSize;
        updateXYValueSizeHint();
    }
}

////------------------------------------------------------------------------------
//void CModelGraphObjConnectionPointGeometry::onGraphObjGeometryValuesUnitChanged(CGraphObj* i_pGraphObj)
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = i_pGraphObj->keyInTree();
//    }
//    CMethodTracer mthTracer(
//        /* pTrcAdminObj       */ m_pTrcAdminObj,
//        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod          */ "onGraphObjGeometryValuesUnitChanged",
//        /* strMethodInArgs    */ strMthInArgs );
//
//    // When applying the changes from the model by invoking "applySettings"
//    // the ContentChangedSignalBlockedCounter is incremented to avoid that the
//    // "onGraphObj<Signal>" slots overwrite settings in the model which haven't been
//    // applied yet.
//    if (m_iContentChangedSignalBlockedCounter == 0)
//    {
//        bool bContentChanged = false;
//
//        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
//
//            if (m_pGraphObjConnectionPoint != nullptr) {
//                // The strings to indicate pixel values are always the same.
//                // When changing from pixel to metric dimension or if the metric dimension unit changes,
//                // the indicated value strings need to be updated to show the values in the new unit.
//                if (m_eDimensionUnit == EScaleDimensionUnit::Pixels) {
//                    CPhysValRect physValRect = m_pGraphObjConnectionPoint->getRect(Units.Length.px);
//                    if (physValRect != m_physValRect) {
//                        bContentChanged = true;
//                        m_physValRect = physValRect;
//                    }
//                }
//                else {
//                    CPhysValRect physValRect = m_pGraphObjConnectionPoint->getRect();
//                    if (physValRect != m_physValRect) {
//                        bContentChanged = true;
//                        m_physValRect = physValRect;
//                    }
//                }
//                if (bContentChanged) {
//                    QModelIndex modelIdxTL = index(0, EColumnXVal);
//                    QModelIndex modelIdxBR = index(m_arLabelSettings.size(), EColumnYVal);
//                    emit_dataChanged(modelIdxTL, modelIdxBR);
//                }
//            }
//        }
//
//        // If the "contentChanged" signal is no longer blocked and the content of
//        // properties widget has been changed ...
//        if (m_iContentChangedSignalBlockedCounter == 0 && bContentChanged) {
//            // .. emit the contentChanged signal and update the enabled state
//            // of the Apply and Reset buttons.
//            emit_contentChanged();
//        }
//        else {
//            m_bContentChanged = true;
//        }
//    }
//}

//------------------------------------------------------------------------------
void CModelGraphObjConnectionPointGeometry::onGraphObjGeometryOnSceneChanged(CGraphObj* i_pGraphObj)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pGraphObj->keyInTree();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onGraphObjGeometryOnSceneChanged",
        /* strMethodInArgs    */ strMthInArgs );

    // When applying the changes from the model by invoking "applySettings"
    // the ContentChangedSignalBlockedCounter is incremented to avoid that the
    // "onGraphObj<Signal>" slots overwrite settings in the model which haven't been
    // applied yet.
    if (m_iContentChangedSignalBlockedCounter == 0)
    {
        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

            if (m_pGraphObjConnectionPoint != nullptr) {
                const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
                CPhysValRect physValRect = m_pGraphObjConnectionPoint->getRect(drawingSize.unit());
                if (m_eDimensionUnit == EScaleDimensionUnit::Pixels) {
                    if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Metric) {
                        // No simple unit conversion is possible here. The Y Scale Axis may
                        // be oriented from top to bottom or bottom to top.
                        // To get the correct scene coordinates we must let the drawing scene
                        // convert the coordinates into pixel values.
                        physValRect = m_pDrawingScene->convert(physValRect, Units.Length.px);
                    }
                }
                bool bContentChanged = false;
                if (physValRect != m_physValRect) {
                    m_physValRect = physValRect;
                    bContentChanged = true;
                }
                if (bContentChanged) {
                    QModelIndex modelIdxTL = index(0, EColumnXVal);
                    QModelIndex modelIdxBR = index(m_arLabelSettings.size()-1, EColumnYVal);
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
void CModelGraphObjConnectionPointGeometry::onGraphObjGeometryLabelChanged(
    CGraphObj* i_pGraphObj, const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pGraphObj->keyInTree() + ", " + i_strName;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onGraphObjGeometryLabelChanged",
        /* strMethodInArgs    */ strMthInArgs );

    // When applying the changes from the model by invoking "applySettings"
    // the ContentChangedSignalBlockedCounter is incremented to avoid that the
    // "onGraphObj<Signal>" slots overwrite settings in the model which haven't been
    // applied yet.
    if (m_iContentChangedSignalBlockedCounter == 0)
    {
        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

            int iRow = getLabelRowIndex(i_strName);
            if (iRow >= 0 && iRow < m_arLabelSettings.size()) {
                SLabelSettings labelSettings = SLabelSettings::fromGraphObj(m_pGraphObjConnectionPoint, i_strName, iRow);
                if (m_arLabelSettings[iRow] != labelSettings) {
                    m_arLabelSettings[iRow] = labelSettings;
                    QModelIndex modelIdxTL = index(iRow, EColumnShowVals);
                    QModelIndex modelIdxBR = index(iRow, EColumnShowLine);
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
void CModelGraphObjConnectionPointGeometry::onGraphObjAboutToBeDestroyed(CGraphObj*)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onGraphObjAboutToBeDestroyed",
        /* strAddInfo   */ "" );

    m_pGraphObjConnectionPoint = nullptr;

    clearModel();
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelGraphObjConnectionPointGeometry::clearModel()
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
void CModelGraphObjConnectionPointGeometry::fillModel()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "fillModel",
        /* strMethodInArgs    */ "" );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();

    if (m_pGraphObjConnectionPoint == nullptr) {
        m_physValRect = CPhysValRect(*m_pDrawingScene);
    }
    else {
        m_physValRect = m_pGraphObjConnectionPoint->getRect(drawingSize.unit());
    }
    if (m_eDimensionUnit == EScaleDimensionUnit::Pixels) {
        if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Metric) {
            // No simple unit conversion is possible here. The Y Scale Axis may
            // be oriented from top to bottom or bottom to top.
            // To get the correct scene coordinates we must let the drawing scene
            // convert the coordinates into pixel values.
            m_physValRect = m_pDrawingScene->convert(m_physValRect, Units.Length.px);
        }
    }

    if (m_arLabelSettings.size() > 0) {
        clearModel();
    }
    if (m_pGraphObjConnectionPoint != nullptr) {
        m_arLabelSettings = getLabelSettings(m_pGraphObjConnectionPoint);
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
QList<CModelGraphObjConnectionPointGeometry::SLabelSettings> CModelGraphObjConnectionPointGeometry::getLabelSettings(CGraphObj* i_pGraphObj) const
//------------------------------------------------------------------------------
{
    QList<SLabelSettings> arLabelSettings;
    CUnit unit = m_pDrawingScene->drawingSize().unit();
    if (m_eDimensionUnit == EScaleDimensionUnit::Pixels) {
        unit = Units.Length.px;
    }
    if (i_pGraphObj != nullptr) {
        QStringList strlstValueNames = i_pGraphObj->getGeometryLabelNames();
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
void CModelGraphObjConnectionPointGeometry::updateXYValueSizeHint()
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
    else if (m_eDimensionUnit == EScaleDimensionUnit::Pixels) {
        physValWidth = CPhysVal(drawingSize.imageWidthInPixels(), Units.Length.px, drawingSize.imageCoorsResolutionInPx());
        physValHeight = CPhysVal(drawingSize.imageHeightInPixels(), Units.Length.px, drawingSize.imageCoorsResolutionInPx());
    }
    // !m_eDimensionUnit.isValid()
    else if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Metric) {
        physValWidth = drawingSize.metricImageWidth();
        physValHeight = drawingSize.metricImageHeight();
    }
    else {
        physValWidth = CPhysVal(drawingSize.imageWidthInPixels(), Units.Length.px, drawingSize.imageCoorsResolutionInPx());
        physValHeight = CPhysVal(drawingSize.imageHeightInPixels(), Units.Length.px, drawingSize.imageCoorsResolutionInPx());
    }
    CPhysVal physValMax = physValWidth > physValHeight ? physValWidth : physValHeight;
    m_strXYValSizeHint = physValMax.toString();
    if (m_eDimensionUnit.isValid()) {
        if( m_eDimensionUnit == EScaleDimensionUnit::Pixels ) {
            // For length and angle one decimal point is used.
            m_strXYValSizeHint += ".0";
        }
    }
    else if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
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
void CModelGraphObjConnectionPointGeometry::emit_contentChanged()
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
void CModelGraphObjConnectionPointGeometry::_beginInsertRows(
    const QModelIndex& i_modelIdxParent, int i_iRowFirst, int i_iRowLast)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent {" + modelIndex2Str(i_modelIdxParent) + "}" +
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
void CModelGraphObjConnectionPointGeometry::_endInsertRows()
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
void CModelGraphObjConnectionPointGeometry::_beginRemoveRows(
    const QModelIndex& i_modelIdxParent, int i_iRowFirst, int i_iRowLast)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Parent {" + modelIndex2Str(i_modelIdxParent) + "}" +
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
void CModelGraphObjConnectionPointGeometry::_endRemoveRows()
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
void CModelGraphObjConnectionPointGeometry::emit_dataChanged(
    const QModelIndex& i_modelIdxTL,
    const QModelIndex& i_modelIdxBR,
    const QVector<int>& i_ariRoles)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "TL {" + modelIndex2Str(i_modelIdxTL) + "}" +
                       ", BR {" + modelIndex2Str(i_modelIdxBR) + "}" +
                       ", Roles [" + QString::number(i_ariRoles.size()) + "]";
        if (i_ariRoles.size() > 0) {
            strMthInArgs += "(";
            for (int idxRole = 0; idxRole < i_ariRoles.size(); idxRole++) {
                if (idxRole > 0) strMthInArgs += ", ";
                strMthInArgs += QString::number(idxRole) + ": " + itemDataRole2Str(i_ariRoles[idxRole]);
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
