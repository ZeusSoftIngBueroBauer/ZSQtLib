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

//#include <QtCore/qglobal.h>
//#include <QtGui/QBitmap>

#include "ZSDraw/ZSDrawCommon.h"
//#include "ZSDraw/ZSDrawGraphObjRect.h"
//#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
//#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysEnumEntry.h"
//#include "ZSSys/ZSSysMath.h"
//#include "ZSSys/ZSSysVersion.h"

//#ifdef _WINDOWS
//// As "min" will be defined as a macro with two arguments and qdatetime uses "min"
//// as a function with no arguments "windows.h" must be included after qdatetime
//// (which is included by ZSSysTime (which again is included by ZSSysAux)).
//#include <windows.h>
//#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
//using namespace ZS::PhysVal;
using namespace ZS::Draw;


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

/*==============================================================================
Enum EPaperFormat
==============================================================================*/

template<> QMutex ZS::System::CEnum<EPaperFormat>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> ZS::System::CEnum<EPaperFormat>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> ZS::System::CEnum<EPaperFormat>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                                Name,      Symbol, Text,  SCIPShort, SCPILong, Value
    /* 0 */ SEnumEntry( static_cast<int>(EPaperFormat::DinA5),       "Din A5",       QSizeF(c_fPaperFormatWidthDinA5_mm, c_fPaperFormatHeightDinA5_mm) ),
    /* 1 */ SEnumEntry( static_cast<int>(EPaperFormat::DinA4),       "Din A4",       QSizeF(c_fPaperFormatWidthDinA4_mm, c_fPaperFormatHeightDinA4_mm) ),
    /* 2 */ SEnumEntry( static_cast<int>(EPaperFormat::DinA3),       "Din A3",       QSizeF(c_fPaperFormatWidthDinA3_mm, c_fPaperFormatHeightDinA3_mm) ),
    /* 3 */ SEnumEntry( static_cast<int>(EPaperFormat::DinA2),       "Din A2",       QSizeF(c_fPaperFormatWidthDinA2_mm, c_fPaperFormatHeightDinA2_mm) ),
    /* 4 */ SEnumEntry( static_cast<int>(EPaperFormat::UserDefined), "User Defined", QSizeF() )
};


/*==============================================================================
Enum EPaperOrientation
==============================================================================*/

template<> QMutex ZS::System::CEnum<EPaperOrientation>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> ZS::System::CEnum<EPaperOrientation>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> ZS::System::CEnum<EPaperOrientation>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                                  Name,      Symbol, Text,  SCIPShort, SCPILong, Value
    /* 0 */ SEnumEntry( static_cast<int>(EPaperOrientation::Portrait),  "Portrait"  ),
    /* 1 */ SEnumEntry( static_cast<int>(EPaperOrientation::Landscape), "Landscape" )
};


/*==============================================================================
Enum EPrinterPaperSource
==============================================================================*/

template<> QMutex ZS::System::CEnum<EPrinterPaperSource>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> ZS::System::CEnum<EPrinterPaperSource>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> ZS::System::CEnum<EPrinterPaperSource>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                                      Name,      Symbol, Text,  SCIPShort, SCPILong, Value
    /* 0 */ SEnumEntry( static_cast<int>(EPrinterPaperSource::AutoSelect), "Auto Select" ),
    /* 1 */ SEnumEntry( static_cast<int>(EPrinterPaperSource::ManualFeed), "Manual Feed" ),
    /* 2 */ SEnumEntry( static_cast<int>(EPrinterPaperSource::Tray1),      "Tray 1"      ),
    /* 3 */ SEnumEntry( static_cast<int>(EPrinterPaperSource::Tray2),      "Tray 2"      )
};


/*==============================================================================
Enum EGraphObjType
==============================================================================*/

//------------------------------------------------------------------------------
const SEnumEntry s_arEnumStrGraphObjType[EGraphObjTypeCount] =
//------------------------------------------------------------------------------
{                       // Enumerator,                 Name,              Symbol, Text,  SCIPShort, SCPILong, Value
    /* 65536 */ SEnumEntry( EGraphObjTypePoint,           "Point",           "Point"            ),
    /* 65537 */ SEnumEntry( EGraphObjTypeLine,            "Line",            "Line"             ),
    /* 65538 */ SEnumEntry( EGraphObjTypeRect,            "Rect",            "Rectangle"        ),
    /* 65539 */ SEnumEntry( EGraphObjTypeEllipse,         "Ellipse",         "Ellipse"          ),
    /* 65540 */ SEnumEntry( EGraphObjTypePolygon,         "Polygon",         "Polygon"          ),
    /* 65541 */ SEnumEntry( EGraphObjTypePolyline,        "Polyline",        "Polyline"         ),
    /* 65542 */ SEnumEntry( EGraphObjTypeText,            "Text",            "Text"             ),
    /* 65543 */ SEnumEntry( EGraphObjTypeImage,           "Image",           "Image"            ),
    /* 65544 */ SEnumEntry( EGraphObjTypeConnectionPoint, "ConnectionPoint", "Connection Point" ),
    /* 65545 */ SEnumEntry( EGraphObjTypeConnectionLine,  "ConnectionLine",  "Connection Line"  ),
    /* 65546 */ SEnumEntry( EGraphObjTypeGroup,           "Group",           "Group"            )
};

//------------------------------------------------------------------------------
const SEnumEntry s_arEnumStrGraphObjClassName[EGraphObjTypeCount] =
//------------------------------------------------------------------------------
{
    /*  0 */ SEnumEntry( EGraphObjTypePoint,           "CGraphObjPoint"           ),
    /*  1 */ SEnumEntry( EGraphObjTypeLine,            "CGraphObjLine"            ),
    /*  2 */ SEnumEntry( EGraphObjTypeRect,            "CGraphObjRect"            ),
    /*  3 */ SEnumEntry( EGraphObjTypeEllipse,         "CGraphObjEllipse"         ),
    /*  4 */ SEnumEntry( EGraphObjTypePolygon,         "CGraphObjPolygon"         ),
    /*  5 */ SEnumEntry( EGraphObjTypePolyline,        "CGraphObjPolyline"        ),
    /*  6 */ SEnumEntry( EGraphObjTypeText,            "CGraphObjText"            ),
    /*  7 */ SEnumEntry( EGraphObjTypeImage,           "CGraphObjImage"           ),
    /*  8 */ SEnumEntry( EGraphObjTypeConnectionPoint, "CGraphObjConnectionPoint" ),
    /*  9 */ SEnumEntry( EGraphObjTypeConnectionLine,  "CGraphObjConnectionLine"  ),
    /* 10 */ SEnumEntry( EGraphObjTypeGroup,           "CGraphObjGroup"           )
};

