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
#include "ZSDraw/Common/ZSDrawCommon.h"
#include "ZSDraw/Common/ZSDrawUnits.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"
#include "ZSPhysVal/ZSPhysVal.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysEnumEntry.h"

#include <QtGui/qbitmap.h>
#include <QtCore/qhash.h>

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::Draw;
using namespace ZS::System;
using namespace ZS::PhysVal;


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

/*==============================================================================
Enum ENormedPaperSizes
==============================================================================*/

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
    /*  1 */ SEnumEntry( EGraphObjTypeLine,            "CGraphObjLine"            ),
    /*  2 */ SEnumEntry( EGraphObjTypeRect,            "CGraphObjRect"            ),
    /*  3 */ SEnumEntry( EGraphObjTypeEllipse,         "CGraphObjEllipse"         ),
    /*  4 */ SEnumEntry( EGraphObjTypePolygon,         "CGraphObjPolygon"         ),
    /*  5 */ SEnumEntry( EGraphObjTypePolyline,        "CGraphObjPolygon"         ),
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
    else if (i_graphObjType == EGraphObjTypeLabelGeometryDX) {
        str = "LabelGeometryDX";
    }
    else if (i_graphObjType == EGraphObjTypeLabelGeometryDY) {
        str = "LabelGeometryDY";
    }
    else if (i_graphObjType == EGraphObjTypeLabelGeometryLength) {
        str = "LabelGeometryLength";
    }
    else if (i_graphObjType == EGraphObjTypeLabelGeometryAngle) {
        str = "LabelGeometryAngle";
    }
    else if (i_graphObjType == EGraphObjTypeUserDefined) {
        str = "UserDefined";
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
    else if (i_graphObjType == EGraphObjTypeLabelGeometryDX) {
        str = "CGraphObjLabelGeometryDX";
    }
    else if (i_graphObjType == EGraphObjTypeLabelGeometryDY) {
        str = "CGraphObjLabelGeometryDY";
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


///*==============================================================================
//Enum EEditTool
//==============================================================================*/
//
////------------------------------------------------------------------------------
//template<> const QVector<SEnumEntry> CEnum<EEditTool>::s_arEnumEntries =
////------------------------------------------------------------------------------
//{
//    /* 0 */ SEnumEntry( static_cast<int>(EEditTool::None),          "None"),
//    /* 1 */ SEnumEntry( static_cast<int>(EEditTool::Select),        "Select"),
//    /* 2 */ SEnumEntry( static_cast<int>(EEditTool::CreateObjects), "CreateObjects"),
//    /* 3 */ SEnumEntry( static_cast<int>(EEditTool::Undefined),     "Undefined")
//};


/*==============================================================================
Enum ECoordinatesVersion
==============================================================================*/

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> CEnum<ECoordinatesVersion>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( static_cast<int>(ECoordinatesVersion::Original),    "Original" ),
    /* 1 */ SEnumEntry( static_cast<int>(ECoordinatesVersion::Transformed), "Current"  )
};


/*==============================================================================
Enum EEditMode
==============================================================================*/

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> CEnum<EEditMode>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( static_cast<int>(EEditMode::None),                  "None"),
    /* 1 */ SEnumEntry( static_cast<int>(EEditMode::CreatingByMouseEvents), "CreatingByMouseEvents")
    ///* 2 */ SEnumEntry( static_cast<int>(EEditMode::Move),           "Move"),
    ///* 3 */ SEnumEntry( static_cast<int>(EEditMode::Resize),         "Resize"),
    ///* 4 */ SEnumEntry( static_cast<int>(EEditMode::Rotate),         "Rotate"),
    ///* 5 */ SEnumEntry( static_cast<int>(EEditMode::MoveShapePoint), "MoveShapePoint"),
    ///* 6 */ SEnumEntry( static_cast<int>(EEditMode::EditText),       "EditText"),
    ///* 7 */ SEnumEntry( static_cast<int>(EEditMode::Undefined),      "Undefined")
};


/*==============================================================================
Enum EEditResizeMode
==============================================================================*/

////------------------------------------------------------------------------------
//template<> const QVector<SEnumEntry> CEnum<EEditResizeMode>::s_arEnumEntries =
////------------------------------------------------------------------------------
//{
//    /* 0 */ SEnumEntry( static_cast<int>(EEditResizeMode::None),      "None"),
//    /* 1 */ SEnumEntry( static_cast<int>(EEditResizeMode::ResizeAll), "ResizeAll"),
//    /* 2 */ SEnumEntry( static_cast<int>(EEditResizeMode::ResizeHor), "ResizeHor"),
//    /* 3 */ SEnumEntry( static_cast<int>(EEditResizeMode::ResizeVer), "ResizeVer"),
//    /* 4 */ SEnumEntry( static_cast<int>(EEditResizeMode::Undefined), "Undefined")
//};


/*==============================================================================
Enum ESelectionPointType
==============================================================================*/

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> CEnum<ESelectionPointType>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry(static_cast<int>(ESelectionPointType::Undefined), "Undefined"),
    /* 1 */ SEnumEntry(static_cast<int>(ESelectionPointType::BoundingRectangle), "BoundingRect"),
    /* 2 */ SEnumEntry(static_cast<int>(ESelectionPointType::PolygonPoint), "PolygonPoint"),
    /* 3 */ SEnumEntry(static_cast<int>(ESelectionPointType::LineCenterPoint), "LineCenterPoint")
};

