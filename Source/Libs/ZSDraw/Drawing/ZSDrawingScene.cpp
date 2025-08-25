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

#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Common/ZSDrawMsg.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactory.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactoryGroup.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjConnectionLine.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjConnectionPoint.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjGroup.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjImage.h"
#include "ZSDraw/Common/ZSDrawUnits.h"
#include "ZSSysGUI/ZSSysGUIAux.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysIdxTree.h"
#include "ZSSys/ZSSysMathScaleDivLines.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtCore/qfile.h>
#include <QtCore/qmimedata.h>
#include <QtCore/qsettings.h>
#include <QtCore/qurl.h>

#if QT_VERSION < 0x050000
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include <QtGui/qbitmap.h>
#include <QtGui/qevent.h>
#include <QtGui/qpainter.h>

#if QT_VERSION < 0x050000
#include <QtGui/QGraphicsSceneEvent>
#else
#include <QtWidgets/QGraphicsSceneEvent>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Draw;
using namespace ZS::PhysVal;


/*******************************************************************************
class CDrawingScene : public QGraphicsScene
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

const QString CDrawingScene::s_strGraphObjNameSeparator = "/";

/*==============================================================================
public: // class methods
==============================================================================*/

////------------------------------------------------------------------------------
//QString CDrawingScene::FindUniqueGraphObjId(
//    const QMap<QString,CGraphObj*>& i_dctpGraphObjs,
//    const QString&                  i_strKeyInTreeCurr )
////------------------------------------------------------------------------------
//{
//    QString strKeyInTree = i_strKeyInTreeCurr;
//    int     iObjNr   = 1;
//    QString strObjNr;
//
//    // Remove trailing number (if any).
//    if( strKeyInTree.length() > 0 && isDigitChar(strKeyInTree[strKeyInTree.length()-1]) )
//    {
//        while( isDigitChar(strKeyInTree[strKeyInTree.length()-1]) )
//        {
//            strObjNr.insert( 0, strKeyInTree[strKeyInTree.length()-1] );
//            strKeyInTree.remove( strKeyInTree.length()-1, 1 );
//        }
//        iObjNr = strObjNr.toInt();
//        iObjNr++;
//    }
//    else
//    {
//        strObjNr = QString::number(iObjNr);
//    }
//
//    QString strObjBaseName = strKeyInTree;
//
//    strKeyInTree = strObjBaseName + strObjNr;
//
//    while( i_dctpGraphObjs.contains(strKeyInTree) )
//    {
//        iObjNr++;
//        strObjNr = QString::number(iObjNr);
//        strKeyInTree = strObjBaseName + strObjNr;
//    }
//
//    return strKeyInTree;
//
//} // FindUniqueGraphObjId

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDrawingScene::CDrawingScene(const QString& i_strName, QObject* i_pObjParent) :
//------------------------------------------------------------------------------
    QGraphicsScene(i_pObjParent),
    m_drawingSize(),
    m_divLinesMetricsX(EScaleAxis::X),
    m_divLinesMetricsY(EScaleAxis::Y),
    m_gridSettings(),
    m_drawSettings(),
    m_pDrawSettingsTmp(nullptr),
    m_mode(EMode::Undefined),
    //m_editTool(EEditTool::None),
    //m_editMode(EEditMode::None),
    //m_editResizeMode(EEditResizeMode::None),
    m_pObjFactory(nullptr),
    m_pGraphObjUnderConstruction(nullptr),
    m_pGraphObjMouseGrabber(nullptr),
    //m_pGraphicsItemAddingShapePoints(nullptr),
    //m_pGraphObjAddingShapePoints(nullptr),
    m_pGraphObjsIdxTree(nullptr),
    //m_pGraphObjsIdxTreeClipboard(nullptr),
    //m_arpGraphicsItemsAcceptingHoverEvents(),
    //m_arpGraphicsItemsBroughtToFront(),
    m_fHitTolerance_px(3.0),
    m_ptMouseEvScenePosOnMousePressEvent(),
    m_pGraphicsItemSelectionArea(nullptr),
    m_arpPhysValShapes(),
    m_arDrawSettingsPhysValShapes(),
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjCoordinateConversions(nullptr),
    m_pTrcAdminObjCursor(nullptr),
    m_pTrcAdminObjPaint(nullptr),
    m_pTrcAdminObjMouseClickEvents(nullptr),
    m_pTrcAdminObjMouseMoveEvents(nullptr),
    m_pTrcAdminObjKeyEvents(nullptr)
{
    setObjectName(i_strName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing", ClassName(), objectName());
    m_pTrcAdminObjCoordinateConversions = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing", ClassName() + "::CoordinateConversions", objectName());
    m_pTrcAdminObjCursor = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing", ClassName() + "::Cursor", objectName());
    m_pTrcAdminObjPaint = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing", ClassName() + "::Paint", objectName());
    m_pTrcAdminObjMouseClickEvents = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing", ClassName() + "::MouseClickEvents", objectName());
    m_pTrcAdminObjMouseMoveEvents = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing", ClassName() + "::MouseMoveEvents", objectName());
    m_pTrcAdminObjKeyEvents = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing", ClassName() + "::KeyEvents", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    m_pGraphObjsIdxTree = new CIdxTree(
        /* strObjName       */ "DrawingScene",
        /* pRootTreeEntry   */ nullptr,
        /* strNodeSeparator */ "::",
        /* bCreateMutex     */ false,
        /* pObjParent       */ nullptr );

    QObject::connect(
        &Units.Length, &CUnitsLength::screenResolutionInPxPerMMChanged,
        this, &CDrawingScene::onDrawUnitsScreenResolutionInPxPerMMChanged );
    QObject::connect(
        &Units.Length, &CUnitsLength::scaleFactorChanged,
        this, &CDrawingScene::onDrawUnitsScaleFactorChanged );

    //m_pGraphObjsIdxTreeClipboard = new CIdxTree(
    //    /* strObjName       */ "DrawingScene-" + objectName() + "-Clipboard-GraphObjs",
    //    /* pRootTreeEntry   */ nullptr,
    //    /* strNodeSeparator */ "::",
    //    /* bCreateMutex     */ false,
    //    /* pObjParent       */ nullptr );

    #pragma message(__TODO__"Crashing when using BspTreeIndex and removing labels after they have been moved and deselected.")
    #pragma message(__TODO__"If an object is highlighted or selected its geometry and bounding rectangle changes.")
    #pragma message(__TODO__"Especially for labels drawing anchor lines to their selection points.")
    #pragma message(__TODO__"Very likely a prepareGeometryChange is missing for this.")
    setItemIndexMethod(NoIndex);

    // To update the mode labels:
    setMode( EMode::Edit/*, m_editTool, m_editMode, m_editResizeMode*/ );

    QSettings settings;

    m_drawSettings.load( settings, objectName() + "/DrawSettings" );

} // ctor

//------------------------------------------------------------------------------
CDrawingScene::~CDrawingScene()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    QSettings settings;

    m_drawSettings.save( settings, objectName() + "/DrawSettings" );

    // Clear (and destroy graphical object items) here as dtor of CGraphObj
    // accesses the drawing scene to remove the object name from the hash.
    clear();

    try {
        delete m_pDrawSettingsTmp;
    }
    catch (...) {
    }
    //try {
    //    delete m_pGraphObjsIdxTreeClipboard;
    //} catch(...) {
    //}
    try {
        delete m_pGraphObjsIdxTree;
    }
    catch(...) {
    }

    for (int idxShape = 0; idxShape < m_arpPhysValShapes.size(); ++idxShape) {
        try {
            delete m_arpPhysValShapes[idxShape];
        }
        catch (...) {
        }
        m_arpPhysValShapes[idxShape] = nullptr;
    }
    m_arpPhysValShapes.clear();
    m_arDrawSettingsPhysValShapes.clear();

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjCoordinateConversions);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjCursor);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjPaint);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjMouseClickEvents);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjMouseMoveEvents);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjKeyEvents);

    //m_drawingSize;
    //m_divLinesMetricsX;
    //m_divLinesMetricsY;
    //m_gridSettings;
    //m_drawSettings;
    m_pDrawSettingsTmp = nullptr;
    m_mode = static_cast<ZS::System::EMode>(0);
    //m_editTool = static_cast<EEditTool>(0);
    //m_editMode = static_cast<EEditMode>(0);
    //m_editResizeMode = static_cast<EEditResizeMode>(0);
    m_pObjFactory = nullptr;
    m_pGraphObjUnderConstruction = nullptr;
    m_pGraphObjMouseGrabber = nullptr;
    //m_pGraphicsItemAddingShapePoints = nullptr;
    //m_pGraphObjAddingShapePoints = nullptr;
    //m_strGraphObjNameSeparator;
    m_pGraphObjsIdxTree = nullptr;
    //m_pGraphObjsIdxTreeClipboard = nullptr;
    //m_arpGraphicsItemsAcceptingHoverEvents;
    //m_arpGraphicsItemsBroughtToFront;
    m_fHitTolerance_px = 0.0;
    //m_ptMouseEvScenePosOnMousePressEvent;
    m_pGraphicsItemSelectionArea = nullptr;
    m_pTrcAdminObj = nullptr;
    m_pTrcAdminObjCoordinateConversions = nullptr;
    m_pTrcAdminObjCursor = nullptr;
    m_pTrcAdminObjPaint = nullptr;
    m_pTrcAdminObjMouseClickEvents = nullptr;
    m_pTrcAdminObjMouseMoveEvents = nullptr;
    m_pTrcAdminObjKeyEvents = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::setDrawingSize( const CDrawingSize& i_drawingSize)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_drawingSize.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setDrawingSize",
        /* strAddInfo   */ strMthInArgs );

    if (m_drawingSize != i_drawingSize)
    {
        m_drawingSize = i_drawingSize;

        Units.Length.setScreenResolutionInPxPerMM(m_drawingSize.screenResolutionInPxPerMM());
        Units.Length.setScaleFactor(m_drawingSize.scaleFactorDividend(), m_drawingSize.scaleFactorDivisor());

        // When using metric units, one pixel will be automatically added to the pixel range
        // of the scene to draw lines at pixel positions.
        QSizeF sizeF = m_drawingSize.imageSizeInPixels();
        QRectF rect(QPointF(0.0, 0.0), sizeF);
        setSceneRect(rect);
        updateDivLinesMetrics();
        update();
        emit_drawingSizeChanged(m_drawingSize);
    }
}

//------------------------------------------------------------------------------
const CDrawingSize& CDrawingScene::drawingSize() const
//------------------------------------------------------------------------------
{
    return m_drawingSize;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Converts the given point in pixels into the current unit of the drawing scene.

    @param [in] i_pt

    @return Converted value.
*/
CPhysValPoint CDrawingScene::convert(const QPointF& i_pt) const
//------------------------------------------------------------------------------
{
    return convert(CPhysValPoint(*this, i_pt, Units.Length.px), m_drawingSize.unit());
}

//------------------------------------------------------------------------------
/*! @brief Converts the given point in pixels into the desired unit.

    @param [in] i_pt
    @param [in] i_unitDst

    @return Converted value.
*/
CPhysValPoint CDrawingScene::convert(const QPointF& i_pt, const CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    return convert(CPhysValPoint(*this, i_pt, Units.Length.px), i_unitDst);
}

//------------------------------------------------------------------------------
/*! @brief Converts the given point into the current unit of the drawing scene.

    @param [in] i_physValPoint

    @return Converted value.
*/
CPhysValPoint CDrawingScene::convert(const CPhysValPoint& i_physValPoint) const
//------------------------------------------------------------------------------
{
    return convert(i_physValPoint, m_drawingSize.unit());
}

//------------------------------------------------------------------------------
/*! @brief Converts the given point into the desired unit.

    @param [in] i_physValPoint
    @param [in] i_unitDst

    @return Converted value.
*/
CPhysValPoint CDrawingScene::convert(const CPhysValPoint& i_physValPoint, const CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    CPhysValPoint physValPoint = i_physValPoint;
    if (i_physValPoint.unit() != i_unitDst) {
        if (Units.Length.isMetricUnit(i_physValPoint.unit()) && Units.Length.isMetricUnit(i_unitDst)) {
            CPhysVal physValX = i_physValPoint.x();
            CPhysVal physValY = i_physValPoint.y();
            physValX.convertValue(i_unitDst);
            physValY.convertValue(i_unitDst);
            physValPoint = CPhysValPoint(*this, physValX, physValY);
        }
        else if ((i_physValPoint.unit() == Units.Length.px) && Units.Length.isMetricUnit(i_unitDst)) {
            QPointF pt = i_physValPoint.toQPointF();
            double fx = m_divLinesMetricsX.getVal(pt.x(), false);
            double fy = m_divLinesMetricsY.getVal(pt.y(), false);
            CPhysVal physValX(fx, m_drawingSize.unit(), m_drawingSize.imageCoorsResolution());
            CPhysVal physValY(fy, m_drawingSize.unit(), m_drawingSize.imageCoorsResolution());
            physValX.convertValue(i_unitDst);
            physValY.convertValue(i_unitDst);
            physValPoint = CPhysValPoint(*this, physValX, physValY);
        }
        else if (Units.Length.isMetricUnit(i_physValPoint.unit()) && (i_unitDst == Units.Length.px)) {
            CPhysVal physValX = i_physValPoint.x();
            CPhysVal physValY = i_physValPoint.y();
            double fX_px = m_divLinesMetricsX.getValInPix(physValX.getVal(m_drawingSize.unit()));
            double fY_px = m_divLinesMetricsY.getValInPix(physValY.getVal(m_drawingSize.unit()));
            physValPoint = CPhysValPoint(*this, fX_px, fY_px, i_unitDst);
        }
    }
    return physValPoint;
}

//------------------------------------------------------------------------------
/*! @brief Converts the given size in pixels into the current unit of the drawing scene.

    @param [in] i_size

    @return Converted value.
*/
CPhysValSize CDrawingScene::convert(const QSizeF& i_size) const
//------------------------------------------------------------------------------
{
    return convert(CPhysValSize(*this, i_size, Units.Length.px), m_drawingSize.unit());
}

//------------------------------------------------------------------------------
/*! @brief Converts the given size in pixels into the desired unit.

    @param [in] i_size
    @param [in] i_unitDst

    @return Converted value.
*/
CPhysValSize CDrawingScene::convert(const QSizeF& i_size, const CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    return convert(CPhysValSize(*this, i_size, Units.Length.px), i_unitDst);
}

//------------------------------------------------------------------------------
/*! @brief Converts the given size into the current unit of the drawing scene.

    @param [in] i_physValSize

    @return Converted value.
*/
CPhysValSize CDrawingScene::convert(const CPhysValSize& i_physValSize) const
//------------------------------------------------------------------------------
{
    return convert(i_physValSize, m_drawingSize.unit());
}

//------------------------------------------------------------------------------
/*! @brief Converts the given size into the desired unit.

    @param [in] i_physValSize
    @param [in] i_unitDst

    @return Converted value.
*/
CPhysValSize CDrawingScene::convert(const CPhysValSize& i_physValSize, const CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    CPhysValSize physValSize = i_physValSize;
    if (i_physValSize.unit() != i_unitDst) {
        if (Units.Length.isMetricUnit(i_physValSize.unit()) && Units.Length.isMetricUnit(i_unitDst)) {
            CPhysVal physValWidth = i_physValSize.width();
            CPhysVal physValHeight = i_physValSize.width();
            physValWidth.convertValue(i_unitDst);
            physValHeight.convertValue(i_unitDst);
            CPhysValRes physValRes = m_drawingSize.imageCoorsResolution();
            physValRes.convertValue(i_unitDst);
            physValWidth.setRes(physValRes);
            physValHeight.setRes(physValRes);
            physValSize = CPhysValSize(*this, physValWidth, physValHeight);
        }
        else if ((i_physValSize.unit() == Units.Length.px) && Units.Length.isMetricUnit(i_unitDst)) {
            // The drawing size in pixels has been incremented by one pixel.
            // If the pixel resolution is e.g. 3.5 px/mm, and the width is 36 px, the width is 10 mm.
            QSizeF sizeF = i_physValSize.toQSizeF();
            double dx = m_divLinesMetricsX.getDistance(sizeF.width());
            double dy = m_divLinesMetricsY.getDistance(sizeF.height());
            CPhysVal physValWidth(dx, m_drawingSize.unit(), m_drawingSize.imageCoorsResolution());
            CPhysVal physValHeight(dy, m_drawingSize.unit(), m_drawingSize.imageCoorsResolution());
            physValWidth.convertValue(i_unitDst);
            physValHeight.convertValue(i_unitDst);
            physValSize = CPhysValSize(*this, physValWidth, physValHeight);
        }
        else if (Units.Length.isMetricUnit(i_physValSize.unit()) && (i_unitDst == Units.Length.px)) {
            // The drawing size in pixels has been incremented by one pixel.
            // If the pixel resolution is e.g. 3.5 px/mm, and the width is 36 px, the width is 10 mm.
            double dx = i_physValSize.width().getVal(m_drawingSize.unit());
            double dy = i_physValSize.height().getVal(m_drawingSize.unit());
            double dx_px = m_divLinesMetricsX.getDistanceInPix(dx);
            double dy_px = m_divLinesMetricsY.getDistanceInPix(dy);
            CPhysVal physValWidth(dx_px, Units.Length.px, m_drawingSize.imageCoorsResolutionInPx());
            CPhysVal physValHeight(dy_px, Units.Length.px, m_drawingSize.imageCoorsResolutionInPx());
            physValSize = CPhysValSize(*this, physValWidth, physValHeight);
        }
    }
    return physValSize;
}

//------------------------------------------------------------------------------
/*! @brief Converts the given line in pixels into the current unit of the drawing scene.

    @param [in] i_line

    @return Converted value.
*/
CPhysValLine CDrawingScene::convert(const QLineF& i_line) const
//------------------------------------------------------------------------------
{
    return convert(CPhysValLine(*this, i_line, Units.Length.px), m_drawingSize.unit());
}

//------------------------------------------------------------------------------
/*! @brief Converts the given line in pixels into the desired unit.

    @param [in] i_line
    @param [in] i_unitDst

    @return Converted value.
*/
CPhysValLine CDrawingScene::convert(const QLineF& i_line, const CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    return convert(CPhysValLine(*this, i_line, Units.Length.px), i_unitDst);
}

//------------------------------------------------------------------------------
/*! @brief Converts the given size into the current unit of the drawing scene.

    @param [in] i_physValPoint

    @return Converted value.
*/
CPhysValLine CDrawingScene::convert(const CPhysValLine& i_physValLine) const
//------------------------------------------------------------------------------
{
    return convert(i_physValLine, m_drawingSize.unit());
}

//------------------------------------------------------------------------------
/*! @brief Converts the given line value into the desired unit.

    @param [in] i_physValLine
    @param [in] i_unitDst

    @return Converted value.
*/
CPhysValLine CDrawingScene::convert(const CPhysValLine& i_physValLine, const CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    CPhysValPoint physValP1 = convert(i_physValLine.p1(), i_unitDst);
    CPhysValPoint physValP2 = convert(i_physValLine.p2(), i_unitDst);
    CPhysValLine physValLine(physValP1, physValP2);
    return physValLine;
}

//------------------------------------------------------------------------------
CPhysValPolygon CDrawingScene::convert(const QPolygonF& i_polygon) const
//------------------------------------------------------------------------------
{
    return convert(CPhysValPolygon(*this, i_polygon, Units.Length.px), m_drawingSize.unit());
}

//------------------------------------------------------------------------------
CPhysValPolygon CDrawingScene::convert(const QPolygonF& i_polygon, const ZS::PhysVal::CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    return convert(CPhysValPolygon(*this, i_polygon, Units.Length.px), i_unitDst);
}

//------------------------------------------------------------------------------
CPhysValPolygon CDrawingScene::convert(const CPhysValPolygon& i_physValPolygon) const
//------------------------------------------------------------------------------
{
    return convert(i_physValPolygon, m_drawingSize.unit());
}

//------------------------------------------------------------------------------
CPhysValPolygon CDrawingScene::convert(const CPhysValPolygon& i_physValPolygon, const ZS::PhysVal::CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    CPhysValPolygon physValPolygonTmp = i_physValPolygon;
    physValPolygonTmp.setAngle(0.0);
    QPolygonF polygon;
    for (int idxPt = 0; idxPt < i_physValPolygon.count(); ++idxPt) {
        QPointF pt = convert(physValPolygonTmp.at(idxPt), i_unitDst).toQPointF();
        polygon.append(pt);
    }
    CPhysValPolygon physValPolygon(*this, polygon, i_unitDst, i_physValPolygon.isClosedPolygon());
    physValPolygon.setAngle(i_physValPolygon.angle());
    return physValPolygon;
}

//------------------------------------------------------------------------------
/*! @brief Converts the given rectangle in pixels into the current unit of the drawing scene.

    @param [in] i_rect

    @return Converted value.
*/
CPhysValRect CDrawingScene::convert(const QRectF& i_rect) const
//------------------------------------------------------------------------------
{
    return convert(CPhysValRect(*this, i_rect.topLeft(), i_rect.size(), Units.Length.px), m_drawingSize.unit());
}

//------------------------------------------------------------------------------
/*! @brief Converts the given rectangle in pixels into the desired unit.

    @param [in] i_rect
    @param [in] i_unitDst

    @return Converted value.
*/
CPhysValRect CDrawingScene::convert(const QRectF& i_rect, const CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    return convert(CPhysValRect(*this, i_rect.topLeft(), i_rect.size(), Units.Length.px), i_unitDst);
}

//------------------------------------------------------------------------------
/*! @brief Converts the given size into the current unit of the drawing scene.

    @param [in] i_physValPoint

    @return Converted value.
*/
CPhysValRect CDrawingScene::convert(const CPhysValRect& i_physValRect) const
//------------------------------------------------------------------------------
{
    return convert(i_physValRect, m_drawingSize.unit());
}

//------------------------------------------------------------------------------
/*! @brief Converts the given rectangle into the desired unit.

    When converting from pixels into metric unit or from metric unit to pixels
    and if the Y Scale is from bottom to top the rectangles height becomes negative.

    On the screen the top line of the rectangle is alway above the bottom line.
    If Y-Scale Orientation is from bottom to top the Y coordinate of the bottom line
    has a smaller value than the Y coordinate of the top line.

    @param [in] i_physValRect
    @param [in] i_unitDst

    @return Converted value.
*/
CPhysValRect CDrawingScene::convert(const CPhysValRect& i_physValRect, const CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    CPhysValRect physValRect(*this, i_unitDst);
    CPhysValSize physValSize = convert(i_physValRect.size(), i_unitDst);
    CPhysValPoint physValCenter = convert(i_physValRect.center(), i_unitDst);
    physValRect.setSize(physValSize);
    physValRect.setCenter(physValCenter);
    physValRect.setAngle(i_physValRect.angle());
    return physValRect;
}

////------------------------------------------------------------------------------
///*! @brief Converts the given X coordinate into the pixel coordinate.
//
//    @return Converted value.
//*/
//double CDrawingScene::toPixelXCoor(const CPhysVal& i_physValXCoor) const
////------------------------------------------------------------------------------
//{
//    return m_drawingSize.toPixelXCoor(i_physValXCoor);
//}
//
////------------------------------------------------------------------------------
///*! @brief Converts the given Y coordinate into the pixel coordinate.
//
//    @return Converted value.
//*/
//double CDrawingScene::toPixelYCoor(const CPhysVal& i_physValYCoor) const
////------------------------------------------------------------------------------
//{
//    return m_drawingSize.toPixelYCoor(i_physValYCoor);
//}
//
////------------------------------------------------------------------------------
//CPhysValPoint CDrawingScene::toPixelCoor(const CPhysValPoint& i_physValPoint) const
////------------------------------------------------------------------------------
//{
//    return m_drawingSize.toPixelCoor(i_physValPoint);
//}
//
////------------------------------------------------------------------------------
//CPhysValSize CDrawingScene::toPixelCoor(const CPhysValSize& i_physValSize) const
////------------------------------------------------------------------------------
//{
//    return m_drawingSize.toPixelCoor(i_physValSize);
//}
//
////------------------------------------------------------------------------------
//CPhysValLine CDrawingScene::toPixelCoor(const CPhysValLine& i_physValLine) const
////------------------------------------------------------------------------------
//{
//    return m_drawingSize.toPixelCoor(i_physValLine);
//}
//
////------------------------------------------------------------------------------
//CPhysValRect CDrawingScene::toPixelCoor(const CPhysValRect& i_physValRect) const
////------------------------------------------------------------------------------
//{
//    return m_drawingSize.toPixelCoor(i_physValRect);
//}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::setGridSettings( const CDrawGridSettings& i_gridSettings)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_gridSettings.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridSettings",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings != i_gridSettings )
    {
        m_gridSettings = i_gridSettings;

        QFont font = m_gridSettings.labelsFont();
        CEnumTextStyle textStyle = m_gridSettings.labelsTextStyle();
        CEnumTextEffect textEffect = m_gridSettings.labelsTextEffect();
        ETextSize textSize = m_gridSettings.labelsTextSize();

        if (isTextStyleBold(textStyle)) {
            font.setBold(true);
        }
        if (isTextStyleItalic(textStyle)) {
            font.setItalic(true);
        }
        if (isTextEffectUnderline(textEffect)) {
            font.setUnderline(true);
        }
        if (isTextEffectStrikeout(textEffect)) {
            font.setStrikeOut(true);
        }
        font.setPixelSize(textSize2SizeInPixels(textSize));

        m_divLinesMetricsX.setDivLinesDistMinInPix(EDivLineLayer::Main, m_gridSettings.linesDistMin());
        m_divLinesMetricsX.setFont(font);
        m_divLinesMetricsX.update();

        m_divLinesMetricsY.setDivLinesDistMinInPix(EDivLineLayer::Main, m_gridSettings.linesDistMin());
        m_divLinesMetricsY.setFont(font);
        m_divLinesMetricsY.update();

        update();

        emit_gridSettingsChanged(m_gridSettings);
    }
}

