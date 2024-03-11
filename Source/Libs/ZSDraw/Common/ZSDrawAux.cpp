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

#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Common/ZSDrawSettings.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjGroup.h"
#include "ZSPhysVal/ZSPhysVal.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysMath.h"

#include <QtCore/qmimedata.h>
#include <QtWidgets/qgraphicsitem.h>

#if QT_VERSION < 0x050000
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::Draw;


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

/*==============================================================================
QPainterPath::ElementType
==============================================================================*/

const SEnumEntry s_arEnumStrPainterPathElemType[] =
{
    /*  0 */ SEnumEntry( QPainterPath::MoveToElement,      "MoveTo"      ),
    /*  1 */ SEnumEntry( QPainterPath::LineToElement,      "LineTo"      ),
    /*  2 */ SEnumEntry( QPainterPath::CurveToElement,     "CurveTo"     ),
    /*  3 */ SEnumEntry( QPainterPath::CurveToDataElement, "CurveToData" )
};

//------------------------------------------------------------------------------
QString ZS::Draw::qPainterPathElemType2Str( int i_type )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str( s_arEnumStrPainterPathElemType, _ZSArrLen(s_arEnumStrPainterPathElemType), i_type );
}

//------------------------------------------------------------------------------
QString ZS::Draw::qPainterPath2Str( QGraphicsItem* i_pGraphObj, const QPainterPath& i_painterPath )
//------------------------------------------------------------------------------
{
    QString str;
    QPainterPath painterPatherSimplified = i_painterPath.simplified();
    str = "[" + QString::number(painterPatherSimplified.elementCount()) + "]";
    if (painterPatherSimplified.elementCount() > 0) {
        str += "(";
        for (int idx = 0; idx < painterPatherSimplified.elementCount(); idx++) {
            if (idx > 0) str += ", ";
            QPainterPath::Element elem = painterPatherSimplified.elementAt(idx);
            QPointF ptElem = i_pGraphObj->mapToScene(QPointF(elem.x, elem.y));
            QString strElem = qPainterPathElemType2Str(elem.type) + " {" + point2Str(ptElem) + "}";
            str += "[" + QString::number(idx) + "] {" + strElem + "}";
        }
        str += ")";
    }
    return str;
}


/*==============================================================================
QTransform
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Draw::qTransformationType2Str( int i_type )
//------------------------------------------------------------------------------
{
    QString str;

    if (i_type == QTransform::TxNone) {
        str = "None";
    }
    else /*if (i_type != QTransform::TxNone)*/ {
        if (i_type & QTransform::TxTranslate) {
            str = "Translate";
        }
        if( i_type & QTransform::TxScale ) {
            if( !str.isEmpty() ) {
                str += "|";
            }
            str += "Scale";
        }
        if( i_type & QTransform::TxRotate ) {
            if( !str.isEmpty() ) {
                str += "|";
            }
            str += "Rotate";
        }
        if( i_type & QTransform::TxShear ) {
            if( !str.isEmpty() ) {
                str += "|";
            }
            str += "Shear";
        }
        if( i_type & QTransform::TxProject ) {
            if( !str.isEmpty() ) {
                str += "|";
            }
            str += "Project";
        }
    }
    return str;
}

//------------------------------------------------------------------------------
QStringList ZS::Draw::qTransformation2StrList( const QTransform& i_transform )
//------------------------------------------------------------------------------
{
    QStringList strlst;

    QString strType = qTransformationType2Str(i_transform.type());
    QString strFormula;

    /*
        m11 m12 m13
        m21 m22 m23
        m31 m32 m33

    Affine:
        x' = m11*x + m21*y + m31;
        y' = m22*y + m12*x + m32;

    Not Affine:
        w' = m32*x + m23*y + m33
        x' = x' / w'
        y' = y' / w'
    */

    QString strm11 = QString::number(fabs(i_transform.m11())); // = scale X (without rotation)
    QString strm12 = QString::number(fabs(i_transform.m12()));
    QString strm13 = QString::number(fabs(i_transform.m13()));
    QString strm21 = QString::number(fabs(i_transform.m21()));
    QString strm22 = QString::number(fabs(i_transform.m22())); // = scale Y (without rotation)
    QString strm23 = QString::number(fabs(i_transform.m23()));
    QString strm31 = QString::number(fabs(i_transform.m31())); // = dx
    QString strm32 = QString::number(fabs(i_transform.m32())); // = dy
    QString strm33 = QString::number(fabs(i_transform.m33()));

    strlst << strType; // Idx 0

    strFormula = "x' = ";

    if( i_transform.m11() > 0.0 ) {
        strFormula += strm11 + "*x";
    }
    else if( i_transform.m11() < 0.0 ) {
        strFormula += "-" + strm11 + "*x";
    }
    if( i_transform.m21() > 0.0 ) {
        strFormula += " + " + strm21 + "*y";
    }
    else if( i_transform.m21() < 0.0 ) {
        strFormula += " - " + strm21 + "*y";
    }
    if( i_transform.m31() > 0.0 ) {
        strFormula += " + " + strm31;
    }
    else if( i_transform.m31() < 0.0 ) {
        strFormula += " - " + strm31;
    }
    strlst << strFormula; // Idx 1

    strFormula = "y' = ";

    if( i_transform.m22() > 0.0 ) {
        strFormula += strm22 + "*y";
    }
    else if( i_transform.m22() < 0.0 ) {
        strFormula += "-" + strm22 + "*y";
    }
    if( i_transform.m12() > 0.0 ) {
        strFormula += " + " + strm12 + "*x";
    }
    else if( i_transform.m12() < 0.0 ) {
        strFormula += " - " + strm12 + "*x";
    }
    if( i_transform.m32() > 0.0 ) {
        strFormula += " + " + strm32;
    }
    else if( i_transform.m32() < 0.0 ) {
        strFormula += " - " + strm32;
    }

    strlst << strFormula; // Idx 2

    if( !i_transform.isAffine() ) {
        strFormula = "w' = ";

        if( i_transform.m32() > 0.0 ) {
            strFormula += strm32 + "*x";
        }
        else if( i_transform.m32() < 0.0 ) {
            strFormula += "-" + strm32 + "*x";
        }
        if( i_transform.m23() > 0.0 ) {
            strFormula += " + " + strm23 + "*y";
        }
        else if( i_transform.m23() < 0.0 ) {
            strFormula += " - " + strm23 + "*y";
        }
        if( i_transform.m33() > 0.0 ) {
            strFormula += " + " + strm33;
        }
        else if( i_transform.m33() < 0.0 ) {
            strFormula += " - " + strm33;
        }
        strlst << strFormula; // Idx 3

        strlst << "x' /= w'"; // Idx 4
        strlst << "y' /= w'"; // Idx 5
    }

    return strlst;

} // qTransformation2StrList


/*==============================================================================
QGraphicsItem::GraphicsItemChange
==============================================================================*/

const ZS::System::SEnumEntry s_arEnumStrGraphicsItemChange[] =
{
    /*  0 */ SEnumEntry( QGraphicsItem::ItemPositionChange, "PositionChange" ),
    /*  1 */ SEnumEntry( 1 /*QGraphicsItem::ItemMatrixChange*/, "MatrixChange" ),
    /*  2 */ SEnumEntry( QGraphicsItem::ItemVisibleChange, "VisibleChange" ),
    /*  3 */ SEnumEntry( QGraphicsItem::ItemEnabledChange, "EnabledChange" ),
    /*  4 */ SEnumEntry( QGraphicsItem::ItemSelectedChange, "SelectedChange" ),
    /*  5 */ SEnumEntry( QGraphicsItem::ItemParentChange, "ParentChange" ),
    /*  6 */ SEnumEntry( QGraphicsItem::ItemChildAddedChange, "ChildAddedChange" ),
    /*  7 */ SEnumEntry( QGraphicsItem::ItemChildRemovedChange, "ChildRemovedChange" ),
    /*  8 */ SEnumEntry( QGraphicsItem::ItemTransformChange, "TransformChange" ),
    /*  9 */ SEnumEntry( QGraphicsItem::ItemPositionHasChanged, "PositionHasChanged" ),
    /* 10 */ SEnumEntry( QGraphicsItem::ItemTransformHasChanged, "TransformHasChanged" ),
    /* 11 */ SEnumEntry( QGraphicsItem::ItemSceneChange, "SceneChange" ),
    /* 12 */ SEnumEntry( QGraphicsItem::ItemVisibleHasChanged, "VisibleHasChanged" ),
    /* 13 */ SEnumEntry( QGraphicsItem::ItemEnabledHasChanged, "EnabledHasChanged" ),
    /* 14 */ SEnumEntry( QGraphicsItem::ItemSelectedHasChanged, "SelectedHasChanged" ),
    /* 15 */ SEnumEntry( QGraphicsItem::ItemParentHasChanged, "ParentHasChanged" ),
    /* 16 */ SEnumEntry( QGraphicsItem::ItemSceneHasChanged, "SceneHasChanged" ),
    /* 17 */ SEnumEntry( QGraphicsItem::ItemCursorChange, "CursorChange" ),
    /* 18 */ SEnumEntry( QGraphicsItem::ItemCursorHasChanged, "CursorHasChanged" ),
    /* 19 */ SEnumEntry( QGraphicsItem::ItemToolTipChange, "ToolTipChange" ),
    /* 20 */ SEnumEntry( QGraphicsItem::ItemToolTipHasChanged, "ToolTipHasChanged" ),
    /* 21 */ SEnumEntry( QGraphicsItem::ItemFlagsChange, "FlagsChange" ),
    /* 22 */ SEnumEntry( QGraphicsItem::ItemFlagsHaveChanged, "FlagsHaveChanged" ),
    /* 23 */ SEnumEntry( QGraphicsItem::ItemZValueChange, "ZValueChange" ),
    /* 24 */ SEnumEntry( QGraphicsItem::ItemZValueHasChanged, "ZValueHasChanged" ),
    /* 25 */ SEnumEntry( QGraphicsItem::ItemOpacityChange, "OpacityChange" ),
    /* 26 */ SEnumEntry( QGraphicsItem::ItemOpacityHasChanged, "OpacityHasChanged" ),
    /* 27 */ SEnumEntry( QGraphicsItem::ItemScenePositionHasChanged, "ScenePositionHasChanged" ),
    #if QT_VERSION >= 0x040700
    /* 28 */ SEnumEntry( QGraphicsItem::ItemRotationChange, "RotationChange" ),
    /* 29 */ SEnumEntry( QGraphicsItem::ItemRotationHasChanged, "RotationHasChanged" ),
    /* 30 */ SEnumEntry( QGraphicsItem::ItemScaleChange, "ScaleChange" ),
    /* 31 */ SEnumEntry( QGraphicsItem::ItemScaleHasChanged, "ScaleHasChanged" ),
    /* 32 */ SEnumEntry( QGraphicsItem::ItemTransformOriginPointChange, "TransformOriginPointChange" ),
    /* 33 */ SEnumEntry( QGraphicsItem::ItemTransformOriginPointHasChanged, "TransformOriginPointHasChanged" )
    #else
    /* 28 */ SEnumEntry( 28, "28" )
    #endif
};

//------------------------------------------------------------------------------
QString ZS::Draw::qGraphicsItemChange2Str( int i_change, const QVariant& i_value, bool i_bIncludeChange )
//------------------------------------------------------------------------------
{
    QString str;
    if (i_bIncludeChange) {
        str = SEnumEntry::enumerator2Str(
            s_arEnumStrGraphicsItemChange, _ZSArrLen(s_arEnumStrGraphicsItemChange), i_change);
    }
    if (!str.isEmpty()) str += ", ";
    str += qVariantType2Str(i_value.type());
    if (((i_change == QGraphicsItem::ItemEnabledChange) || (i_change == QGraphicsItem::ItemEnabledHasChanged))
     || ((i_change == QGraphicsItem::ItemSelectedChange) || (i_change == QGraphicsItem::ItemSelectedHasChanged))
     || ((i_change == QGraphicsItem::ItemVisibleChange) || (i_change == QGraphicsItem::ItemVisibleHasChanged))) {
        str += " {" + bool2Str(i_value.toBool()) + "}";
    }
    else if ((i_change == QGraphicsItem::ItemFlagsChange) || (i_change == QGraphicsItem::ItemFlagsHaveChanged)) {
        str += " {" + qGraphicsItemFlags2Str(i_value.toUInt()) + "}";
    }
    else if ((i_change == QGraphicsItem::ItemParentChange) || (i_change == QGraphicsItem::ItemParentHasChanged)
          || (i_change == QGraphicsItem::ItemChildAddedChange) || (i_change == QGraphicsItem::ItemChildRemovedChange)) {
        QGraphicsItem* pGraphicsItem = i_value.value<QGraphicsItem*>();
        if (pGraphicsItem != nullptr) {
            CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);
            str += QString(pGraphObj == nullptr ? "null" : pGraphObj->path());
        } else {
            str += "null";
        }
    }
    else if (i_value.type() == QVariant::Point) {
        str += " {" + point2Str(i_value.toPoint()) + "}";
    } else if (i_value.type() == QVariant::PointF) {
        str += " {" + point2Str(i_value.toPointF()) + "}";
    } else if (i_value.type() == QVariant::Bool) {
        str += " {" + bool2Str(i_value.toBool()) + "}";
    } else if (i_value.type() == QVariant::Transform) {
        str += " {" + qTransformation2StrList(i_value.value<QTransform>()).join(", ") + "}";
    } else if (i_value.type() == QVariant::Cursor) {
        str += " {Pos {" + qPoint2Str(i_value.value<QCursor>().pos()) + "}}";
    } else {
        str += " {" + i_value.toString() + "}";
    }
    return str;
}