//------------------------------------------------------------------------------
QString ZS::Draw::graphObjType2Str( int i_graphObjType )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_graphObjType == EGraphObjTypeSelectionPoint )
    {
        str = "SelectionPoint";
    }
    else if( i_graphObjType == EGraphObjTypeLabel )
    {
        str = "Label";
    }
    else if ( i_graphObjType >= EGraphObjTypeMin && i_graphObjType <= EGraphObjTypeMax )
    {
        str = SEnumEntry::enumerator2Str(s_arEnumStrGraphObjType, _ZSArrLen(s_arEnumStrGraphObjType), i_graphObjType-EGraphObjTypeMin);
    }
    else
    {
        str = "? (" + QString::number(i_graphObjType) + ")";
    }
    return str;

} // graphObjType2Str

//------------------------------------------------------------------------------
EGraphObjType ZS::Draw::str2GraphObjType( const QString& i_str )
//------------------------------------------------------------------------------
{
    EGraphObjType graphObjType = EGraphObjTypeUndefined;
    
    int iGraphObjType = SEnumEntry::str2Enumerator(s_arEnumStrGraphObjType, _ZSArrLen(s_arEnumStrGraphObjType), i_str);

    if( iGraphObjType >= 0 && iGraphObjType < _ZSArrLen(s_arEnumStrGraphObjType) )
    {
        graphObjType = static_cast<EGraphObjType>(iGraphObjType);
    }
    else
    {
        bool bConverted;
        iGraphObjType = i_str.toInt(&bConverted);
        if( bConverted && iGraphObjType >= 0 && iGraphObjType < _ZSArrLen(s_arEnumStrGraphObjType) )
        {
            graphObjType = static_cast<EGraphObjType>(iGraphObjType);
        }
    }
    return graphObjType;

} // str2GraphObjType

//------------------------------------------------------------------------------
QString ZS::Draw::graphObjType2ClassName( int i_graphObjType )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_graphObjType == EGraphObjTypeSelectionPoint )
    {
        str = "CGraphObjSelectionPoint";
    }
    else if( i_graphObjType == EGraphObjTypeLabel )
    {
        str = "CGraphObjLabel";
    }
    else
    {
        str = SEnumEntry::enumerator2Str(s_arEnumStrGraphObjClassName,_ZSArrLen(s_arEnumStrGraphObjClassName),i_graphObjType);
    }
    return str;

} // graphObjType2ClassName

//------------------------------------------------------------------------------
EGraphObjType ZS::Draw::className2GraphObjType( const QString& i_str )
//------------------------------------------------------------------------------
{
    EGraphObjType graphObjType = EGraphObjTypeUndefined;
    int iGraphObjType = SEnumEntry::str2Enumerator(s_arEnumStrGraphObjClassName,_ZSArrLen(s_arEnumStrGraphObjClassName),i_str);
    if( iGraphObjType >= 0 && iGraphObjType < _ZSArrLen(s_arEnumStrGraphObjClassName) )
    {
        graphObjType = static_cast<EGraphObjType>(iGraphObjType);
    }
    else
    {
        bool bConverted;
        iGraphObjType = i_str.toInt(&bConverted);
        if( bConverted && iGraphObjType >= 0 && iGraphObjType < _ZSArrLen(s_arEnumStrGraphObjClassName) )
        {
            graphObjType = static_cast<EGraphObjType>(iGraphObjType);
        }
    }
    return graphObjType;

} // str2GraphObjClassName


/*==============================================================================
Enum EEditTool
==============================================================================*/

template<> QMutex ZS::System::CEnum<EEditTool>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> ZS::System::CEnum<EEditTool>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> ZS::System::CEnum<EEditTool>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                              Name,      Symbol, Text,  SCIPShort, SCPILong, Value
    /* 0 */ SEnumEntry( static_cast<int>(EEditTool::Select),        "Select"        ),
    /* 1 */ SEnumEntry( static_cast<int>(EEditTool::CreateObjects), "CreateObjects" ),
    /* 2 */ SEnumEntry( static_cast<int>(EEditTool::Ignore),        "Ignore"        ),
    /* 3 */ SEnumEntry( static_cast<int>(EEditTool::Undefined),     "Undefined"     )
};


/*==============================================================================
Enum ECoordinatesVersion
==============================================================================*/

template<> QMutex ZS::System::CEnum<ECoordinatesVersion>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> ZS::System::CEnum<ECoordinatesVersion>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> ZS::System::CEnum<ECoordinatesVersion>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                                    Name,      Symbol, Text,  SCIPShort, SCPILong, Value
    /* 0 */ SEnumEntry( static_cast<int>(ECoordinatesVersion::Original),  "Original"  ),
    /* 1 */ SEnumEntry( static_cast<int>(ECoordinatesVersion::Current),   "Current"   )
};


/*==============================================================================
Enum EEditMode
==============================================================================*/

template<> QMutex ZS::System::CEnum<EEditMode>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> ZS::System::CEnum<EEditMode>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> ZS::System::CEnum<EEditMode>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                                Name,      Symbol, Text,  SCIPShort, SCPILong, Value
    /* 0 */ SEnumEntry( static_cast<int>(EEditMode::Creating),       "Creating"       ),
    /* 1 */ SEnumEntry( static_cast<int>(EEditMode::Move),           "Move"           ),
    /* 2 */ SEnumEntry( static_cast<int>(EEditMode::Resize),         "Resize"         ),
    /* 3 */ SEnumEntry( static_cast<int>(EEditMode::Rotate),         "Rotate"         ),
    /* 4 */ SEnumEntry( static_cast<int>(EEditMode::MoveShapePoint), "MoveShapePoint" ),
    /* 5 */ SEnumEntry( static_cast<int>(EEditMode::EditText),       "EditText"       ),
    /* 6 */ SEnumEntry( static_cast<int>(EEditMode::Ignore),         "Ignore"         ),
    /* 7 */ SEnumEntry( static_cast<int>(EEditMode::Undefined),      "Undefined"      )
};


