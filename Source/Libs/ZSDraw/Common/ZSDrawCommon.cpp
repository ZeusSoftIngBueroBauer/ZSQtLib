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

#include "ZSDraw/Common/ZSDrawCommon.h"
#include "ZSSys/ZSSysEnumEntry.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

/*==============================================================================
Enum ENormedPaperSizes
==============================================================================*/

template<> QMutex CEnum<ENormedPaperSize>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> CEnum<ENormedPaperSize>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> CEnum<ENormedPaperSize>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
    SEnumEntry( static_cast<int>(ENormedPaperSize::DinA0),  "DinA0",  "A0",  "DIN A0",
        QSizeF(c_fNormedPaperSizeWidthDinA0_mm, c_fNormedPaperSizeHeightDinA0_mm) ),
    SEnumEntry( static_cast<int>(ENormedPaperSize::DinA1),  "DinA1",  "A1",  "DIN A1",
        QSizeF(c_fNormedPaperSizeWidthDinA1_mm, c_fNormedPaperSizeHeightDinA1_mm) ),
    SEnumEntry( static_cast<int>(ENormedPaperSize::DinA2),  "DinA2",  "A2",  "DIN A2",
        QSizeF(c_fNormedPaperSizeWidthDinA2_mm, c_fNormedPaperSizeHeightDinA2_mm) ),
    SEnumEntry( static_cast<int>(ENormedPaperSize::DinA3),  "DinA3",  "A3",  "DIN A3",
        QSizeF(c_fNormedPaperSizeWidthDinA3_mm, c_fNormedPaperSizeHeightDinA3_mm) ),
    SEnumEntry( static_cast<int>(ENormedPaperSize::DinA4),  "DinA4",  "A4",  "DIN A4",
        QSizeF(c_fNormedPaperSizeWidthDinA4_mm, c_fNormedPaperSizeHeightDinA4_mm) ),
    SEnumEntry( static_cast<int>(ENormedPaperSize::DinA5),  "DinA5",  "A5",  "DIN A5",
        QSizeF(c_fNormedPaperSizeWidthDinA5_mm, c_fNormedPaperSizeHeightDinA5_mm) ),
    SEnumEntry( static_cast<int>(ENormedPaperSize::DinA6),  "DinA6",  "A6",  "DIN A6",
        QSizeF(c_fNormedPaperSizeWidthDinA6_mm, c_fNormedPaperSizeHeightDinA6_mm) ),
    SEnumEntry( static_cast<int>(ENormedPaperSize::DinA7),  "DinA7",  "A7",  "DIN A7",
        QSizeF(c_fNormedPaperSizeWidthDinA7_mm, c_fNormedPaperSizeHeightDinA7_mm) ),
    SEnumEntry( static_cast<int>(ENormedPaperSize::DinA8),  "DinA8",  "A8",  "DIN A8",
        QSizeF(c_fNormedPaperSizeWidthDinA8_mm, c_fNormedPaperSizeHeightDinA8_mm) ),
    SEnumEntry( static_cast<int>(ENormedPaperSize::DinA9),  "DinA9",  "A9",  "DIN A9",
        QSizeF(c_fNormedPaperSizeWidthDinA9_mm, c_fNormedPaperSizeHeightDinA9_mm) ),
    SEnumEntry( static_cast<int>(ENormedPaperSize::DinA10), "DinA10", "A10", "DIN A10",
        QSizeF(c_fNormedPaperSizeWidthDinA10_mm, c_fNormedPaperSizeHeightDinA10_mm) )
};


/*==============================================================================
Enum EPrinterPaperSource
==============================================================================*/

template<> QMutex CEnum<EPrinterPaperSource>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> CEnum<EPrinterPaperSource>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> CEnum<EPrinterPaperSource>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( static_cast<int>(EPrinterPaperSource::AutoSelect), "Auto Select" ),
    /* 1 */ SEnumEntry( static_cast<int>(EPrinterPaperSource::ManualFeed), "Manual Feed" ),
    /* 2 */ SEnumEntry( static_cast<int>(EPrinterPaperSource::Tray1),      "Tray 1"      ),
    /* 3 */ SEnumEntry( static_cast<int>(EPrinterPaperSource::Tray2),      "Tray 2"      )
};