//------------------------------------------------------------------------------
QString ZS::Draw::qGraphicsItemFlags2Str( quint32 i_flags )
//------------------------------------------------------------------------------
{
    QString str;
    QGraphicsItem::GraphicsItemFlags flags = i_flags;
    if (flags.testFlag(QGraphicsItem::GraphicsItemFlag::ItemIsMovable)) {
        if (!str.isEmpty()) str += "|";
        str += "IsMovable";
    }
    if (flags.testFlag(QGraphicsItem::GraphicsItemFlag::ItemIsSelectable)) {
        if (!str.isEmpty()) str += "|";
        str += "IsSelectable";
    }
    if (flags.testFlag(QGraphicsItem::GraphicsItemFlag::ItemIsFocusable)) {
        if (!str.isEmpty()) str += "|";
        str += "IsFocusable";
    }
    if (flags.testFlag(QGraphicsItem::GraphicsItemFlag::ItemClipsToShape)) {
        if (!str.isEmpty()) str += "|";
        str += "ClipsToShape";
    }
    if (flags.testFlag(QGraphicsItem::GraphicsItemFlag::ItemClipsChildrenToShape)) {
        if (!str.isEmpty()) str += "|";
        str += "ClipsChildrenToShape";
    }
    if (flags.testFlag(QGraphicsItem::GraphicsItemFlag::ItemIgnoresTransformations)) {
        if (!str.isEmpty()) str += "|";
        str += "IgnoresTransformations";
    }
    if (flags.testFlag(QGraphicsItem::GraphicsItemFlag::ItemIgnoresParentOpacity)) {
        if (!str.isEmpty()) str += "|";
        str += "IgnoresParentOpacity";
    }
    if (flags.testFlag(QGraphicsItem::GraphicsItemFlag::ItemDoesntPropagateOpacityToChildren)) {
        if (!str.isEmpty()) str += "|";
        str += "DoesntPropagateOpacityToChildren";
    }
    if (flags.testFlag(QGraphicsItem::GraphicsItemFlag::ItemStacksBehindParent)) {
        if (!str.isEmpty()) str += "|";
        str += "StacksBehindParent";
    }
    if (flags.testFlag(QGraphicsItem::GraphicsItemFlag::ItemUsesExtendedStyleOption)) {
        if (!str.isEmpty()) str += "|";
        str += "UsesExtendedStyleOption";
    }
    if (flags.testFlag(QGraphicsItem::GraphicsItemFlag::ItemHasNoContents)) {
        if (!str.isEmpty()) str += "|";
        str += "HasNoContents";
    }
    if (flags.testFlag(QGraphicsItem::GraphicsItemFlag::ItemSendsGeometryChanges)) {
        if (!str.isEmpty()) str += "|";
        str += "SendsGeometryChanges";
    }
    if (flags.testFlag(QGraphicsItem::GraphicsItemFlag::ItemAcceptsInputMethod)) {
        if (!str.isEmpty()) str += "|";
        str += "AcceptsInputMethod";
    }
    if (flags.testFlag(QGraphicsItem::GraphicsItemFlag::ItemNegativeZStacksBehindParent)) {
        if (!str.isEmpty()) str += "|";
        str += "NegativeZStacksBehindParent";
    }
    if (flags.testFlag(QGraphicsItem::GraphicsItemFlag::ItemIsPanel)) {
        if (!str.isEmpty()) str += "|";
        str += "IsPanel";
    }
    if (flags.testFlag(QGraphicsItem::GraphicsItemFlag::ItemIsFocusScope)) {
        if (!str.isEmpty()) str += "|";
        str += "IsFocusScope";
    }
    if (flags.testFlag(QGraphicsItem::GraphicsItemFlag::ItemSendsScenePositionChanges)) {
        if (!str.isEmpty()) str += "|";
        str += "SendsScenePositionChanges";
    }
    if (flags.testFlag(QGraphicsItem::GraphicsItemFlag::ItemStopsClickFocusPropagation)) {
        if (!str.isEmpty()) str += "|";
        str += "StopsClickFocusPropagation";
    }
    if (flags.testFlag(QGraphicsItem::GraphicsItemFlag::ItemStopsFocusHandling)) {
        if (!str.isEmpty()) str += "|";
        str += "StopsFocusHandling";
    }
    if (flags.testFlag(QGraphicsItem::GraphicsItemFlag::ItemContainsChildrenInShape)) {
        if (!str.isEmpty()) str += "|";
        str += "ContainsChildrenInShape";
    }
    return str;
}

/*==============================================================================
QGraphicsItem::QGraphicsSceneEvents
==============================================================================*/

//------------------------------------------------------------------------------
bool ZS::Draw::isGraphicsSceneDragDropEvent( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    return (dynamic_cast<QGraphicsSceneDragDropEvent*>(i_pEv) != nullptr);
}

//------------------------------------------------------------------------------
QString ZS::Draw::qGraphicsSceneDragDropEvent2Str( QGraphicsSceneDragDropEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString str = "Pos {" + point2Str(i_pEv->pos()) + "}" +
        ", ScenePos {" + point2Str(i_pEv->scenePos()) + "}" +
        ", ScreenPos {" + point2Str(i_pEv->screenPos()) + "}" +
        ", Buttons {" + qMouseButtons2Str(i_pEv->buttons()) + "}" +
        ", KeyboardModifiers {" + qKeyboardModifiers2Str(i_pEv->modifiers()) + "}" +
        ", PossibleActions {" + qDropActions2Str(i_pEv->possibleActions()) +
        ", DropAction: " + qDropAction2Str(i_pEv->dropAction()) +
        ", ProposedAction: " + qDropAction2Str(i_pEv->proposedAction()) +
        ", MimeData: " + QString(i_pEv->mimeData() == nullptr ? "null" : i_pEv->mimeData()->text());
    return str;
}

//------------------------------------------------------------------------------
bool ZS::Draw::isGraphicsSceneHoverEvent( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    return (dynamic_cast<QGraphicsSceneHoverEvent*>(i_pEv) != nullptr);
}

//------------------------------------------------------------------------------
QString ZS::Draw::qGraphicsSceneHoverEvent2Str( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString str = "Pos {" + point2Str(i_pEv->pos()) + "}" +
        ", ScenePos {" + point2Str(i_pEv->scenePos()) + "}" +
        ", ScreenPos {" + point2Str(i_pEv->screenPos()) + "}";
    return str;
}

//------------------------------------------------------------------------------
bool ZS::Draw::isGraphicsSceneMouseEvent( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    return (dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv) != nullptr);
}

//------------------------------------------------------------------------------
QString ZS::Draw::qGraphicsSceneMouseEvent2Str( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    return "Pos {" + point2Str(i_pEv->pos()) + "}" +
        ", ScenePos {" + point2Str(i_pEv->scenePos()) + "}" +
        ", ScreenPos {" + point2Str(i_pEv->screenPos()) + "}" +
        ", Button: " + qMouseButton2Str(i_pEv->button()) +
        ", Buttons: " + qMouseButtons2Str(i_pEv->buttons()) +
        ", Modifiers: " + qKeyboardModifiers2Str(i_pEv->modifiers());
}

//------------------------------------------------------------------------------
bool ZS::Draw::isGraphicsSceneResizeEvent( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    return (dynamic_cast<QGraphicsSceneResizeEvent*>(i_pEv) != nullptr);
}

//------------------------------------------------------------------------------
QString ZS::Draw::qGraphicsSceneResizeEvent2Str( QGraphicsSceneResizeEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString str = "NewSize {" + qSize2Str(i_pEv->newSize()) + "}" +
        ", OldSize {" + qSize2Str(i_pEv->oldSize()) + "}";
    return str;
}

/*******************************************************************************
exported methods implementation
*******************************************************************************/

//------------------------------------------------------------------------------
/*! @brief Returns the polygon points to draw an arrow head at the line end.

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

    Base line types for the arrow head:

         NoLine           Normal         Indented
            ^               ^               ^
           /|\             / \             / \
          / | \           /   \           /   \
         /  |  \         /     \         /  ^  \
        /   |   \       /_______\       /__/ \__\
            |               |               |
            |               |               |
            |               |               |

    @param [in] i_line
        Line for which the polygon points should be calculated.
    @param [in] i_drawSettings
        The draw settings contain the styling for the arrow heads.
    @param [out] i_pplgLineStart
        If not nullptr the polygon points for the arrow head at the line start
        point (P1) are calculated and stored in the passed polygon.
    @param [out] i_pplgLineEnd
        If not nullptr the polygon points for the arrow head at the line end
        point (P2) are calculated and stored in the passed polygon.
*/
void ZS::Draw::getLineEndPolygons(
    const QLineF& i_line,
    const CDrawSettings& i_drawSettings,
    QPolygonF* i_pplgLineStart,
    QPolygonF* i_pplgLineEnd )
//------------------------------------------------------------------------------
{
    if( (i_drawSettings.getLineEndStyle(ELinePoint::Start) != ELineEndStyle::Normal)
     || (i_drawSettings.getLineEndStyle(ELinePoint::End) != ELineEndStyle::Normal) )
    {
        double fdx = i_line.dx();
        double fdy = i_line.dy();
        double flen = i_line.length();

        double fAngle_rad = 0.0;
        if (fabs(flen) > 0.0) {
            fAngle_rad = acos(fdx/flen);
        }
        // Right of y axis ..
        if (fdx >= 0.0) {
            // "Above" x axis ( 1. quadrant, angle returned by acos: 0° <= f <= 90°) ..
            if (fdy <= 0.0) {
            }
            // "Below" x axis ( 2. quadrant, angle returned by acos: 90° <= f <= 180°) ..
            else {
                fAngle_rad *= -1.0;
            }
        }
        // Left of y axis ..
        else
        {
            // "Above" x axis ( 3. quadrant, angle returned by acos: 0° <= f <= 90°) ..
            if (fdy <= 0.0) {
            }
            // "Below" x axis ( 4. quadrant, angle returned by acos: 90° <= f <= 180°) ..
            else {
                fAngle_rad *= -1.0;
            }
        }

        #ifdef _DEBUG
        //double fAngle_deg = 360.0*fAngle_rad/Math::c_f2PI;
        #endif

        for (CEnumLinePoint linePoint = 0; linePoint < CEnumLinePoint::count(); linePoint++) {
            CEnumArrowHeadWidth width = i_drawSettings.getArrowHeadWidth(linePoint);
            CEnumArrowHeadLength length = i_drawSettings.getArrowHeadLength(linePoint);
            CEnumArrowHeadBaseLineType baseLineType = i_drawSettings.getArrowHeadBaseLineType(linePoint);
            QPointF pt;

            if ((linePoint == ELinePoint::Start) && (i_pplgLineStart != nullptr)) {
                QPolygonF plgLineStart;
                pt.setX(i_line.p1().x() + arrowHeadLength2dx(length));
                pt.setY(i_line.p1().y() + arrowHeadWidth2dy(width)/2.0);
                pt = rotatePoint(i_line.p1(), pt, fAngle_rad);
                plgLineStart.append(pt); // Pt1
                plgLineStart.append(i_line.p1()); // Pt2

                pt.setX(i_line.p1().x() + arrowHeadLength2dx(length));
                pt.setY(i_line.p1().y() - arrowHeadWidth2dy(width)/2.0);
                pt = rotatePoint(i_line.p1(), pt, fAngle_rad);
                plgLineStart.append(pt); // Pt3

                if (baseLineType == EArrowHeadBaseLineType::Normal) {
                    pt.setX(i_line.p1().x() + arrowHeadLength2dx(length));
                    pt.setY(i_line.p1().y());
                    pt = rotatePoint(i_line.p1(), pt, fAngle_rad);
                    plgLineStart.append(pt); // Pt4
                }
                else if (baseLineType == EArrowHeadBaseLineType::Indented) {
                    pt.setX(i_line.p1().x() + arrowHeadLength2dx(length)/2.0);
                    pt.setY(i_line.p1().y());
                    pt = rotatePoint(i_line.p1(), pt, fAngle_rad);
                    plgLineStart.append(pt); // Pt4
                }
                *i_pplgLineStart = plgLineStart;
            }
            else if ((linePoint == ELinePoint::End) && (i_pplgLineEnd != nullptr)) {
                QPolygonF plgLineEnd;
                pt.setX(i_line.p2().x() - arrowHeadLength2dx(length));
                pt.setY(i_line.p2().y() + arrowHeadWidth2dy(width)/2.0);
                pt = rotatePoint(i_line.p2(), pt, fAngle_rad);
                plgLineEnd.append(pt); // Pt1
                plgLineEnd.append(i_line.p2()); // Pt2

                pt.setX(i_line.p2().x() - arrowHeadLength2dx(length));
                pt.setY(i_line.p2().y() - arrowHeadWidth2dy(width)/2.0);
                pt = rotatePoint(i_line.p2(), pt, fAngle_rad);
                plgLineEnd.append(pt); // Pt3

                if (baseLineType == EArrowHeadBaseLineType::Normal) {
                    pt.setX(i_line.p2().x() - arrowHeadLength2dx(length));
                    pt.setY(i_line.p2().y() );
                    pt = rotatePoint(i_line.p2(), pt, fAngle_rad);
                    plgLineEnd.append(pt); // Pt4
                }
                else if (baseLineType == EArrowHeadBaseLineType::Indented) {
                    pt.setX(i_line.p2().x() - arrowHeadLength2dx(length)/2.0);
                    pt.setY(i_line.p2().y());
                    pt = rotatePoint(i_line.p2(), pt, fAngle_rad);
                    plgLineEnd.append(pt); // Pt4
                }
                *i_pplgLineEnd = plgLineEnd;
            }
        }
    } // if( i_drawSettings.getLineEndStyle(ELinePoint::Start) != ELineEndStyle::Normal ..
} // getLineEndPolygons

//------------------------------------------------------------------------------
void ZS::Draw::getMaxRectArea(
    double  i_fXLeft1,    double  i_fYTop1,
    double  i_fXRight1,   double  i_fYBottom1,
    double  i_fXLeft2,    double  i_fYTop2,
    double  i_fXRight2,   double  i_fYBottom2,
    double& o_fXLeftMin,  double& o_fYTopMin,
    double& o_fXRightMax, double& o_fYBottomMax )
//------------------------------------------------------------------------------
{
    o_fXLeftMin = i_fXLeft1;
    o_fYTopMin = i_fYTop1;
    o_fXRightMax = i_fXRight1;
    o_fYBottomMax = i_fYBottom1;

    if (o_fXLeftMin > i_fXLeft2) {
        o_fXLeftMin = i_fXLeft2;
    }
    if (o_fYTopMin > i_fYTop2) {
        o_fYTopMin = i_fYTop2;
    }
    if (o_fXRightMax < i_fXRight2) {
        o_fXRightMax = i_fXRight2;
    }
    if (o_fYBottomMax < i_fYBottom2) {
        o_fYBottomMax = i_fYBottom2;
    }
}

//------------------------------------------------------------------------------
void ZS::Draw::getMaxRectArea(
    const CPhysVal& i_physValLeft1, const CPhysVal& i_physValTop1,
    const CPhysVal& i_physValRight1, const CPhysVal& i_physValBottom1,
    const CPhysVal& i_physValLeft2, const CPhysVal& i_physValTop2,
    const CPhysVal& i_physValRight2, const CPhysVal& i_physValBottom2,
    CPhysVal& o_physValLeftMax, CPhysVal& o_physValTopMax,
    CPhysVal& o_physValRightMax, CPhysVal& o_physValBottomMax )
//------------------------------------------------------------------------------
{
    o_physValLeftMax = i_physValLeft1;
    o_physValTopMax = i_physValTop1;
    o_physValRightMax = i_physValRight1;
    o_physValBottomMax = i_physValBottom1;

    if (o_physValLeftMax > i_physValLeft2) {
        o_physValLeftMax = i_physValLeft2;
    }
    if (o_physValTopMax > i_physValTop2) {
        o_physValTopMax = i_physValTop2;
    }
    if (o_physValRightMax < i_physValRight2) {
        o_physValRightMax = i_physValRight2;
    }
    if (o_physValBottomMax < i_physValBottom2) {
        o_physValBottomMax = i_physValBottom2;
    }
}

//------------------------------------------------------------------------------
QRectF ZS::Draw::resizeRect(
    const QRectF& i_rect,
    const CEnumSelectionPoint& i_selPt,
    const QPointF& i_ptSel,
    CEnumSelectionPoint* o_pSelPt )