/*==============================================================================
Enum EEditResizeMode
==============================================================================*/

template<> QMutex ZS::System::CEnum<EEditResizeMode>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> ZS::System::CEnum<EEditResizeMode>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> ZS::System::CEnum<EEditResizeMode>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                                  Name,      Symbol, Text,  SCIPShort, SCPILong, Value
    /* 0 */ SEnumEntry( static_cast<int>(EEditResizeMode::ResizeAll), "ResizeAll" ),
    /* 1 */ SEnumEntry( static_cast<int>(EEditResizeMode::ResizeHor), "ResizeHor" ),
    /* 2 */ SEnumEntry( static_cast<int>(EEditResizeMode::ResizeVer), "ResizeVer" ),
    /* 3 */ SEnumEntry( static_cast<int>(EEditResizeMode::Ignore),    "Ignore"    ),
    /* 4 */ SEnumEntry( static_cast<int>(EEditResizeMode::Undefined), "Undefined" )
};


/*==============================================================================
Enum ESelectionPointType
==============================================================================*/

template<> QMutex ZS::System::CEnum<ESelectionPointType>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> ZS::System::CEnum<ESelectionPointType>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> ZS::System::CEnum<ESelectionPointType>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                                            Name,      Symbol, Text,  SCIPShort, SCPILong, Value
    /* 0 */ SEnumEntry( static_cast<int>(ESelectionPointType::BoundingRectangle), "BoundingRect"      ),
    /* 1 */ SEnumEntry( static_cast<int>(ESelectionPointType::PolygonShapePoint), "PolygonShapePoint" )
};


/*==============================================================================
Enum ESelectionPoints
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Draw::selectionPoints2Str( int i_selPts )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_selPts & ESelectionPointsBoundingRectCorner )
    {
        str += "BoundingRectCorners";
    }
    if( i_selPts & ESelectionPointsBoundingRectLineCenter )
    {
        if( !str.isEmpty() )
        {
            str += "|";
        }
        str += "BoundingRectLineCenter";
    }
    if( i_selPts & ESelectionPointsBoundingRectCenter )
    {
        if( !str.isEmpty() )
        {
            str += "|";
        }
        str += "BoundingRectCenter";
    }
    if( i_selPts & ESelectionPointsBoundingRectRotate )
    {
        if( !str.isEmpty() )
        {
            str += "|";
        }
        str += "BoundingRectRotation";
    }
    if( i_selPts & ESelectionPointsPolygonShapePoints )
    {
        if( !str.isEmpty() )
        {
            str += "|";
        }
        str += "PolygonShapePoints";
    }
    return str;

} // selectionPoints2Str


/*==============================================================================
Enum ESelectionPoint
==============================================================================*/

template<> QMutex ZS::System::CEnum<ESelectionPoint>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> ZS::System::CEnum<ESelectionPoint>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

static double s_fSelectionPointRotateDistance_px = 20.0;

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> ZS::System::CEnum<ESelectionPoint>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                                  Name,      Symbol, Text,  SCIPShort, SCPILong, Value
    /*  0 */ SEnumEntry( static_cast<int>(ESelectionPoint::TopLeft),      "TL", "TopLeft"      ),
    /*  1 */ SEnumEntry( static_cast<int>(ESelectionPoint::TopRight),     "TR", "TopRight"     ),
    /*  2 */ SEnumEntry( static_cast<int>(ESelectionPoint::BottomRight),  "BR", "BottomRight"  ),
    /*  3 */ SEnumEntry( static_cast<int>(ESelectionPoint::BottomLeft),   "BL", "BottomLeft"   ),
    /*  4 */ SEnumEntry( static_cast<int>(ESelectionPoint::TopCenter),    "TC", "TopCenter"    ),
    /*  5 */ SEnumEntry( static_cast<int>(ESelectionPoint::RightCenter),  "RC", "RightCenter"  ),
    /*  6 */ SEnumEntry( static_cast<int>(ESelectionPoint::BottomCenter), "BC", "BottomCenter" ),
    /*  7 */ SEnumEntry( static_cast<int>(ESelectionPoint::LeftCenter),   "LC", "LeftCenter"   ),
    /*  8 */ SEnumEntry( static_cast<int>(ESelectionPoint::Center),       "C",  "Center"       ),
    /*  9 */ SEnumEntry( static_cast<int>(ESelectionPoint::RotateTop),    "OT", "RotateTop"    ),
    /* 10 */ SEnumEntry( static_cast<int>(ESelectionPoint::RotateBottom), "OB", "RotateBottom" ),
    /* 11 */ SEnumEntry( static_cast<int>(ESelectionPoint::Undefined),    "?",  "Undefined"    )
};

//------------------------------------------------------------------------------
void ZS::Draw::setSelectionPointRotateDistance( double i_fDist_px )
//------------------------------------------------------------------------------
{
    s_fSelectionPointRotateDistance_px = i_fDist_px;
}

//------------------------------------------------------------------------------
double ZS::Draw::getSelectionPointRotateDistance()
//------------------------------------------------------------------------------
{
    return s_fSelectionPointRotateDistance_px;
}

//------------------------------------------------------------------------------
QPointF ZS::Draw::getSelectionPoint( const QLineF& i_lin, ESelectionPoint /*i_selPt*/ )
//------------------------------------------------------------------------------
{
    double fx = i_lin.p1().x() + i_lin.dx()/2.0;
    double fy = i_lin.p1().y() + i_lin.dy()/2.0;

    QPointF pt(fx,fy);

    return pt;

} // getSelectionPoint