/*==============================================================================
Enum ESelectionPoints
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Draw::selectionPointTypes2Str( TSelectionPointTypes i_selPts )
//------------------------------------------------------------------------------
{
    QString str;
    if (i_selPts != c_uSelectionPointsNone) {
        if (i_selPts & c_uSelectionPointsBoundingRectAll) {
            str = "BoundingRectAll";
        }
        else {
            if (i_selPts & c_uSelectionPointsBoundingRectCorner) {
                str = "BoundingRectCorners";
            }
            if (i_selPts & c_uSelectionPointsBoundingRectLineCenter) {
                if (!str.isEmpty()) str += "|";
                str += "BoundingRectLineCenter";
            }
            if (i_selPts & c_uSelectionPointsBoundingRectCenter )
            {
                if (!str.isEmpty()) str += "|";
                str += "BoundingRectCenter";
            }
            if (i_selPts & c_uSelectionPointsBoundingRectRotate )
            {
                if (!str.isEmpty()) str += "|";
                str += "BoundingRectRotation";
            }
        }
        if (i_selPts & c_uSelectionPointsPolygonPoints) {
            if (!str.isEmpty()) str += "|";
            str += "PolygonPoints";
        }
        if (i_selPts & c_uSelectionPointsLineCenter) {
            if (!str.isEmpty()) str += "|";
            str += "LineCenter";
        }
    }
    return str;
}


/*==============================================================================
Enum ESelectionPoint
==============================================================================*/

static double s_fSelectionPointRotateDistance_px = 20.0;

//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> CEnum<ESelectionPoint>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
    /*  0 */ SEnumEntry( static_cast<int>(ESelectionPoint::None),            "None",            "-",   "None"              ),
    /*  1 */ SEnumEntry( static_cast<int>(ESelectionPoint::TopLeft),         "TopLeft",         "TL",  "Top Left"          ),
    /*  2 */ SEnumEntry( static_cast<int>(ESelectionPoint::TopRight),        "TopRight",        "TR",  "Top Right"         ),
    /*  3 */ SEnumEntry( static_cast<int>(ESelectionPoint::BottomRight),     "BottomRight",     "BR",  "Bottom Right"      ),
    /*  4 */ SEnumEntry( static_cast<int>(ESelectionPoint::BottomLeft),      "BottomLeft",      "BL",  "Bottom Left"       ),
    /*  5 */ SEnumEntry( static_cast<int>(ESelectionPoint::TopCenter),       "TopCenter",       "TC",  "Top Center"        ),
    /*  6 */ SEnumEntry( static_cast<int>(ESelectionPoint::RightCenter),     "RightCenter",     "RC",  "Right Center"      ),
    /*  7 */ SEnumEntry( static_cast<int>(ESelectionPoint::BottomCenter),    "BottomCenter",    "BC",  "Bottom Center"     ),
    /*  8 */ SEnumEntry( static_cast<int>(ESelectionPoint::LeftCenter),      "LeftCenter",      "LC",  "Left Center"       ),
    /*  9 */ SEnumEntry( static_cast<int>(ESelectionPoint::Center),          "Center",          "C",   "Center"            ),
    /* 10 */ SEnumEntry( static_cast<int>(ESelectionPoint::RotateTop),       "RotateTop",       "OT",  "Rotate Top"        ),
    /* 11 */ SEnumEntry( static_cast<int>(ESelectionPoint::RotateBottom),    "RotateBottom",    "OB",  "Rotate Bottom"     ),
    /* 12 */ SEnumEntry( static_cast<int>(ESelectionPoint::PolygonPoint),    "PolygonPoint",    "PP",  "Polygon Point"     ),
    /* 13 */ SEnumEntry( static_cast<int>(ESelectionPoint::LineCenterPoint), "LineCenterPoint", "LCP", "Line Center Point" ),
    /* 14 */ SEnumEntry( static_cast<int>(ESelectionPoint::All),             "All",             "All", "All"               ),
    /* 15 */ SEnumEntry( static_cast<int>(ESelectionPoint::Any),             "Any",             "Any", "Any"               )
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

    switch (i_selPt) {
        case ESelectionPoint::TopLeft: {
            ptOpp = i_rct.bottomRight();
            break;
        }
        case ESelectionPoint::TopRight: {
            ptOpp = i_rct.bottomLeft();
            break;
        }
        case ESelectionPoint::BottomRight: {
            ptOpp = i_rct.topLeft();
            break;
        }
        case ESelectionPoint::BottomLeft: {
            ptOpp = i_rct.topRight();
            break;
        }
        case ESelectionPoint::TopCenter: {
            ptOpp = QPointF(i_rct.bottom(), i_rct.center().x());
            break;
        }
        case ESelectionPoint::RightCenter: {
            ptOpp = QPointF(i_rct.left(), i_rct.center().y());
            break;
        }
        case ESelectionPoint::BottomCenter: {
            ptOpp = QPointF(i_rct.top(), i_rct.center().x());
            break;
        }
        case ESelectionPoint::LeftCenter:  {
            ptOpp = QPointF(i_rct.right(), i_rct.center().y());
            break;
        }
        case ESelectionPoint::Center: {
            ptOpp = i_rct.center();
            break;
        }
        case ESelectionPoint::RotateTop: {
            ptOpp = QPointF(i_rct.center().x(), i_rct.bottom() + s_fSelectionPointRotateDistance_px);
            break;
        }
        case ESelectionPoint::RotateBottom: {
            ptOpp = QPointF(i_rct.center().x(), i_rct.top() - s_fSelectionPointRotateDistance_px);
            break;
        }
        default: {
            break;
        }
    }
    return ptOpp;
}