//------------------------------------------------------------------------------
{
    CEnumSelectionPoint selPt = i_selPt;

    QRectF rctResult = i_rect;

    double fXPos = i_ptSel.x();
    double fYPos = i_ptSel.y();

    switch (i_selPt.enumerator()) {
        case ESelectionPoint::TopLeft: {
            // Moved right and below right bottom corner ..
            if (fXPos > rctResult.right() && fYPos > rctResult.bottom()) {
                rctResult.setLeft(rctResult.right());
                rctResult.setRight(fXPos);
                rctResult.setTop(rctResult.bottom());
                rctResult.setBottom(fYPos);
                selPt = ESelectionPoint::BottomRight;
            }
            // Moved right of right border ..
            else if (fXPos > rctResult.right()) {
                rctResult.setLeft(rctResult.right());
                rctResult.setRight(fXPos);
                rctResult.setTop(fYPos);
                selPt = ESelectionPoint::TopRight;
            }
            // Moved below bottom border ..
            else if (fYPos > rctResult.bottom()) {
                rctResult.setLeft(fXPos);
                rctResult.setTop(rctResult.bottom());
                rctResult.setBottom(fYPos);
                selPt = ESelectionPoint::BottomLeft;
            }
            // If the bounding rectangle don't need to be twisted ...
            else {
                rctResult.setLeft(fXPos);
                rctResult.setTop(fYPos);
            }
            break;
        }
        case ESelectionPoint::BottomLeft: {
            // Moved right and above right top corner ..
            if (fXPos > rctResult.right() && fYPos < rctResult.top()) {
                rctResult.setLeft(rctResult.right());
                rctResult.setRight(fXPos);
                rctResult.setBottom(rctResult.top());
                rctResult.setTop(fYPos);
                selPt = ESelectionPoint::TopRight;
            }
            // Moved right of right border ..
            else if (fXPos > rctResult.right()) {
                rctResult.setLeft(rctResult.right());
                rctResult.setRight(fXPos);
                rctResult.setBottom(fYPos);
                selPt = ESelectionPoint::BottomRight;
            }
            // Moved above top border ..
            else if (fYPos < rctResult.top()) {
                rctResult.setLeft(fXPos);
                rctResult.setBottom(rctResult.top());
                rctResult.setTop(fYPos);
                selPt = ESelectionPoint::TopLeft;
            }
            // If the bounding rectangle don't need to be twisted ...
            else {
                rctResult.setLeft(fXPos);
                rctResult.setBottom(fYPos);
            }
            break;
        }
        case ESelectionPoint::TopRight: {
            // Moved left and below left bottom corner ..
            if (fXPos < rctResult.left() && fYPos > rctResult.bottom()) {
                rctResult.setRight(rctResult.left());
                rctResult.setLeft(fXPos);
                rctResult.setTop(rctResult.bottom());
                rctResult.setBottom(fYPos);
                selPt = ESelectionPoint::BottomLeft;
            }
            // Moved left of left border ..
            else if (fXPos < rctResult.left()) {
                rctResult.setRight(rctResult.left());
                rctResult.setLeft(fXPos);
                rctResult.setTop(fYPos);
                selPt = ESelectionPoint::TopLeft;
            }
            // Moved below bottom border ..
            else if (fYPos > rctResult.bottom()) {
                rctResult.setRight(fXPos);
                rctResult.setTop(rctResult.bottom());
                rctResult.setBottom(fYPos);
                selPt = ESelectionPoint::BottomRight;
            }
            // If the bounding rectangle don't need to be twisted ...
            else {
                rctResult.setRight(fXPos);
                rctResult.setTop(fYPos);
            }
            break;
        }
        case ESelectionPoint::BottomRight: {
            // Moved left and above left top corner ..
            if (fXPos < rctResult.left() && fYPos < rctResult.top()) {
                rctResult.setRight(rctResult.left());
                rctResult.setLeft(fXPos);
                rctResult.setBottom(rctResult.top());
                rctResult.setTop(fYPos);
                selPt = ESelectionPoint::TopLeft;
            }
            // Moved left of left border ..
            else if (fXPos < rctResult.left()) {
                rctResult.setRight(rctResult.left());
                rctResult.setLeft(fXPos);
                rctResult.setBottom(fYPos);
                selPt = ESelectionPoint::BottomLeft;
            }
            // Moved above top border ..
            else if (fYPos < rctResult.top()) {
                rctResult.setRight(fXPos);
                rctResult.setBottom(rctResult.top());
                rctResult.setTop(fYPos);
                selPt = ESelectionPoint::TopRight;
            }
            // If the bounding rectangle don't need to be twisted ...
            else {
                rctResult.setRight(fXPos);
                rctResult.setBottom(fYPos);
            }
            break;
        }
        case ESelectionPoint::LeftCenter: {
            // Moved right of right border ..
            if (fXPos > rctResult.right()) {
                rctResult.setLeft(rctResult.right());
                rctResult.setRight(fXPos);
                selPt = ESelectionPoint::RightCenter;
            }
            // If the bounding rectangle don't need to be twisted ...
            else {
                rctResult.setLeft(fXPos);
            }
            break;
        }
        case ESelectionPoint::RightCenter: {
            // Moved left of left border ..
            if (fXPos < rctResult.left()) {
                rctResult.setRight(rctResult.left());
                rctResult.setLeft(fXPos);
                selPt = ESelectionPoint::LeftCenter;
            }
            // If the bounding rectangle don't need to be twisted ...
            else {
                rctResult.setRight(fXPos);
            }
            break;
        }
        case ESelectionPoint::TopCenter:
        {
            // Moved below bottom border ..
            if (fYPos > rctResult.bottom()) {
                rctResult.setTop(rctResult.bottom());
                rctResult.setBottom(fYPos);
                selPt = ESelectionPoint::BottomCenter;
            }
            // If the bounding rectangle don't need to be twisted ...
            else {
                rctResult.setTop(fYPos);
            }
            break;
        }
        case ESelectionPoint::BottomCenter: {
            // Moved above top border ..
            if (fYPos < rctResult.top()) {
                rctResult.setBottom(rctResult.top());
                rctResult.setTop(fYPos);
                selPt = ESelectionPoint::TopCenter;
            }
            // If the bounding rectangle don't need to be twisted ...
            else {
                rctResult.setBottom(fYPos);
            }
            break;
        }
        case ESelectionPoint::Center:
        case ESelectionPoint::RotateTop:
        case ESelectionPoint::RotateBottom:
        default:
        {
            break;
        }
    }
    if (o_pSelPt != nullptr) {
        *o_pSelPt = selPt;
    }
    return rctResult;
}

//------------------------------------------------------------------------------
QPolygonF ZS::Draw::resizePolygon(
    const QPolygonF& i_plg,
    const QRectF& i_rctBoundingNew,
    const QRectF& i_rctBoundingOld )
//------------------------------------------------------------------------------
{
    QPolygonF plgResult = i_plg;

    QRectF rctBoundingOld = i_rctBoundingOld;

    if (!rctBoundingOld.isValid()) {
        rctBoundingOld = plgResult.boundingRect();
    }

    double fx0Old = rctBoundingOld.left();
    double fy0Old = rctBoundingOld.top();
    double fWOld  = fabs(rctBoundingOld.width());
    double fHOld  = fabs(rctBoundingOld.height());

    double fx0New = i_rctBoundingNew.left();
    double fy0New = i_rctBoundingNew.top();
    double fWNew  = fabs(i_rctBoundingNew.width());
    double fHNew  = fabs(i_rctBoundingNew.height());

    double fScaleX = 1.0;
    double fScaleY = 1.0;

    if (fWOld > 0.0) {
        fScaleX = fWNew / fWOld;
    }
    if (fHOld > 0.0) {
        fScaleY = fHNew / fHOld;
    }

    for (QPointF& pt : plgResult) {
        double fxdOld = pt.x() - fx0Old;
        double fydOld = pt.y() - fy0Old;
        double fxdNew = fScaleX * fxdOld;
        double fydNew = fScaleY * fydOld;
        pt.setX(fx0New + fxdNew);
        pt.setY(fy0New + fydNew);
        //plgResult[idxPt] = pt;
    }
    return plgResult;
}

//------------------------------------------------------------------------------
/*! @brief Returns the bounding rectangle of the circle defined by its center
           point and radius.
*/
QRectF ZS::Draw::boundingRect(const QPointF& i_pt, double i_fRadius)
//------------------------------------------------------------------------------
{
    return QRectF(i_pt.x()-i_fRadius,  i_pt.y()-i_fRadius, 2.0*i_fRadius, 2.0*i_fRadius);
}

//------------------------------------------------------------------------------
/*! @brief Returns the bounding rectangle of the line.

    The position of P1 and P2 is not relevant. If P1 would be right or up
    P2 the left top corner of the bounding rectangle is either at P1 or P2.
*/
QRectF ZS::Draw::boundingRect(const QLineF& i_line)
//------------------------------------------------------------------------------
{
    QPolygonF plg = line2Polygon(i_line);
    QRectF rectBounding = plg.boundingRect();
    // Width and height of the bounding rectangle must be corrected by one pixel.
    //rectBounding.setWidth(rectBounding.width() + 1.0);
    //rectBounding.setHeight(rectBounding.height() + 1.0);
    return rectBounding;
}

//------------------------------------------------------------------------------
/*! @brief Returns the bounding rectangle of the given graphics items in coordinates
           relative to the given parent group.

    If no parent group is passed (nullptr) the bounding rectangle is returned in
    scene coordinates.
*/
QRectF ZS::Draw::boundingRect(
    const QList<QGraphicsItem*>& i_arpGraphicsItems, QGraphicsItem* i_pGrapicsItemParentGroup)
//------------------------------------------------------------------------------
{
    QVector<CGraphObj*> arpGraphObjChilds;
    for (QGraphicsItem* pGraphicsItemChild : i_arpGraphicsItems) {
        CGraphObj* pGraphObjChild = dynamic_cast<CGraphObj*>(pGraphicsItemChild);
        if (pGraphObjChild != nullptr) {
            arpGraphObjChilds.append(pGraphObjChild);
        }
    }
    CGraphObjGroup* pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(i_pGrapicsItemParentGroup);
    return boundingRect(arpGraphObjChilds, pGraphObjGroup);
}

//------------------------------------------------------------------------------
/*! @brief Returns the bounding rectangle of the given graphics items in coordinates
           relative to the given parent group.

    If no parent group is passed (nullptr) the bounding rectangle is returned in
    scene coordinates.
*/
QRectF ZS::Draw::boundingRect(
    const QVector<CGraphObj*>& i_arpGraphObjs, CGraphObjGroup* i_pGraphObjGroup)
//------------------------------------------------------------------------------
{
    QRectF rectBounding;
    double fXLeftMin = INT_MAX;
    double fYTopMin = INT_MAX;
    double fXRightMax = INT_MIN;
    double fYBottomMax = INT_MIN;
    QGraphicsItem* pGraphicsItemParentGroup = dynamic_cast<QGraphicsItem*>(i_pGraphObjGroup);
    for (CGraphObj* pGraphObj : i_arpGraphObjs) {
        QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(pGraphObj);
        if (pGraphicsItem != nullptr) {
            if (!pGraphObj->isConnectionLine() && !pGraphObj->isSelectionPoint() && !pGraphObj->isLabel()) {
                QRectF rectItem = pGraphObj->getBoundingRect();
                if (i_pGraphObjGroup != nullptr) {
                    rectItem = pGraphicsItem->mapToItem(pGraphicsItemParentGroup, rectItem).boundingRect();
                }
                else {
                    rectItem = pGraphicsItem->mapToScene(rectItem).boundingRect();
                }
                if (rectItem.width() >= 0.0) {
                    if (rectItem.left() < fXLeftMin) {
                        fXLeftMin = rectItem.left();
                    }
                    if (rectItem.right() > fXRightMax) {
                        fXRightMax = rectItem.right();
                    }
                }
                else {
                    if (rectItem.right() < fXLeftMin) {
                        fXLeftMin = rectItem.right();
                    }
                    if (rectItem.left() > fXRightMax) {
                        fXRightMax = rectItem.left();
                    }
                }
                if (rectItem.height() >= 0.0) {
                    if (rectItem.top() < fYTopMin) {
                        fYTopMin = rectItem.top();
                    }
                    if (rectItem.bottom() > fYBottomMax) {
                        fYBottomMax = rectItem.bottom();
                    }
                }
                else {
                    if (rectItem.bottom() < fYTopMin) {
                        fYTopMin = rectItem.bottom();
                    }
                    if (rectItem.top() > fYBottomMax) {
                        fYBottomMax = rectItem.top();
                    }
                }
            }
        }
    }
    if (fXLeftMin == INT_MAX || fYTopMin == INT_MAX || fXRightMax == INT_MIN || fYBottomMax == INT_MIN) {
        fXLeftMin = 0.0;
        fYTopMin = 0.0;
        fXRightMax = 0.0;
        fYBottomMax = 0.0;
    }
    rectBounding.setLeft(fXLeftMin);
    rectBounding.setTop(fYTopMin);
    rectBounding.setRight(fXRightMax);
    rectBounding.setBottom(fYBottomMax);
    return rectBounding;
}

//------------------------------------------------------------------------------
QPolygonF ZS::Draw::line2Polygon(const QLineF& i_line)
//------------------------------------------------------------------------------
{
    QPolygonF plg;
    plg << i_line.p1() << i_line.p2();
    return plg;
}

//------------------------------------------------------------------------------
QPolygonF ZS::Draw::rect2Polygon(const QRectF& i_rct, int i_iSelPtsCount, const ESelectionPoint* i_pSelPts)
//------------------------------------------------------------------------------
{
    static const ESelectionPoint s_arSelPts[] = {
        ESelectionPoint::TopLeft,
        ESelectionPoint::TopRight,
        ESelectionPoint::BottomRight,
        ESelectionPoint::BottomLeft
    };

    const ESelectionPoint* pSelPts = i_pSelPts;
    int iSelPtsCount = i_iSelPtsCount;

    if (pSelPts == nullptr) {
        iSelPtsCount = _ZSArrLen(s_arSelPts);
        pSelPts = s_arSelPts;
    }

    QPolygonF plg(iSelPtsCount);

    for (int idxSelPt = 0; idxSelPt < 4 && idxSelPt < plg.size(); idxSelPt++) {
        ESelectionPoint selPt = pSelPts[idxSelPt];
        switch (selPt) {
            case ESelectionPoint::TopLeft: {
                plg[idxSelPt] = i_rct.topLeft();
                break;
            }
            case ESelectionPoint::TopRight: {
                plg[idxSelPt] = i_rct.topRight();
                break;
            }
            case ESelectionPoint::BottomRight: {
                plg[idxSelPt] = i_rct.bottomRight();
                break;
            }
            case ESelectionPoint::BottomLeft: {
                plg[idxSelPt] = i_rct.bottomLeft();
                break;
            }
            case ESelectionPoint::BottomCenter: {
                plg[idxSelPt] = QLineF(i_rct.bottomLeft(), i_rct.bottomRight()).center();
                break;
            }
            case ESelectionPoint::TopCenter: {
                plg[idxSelPt] = QLineF(i_rct.topLeft(), i_rct.topRight()).center();
                break;
            }
            case ESelectionPoint::RightCenter: {
                plg[idxSelPt] = QLineF(i_rct.topRight(), i_rct.bottomRight()).center();
                break;
            }
            case ESelectionPoint::LeftCenter: {
                plg[idxSelPt] = QLineF(i_rct.topLeft(), i_rct.bottomLeft()).center();
                break;
            }
            case ESelectionPoint::Center: {
                plg[idxSelPt] = i_rct.center();
                break;
            }
            case ESelectionPoint::RotateTop:
            case ESelectionPoint::RotateBottom:
            default: {
                break;
            }
        }
    }
    return plg;

} // rect2Polygon

