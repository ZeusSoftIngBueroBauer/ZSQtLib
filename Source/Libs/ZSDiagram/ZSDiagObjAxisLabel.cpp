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

#include "ZSDiagram/ZSDiagObjAxisLabel.h"
#include "ZSDiagram/ZSDiagramProcPixmap.h"
#include "ZSDiagram/ZSDiagScale.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtGui/qpainter.h>

#include <float.h>
#include <math.h>

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Diagram;
using namespace ZS::PhysVal;


/*******************************************************************************
class CDiagObjAxisLabel : public CDiagObj
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates an axis label instance.

    @param i_strObjName [in]
        Name of the diagram object. Objects of the same type (same class)
        must have a unique name.
    @param i_pDiagScale [in]
        Reference to diagram scale object the axis label is bound to.
    @param i_layoutPos [in]
        Range [Top, Bottom, Left, Right, Center]
        Defines the layout position of the axis label within the diagram.
    @param i_strAxisLabel [in]
        String for the axis label (e.g. "Frequency", "Power", "Time").
        May be empty if no axis label should be indicated.
        But the unit may be configured to be output in the axis label area.
*/
CDiagObjAxisLabel::CDiagObjAxisLabel(
    const QString& i_strObjName,
    CDiagScale*    i_pDiagScale,
    ELayoutPos     i_layoutPos,
    const QString& i_strAxisLabel ) :
//------------------------------------------------------------------------------
    CDiagObj(
        /* strClassName */ CDiagObjAxisLabel::ClassName(),
        /* strObjName   */ i_strObjName,
        /* pDiagScaleX  */ nullptr,
        /* pDiagScaleY  */ nullptr,
        /* layoutPos    */ i_layoutPos ),
    // config values
    m_pDiagScale(i_pDiagScale),
    m_iSpacingDiagPartCenter2DivLineLabels(0),
    m_iSpacingDiagPartCenter2AxisLabel(0),
    m_iSpacingDivLineLabels2AxisLabel(0),
    m_arbShowDivLines(CEnumDivLineLayer::count(), false),
    m_arcolDivLines(CEnumDivLineLayer::count(), Qt::black),
    m_arpenStyleDivLines(CEnumDivLineLayer::count(), Qt::SolidLine),
    m_arbShowDivLineLabels(CEnumDivLineLayer::count(), false),
    m_arcolDivLineLabels(CEnumDivLineLayer::count(), Qt::black),
    m_arfntDivLineLabels(CEnumDivLineLayer::count()),
    m_strAxisLabel(i_strAxisLabel),
    m_bShowAxisLabel(false),
    m_colAxisLabel(Qt::black),
    m_fntAxisLabel(),
    m_pUnit(nullptr), // as default: "use best unit"
    m_unitLabels(i_pDiagScale->getScale().unit()),
    m_strPhysUnitLabels(),
    m_bShowUnitAtDivLines(true),
    m_colUnitAtDivLines(Qt::black),
    m_fntUnitAtDivLines(),
    m_bShowUnitAtAxisLabel(false),
    m_divLinesMetrics(i_strObjName, EScaleDir::X),
    // calculated
    m_sizeDivLineLabelsMaxTextExtent(),
    m_rectAxisLabel(),
    m_rectDivLineLabelsPhysUnit(),
    m_bDivLineLabelsPhysUnitVisible(false),
    m_arbScaleMinMaxValVisible(2, false)
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    // The base class CDiagObj administers two scale objects: ScaleX and ScaleY.
    // The axis label just needs one of those scale objects. To avoid if/else
    // statements which scale object is needed by the axis label object depending
    // on the scale direction the used scale object is stored within the axis
    // label class. But the diagram needs to know to which scale object the
    // axis label object is linked to. So we need to set the member element of
    // the base class correspondingly.
    switch( m_pDiagScale->getScaleDir() )
    {
        case EScaleDir::X:
        {
            m_arpDiagScale[static_cast<int>(EScaleDir::X)] = m_pDiagScale;
            break;
        }
        case EScaleDir::Y:
        {
            m_arpDiagScale[static_cast<int>(EScaleDir::Y)] = m_pDiagScale;
            break;
        }
        default:
        {
            break;
        }
    }

    // Overwrite some default values with more useful values.
    // As default all possible parts are shown for the main layer.
    m_arbShowDivLines[EDivLineLayerMain] = true;
    m_arbShowDivLineLabels[EDivLineLayerMain] = true;
    m_bShowAxisLabel = !i_strAxisLabel.isEmpty();
    m_bShowUnitAtDivLines = true;

    if (i_layoutPos == ELayoutPos::Top || i_layoutPos == ELayoutPos::Bottom) {
        m_iSpacingDiagPartCenter2DivLineLabels = 6;
        m_iSpacingDivLineLabels2AxisLabel = 4;
    }
    else if (i_layoutPos == ELayoutPos::Left || i_layoutPos == ELayoutPos::Right) {
            m_iSpacingDiagPartCenter2DivLineLabels = 4;
            m_iSpacingDivLineLabels2AxisLabel = 2;
    }
} // ctor

//------------------------------------------------------------------------------
/*! @brief Destroys the axis label instance.

    Before destroying the instance the instance must have been removed from
    the diagram.
*/
CDiagObjAxisLabel::~CDiagObjAxisLabel()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    // config values
    m_pDiagScale = nullptr;
    m_pUnit = nullptr;
    m_iSpacingDiagPartCenter2DivLineLabels = 0;
    m_iSpacingDiagPartCenter2AxisLabel = 0;
    m_iSpacingDivLineLabels2AxisLabel = 0;
    //m_arbShowDivLines;
    //m_arcolDivLines;
    //m_arpenStyleDivLines;
    //m_arbShowDivLineLabels;
    //m_arcolDivLineLabels;
    //m_arfntDivLineLabels;
    //m_strAxisLabel;
    m_bShowAxisLabel = false;
    //m_colAxisLabel;
    //m_fntAxisLabel;
    m_pUnit = nullptr;
    //m_unitLabels;
    //m_strPhysUnitLabels;
    m_bShowUnitAtDivLines = false;
    //m_colUnitAtDivLines;
    //m_fntUnitAtDivLines;
    m_bShowUnitAtAxisLabel = false;
    //m_divLinesMetrics;
    // calculated
    //m_sizeDivLineLabelsMaxTextExtent;
    //m_rectAxisLabel;
    //m_rectDivLineLabelsPhysUnit;
    m_bDivLineLabelsPhysUnitVisible = false;
    //m_arbScaleMinMaxValVisible.clear();

} // dtor

/*==============================================================================
public: // instance methods (spacing between areas)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the space in pixels between the diagrams center part
           and the division line labels.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_iSpacing_px [in]
        Spacing in pixel to be used.
*/
void CDiagObjAxisLabel::setSpacingDiagPartCenter2DivLineLabels( int i_iSpacing_px )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iSpacing_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setSpacingDiagPartCenter2DivLineLabels",
        /* strMthInArgs */ strMthInArgs );

    if (m_iSpacingDiagPartCenter2DivLineLabels != i_iSpacing_px)
    {
        m_iSpacingDiagPartCenter2DivLineLabels = i_iSpacing_px;
        invalidate(EUpdateLayoutDataPixmapWidget, true);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the space in pixels between the diagrams center part
           and the division line labels.

    @return Spacing in pixels.
*/
int CDiagObjAxisLabel::spacingDiagPartCenter2DivLineLabels() const
//------------------------------------------------------------------------------
{
    return m_iSpacingDiagPartCenter2DivLineLabels;
}

//------------------------------------------------------------------------------
/*! @brief Sets the space in pixels between the diagrams center part
           and the axis label.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_iSpacing_px [in]
        Spacing in pixel to be used.
*/
void CDiagObjAxisLabel::setSpacingDiagPartCenter2AxisLabel( int i_iSpacing_px )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iSpacing_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setSpacingDiagPartCenter2AxisLabel",
        /* strMthInArgs */ strMthInArgs );

    if (m_iSpacingDiagPartCenter2DivLineLabels != i_iSpacing_px)
    {
        m_iSpacingDiagPartCenter2AxisLabel = i_iSpacing_px;
        invalidate(EUpdateLayoutDataPixmapWidget, true);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the space in pixels between the diagrams center part
           and the axis labels.

    @return Spacing in pixels.
*/
int CDiagObjAxisLabel::spacingDiagPartCenter2AxisLabel() const
//------------------------------------------------------------------------------
{
    return m_iSpacingDiagPartCenter2AxisLabel;
}

//------------------------------------------------------------------------------
/*! @brief Sets the space in pixels between the division line labels
           and the axis label.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_iSpacing_px [in]
        Spacing in pixel to be used.
*/
void CDiagObjAxisLabel::setSpacingDivLineLabels2AxisLabel( int i_iSpacing_px )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iSpacing_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setSpaceDivLineLabels2AxisLabel",
        /* strMthInArgs */ strMthInArgs );

    if (m_iSpacingDivLineLabels2AxisLabel != i_iSpacing_px)
    {
        m_iSpacingDivLineLabels2AxisLabel = i_iSpacing_px;
        invalidate(EUpdateLayoutDataPixmapWidget, true);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the space in pixels between the division line labels
           and the axis label.

    @return Spacing in pixels.
*/
int CDiagObjAxisLabel::spacingDivLineLabels2AxisLabel() const
//------------------------------------------------------------------------------
{
    return m_iSpacingDivLineLabels2AxisLabel;
}

/*==============================================================================
public: // instance methods (division lines)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets visibility of the division lines to false in the given layer.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_eLayer [in]
        Range [Main, Sub]
        By passing an invalid enum value all layers are affected.
*/
void CDiagObjAxisLabel::showDivLines(const CEnumDivLineLayer& i_eLayer)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_eLayer.isValid() ? i_eLayer.toString() : "AllLayers");
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "showDivLines",
        /* strMthInArgs */ strMthInArgs );

    int idxLayerMin = 0;
    int idxLayerMax = CEnumDivLineLayer::count()-1;
    if (i_eLayer.isValid()) {
        idxLayerMin = i_eLayer.enumeratorAsInt();
        idxLayerMax = i_eLayer.enumeratorAsInt();
    }

    bool bChanged = false;
    for (int idxLayer = idxLayerMin; idxLayer <= idxLayerMax; idxLayer++)  {
        if (!m_arbShowDivLines[idxLayer]) {
            m_arbShowDivLines[idxLayer] = true;
            bChanged = true;
        }
    }
    if (bChanged) {
        invalidate(EUpdateLayoutDataPixmapWidget, true);
    }
} // showDivLines

