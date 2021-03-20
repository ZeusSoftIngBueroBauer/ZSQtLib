/*******************************************************************************

Copyright 2004 - 2012 by    ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSDraw_DllMain_h
#define ZSDraw_DllMain_h

// generic export defines
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #define __API_EXPORT __declspec(dllexport)
    #define __API_IMPORT __declspec(dllimport)
#elif defined(__linux__)
    #if (__GNUC__ >= 4)
        #define __API_EXPORT __attribute((visibility("default")))
    #else
        #define __API_EXPORT
    #endif
    #define  __API_IMPORT
#else
    #error Missing export macro
#endif

// specific export defines
#ifdef ZSDRAWDLL_EXPORTS
    #define ZSDRAWDLL_API __API_EXPORT
#else
    #define ZSDRAWDLL_API __API_IMPORT
#endif


#include <QtCore/qabstractitemmodel.h>
#include <QtCore/qstring.h>
#include <QtCore/qrect.h>

#include <QtGui/qcursor.h>

#if QT_VERSION < 0x050000
#include <QtGui/qgraphicsitem.h>
#else
#include <QtWidgets/qgraphicsitem.h>
#endif

#include "ZSPhysVal/ZSPhysVal.h"

class QGraphicsItem;
class QTransform;


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

namespace ZS
{
namespace Draw
{
class CDrawSettings;
class CGraphObj;
class CGraphObjLabel;

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
struct ZSDRAWDLL_API SGraphObjEventFct
//==============================================================================
{
protected: // ctors and dtor
    SGraphObjEventFct() :
        m_pvThis(nullptr),
        m_pvData(nullptr)
    {
    }
    SGraphObjEventFct( void* i_pvThis, void* i_pvData ) :
        m_pvThis(i_pvThis),
        m_pvData(i_pvData)
    {
    }
    ~SGraphObjEventFct()
    {
        m_pvThis = nullptr;
        m_pvData = nullptr;
    }
public: // struct members
    void* m_pvThis;
    void* m_pvData;

}; // struct SGraphObjEventFct


//==============================================================================
struct ZSDRAWDLL_API SGraphObjMouseEventFct : public SGraphObjEventFct
//==============================================================================
{
public: // ctors and dtor
    SGraphObjMouseEventFct() :
        SGraphObjEventFct(),
        m_pFct(nullptr)
    {
    }
    SGraphObjMouseEventFct( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData ) :
        SGraphObjEventFct(i_pvThis,i_pvData),
        m_pFct(i_pFct)
    {
    }
    ~SGraphObjMouseEventFct()
    {
        m_pFct = nullptr;
    }
public: // struct members
    TFctMouseEvent m_pFct;

}; // struct SGraphObjMouseEventFct


//==============================================================================
struct ZSDRAWDLL_API SGraphObjKeyEventFct : public SGraphObjEventFct
//==============================================================================
{
public: // ctors and dtor
    SGraphObjKeyEventFct() :
        SGraphObjEventFct(),
        m_pFct(nullptr)
    {
    }
    SGraphObjKeyEventFct( TFctKeyEvent i_pFct, void* i_pvThis, void* i_pvData ) :
        SGraphObjEventFct(i_pvThis,i_pvData),
        m_pFct(i_pFct)
    {
    }
    ~SGraphObjKeyEventFct()
    {
        m_pFct = nullptr;
    }
public: // struct members
    TFctKeyEvent m_pFct;

}; // struct SGraphObjKeyEventFct


/*==============================================================================
EPaperFormat
==============================================================================*/

const double c_fPaperFormatWidthDinA4_mm  = 210.0;
const double c_fPaperFormatHeightDinA4_mm = 297.0;

typedef enum {
    EPaperFormatDinA5       = 0,
    EPaperFormatDinA4       = 1,
    EPaperFormatDinA3       = 2,
    EPaperFormatUserDefined = 3,
    EPaperFormatCount,
    EPaperFormatUndefined
}   EPaperFormat;

