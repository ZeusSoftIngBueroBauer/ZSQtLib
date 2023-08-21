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

#include "ZSPhysVal/ZSPhysVal.h"
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

template class ZSDRAWDLL_API ZS::System::CEnum<EDrawingDimensionUnit>;
typedef ZS::System::CEnum<EDrawingDimensionUnit> CEnumDrawingDimensionUnit;

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

template class ZSDRAWDLL_API ZS::System::CEnum<ENormedPaperSize>;
typedef ZS::System::CEnum<ENormedPaperSize> CEnumNormedPaperSize;

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
struct ZSDRAWDLL_API SDrawArea
//==============================================================================
{
public: // ctors and dtor
    SDrawArea();
    SDrawArea(
        const ZS::PhysVal::CPhysVal& i_physValWidth,
        const ZS::PhysVal::CPhysVal& i_physValHeight,
        double                       i_fXScaleFactor,
        double                       i_fYScaleFactor );
public: // methods
    bool isValid() const;
public: // struct members
    ZS::PhysVal::CPhysVal m_physValWidth;
    ZS::PhysVal::CPhysVal m_physValHeight;
    double                m_fXScaleFac;
    double                m_fYScaleFac;

}; // struct SDrawArea


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

template class ZSDRAWDLL_API ZS::System::CEnum<EPrinterPaperSource>;
typedef ZS::System::CEnum<EPrinterPaperSource> CEnumPrinterPaperSource;


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

template class ZSDRAWDLL_API ZS::System::CEnum<EEditTool>;
typedef ZS::System::CEnum<EEditTool> CEnumEditTool;


//==============================================================================
/*!
*/
enum class ECoordinatesVersion
//==============================================================================
{
    Original  = 0,
    Current   = 1
};

template class ZSDRAWDLL_API ZS::System::CEnum<ECoordinatesVersion>;
typedef ZS::System::CEnum<ECoordinatesVersion> CEnum;


//==============================================================================
/*!
*/
enum class EEditMode
//==============================================================================
{
    None           = 0,
    Creating       = 1, // The object has been initially created and is still under construction (e.g. adding points to poly lines). !! This mode is not used by the scene !!
    Move           = 2, // The selected objects are moved.
    Resize         = 3, // The selected object is resized.
    Rotate         = 4, // The selected object is rotated around the centre point of its bounding rectangle.
    MoveShapePoint = 5, // A single shape point of the selected object is being moved.
    EditText       = 6, // A single shape point of the selected object is being moved.
    Ignore         = 7  // May be passed to methods to indicate that the argument should be ignored.
};

template class ZSDRAWDLL_API ZS::System::CEnum<EEditMode>;
typedef ZS::System::CEnum<EEditMode> CEnumEditMode;


//==============================================================================
/*!
*/
enum class EEditResizeMode
//==============================================================================
{
    None      = 0,
    ResizeAll = 1, // The selected object is resized horizontally and vertically.
    ResizeHor = 2, // The selected object is resized horizontally.
    ResizeVer = 3, // The selected object is resized vertically.
    Ignore    = 4, // May be passed to methods to indicate that the argument should be ignored.
};

template class ZSDRAWDLL_API ZS::System::CEnum<EEditResizeMode>;
typedef ZS::System::CEnum<EEditResizeMode> CEnumEditResizeMode;


//==============================================================================
/*!
*/
enum class ESelectionPointType
//==============================================================================
{
    BoundingRectangle = 0,
    PolygonShapePoint = 1
};

template class ZSDRAWDLL_API ZS::System::CEnum<ESelectionPointType>;
typedef ZS::System::CEnum<ESelectionPointType> CEnumSelectionPointType;


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
/*!
*/
enum class ESelectionPoint
//==============================================================================
{
    None          =  0,
    TopLeft       =  1, /*!< Also used to select the StartPoint of a line. */
    TopRight      =  2,
    BottomRight   =  3, /*!< Also used to select the EndPoint of a line. */
    BottomLeft    =  4,
    TopCenter     =  5,
    RightCenter   =  6,
    BottomCenter  =  7,
    LeftCenter    =  8,
    Center        =  9,
    RotateTop     = 10,
    RotateBottom  = 11,
    PolygonPoint  = 12,
    All           = 13,
    Any           = 14
};