//------------------------------------------------------------------------------
const CDrawGridSettings& CDrawingScene::gridSettings() const
//------------------------------------------------------------------------------
{
    return m_gridSettings;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CDrawingScene::load( const QString& i_strFileName )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "FileName:" + i_strFileName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "load",
        /* strAddInfo   */ strMthInArgs );

    clear();

    SErrResultInfo errResultInfo;

    QFile fileXML;

    if (i_strFileName.isEmpty()) {
        errResultInfo = ErrResultInfoError("load", EResultInvalidFileName, "File name is empty");
    }
    else {
        fileXML.setFileName(i_strFileName);
        if (!fileXML.exists()) {
            errResultInfo = ErrResultInfoError("load", EResultFileNotFound, i_strFileName);
        }
        else if (!fileXML.open(QIODevice::ReadOnly)) {
            errResultInfo = ErrResultInfoError("load", EResultFileOpenForRead, i_strFileName);
        }
    }

    if (!errResultInfo.isErrorResult()) {
        QXmlStreamReader xmlStreamReader(&fileXML);
        QXmlStreamReader::TokenType xmlStreamTokenType = xmlStreamReader.readNext();
        if (xmlStreamTokenType != QXmlStreamReader::StartDocument) {
            xmlStreamReader.raiseError("Invalid XML document");
        }
        while (!xmlStreamReader.hasError() && !xmlStreamReader.atEnd()) {
            xmlStreamTokenType = xmlStreamReader.readNext();
            if (xmlStreamReader.isStartElement() || xmlStreamReader.isEndElement()) {
                QString strElemName = xmlStreamReader.name().toString();
                if (xmlStreamReader.isStartElement()) {
                    if (strElemName == XmlStreamParser::c_strXmlElemNameDrawing) {
                        CDrawingSize drawingSize;
                        drawingSize.load(xmlStreamReader);
                        if (!xmlStreamReader.hasError()) {
                            setDrawingSize(drawingSize);
                        }
                    }
                    else if (strElemName == XmlStreamParser::c_strXmlElemNameGridSettings) {
                        QXmlStreamAttributes xmlStreamAttrs = xmlStreamReader.attributes();
                        CDrawGridSettings gridSettings;
                        gridSettings.load(xmlStreamReader);
                        if (!xmlStreamReader.hasError()) {
                            setGridSettings(gridSettings);
                        }
                    }
                    else if (strElemName == XmlStreamParser::c_strXmlElemNameGraphObj) {
                        QString strFactoryGroupName;
                        QString strGraphObjType;
                        QString strObjName;
                        QXmlStreamAttributes xmlStreamAttrs = xmlStreamReader.attributes();
                        if (xmlStreamAttrs.hasAttribute(XmlStreamParser::c_strXmlElemNameGraphObjFactoryGroupName)) {
                            strFactoryGroupName = xmlStreamAttrs.value(XmlStreamParser::c_strXmlElemNameGraphObjFactoryGroupName).toString();
                        }
                        else {
                            XmlStreamParser::raiseErrorAttributeNotDefined(
                                xmlStreamReader, strElemName, XmlStreamParser::c_strXmlElemNameGraphObjFactoryGroupName);
                        }
                        if (!xmlStreamReader.hasError()) {
                            if (xmlStreamAttrs.hasAttribute(XmlStreamParser::c_strXmlElemNameGraphObjType)) {
                                strGraphObjType = xmlStreamAttrs.value(XmlStreamParser::c_strXmlElemNameGraphObjType).toString();
                            }
                            else {
                                XmlStreamParser::raiseErrorAttributeNotDefined(
                                    xmlStreamReader, strElemName, XmlStreamParser::c_strXmlElemNameGraphObjType);
                            }
                        }
                        if (!xmlStreamReader.hasError()) {
                            if (xmlStreamAttrs.hasAttribute(XmlStreamParser::c_strXmlElemNameGraphObjName)) {
                                strObjName = xmlStreamAttrs.value(XmlStreamParser::c_strXmlElemNameGraphObjName).toString();
                            }
                            else {
                                XmlStreamParser::raiseErrorAttributeNotDefined(
                                    xmlStreamReader, strElemName, XmlStreamParser::c_strXmlElemNameGraphObjName);
                            }
                        }
                        if (!xmlStreamReader.hasError()) {
                            CObjFactory* pObjFactory = CObjFactory::FindObjFactory(strFactoryGroupName, strGraphObjType);
                            if (pObjFactory == nullptr) {
                                xmlStreamReader.raiseError(
                                    "ObjectFactory \"" + strFactoryGroupName + "::" + strGraphObjType + "\" for element \"" + strElemName + "\" not found");
                            }
                            else {
                                try {
                                    pObjFactory->loadGraphObj(
                                        /* pDrawingScene   */ this,
                                        /* pGraphObjGroup  */ nullptr,
                                        /* strObjName      */ strObjName,
                                        /* xmlStreamReader */ xmlStreamReader );
                                }
                                catch(CException& exc) {
                                    QString strAddErrInfo = exc.toString();
                                    errResultInfo = ErrResultInfoError("load", EResultFileReadContent, strAddErrInfo);
                                }
                            }
                        }
                    }
                } // if (xmlStreamReader.isStartElement())
            } // if (xmlStreamReader.isStartElement() || xmlStreamReader.isEndElement())
        } // while (!xmlStreamReader.hasError() && !xmlStreamReader.atEnd())

        if (xmlStreamReader.hasError()) {
            QString strAddErrInfo;
            strAddErrInfo += xmlStreamReader.errorString() + " on reading \"" + i_strFileName + "\" ";
            strAddErrInfo += "(Line:" + QString::number(xmlStreamReader.lineNumber());
            strAddErrInfo += ", Column:" + QString::number(xmlStreamReader.columnNumber()) + ")";
            errResultInfo = ErrResultInfoError("load", EResultFileReadContent, strAddErrInfo);
        }
    } // if (!errResultInfo.isErrorResult())

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(errResultInfo);
    }
    return errResultInfo;

} // load

//------------------------------------------------------------------------------
SErrResultInfo CDrawingScene::save( const QString& i_strFileName )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "FileName:" + i_strFileName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "save",
        /* strAddInfo   */ strMthInArgs );

    SErrResultInfo errResultInfo;

    QFile fileXML;

    if (i_strFileName.isEmpty()) {
        errResultInfo = ErrResultInfoError("save", EResultInvalidFileName, "File name is empty");
    }
    else {
        fileXML.setFileName(i_strFileName);
        if (!fileXML.open(QIODevice::WriteOnly)) {
            errResultInfo = ErrResultInfoError("save", EResultFileOpenForWrite, i_strFileName);
        }
    }

    if (!errResultInfo.isErrorResult()) {
        QXmlStreamWriter xmlStreamWriter(&fileXML);
        xmlStreamWriter.setAutoFormatting(true);
        xmlStreamWriter.writeStartDocument();
        xmlStreamWriter.writeStartElement(XmlStreamParser::c_strXmlElemNameDrawing);
        m_drawingSize.save(xmlStreamWriter);

        if (!m_gridSettings.isDefault()) {
            xmlStreamWriter.writeStartElement(XmlStreamParser::c_strXmlElemNameGridSettings);
            m_gridSettings.save(xmlStreamWriter);
            xmlStreamWriter.writeEndElement();
        }

        // Connection points need to be recalled before the connection lines as on
        // creating the connection lines their connection points must already exist.
        // For this the connection lines will be saved at the end of the XML file.
        // Labels and selection points will not be saved at all (labels are created by their parents).
        CIdxTree::iterator itIdxTree = m_pGraphObjsIdxTree->begin(CIdxTree::iterator::ETraversalOrder::PreOrder);
        while (itIdxTree != m_pGraphObjsIdxTree->end()) {
            CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(*itIdxTree);
            if (pGraphObj != nullptr && !pGraphObj->isSelectionPoint() && !pGraphObj->isLabel() && !pGraphObj->isConnectionLine()) {
                // Group members will be saved as child items of the groups.
                if (pGraphObj->parentGroup() == nullptr) {
                    errResultInfo = save(pGraphObj, xmlStreamWriter);
                    if (errResultInfo.isErrorResult()) {
                        break;
                    }
                }
            }
            ++itIdxTree;
        }
        if (!errResultInfo.isErrorResult()) {
            CIdxTree::iterator itIdxTree = m_pGraphObjsIdxTree->begin(CIdxTree::iterator::ETraversalOrder::PreOrder);
            while (itIdxTree != m_pGraphObjsIdxTree->end()) {
                CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(*itIdxTree);
                if (pGraphObj != nullptr && pGraphObj->isConnectionLine()) {
                    // Group members will be saved as child items of the groups.
                    if (pGraphObj->parentGroup() == nullptr) {
                        errResultInfo = save(pGraphObj, xmlStreamWriter);
                        if (errResultInfo.isErrorResult()) {
                            break;
                        }
                    }
                }
                ++itIdxTree;
            }
        }
        xmlStreamWriter.writeEndElement();  // Drawing
        xmlStreamWriter.writeEndDocument();
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(errResultInfo);
    }
    return errResultInfo;

} // save

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CDrawingScene::save( CGraphObj* i_pGraphObj, QXmlStreamWriter& i_xmlStreamWriter )
//------------------------------------------------------------------------------
{
    if (i_pGraphObj == nullptr) {
        throw CException( __FILE__, __LINE__, EResultArgOutOfRange, "pGraphObj == nullptr" );
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "GraphObj: " + i_pGraphObj->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "save",
        /* strAddInfo   */ strMthInArgs );

    SErrResultInfo errResultInfo;

    QString strFactoryGroupName = i_pGraphObj->factoryGroupName();
    QString strGraphObjType = i_pGraphObj->typeAsString();
    QString strObjName = i_pGraphObj->name();

    CObjFactory* pObjFactory = CObjFactory::FindObjFactory(strFactoryGroupName, strGraphObjType);

    if (pObjFactory == nullptr) {
        QString strAddErrInfo = strFactoryGroupName + "::" + strGraphObjType;
        errResultInfo = ErrResultInfoError("save", EResultObjFactoryNotFound, strAddErrInfo);
    }
    else {
        i_xmlStreamWriter.writeStartElement(XmlStreamParser::c_strXmlElemNameGraphObj);
        i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlElemNameGraphObjFactoryGroupName, strFactoryGroupName);
        i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlElemNameGraphObjType, strGraphObjType);
        i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlElemNameGraphObjName, strObjName);
        errResultInfo = pObjFactory->saveGraphObj(i_pGraphObj, i_xmlStreamWriter);
        i_xmlStreamWriter.writeEndElement();
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(errResultInfo);
    }
    return errResultInfo;

} // save

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::clear()
//------------------------------------------------------------------------------
{
   CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "clear",
        /* strAddInfo   */ "" );

    emit_aboutToBeCleared();

    m_pGraphicsItemSelectionArea = nullptr;
    m_pGraphObjUnderConstruction = nullptr;
    m_pGraphObjMouseGrabber = nullptr;
    //m_pGraphicsItemAddingShapePoints = nullptr;
    //m_pGraphObjAddingShapePoints = nullptr;

    // Clear internal clipboard.
    //CGraphObj* pGraphObj;
    //CIdxTree::iterator itIdxTree = m_pGraphObjsIdxTreeClipboard->begin(CIdxTree::iterator::ETraversalOrder::PreOrder);
    //while( itIdxTree != m_pGraphObjsIdxTreeClipboard->end() )
    //{
    //    pGraphObj = dynamic_cast<CGraphObj*>(*itIdxTree);
    //    deleteItem(pGraphObj);
    //    ++itIdxTree;
    //}
    //m_pGraphObjsIdxTreeClipboard->clear();
    m_pGraphObjsIdxTree->clear();

    QGraphicsScene::clear();

    //m_arpGraphicsItemsAcceptingHoverEvents.clear();
    //m_arpGraphicsItemsBroughtToFront.clear();

} // clear

//------------------------------------------------------------------------------
/*! Adds the given graph object as an entry to the index tree of graphical objects
    and as an graphics item to the graphics scene.

    If a parent is passed the given graph object will also be added as a child to
    the parent object. This applies both for the tree entry in the index tree as well
    as for the graphics item of Qt's graphics scene.

    @note
        Selection points and labels are "directly" added to the graphics scene as
        they should not appear in the index tree.
        Selection points and labels are not created via an object factory but are
        created by the parent objects they belong to.
        As a rule you could say that "addGraphObj" is only called if the objects
        were created by a factory.
        There is no "removeGraphObj" method. The destructor of the graphical objects
        are emitting the "aboutToBeDestroyed" signal and the drawings scene's slot
        removes the object from the index tree, the graphics scene and some other
        internal states if necessary.

    @param i_pGraphObj [in]
        Pointer to object to be added to the drawing scene.
        The object will be added to the index tree as well as to Qt's graphics scene.
    @param i_pGraphObjParent [in]
        Pointer to parent object or nullptr.
        If a parent object is given, the parent object must have already been added
        before to the drawing scene. The given graph object will be added as a child
        to the parents index tree entry as well as to the parents graphics item.
*/
void CDrawingScene::addGraphObj( CGraphObj* i_pGraphObj, CGraphObjGroup* i_pGraphObjParent )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs =
            QString(i_pGraphObj == nullptr ? "nullptr" : i_pGraphObj->path()) +
            ", Parent: " + QString(i_pGraphObjParent == nullptr ? "nullptr" : i_pGraphObjParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "addGraphObj",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(i_pGraphObj);
    if (pGraphicsItem == nullptr) {
        throw CException(
            __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "dynamic_cast<QGraphicsItem*>(i_pGraphObj)");
    }

    // Please note that selection points and labels should not belong as child to the graphics items.
    // Otherwise the "boundingRect" call of groups (which implicitly calls childrenBoundingRect) does
    // not work as expected as the childs bounding rectangles would be included.
    // In addition selection points and labels must be directly added to the graphics scene as they
    // should not be indicated in the index tree.
    if (i_pGraphObj->isSelectionPoint()) {
        throw CException(
            __FILE__, __LINE__, EResultInvalidMethodCall, "Selection points must be directly added to the graphics scene");
    }
    else if (i_pGraphObj->isLabel()) {
        throw CException(
            __FILE__, __LINE__, EResultInvalidMethodCall, "Labels must be directly added to the graphics scene");
    }

    //QGraphicsItem* pGraphicsItemParent = dynamic_cast<QGraphicsItem*>(i_pGraphObjParent);

    // On adding the item to the graphics scene the itemChange method of the graphics
    // item is called with "SceneHasChanged". On initially setting the drawing scene,
    // the graphical object has to recalculate the position in pixel coordinates.
    //if (pGraphicsItemParent != nullptr && pGraphicsItem->parentItem() != pGraphicsItemParent) {
    //    pGraphicsItem->setParentItem(pGraphicsItemParent);
    //}
    //else {
        QGraphicsScene::addItem(pGraphicsItem);
    //}
    //m_pGraphObjsIdxTree->add(i_pGraphObj, i_pGraphObjParent);
    m_pGraphObjsIdxTree->add(i_pGraphObj);

    if (i_pGraphObjParent != nullptr && i_pGraphObj->parent() != i_pGraphObjParent) {
        i_pGraphObjParent->addToGroup(i_pGraphObj, true);
    }
    QObject::connect(
        i_pGraphObj, &CGraphObj::aboutToBeDestroyed,
        this, &CDrawingScene::onGraphObjAboutToBeDestroyed);

} // addGraphObj

//------------------------------------------------------------------------------
/*! Deletes the given graph object.

    The method may be called by graphical objects itself. E.g. when drawing connection
    lines and the end point is not on a connection point. In those cases the object
    may not be immediately deleted but the delete call must be queued.

    @param i_pGraphObj [in]
        Pointer to object to be deleted and removed from the drawing scene.
    @param i_bQueuedCall [in]
        true, to queue the method call via event.
        false, if the object can immediately deleted.
*/
void CDrawingScene::deleteGraphObj(CGraphObj* i_pGraphObj, bool i_bQueuedCall)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs =
            QString(i_pGraphObj == nullptr ? "nullptr" : i_pGraphObj->path()) +
            ", QueuedCall: " + bool2Str(i_bQueuedCall);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "deleteGraphObj",
        /* strAddInfo   */ strMthInArgs );

    if (i_pGraphObj != nullptr) {
        if (i_bQueuedCall) {
            CMsgReqDeleteGraphObj* pMsgReqDeleteGraphObj =
                new CMsgReqDeleteGraphObj(i_pGraphObj, this, i_pGraphObj->keyInTree());
            QCoreApplication::postEvent(this, pMsgReqDeleteGraphObj);
        }
        else {
            delete i_pGraphObj;
            i_pGraphObj = nullptr;
        }
    }
}

//------------------------------------------------------------------------------
QGraphicsItem* CDrawingScene::findGraphicsItem( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    QGraphicsItem* pGraphicsItem = nullptr;
    CIdxTreeEntry* pTreeEntry = m_pGraphObjsIdxTree->findEntry(i_strKeyInTree);
    if( pTreeEntry != nullptr )
    {
        pGraphicsItem = dynamic_cast<QGraphicsItem*>(pTreeEntry);
    }
    return pGraphicsItem;
}

//------------------------------------------------------------------------------
CGraphObj* CDrawingScene::findGraphObj( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    CGraphObj* pGraphObj = nullptr;
    CIdxTreeEntry* pTreeEntry = m_pGraphObjsIdxTree->findEntry(i_strKeyInTree);
    if( pTreeEntry != nullptr )
    {
        pGraphObj = dynamic_cast<CGraphObj*>(pTreeEntry);
    }
    return pGraphObj;
}

/*==============================================================================
public: // instance methods (replacing methods of QGraphicScene)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Removes the item from the graphics scene by invoking the method
           of the base class QGraphicsScene.

    Some objects like selection points and labels are not part of the index
    tree of graphical objects and are directly added to the scene and
    the drawing scene will not be informed if the objects are about to be
    destroyed (the onGraphObjAboutToBeDestroyed slot is not connected to the
    aboutToBeDestroyed signal of the graphical object).

    When selecting the labels or selection points they become part of some
    internal lists of the drawing scene (e.g. in the list of objects temporarily
    accepting hover events). If the graphical object is removed from the scene
    the item must also be removed from those internal lists. For this the labels
    and selection points need to call the removeItem method of the drawing scene.

    @param [in] i_pGraphicsItem
        Pointer to item which is going to be removed from the graphics scene.
*/
void CDrawingScene::removeItem(QGraphicsItem* i_pGraphicsItem)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);
        strMthInArgs = QString(pGraphObj == nullptr ? "nullptr" : pGraphObj->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "removeItem",
        /* strAddInfo   */ strMthInArgs );

    if (i_pGraphicsItem != nullptr && i_pGraphicsItem->scene() != nullptr) {
        QGraphicsScene::removeItem(i_pGraphicsItem);
    }
    if (dynamic_cast<CGraphObj*>(i_pGraphicsItem) == m_pGraphObjUnderConstruction) {
        QObject::disconnect(
            m_pGraphObjUnderConstruction, &CGraphObj::editModeChanged,
            this, &CDrawingScene::onGraphObjEditModeChanged );
        m_pGraphObjUnderConstruction = nullptr;
    }
    if (dynamic_cast<CGraphObj*>(i_pGraphicsItem) == m_pGraphObjMouseGrabber) {
        m_pGraphObjMouseGrabber = nullptr;
    }
    //if (m_pGraphObjAddingShapePoints != nullptr && m_pGraphObjAddingShapePoints == i_pGraphObj) {
    //    m_pGraphicsItemAddingShapePoints = nullptr;
    //    m_pGraphObjAddingShapePoints = nullptr;
    //}

    //if (i_pGraphicsItem != nullptr && m_arpGraphicsItemsAcceptingHoverEvents.size() > 0) {
    //    invalidateItemInAcceptingHoverEventsList(i_pGraphicsItem);
    //    for (int idxGraphObj = m_arpGraphicsItemsAcceptingHoverEvents.size()-1; idxGraphObj >= 0; idxGraphObj--) {
    //        if (m_arpGraphicsItemsAcceptingHoverEvents[idxGraphObj] == nullptr) {
    //            m_arpGraphicsItemsAcceptingHoverEvents.removeAt(idxGraphObj);
    //        }
    //    }
    //}
    //if (i_pGraphicsItem != nullptr && m_arpGraphicsItemsBroughtToFront.size() > 0) {
    //    invalidateItemInBroughtToFrontList(i_pGraphicsItem);
    //    for (int idxGraphObj = m_arpGraphicsItemsBroughtToFront.size()-1; idxGraphObj >= 0; idxGraphObj--) {
    //        if (m_arpGraphicsItemsBroughtToFront[idxGraphObj] == nullptr) {
    //            m_arpGraphicsItemsBroughtToFront.removeAt(idxGraphObj);
    //        }
    //    }
    //}
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//void CDrawingScene::addChildItems( QGraphicsItem* i_pGraphicsItem )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//
//    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
//    {
//        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);
//        strMthInArgs = QString(pGraphObj == nullptr ? "nullptr" : pGraphObj->path());
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod    */ "addChildItems",
//        /* strAddInfo   */ strMthInArgs );
//
//    CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);
//
//    if( i_pGraphicsItem != nullptr )
//    {
//        QList<QGraphicsItem*> arpGraphicsItemsChilds = i_pGraphicsItem->childItems();
//        QGraphicsItem*        pGraphicsItemChild;
//        CGraphObj*            pGraphObjChild;
//        int                   idxGraphObjChild;
//
//        for( idxGraphObjChild = 0; idxGraphObjChild < arpGraphicsItemsChilds.size(); idxGraphObjChild++ )
//        {
//            pGraphicsItemChild = arpGraphicsItemsChilds[idxGraphObjChild];
//            pGraphObjChild = dynamic_cast<CGraphObj*>(pGraphicsItemChild);
//
//            if( pGraphicsItemChild != nullptr && pGraphObjChild != nullptr )
//            {
//                addItem(pGraphicsItemChild);
//
//                onGraphObjCreationFinished(pGraphObjChild);
//
//                if( pGraphicsItemChild->childItems().size() > 0 )
//                {
//                    addChildItems(pGraphicsItemChild);
//                }
//            }
//        }
//    } // if( i_pGraphicsItem != nullptr )
//
//} // addChildItems

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CDrawingScene::addPhysValShape(CPhysValShape* i_pPhysValShape, const CDrawSettings& i_drawSettings)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pPhysValShape == nullptr ? "null" : i_pPhysValShape->toString());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "addPhysValShape",
        /* strAddInfo   */ strMthInArgs );

    if (i_pPhysValShape == nullptr) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    if (m_arpPhysValShapes.contains(i_pPhysValShape)) {
        throw CException(__FILE__, __LINE__, EResultObjAlreadyInList);
    }
    if (i_pPhysValShape->indexInDrawingScene() != -1) {
        throw CException(__FILE__, __LINE__, EResultObjAlreadyInList);
    }
    int idxShape = m_arpPhysValShapes.size();
    i_pPhysValShape->setIndexInDrawingScene(idxShape);
    m_arpPhysValShapes.append(i_pPhysValShape);
    m_arDrawSettingsPhysValShapes.append(i_drawSettings);
    update();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(idxShape);
    }
    return idxShape;
}

//------------------------------------------------------------------------------
void CDrawingScene::removePhysValShape(CPhysValShape* i_pPhysValShape)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pPhysValShape == nullptr ? "null" : i_pPhysValShape->toString());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "removePhysValShape",
        /* strAddInfo   */ strMthInArgs );

    if (i_pPhysValShape == nullptr) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    if (!m_arpPhysValShapes.contains(i_pPhysValShape)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList);
    }
    int idxShape = i_pPhysValShape->indexInDrawingScene();
    if (idxShape < 0 || idxShape >= m_arpPhysValShapes.size()) {
        throw CException(__FILE__, __LINE__, EResultIdxOutOfRange);
    }
    if (m_arpPhysValShapes[idxShape] != i_pPhysValShape) {
        throw CException(__FILE__, __LINE__, EResultIdxOutOfRange);
    }
    i_pPhysValShape->setIndexInDrawingScene(-1);
    m_arpPhysValShapes.removeAt(idxShape);
    m_arDrawSettingsPhysValShapes.removeAt(idxShape);
    for (; idxShape < m_arpPhysValShapes.size(); ++idxShape) {
        m_arpPhysValShapes[idxShape]->setIndexInDrawingScene(idxShape);
    }
    update();
}