//------------------------------------------------------------------------------
/*! @brief Checks whether the given line is hit by the given point taking the
           given tolerance into account.

          + P1
           \
          +-\-+
         Pt x\| PtToleranceRectangle
          +---+
               \
                 + P2

    The method creates a rectangle whose width and height is twice the tolerance
    and whose center point is the given point.

    The method checks whether P1 and P2 are within this rectangle.
    If neither P1 nor P1 are hit the method checks whether the outer border lines
    of the rectangle (top, bottom, left, right) intersects with the line.
    If none of those lines hit the two diagonales of the tolerance rectangle are
    also checked.
*/
bool ZS::Draw::isLineHit(
    const QLineF& i_line,
    const QPointF& i_pt,
    double i_fTolerance,
    SGraphObjHitInfo* o_pHitInfo)
//------------------------------------------------------------------------------
{
    // We create a square with the SideLength = 2 * fTolerance.
    // For each side we are going to check whether the border line inteferes
    // with the given input line.

    bool bIsHit = false;

    double fTolerance = i_fTolerance;
    if (fTolerance <= 0.0) {
        fTolerance = 2.0;
    }
    if (o_pHitInfo != nullptr) {
        //o_pHitInfo->m_editMode = EEditMode::None;
        //o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
        o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
        o_pHitInfo->m_idxPolygonShapePoint = -1;
        o_pHitInfo->m_idxLineSegment = -1;
        o_pHitInfo->m_ptSelected = QPointF();
        o_pHitInfo->m_cursor = Qt::ArrowCursor;
    }

    double fXLeft   = i_pt.x() - fTolerance;
    double fYTop    = i_pt.y() - fTolerance;
    double fXRight  = i_pt.x() + fTolerance;
    double fYBottom = i_pt.y() + fTolerance;
    QRectF rctPt(QPointF(fXLeft, fYTop), QPointF(fXRight, fYBottom));

    if (rctPt.contains(i_line.p1())) {
        bIsHit = true;
        o_pHitInfo->m_idxPolygonShapePoint = 0;
        o_pHitInfo->m_ptSelected = i_pt;
        o_pHitInfo->m_cursor = Qt::CrossCursor;
    }
    else if (rctPt.contains(i_line.p2())) {
        bIsHit = true;
        o_pHitInfo->m_idxPolygonShapePoint = 1;
        o_pHitInfo->m_ptSelected = i_pt;
        o_pHitInfo->m_cursor = Qt::CrossCursor;
    }
    else {
        enum ERectangleLinesToCheck {
            ELineTop,
            ELineBottom,
            ELineLeft,
            ELineRight,
            ELineDiagTLBR, // TopLeft to BottomRight
            ELineDiagBLTR, // BottomLeft to TopRight
            ELineCOUNT
        };
        QLineF lineRctPt;
        QPointF ptIntersection;
        for (int idxLine = 0; idxLine < ELineCOUNT; ++idxLine) {
            if (idxLine == ELineTop) {
                lineRctPt = QLineF(rctPt.topLeft(), rctPt.topRight());
            }
            else if (idxLine == ELineBottom) {
                lineRctPt = QLineF(rctPt.bottomLeft(), rctPt.bottomRight());
            }
            else if (idxLine == ELineLeft) {
                lineRctPt = QLineF(rctPt.topLeft(), rctPt.bottomLeft());
            }
            else if (idxLine == ELineRight) {
                lineRctPt = QLineF(rctPt.topRight(), rctPt.bottomRight());
            }
            else if (idxLine == ELineDiagTLBR) {
                lineRctPt = QLineF(rctPt.topLeft(), rctPt.bottomRight());
            }
            else if (idxLine == ELineDiagBLTR) {
                lineRctPt = QLineF(rctPt.bottomLeft(), rctPt.topRight());
            }
            if (lineRctPt.intersects(i_line, &ptIntersection) == QLineF::BoundedIntersection) {
                bIsHit = true;
                o_pHitInfo->m_ptSelected = ptIntersection;
                o_pHitInfo->m_cursor = Qt::SizeAllCursor;
                break;
            }
        }
    }
    return bIsHit;

} // isLineHit

//------------------------------------------------------------------------------
bool ZS::Draw::isRectHit(
    const QRectF& i_rct,
    const CEnumFillStyle& i_fillStyle,
    const QPointF& i_pt,
    double i_fTolerance,
    SGraphObjHitInfo* o_pHitInfo )
//------------------------------------------------------------------------------
{
    static const ESelectionPoint s_arSelPtsCorners[] = {
        ESelectionPoint::BottomRight,
        ESelectionPoint::TopRight,
        ESelectionPoint::BottomLeft,
        ESelectionPoint::TopLeft
    };

    static const ESelectionPoint s_arSelPtsLineCenters[] = {
        ESelectionPoint::BottomCenter,
        ESelectionPoint::RightCenter,
        ESelectionPoint::LeftCenter,
        ESelectionPoint::TopCenter
    };

    bool bIsHit = false;

    double fTolerance = i_fTolerance;
    if (fTolerance <= 0.0) {
        fTolerance = 2.0;
    }
    if (o_pHitInfo != nullptr) {
        //o_pHitInfo->m_editMode = EEditMode::None;
        //o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
        o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
        o_pHitInfo->m_idxPolygonShapePoint = -1;
        o_pHitInfo->m_idxLineSegment = -1;
        o_pHitInfo->m_ptSelected = QPointF();
        o_pHitInfo->m_cursor = Qt::ArrowCursor;
    }

    QRectF rctBndTmp = i_rct;
    rctBndTmp.setLeft( i_rct.left() - fTolerance );
    rctBndTmp.setTop( i_rct.top() - fTolerance );
    rctBndTmp.setRight( i_rct.right() + fTolerance );
    rctBndTmp.setBottom( i_rct.bottom() + fTolerance );

    if (rctBndTmp.contains(i_pt)) {
        QPolygonF plg = rect2Polygon(i_rct, _ZSArrLen(s_arSelPtsCorners), s_arSelPtsCorners);
        for (int idxPt = 0; idxPt < plg.size(); ++idxPt) {
            const QPointF& pt = plg[idxPt];
            ESelectionPoint selPt = s_arSelPtsCorners[idxPt];
            QRectF rct = boundingRect(pt, fTolerance);
            if (rct.contains(i_pt)) {
                bIsHit = true;
                if (o_pHitInfo != nullptr) {
                    //o_pHitInfo->m_editMode = selectionPoint2EditMode(selPt);
                    //o_pHitInfo->m_editResizeMode = selectionPoint2EditResizeMode(selPt);
                    o_pHitInfo->m_selPtBoundingRect = selPt;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = pt;
                    o_pHitInfo->m_cursor = selectionPoint2Cursor(selPt);
                }
                break;
            }
        }
        if (!bIsHit) {
            plg = rect2Polygon(i_rct, _ZSArrLen(s_arSelPtsLineCenters), s_arSelPtsLineCenters);
            for (int idxPt = 0; idxPt < plg.size(); idxPt++) {
                const QPointF& pt = plg[idxPt];
                ESelectionPoint selPt = s_arSelPtsLineCenters[idxPt];
                QRectF rct = boundingRect(pt, fTolerance);
                if (rct.contains(i_pt)) {
                    bIsHit = true;
                    if (o_pHitInfo != nullptr) {
                        //o_pHitInfo->m_editMode = selectionPoint2EditMode(selPt);
                        //o_pHitInfo->m_editResizeMode = selectionPoint2EditResizeMode(selPt);
                        o_pHitInfo->m_selPtBoundingRect = selPt;
                        o_pHitInfo->m_idxPolygonShapePoint = -1;
                        o_pHitInfo->m_idxLineSegment = -1;
                        o_pHitInfo->m_ptSelected = pt;
                        o_pHitInfo->m_cursor = selectionPoint2Cursor(selPt);
                    }
                    break;
                }
            }
        }
        if (!bIsHit) {
            plg = i_rct; // this cast return 5 points (left top corner twice)
            for (int idxPt = 0; idxPt < plg.size()-1; idxPt++) {
                QLineF lin(plg[idxPt], plg[idxPt+1]);
                if (isLineHit(lin, i_pt, fTolerance, o_pHitInfo)) {
                    bIsHit = true;
                    if (o_pHitInfo != nullptr) {
                        //o_pHitInfo->m_editMode = EEditMode::Move;
                        //o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
                        o_pHitInfo->m_idxLineSegment = idxPt;
                        o_pHitInfo->m_ptSelected = i_pt;
                    }
                    break;
                }
            }
        }
        if (!bIsHit) {
            if (i_fillStyle == EFillStyle::SolidPattern) {
                bIsHit = true;
                if (o_pHitInfo != nullptr) {
                    //o_pHitInfo->m_editMode = EEditMode::Move;
                    //o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
                    o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = i_pt;
                    o_pHitInfo->m_cursor = Qt::SizeAllCursor;
                }
            }
        }
    }
    return bIsHit;

} // isRectHit

//------------------------------------------------------------------------------
bool ZS::Draw::isEllipseHit(
    const QRectF& i_rct,
    const CEnumFillStyle& i_fillStyle,
    const QPointF& i_pt,
    double i_fTolerance,
    SGraphObjHitInfo* o_pHitInfo )
//------------------------------------------------------------------------------
{
    static const ESelectionPoint s_arSelPtsLineCenters[] = {
        ESelectionPoint::BottomCenter,
        ESelectionPoint::RightCenter,
        ESelectionPoint::LeftCenter,
        ESelectionPoint::TopCenter
    };

    bool bIsHit = false;

    double fTolerance = i_fTolerance;
    if (fTolerance <= 0.0) {
        fTolerance = 2.0;
    }
    if( o_pHitInfo != nullptr )
    {
        //o_pHitInfo->m_editMode = EEditMode::None;
        //o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
        o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
        o_pHitInfo->m_idxPolygonShapePoint = -1;
        o_pHitInfo->m_idxLineSegment = -1;
        o_pHitInfo->m_ptSelected = QPointF();
        o_pHitInfo->m_cursor = Qt::ArrowCursor;
    }

    QRectF rctBndTmp = i_rct;
    rctBndTmp.setLeft( i_rct.left() - fTolerance );
    rctBndTmp.setTop( i_rct.top() - fTolerance );
    rctBndTmp.setRight( i_rct.right() + fTolerance );
    rctBndTmp.setBottom( i_rct.bottom() + fTolerance );

    if (rctBndTmp.contains(i_pt)) {
        QPointF ptC = i_rct.center();

        //QPolygonF       plg;
        //QRectF          rct;
        //QPointF         pt;
        //ESelectionPoint selPt;
        //int             idxPt;

        QPolygonF plg = rect2Polygon(i_rct, _ZSArrLen(s_arSelPtsLineCenters), s_arSelPtsLineCenters);
        for (int idxPt = 0; idxPt < plg.size(); idxPt++) {
            const QPointF& pt = plg[idxPt];
            ESelectionPoint selPt = s_arSelPtsLineCenters[idxPt];
            QRectF rct = boundingRect(pt, fTolerance);
            if (rct.contains(i_pt)) {
                bIsHit = true;
                if (o_pHitInfo != nullptr) {
                    //o_pHitInfo->m_editMode = selectionPoint2EditMode(selPt);
                    //o_pHitInfo->m_editResizeMode = selectionPoint2EditResizeMode(selPt);
                    o_pHitInfo->m_selPtBoundingRect = selPt;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = pt;
                    o_pHitInfo->m_cursor = selectionPoint2Cursor(selPt);
                }
                break;
            }
        }
        if (!bIsHit) {
            double fa = i_rct.width() / 2.0;
            double fb = i_rct.height() / 2.0;
            double fx = i_pt.x() - ptC.x();
            double fy = i_pt.y() - ptC.y();

            // Ellipse as vertical line ..
            if (fa == 0.0) {
                if (fabs(fx) <= fTolerance) {
                    bIsHit = true;
                    if (o_pHitInfo != nullptr) {
                        //o_pHitInfo->m_editMode = EEditMode::Resize;
                        //o_pHitInfo->m_editResizeMode = EEditResizeMode::ResizeHor;
                        o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::RightCenter;
                        o_pHitInfo->m_idxPolygonShapePoint = -1;
                        o_pHitInfo->m_idxLineSegment = -1;
                        o_pHitInfo->m_ptSelected = i_pt;
                        o_pHitInfo->m_cursor = selectionPoint2Cursor(ESelectionPoint::RightCenter);
                    }
                }
            }
            // Ellipse as horizontal line ..
            else if (fb == 0.0) {
                if (fabs(fy) <= fTolerance) {
                    bIsHit = true;
                    if (o_pHitInfo != nullptr) {
                        //o_pHitInfo->m_editMode = EEditMode::Resize;
                        //o_pHitInfo->m_editResizeMode = EEditResizeMode::ResizeVer;
                        o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::BottomCenter;
                        o_pHitInfo->m_idxPolygonShapePoint = -1;
                        o_pHitInfo->m_idxLineSegment = -1;
                        o_pHitInfo->m_ptSelected = i_pt;
                        o_pHitInfo->m_cursor = selectionPoint2Cursor(ESelectionPoint::BottomCenter);
                    }
                }
            }
            // Ellipse is a circle ..
            else if (fa == fb) {
                // Circle equation:
                //   x² + y² = r²
                //   y = sqrt(r² - x²)
                double fyTmp = sqrt(Math::sqr(fa) - Math::sqr(fx));
                if ((fy + fTolerance >= fyTmp) && (fy - fTolerance <= fyTmp)) {
                    bIsHit = true;
                }
                else if ((fy + fTolerance >= -fyTmp) && (fy - fTolerance <= -fyTmp)) {
                    bIsHit = true;
                }
                if (bIsHit && o_pHitInfo != nullptr) {
                    //o_pHitInfo->m_editMode = EEditMode::Move;
                    //o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
                    o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = i_pt;
                    o_pHitInfo->m_cursor = Qt::SizeAllCursor;
                }
            }
            else  {
                // Ellipse equation:
                //   x²/a² + y²/b² = 1
                //   y²/b² = 1 - x²/a²
                //   y² = b² * (1 - x²/a²)
                //   y = b * sqrt(1 - x²/a²)
                if (fx == fa) {
                    if (fabs(fy) <= fTolerance) {
                        bIsHit = true;
                    }
                }
                else if (fy == fb) {
                    if (fabs(fx) <= fTolerance) {
                        bIsHit = true;
                    }
                }
                else {
                    double fyTmp = fb * sqrt(1.0 - Math::sqr(fx/fa));
                    if ((fy + fTolerance >= fyTmp) && (fy - fTolerance <= fyTmp)) {
                        bIsHit = true;
                    }
                    else if ((fy + fTolerance >= -fyTmp) && (fy - fTolerance <= -fyTmp)) {
                        bIsHit = true;
                    }
                }
                if (bIsHit && o_pHitInfo != nullptr) {
                    //o_pHitInfo->m_editMode = EEditMode::Move;
                    //o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
                    o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = i_pt;
                    o_pHitInfo->m_cursor = Qt::SizeAllCursor;
                }
            }
        }
        if (!bIsHit) {
            if (i_fillStyle == EFillStyle::SolidPattern) {
                bIsHit = true;
                if (o_pHitInfo != nullptr) {
                    //o_pHitInfo->m_editMode = EEditMode::Move;
                    //o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
                    o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = i_pt;
                    o_pHitInfo->m_cursor = Qt::SizeAllCursor;
                }
            }
        }
    }
    return bIsHit;

} // isEllipseHit

