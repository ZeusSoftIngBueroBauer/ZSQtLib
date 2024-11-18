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

#ifndef ZSDraw_Common_h
#define ZSDraw_Common_h

#include "ZSDraw/Common/ZSDrawDllMain.h"
#include "ZSSys/ZSSysCommon.h"
#include "ZSSys/ZSSysEnumTemplate.h"

#include <QtGui/qcursor.h>
#if QT_VERSION < 0x050000
#include <QtGui/qgraphicsitem.h>
#else
#include <QtWidgets/qgraphicsitem.h>
#endif

#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4661 ) // The static arrays "CEnum<>::s_arEnumEntries" are defined in the cpp file.
#elif defined __linux__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
#endif

/*******************************************************************************
global type definitions and constants
*******************************************************************************/

namespace ZS {
namespace Draw {
class CGraphObj;
} }

// Note about CEnum template instantiation:
// gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope"
// if the for example the template definition is encapsulated in namespace ZS::System.
// To work around this gcc compile error message the enum classes and the typedef for the
// enum classes are encapsulated in namespace ZS::System but the temlate instantiation is
// done in global namespace.

namespace ZS {
namespace Draw {
//==============================================================================
/*! Normed paper sizes.
*/
enum class ENormedPaperSize {
//==============================================================================
    DinA0, /*!< 841 x 1189 mm. */
    DinA1, /*!< 549 x 841 mm. */
    DinA2, /*!< 420 x 594 mm. */
    DinA3, /*!< 297 x 420 mm. */
    DinA4, /*!< 210 x 297 mm. */
    DinA5, /*!< 148 x 210 mm. */
    DinA6, /*!< 105 x 148 mm. */
    DinA7, /*!< 74 x 105 mm. */
    DinA8, /*!< 52 x 74 mm. */
    DinA9, /*!< 37 x 52 mm. */
    DinA10 /*!< 26 x 37 mm. */
};

} }

// template is out of namespace as gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope."
template class ZSDRAWDLL_API ::CEnum<ZS::Draw::ENormedPaperSize>;
namespace ZS {
namespace Draw {
typedef ::CEnum<ENormedPaperSize> CEnumNormedPaperSize;
} }

namespace ZS {
namespace Draw {

const double c_fNormedPaperSizeWidthDinA0_mm  = 841.0;
const double c_fNormedPaperSizeHeightDinA0_mm = 1189.0;

const double c_fNormedPaperSizeWidthDinA1_mm  = 594;
const double c_fNormedPaperSizeHeightDinA1_mm = c_fNormedPaperSizeWidthDinA0_mm;

const double c_fNormedPaperSizeWidthDinA2_mm  = 420.0;
const double c_fNormedPaperSizeHeightDinA2_mm = c_fNormedPaperSizeWidthDinA1_mm;

const double c_fNormedPaperSizeWidthDinA3_mm  = 297.0;
const double c_fNormedPaperSizeHeightDinA3_mm = c_fNormedPaperSizeWidthDinA2_mm;

const double c_fNormedPaperSizeWidthDinA4_mm  = 210.0;
const double c_fNormedPaperSizeHeightDinA4_mm = c_fNormedPaperSizeWidthDinA3_mm;

const double c_fNormedPaperSizeWidthDinA5_mm  = 148.0;
const double c_fNormedPaperSizeHeightDinA5_mm = c_fNormedPaperSizeWidthDinA4_mm;

const double c_fNormedPaperSizeWidthDinA6_mm  = 105.0;
const double c_fNormedPaperSizeHeightDinA6_mm = c_fNormedPaperSizeWidthDinA5_mm;

const double c_fNormedPaperSizeWidthDinA7_mm  = 74.0;
const double c_fNormedPaperSizeHeightDinA7_mm = c_fNormedPaperSizeWidthDinA6_mm;

const double c_fNormedPaperSizeWidthDinA8_mm  = 52.0;
const double c_fNormedPaperSizeHeightDinA8_mm = c_fNormedPaperSizeWidthDinA7_mm;

const double c_fNormedPaperSizeWidthDinA9_mm  = 37.0;
const double c_fNormedPaperSizeHeightDinA9_mm = c_fNormedPaperSizeWidthDinA8_mm;

const double c_fNormedPaperSizeWidthDinA10_mm  = 26.0;
const double c_fNormedPaperSizeHeightDinA10_mm = c_fNormedPaperSizeWidthDinA9_mm;

//==============================================================================
struct ZSDRAWDLL_API SAttribute
//==============================================================================
{
public: // ctors
    SAttribute() :
        m_strName(),
        m_val(),
        m_bIsUsed(true)
    {
    }
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    SAttribute(const QString& i_strName, QVariant::Type i_type) :
    #else
    SAttribute(const QString& i_strName, QMetaType i_type) :
    #endif
        m_strName(i_strName),
        m_val(i_type),
        m_bIsUsed(true)
    {
    }
    SAttribute( const QString& i_strName, const QVariant& i_val, bool i_bIsUsed ) :
        m_strName(i_strName),
        m_val(i_val),
        m_bIsUsed(i_bIsUsed)
    {
    }
public: // operators
    SAttribute& operator = ( const QVariant& i_val )
    {
        m_val = i_val;
        return *this;
    }
    bool operator == ( const SAttribute& i_attrOther ) const
    {
        bool bEqual = true;
        if( m_strName != i_attrOther.m_strName || m_val != i_attrOther.m_val )
        {
            bEqual = false;
        }
        return bEqual;
    }
    bool operator != ( const SAttribute& i_attrOther ) const
    {
        return !(*this == i_attrOther);
    }
public: // struct members
    QString  m_strName;
    QVariant m_val;
    bool     m_bIsUsed;

}; // struct SAttribute

/*==============================================================================
Signatures for simulation event methods
==============================================================================*/

typedef void (*TFctMouseEvent)( void* i_pvThis, void* i_pvData, CGraphObj* i_pGraphObj, QGraphicsSceneMouseEvent* i_pEv );
typedef void (*TFctKeyEvent)( void* i_pvThis, void* i_pvData, CGraphObj* i_pGraphObj, QKeyEvent* i_pEv );

} } // ZS::Draw

