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
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"
#include "ZSPhysVal/ZSPhysVal.h"
#include "ZSSys/ZSSysMath.h"

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

    if( i_type == QTransform::TxNone )
    {
        str = "None";
    }
    else // if( i_type != QTransform::TxNone )
    {
        if( i_type & QTransform::TxTranslate )
        {
            str = "Translate";
        }
        if( i_type & QTransform::TxScale )
        {
            if( !str.isEmpty() )
            {
                str += "|";
            }
            str += "Scale";
        }
        if( i_type & QTransform::TxRotate )
        {
            if( !str.isEmpty() )
            {
                str += "|";
            }
            str += "Rotate";
        }
        if( i_type & QTransform::TxShear )
        {
            if( !str.isEmpty() )
            {
                str += "|";
            }
            str += "Shear";
        }
        if( i_type & QTransform::TxProject )
        {
            if( !str.isEmpty() )
            {
                str += "|";
            }
            str += "Project";
        }
    } // if( i_type != QTransform::TxNone )

    return str;

} // qTransformationType2Str

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

    if( i_transform.m11() > 0.0 )
    {
        strFormula += strm11 + "*x";
    }
    else if( i_transform.m11() < 0.0 )
    {
        strFormula += "-" + strm11 + "*x";
    }
    if( i_transform.m21() > 0.0 )
    {
        strFormula += " + " + strm21 + "*y";
    }
    else if( i_transform.m21() < 0.0 )
    {
        strFormula += " - " + strm21 + "*y";
    }
    if( i_transform.m31() > 0.0 )
    {
        strFormula += " + " + strm31;
    }
    else if( i_transform.m31() < 0.0 )
    {
        strFormula += " - " + strm31;
    }
    strlst << strFormula; // Idx 1

    strFormula = "y' = ";

    if( i_transform.m22() > 0.0 )
    {
        strFormula += strm22 + "*y";
    }
    else if( i_transform.m22() < 0.0 )
    {
        strFormula += "-" + strm22 + "*y";
    }
    if( i_transform.m12() > 0.0 )
    {
        strFormula += " + " + strm12 + "*x";
    }
    else if( i_transform.m12() < 0.0 )
    {
        strFormula += " - " + strm12 + "*x";
    }
    if( i_transform.m32() > 0.0 )
    {
        strFormula += " + " + strm32;
    }
    else if( i_transform.m32() < 0.0 )
    {
        strFormula += " - " + strm32;
    }

    strlst << strFormula; // Idx 2

    if( !i_transform.isAffine() )
    {
        strFormula = "w' = ";

        if( i_transform.m32() > 0.0 )
        {
            strFormula += strm32 + "*x";
        }
        else if( i_transform.m32() < 0.0 )
        {
            strFormula += "-" + strm32 + "*x";
        }
        if( i_transform.m23() > 0.0 )
        {
            strFormula += " + " + strm23 + "*y";
        }
        else if( i_transform.m23() < 0.0 )
        {
            strFormula += " - " + strm23 + "*y";
        }
        if( i_transform.m33() > 0.0 )
        {
            strFormula += " + " + strm33;
        }
        else if( i_transform.m33() < 0.0 )
        {
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
QString ZS::Draw::qGraphicsItemChange2Str( int i_change )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str( s_arEnumStrGraphicsItemChange, _ZSArrLen(s_arEnumStrGraphicsItemChange), i_change );
}

/*==============================================================================
QGraphicsItem::QGraphicsSceneEvents
==============================================================================*/

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
QString ZS::Draw::qGraphicsSceneMouseEvent2Str( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString str = "Pos {" + point2Str(i_pEv->pos()) + "}" +
        ", ScenePos {" + point2Str(i_pEv->scenePos()) + "}" +
        ", ScreenPos {" + point2Str(i_pEv->screenPos()) + "}";
    return str;
}

/*******************************************************************************
exported methods implementation
*******************************************************************************/

//------------------------------------------------------------------------------
void ZS::Draw::getLineEndPolygons(
    const QLineF&        i_line,
    const CDrawSettings& i_drawSettings,
    QPolygonF*           i_pplgLineStart,
    QPolygonF*           i_pplgLineEnd )
//------------------------------------------------------------------------------
{
    /*--------------------------------------------------------------------------

    Line Start (line.p1(())    Line End (line.p2())

             Pt3                      Pt3
             /                         \
            /                           \
        Pt2/__Pt4____           ____Pt4__\Pt2
           \                             /
            \                           /
             \                         /
             Pt1                      Pt1

    --------------------------------------------------------------------------*/

    if( i_drawSettings.getLineEndStyle(ELinePoint::Start) != ELineEndStyle::Normal
     || i_drawSettings.getLineEndStyle(ELinePoint::End) != ELineEndStyle::Normal )
    {
        double fAngle_rad = 0.0;

        QPolygonF            plgLineStart;
        QPolygonF            plgLineEnd;
        QPointF              pt;
        int                  iLinePoint;
        ELinePoint           linePoint;
        ELineEndWidth        width;
        ELineEndLength       length;
        ELineEndBaseLineType baseLineType;

        double fdx = i_line.dx();
        double fdy = i_line.dy();
        double flen = i_line.length();

        if( fabs(flen) > 0.0 )
        {
            fAngle_rad = acos( fdx / flen );
        }

        // Right of y axis ..
        if( fdx >= 0.0 )
        {
            // "Above" x axis ( 1. quadrant, angle returned by acos: 0° <= f <= 90°) ..
            if( fdy <= 0.0 )
            {
            }
            // "Below" x axis ( 2. quadrant, angle returned by acos: 90° <= f <= 180°) ..
            else
            {
                fAngle_rad *= -1.0;
            }
        }
        // Left of y axis ..
        else
        {
            // "Above" x axis ( 3. quadrant, angle returned by acos: 0° <= f <= 90°) ..
            if( fdy <= 0.0 )
            {
            }
            // "Below" x axis ( 4. quadrant, angle returned by acos: 90° <= f <= 180°) ..
            else
            {
                fAngle_rad *= -1.0;
            }
        }

        #ifdef _DEBUG
        //double fAngle_deg = 360.0*fAngle_rad/Math::c_f2PI;
        #endif

        for( iLinePoint = 0; iLinePoint < CEnumLinePoint::count(); iLinePoint++ )
        {
            linePoint = static_cast<ELinePoint>(iLinePoint);

            width        = i_drawSettings.getLineEndWidth(linePoint);
            length       = i_drawSettings.getLineEndLength(linePoint);
            baseLineType = i_drawSettings.getLineEndBaseLineType(linePoint);

            if( iLinePoint == static_cast<int>(ELinePoint::Start) )
            {
                if( i_pplgLineStart != nullptr )
                {
                    pt.setX( i_line.p1().x() + lineEndLength2dx(length) );
                    pt.setY( i_line.p1().y() + lineEndWidth2dy(width)/2.0 );
                    pt = rotatePoint( i_line.p1(), pt, fAngle_rad );
                    plgLineStart.append(pt); // Pt1

                    plgLineStart.append( i_line.p1() ); // Pt2

                    pt.setX( i_line.p1().x() + lineEndLength2dx(length) );
                    pt.setY( i_line.p1().y() - lineEndWidth2dy(width)/2.0 );
                    pt = rotatePoint( i_line.p1(), pt, fAngle_rad );
                    plgLineStart.append(pt); // Pt3

                    if( baseLineType == ELineEndBaseLineType::Normal )
                    {
                        pt.setX( i_line.p1().x() + lineEndLength2dx(length) );
                        pt.setY( i_line.p1().y() );
                        pt = rotatePoint( i_line.p1(), pt, fAngle_rad );
                        plgLineStart.append(pt); // Pt4
                    }
                    else if( baseLineType == ELineEndBaseLineType::Indented )
                    {
                        pt.setX( i_line.p1().x() + lineEndLength2dx(length)/2.0 );
                        pt.setY( i_line.p1().y() );
                        pt = rotatePoint( i_line.p1(), pt, fAngle_rad );
                        plgLineStart.append(pt); // Pt4
                    }

                    *i_pplgLineStart = plgLineStart;

                } // if( i_pplgLineStart != nullptr )

            } // if( iLinePoint == ELinePoint::Start )

            else // if( iLinePoint == ELinePoint::End )
            {
                if( i_pplgLineEnd != nullptr )
                {
                    pt.setX( i_line.p2().x() - lineEndLength2dx(length) );
                    pt.setY( i_line.p2().y() + lineEndWidth2dy(width)/2.0 );
                    pt = rotatePoint( i_line.p2(), pt, fAngle_rad );
                    plgLineEnd.append(pt); // Pt1

                    plgLineEnd.append( i_line.p2() ); // Pt2

                    pt.setX( i_line.p2().x() - lineEndLength2dx(length) );
                    pt.setY( i_line.p2().y() - lineEndWidth2dy(width)/2.0 );
                    pt = rotatePoint( i_line.p2(), pt, fAngle_rad );
                    plgLineEnd.append(pt); // Pt3

                    if( baseLineType == ELineEndBaseLineType::Normal )
                    {
                        pt.setX( i_line.p2().x() - lineEndLength2dx(length) );
                        pt.setY( i_line.p2().y() );
                        pt = rotatePoint( i_line.p2(), pt, fAngle_rad );
                        plgLineEnd.append(pt); // Pt4
                    }
                    else if( baseLineType == ELineEndBaseLineType::Indented )
                    {
                        pt.setX( i_line.p2().x() - lineEndLength2dx(length)/2.0 );
                        pt.setY( i_line.p2().y() );
                        pt = rotatePoint( i_line.p2(), pt, fAngle_rad );
                        plgLineEnd.append(pt); // Pt4
                    }

                    *i_pplgLineEnd = plgLineEnd;

                } // if( i_pplgLineEnd != nullptr )

            } // if( iLinePoint == ELinePoint::End )

        } // for( iLinePoint = 0; iLinePoint < ELinePointCount; iLinePoint++ )

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
    o_fXLeftMin   = i_fXLeft1;
    o_fYTopMin    = i_fYTop1;
    o_fXRightMax  = i_fXRight1;
    o_fYBottomMax = i_fYBottom1;

    if( o_fXLeftMin > i_fXLeft2 )
    {
        o_fXLeftMin = i_fXLeft2;
    }
    if( o_fYTopMin > i_fYTop2 )
    {
        o_fYTopMin = i_fYTop2;
    }
    if( o_fXRightMax < i_fXRight2 )
    {
        o_fXRightMax = i_fXRight2;
    }
    if( o_fYBottomMax < i_fYBottom2 )
    {
        o_fYBottomMax = i_fYBottom2;
    }

} // getMaxRectArea

//------------------------------------------------------------------------------
void ZS::Draw::getMaxRectArea(
    const CPhysVal& i_physValLeft1,    const CPhysVal& i_physValTop1,
    const CPhysVal& i_physValRight1,   const CPhysVal& i_physValBottom1,
    const CPhysVal& i_physValLeft2,    const CPhysVal& i_physValTop2,
    const CPhysVal& i_physValRight2,   const CPhysVal& i_physValBottom2,
    CPhysVal&       o_physValLeftMax,  CPhysVal&       o_physValTopMax,
    CPhysVal&       o_physValRightMax, CPhysVal&       o_physValBottomMax )
//------------------------------------------------------------------------------
{
    o_physValLeftMax   = i_physValLeft1;
    o_physValTopMax    = i_physValTop1;
    o_physValRightMax  = i_physValRight1;
    o_physValBottomMax = i_physValBottom1;

    if( o_physValLeftMax > i_physValLeft2 )
    {
        o_physValLeftMax = i_physValLeft2;
    }
    if( o_physValTopMax > i_physValTop2 )
    {
        o_physValTopMax = i_physValTop2;
    }
    if( o_physValRightMax < i_physValRight2 )
    {
        o_physValRightMax = i_physValRight2;
    }
    if( o_physValBottomMax < i_physValBottom2 )
    {
        o_physValBottomMax = i_physValBottom2;
    }

} // getMaxRectArea

//------------------------------------------------------------------------------
QRectF ZS::Draw::resizeRect(
    const QRectF&              i_rect,
    const CEnumSelectionPoint& i_selPt,
    const QPointF&             i_ptSel,
    CEnumSelectionPoint*       o_pSelPt )
//------------------------------------------------------------------------------
{
    CEnumSelectionPoint selPt = i_selPt;

    QRectF rctResult = i_rect;

    double fXPos = i_ptSel.x();
    double fYPos = i_ptSel.y();

    switch( i_selPt.enumerator() )
    {
        case ESelectionPoint::TopLeft:
        {
            // Moved right and below right bottom corner ..
            if( fXPos > rctResult.right() && fYPos > rctResult.bottom() )
            {
                rctResult.setLeft(rctResult.right());
                rctResult.setRight(fXPos);
                rctResult.setTop(rctResult.bottom());
                rctResult.setBottom(fYPos);
                selPt = ESelectionPoint::BottomRight;
            }
            // Moved right of right border ..
            else if( fXPos > rctResult.right() )
            {
                rctResult.setLeft(rctResult.right());
                rctResult.setRight(fXPos);
                rctResult.setTop(fYPos);
                selPt = ESelectionPoint::TopRight;
            }
            // Moved below bottom border ..
            else if( fYPos > rctResult.bottom() )
            {
                rctResult.setLeft(fXPos);
                rctResult.setTop(rctResult.bottom());
                rctResult.setBottom(fYPos);
                selPt = ESelectionPoint::BottomLeft;
            }
            // If the bounding rectangle don't need to be twisted ...
            else
            {
                rctResult.setLeft(fXPos);
                rctResult.setTop(fYPos);
            }
            break;
        }

        case ESelectionPoint::BottomLeft:
        {
            // Moved right and above right top corner ..
            if( fXPos > rctResult.right() && fYPos < rctResult.top() )
            {
                rctResult.setLeft(rctResult.right());
                rctResult.setRight(fXPos);
                rctResult.setBottom(rctResult.top());
                rctResult.setTop(fYPos);
                selPt = ESelectionPoint::TopRight;
            }
            // Moved right of right border ..
            else if( fXPos > rctResult.right() )
            {
                rctResult.setLeft(rctResult.right());
                rctResult.setRight(fXPos);
                rctResult.setBottom(fYPos);
                selPt = ESelectionPoint::BottomRight;
            }
            // Moved above top border ..
            else if( fYPos < rctResult.top() )
            {
                rctResult.setLeft(fXPos);
                rctResult.setBottom(rctResult.top());
                rctResult.setTop(fYPos);
                selPt = ESelectionPoint::TopLeft;
            }
            // If the bounding rectangle don't need to be twisted ...
            else
            {
                rctResult.setLeft(fXPos);
                rctResult.setBottom(fYPos);
            }
            break;
        }

        case ESelectionPoint::TopRight:
        {
            // Moved left and below left bottom corner ..
            if( fXPos < rctResult.left() && fYPos > rctResult.bottom() )
            {
                rctResult.setRight(rctResult.left());
                rctResult.setLeft(fXPos);
                rctResult.setTop(rctResult.bottom());
                rctResult.setBottom(fYPos);
                selPt = ESelectionPoint::BottomLeft;
            }
            // Moved left of left border ..
            else if( fXPos < rctResult.left() )
            {
                rctResult.setRight(rctResult.left());
                rctResult.setLeft(fXPos);
                rctResult.setTop(fYPos);
                selPt = ESelectionPoint::TopLeft;
            }
            // Moved below bottom border ..
            else if( fYPos > rctResult.bottom() )
            {
                rctResult.setRight(fXPos);
                rctResult.setTop(rctResult.bottom());
                rctResult.setBottom(fYPos);
                selPt = ESelectionPoint::BottomRight;
            }
            // If the bounding rectangle don't need to be twisted ...
            else
            {
                rctResult.setRight(fXPos);
                rctResult.setTop(fYPos);
            }
            break;
        }

        case ESelectionPoint::BottomRight:
        {
            // Moved left and above left top corner ..
            if( fXPos < rctResult.left() && fYPos < rctResult.top() )
            {
                rctResult.setRight(rctResult.left());
                rctResult.setLeft(fXPos);
                rctResult.setBottom(rctResult.top());
                rctResult.setTop(fYPos);
                selPt = ESelectionPoint::TopLeft;
            }
            // Moved left of left border ..
            else if( fXPos < rctResult.left() )
            {
                rctResult.setRight(rctResult.left());
                rctResult.setLeft(fXPos);
                rctResult.setBottom(fYPos);
                selPt = ESelectionPoint::BottomLeft;
            }
            // Moved above top border ..
            else if( fYPos < rctResult.top() )
            {
                rctResult.setRight(fXPos);
                rctResult.setBottom(rctResult.top());
                rctResult.setTop(fYPos);
                selPt = ESelectionPoint::TopRight;
            }
            // If the bounding rectangle don't need to be twisted ...
            else
            {
                rctResult.setRight(fXPos);
                rctResult.setBottom(fYPos);
            }
            break;
        }

        case ESelectionPoint::LeftCenter:
        {
            // Moved right of right border ..
            if( fXPos > rctResult.right() )
            {
                rctResult.setLeft(rctResult.right());
                rctResult.setRight(fXPos);
                selPt = ESelectionPoint::RightCenter;
            }
            // If the bounding rectangle don't need to be twisted ...
            else
            {
                rctResult.setLeft(fXPos);
            }
            break;
        }

        case ESelectionPoint::RightCenter:
        {
            // Moved left of left border ..
            if( fXPos < rctResult.left() )
            {
                rctResult.setRight(rctResult.left());
                rctResult.setLeft(fXPos);
                selPt = ESelectionPoint::LeftCenter;
            }
            // If the bounding rectangle don't need to be twisted ...
            else
            {
                rctResult.setRight(fXPos);
            }
            break;
        }

        case ESelectionPoint::TopCenter:
        {
            // Moved below bottom border ..
            if( fYPos > rctResult.bottom() )
            {
                rctResult.setTop(rctResult.bottom());
                rctResult.setBottom(fYPos);
                selPt = ESelectionPoint::BottomCenter;
            }
            // If the bounding rectangle don't need to be twisted ...
            else
            {
                rctResult.setTop(fYPos);
            }
            break;
        }

        case ESelectionPoint::BottomCenter:
        {
            // Moved above top border ..
            if( fYPos < rctResult.top() )
            {
                rctResult.setBottom(rctResult.top());
                rctResult.setTop(fYPos);
                selPt = ESelectionPoint::TopCenter;
            }
            // If the bounding rectangle don't need to be twisted ...
            else
            {
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

    if( o_pSelPt != nullptr )
    {
        *o_pSelPt = selPt;
    }

    return rctResult;

} // resizeRect

//------------------------------------------------------------------------------
QPolygonF ZS::Draw::resizePolygon(
    const QPolygonF& i_plg,
    const QRectF&    i_rctBoundingNew,
    const QRectF&    i_rctBoundingOld )
//------------------------------------------------------------------------------
{
    QPolygonF plgResult = i_plg;

    QRectF rctBoundingOld = i_rctBoundingOld;

    if( !rctBoundingOld.isValid() )
    {
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

    if( fWOld > 0.0 )
    {
        fScaleX = fWNew / fWOld;
    }
    if( fHOld > 0.0 )
    {
        fScaleY = fHNew / fHOld;
    }

    QPointF pt;
    int     idxPt;
    double  fxdOld, fydOld;
    double  fxdNew, fydNew;

    for( idxPt = 0; idxPt < plgResult.size(); idxPt++ )
    {
        pt = plgResult[idxPt];

        fxdOld = pt.x() - fx0Old;
        fydOld = pt.y() - fy0Old;

        fxdNew = fScaleX * fxdOld;
        fydNew = fScaleY * fydOld;

        pt.setX( fx0New + fxdNew );
        pt.setY( fy0New + fydNew );

        plgResult[idxPt] = pt;
    }

    return plgResult;

} // resizePolygon

//------------------------------------------------------------------------------
QRectF ZS::Draw::boundingRect( const QPointF& i_pt, double i_fRadius )
//------------------------------------------------------------------------------
{
    return QRectF( i_pt.x()-i_fRadius,  i_pt.y()-i_fRadius, 2.0*i_fRadius, 2.0*i_fRadius );
}

//------------------------------------------------------------------------------
QPolygonF ZS::Draw::rect2Polygon( const QRectF& i_rct, int i_iSelPtsCount, const ESelectionPoint* i_pSelPts )
//------------------------------------------------------------------------------
{
    static const ESelectionPoint s_arSelPts[] = {
        ESelectionPoint::TopLeft,
        ESelectionPoint::TopRight,
        ESelectionPoint::BottomRight,
        ESelectionPoint::BottomLeft
    };

    const ESelectionPoint*  pSelPts = i_pSelPts;
    int                     iSelPtsCount = i_iSelPtsCount;

    if( pSelPts == nullptr )
    {
        iSelPtsCount = _ZSArrLen(s_arSelPts);
        pSelPts = s_arSelPts;
    }

    QPolygonF plg(iSelPtsCount);

    ESelectionPoint selPt;
    int             idxSelPt;

    for( idxSelPt = 0; idxSelPt < 4 && idxSelPt < plg.size(); idxSelPt++ )
    {
        selPt = pSelPts[idxSelPt];

        switch( selPt )
        {
            case ESelectionPoint::TopLeft:
            {
                plg[idxSelPt] = i_rct.topLeft();
                break;
            }
            case ESelectionPoint::TopRight:
            {
                plg[idxSelPt] = i_rct.topRight();
                break;
            }
            case ESelectionPoint::BottomRight:
            {
                plg[idxSelPt] = i_rct.bottomRight();
                break;
            }
            case ESelectionPoint::BottomLeft:
            {
                plg[idxSelPt] = i_rct.bottomLeft();
                break;
            }
            case ESelectionPoint::BottomCenter:
            {
                plg[idxSelPt] = getCenterPoint( QLineF(i_rct.bottomLeft(),i_rct.bottomRight()) );
                break;
            }
            case ESelectionPoint::TopCenter:
            {
                plg[idxSelPt] = getCenterPoint( QLineF(i_rct.topLeft(),i_rct.topRight()) );
                break;
            }
            case ESelectionPoint::RightCenter:
            {
                plg[idxSelPt] = getCenterPoint( QLineF(i_rct.topRight(),i_rct.bottomRight()) );
                break;
            }
            case ESelectionPoint::LeftCenter:
            {
                plg[idxSelPt] = getCenterPoint( QLineF(i_rct.topLeft(),i_rct.bottomLeft()) );
                break;
            }
            case ESelectionPoint::Center:
            {
                plg[idxSelPt] = i_rct.center();
                break;
            }
            case ESelectionPoint::RotateTop:
            case ESelectionPoint::RotateBottom:
            default:
            {
                break;
            }
        }
    }

    return plg;

} // rect2Polygon

//------------------------------------------------------------------------------
bool ZS::Draw::isLineHit( const QLineF& i_lineF, const QPointF& i_point, double i_fTolerance )
//------------------------------------------------------------------------------
{
    // We create a square with the SideLength = 2 * fTolerance.
    // For each side we are going to check whether the border line inteferes
    // with the given input line.

    bool   bIsHit = false;
    double fTolerance = i_fTolerance;

    if( fTolerance <= 0.0 )
    {
        fTolerance = 2.0;
    }

    double fXLeft   = i_point.x() - fTolerance;
    double fYTop    = i_point.y() - fTolerance;
    double fXRight  = i_point.x() + fTolerance;
    double fYBottom = i_point.y() + fTolerance;

    QRectF rctPt( QPointF(fXLeft,fYTop), QPointF(fXRight,fYBottom) );
    QLineF lineRctPt;
    int    idx;

    for( idx = 0; idx < 4; idx++ )
    {
        switch( idx )
        {
            // Rectangle line numbers surrounding the point to be tested:
            //        +-- 0 --+
            //        |       |
            //        3   X   1
            //        |       |
            //        +-- 2 --+
            case 0:
            {
                lineRctPt.setP1( QPointF(rctPt.left(),rctPt.top()) );
                lineRctPt.setP2( QPointF(rctPt.right(),rctPt.top()) );
                break;
            }
            case 1:
            {
                lineRctPt.setP1( QPointF(rctPt.right(),rctPt.top()) );
                lineRctPt.setP2( QPointF(rctPt.right(),rctPt.bottom()) );
                break;
            }
            case 2:
            {
                lineRctPt.setP1( QPointF(rctPt.left(),rctPt.bottom()) );
                lineRctPt.setP2( QPointF(rctPt.right(),rctPt.bottom()) );
                break;
            }
            case 3:
            {
                lineRctPt.setP1( QPointF(rctPt.left(),rctPt.top()) );
                lineRctPt.setP2( QPointF(rctPt.left(),rctPt.bottom()) );
                break;
            }
            default:
            {
                break;
            }
        }
        if (lineRctPt.intersects(i_lineF, nullptr) == QLineF::BoundedIntersection)
        {
            bIsHit = true;
            break;
        }
    }

    return bIsHit;

} // isLineHit

//------------------------------------------------------------------------------
bool ZS::Draw::isRectHit(
    const QRectF&     i_rct,
    EFillStyle        i_fillStyle,
    const QPointF&    i_pt,
    double            i_fTolerance,
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

    bool   bIsHit = false;
    double fTolerance = i_fTolerance;

    if( fTolerance <= 0.0 )
    {
        fTolerance = 2.0;
    }

    if( o_pHitInfo != nullptr )
    {
        o_pHitInfo->m_editMode = EEditMode::None;
        o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
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

    if( rctBndTmp.contains(i_pt) )
    {
        QPolygonF       plg;
        QRectF          rct;
        QLineF          lin;
        QPointF         pt;
        ESelectionPoint selPt;
        int             idxPt;

        plg = rect2Polygon( i_rct, _ZSArrLen(s_arSelPtsCorners), s_arSelPtsCorners );

        for( idxPt = 0; idxPt < plg.size(); idxPt++ )
        {
            selPt = s_arSelPtsCorners[idxPt];

            pt = plg[idxPt];

            rct = boundingRect(pt,fTolerance);

            if( rct.contains(i_pt) )
            {
                bIsHit = true;

                if( o_pHitInfo != nullptr )
                {
                    o_pHitInfo->m_editMode = selectionPoint2EditMode(selPt);
                    o_pHitInfo->m_editResizeMode = selectionPoint2EditResizeMode(selPt);
                    o_pHitInfo->m_selPtBoundingRect = selPt;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = pt;
                    o_pHitInfo->m_cursor = selectionPoint2CursorShape(selPt);
                }
                break;
            }
        }

        if( !bIsHit )
        {
            plg = rect2Polygon( i_rct, _ZSArrLen(s_arSelPtsLineCenters), s_arSelPtsLineCenters );

            for( idxPt = 0; idxPt < plg.size(); idxPt++ )
            {
                selPt = s_arSelPtsLineCenters[idxPt];

                pt = plg[idxPt];

                rct = boundingRect(pt,fTolerance);

                if( rct.contains(i_pt) )
                {
                    bIsHit = true;

                    if( o_pHitInfo != nullptr )
                    {
                        o_pHitInfo->m_editMode = selectionPoint2EditMode(selPt);
                        o_pHitInfo->m_editResizeMode = selectionPoint2EditResizeMode(selPt);
                        o_pHitInfo->m_selPtBoundingRect = selPt;
                        o_pHitInfo->m_idxPolygonShapePoint = -1;
                        o_pHitInfo->m_idxLineSegment = -1;
                        o_pHitInfo->m_ptSelected = pt;
                        o_pHitInfo->m_cursor = selectionPoint2CursorShape(selPt);
                    }
                    break;
                }
            }
        }

        if( !bIsHit )
        {
            plg = i_rct; // this cast return 5 points (left top corner twice)

            for( idxPt = 0; idxPt < plg.size()-1; idxPt++ )
            {
                lin.setP1(plg[idxPt]);
                lin.setP2(plg[idxPt+1]);

                if( isLineHit(lin,i_pt,fTolerance) )
                {
                    bIsHit = true;

                    if( o_pHitInfo != nullptr )
                    {
                        o_pHitInfo->m_editMode = EEditMode::Move;
                        o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
                        o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
                        o_pHitInfo->m_idxPolygonShapePoint = -1;
                        o_pHitInfo->m_idxLineSegment = idxPt;
                        o_pHitInfo->m_ptSelected = i_pt;
                        o_pHitInfo->m_cursor = Qt::SizeAllCursor;
                    }
                    break;
                }
            }
        }

        if( !bIsHit )
        {
            if( i_fillStyle == EFillStyle::SolidPattern )
            {
                bIsHit = true;

                if( o_pHitInfo != nullptr )
                {
                    o_pHitInfo->m_editMode = EEditMode::Move;
                    o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
                    o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = i_pt;
                    o_pHitInfo->m_cursor = Qt::SizeAllCursor;
                }
            }
        }

    } // if( rctBndTmp.contains(i_pt) )

    return bIsHit;

} // isRectHit

//------------------------------------------------------------------------------
bool ZS::Draw::isEllipseHit(
    const QRectF&     i_rct,
    EFillStyle        i_fillStyle,
    const QPointF&    i_pt,
    double            i_fTolerance,
    SGraphObjHitInfo* o_pHitInfo )
//------------------------------------------------------------------------------
{
    static const ESelectionPoint s_arSelPtsLineCenters[] = {
        ESelectionPoint::BottomCenter,
        ESelectionPoint::RightCenter,
        ESelectionPoint::LeftCenter,
        ESelectionPoint::TopCenter
    };

    bool   bIsHit = false;
    double fTolerance = i_fTolerance;

    if( fTolerance <= 0.0 )
    {
        fTolerance = 2.0;
    }

    if( o_pHitInfo != nullptr )
    {
        o_pHitInfo->m_editMode = EEditMode::None;
        o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
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

    if( rctBndTmp.contains(i_pt) )
    {
        QPointF ptC = i_rct.center();

        QPolygonF       plg;
        QRectF          rct;
        QPointF         pt;
        ESelectionPoint selPt;
        int             idxPt;

        plg = rect2Polygon( i_rct, _ZSArrLen(s_arSelPtsLineCenters), s_arSelPtsLineCenters );

        for( idxPt = 0; idxPt < plg.size(); idxPt++ )
        {
            selPt = s_arSelPtsLineCenters[idxPt];

            pt = plg[idxPt];

            rct = boundingRect(pt,fTolerance);

            if( rct.contains(i_pt) )
            {
                bIsHit = true;

                if( o_pHitInfo != nullptr )
                {
                    o_pHitInfo->m_editMode = selectionPoint2EditMode(selPt);
                    o_pHitInfo->m_editResizeMode = selectionPoint2EditResizeMode(selPt);
                    o_pHitInfo->m_selPtBoundingRect = selPt;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = pt;
                    o_pHitInfo->m_cursor = selectionPoint2CursorShape(selPt);
                }
                break;
            }
        }

        if( !bIsHit )
        {
            double fa = i_rct.width() / 2.0;
            double fb = i_rct.height() / 2.0;
            double fx = i_pt.x() - ptC.x();
            double fy = i_pt.y() - ptC.y();

            // Ellipse as vertical line ..
            if( fa == 0.0 )
            {
                if( fabs(fx) <= fTolerance )
                {
                    bIsHit = true;

                    if( o_pHitInfo != nullptr )
                    {
                        selPt = ESelectionPoint::RightCenter;
                        o_pHitInfo->m_editMode = EEditMode::Resize;
                        o_pHitInfo->m_editResizeMode = EEditResizeMode::ResizeHor;
                        o_pHitInfo->m_selPtBoundingRect = selPt;
                        o_pHitInfo->m_idxPolygonShapePoint = -1;
                        o_pHitInfo->m_idxLineSegment = -1;
                        o_pHitInfo->m_ptSelected = pt;
                        o_pHitInfo->m_cursor = selectionPoint2CursorShape(selPt);
                    }
                }
            } // if( fa == 0.0 )

            // Ellipse as horizontal line ..
            else if( fb == 0.0 )
            {
                if( fabs(fy) <= fTolerance )
                {
                    bIsHit = true;

                    if( o_pHitInfo != nullptr )
                    {
                        selPt = ESelectionPoint::BottomCenter;
                        o_pHitInfo->m_editMode = EEditMode::Resize;
                        o_pHitInfo->m_editResizeMode = EEditResizeMode::ResizeVer;
                        o_pHitInfo->m_selPtBoundingRect = selPt;
                        o_pHitInfo->m_idxPolygonShapePoint = -1;
                        o_pHitInfo->m_idxLineSegment = -1;
                        o_pHitInfo->m_ptSelected = pt;
                        o_pHitInfo->m_cursor = selectionPoint2CursorShape(selPt);
                    }
                }
            } // if( fb == 0.0 )

            // Ellipse is a circle ..
            else if( fa == fb )
            {
                // Circle equation:
                //   x² + y² = r²
                //   y = sqrt(r² - x²)

                double fyTmp = sqrt( Math::sqr(fa) - Math::sqr(fx) );

                if( (fy + fTolerance >= fyTmp) && (fy - fTolerance <= fyTmp) )
                {
                    bIsHit = true;
                }
                else if( (fy + fTolerance >= -fyTmp) && (fy - fTolerance <= -fyTmp) )
                {
                    bIsHit = true;
                }
                if( bIsHit && o_pHitInfo != nullptr )
                {
                    o_pHitInfo->m_editMode = EEditMode::Move;
                    o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
                    o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = pt;
                    o_pHitInfo->m_cursor = Qt::SizeAllCursor;
                }
            } // if( fa == fb )

            else // if( fa != 0.0 && fb != 0.0 && fa != fb )
            {
                // Ellipse equation:
                //   x²/a² + y²/b² = 1
                //   y²/b² = 1 - x²/a²
                //   y² = b² * (1 - x²/a²)
                //   y = b * sqrt(1 - x²/a²)

                if( fx == fa )
                {
                    if( fabs(fy) <= fTolerance )
                    {
                        bIsHit = true;
                    }
                }
                else if( fy == fb )
                {
                    if( fabs(fx) <= fTolerance )
                    {
                        bIsHit = true;
                    }
                }
                else
                {
                    double fyTmp = fb * sqrt( 1.0 - Math::sqr(fx/fa) );

                    if( (fy + fTolerance >= fyTmp) && (fy - fTolerance <= fyTmp) )
                    {
                        bIsHit = true;
                    }
                    else if( (fy + fTolerance >= -fyTmp) && (fy - fTolerance <= -fyTmp) )
                    {
                        bIsHit = true;
                    }
                }

                if( bIsHit && o_pHitInfo != nullptr )
                {
                    o_pHitInfo->m_editMode = EEditMode::Move;
                    o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
                    o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = pt;
                    o_pHitInfo->m_cursor = Qt::SizeAllCursor;
                }
            } // if( fa != 0.0 && fb != 0.0 && fa != fb )

        } // if( !bIsHit )

        if( !bIsHit )
        {
            if( i_fillStyle == EFillStyle::SolidPattern )
            {
                bIsHit = true;

                if( o_pHitInfo != nullptr )
                {
                    o_pHitInfo->m_editMode = EEditMode::Move;
                    o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
                    o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = i_pt;
                    o_pHitInfo->m_cursor = Qt::SizeAllCursor;
                }
            }
        }

    } // if( rctBndTmp.contains(i_pt) )

    return bIsHit;

} // isEllipseHit

//------------------------------------------------------------------------------
bool ZS::Draw::isPolygonHit(
    const QPolygonF&  i_plg,
    EFillStyle        i_fillStyle,
    const QPointF&    i_pt,
    double            i_fTolerance,
    SGraphObjHitInfo* o_pHitInfo )
//------------------------------------------------------------------------------
{
    bool   bIsHit = false;
    double fTolerance = i_fTolerance;

    if( fTolerance <= 0.0 )
    {
        fTolerance = 2.0;
    }

    if( o_pHitInfo != nullptr )
    {
        o_pHitInfo->m_editMode = EEditMode::None;
        o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
        o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
        o_pHitInfo->m_idxPolygonShapePoint = -1;
        o_pHitInfo->m_idxLineSegment = -1;
        o_pHitInfo->m_ptSelected = QPointF();
        o_pHitInfo->m_cursor = Qt::ArrowCursor;
    }

    QRectF rctBndTmp = i_plg.boundingRect();

    rctBndTmp.setLeft( rctBndTmp.left() - fTolerance );
    rctBndTmp.setTop( rctBndTmp.top() - fTolerance );
    rctBndTmp.setRight( rctBndTmp.right() + fTolerance );
    rctBndTmp.setBottom( rctBndTmp.bottom() + fTolerance );

    if( rctBndTmp.contains(i_pt) )
    {
        QRectF  rct;
        QLineF  lin;
        QPointF pt;
        int     idxPt;

        for( idxPt = 0; idxPt < i_plg.size(); idxPt++ )
        {
            pt = i_plg[idxPt];

            rct = boundingRect(pt,fTolerance);

            if( rct.contains(i_pt) )
            {
                bIsHit = true;

                if( o_pHitInfo != nullptr )
                {
                    o_pHitInfo->m_editMode = EEditMode::MoveShapePoint;
                    o_pHitInfo->m_idxPolygonShapePoint = idxPt;
                    o_pHitInfo->m_ptSelected = pt;
                    o_pHitInfo->m_cursor = Qt::CrossCursor;
                }
                break;
            }
        }

        if( !bIsHit )
        {
            if( i_plg.size() > 1 )
            {
                for( idxPt = 0; idxPt < i_plg.size()-1; idxPt++ )
                {
                    lin.setP1(i_plg[idxPt]);
                    lin.setP2(i_plg[idxPt+1]);

                    if( isLineHit(lin,i_pt,fTolerance) )
                    {
                        bIsHit = true;

                        if( o_pHitInfo != nullptr )
                        {
                            o_pHitInfo->m_editMode = EEditMode::Move;
                            o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
                            o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
                            o_pHitInfo->m_idxPolygonShapePoint = -1;
                            o_pHitInfo->m_idxLineSegment = idxPt;
                            o_pHitInfo->m_ptSelected = i_pt;
                            o_pHitInfo->m_cursor = Qt::SizeAllCursor;
                        }
                        break;
                    }
                }
            }
        }

        if( !bIsHit )
        {
            if( i_fillStyle == EFillStyle::SolidPattern )
            {
                bIsHit = true;

                if( o_pHitInfo != nullptr )
                {
                    o_pHitInfo->m_editMode = EEditMode::Move;
                    o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
                    o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = i_pt;
                    o_pHitInfo->m_cursor = Qt::SizeAllCursor;
                }
            }
        }

    } // if( rctBndTmp.contains(i_pt) )

    return bIsHit;

} // isPolygonHit

//------------------------------------------------------------------------------
double ZS::Draw::getDist( const QPointF& i_pt1, const QPointF& i_pt2 )
//------------------------------------------------------------------------------
{
    double fXDistPow_px = pow( i_pt2.x() - i_pt1.x(), 2.0 );
    double fYDistPow_px = pow( i_pt2.y() - i_pt1.y(), 2.0 );
    double fRadius_px   = sqrt( fXDistPow_px + fYDistPow_px );
    return fRadius_px;

} // getDist

//------------------------------------------------------------------------------
double ZS::Draw::getAngleRad( const QPointF& i_pt1, const QPointF& i_pt2 )
//------------------------------------------------------------------------------
{
    // Calculates the angle between point 2 and point 1.
    // Point 1 is considered to be the origin of the coordinate system.

    /*
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

    double fAngle_rad = 0.0;

    // If both points are at the same y position (horizontal line) ...
    if( i_pt1.y() == i_pt2.y() )
    {
        // If its a line from right to left ..
        if( i_pt2.x() < i_pt1.x() )
        {
            fAngle_rad = ZS::System::Math::c_fPI;
        }
    }
    else
    {
        double fWidth_px  = i_pt2.x() - i_pt1.x();
        double fRadius_px = getDist(i_pt1,i_pt2);

        fAngle_rad = acos(fWidth_px/fRadius_px);

        if( i_pt2.y() > i_pt1.y() )
        {
            fAngle_rad *= -1.0;
        }
    }

    return fAngle_rad;

} // getAngleRad

//------------------------------------------------------------------------------
QPointF ZS::Draw::rotatePoint( const QPointF& i_ptCenter, const QPointF& i_pt, double i_fAngle_rad )
//------------------------------------------------------------------------------
{
    QPointF ptRes = i_pt;

    if( i_fAngle_rad != 0.0 )
    {
        double fRadius_px     = getDist(i_ptCenter,i_pt);
        double fAnglePt_rad   = getAngleRad(i_ptCenter,i_pt);
        double fAngleRes_rad  = fAnglePt_rad + i_fAngle_rad;

        ptRes.setX( i_ptCenter.x() + fRadius_px * cos(fAngleRes_rad) );
        ptRes.setY( i_ptCenter.y() - fRadius_px * sin(fAngleRes_rad) );
    }

    return ptRes;

} // rotatePoint

//------------------------------------------------------------------------------
QPolygonF ZS::Draw::rotateRect( const QPointF& i_ptCenter, const QRectF& i_rct, double i_fAngle_rad )
//------------------------------------------------------------------------------
{
    QPolygonF polygonRect;

    polygonRect.resize(4);

    polygonRect[0] = rotatePoint( i_ptCenter, i_rct.topLeft(), i_fAngle_rad );
    polygonRect[1] = rotatePoint( i_ptCenter, i_rct.topRight(), i_fAngle_rad );
    polygonRect[2] = rotatePoint( i_ptCenter, i_rct.bottomRight(), i_fAngle_rad );
    polygonRect[3] = rotatePoint( i_ptCenter, i_rct.bottomLeft(), i_fAngle_rad );

    return polygonRect;

} // rotateRect

//------------------------------------------------------------------------------
QPolygonF ZS::Draw::rotatePolygon( const QPointF& i_ptCenter, const QPolygonF& i_polygon, double i_fAngle_rad )
//------------------------------------------------------------------------------
{
    QPolygonF polygon;

    if( i_polygon.size() > 0 )
    {
        polygon.resize(i_polygon.size());

        int idxPt;

        for( idxPt = 0; idxPt < i_polygon.size(); idxPt++ )
        {
            if( i_fAngle_rad != 0.0 )
            {
                polygon[idxPt] = rotatePoint( i_ptCenter, i_polygon[idxPt], i_fAngle_rad );
            }
            else
            {
                polygon[idxPt] = i_polygon[idxPt];
            }
        }
    }
    return polygon;

} // rotatePolygon

//------------------------------------------------------------------------------
QPolygonF ZS::Draw::normalizePolygon( const QPolygonF& i_plg, int i_iPrecision )
//------------------------------------------------------------------------------
{
    QPolygonF plg = i_plg;

    int idxPt;

    // Remove "identical" points:
    if( plg.size() > 2 )
    {
        QPointF pt1;
        QPointF pt2;
        QString strPt1;
        QString strPt2;

        for( idxPt = plg.size()-1; idxPt >= 2; idxPt-- )
        {
            pt1 = plg[idxPt];
            pt2 = plg[idxPt-1];

            strPt1 = point2Str(pt1,'f',0);
            strPt2 = point2Str(pt2,'f',0);

            if( strPt1 == strPt2 )
            {
                plg.remove(idxPt);
            }
        }
    }

    // Remove points on direct lines between two neighbor points:
    if( plg.size() > 3 )
    {
        bool    bPtRemoved = true;
        QLineF  ln;
        QPointF ptCheck;

        while( bPtRemoved )
        {
            bPtRemoved = false;

            for( idxPt = plg.size()-1; idxPt >= 2; idxPt-- )
            {
                ln = QLineF( plg[idxPt], plg[idxPt-2] );
                ptCheck = plg[idxPt-1];

                if( isLineHit(ln,ptCheck,i_iPrecision) )
                {
                    plg.remove(idxPt-1);

                    bPtRemoved = true;
                    break;

                } // if( isLineHit() )

            } // for( idxPt = plg.size()-1; idxPt >= 3; idxPt-- )

        } // while( bNormalizedTmp )

    } // if( plg.size() > 3 )

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
//        double fDistPtRotate2TopCenter = getDist(i_polygonRectSelectionPoints[ESelectionPointRotate],i_polygonRectSelectionPoints[ESelectionPoint::TopCenter]);
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
QPointF ZS::Draw::getCenterPoint( const QLineF& i_line )
//------------------------------------------------------------------------------
{
    QPointF ptCenter;

    double fXLeft   = i_line.p1().x();
    double fYTop    = i_line.p1().y();
    double fXRight  = i_line.p2().x();
    double fYBottom = i_line.p2().y();

    if( fXLeft > fXRight )
    {
        double fXLeftTmp = fXLeft;
        fXLeft = fXRight;
        fXRight = fXLeftTmp;
    }
    if( fYTop > fYBottom )
    {
        double fYTopTmp = fYTop;
        fYTop = fYBottom;
        fYBottom = fYTopTmp;
    }

    ptCenter.setX( fXLeft + (fXRight-fXLeft)/2.0 );
    ptCenter.setY( fYTop + (fYBottom-fYTop)/2.0 );

    return ptCenter;

} // getCenterPoint

//------------------------------------------------------------------------------
QPointF ZS::Draw::getCenterPointOfPolygonRect( const QPolygonF& i_polygonRect )
//------------------------------------------------------------------------------
{
    QPointF ptCenter;

    if( i_polygonRect.size() == 4 )
    {
        double fXLeft   = i_polygonRect[0].x();
        double fYTop    = i_polygonRect[0].y();
        double fXRight  = i_polygonRect[0].x();
        double fYBottom = i_polygonRect[0].y();
        int    idxPt;

        for( idxPt = 1; idxPt < i_polygonRect.size(); idxPt++ )
        {
            if( i_polygonRect[idxPt].x() < fXLeft )
            {
                fXLeft = i_polygonRect[idxPt].x();
            }
            if( i_polygonRect[idxPt].y() < fYTop )
            {
                fYTop = i_polygonRect[idxPt].y();
            }
            if( i_polygonRect[idxPt].x() > fXRight )
            {
                fXRight = i_polygonRect[idxPt].x();
            }
            if( i_polygonRect[idxPt].y() > fYBottom )
            {
                fYBottom = i_polygonRect[idxPt].y();
            }
        }

        if( fXLeft > fXRight )
        {
            double fXLeftTmp = fXLeft;
            fXLeft = fXRight;
            fXRight = fXLeftTmp;
        }
        if( fYTop > fYBottom )
        {
            double fYTopTmp = fYTop;
            fYTop = fYBottom;
            fYBottom = fYTopTmp;
        }

        ptCenter.setX( fXLeft + (fXRight-fXLeft)/2.0 );
        ptCenter.setY( fYTop + (fYBottom-fYTop)/2.0 );

    } // if( i_polygonRect.size() == 4 )

    return ptCenter;

} // getCenterPointOfPolygonRect

//------------------------------------------------------------------------------
QPointF ZS::Draw::getMassCenterPointOfPolygon( const QPolygonF& i_polygon )
//------------------------------------------------------------------------------
{
    QPointF ptCenter;

    if( i_polygon.size() == 1 )
    {
        ptCenter = i_polygon[0];
    }
    else if( i_polygon.size() == 2 )
    {
        ptCenter.setX( (i_polygon[0].x() + i_polygon[1].x()) / 2.0 );
        ptCenter.setY( (i_polygon[0].y() + i_polygon[1].y()) / 2.0 );
    }
    else // if( i_polygon.size() > 2 )
    {
        int    idxPt, idxPt2;
        double ai, atmp = 0, xtmp = 0, ytmp = 0;

        for( idxPt = i_polygon.size()-1, idxPt2 = 0; idxPt2 < i_polygon.size(); idxPt = idxPt2, idxPt2++ )
        {
            ai = i_polygon[idxPt].x() * i_polygon[idxPt2].y() - i_polygon[idxPt2].x() * i_polygon[idxPt].y();
            atmp += ai;
            xtmp += (i_polygon[idxPt2].x() + i_polygon[idxPt].x()) * ai;
            ytmp += (i_polygon[idxPt2].y() + i_polygon[idxPt].y()) * ai;
        }

        if( atmp != 0.0 )
        {
            ptCenter.setX( xtmp / (3.0 * atmp) );
            ptCenter.setY( ytmp / (3.0 * atmp) );
        }
        else
        {
            ptCenter = i_polygon[0];
        }
    }

    return ptCenter;

} // getMassCenterPointOfPolygon

//------------------------------------------------------------------------------
QPolygonF ZS::Draw::getBoundingRectPolygon( const QPolygonF& i_polygon )
//------------------------------------------------------------------------------
{
    QPolygonF polygonBoundingRect;

    polygonBoundingRect.resize(4);

    if( i_polygon.size() > 0 )
    {
        double fXLeft   = i_polygon[0].x();
        double fYTop    = i_polygon[0].y();
        double fXRight  = i_polygon[0].x();
        double fYBottom = i_polygon[0].y();
        int    idxPt;

        for( idxPt = 1; idxPt < i_polygon.size(); idxPt++ )
        {
            if( i_polygon[idxPt].x() < fXLeft )
            {
                fXLeft = i_polygon[idxPt].x();
            }
            if( i_polygon[idxPt].y() < fYTop )
            {
                fYTop = i_polygon[idxPt].y();
            }
            if( i_polygon[idxPt].x() > fXRight )
            {
                fXRight = i_polygon[idxPt].x();
            }
            if( i_polygon[idxPt].y() > fYBottom )
            {
                fYBottom = i_polygon[idxPt].y();
            }
        }

        if( fXLeft > fXRight )
        {
            double fXLeftTmp = fXLeft;
            fXLeft = fXRight;
            fXRight = fXLeftTmp;
        }
        if( fYTop > fYBottom )
        {
            double fYTopTmp = fYTop;
            fYTop = fYBottom;
            fYBottom = fYTopTmp;
        }

        polygonBoundingRect[0] = QPointF(fXLeft,fYTop);
        polygonBoundingRect[1] = QPointF(fXRight,fYTop);
        polygonBoundingRect[2] = QPointF(fXRight,fYBottom);
        polygonBoundingRect[3] = QPointF(fXLeft,fYBottom);

    } // if( i_polygon.size() > 0 )

    return polygonBoundingRect;

} // getBoundingRectPolygon

//------------------------------------------------------------------------------
QString ZS::Draw::point2Str( const QPoint& i_pt )
//------------------------------------------------------------------------------
{
    QString str;

    str = QString::number(i_pt.x()) + "/" + QString::number(i_pt.y());

    return str;

} // point2Str

//------------------------------------------------------------------------------
QString ZS::Draw::point2Str( const QPointF& i_pt, char i_cF, int i_iPrecision, bool i_bRound2Nearest )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_bRound2Nearest && i_iPrecision > 0 )
    {
        str = QString::number(Math::round2Nearest(i_pt.x(),1),i_cF,i_iPrecision) + "/" + QString::number(Math::round2Nearest(i_pt.y(),1),i_cF,i_iPrecision);
    }
    else
    {
        str = QString::number(i_pt.x(),i_cF,i_iPrecision) + "/" + QString::number(i_pt.y(),i_cF,i_iPrecision);
    }

    return str;

} // point2Str

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
    QString str;

    str = QString::number(i_size.width()) + "/" + QString::number(i_size.height());

    return str;

} // size2Str

//------------------------------------------------------------------------------
QString ZS::Draw::size2Str( const QSizeF& i_size, char i_cF, int i_iPrecision, bool i_bRound2Nearest )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_bRound2Nearest && i_iPrecision > 0 )
    {
        str = QString::number(Math::round2Nearest(i_size.width(),1),i_cF,i_iPrecision) + "/" + QString::number(Math::round2Nearest(i_size.height(),1),i_cF,i_iPrecision);
    }
    else
    {
        str = QString::number(i_size.width(),i_cF,i_iPrecision) + "/" + QString::number(i_size.height(),i_cF,i_iPrecision);
    }

    return str;

} // size2Str

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
    QString str;

    str = point2Str(i_line.p1()) + ", " + point2Str(i_line.p2());

    return str;

} // line2Str

//------------------------------------------------------------------------------
QString ZS::Draw::line2Str( const QLineF& i_line, char i_cF, int i_iPrecision, bool i_bRound2Nearest )
//------------------------------------------------------------------------------
{
    QString str;

    str = point2Str(i_line.p1(),i_cF,i_iPrecision,i_bRound2Nearest) + ", " + point2Str(i_line.p2(),i_cF,i_iPrecision,i_bRound2Nearest);

    return str;

} // line2Str

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
    QString str;

    str  = point2Str(i_rct.topLeft()) + ", ";
    str += point2Str(i_rct.topRight()) + ", ";
    str += point2Str(i_rct.bottomRight()) + ", ";
    str += point2Str(i_rct.bottomLeft());

    if( i_bAddWidthAndHeight )
    {
        str += ", " + size2Str(i_rct.size());
    }

    return str;

} // rect2Str

//------------------------------------------------------------------------------
QString ZS::Draw::rect2Str( const QRectF& i_rct, char i_cF, int i_iPrecision, bool i_bRound2Nearest, bool i_bAddWidthAndHeight )
//------------------------------------------------------------------------------
{
    QString str;

    str  = point2Str(i_rct.topLeft(), i_cF, i_iPrecision, i_bRound2Nearest) + ", ";
    str += point2Str(i_rct.topRight(), i_cF, i_iPrecision, i_bRound2Nearest) + ", ";
    str += point2Str(i_rct.bottomRight(), i_cF, i_iPrecision, i_bRound2Nearest) + ", ";
    str += point2Str(i_rct.bottomLeft(), i_cF, i_iPrecision, i_bRound2Nearest);

    if( i_bAddWidthAndHeight )
    {
        str += ", " + size2Str(i_rct.size(), i_cF, i_iPrecision, i_bRound2Nearest);
    }

    return str;

} // rect2Str

//------------------------------------------------------------------------------
QString ZS::Draw::rect2Str( const QPolygon& i_rct, bool i_bAddWidthAndHeight )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_rct.size() >= 4 )
    {
        int idxPt;

        for( idxPt = 0; idxPt < i_rct.size() && idxPt < 4; idxPt++ )
        {
            str += point2Str(i_rct[idxPt]);

            if( idxPt < 3 )
            {
                str += ", ";
            }
        }

        if( i_bAddWidthAndHeight )
        {
            double w = getDist(i_rct[0],i_rct[1]);
            double h = getDist(i_rct[1],i_rct[2]);
            QSize  siz( static_cast<int>(w), static_cast<int>(h) );

            str += ", " + size2Str(siz);
        }
    }

    return str;

} // rect2Str

//------------------------------------------------------------------------------
QString ZS::Draw::rect2Str( const QPolygonF& i_rct, char i_cF, int i_iPrecision, bool i_bRound2Nearest, bool i_bAddWidthAndHeight )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_rct.size() >= 4 )
    {
        int idxPt;

        for( idxPt = 0; idxPt < i_rct.size() && idxPt < 4; idxPt++ )
        {
            str += point2Str(i_rct[idxPt],i_cF,i_iPrecision,i_bRound2Nearest);

            if( idxPt < 3 )
            {
                str += ", ";
            }
        }

        if( i_bAddWidthAndHeight )
        {
            double w = getDist(i_rct[0],i_rct[1]);
            double h = getDist(i_rct[1],i_rct[2]);
            QSize  siz( static_cast<int>(w), static_cast<int>(h) );

            str += ", " + size2Str(siz);
        }
    }

    return str;

} // rect2Str

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
    int     idxPt;

    for( idxPt = 0; idxPt < i_polygon.size(); idxPt++ )
    {
        str += point2Str(i_polygon[idxPt]);

        if( idxPt < i_polygon.size()-1 )
        {
            str += ", ";
        }
    }

    return str;

} // polygon2Str

//------------------------------------------------------------------------------
QString ZS::Draw::polygon2Str( const QPolygonF& i_polygon, char i_cF, int i_iPrecision, bool i_bRound2Nearest )
//------------------------------------------------------------------------------
{
    QString str;
    int     idxPt;

    for( idxPt = 0; idxPt < i_polygon.size(); idxPt++ )
    {
        str += point2Str(i_polygon[idxPt],i_cF,i_iPrecision,i_bRound2Nearest);

        if( idxPt < i_polygon.size()-1 )
        {
            str += ", ";
        }
    }

    return str;

} // polygon2Str

//------------------------------------------------------------------------------
QPolygon ZS::Draw::str2Polygon( const QString& i_str, bool* i_pbConverted )
//------------------------------------------------------------------------------
{
    bool     bConverted = false;
    QPolygon plg;

    QStringList strlst = i_str.split(",");
    int         idxPt;

    for( idxPt = 0; idxPt < strlst.size(); idxPt++ )
    {
        QString strPt = strlst[idxPt];
        QPoint  pt    = str2Point(strPt,&bConverted);

        if( !bConverted )
        {
            break;
        }

        plg.append(pt);
    }

    if( i_pbConverted != nullptr )
    {
        *i_pbConverted = bConverted;
    }

    return plg;

} // str2Polygon

//------------------------------------------------------------------------------
QPolygonF ZS::Draw::str2PolygonF( const QString& i_str, bool* i_pbConverted )
//------------------------------------------------------------------------------
{
    bool      bConverted = false;
    QPolygonF plg;

    QStringList strlst = i_str.split(",");
    int         idxPt;

    for( idxPt = 0; idxPt < strlst.size(); idxPt++ )
    {
        QString strPt = strlst[idxPt];
        QPointF pt    = str2PointF(strPt,&bConverted);

        if( !bConverted )
        {
            break;
        }

        plg.append(pt);
    }

    if( i_pbConverted != nullptr )
    {
        *i_pbConverted = bConverted;
    }

    return plg;

} // str2PolygonF

//------------------------------------------------------------------------------
QPolygonF ZS::Draw::getEllipseFocusPoints( const QRectF& i_rct )
//------------------------------------------------------------------------------
{
    QPolygonF plgFocusPoints(2);

    QPointF ptC = i_rct.center();

    if( i_rct.width() == i_rct.height() )
    {
        plgFocusPoints[0] = ptC;
        plgFocusPoints[1] = ptC;
    }
    else if( i_rct.width() > i_rct.height() )
    {
        double fe = getEllipseCenterFocusDist(i_rct);
        plgFocusPoints[0] = QPointF( ptC.x() - fe, ptC.y() );
        plgFocusPoints[1] = QPointF( ptC.x() + fe, ptC.y() );
    }
    else // if( i_rct.width() < i_rct.height() )
    {
        double fe = getEllipseCenterFocusDist(i_rct);
        plgFocusPoints[0] = QPointF( ptC.x(), ptC.y() - fe );
        plgFocusPoints[1] = QPointF( ptC.x(), ptC.y() + fe );
    }
    return plgFocusPoints;

} // getEllipseFocusPoints

//------------------------------------------------------------------------------
double ZS::Draw::getEllipseCenterFocusDist( const QRectF& i_rct )
//------------------------------------------------------------------------------
{
    double fa = i_rct.width();
    double fb = i_rct.height();

    if( fa < fb )
    {
        double fTmp = fb;
        fb = fa;
        fa = fTmp;
    }

    double fe = sqrt( Math::sqr(fa) - Math::sqr(fb) );

    return fe;

} // getEllipseCenterFocusDist