//------------------------------------------------------------------------------
ESelectionPoint ZS::Draw::getOppositeSelectionPoint( ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    static const ESelectionPoint s_arSelectionPointsOpposite[] = {
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
    if (static_cast<int>(i_selPt) >= 0 && static_cast<int>(i_selPt) < _ZSArrLen(s_arSelectionPointsOpposite)) {
        selPtOpp = s_arSelectionPointsOpposite[static_cast<int>(i_selPt)];
    }
    return selPtOpp;
}

////------------------------------------------------------------------------------
//CEnumEditMode ZS::Draw::selectionPoint2EditMode( ESelectionPoint i_selPt )
////------------------------------------------------------------------------------
//{
//    const EEditMode s_arEditModes[] =
//    {
//        /*  0: None         */ EEditMode::None,
//        /*  1: TopLeft      */ EEditMode::Resize,
//        /*  2: TopRight     */ EEditMode::Resize,
//        /*  3: BottomRight  */ EEditMode::Resize,
//        /*  4: BottomLeft   */ EEditMode::Resize,
//        /*  5: TopCenter    */ EEditMode::Resize,
//        /*  6: RightCenter  */ EEditMode::Resize,
//        /*  7: BottomCenter */ EEditMode::Resize,
//        /*  8: LeftCenter   */ EEditMode::Resize,
//        /*  9: Center       */ EEditMode::Move,
//        /* 10: RotateTop    */ EEditMode::Rotate,
//        /* 11: RotateBottom */ EEditMode::Rotate
//    };
//
//    EEditMode editMode = EEditMode::None;
//    if( static_cast<int>(i_selPt) >= 0 && static_cast<int>(i_selPt) < _ZSArrLen(s_arEditModes) )
//    {
//        editMode = s_arEditModes[static_cast<int>(i_selPt)];
//    }
//    return editMode;
//}
//
////------------------------------------------------------------------------------
//CEnumEditResizeMode ZS::Draw::selectionPoint2EditResizeMode( ESelectionPoint i_selPt )
////------------------------------------------------------------------------------
//{
//    static const EEditResizeMode s_arEditResizeModes[] =
//    {
//        /*  0: None         */ EEditResizeMode::None,
//        /*  1: TopLeft      */ EEditResizeMode::ResizeAll,
//        /*  2: TopRight     */ EEditResizeMode::ResizeAll,
//        /*  3: BottomRight  */ EEditResizeMode::ResizeAll,
//        /*  4: BottomLeft   */ EEditResizeMode::ResizeAll,
//        /*  5: TopCenter    */ EEditResizeMode::ResizeVer,
//        /*  6: RightCenter  */ EEditResizeMode::ResizeHor,
//        /*  7: BottomCenter */ EEditResizeMode::ResizeVer,
//        /*  8: LeftCenter   */ EEditResizeMode::ResizeHor,
//        /*  9: Center       */ EEditResizeMode::None,
//        /* 10: RotateTop    */ EEditResizeMode::None,
//        /* 11: RotateBottom */ EEditResizeMode::None
//    };
//
//    EEditResizeMode editResizeMode = EEditResizeMode::None;
//    if( static_cast<int>(i_selPt) >= 0 && static_cast<int>(i_selPt) < _ZSArrLen(s_arEditResizeModes) )
//    {
//        editResizeMode = s_arEditResizeModes[static_cast<int>(i_selPt)];
//    }
//    return editResizeMode;
//}

//------------------------------------------------------------------------------
QCursor ZS::Draw::selectionPoint2Cursor( ESelectionPoint i_selPt, double i_fRotationAngle_degree )
//------------------------------------------------------------------------------
{
    static const QHash<int, Qt::CursorShape> s_hshCursorShapes =
    {
        {static_cast<int>(ESelectionPoint::None),            Qt::ForbiddenCursor},
        {static_cast<int>(ESelectionPoint::TopLeft),         Qt::SizeFDiagCursor}, /*  \  */
        {static_cast<int>(ESelectionPoint::TopRight),        Qt::SizeBDiagCursor}, /*  /  */
        {static_cast<int>(ESelectionPoint::BottomRight),     Qt::SizeFDiagCursor}, /*  \  */
        {static_cast<int>(ESelectionPoint::BottomLeft),      Qt::SizeBDiagCursor}, /*  /  */
        {static_cast<int>(ESelectionPoint::TopCenter),       Qt::SizeVerCursor},   /*  |  */
        {static_cast<int>(ESelectionPoint::RightCenter),     Qt::SizeHorCursor},   /* <-> */
        {static_cast<int>(ESelectionPoint::BottomCenter),    Qt::SizeVerCursor},   /*  |  */
        {static_cast<int>(ESelectionPoint::LeftCenter),      Qt::SizeHorCursor},   /* <-> */
        {static_cast<int>(ESelectionPoint::Center),          Qt::SizeAllCursor},   /* <|> */
        {static_cast<int>(ESelectionPoint::RotateTop),       Qt::BitmapCursor},    /*  o  */
        {static_cast<int>(ESelectionPoint::RotateBottom),    Qt::BitmapCursor},    /*  o  */
        {static_cast<int>(ESelectionPoint::PolygonPoint),    Qt::CrossCursor},     /*  X  */
        {static_cast<int>(ESelectionPoint::LineCenterPoint), Qt::SizeAllCursor}    /* <|> */
    };

    QCursor cursor = Qt::ArrowCursor;
    if (i_selPt == ESelectionPoint::RotateTop || i_selPt == ESelectionPoint::RotateBottom) {
        QPixmap pxmCursor(":/ZS/Draw/CursorRotateFree16x16.png");
        cursor = QCursor(pxmCursor);
    }
    else {
        cursor = s_hshCursorShapes.value(static_cast<int>(i_selPt), Qt::ArrowCursor);
        if (i_fRotationAngle_degree != 0.0) {
            double fCursorAngle_degree = 0.0;
            if (cursor == Qt::SizeFDiagCursor) {
                // " \ ": 2nd quadrant: arrow from right/bottom -> top/left
                fCursorAngle_degree = 135.0;
            }
            else if (cursor == Qt::SizeBDiagCursor) {
                // " / ": 1st quadrant: arrow from bottom/left -> top/right
                fCursorAngle_degree = 45.0;
            }
            else if (cursor == Qt::SizeHorCursor) {
                // "<->"
                fCursorAngle_degree = 180.0;
            }
            else if (cursor == Qt::SizeVerCursor) {
                /* " | " */
                fCursorAngle_degree = 270.0;
            }
            fCursorAngle_degree += i_fRotationAngle_degree;
            // Force resulting cursor rotation angle to 1st or 2nd quadrant (0..180°)
            while (fCursorAngle_degree >= 180.0) {
                fCursorAngle_degree -= 180.0;
            }
            while (fCursorAngle_degree < 0.0) {
                fCursorAngle_degree += 180.0;
            }
            if (fCursorAngle_degree >= 0.0 && fCursorAngle_degree < 45.0/2.0) { // 0.0 .. 22.5°
                cursor = Qt::SizeHorCursor;
            }
            else if (fCursorAngle_degree >= 45.0/2.0 && fCursorAngle_degree < 3.0*45.0/2.0) { // 22.5° .. 67.5°
                cursor = Qt::SizeBDiagCursor; // 1st quadrant: arrow from bottom/left -> top/right
            }
            else if (fCursorAngle_degree >= 3.0*45.0/2.0 && fCursorAngle_degree < 5.0*45.0/2.0) { // 67.5° .. 112.5°
                cursor = Qt::SizeVerCursor;
            }
            else if (fCursorAngle_degree >= 5.0*45.0/2.0 && fCursorAngle_degree < 7.0*45.0/2.0) { // 112.5° .. 157.5°
                cursor = Qt::SizeFDiagCursor; // 2nd quadrant: arrow from top/left -> bottom/right
            }
            else if (fCursorAngle_degree >= 7.0*45.0/2.0 && fCursorAngle_degree <= 180.0) { // 157.5° .. 180.0°
                cursor = Qt::SizeHorCursor;
            }
        }
    }
    return cursor;
}


/*==============================================================================
Enum ELinePoint
==============================================================================*/

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


namespace ZS
{
namespace Draw
{
/*******************************************************************************
struct SGraphObjAlignment
*******************************************************************************/

//------------------------------------------------------------------------------
QString SGraphObjAlignment::toString() const
//------------------------------------------------------------------------------
{
    QString str = "RefChild:" + m_alignmentRefChild.toString() +
                  ", RefParent:" + m_alignmentRefParent.toString() +
                  ", Absolute:" + bool2Str(m_bAlignAbsolute) +
                  ", Val:" + QString::number(m_fVal);
    return str;
}


/*******************************************************************************
struct SGraphObjSelectionPoint
*******************************************************************************/

/* public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Converts the string into a selection point struct.

    @param [in] i_str
        String to be parsed. Following formats are possible:
            "<SelectionPointType>, <BoundingRectSelectionPoint|IdxPt1>[, <IdxPt2>]"
        For example:
            "BoundingRect, TopLeft"
            "PolygonPoint, 2"
            "LineCenterPoint, 0" (Center point of line segment between points 0 and 1)
    @param [out] o_pbOk (optional)
        If != nullptr:
            true, if the string contained a valid selection point description, false otherwise.
*/
SGraphObjSelectionPoint SGraphObjSelectionPoint::fromString(
    const QString& i_str, bool* o_pbOk)
//------------------------------------------------------------------------------
{
    bool bOk = false;
    SGraphObjSelectionPoint selPt;
    QStringList strlst = i_str.split(",");
    if (strlst.size() == 2) {
        strlst[0] = strlst[0].trimmed();
        strlst[1] = strlst[1].trimmed();
        CEnumSelectionPointType selPtType = CEnumSelectionPointType::fromString(strlst[0], &bOk);
        if (bOk) {
            if (selPtType == ESelectionPointType::BoundingRectangle && strlst.size() == 2) {
                CEnumSelectionPoint selPtTmp = CEnumSelectionPoint::fromString(strlst[1], &bOk);
                if (bOk) {
                    selPt.m_selPtType = selPtType.enumerator();
                    selPt.m_selPt = selPtTmp.enumerator();
                }
            }
            else {
                QString strIdx = strlst[1];
                if (strlst[1].startsWith("P") || strlst[1].startsWith("L")) {
                    strIdx = strlst[1].mid(1);
                }
                int idxPt = strIdx.toInt(&bOk);
                if (bOk && idxPt > 0) {
                    selPt.m_selPtType = selPtType.enumerator();
                    selPt.m_idxPt = idxPt - 1;
                }
            }
        }
    }
    if (o_pbOk != nullptr) {
        *o_pbOk = bOk;
    }
    return selPt;
}

//------------------------------------------------------------------------------
/*! @brief Converts the string into a selection point struct and assignes the
           passed graphical object to the selection point struct.

    @param [in] i_pGraphObj
        Pointer to graphical object for which the string was parsed.
    @param [in] i_str
        String to be parsed (see SGraphObjSelectionPoint::fromString method above).
    @param [out] o_pbOk (optional)
        If != nullptr:
            true, if the string contained a valid selection point description, false otherwise.
*/
SGraphObjSelectionPoint SGraphObjSelectionPoint::fromString(
    CGraphObj* i_pGraphObj, const QString& i_str, bool* o_pbOk)
//------------------------------------------------------------------------------
{
    bool bOk = false;
    SGraphObjSelectionPoint selPt = fromString(i_str, &bOk);
    selPt.m_pGraphObj = i_pGraphObj;
    if (o_pbOk != nullptr) {
        *o_pbOk = bOk;
    }
    return selPt;
}

/* public: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Default constructor creating an invalid selection point descriptor.
*/
SGraphObjSelectionPoint::SGraphObjSelectionPoint() :
//------------------------------------------------------------------------------
    m_pGraphObj(nullptr),
    m_selPtType(ESelectionPointType::Undefined),
    m_selPt(ESelectionPoint::None),
    m_idxPt(-1)
{
}

//------------------------------------------------------------------------------
/*! @brief Default constructor creating an invalid selection point descriptor
           assigning the given graphical object to the struct.

    @param [in] i_pGraphObj
        Pointer to graphical object to be assigned to the descriptor.
*/
SGraphObjSelectionPoint::SGraphObjSelectionPoint(CGraphObj* i_pGraphObj) :
//------------------------------------------------------------------------------
    m_pGraphObj(i_pGraphObj),
    m_selPtType(ESelectionPointType::Undefined),
    m_selPt(ESelectionPoint::None),
    m_idxPt(-1)
{
}

//------------------------------------------------------------------------------
/*! @brief Creates a descriptor for a selection point at the bounding rectangle
           assigning the given graphical object to the struct.

    @param [in] i_pGraphObj
        Pointer to graphical object to be assigned to the descriptor.
    @param [in] i_selPtType
        Selection point type.
        Range [BoundingRectangle]
    @param [in] i_selPt
        Selection point at the bounding rectangle.
        Range [TopLeft, .., RotateBottom]
        PolygonPoint, LineCenterPoint as well as All and Any are not allowed.
*/
SGraphObjSelectionPoint::SGraphObjSelectionPoint(
    CGraphObj* i_pGraphObj, ESelectionPointType i_selPtType, ESelectionPoint i_selPt) :
//------------------------------------------------------------------------------
    m_pGraphObj(i_pGraphObj),
    m_selPtType(i_selPtType),
    m_selPt(i_selPt),
    m_idxPt(-1)
{
    if (i_selPtType != ESelectionPointType::BoundingRectangle) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, CEnumSelectionPointType(i_selPtType).toString());
    }
    if (i_selPt < ESelectionPoint::TopLeft || i_selPt > ESelectionPoint::RotateBottom) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, CEnumSelectionPoint(i_selPt).toString());
    }
}