namespace ZS {
namespace Draw {
//==============================================================================
/*! Paper source for the printer.
*/
enum class EPrinterPaperSource
//==============================================================================
{
    AutoSelect = 0,
    ManualFeed = 1,
    Tray1      = 2,
    Tray2      = 3
};
} }

template class ZSDRAWDLL_API ::CEnum<ZS::Draw::EPrinterPaperSource>;
namespace ZS {
namespace Draw {
typedef ::CEnum<EPrinterPaperSource> CEnumPrinterPaperSource;
} }


namespace ZS {
namespace Draw {
//==============================================================================
/*! Dimension unit for scales.
*/
enum class EScaleDimensionUnit {
//==============================================================================
    Pixels, /*!< Dimensions defined based on pixels. */
    Metric  /*!< Dimensions defined based on metric units.
                 The logical pixel positions got to be calculated depending
                 on the screen resolution given in Dots/Inch or Pixels/Inch. */
};
} }

template class ZSDRAWDLL_API ::CEnum<ZS::Draw::EScaleDimensionUnit>;
namespace ZS {
namespace Draw {
typedef ::CEnum<EScaleDimensionUnit> CEnumScaleDimensionUnit;
} }

namespace ZS {
namespace Draw {
//==============================================================================
/*! enum EGraphObjType

    Not CEnum as the enumerators don`t start with 0 and there will be user defined
    object types.
*/
//==============================================================================
typedef enum {
    EGraphObjTypeMin             = QGraphicsItem::UserType +  0,  // not a SVG element
    EGraphObjTypePoint           = QGraphicsItem::UserType +  0,  // not a SVG element
    EGraphObjTypeLine            = QGraphicsItem::UserType +  1,
    EGraphObjTypeRect            = QGraphicsItem::UserType +  2,
    EGraphObjTypeEllipse         = QGraphicsItem::UserType +  3,
    EGraphObjTypePolygon         = QGraphicsItem::UserType +  4,
    EGraphObjTypePolyline        = QGraphicsItem::UserType +  5,
    EGraphObjTypeText            = QGraphicsItem::UserType +  6,
    EGraphObjTypeImage           = QGraphicsItem::UserType +  7,
    EGraphObjTypeConnectionPoint = QGraphicsItem::UserType +  8,  // not a SVG element
    EGraphObjTypeConnectionLine  = QGraphicsItem::UserType +  9,  // not a SVG element
    EGraphObjTypeGroup           = QGraphicsItem::UserType + 10,  // not a SVG element
    EGraphObjTypeMax             = QGraphicsItem::UserType + 10,  // not a SVG element
    EGraphObjTypeCount           = (EGraphObjTypeMax - EGraphObjTypeMin + 1),
    EGraphObjTypeSelectionPoint,
    EGraphObjTypeLabelMin,
    EGraphObjTypeLabel = EGraphObjTypeLabelMin,
    EGraphObjTypeLabelGeometryPosition,
    EGraphObjTypeLabelGeometryDX,
    EGraphObjTypeLabelGeometryDY,
    EGraphObjTypeLabelGeometryLength,
    EGraphObjTypeLabelGeometryAngle,
    EGraphObjTypeLabelMax = EGraphObjTypeLabelGeometryAngle,
    EGraphObjTypeUserDefined,
    EGraphObjTypeUndefined = -1
}   EGraphObjType;

ZSDRAWDLL_API QString graphObjType2Str( int i_type );
ZSDRAWDLL_API EGraphObjType str2GraphObjType( const QString& i_str );
ZSDRAWDLL_API QString graphObjType2ClassName( int i_type );
ZSDRAWDLL_API EGraphObjType className2GraphObjType( const QString& i_str );
} }


