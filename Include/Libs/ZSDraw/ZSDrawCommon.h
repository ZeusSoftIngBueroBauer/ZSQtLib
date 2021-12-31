/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#include "ZSDraw/ZSDrawDllMain.h"

#if QT_VERSION < 0x050000
#include <QtGui/qgraphicsitem.h>
#else
#include <QtWidgets/qgraphicsitem.h>
#endif

#include "ZSSys/ZSSysEnumTemplate.h"


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

namespace ZS
{
namespace Draw
{
//==============================================================================
/*! Supported paper formats for printing.
*/
enum class EPaperFormat
//==============================================================================
{
    DinA5       = 0, /*!< DinA5 (148 x 210 mm). */
    DinA4       = 1, /*!< DinA4 (210 x 297 mm). */
    DinA3       = 2, /*!< DinA3 (297 x 420 mm). */
    DinA2       = 3, /*!< DinA2 (420 x 594 mm). */
    UserDefined = 4  /*!< User defined format. */
    //Undefined = 5, /*!< Indicates that the format is not yet defined. */
};

template class ZSDRAWDLL_API ZS::System::CEnum<EPaperFormat>;
typedef ZS::System::CEnum<EPaperFormat> CEnumPaperFormat;

const double c_fPaperFormatWidthDinA5_mm  = 148.0;
const double c_fPaperFormatHeightDinA5_mm = 210.0;

const double c_fPaperFormatWidthDinA4_mm  = 210.0;
const double c_fPaperFormatHeightDinA4_mm = 297.0;

const double c_fPaperFormatWidthDinA3_mm  = 297.0;
const double c_fPaperFormatHeightDinA3_mm = 420.0;

const double c_fPaperFormatWidthDinA2_mm  = 420.0;
const double c_fPaperFormatHeightDinA2_mm = 594.0;


//==============================================================================
/*! Paper orientation.
*/
enum class EPaperOrientation
//==============================================================================
{
    Portrait  = 0,   /*!< Portrait (vertical). */
    Landscape = 1    /*!< Landscape (horizontal). */
    //Undefined = 2  /*!< Indicates that the orientation is not yet defined. */
};

template class ZSDRAWDLL_API ZS::System::CEnum<EPaperOrientation>;
typedef ZS::System::CEnum<EPaperOrientation> CEnumPaperOrientation;


//==============================================================================
/*! Paper source for the printer.
*/
enum class EPrinterPaperSource
//==============================================================================
{
    AutoSelect = 0,
    ManualFeed = 1,
    Tray1      = 2,
    Tray2      = 3,
    //Undefined  = 4
};

template class ZSDRAWDLL_API ZS::System::CEnum<EPrinterPaperSource>;
typedef ZS::System::CEnum<EPrinterPaperSource> CEnumPrinterPaperSource;


//==============================================================================
/*! Custom data may be set for QGraphicsItem via the setData call.
    This enum defines the keys for the key/value pairs.
*/
enum class EGraphItemDataKey
//==============================================================================
{
    ObjId   = 0, /*!< Key to set the id of the object. The id must be a string. */
    ObjType = 1  /*!< Key to set the type of the object. The type must be an enumerator of EGraphObjType and is of type int. */
};

template class ZSDRAWDLL_API ZS::System::CEnum<EGraphItemDataKey>;
typedef ZS::System::CEnum<EGraphItemDataKey> CEnumGraphItemDataKey;


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
    Select        = 0, /*! Objects may be selected. */
    CreateObjects = 1, /*! A valid object factory is selected and objects may be created. */
    Ignore        = 2, /*! May be passed to methods to indicate that the argument should be ignored. */
    Undefined     = 3  /*! Indicates that no edit tool is selected. */
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
    //Undefined = 2
};

template class ZSDRAWDLL_API ZS::System::CEnum<ECoordinatesVersion>;
typedef ZS::System::CEnum<ECoordinatesVersion> CEnum;


//==============================================================================
/*!
*/
enum class EEditMode
//==============================================================================
{
    Creating       = 0, // The object has been initially created and is still under construction (e.g. adding points to poly lines). !! This mode is not used by the scene !!
    Move           = 1, // The selected objects are moved.
    Resize         = 2, // The selected object is resized.
    Rotate         = 3, // The selected object is rotated around the centre point of its bounding rectangle.
    MoveShapePoint = 4, // A single shape point of the selected object is being moved.
    EditText       = 5, // A single shape point of the selected object is being moved.
    Ignore         = 6, // May be passed to methods to indicate that the argument should be ignored.
    Select         = 7, // Reserved for internal use (subsystem test).
    Undefined      = 8
};

template class ZSDRAWDLL_API ZS::System::CEnum<EEditMode>;
typedef ZS::System::CEnum<EEditMode> CEnumEditMode;


//==============================================================================
/*!
*/
enum class EEditResizeMode
//==============================================================================
{
    ResizeAll = 0, // The selected object is resized horizontally and vertically.
    ResizeHor = 1, // The selected object is resized horizontally.
    ResizeVer = 2, // The selected object is resized vertically.
    Ignore    = 3, // May be passed to methods to indicate that the argument should be ignored.
    Undefined = 4
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
    //Undefined       = 2
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
    TopLeft       =  0, // =  RectMin = CornerMin
    RectMin       = TopLeft,
    CornerMin     = TopLeft,
    TopRight      =  1,
    BottomRight   =  2,
    BottomLeft    =  3, // = CornerMax
    CornerMax     = BottomLeft,
    TopCenter     =  4, // = LineCenterMin
    LineCenterMin = TopCenter,
    RightCenter   =  5,
    BottomCenter  =  6,
    LeftCenter    =  7, // = RectMax = LineCenterMax
    RectMax       = LeftCenter,
    LineCenterMax = LeftCenter,
    Center        =  8,
    RotateTop     =  9, // = RotateMin
    RotateMin     = RotateTop,
    RotateBottom  = 10, // = RotateMax
    RotateMax     = RotateBottom,
    //Default,
    Undefined     = 11
};

template class ZSDRAWDLL_API ZS::System::CEnum<ESelectionPoint>;
typedef ZS::System::CEnum<ESelectionPoint> CEnumSelectionPoint;

ZSDRAWDLL_API void setSelectionPointRotateDistance( double i_fDist_px );
ZSDRAWDLL_API double getSelectionPointRotateDistance();
ZSDRAWDLL_API QPointF getSelectionPoint( const QLineF& i_lin, ESelectionPoint i_selPt );
ZSDRAWDLL_API QPointF getSelectionPoint( const QRectF& i_rct, ESelectionPoint i_selPt );
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
    Start     = 0,
    End       = 1,
    Undefined = 2
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
   BoldItalic = 3, // Italic|Bold
};

template class ZSDRAWDLL_API ZS::System::CEnum<ETextStyle>;
typedef ZS::System::CEnum<ETextStyle> CEnumTextStyle;


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
ZSDRAWDLL_API ETextSize str2TextSize( const QString& i_str );


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
    //Default              = 19,
    //Undefined
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
    //Default,
    //Undefined
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
    //Default,
    //Undefined
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
    //Default,
    //Undefined
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
    //Default,
    //Undefined
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
    //Default,
    //Undefined
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
    //Default,
    //Undefined
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
    //Default,
    //Undefined
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
    Left      = 0,
    HCenter   = 1,
    Right     = 2,
    Top       = 3,
    VCenter   = 4,
    Bottom    = 5,
    Width     = 6,
    Height    = 7,
    Undefined = 8
};

template class ZSDRAWDLL_API ZS::System::CEnum<EAlignmentRef>;
typedef ZS::System::CEnum<EAlignmentRef> CEnumAlignmentRef;

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_Common_h