//------------------------------------------------------------------------------
/*! @brief Creates a descriptor for a polygon point or a line center point
           assigning the given graphical object to the struct.

    @param [in] i_pGraphObj
        Pointer to graphical object to be assigned to the descriptor.
    @param [in] i_selPtType
        Selection point type.
        Range [PolygonPoint, LineCenterPoint]
    @param [in] i_idxPt
        Index of the polygon (or line) point.
        For i_selPtType = LineCenterPoint the end of the line segment is (i_idxPt + 1).
*/
SGraphObjSelectionPoint::SGraphObjSelectionPoint(
    CGraphObj* i_pGraphObj, ESelectionPointType i_selPtType, int i_idxPt) :
//------------------------------------------------------------------------------
    m_pGraphObj(i_pGraphObj),
    m_selPtType(i_selPtType),
    m_selPt(i_selPtType == ESelectionPointType::PolygonPoint ? ESelectionPoint::PolygonPoint : ESelectionPoint::LineCenterPoint),
    m_idxPt(i_idxPt)
{
    if (i_selPtType != ESelectionPointType::PolygonPoint && i_selPtType != ESelectionPointType::LineCenterPoint) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, CEnumSelectionPointType(i_selPtType).toString());
    }
}

//------------------------------------------------------------------------------
SGraphObjSelectionPoint::SGraphObjSelectionPoint(const SGraphObjSelectionPoint& i_other) :
//------------------------------------------------------------------------------
    m_pGraphObj(i_other.m_pGraphObj),
    m_selPtType(i_other.m_selPtType),
    m_selPt(i_other.m_selPt),
    m_idxPt(i_other.m_idxPt)
{
}