ZSDRAWDLL_API QString paperFormat2Str( int i_iVal );
ZSDRAWDLL_API EPaperFormat str2PaperFormat( const QString& i_strVal );


/*==============================================================================
EPaperOrientation
==============================================================================*/

typedef enum {
    EPaperOrientationUpright   = 0,
    EPaperOrientationLandscape = 1,
    EPaperOrientationMax       = 1,
    EPaperOrientationCount,
    EPaperOrientationUndefined
}   EPaperOrientation;

ZSDRAWDLL_API QString paperOrientation2Str( int i_iVal );
ZSDRAWDLL_API EPaperOrientation str2PaperOrientation( const QString& i_strVal );


/*==============================================================================
EPrinterPaperSource
==============================================================================*/

typedef enum {
    EPrinterPaperSourceAutoSelect = 0,
    EPrinterPaperSourceManualFeed = 1,
    EPrinterPaperSourceTray1      = 2,
    EPrinterPaperSourceTray2      = 3,
    EPrinterPaperSourceMax        = 3,
    EPrinterPaperSourceCount,
    EPrinterPaperSourceUndefined
}   EPrinterPaperSource;

ZSDRAWDLL_API QString printerPaperSource2Str( int i_iVal );
ZSDRAWDLL_API EPrinterPaperSource str2PrinterPaperSource( const QString& i_strVal );


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


/*==============================================================================
EGraphObjType
==============================================================================*/

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


/*==============================================================================
QPainterPath::ElementType
==============================================================================*/

ZSDRAWDLL_API QString painterPathElemType2Str( int i_type );
ZSDRAWDLL_API QString painterPath2Str( QGraphicsItem* i_pGraphObj, const QPainterPath& i_painterPath );


/*==============================================================================
QTransform
==============================================================================*/

ZSDRAWDLL_API QString transformationType2Str( int i_type );
ZSDRAWDLL_API QStringList transformation2StrList( const QTransform& i_transform );


/*==============================================================================
EEditTool
==============================================================================*/

typedef enum {
    EEditToolSelect        = 0, // Objects may be selected.
    EEditToolCreateObjects = 1, // A valid object factory is selected and objects may be created.
    EEditToolCount,
    EEditToolUndefined,
    EEditToolIgnore             // May be passed to methods to indicate that the argument should be ignored.
}   EEditTool;

ZSDRAWDLL_API QString editTool2Str( int i_iVal );


/*==============================================================================
ECoordinatesVersion
==============================================================================*/

typedef enum {
    ECoordinatesVersionOriginal = 0,
    ECoordinatesVersionCurrent  = 1,
    ECoordinatesVersionCount,
    ECoordinatesVersionUndefined
}   ECoordinatesVersion;

ZSDRAWDLL_API QString coordinatesVersion2Str( int i_iVal );


/*==============================================================================
EEditMode
==============================================================================*/

typedef enum {
    EEditModeCreating       = 0, // The object has been initially created and is still under construction (e.g. adding points to poly lines). !! This mode is not used by the scene !!
    EEditModeMove           = 1, // The selected objects are moved.
    EEditModeResize         = 2, // The selected object is resized.
    EEditModeRotate         = 3, // The selected object is rotated around the centre point of its bounding rectangle.
    EEditModeMoveShapePoint = 4, // A single shape point of the selected object is being moved.
    EEditModeEditText       = 5, // A single shape point of the selected object is being moved.
    EEditModeCount,
    EEditModeUndefined,
    EEditModeIgnore,             // May be passed to methods to indicate that the argument should be ignored.
    EEditModeSelect              // Reserved for internal use (subsystem test).
}   EEditMode;

ZSDRAWDLL_API QString editMode2Str( int i_iMode );


/*==============================================================================
EEditResizeMode
==============================================================================*/