/*==============================================================================
Enum EScaleDimensionUnit
==============================================================================*/

template<> QMutex CEnum<EScaleDimensionUnit>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> CEnum<EScaleDimensionUnit>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> CEnum<EScaleDimensionUnit>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( static_cast<int>(EScaleDimensionUnit::Pixels), "Pixels" ),
    /* 1 */ SEnumEntry( static_cast<int>(EScaleDimensionUnit::Metric), "Metric" )
};


/*==============================================================================
Enum EGraphObjType
==============================================================================*/

//------------------------------------------------------------------------------
const SEnumEntry s_arEnumStrGraphObjType[EGraphObjTypeCount] =
//------------------------------------------------------------------------------
{
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
    if (i_graphObjType == EGraphObjTypeSelectionPoint) {
        str = "SelectionPoint";
    }
    else if (i_graphObjType == EGraphObjTypeLabel) {
        str = "Label";
    }
    else if (i_graphObjType == EGraphObjTypeLabelGeometryPosition) {
        str = "LabelGeometryPosition";
    }
    else if (i_graphObjType == EGraphObjTypeLabelGeometryHeight) {
        str = "LabelGeometryHeight";
    }
    else if (i_graphObjType == EGraphObjTypeLabelGeometryWidth) {
        str = "LabelGeometryWidth";
    }
    else if (i_graphObjType == EGraphObjTypeLabelGeometryLength) {
        str = "LabelGeometryLength";
    }
    else if (i_graphObjType == EGraphObjTypeLabelGeometryAngle) {
        str = "LabelGeometryAngle";
    }
    else if (i_graphObjType == EGraphObjTypeUndefined) {
        str = "Undefined";
    }
    else if (i_graphObjType >= EGraphObjTypeMin && i_graphObjType <= EGraphObjTypeMax) {
        str = SEnumEntry::enumerator2Str(
            s_arEnumStrGraphObjType, _ZSArrLen(s_arEnumStrGraphObjType), i_graphObjType-EGraphObjTypeMin);
    }
    else {
        str = "? (" + QString::number(i_graphObjType) + ")";
    }
    return str;
}

//------------------------------------------------------------------------------
EGraphObjType ZS::Draw::str2GraphObjType( const QString& i_str )
//------------------------------------------------------------------------------
{
    EGraphObjType graphObjType = EGraphObjTypeUndefined;
    bool bOk = false;
    int iGraphObjType = SEnumEntry::str2Enumerator(
        s_arEnumStrGraphObjType, _ZSArrLen(s_arEnumStrGraphObjType), i_str, -1, Qt::CaseInsensitive, &bOk);
    if (bOk) {
        graphObjType = static_cast<EGraphObjType>(iGraphObjType);
    }
    return graphObjType;
}

//------------------------------------------------------------------------------
QString ZS::Draw::graphObjType2ClassName( int i_graphObjType )
//------------------------------------------------------------------------------
{
    QString str;
    if (i_graphObjType == EGraphObjTypeSelectionPoint) {
        str = "CGraphObjSelectionPoint";
    }
    else if (i_graphObjType == EGraphObjTypeLabel) {
        str = "CGraphObjLabel";
    }
    else if (i_graphObjType == EGraphObjTypeLabelGeometryPosition) {
        str = "CGraphObjLabelGeometryPosition";
    }
    else if (i_graphObjType == EGraphObjTypeLabelGeometryHeight) {
        str = "CGraphObjLabelGeometryHeight";
    }
    else if (i_graphObjType == EGraphObjTypeLabelGeometryWidth) {
        str = "CGraphObjLabelGeometryWidth";
    }
    else if (i_graphObjType == EGraphObjTypeLabelGeometryLength) {
        str = "CGraphObjLabelGeometryLength";
    }
    else if (i_graphObjType == EGraphObjTypeLabelGeometryAngle) {
        str = "CGraphObjLabelGeometryAngle";
    }
    else {
        str = SEnumEntry::enumerator2Str(
            s_arEnumStrGraphObjClassName, _ZSArrLen(s_arEnumStrGraphObjClassName), i_graphObjType);
    }
    return str;
}

