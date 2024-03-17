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
#include "ZSPhysVal/ZSPhysUnit.h"

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
ZSDRAWDLL_API bool isGraphicsSceneDragDropEvent( QEvent* i_pEv );
ZSDRAWDLL_API QString qGraphicsSceneDragDropEvent2Str( QGraphicsSceneDragDropEvent* i_pEv );
ZSDRAWDLL_API bool isGraphicsSceneHoverEvent( QEvent* i_pEv );
ZSDRAWDLL_API QString qGraphicsSceneHoverEvent2Str( QGraphicsSceneHoverEvent* i_pEv );
ZSDRAWDLL_API bool isGraphicsSceneMouseEvent( QEvent* i_pEv );
ZSDRAWDLL_API QString qGraphicsSceneMouseEvent2Str( QGraphicsSceneMouseEvent* i_pEv );
ZSDRAWDLL_API bool isGraphicsSceneResizeEvent( QEvent* i_pEv );
ZSDRAWDLL_API QString qGraphicsSceneResizeEvent2Str( QGraphicsSceneResizeEvent* i_pEv );

ZSDRAWDLL_API void getLineEndPolygons(
    const QLineF&        i_line,
    const CDrawSettings& i_drawSettings,
    QPolygonF*           i_pplgLineStart, // != nullptr to calculate it, may be resized
    QPolygonF*           i_pplgLineEnd ); // != nullptr to calculate it, may be resized

ZSDRAWDLL_API void getMaxRectArea(
    double  i_fX1Left,    double  i_fY1Top,
    double  i_fX1Right,   double  i_fY1Bottom,
    double  i_fX2Left,    double  i_fY2Top,
    double  i_fX2Right,   double  i_fY2Bottom,
    double& o_fXLeftMax,  double& o_fYTopMax,
    double& o_fXRightMax, double& o_fYBottomMax );

ZSDRAWDLL_API void getMaxRectArea(
    const ZS::PhysVal::CPhysVal& i_physVal1Left,    const ZS::PhysVal::CPhysVal& i_physVal1Top,
    const ZS::PhysVal::CPhysVal& i_physVal1Right,   const ZS::PhysVal::CPhysVal& i_physVal1Bottom,
    const ZS::PhysVal::CPhysVal& i_physVal2Left,    const ZS::PhysVal::CPhysVal& i_physVal2Top,
    const ZS::PhysVal::CPhysVal& i_physVal2Right,   const ZS::PhysVal::CPhysVal& i_physVal2Bottom,
    ZS::PhysVal::CPhysVal&       o_physValLeftMin,  ZS::PhysVal::CPhysVal&       o_physValTopMin,
    ZS::PhysVal::CPhysVal&       o_physValRightMax, ZS::PhysVal::CPhysVal&       o_physValBottomMax );

ZSDRAWDLL_API QRectF resizeRect(
    const QRectF&              i_rect,
    const CEnumSelectionPoint& i_selPt,
    const QPointF&             i_ptSel,
    CEnumSelectionPoint*       o_pSelPt = nullptr );

ZSDRAWDLL_API QPolygonF resizePolygon(
    const QPolygonF& i_plg,
    const QRectF&    i_rctBoundingNew,
    const QRectF&    i_rctBoundingOld = QRectF() );

ZSDRAWDLL_API QRectF boundingRect(const QPointF& i_pt, double i_fRadius);
ZSDRAWDLL_API QRectF boundingRect(const QLineF& i_line);
ZSDRAWDLL_API QRectF boundingRect(const QList<QGraphicsItem*>& i_arpGraphicsItems, QGraphicsItem* i_pGrapicsItemParentGroup = nullptr);
ZSDRAWDLL_API QRectF boundingRect(const QVector<CGraphObj*>& i_arpGraphObjs, CGraphObjGroup* i_pGraphObjGroup = nullptr);
ZSDRAWDLL_API QPolygonF line2Polygon(const QLineF& i_line);
ZSDRAWDLL_API QPolygonF rect2Polygon(const QRectF& i_rct, int i_iSelPtsCount = -1, const ESelectionPoint* i_pSelPts = nullptr);
ZSDRAWDLL_API bool isLineHit(const QLineF& i_line, const QPointF& i_pt, double i_fTolerance, SGraphObjHitInfo* o_pHitInfo);
ZSDRAWDLL_API bool isRectHit(const QRectF& i_rct, const CEnumFillStyle& i_fillStyle, const QPointF& i_pt, double i_fTolerance, SGraphObjHitInfo* o_pHitInfo);
ZSDRAWDLL_API bool isEllipseHit(const QRectF& i_rct, const CEnumFillStyle& i_fillStyle, const QPointF& i_pt, double i_fTolerance, SGraphObjHitInfo* o_pHitInfo);
ZSDRAWDLL_API bool isPolygonHit(const QPolygonF& i_rct, const CEnumFillStyle& i_fillStyle, const QPointF& i_pt, double i_fTolerance, SGraphObjHitInfo* o_pHitInfo);
ZSDRAWDLL_API double getDistance(const QPointF& i_pt, const QLineF& i_line);
ZSDRAWDLL_API double getAngleRad(const QPointF& i_pt1, const QPointF& i_pt2);
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
ZSDRAWDLL_API QLineF getPerpendicularLine(const QLineF& i_line, const QPointF& i_pt, double i_fMinLength_px = 0.0);