//namespace ZS {
//namespace Draw {
////==============================================================================
///*! Defines the current edit tool.
//*/
//enum class EEditTool
////==============================================================================
//{
//    None          = 0, /*! Indicates that no edit tool is selected. */
//    Select        = 1, /*! Objects may be selected. */
//    CreateObjects = 2, /*! A valid object factory is selected and objects may be created. */
//    Undefined     = 3  /*! May be passed to methods to indicate that the argument should be ignored. */
//};
//} }
//
//template class ZSDRAWDLL_API ::CEnum<ZS::Draw::EEditTool>;
//namespace ZS {
//namespace Draw {
//typedef ::CEnum<EEditTool> CEnumEditTool;
//} }


namespace ZS {
namespace Draw {
//==============================================================================
/*! When formating the shape of an object using transformatons like scaling or
    or rotating the original coordinates need to be kept to avoid inaccuracies.
    E.g. when rotating an object around its center by 360 degrees the objects
    position and size should be the same as it would not have been rotated at all.
*/
enum class ECoordinatesVersion
//==============================================================================
{
    Original    = 0,    /*!< The coordinate has been explicitly set. */
    Transformed = 1     /*!< Current coordinates resulting when transforming the original coordinates. */
};
} }

template class ZSDRAWDLL_API ::CEnum<ZS::Draw::ECoordinatesVersion>;
namespace ZS {
namespace Draw {
typedef ::CEnum<ECoordinatesVersion> CEnumCoordinatesVersion;
} }


namespace ZS {
namespace Draw {
//==============================================================================
/*! Defines the current edit mode of the drawing scene and the graphics item.
*/
enum class EEditMode
//==============================================================================
{
    None                  = 0, /*!< No edit mode is selected. */
    CreatingByMouseEvents = 1  /*!< The object has been initially created by a mouse press event
                                    and is still under construction by followoing mouse events.
                                    For example:
                                    - Resizing a line by moving the line end point.
                                    - Resizing a rectangle by moving the bottom right corner.
                                    - Adding points to poly lines. */
    //Move           = 2, /*!< The selected objects are moved. */
    //Resize         = 3, /*!< The selected object is resized. */
    //Rotate         = 4, /*!< The selected object is rotated around the centre point of its bounding rectangle. */
    //MoveShapePoint = 5, /*!< A single shape point of the selected object is being moved. */
    //EditText       = 6, /*!< A single shape point of the selected object is being moved. */
    //Undefined      = 7  /*!< May be passed to methods to indicate that the argument should be ignored. */
};
} }

template class ZSDRAWDLL_API ::CEnum<ZS::Draw::EEditMode>;
namespace ZS {
namespace Draw {
typedef ::CEnum<EEditMode> CEnumEditMode;
} }

//namespace ZS {
//namespace Draw {
////==============================================================================
///*! When currently resizing an object (EditMode::Resize) the resize mode defines
//    in which direction the object is being resized.
//*/
//enum class EEditResizeMode
////==============================================================================
//{
//    None      = 0, /*!< No resize mode selected. */
//    ResizeAll = 1, /*!< The selected object is resized horizontally and vertically. */
//    ResizeHor = 2, /*!< The selected object is resized horizontally. */
//    ResizeVer = 3, /*!< The selected object is resized vertically. */
//    Undefined = 4, /*!< May be passed to methods to indicate that the argument should be ignored. */
//};
//} }
//
//template class ZSDRAWDLL_API ::CEnum<ZS::Draw::EEditResizeMode>;
//namespace ZS {
//namespace Draw {
//typedef ::CEnum<EEditResizeMode> CEnumEditResizeMode;
//} }


namespace ZS {
namespace Draw {
//==============================================================================
/*! Object may provide different selection points for editing and selecting.
*/
enum class ESelectionPointType
//==============================================================================
{
    Undefined         = 0,  /*!< The selection point is not defined (invalid). */
    BoundingRectangle = 1,  /*!< The selection point is at the bounding rectangle. */
    PolygonPoint      = 2,  /*!< The selection point is one of the points of a line or a polygon. */
    LineCenterPoint   = 3   /*!< The selection point is at the center point of a line (also line segments of polygons). */
};
} }