//------------------------------------------------------------------------------
EGraphObjType ZS::Draw::className2GraphObjType( const QString& i_str )
//------------------------------------------------------------------------------
{
    EGraphObjType graphObjType = EGraphObjTypeUndefined;
    bool bOk = false;
    int iGraphObjType = SEnumEntry::str2Enumerator(
        s_arEnumStrGraphObjClassName, _ZSArrLen(s_arEnumStrGraphObjClassName), i_str, -1, Qt::CaseInsensitive, &bOk);
    if (bOk) {
        graphObjType = static_cast<EGraphObjType>(iGraphObjType);
    }
    return graphObjType;
}


/*==============================================================================
Enum EEditTool
==============================================================================*/

template<> QMutex CEnum<EEditTool>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> CEnum<EEditTool>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> CEnum<EEditTool>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( static_cast<int>(EEditTool::None),          "None",          "-" ),
    /* 1 */ SEnumEntry( static_cast<int>(EEditTool::Select),        "Select",        "S" ),
    /* 2 */ SEnumEntry( static_cast<int>(EEditTool::CreateObjects), "CreateObjects", "C" ),
    /* 3 */ SEnumEntry( static_cast<int>(EEditTool::Ignore),        "Ignore",        "I" )
};


/*==============================================================================
Enum ECoordinatesVersion
==============================================================================*/

template<> QMutex CEnum<ECoordinatesVersion>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> CEnum<ECoordinatesVersion>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> CEnum<ECoordinatesVersion>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( static_cast<int>(ECoordinatesVersion::Original), "Original" ),
    /* 1 */ SEnumEntry( static_cast<int>(ECoordinatesVersion::Transformed),  "Current"  )
};


/*==============================================================================
Enum EEditMode
==============================================================================*/

template<> QMutex CEnum<EEditMode>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> CEnum<EEditMode>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> CEnum<EEditMode>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( static_cast<int>(EEditMode::None),           "None",           "-" ),
    /* 1 */ SEnumEntry( static_cast<int>(EEditMode::Creating),       "Creating",       "C" ),
    /* 2 */ SEnumEntry( static_cast<int>(EEditMode::Move),           "Move",           "M" ),
    /* 3 */ SEnumEntry( static_cast<int>(EEditMode::Resize),         "Resize",         "S" ),
    /* 4 */ SEnumEntry( static_cast<int>(EEditMode::Rotate),         "Rotate",         "R" ),
    /* 5 */ SEnumEntry( static_cast<int>(EEditMode::MoveShapePoint), "MoveShapePoint", "M" ),
    /* 6 */ SEnumEntry( static_cast<int>(EEditMode::EditText),       "EditText",       "E" ),
    /* 7 */ SEnumEntry( static_cast<int>(EEditMode::Ignore),         "Ignore",         "I" )
};


/*==============================================================================
Enum EEditResizeMode
==============================================================================*/

template<> QMutex CEnum<EEditResizeMode>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> CEnum<EEditResizeMode>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> CEnum<EEditResizeMode>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( static_cast<int>(EEditResizeMode::None),      "None",      "-" ),
    /* 1 */ SEnumEntry( static_cast<int>(EEditResizeMode::ResizeAll), "ResizeAll", "A" ),
    /* 2 */ SEnumEntry( static_cast<int>(EEditResizeMode::ResizeHor), "ResizeHor", "H" ),
    /* 3 */ SEnumEntry( static_cast<int>(EEditResizeMode::ResizeVer), "ResizeVer", "V" ),
    /* 4 */ SEnumEntry( static_cast<int>(EEditResizeMode::Ignore),    "Ignore",    "I" )
};


/*==============================================================================
Enum ESelectionPointType
==============================================================================*/

template<> QMutex CEnum<ESelectionPointType>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> CEnum<ESelectionPointType>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> CEnum<ESelectionPointType>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( static_cast<int>(ESelectionPointType::Undefined),         "Undefined"         ),
    /* 1 */ SEnumEntry( static_cast<int>(ESelectionPointType::BoundingRectangle), "BoundingRect"      ),
    /* 2 */ SEnumEntry( static_cast<int>(ESelectionPointType::PolygonShapePoint), "PolygonShapePoint" )
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