//------------------------------------------------------------------------------
/*! @brief Sets visibility of the division lines to false in the given layer.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_eLayer [in]
        Range [Main, Sub]
        By passing an invalid enum value all layers are affected.
*/
void CDiagObjAxisLabel::hideDivLines(const CEnumDivLineLayer& i_eLayer)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_eLayer.isValid() ? i_eLayer.toString() : "AllLayers");
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hideDivLines",
        /* strMthInArgs */ strMthInArgs );

    int idxLayerMin = 0;
    int idxLayerMax = CEnumDivLineLayer::count()-1;
    if (i_eLayer.isValid()) {
        idxLayerMin = i_eLayer.enumeratorAsInt();
        idxLayerMax = i_eLayer.enumeratorAsInt();
    }

    bool bChanged = false;
    for (int idxLayer = idxLayerMin; idxLayer <= idxLayerMax; idxLayer++) {
        if (m_arbShowDivLines[idxLayer]) {
            m_arbShowDivLines[idxLayer] = false;
            bChanged = true;
        }
    }
    if (bChanged) {
        invalidate(EUpdateLayoutDataPixmapWidget, true);
    }
} // hideDivLines

//------------------------------------------------------------------------------
/*! @brief Sets the foreground color to paint the division lines in the given layer.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_color [in]
        Color to be used.
    @param i_eLayer [in]
        Range [Main, Sub]
        By passing an invalid enum value all layers are affected.
*/
void CDiagObjAxisLabel::setDivLinesColor(
    const QColor& i_color, const ZS::System::CEnumDivLineLayer& i_eLayer)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_color.name() + ", " + QString(i_eLayer.isValid() ? i_eLayer.toString() : "AllLayers");
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setDivLinesColor",
        /* strMthInArgs */ strMthInArgs );

    int idxLayerMin = 0;
    int idxLayerMax = CEnumDivLineLayer::count()-1;
    if (i_eLayer.isValid()) {
        idxLayerMin = i_eLayer.enumeratorAsInt();
        idxLayerMax = i_eLayer.enumeratorAsInt();
    }

    bool bChanged = false;
    for (int idxLayer = idxLayerMin; idxLayer <= idxLayerMax; idxLayer++) {
        if (m_arcolDivLines[idxLayer] != i_color) {
            m_arcolDivLines[idxLayer] = i_color;
            bChanged = true;
        }
    }
    if (bChanged) {
        invalidate(EUpdateLayoutDataPixmapWidget, true);
    }
} // setDivLinesColor

//------------------------------------------------------------------------------
/*! @brief Returns the foreground color used to paint the division lines in the given layer.

    @param i_eLayer [in]
        Range [Main, Sub]

    @return Used color.
*/
QColor CDiagObjAxisLabel::divLinesColor(const CEnumDivLineLayer& i_eLayer) const
//------------------------------------------------------------------------------
{
    return m_arcolDivLines[i_eLayer.enumeratorAsInt()];
}

//------------------------------------------------------------------------------
/*! @brief Sets the pen style to paint the division lines in the given layer.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_penStyle [in]
        Pen style to be used.
    @param i_eLayer [in]
        Range [Main, Sub]
        By passing an invalid enum value all layers are affected.
*/
void CDiagObjAxisLabel::setDivLinesPenStyle(
    const Qt::PenStyle& i_penStyle, const CEnumDivLineLayer& i_eLayer)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qPenStyle2Str(i_penStyle) + ", " + QString(i_eLayer.isValid() ? i_eLayer.toString() : "AllLayers");
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setDivLinesPenStyle",
        /* strMthInArgs */ strMthInArgs );

    int idxLayerMin = 0;
    int idxLayerMax = CEnumDivLineLayer::count()-1;
    if (i_eLayer.isValid()) {
        idxLayerMin = i_eLayer.enumeratorAsInt();
        idxLayerMax = i_eLayer.enumeratorAsInt();
    }

    bool bChanged = false;
    for (int idxLayer = idxLayerMin; idxLayer <= idxLayerMax; idxLayer++) {
        if (m_arpenStyleDivLines[idxLayer] != i_penStyle) {
            m_arpenStyleDivLines[idxLayer] = i_penStyle;
            bChanged = true;
        }
    }
    if (bChanged) {
        invalidate(EUpdateLayoutDataPixmapWidget, true);
    }
} // setDivLinesPenStyle

//------------------------------------------------------------------------------
/*! @brief Returns the pen style used to paint the division lines in the given layer.

    For the Lines part this will be the line color.
    For the (division line) Labels and AxisLabel parts this method call makes not much sense.

    @param i_eLayer [in]
        Range [Main, Sub]

    @return Used color.
*/
Qt::PenStyle CDiagObjAxisLabel::divLinesPenStyle(const CEnumDivLineLayer& i_eLayer) const
//------------------------------------------------------------------------------
{
    return m_arpenStyleDivLines[i_eLayer.enumeratorAsInt()];
}

/*==============================================================================
public: // instance methods (division line labels)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets visibility of the division line labels to true in the given layer.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_eLayer [in]
        Range [Main, Sub]
        By passing an invalid enum value all layers are affected.
*/
void CDiagObjAxisLabel::showDivLineLabels(const CEnumDivLineLayer& i_eLayer)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_eLayer.isValid() ? i_eLayer.toString() : "AllLayers");
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "showDivLineLabels",
        /* strMthInArgs */ strMthInArgs );

    int idxLayerMin = 0;
    int idxLayerMax = CEnumDivLineLayer::count()-1;
    if (i_eLayer.isValid()) {
        idxLayerMin = i_eLayer.enumeratorAsInt();
        idxLayerMax = i_eLayer.enumeratorAsInt();
    }

    bool bChanged = false;
    for (int idxLayer = idxLayerMin; idxLayer <= idxLayerMax; idxLayer++)  {
        if (!m_arbShowDivLineLabels[idxLayer]) {
            m_arbShowDivLineLabels[idxLayer] = true;
            bChanged = true;
        }
    }
    if (bChanged) {
        invalidate(EUpdateLayoutDataPixmapWidget, true);
    }
} // showDivLineLabels

//------------------------------------------------------------------------------
/*! @brief Sets visibility of the division line labels to false in the given layer.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_eLayer [in]
        Range [Main, Sub]
        By passing an invalid enum value all layers are affected.
*/
void CDiagObjAxisLabel::hideDivLineLabels(const CEnumDivLineLayer& i_eLayer)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_eLayer.isValid() ? i_eLayer.toString() : "AllLayers");
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hideDivLineLabels",
        /* strMthInArgs */ strMthInArgs );

    int idxLayerMin = 0;
    int idxLayerMax = CEnumDivLineLayer::count()-1;
    if (i_eLayer.isValid()) {
        idxLayerMin = i_eLayer.enumeratorAsInt();
        idxLayerMax = i_eLayer.enumeratorAsInt();
    }

    bool bChanged = false;
    for (int idxLayer = idxLayerMin; idxLayer <= idxLayerMax; idxLayer++) {
        if (m_arbShowDivLineLabels[idxLayer]) {
            m_arbShowDivLineLabels[idxLayer] = false;
            bChanged = true;
        }
    }
    if (bChanged) {
        invalidate(EUpdateLayoutDataPixmapWidget, true);
    }
} // hideDivLineLabels

//------------------------------------------------------------------------------
/*! @brief Sets the foreground color to output the division line labels in the given layer.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_color [in]
        Color to be used.
    @param i_eLayer [in]
        Range [Main, Sub]
        By passing an invalid enum value all layers are affected.
*/
void CDiagObjAxisLabel::setDivLineLabelsColor(
    const QColor& i_color, const ZS::System::CEnumDivLineLayer& i_eLayer)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_color.name() + ", " + QString(i_eLayer.isValid() ? i_eLayer.toString() : "AllLayers");
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setDivLineLabelsColor",
        /* strMthInArgs */ strMthInArgs );

    int idxLayerMin = 0;
    int idxLayerMax = CEnumDivLineLayer::count()-1;
    if (i_eLayer.isValid()) {
        idxLayerMin = i_eLayer.enumeratorAsInt();
        idxLayerMax = i_eLayer.enumeratorAsInt();
    }

    bool bChanged = false;
    for (int idxLayer = idxLayerMin; idxLayer <= idxLayerMax; idxLayer++) {
        if (m_arcolDivLineLabels[idxLayer] != i_color) {
            m_arcolDivLineLabels[idxLayer] = i_color;
            bChanged = true;
        }
    }
    if (bChanged) {
        invalidate(EUpdateLayoutDataPixmapWidget, true);
    }
} // setDivLineLabelsColor