//------------------------------------------------------------------------------
QPointF ZS::Draw::getSelectionPoint( const QRectF& i_rct, ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    QPointF pt = i_rct.center();

    switch( i_selPt )
    {
        case ESelectionPoint::TopLeft:
        {
            pt = i_rct.topLeft();
            break;
        }
        case ESelectionPoint::TopRight:
        {
            pt = i_rct.topRight();
            break;
        }
        case ESelectionPoint::BottomRight:
        {
            pt = i_rct.bottomRight();
            break;
        }
        case ESelectionPoint::BottomLeft:
        {
            pt = i_rct.bottomLeft();
            break;
        }
        case ESelectionPoint::TopCenter:
        {
            pt = QPointF( i_rct.center().x(), i_rct.top() );
            break;
        }
        case ESelectionPoint::RightCenter:
        {
            pt = QPointF( i_rct.right(), i_rct.center().y() );
            break;
        }
        case ESelectionPoint::BottomCenter:
        {
            pt = QPointF( i_rct.center().x(), i_rct.bottom() );
            break;
        }
        case ESelectionPoint::LeftCenter:
        {
            pt = QPointF( i_rct.left(), i_rct.center().y() );
            break;
        }
        case ESelectionPoint::Center:
        {
            pt = i_rct.center();
            break;
        }
        case ESelectionPoint::RotateTop:
        {
            pt = QPointF( i_rct.center().x(), i_rct.top()-s_fSelectionPointRotateDistance_px );
            break;
        }
        case ESelectionPoint::RotateBottom:
        {
            pt = QPointF( i_rct.center().x(), i_rct.bottom()+s_fSelectionPointRotateDistance_px );
            break;
        }
        default:
        {
            break;
        }
    } // switch( i_selPt )

    return pt;

} // getSelectionPoint

//------------------------------------------------------------------------------
QPointF ZS::Draw::getSelectionPointOpposite( const QRectF& i_rct, ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    QPointF ptOpp = i_rct.center();

    switch( i_selPt )
    {
        case ESelectionPoint::TopLeft:
        {
            ptOpp = i_rct.bottomRight();
            break;
        }
        case ESelectionPoint::TopRight:
        {
            ptOpp = i_rct.bottomLeft();
            break;
        }
        case ESelectionPoint::BottomRight:
        {
            ptOpp = i_rct.topLeft();
            break;
        }
        case ESelectionPoint::BottomLeft:
        {
            ptOpp = i_rct.topRight();
            break;
        }
        case ESelectionPoint::TopCenter:
        {
            ptOpp = QPointF( i_rct.bottom(), i_rct.center().x() );
            break;
        }
        case ESelectionPoint::RightCenter:
        {
            ptOpp = QPointF( i_rct.left(), i_rct.center().y() );
            break;
        }
        case ESelectionPoint::BottomCenter:
        {
            ptOpp = QPointF( i_rct.top(), i_rct.center().x() );
            break;
        }
        case ESelectionPoint::LeftCenter:
        {
            ptOpp = QPointF( i_rct.right(), i_rct.center().y() );
            break;
        }
        case ESelectionPoint::Center:
        {
            ptOpp = i_rct.center();
            break;
        }
        case ESelectionPoint::RotateTop:
        {
            ptOpp = QPointF( i_rct.center().x(), i_rct.bottom()+s_fSelectionPointRotateDistance_px );
            break;
        }
        case ESelectionPoint::RotateBottom:
        {
            ptOpp = QPointF( i_rct.center().x(), i_rct.top()-s_fSelectionPointRotateDistance_px );
            break;
        }
        default:
        {
            break;
        }
    } // switch( i_selPt )

    return ptOpp;

} // getSelectionPointOpposite

//------------------------------------------------------------------------------
ESelectionPoint ZS::Draw::getSelectionPointOpposite( ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    static const ESelectionPoint s_arSelectionPointsOpposite[] =
    {
        /*  0: TopLeft      */ ESelectionPoint::BottomRight,
        /*  1: TopRight     */ ESelectionPoint::BottomLeft,
        /*  2: BottomRight  */ ESelectionPoint::TopLeft,
        /*  3: BottomLeft   */ ESelectionPoint::TopRight,
        /*  4: TopCenter    */ ESelectionPoint::BottomCenter,
        /*  5: RightCenter  */ ESelectionPoint::LeftCenter,
        /*  6: BottomCenter */ ESelectionPoint::TopCenter,
        /*  7: LeftCenter   */ ESelectionPoint::RightCenter,
        /*  8: Center       */ ESelectionPoint::Center,
        /*  9: RotateTop    */ ESelectionPoint::RotateBottom,
        /* 10: RotateBottom */ ESelectionPoint::RotateTop,
        /* 11: Undefined    */ ESelectionPoint::Undefined
    };

    ESelectionPoint selPtOpp = ESelectionPoint::Undefined;

    if( static_cast<int>(i_selPt) >= 0 && static_cast<int>(i_selPt) < _ZSArrLen(s_arSelectionPointsOpposite) )
    {
        selPtOpp = s_arSelectionPointsOpposite[static_cast<int>(i_selPt)];
    }
    return selPtOpp;

} // getSelectionPointOpposite

//------------------------------------------------------------------------------
CEnumEditMode ZS::Draw::selectionPoint2EditMode( ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    const EEditMode s_arEditModes[] =
    {
        /*  0: TopLeft      */ EEditMode::Resize,
        /*  1: TopRight     */ EEditMode::Resize,
        /*  2: BottomRight  */ EEditMode::Resize,
        /*  3: BottomLeft   */ EEditMode::Resize,
        /*  4: TopCenter    */ EEditMode::Resize,
        /*  5: RightCenter  */ EEditMode::Resize,
        /*  6: BottomCenter */ EEditMode::Resize,
        /*  7: LeftCenter   */ EEditMode::Resize,
        /*  8: Center       */ EEditMode::Move,
        /*  9: RotateTop    */ EEditMode::Rotate,
        /* 10: RotateBottom */ EEditMode::Rotate,
        /* 11: Undefined    */ EEditMode::Undefined
    };

    EEditMode editMode = EEditMode::Undefined;

    if( static_cast<int>(i_selPt) >= 0 && static_cast<int>(i_selPt) < _ZSArrLen(s_arEditModes) )
    {
        editMode = s_arEditModes[static_cast<int>(i_selPt)];
    }
    return editMode;

} // selectionPoint2EditMode