template<> QMutex CEnum<ESelectionPoint>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> CEnum<ESelectionPoint>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

static double s_fSelectionPointRotateDistance_px = 20.0;

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> CEnum<ESelectionPoint>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
    /*  0 */ SEnumEntry( static_cast<int>(ESelectionPoint::None),         "None",         "-",   "None"          ),
    /*  1 */ SEnumEntry( static_cast<int>(ESelectionPoint::TopLeft),      "TopLeft",      "TL",  "Top Left"      ),
    /*  2 */ SEnumEntry( static_cast<int>(ESelectionPoint::TopRight),     "TopRight",     "TR",  "Top Right"     ),
    /*  3 */ SEnumEntry( static_cast<int>(ESelectionPoint::BottomRight),  "BottomRight",  "BR",  "Bottom Right"  ),
    /*  4 */ SEnumEntry( static_cast<int>(ESelectionPoint::BottomLeft),   "BottomLeft",   "BL",  "Bottom Left"   ),
    /*  5 */ SEnumEntry( static_cast<int>(ESelectionPoint::TopCenter),    "TopCenter",    "TC",  "Top Center"    ),
    /*  6 */ SEnumEntry( static_cast<int>(ESelectionPoint::RightCenter),  "RightCenter",  "RC",  "Right Center"  ),
    /*  7 */ SEnumEntry( static_cast<int>(ESelectionPoint::BottomCenter), "BottomCenter", "BC",  "Bottom Center" ),
    /*  8 */ SEnumEntry( static_cast<int>(ESelectionPoint::LeftCenter),   "LeftCenter",   "LC",  "Left Center"   ),
    /*  9 */ SEnumEntry( static_cast<int>(ESelectionPoint::Center),       "Center",       "C",   "Center"        ),
    /* 10 */ SEnumEntry( static_cast<int>(ESelectionPoint::RotateTop),    "RotateTop",    "OT",  "Rotate Top"    ),
    /* 11 */ SEnumEntry( static_cast<int>(ESelectionPoint::RotateBottom), "RotateBottom", "OB",  "Rotate Bottom" ),
    /* 12 */ SEnumEntry( static_cast<int>(ESelectionPoint::PolygonPoint), "PolygonPoint", "PP",  "Polygon Point" ),
    /* 13 */ SEnumEntry( static_cast<int>(ESelectionPoint::All),          "All",          "All", "All"           ),
    /* 14 */ SEnumEntry( static_cast<int>(ESelectionPoint::Any),          "Any",          "Any", "Any"           )
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
QPointF ZS::Draw::getOppositeSelectionPoint( const QRectF& i_rct, ESelectionPoint i_selPt )
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

} // getOppositeSelectionPoint

//------------------------------------------------------------------------------
ESelectionPoint ZS::Draw::getOppositeSelectionPoint( ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    static const ESelectionPoint s_arSelectionPointsOpposite[] =
    {
        /*  0: None         */ ESelectionPoint::None,
        /*  1: TopLeft      */ ESelectionPoint::BottomRight,
        /*  2: TopRight     */ ESelectionPoint::BottomLeft,
        /*  3: BottomRight  */ ESelectionPoint::TopLeft,
        /*  4: BottomLeft   */ ESelectionPoint::TopRight,
        /*  5: TopCenter    */ ESelectionPoint::BottomCenter,
        /*  6: RightCenter  */ ESelectionPoint::LeftCenter,
        /*  7: BottomCenter */ ESelectionPoint::TopCenter,
        /*  8: LeftCenter   */ ESelectionPoint::RightCenter,
        /*  9: Center       */ ESelectionPoint::Center,
        /* 10: RotateTop    */ ESelectionPoint::RotateBottom,
        /* 11: RotateBottom */ ESelectionPoint::RotateTop
    };

    ESelectionPoint selPtOpp = ESelectionPoint::None;

    if( static_cast<int>(i_selPt) >= 0 && static_cast<int>(i_selPt) < _ZSArrLen(s_arSelectionPointsOpposite) )
    {
        selPtOpp = s_arSelectionPointsOpposite[static_cast<int>(i_selPt)];
    }
    return selPtOpp;

} // getOppositeSelectionPoint