//------------------------------------------------------------------------------
/*! @brief Returns the foreground color used to output the division line labels in the given layer.

    @param i_eLayer [in]
        Range [Main, Sub]

    @return Used color.
*/
QColor CDiagObjAxisLabel::divLineLabelsColor(const CEnumDivLineLayer& i_eLayer) const
//------------------------------------------------------------------------------
{
    return m_arcolDivLineLabels[i_eLayer.enumeratorAsInt()];
}

//------------------------------------------------------------------------------
/*! @brief Sets the font to output the division line labels in the given layer.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_font [in]
        Font to be used.
    @param i_eLayer [in]
        Range [Main, Sub]
        By passing an invalid enum value all layers are affected.
*/
void CDiagObjAxisLabel::setDivLineLabelsFont(
    const QFont& i_font, const CEnumDivLineLayer& i_eLayer)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_font.family() + ", " + QString(i_eLayer.isValid() ? i_eLayer.toString() : "AllLayers");
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setDivLineLabelsFont",
        /* strMthInArgs */ strMthInArgs );

    int idxLayerMin = 0;
    int idxLayerMax = CEnumDivLineLayer::count()-1;
    if (i_eLayer.isValid()) {
        idxLayerMin = i_eLayer.enumeratorAsInt();
        idxLayerMax = i_eLayer.enumeratorAsInt();
    }

    bool bChanged = false;
    for (int idxLayer = idxLayerMin; idxLayer <= idxLayerMax; idxLayer++) {
        if (m_arfntDivLineLabels[idxLayer] != i_font) {
            m_arfntDivLineLabels[idxLayer] = i_font;
            bChanged = true;
        }
    }
    if (bChanged) {
        invalidate(EUpdateLayoutDataPixmapWidget, true);
    }
} // setDivLineLabelsFont

//------------------------------------------------------------------------------
/*! @brief Returns the font used to output the division line labels in the given layer.

    For the Lines part this will be the line color.
    For the (division line) Labels and AxisLabel parts this method call makes not much sense.

    @param i_eLayer [in]
        Range [Main, Sub]

    @return Used color.
*/
QFont CDiagObjAxisLabel::divLineLabelsFont(const CEnumDivLineLayer& i_eLayer) const
//------------------------------------------------------------------------------
{
    return m_arfntDivLineLabels[i_eLayer.enumeratorAsInt()];
}

//------------------------------------------------------------------------------
/*! @brief Sets the maximum number of digits to indicate the values of the division lines.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_iDigitsCountMax [in]
        Maximum number of allowed digits to label the division lines.
*/
void CDiagObjAxisLabel::setDigitsCountMax( int i_iDigitsCountMax )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iDigitsCountMax);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setDigitsCountMax",
        /* strMthInArgs */ strMthInArgs );

    bool bChanged = m_divLinesMetrics.setDigitsCountMax(i_iDigitsCountMax);
    if (bChanged) {
        invalidate(EUpdateLayoutDataPixmapWidget, true);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the maximum number of digits.

    @Return Maximum number of digits.
*/
int CDiagObjAxisLabel::digitsCountMax() const
//------------------------------------------------------------------------------
{
    return m_divLinesMetrics.digitsCountMax();
}

//------------------------------------------------------------------------------
/*! @brief Sets whether engineering format should be used to output the values
           at the division lines.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_bUseEngineeringFormat [in]
        true, if engineering format should be used, false otherwise.
*/
void CDiagObjAxisLabel::setUseEngineeringFormat( bool i_bUseEngineeringFormat )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bUseEngineeringFormat);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setUseEngineeringFormat",
        /* strMthInArgs */ strMthInArgs );

    bool bChanged = m_divLinesMetrics.setUseEngineeringFormat(i_bUseEngineeringFormat);
    if (bChanged) {
        invalidate(EUpdateLayoutDataPixmapWidget, true);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns whether engineering format should be used.

    @Return true, if engineering format should be used, false otherwise.
*/
bool CDiagObjAxisLabel::useEngineeringFormat() const
//------------------------------------------------------------------------------
{
    return m_divLinesMetrics.useEngineeringFormat();
}

/*==============================================================================
public: // instance methods (axis label)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the axis label (e.g. "Frequency").

    The flag to indicate the axis label is automatically updated depending
    on whether an empty string is passed.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_strLabel [in]
        String used to label the axis.
*/
void CDiagObjAxisLabel::setAxisLabel(const QString& i_strLabel)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strLabel;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setAxisLabel",
        /* strMthInArgs */ strMthInArgs );

    if (m_strAxisLabel != i_strLabel) {
        m_strAxisLabel = i_strLabel;
        if (m_strAxisLabel.isEmpty()) {
            hideAxisLabel();
        }
        else {
            showAxisLabel();
        }
        invalidate(EUpdateLayoutDataPixmapWidget, true);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the axis label (e.g. "Frequency").

    @Return String used to label the axis.
*/
QString CDiagObjAxisLabel::axisLabel() const
//------------------------------------------------------------------------------
{
    return m_strAxisLabel;
}

//------------------------------------------------------------------------------
/*! @brief Sets visibility of the axis label to true.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.
*/
void CDiagObjAxisLabel::showAxisLabel()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "showAxisLabel",
        /* strMthInArgs */ "" );

    bool bChanged = false;
    if (!m_bShowAxisLabel) {
        m_bShowAxisLabel = true;
        bChanged = true;
    }
    if (bChanged) {
        invalidate(EUpdateLayoutDataPixmapWidget, true);
    }
} // showAxisLabel

//------------------------------------------------------------------------------
/*! @brief Sets visibility of the axis label to false.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.
*/
void CDiagObjAxisLabel::hideAxisLabel()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hideAxisLabel",
        /* strMthInArgs */ "" );

    bool bChanged = false;
    if (m_bShowAxisLabel) {
        m_bShowAxisLabel = false;
        bChanged = true;
    }
    if (bChanged) {
        invalidate(EUpdateLayoutDataPixmapWidget, true);
    }
} // hideAxisLabel

//------------------------------------------------------------------------------
/*! @brief Sets the text color to output the axis label.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_color [in]
        Color to be used.
*/
void CDiagObjAxisLabel::setAxisLabelColor(const QColor& i_color)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_color.name();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setAxisLabelColor",
        /* strMthInArgs */ strMthInArgs );

    bool bChanged = false;
    if (m_colAxisLabel != i_color) {
        m_colAxisLabel = i_color;
        bChanged = true;
    }
    if (bChanged) {
        invalidate(EUpdateLayoutDataPixmapWidget, true);
    }
} // setAxisLabelColor

//------------------------------------------------------------------------------
/*! @brief Returns the text color used to output the axis label.

    @return Used color.
*/
QColor CDiagObjAxisLabel::axisLabelColor() const
//------------------------------------------------------------------------------
{
    return m_colAxisLabel;
}

//------------------------------------------------------------------------------
/*! @brief Sets the font to output the axis label.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_font [in]
        Font to be used.
*/
void CDiagObjAxisLabel::setAxisLabelFont(const QFont& i_font)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_font.family();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setAxisLabelFont",
        /* strMthInArgs */ strMthInArgs );

    bool bChanged = false;
    if (m_fntAxisLabel != i_font) {
        m_fntAxisLabel = i_font;
        bChanged = true;
    }
    if (bChanged) {
        invalidate(EUpdateLayoutDataPixmapWidget, true);
    }
} // setAxisLabelFont

//------------------------------------------------------------------------------
/*! @brief Returns the font used to output the division line labels in the given layer.

    @return Used color.
*/
QFont CDiagObjAxisLabel::axisLabelFont() const
//------------------------------------------------------------------------------
{
    return m_fntAxisLabel;
}

/*==============================================================================
public: // instance methods (unit)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the unit to be used for indicating the division line labels.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_pUnit [in]
        If nullptr the axis label uses the "best unit" for human readability
        to indicate the values (at maximum 3 leading digits).
        Otherwise the axis labels are indicated in the specified unit.
*/
void CDiagObjAxisLabel::setUnit( CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pUnit == nullptr ? "null" : i_pUnit->symbol());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setUnit",
        /* strMthInArgs */ strMthInArgs );

    if (m_pUnit != i_pUnit) {
        m_pUnit = i_pUnit;
        invalidate(EUpdateLayoutDataPixmapWidget, true);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the configured unit used to indicate the division line labels.

    @return nullptr if the axis label uses the "best unit" for human readability
            or valid reference to configured unit.
*/
CUnit* CDiagObjAxisLabel::getUnit()
//------------------------------------------------------------------------------
{
    return m_pUnit;
}

/*==============================================================================
public: // instance methods (unit at max scale value)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets visibility of the axis label to true.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.
*/
void CDiagObjAxisLabel::showUnitAtDivLines()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "showUnitAtDivLines",
        /* strMthInArgs */ "" );

    bool bChanged = false;
    if (!m_bShowUnitAtDivLines) {
        m_bShowUnitAtDivLines = true;
        bChanged = true;
    }
    if (bChanged) {
        invalidate(EUpdateLayoutDataPixmapWidget, true);
    }
} // showUnitAtDivLines

//------------------------------------------------------------------------------
/*! @brief Sets visibility of the axis label to false.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.
*/
void CDiagObjAxisLabel::hideUnitAtDivLines()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hideUnitAtDivLines",
        /* strMthInArgs */ "" );

    bool bChanged = false;
    if (m_bShowUnitAtDivLines) {
        m_bShowUnitAtDivLines = false;
        bChanged = true;
    }
    if (bChanged) {
        invalidate(EUpdateLayoutDataPixmapWidget, true);
    }
} // hideUnitAtDivLines

