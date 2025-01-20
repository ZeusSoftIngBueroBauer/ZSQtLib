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

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjPolygonGeometryModel.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjPolygon.h"
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
class ZSDRAWDLL_API CModelGraphObjPolygonGeometry : publicQAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
QString CModelGraphObjPolygonGeometry::column2Str(int i_clm)
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
QString CModelGraphObjPolygonGeometry::itemDataRole2Str(int i_iRole)
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
QString CModelGraphObjPolygonGeometry::modelIndex2Str(const QModelIndex& i_modelIdx) const
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
CModelGraphObjPolygonGeometry::SLabelSettings CModelGraphObjPolygonGeometry::SLabelSettings::fromGraphObj(
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
CModelGraphObjPolygonGeometry::SLabelSettings::SLabelSettings() :
//------------------------------------------------------------------------------
    m_strValueName(), m_iRowIdx(-1),
    m_bVisible(false), m_bLineVisible(false)
{
}

//------------------------------------------------------------------------------
CModelGraphObjPolygonGeometry::SLabelSettings::SLabelSettings(
    const QString& i_strValueName, int i_iRowIdx,
    const CPhysVal& i_physValX, const CPhysVal& i_physValY,
    bool i_bVisible, bool i_bLineVisible) :
//------------------------------------------------------------------------------
    m_strValueName(), m_iRowIdx(-1),
    m_bVisible(false), m_bLineVisible(false)
{
}

//------------------------------------------------------------------------------
bool CModelGraphObjPolygonGeometry::SLabelSettings::operator == (const SLabelSettings& i_other) const
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
bool CModelGraphObjPolygonGeometry::SLabelSettings::operator != (const SLabelSettings& i_other) const
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
CModelGraphObjPolygonGeometry::CModelGraphObjPolygonGeometry(
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
    m_pGraphObjPolygon(nullptr),
    m_drawingSize(i_pDrawingScene->drawingSize()),
    m_physValPolygon(*i_pDrawingScene),
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
        this, &CModelGraphObjPolygonGeometry::onDrawingSceneDrawingSizeChanged);

} // ctor