//------------------------------------------------------------------------------
CEnumEditMode ZS::Draw::selectionPoint2EditMode( ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    const EEditMode s_arEditModes[] =
    {
        /*  0: None         */ EEditMode::None,
        /*  1: TopLeft      */ EEditMode::Resize,
        /*  2: TopRight     */ EEditMode::Resize,
        /*  3: BottomRight  */ EEditMode::Resize,
        /*  4: BottomLeft   */ EEditMode::Resize,
        /*  5: TopCenter    */ EEditMode::Resize,
        /*  6: RightCenter  */ EEditMode::Resize,
        /*  7: BottomCenter */ EEditMode::Resize,
        /*  8: LeftCenter   */ EEditMode::Resize,
        /*  9: Center       */ EEditMode::Move,
        /* 10: RotateTop    */ EEditMode::Rotate,
        /* 11: RotateBottom */ EEditMode::Rotate
    };

    EEditMode editMode = EEditMode::None;

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
        /*  0: None         */ EEditResizeMode::None,
        /*  1: TopLeft      */ EEditResizeMode::ResizeAll,
        /*  2: TopRight     */ EEditResizeMode::ResizeAll,
        /*  3: BottomRight  */ EEditResizeMode::ResizeAll,
        /*  4: BottomLeft   */ EEditResizeMode::ResizeAll,
        /*  5: TopCenter    */ EEditResizeMode::ResizeVer,
        /*  6: RightCenter  */ EEditResizeMode::ResizeHor,
        /*  7: BottomCenter */ EEditResizeMode::ResizeVer,
        /*  8: LeftCenter   */ EEditResizeMode::ResizeHor,
        /*  9: Center       */ EEditResizeMode::None,
        /* 10: RotateTop    */ EEditResizeMode::None,
        /* 11: RotateBottom */ EEditResizeMode::None
    };

    EEditResizeMode editResizeMode = EEditResizeMode::None;

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
        /*  0: None         */ Qt::ForbiddenCursor,
        /*  1: TopLeft      */ Qt::SizeFDiagCursor, /*  \  */
        /*  2: TopRight     */ Qt::SizeBDiagCursor, /*  /  */
        /*  3: BottomRight  */ Qt::SizeFDiagCursor, /*  \  */
        /*  4: BottomLeft   */ Qt::SizeBDiagCursor, /*  /  */
        /*  5: TopCenter    */ Qt::SizeVerCursor,
        /*  6: RightCenter  */ Qt::SizeHorCursor,
        /*  7: BottomCenter */ Qt::SizeVerCursor,
        /*  8: LeftCenter   */ Qt::SizeHorCursor,
        /*  9: Center       */ Qt::SizeAllCursor,
        /* 10: RotateTop    */ Qt::BitmapCursor,
        /* 11: RotateBottom */ Qt::BitmapCursor
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

template<> QMutex CEnum<ELinePoint>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> CEnum<ELinePoint>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> CEnum<ELinePoint>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( static_cast<int>(ELinePoint::None),  "None",  "-"     ),
    /* 1 */ SEnumEntry( static_cast<int>(ELinePoint::Start), "Start", "Start" ),
    /* 2 */ SEnumEntry( static_cast<int>(ELinePoint::End),   "End",   "End"   )
};


/*==============================================================================
Enum ETextStyle
==============================================================================*/

template<> QMutex CEnum<ETextStyle>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> CEnum<ETextStyle>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> CEnum<ETextStyle>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( static_cast<int>(ETextStyle::Normal),     "Normal",     "Normal"      ),
    /* 1 */ SEnumEntry( static_cast<int>(ETextStyle::Italic),     "Italic",     "Italic"      ),
    /* 2 */ SEnumEntry( static_cast<int>(ETextStyle::Bold),       "Bold",       "Bold"        ),
    /* 3 */ SEnumEntry( static_cast<int>(ETextStyle::BoldItalic), "BoldItalic", "Bold Italic" )
};

