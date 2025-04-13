/*******************************************************************************

Copyright 2004 - 2023 by    ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSDraw_Aux_h
#define ZSDraw_Aux_h

#include "ZSDraw/Common/ZSDrawCommon.h"
#include "ZSDraw/Common/ZSDrawUnits.h"

#include <QtCore/qstring.h>
#include <QtCore/qrect.h>
#include <QtGui/qfont.h>


#if QT_VERSION < 0x050000
#include <QtGui/qgraphicsitem.h>
#include <QtGui/qgraphicssceneevent.h>
#else
#include <QtWidgets/qgraphicsitem.h>
#include "QtWidgets/qgraphicssceneevent.h"
#endif

class QGraphicsItem;
class QLineF;
class QPainterPath;
class QPolygonF;
class QTransform;
class QXmlStreamAttributes;
class QXmlStreamWriter;
class QXmlStreamReader;

namespace ZS
{
namespace PhysVal
{
class CPhysVal;
}
namespace Draw
{
class CDrawSettings;
class CGraphObj;
class CGraphObjGroup;
struct SGraphObjHitInfo;

ZSDRAWDLL_API QString qPainterPathElemType2Str( int i_type );
ZSDRAWDLL_API QString qPainterPath2Str( QGraphicsItem* i_pGraphObj, const QPainterPath& i_painterPath );
ZSDRAWDLL_API QString qTransformationType2Str( int i_type );
ZSDRAWDLL_API QStringList qTransformation2StrList( const QTransform& i_transform );
ZSDRAWDLL_API QString qGraphicsItemChange2Str( int i_change, const QVariant& i_value, bool i_bIncludeChange = true );
ZSDRAWDLL_API QString qGraphicsItemFlags2Str( const QGraphicsItem::GraphicsItemFlags& i_flags );
ZSDRAWDLL_API QString qGraphicsItemType2Str( int i_type );

ZSDRAWDLL_API bool isGraphicsSceneEvent( QEvent* i_pEv );
ZSDRAWDLL_API QString qGraphicsSceneEvent2Str( QGraphicsSceneEvent* i_pEv );
ZSDRAWDLL_API bool isGraphicsSceneDragDropEvent( QEvent* i_pEv );
ZSDRAWDLL_API QString qGraphicsSceneDragDropEvent2Str( QGraphicsSceneDragDropEvent* i_pEv );
ZSDRAWDLL_API bool isGraphicsSceneHoverEvent( QEvent* i_pEv );
ZSDRAWDLL_API QString qGraphicsSceneHoverEvent2Str( QGraphicsSceneHoverEvent* i_pEv );
ZSDRAWDLL_API bool isGraphicsSceneMouseEvent( QEvent* i_pEv );
ZSDRAWDLL_API QString qGraphicsSceneMouseEvent2Str( QGraphicsSceneMouseEvent* i_pEv );
ZSDRAWDLL_API bool isGraphicsSceneResizeEvent( QEvent* i_pEv );
ZSDRAWDLL_API QString qGraphicsSceneResizeEvent2Str( QGraphicsSceneResizeEvent* i_pEv );
ZSDRAWDLL_API bool isGraphicsSceneContextMenuEvent( QEvent* i_pEv );
ZSDRAWDLL_API QString qGraphicsSceneContextMenuEventReason2Str( QGraphicsSceneContextMenuEvent::Reason i_reason );
ZSDRAWDLL_API QString qGraphicsSceneContextMenuEvent2Str( QGraphicsSceneContextMenuEvent* i_pEv );

// QPolygon is defined in QtGui. If available in QtCore some methods should be moved to ZS::System::Math.

ZSDRAWDLL_API void getLineEndArrowPolygons(
    const QLineF& i_line,
    const CDrawSettings& i_drawSettings,
    QPolygonF* i_pplgLineStart, // != nullptr to calculate it, may be resized
    QPolygonF* i_pplgLineEnd ); // != nullptr to calculate it, may be resized

//ZSDRAWDLL_API QRectF resizeRect(
//    const QRectF& i_rect,
//    const CEnumSelectionPoint& i_selPt,
//    const QPointF& i_ptSel,
//    CEnumSelectionPoint* o_pSelPt = nullptr );

//ZSDRAWDLL_API QPolygonF resizePolygon(
//    const QPolygonF& i_plg,
//    const QRectF& i_rctBoundingNew,
//    const QRectF& i_rctBoundingOld = QRectF());

ZSDRAWDLL_API QRectF boundingRect(const QPointF& i_pt, double i_fRadius);
ZSDRAWDLL_API QRectF boundingRect(const QLineF& i_line);
ZSDRAWDLL_API QRectF boundingRect(const QPolygonF& i_polygon);
ZSDRAWDLL_API QRectF boundingRect(const QList<QGraphicsItem*>& i_arpGraphicsItems, QGraphicsItem* i_pGrapicsItemParentGroup = nullptr);
ZSDRAWDLL_API QRectF boundingRect(const QVector<CGraphObj*>& i_arpGraphObjs, CGraphObjGroup* i_pGraphObjGroup = nullptr);
ZSDRAWDLL_API QPolygonF line2Polygon(const QLineF& i_line);
ZSDRAWDLL_API QPolygonF rect2Polygon(const QRectF& i_rct, int i_iSelPtsCount = -1, const ESelectionPoint* i_pSelPts = nullptr);
ZSDRAWDLL_API bool isLineHit(const QLineF& i_line, const QPointF& i_pt, double i_fTolerance, SGraphObjHitInfo* o_pHitInfo);
ZSDRAWDLL_API bool isRectHit(const QRectF& i_rct, const CEnumFillStyle& i_fillStyle, const QPointF& i_pt, double i_fTolerance, SGraphObjHitInfo* o_pHitInfo);
ZSDRAWDLL_API bool isEllipseHit(const QRectF& i_rct, const CEnumFillStyle& i_fillStyle, const QPointF& i_pt, double i_fTolerance, SGraphObjHitInfo* o_pHitInfo);
ZSDRAWDLL_API bool isPolygonHit(const QPolygonF& i_plg, const CEnumFillStyle& i_fillStyle, const QPointF& i_pt, double i_fTolerance, SGraphObjHitInfo* o_pHitInfo);
ZSDRAWDLL_API double getDistance(const QPointF& i_pt, const QLineF& i_line);
ZSDRAWDLL_API double getRadius(const QSizeF& i_size);
ZSDRAWDLL_API QPointF getCornerPoint(const QPointF& i_ptCenter, const QSizeF& i_size, double i_fAngle_rad, ESelectionPoint i_selPt);
//ZSDRAWDLL_API double getAngleRad(const QSizeF& i_size, int* o_piQuadrant = nullptr);
//ZSDRAWDLL_API double getAngleDegree(const QSizeF& i_size, int* o_piQuadrant = nullptr);
ZSDRAWDLL_API double getAngleRad(const QPointF& i_pt1, const QPointF& i_pt2);
ZSDRAWDLL_API double getAngleDegree(const QPointF& i_pt1, const QPointF& i_pt2);
ZSDRAWDLL_API QPointF rotatePoint(const QPointF& i_ptCenter, const QPointF& i_pt, double i_fAngle_rad);
ZSDRAWDLL_API QPolygonF rotateRect(const QPointF& i_ptCenter, const QRectF& i_rct, double i_fAngle_rad);
ZSDRAWDLL_API QPolygonF rotatePolygon(const QPointF& i_ptCenter, const QPolygonF& i_plg, double i_fAngle_rad);
ZSDRAWDLL_API QPolygonF normalizePolygon(const QPolygonF& i_plg, int i_iPrecision = 0);
ZSDRAWDLL_API QPointF getCenterPointOfPolygonRect(const QPolygonF& i_plgRect);
ZSDRAWDLL_API QPointF getMassCenterPointOfPolygon(const QPolygonF& i_plg);
ZSDRAWDLL_API QPolygonF getBoundingRectPolygon(const QPolygonF& i_plg);
ZSDRAWDLL_API QPolygonF getEllipseFocusPoints(const QRectF& i_rct);
ZSDRAWDLL_API double getEllipseCenterFocusDist(const QRectF& i_rct);
ZSDRAWDLL_API QPointF getSelectionPointCoors(const QLineF& i_lin, ESelectionPoint i_selPt);
ZSDRAWDLL_API QPointF getSelectionPointCoors(const QRectF& i_rct, ESelectionPoint i_selPt);
ZSDRAWDLL_API QPointF getSelectionPointCoors(const QLineF& i_lin, const QRectF& i_rct, ESelectionPoint* o_pSelPt = nullptr);
ZSDRAWDLL_API QLineF getLineFromPolar(double i_fLength_px, double i_fAngle_degrees, const QLineF& i_line);
ZSDRAWDLL_API SPolarCoors getPolarCoors(const QLineF& i_line, const QPointF& i_pt);
ZSDRAWDLL_API QLineF getPerpendicularLine(const QLineF& i_line, const QPointF& i_pt, double i_fMinLength_px = 0.0, QPointF* o_pPtIntersection = nullptr);

//ZSDRAWDLL_API QString qRect2Str( const QPolygon& i_plgRect, const QString& i_strSeparator = ", " );
//ZSDRAWDLL_API QString qRect2Str( const QPolygonF& i_plgRect, const QString& i_strSeparator = ", ", char i_chFormat = 'f', int i_iPrecision = 1, bool i_bRound2Nearest = true );
ZSDRAWDLL_API QString qPolygon2Str( const QPolygon& i_polygon, const QString& i_strSeparator = ", " );
ZSDRAWDLL_API QString qPolygon2Str( const QPolygonF& i_polygon, const QString& i_strSeparator = ", ", char i_cF = 'f', int i_iPrecision = 1, bool i_bRound2Nearest = true );
//ZSDRAWDLL_API QPolygon str2QPolygon( const QString& i_str, bool* o_pbConverted, const QString& i_strSeparator = "," );
//ZSDRAWDLL_API QPolygonF str2QPolygonF( const QString& i_str, bool* o_pbConverted, const QString& i_strSeparator = "," );


namespace XmlStreamParser
{
const QString c_strXmlElemNameDrawing = "Drawing";
const QString c_strXmlElemNameGraphObj = "GraphObj";
const QString c_strXmlElemNameGridSettings = "GridSettings";
const QString c_strXmlElemNameDrawSettings = "DrawSettings";
const QString c_strXmlElemNameGeometry = "Geometry";
const QString c_strXmlElemNamePosition = "Position";
const QString c_strXmlElemNameShapePoints = "ShapePoints";
const QString c_strXmlElemNameShapePointP = "P";
const QString c_strXmlElemNameShapePointP1 = "P1";
const QString c_strXmlElemNameShapePointP2 = "P2";
const QString c_strXmlElemNameRectangle = "Rectangle";
const QString c_strXmlElemNameTopLeft = "TopLeft";
const QString c_strXmlElemNameTopRight = "TopRight";
const QString c_strXmlElemNameBottomRight = "BottomRight";
const QString c_strXmlElemNameBottomLeft = "BottomLeft";
const QString c_strXmlElemNameCenter = "Center";
const QString c_strXmlElemNameWidth = "Width";
const QString c_strXmlElemNameHeight = "Height";
const QString c_strXmlElemNameSize = "Size";
const QString c_strXmlElemNameAngle = "Angle";
const QString c_strXmlElemNameUnit = "Unit";
const QString c_strXmlElemNameZValue = "ZValue";
const QString c_strXmlElemNameTextLabels = "TextLabels";
const QString c_strXmlElemNameGeometryLabels = "GeometryLabels";
const QString c_strXmlElemNameLabel = "Label";

const QString c_strXmlElemNameGraphObjFactoryGroupName = "FactoryGroupName";
const QString c_strXmlElemNameGraphObjType = "ObjectType";
const QString c_strXmlElemNameGraphObjName = "ObjectName";
const QString c_strXmlElemNameKey = "Key";
const QString c_strXmlElemNameDimensionUnit = "DimensionUnit";
const QString c_strXmlElemNameScreenResolutionPxPerMilliMeter = "ScreenResolutionPxPerMM";
const QString c_strXmlElemNameMetricImageCoorsDecimals = "MetricImageCoorsDecimals";
const QString c_strXmlElemNameScaleFactor = "ScaleFactor";
const QString c_strXmlElemNamePaperSize = "PaperSize";
const QString c_strXmlElemNamePaperOrientation = "PaperOrientation";
const QString c_strXmlElemNameYScaleAxisOrientation = "YScaleAxisOrientation";
const QString c_strXmlElemNameLinesVisible = "LinesVisible";
const QString c_strXmlElemNameLinesDistMin = "LinesDistMin";
const QString c_strXmlElemNameLabelsVisible = "LabelsVisible";
const QString c_strXmlElemNameAnchorLineVisible = "AnchorLineVisible";
const QString c_strXmlElemNameVisible = "Visible";
const QString c_strXmlElemNameText = "Text";
const QString c_strXmlElemNameSelPt = "SelPt";
const QString c_strXmlElemNameDistance = "Distance";
const QString c_strXmlElemNamePenColor = "PenColor";
const QString c_strXmlElemNamePenWidth = "PenWidth";
const QString c_strXmlElemNameFillColor = "FillColor";
const QString c_strXmlElemNameFillStyle = "FillStyle";
const QString c_strXmlElemNameLineStyle = "LineStyle";
const QString c_strXmlElemNameLineWidth = "LineWidth";
const QString c_strXmlElemNameLineColor = "LineColor";
const QString c_strXmlElemNameFont = "Font";
const QString c_strXmlElemNameLineRecordType = "LineRecordType";
const QString c_strXmlElemNameLineExtent = "LineExtent";
const QString c_strXmlElemNameLineP1EndStyle = "LineP1EndStyle";
const QString c_strXmlElemNameLineP1ArrowHeadBaseLineType = "LineP1ArrowHeadBaseLineType";
const QString c_strXmlElemNameLineP1ArrowHeadFillStyle = "LineP1ArrowHeadFillStyle";
const QString c_strXmlElemNameLineP1ArrowHeadWidth = "LineP1ArrowHeadWidth";
const QString c_strXmlElemNameLineP1ArrowHeadLength = "LineP1ArrowHeadLength";
const QString c_strXmlElemNameLineP2EndStyle = "LineP2EndStyle";
const QString c_strXmlElemNameLineP2ArrowHeadBaseLineType = "LineP2ArrowHeadBaseLineType";
const QString c_strXmlElemNameLineP2ArrowHeadFillStyle = "LineP2ArrowHeadFillStyle";
const QString c_strXmlElemNameLineP2ArrowHeadWidth = "LineP2ArrowHeadWidth";
const QString c_strXmlElemNameLineP2ArrowHeadLength = "LineP2ArrowHeadLength";
const QString c_strXmlElemNameTextSize = "TextSize";
const QString c_strXmlElemNameTextColor = "TextColor";
const QString c_strXmlElemNameTextStyle = "TextStyle";
const QString c_strXmlElemNameTextEffect = "TextEffect";

ZSDRAWDLL_API void raiseErrorAttributeNotDefined(QXmlStreamReader& i_xmlStreamReader, const QString& i_strElemName, const QString& i_strAttrName);
ZSDRAWDLL_API void raiseErrorAttributeOutOfRange(QXmlStreamReader& i_xmlStreamReader, const QString& i_strElemName, const QString& i_strAttrName, const QString& i_strAttrVal);
ZSDRAWDLL_API CEnumScaleDimensionUnit getDimensionUnit(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, const CEnumScaleDimensionUnit& i_eDefaultVal = EScaleDimensionUnit::Pixels);
ZSDRAWDLL_API ZS::System::CEnumYScaleAxisOrientation getYScaleAxisOrientation(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, const ZS::System::CEnumYScaleAxisOrientation& i_eDefaultVal = ZS::System::EYScaleAxisOrientation::TopDown);
ZSDRAWDLL_API CEnumNormedPaperSize getNormedPaperSize(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, const CEnumNormedPaperSize& i_eDefaultVal = CEnumNormedPaperSize());
ZSDRAWDLL_API ZS::System::CEnumOrientation getOrientation(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, const ZS::System::CEnumOrientation& i_eDefaultVal = ZS::System::CEnumOrientation());
ZSDRAWDLL_API CEnumLineStyle getLineStyle(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, const CEnumLineStyle& i_eDefaultVal = ELineStyle::SolidLine);
ZSDRAWDLL_API ETextSize getTextSize(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, ETextSize i_eDefaultVal = ETextSize11);
ZSDRAWDLL_API CEnumTextStyle getTextStyle(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, const CEnumTextStyle& i_eDefaultVal = ETextStyle::Normal);
ZSDRAWDLL_API CEnumTextEffect getTextEffect(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, const CEnumTextEffect& i_eDefaultVal = ETextEffect::None);
ZSDRAWDLL_API ZS::PhysVal::CUnit getUnit(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, const ZS::PhysVal::CUnit& i_unitDefault = Units.Length.mm);
ZSDRAWDLL_API ZS::PhysVal::CPhysVal getPhysVal(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, const ZS::PhysVal::CPhysVal& i_physValDefault = ZS::PhysVal::CPhysVal(0.0, Units.Length.mm));
ZSDRAWDLL_API QFont getFont(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, const QFont& i_fntDefault = QFont());
ZSDRAWDLL_API QColor getColor(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, const QColor& i_clrDefault = Qt::black);
ZSDRAWDLL_API bool getBoolVal(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, bool i_bDefaultVal = false);
ZSDRAWDLL_API int getIntVal(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, int iValDefault = 0);
ZSDRAWDLL_API std::pair<int, int> getIntPair(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, const QString& i_strDelimiter = ":", bool i_bAttrIsMandatory = true, const std::pair<int, int>& i_valDefault = std::make_pair(0, 0));
ZSDRAWDLL_API double getDoubleVal(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, double fValDefault = 0.0);

} // namespace XmlStreamParser

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_Aux_h