//------------------------------------------------------------------------------
int CDrawingScene::physValShapesCount() const
//------------------------------------------------------------------------------
{
    return m_arpPhysValShapes.size();
}

//------------------------------------------------------------------------------
CPhysValShape* CDrawingScene::getPhysValShape(int i_idxShape) const
//------------------------------------------------------------------------------
{
    if (i_idxShape < 0 || i_idxShape >= m_arpPhysValShapes.size()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    return m_arpPhysValShapes[i_idxShape];
}

//------------------------------------------------------------------------------
void CDrawingScene::removeAndDeleteAllPhysValShapes()
//------------------------------------------------------------------------------
{
    for (int idxShape = 0; idxShape < m_arpPhysValShapes.size(); ++idxShape) {
        delete m_arpPhysValShapes[idxShape];
        m_arpPhysValShapes[idxShape] = nullptr;
    }
    m_arpPhysValShapes.clear();
    m_arDrawSettingsPhysValShapes.clear();
    update();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Setzt den Mode der Scene damit diese weiss, wie mit nachfolgenden Events zu
    verfahren ist.

    Um ein grafisches Objekt ueber Maus Events zu erzeugen, zu verschieben oder zu
    rotieren, seine Form zu ändern, zu selektieren und zu gruppieren, muessen verschiedene
    Modi korrekt gesetzt werden.

    @param i_mode [in] Range [Edit, View und Undefined]
        Undefined ist zu übergeben, wenn der Mode nicht geändert werden soll und das Argument
        somit zu ignorieren ist.
        Nur im Edit Mode können Objekte auf der grafischen Oberfläche durch die Maus angelegt
        und verändert werden.
        Der View Mode dient dazu, die Maus-Events an die Objekte selbst weiterzuleiten,
        die entsprechend darauf reagieren können. Ist z.B. eine ComboBox als Objekt der Empfänger
        der Maus-Events verarbeitet die ComboBox die Events, um z.B. die PullDown-List zu öffnen.
        Bei dem Objekt kann es sich aber auch um ein vollständig benutzerdefiniertes Objekt wie
        einen grafischen Schalter handeln, der über Maus-Klicks seinen On/Off Zustand ändert.
    @param i_editTool [in] Range [None, Select, CreateObjects, Undefined]
        Undefined ist zu übergeben, wenn das EditTool nicht geändert werden soll und das Argument
        somit zu ignorieren ist.
        None zeigt an, dass kein EditTool ausgewählt ist.
        Wird Select übergeben, sollen nachfolgende Maus-Events dazu dienen, Objekte zu selektieren.
        Mit CreateObjects wird angezeigt, dass über nachfolgende Maus-Events Objekte erzeugt werden sollen.
    @param i_editMode [in] Range [None, Creating, Move, Resize, Rotate, MoveShapePoint, EditText, Undefined]
        Undefined ist zu übergeben, wenn der EditMode nicht geändert werden soll und das Argument
        somit zu ignorieren ist.
        None zeigt an, dass kein EditMode ausgewählt ist.
        Die anderen EditModes zeigen an, auf welche Art und Weise das Objekt zu modifizieren ist und hängen
        im wesentlichen davon ab, an welchem Eckpunkt (SelectionPoint) das Objekt mit der Maus gepackt wurde.
    @param i_editResizeMode [in] Range [None, ResizeAll, ResizeHor, ResizeVer, Undefined]
        Undefined ist zu übergeben, wenn der ResizeMode nicht geändert werden soll und das Argument
        somit zu ignorieren ist.
        None zeigt an, dass kein ResizeMode ausgewählt ist.
    @param i_bObjFactoryTypeChanged [in]
        true, falls der grafische Object Type und damit die Objekt Factory zur Erzeugung des Objekt verändert wurde.
        falls otherwise.
*/
void CDrawingScene::setMode(const ZS::System::CEnumMode& i_mode)
    //const CEnumEditTool&         i_editTool,
    //const CEnumEditMode&         i_editMode,
    //const CEnumEditResizeMode&   i_editResizeMode,
    //bool                         i_bObjFactoryTypeChanged )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Mode:" + i_mode.toString();
            //", EditTool:" + i_editTool.toString() +
            //", EditMode:" + i_editMode.toString() +
            //", ResizeMode:" + i_editResizeMode.toString() +
            //", ObjFactoryChanged:" + bool2Str(i_bObjFactoryTypeChanged);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setMode",
        /* strAddInfo   */ strMthInArgs );
    //if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
    //    traceInternalStates(mthTracer, EMethodDir::Enter, "States");
    //}

    bool bModeChanged = false;

    if (i_mode != EMode::Undefined && m_mode != i_mode) {
        bModeChanged = true;
        m_mode = i_mode;
        if (m_mode == EMode::View) {
            //if (i_editTool == EEditTool::Undefined) {
            //    m_editTool = EEditTool::None;
            //}
            //if (i_editMode == EEditMode::Undefined) {
            //    m_editMode = EEditMode::None;
            //}
            //if (i_editResizeMode == EEditResizeMode::Undefined) {
            //    m_editResizeMode = EEditResizeMode::None;
            //}
        }
        clearSelection();
    }

    //if (i_editTool != EEditTool::Undefined && m_editTool != i_editTool) {
    //    bModeChanged = true;
    //    m_editTool = i_editTool;
    //    clearSelection();
    //}

    //QString strObjFactory;

    //if (m_pObjFactory != nullptr) {
    //    strObjFactory = m_pObjFactory->getGraphObjTypeAsString();
    //}
    //else {
    //    //m_pGraphObjUnderConstruction = nullptr;
    //    QObject::disconnect(
    //        m_pGraphObjUnderConstruction, &CGraphObj::editModeChanged,
    //        this, &CDrawingScene::onGraphObjEditModeChanged );
    //}

    //if (m_editTool != EEditTool::CreateObjects) {
    //    //m_pGraphObjUnderConstruction = nullptr;
    //    QObject::disconnect(
    //        m_pGraphObjUnderConstruction, &CGraphObj::editModeChanged,
    //        this, &CDrawingScene::onGraphObjEditModeChanged );
    //}

    //if (i_editMode != EEditMode::Undefined && m_editMode != i_editMode) {
    //    bModeChanged = true;
    //    m_editMode = i_editMode;
    //}

    //if (i_editResizeMode != EEditResizeMode::Undefined && m_editResizeMode != i_editResizeMode) {
    //    bModeChanged = true;
    //    m_editResizeMode = i_editResizeMode;
    //}

    if (bModeChanged /*|| i_bObjFactoryTypeChanged*/) {
        update();
        emit_modeChanged(m_mode);
    }

    //if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
    //    traceInternalStates(mthTracer, EMethodDir::Leave, "States");
    //}
} // setMode

//------------------------------------------------------------------------------
void CDrawingScene::setMouseGrabber(CGraphObj* i_pGraphObj)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pGraphObj == nullptr ? "nullptr" : i_pGraphObj->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setMouseGrabber",
        /* strAddInfo   */ strMthInArgs );

    if (m_pGraphObjMouseGrabber != nullptr) {
        SErrResultInfo errResultInfo(
            EResultInvalidMethodCall, EResultSeverityError, "Mouse Grabber already set");
        CErrLog::GetInstance()->addEntry(errResultInfo);
    }
    m_pGraphObjMouseGrabber = i_pGraphObj;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Activates the given drawing tool by activating the corresponding object factory.

    The method may only be called if the drawing scene is in Edit mode.

    If a new object factory is activated the scenes mode will be adjusted
    so that a graphical object will be created by the following mouse click event.

    ////The method may also change whether the graphics items accept hover events.
    ////As long as a drawing tool is selected, graphics items don't accpet hover events
    ////and cannot be selected.
    ////The drawing tool connection line is an exception to this rule as when creating
    ////connection lines it must be checked whether connection points are hit.

    @param i_pObjFactory [in] Pointer to Object Factory instance to be used as the
                              drawing tool's object factory.
                              nullptr may be passed to deactivate any currently
                              selected object factory.

    @note Throws a critical exception
          - with Result = InvalidMethodCall if the scene is not in Edit mode.
*/
void CDrawingScene::setCurrentDrawingTool( CObjFactory* i_pObjFactory )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pObjFactory == nullptr ? "nullptr" : i_pObjFactory->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCurrentDrawingTool",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Enter, "States");
    }

    if (m_mode != EMode::Edit) {
        throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "m_mode != EMode::Edit");
    }
    if (m_pGraphObjUnderConstruction != nullptr) {
        throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "m_pGraphObjUnderConstruction != nullptr");
    }

    if (m_pObjFactory != i_pObjFactory) {
        bool bDeselectItems = false;
        QString strFactoryGrpName;
        QString strGraphObjType;
        //EGraphObjType graphObjType = EGraphObjTypeUndefined;
        if (m_pObjFactory != nullptr) {
            QObject::disconnect(
                m_pObjFactory, &CObjFactory::destroyed,
                this, &CDrawingScene::onGraphObjFactoryDestroyed );
            // If no drawing tool is selected anymore ...
            if (i_pObjFactory == nullptr) {
                bDeselectItems = true;
            }
        }
        //else {
        //    // If a drawing tool has been selected ...
        //    if (i_pObjFactory != nullptr) {
        //        bAdjustItemsStates = true;
        //    }
        //}
        m_pObjFactory = i_pObjFactory;
        if (m_pObjFactory != nullptr) {
            QObject::connect(
                m_pObjFactory, &CObjFactory::destroyed,
                this, &CDrawingScene::onGraphObjFactoryDestroyed );
            strFactoryGrpName = m_pObjFactory->groupName();
            strGraphObjType = m_pObjFactory->graphObjTypeAsString();
            //graphObjType = m_pObjFactory->graphObjType();
        }
        if (bDeselectItems) {
            // The items call returns a list of all items on the scene ordered by the stacking order.
            // This list may also include selection points. When deselecting an object, the selection
            // points are destroyed and the entries in the items list are pointing to those deleted
            // selection points. To avoid a crash, selection points will not be added to the items
            // which got to be deselected.
            QList<QGraphicsItem*> arpGraphicsItemsToDeselect;
            QList<QGraphicsItem*> arpGraphicsItemsTmp = items();
            for (QGraphicsItem* pGraphicsItem : arpGraphicsItemsTmp) {
                CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);
                if (pGraphObj != nullptr && !pGraphObj->isSelectionPoint()) {
                    if (pGraphicsItem->isSelected()) {
                        arpGraphicsItemsToDeselect.append(pGraphicsItem);
                    }
                }
            }
            for (QGraphicsItem* pGraphicsItem : arpGraphicsItemsToDeselect) {
                CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);
                if (pGraphObj != nullptr) {
                    //pGraphicsItem->setAcceptHoverEvents(graphObjType == EGraphObjTypeUndefined || graphObjType == EGraphObjTypeConnectionLine);
                    pGraphicsItem->setSelected(false);
                }
            }
        }
        emit_drawingToolChanged(strFactoryGrpName, strGraphObjType);
    }
    //if (iGraphObjTypePrev != iGraphObjTypeCurr) {
    //    if (iGraphObjTypeCurr == static_cast<int>(EGraphObjTypeUndefined)) {
    //        CEnumEditTool editTool = m_editTool;
    //        if (m_editTool != EEditTool::Select) {
    //            editTool = EEditTool::Select;
    //        }
    //        setMode(EMode::Undefined, editTool, EEditMode::None, EEditResizeMode::None, true);
    //    }
    //    else {
    //        setMode(EMode::Undefined, EEditTool::CreateObjects, EEditMode::None, EEditResizeMode::None, true);
    //    }
    //}

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Leave, "States");
    }
}

//------------------------------------------------------------------------------
/*! Activates the given drawing tool by activating the corresponding object factory.

    The method may only be called if the drawing scene is in Edit mode.

    The object factory with the given group name and graph object type must have
    been created before.

    @param i_strFactoryGrpName [in] Group name of the drawing tool's object factory.
    @param i_strGraphObjType [in] Name of the drawing tool's object class.
                                  An empty string may be passed to deactivate any
                                  currently selected object factory.

    @note Throws a critical exception
          - with Result = InvalidMethodCall if the scene is not in Edit mode.
          - with Result = ObjNotRegistered if no object factory with the given group name
                          and object type is not registered.
*/
void CDrawingScene::setCurrentDrawingTool(
    const QString& i_strFactoryGrpName,
    const QString& i_strGraphObjType )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strFactoryGrpName + "::" + i_strGraphObjType;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCurrentDrawingTool",
        /* strAddInfo   */ strMthInArgs );

    if (m_mode != EMode::Edit) {
        throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "m_mode != EMode::Edit");
    }
    CObjFactory* pObjFactory = CObjFactory::FindObjFactory(i_strFactoryGrpName, i_strGraphObjType);
    if (pObjFactory == nullptr) {
        throw CException(__FILE__, __LINE__, EResultObjNotRegistered, "pObjFactory == nullptr");
    }
    setCurrentDrawingTool(pObjFactory);
}

//------------------------------------------------------------------------------
/*! Returns the type of the currently selected drawing tool as an integer value.

    @return Type as integer value of the currently selected drawing tool.
*/
EGraphObjType CDrawingScene::getCurrentDrawingToolGraphObjType() const
//------------------------------------------------------------------------------
{
    EGraphObjType graphObjFactoryType = EGraphObjTypeUndefined;
    if (m_pObjFactory != nullptr) {
        graphObjFactoryType = m_pObjFactory->graphObjType();
    }
    return graphObjFactoryType;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Checks whether a connection point is at the given position.

    @param i_ptScenePos [in] Scene coordinates to be checked.

    @return Pointer to connection point item if there is a connection point at
            the given position. nullptr otherwise.
*/
CGraphObjConnectionPoint* CDrawingScene::getConnectionPoint( const QPointF& i_ptScenePos )
//------------------------------------------------------------------------------
{
    CGraphObjConnectionPoint* pGraphObjCnctPt = nullptr;
    QList<QGraphicsItem*> arpGraphicsItems = items(i_ptScenePos);
    for (QGraphicsItem* pGraphicsItem : arpGraphicsItems) {
        if (pGraphicsItem->type() == static_cast<int>(EGraphObjTypeConnectionPoint)) {
            pGraphObjCnctPt = dynamic_cast<CGraphObjConnectionPoint*>(pGraphicsItem);
            break;
        }
    }
    return pGraphObjCnctPt;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the proposed mouse cursor shape.

    The mouse cursor shape depends on the graphic item at the given position
    and the currently selected edit tool and edit mode.

    @param i_ptScenePos [in]
        Scene coordinates to be checked.

    @return Proposed cursor shape.
*/
QCursor CDrawingScene::getProposedCursor( const QPointF& i_ptScenePos ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCursor, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Pos:" + qPoint2Str(i_ptScenePos);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCursor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "getProposedCursor",
        /* strAddInfo   */ strMthInArgs );

    QCursor cursor = Qt::ArrowCursor;

    if (m_mode == EMode::Edit) {
        if (sceneRect().contains(i_ptScenePos)) {
            if (m_pObjFactory != nullptr) {
                cursor = Qt::CrossCursor;
            }
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(qCursorShape2Str(cursor.shape()));
    }
    return cursor;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
double CDrawingScene::bringToFront( QGraphicsItem* i_pGraphicsItem, const QPointF& i_ptScenePos )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);
        strMthInArgs = QString(pGraphObj == nullptr ? "nullptr" : pGraphObj->path()) +
                       ", ScenePos {" + qPoint2Str(i_ptScenePos) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "bringToFront",
        /* strAddInfo   */ strMthInArgs );

    QList<QGraphicsItem*> arpGraphicsItems = items(i_ptScenePos);
    return bringToFront(i_pGraphicsItem, arpGraphicsItems);
}

//------------------------------------------------------------------------------
double CDrawingScene::sendToBack( QGraphicsItem* i_pGraphicsItem, const QPointF& i_ptScenePos )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);
        strMthInArgs = QString(pGraphObj == nullptr ? "nullptr" : pGraphObj->path()) +
                       ", ScenePos {" + qPoint2Str(i_ptScenePos) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "sendToBack",
        /* strAddInfo   */ strMthInArgs );

    QList<QGraphicsItem*> arpGraphicsItems = items(i_ptScenePos);
    return sendToBack(i_pGraphicsItem, arpGraphicsItems);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Brings the given graphic item into front of the given list of graphic items.
    If an empty list is passed the given graphic item will be pushed into front
    of all graphic items currently available in the scene.

    The order of the graphic items is defined by the zValue. After calling this method
    the zValue of the given graphic is 0.1. higher than the highest zValue of the
    given list of graphic items.

    @param i_pGraphicsItem [in]
        Pointer to item which should be pushed to front
    @param i_arpGraphicsItems [in]
        List of graphic items the item should be pushed into front.
        If the list is empty the item will be pushed into front of
        all items currently in the scene.

    @return Resulting zValue assigned to the given graphic item.
*/
double CDrawingScene::bringToFront( QGraphicsItem* i_pGraphicsItem, const QList<QGraphicsItem*>& i_arpGraphicsItems )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);
        strMthInArgs = "Item: " + QString(pGraphObj == nullptr ? "nullptr" : pGraphObj->keyInTree()) +
                       ", Items[" + QString::number(i_arpGraphicsItems.size()) + "]";
        if (i_arpGraphicsItems.size() > 0) {
            strMthInArgs += "(";
            for (const QGraphicsItem* pGraphicsItem : i_arpGraphicsItems) {
                const CGraphObj* pGraphObj = dynamic_cast<const CGraphObj*>(pGraphicsItem);
                if (pGraphObj != nullptr) {
                    if (!strMthInArgs.endsWith("(")) strMthInArgs += ", ";
                    strMthInArgs += pGraphObj->typeAsString() + ": " + pGraphObj->path();
                }
            }
            strMthInArgs += ")";
        }
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "bringToFront",
        /* strAddInfo   */ strMthInArgs );

    QList<QGraphicsItem*> arpGraphicsItems;
    if (i_arpGraphicsItems.size() == 0) {
        arpGraphicsItems = items();
    }
    else {
        arpGraphicsItems = i_arpGraphicsItems;
    }
    double fZValue = 0.0;
    for (QGraphicsItem* pGraphicsItem : arpGraphicsItems) {
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);
        if (pGraphObj != nullptr && !pGraphObj->isSelectionPoint() && !pGraphObj->isLabel()) {
            if (pGraphicsItem->zValue() >= fZValue) {
                fZValue = pGraphicsItem->zValue() + 0.1;
            }
        }
    }
    CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);
    if (pGraphObj != nullptr) {
        // The zValue of Selection points and Labels are updated if the
        // zValue of their parent object is changed.
        if (pGraphObj != nullptr && !pGraphObj->isSelectionPoint() && !pGraphObj->isLabel()) {
            pGraphObj->setStackingOrderValue(fZValue);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(QString::number(fZValue));
    }
    return fZValue;
}

//------------------------------------------------------------------------------
/*! Sends the given graphic item into back of the given list of graphic items.
    If an empty list is passed the given graphic item will be pushed to the bottom
    of all graphic items currently available in the scene.

    The order of the graphic items is defined by the zValue. After calling this method
    the zValue of the given graphic is 0.1. lower than the lowest zValue of the
    given list of graphic items.

    @param i_pGraphicsItem [in]
        Pointer to item which should be pushed to back.
    @param i_arpGraphicsItems [in]
        List of graphic items the item should be pushed to back.
        If the list is empty the item will be pushed behind
        all items currently in the scene.

    @return Resulting zValue assigned to the given graphic item.
*/
double CDrawingScene::sendToBack( QGraphicsItem* i_pGraphicsItem, const QList<QGraphicsItem*>& i_arpGraphicsItems )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);
        strMthInArgs = "Item: " + QString(pGraphObj == nullptr ? "nullptr" : pGraphObj->keyInTree()) +
                       ", Items[" + QString::number(i_arpGraphicsItems.size()) + "]";
        if (i_arpGraphicsItems.size() > 0) {
            strMthInArgs += "(";
            for (const QGraphicsItem* pGraphicsItem : i_arpGraphicsItems) {
                const CGraphObj* pGraphObj = dynamic_cast<const CGraphObj*>(pGraphicsItem);
                if (pGraphObj != nullptr) {
                    if (!strMthInArgs.endsWith("(")) strMthInArgs += ", ";
                    strMthInArgs += pGraphObj->typeAsString() + ": " + pGraphObj->path();
                }
            }
            strMthInArgs += ")";
        }
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "sendToBack",
        /* strAddInfo   */ strMthInArgs );

    QList<QGraphicsItem*> arpGraphicsItems;
    if (i_arpGraphicsItems.size() == 0) {
        arpGraphicsItems = items();
    }
    else {
        arpGraphicsItems = i_arpGraphicsItems;
    }
    double fZValue = 0.0;
    for (QGraphicsItem* pGraphicsItem : arpGraphicsItems) {
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);
        if (!pGraphObj->isSelectionPoint() && !pGraphObj->isLabel()) {
            if (pGraphicsItem->zValue() <= fZValue) {
                fZValue = pGraphicsItem->zValue() - 0.1;
            }
        }
    }
    CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);
    if (pGraphObj != nullptr) {
        // The zValue of Selection points and Labels are updated if the
        // zValue of their parent object is changed.
        if (!pGraphObj->isSelectionPoint() && !pGraphObj->isLabel()) {
            pGraphObj->setStackingOrderValue(fZValue);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(QString::number(fZValue));
    }
    return fZValue;
}

/*==============================================================================
public: // to be called by graphical objects (as graphical objects are not derived from QObject and cannot emit signals)
==============================================================================*/

//------------------------------------------------------------------------------
/*! This method has to be called by graphical objects (polygons and connection lines)
    if new shape points will be inserted at the beginning or appended at the end
    of the polygons so that the drawing scene will further on dispatch mouse events
    "directly" to the object "under construction". On extended polygons the object
    is not hit and the graphics scene does not dispatch the mouse event to the objects
    or their selection points (similar to newly creating objects).

    @param i_pGraphObj [in] Pointer to graphic item.
*/
//void CDrawingScene::onGraphObjAddingShapePointsStarted( CGraphObj* i_pGraphObj )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = QString(i_pGraphObj == nullptr ? "nullptr" : i_pGraphObj->keyInTree());
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod    */ "onGraphObjAddingShapePointsStarted",
//        /* strAddInfo   */ strMthInArgs );
//
//    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
//    //    traceInternalStates(mthTracer, EMethodDir::Enter);
//    //}
//
//    if( i_pGraphObj == nullptr )
//    {
//        throw CException( __FILE__, __LINE__, EResultArgOutOfRange, "i_pGraphObj == nullptr" );
//    }
//
//    //if( m_pGraphObjAddingShapePoints != nullptr )
//    //{
//    //    throw CException( __FILE__, __LINE__, EResultInternalProgramError, "pGraphObjAddingShapePoints != nullptr" );
//    //}
//
//    //m_pGraphObjAddingShapePoints = i_pGraphObj;
//    //m_pGraphicsItemAddingShapePoints = dynamic_cast<QGraphicsItem*>(i_pGraphObj);
//
//    //if( m_pGraphicsItemAddingShapePoints == nullptr )
//    //{
//    //    throw CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphicsItemAddingShapePoints == nullptr" );
//    //}
//
//    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
//    //    traceInternalStates(mthTracer, EMethodDir::Leave);
//    //}
//} // onGraphObjAddingShapePointsStarted

//------------------------------------------------------------------------------
/*! This method has to be called by graphical objects (polygons and connection lines)
    if new shape points will be inserted at the beginning or appended at the end
    of the polygons so that the drawing scene will further on dispatch mouse events
    "directly" to the object "under construction". On extended polygons the object
    is not hit and the graphics scene does not dispatch the mouse event to the objects
    or their selection points (similar to newly creating objects).

    @param i_pGraphObj [in] Pointer to graphic item.
*/
//void CDrawingScene::onGraphObjAddingShapePointsFinished( CGraphObj* i_pGraphObj )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = QString(i_pGraphObj == nullptr ? "nullptr" : i_pGraphObj->keyInTree());
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod    */ "onGraphObjAddingShapePointsFinished",
//        /* strAddInfo   */ strMthInArgs );
//
//    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
//    //    traceInternalStates(mthTracer, EMethodDir::Enter);
//    //}
//
//    if( i_pGraphObj == nullptr )
//    {
//        throw CException( __FILE__, __LINE__, EResultArgOutOfRange, "i_pGraphObj == nullptr" );
//    }
//
//    if( m_pGraphObjAddingShapePoints != nullptr && m_pGraphObjAddingShapePoints != i_pGraphObj )
//    {
//        throw CException( __FILE__, __LINE__, EResultInternalProgramError, "pGraphObjAddingShapePoints != i_pGraphObj" );
//    }
//
//    m_pGraphObjAddingShapePoints = nullptr;
//    m_pGraphicsItemAddingShapePoints = nullptr;
//
//    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
//    //    traceInternalStates(mthTracer, EMethodDir::Leave);
//    //}
//} // onGraphObjAddingShapePointsFinished