//------------------------------------------------------------------------------
bool ZS::Draw::isTextStyleBold(const CEnumTextStyle& i_textStyle)
//------------------------------------------------------------------------------
{
    if (i_textStyle == ETextStyle::Bold || i_textStyle == ETextStyle::BoldItalic)
    {
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
bool ZS::Draw::isTextStyleItalic(const CEnumTextStyle& i_textStyle)
//------------------------------------------------------------------------------
{
    if (i_textStyle == ETextStyle::Italic || i_textStyle == ETextStyle::BoldItalic)
    {
        return true;
    }
    return false;
}

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
{
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
}

//------------------------------------------------------------------------------
QString ZS::Draw::textSize2Str( ETextSize i_textSize )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrTextSize,_ZSArrLen(s_arEnumStrTextSize),i_textSize);
}

//------------------------------------------------------------------------------
ETextSize ZS::Draw::str2TextSize( const QString& i_str, bool* o_pbConverted )
//------------------------------------------------------------------------------
{
    ETextSize textSize = ETextSize11;
    int iTextSize = SEnumEntry::str2Enumerator(s_arEnumStrTextSize,_ZSArrLen(s_arEnumStrTextSize),i_str);
    bool bConverted = false;

    if( iTextSize >= 0 && iTextSize < _ZSArrLen(s_arEnumStrTextSize) )
    {
        textSize = static_cast<ETextSize>(iTextSize);
        bConverted = true;
    }
    else
    {
        iTextSize = i_str.toInt(&bConverted);
        if( bConverted && iTextSize >= 0 && iTextSize < _ZSArrLen(s_arEnumStrTextSize) )
        {
            textSize = static_cast<ETextSize>(iTextSize);
        }
    }
    if (o_pbConverted != nullptr)
    {
        *o_pbConverted = bConverted;
    }
    return textSize;
}


/*==============================================================================
Enum ETextEffect
==============================================================================*/

template<> QMutex CEnum<ETextEffect>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> CEnum<ETextEffect>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> CEnum<ETextEffect>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( static_cast<int>(ETextEffect::None),               "None",               "None"               ),
    /* 1 */ SEnumEntry( static_cast<int>(ETextEffect::Strikeout),          "Strikeout",          "Strikeout"          ),
    /* 2 */ SEnumEntry( static_cast<int>(ETextEffect::Underline),          "Underline",          "Underline"          ),
    /* 3 */ SEnumEntry( static_cast<int>(ETextEffect::StrikeoutUnderline), "StrikeoutUnderline", "StrikeoutUnderline" )
};

//------------------------------------------------------------------------------
bool ZS::Draw::isTextEffectStrikeout(const CEnumTextEffect& i_textEffect)
//------------------------------------------------------------------------------
{
    if (i_textEffect == ETextEffect::Strikeout || i_textEffect == ETextEffect::StrikeoutUnderline)
    {
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
bool ZS::Draw::isTextEffectUnderline(const CEnumTextEffect& i_textEffect)
//------------------------------------------------------------------------------
{
    if (i_textEffect == ETextEffect::Underline || i_textEffect == ETextEffect::StrikeoutUnderline)
    {
        return true;
    }
    return false;
}

/*==============================================================================
Enum EFillStyle
==============================================================================*/

template<> QMutex CEnum<EFillStyle>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> CEnum<EFillStyle>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

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
template<> const QVector<SEnumEntry> CEnum<EFillStyle>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
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
Qt::BrushStyle ZS::Draw::fillStyle2QtBrushStyle( const CEnumFillStyle& i_fillStyle )
//------------------------------------------------------------------------------
{
    Qt::BrushStyle brushStyle = Qt::NoBrush;
    if ((i_fillStyle.enumeratorAsInt() >= 0) && (i_fillStyle.enumeratorAsInt() < _ZSArrLen(s_arFillStyles2QtBrushStyles))) {
        brushStyle = s_arFillStyles2QtBrushStyles[i_fillStyle.enumeratorAsInt()];
    }
    return brushStyle;
}

//------------------------------------------------------------------------------
bool ZS::Draw::isFillStyleGradientPattern( const CEnumFillStyle& i_fillStyle )
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
}


/*==============================================================================
Enum ELineStyle
==============================================================================*/

template<> QMutex CEnum<ELineStyle>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> CEnum<ELineStyle>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

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
template<> const QVector<SEnumEntry> CEnum<ELineStyle>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( static_cast<int>(ELineStyle::NoLine),         "NoLine",         "No Line"                   ),
    /* 1 */ SEnumEntry( static_cast<int>(ELineStyle::SolidLine),      "SolidLine",      "Solid Line"                ),
    /* 2 */ SEnumEntry( static_cast<int>(ELineStyle::DashLine),       "DashLine",       "Dashed Line"               ),
    /* 3 */ SEnumEntry( static_cast<int>(ELineStyle::DotLine),        "DotLine",        "Dotted Line"               ),
    /* 4 */ SEnumEntry( static_cast<int>(ELineStyle::DashDotLine),    "DashDotLine",    "Dashed Dotted Line"        ),
    /* 5 */ SEnumEntry( static_cast<int>(ELineStyle::DashDotDotLine), "DashDotDotLine", "Dashed Dotted Dotted Line" )
};