//------------------------------------------------------------------------------
/*! @brief Sets the text color to output the unit at the maximum scale value.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_color [in]
        Color to be used.
*/
void CDiagObjAxisLabel::setUnitColorAtDivLines(const QColor& i_color)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_color.name();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setUnitColorAtDivLines",
        /* strMthInArgs */ strMthInArgs );

    bool bChanged = false;
    if (m_colUnitAtDivLines != i_color) {
        m_colUnitAtDivLines = i_color;
        bChanged = true;
    }
    if (bChanged) {
        invalidate(EUpdateLayoutDataPixmapWidget, true);
    }
} // setUnitColorAtDivLines

//------------------------------------------------------------------------------
/*! @brief Returns the text color used to output the unit at the maximum scale value.

    @return Used color.
*/
QColor CDiagObjAxisLabel::unitColorAtDivLines() const
//------------------------------------------------------------------------------
{
    return m_colUnitAtDivLines;
}

//------------------------------------------------------------------------------
/*! @brief Sets the font to output the unit at the maximum scale value.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_font [in]
        Font to be used.
*/
void CDiagObjAxisLabel::setUnitFontAtDivLines(const QFont& i_font)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_font.family();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setUnitFontAtDivLines",
        /* strMthInArgs */ strMthInArgs );

    bool bChanged = false;
    if (m_fntUnitAtDivLines != i_font) {
        m_fntUnitAtDivLines = i_font;
        bChanged = true;
    }
    if (bChanged) {
        invalidate(EUpdateLayoutDataPixmapWidget, true);
    }
} // setUnitFontAtDivLines

//------------------------------------------------------------------------------
/*! @brief Returns the font used to output the unit at the maximum scale value.

    @return Used color.
*/
QFont CDiagObjAxisLabel::unitFontAtDivLines() const
//------------------------------------------------------------------------------
{
    return m_fntUnitAtDivLines;
}

/*==============================================================================
public: // instance methods (unit at axis label)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets visibility of the axis label to true.

    To output the unit at the axis label the font and text color of the
    axis label is used.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.
*/
void CDiagObjAxisLabel::showUnitAtAxisLabel()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "showUnitAtAxisLabel",
        /* strMthInArgs */ "" );

    bool bChanged = false;
    if (!m_bShowUnitAtAxisLabel) {
        m_bShowUnitAtAxisLabel = true;
        bChanged = true;
    }
    if (bChanged) {
        invalidate(EUpdateLayoutDataPixmapWidget, true);
    }
} // showUnitAtAxisLabel

//------------------------------------------------------------------------------
/*! @brief Sets visibility of the axis label to false.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.
*/
void CDiagObjAxisLabel::hideUnitAtAxisLabel()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hideUnitAtAxisLabel",
        /* strMthInArgs */ "" );

    bool bChanged = false;
    if (m_bShowUnitAtAxisLabel) {
        m_bShowUnitAtAxisLabel = false;
        bChanged = true;
    }
    if (bChanged) {
        invalidate(EUpdateLayoutDataPixmapWidget, true);
    }
} // hideUnitAtAxisLabel

/*==============================================================================
public: // overridables of base class CDiagObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Overrides CDiagObj::show.

    Sets the visibility of the axis label to true.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_bInformDiagram [in]
        true if the diagram should be informed that the objects data has been changed,
        false otherwise.
*/
void CDiagObjAxisLabel::show( bool i_bInformDiagram )
//------------------------------------------------------------------------------
{
    CDiagObj::show(i_bInformDiagram);
}

//------------------------------------------------------------------------------
/*! @brief Overrides CDiagObj::hide.

    Sets the visibility of the axis label to false.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_bInformDiagram [in]
        true if the diagram should be informed that the objects data has been changed,
        false otherwise.
*/
void CDiagObjAxisLabel::hide( bool i_bInformDiagram )
//------------------------------------------------------------------------------
{
    CDiagObj::hide(i_bInformDiagram);
}

//------------------------------------------------------------------------------
/*! @brief Overrides CDiagObj::sizeHint to return the width and height of the object.

    This method is used for layout processing in the diagram.

    Depending on the internally calculated data during layout processing the
    desired width and height is returned.

    For the axis label the area for the division lines, the strings to indicate
    the values at the division lines, the axis label, the unit string and the
    minimum and maximum scale value may have to be taken into account together
    with the used spacings between the different areas.

    The fonts used for the labels must also be taken into account.

    @return Desired size of the object.
*/
QSize CDiagObjAxisLabel::sizeHint()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "sizeHint",
        /* strAddInfo   */ "" );

    int cxWidth  = 0;
    int cyHeight = 0;

    if (m_pDiagram == nullptr)
    {
        return QSize(0,0);
    }

    if (isVisible())
    {
        const CPixmapDiagram* pPixmapDiagram = nullptr;

        // As a matter of fact there is no sense in adding an axis label object to
        // a diagram just designed to analyze data.
        if (m_pDiagram->getUpdateType() >= EDiagramUpdateTypePixmap)
        {
            pPixmapDiagram = dynamic_cast<const CPixmapDiagram*>(m_pDiagram);
        }
        if (pPixmapDiagram != nullptr)
        {
            bool bShowDivLineLabels = m_arbShowDivLineLabels[EDivLineLayerMain];

            if (m_pDiagScale->getScaleDir() == EScaleDir::X)
            {
                // The axis label coveres the whole horizontal extent of the diagram.
                cxWidth = pPixmapDiagram->getWidth();

                // The height need to be calculated.
                cyHeight = 0;

                // The configured space between the diagrams center part and the axis label
                // is not taken into account if a spacing between the division line labels
                // and the axis label is set and both parts are visible.
                // The space between the diagrams center part and the division line labels
                // as well as the space between the division line label and the axis label
                // don't need to be taken into account if a space between the diagrams center
                // part and the axis label is configured.

                /*
                     +---------|---------|---------|---------|---------|---------|
                         m_iSpacingDiagPartCenter2DivLineLabels          |
                    0.0  LabelsMaxTextExtent.height        40.0        |
                         m_iSpacingDivLineLabels2AxisLabel        m_iSpacingDiagPartCenter2AxisLabel
                                      Frequency/MHz
                */

                // If axis label is indicated ...
                if (m_bShowAxisLabel || m_bShowUnitAtAxisLabel)
                {
                    if (m_iSpacingDiagPartCenter2AxisLabel > 0) {
                        cyHeight += m_iSpacingDiagPartCenter2AxisLabel;
                    }
                    else if (bShowDivLineLabels) {
                        cyHeight +=
                            (m_iSpacingDiagPartCenter2DivLineLabels
                            + m_sizeDivLineLabelsMaxTextExtent.height()
                            + m_iSpacingDivLineLabels2AxisLabel);
                    }
                    cyHeight += m_rectAxisLabel.height();
                }
                // If only the division line labels are indicated ...
                else if (bShowDivLineLabels)
                {
                    cyHeight +=
                        (m_iSpacingDiagPartCenter2DivLineLabels
                        + m_sizeDivLineLabelsMaxTextExtent.height());
                }
            }
            else if (m_pDiagScale->getScaleDir() == EScaleDir::Y)
            {
                // The axis label coveres the whole vertical extent of the diagram.
                cyHeight = pPixmapDiagram->getHeight();

                // The width need to be calculated.
                cxWidth = 0;

                // Similar to horizontal orientation the configured space between the diagrams
                // center part and the axis label is not taken into account if a spacing between
                // the division line labels and the axis label is set and both parts are visible.
                // The space between the diagrams center part and the division line labels
                // as well as the space between the division line label and the axis label
                // don't need to be taken into account if a space between the diagrams center
                // part and the axis label is configured.

                // For vertical spacing also the text extent of the minimum and maximum scale
                // value in the division line labels area must be considered if they are indicated
                // not as part of the division lines but separately if less than two division lines
                // are visible.

                /*      0.0 +
                    P       |
                    o  40.0 +
                    w       |
                    e  30.0 +
                    r       |
                    /  20.0 +
                    d       |
                    B  10.0 +
                    b       |
                        0.0 +
                */

                // If axis label is indicated ...
                if (m_bShowAxisLabel || m_bShowUnitAtAxisLabel)
                {
                    if (m_iSpacingDiagPartCenter2AxisLabel > 0) {
                        cxWidth += m_iSpacingDiagPartCenter2AxisLabel;
                    }
                    else if (bShowDivLineLabels) {
                        cxWidth +=
                            (m_iSpacingDiagPartCenter2DivLineLabels
                            + m_sizeDivLineLabelsMaxTextExtent.width()
                            + m_iSpacingDivLineLabels2AxisLabel);
                    }
                    cxWidth += m_rectAxisLabel.width();
                }
                // If only the division line labels are indicated ...
                else if (bShowDivLineLabels)
                {
                    cxWidth +=
                        (m_iSpacingDiagPartCenter2DivLineLabels
                        + m_sizeDivLineLabelsMaxTextExtent.width());
                }
                if (m_divLinesMetrics.getScaleMinValBoundingRect().width() > cxWidth)
                {
                    cxWidth = m_divLinesMetrics.getScaleMinValBoundingRect().width();
                }
                if (m_divLinesMetrics.getScaleMaxValBoundingRect().width() > cxWidth)
                {
                    cxWidth = m_divLinesMetrics.getScaleMaxValBoundingRect().width();
                }
                if (m_rectDivLineLabelsPhysUnit.width() > cxWidth)
                {
                    cxWidth = m_rectDivLineLabelsPhysUnit.width();
                }
            }
        } // if( pPixmapDiagram != nullptr )
    } // if( isVisible() )

    QSize size(cxWidth, cyHeight);
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(qSize2Str(size));
    }
    return size;

} // sizeHint