template class ZSDRAWDLL_API ::CEnum<ZS::Draw::ESelectionPointType>;
namespace ZS {
namespace Draw {
typedef ::CEnum<ESelectionPointType> CEnumSelectionPointType;
} }

namespace ZS {
namespace Draw {
//==============================================================================
/*! TSelectionPoints
*/
//==============================================================================

typedef uint16_t TSelectionPointTypes;

const TSelectionPointTypes c_uSelectionPointsNone = 0x00;
const TSelectionPointTypes c_uSelectionPointsBoundingRectCorner     = 0x01;
const TSelectionPointTypes c_uSelectionPointsBoundingRectLineCenter = 0x02;  // Center points of surrounding frame lines
const TSelectionPointTypes c_uSelectionPointsBoundingRectCenter     = 0x04;
const TSelectionPointTypes c_uSelectionPointsBoundingRectRotate     = 0x10;
const TSelectionPointTypes c_uSelectionPointsBoundingRectAll        = 0x1f;
const TSelectionPointTypes c_uSelectionPointsPolygonPoints          = 0x20;
const TSelectionPointTypes c_uSelectionPointsLineCenter             = 0x40;
const TSelectionPointTypes c_uSelectionPointsAll                    = 0x7f;

ZSDRAWDLL_API QString selectionPointTypes2Str( TSelectionPointTypes i_selPts );
} }


namespace ZS {
namespace Draw {
//==============================================================================
/*! Defines the different selection points which an object may provide for
    editing and moving an object.

    Selection points may be at the bounding rectangle:

                          RotateTop
                              +
             TopLeft      TopCenter       TopRight
               +--------------+--------------+
               |            Center           |
    LeftCenter +              +              + RightCenter
               |                             |
               +--------------+--------------+
             BottomLeft   BottomCenter    BottomRight
                              +
                          RotateBottom

    Selection points may also be specific points of a line or polygon (P1, P2, P3, P4)
    or at the center point of lines and line segments (L1, L2, L3).

       P1      L1     P2
        +-------x-------+
                         \
                          x L2
                           \
           P4 +------x------+ P3
                     L3
*/
enum class ESelectionPoint
//==============================================================================
{
    None            =  0, /*!< No selection point. */
    TopLeft         =  1, /*!< Top left corner of a rectangle. */
    TopRight        =  2, /*!< Top right corner of a rectangle. */
    BottomRight     =  3, /*!< Bottom right corner of a rectangle. */
    BottomLeft      =  4, /*!< Top right corner of a rectangle. */
    TopCenter       =  5, /*!< Center point of the top line of a rectangle. */
    RightCenter     =  6, /*!< Center point of the right line of a rectangle. */
    BottomCenter    =  7, /*!< Center point of the bottom line of a rectangle. */
    LeftCenter      =  8, /*!< Center point of the left line of a rectangle. */
    Center          =  9, /*!< Center point of the rectangle. */
    RotateTop       = 10, /*!< Rotation point above the rectangle. */
    RotateBottom    = 11, /*!< Rotation point below the rectangle. */
    PolygonPoint    = 12, /*!< A specific polygon or line point. */
    LineCenterPoint = 13, /*!< The center point of a specific polygon line segment (or line). */
    All             = 14, /*!< To select all selection points at once. */
    Any             = 15  /*!< To specify any selection point. */
};
} }

template class ZSDRAWDLL_API ::CEnum<ZS::Draw::ESelectionPoint>;
namespace ZS {
namespace Draw {
typedef ::CEnum<ESelectionPoint> CEnumSelectionPoint;

const int ESelectionPointRectMin = static_cast<int>(ESelectionPoint::TopLeft);
const int ESelectionPointCornerMin = static_cast<int>(ESelectionPoint::TopLeft);
const int ESelectionPointCornerMax = static_cast<int>(ESelectionPoint::BottomLeft);
const int ESelectionPointLineCenterMin = static_cast<int>(ESelectionPoint::TopCenter);
const int ESelectionPointRectMax = static_cast<int>(ESelectionPoint::LeftCenter);
const int ESelectionPointLineCenterMax = static_cast<int>(ESelectionPoint::LeftCenter);
const int ESelectionPointRotateMin = static_cast<int>(ESelectionPoint::RotateTop);
const int ESelectionPointRotateMax = static_cast<int>(ESelectionPoint::RotateBottom);

ZSDRAWDLL_API void setSelectionPointRotateDistance( double i_fDist_px );
ZSDRAWDLL_API double getSelectionPointRotateDistance();
ZSDRAWDLL_API QPointF getOppositeSelectionPoint( const QRectF& i_rct, ESelectionPoint i_selPt );
ZSDRAWDLL_API ESelectionPoint getOppositeSelectionPoint( ESelectionPoint i_selPt );
//ZSDRAWDLL_API CEnumEditMode selectionPoint2EditMode( ESelectionPoint i_selPt );
//ZSDRAWDLL_API CEnumEditResizeMode selectionPoint2EditResizeMode( ESelectionPoint i_selPt );
ZSDRAWDLL_API QCursor selectionPoint2Cursor( ESelectionPoint i_selPt, double i_fRotationAngle_degree = 0.0 );
} }