//------------------------------------------------------------------------------
Qt::PenStyle ZS::Draw::lineStyle2QtPenStyle( const CEnumLineStyle& i_lineStyle )
//------------------------------------------------------------------------------
{
    Qt::PenStyle penStyle = Qt::NoPen;
    if ((i_lineStyle.enumeratorAsInt() >= 0) && (i_lineStyle.enumeratorAsInt() < _ZSArrLen(s_arLineStyles2QtPenStyles))) {
        penStyle = s_arLineStyles2QtPenStyles[i_lineStyle.enumeratorAsInt()];
    }
    return penStyle;
}


/*==============================================================================
Enum ELineRecordType
==============================================================================*/

template<> QMutex CEnum<ELineRecordType>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> CEnum<ELineRecordType>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> CEnum<ELineRecordType>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( static_cast<int>(ELineRecordType::Normal), "Normal", "Normal" ),
    /* 1 */ SEnumEntry( static_cast<int>(ELineRecordType::Double), "Double", "Double" ),
    /* 2 */ SEnumEntry( static_cast<int>(ELineRecordType::Triple), "Triple", "Triple" )
};


/*==============================================================================
Enum ELineEndStyle
==============================================================================*/

template<> QMutex CEnum<ELineEndStyle>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> CEnum<ELineEndStyle>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> CEnum<ELineEndStyle>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( static_cast<int>(ELineEndStyle::Normal),    "Normal",    "Normal"     ),
    /* 1 */ SEnumEntry( static_cast<int>(ELineEndStyle::ArrowHead), "ArrowHead", "Arrow Head" )
};


/*==============================================================================
Enum EArrowHeadFillStyle
==============================================================================*/

template<> QMutex CEnum<EArrowHeadFillStyle>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> CEnum<EArrowHeadFillStyle>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

const Qt::BrushStyle s_arLineEndFillStyles2QtBrushStyles[] =
{
    /*  0: EArrowHeadFillStyle::NoFill       */ Qt::NoBrush,
    /*  1: EArrowHeadFillStyle::SolidPattern */ Qt::SolidPattern
};

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> CEnum<EArrowHeadFillStyle>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
    SEnumEntry( static_cast<int>(EArrowHeadFillStyle::NoFill),       "NoFill",       "No Fill"       ),
    SEnumEntry( static_cast<int>(EArrowHeadFillStyle::SolidPattern), "SolidPattern", "Solid Pattern" )
};

//------------------------------------------------------------------------------
Qt::BrushStyle ZS::Draw::arrowHeadFillStyle2QtBrushStyle( const CEnumArrowHeadFillStyle& i_fillStyle )
//------------------------------------------------------------------------------
{
    Qt::BrushStyle brushStyle = Qt::NoBrush;
    if ((i_fillStyle.enumeratorAsInt() >= 0) && (i_fillStyle.enumeratorAsInt() < _ZSArrLen(s_arLineEndFillStyles2QtBrushStyles))) {
        brushStyle = s_arLineEndFillStyles2QtBrushStyles[i_fillStyle.enumeratorAsInt()];
    }
    return brushStyle;
}


/*==============================================================================
Enum EArrowHeadBaseLineType
==============================================================================*/