/*==============================================================================
public: // must overridables of base class CDiagObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Clones the object and adds it to the target diagram.

    To clone the object a scale object with the same name as bound to
    the current object must already in exist in the target diagram.

    @param i_pDiagramTrg [in]
        Reference to target diagram the cloned object should be added to.

    @return Pointer to cloned object.
*/
CDiagObj* CDiagObjAxisLabel::clone( CDataDiagram* i_pDiagramTrg ) const
//------------------------------------------------------------------------------
{
    if( i_pDiagramTrg == nullptr || m_pDiagScale == nullptr )
    {
        return nullptr;
    }

    CDiagScale* pDiagScale = i_pDiagramTrg->findDiagScale(m_pDiagScale->getObjName());

    CDiagObjAxisLabel* pDiagObjCloned = new CDiagObjAxisLabel(
        /* strObjName   */ m_strObjName,
        /* pDiagScale   */ pDiagScale,
        /* layoutPos    */ m_layoutPos,
        /* strAxisLabel */ m_strAxisLabel );

    // Members from base class CDiagObj:
    pDiagObjCloned->m_rectContent = m_rectContent;
    pDiagObjCloned->m_bAdjustContentRect2DiagPartCenter = m_bAdjustContentRect2DiagPartCenter;
    pDiagObjCloned->m_bVisible = m_bVisible;
    pDiagObjCloned->m_state = m_state;
    pDiagObjCloned->m_bIsFocusable = m_bIsFocusable;
    pDiagObjCloned->m_bIsEditable = m_bIsEditable;

    // Members from this class:
    pDiagObjCloned->m_iSpacingDiagPartCenter2DivLineLabels = m_iSpacingDiagPartCenter2DivLineLabels;
    pDiagObjCloned->m_iSpacingDiagPartCenter2AxisLabel = m_iSpacingDiagPartCenter2AxisLabel;
    pDiagObjCloned->m_iSpacingDivLineLabels2AxisLabel = m_iSpacingDivLineLabels2AxisLabel;
    pDiagObjCloned->m_arbShowDivLines = m_arbShowDivLines;
    pDiagObjCloned->m_arcolDivLines = m_arcolDivLines;
    pDiagObjCloned->m_arpenStyleDivLines = m_arpenStyleDivLines;
    pDiagObjCloned->m_arbShowDivLineLabels = m_arbShowDivLineLabels;
    pDiagObjCloned->m_arcolDivLineLabels = m_arcolDivLineLabels;
    pDiagObjCloned->m_arfntDivLineLabels = m_arfntDivLineLabels;
    pDiagObjCloned->m_strAxisLabel = m_strAxisLabel;
    pDiagObjCloned->m_bShowAxisLabel = m_bShowAxisLabel;
    pDiagObjCloned->m_colAxisLabel = m_colAxisLabel;
    pDiagObjCloned->m_fntAxisLabel = m_fntAxisLabel;
    pDiagObjCloned->m_pUnit = m_pUnit;
    pDiagObjCloned->m_unitLabels = m_unitLabels;
    pDiagObjCloned->m_strPhysUnitLabels = m_strPhysUnitLabels;
    pDiagObjCloned->m_bShowUnitAtDivLines = m_bShowUnitAtDivLines;
    pDiagObjCloned->m_colUnitAtDivLines = m_colUnitAtDivLines;
    pDiagObjCloned->m_fntUnitAtDivLines = m_fntUnitAtDivLines;
    pDiagObjCloned->m_bShowUnitAtAxisLabel = m_bShowUnitAtAxisLabel;
    pDiagObjCloned->m_divLinesMetrics = m_divLinesMetrics;
    pDiagObjCloned->m_sizeDivLineLabelsMaxTextExtent = m_sizeDivLineLabelsMaxTextExtent;
    pDiagObjCloned->m_rectAxisLabel = m_rectAxisLabel;
    pDiagObjCloned->m_rectDivLineLabelsPhysUnit = m_rectDivLineLabelsPhysUnit;
    pDiagObjCloned->m_bDivLineLabelsPhysUnitVisible = m_bDivLineLabelsPhysUnitVisible;
    pDiagObjCloned->m_arbScaleMinMaxValVisible = m_arbScaleMinMaxValVisible;

    i_pDiagramTrg->addDiagObj(pDiagObjCloned);

    return pDiagObjCloned;

} // clone

//------------------------------------------------------------------------------
/*! @brief Updates the internal data of the object.

    The axis label has to do the following depending on the process depths:

    - Layout
      Calculate the extent in width if bound to a Y-scale or the height if
      bound to a X-scale object.
    - Data
      Calculate the final position of the division lines, the division line
      labels, the axis label and the unit string.
    - Pixmap
      Draw the object into the paint device.
    - Widget
      Invalidate the previous and the new rectangle widget areas covered by the object.

    @param i_uUpdateFlags [in]
        Bitmap with dirty flags specifying for which process depth
        (Layout, Data, Pixmap, Widget) the internal data should be recalculated.
        The passed flags will be compared with the internal update flags.
        If the internal dirty flag is not set the method does nothing.
        If the internal dirty flag is set the method calculates the process depth
        and resets the dirty flag.
    @param i_pPaintDevice [in]
        Only used for pixmap processing to draw the content of the object.
*/
void CDiagObjAxisLabel::update( unsigned int i_uUpdateFlags, QPaintDevice* i_pPaintDevice )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObjUpdate, EMethodTraceDetailLevel::ArgsNormal))
    {
        strTrcMsg = updateFlags2Str(i_uUpdateFlags);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "update",
        /* strAddInfo   */ strTrcMsg );

    if( i_uUpdateFlags == EUpdateNone || m_pDiagram == nullptr || m_pDiagScale == nullptr || !isVisible() )
    {
        return;
    }
    // As a matter of fact there is no sense in adding an axis label object to
    // a diagram just designed to analyze data.
    if( m_pDiagram->getUpdateType() < EDiagramUpdateTypePixmap )
    {
        return;
    }
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

    // Please note that only if by invalidating the flag (setting the bit) in the
    // internal update flags and on requesting the process depth by this update
    // method the corresponding update process will be executed.

    // If the width and height for the sizeHint method need to be recalculated ..
    if( i_uUpdateFlags & EUpdateLayout && m_uUpdateFlags & EUpdateLayout )
    {
        updateLayout();
    }

    // If the internal data structures need to be updated ..
    if( i_uUpdateFlags & EUpdateData && m_uUpdateFlags & EUpdateData )
    {
        updateData();
    }

    // If recalculation of bitmap is needed ..
    if( i_uUpdateFlags & EUpdatePixmap && m_uUpdateFlags & EUpdatePixmap && i_pPaintDevice != nullptr )
    {
        updatePixmap(i_pPaintDevice);
    }

    // If the widget need to be updated ..
    if( i_uUpdateFlags & EUpdateWidget && m_uUpdateFlags & EUpdateWidget )
    {
        updateWidget();
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

} // update

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to do calculation for layout processing.

    Calculates the sizes in width and height of the axis label,
    the minimum and maximum scale values and divison line labels.

    Layout processing must provide all data necessary for the "sizeHint" call
    of the diagram so that the diagram is able to calculate the width and
    height of the top, bottom, left and right areas.
*/
void CDiagObjAxisLabel::updateLayout()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateLayout",
        /* strAddInfo   */ "" );

    if (m_pDiagram == nullptr || m_pDiagScale == nullptr || !isVisible())
    {
        return;
    }

    // As a matter of fact there is no sense in adding an axis label object to
    // a diagram just designed to analyze data.
    if (m_pDiagram->getUpdateType() >= EDiagramUpdateTypePixmap)
    {
        const CPixmapDiagram* pPixmapDiagram = dynamic_cast<const CPixmapDiagram*>(m_pDiagram);
        if (pPixmapDiagram == nullptr)
        {
            return;
        }
    }

    // Calculate string to indicate the physical unit
    m_unitLabels = getAxisLabelUnit();
    if (m_unitLabels.isValid()) {
        m_strPhysUnitLabels = m_unitLabels.symbol();
    }
    else {
        m_strPhysUnitLabels = "";
    }

    // Calculate size in width and height for the axis label.
    // The axis label may also include the unit string.
    QSize sizeAxisLabel = getAxisLabelTextExtent();
    // The sizeHint method uses width and height of the axis label
    // rectangle for layout processing of the diagram areas.
    m_rectAxisLabel = QRect(
        0, 0, sizeAxisLabel.width(), sizeAxisLabel.height());

    // Initialize division lines metrics calculator of this diag axis label with the calculated
    // results (position of divison lines) from the scales division lines calculator.
    m_divLinesMetrics = m_pDiagScale->getDivLinesScaler();

    if (m_pDiagScale->getScaleUnit() != m_unitLabels)
    {
        CScale scale = m_pDiagScale->getScale();
        scale.setUnit(m_unitLabels);
        m_divLinesMetrics.setScaleMin(scale.minVal().getVal());
        m_divLinesMetrics.setScaleMax(scale.maxVal().getVal());
        m_divLinesMetrics.setScaleRes(scale.res().getVal());
    }

    // If the unit string should be indicated in the area of the division line labels
    // the maximum text extent of the division lines may have to be adjusted.
    QSize sizeDivLineLabelsPhysUnit = getDivLineLabelsUnitTextExtent();
    m_divLinesMetrics.setDivLineLabelsMinTextExtent(sizeDivLineLabelsPhysUnit);
    // The sizeHint method uses width and height of the unit strings
    // rectangle for layout processing of the diagram areas.
    // If the unit strings rectangle need to be considered ...
    m_rectDivLineLabelsPhysUnit = QRect(
        0, 0, sizeDivLineLabelsPhysUnit.width(), sizeDivLineLabelsPhysUnit.height());

    // Calculate bounding rectangles of the division line labels.
    // This also sets visibility flags for the labels so that
    // labels do not overlap each other.
    m_divLinesMetrics.update();

    // If the unit string should be indicated in the area of the division line labels
    // the maximum text extent of the division line labels calculated by the division
    // lines metrics calculator may have to be adjusted.
    m_sizeDivLineLabelsMaxTextExtent = m_divLinesMetrics.getDivLineLabelsMaxTextExtent();
    if (sizeDivLineLabelsPhysUnit.isValid()) {
        if (m_layoutPos == ELayoutPos::Top || m_layoutPos == ELayoutPos::Bottom) {
            if (sizeDivLineLabelsPhysUnit.height() > m_sizeDivLineLabelsMaxTextExtent.height()) {
                m_sizeDivLineLabelsMaxTextExtent.setHeight(sizeDivLineLabelsPhysUnit.height());
            }
        }
        else if (m_layoutPos == ELayoutPos::Left || m_layoutPos == ELayoutPos::Right) {
            if (sizeDivLineLabelsPhysUnit.width() > m_sizeDivLineLabelsMaxTextExtent.width()) {
                m_sizeDivLineLabelsMaxTextExtent.setWidth(sizeDivLineLabelsPhysUnit.width());
            }
        }
    }

    // Mark current process depth as executed (reset bit)
    //---------------------------------------------------

    validate(EUpdateLayout);

} // updateLayout

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to do calculation for data processing.

    The diagrams layout processing is finished and the area of
    axis label is now known. The axis label, the unit and the
    division line labels may now be moved to their final positions.