/*==============================================================================
public: // to be called by graphical objects (as graphical objects are not derived from QObject and cannot emit signals)
==============================================================================*/

//------------------------------------------------------------------------------
/*! This method has to be called by graphical objects AFTER they have been
    initially created (after they have been completely created e.g. after
    a rectangle has been resized and the mouse button has been released).
    After creating the object via mouse events the edit mode of the drawing
    has to be reset.

    @param i_pGraphObj [in] Pointer to graphic item.
*/
//void CDrawingScene::onGraphObjCreationFinished( CGraphObj* i_pGraphObj )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = QString(i_pGraphObj == nullptr ? "nullptr" : i_pGraphObj->keyInTree());
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod    */ "onGraphObjCreationFinished",
//        /* strAddInfo   */ strMthInArgs );
//
//    if(i_pGraphObj == nullptr) {
//        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pGraphObj == nullptr");
//    }
//
//    // Reset drawings scenes edit modes:
//    //----------------------------------
//
//    //if (m_mode == EMode::Edit) {
//    //    if (m_editTool == EEditTool::CreateObjects) {
//    //        if (m_pGraphObjUnderConstruction == i_pGraphObj) {
//    //            m_pGraphObjUnderConstruction->setSelected(false);
//    //            m_pGraphObjUnderConstruction->setAcceptHoverEvents(false);
//    //            m_pGraphObjUnderConstruction->setAcceptedMouseButtons(Qt::NoButton);
//    //            m_pGraphObjUnderConstruction = nullptr;
//    //            QObject::disconnect(
//    //                m_pGraphObjUnderConstruction, &CGraphObj::editModeChanged,
//    //                this, &CDrawingScene::onGraphObjEditModeChanged );
//    //            m_pGraphicsItemAddingShapePoints = nullptr;
//    //            m_pGraphObjAddingShapePoints = nullptr;
//    //            setMode(EMode::Undefined, EEditTool::Undefined, EEditMode::None, EEditResizeMode::None, false);
//    //        }
//    //    }
//    //}
//    //emit_graphObjCreated(i_pGraphObj);
//}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CDrawingScene::findUniqueGraphObjName( CGraphObj* i_pGraphObj )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pGraphObj == nullptr ? "null" : i_pGraphObj->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "findUniqueGraphObjName",
        /* strAddInfo   */ strMthInArgs );

    if (i_pGraphObj == nullptr) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "pGraphObj == nullptr");
    }

    QString strObjName = i_pGraphObj->name();
    int iObjNr = 1;
    QString strObjNr;

    // Remove trailing number (if any).
    if (strObjName.length() > 0 && isDigitChar(strObjName[strObjName.length()-1])) {
        while (isDigitChar(strObjName[strObjName.length()-1])) {
            strObjNr.insert(0, strObjName[strObjName.length()-1]);
            strObjName.remove(strObjName.length()-1, 1);
        }
        iObjNr = strObjNr.toInt();
    }
    else {
        strObjNr = QString::number(iObjNr);
    }

    QString strObjNameParent;
    if (i_pGraphObj->parentGroup() != nullptr) {
        strObjNameParent = i_pGraphObj->parentGroup()->name();
    }

    QString strObjBaseName = strObjName;
    if (strObjNameParent.isEmpty()) {
        strObjName = strObjBaseName + strObjNr;
    }
    else {
        strObjName = strObjNameParent + s_strGraphObjNameSeparator + strObjBaseName + strObjNr;
    }

    QList<QGraphicsItem*> arpGraphicsItems = items();
    bool bUniqueName = false;
    while (!bUniqueName) {
        bUniqueName = true;
        for (QGraphicsItem* pGraphicsItem : arpGraphicsItems) {
            CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);
            if (pGraphObj != nullptr && pGraphObj != i_pGraphObj) {
                if (strObjName.compare(pGraphObj->name(), Qt::CaseInsensitive) == 0) {
                    bUniqueName = false;
                    break;
                }
            }
        }
        if (!bUniqueName) {
            iObjNr++;
            strObjNr = QString::number(iObjNr);
            if (strObjNameParent.isEmpty()) {
                strObjName = strObjBaseName + strObjNr;
            }
            else {
                strObjName = strObjNameParent + s_strGraphObjNameSeparator + strObjBaseName + strObjNr;
            }
        }
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(strObjName);
    }
    return strObjName;

} // findUniqueGraphObjName

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Combines the currently selected objects into a group and returns the newly created group.

    A graphical group item is created and the selected objects will be added
    as childs of this group.

    The coordinates of the child objects are adjusted so that they are relative
    to their new parent group.
*/
CGraphObjGroup* CDrawingScene::groupGraphObjsSelected()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "groupGraphObjsSelected",
        /* strAddInfo   */ "" );

    CGraphObjGroup* pGraphObjGroup = nullptr;

    QList<QGraphicsItem*> arpGraphicsItemsSelected = selectedItems();

    if (arpGraphicsItemsSelected.size() > 1) {
        // First unselect all child items which will be added to the group.
        unselectGraphicsItems(arpGraphicsItemsSelected);

        // Selection points, labels and connection lines will not become part of the newly
        // created group and will be removed from the list of selected items.
        // In addition items which already belong as childs to a group got to be removed
        // from the list of selected items as those childs will be added to the new group
        // as childs of the item.
        QList<QGraphicsItem*> arpGraphicsItemsToBeRemoved;
        for (QGraphicsItem* pGraphicsItemSelected : arpGraphicsItemsSelected) {
            CGraphObj* pGraphObjSelected = dynamic_cast<CGraphObj*>(pGraphicsItemSelected);
            if (pGraphObjSelected->isConnectionLine() || pGraphObjSelected->isSelectionPoint() || pGraphObjSelected->isLabel()) {
                arpGraphicsItemsToBeRemoved.append(pGraphicsItemSelected);
            }
            else if (pGraphicsItemSelected->parentItem() != nullptr) {
                CGraphObjGroup* pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(pGraphicsItemSelected->parentItem());
                if (pGraphObjGroup != nullptr) {
                    arpGraphicsItemsToBeRemoved.append(pGraphicsItemSelected);
                }
            }
        }
        for (QGraphicsItem* pGraphicsItemSelected : arpGraphicsItemsToBeRemoved) {
            arpGraphicsItemsSelected.removeOne(pGraphicsItemSelected);
        }
        pGraphObjGroup = groupGraphObjs(arpGraphicsItemsSelected);
        pGraphObjGroup->setSelected(true);
    }

    if (m_pGraphicsItemSelectionArea != nullptr) {
        QGraphicsScene::removeItem(m_pGraphicsItemSelectionArea);
        delete m_pGraphicsItemSelectionArea;
        m_pGraphicsItemSelectionArea = nullptr;
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(QString(pGraphObjGroup == nullptr ? "null" : pGraphObjGroup->path()));
    }
    return pGraphObjGroup;
}

//------------------------------------------------------------------------------
/*! Dissolves all currently selected group objects and destroys the group objects.

    The child objects are retained and are assigned to the drawing scene as
    immediate child objects. The coordinates of the child objects are adjusted so
    that they become relative to the drawing scene.
*/
int CDrawingScene::ungroupGraphObjsSelected()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ungroupGraphObjsSelected",
        /* strAddInfo   */ "" );
    //if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
    //    traceInternalStates(mthTracer, EMethodDir::Enter);
    //}

    int iObjsUngroupedCount = 0;

    QList<QGraphicsItem*> arpGraphicsItemsSelected = selectedItems();

    for (QGraphicsItem* pGraphicsItemSelected : arpGraphicsItemsSelected) {
        CGraphObj* pGraphObjSelected = dynamic_cast<CGraphObj*>(pGraphicsItemSelected);
        if (pGraphObjSelected == nullptr) {
            throw CException(__FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjSelected == nullptr");
        }
        if (pGraphObjSelected->isGroup()) {
            CGraphObjGroup* pGraphObjGroupSelected = dynamic_cast<CGraphObjGroup*>(pGraphicsItemSelected);
            if (pGraphObjGroupSelected == nullptr) {
                throw CException(__FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjGroup == nullptr");
            }
            QGraphicsItemGroup* pGraphicsItemGroupSelected = dynamic_cast<QGraphicsItemGroup*>(pGraphObjGroupSelected);
            if (pGraphicsItemGroupSelected == nullptr) {
                throw CException(__FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphicsItemGroup == nullptr");
            }
            for (QGraphicsItem* pGraphicsItemChild : pGraphicsItemGroupSelected->childItems()) {
                CGraphObj* pGraphObjChild = dynamic_cast<CGraphObj*>(pGraphicsItemChild);
                if (pGraphObjChild == nullptr) {
                    throw CException(__FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjChild == nullptr");
                }
                pGraphObjGroupSelected->removeFromGroup(pGraphObjChild);
            }
            delete pGraphObjGroupSelected;
            pGraphObjGroupSelected = nullptr;
            iObjsUngroupedCount++;
        }
    }
    //if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
    //    traceInternalStates(mthTracer, EMethodDir::Leave);
    //}
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(iObjsUngroupedCount);
    }
    return iObjsUngroupedCount;
}

//------------------------------------------------------------------------------
/*! Combines the passed list of graphical objects into a group and returns the newly created group.

    A graphical group item is created and the objects will be added as childs of this group.

    The coordinates of the child objects are adjusted so that they are relative
    to their new parent group.
*/
CGraphObjGroup* CDrawingScene::groupGraphObjs(QList<QGraphicsItem*> i_arpGraphicsItems)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Items [" + QString::number(i_arpGraphicsItems.size()) + "]";
        if (i_arpGraphicsItems.size() > 0) {
            strMthInArgs += "(";
            for (const QGraphicsItem* pGraphicsItem : i_arpGraphicsItems) {
                if (!strMthInArgs.endsWith("(")) strMthInArgs += ", ";
                const CGraphObj* pGraphObj = dynamic_cast<const CGraphObj*>(pGraphicsItem);
                strMthInArgs += QString(pGraphObj == nullptr ? "null" : pGraphObj->path());
            }
            strMthInArgs += ")";
        }
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "groupGraphObjs",
        /* strAddInfo   */ strMthInArgs );
    //if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
    //    traceInternalStates(mthTracer, EMethodDir::Enter);
    //}

    CGraphObjGroup* pGraphObjGroup = nullptr;

    CObjFactory* pObjFactoryTmp = CObjFactory::FindObjFactory(
        CObjFactory::c_strGroupNameStandardShapes, graphObjType2Str(EGraphObjTypeGroup));
    CObjFactoryGroup* pObjFactoryGroup = dynamic_cast<CObjFactoryGroup*>(pObjFactoryTmp);
    if (pObjFactoryGroup != nullptr) {
        // Calculate resulting bounding rectangle of group (without selection rectangle and selection points).
        QRectF rctGroupSceneCoors = getBoundingRect(i_arpGraphicsItems);

        pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(pObjFactoryGroup->createGraphObj(this, CDrawSettings(EGraphObjTypeGroup)));
        if (pGraphObjGroup == nullptr) {
            throw CException(__FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjGroup == nullptr");
        }
        QGraphicsItemGroup* pGraphicsItemGroup = dynamic_cast<QGraphicsItemGroup*>(pGraphObjGroup);
        if (pGraphicsItemGroup == nullptr) {
            throw CException(__FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphicsItemGroup == nullptr");
        }

        // Add new (empty) group to graphics scene.
        addGraphObj(pGraphObjGroup);

        // A newly created object will be positioned relative to the top left corner of the
        // drawing scene. On adding an item as a child to a parent item (group) the child
        // item are positioned relative to the center point of the parent's bounding rectangle.

        // ConnectionLines are treated special. If one of the line's connection points don't
        // belong to the group, the connection line also does not belong to the group.
        // If both connection points of a connection line are selected and will be grouped
        // the connection line may not have been selected.
        // Should connection lines belong to groups at all?
        // No, not directly. They are linked to connection points. If a connection point
        // is part of a group also the connection line is part of the group. And if the
        // line's connection points belong to different groups the connection line
        // indirectly belongs to two different groups.

        //SGraphObjAlignment alignmentWidth(
        //    /* refChild  */ EAlignmentRef::Width,
        //    /* refParent */ EAlignmentRef::Width,
        //    /* bAbsolute */ false );
        //SGraphObjAlignment alignmentHeight(
        //    /* refChild  */ EAlignmentRef::Height,
        //    /* refParent */ EAlignmentRef::Height,
        //    /* bAbsolute */ false );
        //SGraphObjAlignment alignmentLeft(
        //    /* refChild  */ EAlignmentRef::Left,
        //    /* refParent */ EAlignmentRef::Left,
        //    /* bAbsolute */ false );
        //SGraphObjAlignment alignmentTop(
        //    /* refChild  */ EAlignmentRef::Top,
        //    /* refParent */ EAlignmentRef::Top,
        //    /* bAbsolute */ false );

        //CPhysValRect physValRect(rctGroupSceneCoors, m_drawingSize.imageCoorsResolutionInPx(), Units.Length.px);
        //physValRect = convert(physValRect);
        //pGraphObjGroup->setRect(physValRect);

        // Add child items to group.
        for (QGraphicsItem* pGraphicsItem : i_arpGraphicsItems) {
            CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);
            if (pGraphObj != nullptr) {
                pGraphObjGroup->addToGroup(pGraphObj);

                //alignmentLeft.m_fVal = 0.0;
                //alignmentTop.m_fVal = 0.0;
                //alignmentWidth.m_fVal = 0.0;
                //alignmentHeight.m_fVal = 0.0;

                //if (rctGroupSceneCoors.width() != 0.0) {
                //    alignmentLeft.m_fVal = posItem.x() / rctGroupSceneCoors.width();
                //}
                //if (rctGroupSceneCoors.height() != 0.0) {
                //    alignmentTop.m_fVal = posItem.y() / rctGroupSceneCoors.height();
                //}
                //if (rctGroupSceneCoors.width() != 0.0) {
                //    alignmentWidth.m_fVal = sizItem.width() / rctGroupSceneCoors.width();
                //}
                //if (rctGroupSceneCoors.height() != 0.0) {
                //    alignmentHeight.m_fVal = sizItem.height() / rctGroupSceneCoors.height();
                //}

                //// The alignments will be adjusted in the order they are added. The order
                //// takes effect on the result. Usually the size should be adjusted before
                //// the positions to get relative adjustments working as expected.
                //pGraphObj->addAlignment(alignmentWidth);
                //pGraphObj->addAlignment(alignmentHeight);
                //pGraphObj->addAlignment(alignmentLeft);
                //pGraphObj->addAlignment(alignmentTop);
            }
        }
    }
    //if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
    //    traceInternalStates(mthTracer, EMethodDir::Leave);
    //}
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(QString(pGraphObjGroup == nullptr ? "null" : pGraphObjGroup->path()));
    }
    return pGraphObjGroup;
}

//------------------------------------------------------------------------------
/*! Combines the passed list of graphical objects into a group and returns the newly created group.

    A graphical group item is created and the objects will be added as childs of this group.

    The coordinates of the child objects are adjusted so that they are relative
    to their new parent group.
*/
CGraphObjGroup* CDrawingScene::groupGraphObjs(QList<CGraphObj*> i_arpGraphObjs)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Items [" + QString::number(i_arpGraphObjs.size()) + "]";
        if (i_arpGraphObjs.size() > 0) {
            strMthInArgs += "(";
            for (const CGraphObj* pGraphObj : i_arpGraphObjs) {
                if (!strMthInArgs.endsWith("(")) strMthInArgs += ", ";
                strMthInArgs += QString(pGraphObj == nullptr ? "null" : pGraphObj->path());
            }
            strMthInArgs += ")";
        }
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "groupGraphObjs",
        /* strAddInfo   */ strMthInArgs );
    //if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
    //    traceInternalStates(mthTracer, EMethodDir::Enter);
    //}

    CGraphObjGroup* pGraphObjGroup = nullptr;

    CObjFactory* pObjFactoryTmp = CObjFactory::FindObjFactory(
        CObjFactory::c_strGroupNameStandardShapes, graphObjType2Str(EGraphObjTypeGroup));
    CObjFactoryGroup* pObjFactoryGroup = dynamic_cast<CObjFactoryGroup*>(pObjFactoryTmp);
    if (pObjFactoryGroup != nullptr) {
        QList<QGraphicsItem*> arpGraphicsItems;
        for (CGraphObj* pGraphObj : i_arpGraphObjs) {
            QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(pGraphObj);
            if (pGraphicsItem != nullptr) {
                arpGraphicsItems.append(pGraphicsItem);
            }
        }
        pGraphObjGroup = groupGraphObjs(arpGraphicsItems);
    }

    //if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
    //    traceInternalStates(mthTracer, EMethodDir::Leave);
    //}
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(QString(pGraphObjGroup == nullptr ? "null" : pGraphObjGroup->path()));
    }
    return pGraphObjGroup;
}

//------------------------------------------------------------------------------
/*! Removes all items belonging to the passed group from this group and destroys the group object.

    The item's will be reparented to the group's parent item, or to no parent
    if this group has no parent. Its position and transformation relative to the
    scene will stay intact.
*/
void CDrawingScene::ungroup(CGraphObjGroup* i_pGraphObjGroup)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pGraphObjGroup == nullptr ? "null" : i_pGraphObjGroup->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ungroup",
        /* strAddInfo   */ strMthInArgs);

    if (i_pGraphObjGroup != nullptr) {
        for (CGraphObj* pGraphObjChild : i_pGraphObjGroup->childs()) {
            i_pGraphObjGroup->removeFromGroup(pGraphObjChild);
        }
        delete i_pGraphObjGroup;
        i_pGraphObjGroup = nullptr;
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::setDrawSettings( const CDrawSettings& i_settings )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_settings.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setDrawSettings",
        /* strAddInfo   */ strMthInArgs );

    // When emitting the drawSettingsChanged signal new settings may be applied
    // as reentries by the signal receivers. Those new settings may have to be
    // temporarily stored and a new temporary buffer has to be allocated.
    // The current buffer pointer will be locally saved and freed after emitting
    // the signals. If new settings got to be saved a new buffer is allocated.
    CDrawSettings* pDrawSettingsTmp = m_pDrawSettingsTmp;
    m_pDrawSettingsTmp = nullptr;

    if (i_settings != m_drawSettings) {
        m_drawSettings = i_settings;
        emit_drawSettingsChanged(m_drawSettings);
    }
    delete pDrawSettingsTmp;
    pDrawSettingsTmp = nullptr;

}

//------------------------------------------------------------------------------
/*! @brief Emits the drawSettingsChanged signal.

    If there are temporary stored changes those changes will first be taken
    over before the signal is emitted.

    The temporary cache is used if several draw setting attributes got to be
    modified one after another. For this the flag i_bImmediatelyApplySetting
    has been passed with false to the "set<DrawSettingProperty>" methods.
    After modifying all attributes "updateDrawSettings" must be called to take
    over the modified settings and to emit the drawSettingsChanged signal.

    @return true, if there were changes, false otherwise.
*/
bool CDrawingScene::updateDrawSettings()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateDrawSettings",
        /* strAddInfo   */ "" );

    bool bChanged = false;
    if (m_pDrawSettingsTmp != nullptr) {
        if (*m_pDrawSettingsTmp != m_drawSettings) {
            bChanged = true;
            // setDrawSettings will delete the temporary buffer.
            setDrawSettings(*m_pDrawSettingsTmp);
        }
        else {
            delete m_pDrawSettingsTmp;
            m_pDrawSettingsTmp = nullptr;
        }
    }

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bChanged);
    }
    return bChanged;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the pen color to be used when creating new graphical objects.

    If several draw setting attributes got to modified one after another
    the flag i_bImmediatelyApplySetting should be set to false.
    In this case the modified draw setting attribute will be stored in a
    temporary cache and the signal "drawSettingsChanged" is not emitted.

    After setting all draw attributes the method updateDrawSettings must be
    called to take over the modified draw attributes from the temporary cache
    to the current draw settings. The updateDrawSettings method will emit the
    drawSettingsChanged signal.

    @param [in] i_clr
        New pen color to be used.
    @param [in] i_bImmediatelyApplySetting (default: true)
        Set this flag to false if further set<DrawAttribute> will follow and you
        want to update all changes at once later on.
*/
void CDrawingScene::setPenColor( const QColor& i_clr, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_clr.name() + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setPenColor",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (i_clr != m_drawSettings.penColor()) {
            m_drawSettings.setPenColor(i_clr);
            emit_drawSettingsChanged(m_drawSettings);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setPenColor(i_clr);
    }
}

//------------------------------------------------------------------------------
void CDrawingScene::setPenWidth( int i_iPenWidth, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iPenWidth);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setPenWidth",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (i_iPenWidth != m_drawSettings.penWidth()) {
            m_drawSettings.setPenWidth(i_iPenWidth);
            emit_drawSettingsChanged(m_drawSettings);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setPenWidth(i_iPenWidth);
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::setLineStyle( const CEnumLineStyle& i_lineStyle, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_lineStyle.toString() + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setLineStyle",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (i_lineStyle != m_drawSettings.lineStyle()) {
            m_drawSettings.setLineStyle(i_lineStyle);
            emit_drawSettingsChanged(m_drawSettings);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setLineStyle(i_lineStyle);
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::setFillColor( const QColor& i_clr, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_clr.name() + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setFillColor",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (i_clr != m_drawSettings.fillColor()) {
            m_drawSettings.setFillColor(i_clr);
            emit_drawSettingsChanged(m_drawSettings);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setFillColor(i_clr);
    }
}

//------------------------------------------------------------------------------
void CDrawingScene::setFillStyle( const CEnumFillStyle& i_fillStyle, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_fillStyle.toString() + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setFillStyle",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (i_fillStyle != m_drawSettings.fillStyle()) {
            m_drawSettings.setFillStyle(i_fillStyle);
            emit_drawSettingsChanged(m_drawSettings);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setFillStyle(i_fillStyle);
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::setLineRecordType( const CEnumLineRecordType& i_recordType, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_recordType.toString() + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setLineRecordType",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (i_recordType != m_drawSettings.lineRecordType()) {
            m_drawSettings.setLineRecordType(i_recordType);
            emit_drawSettingsChanged(m_drawSettings);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setLineRecordType(i_recordType);
    }
}

//------------------------------------------------------------------------------
void CDrawingScene::setLineExtent( int i_iLineExtent, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iLineExtent) + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setLineExtent",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (i_iLineExtent != m_drawSettings.lineExtent()) {
            m_drawSettings.setLineExtent(i_iLineExtent);
            emit_drawSettingsChanged(m_drawSettings);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setLineExtent(i_iLineExtent);
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::setLineEndStyle(
    const CEnumLinePoint& i_linePoint, const CEnumLineEndStyle& i_endStyle,
    bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_linePoint.toString() + ", " + i_endStyle.toString()
            + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setLineEndStyle",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (i_endStyle != m_drawSettings.lineEndStyle(i_linePoint)) {
            m_drawSettings.setLineEndStyle(i_linePoint, i_endStyle);
            emit_drawSettingsChanged(m_drawSettings);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setLineEndStyle(i_linePoint, i_endStyle);
    }
}

//------------------------------------------------------------------------------
void CDrawingScene::setArrowHeadBaseLineType(
    const CEnumLinePoint& i_linePoint, const CEnumArrowHeadBaseLineType& i_baseLineType,
    bool i_bImmediatelyApplySetting)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_linePoint.toString() + ", " + i_baseLineType.toString()
            + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setArrowHeadBaseLineType",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (i_baseLineType != m_drawSettings.arrowHeadBaseLineType(i_linePoint)) {
            m_drawSettings.setArrowHeadBaseLineType(i_linePoint, i_baseLineType);
            emit_drawSettingsChanged(m_drawSettings);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setArrowHeadBaseLineType(i_linePoint, i_baseLineType);
    }
}

//------------------------------------------------------------------------------
void CDrawingScene::setArrowHeadFillStyle(
    const CEnumLinePoint& i_linePoint, const CEnumArrowHeadFillStyle& i_fillStyle,
    bool i_bImmediatelyApplySetting)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_linePoint.toString() + ", " + i_fillStyle.toString()
            + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setArrowHeadFillStyle",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (i_fillStyle != m_drawSettings.arrowHeadFillStyle(i_linePoint)) {
            m_drawSettings.setArrowHeadFillStyle(i_linePoint, i_fillStyle);
            emit_drawSettingsChanged(m_drawSettings);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setArrowHeadFillStyle(i_linePoint, i_fillStyle);
    }
}