// Format: x/y (e.g. "2.3/4.5")
ZSDRAWDLL_API QString point2Str( const QPoint& i_pt );
ZSDRAWDLL_API QString point2Str( const QPointF& i_pt, char i_cF = 'f', int i_iPrecision = 1, bool i_bRound2Nearest = true );
ZSDRAWDLL_API QPoint  str2Point( const QString& i_str, bool* i_pbConverted );
ZSDRAWDLL_API QPointF str2PointF( const QString& i_str, bool* i_pbConverted );

// Format: w/h (e.g. "2.3/4.5")
ZSDRAWDLL_API QString size2Str( const QSize& i_size );
ZSDRAWDLL_API QString size2Str( const QSizeF& i_size, char i_cF = 'f', int i_iPrecision = 1, bool i_bRound2Nearest = true );
ZSDRAWDLL_API QSize   str2Size( const QString& i_str, bool* i_pbConverted );
ZSDRAWDLL_API QSizeF  str2SizeF( const QString& i_str, bool* i_pbConverted );

// Format: pt1, pt2 (e.g. "2.3/4.5, 5.6/7.8")
ZSDRAWDLL_API QString line2Str( const QLine& i_line );
ZSDRAWDLL_API QString line2Str( const QLineF& i_line, char i_cF = 'f', int i_iPrecision = 1, bool i_bRound2Nearest = true );
ZSDRAWDLL_API QLine   str2Line( const QString& i_str, bool* i_pbConverted );
ZSDRAWDLL_API QLineF  str2LineF( const QString& i_str, bool* i_pbConverted );

// Format: ptLT, ptRT, ptRB, ptLB[, w/h] (e.g. "2.3/4.5, 6.7/4.5, 6.7/8.9, 2.3/8.9")
ZSDRAWDLL_API QString rect2Str( const QRect& i_rct, bool i_bAddWidthAndHeight = false );
ZSDRAWDLL_API QString rect2Str( const QRectF& i_rct, char i_cF = 'f', int i_iPrecision = 1, bool i_bRound2Nearest = true, bool i_bAddWidthAndHeight = false );
ZSDRAWDLL_API QString rect2Str( const QPolygon& i_plgRect, bool i_bAddWidthAndHeight = false );
ZSDRAWDLL_API QString rect2Str( const QPolygonF& i_plgRect, char i_cF = 'f', int i_iPrecision = 1, bool i_bRound2Nearest = true, bool i_bAddWidthAndHeight = false );
ZSDRAWDLL_API QRect   str2Rect( const QString& i_str, bool* i_pbConverted );
ZSDRAWDLL_API QRectF  str2RectF( const QString& i_str, bool* i_pbConverted );

// Format: pt1, .., ptN (e.g. "2.3/4.5, 6.7/8.9, 0.1/2.3, 4.5/6.7")
ZSDRAWDLL_API QString   polygon2Str( const QPolygon& i_polygon );
ZSDRAWDLL_API QString   polygon2Str( const QPolygonF& i_polygon, char i_cF = 'f', int i_iPrecision = 1, bool i_bRound2Nearest = true );
ZSDRAWDLL_API QPolygon  str2Polygon( const QString& i_str, bool* i_pbConverted );
ZSDRAWDLL_API QPolygonF str2PolygonF( const QString& i_str, bool* i_pbConverted );