typedef enum {
    EEditResizeModeResizeAll = 0, // The selected object is resized horizontally and vertically.
    EEditResizeModeResizeHor = 1, // The selected object is resized horizontally.
    EEditResizeModeResizeVer = 2, // The selected object is resized vertically.
    EEditResizeModeCount,
    EEditResizeModeUndefined,
    EEditResizeModeIgnore         // May be passed to methods to indicate that the argument should be ignored.
}   EEditResizeMode;

ZSDRAWDLL_API QString editResizeMode2Str( int i_iMode );


/*==============================================================================
ESelectionPointType
==============================================================================*/

typedef enum
{
    ESelectionPointTypeBoundingRectangle = 0,
    ESelectionPointTypePolygonShapePoint = 1,
    ESelectionPointTypeCount,
    ESelectionPointTypeUndefined
}   ESelectionPointType;

ZSDRAWDLL_API QString selectionPointType2Str( int i_type );
ZSDRAWDLL_API ESelectionPointType str2SelectionPointType( const QString& i_str );


/*==============================================================================
ESelectionPoints
==============================================================================*/

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


/*==============================================================================
ESelectionPoint
==============================================================================*/

typedef enum {
    ESelectionPointRectMin       =  0,
    ESelectionPointCornerMin     =  0,
    ESelectionPointTopLeft       =  0,
    ESelectionPointTopRight      =  1,
    ESelectionPointBottomRight   =  2,
    ESelectionPointBottomLeft    =  3,
    ESelectionPointCornerMax     =  3,
    ESelectionPointLineCenterMin =  4,
    ESelectionPointTopCenter     =  4,
    ESelectionPointRightCenter   =  5,
    ESelectionPointBottomCenter  =  6,
    ESelectionPointLeftCenter    =  7,
    ESelectionPointLineCenterMax =  7,
    ESelectionPointRectMax       =  7,
    ESelectionPointCenter        =  8,
    ESelectionPointRotateMin     =  9,
    ESelectionPointRotateTop     =  9,
    ESelectionPointRotateBottom  = 10,
    ESelectionPointRotateMax     = 10,
    ESelectionPointCount,
    ESelectionPointDefault,
    ESelectionPointUndefined
}   ESelectionPoint;

ZSDRAWDLL_API QString selectionPoint2Str( int i_selPt, bool i_bShort = false );
ZSDRAWDLL_API ESelectionPoint str2SelectionPoint( const QString& i_strPoint );
ZSDRAWDLL_API void setSelectionPointRotateDistance( double i_fDist_px );
ZSDRAWDLL_API double getSelectionPointRotateDistance();
ZSDRAWDLL_API QPointF getSelectionPoint( const QLineF& i_lin, ESelectionPoint i_selPt );
ZSDRAWDLL_API QPointF getSelectionPoint( const QRectF& i_rct, ESelectionPoint i_selPt );
ZSDRAWDLL_API QPointF getSelectionPointOpposite( const QLineF& i_lin, ESelectionPoint i_selPt );
ZSDRAWDLL_API QPointF getSelectionPointOpposite( const QRectF& i_rct, ESelectionPoint i_selPt );
ZSDRAWDLL_API ESelectionPoint getSelectionPointOpposite( ESelectionPoint i_selPt );
ZSDRAWDLL_API EEditMode selectionPoint2EditMode( ESelectionPoint i_selPt );
ZSDRAWDLL_API EEditResizeMode selectionPoint2EditResizeMode( ESelectionPoint i_selPt );
ZSDRAWDLL_API Qt::CursorShape selectionPoint2CursorShape( ESelectionPoint i_selPt );


/*==============================================================================
QGraphicsItem::GraphicsItemChange
==============================================================================*/

ZSDRAWDLL_API QString graphicsItemChange2Str( int i_change );


/*==============================================================================
ELinePoint
==============================================================================*/

typedef enum {
    ELinePointStart = 0,
    ELinePointEnd   = 1,
    ELinePointCount,
    ELinePointUndefined
}   ELinePoint;

ZSDRAWDLL_API QString linePoint2Str( int i_linePoint );
ZSDRAWDLL_API ELinePoint str2LinePoint( const QString& i_str );