//------------------------------------------------------------------------------
void CDrawingScene::setArrowHeadWidth(
    const CEnumLinePoint& i_linePoint, const CEnumArrowHeadWidth& i_width,
    bool i_bImmediatelyApplySetting)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_linePoint.toString() + ", " + i_width.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setArrowHeadWidth",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (i_width != m_drawSettings.arrowHeadWidth(i_linePoint)) {
            m_drawSettings.setArrowHeadWidth(i_linePoint, i_width);
            emit_drawSettingsChanged(m_drawSettings);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setArrowHeadWidth(i_linePoint, i_width);
    }
}

//------------------------------------------------------------------------------
void CDrawingScene::setArrowHeadLength(
    const CEnumLinePoint& i_linePoint, const CEnumArrowHeadLength& i_length,
    bool i_bImmediatelyApplySetting)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_linePoint.toString() + ", " + i_length.toString()
            + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setArrowHeadLength",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (i_length != m_drawSettings.arrowHeadLength(i_linePoint)) {
            m_drawSettings.setArrowHeadLength(i_linePoint, i_length);
            emit_drawSettingsChanged(m_drawSettings);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setArrowHeadLength(i_linePoint, i_length);
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::setTextColor( const QColor& i_clr, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_clr.name() + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setTextColor",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (i_clr != m_drawSettings.textColor()) {
            m_drawSettings.setTextColor(i_clr);
            emit_drawSettingsChanged(m_drawSettings);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setTextColor(i_clr);
    }
}

//------------------------------------------------------------------------------
void CDrawingScene::setTextFont( const QFont& i_fnt, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_fnt.family() + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setTextFont",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (i_fnt != m_drawSettings.textFont()) {
            m_drawSettings.setTextFont(i_fnt);
            emit_drawSettingsChanged(m_drawSettings);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setTextFont(i_fnt);
    }
}

//------------------------------------------------------------------------------
void CDrawingScene::setTextSize( ETextSize i_size, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = textSize2Str(i_size) + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setTextSize",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (i_size != m_drawSettings.textSize()) {
            m_drawSettings.setTextSize(i_size);
            emit_drawSettingsChanged(m_drawSettings);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setTextSize(i_size);
    }
}

//------------------------------------------------------------------------------
void CDrawingScene::setTextStyle( const CEnumTextStyle& i_textStyle, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_textStyle.toString() + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setTextStyle",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (i_textStyle != m_drawSettings.textStyle()) {
            m_drawSettings.setTextStyle(i_textStyle);
            emit_drawSettingsChanged(m_drawSettings);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setTextStyle(i_textStyle);
    }
}

//------------------------------------------------------------------------------
void CDrawingScene::setTextEffect( const CEnumTextEffect& i_textEffect, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_textEffect.toString() + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setTextEffect",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (i_textEffect != m_drawSettings.textEffect()) {
            m_drawSettings.setTextEffect(i_textEffect);
            emit_drawSettingsChanged(m_drawSettings);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setTextEffect(i_textEffect);
    }
}

/*==============================================================================
public: // overridables of base class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CDrawingScene::event(QEvent* i_pEv)
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;
    if (i_pEv->type() >= QEvent::User) {
        CMsg* pMsg = nullptr;
        try {
            pMsg = dynamic_cast<CMsg*>(i_pEv);
        }
        catch (...) {
            pMsg = nullptr;
        }
        if (pMsg != nullptr) {
            QString strMthInArgs;
            if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
                strMthInArgs = pMsg->getAddTrcInfoStr();
            }
            CMethodTracer mthTracer(
                /* pAdminObj    */ m_pTrcAdminObj,
                /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
                /* strMethod    */ "event",
                /* strAddInfo   */ strMthInArgs );
            if (pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeReq) {
                CMsgReq* pMsgReq = dynamic_cast<CMsgReq*>(pMsg);
                if (pMsgReq == nullptr) {
                    throw CException(__FILE__, __LINE__, EResultMessageTypeMismatch);
                }
                switch (static_cast<int>(pMsgReq->type())) {
                    case EMsgType::EMsgTypeReqDeleteGraphobj: {
                        CMsgReqDeleteGraphObj* pMsgReqDeleteGraphObj = dynamic_cast<CMsgReqDeleteGraphObj*>(pMsgReq);
                        if (pMsgReqDeleteGraphObj == nullptr) {
                            throw CException(__FILE__, __LINE__, EResultMessageTypeMismatch);
                        }
                        CGraphObj* pGraphObj = this->findGraphObj(pMsgReqDeleteGraphObj->keyInTree());
                        if (pGraphObj != nullptr) {
                            delete pGraphObj;
                            pGraphObj = nullptr;
                        }
                        break;
                    }
                    default: {
                        break;
                    }
                }
            }
        }
    }
    if (!bEventHandled) {
        bEventHandled = QGraphicsScene::event(i_pEv);
    }
    return bEventHandled;
}

/*==============================================================================
public: // overridables of base class QGraphicsScene
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::dragEnterEvent( QGraphicsSceneDragDropEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneDragDropEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dragEnterEvent",
        /* strAddInfo   */ strMthInArgs );

    const QMimeData* pMimeData = i_pEv->mimeData();
    if (pMimeData != nullptr) {
        if (pMimeData->hasText()) {
            i_pEv->accept();
        }
        else if (pMimeData->hasImage()) {
            i_pEv->accept();
        }
        else if (pMimeData->hasUrls()) {
            i_pEv->accept();
        }
        else {
            i_pEv->ignore();
        }
    }
    else {
        i_pEv->ignore();
    }

    //QGraphicsScene::dragEnterEvent(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CDrawingScene::dragMoveEvent( QGraphicsSceneDragDropEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneDragDropEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dragMoveEvent",
        /* strAddInfo   */ strMthInArgs );

    const QMimeData* pMimeData = i_pEv->mimeData();
    if (pMimeData != nullptr) {
        if (pMimeData->hasText()) {
            i_pEv->accept();
        }
        else if (pMimeData->hasImage()) {
            i_pEv->accept();
        }
        else if (pMimeData->hasUrls()) {
            i_pEv->accept();
        }
        else {
            i_pEv->ignore();
        }
    }
    else {
        i_pEv->ignore();
    }
    //QGraphicsScene::dragMoveEvent(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

//------------------------------------------------------------------------------
void CDrawingScene::dragLeaveEvent( QGraphicsSceneDragDropEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneDragDropEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dragLeaveEvent",
        /* strAddInfo   */ strMthInArgs );

    const QMimeData* pMimeData = i_pEv->mimeData();
    if (pMimeData != nullptr) {
        if (sceneRect().contains(i_pEv->scenePos())) {
            if (pMimeData->hasText()) {
                i_pEv->accept();
            }
            else if (pMimeData->hasImage()) {
                i_pEv->accept();
            }
            else if (pMimeData->hasUrls()) {
                i_pEv->accept();
            }
            else {
                i_pEv->ignore();
            }
        }
        else {
            i_pEv->ignore();
        }
    }
    else {
        i_pEv->ignore();
    }
    //QGraphicsScene::dragLeaveEvent(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CDrawingScene::dropEvent( QGraphicsSceneDragDropEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneDragDropEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dropEvent",
        /* strAddInfo   */ strMthInArgs );

    bool bAccepted = false;

    const QMimeData* pMimeData = i_pEv->mimeData();
    if (pMimeData != nullptr && sceneRect().contains(i_pEv->scenePos())) {
        if (pMimeData->hasText()) {
            bAccepted = true;
            QString strMimeData;
            if (pMimeData != nullptr) {
                strMimeData = pMimeData->text();
            }
            QStringList strlstMimeData = strMimeData.split("; ",Qt::SkipEmptyParts);
            if (strlstMimeData.size() > 0) {
                clearSelection();
                for (int idxObj = 0; idxObj < strlstMimeData.size(); idxObj++) {
                    QString strObj = strlstMimeData[idxObj];
                    QStringList strlstObjPath = strObj.split("::");
                    if (strlstObjPath.size() > 1) { // must contain type and object path
                        QString strType = strlstObjPath[0];
                        if (strType.compare("ObjFactory",Qt::CaseInsensitive) == 0) {
                            strlstObjPath.removeFirst();
                            QString strObjPath = strlstObjPath.join("::");
                            CObjFactory* pObjFactory = CObjFactory::FindObjFactory(strObjPath);
                            if (pObjFactory != nullptr) {
                                CGraphObj* pGraphObj = pObjFactory->createGraphObj(this, convert(i_pEv->scenePos()), m_drawSettings);
                                QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(pGraphObj);
                                if (pGraphicsItem == nullptr) {
                                    throw CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphicsItem == nullptr" );
                                }
                                addGraphObj(pGraphObj);
                                //pGraphicsItem->setPos( i_pEv->scenePos() );
                                pGraphicsItem->setSelected(true);
                                //onGraphObjCreationFinished(pGraphObj);
                                //setMode(EMode::Undefined, EEditTool::Select, EEditMode::Move, EEditResizeMode::None, false);
                            }
                        }
                    }
                }
            }
        }
        else if (pMimeData->hasImage()) {
            bAccepted = true;
        }
        else if (pMimeData->hasUrls()) {
            bAccepted = true;
            QList<QUrl> arurls = pMimeData->urls();
            for (const QUrl& url : arurls) {
                QString strFilePath = url.toLocalFile();
                CGraphObjImage* pGraphObjImage = new CGraphObjImage(this);
                pGraphObjImage->setDrawSettings(m_drawSettings);
                pGraphObjImage->setImageFilePath(strFilePath);
                addGraphObj(pGraphObjImage);
                pGraphObjImage->setPos(i_pEv->scenePos());
                //onGraphObjCreationFinished(pGraphObjImage);
                //setMode(EMode::Undefined, EEditTool::Select, EEditMode::Move, EEditResizeMode::None, false);
            }
        }
    }
    if (bAccepted) {
        i_pEv->accept();
    }
    else {
        i_pEv->ignore();
    }
    //QGraphicsScene::dropEvent(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

/*==============================================================================
public: // overridables of base class QGraphicsScene
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Handles the mouse press event.

    In edit mode:

    - If currently an object is "under construction" the mouse event will be
      forwarded to the object under construction. Polygons for example will
      add new shape points when receiving mouse press events.
    - If currently no object is "under construction" but an edit tool is
      selected (if a graph object factory has been set), a new graphical object
      is created by the factory.
    - If currently no object is "under construction" but the edit tool
      "connection lines" is selected the method checks whethr a connection point
      has been hit as connection lines may only be created between connection points.

    If not in edit mode or if in edit mode the mouse press event was not handled
    the mouse event will be forwarded to the top most object which has been hit
    by the mouse cursor (see method forwardMouseEventToObjectsHit for further
    details on how the top most object is determined).
*/
void CDrawingScene::mousePressEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Enter, "States");
        traceAllItems(mthTracer, EMethodDir::Enter, "StackingOrder");
    }

    bool bEventHandled = false;

    m_ptMouseEvScenePosOnMousePressEvent = i_pEv->scenePos();

    emit_mousePosChanged(i_pEv->scenePos());

    EGraphObjType graphObjFactoryType = EGraphObjTypeUndefined;
    if (m_pObjFactory != nullptr) {
        graphObjFactoryType = m_pObjFactory->graphObjType();
    }

    if (m_pGraphicsItemSelectionArea != nullptr) {
        QGraphicsScene::removeItem(m_pGraphicsItemSelectionArea);
        delete m_pGraphicsItemSelectionArea;
        m_pGraphicsItemSelectionArea = nullptr;
    }

    if (m_mode == EMode::Edit) {
        // If currently an object is "under construction" ...
        //if (m_pGraphObjUnderConstruction != nullptr || m_pGraphicsItemAddingShapePoints != nullptr) {
        //    // ... forward mouse event to object "under construction".
        //    if (m_pGraphObjUnderConstruction != nullptr) {
        //        forwardMouseEvent(m_pGraphObjUnderConstruction, i_pEv);
        //    }
        //    else /*if (m_pGraphicsItemAddingShapePoints != nullptr)*/ {
        //        forwardMouseEvent(m_pGraphicsItemAddingShapePoints, i_pEv);
        //    }
        //    bEventHandled = i_pEv->isAccepted();
        //}
        // If no object is "under construction" ...
        if (m_pGraphObjUnderConstruction == nullptr) {
            // If an object got to be created ...
            if (m_pObjFactory != nullptr) {
                bool bCreateObj = true;
                CGraphObjConnectionPoint* pGraphObjCnctPtHit = nullptr;
                // Connection lines may only be created on connection points.
                // A connection point must have been hit to create connection lines.
                if (graphObjFactoryType == EGraphObjTypeConnectionLine) {
                    // Check whether a connection point has been hit.
                    pGraphObjCnctPtHit = getConnectionPoint(i_pEv->scenePos());
                    if (pGraphObjCnctPtHit == nullptr) {
                        bCreateObj = false;
                    }
                }
                if (bCreateObj) {
                    // Don't create objects if mouse press was outside the scene rectangle.
                    if (!sceneRect().contains(i_pEv->scenePos())) {
                        bCreateObj = false;
                    }
                }
                if (bCreateObj) {
                    // Unselect all currently selected objects.
                    clearSelection();

                    // Graphical objects must always be created at their transformation origin points.
                    // Otherwise mapping coordinates to group coordinates does not work correctly.
                    // This is especially true for connection points and circles.
                    m_pGraphObjUnderConstruction = m_pObjFactory->createGraphObj(this, convert(i_pEv->scenePos()), m_drawSettings);
                    if (m_pGraphObjUnderConstruction != nullptr) {
                        QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(m_pGraphObjUnderConstruction);
                        if (pGraphicsItem == nullptr) {
                            throw CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphicsItem == nullptr" );
                        }
                        addGraphObj(m_pGraphObjUnderConstruction);

                        // On creating objects by a mouse press events the object may be "under construction"
                        // and following mouse move events may be used to immediately resize the object.
                        // For this the object will be selected and selection points are created.
                        // The selection points must get the following mouse move events to modify the
                        // shape of the graphical object.
                        // Afterwards QGraphicsScene::mousePressEvent must be called to set the mouse grabber item.
                        m_pGraphObjUnderConstruction->setEditMode(EEditMode::CreatingByMouseEvents);
                        QObject::connect(
                            m_pGraphObjUnderConstruction, &CGraphObj::editModeChanged,
                            this, &CDrawingScene::onGraphObjEditModeChanged );

                        CGraphObjConnectionLine* pGraphObjCnctLineCreating = nullptr;
                        //CGraphObjConnectionPoint* pGraphObjCnctPtCreating = nullptr;
                        if (m_pGraphObjUnderConstruction->isConnectionLine()) {
                            pGraphObjCnctLineCreating = dynamic_cast<CGraphObjConnectionLine*>(m_pGraphObjUnderConstruction);
                        }
                        //else if (pGraphObj->isConnectionPoint()) {
                        //    pGraphObjCnctPtCreating = dynamic_cast<CGraphObjConnectionPoint*>(pGraphObj);
                        //}

                        //#pragma message(__TODO__)
                        //if (pGraphObjCnctPtCreating != nullptr) {
                        //    // Need to set left top position of connection points as transformation origin point.
                        //    // Otherwise mapping coordinates to group coordinates does not work correctly.
                        //    QRectF rctGraphObjCnctPt = pGraphObjCnctPtCreating->rect();
                        //    QPointF ptPosGraphObjCnctPt = QPointF(
                        //        i_pEv->scenePos().x() - rctGraphObjCnctPt.width()/2.0,
                        //        i_pEv->scenePos().y() - rctGraphObjCnctPt.height()/2.0 );
                        //    pGraphObjCnctPtCreating->setPos(ptPosGraphObjCnctPt);
                        //}
                        //else {
                        //    //m_pGraphObjUnderConstruction->setPos(i_pEv->scenePos());
                        //}

                        if (pGraphObjCnctLineCreating != nullptr && pGraphObjCnctPtHit != nullptr) {
                            pGraphObjCnctLineCreating->setConnectionPoint(ELinePoint::Start, pGraphObjCnctPtHit);
                        }
                        //emit_modeChanged(m_mode);
                    }
                }
            }
            // If no object got to be created ...
            else /*if (m_pObjFactory == nullptr)*/ {
                // Check if an object has been clicked.
                // If an object has been clicked, without pressing the keyboard modifiers
                // Shift or Control, the newly clicked object should be the only one to be selected.

                if (!i_pEv->modifiers().testFlag(Qt::ShiftModifier) && !i_pEv->modifiers().testFlag(Qt::ControlModifier)) {
                    QGraphicsItem* pGraphicsItemPressed = itemAt(i_pEv->scenePos(), QTransform());
                    if (pGraphicsItemPressed != nullptr) {
                        QList<QGraphicsItem*> arpGraphicsItemsSelected = selectedItems();
                        if (!arpGraphicsItemsSelected.empty()) {
                            unselectGraphicsItems(arpGraphicsItemsSelected);
                        }
                    }
                }

                // Dispatch mouse event to objects "under cursor".
                QGraphicsScene_mousePressEvent(i_pEv);
                if (m_pGraphObjMouseGrabber != nullptr) {
                    dynamic_cast<QGraphicsItem*>(m_pGraphObjMouseGrabber)->grabMouse();
                    m_pGraphObjMouseGrabber = nullptr;
                }
                bEventHandled = true;

                // If no object has been selected ...
                if (selectedItems().size() == 0) {
                    // .. create selection rectangle area if not yet existing.
                    //setMode(EMode::Undefined, EEditTool::Undefined, EEditMode::None, EEditResizeMode::None, false);
                    QRectF rctSelectionArea(
                        /* x      */ i_pEv->scenePos().x(),
                        /* y      */ i_pEv->scenePos().y(),
                        /* width  */ 1,
                        /* height */ 1 );
                    if (m_pGraphicsItemSelectionArea == nullptr) {
                        QPen penSelectionArea(Qt::DotLine);
                        penSelectionArea.setColor(Qt::black);
                        QBrush brsSelectionArea(Qt::SolidPattern);
                        int iHue   = 240; // 0..359
                        int iSat   =  63; // 0..255
                        int iVal   = 127; // 0..255
                        int iAlpha =  63; // 0..255
                        brsSelectionArea.setColor(QColor::fromHsv(iHue, iSat, iVal, iAlpha));
                        m_pGraphicsItemSelectionArea = addRect(
                            /* rect   */ rctSelectionArea,
                            /* pen    */ penSelectionArea,
                            /* brush  */ brsSelectionArea );
                    }
                }
            }
        }
    }

    if (!bEventHandled) {
        // Dispatch mouse event to objects "under cursor".
        QGraphicsScene_mousePressEvent(i_pEv);
        if (m_pGraphObjMouseGrabber != nullptr) {
            dynamic_cast<QGraphicsItem*>(m_pGraphObjMouseGrabber)->grabMouse();
            m_pGraphObjMouseGrabber = nullptr;
        }
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Leave, "States");
        traceAllItems(mthTracer, EMethodDir::Enter, "StackingOrder");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
} // mousePressEvent

//------------------------------------------------------------------------------
/*! @brief Handles the mouse release event.
*/
void CDrawingScene::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "mouseReleaseEvent",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Enter, "States");
    }

    EGraphObjType graphObjFactoryType = EGraphObjTypeUndefined;
    if (m_pObjFactory != nullptr) {
        graphObjFactoryType = m_pObjFactory->graphObjType();
    }

    bool bEventHandled = false;

    QPointF ptScenePosMouseEvent = i_pEv->scenePos();

    emit_mousePosChanged(ptScenePosMouseEvent);

    if (m_mode == EMode::Edit) {
        // If currently an object is "under construction" ...
        //if (m_pGraphObjUnderConstruction != nullptr || m_pGraphicsItemAddingShapePoints != nullptr)
        //{
        //    // ... forward mouse event to object "under construction".
        //    if (m_pGraphObjUnderConstruction != nullptr) {
        //        forwardMouseEvent(m_pGraphObjUnderConstruction, i_pEv);
        //    }
        //    else /*if (m_pGraphicsItemAddingShapePoints != nullptr)*/ {
        //        forwardMouseEvent(m_pGraphicsItemAddingShapePoints, i_pEv);
        //    }
        //}
        // If an object is "under construction" ...
        if (m_pGraphObjUnderConstruction != nullptr) {
            // If an obect is under construction, the mouse grabber item is a selection point.
            // If mouse release events don't finish the object creation, a selection point must
            // still be a mouse grabber after the mouse release event has been dispatched.
            // After dispatching the mouse release event to the mouse grabber item, the
            // graphics scene will reset the mouse grabber. So it is useless trying to
            // keep the mouse grabber within the item's mouseReleaseEvent method.
            // We cannot simply store the current mouse grabber and restore the grabber after
            // the event has been dispatched as the selection point, which should grab the mouse
            // events, may change and the current mouse grabber item may even be destroyed while
            // dispatching the mouse event.
            // Which selection point should be the the mouse grabber after handling the mouse event,
            // knows the object under construction. If a selection point should become (or remain)
            // the mouse grabber item, the object under construction needs to set a "mouseGrabber"
            // flag by calling "setMouseGrabberItem". After the mouse event has been dispatched,
            // the graphics scene checks whether a mouse grabber item has been explicitly set,
            // uses this as the mouse grabber and invalidates the "mouseGrabberItem" flag again.

            // Dispatch mouse event to objects "under cursor".
            QGraphicsScene_mouseReleaseEvent(i_pEv);
            bEventHandled = true;
            if (m_pGraphObjMouseGrabber != nullptr) {
                dynamic_cast<QGraphicsItem*>(m_pGraphObjMouseGrabber)->grabMouse();
                m_pGraphObjMouseGrabber = nullptr;
            }

            // If an object is under construction the mouse grabber item is a selection point
            // and mouse release events should not invalidate the mouse grabber item but
            // the selection point should be kept as the mouse grabber item.
            //if (m_pGraphObjUnderConstruction->mouseReleaseEventFinishesObjectCreation()) {
            //    m_pGraphObjUnderConstruction->setEditMode(EEditMode::None);
            //    dynamic_cast<QGraphicsItem*>(m_pGraphObjUnderConstruction)->setSelected(true);
            //    QObject::disconnect(
            //        m_pGraphObjUnderConstruction, &CGraphObj::editModeChanged,
            //        this, &CDrawingScene::onGraphObjEditModeChanged );
            //    m_pGraphObjUnderConstruction = nullptr;
            //    setCurrentDrawingTool(nullptr);
            //}
            //else if (pGraphicsItemMouseGrabber != nullptr && mouseGrabberItem() == nullptr) {
            //    pGraphicsItemMouseGrabber->grabMouse();
            //}
        }
        // If no object is "under construction" ...
        else /*if (m_pGraphObjUnderConstruction == nullptr)*/ {
            if (m_pGraphicsItemSelectionArea != nullptr) {
                QRectF rctSelectionArea(
                    /* x      */ m_ptMouseEvScenePosOnMousePressEvent.x(),
                    /* y      */ m_ptMouseEvScenePosOnMousePressEvent.y(),
                    /* width  */ ptScenePosMouseEvent.x() - m_ptMouseEvScenePosOnMousePressEvent.x() + 1,
                    /* height */ ptScenePosMouseEvent.y() - m_ptMouseEvScenePosOnMousePressEvent.y() + 1 );

                QPainterPath path;
                path.addRect(rctSelectionArea);
                #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                setSelectionArea(path, Qt::ContainsItemShape);
                #else
                setSelectionArea(path, Qt::ReplaceSelection, Qt::ContainsItemShape);
                #endif
                bEventHandled = true;

                QList<QGraphicsItem*> arpGraphicsItemsSelected = selectedItems();
                if (arpGraphicsItemsSelected.size() > 1) {
                    // Selection points, labels and connection lines will not become part of a group
                    // will be removed from the list of selected items.
                    // In addition items which already belong as childs to a group got to be removed
                    // from the list of selected items as those childs will be added to the new group
                    // as childs of the item.
                    QList<QGraphicsItem*> arpGraphicsItemsToBeRemoved;
                    for (QGraphicsItem* pGraphicsItemSelected : arpGraphicsItemsSelected) {
                        CGraphObj* pGraphObjSelected = dynamic_cast<CGraphObj*>(pGraphicsItemSelected);
                        if (pGraphObjSelected->isConnectionLine() || pGraphObjSelected->isSelectionPoint() || pGraphObjSelected->isLabel()) {
                            arpGraphicsItemsToBeRemoved.append(pGraphicsItemSelected);
                        }
                        else if (pGraphicsItemSelected->parentItem() != nullptr) {
                            CGraphObjGroup* pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(pGraphicsItemSelected->parentItem());
                            if (pGraphObjGroup != nullptr) {
                                arpGraphicsItemsToBeRemoved.append(pGraphicsItemSelected);
                            }
                        }
                    }
                    for (QGraphicsItem* pGraphicsItemSelected : arpGraphicsItemsToBeRemoved) {
                        arpGraphicsItemsSelected.removeOne(pGraphicsItemSelected);
                    }
                }
                if (arpGraphicsItemsSelected.size() > 1) {
                    QRectF rctBounding;
                    for (QGraphicsItem* pGraphicsItemSelected : arpGraphicsItemsSelected) {
                        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItemSelected);
                        if (pGraphObj != nullptr) {
                            rctBounding |= pGraphicsItemSelected->mapToScene(pGraphObj->getBoundingRect()).boundingRect();
                        }
                    }
                    m_pGraphicsItemSelectionArea->setRect(rctBounding);
                }
                else {
                    QGraphicsScene::removeItem(m_pGraphicsItemSelectionArea);
                    delete m_pGraphicsItemSelectionArea;
                    m_pGraphicsItemSelectionArea = nullptr;
                }
            }
        }
    }

    if (!bEventHandled) {
        // Dispatch mouse event to objects "under cursor".
        QGraphicsScene_mouseReleaseEvent(i_pEv);
        if (m_pGraphObjMouseGrabber != nullptr) {
            dynamic_cast<QGraphicsItem*>(m_pGraphObjMouseGrabber)->grabMouse();
            m_pGraphObjMouseGrabber = nullptr;
        }
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Leave, "States");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
} // mouseReleaseEvent

