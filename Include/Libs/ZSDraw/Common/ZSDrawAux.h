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

#include "ZSDraw/Common/ZSDrawDllMain.h"

#include <QtCore/qstring.h>
#include <QtCore/qrect.h>

#if QT_VERSION < 0x050000
#include <QtGui/qgraphicssceneevent.h>
#else
#include "QtWidgets/qgraphicssceneevent.h"
#endif

#include "ZSDraw/Common/ZSDrawCommon.h"

class QGraphicsItem;
class QLineF;
class QPainterPath;
class QPolygonF;
class QTransform;


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

namespace ZS
{
namespace PhysVal
{
class CPhysVal;
}
namespace Draw
{
class CDrawSettings;
struct SGraphObjHitInfo;


ZSDRAWDLL_API QString qPainterPathElemType2Str( int i_type );
ZSDRAWDLL_API QString qPainterPath2Str( QGraphicsItem* i_pGraphObj, const QPainterPath& i_painterPath );
ZSDRAWDLL_API QString qTransformationType2Str( int i_type );
ZSDRAWDLL_API QStringList qTransformation2StrList( const QTransform& i_transform );
ZSDRAWDLL_API QString qGraphicsItemChange2Str( int i_change, const QVariant& i_value, bool i_bIncludeChange = true );
ZSDRAWDLL_API QString qGraphicsItemFlags2Str( quint32 i_flags );
ZSDRAWDLL_API bool isGraphicsSceneHoverEvent( QEvent* i_pEv );
ZSDRAWDLL_API QString qGraphicsSceneHoverEvent2Str( QGraphicsSceneHoverEvent* i_pEv );
ZSDRAWDLL_API bool isGraphicsSceneMouseEvent( QEvent* i_pEv );
ZSDRAWDLL_API QString qGraphicsSceneMouseEvent2Str( QGraphicsSceneMouseEvent* i_pEv );


/*******************************************************************************
interface description of exported methods
*******************************************************************************/

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
    const QPolygonF& i_plg,                         // Resize from old bounding rect of this polygon
    const QRectF&    i_rctBoundingNew,              // to the new bounding rectangle.
    const QRectF&    i_rctBoundingOld = QRectF() ); // (optional if already known)

ZSDRAWDLL_API QRectF boundingRect( const QPointF& i_pt, double i_fRadius );
ZSDRAWDLL_API QPolygonF rect2Polygon( const QRectF& i_rct, int i_iSelPtsCount = -1, const ESelectionPoint* i_pSelPts = nullptr ); // if != nullptr must contain exactly 4 elements (for each corner)
ZSDRAWDLL_API bool isLineHit( const QLineF& i_lin, const QPointF& i_pt, double i_fTolerance );
ZSDRAWDLL_API bool isRectHit( const QRectF& i_rct, EFillStyle i_fillStyle, const QPointF& i_pt, double i_fTolerance, SGraphObjHitInfo* o_pHitInfo );
ZSDRAWDLL_API bool isEllipseHit( const QRectF& i_rct, EFillStyle i_fillStyle, const QPointF& i_pt, double i_fTolerance, SGraphObjHitInfo* o_pHitInfo );
ZSDRAWDLL_API bool isPolygonHit( const QPolygonF& i_rct, EFillStyle i_fillStyle, const QPointF& i_pt, double i_fTolerance, SGraphObjHitInfo* o_pHitInfo );
ZSDRAWDLL_API double getDist( const QPointF& i_pt1, const QPointF& i_pt2 );
ZSDRAWDLL_API double getAngleRad( const QPointF& i_pt1, const QPointF& i_pt2 );
ZSDRAWDLL_API QPointF rotatePoint( const QPointF& i_ptCenter, const QPointF& i_pt, double i_fAngle_rad );
ZSDRAWDLL_API QPolygonF rotateRect( const QPointF& i_ptCenter, const QRectF& i_rct, double i_fAngle_rad );
ZSDRAWDLL_API QPolygonF rotatePolygon( const QPointF& i_ptCenter, const QPolygonF& i_plg, double i_fAngle_rad );
ZSDRAWDLL_API QPolygonF normalizePolygon( const QPolygonF& i_plg, int i_iPrecision = 0 );
ZSDRAWDLL_API QPointF getCenterPoint( const QLineF& i_line ); // polygonRect must contain exactly 4 elements
ZSDRAWDLL_API QPointF getCenterPointOfPolygonRect( const QPolygonF& i_plgRect ); // plgRect must contain exactly 4 elements
ZSDRAWDLL_API QPointF getMassCenterPointOfPolygon( const QPolygonF& i_plg );
ZSDRAWDLL_API QPolygonF getBoundingRectPolygon( const QPolygonF& i_plg );
ZSDRAWDLL_API QPolygonF getEllipseFocusPoints( const QRectF& i_rct );
ZSDRAWDLL_API double getEllipseCenterFocusDist( const QRectF& i_rct );

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

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_Aux_h