/*==============================================================================
ETextStyle
==============================================================================*/

typedef enum {
    ETextStyleNormal     =  0x00,
    ETextStyleItalic     =  0x01,
    ETextStyleBold       =  0x02,
    ETextStyleBoldItalic =  0x03, // Italic|Bold
    ETextStyleCount
}   ETextStyle;

ZSDRAWDLL_API QString textStyle2Str( int i_textStyle );
ZSDRAWDLL_API ETextStyle str2TextStyle( const QString& i_str );


/*==============================================================================
ETextSize
==============================================================================*/

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

ZSDRAWDLL_API int textSize2SizeInPixels( int i_textSize );
ZSDRAWDLL_API QString textSize2Str( int i_textSize );
ZSDRAWDLL_API ETextSize str2TextSize( const QString& i_str );


/*==============================================================================
ETextEffect
==============================================================================*/

typedef enum {
    ETextEffectNone               = 0x00,
    ETextEffectStrikeout          = 0x01,
    ETextEffectUnderline          = 0x02,
    ETextEffectStrikeoutUnderline = 0x03, // Strikeout|Underline
    ETextEffectCount
}   ETextEffect;

ZSDRAWDLL_API QString textEffect2Str( int i_textEffect );
ZSDRAWDLL_API ETextEffect str2TextEffect( const QString& i_str );


/*==============================================================================
EFillStyle
==============================================================================*/

typedef enum {
    EFillStyleNoFill                 =  0,
    EFillStyleNone                   =  0, // Alias for "NoFill"
    EFillStyleSolidPattern           =  1,
    EFillStyleDense1Pattern          =  2,
    EFillStyleDense2Pattern          =  3,
    EFillStyleDense3Pattern          =  4,
    EFillStyleDense4Pattern          =  5,
    EFillStyleDense5Pattern          =  6,
    EFillStyleDense6Pattern          =  7,
    EFillStyleDense7Pattern          =  8,
    EFillStyleHorPattern             =  9,
    EFillStyleVerPattern             = 10,
    EFillStyleCrossPattern           = 11,
    EFillStyleBDiagPattern           = 12,
    EFillStyleFDiagPattern           = 13,
    EFillStyleDiagCrossPattern       = 14,
    EFillStyleLinearGradientPattern  = 15,
    EFillStyleRadialGradientPattern  = 16,
    EFillStyleConicalGradientPattern = 17,
    EFillStyleTexturePattern         = 18,
    EFillStyleCount,
    EFillStyleDefault,
    EFillStyleUndefined
}   EFillStyle;

ZSDRAWDLL_API Qt::BrushStyle fillStyle2QtBrushStyle( int i_fillStyle );
ZSDRAWDLL_API bool isFillStyleGradientPattern( int i_fillStyle );
ZSDRAWDLL_API QString fillStyle2Str( int i_fillStyle );
ZSDRAWDLL_API EFillStyle str2FillStyle( const QString& i_str );


/*==============================================================================
ELineStyle
==============================================================================*/

typedef enum {
    ELineStyleNoLine         = 0,
    ELineStyleSolidLine      = 1,
    ELineStyleDashLine       = 2,
    ELineStyleDotLine        = 3,
    ELineStyleDashDotLine    = 4,
    ELineStyleDashDotDotLine = 5,
    ELineStyleCount,
    ELineStyleDefault,
    ELineStyleUndefined
}   ELineStyle;

ZSDRAWDLL_API Qt::PenStyle lineStyle2QtPenStyle( int i_lineStyle );
ZSDRAWDLL_API QString lineStyle2Str( int i_lineStyle );
ZSDRAWDLL_API ELineStyle str2LineStyle( const QString& i_str );


/*==============================================================================
ELineRecordType
==============================================================================*/

typedef enum {
    ELineRecordTypeNormal = 0,
    ELineRecordTypeDouble = 1,
    ELineRecordTypeTriple = 2,
    ELineRecordTypeCount,
    ELineRecordTypeDefault,
    ELineRecordTypeUndefined
}   ELineRecordType;