//------------------------------------------------------------------------------
bool ZS::Draw::isPolygonHit(
    const QPolygonF& i_plg,
    const CEnumFillStyle& i_fillStyle,
    const QPointF& i_pt,
    double i_fTolerance,
    SGraphObjHitInfo* o_pHitInfo )
//------------------------------------------------------------------------------
{
    bool   bIsHit = false;
    double fTolerance = i_fTolerance;

    if (fTolerance <= 0.0) {
        fTolerance = 2.0;
    }
    if (o_pHitInfo != nullptr) {
        //o_pHitInfo->m_editMode = EEditMode::None;
        //o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
        o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
        o_pHitInfo->m_idxPolygonShapePoint = -1;
        o_pHitInfo->m_idxLineSegment = -1;
        o_pHitInfo->m_ptSelected = QPointF();
        o_pHitInfo->m_cursor = Qt::ArrowCursor;
    }

    QRectF rctBndTmp = i_plg.boundingRect();
    rctBndTmp.setLeft(rctBndTmp.left() - fTolerance);
    rctBndTmp.setTop(rctBndTmp.top() - fTolerance);
    rctBndTmp.setRight(rctBndTmp.right() + fTolerance);
    rctBndTmp.setBottom(rctBndTmp.bottom() + fTolerance);

    if (rctBndTmp.contains(i_pt)) {
        for (int idxPt = 0; idxPt < i_plg.size(); idxPt++) {
            const QPointF& pt = i_plg[idxPt];
            QRectF rct = boundingRect(pt, fTolerance);
            if (rct.contains(i_pt)) {
                bIsHit = true;
                if (o_pHitInfo != nullptr) {
                    //o_pHitInfo->m_editMode = EEditMode::MoveShapePoint;
                    o_pHitInfo->m_idxPolygonShapePoint = idxPt;
                    o_pHitInfo->m_ptSelected = pt;
                    o_pHitInfo->m_cursor = Qt::CrossCursor;
                }
                break;
            }
        }
        if (!bIsHit) {
            if (i_plg.size() > 1) {
                for (int idxPt = 0; idxPt < i_plg.size()-1; idxPt++) {
                    QLineF lin(i_plg[idxPt], i_plg[idxPt+1]);
                    if (isLineHit(lin, i_pt, fTolerance, o_pHitInfo)) {
                        bIsHit = true;
                        if (o_pHitInfo != nullptr) {
                            //o_pHitInfo->m_editMode = EEditMode::Move;
                            //o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
                            o_pHitInfo->m_idxLineSegment = idxPt;
                            o_pHitInfo->m_ptSelected = i_pt;
                        }
                        break;
                    }
                }
            }
        }
        if (!bIsHit) {
            if (i_fillStyle == EFillStyle::SolidPattern) {
                bIsHit = true;
                if (o_pHitInfo != nullptr) {
                    //o_pHitInfo->m_editMode = EEditMode::Move;
                    //o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
                    o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = i_pt;
                    o_pHitInfo->m_cursor = Qt::SizeAllCursor;
                }
            }
        }
    }
    return bIsHit;

} // isPolygonHit

//------------------------------------------------------------------------------
/*! @brief Returns the shortest distance from the given point to the given line.
*/
//------------------------------------------------------------------------------
double ZS::Draw::getDistance( const QPointF& i_pt, const QLineF& i_line )
{
    // Get a parallel line to the line to which the distance has to be
    // calculated with the given point as the start point.
    QPointF ptOffset = i_pt - i_line.p1();
    QLineF parallelLine = i_line.translated(ptOffset);
    // Get perpendicular line of the parallel line and determine
    // the intersection point with the linked line.
    QLineF perpendicularLine = parallelLine.normalVector();
    QPointF ptIntersection;
    perpendicularLine.intersects(i_line, &ptIntersection);
    return QLineF(i_pt, ptIntersection).length();
}

//------------------------------------------------------------------------------
/*! @brief Calculates the angle between point 2 and point 1.

    Point 1 is considered to be the origin of the coordinate system.

            Q2        |      Q1        
      (PI/2 .. PI)    | (0.0 .. PI/2)  
      (90° .. 180°)   | (0 .. 90°)     
                      |                
    -----------------Pt1---------------
                      |                
      (-180° .. -90°) | (-90 .. 0°)    
      (-PI .. -PI/2)  | (-PI/2 .. 0.0) 
            Q3        |      Q4        
*/
double ZS::Draw::getAngleRad( const QPointF& i_pt1, const QPointF& i_pt2 )
//------------------------------------------------------------------------------
{
    double fAngle_rad = 0.0;

    // If both points are at the same y position (horizontal line) ...
    if (i_pt1.y() == i_pt2.y()) {
        // If its a line from right to left ..
        if (i_pt2.x() < i_pt1.x()) {
            fAngle_rad = ZS::System::Math::c_fPI;
        }
    }
    else {
        double fWidth_px  = i_pt2.x() - i_pt1.x();
        double fRadius_px = QLineF(i_pt1, i_pt2).length();
        fAngle_rad = acos(fWidth_px/fRadius_px);
        if (i_pt2.y() > i_pt1.y()) {
            fAngle_rad *= -1.0;
        }
    }
    return fAngle_rad;
}

//------------------------------------------------------------------------------
QPointF ZS::Draw::rotatePoint( const QPointF& i_ptCenter, const QPointF& i_pt, double i_fAngle_rad )
//------------------------------------------------------------------------------
{
    QPointF ptRes = i_pt;
    if (i_fAngle_rad != 0.0) {
        double fRadius_px = QLineF(i_ptCenter, i_pt).length();
        double fAnglePt_rad = getAngleRad(i_ptCenter,i_pt);
        double fAngleRes_rad = fAnglePt_rad + i_fAngle_rad;
        ptRes.setX( i_ptCenter.x() + fRadius_px * cos(fAngleRes_rad) );
        ptRes.setY( i_ptCenter.y() - fRadius_px * sin(fAngleRes_rad) );
    }
    return ptRes;
}

//------------------------------------------------------------------------------
QPolygonF ZS::Draw::rotateRect( const QPointF& i_ptCenter, const QRectF& i_rct, double i_fAngle_rad )
//------------------------------------------------------------------------------
{
    QPolygonF polygonRect;
    polygonRect.resize(4);
    polygonRect[0] = rotatePoint(i_ptCenter, i_rct.topLeft(), i_fAngle_rad);
    polygonRect[1] = rotatePoint(i_ptCenter, i_rct.topRight(), i_fAngle_rad);
    polygonRect[2] = rotatePoint(i_ptCenter, i_rct.bottomRight(), i_fAngle_rad);
    polygonRect[3] = rotatePoint(i_ptCenter, i_rct.bottomLeft(), i_fAngle_rad);
    return polygonRect;
}

//------------------------------------------------------------------------------
QPolygonF ZS::Draw::rotatePolygon( const QPointF& i_ptCenter, const QPolygonF& i_polygon, double i_fAngle_rad )
//------------------------------------------------------------------------------
{
    QPolygonF polygon;
    if (i_polygon.size() > 0) {
        polygon.resize(i_polygon.size());
        for (int idxPt = 0; idxPt < i_polygon.size(); idxPt++) {
            if (i_fAngle_rad != 0.0) {
                polygon[idxPt] = rotatePoint(i_ptCenter, i_polygon[idxPt], i_fAngle_rad);
            }
            else {
                polygon[idxPt] = i_polygon[idxPt];
            }
        }
    }
    return polygon;
}

//------------------------------------------------------------------------------
QPolygonF ZS::Draw::normalizePolygon( const QPolygonF& i_plg, int i_iPrecision )
//------------------------------------------------------------------------------
{
    QPolygonF plg = i_plg;

    // Remove "identical" points:
    if (plg.size() > 2) {
        QPointF pt1;
        QPointF pt2;
        QString strPt1;
        QString strPt2;

        for (int idxPt = plg.size()-1; idxPt >= 2; idxPt--) {
            QPointF pt1 = plg[idxPt];
            QPointF pt2 = plg[idxPt-1];
            QString strPt1 = point2Str(pt1, 'f', 0);
            QString strPt2 = point2Str(pt2, 'f', 0);
            if (strPt1 == strPt2) {
                plg.remove(idxPt);
            }
        }
    }

    // Remove points on direct lines between two neighbor points:
    if (plg.size() > 3) {
        bool bPtRemoved = true;
        while (bPtRemoved) {
            bPtRemoved = false;
            for (int idxPt = plg.size()-1; idxPt >= 2; idxPt--) {
                QLineF ln = QLineF( plg[idxPt], plg[idxPt-2] );
                QPointF ptCheck = plg[idxPt-1];
                if (isLineHit(ln, ptCheck, i_iPrecision, nullptr)) {
                    plg.remove(idxPt-1);
                    bPtRemoved = true;
                    break;
                }
            }
        }
    }
    return plg;

} // normalizePolygon

////------------------------------------------------------------------------------
//QRectF ZS::Draw::rotatePolygonRect2HorRect( const QPointF& i_ptCenter, const QPolygonF& i_polygonRect )
////------------------------------------------------------------------------------
//{
//    QRectF rctHor;
//
//    if( i_polygonRect.size() == 4 )
//    {
//        double fAngle_rad = getAngleRad( i_polygonRect[0], i_polygonRect[1] );
//
//        if( fAngle_rad != 0.0 )
//        {
//            QPointF ptTopLeft     = rotatePoint( i_ptCenter, i_polygonRect[0], -fAngle_rad );
//            QPointF ptBottomRight = rotatePoint( i_ptCenter, i_polygonRect[2], -fAngle_rad );
//
//            rctHor = QRectF( ptTopLeft, ptBottomRight );
//        }
//        else
//        {
//            rctHor = QRectF( i_polygonRect[0], i_polygonRect[2] );
//        }
//    }
//
//    return rctHor;
//
//} // rotatePolygonRect2HorRect

////------------------------------------------------------------------------------
//QPolygonF ZS::Draw::rotatePolygonRectSelectionPoints2HorRectSelectionPoints( const QPointF& i_ptCenter, const QPolygonF& i_polygonRectSelectionPoints )
////------------------------------------------------------------------------------
//{
//    QPolygonF polygonRectSelectionPoints;
//
//    polygonRectSelectionPoints.resize(CEnumSelectionPoint::count());
//
//    if( i_polygonRectSelectionPoints.size() == CEnumSelectionPoint::count() )
//    {
//        double fAngle_rad = getAngleRad( i_polygonRectSelectionPoints[ESelectionPoint::TopLeft], i_polygonRectSelectionPoints[ESelectionPoint::TopRight] );
//
//        double fDistPtRotate2TopCenter = QLineF(i_polygonRectSelectionPoints[ESelectionPointRotate], i_polygonRectSelectionPoints[ESelectionPoint::TopCenter]).length();
//
//        polygonRectSelectionPoints[ESelectionPoint::TopLeft] = rotatePoint( i_ptCenter, i_polygonRectSelectionPoints[ESelectionPoint::TopLeft], -fAngle_rad );
//        polygonRectSelectionPoints[ESelectionPoint::BottomRight] = rotatePoint( i_ptCenter, i_polygonRectSelectionPoints[ESelectionPoint::BottomRight], -fAngle_rad );
//
//        QRectF rctTmp( polygonRectSelectionPoints[ESelectionPoint::TopLeft], polygonRectSelectionPoints[ESelectionPoint::BottomRight] );
//
//        polygonRectSelectionPoints[ESelectionPoint::TopRight] = rctTmp.topRight();
//        polygonRectSelectionPoints[ESelectionPoint::BottomLeft] = rctTmp.bottomLeft();
//
//        QPointF ptCenterTmp = rctTmp.center();
//
//        polygonRectSelectionPoints[ESelectionPoint::LeftCenter].setX( rctTmp.left() );
//        polygonRectSelectionPoints[ESelectionPoint::LeftCenter].setY( ptCenterTmp.y() );
//        polygonRectSelectionPoints[ESelectionPoint::RightCenter].setX( rctTmp.right() );
//        polygonRectSelectionPoints[ESelectionPoint::RightCenter].setY( ptCenterTmp.y() );
//        polygonRectSelectionPoints[ESelectionPoint::TopCenter].setX( ptCenterTmp.x() );
//        polygonRectSelectionPoints[ESelectionPoint::TopCenter].setY( rctTmp.top() );
//        polygonRectSelectionPoints[ESelectionPoint::BottomCenter].setX( ptCenterTmp.x() );
//        polygonRectSelectionPoints[ESelectionPoint::BottomCenter].setY( rctTmp.bottom() );
//
//        polygonRectSelectionPoints[ESelectionPointRotate].setX( ptCenterTmp.x() );
//        polygonRectSelectionPoints[ESelectionPointRotate].setY( rctTmp.top() - fDistPtRotate2TopCenter );
//
//        polygonRectSelectionPoints[ESelectionPoint::Center] = ptCenterTmp;
//        polygonRectSelectionPoints[ESelectionPointRotate] = ;
//    }
//
//    return polygonRectSelectionPoints;
//
//} // rotatePolygonRectSelectionPoints2HorRectSelectionPoints

//------------------------------------------------------------------------------
QPointF ZS::Draw::getCenterPointOfPolygonRect( const QPolygonF& i_polygonRect )
//------------------------------------------------------------------------------
{
    QPointF ptCenter;

    if (i_polygonRect.size() == 4) {
        double fXLeft   = i_polygonRect[0].x();
        double fYTop    = i_polygonRect[0].y();
        double fXRight  = i_polygonRect[0].x();
        double fYBottom = i_polygonRect[0].y();
        for (int idxPt = 1; idxPt < i_polygonRect.size(); idxPt++) {
            if (i_polygonRect[idxPt].x() < fXLeft) {
                fXLeft = i_polygonRect[idxPt].x();
            }
            if (i_polygonRect[idxPt].y() < fYTop) {
                fYTop = i_polygonRect[idxPt].y();
            }
            if (i_polygonRect[idxPt].x() > fXRight) {
                fXRight = i_polygonRect[idxPt].x();
            }
            if (i_polygonRect[idxPt].y() > fYBottom) {
                fYBottom = i_polygonRect[idxPt].y();
            }
        }
        if (fXLeft > fXRight) {
            double fXLeftTmp = fXLeft;
            fXLeft = fXRight;
            fXRight = fXLeftTmp;
        }
        if (fYTop > fYBottom) {
            double fYTopTmp = fYTop;
            fYTop = fYBottom;
            fYBottom = fYTopTmp;
        }
        ptCenter.setX( fXLeft + (fXRight-fXLeft)/2.0 );
        ptCenter.setY( fYTop + (fYBottom-fYTop)/2.0 );
    }
    return ptCenter;

} // getCenterPointOfPolygonRect