//------------------------------------------------------------------------------
CEnumEditResizeMode ZS::Draw::selectionPoint2EditResizeMode( ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    static const EEditResizeMode s_arEditResizeModes[] =
    {
        /*  0: TopLeft      */ EEditResizeMode::ResizeAll,
        /*  1: TopRight     */ EEditResizeMode::ResizeAll,
        /*  2: BottomRight  */ EEditResizeMode::ResizeAll,
        /*  3: BottomLeft   */ EEditResizeMode::ResizeAll,
        /*  4: TopCenter    */ EEditResizeMode::ResizeVer,
        /*  5: RightCenter  */ EEditResizeMode::ResizeHor,
        /*  6: BottomCenter */ EEditResizeMode::ResizeVer,
        /*  7: LeftCenter   */ EEditResizeMode::ResizeHor,
        /*  8: Center       */ EEditResizeMode::Undefined,
        /*  9: RotateTop    */ EEditResizeMode::Undefined,
        /* 10: RotateBottom */ EEditResizeMode::Undefined,
        /* 11: Undefined    */ EEditResizeMode::Undefined
    };

    EEditResizeMode editResizeMode = EEditResizeMode::Undefined;

    if( static_cast<int>(i_selPt) >= 0 && static_cast<int>(i_selPt) < _ZSArrLen(s_arEditResizeModes) )
    {
        editResizeMode = s_arEditResizeModes[static_cast<int>(i_selPt)];
    }
    return editResizeMode;

} // selectionPoint2EditResizeMode

//------------------------------------------------------------------------------
Qt::CursorShape ZS::Draw::selectionPoint2CursorShape( ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    static const Qt::CursorShape s_arCursorShapes[] =
    {
        /*  0: TopLeft      */ Qt::SizeFDiagCursor, /*  \  */
        /*  1: TopRight     */ Qt::SizeBDiagCursor, /*  /  */
        /*  2: BottomRight  */ Qt::SizeFDiagCursor, /*  \  */
        /*  3: BottomLeft   */ Qt::SizeBDiagCursor, /*  /  */
        /*  4: TopCenter    */ Qt::SizeVerCursor,
        /*  5: RightCenter  */ Qt::SizeHorCursor,
        /*  6: BottomCenter */ Qt::SizeVerCursor,
        /*  7: LeftCenter   */ Qt::SizeHorCursor,
        /*  8: Center       */ Qt::SizeAllCursor,
        /*  9: RotateTop    */ Qt::BitmapCursor,
        /* 10: RotateBottom */ Qt::BitmapCursor,
        /* 11: Undefined    */ Qt::ForbiddenCursor
    };

    Qt::CursorShape cursorShape = Qt::ArrowCursor;

    if( static_cast<int>(i_selPt) >= 0 && static_cast<int>(i_selPt) < _ZSArrLen(s_arCursorShapes) )
    {
        cursorShape = s_arCursorShapes[static_cast<int>(i_selPt)];
    }
    return cursorShape;

} // selectionPoint2CursorShape


/*==============================================================================
Enum ELinePoint
==============================================================================*/

template<> QMutex ZS::System::CEnum<ELinePoint>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> ZS::System::CEnum<ELinePoint>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> ZS::System::CEnum<ELinePoint>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                           Name,        Symbol, Text,  SCIPShort, SCPILong, Value
    /* 0 */ SEnumEntry( static_cast<int>(ELinePoint::Start),     "Start",     "Start" ),
    /* 1 */ SEnumEntry( static_cast<int>(ELinePoint::End),       "End",       "End"   ),
    /* 2 */ SEnumEntry( static_cast<int>(ELinePoint::Undefined), "Undefined",  "?"    )
};


/*==============================================================================
Enum ETextStyle
==============================================================================*/

template<> QMutex ZS::System::CEnum<ETextStyle>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> ZS::System::CEnum<ETextStyle>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> ZS::System::CEnum<ETextStyle>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                            Name,         Symbol, Text,  SCIPShort, SCPILong, Value
    /* 0 */ SEnumEntry( static_cast<int>(ETextStyle::Normal),     "Normal",     "Normal"      ),
    /* 1 */ SEnumEntry( static_cast<int>(ETextStyle::Italic),     "Italic",     "Italic"      ),
    /* 2 */ SEnumEntry( static_cast<int>(ETextStyle::Bold),       "Bold",       "Bold"        ),
    /* 3 */ SEnumEntry( static_cast<int>(ETextStyle::BoldItalic), "BoldItalic", "Bold Italic" )
};


/*==============================================================================
Enum ETextSize
==============================================================================*/

//------------------------------------------------------------------------------
const int s_ariTextSizes_px[] =
//------------------------------------------------------------------------------
{
    /*  0: ETextSize6  */  6,
    /*  1: ETextSize7  */  7,
    /*  2: ETextSize8  */  8,
    /*  3: ETextSize9  */  9,
    /*  4: ETextSize10 */ 10,
    /*  5: ETextSize11 */ 11,
    /*  6: ETextSize12 */ 12,
    /*  7: ETextSize14 */ 14,
    /*  8: ETextSize16 */ 16,
    /*  9: ETextSize18 */ 18,
    /* 10: ETextSize20 */ 20,
    /* 11: ETextSize22 */ 22,
    /* 12: ETextSize24 */ 24,
    /* 13: ETextSize26 */ 26,
    /* 14: ETextSize28 */ 28,
    /* 15: ETextSize36 */ 36,
    /* 16: ETextSize48 */ 48,
    /* 17: ETextSize72 */ 72
};

//------------------------------------------------------------------------------
const ZS::System::SEnumEntry s_arEnumStrTextSize[] =
//------------------------------------------------------------------------------
{                       // Enumerator,                                  Name,      Symbol, Text,  SCIPShort, SCPILong, Value
    /*  0 */ SEnumEntry( ETextSize6,   "6" ),
    /*  1 */ SEnumEntry( ETextSize7,   "7" ),
    /*  2 */ SEnumEntry( ETextSize8,   "8" ),
    /*  3 */ SEnumEntry( ETextSize9,   "9" ),
    /*  4 */ SEnumEntry( ETextSize10, "10" ),
    /*  5 */ SEnumEntry( ETextSize11, "11" ),
    /*  6 */ SEnumEntry( ETextSize12, "12" ),
    /*  7 */ SEnumEntry( ETextSize14, "14" ),
    /*  8 */ SEnumEntry( ETextSize16, "16" ),
    /*  9 */ SEnumEntry( ETextSize18, "18" ),
    /* 10 */ SEnumEntry( ETextSize20, "20" ),
    /* 11 */ SEnumEntry( ETextSize22, "22" ),
    /* 12 */ SEnumEntry( ETextSize24, "24" ),
    /* 13 */ SEnumEntry( ETextSize26, "26" ),
    /* 14 */ SEnumEntry( ETextSize28, "28" ),
    /* 15 */ SEnumEntry( ETextSize36, "36" ),
    /* 16 */ SEnumEntry( ETextSize48, "48" ),
    /* 17 */ SEnumEntry( ETextSize72, "72" )
};