/* public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
SGraphObjSelectionPoint& SGraphObjSelectionPoint::operator = (const SGraphObjSelectionPoint& i_other)
//------------------------------------------------------------------------------
{
    m_pGraphObj = i_other.m_pGraphObj;
    m_selPtType = i_other.m_selPtType;
    m_selPt = i_other.m_selPt;
    m_idxPt = i_other.m_idxPt;
    return *this;
}

//------------------------------------------------------------------------------
bool SGraphObjSelectionPoint::operator == (const SGraphObjSelectionPoint& i_other) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;
    if (m_pGraphObj != i_other.m_pGraphObj) {
        bEqual = false;
    }
    else if (m_selPtType != i_other.m_selPtType) {
        bEqual = false;
    }
    else if (m_selPt != i_other.m_selPt) {
        bEqual = false;
    }
    else if (m_idxPt != i_other.m_idxPt) {
        bEqual = false;
    }
    return bEqual;
}

//------------------------------------------------------------------------------
bool SGraphObjSelectionPoint::operator != (const SGraphObjSelectionPoint& i_other) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/* public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
QString SGraphObjSelectionPoint::name() const
//------------------------------------------------------------------------------
{
    QString strName;
    if (m_pGraphObj != nullptr) {
        strName = m_pGraphObj->path() + ".";
    }
    if (m_selPtType == ESelectionPointType::BoundingRectangle) {
        strName += CEnumSelectionPoint(m_selPt).toString();
    } else if (m_selPtType == ESelectionPointType::PolygonPoint) {
        strName += "P" + QString::number(m_idxPt+1);
    } else if (m_selPtType == ESelectionPointType::LineCenterPoint) {
        strName += "L" + QString::number(m_idxPt+1);
    } else {
        strName += "?";
    }
    return strName;
}

//------------------------------------------------------------------------------
QString SGraphObjSelectionPoint::toString(bool i_bIncludeGraphObj) const
//------------------------------------------------------------------------------
{
    QString str;
    if (i_bIncludeGraphObj && m_pGraphObj != nullptr) {
        str = m_pGraphObj->path() + ".";
    }
    str += CEnumSelectionPointType(m_selPtType).toString() + ", ";
    if (m_selPtType == ESelectionPointType::BoundingRectangle) {
        str += CEnumSelectionPoint(m_selPt).toString();
    } else if (m_selPtType == ESelectionPointType::PolygonPoint) {
        str += "P" + QString::number(m_idxPt+1);
    } else if (m_selPtType == ESelectionPointType::LineCenterPoint) {
        str += "L" + QString::number(m_idxPt+1);
    } else {
        str += "?";
    }
    return str;
}

//------------------------------------------------------------------------------
bool SGraphObjSelectionPoint::isValid() const
//------------------------------------------------------------------------------
{
    return (m_selPtType != ESelectionPointType::Undefined);
}

/*******************************************************************************
struct SGraphObjHitInfo
*******************************************************************************/