*/
void CDiagObjAxisLabel::updateData()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateData",
        /* strAddInfo   */ "" );

    const CPixmapDiagram* pPixmapDiagram = nullptr;

    // As a matter of fact there is no sense in adding an axis label object to
    // a diagram just designed to analyze data.
    if( m_pDiagram->getUpdateType() >= EDiagramUpdateTypePixmap )
    {
        pPixmapDiagram = dynamic_cast<const CPixmapDiagram*>(m_pDiagram);
    }
    if( pPixmapDiagram == nullptr )
    {
        return;
    }

    // Data processing for position of division lines is moved to pixmap processing.
    // Otherwise the position of the lines would have to be stored in an array of QLine objects.
    // The relative position of the division lines are retrieved from the
    // division lines metrics calculator and moved to fit into the content
    // rectangle of the axis label when painting the lines.

    // Data processing for position of division line labels is also moved to pixmap processing.
    // Otherwise the position of the labels bounding rectangles would have to be stored in
    // an array of QRect objects.
    // The relative position of the division lines are retrieved from the
    // division lines metrics calculator and moved to fit into the content
    // rectangle of the axis label when painting the lines.

    m_rectAxisLabel = getAxisLabelBoundingRect();
    m_rectDivLineLabelsPhysUnit = getDivLineLabelsUnitBoundingRect();

    // Mark current process depth as executed (reset bit)
    validate(EUpdateData);

} // updateData

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::updatePixmap( QPaintDevice* i_pPaintDevice )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updatePixmap",
        /* strAddInfo   */ "" );

    const CPixmapDiagram* pPixmapDiagram = nullptr;

    // As a matter of fact there is no sense in adding an axis label object to
    // a diagram just designed to analyze data.
    if( m_pDiagram != nullptr && m_pDiagram->getUpdateType() >= EDiagramUpdateTypePixmap )
    {
        pPixmapDiagram = dynamic_cast<const CPixmapDiagram*>(m_pDiagram);
    }
    if( pPixmapDiagram == nullptr )
    {
        return;
    }

    paintDivisionLines(i_pPaintDevice);
    paintDivisionLineLabels(i_pPaintDevice);
    paintDivisionLineLabelsUnit(i_pPaintDevice);
    paintScaleMinMaxVals(i_pPaintDevice);
    paintAxisLabel(i_pPaintDevice);


    // Mark current process depth as executed (reset bit)
    validate(EUpdatePixmap);

} // updatePixmap

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::updateWidget()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateWidget",
        /* strAddInfo   */ "" );

    if( m_pDiagram->getUpdateType() >= EDiagramUpdateTypeWidget )
    {
        // Invalidate output region of the diagram object to update (repaint) content of diagram.
        if( m_rectContentPrev.isValid() )
        {
            m_pDiagram->update(this, m_rectContentPrev);
            m_rectContentPrev = m_rectContent;
        }
        if( m_rectContent.isValid() )
        {
            m_pDiagram->update(this, m_rectContent);
        }

    } // if( pWdgtDiagram != nullptr )

    // Mark current process depth as executed (reset bit)
    validate(EUpdateWidget);

} // updateWidget

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the rectangle part used to draw the axis taking the
           left, right, bottom and top margins into account.

    @return Rectangle within the diagram to draw axis labels.
*/
QRect CDiagObjAxisLabel::getDiagRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "getDiagRect",
        /* strAddInfo   */ "" );

    QRect rectDiag(0, 0, 1, 1);

    const CPixmapDiagram* pPixmapDiagram = dynamic_cast<const CPixmapDiagram*>(m_pDiagram);
    if( pPixmapDiagram != nullptr )
    {
        rectDiag.setLeft(pPixmapDiagram->getMarginLeft());
        rectDiag.setWidth(pPixmapDiagram->getWidth()
                        - pPixmapDiagram->getMarginLeft()
                        - pPixmapDiagram->getMarginRight());
        rectDiag.setTop(pPixmapDiagram->getMarginTop());
        rectDiag.setHeight(pPixmapDiagram->getHeight()
                         - pPixmapDiagram->getMarginTop()
                         - pPixmapDiagram->getMarginBottom());
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rectDiag) + "}");
    }
    return rectDiag;
}

//------------------------------------------------------------------------------
/*! @brief Returns the unit in which the labels should be indicated.

    If the unit has not been explicitly set the best unit to indicate the values
    is calculated. The best unit is the unit in which the values are displayed
    with at least one but no more than three digits before the decimal point.

    @return Unit in which the labels should be indicated.
*/
CUnit CDiagObjAxisLabel::getAxisLabelUnit() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "getAxisLabelUnit",
        /* strAddInfo   */ "" );

    PhysVal::CUnit unit;

    // If the unit, in which the division lines should be labeled, has been explicitly specified ...
    if( m_pUnit != nullptr )
    {
        unit = *m_pUnit;
    }
    // If the "best" unit to indicate the values should be used ...
    else
    {
        CUnit  unitScaleMinValTmp;
        double fScaleMinValTmp;
        int    iDigitsLeadingScaleMinVal;
        int    iDigitsTrailingScaleMinVal;

        unit = m_pDiagScale->getScale().unit();

        double fScaleMinVal = m_pDiagScale->getScale().minVal().getVal();
        double fScaleMaxVal = m_pDiagScale->getScale().maxVal().getVal();

        TFormatResult formatResult = PhysVal::formatValue(
            /* fValue                */ fScaleMinVal,
            /* unitVal               */ unit,
            /* iDigitsMantissaMax    */ 0,
            /* bAccuracyLimitsMant.  */ false,
            /* iDigitsAccuracy       */ 0,
            /* iDigitsExponent       */ 0,
            /* bUseEngineeringFormat */ false,
            /* pfVal                 */ &fScaleMinValTmp,
            /* pstr                  */ nullptr,
            /* pUnit                 */ &unitScaleMinValTmp,
            /* piDigitsLeading       */ &iDigitsLeadingScaleMinVal,
            /* piDigitsTrailing      */ &iDigitsTrailingScaleMinVal );

        if( !(formatResult & FormatResult::Error) ) //lint !e655
        {
            CUnit  unitScaleMaxValTmp;
            double fScaleMaxValTmp;
            int    iDigitsLeadingScaleMaxVal;
            int    iDigitsTrailingScaleMaxVal;

            formatResult = PhysVal::formatValue(
                /* fValue                */ fScaleMaxVal,
                /* unitVal               */ unit,
                /* iDigitsMantissaMax    */ 0,
                /* bAccuracyLimitsMant.  */ false,
                /* iDigitsAccuracy       */ 0,
                /* iDigitsExponent       */ 0,
                /* bUseEngineeringFormat */ false,
                /* pfVal                 */ &fScaleMaxValTmp,
                /* pstr                  */ nullptr,
                /* pUnit                 */ &unitScaleMaxValTmp,
                /* piDigitsLeading       */ &iDigitsLeadingScaleMaxVal,
                /* piDigitsTrailing      */ &iDigitsTrailingScaleMaxVal );

            if( !(formatResult & FormatResult::Error) ) //lint !e655
            {
                // If the best unit for the scale minimum and maximum scale value is the same ...
                if( unitScaleMinValTmp == unitScaleMaxValTmp )
                {
                    unit = unitScaleMinValTmp;
                }
                // If the best unit for the scale minimum and maximum scale value is different ...
                else
                {
                    // ... we need to decide which of both is the best unit to indicate the
                    // labels at the division lines.

                    // If the minimum scale value was 0.0 ...
                    if( fabs(fScaleMinVal) == 0.0 )
                    {
                        // .. we take the best unit for the scale minimum value.
                        unit = unitScaleMaxValTmp;
                    }
                    // If the maximum scale value was 0.0 ...
                    else if( fabs(fScaleMaxVal) == 0.0 )
                    {
                        // .. we take the best unit for the scale minimum value.
                        unit = unitScaleMinValTmp;
                    }
                    // If the absolute value of both values were greater or equal than 1.0 ...
                    else if( fabs(fScaleMinVal) >= 1.0 && fabs(fScaleMaxVal) >= 1.0 )
                    {
                        // .. we take the best unit for the absolute greater value.
                        if( fabs(fScaleMinVal) < fabs(fScaleMaxVal) )
                        {
                            unit = unitScaleMaxValTmp;
                        }
                        else
                        {
                            unit = unitScaleMinValTmp;
                        }
                    }
                    // If the absolute value of both values were less or equal than 1.0 ...
                    else if( fabs(fScaleMinVal) <= 1.0 && fabs(fScaleMaxVal) <= 1.0 )
                    {
                        // .. we take the best unit for the absolute lower value.
                        if( fabs(fScaleMinVal) < fabs(fScaleMaxVal) )
                        {
                            unit = unitScaleMinValTmp;
                        }
                        else
                        {
                            unit = unitScaleMaxValTmp;
                        }
                    }
                    // If the absolute value of the scale max value is greater or equal than 1.0 ...
                    else if( fabs(fScaleMaxVal) >= 1.0 )
                    {
                        // .. we take the best unit of the scale max value.
                        unit = unitScaleMaxValTmp;
                    }
                    // If the absolute value of the scale max value is less than 1.0 ...
                    else // if( fabs(fScaleMaxVal) < 1.0 )
                    {
                        // .. we take the best unit of the scale min value.
                        unit = unitScaleMinValTmp;
                    }
                }
            }
        }
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(unit.symbol());
    }
    return unit;
}