//------------------------------------------------------------------------------
int ZS::Draw::textSize2SizeInPixels( ETextSize i_textSize )
//------------------------------------------------------------------------------
{
    int iSize_px = 6;

    if( i_textSize >= 0 && i_textSize < ETextSizeCount )
    {
        return iSize_px = s_ariTextSizes_px[i_textSize];
    }
    return iSize_px;

} // textSize2SizeInPixels

//------------------------------------------------------------------------------
QString ZS::Draw::textSize2Str( ETextSize i_textSize )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrTextSize,_ZSArrLen(s_arEnumStrTextSize),i_textSize);
}

//------------------------------------------------------------------------------
ETextSize ZS::Draw::str2TextSize( const QString& i_str )
//------------------------------------------------------------------------------
{
    ETextSize textSize = ETextSize11;
    int       iTextSize = SEnumEntry::str2Enumerator(s_arEnumStrTextSize,_ZSArrLen(s_arEnumStrTextSize),i_str);

    if( iTextSize >= 0 && iTextSize < _ZSArrLen(s_arEnumStrTextSize) )
    {
        textSize = static_cast<ETextSize>(iTextSize);
    }
    else
    {
        bool bConverted;
        iTextSize = i_str.toInt(&bConverted);
        if( bConverted && iTextSize >= 0 && iTextSize < _ZSArrLen(s_arEnumStrTextSize) )
        {
            textSize = static_cast<ETextSize>(iTextSize);
        }
    }
    return textSize;

} // str2TextSize


/*==============================================================================
Enum ETextEffect
==============================================================================*/

template<> QMutex ZS::System::CEnum<ETextEffect>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> ZS::System::CEnum<ETextEffect>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> ZS::System::CEnum<ETextEffect>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                                     Name,      Symbol, Text,  SCIPShort, SCPILong, Value
    /* 0 */ SEnumEntry( static_cast<int>(ETextEffect::None),               "None",               "None"               ),
    /* 1 */ SEnumEntry( static_cast<int>(ETextEffect::Strikeout),          "Strikeout",          "Strikeout"          ),
    /* 2 */ SEnumEntry( static_cast<int>(ETextEffect::Underline),          "Underline",          "Underline"          ),
    /* 3 */ SEnumEntry( static_cast<int>(ETextEffect::StrikeoutUnderline), "StrikeoutUnderline", "StrikeoutUnderline" )
};


/*==============================================================================
Enum EFillStyle
==============================================================================*/

template<> QMutex ZS::System::CEnum<EFillStyle>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> ZS::System::CEnum<EFillStyle>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

const Qt::BrushStyle s_arFillStyles2QtBrushStyles[] =
{
    /*  0: None                     */ Qt::NoBrush,
    /*  1: SolidPattern             */ Qt::SolidPattern,
    /*  2: Dense1Pattern            */ Qt::Dense1Pattern,
    /*  3: Dense2Pattern            */ Qt::Dense2Pattern,
    /*  4: Dense3Pattern            */ Qt::Dense3Pattern,
    /*  5: Dense4Pattern            */ Qt::Dense4Pattern,
    /*  6: Dense5Pattern            */ Qt::Dense5Pattern,
    /*  7: Dense6Pattern            */ Qt::Dense6Pattern,
    /*  8: Dense7Pattern            */ Qt::Dense7Pattern,
    /*  9: HorPattern               */ Qt::HorPattern,
    /* 10: VerPattern               */ Qt::VerPattern,
    /* 11: CrossPattern             */ Qt::CrossPattern,
    /* 12: BDiagPattern             */ Qt::BDiagPattern,
    /* 13: FDiagPattern             */ Qt::FDiagPattern,
    /* 14: DiagCrossPattern         */ Qt::DiagCrossPattern,
    /* 15: LinearGradientPattern    */ Qt::LinearGradientPattern,
    /* 16: RadialGradientPattern    */ Qt::RadialGradientPattern,
    /* 17: ConicalGradientPattern   */ Qt::ConicalGradientPattern,
    /* 18: TexturePattern           */ Qt::TexturePattern
};

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> ZS::System::CEnum<EFillStyle>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                                  Name,      Symbol, Text,  SCIPShort, SCPILong, Value
    /*  0 */ SEnumEntry( static_cast<int>(EFillStyle::NoFill),                 "NoFill",                  "No Fill"                ),
    /*  1 */ SEnumEntry( static_cast<int>(EFillStyle::SolidPattern),           "SolidPattern",            "Solid Pattern"          ),
    /*  2 */ SEnumEntry( static_cast<int>(EFillStyle::Dense1Pattern),          "Dense1Pattern",           "Dense1Pattern"          ),
    /*  3 */ SEnumEntry( static_cast<int>(EFillStyle::Dense2Pattern),          "Dense2Pattern",           "Dense2Pattern"          ),
    /*  4 */ SEnumEntry( static_cast<int>(EFillStyle::Dense3Pattern),          "Dense3Pattern",           "Dense3Pattern"          ),
    /*  5 */ SEnumEntry( static_cast<int>(EFillStyle::Dense4Pattern),          "Dense4Pattern",           "Dense4Pattern"          ),
    /*  6 */ SEnumEntry( static_cast<int>(EFillStyle::Dense5Pattern),          "Dense5Pattern",           "Dense5Pattern"          ),
    /*  7 */ SEnumEntry( static_cast<int>(EFillStyle::Dense6Pattern),          "Dense6Pattern",           "Dense6Pattern"          ),
    /*  8 */ SEnumEntry( static_cast<int>(EFillStyle::Dense7Pattern),          "Dense7Pattern",           "Dense7Pattern"          ),
    /*  9 */ SEnumEntry( static_cast<int>(EFillStyle::HorPattern),             "HorPattern",              "HorPattern"             ),
    /* 10 */ SEnumEntry( static_cast<int>(EFillStyle::VerPattern),             "VerPattern",              "VerPattern"             ),
    /* 11 */ SEnumEntry( static_cast<int>(EFillStyle::CrossPattern),           "CrossPattern",            "CrossPattern"           ),
    /* 12 */ SEnumEntry( static_cast<int>(EFillStyle::BDiagPattern),           "BDiagPattern",            "BDiagPattern"           ),
    /* 13 */ SEnumEntry( static_cast<int>(EFillStyle::FDiagPattern),           "FDiagPattern",            "FDiagPattern"           ),
    /* 14 */ SEnumEntry( static_cast<int>(EFillStyle::DiagCrossPattern),       "DiagCrossPattern",        "DiagCrossPattern"       ),
    /* 15 */ SEnumEntry( static_cast<int>(EFillStyle::LinearGradientPattern),  "LinearGradientPattern",   "LinearGradientPattern"  ),
    /* 16 */ SEnumEntry( static_cast<int>(EFillStyle::RadialGradientPattern),  "RadialGradientPattern",   "RadialGradientPattern"  ),
    /* 17 */ SEnumEntry( static_cast<int>(EFillStyle::ConicalGradientPattern), "ConicalGradientPattern",  "ConicalGradientPattern" ),
    /* 18 */ SEnumEntry( static_cast<int>(EFillStyle::TexturePattern),         "TexturePattern",          "TexturePattern"         )
};