namespace ZS {
namespace Draw {
//==============================================================================
/*! Defines the possible line points.
*/
enum class ELinePoint
//==============================================================================
{
    None  = 0,  /*!< No line point selected. */
    Start = 1,  /*!< Start point of a line (usually P1). */
    End   = 2   /*!< End point of a line (usually P2). */
};
} }

template class ZSDRAWDLL_API ::CEnum<ZS::Draw::ELinePoint>;
namespace ZS {
namespace Draw {
typedef ::CEnum<ELinePoint> CEnumLinePoint;
} }


namespace ZS {
namespace Draw {
//==============================================================================
/*!
*/
enum class ETextStyle
//==============================================================================
{
   Normal     = 0,
   Italic     = 1,
   Bold       = 2,
   BoldItalic = 3  /*!< Italic|Bold */
};
} }

template class ZSDRAWDLL_API ::CEnum<ZS::Draw::ETextStyle>;
namespace ZS {
namespace Draw {
typedef ::CEnum<ETextStyle> CEnumTextStyle;

ZSDRAWDLL_API bool isTextStyleBold(const CEnumTextStyle& i_style);
ZSDRAWDLL_API bool isTextStyleItalic(const CEnumTextStyle& i_style);
} }


namespace ZS {
namespace Draw {
//==============================================================================
/*!
enum ETextSize
*/
//==============================================================================

typedef enum {
    ETextSize6  =  0,
    ETextSize7  =  1,
    ETextSize8  =  2,
    ETextSize9  =  3,
    ETextSize10 =  4,
    ETextSize11 =  5,
    ETextSize12 =  6,
    ETextSize14 =  7,
    ETextSize16 =  8,
    ETextSize18 =  9,
    ETextSize20 = 10,
    ETextSize22 = 11,
    ETextSize24 = 12,
    ETextSize26 = 13,
    ETextSize28 = 14,
    ETextSize36 = 15,
    ETextSize48 = 16,
    ETextSize72 = 17,
    ETextSizeCount,
    ETextSizeMin = ETextSize6,
    ETextSizeMax = ETextSize72
}   ETextSize;

ZSDRAWDLL_API int textSize2SizeInPixels( ETextSize i_textSize );
ZSDRAWDLL_API QString textSize2Str( ETextSize i_textSize );
ZSDRAWDLL_API ETextSize str2TextSize( const QString& i_str, bool* o_pbConverted = nullptr );
} }


namespace ZS {
namespace Draw {
//==============================================================================
/*!
*/
enum class ETextEffect
//==============================================================================
{
    None               = 0,
    Strikeout          = 1,
    Underline          = 2,
    StrikeoutUnderline = 3, // Strikeout|Underline
};
} }

template class ZSDRAWDLL_API ::CEnum<ZS::Draw::ETextEffect>;
namespace ZS {
namespace Draw {
typedef ::CEnum<ETextEffect> CEnumTextEffect;

ZSDRAWDLL_API bool isTextEffectStrikeout(const CEnumTextEffect& i_textEffect);
ZSDRAWDLL_API bool isTextEffectUnderline(const CEnumTextEffect& i_textEffect);
} }

namespace ZS {
namespace Draw {
//==============================================================================
/*!
*/
enum class EFillStyle
//==============================================================================
{
    NoFill                 =  0,
    None                   =  NoFill,
    SolidPattern           =  1,
    Dense1Pattern          =  2,
    Dense2Pattern          =  3,
    Dense3Pattern          =  4,
    Dense4Pattern          =  5,
    Dense5Pattern          =  6,
    Dense6Pattern          =  7,
    Dense7Pattern          =  8,
    HorPattern             =  9,
    VerPattern             = 10,
    CrossPattern           = 11,
    BDiagPattern           = 12,
    FDiagPattern           = 13,
    DiagCrossPattern       = 14,
    LinearGradientPattern  = 15,
    RadialGradientPattern  = 16,
    ConicalGradientPattern = 17,
    TexturePattern         = 18
} ;
} }

template class ZSDRAWDLL_API ::CEnum<ZS::Draw::EFillStyle>;
namespace ZS {
namespace Draw {
typedef ::CEnum<EFillStyle> CEnumFillStyle;

ZSDRAWDLL_API Qt::BrushStyle fillStyle2QtBrushStyle( const CEnumFillStyle& i_fillStyle );
ZSDRAWDLL_API bool isFillStyleGradientPattern( const CEnumFillStyle& i_fillStyle );
} }