//------------------------------------------------------------------------------
/*! @brief Handles the mouse double click event.
*/
void CDrawingScene::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "mouseDoubleClickEvent",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Enter, "States");
    }

    bool bEventHandled = false;

    EGraphObjType graphObjFactoryType = EGraphObjTypeUndefined;
    if (m_pObjFactory != nullptr) {
        graphObjFactoryType = m_pObjFactory->graphObjType();
    }

    QPointF ptScenePosMouseEvent = i_pEv->scenePos();

    emit_mousePosChanged(ptScenePosMouseEvent);

    if (m_mode == EMode::Edit) {
        // If currently an object is "under construction" ...
        //if (m_pGraphObjUnderConstruction != nullptr || m_pGraphicsItemAddingShapePoints != nullptr) {
        //    // If currently a connection line is "under construction" ...
        //    if (m_pGraphObjUnderConstruction != nullptr && m_pGraphObjUnderConstruction->isConnectionLine()) {
        //        CGraphObjConnectionPoint* pGraphObjCnctPt = nullptr;
        //        CGraphObjConnectionLine* pGraphObjCnctLine = dynamic_cast<CGraphObjConnectionLine*>(m_pGraphObjUnderConstruction);
        //        if (pGraphObjCnctLine != nullptr) {
        //            pGraphObjCnctPt = getConnectionPoint(i_pEv->scenePos());
        //            if (pGraphObjCnctPt != nullptr) {
        //                bIsValidEndPoint = true;
        //            }
        //        }
        //        if (!bIsValidEndPoint) {
        //            delete m_pGraphObjUnderConstruction;
        //            m_pGraphObjUnderConstruction = nullptr;
        //            m_pGraphicsItemAddingShapePoints = nullptr;
        //            m_pGraphObjAddingShapePoints = nullptr;
        //            pGraphObjCnctLine = nullptr;
        //        }
        //    }
        //    // ... dispatch mouse event to object "under construction".
        //    if (m_pGraphObjUnderConstruction != nullptr) {
        //        forwardMouseEvent(m_pGraphObjUnderConstruction, i_pEv);
        //    }
        //    else if (m_pGraphicsItemAddingShapePoints != nullptr) {
        //        forwardMouseEvent(m_pGraphicsItemAddingShapePoints, i_pEv);
        //    }
        //}
        // If an object is "under construction" ...
        if (m_pGraphObjUnderConstruction != nullptr) {
            // Dispatch mouse event to objects "under cursor".
            QGraphicsScene_mouseDoubleClickEvent(i_pEv);
            bEventHandled = true;
        }
    }

    if (!bEventHandled) {
        // Dispatch mouse event to objects "under cursor".
        QGraphicsScene_mouseDoubleClickEvent(i_pEv);
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Leave, "States");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
} // mouseDoubleClickEvent

//------------------------------------------------------------------------------
/*! @brief Handles the mouse move event.

    What has to be done on moving the mouse while the left button is pressed
    depends on the activated drawing tool and whether an object is currently
    under construction or selected.

    If an object is selected the edit mode must have been set correspondingly on
    receiving the prior mouse press event.

    If a polygon or polygon is under construction new line segments will be
    appended to the previously created line segment (free hand painting).

    If no drawing tool is activated and no object is under construction the
    selected objects will be changed depending on the edit mode set by the
    prior mouse press event.

    If no drawing tool is activated and no object is selected the mouse move
    event is used to select a rectangle area within the drawing selecting all
    objects within this area later on receiving the mouse release event.
*/
void CDrawingScene::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    int iMouseButtonState = i_pEv->buttons() & Qt::MouseButtonMask;
    CTrcAdminObj* pTrcAdminObj = m_pTrcAdminObjMouseMoveEvents;
    if (iMouseButtonState != Qt::NoButton) {
        pTrcAdminObj = m_pTrcAdminObj;
    }
    QString strMthInArgs;
    if (areMethodCallsActive(pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "mouseMoveEvent",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Enter, "States");
    }

    bool bEventHandled = false;

    emit_mousePosChanged(i_pEv->scenePos());

    EGraphObjType graphObjFactoryType = EGraphObjTypeUndefined;
    if (m_pObjFactory != nullptr) {
        graphObjFactoryType = m_pObjFactory->graphObjType();
    }

    if (m_mode == EMode::Edit) {
        // If currently an object is "under construction" ...
        //if (m_pGraphObjUnderConstruction != nullptr || m_pGraphicsItemAddingShapePoints != nullptr) {
        //    // ... forward mouse event to object "under construction".
        //    if (m_pGraphObjUnderConstruction != nullptr) {
        //        forwardMouseEvent(m_pGraphObjUnderConstruction, i_pEv);
        //    }
        //    else /*if (m_pGraphicsItemAddingShapePoints != nullptr)*/ {
        //        forwardMouseEvent(m_pGraphicsItemAddingShapePoints, i_pEv);
        //    }
        //}
        // If currently no object is "under construction" ...
        {
            //bool bDispatchMouseEvents2ObjectsUnderMouseCursor = false;
            //bool bDispatchMouseEvents2ConnectionPointsUnderMouseCursor = false;
            //if (m_editTool == EEditTool::CreateObjects) {
            //    // If no mouse button is pressed ...
            //    if (iMouseButtonState == Qt::NoButton) {
            //        // If the drawing tool "ConnectionLine" is selected ...
            //        if (m_pObjFactory != nullptr && graphObjFactoryType == EGraphObjTypeConnectionLine) {
            //            bDispatchMouseEvents2ConnectionPointsUnderMouseCursor = true;
            //        }
            //    }
            //}
            if (m_pObjFactory == nullptr /*&&m_editTool == EEditTool::Select*/) {
                if (iMouseButtonState & Qt::LeftButton) {
                    if (m_pGraphicsItemSelectionArea != nullptr) {
                        QRectF rctSelectionArea(
                            /* x      */ m_ptMouseEvScenePosOnMousePressEvent.x(),
                            /* y      */ m_ptMouseEvScenePosOnMousePressEvent.y(),
                            /* width  */ i_pEv->scenePos().x() - m_ptMouseEvScenePosOnMousePressEvent.x() + 1,
                            /* height */ i_pEv->scenePos().y() - m_ptMouseEvScenePosOnMousePressEvent.y() + 1 );
                        m_pGraphicsItemSelectionArea->setRect(rctSelectionArea);
                        bEventHandled = true;
                    }
                    else {
                        // Dispatch mouse event to objects "under cursor".
                        QGraphicsScene_mouseMoveEvent(i_pEv);
                        bEventHandled = true;
                    }
                }
                else if (iMouseButtonState == Qt::NoButton) {
                    // Dispatch mouse event to objects "under cursor".
                    QGraphicsScene_mouseMoveEvent(i_pEv);
                    bEventHandled = true;
                    //bDispatchMouseEvents2ObjectsUnderMouseCursor = true;
                }
            }
            else {
                // Dispatch mouse event to objects "under cursor".
                QGraphicsScene_mouseMoveEvent(i_pEv);
                bEventHandled = true;
            }
            //if (bDispatchMouseEvents2ObjectsUnderMouseCursor || bDispatchMouseEvents2ConnectionPointsUnderMouseCursor) {
            //    // Some items may completely overlap (encircle) other objects (a big rectangle may completely
            //    // enclose a smaller rectangle or the bounding rectangle of a polygon may enclose other objects).
            //    // In this case Qt's graphic scene does not dispatch the mouse events as hover events to the
            //    // enclosed objects if they are not "on top" (according to the scene's item list Z order) of the
            //    // enclosing items. If the outer object does not hide the inner object (fill style solid pattern)
            //    // this behavior is not what we want and what the user expects. E.g. if a rectangle with
            //    // "FillStyle = NoFill" would enclose another object the user would expect that the inner object
            //    // can be selected by mouse clicks. And if you consider polygons which are never "filled" objects,
            //    // the inner objects should always be selectable by mouse clicks. The polygon as the outer object
            //    // should only be selected if one of its line segments would be hit.
            //    QRectF rctToCheck(
            //        /* x      */ i_pEv->scenePos().x() - m_fHitTolerance_px,
            //        /* y      */ i_pEv->scenePos().y() - m_fHitTolerance_px,
            //        /* width  */ 2*m_fHitTolerance_px,
            //        /* height */ 2*m_fHitTolerance_px );
            //    QList<QGraphicsItem*> arpGraphicsItemsIntersected =
            //        items(rctToCheck, Qt::IntersectsItemShape, Qt::AscendingOrder);
            //    QList<QGraphicsItem*> arpGraphicsItemsHit;

            //    // Create array with objects which have been hit by mouse cursor.
            //    if (arpGraphicsItemsIntersected.size() > 0) {
            //        for (QGraphicsItem* pGraphicsItem : arpGraphicsItemsIntersected) {
            //            CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);
            //            if (pGraphicsItem != nullptr && pGraphObj != nullptr) {
            //                bool bCheckIsHit = false;
            //                if (pGraphObj->isSelectionPoint()) {
            //                }
            //                else if (pGraphObj->isConnectionPoint()) {
            //                    bCheckIsHit = bDispatchMouseEvents2ConnectionPointsUnderMouseCursor ||
            //                                  bDispatchMouseEvents2ObjectsUnderMouseCursor;
            //                }
            //                else if (pGraphicsItem->parentItem() == nullptr) {
            //                    bCheckIsHit = bDispatchMouseEvents2ObjectsUnderMouseCursor;
            //               }
            //               if (bCheckIsHit) {
            //                    QPointF ptMouseItemPos = pGraphicsItem->mapFromScene(i_pEv->scenePos());
            //                    bool bGraphObjHit = pGraphObj->isHit(ptMouseItemPos, nullptr);
            //                    if (bGraphObjHit) {
            //                        arpGraphicsItemsHit.append(pGraphicsItem);
            //                    }
            //                }
            //            }
            //        }
            //    }

            //    // Objects which have been hit by mouse cursor temporarily accept hover events.
            //    for (QGraphicsItem* pGraphicsItemHit : arpGraphicsItemsHit) {
            //        CGraphObj* pGraphObjHit = dynamic_cast<CGraphObj*>(pGraphicsItemHit);
            //        if (pGraphicsItemHit != nullptr && pGraphObjHit != nullptr) {
            //            #if QT_VERSION < 0x050000
            //            if (!pGraphicsItemHit->acceptsHoverEvents()) {
            //            #else
            //            if (!pGraphicsItemHit->acceptHoverEvents()) {
            //            #endif
            //                pGraphicsItemHit->setAcceptHoverEvents(true);
            //                pGraphicsItemHit->setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton|Qt::MiddleButton|Qt::XButton1|Qt::XButton2);
            //                pGraphObjHit->setIsHit(true);
            //                // Append object hit by mouse cursor to list of objects temporarily accepting hover events (if not yet already part of that list).
            //                bool bGraphObjAcceptingHoverEventsFound = false;
            //                for (int idxGraphObj = 0; idxGraphObj < m_arpGraphicsItemsAcceptingHoverEvents.size(); idxGraphObj++) {
            //                    QGraphicsItem* pGraphicsItem = m_arpGraphicsItemsAcceptingHoverEvents[idxGraphObj];
            //                    CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);
            //                    // If already part of list ...
            //                    if (pGraphObjHit == pGraphObj) {
            //                        bGraphObjAcceptingHoverEventsFound = true;
            //                        break;
            //                    }
            //                }
            //                // If not already part of list ...
            //                if (!bGraphObjAcceptingHoverEventsFound) {
            //                    m_arpGraphicsItemsAcceptingHoverEvents.append(pGraphicsItemHit);
            //                }
            //            }
            //        }
            //    }

            //    // Objects which have not been hit, but have been part of the list temporarily accepting hover events,
            //    // must be removed from the list and are no longer accepting hover events.
            //    if (m_arpGraphicsItemsAcceptingHoverEvents.size() > 0) {
            //        for (int idxGraphObj = 0; idxGraphObj < m_arpGraphicsItemsAcceptingHoverEvents.size(); idxGraphObj++) {
            //            bool bGraphObjHitFound = false;
            //            QGraphicsItem* pGraphicsItem = m_arpGraphicsItemsAcceptingHoverEvents[idxGraphObj];
            //            CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);
            //            for (int idxGraphObjHit = 0; idxGraphObjHit < arpGraphicsItemsHit.size(); idxGraphObjHit++) {
            //                QGraphicsItem* pGraphicsItemHit = arpGraphicsItemsHit[idxGraphObjHit];
            //                CGraphObj* pGraphObjHit = dynamic_cast<CGraphObj*>(pGraphicsItemHit);
            //                if (pGraphObjHit == pGraphObj) {
            //                    bGraphObjHitFound = true;
            //                    break;
            //                }
            //            }
            //            if (!bGraphObjHitFound) {
            //                #if QT_VERSION < 0x050000
            //                if (pGraphicsItem->acceptsHoverEvents()) {
            //                #else
            //                if (pGraphicsItem->acceptHoverEvents()) {
            //                #endif
            //                    pGraphicsItem->setAcceptHoverEvents(false);
            //                    pGraphicsItem->setAcceptedMouseButtons(Qt::NoButton);
            //                    pGraphicsItem->unsetCursor();
            //                }
            //                if (/*!pGraphicsItem->isSelected() &&*/ pGraphObj->isHit()) {
            //                    pGraphObj->setIsHit(false);
            //                }
            //                m_arpGraphicsItemsAcceptingHoverEvents[idxGraphObj] = nullptr;
            //            }
            //        }
            //        for (int idxGraphObj = m_arpGraphicsItemsAcceptingHoverEvents.size()-1; idxGraphObj >= 0; idxGraphObj--) {
            //            if (m_arpGraphicsItemsAcceptingHoverEvents[idxGraphObj] == nullptr) {
            //                m_arpGraphicsItemsAcceptingHoverEvents.removeAt(idxGraphObj);
            //            }
            //        }
            //    }

            //    QGraphicsScene_mouseMoveEvent(i_pEv);
            //    bEventHandled = true;

            //}
        }
    }

    if (!bEventHandled) {
        // Dispatch mouse event to objects "under cursor".
        QGraphicsScene_mouseMoveEvent(i_pEv);
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Leave, "States");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
} // mouseMoveEvent