//------------------------------------------------------------------------------
Qt::BrushStyle ZS::Draw::fillStyle2QtBrushStyle( EFillStyle i_fillStyle )
//------------------------------------------------------------------------------
{
    Qt::BrushStyle brushStyle = Qt::NoBrush;

    if( static_cast<int>(i_fillStyle) >= 0 && static_cast<int>(i_fillStyle) < _ZSArrLen(s_arFillStyles2QtBrushStyles) )
    {
        brushStyle = s_arFillStyles2QtBrushStyles[static_cast<int>(i_fillStyle)];
    }
    return brushStyle;

} // fillStyle2QtBrushStyle

//------------------------------------------------------------------------------
bool ZS::Draw::isFillStyleGradientPattern( EFillStyle i_fillStyle )
//------------------------------------------------------------------------------
{
    bool bIsGradientPattern = false;

    if( i_fillStyle == EFillStyle::LinearGradientPattern
     || i_fillStyle == EFillStyle::RadialGradientPattern
     || i_fillStyle == EFillStyle::ConicalGradientPattern )
    {
        bIsGradientPattern = true;
    }
    return bIsGradientPattern;

} // isFillStyleGradientPattern


/*==============================================================================
Enum ELineStyle
==============================================================================*/

template<> QMutex ZS::System::CEnum<ELineStyle>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> ZS::System::CEnum<ELineStyle>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

const Qt::PenStyle s_arLineStyles2QtPenStyles[] =
{
    /* 0: NoLine         */ Qt::NoPen,
    /* 1: SolidLine      */ Qt::SolidLine,
    /* 2: DashLine       */ Qt::DashLine,
    /* 3: DotLine        */ Qt::DotLine,
    /* 4: DashDotLine    */ Qt::DashDotLine,
    /* 5: DashDotDotLine */ Qt::DashDotDotLine
};

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> ZS::System::CEnum<ELineStyle>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                                Name,      Symbol, Text,  SCIPShort, SCPILong, Value
    /* 0 */ SEnumEntry( static_cast<int>(ELineStyle::NoLine),         "NoLine",         "No Line"                   ),
    /* 1 */ SEnumEntry( static_cast<int>(ELineStyle::SolidLine),      "SolidLine",      "Solid Line"                ),
    /* 2 */ SEnumEntry( static_cast<int>(ELineStyle::DashLine),       "DashLine",       "Dashed Line"               ),
    /* 3 */ SEnumEntry( static_cast<int>(ELineStyle::DotLine),        "DotLine",        "Dotted Line"               ),
    /* 4 */ SEnumEntry( static_cast<int>(ELineStyle::DashDotLine),    "DashDotLine",    "Dashed Dotted Line"        ),
    /* 5 */ SEnumEntry( static_cast<int>(ELineStyle::DashDotDotLine), "DashDotDotLine", "Dashed Dotted Dotted Line" )
};

//------------------------------------------------------------------------------
Qt::PenStyle ZS::Draw::lineStyle2QtPenStyle( ELineStyle i_lineStyle )
//------------------------------------------------------------------------------
{
    Qt::PenStyle penStyle = Qt::NoPen;

    if( static_cast<int>(i_lineStyle) >= 0 && static_cast<int>(i_lineStyle) < _ZSArrLen(s_arLineStyles2QtPenStyles) )
    {
        penStyle = s_arLineStyles2QtPenStyles[static_cast<int>(i_lineStyle)];
    }
    return penStyle;

} // lineStyle2QtPenStyle


/*==============================================================================
Enum ELineRecordType
==============================================================================*/

template<> QMutex ZS::System::CEnum<ELineRecordType>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> ZS::System::CEnum<ELineRecordType>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> ZS::System::CEnum<ELineRecordType>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                             Name,      Symbol, Text,  SCIPShort, SCPILong, Value
    /* 0 */ SEnumEntry( static_cast<int>(ELineRecordType::Normal), "Normal", "Normal" ),
    /* 1 */ SEnumEntry( static_cast<int>(ELineRecordType::Double), "Double", "Double" ),
    /* 2 */ SEnumEntry( static_cast<int>(ELineRecordType::Triple), "Triple", "Triple" )
};


/*==============================================================================
Enum ELineEndStyle
==============================================================================*/

template<> QMutex ZS::System::CEnum<ELineEndStyle>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> ZS::System::CEnum<ELineEndStyle>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> ZS::System::CEnum<ELineEndStyle>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( static_cast<int>(ELineEndStyle::Normal),    "Normal",    "Normal"     ),
    /* 1 */ SEnumEntry( static_cast<int>(ELineEndStyle::ArrowHead), "ArrowHead", "Arrow Head" )
};


/*==============================================================================
Enum ELineEndFillStyle
==============================================================================*/

template<> QMutex ZS::System::CEnum<ELineEndFillStyle>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> ZS::System::CEnum<ELineEndFillStyle>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