//------------------------------------------------------------------------------
QPointF ZS::Draw::getMassCenterPointOfPolygon( const QPolygonF& i_polygon )
//------------------------------------------------------------------------------
{
    QPointF ptCenter;
    if (i_polygon.size() == 1) {
        ptCenter = i_polygon[0];
    }
    else if (i_polygon.size() == 2) {
        ptCenter.setX( (i_polygon[0].x() + i_polygon[1].x()) / 2.0 );
        ptCenter.setY( (i_polygon[0].y() + i_polygon[1].y()) / 2.0 );
    }
    else { // if( i_polygon.size() > 2 )
        int idxPt, idxPt2;
        double ai, atmp = 0, xtmp = 0, ytmp = 0;
        for (idxPt = i_polygon.size()-1, idxPt2 = 0; idxPt2 < i_polygon.size(); idxPt = idxPt2, idxPt2++) {
            ai = i_polygon[idxPt].x() * i_polygon[idxPt2].y() - i_polygon[idxPt2].x() * i_polygon[idxPt].y();
            atmp += ai;
            xtmp += (i_polygon[idxPt2].x() + i_polygon[idxPt].x()) * ai;
            ytmp += (i_polygon[idxPt2].y() + i_polygon[idxPt].y()) * ai;
        }
        if (atmp != 0.0) {
            ptCenter.setX( xtmp / (3.0 * atmp) );
            ptCenter.setY( ytmp / (3.0 * atmp) );
        }
        else {
            ptCenter = i_polygon[0];
        }
    }
    return ptCenter;
}

//------------------------------------------------------------------------------
QPolygonF ZS::Draw::getBoundingRectPolygon( const QPolygonF& i_polygon )
//------------------------------------------------------------------------------
{
    QPolygonF polygonBoundingRect;
    polygonBoundingRect.resize(4);
    if (i_polygon.size() > 0) {
        double fXLeft   = i_polygon[0].x();
        double fYTop    = i_polygon[0].y();
        double fXRight  = i_polygon[0].x();
        double fYBottom = i_polygon[0].y();
        for (int idxPt = 1; idxPt < i_polygon.size(); idxPt++) {
            if (i_polygon[idxPt].x() < fXLeft) {
                fXLeft = i_polygon[idxPt].x();
            }
            if (i_polygon[idxPt].y() < fYTop) {
                fYTop = i_polygon[idxPt].y();
            }
            if (i_polygon[idxPt].x() > fXRight) {
                fXRight = i_polygon[idxPt].x();
            }
            if (i_polygon[idxPt].y() > fYBottom) {
                fYBottom = i_polygon[idxPt].y();
            }
        }
        if (fXLeft > fXRight) {
            double fXLeftTmp = fXLeft;
            fXLeft = fXRight;
            fXRight = fXLeftTmp;
        }
        if (fYTop > fYBottom) {
            double fYTopTmp = fYTop;
            fYTop = fYBottom;
            fYBottom = fYTopTmp;
        }
        polygonBoundingRect[0] = QPointF(fXLeft,fYTop);
        polygonBoundingRect[1] = QPointF(fXRight,fYTop);
        polygonBoundingRect[2] = QPointF(fXRight,fYBottom);
        polygonBoundingRect[3] = QPointF(fXLeft,fYBottom);
    }
    return polygonBoundingRect;
}

//------------------------------------------------------------------------------
QPolygonF ZS::Draw::getEllipseFocusPoints( const QRectF& i_rct )
//------------------------------------------------------------------------------
{
    QPolygonF plgFocusPoints(2);
    QPointF ptC = i_rct.center();
    if (i_rct.width() == i_rct.height()) {
        plgFocusPoints[0] = ptC;
        plgFocusPoints[1] = ptC;
    }
    else if (i_rct.width() > i_rct.height()) {
        double fe = getEllipseCenterFocusDist(i_rct);
        plgFocusPoints[0] = QPointF( ptC.x() - fe, ptC.y() );
        plgFocusPoints[1] = QPointF( ptC.x() + fe, ptC.y() );
    }
    else { // if( i_rct.width() < i_rct.height() )
        double fe = getEllipseCenterFocusDist(i_rct);
        plgFocusPoints[0] = QPointF( ptC.x(), ptC.y() - fe );
        plgFocusPoints[1] = QPointF( ptC.x(), ptC.y() + fe );
    }
    return plgFocusPoints;
}

//------------------------------------------------------------------------------
double ZS::Draw::getEllipseCenterFocusDist( const QRectF& i_rct )
//------------------------------------------------------------------------------
{
    double fa = i_rct.width();
    double fb = i_rct.height();
    if (fa < fb) {
        double fTmp = fb;
        fb = fa;
        fa = fTmp;
    }
    return sqrt(Math::sqr(fa) - Math::sqr(fb));
}

//------------------------------------------------------------------------------
QPointF ZS::Draw::getSelectionPointCoors( const QLineF& i_lin, ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    QPointF pt = i_lin.center();
    if (i_selPt == ESelectionPoint::Center) {
    }
    else if (i_selPt == ESelectionPoint::TopLeft) {
        pt = i_lin.p1();
    }
    else if (i_selPt == ESelectionPoint::BottomRight) {
        pt = i_lin.p2();
    }
    return pt;
}

//------------------------------------------------------------------------------
QPointF ZS::Draw::getSelectionPointCoors( const QRectF& i_rct, ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    QPointF pt = i_rct.center();
    if (i_selPt == ESelectionPoint::Center) {
    }
    else if (i_selPt == ESelectionPoint::TopLeft) {
        pt = i_rct.topLeft();
    }
    else if (i_selPt == ESelectionPoint::TopRight) {
        pt = i_rct.topRight();
    }
    else if (i_selPt == ESelectionPoint::BottomRight) {
        pt = i_rct.bottomRight();
    }
    else if (i_selPt == ESelectionPoint::BottomLeft) {
        pt = i_rct.bottomLeft();
    }
    else if (i_selPt == ESelectionPoint::TopCenter) {
        pt = QPointF(i_rct.center().x(), i_rct.top());
    }
    else if (i_selPt == ESelectionPoint::RightCenter) {
        pt = QPointF(i_rct.right(), i_rct.center().y());
    }
    else if (i_selPt == ESelectionPoint::BottomCenter) {
        pt = QPointF(i_rct.center().x(), i_rct.bottom());
    }
    else if (i_selPt == ESelectionPoint::LeftCenter) {
        pt = QPointF(i_rct.left(), i_rct.center().y());
    }
    else if (i_selPt == ESelectionPoint::RotateTop) {
        pt = QPointF(i_rct.center().x(), i_rct.top()-getSelectionPointRotateDistance());
    }
    else if (i_selPt == ESelectionPoint::RotateBottom) {
        pt = QPointF(i_rct.center().x(), i_rct.bottom()+getSelectionPointRotateDistance());
    }
    return pt;
}

//------------------------------------------------------------------------------
/*! @brief Calculates the best position at the given rectangle to connect a line.

    The best position is one of the center points at the given rectangle.

    "QLineF::angle" is used to calculate the angle of the line between the two anchor points.

    From Qts documentation:
    -----------------------
    The return value will be in the range of values from 0.0 up
    to but not including 360.0. The angles are measured counter-clockwise from
    a point on the x-axis to the right of the origin (x > 0).
    The following diagram should also clarify whats been returned by "QLineF::angle":

                     90°
       135°    16     1     2    45°
            15        |        3
          14      +---+---+      4
    180° 13-------| Label |-------5   0°  (360°)
          12      +---+---+      6
            11        |        7
       225°    10     9     8   315°
                     270°

    Selection Point Position | clockwise | Selectoin Point
    of "Parent Item"         |           | of Label
    -------------------------+-----------+-----------------
    16, 1, 2                 | 135°-45°  | TopCenter
    3, 4, 5, 6, 7            |  45°-315° | RightCenter
    8, 9, 10                 | 315°-225° | BottomCenter
    11, 12, 13, 14, 15       | 225°-135° | LeftCenter

    If the angle is calculated the distance between the linked selection point
    of the parent item to the anchor line will also be taken into account.
    E.g. if the label is very close to the parent item it is better not to draw
    the anchor line.

    @param [in] i_line
    @param [in] i_rct
    @param [out] o_pSelPt

    @return Coorindates of the selection points on the bounding rectangle.
*/
QPointF ZS::Draw::getSelectionPointCoors(
    const QLineF& i_lin, const QRectF& i_rct, ESelectionPoint* o_pSelPt )
//------------------------------------------------------------------------------
{
    double fAngle = i_lin.angle();
    ESelectionPoint selPt = ESelectionPoint::None;
    if (fAngle >= 45.0 && fAngle <= 135.0) {
        selPt = ESelectionPoint::TopCenter;
    }
    else if (fAngle >= 225.0 && fAngle <= 315.0) {
        selPt = ESelectionPoint::BottomCenter;
    }
    else if (fAngle > 135.0 && fAngle < 225.0) {
        selPt = ESelectionPoint::LeftCenter;
    }
    else if ((fAngle > 315.0 && fAngle <= 360.0) || (fAngle >= 0.0 && fAngle < 45.0)) {
        selPt = ESelectionPoint::RightCenter;
    }
    if (o_pSelPt != nullptr) {
        *o_pSelPt = selPt;
    }
    return ZS::Draw::getSelectionPointCoors(i_rct, selPt);
}

//------------------------------------------------------------------------------
/*! @brief Returns a line with the given length and angle whose start point is at
           the start point of the passed line.

    Example: Input: Horizontal Line, Any Length, Angle = -90° (or +270)

                 P2 +
                    |
         Calculated |
         Line       | -90°
                 P1 +----------Line-----------+
                    P1                        P2

    Example: Input: Vertical Line, Any Length, Angle = 30°

                 P1 + P1
                  / |
      Calculated /  |
      Line      /30°|
               /    | Line
           P2 +     |
                    + P2
*/
QLineF ZS::Draw::getLineFromPolar(
    double i_fLength_px, double i_fAngle_degrees, const QLineF& i_line)
//------------------------------------------------------------------------------
{
    double fAngle_degrees = i_line.angle() + i_fAngle_degrees;
    QLineF line = QLineF::fromPolar(i_fLength_px, fAngle_degrees);
    QPointF ptOffset = i_line.p1();
    line.translate(ptOffset);
    return line;
}

//------------------------------------------------------------------------------
/*! @brief Calculates the perpendicular line to the given line.
           The start point of the resulting line is at the passed point.
           The end point of the result line is on the given line.

                i_pt
                 x
                 | Perpendicular line through i_pt
                 |
        P1 +-----x-------------------------+ P2 i_line

    @param [in] i_line
        Line for which the perpendicular should be calculated.
    @param [in] i_pt
        Start point from which the perpendicular line starts.
    @param [in] i_fMinLength_px
        Minimum length of the returned perpendicular line.
        Ignored if set to value <= 0.0.
        For values > 0.0 the returned perpendiculars length is of this
        minimum length. Useful if the passed line is on the given line
        as in this case the resulting line would have zero length.

    @return Perpendicular line with the given length whose start point is
            at the given point.
*/
QLineF ZS::Draw::getPerpendicularLine(const QLineF& i_line, const QPointF& i_pt, double i_fMinLength_px)
//------------------------------------------------------------------------------
{
    // Find parallel line to the given line that passes through the given point.
    QPointF ptOffset = i_pt - i_line.p1();
    QLineF parallelLine = i_line.translated(ptOffset);
    // Returns a line that is perpendicular to this line with
    // the same starting point and length.
    QLineF perpendicularLineTmp = parallelLine.normalVector();
    // Now get the intersection point of the perpendicular line with the given line.
    QPointF ptIntersection;
    perpendicularLineTmp.intersects(i_line, &ptIntersection);
    // Return the line from the given point to the intersection point.
    QLineF perpendicularLine(i_pt, ptIntersection);
    if (i_fMinLength_px > 0.0 && perpendicularLine.length() < i_fMinLength_px) {
        perpendicularLine = perpendicularLineTmp.translated(perpendicularLineTmp.p1()-ptIntersection);
        perpendicularLine.setLength(i_fMinLength_px);
    }
    return perpendicularLine;
}

//------------------------------------------------------------------------------
QString ZS::Draw::point2Str( const QPoint& i_pt )
//------------------------------------------------------------------------------
{
    return QString::number(i_pt.x()) + "/" + QString::number(i_pt.y());
}

//------------------------------------------------------------------------------
QString ZS::Draw::point2Str( const QPointF& i_pt, char i_cF, int i_iPrecision, bool i_bRound2Nearest )
//------------------------------------------------------------------------------
{
    QString str;
    if (i_bRound2Nearest && i_iPrecision > 0) {
        str = QString::number(Math::round2Nearest(i_pt.x(),1), i_cF, i_iPrecision) + "/"
            + QString::number(Math::round2Nearest(i_pt.y(),1), i_cF, i_iPrecision);
    }
    else {
        str = QString::number(i_pt.x(), i_cF, i_iPrecision) + "/"
            + QString::number(i_pt.y(), i_cF, i_iPrecision);
    }
    return str;
}

//------------------------------------------------------------------------------
QPoint ZS::Draw::str2Point( const QString& i_str, bool* i_pbConverted )
//------------------------------------------------------------------------------
{
    bool   bConverted = false;
    QPoint pt;

    QStringList strlst = i_str.split("/");

    if( strlst.size() == 2 )
    {
        QString strX = strlst[0];
        QString strY = strlst[1];
        bool    bXOk, bYOk;
        int     x = strX.toInt(&bXOk);
        int     y = strY.toInt(&bYOk);

        bConverted = bXOk && bYOk;

        if( bConverted )
        {
            pt.setX(x);
            pt.setY(y);
        }
    }

    if( i_pbConverted != nullptr )
    {
        *i_pbConverted = bConverted;
    }

    return pt;

} // str2Point

//------------------------------------------------------------------------------
QPointF ZS::Draw::str2PointF( const QString& i_str, bool* i_pbConverted )
//------------------------------------------------------------------------------
{
    bool    bConverted = false;
    QPointF pt;

    QStringList strlst = i_str.split("/");

    if( strlst.size() == 2 )
    {
        QString strX = strlst[0];
        QString strY = strlst[1];
        bool    bXOk, bYOk;
        double  x = strX.toDouble(&bXOk);
        double  y = strY.toDouble(&bYOk);

        bConverted = bXOk && bYOk;

        if( bConverted )
        {
            pt.setX(x);
            pt.setY(y);
        }
    }

    if( i_pbConverted != nullptr )
    {
        *i_pbConverted = bConverted;
    }

    return pt;

} // str2PointF