/*==============================================================================
public: // overridables of base class QGraphicsScene
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Handles the key press event.
*/
void CDrawingScene::keyPressEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjKeyEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qKeyEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjKeyEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "keyPressEvent",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Enter, "States");
    }

    QGraphicsScene::keyPressEvent(i_pEv);

    if (!i_pEv->isAccepted()) {
        // As default the key is not handled by the manual test.
        i_pEv->ignore();

        // Pressed any key for moving or changing the shape of the objects ...
        if (i_pEv->key() == Qt::Key_Left || i_pEv->key() == Qt::Key_Right || i_pEv->key() == Qt::Key_Up || i_pEv->key() == Qt::Key_Down) {
            //if (m_editTool == EEditTool::Select) {
                QList<QGraphicsItem*> arpGraphicsItemsSelected = selectedItems();
                // If at least one object has been selected which can be moved, resized or rotated ...
                if (arpGraphicsItemsSelected.size() > 0) {
                    // We are going to change the position or rotate the objects.
                    double fXOffs = 0;
                    double fYOffs = 0;
                    if (i_pEv->key() == Qt::Key_Left) {
                        fXOffs = -1.0;
                    }
                    else if (i_pEv->key() == Qt::Key_Right) {
                        fXOffs = 1.0;
                    }
                    else if (i_pEv->key() == Qt::Key_Up) {
                        fYOffs = -1.0;
                    }
                    else if (i_pEv->key() == Qt::Key_Down) {
                        fYOffs = 1.0;
                    }
                    for (QGraphicsItem* pGraphicsItem : arpGraphicsItemsSelected) {
                        // Children will be moved together with their parents ...
                        if (pGraphicsItem->parentItem() == nullptr) {
                            QPointF ptPos = pGraphicsItem->pos();
                            ptPos.setX(ptPos.x() + fXOffs);
                            ptPos.setY(ptPos.y() + fYOffs);
                            pGraphicsItem->setPos(ptPos);
                        }
                    }
                }
            //} // if( m_editTool == EEditTool::Select )
        }

        // Pressed any key for moving or changing the shape of the objects ...
        else if (i_pEv->key() == Qt::Key_Plus || i_pEv->key() == Qt::Key_Minus) {
            //if (m_editTool == EEditTool::Select) {
                QList<QGraphicsItem*> arpGraphicsItemsSelected = selectedItems();
                // If at least one object has been selected which can be rotated ...
                if (arpGraphicsItemsSelected.size() > 0) {
                    double fRotAngleOffs_deg = 0.0;
                    double fRotAngle_deg = 0.0;
                    if (i_pEv->key() == Qt::Key_Plus) {
                        fRotAngleOffs_deg = 1.0; // by one degree
                    }
                    else {
                        fRotAngleOffs_deg = -1.0; // be one degree
                    }
                    for (QGraphicsItem* pGraphicsItem : arpGraphicsItemsSelected) {
                        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);
                        // Children will be moved together with their parents ...
                        if (pGraphObj != nullptr && pGraphicsItem->parentItem() == nullptr) {
                            fRotAngle_deg = 0.0; //pGraphObj->getRotationAngleInDegree();
                            fRotAngle_deg += fRotAngleOffs_deg;
                            //pGraphObj->setRotationAngleInDegree(fRotAngle_deg);
                        }
                    }
                }
            //} // if( m_editTool == EEditTool::Select )
        }

        // Pressed key to delete selected objects ...
        else if (i_pEv->key() == Qt::Key_Delete) {
            QList<QGraphicsItem*> arpGraphicsItemsSelected = selectedItems();
            QStringList strlstGraphObjIdsSelected;
            // On deleting a graphical object the object may destroy other graphical objects
            // (e.g. connection points may delete connection lines) and the list of selected
            // items may change while the dtor is in process. So we don't store a list with object
            // references but a list with object ids and check whether the object still belong
            // to the drawing scene before deleting the object.
            for (QGraphicsItem* pGraphicsItem : arpGraphicsItemsSelected) {
                if (pGraphicsItem != m_pGraphicsItemSelectionArea) {
                    CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);
                    if (pGraphObj != nullptr) {
                        strlstGraphObjIdsSelected.append(pGraphObj->keyInTree());
                    }
                }
            }
            for (const QString& strGraphObjId : strlstGraphObjIdsSelected) {
                CGraphObj* pGraphObj = findGraphObj(strGraphObjId);
                if (pGraphObj != nullptr) {
                    delete pGraphObj;
                    pGraphObj = nullptr;
                }
            }
        }

        // Pressed key to copy selected objects to the (internal) clipboard ...
        else if (i_pEv->key() == Qt::Key_C) {
            if (i_pEv->modifiers() == Qt::ControlModifier) {
                //// Clear internal clipboard.
                //if (m_dctpGraphObjsClipboard.size() > 0) {
                //    QMap<QString,CGraphObj*>::iterator itGraphObjsClipboard = m_dctpGraphObjsClipboard.begin();
                //    while( itGraphObjsClipboard != m_dctpGraphObjsClipboard.end() )
                //    {
                //        pGraphObj = itGraphObjsClipboard.value();

                //        if( pGraphObj != nullptr )
                //        {
                //            deleteItem(pGraphObj);
                //        }
                //        ++itGraphObjsClipboard;
                //    }
                //}
                //m_dctpGraphObjsClipboard.clear();

                //// Add selected objects to the internal clipboard. Not only the references
                //// are copied but the objects will be newly created (cloned). By simply storing
                //// the references changing attributes on currently selected objects would take
                //// effect on objects in the clipboard.
                //// Only those objects will be added to the clipboard which don't belong to a
                //// group (which don't have parent objects) as the parent object must clone its
                //// children.
                //// Cloning connection points and connection lines is not that simply as cloning
                //// a group with children. Connection lines can only be cloned if both of its
                //// connection points will be cloned and the cloned connection line must be
                //// connected with the clones of its original connection points.

                //QList<QGraphicsItem*>     arpGraphicsItemsSelected = selectedItems();
                //CGraphObj*                pGraphObjClone;
                //CGraphObjConnectionLine*  pGraphObjCnctLine;
                //CGraphObjConnectionLine*  pGraphObjCnctLineClone;
                //CGraphObjConnectionPoint* pGraphObjCnctPt1;
                //CGraphObjConnectionPoint* pGraphObjCnctPt2;
                //CGraphObjConnectionPoint* pGraphObjCnctPt1Clone;
                //CGraphObjConnectionPoint* pGraphObjCnctPt2Clone;
                //QString                   strGraphObjName;
                //QString                   strGraphObjId;
                //QList<CGraphObj*>         arpGraphObjsOrigin;
                //QList<CGraphObj*>         arpGraphObjsClones;
                //int                       idxGraphObjTmp;

                //// During the first loop connection lines will not be cloned. Instead we are going to keep
                //// a list with original connection points and their clones. This way we are able in the
                //// second loop on cloning connection lines to detect the clones of the connection points.
                //for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsSelected.size(); idxGraphObj++ )
                //{
                //    pGraphicsItem = arpGraphicsItemsSelected[idxGraphObj];

                //    // Only copy top level parent objects ..
                //    if( pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr )
                //    {
                //        pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

                //        if( pGraphObj != nullptr && pGraphObj->getType() != EGraphObjTypeConnectionLine )
                //        {
                //            pGraphObjClone = pGraphObj->clone();

                //            if( pGraphObjClone != nullptr )
                //            {
                //                // The name of the clone should end with " Copy <Nr>". If not ..
                //                strGraphObjName = pGraphObjClone->name();
                //                if( !strGraphObjName.endsWith(" Copy 1",Qt::CaseInsensitive) )
                //                {
                //                    strGraphObjName += " Copy 1";
                //                    pGraphObjClone->setName(strGraphObjName);
                //                }
                //                strGraphObjName = findUniqueGraphObjName(pGraphObjClone);
                //                pGraphObjClone->setName(strGraphObjName);

                //                strGraphObjId = pGraphObjClone->keyInTree();
                //                strGraphObjId = FindUniqueGraphObjId( m_dctpGraphObjsClipboard, strGraphObjId );
                //                pGraphObjClone->setKeyInTree(strGraphObjId);
                //                m_dctpGraphObjsClipboard[strGraphObjId] = pGraphObjClone;

                //                if( pGraphObjClone->getType() == EGraphObjTypeConnectionPoint )
                //                {
                //                    arpGraphObjsOrigin.append(pGraphObj);
                //                    arpGraphObjsClones.append(pGraphObjClone);
                //                }

                //            } // if( pGraphObjClone != nullptr )
                //        } // if( pGraphObj != nullptr && pGraphObj->getType() != EGraphObjTypeConnectionLine )
                //    } // if( pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr )
                //} // for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsSelected.size(); idxGraphObj++ )

                //for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsSelected.size(); idxGraphObj++ )
                //{
                //    pGraphicsItem = arpGraphicsItemsSelected[idxGraphObj];
                //    pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

                //    if( pGraphObj != nullptr && pGraphObj->getType() == EGraphObjTypeConnectionLine )
                //    {
                //        pGraphObjCnctLine = dynamic_cast<CGraphObjConnectionLine*>(pGraphObj);

                //        if( pGraphObjCnctLine != nullptr )
                //        {
                //            pGraphObjCnctPt1 = pGraphObjCnctLine->getConnectionPoint(ELinePoint::Start);
                //            pGraphObjCnctPt2 = pGraphObjCnctLine->getConnectionPoint(ELinePoint::End);
                //            pGraphObjCnctPt1Clone = nullptr;
                //            pGraphObjCnctPt2Clone = nullptr;

                //            // Look for the original connection points.
                //            for( idxGraphObjTmp = 0; idxGraphObjTmp < arpGraphObjsOrigin.size(); idxGraphObjTmp++ )
                //            {
                //                if( arpGraphObjsOrigin[idxGraphObjTmp] == pGraphObjCnctPt1 )
                //                {
                //                    pGraphObjCnctPt1Clone = dynamic_cast<CGraphObjConnectionPoint*>(arpGraphObjsClones[idxGraphObjTmp]);
                //                }
                //                if( arpGraphObjsOrigin[idxGraphObjTmp] == pGraphObjCnctPt2 )
                //                {
                //                    pGraphObjCnctPt2Clone = dynamic_cast<CGraphObjConnectionPoint*>(arpGraphObjsClones[idxGraphObjTmp]);
                //                }
                //                if( pGraphObjCnctPt1Clone != nullptr && pGraphObjCnctPt2Clone != nullptr )
                //                {
                //                    break;
                //                }
                //            }

                //            // The references to the connection points cannot be copied with the
                //            // connection lines as the connection points have also been cloned.
                //            // Instead the cloned connection line got to be connected to the clones
                //            // of the connection points 
                //            if( pGraphObjCnctPt1Clone != nullptr && pGraphObjCnctPt2Clone != nullptr )
                //            {
                //                pGraphObjCnctLineClone = dynamic_cast<CGraphObjConnectionLine*>(pGraphObj->clone());

                //                if( pGraphObjCnctLineClone != nullptr )
                //                {
                //                    pGraphObjCnctLineClone->setConnectionPoint(ELinePoint::Start,pGraphObjCnctPt1Clone);
                //                    pGraphObjCnctLineClone->setConnectionPoint(ELinePoint::End,pGraphObjCnctPt2Clone);

                //                    // The name of the clone should end with " Copy <Nr>". If not ..
                //                    strGraphObjName = pGraphObjCnctLineClone->name();
                //                    if( !strGraphObjName.endsWith(" Copy 1",Qt::CaseInsensitive) )
                //                    {
                //                        strGraphObjName += " Copy 1";
                //                        pGraphObjCnctLineClone->setName(strGraphObjName);
                //                    }
                //                    strGraphObjName = findUniqueGraphObjName(pGraphObjCnctLineClone);
                //                    pGraphObjCnctLineClone->setName(strGraphObjName);

                //                    strGraphObjId = pGraphObjCnctLineClone->keyInTree();
                //                    strGraphObjId = FindUniqueGraphObjId( m_dctpGraphObjsClipboard, strGraphObjId );
                //                    pGraphObjCnctLineClone->setKeyInTree(strGraphObjId);
                //                    m_dctpGraphObjsClipboard[strGraphObjId] = pGraphObjCnctLineClone;
                //                }
                //            } // if( pGraphObjCnctPt1Clone != nullptr && pGraphObjCnctPt2Clone != nullptr )
                //        } // if( pGraphObjCnctLine != nullptr )
                //    } // if( pGraphObj != nullptr && pGraphObj->getType() == EGraphObjTypeConnectionLine )
                //} // for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsSelected.size(); idxGraphObj++ )
            }
        }

        // Pressed key to insert the objects from the (internal) clipboard ...
        else if (i_pEv->key() == Qt::Key_V) {
            if (i_pEv->modifiers() == Qt::ControlModifier) {
                //CGraphObj*                pGraphObjClone;
                //QGraphicsItem*            pGraphicsItemClone;
                //CGraphObjConnectionLine*  pGraphObjCnctLine;
                //CGraphObjConnectionLine*  pGraphObjCnctLineClone;
                //CGraphObjConnectionPoint* pGraphObjCnctPt1;
                //CGraphObjConnectionPoint* pGraphObjCnctPt2;
                //CGraphObjConnectionPoint* pGraphObjCnctPt1Clone;
                //CGraphObjConnectionPoint* pGraphObjCnctPt2Clone;
                //QString                   strGraphObjName;
                //QString                   strGraphObjId;
                //QList<CGraphObj*>         arpGraphObjsOrigin;
                //QList<CGraphObj*>         arpGraphObjsClones;
                //int                       idxGraphObjTmp;
                //QPointF                   ptPos;

                // Add objects from the internal clipboard to the drawing scene. Not only the references
                // have been copied but the objects have already been newly created (cloned).
                // We leave the clones in the clipboard as the objects may be copied several times.
                // But for this we have to clone the clone.
                // Concerning cloning group objects, connection points and their connection lines the
                // same applies as on cloning the objects to the clipboard (see "Key_C" above).

                //if( m_dctpGraphObjsClipboard.size() > 0 )
                //{
                //    clearSelection();

                //    QMap<QString,CGraphObj*>::iterator itGraphObjsClipboard = m_dctpGraphObjsClipboard.begin();

                //    while( itGraphObjsClipboard != m_dctpGraphObjsClipboard.end() )
                //    {
                //        pGraphObj = itGraphObjsClipboard.value();

                //        if( pGraphObj != nullptr && pGraphObj->getType() != EGraphObjTypeConnectionLine )
                //        {
                //            // We leave the clones in the clipboard as the objects may be copied several times.
                //            // But for this we have to clone the clone.
                //            pGraphObjClone = pGraphObj->clone();

                //            if( pGraphObjClone != nullptr )
                //            {
                //                strGraphObjName = pGraphObjClone->name();
                //                strGraphObjName = findUniqueGraphObjName(pGraphObjClone);
                //                pGraphObjClone->setName(strGraphObjName);

                //                strGraphObjId = pGraphObjClone->keyInTree();
                //                strGraphObjId = FindUniqueGraphObjId( m_dctpGraphObjs, strGraphObjId );
                //                pGraphObjClone->setKeyInTree(strGraphObjId);

                //                pGraphicsItemClone = dynamic_cast<QGraphicsItem*>(pGraphObjClone);

                //                if( pGraphicsItemClone == nullptr )
                //                {
                //                    throw CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphicsItem == nullptr" );
                //                }

                //                addGraphObj(pGraphObjClone);

                //                onGraphObjCreationFinished(pGraphObjClone);

                //                ptPos = pGraphicsItemClone->pos();

                //                ptPos.setX( ptPos.x() + 10 );
                //                ptPos.setY( ptPos.y() + 10 );

                //                pGraphicsItemClone->setPos(ptPos);
                //                pGraphicsItemClone->setSelected(true);

                //                if( pGraphicsItemClone->childItems().size() > 0 )
                //                {
                //                    // Rekursively add child items to the drawings scene's list of graphics items.
                //                    addChildItems(pGraphicsItemClone);
                //                }

                //                if( pGraphObjClone->getType() == EGraphObjTypeConnectionPoint )
                //                {
                //                    arpGraphObjsOrigin.append(pGraphObj);
                //                    arpGraphObjsClones.append(pGraphObjClone);
                //                }

                //            } // if( pGraphObjClone != nullptr )
                //        } // if( pGraphObj != nullptr && pGraphObj->getType() != EGraphObjTypeConnectionLine )

                //        ++itGraphObjsClipboard;

                //    } // while( itGraphObjsClipboard != m_dctpGraphObjsClipboard.end() )

                //    itGraphObjsClipboard = m_dctpGraphObjsClipboard.begin();

                //    while( itGraphObjsClipboard != m_dctpGraphObjsClipboard.end() )
                //    {
                //        pGraphObj = itGraphObjsClipboard.value();

                //        if( pGraphObj != nullptr && pGraphObj->getType() == EGraphObjTypeConnectionLine )
                //        {
                //            pGraphObjCnctLine = dynamic_cast<CGraphObjConnectionLine*>(pGraphObj);

                //            if( pGraphObjCnctLine != nullptr )
                //            {
                //                pGraphObjCnctPt1 = pGraphObjCnctLine->getConnectionPoint(ELinePoint::Start);
                //                pGraphObjCnctPt2 = pGraphObjCnctLine->getConnectionPoint(ELinePoint::End);
                //                pGraphObjCnctPt1Clone = nullptr;
                //                pGraphObjCnctPt2Clone = nullptr;

                //                // Look for the "original" connection points in the clipboard.
                //                for( idxGraphObjTmp = 0; idxGraphObjTmp < arpGraphObjsOrigin.size(); idxGraphObjTmp++ )
                //                {
                //                    if( arpGraphObjsOrigin[idxGraphObjTmp] == pGraphObjCnctPt1 )
                //                    {
                //                        pGraphObjCnctPt1Clone = dynamic_cast<CGraphObjConnectionPoint*>(arpGraphObjsClones[idxGraphObjTmp]);
                //                    }
                //                    if( arpGraphObjsOrigin[idxGraphObjTmp] == pGraphObjCnctPt2 )
                //                    {
                //                        pGraphObjCnctPt2Clone = dynamic_cast<CGraphObjConnectionPoint*>(arpGraphObjsClones[idxGraphObjTmp]);
                //                    }
                //                    if( pGraphObjCnctPt1Clone != nullptr && pGraphObjCnctPt2Clone != nullptr )
                //                    {
                //                        break;
                //                    }
                //                }

                //                // The references to the connection points cannot be copied with the
                //                // connection lines as the connection points have also been cloned.
                //                // Instead the cloned connection line got to be connected to the clones
                //                // of the connection points 
                //                if( pGraphObjCnctPt1Clone != nullptr && pGraphObjCnctPt2Clone != nullptr )
                //                {
                //                    // We leave the clones in the clipboard as the objects may be copied several times.
                //                    // But for this we have to clone the clone.
                //                    pGraphObjClone = pGraphObj->clone();
                //                    pGraphObjCnctLineClone = dynamic_cast<CGraphObjConnectionLine*>(pGraphObjClone);

                //                    if( pGraphObjCnctLineClone != nullptr )
                //                    {
                //                        pGraphObjCnctLineClone->setConnectionPoint(ELinePoint::Start,pGraphObjCnctPt1Clone);
                //                        pGraphObjCnctLineClone->setConnectionPoint(ELinePoint::End,pGraphObjCnctPt2Clone);

                //                        strGraphObjName = pGraphObjCnctLineClone->name();
                //                        strGraphObjName = findUniqueGraphObjName(pGraphObjCnctLineClone);
                //                        pGraphObjCnctLineClone->setName(strGraphObjName);

                //                        strGraphObjId = pGraphObjCnctLineClone->keyInTree();
                //                        strGraphObjId = FindUniqueGraphObjId( m_dctpGraphObjsClipboard, strGraphObjId );
                //                        pGraphObjCnctLineClone->setKeyInTree(strGraphObjId);

                //                        pGraphicsItemClone = dynamic_cast<QGraphicsItem*>(pGraphObjCnctLineClone);

                //                        if( pGraphicsItemClone == nullptr )
                //                        {
                //                            throw CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphicsItem == nullptr" );
                //                        }

                //                        addGraphObj(pGraphObjClone);

                //                        onGraphObjCreationFinished(pGraphObjCnctLineClone);

                //                        // Please note that the position for connection lines is defined by the connection points
                //                        // and that connection lines never have children (calling "addChildItems" is not necessary).
                //                        pGraphicsItemClone->setSelected(true);

                //                    } // if( pGraphObjCnctLineClone != nullptr )
                //                } // if( pGraphObjCnctPt1Clone != nullptr && pGraphObjCnctPt2Clone != nullptr )
                //            } // if( pGraphObjClone != nullptr )
                //        } // if( pGraphObj != nullptr && pGraphObj->getType() != EGraphObjTypeConnectionLine )

                //        ++itGraphObjsClipboard;

                //    } // while( itGraphObjsClipboard != m_dctpGraphObjsClipboard.end() )
                //} // if( m_dctpGraphObjsClipboard.size() > 0 )
            }
        }
    } // if( !i_pEv->isAccepted() )

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Leave, "States");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
} // keyPressEvent

//------------------------------------------------------------------------------
/*! @brief Handles the key release event.
*/
void CDrawingScene::keyReleaseEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjKeyEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qKeyEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjKeyEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "keyReleaseEvent",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Enter, "States");
    }

    QGraphicsScene::keyReleaseEvent(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Leave, "States");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
} // keyReleaseEvent

/*==============================================================================
protected: // overridables of base class QGraphicsScene
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::drawBackground( QPainter* i_pPainter, const QRectF& i_rect )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjPaint, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qRect2Str(i_rect);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaint,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "drawBackground",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsScene::drawBackground(i_pPainter, i_rect);

    QRectF rctScene = sceneRect();
    //double xLeft = rctScene.left();
    //double yTop = rctScene.top();
    //double xRight = rctScene.right();
    //double yBottom = rctScene.bottom();

    //static int s_iCallCount = 0;
    //if (s_iCallCount == 0) {
    //    QLineF lineTop(xLeft, yTop, xRight, yTop);
    //    QLineF lineRight(xRight, yTop, xRight, yBottom);
    //    QLineF lineBottom(xRight, yBottom, xLeft, yBottom);
    //    QLineF lineLeft(xLeft, yBottom, xLeft, yTop);
    //    QGraphicsScene::addLine(lineTop, QPen(Qt::red));
    //    QGraphicsScene::addLine(lineRight, QPen(Qt::red));
    //    QGraphicsScene::addLine(lineBottom, QPen(Qt::red));
    //    QGraphicsScene::addLine(lineLeft, QPen(Qt::red));
    //}
    //s_iCallCount++;

    i_pPainter->save();

    #pragma message(__TODO__"Background color as changable property")

    //rctScene = QRectF(rctScene.x() + 5, rctScene.y() + 5, rctScene.width()-10, rctScene.height()-10);
    //i_pPainter->setPen(Qt::NoPen);
    //i_pPainter->setBrush(Qt::yellow);
    //i_pPainter->setBrush(Qt::white);
    //i_pPainter->drawRect(rctScene);

    if (m_gridSettings.areLinesVisible() || m_gridSettings.areLabelsVisible()) {
        if (m_gridSettings.areLinesVisible()) {
            drawGridLines(i_pPainter, i_rect);
        }
    }
    if (m_arpPhysValShapes.size() > 0) {
        drawPhysValShapes(i_pPainter, i_rect);
    }
    i_pPainter->restore();
}

//------------------------------------------------------------------------------
void CDrawingScene::drawForeground( QPainter* i_pPainter, const QRectF& i_rect )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjPaint, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qRect2Str(i_rect);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaint,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "drawForeground",
        /* strAddInfo   */ strMthInArgs );

    //i_pPainter->setClipRect(i_rect);

    QGraphicsScene::drawForeground(i_pPainter, i_rect);
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief This slot is called if the screen resolution in pixels/mm has been changed.

    The metrics image size of the drawing size need to be updated.
    The method emits the drawingSizeChanged signal.
*/
void CDrawingScene::onDrawUnitsScreenResolutionInPxPerMMChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawUnitsScreenResolutionInPxPerMMChanged",
        /* strAddInfo   */ "" );

    CDrawingSize drawingSize = m_drawingSize;
    drawingSize.setScreenResolutionInPxPerMM(Units.Length.screenResolutionInPxPerMM());
    setDrawingSize(drawingSize);
}

//------------------------------------------------------------------------------
/*! @brief This slot is called if the screen resolution in pixels/mm has been changed.

    The metrics image size of the drawing size need to be updated.
    The method emits the drawingSizeChanged signal.
*/
void CDrawingScene::onDrawUnitsScaleFactorChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawUnitsScaleFactorChanged",
        /* strAddInfo   */ "" );

    CDrawingSize drawingSize = m_drawingSize;
    drawingSize.setScaleFactor(Units.Length.scaleFactorDividend(), Units.Length.scaleFactorDivisor());
    setDrawingSize(drawingSize);
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
/*! This slot is called if the currently selected drawing tool (the active
    object factory) is destroyed.

    During system shutdown or on unloading a library with user defined object
    factories this might be essential to avoid accessing a deleted factory instanz.

    @param i_pObjFactoy [in]
        Pointer to destroyed object factory.
*/
void CDrawingScene::onGraphObjFactoryDestroyed(QObject* i_pObjFactory)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pObjFactory == nullptr ? "nullptr" : i_pObjFactory->objectName());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onGraphObjFactoryDestroyed",
        /* strAddInfo   */ strMthInArgs );

    if( m_pObjFactory == i_pObjFactory )
    {
        m_pObjFactory = nullptr;
    }
}

//------------------------------------------------------------------------------
void CDrawingScene::onGraphObjAboutToBeDestroyed(CGraphObj* i_pGraphObj)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pGraphObj->keyInTree();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onGraphObjAboutToBeDestroyed",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(i_pGraphObj);
    if (pGraphicsItem != nullptr && pGraphicsItem->scene() != nullptr) {
        removeItem(pGraphicsItem);
    }

    // The children will be recursively deleted by the CGraphObj destructor.
    /*
    QList<QGraphicsItem*> arpGraphicsItemsChilds = pGraphicsItem->childItems();
    QStringList strlstGraphObjIdsChilds;

    // On deleting a graphical object the object may destroy other graphical objects
    // (e.g. connection points may delete connection lines) and the list of selected
    // items may change while the dtor is in process. So we don't store a list with object
    // references but a list with object ids and check whether the object still belong
    // to the drawing scene before deleting the object.

    for (int idxChild = 0; idxChild < arpGraphicsItemsChilds.size(); idxChild++) {
        QGraphicsItem* pGraphicsItemChild = arpGraphicsItemsChilds[idxChild];
        CGraphObj* pGraphObjChild = dynamic_cast<CGraphObj*>(pGraphicsItemChild);
        if (pGraphObjChild != nullptr) {
            strlstGraphObjIdsChilds.append(pGraphObjChild->keyInTree());
        }
    }
    for (int idxChild = 0; idxChild < strlstGraphObjIdsChilds.size(); idxChild++) {
        QString strGraphObjIdChild = strlstGraphObjIdsChilds[idxChild];
        CGraphObj* pGraphObjChild = findGraphObj(strGraphObjIdChild);
        if (pGraphObjChild != nullptr) {
            deleteItem(pGraphObjChild);
            pGraphObjChild = nullptr;
        }
    }
    */
}

//------------------------------------------------------------------------------
void CDrawingScene::onGraphObjEditModeChanged(
    CGraphObj* i_pGraphObj, const CEnumEditMode& i_eModeCurr, const CEnumEditMode& i_eModePrev)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pGraphObj->keyInTree() + ", Curr: " + i_eModeCurr.toString() + ", Prev: " + i_eModePrev.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onGraphObjEditModeChanged",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Enter, "States");
    }

    if (m_pGraphObjUnderConstruction != nullptr && m_pGraphObjUnderConstruction == i_pGraphObj) {
        // Object creation has been finished. Unset the current drawing tool and select
        // the currently created object.
        if (i_eModePrev == EEditMode::CreatingByMouseEvents) {
            if (m_pGraphObjUnderConstruction->type() == EGraphObjTypeConnectionLine) {
            }
            else {
                dynamic_cast<QGraphicsItem*>(m_pGraphObjUnderConstruction)->setSelected(true);
            }
            QObject::disconnect(
                m_pGraphObjUnderConstruction, &CGraphObj::editModeChanged,
                this, &CDrawingScene::onGraphObjEditModeChanged );
            m_pGraphObjUnderConstruction = nullptr;
            setCurrentDrawingTool(nullptr);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Leave, "States");
    }
}

/*==============================================================================
protected: // auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Forwards (sends) the event to the given graphics item.

    Before sending the event to the graphics item the scene pos and last scene pos
    are mapped to the local coordinate system of the item.
*/
//void CDrawingScene::forwardMouseEvent(QGraphicsItem* i_pGraphicsItem, QGraphicsSceneMouseEvent* i_pEv)
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
//        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);
//        strMthInArgs = pGraphObj->keyInTree() + ", {" + qGraphicsSceneMouseEvent2Str(i_pEv) + "}";
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod    */ "forwardMouseEvent",
//        /* strAddInfo   */ strMthInArgs );
//
//    QGraphicsSceneMouseEvent ev(i_pEv->type());
//
//    QPointF ptMouseItemPos = i_pGraphicsItem->mapFromScene(i_pEv->scenePos());
//    QPointF ptMouseItemPosLast = i_pGraphicsItem->mapFromScene(i_pEv->lastScenePos());
//
//    ev.setWidget(i_pEv->widget());
//    ev.setPos(ptMouseItemPos);
//    ev.setLastPos(ptMouseItemPosLast);
//    ev.setScenePos(i_pEv->scenePos());
//    ev.setLastScenePos(i_pEv->lastScenePos());
//    ev.setScreenPos(i_pEv->screenPos());
//    ev.setLastScreenPos(i_pEv->lastScreenPos());
//    ev.setButton(i_pEv->button());
//    ev.setButtons(i_pEv->buttons());
//    ev.setModifiers(i_pEv->modifiers());
//
//    sendEvent(i_pGraphicsItem, &ev);
//}

//------------------------------------------------------------------------------
/*! @brief Forwards (sends, dispatches) the event to the objects which are hit
           by the current mouse cursor.

    Before forwarding the event the objects hit will be sorted so that the "top most"
    object will receive the event first and can decide whether the mouse event should
    be eaten or not.

    Some items may completely overlap (encircle) other objects. For example a big
    rectangle may completely enclose a smaller rectangle or the bounding rectangle
    of a polygon may enclose other objects.

    In this case Qt's graphic scene does not dispatch the mouse events as hover events
    to the enclosed objects if they are not "on top" (according to the scene's item
    list Z order) of the enclosing items. If the outer object does not hide the inner
    object (fill style solid pattern) this behavior is not desired and what the user
    would expect. E.g. if a rectangle with "FillStyle = NoFill" would enclose another
    object the user would expect that the inner object can be selected by mouse clicks.
    And if you consider polygons which are never "filled" objects the inner objects
    should always be selectable by mouse clicks. The polygon as the outer object
    should only be selected if one of its line segments would be hit.
*/
//void CDrawingScene::forwardMouseEventToObjectsHit(QGraphicsSceneMouseEvent* i_pEv)
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = "{" + qGraphicsSceneMouseEvent2Str(i_pEv) + "}";
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod    */ "forwardMouseEventToObjectsHit",
//        /* strAddInfo   */ strMthInArgs );
//
//    QRectF rctToCheck(
//        /* x      */ i_pEv->scenePos().x() - m_fHitTolerance_px,
//        /* y      */ i_pEv->scenePos().y() - m_fHitTolerance_px,
//        /* width  */ 2.0 * m_fHitTolerance_px,
//        /* height */ 2.0 * m_fHitTolerance_px);
//    QList<QGraphicsItem*> arpGraphicsItemsIntersected =
//        items(rctToCheck, Qt::IntersectsItemShape, Qt::AscendingOrder);
//    QList<QGraphicsItem*> arpGraphicsItemsHit;
//    if (arpGraphicsItemsIntersected.size() > 0) {
//        for (int idxGraphObj = 0; idxGraphObj < arpGraphicsItemsIntersected.size(); idxGraphObj++) {
//            QGraphicsItem* pGraphicsItem = arpGraphicsItemsIntersected[idxGraphObj];
//            CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);
//            if (pGraphicsItem != nullptr && pGraphObj != nullptr) {
//                QPointF ptMouseItemPos = pGraphicsItem->mapFromScene(i_pEv->scenePos());
//                // Selected graphic items are considered as being hit if it has been clicked
//                // inside their bounding rectangle. But if selected items overlap other items
//                // whose shape edge points or bounding rectangles have been clicked the mouse
//                // events should be passed through to those objects.
//                bool bGraphObjHit = pGraphObj->isHit(ptMouseItemPos, nullptr);
//                if (bGraphObjHit) {
//                    arpGraphicsItemsHit.append(pGraphicsItem);
//                }
//            }
//        }
//    }
//
//    /*  Temporarily bring all objects to front which have been hit so that
//        Qt dispatches the mouse press events to those objects.
//        We use a double nested loop to achieve the following order:
//
//        1. Not selected objects
//            1.1. Graphics items which are not groups, connection points, labels or selection points.
//            1.2. Selection Points of Graphics Items
//            1.3. Labels of Graphics Items
//            1.4. Connection Points
//            1.5. Groups (only in view mode)
//        2. Selected objects
//            1.1. Graphics items which are not groups, connection points, labels or selection points.
//            2.2. Selection Points of Graphics Items
//            2.3. Labels of Graphics Items
//            2.4. Connection Points
//            2.5. Groups (only in view mode)
//
//        Items at the end of the list are "above" the objects listed before
//        and will receive the mouse events.
//        Please note that the second outer loop for selected objects will be
//        executed also in view mode even if unnecessary as in simulation
//        mode there will be no selected objects.
//    */
//
//    if (arpGraphicsItemsHit.size() > 0) {
//        // In first loop for all objects hit, check objects which are unselected.
//        // In second loop for all objects hit, check objects which are selected.
//        const QVector<bool> c_arbSelected = {false, true};
//        const QVector<EGraphObjType> c_arGraphObjTypes = {
//            EGraphObjTypeUndefined,
//            EGraphObjTypeSelectionPoint,
//            EGraphObjTypeLabel,
//            EGraphObjTypeConnectionPoint,
//            EGraphObjTypeGroup
//        };
//        for (bool bSelected : c_arbSelected) {
//            for (EGraphObjType graphObjType : c_arGraphObjTypes) {
//                for (QGraphicsItem* pGraphicsItemHit : arpGraphicsItemsHit) {
//                    bool bBringToFront = false;
//                    CGraphObj* pGraphObjHit = dynamic_cast<CGraphObj*>(pGraphicsItemHit);
//                    if (pGraphicsItemHit != nullptr && pGraphObjHit != nullptr) {
//                        if (bSelected == pGraphicsItemHit->isSelected()) {
//                            if (graphObjType == EGraphObjTypeUndefined) {
//                                if (!pGraphObjHit->isSelectionPoint() && !pGraphObjHit->isLabel()
//                                    && !pGraphObjHit->isConnectionPoint() && !pGraphObjHit->isGroup()) {
//                                    bBringToFront = true;
//                                }
//                            }
//                            else if (graphObjType == EGraphObjTypeLabel) {
//                                bBringToFront = pGraphObjHit->isLabel();
//                            }
//                            else {
//                                bBringToFront = (graphObjType == pGraphObjHit->type());
//                            }
//                        }
//                        if (bBringToFront) {
//                            bringToFront(pGraphicsItemHit, arpGraphicsItemsIntersected);
//                            pGraphicsItemHit->setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton|Qt::MiddleButton|Qt::XButton1|Qt::XButton2);
//                            // Append object hit by mouse cursor to list of objects temporarily brought to front.
//                            bool bGraphObjBroughtToFrontFound = false;
//                            for (QGraphicsItem* pGraphicsItem : m_arpGraphicsItemsBroughtToFront) {
//                                CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);
//                                // If already part of list ...
//                                if (pGraphObjHit == pGraphObj) {
//                                    bGraphObjBroughtToFrontFound = true;
//                                    break;
//                                }
//                            }
//                            // If not already part of list ...
//                            if (!bGraphObjBroughtToFrontFound) {
//                                m_arpGraphicsItemsBroughtToFront.append(pGraphicsItemHit);
//                            }
//                        }
//                    }
//                } // for (int idxGraphObj = arpGraphicsItemsHit.size()-1; idxGraphObj >= 0; idxGraphObj--)
//            } // for (int idxObjType = 0; idxObjType < c_iInnerLoopCount; idxObjType++)
//        } // for (int idxSel = 0; idxSel < c_iOuterLoopCount; idxSel++)
//    } // if( arpGraphicsItemsHit.size() > 0 )
//
//    // Dispatch mouse press event to objects which have been hit.
//    QGraphicsScene_mousePressEvent(i_pEv);
//
//    // After the mouse event has been dispatched the Z-order of the objects temporarily brought to front
//    // must be restored. While dispatching the mouse press event to the graphics scene objects some of the
//    // objects may be destroyed (e.g. selection points or labels).
//    for (QGraphicsItem* pGraphicsItem : m_arpGraphicsItemsBroughtToFront) {
//        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);
//        if (pGraphObj != nullptr) {
//            pGraphObj->resetStackingOrderValueToOriginalValue();
//        }
//    }
//    m_arpGraphicsItemsBroughtToFront.clear();
//
//    // In edit mode ..
//    if (m_mode == EMode::Edit) {
//        // If no item has been hit ...
//        if (selectedItems().size() == 0) {
//            // .. create selection rectangle area if not yet existing.
//            //setMode(EMode::Undefined, EEditTool::Undefined, EEditMode::None, EEditResizeMode::None, false);
//            QRectF rctSelectionArea(
//                /* x      */ i_pEv->scenePos().x(),
//                /* y      */ i_pEv->scenePos().y(),
//                /* width  */ 1,
//                /* height */ 1 );
//            if (m_pGraphicsItemSelectionArea == nullptr) {
//                QPen penSelectionArea(Qt::DotLine);
//                penSelectionArea.setColor(Qt::black);
//                QBrush brsSelectionArea(Qt::SolidPattern);
//                int iHue   = 240; // 0..359
//                int iSat   =  63; // 0..255
//                int iVal   = 127; // 0..255
//                int iAlpha =  63; // 0..255
//                brsSelectionArea.setColor(QColor::fromHsv(iHue, iSat, iVal, iAlpha));
//                m_pGraphicsItemSelectionArea = addRect(
//                    /* rect   */ rctSelectionArea,
//                    /* pen    */ penSelectionArea,
//                    /* brush  */ brsSelectionArea );
//            }
//        }
//    }
//}