ZSDRAWDLL_API QString lineRecordType2Str( int i_lineRecordType );
ZSDRAWDLL_API ELineRecordType str2LineRecordType( const QString& i_str );


/*==============================================================================
ELineEndStyle
==============================================================================*/

typedef enum {
    ELineEndStyleNormal    = 0,
    ELineEndStyleArrowHead = 1,
    ELineEndStyleCount,
    ELineEndStyleDefault,
    ELineEndStyleUndefined
}   ELineEndStyle;

ZSDRAWDLL_API QString lineEndStyle2Str( int i_lineEndStyle );
ZSDRAWDLL_API ELineEndStyle str2LineEndStyle( const QString& i_str );


/*==============================================================================
ELineEndFillStyle
==============================================================================*/

typedef enum {
    ELineEndFillStyleNoFill       = 0,
    ELineEndFillStyleSolidPattern = 1,
    ELineEndFillStyleCount,
    ELineEndFillStyleDefault,
    ELineEndFillStyleUndefined
}   ELineEndFillStyle;

ZSDRAWDLL_API Qt::BrushStyle lineEndFillStyle2QtBrushStyle( int i_fillStyle );
ZSDRAWDLL_API QString lineEndFillStyle2Str( int i_lineEndFillStyle );
ZSDRAWDLL_API ELineEndFillStyle str2LineEndFillStyle( const QString& i_str );


/*==============================================================================
ELineEndBaseLineType
==============================================================================*/

typedef enum {
    ELineEndBaseLineTypeNoLine   = 0,
    ELineEndBaseLineTypeNormal   = 1, // straight lined
    ELineEndBaseLineTypeIndented = 2,
    ELineEndBaseLineTypeCount,
    ELineEndBaseLineTypeDefault,
    ELineEndBaseLineTypeUndefined
}   ELineEndBaseLineType;

ZSDRAWDLL_API QString lineEndBaseLineType2Str( int i_lineEndBaseLineType );
ZSDRAWDLL_API ELineEndBaseLineType str2LineEndBaseLineType( const QString& i_str );


/*==============================================================================
ELineEndWidth
==============================================================================*/

typedef enum {
    ELineEndWidthThin   = 0,    //  7 Pixel
    ELineEndWidthMedium = 1,    // 13 Pixel
    ELineEndWidthWide   = 2,    // 19 Pixel
    ELineEndWidthCount,
    ELineEndWidthDefault,
    ELineEndWidthUndefined
}   ELineEndWidth;

ZSDRAWDLL_API double lineEndWidth2dy( int i_lineEndWidth );
ZSDRAWDLL_API QString lineEndWidth2Str( int i_lineEndWidth );
ZSDRAWDLL_API ELineEndWidth str2LineEndWidth( const QString& i_str );


/*==============================================================================
ELineEndLength
==============================================================================*/

typedef enum {
    ELineEndLengthShort  = 0,   //  7 Pixel
    ELineEndLengthMedium = 1,   // 10 Pixel
    ELineEndLengthLong   = 2,   // 13 Pixel
    ELineEndLengthCount,
    ELineEndLengthDefault,
    ELineEndLengthUndefined
}   ELineEndLength;

ZSDRAWDLL_API double lineEndLength2dx( int i_lineEndLength );
ZSDRAWDLL_API QString lineEndLength2Str( int i_lineEndLength );
ZSDRAWDLL_API ELineEndLength str2LineEndLength( const QString& i_str );


/*==============================================================================
EAlignmentRefLine
==============================================================================*/

typedef enum {
    EAlignmentRefLeft     = 0,
    EAlignmentRefHCenter  = 1,
    EAlignmentRefRight    = 2,
    EAlignmentRefTop      = 3,
    EAlignmentRefVCenter  = 4,
    EAlignmentRefBottom   = 5,
    EAlignmentRefWidth    = 6,
    EAlignmentRefHeight   = 7,
    EAlignmentRefCount,
    EAlignmentRefUndefined
}   EAlignmentRef;