//------------------------------------------------------------------------------
QString ZS::Draw::size2Str( const QSize& i_size )
//------------------------------------------------------------------------------
{
    return QString::number(i_size.width()) + "/" + QString::number(i_size.height());
}

//------------------------------------------------------------------------------
QString ZS::Draw::size2Str( const QSizeF& i_size, char i_cF, int i_iPrecision, bool i_bRound2Nearest )
//------------------------------------------------------------------------------
{
    QString str;
    if (i_bRound2Nearest && i_iPrecision > 0) {
        str = QString::number(Math::round2Nearest(i_size.width(),1), i_cF, i_iPrecision) + "/"
            + QString::number(Math::round2Nearest(i_size.height(),1), i_cF, i_iPrecision);
    }
    else {
        str = QString::number(i_size.width(), i_cF, i_iPrecision) + "/"
            + QString::number(i_size.height(), i_cF, i_iPrecision);
    }
    return str;
}

//------------------------------------------------------------------------------
QSize ZS::Draw::str2Size( const QString& i_str, bool* i_pbConverted )
//------------------------------------------------------------------------------
{
    bool  bConverted = false;
    QSize siz;

    QStringList strlst = i_str.split("/");

    if( strlst.size() == 2 )
    {
        QString strX = strlst[0];
        QString strY = strlst[1];
        bool    bXOk, bYOk;
        int     x = strX.toInt(&bXOk);
        int     y = strY.toInt(&bYOk);

        bConverted = bXOk && bYOk;

        if( bConverted )
        {
            siz.setWidth(x);
            siz.setHeight(y);
        }
    }

    if( i_pbConverted != nullptr )
    {
        *i_pbConverted = bConverted;
    }

    return siz;

} // str2Size

//------------------------------------------------------------------------------
QSizeF ZS::Draw::str2SizeF( const QString& i_str, bool* i_pbConverted )
//------------------------------------------------------------------------------
{
    bool   bConverted = false;
    QSizeF siz;

    QStringList strlst = i_str.split("/");

    if( strlst.size() == 2 )
    {
        QString strX = strlst[0];
        QString strY = strlst[1];
        bool    bXOk, bYOk;
        double  x = strX.toDouble(&bXOk);
        double  y = strY.toDouble(&bYOk);

        bConverted = bXOk && bYOk;

        if( bConverted )
        {
            siz.setWidth(x);
            siz.setHeight(y);
        }
    }

    if( i_pbConverted != nullptr )
    {
        *i_pbConverted = bConverted;
    }

    return siz;

} // str2SizeF

//------------------------------------------------------------------------------
QString ZS::Draw::line2Str( const QLine& i_line )
//------------------------------------------------------------------------------
{
    return point2Str(i_line.p1()) + ", " + point2Str(i_line.p2());
}

//------------------------------------------------------------------------------
QString ZS::Draw::line2Str( const QLineF& i_line, char i_cF, int i_iPrecision, bool i_bRound2Nearest )
//------------------------------------------------------------------------------
{
    return point2Str(i_line.p1(), i_cF, i_iPrecision, i_bRound2Nearest) + ", " +
           point2Str(i_line.p2(), i_cF, i_iPrecision, i_bRound2Nearest);
}

//------------------------------------------------------------------------------
QLine ZS::Draw::str2Line( const QString& i_str, bool* i_pbConverted )
//------------------------------------------------------------------------------
{
    bool  bConverted = false;
    QLine line;

    QStringList strlst = i_str.split(",");

    if( strlst.size() == 2 )
    {
        QString strP1 = strlst[0];
        QString strP2 = strlst[1];
        bool    bP1Ok, bP2Ok;
        QPoint  p1 = str2Point(strP1,&bP1Ok);
        QPoint  p2 = str2Point(strP2,&bP2Ok);

        bConverted = bP1Ok && bP2Ok;

        if( bConverted )
        {
            line.setP1(p1);
            line.setP2(p2);
        }
    }

    if( i_pbConverted != nullptr )
    {
        *i_pbConverted = bConverted;
    }

    return line;

} // str2Line

//------------------------------------------------------------------------------
QLineF ZS::Draw::str2LineF( const QString& i_str, bool* i_pbConverted )
//------------------------------------------------------------------------------
{
    bool   bConverted = false;
    QLineF line;

    QStringList strlst = i_str.split(",");

    if( strlst.size() == 2 )
    {
        QString strP1 = strlst[0];
        QString strP2 = strlst[1];
        bool    bP1Ok, bP2Ok;
        QPointF p1 = str2PointF(strP1,&bP1Ok);
        QPointF p2 = str2PointF(strP2,&bP2Ok);

        bConverted = bP1Ok && bP2Ok;

        if( bConverted )
        {
            line.setP1(p1);
            line.setP2(p2);
        }
    }

    if( i_pbConverted != nullptr )
    {
        *i_pbConverted = bConverted;
    }

    return line;

} // str2LineF

//------------------------------------------------------------------------------
QString ZS::Draw::rect2Str( const QRect& i_rct, bool i_bAddWidthAndHeight )
//------------------------------------------------------------------------------
{
    QString str =
        point2Str(i_rct.topLeft()) + ", " +
        point2Str(i_rct.topRight()) + ", " +
        point2Str(i_rct.bottomRight()) + ", " +
        point2Str(i_rct.bottomLeft());
    if (i_bAddWidthAndHeight) {
        str += ", " + size2Str(i_rct.size());
    }
    return str;
}

//------------------------------------------------------------------------------
QString ZS::Draw::rect2Str( const QRectF& i_rct, char i_cF, int i_iPrecision, bool i_bRound2Nearest, bool i_bAddWidthAndHeight )
//------------------------------------------------------------------------------
{
    QString str =
        point2Str(i_rct.topLeft(), i_cF, i_iPrecision, i_bRound2Nearest) + ", " +
        point2Str(i_rct.topRight(), i_cF, i_iPrecision, i_bRound2Nearest) + ", " +
        point2Str(i_rct.bottomRight(), i_cF, i_iPrecision, i_bRound2Nearest) + ", " +
        point2Str(i_rct.bottomLeft(), i_cF, i_iPrecision, i_bRound2Nearest);
    if (i_bAddWidthAndHeight) {
        str += ", " + size2Str(i_rct.size(), i_cF, i_iPrecision, i_bRound2Nearest);
    }
    return str;
}

//------------------------------------------------------------------------------
QString ZS::Draw::rect2Str( const QPolygon& i_rct, bool i_bAddWidthAndHeight )
//------------------------------------------------------------------------------
{
    QString str;
    if (i_rct.size() >= 4) {
        for (int idxPt = 0; idxPt < i_rct.size() && idxPt < 4; idxPt++) {
            if (idxPt > 0) str += ", ";
            str += point2Str(i_rct[idxPt]);
        }
        if (i_bAddWidthAndHeight) {
            double w = QLineF(i_rct[0], i_rct[1]).length();
            double h = QLineF(i_rct[1], i_rct[2]).length();
            QSize  siz( static_cast<int>(w), static_cast<int>(h) );
            str += ", " + size2Str(siz);
        }
    }
    return str;
}

//------------------------------------------------------------------------------
QString ZS::Draw::rect2Str( const QPolygonF& i_rct, char i_cF, int i_iPrecision, bool i_bRound2Nearest, bool i_bAddWidthAndHeight )
//------------------------------------------------------------------------------
{
    QString str;
    if (i_rct.size() >= 4) {
        for (int idxPt = 0; idxPt < i_rct.size() && idxPt < 4; idxPt++) {
            if (idxPt > 0) str += ", ";
            str += point2Str(i_rct[idxPt], i_cF, i_iPrecision, i_bRound2Nearest);
        }
        if (i_bAddWidthAndHeight) {
            double w = QLineF(i_rct[0], i_rct[1]).length();
            double h = QLineF(i_rct[1], i_rct[2]).length();
            QSize  siz( static_cast<int>(w), static_cast<int>(h) );
            str += ", " + size2Str(siz);
        }
    }
    return str;
}

//------------------------------------------------------------------------------
QRect ZS::Draw::str2Rect( const QString& i_str, bool* i_pbConverted )
//------------------------------------------------------------------------------
{
    bool  bConverted = false;
    QRect rct;

    QStringList strlst = i_str.split(",");

    if( strlst.size() >= 4 )
    {
        QString strLT = strlst[0];
        QString strRT = strlst[1];
        QString strRB = strlst[2];
        QString strLB = strlst[3];

        bool    bLTOk, bRTOk, bRBOk, bLBOk;
        QPoint  ptLT = str2Point(strLT,&bLTOk);
        QPoint  ptRB = str2Point(strRB,&bRBOk);

        str2Point(strRT,&bRTOk);
        str2Point(strLB,&bLBOk);

        bConverted = bLTOk && bRTOk && bRBOk && bLBOk;

        if( bConverted )
        {
            rct = QRect(ptLT,ptRB);
        }
    }

    if( i_pbConverted != nullptr )
    {
        *i_pbConverted = bConverted;
    }

    return rct;

} // str2Rect

//------------------------------------------------------------------------------
QRectF ZS::Draw::str2RectF( const QString& i_str, bool* i_pbConverted )
//------------------------------------------------------------------------------
{
    bool   bConverted = false;
    QRectF rct;

    QStringList strlst = i_str.split(",");

    if( strlst.size() >= 4 )
    {
        QString strLT = strlst[0];
        QString strRT = strlst[1];
        QString strRB = strlst[2];
        QString strLB = strlst[3];

        bool    bLTOk, bRTOk, bRBOk, bLBOk;
        QPointF ptLT = str2PointF(strLT,&bLTOk);
        QPointF ptRB = str2PointF(strRB,&bRBOk);

        str2PointF(strRT,&bRTOk);
        str2PointF(strLB,&bLBOk);

        bConverted = bLTOk && bRTOk && bRBOk && bLBOk;

        if( bConverted )
        {
            rct = QRectF(ptLT,ptRB);
        }
    }

    if( i_pbConverted != nullptr )
    {
        *i_pbConverted = bConverted;
    }

    return rct;

} // str2RectF

//------------------------------------------------------------------------------
QString ZS::Draw::polygon2Str( const QPolygon& i_polygon )
//------------------------------------------------------------------------------
{
    QString str;
    for (int idxPt = 0; idxPt < i_polygon.size(); idxPt++) {
        if (idxPt > 0) str += ", ";
        str += point2Str(i_polygon[idxPt]);
    }
    return str;
}

//------------------------------------------------------------------------------
QString ZS::Draw::polygon2Str( const QPolygonF& i_polygon, char i_cF, int i_iPrecision, bool i_bRound2Nearest )
//------------------------------------------------------------------------------
{
    QString str;
    for (int idxPt = 0; idxPt < i_polygon.size(); idxPt++) {
        if (idxPt > 0) str += ", ";
        str += point2Str(i_polygon[idxPt], i_cF, i_iPrecision, i_bRound2Nearest);
    }
    return str;
}

//------------------------------------------------------------------------------
QPolygon ZS::Draw::str2Polygon( const QString& i_str, bool* i_pbConverted )
//------------------------------------------------------------------------------
{
    bool bConverted = false;
    QPolygon plg;
    QStringList strlst = i_str.split(",");
    for (int idxPt = 0; idxPt < strlst.size(); idxPt++) {
        QString strPt = strlst[idxPt];
        QPoint pt = str2Point(strPt, &bConverted);
        if (!bConverted) {
            break;
        }
        plg.append(pt);
    }
    if (i_pbConverted != nullptr) {
        *i_pbConverted = bConverted;
    }
    return plg;
}

//------------------------------------------------------------------------------
QPolygonF ZS::Draw::str2PolygonF( const QString& i_str, bool* i_pbConverted )
//------------------------------------------------------------------------------
{
    bool bConverted = false;
    QPolygonF plg;
    QStringList strlst = i_str.split(",");
    for (int idxPt = 0; idxPt < strlst.size(); idxPt++) {
        QString strPt = strlst[idxPt];
        QPointF pt = str2PointF(strPt, &bConverted);
        if (!bConverted) {
            break;
        }
        plg.append(pt);
    }
    if (i_pbConverted != nullptr) {
        *i_pbConverted = bConverted;
    }
    return plg;
}