template class ZSDRAWDLL_API ZS::System::CEnum<ESelectionPoint>;
typedef ZS::System::CEnum<ESelectionPoint> CEnumSelectionPoint;

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
/*!
*/
enum class ELinePoint
//==============================================================================
{
    None  = 0,
    Start = 1,
    End   = 2
};

template class ZSDRAWDLL_API ZS::System::CEnum<ELinePoint>;
typedef ZS::System::CEnum<ELinePoint> CEnumLinePoint;


//==============================================================================
/*!
*/
enum class ETextStyle
//==============================================================================
{
   Normal     = 0,
   Italic     = 1,
   Bold       = 2,
   BoldItalic = 3  // Italic|Bold
};

template class ZSDRAWDLL_API ZS::System::CEnum<ETextStyle>;
typedef ZS::System::CEnum<ETextStyle> CEnumTextStyle;

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

template class ZSDRAWDLL_API ZS::System::CEnum<ETextEffect>;
typedef ZS::System::CEnum<ETextEffect> CEnumTextEffect;

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

template class ZSDRAWDLL_API ZS::System::CEnum<EFillStyle>;
typedef ZS::System::CEnum<EFillStyle> CEnumFillStyle;

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

template class ZSDRAWDLL_API ZS::System::CEnum<ELineStyle>;
typedef ZS::System::CEnum<ELineStyle> CEnumLineStyle;

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

template class ZSDRAWDLL_API ZS::System::CEnum<ELineRecordType>;
typedef ZS::System::CEnum<ELineRecordType> CEnumLineRecordType;


//==============================================================================
/*!
*/
enum class ELineEndStyle
//==============================================================================
{
    Normal    = 0,
    ArrowHead = 1
};

template class ZSDRAWDLL_API ZS::System::CEnum<ELineEndStyle>;
typedef ZS::System::CEnum<ELineEndStyle> CEnumLineEndStyle;


//==============================================================================
/*!
*/
enum class ELineEndFillStyle
//==============================================================================
{
    NoFill       = 0,
    SolidPattern = 1
};

template class ZSDRAWDLL_API ZS::System::CEnum<ELineEndFillStyle>;
typedef ZS::System::CEnum<ELineEndFillStyle> CEnumLineEndFillStyle;

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

template class ZSDRAWDLL_API ZS::System::CEnum<ELineEndBaseLineType>;
typedef ZS::System::CEnum<ELineEndBaseLineType> CEnumLineEndBaseLineType;


//==============================================================================
/*!
*/
enum class ELineEndWidth
//==============================================================================
{
    Thin   = 0,    //  7 Pixel
    Medium = 1,    // 13 Pixel
    Wide   = 2     // 19 Pixel
};

template class ZSDRAWDLL_API ZS::System::CEnum<ELineEndWidth>;
typedef ZS::System::CEnum<ELineEndWidth> CEnumLineEndWidth;

ZSDRAWDLL_API double lineEndWidth2dy( ELineEndWidth i_lineEndWidth );


//==============================================================================
/*!
*/
enum class ELineEndLength
//==============================================================================
{
    Short  = 0,   //  7 Pixel
    Medium = 1,   // 10 Pixel
    Long   = 2    // 13 Pixel
};

template class ZSDRAWDLL_API ZS::System::CEnum<ELineEndLength>;
typedef ZS::System::CEnum<ELineEndLength> CEnumLineEndLength;

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

template class ZSDRAWDLL_API ZS::System::CEnum<EAlignmentRef>;
typedef ZS::System::CEnum<EAlignmentRef> CEnumAlignmentRef;

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_Common_h