//------------------------------------------------------------------------------
/*! @brief Checks whether the axis label is visible.

    The unit may also be indicated together with the axis label (or instead).

    @return true if the axis label should be indicated.
*/
bool CDiagObjAxisLabel::isAxisLabelVisible() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "isAxisLabelVisible",
        /* strAddInfo   */ "" );
    bool bVisible = (m_bShowAxisLabel || m_bShowUnitAtAxisLabel);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bVisible);
    }
    return bVisible;
}

//------------------------------------------------------------------------------
/*! @brief Calculates the size (width and height) of the bounding rectangle
           for the unit string using metrics of the font to be used if the
           unit string should be indicated in the area of the division lines.

    If the axis label is positioned on the left or right layout position
    of the diagram the bounding rectangle is rotated by 90 degrees
    (width becomes height and height becomes width).

    @return Bounding rectangle of the unit string.
*/
QSize CDiagObjAxisLabel::getDivLineLabelsUnitTextExtent() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "getDivLineLabelsUnitTextExtent",
        /* strAddInfo   */ "" );

    QSize size;

    // If the unit string should be indicated in the area of the division line labels
    // the maximum text extent of the division lines may have to be adjusted.
    if (m_bShowUnitAtDivLines)
    {
        QFontMetrics fntmtr(m_fntUnitAtDivLines);
        size = fntmtr.boundingRect(m_strPhysUnitLabels).size();
        size.setHeight(size.height() + 2);
        size.setWidth(size.width() + 2);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qSize2Str(size) + "}");
    }
    return size;
}

//------------------------------------------------------------------------------
/*! @brief Calculates the bounding rectangle for the unit string using metrics
           of the font to be used if the unit string should be indicated in the
           area of the division lines.

    If the axis label is positioned on the left or right layout position
    of the diagram the bounding rectangle is rotated by 90 degrees
    (width becomes height and height becomes width).

    @return Bounding rectangle of the unit string.
*/
QRect CDiagObjAxisLabel::getDivLineLabelsUnitBoundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "getDivLineLabelsUnitBoundingRect",
        /* strAddInfo   */ "" );

    // As calculated in layout processing.
    QRect rect;

    // If the unit string should be indicated in the area of the division line labels
    // the maximum text extent of the division lines may have to be adjusted.
    // If the unit string should be indicated in the area of the division line labels
    if (m_bShowUnitAtDivLines)
    {
        // TODO: Move it to the final position
        rect = m_rectDivLineLabelsPhysUnit;

        //QFontMetrics fntmtr(m_fntUnitAtDivLines);
        //rect = fntmtr.boundingRect(m_strPhysUnitLabels);
        //rect.setHeight(rect.height() + 2);
        //rect.setWidth(rect.width() + 2);

        //if (m_layoutPos == ELayoutPos::Left || m_layoutPos == ELayoutPos::Right) {
        //    // The label will be drawn rotated by 90 degrees.
        //    // The width becomes the height.
        //    int cyHeight = rect.height();
        //    rect.setHeight(rect.width());
        //    rect.setWidth(cyHeight);
        //}
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rect) + "}");
    }
    return rect;
}

//------------------------------------------------------------------------------
/*! @brief Method used for layout processing to calculate the size (width and height)
           of the bounding rectangle for the axis label using metrics of the font to be used.

    The bounding rectangles size may also include the unit string if the unit
    string should be indicated together with the axis label.

    If the axis label is positioned on the left or right layout position
    of the diagram the bounding rectangle is rotated by 90 degrees
    (width becomes height and height becomes width).

    @return Bounding rectangle of the axis label.
*/
QSize CDiagObjAxisLabel::getAxisLabelTextExtent() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "getAxisLabelTextExtent",
        /* strAddInfo   */ "" );

    QSize size;

    if (isAxisLabelVisible())
    {
        QFontMetrics fntmtr(m_fntAxisLabel);
        QString strAxisLabel = m_strAxisLabel;

        if (m_bShowUnitAtAxisLabel) {
            if (!strAxisLabel.isEmpty()) {
                strAxisLabel += " / ";
            }
            strAxisLabel += m_strPhysUnitLabels;
        }
        size = fntmtr.boundingRect(strAxisLabel).size();
        size.setHeight(size.height() + 2);
        size.setWidth(size.width() + 2);

        if (m_layoutPos == ELayoutPos::Left || m_layoutPos == ELayoutPos::Right) {
            // The label will be drawn rotated by 90 degrees.
            // The width becomes the height.
            int cyHeight = size.height();
            size.setHeight(size.width());
            size.setWidth(cyHeight);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qSize2Str(size) + "}");
    }
    return size;
}

//------------------------------------------------------------------------------
/*! @brief Method used for data processing to calculate the final position
           of the bounding rectangle for the axis label.

    The size (width and height) of the axis labels bounding rectangle must
    have already been calculated during layout processing.

    The diagram updates the geometry of the object in x/y position and width and height
    (content rectangle) before invoking the update method for data processing.

    @return Bounding rectangle of the axis label at the final position.
*/
QRect CDiagObjAxisLabel::getAxisLabelBoundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "getAxisLabelBoundingRect",
        /* strAddInfo   */ "" );

    QRect rect;

    if (isAxisLabelVisible())
    {
        rect = m_rectAxisLabel;

        if (m_layoutPos == ELayoutPos::Top || m_layoutPos == ELayoutPos::Bottom)
        {
            int cyOffset = m_iSpacingDiagPartCenter2DivLineLabels
                         + m_sizeDivLineLabelsMaxTextExtent.height()
                         + m_iSpacingDivLineLabels2AxisLabel;
            if (cyOffset < m_iSpacingDiagPartCenter2AxisLabel) {
                cyOffset = m_iSpacingDiagPartCenter2AxisLabel;
            }
            rect.moveLeft(m_rectContent.left() + m_rectContent.width()/2 - m_rectAxisLabel.width()/2);

            if (m_layoutPos == ELayoutPos::Top) {
                rect.moveBottom(m_rectContent.bottom() - cyOffset);
            }
            else {
                rect.moveBottom(m_rectContent.top() + cyOffset);
            }
        }
        else if (m_layoutPos == ELayoutPos::Left || m_layoutPos == ELayoutPos::Right)
        {
            int cxOffset = m_iSpacingDiagPartCenter2DivLineLabels
                         + m_sizeDivLineLabelsMaxTextExtent.width()
                         + m_iSpacingDivLineLabels2AxisLabel;
            if (cxOffset < m_iSpacingDiagPartCenter2AxisLabel) {
                cxOffset = m_iSpacingDiagPartCenter2AxisLabel;
            }
            rect.moveTop(m_rectContent.top() + m_rectContent.height()/2 - m_rectAxisLabel.height()/2);

            if (m_layoutPos == ELayoutPos::Left) {
                rect.moveLeft(m_rectContent.right() - cxOffset);
            }
            else {
                rect.moveLeft(m_rectContent.left() + cxOffset);
            }
        }
    } // if (isAxisLabelVisible())

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rect) + "}");
    }
    return rect;
}

//------------------------------------------------------------------------------
/*! @brief Checks whether the given rectangle intersects the label showing the
           unit of the axis.

    @param i_rect [in]
        Rectangle to be checked whether it intersects any of the existing label rectangles.

    @return true if the passed rectangle intersects the unit label, false otherwise.
*/
bool CDiagObjAxisLabel::intersectsDivLineLabelsPhysUnit(const QRect& i_rect) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjUpdate, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qRect2Str(i_rect);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "intersectsDivLineLabelsPhysUnit",
        /* strAddInfo   */ strMthInArgs );

    bool bIntersect = false;

    if( m_bShowUnitAtDivLines && m_bDivLineLabelsPhysUnitVisible )
    {
        if( i_rect.intersects(m_rectDivLineLabelsPhysUnit) )
        {
            bIntersect = true;
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bIntersect);
    }
    return bIntersect;
}