const Qt::BrushStyle s_arLineEndFillStyles2QtBrushStyles[] =
{
    /*  0: ELineEndFillStyle::NoFill       */ Qt::NoBrush,
    /*  1: ELineEndFillStyle::SolidPattern */ Qt::SolidPattern
};

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> ZS::System::CEnum<ELineEndFillStyle>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                             Name,      Symbol, Text,  SCIPShort, SCPILong, Value
    SEnumEntry( static_cast<int>(ELineEndFillStyle::NoFill),       "NoFill",       "No Fill"       ),
    SEnumEntry( static_cast<int>(ELineEndFillStyle::SolidPattern), "SolidPattern", "Solid Pattern" )
};

//------------------------------------------------------------------------------
Qt::BrushStyle ZS::Draw::lineEndFillStyle2QtBrushStyle( ELineEndFillStyle i_fillStyle )
//------------------------------------------------------------------------------
{
    Qt::BrushStyle brushStyle = Qt::NoBrush;

    if( static_cast<int>(i_fillStyle) >= 0 && static_cast<int>(i_fillStyle) < _ZSArrLen(s_arLineEndFillStyles2QtBrushStyles) )
    {
        brushStyle = s_arLineEndFillStyles2QtBrushStyles[static_cast<int>(i_fillStyle)];
    }
    return brushStyle;

} // lineEndFillStyle2QtBrushStyle


/*==============================================================================
Enum ELineEndBaseLineType
==============================================================================*/

template<> QMutex ZS::System::CEnum<ELineEndBaseLineType>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> ZS::System::CEnum<ELineEndBaseLineType>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> ZS::System::CEnum<ELineEndBaseLineType>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                            Name,      Symbol, Text,  SCIPShort, SCPILong, Value
    SEnumEntry( static_cast<int>(ELineEndBaseLineType::NoLine),   "NoLine",   "No Line"  ),
    SEnumEntry( static_cast<int>(ELineEndBaseLineType::Normal),   "Normal",   "Normal"   ),
    SEnumEntry( static_cast<int>(ELineEndBaseLineType::Indented), "Indented", "Indented" )
};


/*==============================================================================
Enum ELineEndWidth
==============================================================================*/

template<> QMutex ZS::System::CEnum<ELineEndWidth>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> ZS::System::CEnum<ELineEndWidth>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

const double s_arfLineEndWidths[] =
{
    /* Thin   */  7.0,
    /* Medium */ 13.0,
    /* Wide   */ 19.0
};

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> ZS::System::CEnum<ELineEndWidth>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                           Name,      Symbol, Text,  SCIPShort, SCPILong, Value
    /* 0 */ SEnumEntry( static_cast<int>(ELineEndWidth::Thin),   "Thin",   "Thin"   ),
    /* 1 */ SEnumEntry( static_cast<int>(ELineEndWidth::Medium), "Medium", "Medium" ),
    /* 2 */ SEnumEntry( static_cast<int>(ELineEndWidth::Wide),   "Wide",   "Wide"   )
};

//------------------------------------------------------------------------------
double ZS::Draw::lineEndWidth2dy( ELineEndWidth i_lineEndWidth )
//------------------------------------------------------------------------------
{
    double fdy = 0.0;

    if( static_cast<int>(i_lineEndWidth) >= 0 && static_cast<int>(i_lineEndWidth) < CEnumLineEndWidth::count() )
    {
        fdy = s_arfLineEndWidths[static_cast<int>(i_lineEndWidth)];
    }
    return fdy;

} // lineEndWidth2dy


/*==============================================================================
Enum ELineEndLength
==============================================================================*/

template<> QMutex ZS::System::CEnum<ELineEndLength>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> ZS::System::CEnum<ELineEndLength>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

const double s_arfLineEndLengths[] =
{
    /* Short  */  7.0,
    /* Medium */ 10.0,
    /* Long   */ 13.0
};

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> ZS::System::CEnum<ELineEndLength>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                            Name,      Symbol, Text,  SCIPShort, SCPILong, Value
    /* 0 */ SEnumEntry( static_cast<int>(ELineEndLength::Short),  "Short",  "Short"  ),
    /* 1 */ SEnumEntry( static_cast<int>(ELineEndLength::Medium), "Medium", "Medium" ),
    /* 2 */ SEnumEntry( static_cast<int>(ELineEndLength::Long),   "Long",   "Long"   )
};

//------------------------------------------------------------------------------
double ZS::Draw::lineEndLength2dx( ELineEndLength i_lineEndLength )
//------------------------------------------------------------------------------
{
    double fdx = 0.0;

    if( static_cast<int>(i_lineEndLength) >= 0 && static_cast<int>(i_lineEndLength) < CEnumLineEndWidth::count() )
    {
        fdx = s_arfLineEndLengths[static_cast<int>(i_lineEndLength)];
    }
    return fdx;

} // lineEndLength2dx


/*==============================================================================
Enum EAlignmentRef
==============================================================================*/

template<> QMutex ZS::System::CEnum<EAlignmentRef>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> ZS::System::CEnum<EAlignmentRef>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> ZS::System::CEnum<EAlignmentRef>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                              Name,      Symbol, Text,  SCIPShort, SCPILong, Value
    /* 0 */ SEnumEntry( static_cast<int>(EAlignmentRef::Left),      "Left",       "L"  ),
    /* 1 */ SEnumEntry( static_cast<int>(EAlignmentRef::HCenter),   "HCenter",    "HC" ),
    /* 2 */ SEnumEntry( static_cast<int>(EAlignmentRef::Right),     "Right",      "R"  ),
    /* 3 */ SEnumEntry( static_cast<int>(EAlignmentRef::Top),       "Top",        "T"  ),
    /* 4 */ SEnumEntry( static_cast<int>(EAlignmentRef::VCenter),   "VCenter",    "VC" ),
    /* 5 */ SEnumEntry( static_cast<int>(EAlignmentRef::Bottom),    "Bottom",     "B"  ),
    /* 6 */ SEnumEntry( static_cast<int>(EAlignmentRef::Width),     "Width",      "W"  ),
    /* 7 */ SEnumEntry( static_cast<int>(EAlignmentRef::Height),    "Height",     "H"  ),
    /* 8 */ SEnumEntry( static_cast<int>(EAlignmentRef::Undefined), "Undefined",  "?"  )
};