/* public: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
SGraphObjHitInfo::SGraphObjHitInfo() :
//------------------------------------------------------------------------------
    //m_editMode(EEditMode::None),
    //m_editResizeMode(EEditResizeMode::None),
    m_selPtBoundingRect(ESelectionPoint::None),
    m_idxPolygonShapePoint(-1),
    m_idxLineSegment(-1),
    m_ptSelected(),
    m_cursor()
{
} // ctor

/* public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
bool SGraphObjHitInfo::isBoundingRectSelectionPointHit() const
//------------------------------------------------------------------------------
{
    bool bIsHit = false;
    //if (m_editMode == EEditMode::Resize && m_editResizeMode != EEditResizeMode::None) {
        bIsHit = (m_selPtBoundingRect >= ESelectionPointRectMin) && (m_selPtBoundingRect <= ESelectionPointRectMax);
    //}
    return bIsHit;
}

//------------------------------------------------------------------------------
bool SGraphObjHitInfo::isSelectionPointHit() const
//------------------------------------------------------------------------------
{
    bool bIsHit = false;
    //if (m_editMode != EEditMode::None) {
        bIsHit = (m_selPtBoundingRect != ESelectionPoint::None);
    //}
    return bIsHit;
}

//------------------------------------------------------------------------------
bool SGraphObjHitInfo::isPolygonShapePointHit() const
//------------------------------------------------------------------------------
{
    bool bIsHit = false;
    //if (m_editMode == EEditMode::MoveShapePoint) {
        bIsHit = (m_idxPolygonShapePoint >= 0);
    //}
    return bIsHit;
}

//------------------------------------------------------------------------------
bool SGraphObjHitInfo::isLineSegmentHit() const
//------------------------------------------------------------------------------
{
    bool bIsHit = false;
    //if (m_editMode != EEditMode::None) {
        bIsHit = (m_idxLineSegment >= 0);
    //}
    return bIsHit;
}

/* public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief 

    Calculated depending on editMode, editResizeMode and selected point
    (which must have been set before).
*/
//void SGraphObjHitInfo::setCursor(double i_fGraphObjRotAngle_rad)
////------------------------------------------------------------------------------
//{
//    //double fCursorAngle_rad = 0.0;
//    //bool   bSetResizeCursor = false;
//
//    //switch (m_editMode.enumerator())
//    //{
//    //    case EEditMode::Move:
//    //    {
//    //        m_cursor = Qt::SizeAllCursor;
//    //        break;
//    //    }
//    //    case EEditMode::Resize:
//    //    {
//    //        switch( m_editResizeMode.enumerator() )
//    //        {
//    //            case EEditResizeMode::ResizeAll:
//    //            {
//    //                switch( m_selPtBoundingRect.enumerator() )
//    //                {
//    //                    case ESelectionPoint::TopLeft:
//    //                    case ESelectionPoint::BottomRight:
//    //                    {
//    //                        fCursorAngle_rad = i_fGraphObjRotAngle_rad + Math::c_f135Degrees_rad; // 2nd quadrant: arrow from right/bottom -> top/left
//    //                        m_cursor = Qt::SizeFDiagCursor; /*  \  */
//    //                        bSetResizeCursor = true;
//    //                        break;
//    //                    }
//    //                    case ESelectionPoint::TopRight:
//    //                    case ESelectionPoint::BottomLeft:
//    //                    {
//    //                        fCursorAngle_rad = i_fGraphObjRotAngle_rad + Math::c_f45Degrees_rad; // 1st quadrant: arrow from bottom/left -> top/right
//    //                        m_cursor = Qt::SizeBDiagCursor; /*  /  */
//    //                        bSetResizeCursor = true;
//    //                        break;
//    //                    }
//    //                    case ESelectionPoint::RotateTop:
//    //                    case ESelectionPoint::RotateBottom:
//    //                    {
//    //                        QBitmap bmpCursor(":/ZS/Draw/CursorRotateFree16x16.png");
//    //                        m_cursor = QCursor(bmpCursor);
//    //                        break;
//    //                    }
//    //                    case ESelectionPoint::LeftCenter:
//    //                    case ESelectionPoint::RightCenter:
//    //                    case ESelectionPoint::TopCenter:
//    //                    case ESelectionPoint::BottomCenter:
//    //                    case ESelectionPoint::Center:
//    //                    default:
//    //                    {
//    //                        break;
//    //                    }
//    //                }
//    //                break;
//    //            }
//    //            case EEditResizeMode::ResizeHor:
//    //            {
//    //                fCursorAngle_rad = i_fGraphObjRotAngle_rad;
//    //                m_cursor = Qt::SizeHorCursor;
//    //                bSetResizeCursor = true;
//    //                break;
//    //            }
//    //            case EEditResizeMode::ResizeVer:
//    //            {
//    //                fCursorAngle_rad = i_fGraphObjRotAngle_rad + Math::c_f90Degrees_rad;
//    //                m_cursor = Qt::SizeVerCursor;
//    //                bSetResizeCursor = true;
//    //                break;
//    //            }
//    //            default:
//    //            {
//    //                break;
//    //            }
//    //        }
//    //        break;
//    //    }
//    //    case EEditMode::Rotate:
//    //    {
//    //        QBitmap bmpCursor(":/ZS/Draw/CursorRotateFree16x16.png");
//    //        m_cursor = QCursor(bmpCursor);
//    //        break;
//    //    }
//    //    case EEditMode::MoveShapePoint:
//    //    {
//    //        m_cursor = Qt::CrossCursor;
//    //        break;
//    //    }
//    //    case EEditMode::EditText:
//    //    {
//    //        m_cursor = Qt::IBeamCursor;
//    //        break;
//    //    }
//    //    default:
//    //    {
//    //        break;
//    //    }
//    //} // switch( editMode )
//
//    //if( bSetResizeCursor )
//    //{
//    //    // Force resulting cursor rotation angle to 1st or 2nd quadrant (0..180°)
//    //    while( fCursorAngle_rad >= Math::c_f180Degrees_rad )
//    //    {
//    //        fCursorAngle_rad -= Math::c_f180Degrees_rad;
//    //    }
//    //    while( fCursorAngle_rad < 0.0 )
//    //    {
//    //        fCursorAngle_rad += Math::c_f180Degrees_rad;
//    //    }
//    //    if( fCursorAngle_rad >= 0.0 && fCursorAngle_rad < Math::c_f45Degrees_rad/2.0 ) // 0.0 .. 22.5°
//    //    {
//    //        m_cursor = Qt::SizeHorCursor;
//    //    }
//    //    else if( fCursorAngle_rad >= Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < 3.0*Math::c_f45Degrees_rad/2.0 ) // 22.5° .. 67.5°
//    //    {
//    //        m_cursor = Qt::SizeBDiagCursor; // 1st quadrant: arrow from bottom/left -> top/right
//    //    }
//    //    else if( fCursorAngle_rad >= 3.0*Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < 5.0*Math::c_f45Degrees_rad/2.0 ) // 67.5° .. 112.5°
//    //    {
//    //        m_cursor = Qt::SizeVerCursor;
//    //    }
//    //    else if( fCursorAngle_rad >= 5.0*Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < 7.0*Math::c_f45Degrees_rad/2.0 ) // 112.5° .. 157.5°
//    //    {
//    //        m_cursor = Qt::SizeFDiagCursor; // 2nd quadrant: arrow from top/left -> bottom/right
//    //    }
//    //    else if( fCursorAngle_rad >= 7.0*Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < Math::c_f180Degrees_rad ) // 157.5° .. 180.0°
//    //    {
//    //        m_cursor = Qt::SizeHorCursor;
//    //    }
//    //} // if( bSetResizeCursor )
//} // setCursor