ZSDRAWDLL_API QString alignmentRef2Str( int i_ref, bool i_bShort = false );
ZSDRAWDLL_API EAlignmentRef str2AlignmentRef( const QString& i_str );


//==============================================================================
struct ZSDRAWDLL_API SGraphObjAlignment
//==============================================================================
{
public: // ctor
    SGraphObjAlignment() :
        m_alignmentRefChild(EAlignmentRefUndefined),
        m_alignmentRefParent(EAlignmentRefUndefined),
        m_bAlignAbsolute(true),
        m_fVal(0.0)
    {
    }
    SGraphObjAlignment(
        EAlignmentRef i_refChild,
        EAlignmentRef i_refParent,
        bool          i_bAbsolute,
        double        i_fVal = 0.0 ) :
        m_alignmentRefChild(i_refChild),
        m_alignmentRefParent(i_refParent),
        m_bAlignAbsolute(i_bAbsolute),
        m_fVal(i_fVal)
    {
    }
public: // struct members
    EAlignmentRef m_alignmentRefChild;
    EAlignmentRef m_alignmentRefParent;
    bool          m_bAlignAbsolute;
    double        m_fVal; // if aligned absolute value in pixels, if aligned relative scale factor to width or height of parent (usually ranging from 0..1)

}; // struct SGraphObjAlignment

ZSDRAWDLL_API QString graphObjAlignment2Str( const SGraphObjAlignment& i_alignment );


//==============================================================================
struct ZSDRAWDLL_API SGraphObjHitInfo
//==============================================================================
{
public: // ctor
    SGraphObjHitInfo();
public: // struct methods
    bool isBoundingRectSelectionPointHit() const;
    bool isSelectionPointHit() const;
    bool isPolygonShapePointHit() const;
    bool isLineSegmentHit() const;
public: // struct methods
    void setCursor( double i_fGraphObjRotAngle_rad ); // calculated depending on editMode, editResizeMode and selected point (which must have been set before)
public: // struct members
    EEditMode       m_editMode;
    EEditResizeMode m_editResizeMode;
    ESelectionPoint m_selPtBoundingRect;
    int             m_idxPolygonShapePoint;
    int             m_idxLineSegment;
    QPointF         m_ptSelected;
    QCursor         m_cursor;

}; // struct SGraphObjHitInfo


//==============================================================================
struct ZSDRAWDLL_API SGraphObjLabel
//==============================================================================
{
public: // ctors
    SGraphObjLabel() :
        m_strKey(),
        m_strText(),
        m_selPt(ESelectionPointUndefined),
        m_sizDist(),
        m_bDistValid(false),
        m_bVisible(false),
        m_pGraphObjLabel(nullptr)
    {
    }
    SGraphObjLabel( const QString& i_strKey, const QString& i_strText, ESelectionPoint i_selPt = ESelectionPointTopCenter ) :
        m_strKey(i_strKey),
        m_strText(i_strText),
        m_selPt(i_selPt),
        m_sizDist(),
        m_bDistValid(false),
        m_bVisible(false),
        m_pGraphObjLabel(nullptr)
    {
    }
public: // struct members
    QString         m_strKey;
    QString         m_strText;
    ESelectionPoint m_selPt;        // Selection point of the parent item the label is aligned to.
    QSizeF          m_sizDist;      // Distance between the scene position of the label and selection point of parent item.
    bool            m_bDistValid;   // If the graphic item is created for the first time the distance will be calculated and stored for following show events.
    bool            m_bVisible;
    CGraphObjLabel* m_pGraphObjLabel;

}; // struct SGraphObjLabel


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
    const QRectF&    i_rect,
    ESelectionPoint  i_selPt,
    const QPointF&   i_ptSel,
    ESelectionPoint* o_pSelPt = nullptr );

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

#endif // #ifndef ZSDraw_DllMain_h