//------------------------------------------------------------------------------
QString CDiagObjAxisLabel::formatValue( double i_fVal, CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjUpdate, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fVal) + QString(i_pUnit == nullptr ? "" : " " + i_pUnit->symbol());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "formatValue",
        /* strAddInfo   */ strMthInArgs );

    // Formats the value in the given unit into a string according to the internally
    // calculated number of leading and trailing digits and the internally calculated
    // physical unit used to indicate the labels at the division lines.

    double fVal = i_fVal;
    CUnit unit = m_unitLabels;

    if (i_pUnit != nullptr) {
        unit = *i_pUnit;
    }
    if (unit != m_unitLabels) {
        fVal = unit.convertValue(fVal, m_unitLabels);
    }
    QString strVal = m_divLinesMetrics.formatValue(i_fVal);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(strVal);
    }
    return strVal;

} // formatValue

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::paintDivisionLines(QPaintDevice* i_pPaintDevice)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qSize2Str(QSize(i_pPaintDevice->width(), i_pPaintDevice->height()));
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "paintDivisionLines",
        /* strAddInfo   */ strMthInArgs );

    QPainter painter(i_pPaintDevice);

    for (int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++)
    {
        if( m_arbShowDivLines[iLayer] )
        {
            QPen pen(m_arcolDivLines[iLayer]);
            pen.setStyle(m_arpenStyleDivLines[iLayer]);
            painter.setPen(pen);

            for (int idxDivLine = 0; idxDivLine < m_divLinesMetrics.getDivLinesCount(iLayer); idxDivLine++)
            {
                // Please note that the division lines metrics calculator is a clone from the
                // diagrams scale object and the geometry set at the calculater already has
                // the margins between the content rectangle of the axis label set as the
                // minimum and maximum pixel range values. No need to move the division line
                // pixel values to the content rectangle of the axis label object.

                int iDivLine_px = m_divLinesMetrics.getDivLineInPix(iLayer, idxDivLine);
                int x1 = 0, y1 = 0, x2 = 0, y2 = 0;

                if (m_layoutPos == ELayoutPos::Top) {
                    x1 = x2 = iDivLine_px;
                    y2 = m_rectContent.bottom();
                    y1 = y2 - m_iSpacingDiagPartCenter2DivLineLabels + 2;
                }
                else if (m_layoutPos == ELayoutPos::Bottom) {
                    x1 = x2 = iDivLine_px;
                    y1 = m_rectContent.top();
                    y2 = m_rectContent.top() + m_iSpacingDiagPartCenter2DivLineLabels - 2;
                }
                else if (m_layoutPos == ELayoutPos::Left) {
                    y1 = y2 = iDivLine_px;
                    x1 = m_rectContent.right();
                    x2 = x1 - m_iSpacingDiagPartCenter2DivLineLabels + 2;
                }
                else if (m_layoutPos == ELayoutPos::Right) {
                    y1 = y2 = iDivLine_px;
                    x2 = m_rectContent.left();
                    x1 = x2 + m_iSpacingDiagPartCenter2DivLineLabels - 2;
                }
                painter.drawLine(x1, y1, x2, y2);
            }
        }
    }
} // paintDivisionLines

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::paintDivisionLineLabels(QPaintDevice* i_pPaintDevice)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qSize2Str(QSize(i_pPaintDevice->width(), i_pPaintDevice->height()));
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "paintDivisionLineLabels",
        /* strAddInfo   */ strMthInArgs );

    QPainter painter(i_pPaintDevice);

    for (int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++)
    {
        if( m_arbShowDivLineLabels[iLayer] )
        {
            painter.setPen(m_arcolDivLineLabels[iLayer]);
            painter.setFont(m_arfntDivLineLabels[iLayer]);

            for (int idxDivLine = 0; idxDivLine < m_divLinesMetrics.getDivLinesCount(iLayer); idxDivLine++)
            {
                // Please note that the division lines metrics calculator is a clone from the
                // diagrams scale object and the geometry set at the calculater already has
                // the margins between the content rectangle of the axis label set as the
                // minimum and maximum pixel range values. No need to move the division line
                // pixel values to the content rectangle of the axis label object.

                if (m_divLinesMetrics.isDivLineLabelVisible(iLayer, idxDivLine))
                {
                    QString strDivLineLabel = m_divLinesMetrics.getDivLineLabelText(iLayer, idxDivLine);
                    QRect rectDivLineLabel = m_divLinesMetrics.getDivLineLabelBoundingRect(iLayer, idxDivLine);
                    QRect rect;

                    if (m_layoutPos == ELayoutPos::Top) {
                        rect.setLeft(rectDivLineLabel.left());
                        rect.setRight(rect.left() + rectDivLineLabel.width());
                        rect.setBottom(m_rectContent.bottom() - m_iSpacingDiagPartCenter2DivLineLabels);
                        rect.setTop(rect.top() - rectDivLineLabel.height());
                    }
                    else if (m_layoutPos == ELayoutPos::Bottom) {
                        rect.setLeft(rectDivLineLabel.left());
                        rect.setRight(rect.left() + rectDivLineLabel.width());
                        rect.setTop(m_rectContent.top() + m_iSpacingDiagPartCenter2DivLineLabels);
                        rect.setBottom(rect.top() + rectDivLineLabel.height());
                    }
                    else if (m_layoutPos == ELayoutPos::Left) {
                        rect.setTop(rectDivLineLabel.top());
                        rect.setBottom(rect.top() + rectDivLineLabel.height());
                        rect.setRight(m_rectContent.right() - m_iSpacingDiagPartCenter2DivLineLabels);
                        rect.setLeft(rect.right() - rectDivLineLabel.width());
                    }
                    else if (m_layoutPos == ELayoutPos::Right) {
                        rect.setTop( rectDivLineLabel.top());
                        rect.setBottom(rect.top() + rectDivLineLabel.height());
                        rect.setLeft(m_rectContent.left() + m_iSpacingDiagPartCenter2DivLineLabels);
                        rect.setRight(rect.left() + rectDivLineLabel.width());
                    }
                    if (m_layoutPos == ELayoutPos::Top || m_layoutPos == ELayoutPos::Bottom) {
                        painter.drawText(rect, Qt::AlignVCenter|Qt::AlignHCenter, strDivLineLabel);
                    }
                    else if (m_layoutPos == ELayoutPos::Left || m_layoutPos == ELayoutPos::Right) {
                        painter.drawText(rect, Qt::AlignVCenter|Qt::AlignRight, strDivLineLabel);
                    }
                }
            }
        }
    }
} // paintDivisionLineLabels

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::paintDivisionLineLabelsUnit(QPaintDevice* i_pPaintDevice)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qSize2Str(QSize(i_pPaintDevice->width(), i_pPaintDevice->height()));
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "paintDivisionLineLabelsUnit",
        /* strAddInfo   */ strMthInArgs );

    QPainter painter(i_pPaintDevice);

    //if (m_divLinesMetrics.scaleDir() == EScaleDir::Y) {
    //    painter.setPen(Qt::red);
    //    painter.setBrush(Qt::yellow);
    //    painter.drawRect(m_rectContent);
    //    painter.setPen(Qt::blue);
    //    painter.setBrush(Qt::white);
    //    painter.drawRect(
    //        m_rectContent.left()+2, m_divLinesMetrics.scaleMaxInPix(),
    //        m_rectContent.width()-4, m_divLinesMetrics.scaleRangeInPix());
    //    painter.setBrush(Qt::NoBrush);
    //}

} // paintDivisionLineLabelsUnit

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::paintScaleMinMaxVals(QPaintDevice* i_pPaintDevice)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qSize2Str(QSize(i_pPaintDevice->width(), i_pPaintDevice->height()));
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "paintScaleMinMaxVals",
        /* strAddInfo   */ strMthInArgs );

    QPainter painter(i_pPaintDevice);

    //if (m_divLinesMetrics.scaleDir() == EScaleDir::Y) {
    //    painter.setPen(Qt::red);
    //    painter.setBrush(Qt::yellow);
    //    painter.drawRect(m_rectContent);
    //    painter.setPen(Qt::blue);
    //    painter.setBrush(Qt::white);
    //    painter.drawRect(
    //        m_rectContent.left()+2, m_divLinesMetrics.scaleMaxInPix(),
    //        m_rectContent.width()-4, m_divLinesMetrics.scaleRangeInPix());
    //    painter.setBrush(Qt::NoBrush);
    //}

} // paintScaleMinMaxVals

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::paintAxisLabel(QPaintDevice* i_pPaintDevice)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qSize2Str(QSize(i_pPaintDevice->width(), i_pPaintDevice->height()));
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "paintAxisLabel",
        /* strAddInfo   */ strMthInArgs );

    QString strAxisLabel;
    if (isAxisLabelVisible()) {
        strAxisLabel = m_strAxisLabel;
        if (m_bShowUnitAtAxisLabel) {
            if (!strAxisLabel.isEmpty()) {
                strAxisLabel += " / ";
            }
            strAxisLabel += m_strPhysUnitLabels;
        }
    }

    if (!strAxisLabel.isEmpty()) {
        QPainter painter(i_pPaintDevice);
        painter.setFont(m_fntAxisLabel);
        painter.setPen(m_colAxisLabel);
        if (m_layoutPos == ELayoutPos::Top || m_layoutPos == ELayoutPos::Bottom) {
            painter.drawText(m_rectAxisLabel, Qt::AlignVCenter|Qt::AlignHCenter, strAxisLabel);
        }
        else if (m_layoutPos == ELayoutPos::Left || m_layoutPos == ELayoutPos::Right) {
            #if QT_VERSION < 0x050000
            QMatrix matrix;
            #else
            QTransform matrix;
            #endif
            matrix.translate(
                static_cast<double>(m_rectAxisLabel.center().x()),
                static_cast<double>(m_rectAxisLabel.center().y()));
            matrix.rotate(-90.0);
            #if QT_VERSION < 0x050000
            painter.setWorldMatrix(matrix);
            #else
            painter.setWorldTransform(matrix);
            #endif
            painter.drawText(
                /* x   */ -m_rectAxisLabel.height()/2,
                /* y   */ m_rectAxisLabel.width()/2,
                /* str */ strAxisLabel );
        }
    }
} // paintAxisLabel