namespace ZS {
namespace Draw {
//==============================================================================
/*!
*/
enum class ELineStyle
//==============================================================================
{
    NoLine         = 0,
    SolidLine      = 1,
    DashLine       = 2,
    DotLine        = 3,
    DashDotLine    = 4,
    DashDotDotLine = 5
};
} }

template class ZSDRAWDLL_API ::CEnum<ZS::Draw::ELineStyle>;
namespace ZS {
namespace Draw {
typedef ::CEnum<ELineStyle> CEnumLineStyle;

ZSDRAWDLL_API Qt::PenStyle lineStyle2QtPenStyle( const CEnumLineStyle& i_lineStyle );
} }


namespace ZS {
namespace Draw {
//==============================================================================
/*! @brief A line may be indicated with one line (normal), a double line or a triple line.
*/
enum class ELineRecordType
//==============================================================================
{
    Normal = 0,
    Double = 1,
    Triple = 2
};
} }

template class ZSDRAWDLL_API ::CEnum<ZS::Draw::ELineRecordType>;
namespace ZS {
namespace Draw {
typedef ::CEnum<ELineRecordType> CEnumLineRecordType;
} }


namespace ZS {
namespace Draw {
//==============================================================================
/*! @brief A line end may be indicated normal or showing an arrow head.

    The direction of the arrow head depends on whether the arrow should be shown
    at the line start (P1) or line end (P2). To draw an arrow head a polygon with
    4 points is used as shwon in the figure below (assuming a vertical line with
    P1 at top and P2 at bottom):

                           |--Width--|

                               2^                    ---
                               / \                    |
         Line Start (P1)      /   \                 Length
                             /     \                  |
                          1 /___4___\3 BaseLine      ---
                                |
                                |
                                |
                           1____|____3 BaseLine
                            \   4   /
                             \     /
         Line End (P2)        \   /
                               \ /
                               2'

    The arrow head may be styled with a

    - FillStyle (NoFill, SolidPattern),
    - Width (Thin, Medium, Wide),
    - Length (Short, Medium, Long) and
    - a BaseLine.
*/
enum class ELineEndStyle
//==============================================================================
{
    Normal    = 0,
    ArrowHead = 1
};
} }

template class ZSDRAWDLL_API ::CEnum<ZS::Draw::ELineEndStyle>;
namespace ZS {
namespace Draw {
typedef ::CEnum<ELineEndStyle> CEnumLineEndStyle;
} }


namespace ZS {
namespace Draw {
//==============================================================================
/*!
*/
enum class EArrowHeadFillStyle
//==============================================================================
{
    NoFill       = 0,
    SolidPattern = 1
};
} }

template class ZSDRAWDLL_API ::CEnum<ZS::Draw::EArrowHeadFillStyle>;
namespace ZS {
namespace Draw {
typedef ::CEnum<EArrowHeadFillStyle> CEnumArrowHeadFillStyle;

ZSDRAWDLL_API Qt::BrushStyle arrowHeadFillStyle2QtBrushStyle(const CEnumArrowHeadFillStyle& i_fillStyle);
} }


namespace ZS {
namespace Draw {
//==============================================================================
/*! Base line type for the arrow head.

         NoLine           Normal         Indented
            ^               ^               ^
           /|\             / \             / \
          / | \           /   \           /   \
         /  |  \         /     \         /  ^  \
        /   |   \       /_______\       /__/ \__\
            |               |               |
            |               |               |
            |               |               |
*/
enum class EArrowHeadBaseLineType
//==============================================================================
{
    NoLine   = 0,
    Normal   = 1, // straight lined
    Indented = 2
};
} }

template class ZSDRAWDLL_API ::CEnum<ZS::Draw::EArrowHeadBaseLineType>;
namespace ZS {
namespace Draw {
typedef ::CEnum<EArrowHeadBaseLineType> CEnumArrowHeadBaseLineType;
} }


namespace ZS {
namespace Draw {
//==============================================================================
/*!
*/
enum class EArrowHeadWidth
//==============================================================================
{
    Thin   = 0,    /*!<  7 Pixel */
    Medium = 1,    /*!< 13 Pixel */
    Wide   = 2     /*!< 19 Pixel */
};
} }

template class ZSDRAWDLL_API ::CEnum<ZS::Draw::EArrowHeadWidth>;
namespace ZS {
namespace Draw {
typedef ::CEnum<EArrowHeadWidth> CEnumArrowHeadWidth;

ZSDRAWDLL_API double arrowHeadWidth2dy(const CEnumArrowHeadWidth& i_width);
} }


