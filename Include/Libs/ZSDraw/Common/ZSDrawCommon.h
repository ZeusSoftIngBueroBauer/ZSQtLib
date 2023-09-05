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

#include "ZSSys/ZSSysEnumTemplate.h"

#if QT_VERSION < 0x050000
#include <QtGui/qgraphicsitem.h>
#else
#include <QtWidgets/qgraphicsitem.h>
#endif


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

namespace ZS
{
namespace Draw
{
class CGraphObj;

//==============================================================================
/*! Dimension unit for the drawing.
*/
enum class EDrawingDimensionUnit {
//==============================================================================
    Pixels, /*!< Dimensions defined based on pixels. */
    Metric  /*!< Dimensions defined based on metric units.
                 The logical pixel positions got to be calculated depending
                 on the screen resolution given in Dots/Inch or Pixels/Inch. */
};

template class ZSDRAWDLL_API ::CEnum<ZS::Draw::EDrawingDimensionUnit>;
typedef ::CEnum<EDrawingDimensionUnit> CEnumDrawingDimensionUnit;

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

template class ZSDRAWDLL_API ::CEnum<ENormedPaperSize>;
typedef ::CEnum<ENormedPaperSize> CEnumNormedPaperSize;

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
    SAttribute( const QString& i_strName, QVariant::Type i_type ) :
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

template class ZSDRAWDLL_API ::CEnum<EPrinterPaperSource>;
typedef ::CEnum<EPrinterPaperSource> CEnumPrinterPaperSource;


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
    EGraphObjTypeUserDefined,
    EGraphObjTypeSelectionPoint,
    EGraphObjTypeLabel,
    EGraphObjTypeUndefined = -1
}   EGraphObjType;

ZSDRAWDLL_API QString graphObjType2Str( int i_type );
ZSDRAWDLL_API EGraphObjType str2GraphObjType( const QString& i_str );
ZSDRAWDLL_API QString graphObjType2ClassName( int i_type );
ZSDRAWDLL_API EGraphObjType className2GraphObjType( const QString& i_str );


//==============================================================================
/*! Defines the current edit tool.
*/
enum class EEditTool
//==============================================================================
{
    None          = 0, /*! Indicates that no edit tool is selected. */
    Select        = 1, /*! Objects may be selected. */
    CreateObjects = 2, /*! A valid object factory is selected and objects may be created. */
    Ignore        = 3  /*! May be passed to methods to indicate that the argument should be ignored. */
};

template class ZSDRAWDLL_API ::CEnum<EEditTool>;
typedef ::CEnum<EEditTool> CEnumEditTool;


//==============================================================================
/*! When formating the shape of an object using transformatons like scaling or
    or rotating the original coordinates need to be kept to avoid inaccuracies.
    E.g. when rotating an object around its center by 360° the objects position
    and size should be the same as it would not have been rotated at all.
*/
enum class ECoordinatesVersion
//==============================================================================
{
    Original    = 0,    /*!< The coordinate has been explicitly set. */
    Transformed = 1     /*!< Current coordinates resulting when transforming the original coordinates. */
};

template class ZSDRAWDLL_API ::CEnum<ECoordinatesVersion>;
typedef ::CEnum<ECoordinatesVersion> CEnumCoordinatesVersion;


//==============================================================================
/*! Defines the current edit mode of the drawing scene and the graphics item.
*/
enum class EEditMode
//==============================================================================
{
    None           = 0, /*!< No edit mode is selected. */
    Creating       = 1, /*!< The object has been initially created and is still under
                             construction (e.g. adding points to poly lines). 
                             !! This mode is not used by the scene !! */
    Move           = 2, /*!< The selected objects are moved. */
    Resize         = 3, /*!< The selected object is resized. */
    Rotate         = 4, /*!< The selected object is rotated around the centre point of its bounding rectangle. */
    MoveShapePoint = 5, /*!< A single shape point of the selected object is being moved. */
    EditText       = 6, /*!< A single shape point of the selected object is being moved. */
    Ignore         = 7  /*!< May be passed to methods to indicate that the argument should be ignored. */
};

template class ZSDRAWDLL_API ::CEnum<EEditMode>;
typedef ::CEnum<EEditMode> CEnumEditMode;


//==============================================================================
/*! When currently resizing an object (EditMode::Resize) the resize mode defines
    in which direction the object is being resized.
*/
enum class EEditResizeMode
//==============================================================================
{
    None      = 0, /*!< No resize mode selected. */
    ResizeAll = 1, /*!< The selected object is resized horizontally and vertically. */
    ResizeHor = 2, /*!< The selected object is resized horizontally. */
    ResizeVer = 3, /*!< The selected object is resized vertically. */
    Ignore    = 4, /*!< May be passed to methods to indicate that the argument should be ignored. */
};

template class ZSDRAWDLL_API ::CEnum<EEditResizeMode>;
typedef ::CEnum<EEditResizeMode> CEnumEditResizeMode;


//==============================================================================
/*! Object may provide different selection points for editing and selecting.
*/
enum class ESelectionPointType
//==============================================================================
{
    BoundingRectangle = 0,  /*!< The selection point is at the bounding rectangle. */
    PolygonShapePoint = 1   /*!< The selection point is one of the points of a polygon. */
};

template class ZSDRAWDLL_API ::CEnum<ESelectionPointType>;
typedef ::CEnum<ESelectionPointType> CEnumSelectionPointType;


//==============================================================================
/*! enum ESelectionPoints
*/
//==============================================================================

typedef enum {
    ESelectionPointsNone                   = 0x00,
    ESelectionPointsBoundingRectCorner     = 0x01,
    ESelectionPointsBoundingRectLineCenter = 0x02,  // Center points of surrounding frame lines
    ESelectionPointsBoundingRectCenter     = 0x04,
    ESelectionPointsBoundingRectRotate     = 0x10,
    ESelectionPointsBoundingRectAll        = 0x1f,
    ESelectionPointsPolygonShapePoints     = 0x20,  // Lines, polylines, polygons and connection lines have additional selection points
    ESelectionPointsAll                    = 0x3f,
}   ESelectionPoints;

ZSDRAWDLL_API QString selectionPoints2Str( int i_selPts );


//==============================================================================
/*! Defines the different selection points which an object may provide for
    editing and moving an object.

    Selection points are usually at the bounding rectangle or, in case of polygons,
    may also be a specific point of a polygon.

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
*/
enum class ESelectionPoint
//==============================================================================
{
    None          =  0, /*!< No selection point. */
    TopLeft       =  1, /*!< Top left corner of a rectangle. Also used to select the StartPoint of a line. */
    TopRight      =  2, /*!< Top right corner of a rectangle. */
    BottomRight   =  3, /*!< Bottom right corner of a rectangle. Also used to select the EndPoint of a line. */
    BottomLeft    =  4, /*!< Top right corner of a rectangle. */
    TopCenter     =  5, /*!< Center point of the top line of a rectangle. */
    RightCenter   =  6, /*!< Center point of the right line of a rectangle. */
    BottomCenter  =  7, /*!< Center point of the bottom line of a rectangle. */
    LeftCenter    =  8, /*!< Center point of the left line of a rectangle. */
    Center        =  9, /*!< Center point of the rectangle. */
    RotateTop     = 10, /*!< Rotation point above the rectangle. */
    RotateBottom  = 11, /*!< Rotation point below the rectangle. */
    PolygonPoint  = 12, /*!< A specific polygon point. */
    All           = 13, /*!< To select all selection points at once. */
    Any           = 14  /*!< To specify any selection point. */
};

template class ZSDRAWDLL_API ::CEnum<ESelectionPoint>;
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
ZSDRAWDLL_API QPointF getSelectionPointCoors( const QLineF& i_lin, ESelectionPoint i_selPt );
ZSDRAWDLL_API QPointF getSelectionPointCoors( const QRectF& i_rct, ESelectionPoint i_selPt );
ZSDRAWDLL_API QPointF getSelectionPointOpposite( const QRectF& i_rct, ESelectionPoint i_selPt );
ZSDRAWDLL_API ESelectionPoint getSelectionPointOpposite( ESelectionPoint i_selPt );
ZSDRAWDLL_API CEnumEditMode selectionPoint2EditMode( ESelectionPoint i_selPt );
ZSDRAWDLL_API CEnumEditResizeMode selectionPoint2EditResizeMode( ESelectionPoint i_selPt );
ZSDRAWDLL_API Qt::CursorShape selectionPoint2CursorShape( ESelectionPoint i_selPt );


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

template class ZSDRAWDLL_API ::CEnum<ELinePoint>;
typedef ::CEnum<ELinePoint> CEnumLinePoint;


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

template class ZSDRAWDLL_API ::CEnum<ETextStyle>;
typedef ::CEnum<ETextStyle> CEnumTextStyle;

ZSDRAWDLL_API bool isTextStyleBold(ETextStyle i_style);
ZSDRAWDLL_API bool isTextStyleItalic(ETextStyle i_style);


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

template class ZSDRAWDLL_API ::CEnum<ETextEffect>;
typedef ::CEnum<ETextEffect> CEnumTextEffect;

ZSDRAWDLL_API bool isTextEffectStrikeout(ETextEffect i_textEffect);
ZSDRAWDLL_API bool isTextEffectUnderline(ETextEffect i_textEffect);

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

template class ZSDRAWDLL_API ::CEnum<EFillStyle>;
typedef ::CEnum<EFillStyle> CEnumFillStyle;

ZSDRAWDLL_API Qt::BrushStyle fillStyle2QtBrushStyle( EFillStyle i_fillStyle );
ZSDRAWDLL_API bool isFillStyleGradientPattern( EFillStyle i_fillStyle );


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

template class ZSDRAWDLL_API ::CEnum<ELineStyle>;
typedef ::CEnum<ELineStyle> CEnumLineStyle;

ZSDRAWDLL_API Qt::PenStyle lineStyle2QtPenStyle( ELineStyle i_lineStyle );


//==============================================================================
/*!
*/
enum class ELineRecordType
//==============================================================================
{
    Normal = 0,
    Double = 1,
    Triple = 2
};

template class ZSDRAWDLL_API ::CEnum<ELineRecordType>;
typedef ::CEnum<ELineRecordType> CEnumLineRecordType;


//==============================================================================
/*!
*/
enum class ELineEndStyle
//==============================================================================
{
    Normal    = 0,
    ArrowHead = 1
};

template class ZSDRAWDLL_API ::CEnum<ELineEndStyle>;
typedef ::CEnum<ELineEndStyle> CEnumLineEndStyle;


//==============================================================================
/*!
*/
enum class ELineEndFillStyle
//==============================================================================
{
    NoFill       = 0,
    SolidPattern = 1
};

template class ZSDRAWDLL_API ::CEnum<ELineEndFillStyle>;
typedef ::CEnum<ELineEndFillStyle> CEnumLineEndFillStyle;

ZSDRAWDLL_API Qt::BrushStyle lineEndFillStyle2QtBrushStyle( ELineEndFillStyle i_fillStyle );


//==============================================================================
/*!
*/
enum class ELineEndBaseLineType
//==============================================================================
{
    NoLine   = 0,
    Normal   = 1, // straight lined
    Indented = 2
};

template class ZSDRAWDLL_API ::CEnum<ELineEndBaseLineType>;
typedef ::CEnum<ELineEndBaseLineType> CEnumLineEndBaseLineType;


//==============================================================================
/*!
*/
enum class ELineEndWidth
//==============================================================================
{
    Thin   = 0,    /*!<  7 Pixel */
    Medium = 1,    /*!< 13 Pixel */
    Wide   = 2     /*!< 19 Pixel */
};

template class ZSDRAWDLL_API ::CEnum<ELineEndWidth>;
typedef ::CEnum<ELineEndWidth> CEnumLineEndWidth;

ZSDRAWDLL_API double lineEndWidth2dy( ELineEndWidth i_lineEndWidth );


//==============================================================================
/*!
*/
enum class ELineEndLength
//==============================================================================
{
    Short  = 0,   /*!<  7 Pixel */
    Medium = 1,   /*!< 10 Pixel */
    Long   = 2    /*!< 13 Pixel */
};

template class ZSDRAWDLL_API ::CEnum<ELineEndLength>;
typedef ::CEnum<ELineEndLength> CEnumLineEndLength;

ZSDRAWDLL_API double lineEndLength2dx( ELineEndLength i_lineEndLength );


//==============================================================================
/*!
*/
enum class EAlignmentRef
//==============================================================================
{
    None      = 0,
    Left      = 1,
    HCenter   = 2,
    Right     = 3,
    Top       = 4,
    VCenter   = 5,
    Bottom    = 6,
    Width     = 7,
    Height    = 8
};

template class ZSDRAWDLL_API ::CEnum<EAlignmentRef>;
typedef ::CEnum<EAlignmentRef> CEnumAlignmentRef;

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_Common_h