//------------------------------------------------------------------------------
CModelGraphObjPolygonGeometry::~CModelGraphObjPolygonGeometry()
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
    m_pGraphObjPolygon = nullptr;
    //m_drawingSize;
    //m_physValPolygon;
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
void CModelGraphObjPolygonGeometry::setFont(const QFont& i_font)
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
QFont CModelGraphObjPolygonGeometry::font() const
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
bool CModelGraphObjPolygonGeometry::setKeyInTree(const QString& i_strKeyInTree)
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

        if (m_pGraphObjPolygon != nullptr) {
            QObject::disconnect(
                m_pGraphObjPolygon, &CGraphObj::geometryOnSceneChanged,
                this, &CModelGraphObjPolygonGeometry::onGraphObjGeometryOnSceneChanged);
            //QObject::disconnect(
            //    m_pGraphObjPolygon, &CGraphObj::geometryValuesUnitChanged,
            //    this, &CModelGraphObjPolygonGeometry::onGraphObjGeometryValuesUnitChanged);
            QObject::disconnect(
                m_pGraphObjPolygon, &CGraphObj::geometryLabelChanged,
                this, &CModelGraphObjPolygonGeometry::onGraphObjGeometryLabelChanged);
            QObject::disconnect(
                m_pGraphObjPolygon, &CGraphObj::aboutToBeDestroyed,
                this, &CModelGraphObjPolygonGeometry::onGraphObjAboutToBeDestroyed);
        }

        m_strKeyInTree = i_strKeyInTree;

        if (m_strKeyInTree.isEmpty()) {
            m_pGraphObjPolygon = nullptr;
        }
        else {
            m_pGraphObjPolygon = dynamic_cast<CGraphObjPolygon*>(m_pDrawingScene->findGraphObj(i_strKeyInTree));
        }

        if (m_pGraphObjPolygon != nullptr) {
            QObject::connect(
                m_pGraphObjPolygon, &CGraphObj::geometryOnSceneChanged,
                this, &CModelGraphObjPolygonGeometry::onGraphObjGeometryOnSceneChanged);
            //QObject::connect(
            //    m_pGraphObjPolygon, &CGraphObj::geometryValuesUnitChanged,
            //    this, &CModelGraphObjPolygonGeometry::onGraphObjGeometryValuesUnitChanged);
            QObject::connect(
                m_pGraphObjPolygon, &CGraphObj::geometryLabelChanged,
                this, &CModelGraphObjPolygonGeometry::onGraphObjGeometryLabelChanged);
            QObject::connect(
                m_pGraphObjPolygon, &CGraphObj::aboutToBeDestroyed,
                this, &CModelGraphObjPolygonGeometry::onGraphObjAboutToBeDestroyed);
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
QString CModelGraphObjPolygonGeometry::getKeyInTree() const
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
bool CModelGraphObjPolygonGeometry::hasErrors() const
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
bool CModelGraphObjPolygonGeometry::hasChanges() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hasChanges",
        /* strAddInfo   */ "" );

    bool bHasChanges = false;
    if (m_pGraphObjPolygon != nullptr) {
        const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
        CPhysValPolygon physValPolygon = m_pGraphObjPolygon->getPolygon(drawingSize.unit());
        bHasChanges = (m_physValPolygon != physValPolygon);
        if (!bHasChanges) {
            QList<SLabelSettings> arLabelSettings = getLabelSettings(m_pGraphObjPolygon);
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
void CModelGraphObjPolygonGeometry::acceptChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "acceptChanges",
        /* strAddInfo   */ "" );

    if (m_pGraphObjPolygon != nullptr && !hasErrors())
    {
        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

            if (m_pGraphObjPolygon != nullptr) {
                m_pGraphObjPolygon->setPolygon(m_physValPolygon);
                for (const SLabelSettings& labelSettings : m_arLabelSettings) {
                    labelSettings.m_bVisible ?
                        m_pGraphObjPolygon->showGeometryLabel(labelSettings.m_strValueName) :
                        m_pGraphObjPolygon->hideGeometryLabel(labelSettings.m_strValueName);
                    labelSettings.m_bLineVisible ?
                        m_pGraphObjPolygon->showGeometryLabelAnchorLine(labelSettings.m_strValueName) :
                        m_pGraphObjPolygon->hideGeometryLabelAnchorLine(labelSettings.m_strValueName);
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
void CModelGraphObjPolygonGeometry::rejectChanges()
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
int CModelGraphObjPolygonGeometry::getLabelRowIndex(const QString& i_strLabelName) const
//------------------------------------------------------------------------------
{
    int iRowIdx = -1;
    for (const SLabelSettings& labelSettings : m_arLabelSettings) {
        if (i_strLabelName == labelSettings.m_strValueName) {
            iRowIdx = labelSettings.m_iRowIdx;
            break;
        }
    }
    return iRowIdx;
}

//------------------------------------------------------------------------------
/*! @brief Returns a list with the current names of the labels.
*/
QStringList CModelGraphObjPolygonGeometry::labelNames() const
//------------------------------------------------------------------------------
{
    QStringList strlstNames;
    for (const SLabelSettings& labelSettings : m_arLabelSettings) {
        strlstNames.append(labelSettings.m_strValueName);
    }
    return strlstNames;
}

//------------------------------------------------------------------------------
/*! @brief Returns a list with the current names of the labels.
*/
QString CModelGraphObjPolygonGeometry::getLabelName(int i_iRow) const
//------------------------------------------------------------------------------
{
    return m_arLabelSettings[i_iRow].m_strValueName;
}

//------------------------------------------------------------------------------
/*! @brief Returns true if the specified row contains the coordinates of a polygon point,
           false otherwise.

    @param [in] i_iRow
        Row number to be checked.
*/
bool CModelGraphObjPolygonGeometry::isPointRow(int i_iRow) const
//------------------------------------------------------------------------------
{
    return CGraphObjPolygon::isPolygonPointLabelName(m_arLabelSettings[i_iRow].m_strValueName);
}

//------------------------------------------------------------------------------
void CModelGraphObjPolygonGeometry::removePoint(const QString& i_strLabelName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strLabelName;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "removePoint",
        /* strMethodInArgs    */ strMthInArgs );

    int idxPt = CGraphObjPolygon::extractIndexFromPolygonPointLabelName(i_strLabelName);
    int iRow = getLabelRowIndex(i_strLabelName);
    _beginRemoveRows(QModelIndex(), iRow, iRow);
    m_physValPolygon.removeAt(idxPt);
    m_arLabelSettings.removeAt(iRow);
    _endRemoveRows();
    for (; iRow < m_arLabelSettings.size(); ++iRow, ++idxPt) {
        m_arLabelSettings[iRow].m_iRowIdx = iRow;
        m_arLabelSettings[iRow].m_strValueName = CGraphObjPolygon::createPolygonPointLabelName(idxPt);
        QModelIndex modelIdxTL = index(iRow, 0);
        QModelIndex modelIdxBR = index(iRow, EColumnCount-1);
        emit_dataChanged(modelIdxTL, modelIdxBR);
    }
    emit_contentChanged();
}

//------------------------------------------------------------------------------
void CModelGraphObjPolygonGeometry::insertPointBefore(const QString& i_strLabelName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strLabelName;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "insertPointBefore",
        /* strMethodInArgs    */ strMthInArgs );

    int idxPt = CGraphObjPolygon::extractIndexFromPolygonPointLabelName(i_strLabelName);
    int iRow = getLabelRowIndex(i_strLabelName);
    _beginInsertRows(QModelIndex(), iRow, iRow);
    m_physValPolygon.insert(idxPt, CPhysValPoint(*m_pDrawingScene));
    m_arLabelSettings.insert(iRow, SLabelSettings());
    m_arLabelSettings[iRow].m_iRowIdx = iRow;
    m_arLabelSettings[iRow].m_strValueName = CGraphObjPolygon::createPolygonPointLabelName(idxPt);
    _endInsertRows();
    for (++iRow, ++idxPt; iRow < m_arLabelSettings.size(); ++iRow, ++idxPt) {
        m_arLabelSettings[iRow].m_iRowIdx = iRow;
        m_arLabelSettings[iRow].m_strValueName = CGraphObjPolygon::createPolygonPointLabelName(idxPt);
        QModelIndex modelIdxTL = index(iRow, 0);
        QModelIndex modelIdxBR = index(iRow, EColumnCount-1);
        emit_dataChanged(modelIdxTL, modelIdxBR);
    }
    emit_contentChanged();
}

//------------------------------------------------------------------------------
void CModelGraphObjPolygonGeometry::insertPointAfter(const QString& i_strLabelName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strLabelName;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "insertPointAfter",
        /* strMethodInArgs    */ strMthInArgs );

    int idxPt = CGraphObjPolygon::extractIndexFromPolygonPointLabelName(i_strLabelName) + 1;
    int iRow = getLabelRowIndex(i_strLabelName) + 1;
    _beginInsertRows(QModelIndex(), iRow, iRow);
    m_physValPolygon.insert(idxPt, CPhysValPoint(*m_pDrawingScene));
    m_arLabelSettings[iRow].m_iRowIdx = iRow;
    m_arLabelSettings[iRow].m_strValueName = CGraphObjPolygon::createPolygonPointLabelName(idxPt);
    _endInsertRows();
    for (++iRow, ++idxPt; iRow < m_arLabelSettings.size(); ++iRow, ++idxPt) {
        m_arLabelSettings[iRow].m_iRowIdx = iRow;
        m_arLabelSettings[iRow].m_strValueName = CGraphObjPolygon::createPolygonPointLabelName(idxPt);
        QModelIndex modelIdxTL = index(iRow, 0);
        QModelIndex modelIdxBR = index(iRow, EColumnCount-1);
        emit_dataChanged(modelIdxTL, modelIdxBR);
    }
    emit_contentChanged();
}

/*==============================================================================
public: // must overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelGraphObjPolygonGeometry::rowCount(const QModelIndex& i_modelIdxParent) const
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
int CModelGraphObjPolygonGeometry::columnCount(const QModelIndex& i_modelIdxParent) const
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
QVariant CModelGraphObjPolygonGeometry::data(const QModelIndex& i_modelIdx, int i_iRole) const
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

    if (m_pGraphObjPolygon != nullptr && i_modelIdx.isValid()) {
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
                        if (labelSettings.m_strValueName == CGraphObjPolygon::c_strGeometryLabelNameCenter) {
                            varData = m_physValPolygon.center().x().toString();
                        }
                        else if (labelSettings.m_strValueName == CGraphObjPolygon::c_strGeometryLabelNameWidth) {
                            varData = m_physValPolygon.width().toString();
                        }
                        else if (labelSettings.m_strValueName == CGraphObjPolygon::c_strGeometryLabelNameHeight) {
                            varData = m_physValPolygon.height().toString();
                        }
                        else if (labelSettings.m_strValueName == CGraphObjPolygon::c_strGeometryLabelNameAngle) {
                            varData = m_physValPolygon.angle().toString();
                        }
                        else if (labelSettings.m_strValueName.startsWith(CGraphObjPolygon::c_strGeometryLabelNameP)) {
                            int idxPt = CGraphObjPolygon::extractIndexFromPolygonPointLabelName(labelSettings.m_strValueName);
                            varData = m_physValPolygon.at(idxPt).x().toString();
                        }
                    }
                    else if (i_iRole == Qt::EditRole) {
                        if (labelSettings.m_strValueName == CGraphObjPolygon::c_strGeometryLabelNameCenter) {
                            varData = QVariant::fromValue(m_physValPolygon.center().x());
                        }
                        else if (labelSettings.m_strValueName == CGraphObjPolygon::c_strGeometryLabelNameWidth) {
                            varData = QVariant::fromValue(m_physValPolygon.width());
                        }
                        else if (labelSettings.m_strValueName == CGraphObjPolygon::c_strGeometryLabelNameHeight) {
                            varData = QVariant::fromValue(m_physValPolygon.height());
                        }
                        else if (labelSettings.m_strValueName == CGraphObjPolygon::c_strGeometryLabelNameAngle) {
                            varData = QVariant::fromValue(m_physValPolygon.angle());
                        }
                        else if (labelSettings.m_strValueName.startsWith(CGraphObjPolygon::c_strGeometryLabelNameP)) {
                            int idxPt = CGraphObjPolygon::extractIndexFromPolygonPointLabelName(labelSettings.m_strValueName);
                            varData = QVariant::fromValue(m_physValPolygon.at(idxPt).x());
                        }
                    }
                    else if (i_iRole == Qt::TextAlignmentRole) {
                        varData = static_cast<int>(Qt::AlignRight|Qt::AlignVCenter);
                    }
                    else if (i_iRole == Qt::SizeHintRole) {
                        varData = m_sizeXYValSizeHint;
                    }
                    else if (i_iRole == ERoleMinimumValue) {
                        if (labelSettings.m_strValueName == CGraphObjPolygon::c_strGeometryLabelNameAngle) {
                            varData = 0.0;
                        }
                        else if (labelSettings.m_strValueName == CGraphObjPolygon::c_strGeometryLabelNameWidth) {
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
                        if (labelSettings.m_strValueName == CGraphObjPolygon::c_strGeometryLabelNameHeight) {
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
                        if (labelSettings.m_strValueName == CGraphObjPolygon::c_strGeometryLabelNameAngle) {
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
                        if (labelSettings.m_strValueName == CGraphObjPolygon::c_strGeometryLabelNameCenter) {
                            varData = m_physValPolygon.center().y().toString();
                        }
                        else if (labelSettings.m_strValueName.startsWith(CGraphObjPolygon::c_strGeometryLabelNameP)) {
                            int idxPt = CGraphObjPolygon::extractIndexFromPolygonPointLabelName(labelSettings.m_strValueName);
                            varData = m_physValPolygon.at(idxPt).y().toString();
                        }
                    }
                    else if (i_iRole == Qt::EditRole) {
                        if (labelSettings.m_strValueName == CGraphObjPolygon::c_strGeometryLabelNameCenter) {
                            varData = QVariant::fromValue(m_physValPolygon.center().y());
                        }
                        else if (labelSettings.m_strValueName.startsWith(CGraphObjPolygon::c_strGeometryLabelNameP)) {
                            int idxPt = CGraphObjPolygon::extractIndexFromPolygonPointLabelName(labelSettings.m_strValueName);
                            varData = QVariant::fromValue(m_physValPolygon.at(idxPt).y());
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
bool CModelGraphObjPolygonGeometry::setData(
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

    if (m_pGraphObjPolygon != nullptr && i_modelIdx.isValid()) {
        int iRow = i_modelIdx.row();
        int iClm = i_modelIdx.column();
        if ((iRow >= 0) && (iRow < m_arLabelSettings.size())) {
            QVector<bool> arbColumnsChanged(EColumnCount, false);
            SLabelSettings labelSettings = m_arLabelSettings[iRow];
            CPhysValPolygon physValPolygon = m_physValPolygon;
            switch (iClm) {
                case EColumnName: {
                    break;
                }
                case EColumnXVal: {
                    if (i_iRole == Qt::EditRole) {
                        if (labelSettings.m_strValueName == CGraphObjPolygon::c_strGeometryLabelNameCenter) {
                            CPhysValPoint physValPtCenter = physValPolygon.center();
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValPtCenter.setX(physVal);
                            physValPolygon.setCenter(physValPtCenter);
                            arbColumnsChanged[EColumnXVal] = true;
                            arbColumnsChanged[EColumnYVal] = true;
                        }
                        else if (labelSettings.m_strValueName == CGraphObjPolygon::c_strGeometryLabelNameWidth) {
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValPolygon.setWidth(physVal);
                            arbColumnsChanged[EColumnXVal] = true;
                            arbColumnsChanged[EColumnYVal] = true;
                        }
                        else if (labelSettings.m_strValueName == CGraphObjPolygon::c_strGeometryLabelNameHeight) {
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValPolygon.setHeight(physVal);
                            arbColumnsChanged[EColumnXVal] = true;
                            arbColumnsChanged[EColumnYVal] = true;
                        }
                        else if (labelSettings.m_strValueName == CGraphObjPolygon::c_strGeometryLabelNameAngle) {
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValPolygon.setAngle(physVal);
                            arbColumnsChanged[EColumnXVal] = true;
                            arbColumnsChanged[EColumnYVal] = true;
                        }
                        else if (labelSettings.m_strValueName.startsWith(CGraphObjPolygon::c_strGeometryLabelNameP)) {
                            int idxPt = CGraphObjPolygon::extractIndexFromPolygonPointLabelName(labelSettings.m_strValueName);
                            CPhysValPoint physValPt = physValPolygon.at(idxPt);
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValPt.setX(physVal);
                            physValPolygon.replace(idxPt, physValPt);
                            arbColumnsChanged[EColumnXVal] = true;
                            arbColumnsChanged[EColumnYVal] = true;
                        }
                        bDataSet = true;
                    }
                    break;
                }
                case EColumnYVal: {
                    if (i_iRole == Qt::EditRole) {
                        if (labelSettings.m_strValueName == CGraphObjPolygon::c_strGeometryLabelNameCenter) {
                            CPhysValPoint physValPtCenter = physValPolygon.center();
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValPtCenter.setY(physVal);
                            physValPolygon.setCenter(physValPtCenter);
                            arbColumnsChanged[EColumnXVal] = true;
                            arbColumnsChanged[EColumnYVal] = true;
                        }
                        else if (labelSettings.m_strValueName.startsWith(CGraphObjPolygon::c_strGeometryLabelNameP)) {
                            int idxPt = CGraphObjPolygon::extractIndexFromPolygonPointLabelName(labelSettings.m_strValueName);
                            CPhysValPoint physValPt = physValPolygon.at(idxPt);
                            CPhysVal physVal = i_varData.value<CPhysVal>();
                            physValPt.setY(physVal);
                            physValPolygon.replace(idxPt, physValPt);
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
            if (m_physValPolygon != physValPolygon) {
                m_physValPolygon = physValPolygon;
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
QVariant CModelGraphObjPolygonGeometry::headerData(int i_iSection, Qt::Orientation i_orientation, int i_iRole) const
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
Qt::ItemFlags CModelGraphObjPolygonGeometry::flags(const QModelIndex& i_modelIdx) const
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
    if (m_pGraphObjPolygon != nullptr && i_modelIdx.isValid()) {
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
                    if (labelSettings.m_strValueName == CGraphObjPolygon::c_strGeometryLabelNameCenter) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    else if (labelSettings.m_strValueName == CGraphObjPolygon::c_strGeometryLabelNameWidth) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    else if (labelSettings.m_strValueName == CGraphObjPolygon::c_strGeometryLabelNameHeight) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    else if (labelSettings.m_strValueName == CGraphObjPolygon::c_strGeometryLabelNameAngle) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    else if (labelSettings.m_strValueName.startsWith(CGraphObjPolygon::c_strGeometryLabelNameP)) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    break;
                }
                case EColumnYVal: {
                    if (labelSettings.m_strValueName == CGraphObjPolygon::c_strGeometryLabelNameCenter) {
                        uFlags = uFlags | Qt::ItemIsEditable;
                    }
                    else if (labelSettings.m_strValueName.startsWith(CGraphObjPolygon::c_strGeometryLabelNameP)) {
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
void CModelGraphObjPolygonGeometry::onDrawingSceneDrawingSizeChanged(const CDrawingSize& i_drawingSize)
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
//void CModelGraphObjPolygonGeometry::onGraphObjGeometryValuesUnitChanged(CGraphObj* i_pGraphObj)
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
//            if (m_pGraphObjPolygon != nullptr) {
//                // The strings to indicate pixel values are always the same.
//                // When changing from pixel to metric dimension or if the metric dimension unit changes,
//                // the indicated value strings need to be updated to show the values in the new unit.
//                if (m_eDimensionUnit == EScaleDimensionUnit::Pixels) {
//                    CPhysValPolygon physValPolygon = m_pGraphObjPolygon->getPolygon(Units.Length.px);
//                    if (physValPolygon != m_physValPolygon) {
//                        bContentChanged = true;
//                        m_physValPolygon = physValPolygon;
//                    }
//                }
//                else {
//                    CPhysValPolygon physValPolygon = m_pGraphObjPolygon->getPolygon();
//                    if (physValPolygon != m_physValPolygon) {
//                        bContentChanged = true;
//                        m_physValPolygon = physValPolygon;
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
void CModelGraphObjPolygonGeometry::onGraphObjGeometryOnSceneChanged(CGraphObj* i_pGraphObj)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pGraphObj->keyInTree();
        if (m_pGraphObjPolygon == i_pGraphObj) {
            strMthInArgs += " [" + QString::number(m_pGraphObjPolygon->count()) + "]";
        }
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onGraphObjGeometryOnSceneChanged",
        /* strMethodInArgs    */ strMthInArgs );
    
    if (m_pGraphObjPolygon != i_pGraphObj) {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError, "i_pGraphObj != m_pGraphObjPolygon");
    }
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        QString strRuntimeInfo = "-+ Polygon [" + QString::number(m_physValPolygon.count()) + "]";
        mthTracer.trace(strRuntimeInfo);
    }
    // When applying the changes from the model by invoking "applySettings"
    // the ContentChangedSignalBlockedCounter is incremented to avoid that the
    // "onGraphObj<Signal>" slots overwrite settings in the model which haven't been
    // applied yet.
    if (m_iContentChangedSignalBlockedCounter == 0)
    {
        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

            if (m_pGraphObjPolygon != nullptr) {
                const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
                CPhysValPolygon physValPolygon = m_pGraphObjPolygon->getPolygon(drawingSize.unit());
                if (m_eDimensionUnit == EScaleDimensionUnit::Pixels) {
                    if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Metric) {
                        // No simple unit conversion is possible here. The Y Scale Axis may
                        // be oriented from top to bottom or bottom to top.
                        // To get the correct scene coordinates we must let the drawing scene
                        // convert the coordinates into pixel values.
                        physValPolygon = m_pDrawingScene->convert(physValPolygon, Units.Length.px);
                    }
                }
                bool bRowsInserted = false;
                bool bRowsRemoved = false;
                bool bContentChanged = false;
                if (physValPolygon != m_physValPolygon) {
                    bContentChanged = false;
                    if (m_physValPolygon.count() < physValPolygon.count()) {
                        // Could detect which points have been added by comparing the points.
                        // But this is not really necessary.
                        bRowsInserted = true;
                    }
                    else if (m_physValPolygon.count() > physValPolygon.count()) {
                        // Could detect which points have been removed by comparing the points.
                        // But this is not really necessary.
                        bRowsRemoved = true;
                    }
                    m_physValPolygon = physValPolygon;
                    bContentChanged = true;
                }
                if (bContentChanged) {
                    if (bRowsInserted || bRowsRemoved) {
                        _beginResetModel();
                        m_arLabelSettings = getLabelSettings(m_pGraphObjPolygon);
                        _endResetModel();
                    }
                    else {
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
        }

        // If the "contentChanged" signal is no longer blocked and the content of
        // properties widget has been changed ...
        if (m_iContentChangedSignalBlockedCounter == 0 && m_bContentChanged) {
            // .. emit the contentChanged signal and update the enabled state
            // of the Apply and Reset buttons.
            emit_contentChanged();
        }
    }
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        QString strRuntimeInfo = "+- Polygon [" + QString::number(m_physValPolygon.count()) + "]";
        mthTracer.trace(strRuntimeInfo);
    }
}