namespace XmlStreamParser
{
const QString c_strXmlElemNameDrawing = "Drawing";
const QString c_strXmlElemNameGraphObj = "GraphObj";
const QString c_strXmlElemNameGridSettings = "GridSettings";
const QString c_strXmlElemNameDrawSettings = "DrawSettings";
const QString c_strXmlElemNameGeometry = "Geometry";
const QString c_strXmlElemNameShapePoints = "ShapePoints";
const QString c_strXmlElemNameShapePointP1 = "P1";
const QString c_strXmlElemNameShapePointP2 = "P2";
const QString c_strXmlElemNameRectangle = "Rectangle";
const QString c_strXmlElemNameShapePointTopLeft = "TopLeft";
const QString c_strXmlElemNameSize = "Size";
const QString c_strXmlElemNameZValue = "ZValue";
const QString c_strXmlElemNameTextLabels = "TextLabels";
const QString c_strXmlElemNameGeometryLabels = "GeometryLabels";
const QString c_strXmlElemNameLabel = "Label";

const QString c_strXmlAttrGraphObjFactoryGroupName = "FactoryGroupName";
const QString c_strXmlAttrGraphObjType = "ObjectType";
const QString c_strXmlAttrGraphObjName = "ObjectName";
const QString c_strXmlAttrKey = "Key";
const QString c_strXmlAttrDimensionUnit = "DimensionUnit";
const QString c_strXmlAttrScreenResolutionPxPerMilliMeter = "ScreenResolutionPxPerMM";
const QString c_strXmlAttrMetricImageCoorsDecimals = "MetricImageCoorsDecimals";
const QString c_strXmlAttrUnit = "Unit";
const QString c_strXmlAttrWidth = "Width";
const QString c_strXmlAttrHeight = "Height";
const QString c_strXmlAttrScaleFactor = "ScaleFactor";
const QString c_strXmlAttrPaperSize = "PaperSize";
const QString c_strXmlAttrPaperOrientation = "PaperOrientation";
const QString c_strXmlAttrYScaleAxisOrientation = "YScaleAxisOrientation";
const QString c_strXmlAttrLinesVisible = "LinesVisible";
const QString c_strXmlAttrLinesDistMin = "LinesDistMin";
const QString c_strXmlAttrLabelsVisible = "LabelsVisible";
const QString c_strXmlAttrAnchorLineVisible = "AnchorLineVisible";
const QString c_strXmlAttrVisible = "Visible";
const QString c_strXmlAttrText = "Text";
const QString c_strXmlAttrSelPt = "SelPt";
const QString c_strXmlAttrDistance = "Distance";
const QString c_strXmlAttrPenColor = "PenColor";
const QString c_strXmlAttrPenWidth = "PenWidth";
const QString c_strXmlAttrFillColor = "FillColor";
const QString c_strXmlAttrFillStyle = "FillStyle";
const QString c_strXmlAttrLineStyle = "LineStyle";
const QString c_strXmlAttrLineWidth = "LineWidth";
const QString c_strXmlAttrLineColor = "LineColor";
const QString c_strXmlAttrFont = "Font";
const QString c_strXmlAttrLineRecordType = "LineRecordType";
const QString c_strXmlAttrLineExtent = "LineExtent";
const QString c_strXmlAttrLineP1EndStyle = "LineP1EndStyle";
const QString c_strXmlAttrLineP1ArrowHeadBaseLineType = "LineP1ArrowHeadBaseLineType";
const QString c_strXmlAttrLineP1ArrowHeadFillStyle = "LineP1ArrowHeadFillStyle";
const QString c_strXmlAttrLineP1ArrowHeadWidth = "LineP1ArrowHeadWidth";
const QString c_strXmlAttrLineP1ArrowHeadLength = "LineP1ArrowHeadLength";
const QString c_strXmlAttrLineP2EndStyle = "LineP2EndStyle";
const QString c_strXmlAttrLineP2ArrowHeadBaseLineType = "LineP2ArrowHeadBaseLineType";
const QString c_strXmlAttrLineP2ArrowHeadFillStyle = "LineP2ArrowHeadFillStyle";
const QString c_strXmlAttrLineP2ArrowHeadWidth = "LineP2ArrowHeadWidth";
const QString c_strXmlAttrLineP2ArrowHeadLength = "LineP2ArrowHeadLength";
const QString c_strXmlAttrTextSize = "TextSize";
const QString c_strXmlAttrTextColor = "TextColor";
const QString c_strXmlAttrTextStyle = "TextStyle";
const QString c_strXmlAttrTextEffect = "TextEffect";

ZSDRAWDLL_API void raiseErrorAttributeNotDefined(QXmlStreamReader& i_xmlStreamReader, const QString& i_strElemName, const QString& i_strAttrName);
ZSDRAWDLL_API void raiseErrorAttributeOutOfRange(QXmlStreamReader& i_xmlStreamReader, const QString& i_strElemName, const QString& i_strAttrName, const QString& i_strAttrVal);
ZSDRAWDLL_API CEnumScaleDimensionUnit getDimensionUnit(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, const CEnumScaleDimensionUnit& i_eDefaultVal = EScaleDimensionUnit::Pixels);
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