//==============================================================================
namespace ZS { namespace Draw { namespace XmlStreamParser {
//==============================================================================

//------------------------------------------------------------------------------
void raiseErrorAttributeNotDefined(
    QXmlStreamReader& i_xmlStreamReader,
    const QString& i_strElemName,
    const QString& i_strAttrName)
//------------------------------------------------------------------------------
{
    i_xmlStreamReader.raiseError(
        "Attribute \"" + i_strAttrName + "\" for element \"" + i_strElemName + "\" not defined");
}

//------------------------------------------------------------------------------
void raiseErrorAttributeOutOfRange(
    QXmlStreamReader& i_xmlStreamReader,
    const QString& i_strElemName,
    const QString& i_strAttrName,
    const QString& i_strAttrVal)
//------------------------------------------------------------------------------
{
    i_xmlStreamReader.raiseError(
        "Attribute \"" + i_strAttrName + "\" for element \"" + i_strElemName + "\" (=" + i_strAttrVal + ") is out of range");
}

//------------------------------------------------------------------------------
CEnumScaleDimensionUnit getDimensionUnit(
    QXmlStreamReader& i_xmlStreamReader,
    QXmlStreamAttributes& i_xmlStreamAttrs,
    const QString& i_strElemName,
    const QString& i_strAttrName,
    bool i_bAttrIsMandatory,
    const CEnumScaleDimensionUnit& i_eDefaultVal )
//------------------------------------------------------------------------------
{
    CEnumScaleDimensionUnit dimensionUnit = i_eDefaultVal;
    if( !i_xmlStreamAttrs.hasAttribute(i_strAttrName) ) {
        if (i_bAttrIsMandatory) {
            raiseErrorAttributeNotDefined(i_xmlStreamReader, i_strElemName, i_strAttrName);
        }
    }
    else {
        QString strAttrVal = i_xmlStreamAttrs.value(i_strAttrName).toString();
        bool bOk = true;
        CEnumScaleDimensionUnit eVal = CEnumScaleDimensionUnit::fromString(strAttrVal, &bOk);
        if (bOk) {
            dimensionUnit = eVal;
        } else {
            raiseErrorAttributeOutOfRange(
                i_xmlStreamReader, i_strElemName, i_strAttrName, strAttrVal);
        }
    }
    return dimensionUnit;
}

//------------------------------------------------------------------------------
CEnumNormedPaperSize getNormedPaperSize(
    QXmlStreamReader& i_xmlStreamReader,
    QXmlStreamAttributes& i_xmlStreamAttrs,
    const QString& i_strElemName,
    const QString& i_strAttrName,
    bool i_bAttrIsMandatory,
    const CEnumNormedPaperSize& i_eDefaultVal )
//------------------------------------------------------------------------------
{
    CEnumNormedPaperSize paperSize = i_eDefaultVal;
    if( !i_xmlStreamAttrs.hasAttribute(i_strAttrName) ) {
        if (i_bAttrIsMandatory) {
            raiseErrorAttributeNotDefined(i_xmlStreamReader, i_strElemName, i_strAttrName);
        }
    }
    else {
        QString strAttrVal = i_xmlStreamAttrs.value(i_strAttrName).toString();
        bool bOk = true;
        CEnumNormedPaperSize eVal = CEnumNormedPaperSize::fromString(strAttrVal, &bOk);
        if (bOk) {
            paperSize = eVal;
        } else {
            raiseErrorAttributeOutOfRange(
                i_xmlStreamReader, i_strElemName, i_strAttrName, strAttrVal);
        }
    }
    return paperSize;
}

//------------------------------------------------------------------------------
CEnumOrientation getOrientation(
    QXmlStreamReader& i_xmlStreamReader,
    QXmlStreamAttributes& i_xmlStreamAttrs,
    const QString& i_strElemName,
    const QString& i_strAttrName,
    bool i_bAttrIsMandatory,
    const CEnumOrientation& i_eDefaultVal )
//------------------------------------------------------------------------------
{
    CEnumOrientation orientation = i_eDefaultVal;
    if( !i_xmlStreamAttrs.hasAttribute(i_strAttrName) ) {
        if (i_bAttrIsMandatory) {
            raiseErrorAttributeNotDefined(i_xmlStreamReader, i_strElemName, i_strAttrName);
        }
    }
    else {
        QString strAttrVal = i_xmlStreamAttrs.value(i_strAttrName).toString();
        bool bOk = true;
        CEnumOrientation eVal = CEnumOrientation::fromString(strAttrVal, &bOk);
        if (bOk) {
            orientation = eVal;
        } else {
            raiseErrorAttributeOutOfRange(
                i_xmlStreamReader, i_strElemName, i_strAttrName, strAttrVal);
        }
    }
    return orientation;
}

//------------------------------------------------------------------------------
CEnumLineStyle getLineStyle(
    QXmlStreamReader& i_xmlStreamReader,
    QXmlStreamAttributes& i_xmlStreamAttrs,
    const QString& i_strElemName,
    const QString& i_strAttrName,
    bool i_bAttrIsMandatory,
    const CEnumLineStyle& i_eDefaultVal )
//------------------------------------------------------------------------------
{
    CEnumLineStyle eLineStyle = i_eDefaultVal;
    if( !i_xmlStreamAttrs.hasAttribute(i_strAttrName) ) {
        if (i_bAttrIsMandatory) {
            raiseErrorAttributeNotDefined(i_xmlStreamReader, i_strElemName, i_strAttrName);
        }
    }
    else {
        QString strAttrVal = i_xmlStreamAttrs.value(i_strAttrName).toString();
        bool bOk = true;
        CEnumLineStyle eVal = CEnumLineStyle::fromString(strAttrVal, &bOk);
        if (bOk) {
            eLineStyle = eVal;
        } else {
            raiseErrorAttributeOutOfRange(
                i_xmlStreamReader, i_strElemName, i_strAttrName, strAttrVal);
        }
    }
    return eLineStyle;
}

//------------------------------------------------------------------------------
ETextSize getTextSize(
    QXmlStreamReader& i_xmlStreamReader,
    QXmlStreamAttributes& i_xmlStreamAttrs,
    const QString& i_strElemName,
    const QString& i_strAttrName,
    bool i_bAttrIsMandatory,
    ETextSize i_eDefaultVal )
//------------------------------------------------------------------------------
{
    ETextSize textSize = i_eDefaultVal;
    if( !i_xmlStreamAttrs.hasAttribute(i_strAttrName) ) {
        if (i_bAttrIsMandatory) {
            raiseErrorAttributeNotDefined(i_xmlStreamReader, i_strElemName, i_strAttrName);
        }
    }
    else {
        QString strAttrVal = i_xmlStreamAttrs.value(i_strAttrName).toString();
        bool bOk = true;
        ETextSize eVal = str2TextSize(strAttrVal, &bOk);
        if (bOk) {
            textSize = eVal;
        } else {
            raiseErrorAttributeOutOfRange(
                i_xmlStreamReader, i_strElemName, i_strAttrName, strAttrVal);
        }
    }
    return textSize;
}

//------------------------------------------------------------------------------
CEnumTextStyle getTextStyle(
    QXmlStreamReader& i_xmlStreamReader,
    QXmlStreamAttributes& i_xmlStreamAttrs,
    const QString& i_strElemName,
    const QString& i_strAttrName,
    bool i_bAttrIsMandatory,
    const CEnumTextStyle& i_eDefaulVal )
//------------------------------------------------------------------------------
{
    CEnumTextStyle eTextStyle = i_eDefaulVal;
    if( !i_xmlStreamAttrs.hasAttribute(i_strAttrName) ) {
        if (i_bAttrIsMandatory) {
            raiseErrorAttributeNotDefined(i_xmlStreamReader, i_strElemName, i_strAttrName);
        }
    }
    else {
        QString strAttrVal = i_xmlStreamAttrs.value(i_strAttrName).toString();
        bool bOk = true;
        CEnumTextStyle eVal = CEnumTextStyle::fromString(strAttrVal, &bOk);
        if (bOk) {
            eTextStyle = eVal;
        } else {
            raiseErrorAttributeOutOfRange(
                i_xmlStreamReader, i_strElemName, i_strAttrName, strAttrVal);
        }
    }
    return eTextStyle;
}

//------------------------------------------------------------------------------
CEnumTextEffect getTextEffect(
    QXmlStreamReader& i_xmlStreamReader,
    QXmlStreamAttributes& i_xmlStreamAttrs,
    const QString& i_strElemName,
    const QString& i_strAttrName,
    bool i_bAttrIsMandatory,
    const CEnumTextEffect& i_eDefaultVal )
//------------------------------------------------------------------------------
{
    CEnumTextEffect eTextEffect = i_eDefaultVal;
    if( !i_xmlStreamAttrs.hasAttribute(i_strAttrName) ) {
        if (i_bAttrIsMandatory) {
            raiseErrorAttributeNotDefined(i_xmlStreamReader, i_strElemName, i_strAttrName);
        }
    }
    else {
        QString strAttrVal = i_xmlStreamAttrs.value(i_strAttrName).toString();
        bool bOk = true;
        CEnumTextEffect eVal = CEnumTextEffect::fromString(strAttrVal, &bOk);
        if (bOk) {
            eTextEffect = eVal;
        } else {
            raiseErrorAttributeOutOfRange(
                i_xmlStreamReader, i_strElemName, i_strAttrName, strAttrVal);
        }
    }
    return eTextEffect;
}

//------------------------------------------------------------------------------
CUnit getUnit(
    QXmlStreamReader& i_xmlStreamReader,
    QXmlStreamAttributes& i_xmlStreamAttrs,
    const QString& i_strElemName,
    const QString& i_strAttrName,
    bool i_bAttrIsMandatory,
    const CUnit& i_unitDefault )
//------------------------------------------------------------------------------
{
    CUnit unit = i_unitDefault;
    if( !i_xmlStreamAttrs.hasAttribute(i_strAttrName) ) {
        if (i_bAttrIsMandatory) {
            raiseErrorAttributeNotDefined(i_xmlStreamReader, i_strElemName, i_strAttrName);
        }
    }
    else {
        QString strAttrVal = i_xmlStreamAttrs.value(i_strAttrName).toString();
        CUnit unitTmp = strAttrVal;
        if (unitTmp.isValid()) {
            unit = unitTmp;
        } else {
            raiseErrorAttributeOutOfRange(
                i_xmlStreamReader, i_strElemName, i_strAttrName, strAttrVal);
        }
    }
    return unit;
}

//------------------------------------------------------------------------------
CPhysVal getPhysVal(
    QXmlStreamReader& i_xmlStreamReader,
    QXmlStreamAttributes& i_xmlStreamAttrs,
    const QString& i_strElemName,
    const QString& i_strAttrName,
    bool i_bAttrIsMandatory,
    const CPhysVal& i_physValDefault )
//------------------------------------------------------------------------------
{
    CPhysVal physVal = i_physValDefault;
    if( !i_xmlStreamAttrs.hasAttribute(i_strAttrName) ) {
        if (i_bAttrIsMandatory) {
            raiseErrorAttributeNotDefined(i_xmlStreamReader, i_strElemName, i_strAttrName);
        }
    }
    else {
        QString strAttrVal = i_xmlStreamAttrs.value(i_strAttrName).toString();
        CPhysVal physValTmp(Units.Length);
        physValTmp = strAttrVal;
        if (physValTmp.isValid()) {
            physVal = physValTmp;
        } else {
            raiseErrorAttributeOutOfRange(
                i_xmlStreamReader, i_strElemName, i_strAttrName, strAttrVal);
        }
    }
    return physVal;
}

//------------------------------------------------------------------------------
QFont getFont(
    QXmlStreamReader& i_xmlStreamReader,
    QXmlStreamAttributes& i_xmlStreamAttrs,
    const QString& i_strElemName,
    const QString& i_strAttrName,
    bool i_bAttrIsMandatory,
    const QFont& i_fntDefault )
//------------------------------------------------------------------------------
{
    QFont fnt = i_fntDefault;
    if (!i_xmlStreamAttrs.hasAttribute(i_strAttrName)) {
        if (i_bAttrIsMandatory) {
            raiseErrorAttributeNotDefined(i_xmlStreamReader, i_strElemName, i_strAttrName);
        }
    }
    else {
        QString strAttrVal = i_xmlStreamAttrs.value(i_strAttrName).toString();
        fnt.setFamily(strAttrVal);
    }
    return fnt;
}

//------------------------------------------------------------------------------
QColor getColor(
    QXmlStreamReader& i_xmlStreamReader,
    QXmlStreamAttributes& i_xmlStreamAttrs,
    const QString& i_strElemName,
    const QString& i_strAttrName,
    bool i_bAttrIsMandatory,
    const QColor& i_clrDefault )
//------------------------------------------------------------------------------
{
    QColor clr = i_clrDefault;
    if (!i_xmlStreamAttrs.hasAttribute(i_strAttrName)) {
        if (i_bAttrIsMandatory) {
            raiseErrorAttributeNotDefined(i_xmlStreamReader, i_strElemName, i_strAttrName);
        }
    }
    else {
        QString strAttrVal = i_xmlStreamAttrs.value(i_strAttrName).toString();
        clr.setNamedColor(strAttrVal);
    }
    return clr;
}

//------------------------------------------------------------------------------
bool getBoolVal(
    QXmlStreamReader& i_xmlStreamReader,
    QXmlStreamAttributes& i_xmlStreamAttrs,
    const QString& i_strElemName,
    const QString& i_strAttrName,
    bool i_bAttrIsMandatory,
    bool i_bValDefault )
//------------------------------------------------------------------------------
{
    bool bVal = i_bValDefault;
    if (!i_xmlStreamAttrs.hasAttribute(i_strAttrName)) {
        if (i_bAttrIsMandatory) {
            raiseErrorAttributeNotDefined(i_xmlStreamReader, i_strElemName, i_strAttrName);
        }
    }
    else {
        QString strAttrVal = i_xmlStreamAttrs.value(i_strAttrName).toString();
        bool bOk = true;
        bool bValTmp = str2Bool(strAttrVal, &bOk);
        if (bOk) {
            bVal = bValTmp;
        } else {
            raiseErrorAttributeOutOfRange(
                i_xmlStreamReader, i_strElemName, i_strAttrName, strAttrVal);
        }
    }
    return bVal;
}

//------------------------------------------------------------------------------
int getIntVal(
    QXmlStreamReader& i_xmlStreamReader,
    QXmlStreamAttributes& i_xmlStreamAttrs,
    const QString& i_strElemName,
    const QString& i_strAttrName,
    bool i_bAttrIsMandatory,
    int i_iValDefault )
//------------------------------------------------------------------------------
{
    int iVal = i_iValDefault;
    if (!i_xmlStreamAttrs.hasAttribute(i_strAttrName)) {
        if (i_bAttrIsMandatory) {
            raiseErrorAttributeNotDefined(i_xmlStreamReader, i_strElemName, i_strAttrName);
        }
    }
    else {
        QString strAttrVal = i_xmlStreamAttrs.value(i_strAttrName).toString();
        bool bOk = true;
        int iValTmp = strAttrVal.toInt(&bOk);
        if (bOk) {
            iVal = iValTmp;
        } else {
            raiseErrorAttributeOutOfRange(
                i_xmlStreamReader, i_strElemName, i_strAttrName, strAttrVal);
        }
    }
    return iVal;
}

//------------------------------------------------------------------------------
std::pair<int, int> getIntPair(
    QXmlStreamReader& i_xmlStreamReader,
    QXmlStreamAttributes& i_xmlStreamAttrs,
    const QString& i_strElemName,
    const QString& i_strAttrName,
    const QString& i_strDelimiter,
    bool i_bAttrIsMandatory,
    const std::pair<int, int>& i_valDefault )
//------------------------------------------------------------------------------
{
    std::pair<int, int> iPair = i_valDefault;
    if (!i_xmlStreamAttrs.hasAttribute(i_strAttrName)) {
        if (i_bAttrIsMandatory) {
            raiseErrorAttributeNotDefined(i_xmlStreamReader, i_strElemName, i_strAttrName);
        }
    }
    else {
        QString strAttrVal = i_xmlStreamAttrs.value(i_strAttrName).toString();
        QStringList strlstVals = strAttrVal.split(i_strDelimiter, Qt::SkipEmptyParts);
        if (strlstVals.size() != 2) {
            raiseErrorAttributeOutOfRange(
                i_xmlStreamReader, i_strElemName, i_strAttrName, strAttrVal);
        }
        else {
            bool bOk = true;
            int iVal1 = strlstVals[0].toInt(&bOk);
            if (!bOk) {
                raiseErrorAttributeOutOfRange(
                    i_xmlStreamReader, i_strElemName, i_strAttrName, strAttrVal);
            }
            else {
                int iVal2 = strlstVals[1].toInt(&bOk);
                if (!bOk) {
                    raiseErrorAttributeOutOfRange(
                        i_xmlStreamReader, i_strElemName, i_strAttrName, strAttrVal);
                }
                else {
                    iPair = std::make_pair(iVal1, iVal2);
                }
            }
        }
    }
    return iPair;
}

//------------------------------------------------------------------------------
double getDoubleVal(
    QXmlStreamReader& i_xmlStreamReader,
    QXmlStreamAttributes& i_xmlStreamAttrs,
    const QString& i_strElemName,
    const QString& i_strAttrName,
    bool i_bAttrIsMandatory,
    double i_fValDefault )
//------------------------------------------------------------------------------
{
    double fVal = i_fValDefault;
    if (!i_xmlStreamAttrs.hasAttribute(i_strAttrName)) {
        if (i_bAttrIsMandatory) {
            raiseErrorAttributeNotDefined(i_xmlStreamReader, i_strElemName, i_strAttrName);
        }
    }
    else {
        QString strAttrVal = i_xmlStreamAttrs.value(i_strAttrName).toString();
        bool bOk = true;
        double fValTmp = strAttrVal.toDouble(&bOk);
        if (bOk) {
            fVal = fValTmp;
        } else {
            raiseErrorAttributeOutOfRange(
                i_xmlStreamReader, i_strElemName, i_strAttrName, strAttrVal);
        }
    }
    return fVal;
}

} } } // namespace ZS::Draw::XmlStreamParser