//------------------------------------------------------------------------------
void CModelGraphObjPolygonGeometry::onGraphObjGeometryLabelChanged(
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
                SLabelSettings labelSettings = SLabelSettings::fromGraphObj(m_pGraphObjPolygon, i_strName, iRow);
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
void CModelGraphObjPolygonGeometry::onGraphObjAboutToBeDestroyed(CGraphObj*)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onGraphObjAboutToBeDestroyed",
        /* strAddInfo   */ "" );

    m_pGraphObjPolygon = nullptr;

    clearModel();
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelGraphObjPolygonGeometry::clearModel()
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
void CModelGraphObjPolygonGeometry::fillModel()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "fillModel",
        /* strMethodInArgs    */ "" );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();

    if (m_pGraphObjPolygon == nullptr) {
        m_physValPolygon = CPhysValPolygon(*m_pDrawingScene);
    }
    else {
        m_physValPolygon = m_pGraphObjPolygon->getPolygon(drawingSize.unit());
    }
    if (m_eDimensionUnit == EScaleDimensionUnit::Pixels) {
        if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Metric) {
            // No simple unit conversion is possible here. The Y Scale Axis may
            // be oriented from top to bottom or bottom to top.
            // To get the correct scene coordinates we must let the drawing scene
            // convert the coordinates into pixel values.
            m_physValPolygon = m_pDrawingScene->convert(m_physValPolygon, Units.Length.px);
        }
    }

    if (m_arLabelSettings.size() > 0) {
        clearModel();
    }
    if (m_pGraphObjPolygon != nullptr) {
        m_arLabelSettings = getLabelSettings(m_pGraphObjPolygon);
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
QList<CModelGraphObjPolygonGeometry::SLabelSettings> CModelGraphObjPolygonGeometry::getLabelSettings(CGraphObj* i_pGraphObj) const
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
void CModelGraphObjPolygonGeometry::updateXYValueSizeHint()
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
void CModelGraphObjPolygonGeometry::emit_contentChanged()
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
void CModelGraphObjPolygonGeometry::_beginResetModel()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "beginResetModel",
        /* strAddInfo   */ "" );

    beginResetModel();
}
//------------------------------------------------------------------------------
void CModelGraphObjPolygonGeometry::_endResetModel()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "endResetModel",
        /* strAddInfo   */ "" );

    endResetModel();
}

//------------------------------------------------------------------------------
void CModelGraphObjPolygonGeometry::_beginInsertRows(
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
void CModelGraphObjPolygonGeometry::_endInsertRows()
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
void CModelGraphObjPolygonGeometry::_beginRemoveRows(
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
void CModelGraphObjPolygonGeometry::_endRemoveRows()
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
void CModelGraphObjPolygonGeometry::emit_dataChanged(
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