template<> QMutex CEnum<EArrowHeadBaseLineType>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> CEnum<EArrowHeadBaseLineType>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> CEnum<EArrowHeadBaseLineType>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
    SEnumEntry( static_cast<int>(EArrowHeadBaseLineType::NoLine),   "NoLine",   "No Line"  ),
    SEnumEntry( static_cast<int>(EArrowHeadBaseLineType::Normal),   "Normal",   "Normal"   ),
    SEnumEntry( static_cast<int>(EArrowHeadBaseLineType::Indented), "Indented", "Indented" )
};


/*==============================================================================
Enum EArrowHeadWidth
==============================================================================*/

template<> QMutex CEnum<EArrowHeadWidth>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> CEnum<EArrowHeadWidth>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

const double s_arfArrowHeadWidths[] =
{
    /* Thin   */  7.0,
    /* Medium */ 13.0,
    /* Wide   */ 19.0
};

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> CEnum<EArrowHeadWidth>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( static_cast<int>(EArrowHeadWidth::Thin),   "Thin",   "Thin"   ),
    /* 1 */ SEnumEntry( static_cast<int>(EArrowHeadWidth::Medium), "Medium", "Medium" ),
    /* 2 */ SEnumEntry( static_cast<int>(EArrowHeadWidth::Wide),   "Wide",   "Wide"   )
};

//------------------------------------------------------------------------------
double ZS::Draw::arrowHeadWidth2dy( const CEnumArrowHeadWidth& i_width )
//------------------------------------------------------------------------------
{
    double fdy = 0.0;
    if ((i_width.enumeratorAsInt() >= 0) && (i_width.enumeratorAsInt() < _ZSArrLen(s_arfArrowHeadWidths))) {
        fdy = s_arfArrowHeadWidths[i_width.enumeratorAsInt()];
    }
    return fdy;
}


/*==============================================================================
Enum EArrowHeadLength
==============================================================================*/

template<> QMutex CEnum<EArrowHeadLength>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> CEnum<EArrowHeadLength>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

const double s_arfLineEndLengths[] =
{
    /* Short  */  7.0,
    /* Medium */ 10.0,
    /* Long   */ 13.0
};

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> CEnum<EArrowHeadLength>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( static_cast<int>(EArrowHeadLength::Short),  "Short",  "Short"  ),
    /* 1 */ SEnumEntry( static_cast<int>(EArrowHeadLength::Medium), "Medium", "Medium" ),
    /* 2 */ SEnumEntry( static_cast<int>(EArrowHeadLength::Long),   "Long",   "Long"   )
};

//------------------------------------------------------------------------------
double ZS::Draw::arrowHeadLength2dx( const CEnumArrowHeadLength& i_length )
//------------------------------------------------------------------------------
{
    double fdx = 0.0;
    if ((i_length.enumeratorAsInt() >= 0) && (i_length.enumeratorAsInt() < _ZSArrLen(s_arfLineEndLengths))) {
        fdx = s_arfLineEndLengths[i_length.enumeratorAsInt()];
    }
    return fdx;
}


/*==============================================================================
Enum EAlignmentRef
==============================================================================*/

template<> QMutex CEnum<EAlignmentRef>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> CEnum<EAlignmentRef>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> CEnum<EAlignmentRef>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( static_cast<int>(EAlignmentRef::None),      "None",    "-"  ),
    /* 1 */ SEnumEntry( static_cast<int>(EAlignmentRef::Left),      "Left",    "L"  ),
    /* 2 */ SEnumEntry( static_cast<int>(EAlignmentRef::HCenter),   "HCenter", "HC" ),
    /* 3 */ SEnumEntry( static_cast<int>(EAlignmentRef::Right),     "Right",   "R"  ),
    /* 4 */ SEnumEntry( static_cast<int>(EAlignmentRef::Top),       "Top",     "T"  ),
    /* 5 */ SEnumEntry( static_cast<int>(EAlignmentRef::VCenter),   "VCenter", "VC" ),
    /* 6 */ SEnumEntry( static_cast<int>(EAlignmentRef::Bottom),    "Bottom",  "B"  ),
    /* 7 */ SEnumEntry( static_cast<int>(EAlignmentRef::Width),     "Width",   "W"  ),
    /* 8 */ SEnumEntry( static_cast<int>(EAlignmentRef::Height),    "Height",  "H"  )
};