/* public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief 

*/
QString SGraphObjHitInfo::toString() const
//------------------------------------------------------------------------------
{
    QString str = //"EditMode:" + m_editMode.toString() +
        //", ResizeMode:" + m_editResizeMode.toString() +
        ", SelPtBoundingRect: " + m_selPtBoundingRect.toString() +
        ", PolygonShapePoint: " + QString::number(m_idxPolygonShapePoint) +
        ", LineSegment: " + QString::number(m_idxLineSegment) +
        ", PointSelected {" + qPoint2Str(m_ptSelected) + "}" +
        ", Cursor:" + qCursorShape2Str(m_cursor.shape());
    return str;
}


/*******************************************************************************
struct SPolarCoors
*******************************************************************************/

/*==============================================================================
public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
SPolarCoors SPolarCoors::fromString(const QString& i_str, const QString& i_strSeparator, bool* i_pbOk)
//------------------------------------------------------------------------------
{
    bool bOk = false;
    SPolarCoors polarCoors;
    QStringList strlst = i_str.split(i_strSeparator);
    if (strlst.size() == 2) {
        strlst[0] = strlst[0].trimmed();
        strlst[1] = strlst[1].trimmed();
        double fLength_px = strlst[0].toDouble(&bOk);
        if (!bOk) {
            CPhysVal physValLength(Units.Length.px);
            try {
                PhysVal::TFormatResult formatResult = physValLength.setVal(strlst[0]);
                if (formatResult == PhysVal::FormatResult::Ok) {
                    fLength_px = physValLength.getVal(Units.Length.px);
                    bOk = true;
                }
            } catch (...) {
                bOk = false;
            }
        }
        if (bOk) {
            double fAngle_degrees = strlst[1].toDouble(&bOk);
            if (!bOk) {
                CPhysVal physValAngle(Units.Angle.Degree);
                try {
                    PhysVal::TFormatResult formatResult = physValAngle.setVal(strlst[1]);
                    if (formatResult == PhysVal::FormatResult::Ok) {
                        fAngle_degrees = physValAngle.getVal(Units.Angle.Degree);
                        bOk = true;
                    }
                } catch (...) {
                    bOk = false;
                }
            }
            if (bOk) {
                polarCoors.m_fLength_px = fLength_px;
                polarCoors.m_fAngle_degrees = fAngle_degrees;
            }
        }
    }
    if (i_pbOk != nullptr) {
        *i_pbOk = bOk;
    }
    return polarCoors;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
SPolarCoors::SPolarCoors() :
//------------------------------------------------------------------------------
    m_fLength_px(0.0),
    m_fAngle_degrees(0.0)
{
}

//------------------------------------------------------------------------------
SPolarCoors::SPolarCoors(double i_fLength_px, double i_fAngle_degrees) :
//------------------------------------------------------------------------------
    m_fLength_px(i_fLength_px),
    m_fAngle_degrees(i_fAngle_degrees)
{
}

//------------------------------------------------------------------------------
SPolarCoors::SPolarCoors(const SPolarCoors& i_other) :
//------------------------------------------------------------------------------
    m_fLength_px(i_other.m_fLength_px),
    m_fAngle_degrees(i_other.m_fAngle_degrees)
{
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool SPolarCoors::operator == (const SPolarCoors& i_other) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;
    if (m_fLength_px != i_other.m_fLength_px) {
        bEqual = false;
    }
    else if (m_fAngle_degrees != i_other.m_fAngle_degrees) {
        bEqual = false;
    }
    return bEqual;
}

//------------------------------------------------------------------------------
bool SPolarCoors::operator != (const SPolarCoors& i_other) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/*==============================================================================
public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
QString SPolarCoors::toString(bool i_bAddUnit, const QString& i_strSeparator) const
//------------------------------------------------------------------------------
{
    QString str = QString::number(m_fLength_px);
    if (i_bAddUnit) {
        str += " " + Units.Length.px.symbol();
    }
    str += i_strSeparator + QString::number(m_fAngle_degrees);
    if (i_bAddUnit) {
        str += " " + Units.Angle.Degree.symbol();
    }
    return str;
}


/*******************************************************************************
struct SLabelDscr
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
SLabelDscr::SLabelDscr() :
//------------------------------------------------------------------------------
    m_strKey(),
    m_labelType(EGraphObjTypeUndefined),
    m_strText(),
    m_selPt1(),
    m_selPt2(),
    m_polarCoorsToLinkedSelPt(),
    m_bLabelIsVisible(false),
    m_bShowAnchorLine(false)
{
}

//------------------------------------------------------------------------------
SLabelDscr::SLabelDscr(EGraphObjType i_labelType) :
//------------------------------------------------------------------------------
    m_strKey(),
    m_labelType(i_labelType),
    m_strText(),
    m_selPt1(),
    m_selPt2(),
    m_polarCoorsToLinkedSelPt(),
    m_bLabelIsVisible(false),
    m_bShowAnchorLine(false)
{
}

//------------------------------------------------------------------------------
SLabelDscr::SLabelDscr(EGraphObjType i_labelType, const QString& i_strKey) :
//------------------------------------------------------------------------------
    m_strKey(i_strKey),
    m_labelType(i_labelType),
    m_strText(),
    m_selPt1(),
    m_selPt2(),
    m_polarCoorsToLinkedSelPt(),
    m_bLabelIsVisible(false),
    m_bShowAnchorLine(false)
{
}

//------------------------------------------------------------------------------
SLabelDscr::SLabelDscr(
    EGraphObjType i_labelType, const QString& i_strKey,
    const SGraphObjSelectionPoint& i_selPt) :
//------------------------------------------------------------------------------
    m_strKey(i_strKey),
    m_labelType(i_labelType),
    m_strText(),
    m_selPt1(i_selPt),
    m_selPt2(),
    m_polarCoorsToLinkedSelPt(),
    m_bLabelIsVisible(false),
    m_bShowAnchorLine(false)
{
}

//------------------------------------------------------------------------------
SLabelDscr::SLabelDscr(
    EGraphObjType i_labelType, const QString& i_strKey,
    const SGraphObjSelectionPoint& i_selPt1,
    const SGraphObjSelectionPoint& i_selPt2) :
//------------------------------------------------------------------------------
    m_strKey(i_strKey),
    m_labelType(i_labelType),
    m_strText(),
    m_selPt1(i_selPt1),
    m_selPt2(i_selPt2),
    m_polarCoorsToLinkedSelPt(),
    m_bLabelIsVisible(false),
    m_bShowAnchorLine(false)
{
}

//------------------------------------------------------------------------------
SLabelDscr::SLabelDscr(
    EGraphObjType i_labelType, const QString& i_strKey, const QString& i_strText,
    const SGraphObjSelectionPoint& i_selPt) :
//------------------------------------------------------------------------------
    m_strKey(i_strKey),
    m_labelType(i_labelType),
    m_strText(i_strText),
    m_selPt1(i_selPt),
    m_selPt2(),
    m_polarCoorsToLinkedSelPt(),
    m_bLabelIsVisible(false),
    m_bShowAnchorLine(false)
{
}

//------------------------------------------------------------------------------
SLabelDscr::SLabelDscr(
    EGraphObjType i_labelType, const QString& i_strKey, const QString& i_strText,
    const SGraphObjSelectionPoint& i_selPt1, const SGraphObjSelectionPoint& i_selPt2) :
//------------------------------------------------------------------------------
    m_strKey(i_strKey),
    m_labelType(i_labelType),
    m_strText(i_strText),
    m_selPt1(i_selPt1),
    m_selPt2(i_selPt2),
    m_polarCoorsToLinkedSelPt(),
    m_bLabelIsVisible(false),
    m_bShowAnchorLine(false)
{
}

//------------------------------------------------------------------------------
SLabelDscr::SLabelDscr(const SLabelDscr& i_other) :
//------------------------------------------------------------------------------
    m_strKey(i_other.m_strKey),
    m_labelType(i_other.m_labelType),
    m_strText(i_other.m_strText),
    m_selPt1(i_other.m_selPt1),
    m_selPt2(i_other.m_selPt2),
    m_polarCoorsToLinkedSelPt(i_other.m_polarCoorsToLinkedSelPt),
    m_bLabelIsVisible(i_other.m_bLabelIsVisible),
    m_bShowAnchorLine(i_other.m_bShowAnchorLine)
{
}

} // namespace Draw

} // namespace ZS