namespace ZS {
namespace Draw {
//==============================================================================
/*!
*/
enum class EArrowHeadLength
//==============================================================================
{
    Short  = 0,   /*!<  7 Pixel */
    Medium = 1,   /*!< 10 Pixel */
    Long   = 2    /*!< 13 Pixel */
};
} }

template class ZSDRAWDLL_API ::CEnum<ZS::Draw::EArrowHeadLength>;
namespace ZS {
namespace Draw {
typedef ::CEnum<EArrowHeadLength> CEnumArrowHeadLength;

ZSDRAWDLL_API double arrowHeadLength2dx(const CEnumArrowHeadLength& i_length);
} }


namespace ZS {
namespace Draw {
//==============================================================================
/*!
*/
enum class EAlignmentRef
//==============================================================================
{
    None    = 0,
    Left    = 1,
    HCenter = 2,
    Right   = 3,
    Top     = 4,
    VCenter = 5,
    Bottom  = 6,
    Width   = 7,
    Height  = 8
};
} }

template class ZSDRAWDLL_API ::CEnum<ZS::Draw::EAlignmentRef>;

namespace ZS {
namespace Draw {
typedef ::CEnum<EAlignmentRef> CEnumAlignmentRef;
} }


namespace ZS
{
namespace Draw
{
//******************************************************************************
struct ZSDRAWDLL_API SGraphObjAlignment
//******************************************************************************
{
public: // ctor
    SGraphObjAlignment() :
        m_alignmentRefChild(EAlignmentRef::None),
        m_alignmentRefParent(EAlignmentRef::None),
        m_bAlignAbsolute(true),
        m_fVal(0.0)
    {
    }
    SGraphObjAlignment(EAlignmentRef i_refChild, EAlignmentRef i_refParent, bool i_bAbsolute, double i_fVal = 0.0) :
        m_alignmentRefChild(i_refChild),
        m_alignmentRefParent(i_refParent),
        m_bAlignAbsolute(i_bAbsolute),
        m_fVal(i_fVal)
    {
    }
public: // struct methods
    QString toString() const;
public: // struct members
    CEnumAlignmentRef m_alignmentRefChild;
    CEnumAlignmentRef m_alignmentRefParent;
    bool m_bAlignAbsolute;
    /*!< If aligned absolute: value in pixels.
         If aligned relative: scale factor to width or height of parent (usually ranging from 0..1) */
    double m_fVal;

}; // struct SGraphObjAlignment


//******************************************************************************
/*! @brief Defines a selection point.

    Selection points may be at the bounding rectangle, at shape points or in
    the center of line segments of a polygon.
*/
struct ZSDRAWDLL_API SGraphObjSelectionPoint
//******************************************************************************
{
public: // struct methods
    static SGraphObjSelectionPoint fromString(const QString& i_str, bool* o_pbOk = nullptr);
    static SGraphObjSelectionPoint fromString(CGraphObj* i_pGraphObj, const QString& i_str, bool* o_pbOk = nullptr);
public: // ctors
    SGraphObjSelectionPoint();
    SGraphObjSelectionPoint(CGraphObj* i_pGraphObj);
    SGraphObjSelectionPoint(CGraphObj* i_pGraphObj, ESelectionPointType i_selPtType, ESelectionPoint i_selPt);
    SGraphObjSelectionPoint(CGraphObj* i_pGraphObj, ESelectionPointType i_selPtType, int i_idxPt);
    SGraphObjSelectionPoint(const SGraphObjSelectionPoint& i_other);
public: // operators
    SGraphObjSelectionPoint& operator = (const SGraphObjSelectionPoint& i_other);
    bool operator == (const SGraphObjSelectionPoint& i_other) const;
    bool operator != (const SGraphObjSelectionPoint& i_other) const;
public: // struct methods
    QString name() const;
    QString toString(bool i_bIncludeGraphObj) const;
public: // struct members
    /*!< Graphical object the selection point belongs to.
         nullptr, if the selection point is not used (undefined). */
    CGraphObj* m_pGraphObj;
    /*!< Type of the selection point. Selection points are differentiated into
         selection points on the bounding rectangle around the graphical object
         or into polygon shape points. */
    ESelectionPointType m_selPtType;
    /*!< For selection points on the bounding rectangle specifies the position
         on (or within) the bounding rectangle.
         For polygon shape points the enum is set to PolygonPoint. */
    ESelectionPoint m_selPt;
    /*!< For selection points on a polygon the index of the polygon point
         is stored the selection point is assigned to. */
    int m_idxPt;

}; // struct SGraphObjSelectionPoint


//******************************************************************************
/*!
*/
struct ZSDRAWDLL_API SGraphObjHitInfo
//******************************************************************************
{
public: // ctor
    SGraphObjHitInfo();
public: // struct methods
    bool isBoundingRectSelectionPointHit() const;
    bool isSelectionPointHit() const;
    bool isPolygonShapePointHit() const;
    bool isLineSegmentHit() const;
public: // struct methods
    //void setCursor(double i_fGraphObjRotAngle_rad);
public: // struct methods
    QString toString() const;
public: // struct members
    //CEnumEditMode m_editMode;
    //CEnumEditResizeMode m_editResizeMode;
    CEnumSelectionPoint m_selPtBoundingRect;
    int m_idxPolygonShapePoint;
    int m_idxLineSegment;
    QPointF m_ptSelected;
    QCursor m_cursor;

}; // struct SGraphObjHitInfo


//******************************************************************************
/*! @brief Struct defining a line in polar coordinates.
*/
struct ZSDRAWDLL_API SPolarCoors
//******************************************************************************
{
public: // struct methods
    static SPolarCoors fromString(const QString& i_str, const QString& i_strSeparator = "/", bool* i_pbOk = nullptr);
public: // ctors
    SPolarCoors();
    SPolarCoors(double i_fLength_px, double i_fAngle_degrees);
    SPolarCoors(const SPolarCoors& i_other);
public: // operators
    bool operator == (const SPolarCoors& i_other) const;
    bool operator != (const SPolarCoors& i_other) const;
public: // struct methods
    QString toString(bool i_bAddUnit = false, const QString& i_strSeparator = "/") const;
public: // struct members
    double m_fLength_px;
    double m_fAngle_degrees;
};


//******************************************************************************
/*! @brief Struct defining the properties of a label.

    As long as a label is not added to the graphics scene the label objects are
    not created but the descriptors are kept in the parent object so that the
    labels can be created with the desired properties on demand.
*/
struct ZSDRAWDLL_API SLabelDscr
//******************************************************************************
{
public: // ctors
    SLabelDscr();
    SLabelDscr(EGraphObjType i_labelType);
    SLabelDscr(EGraphObjType i_labelType, const QString& i_strKey);
    SLabelDscr(EGraphObjType i_labelType, const QString& i_strKey, const SGraphObjSelectionPoint& i_selPt);
    SLabelDscr(EGraphObjType i_labelType, const QString& i_strKey, const SGraphObjSelectionPoint& i_selPt1, const SGraphObjSelectionPoint& i_selPt2);
    SLabelDscr(EGraphObjType i_labelType, const QString& i_strKey, const QString& i_strText, const SGraphObjSelectionPoint& i_selPt);
    SLabelDscr(EGraphObjType i_labelType, const QString& i_strKey, const QString& i_strText, const SGraphObjSelectionPoint& i_selPt1, const SGraphObjSelectionPoint& i_selPt2);
    SLabelDscr(const SLabelDscr& i_other);
public: // struct members
    /*!< Key of the label within the list of labels of the graphical objects. */
    QString m_strKey;
    /*!< Type of the label.
         Range [EGraphObjTypeLabel, EGraphObjTypeLabelGeometryPosition, ...] */
    EGraphObjType m_labelType;
    /*!< For text labels or geometry labels (like position) selPt1 defines the
         selection point the label is linked to.
         For geometry labels using two selection points like length, width or height,
         selPt1 defines the first selection point the label is linked to. */
    SGraphObjSelectionPoint m_selPt1;
    /*!< For text labels or geometry labels (like position) selPt2 is not used and set to Undefined.
         For geometry labels using two selection points like length, width or height,
         selPt2 defines the second selection point the label is linked to. */
    SGraphObjSelectionPoint m_selPt2;
    /*!< Text to be indicated by text labels. Not set (empty string) for geometry labels.
         For geometry labels the indicated text is calculated during runtime
         by the label item and set at the QGraphicsSimpleTextItem. */
    QString m_strText;
    /*!< When modifying the shape of graphical objects the relative position of labels
         linked to the graphical objects should always be the same.
         This can only be managed when keeping the distance and the angle to the selection point
         after moving the labels (see also method getAnchorLineToSelectionPointFromPolarInSceneCoors). */
    SPolarCoors m_polarCoorsToLinkedSelPt;
    /*!< Flag indicating whether the label is visible. */
    bool m_bLabelIsVisible;
    /*!< Flag to indicate whether the anchor line (line from label to parent's selection point the
         label is linked to) should always be visible. */
    bool m_bShowAnchorLine;

}; // struct SLabelDscr

} // namespace Draw

} // namespace ZS


#ifdef _WINDOWS
#pragma warning( pop )
#elif defined __linux__
#pragma GCC diagnostic pop
#endif

#endif // #ifndef ZSDraw_Common_h