//------------------------------------------------------------------------------
/*! @brief Removes the item and all its childs recursively from the list of 
           items accepting hover events.

    If a graphical object is destroyed the signal aboutToBeDestroyed is emitted
    and the drawing scene removes the graphical object from all members the
    object has been kept for internal processing. But selection points and
    labels are not added to the drawing scene and therefore the drawing scene
    will not be informed if labels or selection points are destroyed and removed
    from the graphics scene. But labels and selection points may have been added
    to the list of items accepting hover events.

    If a graphical object emits the aboutToBeDestroyed signal its selection points
    got to be removed from the list of items accepting hover events.
    This internal method sets the element of the given graphics item and all its
    child to nullptr. After invoking the method the list got to be cleared by
    removing all nullptr elements.

    @note Labels and selection points are not added as childs to the graphic item
          they belong to. They are only linked to its parent via the parent member.
*/
//void CDrawingScene::invalidateItemInAcceptingHoverEventsList(QGraphicsItem* i_pGraphicsItem)
////------------------------------------------------------------------------------
//{
//    for (int idxGraphObj = m_arpGraphicsItemsAcceptingHoverEvents.size()-1; idxGraphObj >= 0; idxGraphObj--) {
//        QGraphicsItem* pGraphicsItem = m_arpGraphicsItemsAcceptingHoverEvents[idxGraphObj];
//        if (pGraphicsItem != nullptr && (pGraphicsItem == i_pGraphicsItem || pGraphicsItem->parentItem() == i_pGraphicsItem)) {
//            m_arpGraphicsItemsAcceptingHoverEvents[idxGraphObj] = nullptr;
//        }
//    }
//}

//------------------------------------------------------------------------------
/*! @brief Removes the item and all its childs recursively from the list of 
           items temporarily brought to front.

    see invalidateItemInAcceptingHoverEventsList
*/
//void CDrawingScene::invalidateItemInBroughtToFrontList(QGraphicsItem* i_pGraphicsItem)
////------------------------------------------------------------------------------
//{
//    for (int idxGraphObj = m_arpGraphicsItemsBroughtToFront.size()-1; idxGraphObj >= 0; idxGraphObj--) {
//        QGraphicsItem* pGraphicsItem = m_arpGraphicsItemsBroughtToFront[idxGraphObj];
//        if (pGraphicsItem != nullptr && (pGraphicsItem == i_pGraphicsItem || pGraphicsItem->parentItem() == i_pGraphicsItem)) {
//            m_arpGraphicsItemsBroughtToFront[idxGraphObj] = nullptr;
//        }
//    }
//}

//------------------------------------------------------------------------------
void CDrawingScene::updateDivLinesMetrics()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateDivLinesMetrics",
        /* strAddInfo   */ "" );

    if (m_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        // Just a small note about pixel range and min and max values:
        // If you don't use a metric system like in drawings and define
        // a 500 pixel range, min is at 0, max is at 499. To have min
        // and max set to 0 and 500 a range of 501 pixels must be defined.
        // Pixel drawing: the origin is at the top left corner:
        // XScaleMin = XMin_px, XScaleMax = XMax_px
        // YScaleMin = XMin_px, YScaleMax = XMax_px
        // The greater the value, the greater the pixel coordinate on the screen.
        m_divLinesMetricsX.setUseWorldCoordinateTransformation(false);
        m_divLinesMetricsX.setScale(
            /* fScaleMinVal */ 0.0,
            /* fScaleMaxVal */ m_drawingSize.imageWidthInPixels() - 1,
            /* fScaleResVal */ m_drawingSize.imageCoorsResolution(Units.Length.px).getVal());
        // The Y scale direction is from top to bottom.
        m_divLinesMetricsY.setUseWorldCoordinateTransformation(false);
        m_divLinesMetricsY.setScale(
            /* fScaleMinVal */ 0.0,
            /* fScaleMaxVal */ m_drawingSize.imageHeightInPixels() - 1,
            /* fScaleResVal */ m_drawingSize.imageCoorsResolution(Units.Length.px).getVal());
        //m_divLinesMetricsY.setYScaleAxisOrientation(EYScaleAxisOrientation::TopDown);
    }
    else /*if (m_drawingSize.dimensionUnit() == EScaleDimensionUnit::Metric)*/ {
        // In order to draw division lines at min and max scale the width
        // in pixels got to be extended by one pixel when using metric scales
        // (see also documentation at class CScaleDivLines). This must have
        // been taken into account by the CDrawingSize class when calculating
        // the width and height of the image size in pixels.
        // Metric units:
        // Depending on the YScaleAxisOrientation the origin is either
        // at the top left or bottom left corner.
        // XScaleMin = XMin_px, XScaleMax = XMax_px
        // YScaleMin = XMax_px, YScaleMax = XMin_px
        // The greater the value, the less the pixel coordinate on the screen.
        m_divLinesMetricsX.setUseWorldCoordinateTransformation(true);
        m_divLinesMetricsX.setScale(
            /* fScaleMinVal */ 0.0,
            /* fScaleMaxVal */ m_drawingSize.metricImageWidth().getVal(),
            /* fScaleResVal */ m_drawingSize.imageCoorsResolution(m_drawingSize.unit()).getVal(),
            /* fMin_px      */ 0,
            /* fMax_px      */ m_drawingSize.imageWidthInPixels() - 1);
        m_divLinesMetricsY.setUseWorldCoordinateTransformation(true);
        if (m_drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
            m_divLinesMetricsY.setScale(
                /* fScaleMinVal */ 0.0,
                /* fScaleMaxVal */ m_drawingSize.metricImageHeight().getVal(),
                /* fScaleResVal */ m_drawingSize.imageCoorsResolution(m_drawingSize.unit()).getVal(),
                /* fMin_px      */ 0,
                /* fMax_px      */ m_drawingSize.imageHeightInPixels() - 1);
        }
        else {
            m_divLinesMetricsY.setScale(
                /* fScaleMinVal */ 0.0,
                /* fScaleMaxVal */ m_drawingSize.metricImageHeight().getVal(),
                /* fScaleResVal */ m_drawingSize.imageCoorsResolution(m_drawingSize.unit()).getVal(),
                /* fMin_px      */ m_drawingSize.imageHeightInPixels() - 1,
                /* fMax_px      */ 0);
        }
    }
    m_divLinesMetricsX.update();
    m_divLinesMetricsY.update();
}

//------------------------------------------------------------------------------
void CDrawingScene::drawGridLines(QPainter* i_pPainter, const QRectF& i_rect)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Rect {" + qRect2Str(i_rect) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaint,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "drawGridLines",
        /* strAddInfo   */ strMthInArgs );

    #pragma message(__TODO__"m_gridSettings.linesDistMinXInPixels()")
    #pragma message(__TODO__"m_gridSettings.linesDistMinYInPixels()")
    #pragma message(__TODO__"m_gridSettings.useDivLineDistValDecimalFactor25()")

    i_pPainter->save();
    QRectF rct = sceneRect();
    if (m_divLinesMetricsX.getDivLinesCount(EDivLineLayer::Main) > 0) {
        QPen pen(m_gridSettings.linesColor());
        pen.setStyle(lineStyle2QtPenStyle(m_gridSettings.linesStyle().enumerator()));
        pen.setWidth(m_gridSettings.linesWidth());
        i_pPainter->setPen(pen);
        for (int idxLine = 0; idxLine < m_divLinesMetricsX.getDivLinesCount(EDivLineLayer::Main); ++idxLine ) {
            int x = m_divLinesMetricsX.getDivLineInPix(EDivLineLayer::Main, idxLine);
            i_pPainter->drawLine(x, rct.top(), x, rct.bottom());
        }
    }
    if (m_divLinesMetricsY.getDivLinesCount(EDivLineLayer::Main) > 0) {
        QPen pen(m_gridSettings.linesColor());
        pen.setStyle(lineStyle2QtPenStyle(m_gridSettings.linesStyle().enumerator()));
        pen.setWidth(m_gridSettings.linesWidth());
        i_pPainter->setPen(pen);
        for (int idxLine = 0; idxLine < m_divLinesMetricsY.getDivLinesCount(EDivLineLayer::Main); ++idxLine ) {
            int y = m_divLinesMetricsY.getDivLineInPix(EDivLineLayer::Main, idxLine);
            i_pPainter->drawLine(rct.left(), y, rct.right(), y);
        }
    }
    i_pPainter->restore();
}

//------------------------------------------------------------------------------
void CDrawingScene::drawPhysValShapes(QPainter* i_pPainter, const QRectF& i_rect)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Rect {" + qRect2Str(i_rect) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaint,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "drawPhysValShapes",
        /* strAddInfo   */ strMthInArgs );

    for (int idxShape = 0; idxShape < m_arpPhysValShapes.size(); ++idxShape) {
        CPhysValShape* pPhysValShape = m_arpPhysValShapes[idxShape];
        if (pPhysValShape != nullptr) {
            pPhysValShape->draw(i_pPainter, i_rect, m_arDrawSettingsPhysValShapes[idxShape]);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the bounding rectangle in scene coordinates of the passed graphics items.

    Labels and selection points are not taken into account.
*/
QRectF CDrawingScene::getBoundingRect(const QList<QGraphicsItem*>& i_arpGraphicsItems) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "GraphicsItems [" + QString::number(i_arpGraphicsItems.size()) + "]";
        if (i_arpGraphicsItems.size() > 0) {
            strMthInArgs += "(";
            for (QGraphicsItem* pGraphicsItem : i_arpGraphicsItems) {
                CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);
                if (pGraphObj != nullptr) {
                    if (!strMthInArgs.endsWith("(")) strMthInArgs += ", ";
                    strMthInArgs += "{"+ pGraphObj->typeAsString() + ": "+ pGraphObj->path() + "}";
                }
            }
            strMthInArgs += ")";
        }
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "getBoundingRect",
        /* strAddInfo   */ strMthInArgs );

    QRectF rectBounding = boundingRect(i_arpGraphicsItems);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(qRect2Str(rectBounding));
    }
    return rectBounding;
}

//------------------------------------------------------------------------------
void CDrawingScene::unselectGraphicsItems(const QList<QGraphicsItem*>& i_arpGraphicsItems) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "GraphicsItems [" + QString::number(i_arpGraphicsItems.size()) + "]";
        if (i_arpGraphicsItems.size() > 0) {
            strMthInArgs += "(";
            for (QGraphicsItem* pGraphicsItem : i_arpGraphicsItems) {
                CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);
                if (pGraphObj != nullptr) {
                    if (!strMthInArgs.endsWith("(")) strMthInArgs += ", ";
                    strMthInArgs += "{"+ pGraphObj->typeAsString() + ": "+ pGraphObj->path() + "}";
                }
            }
            strMthInArgs += ")";
        }
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "unselectGraphicsItems",
        /* strAddInfo   */ strMthInArgs );

    for (QGraphicsItem* pGraphicsItemSelected : i_arpGraphicsItems) {
        pGraphicsItemSelected->setSelected(false);
    }
}

/*==============================================================================
protected: // auxiliary methods (trace emitting signals)
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::emit_aboutToBeCleared()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_aboutToBeCleared",
        /* strAddInfo   */ "" );

    emit aboutToBeCleared();
}

//------------------------------------------------------------------------------
void CDrawingScene::emit_drawingSizeChanged( const CDrawingSize& i_size )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_size.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_drawingSizeChanged",
        /* strAddInfo   */ strMthInArgs );

    emit drawingSizeChanged(i_size);
}

//------------------------------------------------------------------------------
void CDrawingScene::emit_gridSettingsChanged( const CDrawGridSettings& i_settings )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_settings.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_gridSettingsChanged",
        /* strAddInfo   */ strMthInArgs );

    emit gridSettingsChanged(i_settings);
}

//------------------------------------------------------------------------------
void CDrawingScene::emit_mousePosChanged( const QPointF& i_ptMousePos )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObjMouseMoveEvents, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = qPoint2Str(i_ptMousePos);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseMoveEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_mousePosChanged",
        /* strAddInfo   */ strMthInArgs );

    emit mousePosChanged(i_ptMousePos);
}

//------------------------------------------------------------------------------
void CDrawingScene::emit_modeChanged(const ZS::System::CEnumMode& i_eMode)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_eMode.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_modeChanged",
        /* strAddInfo   */ strMthInArgs );

    emit modeChanged(i_eMode);
}

//------------------------------------------------------------------------------
void CDrawingScene::emit_drawingToolChanged(
    const QString& i_strFactoryGrpName, const QString& i_strGraphObjType)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strFactoryGrpName + ", " + i_strGraphObjType;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_drawingToolChanged",
        /* strAddInfo   */ strMthInArgs );

    emit drawingToolChanged(i_strFactoryGrpName, i_strGraphObjType);
}

//------------------------------------------------------------------------------
void CDrawingScene::emit_drawSettingsChanged( const ZS::Draw::CDrawSettings& i_drawSettings )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        //strMthInArgs = i_drawSettings.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_drawSettingsChanged",
        /* strAddInfo   */ strMthInArgs );

    emit drawSettingsChanged(i_drawSettings);
}

/*==============================================================================
protected: // auxiliary instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::QGraphicsScene_mousePressEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "QGraphicsScene_mousePressEvent",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Enter, "States");
        traceItemsAtScenePos(mthTracer, EMethodDir::Enter, i_pEv->scenePos());
        traceAllItems(mthTracer, EMethodDir::Enter, "StackingOrder");
    }

    QGraphicsScene::mousePressEvent(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Leave, "States");
        traceItemsAtScenePos(mthTracer, EMethodDir::Leave, i_pEv->scenePos());
        traceAllItems(mthTracer, EMethodDir::Enter, "StackingOrder");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

//------------------------------------------------------------------------------
void CDrawingScene::QGraphicsScene_mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "QGraphicsScene_mouseReleaseEvent",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Enter, "States");
        //traceItemsAtScenePos(mthTracer, EMethodDir::Enter, i_pEv->scenePos());
    }

    QGraphicsScene::mouseReleaseEvent(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Leave, "States");
        //traceItemsAtScenePos(mthTracer, EMethodDir::Leave, i_pEv->scenePos());
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

//------------------------------------------------------------------------------
void CDrawingScene::QGraphicsScene_mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "QGraphicsScene_mouseDoubleClickEvent",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Enter, "States");
        //traceItemsAtScenePos(mthTracer, EMethodDir::Enter, i_pEv->scenePos());
    }

    QGraphicsScene::mouseDoubleClickEvent(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Leave, "States");
        //traceItemsAtScenePos(mthTracer, EMethodDir::Leave, i_pEv->scenePos());
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

//------------------------------------------------------------------------------
void CDrawingScene::QGraphicsScene_mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseMoveEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "QGraphicsScene_mouseMoveEvent",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Enter, "States");
        //traceItemsAtScenePos(mthTracer, EMethodDir::Enter, i_pEv->scenePos());
    }

    QGraphicsScene::mouseMoveEvent(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Leave, "States");
        //traceItemsAtScenePos(mthTracer, EMethodDir::Leave, i_pEv->scenePos());
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

/*==============================================================================
public: // auxiliary instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::traceInternalStates(
    CMethodTracer& i_mthTracer,
    EMethodDir i_mthDir,
    const QString& i_strWhat)
//------------------------------------------------------------------------------
{
    if (i_strWhat.isEmpty() || i_strWhat.contains("States", Qt::CaseInsensitive)) {
        QString strRuntimeInfo;
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = "   ";
        QGraphicsItem* pGraphicsItemMouseGrabber = mouseGrabberItem();
        CGraphObj* pGraphObjMouseGrabber = dynamic_cast<CGraphObj*>(pGraphicsItemMouseGrabber);
        QList<QGraphicsItem*> arpGraphicsItemsSelected = selectedItems();
        strRuntimeInfo += "Mode: " + m_mode.toString() +
            ", ObjFactory: " + QString(m_pObjFactory == nullptr ? "nullptr" : m_pObjFactory->path()) +
            ", UnderConstruction: " + QString(m_pGraphObjUnderConstruction == nullptr ? "nullptr" : m_pGraphObjUnderConstruction->path()) +
            ", HitTol: " + QString::number(m_fHitTolerance_px) + " px" +
            ", SelectArea {" + QString(m_pGraphicsItemSelectionArea == nullptr ? "null" : qRect2Str(m_pGraphicsItemSelectionArea->rect())) + "}" +
            ", MouseGrabber: " + QString(pGraphObjMouseGrabber == nullptr ? "null" : pGraphObjMouseGrabber->path()) +
            ", SelectedItems [" + QString::number(arpGraphicsItemsSelected.size()) + "]";
        if (!arpGraphicsItemsSelected.isEmpty()) {
            strRuntimeInfo += "(";
            for (QGraphicsItem* pGraphicsItemSelected : arpGraphicsItemsSelected) {
                CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItemSelected);
                if (pGraphObj != nullptr) {
                    if (!strRuntimeInfo.endsWith("(")) strRuntimeInfo += ", ";
                    strRuntimeInfo += pGraphObj->path();
                }
            }
            strRuntimeInfo += ")";
        }
        i_mthTracer.trace(strRuntimeInfo);
    }
    if (i_strWhat.isEmpty() || i_strWhat.contains("DrawingSize", Qt::CaseInsensitive)) {
        QString strRuntimeInfo;
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = "   ";
        strRuntimeInfo += "DrawingSize {" + m_drawingSize.toString() + "}";
        i_mthTracer.trace(strRuntimeInfo);
    }
    //if (i_strFilter.isEmpty() || i_strFilter.contains("DivLineMetrics", Qt::CaseInsensitive)) {
    //    QString strRuntimeInfo;
    //    if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
    //    else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
    //    else strRuntimeInfo = "   ";
    //    strRuntimeInfo += "DivLineMetrics {X {" + m_divLinesMetricsX.toString() + "}" +
    //        ", Y {" + m_divLinesMetricsY.toString() + "}}";
    //    i_mthTracer.trace(strRuntimeInfo);
    //}
    if (i_strWhat.isEmpty() || i_strWhat.contains("GridSettings", Qt::CaseInsensitive)) {
        QString strRuntimeInfo;
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = "   ";
        strRuntimeInfo += "GridSettings {" + m_gridSettings.toString() + "}";
        i_mthTracer.trace(strRuntimeInfo);
    }
    if (i_strWhat.isEmpty() || i_strWhat.contains("DrawSettings", Qt::CaseInsensitive)) {
        QString strRuntimeInfo;
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = "   ";
        strRuntimeInfo += "DrawSettings {" + m_drawSettings.toString() + "}";
        i_mthTracer.trace(strRuntimeInfo);
    }
}

//------------------------------------------------------------------------------
void CDrawingScene::traceItemsAtScenePos(
    ZS::System::CMethodTracer& i_mthTracer,
    ZS::System::EMethodDir i_mthDir,
    const QPointF& i_ptScenePos,
    const QString& i_strWhat)
//------------------------------------------------------------------------------
{
    QList<QGraphicsItem*> arpGraphicsItems = items(i_ptScenePos);
    QString strRuntimeInfo;
    if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
    else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
    else strRuntimeInfo = "   ";
    strRuntimeInfo += "QGraphicsScene::itemsAtScenePos: " + qPoint2Str(i_ptScenePos);
    i_mthTracer.trace(strRuntimeInfo);
    traceItems(i_mthTracer, i_mthDir, arpGraphicsItems, i_strWhat);
}

//------------------------------------------------------------------------------
void CDrawingScene::traceAllItems(
    ZS::System::CMethodTracer& i_mthTracer,
    ZS::System::EMethodDir i_mthDir,
    const QString& i_strWhat)
//------------------------------------------------------------------------------
{
    traceItems(i_mthTracer, i_mthDir, items(), i_strWhat);
}

//------------------------------------------------------------------------------
void CDrawingScene::traceItems(
    ZS::System::CMethodTracer& i_mthTracer,
    ZS::System::EMethodDir i_mthDir,
    QList<QGraphicsItem*> arpGraphicsItems,
    const QString& i_strWhat)
//------------------------------------------------------------------------------
{
    QString strRuntimeInfo;
    if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
    else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
    else strRuntimeInfo = "   ";
    strRuntimeInfo += "Items[" + QString::number(arpGraphicsItems.size()) + "]";
    i_mthTracer.trace(strRuntimeInfo);
    if (!arpGraphicsItems.isEmpty()) {
        for (QGraphicsItem* pGraphicsItem : arpGraphicsItems) {
            CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);
            if (pGraphObj != nullptr) {
                if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ . ";
                else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- . ";
                else strRuntimeInfo = "   . ";
                strRuntimeInfo += pGraphObj->typeAsString() + " " + pGraphObj->path() + " {";
                if (i_strWhat.isEmpty() || i_strWhat.contains("StackingOrder")) {
                    if (!strRuntimeInfo.endsWith("{")) strRuntimeInfo += ", ";
                    strRuntimeInfo += "ZVal: " + QString::number(pGraphicsItem->zValue());
                }
                strRuntimeInfo += "}";
                i_mthTracer.trace(strRuntimeInfo);
            }
        }
    }
}
