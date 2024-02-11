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

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjGroup.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabelGeometryAngle.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabelGeometryDX.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabelGeometryDY.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabelGeometryLength.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabelGeometryPosition.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Widgets/GraphObjFormat/ZSDrawDlgFormatGraphObjs.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysIdxTree.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtGui/QBitmap>
#include <QtGui/QPainter>

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::Draw;


/*******************************************************************************
struct SGraphObjSelectionPoint
*******************************************************************************/

/* public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
SGraphObjSelectionPoint SGraphObjSelectionPoint::fromString(
    const QString& i_str, bool* i_pbOk)
//------------------------------------------------------------------------------
{
    bool bOk = false;
    SGraphObjSelectionPoint selPt;
    QStringList strlst = i_str.split(",");
    if (strlst.size() == 2) {
        strlst[0]  = strlst[0].trimmed();
        strlst[1]  = strlst[1].trimmed();
        CEnumSelectionPointType selPtType = CEnumSelectionPointType::fromString(strlst[0], &bOk);
        if (bOk) {
            selPt.m_selPtType = selPtType.enumerator();
            if (selPtType == ESelectionPointType::BoundingRectangle) {
                CEnumSelectionPoint selPtTmp = CEnumSelectionPoint::fromString(strlst[1], &bOk);
                if (bOk) {
                    selPt.m_selPt = selPtTmp.enumerator();
                }
            }
            else {
                int idxPt = strlst[1].toInt(&bOk);
                if (bOk) {
                    selPt.m_idxPt = idxPt;
                }
            }
        }
    }
    if (i_pbOk != nullptr) {
        *i_pbOk = bOk;
    }
    return selPt;
}

//------------------------------------------------------------------------------
SGraphObjSelectionPoint SGraphObjSelectionPoint::fromString(
    CGraphObj* i_pGraphObj, const QString& i_str, bool* i_pbOk)
//------------------------------------------------------------------------------
{
    bool bOk = false;
    SGraphObjSelectionPoint selPt = fromString(i_str, &bOk);
    selPt.m_pGraphObj = i_pGraphObj;
    if (i_pbOk != nullptr) {
        *i_pbOk = bOk;
    }
    return selPt;
}

/* public: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
SGraphObjSelectionPoint::SGraphObjSelectionPoint() :
//------------------------------------------------------------------------------
    m_pGraphObj(nullptr),
    m_selPtType(ESelectionPointType::Undefined),
    m_selPt(ESelectionPoint::None),
    m_idxPt(-1)
{
}

//------------------------------------------------------------------------------
SGraphObjSelectionPoint::SGraphObjSelectionPoint(CGraphObj* i_pGraphObj) :
//------------------------------------------------------------------------------
    m_pGraphObj(i_pGraphObj),
    m_selPtType(ESelectionPointType::Undefined),
    m_selPt(ESelectionPoint::None),
    m_idxPt(-1)
{
}

//------------------------------------------------------------------------------
SGraphObjSelectionPoint::SGraphObjSelectionPoint(CGraphObj* i_pGraphObj, ESelectionPoint i_selPt) :
//------------------------------------------------------------------------------
    m_pGraphObj(i_pGraphObj),
    m_selPtType(ESelectionPointType::BoundingRectangle),
    m_selPt(i_selPt),
    m_idxPt(-1)
{
}

//------------------------------------------------------------------------------
SGraphObjSelectionPoint::SGraphObjSelectionPoint(CGraphObj* i_pGraphObj, int idxPt) :
//------------------------------------------------------------------------------
    m_pGraphObj(i_pGraphObj),
    m_selPtType(ESelectionPointType::PolygonShapePoint),
    m_selPt(ESelectionPoint::PolygonPoint),
    m_idxPt(idxPt)
{
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
    } else {
        strName += "ShapePoint" + QString::number(m_idxPt);
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
    str += CEnumSelectionPointType(m_selPtType).toString();
    if (m_selPtType == ESelectionPointType::BoundingRectangle) {
        str += "." + CEnumSelectionPoint(m_selPt).toString();
    } else if (m_selPtType == ESelectionPointType::PolygonShapePoint) {
        str += "." + QString::number(m_idxPt);
    }
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
        strlst[0]  = strlst[0].trimmed();
        strlst[1]  = strlst[1].trimmed();
        double fLength_px = strlst[0].toDouble(&bOk);
        if (!bOk) {
            CPhysVal physValLength(Units.Length.px);
            try {
                physValLength.setVal(strlst[0]);
                fLength_px = physValLength.getVal(Units.Length.px);
                bOk = true;
            } catch (...) {
                bOk = false;
            }
        }
        if (bOk) {
            double fAngle_degrees = strlst[1].toDouble(&bOk);
            if (!bOk) {
                CPhysVal physValAngle(Units.Angle.Degree);
                try {
                    physValAngle.setVal(strlst[0]);
                    fAngle_degrees = physValAngle.getVal(Units.Angle.Degree);
                    bOk = true;
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
    m_labelType(EGraphObjTypeUndefined),
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
    m_labelType(EGraphObjTypeUndefined),
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
    m_labelType(EGraphObjTypeUndefined),
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
    m_labelType(EGraphObjTypeUndefined),
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
        ", SelPtBoundingRect:" + m_selPtBoundingRect.toString() +
        ", PolygonShapePoint:" + QString::number(m_idxPolygonShapePoint) +
        ", LineSegment:" + QString::number(m_idxLineSegment) +
        ", PointSelected:" + point2Str(m_ptSelected) +
        ", Cursor:" + qCursorShape2Str(m_cursor.shape());
    return str;
}


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
class CGraphObj
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

const QString CGraphObj::c_strLabelName = "Name";
const QString CGraphObj::c_strGeometryLabelNameP1 = "P1";
const QString CGraphObj::c_strGeometryLabelNameP2 = "P2";
const QString CGraphObj::c_strGeometryLabelNameCenter = "Center";
const QString CGraphObj::c_strGeometryLabelNameDX = "dX";
const QString CGraphObj::c_strGeometryLabelNameDY = "dY";
const QString CGraphObj::c_strGeometryLabelNameLength = "Length";
const QString CGraphObj::c_strGeometryLabelNameAngle = "Angle";

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
/*! Creates a graphical object instance.

    The ctor does neither add the instance as a graphic item to Qt's grapics scene
    nor to the index tree of the drawing scene.

    Please also note that the constructor of the base class does not add the
    predefined label "Name". Othwerwise labels would also be added to labels which
    again would have labels and so on (stack overflow). Also selection points should
    not get a label. For this, and to be flexible, the "Name" label usually invisible
    as default should be added in the constructor of the derived class - if desired.

    @param i_pDrawingScene [in]
        Pointer to drawing scene from which the object is created.
    @param i_strFactoryGroupName [in]
        The group name of the object factory used to create the graphical objects
        of the given type (e.g. "Draw::Standard Shapes", "Draw::Widgets", "Draw::Electricity").
    @param i_type [in]
        Type as int of the graphical item.
    @param i_strType [in]
        Type as string of the graphical item (e.g. "Line", "Rect", "CheckBox", "Resistor").
    @param i_strObjName [in]
        Object name (e.g. "Line1", "Rect5", "CheckBoxUseFeature", "Resistor500OhmToGnd").
    @param i_drawSettings [in]
        Draw settings used to paint the item on the scene.
    @param idxTreeEntryType [in]
        Entry type. As default the entry type of the graph objects is Branch as all of
        them may have children. Groups anyway have children. But also any other types
        as if objects are selected they will create selection points. Also labels may
        be created as children. Only for selection points and labels the entry type
        will be set to Leave.
*/
CGraphObj::CGraphObj(
    CDrawingScene* i_pDrawingScene,
    const QString& i_strFactoryGroupName,
    EGraphObjType i_type,
    const QString& i_strType,
    const QString& i_strObjName,
    CIdxTreeEntry::EEntryType i_idxTreeEntryType ) :
//------------------------------------------------------------------------------
    CIdxTreeEntry(i_idxTreeEntryType, i_strObjName),
    m_bDtorInProgress(false),
    m_bAboutToBeDestroyedEmitted(false),
    m_bForceConversionToSceneCoors(false),
    m_pDrawingScene(i_pDrawingScene),
    m_strFactoryGroupName(i_strFactoryGroupName),
    m_type(i_type),
    m_strType(i_strType),
    m_drawSettings(i_type),
    m_pDrawSettingsTmp(nullptr),
    m_physValSizeMinimum(),
    m_physValSizeMaximum(),
    m_physValSizeFixed(),
    m_arAlignments(),
    m_bIsHit(false),
    m_bIsHighlighted(false),
    m_editMode(EEditMode::None),
    //m_editResizeMode(EEditResizeMode::None),
    m_arfZValues(CEnumRowVersion::count(), 0.0),
    m_transform(),
    m_transformByGroup(),
    //m_ptScenePos(),
    //m_idxSelPtSelectedPolygon(-1),
    m_arpSelPtsPolygon(),
    //m_selPtSelectedBoundingRect(ESelectionPoint::None),
    m_arpSelPtsBoundingRect(CEnumSelectionPoint::count()),
    m_strlstPredefinedLabelNames(),
    m_hshLabelDscrs(),
    m_hshpLabels(),
    m_strlstGeometryLabelNames(),
    m_hshGeometryLabelDscrs(),
    m_hshpGeometryLabels(),
    m_strToolTip(),
    m_strEditInfo(),
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    m_bCoorsDirty(true),
    m_rctCurr(),
    m_fRotAngleCurr_deg(0.0),
    m_ptRotOriginCurr(),
    // Original coordinates and transform values:
    m_bHasValidOrigCoors(false),
    m_ptPosOrig(),
    m_sizOrig(),
    m_fRotAngleOrig_deg(0.0),
    m_ptRotOriginOrig(),
#endif
    // Coordinates stored on mouse press events:
    //m_ptScenePosOnMousePressEvent(),
    //m_ptMouseEvScenePosOnMousePressEvent(),
    //m_rctOnMousePressEvent(),
    //m_ptRotOriginOnMousePressEvent(),
    m_iItemChangeUpdateOriginalCoorsBlockedCounter(false),
    // Simulation Functions:
    //m_arMousePressEventFunctions(),
    //m_arMouseReleaseEventFunctions(),
    //m_arMouseDoubleClickEventFunctions(),
    //m_arMouseMoveEventFunctions(),
    //m_arKeyPressEventFunctions(),
    //m_arKeyReleaseEventFunctions(),
    m_pTrcAdminObjCtorsAndDtor(nullptr),
    m_pTrcAdminObjItemChange(nullptr),
    m_pTrcAdminObjBoundingRect(nullptr),
    //m_pTrcAdminObjIsHit(nullptr),
    m_pTrcAdminObjCursor(nullptr),
    m_pTrcAdminObjPaint(nullptr),
    //m_pTrcAdminObjSceneEvent(nullptr),
    m_pTrcAdminObjSceneEventFilter(nullptr),
    m_pTrcAdminObjHoverEvents(nullptr),
    m_pTrcAdminObjMouseClickEvents(nullptr),
    m_pTrcAdminObjMouseMoveEvents(nullptr),
    m_pTrcAdminObjKeyEvents(nullptr)
{
    QObject::connect(
        m_pDrawingScene, &CDrawingScene::drawingSizeChanged,
        this, &CGraphObj::onDrawingSizeChanged);

} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Destroys the graphical object.

    All selection points and labels will be destroyed together with the object.
    The destructor of the CIdxTreeEntry will remove the item from the index tree.

    @note The destructor of the CGraphObj base class does not remove the graphical
          item from the graphics scene. This must be done in the destructor of
          the derived classes. For this the signal "aboutToBeDestroyed" has to
          be called in the derived classes. A counter ensures that the signal is
          just emitted once by the top most class in the class hierarchy.
          Please also note that the destructor of CIdxTreeEntry removes the
          grahical object from the index tree - if the object was added to the
          index tree.

    The dynamic cast to QGraphicsItem may return nullptr if the dtor of QGraphicsItem
    has already been executed. As CGraphObj is not derived from QGraphicsItem but
    only the derived classes inherit QGraphicsItem the destructor of QGraphicsItem
    may have already been processed before the destructor of the CGraphObj base class.
    The order the dtors of inherited classes are called depend on the order the classes
    appear in the list of the inherited classes on defining the class implementation.
    We cannot (or we don't want) to rely on that the order is defined that way that
    the destructor of QGraphicsItem is called after the destructor of CGraphObj.
    So we cannot remove the QGraphicsItem by calling "QGraphicsScene::removeItem" from
    within the destructor of CGraphObj. The  graphics item must be removed from the
    graphics scene's item list before the dtor of class QGraphicsItem is called.
    And this is only always the case in the dtor of the class derived from QGraphicsItem.

    Inheritance Graph:

      +---------------+     +----------------+
      | CIdxTreeEntry |     | QGraphicsItem  |
      +---------------+     +----------------+
              +                      +
              |                      |
      +---------------+    +-------------------+
      |   CGraphObj   |    | QGraphicsLineItem |
      +---------------+    +-------------------+
                 +             +
                  \           /
               +-----------------+
               |  CGraphObjLine  |
               +-----------------+
*/
CGraphObj::~CGraphObj()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::dtor",
        /* strAddInfo   */ "" );

    m_bDtorInProgress = true;

    // Should already have been called by derived classes. If not, this may lead
    // to exceptions when removing the graphics item from the graphics scene.
    emit_aboutToBeDestroyed();

    try {
        delete m_pDrawSettingsTmp;
    } catch (...) {
    }

    if (m_arpSelPtsBoundingRect.size() > 0) {
        for (int idxSelPt = m_arpSelPtsBoundingRect.size()-1; idxSelPt >= 0; idxSelPt--) {
            CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];
            if (pGraphObjSelPt != nullptr) {
                m_arpSelPtsBoundingRect[idxSelPt] = nullptr;
                try {
                    delete pGraphObjSelPt;
                }
                catch(...) {
                }
                pGraphObjSelPt = nullptr;
            }
        }
    }

    if (m_arpSelPtsPolygon.size() > 0) {
        for (int idxSelPt = m_arpSelPtsPolygon.size()-1; idxSelPt >= 0; idxSelPt--) {
            CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
            if (pGraphObjSelPt != nullptr) {
                m_arpSelPtsPolygon[idxSelPt] = nullptr;
                try {
                    delete pGraphObjSelPt;
                }
                catch(...) {
                }
                pGraphObjSelPt = nullptr;
            }
        }
    }

    QStringList strlstLabelNames = m_hshpLabels.keys();
    for (const QString& strLabelName : strlstLabelNames) {
        CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(strLabelName, nullptr);
        if (pGraphObjLabel != nullptr) {
            try {
                delete pGraphObjLabel;
            }
            catch(...) {
            }
            pGraphObjLabel = nullptr;
        }
    }

    QStringList strlstValueLabelNames = m_hshpGeometryLabels.keys();
    for (const QString& strLabelName : strlstValueLabelNames) {
        CGraphObjLabel* pGraphObjLabel = m_hshpGeometryLabels.value(strLabelName, nullptr);
        if (pGraphObjLabel != nullptr) {
            try {
                delete pGraphObjLabel;
            }
            catch(...) {
            }
            pGraphObjLabel = nullptr;
        }
    }

    // Please note that "onGraphObjAboutToBeDestroyed" is used to remove the graphical object from
    // the dictionary, the index list, and the sorted object pools of the drawing scene.
    // But selection points and labels have only been added to the item list of the graphics scene
    // and therefore don't have valid keys in the index tree.
    //if( m_pDrawingScene != nullptr )
    //{
    //    if( !m_strKeyInTree.isEmpty() )
    //    {
    //        try
    //        {
    //            m_pDrawingScene->onGraphObjAboutToBeDestroyed(m_strKeyInTree);
    //        }
    //        catch(...)
    //        {
    //        }
    //    }
    //    else
    //    {
    //        //QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    //        //m_pDrawingScene->removeItem(pGraphicsItem);
    //    }
    //} // if( m_pDrawingScene != nullptr )

    mthTracer.onAdminObjAboutToBeReleased();

    releaseTraceAdminObjs();

    m_bDtorInProgress = false;
    m_bAboutToBeDestroyedEmitted = false;
    m_bForceConversionToSceneCoors = false;
    m_pDrawingScene = nullptr;
    //m_strFactoryGroupName;
    m_type = static_cast<EGraphObjType>(0);
    //m_strType;
    //m_drawSettings;
    m_pDrawSettingsTmp = nullptr;
    //m_physValSizeMinimum;
    //m_physValSizeMaximum;
    //m_physValSizeFixed;
    //m_arAlignments;
    m_bIsHit = false;
    m_bIsHighlighted = false;
    m_editMode = static_cast<EEditMode>(0);
    //m_editResizeMode = static_cast<EEditResizeMode>(0);
    //m_arfZValues.clear();
    //m_transform;
    //m_transformByGroup;
    //m_ptScenePos;
    //m_idxSelPtSelectedPolygon = 0;
    //m_arpSelPtsPolygon;
    //m_selPtSelectedBoundingRect = static_cast<ESelectionPoint>(0);
    //m_arpSelPtsBoundingRect;
    //m_strlstPredefinedLabelNames;
    //m_hshLabelDscrs;
    //m_hshpLabels;
    //m_strlstGeometryLabelNames;
    //m_hshGeometryLabelDscrs;
    //m_hshpGeometryLabels;
    //m_strToolTip;
    //m_strEditInfo;
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    m_bCoorsDirty = false;
    //m_rctCurr;
    m_fRotAngleCurr_deg = 0.0;
    //m_ptRotOriginCurr;
    // Original item coordinates and transform values:
    m_bHasValidOrigCoors = false;
    //m_ptPosOrig;
    //m_sizOrig;
    m_fRotAngleOrig_deg = 0.0;
    //m_ptRotOriginOrig;
#endif
    // Coordinates stored on mouse press events:
    //m_ptScenePosOnMousePressEvent;
    //m_ptMouseEvScenePosOnMousePressEvent;
    //m_rctOnMousePressEvent;
    //m_ptRotOriginOnMousePressEvent;
    m_iItemChangeUpdateOriginalCoorsBlockedCounter = false;
    // Simulation Functions:
    //m_arMousePressEventFunctions;
    //m_arMouseReleaseEventFunctions;
    //m_arMouseDoubleClickEventFunctions;
    //m_arMouseMoveEventFunctions;
    //m_arKeyPressEventFunctions;
    //m_arKeyReleaseEventFunctions;
    // Method Tracing
    m_pTrcAdminObjCtorsAndDtor = nullptr;
    m_pTrcAdminObjItemChange = nullptr;
    m_pTrcAdminObjBoundingRect = nullptr;
    //m_pTrcAdminObjIsHit = nullptr;
    m_pTrcAdminObjCursor = nullptr;
    m_pTrcAdminObjPaint = nullptr;
    //m_pTrcAdminObjSceneEvent = nullptr;
    m_pTrcAdminObjSceneEventFilter = nullptr;
    m_pTrcAdminObjHoverEvents = nullptr;
    m_pTrcAdminObjMouseClickEvents = nullptr;
    m_pTrcAdminObjMouseMoveEvents = nullptr;
    m_pTrcAdminObjKeyEvents = nullptr;

} // dtor

/*==============================================================================
protected: // instance methods (trace admin objects have to be created in ctor of "final" class)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates the trace admin objects for method tracing.

    This method has to be called in the constructor of the "final" class.

    Please note that the counterpart "releaseTraceAdminObjs" is called by the
    destructor of the base class CGraphObj. !! Don't call "releaseTraceAdminObjs"
    in the destructor of the derived class. !!

    @param [in] i_strClassName
        Class name of the derived class. The class name may be preceded with
        a group name like "StandardShapes", "Electricity" or "QtWidget" followed
        by a double colon like "createTraceAdminObjs("StandardShapes::" + ClassName)".
*/
void CGraphObj::createTraceAdminObjs(const QString& i_strClassName)
//------------------------------------------------------------------------------
{
    if (m_pTrcAdminObjCtorsAndDtor == nullptr) {
        m_pTrcAdminObjCtorsAndDtor = CTrcServer::GetTraceAdminObj(
            NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::CtorsAndDtor");
        m_pTrcAdminObjItemChange = CTrcServer::GetTraceAdminObj(
            NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::ItemChange");
        m_pTrcAdminObjBoundingRect = CTrcServer::GetTraceAdminObj(
            NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::BoundingRect");
        //m_pTrcAdminObjIsHit = CTrcServer::GetTraceAdminObj(
        //    NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::IsHit");
        m_pTrcAdminObjCursor = CTrcServer::GetTraceAdminObj(
            NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::Cursor");
        m_pTrcAdminObjPaint = CTrcServer::GetTraceAdminObj(
            NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::Paint");
        //m_pTrcAdminObjSceneEvent = CTrcServer::GetTraceAdminObj(
        //    NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::SceneEvent");
        m_pTrcAdminObjSceneEventFilter = CTrcServer::GetTraceAdminObj(
            NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::SceneEventFilter");
        m_pTrcAdminObjHoverEvents = CTrcServer::GetTraceAdminObj(
            NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::HoverEvents");
        m_pTrcAdminObjMouseClickEvents = CTrcServer::GetTraceAdminObj(
            NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::MouseClickEvents");
        m_pTrcAdminObjMouseMoveEvents = CTrcServer::GetTraceAdminObj(
            NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::MouseMoveEvents");
        m_pTrcAdminObjKeyEvents = CTrcServer::GetTraceAdminObj(
            NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::KeyEvents");
    }
}

//------------------------------------------------------------------------------
/*! @brief Releases the trace admin objects for method tracing.

    This method is called by the destructor of the base class CGraphObj and
    therefore ! MUST NOT ! be called by the destructor of the derived class.

    Please note that the counterpart "createTraceAdminObjs" !MUST BE! called
    by the constructor of the derived class.
*/
void CGraphObj::releaseTraceAdminObjs()
//------------------------------------------------------------------------------
{
    if (m_pTrcAdminObjCtorsAndDtor != nullptr) {
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjCtorsAndDtor);
        m_pTrcAdminObjCtorsAndDtor = nullptr;
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjItemChange);
        m_pTrcAdminObjItemChange = nullptr;
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjBoundingRect);
        m_pTrcAdminObjBoundingRect = nullptr;
        //CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjIsHit);
        //m_pTrcAdminObjIsHit = nullptr;
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjCursor);
        m_pTrcAdminObjCursor = nullptr;
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjPaint);
        m_pTrcAdminObjPaint = nullptr;
        //CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjSceneEvent);
        //m_pTrcAdminObjSceneEvent = nullptr;
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjSceneEventFilter);
        m_pTrcAdminObjSceneEventFilter = nullptr;
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjHoverEvents);
        m_pTrcAdminObjHoverEvents = nullptr;
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjMouseClickEvents);
        m_pTrcAdminObjMouseClickEvents = nullptr;
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjMouseMoveEvents);
        m_pTrcAdminObjMouseMoveEvents = nullptr;
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjKeyEvents);
        m_pTrcAdminObjKeyEvents = nullptr;
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the object type.
*/
EGraphObjType CGraphObj::type() const
//------------------------------------------------------------------------------
{
    return m_type;
}

//------------------------------------------------------------------------------
/*! @brief Returns the object type as a string.
*/
QString CGraphObj::typeAsString() const
//------------------------------------------------------------------------------
{
    return m_strType;
}

//------------------------------------------------------------------------------
/*! @brief Convenience method to returning true if this object is a point.
*/
bool CGraphObj::isPoint() const
//------------------------------------------------------------------------------
{
    return m_type == EGraphObjTypePoint;
}

//------------------------------------------------------------------------------
/*! @brief Convenience method to returning true if this object is a line.
*/
bool CGraphObj::isLine() const
//------------------------------------------------------------------------------
{
    return m_type == EGraphObjTypeLine;
}

//------------------------------------------------------------------------------
/*! @brief Convenience method to returning true if this object is a rectangle.
*/
bool CGraphObj::isRect() const
//------------------------------------------------------------------------------
{
    return m_type == EGraphObjTypeRect;
}

//------------------------------------------------------------------------------
/*! @brief Convenience method to returning true if this object is an ellipse.
*/
bool CGraphObj::isEllipse() const
//------------------------------------------------------------------------------
{
    return m_type == EGraphObjTypeEllipse;
}

//------------------------------------------------------------------------------
/*! @brief Convenience method to returning true if this object is a polygon.
*/
bool CGraphObj::isPolygon() const
//------------------------------------------------------------------------------
{
    return m_type == EGraphObjTypePolygon;
}

//------------------------------------------------------------------------------
/*! @brief Convenience method to returning true if this object is a polyline.
*/
bool CGraphObj::isPolyline() const
//------------------------------------------------------------------------------
{
    return m_type == EGraphObjTypePolyline;
}

//------------------------------------------------------------------------------
/*! @brief Convenience method to returning true if this object is a text.
*/
bool CGraphObj::isText() const
//------------------------------------------------------------------------------
{
    return m_type == EGraphObjTypeText;
}

//------------------------------------------------------------------------------
/*! @brief Convenience method to returning true if this object is an image.
*/
bool CGraphObj::isImage() const
//------------------------------------------------------------------------------
{
    return m_type == EGraphObjTypeImage;
}

//------------------------------------------------------------------------------
/*! @brief Convenience method to returning true if this object is a connection point.
*/
bool CGraphObj::isConnectionPoint() const
//------------------------------------------------------------------------------
{
    return m_type == EGraphObjTypeConnectionPoint;
}

//------------------------------------------------------------------------------
/*! @brief Convenience method to returning true if this object is a connection line.
*/
bool CGraphObj::isConnectionLine() const
//------------------------------------------------------------------------------
{
    return m_type == EGraphObjTypeConnectionLine;
}

//------------------------------------------------------------------------------
/*! @brief Convenience method to returning true if this object is a group.
*/
bool CGraphObj::isGroup() const
//------------------------------------------------------------------------------
{
    return m_type == EGraphObjTypeGroup;
}

//------------------------------------------------------------------------------
/*! @brief Convenience method to returning true if this object is a selection point.
*/
bool CGraphObj::isSelectionPoint() const
//------------------------------------------------------------------------------
{
    return m_type == EGraphObjTypeSelectionPoint;
}

//------------------------------------------------------------------------------
/*! @brief Convenience method to returning true if this object is a label.
*/
bool CGraphObj::isLabel() const
//------------------------------------------------------------------------------
{
    return ((m_type >= EGraphObjTypeLabelMin) && (m_type <= EGraphObjTypeLabelMax));
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the drawing scene the object belongs to.
*/
CDrawingScene* CGraphObj::drawingScene()
//------------------------------------------------------------------------------
{
    return m_pDrawingScene;
}

//------------------------------------------------------------------------------
/*! @brief Returns the parent object the object belongs to.

    For selection points this is the object creating the selection points and
    to which the selection points are linked to.
    For labels this is also the object creating the labels and to which the
    labels are linked to.
    If the object is added as a child to a group the parent is the group object.

    @return nullptr, if the object is neither a selection point, a label or is not
            added as a child to a group, pointer to parent otherwise.
*/
//------------------------------------------------------------------------------
CGraphObj* CGraphObj::parentGraphObj()
{
    return dynamic_cast<CGraphObj*>(m_pParentBranch);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
///*! @brief Sets the parent object the object belongs to.
//
//    For selection points this is the object creating the selection points and
//    to which the selection points are linked to.
//    For labels this is also the object creating the labels and to which the
//    labels are linked to.
//    If the object is added as a child to a group the parent is the group object.
//*/
//void CGraphObj::setParentGraphObj(CGraphObj* i_pGraphObjParent)
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = QString(i_pGraphObjParent == nullptr ? "null" : i_pGraphObjParent->keyInTree());
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::rename",
//        /* strAddInfo   */ strMthInArgs );
//
//    if (m_pGraphObjParent != i_pGraphObjParent) {
//        if (m_pGraphObjParent != nullptr) {
//            QObject::disconnect(
//                m_pGraphObjParent, &CGraphObj::geometryChanged,
//                this, &CGraphObj::onGraphObjParentGeometryChanged);
//            QObject::disconnect(
//                m_pGraphObjParent, &CGraphObj::zValueChanged,
//                this, &CGraphObj::onGraphObjParentZValueChanged);
//        }
//        m_pGraphObjParent = i_pGraphObjParent;
//        if (m_pGraphObjParent != nullptr) {
//            QObject::connect(
//                m_pGraphObjParent, &CGraphObj::geometryChanged,
//                this, &CGraphObj::onGraphObjParentGeometryChanged);
//            QObject::connect(
//                m_pGraphObjParent, &CGraphObj::zValueChanged,
//                this, &CGraphObj::onGraphObjParentZValueChanged);
//        }
//    }
//}

//------------------------------------------------------------------------------
/*! This method is provided for convinience to rename an entry without the need
    of accessing the index tree and invoking the rename method of the index tree.

    On changing the name of an index tree entry also its key in the index tree
    and the key of all child entries got to be updated. This is handled by the
    index tree. For this this method call is delegated to the index tree of the
    entry. As a precondition the entry (the graphical object) must have been
    added before to the index tree of the drawing scene.

    As a reentry the index tree calls "setName" of the tree entry. "setName"
    is overridden by the graphical object to update the label text (if visible).

    @param i_strNameNew [in] New name of the tree entry.

    @note Throws a critical exception
          - with Result = ObjNotInList if the tree entry does not belong to the
            index tree of the drawing scene.
*/
void CGraphObj::rename( const QString& i_strNameNew )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strNameNew;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::rename",
        /* strAddInfo   */ strMthInArgs );

    if (m_pTree == nullptr) {
        SErrResultInfo errResultInfo = ErrResultInfoCritical("rename", EResultObjNotInList, m_strName);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    if (m_strName != i_strNameNew) {
        // "setName" is called as reentry by the index tree.
        m_pTree->rename(this, i_strNameNew);
    }
}

/*==============================================================================
protected: // overridables of base class CIdxTreeEntry
==============================================================================*/

//------------------------------------------------------------------------------
/*! Sets the current name of the object.

    This method is for internal use only and is called by the index tree
    if an entry is renamed.

    Changing the name also changes its key.
    If the name of a branch is changed also the keys of all children must
    be set correspondingly. This is handled by the index tree.

    If the name of the object is set (either after initially creating the object
    or after renaming the object) the name and path indicated in visible labels
    must be updated.

    @param i_strName [in] New name of the tree entry.
*/
void CGraphObj::setName( const QString& i_strName )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setName",
        /* strAddInfo   */ strMthInArgs );

    if (m_strName != i_strName)
    {
        CIdxTreeEntry::setName(i_strName);
        if (m_hshLabelDscrs.contains(c_strLabelName)) {
            m_hshLabelDscrs[c_strLabelName].m_strText = i_strName;
        }
        CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(c_strLabelName, nullptr);
        if (pGraphObjLabel != nullptr) {
            pGraphObjLabel->setText(name());
        }
        //updateEditInfo();
        //updateToolTip();
    }
}

//------------------------------------------------------------------------------
/*! Sets the key of the entry in the index tree.

    This method is for internal use only and is implicitely called by the
    index tree if the name or the parent of an entry is changed.

    If the name or the parent of the object is changed the name and path indicated
    in visible labels must be updated.

    @param i_strKey [in] New key of the tree entry.
*/
void CGraphObj::setKeyInTree( const QString& i_strKey )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strKey;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setKeyInTree",
        /* strAddInfo   */ strMthInArgs );

    if (m_strKeyInTree != i_strKey)
    {
        CIdxTreeEntry::setKeyInTree(i_strKey);
        if (m_hshLabelDscrs.contains(c_strLabelName)) {
            m_hshLabelDscrs[c_strLabelName].m_strText = name();
        }
        CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(c_strLabelName, nullptr);
        if (pGraphObjLabel != nullptr) {
            pGraphObjLabel->setText(name());
        }
        //updateEditInfo();
        //updateToolTip();
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the group name of the class factory used to create objects.

    The factory group name and the type of the object uniquely define the object factory.
*/
QString CGraphObj::getFactoryGroupName() const
//------------------------------------------------------------------------------
{
    return m_strFactoryGroupName;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns a human readable string containing the coordinates of all
           shape points of the object.

    This method is mainly used for debugging and test purposes.
*/
QString CGraphObj::getScenePolygonShapePointsString() const
//------------------------------------------------------------------------------
{
    return "";
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the currently selected index of the polygons shape point
           if the form of the object is modified by moving a polygon shape point.
*/
//int CGraphObj::getSelectedPolygonShapePointIndex() const
////------------------------------------------------------------------------------
//{
//    return m_idxSelPtSelectedPolygon;
//}

//------------------------------------------------------------------------------
/*! @brief Returns the currently selected point at the bounding rectangle if
           if the form of the object is modified by moving a selection point
           at the bounding rectangle.

     PolygonPoint is returned if a polygon point is selected.
*/
//CEnumSelectionPoint CGraphObj::getSelectedBoundingRectPoint() const
////------------------------------------------------------------------------------
//{
//    return m_selPtSelectedBoundingRect;
//}

//------------------------------------------------------------------------------
/*! @brief Returns a human readable tool tip string.
*/
//QString CGraphObj::getToolTip() const
////------------------------------------------------------------------------------
//{
//    return m_strToolTip;
//}

//------------------------------------------------------------------------------
/*! @brief Returns a human readable string with information about the current editing mode.
*/
//QString CGraphObj::getEditInfo() const
////------------------------------------------------------------------------------
//{
//    return m_strEditInfo;
//}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/* @brief

    Must be overridden to create a user defined dialog.
*/
void CGraphObj::onCreateAndExecDlgFormatGraphObjs()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::onCreateAndExecDlgFormatGraphObjs",
        /* strAddInfo   */ "" );

    CDlgFormatGraphObjs* pDlgFormatGraphObjs = new CDlgFormatGraphObjs(m_pDrawingScene, this);
    pDlgFormatGraphObjs->setCurrentWidget(CDlgFormatGraphObjs::c_strWdgtLineStyle);
    pDlgFormatGraphObjs->exec();
    delete pDlgFormatGraphObjs;
    pDlgFormatGraphObjs = nullptr;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the draw settings like pen color, pen style, brush style etc.
           used to paint the object.
*/
void CGraphObj::setDrawSettings( const CDrawSettings& i_drawSettings )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_drawSettings.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setDrawSettings",
        /* strAddInfo   */ strMthInArgs );

    // When emitting the drawSettingsChanged signal new settings may be applied
    // as reentries by the signal receivers. Those new settings may have to be
    // temporarily stored and a new temporary buffer has to be allocated.
    // The current buffer pointer will be locally saved and freed after emitting
    // the signals. If new settings got to be saved a new buffer is allocated.
    CDrawSettings* pDrawSettingsTmp = m_pDrawSettingsTmp;
    m_pDrawSettingsTmp = nullptr;

    if (m_drawSettings != i_drawSettings) {
        CDrawSettings drawSettingsOld = m_drawSettings;
        m_drawSettings = i_drawSettings;
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
        onDrawSettingsChanged(drawSettingsOld);
    }
    delete pDrawSettingsTmp;
    pDrawSettingsTmp = nullptr;
}

//------------------------------------------------------------------------------
/*! @brief Returns the currently used draw settings like pen color, pen style,
           brush style etc. used to paint the object.
*/
CDrawSettings CGraphObj::getDrawSettings() const
//------------------------------------------------------------------------------
{
    return m_drawSettings;
}

//------------------------------------------------------------------------------
/*! @brief Emits the drawSettingsChanged signal.

    If there are temporary stored changes those changes will first be taken
    over before the signal is emitted.

    The temporary cache is used if several draw setting attributes got to be
    modified one after another. For this the flag i_bImmediatelyApplySetting
    has been passed with false to the "set<DrawSettingProperty>" methods.
    After modifying all attributes "updateDrawSettings" must be called to take
    over the modified settings, to repaint the graphics item with the modified
    attributes and to emit the drawSettingsChanged signal.

    @return true, if there were changes, false otherwise.
*/
bool CGraphObj::updateDrawSettings()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::updateDrawSettings",
        /* strAddInfo   */ "" );

    bool bChanged = false;
    if (m_pDrawSettingsTmp != nullptr) {
        if (*m_pDrawSettingsTmp != m_drawSettings) {
            bChanged = true;
            // setDrawSettings will delete the temporary buffer.
            setDrawSettings(*m_pDrawSettingsTmp);
        }
        else {
            delete m_pDrawSettingsTmp;
            m_pDrawSettingsTmp = nullptr;
        }
    }

    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bChanged);
    }
    return bChanged;
}

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief

    Must be overridden to apply the draw settings at the graphics item in derived classes.
*/
void CGraphObj::onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "OldSettings {" + i_drawSettingsOld.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::onDrawSettingsChanged",
        /* strAddInfo   */ strMthInArgs );

    /* Template for copy and paste in inherited classes:
    bool bDrawSettingsChanged = (m_drawSettings != i_drawSettingsOld);
    if (bDrawSettingsChanged) {
        ...
        Add code to apply changes
        ...
        update(); // force paint method
        emit_drawSettingsChanged(); // inform others of changes
    }
    */
} // onDrawSettingsChanged

/*==============================================================================
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") ..
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the pen color used to paint the object.

    If several draw setting attributes got to modified one after another
    the flag i_bImmediatelyApplySetting should be set to false.
    In this case the modified draw setting attribute will be stored in a
    temporary cache, the signal "drawSettingsChanged" is not emitted and
    the object is not forced to be repainted.

    After setting all draw attributes the method updateDrawSettings must be
    called to take over the modified draw attributes from the temporary cache
    to the current draw settings. The updateDrawSettings method will update
    (repaint) the graphics item and emits the drawSettingsChanged signal.

    @param [in] i_clr
        New pen color to be used.
    @param [in] i_bImmediatelyApplySetting (default: true)
        Set this flag to false if further set<DrawAttribute> will follow and you
        want to update all changes at once later on.
*/
void CGraphObj::setPenColor( const QColor& i_clr, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_clr.name() + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setPenColor",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getPenColor() != i_clr) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setPenColor(i_clr);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setPenColor(i_clr);
    }
}

//------------------------------------------------------------------------------
/*! @brief 
*/
QColor CGraphObj::getPenColor() const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getPenColor();
}

//------------------------------------------------------------------------------
/*! @brief Sets the pen width used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setPenWidth( int i_iLineWidth, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iLineWidth) + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setPenWidth",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getPenWidth() != i_iLineWidth) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setPenWidth(i_iLineWidth);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setPenWidth(i_iLineWidth);
    }
}

//------------------------------------------------------------------------------
/*! @brief 
*/
int CGraphObj::getPenWidth() const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getPenWidth();
}

/*==============================================================================
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") ..
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the line style used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setLineStyle( const CEnumLineStyle& i_lineStyle, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_lineStyle.toString() + ", Apply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setLineStyle",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getLineStyle() != i_lineStyle) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setLineStyle(i_lineStyle);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setLineStyle(i_lineStyle);
    }
} // setLineStyle

//------------------------------------------------------------------------------
/*! @brief 
*/
CEnumLineStyle CGraphObj::getLineStyle() const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getLineStyle();
}

/*==============================================================================
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") ..
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the fill color width used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setFillColor( const QColor& i_clr, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_clr.name() + ", Apply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setFillColor",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getFillColor() != i_clr) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setFillColor(i_clr);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setFillColor(i_clr);
    }
} // setFillColor

//------------------------------------------------------------------------------
/*! @brief 
*/
QColor CGraphObj::getFillColor() const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getFillColor();
}

//------------------------------------------------------------------------------
/*! @brief Sets the fill style used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setFillStyle( const CEnumFillStyle& i_fillStyle, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_fillStyle.toString() + ", Apply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setFillStyle",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getFillStyle() != i_fillStyle) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setFillStyle(i_fillStyle);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setFillStyle(i_fillStyle);
    }
} // setFillStyle

//------------------------------------------------------------------------------
/*! @brief 
*/
CEnumFillStyle CGraphObj::getFillStyle() const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getFillStyle();
}

/*==============================================================================
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") ..
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the line record type used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setLineRecordType( const CEnumLineRecordType& i_recordType, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_recordType.toString() + ", Apply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setLineRecordType",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getLineRecordType() != i_recordType) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setLineRecordType(i_recordType);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setLineRecordType(i_recordType);
    }
} // setLineRecordType

//------------------------------------------------------------------------------
/*! @brief 
*/
CEnumLineRecordType CGraphObj::getLineRecordType() const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getLineRecordType();
}

//------------------------------------------------------------------------------
/*! @brief Sets the line extent used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setLineExtent( int i_iLineExtent, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iLineExtent) + ", Apply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setLineExtent",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getLineExtent() != i_iLineExtent) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setLineExtent(i_iLineExtent);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setLineExtent(i_iLineExtent);
    }
} // setLineExtent

//------------------------------------------------------------------------------
/*! @brief 
*/
int CGraphObj::getLineExtent() const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getLineExtent();
}

/*==============================================================================
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") ..
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the line end style used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setLineEndStyle(
    const CEnumLinePoint& i_linePoint, const CEnumLineEndStyle& i_endStyle, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_linePoint.toString() + ", " + i_endStyle.toString() + ", Apply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setLineEndStyle",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getLineEndStyle(i_linePoint) != i_endStyle) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setLineEndStyle(i_linePoint, i_endStyle);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setLineEndStyle(i_linePoint, i_endStyle);
    }
} // setLineEndStyle

//------------------------------------------------------------------------------
/*! @brief 
*/
CEnumLineEndStyle CGraphObj::getLineEndStyle(const CEnumLinePoint& i_linePoint) const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getLineEndStyle(i_linePoint);
}

//------------------------------------------------------------------------------
/*! @brief Sets the line end base line type used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setArrowHeadBaseLineType(
    const CEnumLinePoint& i_linePoint, const CEnumArrowHeadBaseLineType& i_baseLineType, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_linePoint.toString() + ", " + i_baseLineType.toString() + ", Apply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setArrowHeadBaseLineType",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getArrowHeadBaseLineType(i_linePoint) != i_baseLineType) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setArrowHeadBaseLineType(i_linePoint, i_baseLineType);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setArrowHeadBaseLineType(i_linePoint, i_baseLineType);
    }
} // setArrowHeadBaseLineType

//------------------------------------------------------------------------------
/*! @brief 
*/
CEnumArrowHeadBaseLineType CGraphObj::getArrowHeadBaseLineType( const CEnumLinePoint& i_linePoint) const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getArrowHeadBaseLineType(i_linePoint);
}

//------------------------------------------------------------------------------
/*! @brief Sets the line end fill style used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setArrowHeadFillStyle(
    const CEnumLinePoint& i_linePoint, const CEnumArrowHeadFillStyle& i_fillStyle, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_linePoint.toString() + ", " + i_fillStyle.toString() + ", Apply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setArrowHeadFillStyle",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getArrowHeadFillStyle(i_linePoint) != i_fillStyle) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setArrowHeadFillStyle(i_linePoint, i_fillStyle);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setArrowHeadFillStyle(i_linePoint, i_fillStyle);
    }
} // setArrowHeadFillStyle

//------------------------------------------------------------------------------
/*! @brief 
*/
CEnumArrowHeadFillStyle CGraphObj::getArrowHeadFillStyle(
    const CEnumLinePoint& i_linePoint) const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getArrowHeadFillStyle(i_linePoint);
}

//------------------------------------------------------------------------------
/*! @brief Sets the line end width used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setArrowHeadWidth(
    const CEnumLinePoint& i_linePoint, const CEnumArrowHeadWidth& i_width, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_linePoint.toString() + ", " + i_width.toString() + ", Apply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setArrowHeadWidth",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getArrowHeadWidth(i_linePoint) != i_width) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setArrowHeadWidth(i_linePoint, i_width);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setArrowHeadWidth(i_linePoint, i_width);
    }
} // setArrowHeadWidth

//------------------------------------------------------------------------------
/*! @brief 
*/
CEnumArrowHeadWidth CGraphObj::getArrowHeadWidth(const CEnumLinePoint& i_linePoint) const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getArrowHeadWidth(i_linePoint);
}

//------------------------------------------------------------------------------
/*! @brief Sets the line end length used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setArrowHeadLength(
    const CEnumLinePoint& i_linePoint, const CEnumArrowHeadLength& i_length, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_linePoint.toString() + ", " + i_length.toString() + ", Apply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setArrowHeadLength",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getArrowHeadLength(i_linePoint) != i_length) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setArrowHeadLength(i_linePoint, i_length);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setArrowHeadLength(i_linePoint, i_length);
    }
} // setArrowHeadLength

//------------------------------------------------------------------------------
/*! @brief 
*/
CEnumArrowHeadLength CGraphObj::getArrowHeadLength( const CEnumLinePoint& i_linePoint ) const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getArrowHeadLength(i_linePoint);
}

/*==============================================================================
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") ..
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the text color used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setTextColor( const QColor& i_clr, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_clr.name() + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setTextColor",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getTextColor() != i_clr) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setTextColor(i_clr);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setTextColor(i_clr);
    }
} // setTextColor

//------------------------------------------------------------------------------
/*! @brief 
*/
QColor CGraphObj::getTextColor() const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getTextColor();
}

//------------------------------------------------------------------------------
/*! @brief Sets the font used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setTextFont( const QFont& i_fnt, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_fnt.family() + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setTextFont",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getTextFont() != i_fnt) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setTextFont(i_fnt);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setTextFont(i_fnt);
    }
} // setTextFont

//------------------------------------------------------------------------------
/*! @brief 
*/
QFont CGraphObj::getTextFont() const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getTextFont();
}

//------------------------------------------------------------------------------
/*! @brief Sets the text size used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setTextSize( ETextSize i_size, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = textSize2Str(i_size) + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setTextSize",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getTextSize() != i_size) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setTextSize(i_size);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setTextSize(i_size);
    }
} // setTextSize

//------------------------------------------------------------------------------
/*! @brief 
*/
ETextSize CGraphObj::getTextSize() const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getTextSize();
}

//------------------------------------------------------------------------------
/*! @brief Sets the text style used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setTextStyle( const CEnumTextStyle& i_textStyle, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_textStyle.toString() + ", eApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setTextStyle",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getTextStyle() != i_textStyle) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setTextStyle(i_textStyle);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setTextStyle(i_textStyle);
    }
} // setTextStyle

//------------------------------------------------------------------------------
/*! @brief 
*/
CEnumTextStyle CGraphObj::getTextStyle() const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getTextStyle();
}

//------------------------------------------------------------------------------
/*! @brief Sets the text effect used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setTextEffect( const CEnumTextEffect& i_textEffect, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_textEffect.toString() + ", Apply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setTextEffect",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getTextEffect() != i_textEffect) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setTextEffect(i_textEffect);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setTextEffect(i_textEffect);
    }
} // setTextEffect

//------------------------------------------------------------------------------
/*! @brief 
*/
CEnumTextEffect CGraphObj::getTextEffect() const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getTextEffect();
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::setMinimumWidth( const CPhysVal& i_physValWidth )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValWidth.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setMinimumWidth",
        /* strAddInfo   */ strMthInArgs );

    m_physValSizeMinimum.setWidth(i_physValWidth);

    //if (m_physValSizeMinimum.width() > 0.0) {
    //    CPhysValSize physValSize = getSize(m_physValSizeMinimum.unit());
    //    if (physValSize.width() < m_physValSizeMinimum.width()) {
    //        physValSize.setWidth(m_physValSizeMinimum.width());
    //        setSize(physValSize);
    //    }
    //}
}

//------------------------------------------------------------------------------
bool CGraphObj::hasMinimumWidth() const
//------------------------------------------------------------------------------
{
    bool bHas = false;
    if (m_physValSizeFixed.width() > 0.0) {
        bHas = true;
    }
    else if (m_physValSizeMinimum.width() > 0.0) {
        bHas = true;
    }
    return bHas;
}

//------------------------------------------------------------------------------
CPhysVal CGraphObj::getMinimumWidth(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysVal physValWidth(0.0, drawingSize.unit(), drawingSize.imageCoorsResolution(drawingSize.unit()));
    if (m_physValSizeFixed.width() > 0.0) {
        physValWidth = m_physValSizeFixed.width();
    }
    else if (m_physValSizeMinimum.width() > 0.0) {
        physValWidth = m_physValSizeMinimum.width();
    }
    physValWidth.convertValue(i_unit);
    return physValWidth;
}

//------------------------------------------------------------------------------
void CGraphObj::setMinimumHeight( const CPhysVal& i_physValHeight )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValHeight.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setMinimumHeight",
        /* strAddInfo   */ strMthInArgs );

    m_physValSizeMinimum.setHeight(i_physValHeight);

    //if (m_physValSizeMinimum.height() > 0.0) {
    //    CPhysValSize physValSize = getSize(m_physValSizeMinimum.unit());
    //    if (physValSize.height() < m_physValSizeMinimum.height()) {
    //        physValSize.setHeight(m_physValSizeMinimum.height());
    //        setSize(physValSize);
    //    }
    //}
}

//------------------------------------------------------------------------------
bool CGraphObj::hasMinimumHeight() const
//------------------------------------------------------------------------------
{
    bool bHas = false;
    if (m_physValSizeFixed.height() > 0.0) {
        bHas = true;
    }
    else if (m_physValSizeMinimum.height() > 0.0) {
        bHas = true;
    }
    return bHas;
}

//------------------------------------------------------------------------------
CPhysVal CGraphObj::getMinimumHeight(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysVal physValHeight(0.0, drawingSize.unit(), drawingSize.imageCoorsResolution(drawingSize.unit()));
    if (m_physValSizeFixed.height() > 0.0) {
        physValHeight = m_physValSizeFixed.height();
    }
    else if (m_physValSizeMinimum.height() > 0.0) {
        physValHeight = m_physValSizeMinimum.height();
    }
    physValHeight.convertValue(i_unit);
    return physValHeight;
}

//------------------------------------------------------------------------------
void CGraphObj::setMinimumSize( const CPhysValSize& i_physValSize )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValSize.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setMinimumSize",
        /* strAddInfo   */ strMthInArgs );

    m_physValSizeMinimum = i_physValSize;

    //if (m_physValSizeMinimum.isValid()) {
    //    CPhysValSize physValSize = getSize(m_physValSizeMinimum.unit());
    //    if (physValSize.width() < m_physValSizeMinimum.width() || physValSize.height() < m_physValSizeMinimum.height()) {
    //        if (physValSize.width() < m_physValSizeMinimum.width()) {
    //            physValSize.setWidth( m_physValSizeMinimum.width() );
    //        }
    //        if (physValSize.height() < m_physValSizeMinimum.height()) {
    //            physValSize.setHeight( m_physValSizeMinimum.height() );
    //        }
    //        setSize(physValSize);
    //    }
    //}
}

//------------------------------------------------------------------------------
bool CGraphObj::hasMinimumSize() const
//------------------------------------------------------------------------------
{
    bool bHas = false;
    if (m_physValSizeFixed.isValid() ) {
        bHas = true;
    }
    else if (m_physValSizeMinimum.isValid()) {
        bHas = true;
    }
    return bHas;
}

//------------------------------------------------------------------------------
CPhysValSize CGraphObj::getMinimumSize(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    CPhysValSize physValSize;
    if (m_physValSizeFixed.isValid()) {
        physValSize = m_physValSizeFixed;
    }
    else if (m_physValSizeMinimum.isValid()) {
        physValSize = m_physValSizeMinimum;
    }
    m_pDrawingScene->convert(physValSize, i_unit);
    return physValSize;
}

//------------------------------------------------------------------------------
void CGraphObj::setMaximumWidth( const CPhysVal& i_physValWidth )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValWidth.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setMaximumWidth",
        /* strAddInfo   */ strMthInArgs );

    m_physValSizeMaximum.setWidth(i_physValWidth);

    //if (m_physValSizeMaximum.width() > 0.0) {
    //    CPhysValSize physValSize = getSize(m_physValSizeMaximum.unit());
    //    if (physValSize.width() > m_physValSizeMaximum.width()) {
    //        physValSize.setWidth( m_physValSizeMaximum.width() );
    //        setSize(physValSize);
    //    }
    //}
}

//------------------------------------------------------------------------------
bool CGraphObj::hasMaximumWidth() const
//------------------------------------------------------------------------------
{
    bool bHas = false;
    if (m_physValSizeFixed.width() > 0.0) {
        bHas = true;
    }
    else if (m_physValSizeMaximum.width() > 0.0) {
        bHas = true;
    }
    return bHas;
}

//------------------------------------------------------------------------------
CPhysVal CGraphObj::getMaximumWidth(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysVal physValWidth(0.0, drawingSize.unit(), drawingSize.imageCoorsResolution(drawingSize.unit()));
    if (m_physValSizeFixed.width() > 0.0) {
        physValWidth = m_physValSizeFixed.width();
    }
    else if (m_physValSizeMaximum.width() > 0.0) {
        physValWidth = m_physValSizeMaximum.width();
    }
    physValWidth.convertValue(i_unit);
    return physValWidth;
}

//------------------------------------------------------------------------------
void CGraphObj::setMaximumHeight( const CPhysVal& i_physValHeight )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValHeight.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setMaximumHeight",
        /* strAddInfo   */ strMthInArgs );

    m_physValSizeMaximum.setHeight(i_physValHeight);

    //if (m_physValSizeMaximum.height() > 0.0) {
    //    CPhysValSize physValSize = getSize(m_physValSizeMaximum.unit());
    //    if (physValSize.height() > m_physValSizeMaximum.height()) {
    //        physValSize.setHeight( m_physValSizeMaximum.height() );
    //        setSize(physValSize);
    //    }
    //}
}

//------------------------------------------------------------------------------
bool CGraphObj::hasMaximumHeight() const
//------------------------------------------------------------------------------
{
    bool bHas = false;
    if (m_physValSizeFixed.height() > 0.0) {
        bHas = true;
    }
    else if (m_physValSizeMaximum.height() > 0.0) {
        bHas = true;
    }
    return bHas;
}

//------------------------------------------------------------------------------
CPhysVal CGraphObj::getMaximumHeight(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysVal physValHeight(0.0, drawingSize.unit(), drawingSize.imageCoorsResolution(drawingSize.unit()));
    if (m_physValSizeFixed.height() > 0.0) {
        physValHeight = m_physValSizeFixed.height();
    }
    else if (m_physValSizeMaximum.height() > 0.0) {
        physValHeight = m_physValSizeMaximum.height();
    }
    physValHeight.convertValue(i_unit);
    return physValHeight;
}

//------------------------------------------------------------------------------
void CGraphObj::setMaximumSize( const CPhysValSize& i_physValSize )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValSize.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setMaximumSize",
        /* strAddInfo   */ strMthInArgs );

    m_physValSizeMaximum = i_physValSize;

    //if (m_physValSizeMaximum.isValid()) {
    //    CPhysValSize physValSize = getSize(m_physValSizeMaximum.unit());
    //    if (physValSize.width() > m_physValSizeMaximum.width()
    //     || physValSize.height() > m_physValSizeMaximum.height()) {
    //        if (physValSize.width() > m_physValSizeMaximum.width()) {
    //            physValSize.setWidth( m_physValSizeMaximum.width() );
    //        }
    //        if (physValSize.height() > m_physValSizeMaximum.height()) {
    //            physValSize.setHeight( m_physValSizeMaximum.height() );
    //        }
    //        setSize(physValSize);
    //    }
    //}
}

//------------------------------------------------------------------------------
bool CGraphObj::hasMaximumSize() const
//------------------------------------------------------------------------------
{
    bool bHas = false;
    if (m_physValSizeFixed.isValid()) {
        bHas = true;
    }
    else if (m_physValSizeMaximum.isValid()) {
        bHas = true;
    }
    return bHas;
}

//------------------------------------------------------------------------------
CPhysValSize CGraphObj::getMaximumSize(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    CPhysValSize physValSize;
    if (m_physValSizeFixed.isValid()) {
        physValSize = m_physValSizeFixed;
    }
    else if (m_physValSizeMaximum.isValid()) {
        physValSize = m_physValSizeMaximum;
    }
    m_pDrawingScene->convert(physValSize, i_unit);
    return physValSize;
}

//------------------------------------------------------------------------------
void CGraphObj::setFixedWidth( const CPhysVal& i_physValWidth )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValWidth.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setFixedWidth",
        /* strAddInfo   */ strMthInArgs );

    m_physValSizeFixed.setWidth(i_physValWidth);

    //if (m_physValSizeFixed.width() > 0.0) {
    //    CPhysValSize physValSize = getSize(m_physValSizeFixed.unit());
    //    if (physValSize.width() != m_physValSizeFixed.width()) {
    //        physValSize.setWidth( m_physValSizeFixed.width() );
    //        setSize(physValSize);
    //    }
    //}
}

//------------------------------------------------------------------------------
bool CGraphObj::hasFixedWidth() const
//------------------------------------------------------------------------------
{
    bool bHas = false;
    if (m_physValSizeFixed.width() > 0.0) {
        bHas = true;
    }
    else if ((m_physValSizeMinimum.width() > 0.0)
          && (m_physValSizeMinimum.width() == m_physValSizeMaximum.width())) {
        bHas = true;
    }
    return bHas;
}

//------------------------------------------------------------------------------
CPhysVal CGraphObj::getFixedWidth(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysVal physValWidth(0.0, drawingSize.unit(), drawingSize.imageCoorsResolution(drawingSize.unit()));
    if (m_physValSizeFixed.width() > 0.0) {
        physValWidth = m_physValSizeFixed.width();
    }
    else if ((m_physValSizeMinimum.width() > 0.0)
          && (m_physValSizeMinimum.width() == m_physValSizeMaximum.width())) {
        physValWidth = m_physValSizeMinimum.width();
    }
    physValWidth.convertValue(i_unit);
    return physValWidth;
}

//------------------------------------------------------------------------------
void CGraphObj::setFixedHeight( const CPhysVal& i_physValHeight )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValHeight.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setFixedHeight",
        /* strAddInfo   */ strMthInArgs );

    m_physValSizeFixed.setHeight(i_physValHeight);

    //if (m_physValSizeFixed.height() > 0.0) {
    //    CPhysValSize physValSize = getSize(m_physValSizeFixed.unit());
    //    if (physValSize.height() != m_physValSizeFixed.height()) {
    //        physValSize.setHeight( m_physValSizeFixed.height() );
    //        setSize(physValSize);
    //    }
    //}
}

//------------------------------------------------------------------------------
bool CGraphObj::hasFixedHeight() const
//------------------------------------------------------------------------------
{
    bool bHas = false;
    if (m_physValSizeFixed.height() > 0.0) {
        bHas = true;
    }
    else if ((m_physValSizeMinimum.height() > 0.0)
          && (m_physValSizeMinimum.height() == m_physValSizeMaximum.height())) {
        bHas = true;
    }
    return bHas;
}

//------------------------------------------------------------------------------
CPhysVal CGraphObj::getFixedHeight(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysVal physValHeight(0.0, drawingSize.unit(), drawingSize.imageCoorsResolution(drawingSize.unit()));
    if (m_physValSizeFixed.height() > 0.0) {
        physValHeight = m_physValSizeFixed.height();
    }
    else if ((m_physValSizeMinimum.height() > 0.0)
          && (m_physValSizeMinimum.height() == m_physValSizeMaximum.height())) {
        physValHeight = m_physValSizeMinimum.height();
    }
    physValHeight.convertValue(i_unit);
    return physValHeight;
}

//------------------------------------------------------------------------------
void CGraphObj::setFixedSize( const CPhysValSize& i_physValSize )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValSize.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setFixedSize",
        /* strAddInfo   */ strMthInArgs );

    m_physValSizeFixed = i_physValSize;

    //if (m_physValSizeFixed.isValid()) {
    //    CPhysValSize physValSize = getSize(m_physValSizeFixed.unit());
    //    if (physValSize.width() != m_physValSizeFixed.width()
    //     || physValSize.height() != m_physValSizeFixed.height()) {
    //        if (physValSize.width() != m_physValSizeFixed.width()) {
    //            physValSize.setWidth( m_physValSizeFixed.width() );
    //        }
    //        if (physValSize.height() != m_physValSizeFixed.height()) {
    //            physValSize.setHeight( m_physValSizeFixed.height() );
    //        }
    //        setSize(physValSize);
    //    }
    //}
}

//------------------------------------------------------------------------------
bool CGraphObj::hasFixedSize() const
//------------------------------------------------------------------------------
{
    bool bHas = false;
    if (m_physValSizeFixed.isValid()) {
        bHas = true;
    }
    else if (m_physValSizeMinimum.isValid() && m_physValSizeMaximum.isValid()) {
        if ((m_physValSizeMinimum.width() == m_physValSizeMaximum.width())
         && (m_physValSizeMinimum.height() == m_physValSizeMaximum.height())) {
            bHas = true;
        }
    }
    return bHas;
}

//------------------------------------------------------------------------------
CPhysValSize CGraphObj::getFixedSize(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    CPhysValSize physValSize;
    if (m_physValSizeFixed.isValid()) {
        physValSize = m_physValSizeFixed;
    }
    else if (m_physValSizeMinimum.isValid() && m_physValSizeMaximum.isValid()) {
        if ((m_physValSizeMinimum.width() == m_physValSizeMaximum.width())
         && (m_physValSizeMinimum.height() == m_physValSizeMaximum.height())) {
            physValSize = m_physValSizeMinimum;
        }
    }
    m_pDrawingScene->convert(physValSize, i_unit);
    return physValSize;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief

    The alignments will be adjusted in the order they are added.
    The order takes effect on the result. Usually the size should be adjusted
    before the positions to get relative adjustments working as expected.
*/
int CGraphObj::addAlignment( const SGraphObjAlignment& i_alignment )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_alignment.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::addAlignment",
        /* strAddInfo   */ strMthInArgs );

    int idx = m_arAlignments.size();
    m_arAlignments.append(i_alignment);

    if (m_pTree != nullptr) {
        m_pTree->onTreeEntryChanged(this);
    }
    return idx;
}

//------------------------------------------------------------------------------
int CGraphObj::getAlignmentCount() const
//------------------------------------------------------------------------------
{
    return m_arAlignments.size();
}

//------------------------------------------------------------------------------
SGraphObjAlignment CGraphObj::getAlignment( int i_idx ) const
//------------------------------------------------------------------------------
{
    if (i_idx < 0 || i_idx >= m_arAlignments.size()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "Idx:" + QString::number(i_idx) );
    }
    return m_arAlignments[i_idx];
}

//------------------------------------------------------------------------------
void CGraphObj::setAlignment( int i_idx, const SGraphObjAlignment& i_alignment )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_idx) + ": " + i_alignment.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setAlignment",
        /* strAddInfo   */ strMthInArgs );

    if (i_idx < 0 || i_idx >= m_arAlignments.size()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "Idx:" + QString::number(i_idx));
    }
    m_arAlignments[i_idx] = i_alignment;

    if (m_pTree != nullptr) {
        m_pTree->onTreeEntryChanged(this);
    }
}

//------------------------------------------------------------------------------
void CGraphObj::removeAlignment( int i_idx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_idx);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::removeAlignment",
        /* strAddInfo   */ strMthInArgs );

    if (i_idx < 0 || i_idx >= m_arAlignments.size()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "Idx:" + QString::number(i_idx) );
    }
    m_arAlignments.removeAt(i_idx);

    if (m_pTree != nullptr) {
        m_pTree->onTreeEntryChanged(this);
    }
}

//------------------------------------------------------------------------------
void CGraphObj::clearAlignments()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::clearAlignments",
        /* strAddInfo   */ "" );

    m_arAlignments.clear();

    if (m_pTree != nullptr) {
        m_pTree->onTreeEntryChanged(this);
    }
}

/*==============================================================================
public: // overridables
==============================================================================*/

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//------------------------------------------------------------------------------
/*! Accepts the current coordindates of the graphic item as the original coordinates.

    The current coordinates are in scene coordinates and define the position and size
    of the item on the scene.

    When rotating an item or resizing a group the original coordinates are important
    and will be used to calculate the resulting position and size of the item and
    its children on the scene.

    If a transformation, grouping and/or ungrouping items, resizing a group, initial
    creating or changing the position of an item has been finished it is essential
    to accept the current as the original coordinates.

    Items belonging to a group may not be absolutely positioned within the group but
    relative with alignment parameters like aligned to top, to bottom, center etc.
    If the group is resized the relative distance to the border should be kept while
    resizing the group. For this "applyGeometryChangeToChildrens" is using the
    original coordinates.
*/
void CGraphObj::acceptCurrentAsOriginalCoors()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::acceptCurrentAsOriginalCoors",
        /* strAddInfo   */ "" );

    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
    {
        m_ptPosOrig = pGraphicsItem->pos();
    }

    m_sizOrig = m_rctCurr.size();
    m_fRotAngleOrig_deg = m_fRotAngleCurr_deg;
    m_ptRotOriginOrig = m_ptRotOriginCurr;
    //m_fScaleFacXOrig = m_fScaleFacXCurr;
    //m_fScaleFacYOrig = m_fScaleFacYCurr;

    m_bHasValidOrigCoors = true;

    if (m_pTree != nullptr) {
        m_pTree->onTreeEntryChanged(this);
    }
} // acceptCurrentAsOriginalCoors
#endif

/*==============================================================================
public: // must overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the position of the item in parent coordinates in the current
           unit of the drawing scene.

    For further details see getPos below.
*/
CPhysValPoint CGraphObj::getPos() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return getPos(drawingSize.unit());
}

//------------------------------------------------------------------------------
/*! @brief Returns the position of the item in parent coordinates in the given unit.
           If the item has no parent, its position is given in scene coordinates.

    @param i_unit [in]
        Desired unit.

    @return Position of the item in parent coordinates.
*/
CPhysValPoint CGraphObj::getPos(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
#pragma message(__TODO__"Pure virtual")
    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");
    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
    if (pGraphicsItem == nullptr) {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    }
    QPointF ptPos_px = pGraphicsItem->pos();
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysValPoint physValPoint(ptPos_px, drawingSize.imageCoorsResolutionInPx(), Units.Length.px);
    m_pDrawingScene->convert(physValPoint, i_unit);
    return physValPoint;
}

////------------------------------------------------------------------------------
///*! @brief Pure virtual method which must be overridden by derived class to set
//           the width of the object.
//
//    This method is used by groups to resize their children.
//    How the width is interpreted depends on the type of object.
//    For lines for example width is interpreted as "dX".
//
//    @param [in] i_physValWidth
//        New width of the object.
//*/
//void CGraphObj::setWidth( const CPhysVal& i_physValWidth )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = i_physValWidth.toString();
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::setWidth",
//        /* strAddInfo   */ strMthInArgs );
//#pragma message(__TODO__"Pure virtual")
//    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");
//    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
//    if (pGraphicsItem == nullptr) {
//        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
//    }
//}
//
////------------------------------------------------------------------------------
///*! @brief Pure virtual method which must be overridden by derived class to
//           return the width of the object.
//
//    This method is used by groups to resize their children.
//    How the width is interpreted depends on the type of object.
//    For lines for example width is interpreted as "dX".
//
//    @return Width of the object.
//*/
//CPhysVal CGraphObj::getWidth( const CUnit& i_unit, ECoordinatesVersion i_version ) const
////------------------------------------------------------------------------------
//{
//#pragma message(__TODO__"Pure virtual")
//    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");
//    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
//    if (pGraphicsItem == nullptr) {
//        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
//    }
//    if (i_version == ECoordinatesVersion::Original) {
//        // This method may only be called if overridden by derived classes.
//        throw CException(__FILE__, __LINE__, EResultInvalidMethodCall);
//    }
//    double fWidth_px = getBoundingRect(true).width();
//    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
//    CPhysVal physValWidth(fWidth_px, Units.Length.px, drawingSize.imageCoorsResolutionInPx());
//    // A value like width and height can be directly converted without the drawing size.
//    physValWidth.convertValue(i_unit);
//    return physValWidth;
//}
//
////------------------------------------------------------------------------------
///*! @brief Pure virtual method which must be overridden by derived class to set
//           the height of the object.
//
//    This method is used by a group to resize its children.
//    How the height is interpreted depends on the type of object.
//    For lines for example width is interpreted as "dY".
//
//    @param [in] i_physValHeight
//        New height of the object.
//*/
//void CGraphObj::setHeight( const CPhysVal& i_physValHeight )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = i_physValHeight.toString();
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::setHeight",
//        /* strAddInfo   */ strMthInArgs );
//#pragma message(__TODO__"Pure virtual")
//    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");
//    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
//    if (pGraphicsItem == nullptr) {
//        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
//    }
//}
//
////------------------------------------------------------------------------------
///*! @brief Pure virtual method which must be overridden by derived class to
//           return the width of the object.
//
//    This method is used by groups to resize their children.
//    How the width is interpreted depends on the type of object.
//    For lines for example height is interpreted as "dY".
//
//    @return Height of the object.
//*/
//CPhysVal CGraphObj::getHeight( const CUnit& i_unit, ECoordinatesVersion i_version ) const
////------------------------------------------------------------------------------
//{
//#pragma message(__TODO__"Pure virtual")
//    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");
//    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
//    if (pGraphicsItem == nullptr) {
//        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
//    }
//    if (i_version == ECoordinatesVersion::Original) {
//        // This method may only be called if overridden by derived classes.
//        throw CException(__FILE__, __LINE__, EResultInvalidMethodCall);
//    }
//    double fHeight_px = getBoundingRect(true).height();
//    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
//    CPhysVal physValHeight(fHeight_px, Units.Length.px, drawingSize.imageCoorsResolutionInPx());
//    // A value like width and height can be directly converted without the drawing size.
//    physValHeight.convertValue(i_unit);
//    return physValHeight;
//}
//
////------------------------------------------------------------------------------
///*! @brief Pure virtual method which must be overridden by derived class to set
//           the size in width and height of the object.
//
//    This method is used by a group to resize its children.
//    How the size is interpreted depends on the type of object.
//    For lines for example size is interpreted as "dX/dY".
//
//    @param [in] i_physValWidth
//        New width of the object.
//    @param [in] i_physValHeight
//        New height of the object.
//*/
//void CGraphObj::setSize( const CPhysVal& i_physValWidth, const CPhysVal& i_physValHeight )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = "Width: " + i_physValWidth.toString() + ", Height: " + i_physValHeight.toString();
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::setSize",
//        /* strAddInfo   */ strMthInArgs );
//#pragma message(__TODO__"Pure virtual")
//    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");
//    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
//    if (pGraphicsItem == nullptr) {
//        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
//    }
//}
//
////------------------------------------------------------------------------------
///*! @brief Pure virtual method which must be overridden by derived class to set
//           the size in width and height of the object.
//
//    This method is used by a group to resize its children.
//    How the size is interpreted depends on the type of object.
//    For lines for example size is interpreted as "dX/dY".
//
//    @param [in] i_physValSize
//        New size of the object.
//*/
//void CGraphObj::setSize( const CPhysValSize& i_physValSize )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = i_physValSize.toString();
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::setSize",
//        /* strAddInfo   */ strMthInArgs );
//#pragma message(__TODO__"Pure virtual")
//    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");
//    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
//    if (pGraphicsItem == nullptr) {
//        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
//    }
//}
//
////------------------------------------------------------------------------------
///*! @brief Pure virtual method which must be overridden by derived class to
//           return the size of the object.
//
//    This method is used by groups to resize their children.
//    How the size is interpreted depends on the type of object.
//    For lines for example size is interpreted as "dX/dY".
//
//    @return Size of the object.
//*/
//CPhysValSize CGraphObj::getSize( const CUnit& i_unit, ECoordinatesVersion i_version ) const
////------------------------------------------------------------------------------
//{
//#pragma message(__TODO__"Pure virtual")
//    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");
//    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
//    if (pGraphicsItem == nullptr) {
//        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
//    }
//    if (i_version == ECoordinatesVersion::Original) {
//        // This method may only be called if overridden by derived classes.
//        throw CException(__FILE__, __LINE__, EResultInvalidMethodCall);
//    }
//    QSizeF size_px = getBoundingRect(true).size();
//    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
//    CPhysValSize physValSize(size_px, drawingSize.imageCoorsResolutionInPx(), Units.Length.px);
//    // Width and height could be directly converted without the drawing size.
//    // But the drawing scene provides a method to convert CPhysValSize which we can also use.
//    m_pDrawingScene->convert(physValSize, i_unit);
//    return physValSize;
//}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Maps the given point, which is in this item's coordinate system defined
           in pixels, to the physical value in the parent's coordinate system,
           and returns the mapped coordinate in the unit of the drawing scene.

    Internally the graphics items coordinates are in pixel coordinates given
    relative to the center point of its bounding rectangle.

    For the user the coordinates must be provided relative to the origin point of
    their parent, which is either the scene or a parent group.

    Depending on the Y-Scale orientation (TopDown or BottomUp) the origin of the
    parent scene or parent group is either the top left or bottom left corner.

    @param [in] i_pt
        Point in the item coordinates.

    @return Point in the unit of the drawing scene as it should be provided to the user.
*/
CPhysValPoint CGraphObj::mapToPhysValPoint(const QPointF& i_pt) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    // First map the point, which is in this item's coordinate system, to its parent's
    // coordinate system. If the item has no parent, point will be mapped to the scene's
    // coordinate system.
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    QPointF pt = pGraphicsItemThis->mapToParent(i_pt);
    QGraphicsItem* pGraphicsItemParent = pGraphicsItemThis->parentItem();
    CGraphObjGroup* pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(pGraphicsItemParent);
    // If the item belongs to a group ...
    if (pGraphObjGroup != nullptr) {
        // The point coordinates are relative to the origin of the parent graphics item
        // which is the center point of the parents bounding rectangle. The pixel coordinates
        // got to mapped to the top left corner of the parent's bounding rectangle.
        QRectF rectBoundingParent = pGraphObjGroup->getBoundingRect();
        pt -= rectBoundingParent.topLeft();
    }
    // Now point is in pixel coordinates relative to the top left corner of it's parent.
    CPhysValPoint physValPoint(pt, drawingSize.imageCoorsResolutionInPx(), Units.Length.px);
    // If the drawing scene don't use pixels but a metric system ..
    if (Units.Length.isMetricUnit(drawingSize.unit())) {
        // .. the coordinates got to be converted into metric unit taken the Y-Scale
        // orientation into account.
        // If the item belongs to a group ...
        if (pGraphObjGroup != nullptr) {
            // .. let the group convert the pixel values into the unit of the drawing scene.
            physValPoint = pGraphObjGroup->convert(physValPoint);
        }
        // If the item is not a child of a group ...
        else {
            // .. let the scene convert the pixel values into it's unit.
            physValPoint = m_pDrawingScene->convert(physValPoint);
        }
    }
    return physValPoint;
}

//------------------------------------------------------------------------------
CPhysValLine CGraphObj::mapToPhysValLine(const QLineF& i_line) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    // First map the line, which is in this item's coordinate system, to its parent's
    // coordinate system. If the item has no parent, line will be mapped to the scene's
    // coordinate system.
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    QPointF pt1 = pGraphicsItemThis->mapToParent(i_line.p1());
    QPointF pt2 = pGraphicsItemThis->mapToParent(i_line.p2());
    QGraphicsItem* pGraphicsItemParent = pGraphicsItemThis->parentItem();
    CGraphObjGroup* pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(pGraphicsItemParent);
    // If the item belongs to a group ...
    if (pGraphObjGroup != nullptr) {
        // The line coordinates are relative to the origin of the parent graphics item
        // which is the center point of the parents bounding rectangle. The pixel coordinates
        // got to mapped to the top left corner of the parent's bounding rectangle.
        QRectF rectBoundingParent = pGraphObjGroup->getBoundingRect();
        pt1 -= rectBoundingParent.topLeft();
        pt2 -= rectBoundingParent.topLeft();
    }
    // Now line is in pixel coordinates relative to the top left corner of it's parent.
    CPhysValLine physValLine(pt1, pt2, drawingSize.imageCoorsResolutionInPx(), Units.Length.px);
    // If the drawing scene don't use pixels but a metric system ..
    if (Units.Length.isMetricUnit(drawingSize.unit())) {
        // .. the coordinates got to be converted into metric unit taken the Y-Scale
        // orientation into account.
        // If the item belongs to a group ...
        if (pGraphObjGroup != nullptr) {
            // .. let the group convert the pixel values into the unit of the drawing scene.
            physValLine = pGraphObjGroup->convert(physValLine);
        }
        // If the item is not a child of a group ...
        else {
            // .. let the scene convert the pixel values into it's unit.
            physValLine = m_pDrawingScene->convert(physValLine);
        }
    }
    return physValLine;
}

//------------------------------------------------------------------------------
CPhysValRect CGraphObj::mapToPhysValRect(const QRectF& i_rect) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    // First map the rectangle, which is in this item's coordinate system, to its parent's
    // coordinate system. If the item has no parent, rect will be mapped to the scene's
    // coordinate system.
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    QPointF ptTL = pGraphicsItemThis->mapToParent(i_rect.topLeft());
    QPointF ptBR = pGraphicsItemThis->mapToParent(i_rect.bottomRight());
    QGraphicsItem* pGraphicsItemParent = pGraphicsItemThis->parentItem();
    CGraphObjGroup* pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(pGraphicsItemParent);
    // If the item belongs to a group ...
    if (pGraphObjGroup != nullptr) {
        // The line coordinates are relative to the origin of the parent graphics item
        // which is the center point of the parents bounding rectangle. The pixel coordinates
        // got to mapped to the top left corner of the parent's bounding rectangle.
        QRectF rectBoundingParent = pGraphObjGroup->getBoundingRect();
        ptTL -= rectBoundingParent.topLeft();
        ptBR -= rectBoundingParent.topLeft();
    }
    // Now recte is in pixel coordinates relative to the top left corner of it's parent.
    CPhysValRect physValRect(ptTL, ptBR, drawingSize.imageCoorsResolutionInPx(), Units.Length.px);
    // If the drawing scene don't use pixels but a metric system ..
    if (Units.Length.isMetricUnit(drawingSize.unit())) {
        // .. the coordinates got to be converted into metric unit taken the Y-Scale
        // orientation into account.
        // If the item belongs to a group ...
        if (pGraphObjGroup != nullptr) {
            // .. let the group convert the pixel values into the unit of the drawing scene.
            physValRect = pGraphObjGroup->convert(physValRect);
        }
        // If the item is not a child of a group ...
        else {
            // .. let the scene convert the pixel values into it's unit.
            physValRect = m_pDrawingScene->convert(physValRect);
        }
    }
    return physValRect;
}

/*==============================================================================
public: // must overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Pure virtual method which must be overridden by derived classes to
           return the current bounding rectangle of the object in local coordinates.

    @return Bounding rectangle in local coordinates.
*/
QRectF CGraphObj::getBoundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "getBoundingRect",
        /* strAddInfo   */ "" );

#pragma message(__TODO__"Pure virtual")
    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");

    QRectF rctBounding;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

/*==============================================================================
public: // overridables
==============================================================================*/

////------------------------------------------------------------------------------
///*! @brief Pure virtual method which must be overridden by derived classes to
//           return the original bounding rectangle of the object in local coordinates.
//
//    This method is used by a group to resize its children.
//
//    Please refer to documentation about the difference between current (transformed)
//    and original coordinates at base class CGraphObj.
//
//    @return Original bounding rectangle in local coordinates.
//*/
//QRectF CGraphObj::getOriginalBoundingRectInParent() const
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "getOriginalBoundingRectInParent",
//        /* strAddInfo   */ "" );
//
//    QRectF rctBounding = getCurrentBoundingRect();
//    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
//        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
//    }
//    return rctBounding;
//}

////------------------------------------------------------------------------------
///*! @brief Pure virtual method which must be overridden by derived classes to
//           set the bounding rectangle of the object in local coordinates.
//
//    This method is used by a group to resize its children.
//
//    @param [in] i_rectBounding
//        New bounding rectangle of the object in local coordinates.
//*/
//void CGraphObj::setCurrentBoundingRectInParent(const QRectF& i_rectBounding)
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = qRect2Str(i_rectBounding);
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "setCurrentBoundingRectInParent",
//        /* strAddInfo   */ strMthInArgs );
//}

/*==============================================================================
public: // must overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::setGroupScale(double i_fXScale, double i_fYScale)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fXScale) + ", " + QString::number(i_fYScale);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setGroupScale",
        /* strAddInfo   */ strMthInArgs );
    #pragma message(__TODO__"Pure virtual")
    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");
    m_transformByGroup.scale(i_fXScale, i_fYScale);
}

////------------------------------------------------------------------------------
//void CGraphObj::setRotationAngleInDegree( double i_fRotAngle_deg )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = QString::number(i_fRotAngle_deg, 'f', 1);
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::setRotationAngleInDegree",
//        /* strAddInfo   */ strMthInArgs );
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//    m_fRotAngleCurr_deg = i_fRotAngle_deg;
//#endif
//
//    //updateTransform();
//}

////------------------------------------------------------------------------------
//double CGraphObj::getRotationAngleInDegree()
////------------------------------------------------------------------------------
//{
//    double fRotAngle_deg = 0.0;
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//    if (i_version == ECoordinatesVersion::Original) {
//        fRotAngle_deg = m_fRotAngleOrig_deg;
//    }
//    else {
//        fRotAngle_deg = m_fRotAngleCurr_deg;
//    }
//#endif
//    return fRotAngle_deg;
//}

/*==============================================================================
public: // overridables
==============================================================================*/

////------------------------------------------------------------------------------
//void CGraphObj::setScaleFacX( double i_fScaleFacX )
////------------------------------------------------------------------------------
//{
//    m_fScaleFacXCurr = i_fScaleFacX;
//
//    updateTransform();
//
//} // setScaleFacX

////------------------------------------------------------------------------------
//void CGraphObj::setScaleFacY( double i_fScaleFacY )
////------------------------------------------------------------------------------
//{
//    m_fScaleFacYCurr = i_fScaleFacY;
//
//    updateTransform();
//
//} // setScaleFacY

////------------------------------------------------------------------------------
//void CGraphObj::setScaleFactors( double i_fScaleFacX, double i_fScaleFacY )
////------------------------------------------------------------------------------
//{
//    m_fScaleFacXCurr = i_fScaleFacX;
//    m_fScaleFacYCurr = i_fScaleFacY;
//
//    updateTransform();
//
//} // setScaleFactors

////------------------------------------------------------------------------------
//double CGraphObj::getScaleFacX( ECoordinatesVersion i_version )
////------------------------------------------------------------------------------
//{
//    double fScaleFacX = 0.0;
//
//    if( i_version == ECoordinatesVersion::Original )
//    {
//        fScaleFacX = m_fScaleFacXOrig;
//    }
//    else
//    {
//        fScaleFacX = m_fScaleFacXCurr;
//    }
//    return fScaleFacX;
//
//} // getScaleFacX

////------------------------------------------------------------------------------
//double CGraphObj::getScaleFacY( ECoordinatesVersion i_version )
////------------------------------------------------------------------------------
//{
//    double fScaleFacY = 0.0;
//
//    if( i_version == ECoordinatesVersion::Original )
//    {
//        fScaleFacY = m_fScaleFacYOrig;
//    }
//    else
//    {
//        fScaleFacY = m_fScaleFacYCurr;
//    }
//    return fScaleFacY;
//
//} // getScaleFacY

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the current edit mode of the object.

    The edit mode defines how mouse and key events are handled.
*/
void CGraphObj::setEditMode(const CEnumEditMode& i_eMode)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_eMode.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setEditMode",
        /* strAddInfo   */ strMthInArgs );

    if (m_editMode != i_eMode) {
        m_editMode = i_eMode;
        if (m_editMode == EEditMode::CreatingByMouseEvents) {
            QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
            if (pGraphicsItemThis != nullptr) {
                // Immeadiately select the object to create the selection points.
                // Following mouse move press, mouse move and mouse release events will
                // be forwarded by the scene to the selection point responsible for
                // resizing the obejct (the top most selection point most recently created).
                pGraphicsItemThis->setSelected(true);
                // The object is under construction. Hover events will not be accepted.
                // Only when no drawing tool is selected in the drawing scene hover
                // events may be accepted.
                pGraphicsItemThis->setAcceptHoverEvents(false);
            }
        }
        emit_editModeChanged(m_editMode);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the current edit mode of the object.

    The edit mode defines how mouse and key events are handled.
*/
CEnumEditMode CGraphObj::editMode() const
//------------------------------------------------------------------------------
{
    return m_editMode;
}

////------------------------------------------------------------------------------
//void CGraphObj::setEditResizeMode( EEditResizeMode i_editResizeMode )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = CEnumEditResizeMode(i_editResizeMode).toString();
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::setEditResizeMode",
//        /* strAddInfo   */ strMthInArgs );
//
//    m_editResizeMode = i_editResizeMode;
//}

//------------------------------------------------------------------------------
/*! @brief Returns the current edit mode of the object.

    If the object is currently being resized (editMode == Resize) the resize mode
    defines how the object will be resized.
*/
//CEnumEditResizeMode CGraphObj::editResizeMode() const
////------------------------------------------------------------------------------
//{
//    return m_editResizeMode;
//}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::setIsHit(bool i_bIsHit)
//-----------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bIsHit);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setIsHit",
        /* strAddInfo   */ strMthInArgs );

    if (m_bIsHit != i_bIsHit) {
        m_bIsHit = i_bIsHit;
        QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
        if (pGraphicsItemThis != nullptr) {
            pGraphicsItemThis->update();
        }
    }
}

//------------------------------------------------------------------------------
bool CGraphObj::isHit() const
//------------------------------------------------------------------------------
{
    return m_bIsHit;
}

////------------------------------------------------------------------------------
//bool CGraphObj::isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjIsHit, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = "Point:" + point2Str(i_pt) +
//            ", HitInfo, " + QString(o_pHitInfo == nullptr ? "null" : pointer2Str(o_pHitInfo));
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjIsHit,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "isHit",
//        /* strAddInfo   */ strMthInArgs );
//
//    bool bIsHit = false;
//
//    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
//    if (pGraphicsItem != nullptr) {
//        if (pGraphicsItem->isSelected()) {
//            bIsHit = isPolygonSelectionPointHit(i_pt,o_pHitInfo);
//            if (!bIsHit) {
//                bIsHit = isBoundingRectSelectionPointHit(
//                    /* pt               */ i_pt,
//                    /* iSelPtsCount     */ -1,
//                    /* pSelPts          */ nullptr,
//                    /* pGraphObjHitInfo */ o_pHitInfo );
//            }
//        }
//        if (!bIsHit) {
//            if (pGraphicsItem->isSelected() || m_drawSettings.getFillStyle() == EFillStyle::SolidPattern) {
//                bIsHit = pGraphicsItem->contains(i_pt);
//                if (o_pHitInfo != nullptr) {
//                    //o_pHitInfo->m_editMode = EEditMode::Move;
//                    //o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
//                    o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
//                    o_pHitInfo->m_idxPolygonShapePoint = -1;
//                    o_pHitInfo->m_idxLineSegment = -1;
//                    o_pHitInfo->m_ptSelected = i_pt;
//                }
//            }
//        }
//        if (bIsHit && o_pHitInfo != nullptr) {
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//            o_pHitInfo->setCursor( Math::deg2Rad(m_fRotAngleCurr_deg) );
//#endif
//        }
//    }
//
//    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
//        QString strMthOutArgs;
//        if (o_pHitInfo != nullptr) {
//            strMthOutArgs = "HitInfo {" + o_pHitInfo->toString() + "}";
//            mthTracer.setMethodOutArgs(strMthOutArgs);
//        }
//        mthTracer.setMethodReturn(bIsHit);
//    }
//    return bIsHit;
//}

//------------------------------------------------------------------------------
void CGraphObj::setIsHighlighted(bool i_bIsHighlighted)
//-----------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bIsHighlighted);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setIsHighlighted",
        /* strAddInfo   */ strMthInArgs );

    if (m_bIsHighlighted != i_bIsHighlighted) {
        m_bIsHighlighted = i_bIsHighlighted;
        QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
        if (pGraphicsItemThis != nullptr) {
            pGraphicsItemThis->update();
        }
        if (m_bIsHighlighted) {
            tracePositionInfo(mthTracer);
        }
    }
}

//------------------------------------------------------------------------------
bool CGraphObj::isHighlighted() const
//------------------------------------------------------------------------------
{
    return m_bIsHighlighted;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the proposed cursor shape for the given point.

    The cursor shape depends which shape point of the object has been hit.
    If a selection point has been hit, the position and type of selection point
    defines the cursor shape.

    The base implementation just checks whether a selection point has been hit
    and returns the cursor shape proposed by the selection point.

    @param i_pt [in] Point to be check in local coordinates.
*/
QCursor CGraphObj::getProposedCursor(const QPointF& i_pt) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCursor, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Point:" + point2Str(i_pt);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCursor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "getProposedCursor",
        /* strAddInfo   */ strMthInArgs );

    QCursor cursor = Qt::ArrowCursor;
    if (m_bIsHit) {
        cursor = Qt::SizeAllCursor;
    }
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    if (pGraphicsItemThis != nullptr) {
        CGraphObjSelectionPoint* pGraphObjSelPtHit = getSelectionPointHit(i_pt);
        if (pGraphObjSelPtHit != nullptr) {
            cursor = pGraphObjSelPtHit->getProposedCursor(i_pt);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs(qCursorShape2Str(cursor.shape()));
    }
    return cursor;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Brings the object into front of all objects intersecting the bounding
           rectangle of this object.

    The method does not set the member "m_fZValue" as this member is used to
    restore the stacking order on deselecting the object.

    @return The new ZValue.
*/
double CGraphObj::bringToFront()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::bringToFront",
        /* strAddInfo   */ "" );

    double fZValue = 0.0;
    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr) {
        QRectF rctBounding = pGraphicsItem->boundingRect();
        rctBounding = pGraphicsItem->mapRectToScene(rctBounding);
        QList<QGraphicsItem*> arpGraphicsItemsIntersected = m_pDrawingScene->items(rctBounding);
        m_pDrawingScene->bringToFront(pGraphicsItem, arpGraphicsItemsIntersected);
        fZValue = pGraphicsItem->zValue();
        m_arfZValues[static_cast<int>(ERowVersion::Current)] = fZValue;
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(QString::number(fZValue));
    }
    return fZValue;
}

//------------------------------------------------------------------------------
/*! @brief Sets the stacking order value (zValue) of the graphical object.

    You must call this method instead of setZValue of QGraphicsItem.

    @param [in] i_fZValue
    @param [in] i_version
         Two values are stored. The original version is the ZValue which
         will be initially used and if resetStackingOrderValueToOriginalValue
         is called. The current version is the ZValue which is currently set
         at the graphics item. The current and original versions are mainly
         used to temporarily modify the ZValue to bring the object in front
         and back again.

    @return zValue which was set before at the graphics item
*/
double CGraphObj::setStackingOrderValue(double i_fZValue, ERowVersion i_version)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fZValue, 'f', 1) + ", " + CEnumRowVersion(i_version).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setStackingOrderValue",
        /* strAddInfo   */ strMthInArgs );

    double fZValuePrev = 0.0;
    m_arfZValues[static_cast<int>(i_version)] = i_fZValue;
    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr) {
        fZValuePrev = pGraphicsItem->zValue();
        pGraphicsItem->setZValue(i_fZValue);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev: " + QString::number(fZValuePrev, 'f', 1));
    }
    return fZValuePrev;
}

//------------------------------------------------------------------------------
double CGraphObj::getStackingOrderValue(ERowVersion i_version) const
//------------------------------------------------------------------------------
{
    return m_arfZValues[static_cast<int>(i_version)];
}

//------------------------------------------------------------------------------
/*! @brief Sets the stacking order value (zValue) of the graphical object
           to the original value.

    Two values are stored. The original version is the ZValue which
    will be initially used and if resetStackingOrderValueToOriginalValue
    is called. The current version is the ZValue which is currently set
    at the graphics item. The current and original versions are mainly
    used to temporarily modify the ZValue to bring the object in front
    and back again.

    @return zValue which was set before at the graphics item
*/
double CGraphObj::resetStackingOrderValueToOriginalValue()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Orig: " + QString::number(m_arfZValues[static_cast<int>(ERowVersion::Original)], 'f', 1);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::resetStackingOrderValueToOriginalValue",
        /* strAddInfo   */ strMthInArgs );

    double fZValuePrev = 0.0;
    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr) {
        fZValuePrev = pGraphicsItem->zValue();
        m_arfZValues[static_cast<int>(ERowVersion::Current)] =
            m_arfZValues[static_cast<int>(ERowVersion::Original)];
        pGraphicsItem->setZValue(m_arfZValues[static_cast<int>(ERowVersion::Original)]);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev: " + QString::number(fZValuePrev, 'f', 1));
    }
    return fZValuePrev;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the selection point object which is under the given point.

    @return Selection point hit or nullptr, if no selection point is under the given point.
*/
CGraphObjSelectionPoint* CGraphObj::getSelectionPointHit(const QPointF& i_pt) const
//------------------------------------------------------------------------------
{
    CGraphObjSelectionPoint* pGraphObjSelPtHit = nullptr;
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    if (pGraphicsItemThis != nullptr) {
        for (CGraphObjSelectionPoint* pGraphObjSelPt : m_arpSelPtsPolygon) {
            if (pGraphObjSelPt != nullptr) {
                QPointF ptTmp = pGraphicsItemThis->mapToItem(pGraphObjSelPt, i_pt);
                if (pGraphObjSelPt->contains(ptTmp)) {
                    pGraphObjSelPtHit = pGraphObjSelPt;
                    break;
                }
            }
        }
        if (pGraphObjSelPtHit == nullptr) {
            for (CGraphObjSelectionPoint* pGraphObjSelPt : m_arpSelPtsBoundingRect) {
                if (pGraphObjSelPt != nullptr) {
                    QPointF ptTmp = pGraphicsItemThis->mapToItem(pGraphObjSelPt, i_pt);
                    if (pGraphObjSelPt->contains(ptTmp)) {
                        pGraphObjSelPtHit = pGraphObjSelPt;
                        break;
                    }
                }
            }
        }
    }
    return pGraphObjSelPtHit;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the coordinates of the selection point in scene coordinates.
*/
QPointF CGraphObj::getSelectionPointCoorsInSceneCoors( ESelectionPoint i_selPt ) const
//------------------------------------------------------------------------------
{
    QPointF ptScenePos;
    QRectF rectBounding = getBoundingRect();
    QPointF ptPos = ZS::Draw::getSelectionPointCoors(rectBounding, i_selPt);
    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr) {
        ptScenePos = pGraphicsItem->mapToScene(ptPos);
    }
    return ptScenePos;
}

//------------------------------------------------------------------------------
/*! @brief Returns the coordinates of the selection point in scene coordinates.

    For polygon shapes the method got to be overridden and should return
    the corresponding coordinates of the polygon point.
    The default implementation returns the center of the bounding rectangle.
*/
QPointF CGraphObj::getSelectionPointCoorsInSceneCoors( int i_idxPt ) const
//------------------------------------------------------------------------------
{
    QPointF ptScenePos;
    QRectF rectBounding = getBoundingRect();
    QPointF ptPos = ZS::Draw::getSelectionPointCoors(rectBounding, ESelectionPoint::Center);
    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr) {
        ptScenePos = pGraphicsItem->mapToScene(ptPos);
    }
    return ptScenePos;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the polar coordinates in length and angle of the given point
           to the selection point of the graphical object.

    How the angle of anchor lines to selection points is interpreted depends on
    the graphical object type and the selection point.

    This method must be reimplemented by derived classes to ensure that labels
    linked to the selection point are always at the same relative position
    no matter how the object will be rotated.

    @param [in] i_pt
        Point in scene coordinates to which the polar coordinates from the
        given selection point have to be returned.
    @param [in] i_selPt
        Selection point on the bounding rectangle.
*/
SPolarCoors CGraphObj::getPolarCoorsToSelectionPointFromSceneCoors(const QPointF& i_pt, ESelectionPoint i_selPt) const
//------------------------------------------------------------------------------
{
#pragma message(__TODO__"pure virtual")
    SPolarCoors polarCoors;
    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");
    return polarCoors;
}

//------------------------------------------------------------------------------
/*! @brief Returns the polar coordinates in length and angle of the given point
           to the selection point of the graphical object.

    See above for more details.
*/
SPolarCoors CGraphObj::getPolarCoorsToSelectionPointFromSceneCoors(const QPointF& i_pt, int i_idxPt) const
//------------------------------------------------------------------------------
{
#pragma message(__TODO__"pure virtual")
    SPolarCoors polarCoors;
    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");
    return polarCoors;
}

//------------------------------------------------------------------------------
/*! @brief Returns a line with the given length and angle with the start point (P1)
           at the given selection point in scene coordinates.

    This method must be reimplemented by derived classes to ensure that labels
    linked to the selection point are always at the same relative position
    no matter how the object will be rotated.

    This behaviour can only be managed if the labels save the length and the
    angle of the anchor lines to the selection point. But how the angle for
    a selection point is interpreted depends on the graphical object type.
    The example below shows how a line would interprete the angle to the
    line end points.
                                  +-------+      AnchorLine
                                  | Label |          + P2
                                  +---|---+          |
                                   90°| Length       | 
            +-- Line -----------------+              + P1
            P1                        P2

            If rotated by -90 (or +270) degrees:

                 P1 +
                    |
                   Line
                    |
                    | 90° +-------+
                 P2 +-----|-Label |
                   Length +-------+

       AnchorLine   +-----+
                   P1     P2
*/
QLineF CGraphObj::getAnchorLineToSelectionPointFromPolarInSceneCoors(
    const SPolarCoors& i_polarCoors, ESelectionPoint i_selPt) const
//------------------------------------------------------------------------------
{
#pragma message(__TODO__"pure virtual")
    QLineF anchorLine;
    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");
    return anchorLine;
}

//------------------------------------------------------------------------------
/*! @brief Returns a line with the given length and angle with the start point (P1)
           at the given selection point in scene coordinates.

    See above for more details.
*/
QLineF CGraphObj::getAnchorLineToSelectionPointFromPolarInSceneCoors(
    const SPolarCoors& i_polarCoors, int i_idxPt) const
//------------------------------------------------------------------------------
{
#pragma message(__TODO__"pure virtual")
    QLineF anchorLine;
    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");
    return anchorLine;
}

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief

    Not just hides but also destroys the selection points.
*/
void CGraphObj::hideSelectionPoints( ESelectionPoints i_selPts )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = selectionPoints2Str(i_selPts);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::hideSelectionPoints",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr)
    {
        for (int idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++)
        {
            ESelectionPoint selPt = static_cast<ESelectionPoint>(idxSelPt);
            bool bHideSelPt = false;

            if (idxSelPt >= ESelectionPointCornerMin && idxSelPt <= ESelectionPointCornerMax) {
                if (i_selPts & ESelectionPointsBoundingRectCorner) {
                    bHideSelPt = true;
                }
            }
            else if (idxSelPt >= ESelectionPointLineCenterMin && idxSelPt <= ESelectionPointLineCenterMax) {
                if (i_selPts & ESelectionPointsBoundingRectLineCenter) {
                    bHideSelPt = true;
                }
            }
            else if (selPt == ESelectionPoint::Center) {
                if (i_selPts & ESelectionPointsBoundingRectCenter) {
                    bHideSelPt = true;
                }
            }
            else if (idxSelPt >= ESelectionPointRotateMin && idxSelPt <= ESelectionPointRotateMax) {
                if (i_selPts & ESelectionPointsBoundingRectRotate) {
                    bHideSelPt = true;
                }
            }

            if (bHideSelPt) {
                // Deleting child objects leads to itemChange and an updateToolTip call
                // accessing the array of selection points.
                CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];
                if (pGraphObjSelPt != nullptr) {
                    m_arpSelPtsBoundingRect[idxSelPt] = nullptr;
                    //m_pDrawingScene->removeGraphObj(pGraphObjSelPt); // the dtor of the selection point removes itself from the drawing scene
                    delete pGraphObjSelPt;
                    pGraphObjSelPt = nullptr;
                }
            }
        }

        if (i_selPts & ESelectionPointsPolygonShapePoints) {
            if (m_arpSelPtsPolygon.size() > 0) {
                for (int idxSelPt = m_arpSelPtsPolygon.size()-1; idxSelPt >= 0; idxSelPt--) {
                    // Deleting child objects leads to itemChange and an updateToolTip call
                    // accessing the array of selection points.
                    CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
                    m_arpSelPtsPolygon[idxSelPt] = nullptr;
                    //m_pDrawingScene->removeGraphObj(pGraphObjSelPt); // the dtor of the selection point (dtor of CGraphObj) removes itself from the drawing scene
                    delete pGraphObjSelPt;
                    pGraphObjSelPt = nullptr;
                }
                m_arpSelPtsPolygon.clear();
            }
        }
    }
} // hideSelectionPoints

////------------------------------------------------------------------------------
//void CGraphObj::bringSelectionPointsToFront( ESelectionPoints i_selPts )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = selectionPoints2Str(i_selPts);
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::bringSelectionPointsToFront",
//        /* strAddInfo   */ strMthInArgs );
//
//    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
//
//    if (pGraphicsItem != nullptr)
//    {
//        for (int idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++)
//        {
//            ESelectionPoint selPt = static_cast<ESelectionPoint>(idxSelPt);
//            bool bBringToFront = false;
//
//            if (idxSelPt >= ESelectionPointCornerMin && idxSelPt <= ESelectionPointCornerMax) {
//                if (i_selPts & ESelectionPointsBoundingRectCorner) {
//                    bBringToFront = true;
//                }
//            }
//            else if (idxSelPt >= ESelectionPointLineCenterMin && idxSelPt <= ESelectionPointLineCenterMax) {
//                if (i_selPts & ESelectionPointsBoundingRectLineCenter) {
//                    bBringToFront = true;
//                }
//            }
//            else if (selPt == ESelectionPoint::Center) {
//                if (i_selPts & ESelectionPointsBoundingRectCenter) {
//                    bBringToFront = true;
//                }
//            }
//            else if (idxSelPt >= ESelectionPointRotateMin && idxSelPt <= ESelectionPointRotateMax) {
//                if (i_selPts & ESelectionPointsBoundingRectRotate) {
//                    bBringToFront = true;
//                }
//            }
//
//            if (bBringToFront) {
//                CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];
//                if (pGraphObjSelPt != nullptr) {
//                    QRectF rct = pGraphObjSelPt->rect();
//                    rct = pGraphObjSelPt->mapRectToScene(rct);
//                    QList<QGraphicsItem*> arpGraphicsItemsIntersected = m_pDrawingScene->items(rct);
//                    m_pDrawingScene->bringToFront(pGraphObjSelPt, arpGraphicsItemsIntersected);
//                }
//            }
//        }
//
//        if (i_selPts & ESelectionPointsPolygonShapePoints) {
//            for (int idxSelPt = 0; idxSelPt < m_arpSelPtsPolygon.size(); idxSelPt++) {
//                CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
//                if (pGraphObjSelPt != nullptr) {
//                    QRectF rct = pGraphObjSelPt->rect();
//                    rct = pGraphObjSelPt->mapRectToScene(rct);
//                    QList<QGraphicsItem*> arpGraphicsItemsIntersected = m_pDrawingScene->items(rct);
//                    m_pDrawingScene->bringToFront(pGraphObjSelPt, arpGraphicsItemsIntersected);
//                }
//            }
//        }
//    }
//} // bringSelectionPointsToFront

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates the selection points if not yet created and adds them to
           the graphics scene.
*/
void CGraphObj::showSelectionPointsOfBoundingRect( const QRectF& i_rct, unsigned char i_selPts )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = rect2Str(i_rct);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::showSelectionPointsOfBoundingRect",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if (pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr) {
        for (int idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++) {
            ESelectionPoint selPt = static_cast<ESelectionPoint>(idxSelPt);
            bool bShowSelPt = false;
            if (idxSelPt >= ESelectionPointCornerMin && idxSelPt <= ESelectionPointCornerMax) {
                if (i_selPts & ESelectionPointsBoundingRectCorner) {
                    bShowSelPt = true;
                }
            }
            else if (idxSelPt >= ESelectionPointLineCenterMin && idxSelPt <= ESelectionPointLineCenterMax) {
                if (i_selPts & ESelectionPointsBoundingRectLineCenter) {
                    bShowSelPt = true;
                }
            }
            else if (selPt == ESelectionPoint::Center) {
                if (i_selPts & ESelectionPointsBoundingRectCenter) {
                    bShowSelPt = true;
                }
            }
            else if (idxSelPt >= ESelectionPointRotateMin && idxSelPt <= ESelectionPointRotateMax) {
                if (i_selPts & ESelectionPointsBoundingRectRotate) {
                    bShowSelPt = true;
                }
            }
            if (bShowSelPt) {
                CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];
                if (pGraphObjSelPt == nullptr) {
                    pGraphObjSelPt = new CGraphObjSelectionPoint(m_pDrawingScene, SGraphObjSelectionPoint(this, selPt));
                    m_arpSelPtsBoundingRect[idxSelPt] = pGraphObjSelPt;

                    // Please note that selection points should not belong as child to the graphics items
                    // for which the selection points are created. Otherwise the "boundingRect" call
                    // of groups (which implicitly calls childrenBoundingRect) does not work as expected
                    // as the selection points would be included.
                    // In addition selection points must be directly added to the graphics scene as they
                    // should not be indicated in the index tree.
                    m_pDrawingScene->addItem(pGraphObjSelPt);

                    // Event filters can only be installed on items in a scene.
                    pGraphObjSelPt->installSceneEventFilter(pGraphicsItem);

                    QObject::connect(
                        pGraphObjSelPt, &CGraphObj::aboutToBeDestroyed,
                        this, &CGraphObj::onSelectionPointAboutToBeDestroyed);
                    QObject::connect(
                        pGraphObjSelPt, &CGraphObj::geometryChanged,
                        this, &CGraphObj::onSelectionPointGeometryChanged);
                }
            }
        }
    }
} // showSelectionPointsOfBoundingRect

////------------------------------------------------------------------------------
//void CGraphObj::updateSelectionPointsOfBoundingRect( const QRectF& i_rct, unsigned char i_selPts )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = rect2Str(i_rct);
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::updateSelectionPointsOfBoundingRect",
//        /* strAddInfo   */ strMthInArgs );
//
//    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
//    if (pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr)
//    {
//        for (int idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++) {
//            ESelectionPoint selPt = static_cast<ESelectionPoint>(idxSelPt);
//            bool bUpdateSelPt = false;
//            if (idxSelPt >= ESelectionPointCornerMin && idxSelPt <= ESelectionPointCornerMax) {
//                if (i_selPts & ESelectionPointsBoundingRectCorner) {
//                    bUpdateSelPt = true;
//                }
//            }
//            else if (idxSelPt >= ESelectionPointLineCenterMin && idxSelPt <= ESelectionPointLineCenterMax) {
//                if (i_selPts & ESelectionPointsBoundingRectLineCenter) {
//                    bUpdateSelPt = true;
//                }
//            }
//            else if (selPt == ESelectionPoint::Center) {
//                if (i_selPts & ESelectionPointsBoundingRectCenter) {
//                    bUpdateSelPt = true;
//                }
//            }
//            else if (idxSelPt >= ESelectionPointRotateMin && idxSelPt <= ESelectionPointRotateMax) {
//                if (i_selPts & ESelectionPointsBoundingRectRotate) {
//                    bUpdateSelPt = true;
//                }
//            }
//            if (bUpdateSelPt) {
//                CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];
//                if (pGraphObjSelPt != nullptr) {
//                    QPointF ptSel;
//                    if (selPt == ESelectionPoint::RotateTop /*&& m_fScaleFacYCurr != 0.0*/) {
//                        ptSel = ZS::Draw::getSelectionPointCoors(i_rct,ESelectionPoint::TopCenter);
//                        ptSel.setY( ptSel.y() - getSelectionPointRotateDistance()/*/m_fScaleFacYCurr*/ );
//                        ptSel = pGraphicsItem->mapToScene(ptSel);
//                    }
//                    else if (selPt == ESelectionPoint::RotateBottom /*&& m_fScaleFacYCurr != 0.0*/) {
//                        ptSel = ZS::Draw::getSelectionPointCoors(i_rct,ESelectionPoint::BottomCenter);
//                        ptSel.setY( ptSel.y() + getSelectionPointRotateDistance()/*/m_fScaleFacYCurr*/ );
//                        ptSel = pGraphicsItem->mapToScene(ptSel);
//                    }
//                    else {
//                        ptSel = ZS::Draw::getSelectionPointCoors(i_rct,selPt);
//                        ptSel = pGraphicsItem->mapToScene(ptSel);
//                    }
//                    pGraphObjSelPt->setPos(ptSel);
//                }
//            }
//        }
//    }
//} // updateSelectionPointsOfBoundingRect

//------------------------------------------------------------------------------
/*! @brief Creates the selection points if not yet created and adds them to
           the graphics scene.
*/
void CGraphObj::showSelectionPointsOfPolygon( const QPolygonF& i_plg )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = polygon2Str(i_plg);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::showSelectionPointsOfPolygon",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr) {
        if (m_arpSelPtsPolygon.size() != i_plg.size()) {
            if (m_arpSelPtsPolygon.size() > 0) {
                for (int idxSelPt = m_arpSelPtsPolygon.size()-1; idxSelPt >= 0; idxSelPt--) {
                    // Deleting child objects leads to itemChange and an updateToolTip call
                    // accessing the array of selection points.
                    CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
                    m_arpSelPtsPolygon[idxSelPt] = nullptr;
                    // The dtor of the selection point (dtor of CGraphObj) removes itself from the drawing scene.
                    //m_pDrawingScene->removeGraphObj(pGraphObjSelPt);
                    delete pGraphObjSelPt;
                    pGraphObjSelPt = nullptr;
                }
                m_arpSelPtsPolygon.clear();
            }
            for (int idxSelPt = 0; idxSelPt < i_plg.size(); idxSelPt++) {
                m_arpSelPtsPolygon.append(nullptr);
            }
        }
        for (int idxSelPt = 0; idxSelPt < i_plg.size(); idxSelPt++) {
            CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
            if (pGraphObjSelPt == nullptr) {
                pGraphObjSelPt = new CGraphObjSelectionPoint(m_pDrawingScene, SGraphObjSelectionPoint(this, idxSelPt));
                m_arpSelPtsPolygon[idxSelPt] = pGraphObjSelPt;

                // Please note that selection points should not belong as child to the graphics items
                // for which the selection points are created. Otherwise the "boundingRect" call
                // of groups (which implicitly calls childrenBoundingRect) does not work as expected
                // as the selection points would be included.
                // In addition selection points must be directly added to the graphics scene as they
                // should not be indicated in the index tree.
                m_pDrawingScene->addItem(pGraphObjSelPt);

                // Event filters can only be installed on items in a scene.
                pGraphObjSelPt->installSceneEventFilter(pGraphicsItem);

                QObject::connect(
                    pGraphObjSelPt, &CGraphObj::aboutToBeDestroyed,
                    this, &CGraphObj::onSelectionPointAboutToBeDestroyed);
                QObject::connect(
                    pGraphObjSelPt, &CGraphObj::geometryChanged,
                    this, &CGraphObj::onSelectionPointGeometryChanged);
            }
        }
    }
} // showSelectionPointsOfPolygon

////------------------------------------------------------------------------------
//void CGraphObj::updateSelectionPointsOfPolygon( const QPolygonF& i_plg )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = polygon2Str(i_plg);
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::updateSelectionPointsOfPolygon",
//        /* strAddInfo   */ strMthInArgs );
//
//    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
//    if (pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr) {
//        if (i_plg.size() == m_arpSelPtsPolygon.size()) {
//            for (int idxSelPt = 0; idxSelPt < i_plg.size(); idxSelPt++) {
//                CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
//                if (pGraphObjSelPt != nullptr) {
//                    QPointF ptSel = i_plg[idxSelPt];
//                    ptSel = pGraphicsItem->mapToScene(ptSel);
//                    pGraphObjSelPt->setPos(ptSel);
//                }
//            }
//        }
//    }
//}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the list of all existing label names.

    @return List of existing label names.
*/
QString CGraphObj::findUniqueLabelName(const QString& i_strPrefix) const
//------------------------------------------------------------------------------
{
    QString strName = i_strPrefix;
    if (strName.isEmpty()) {
        strName = "Label";
    }
    strName += QString::number(m_hshLabelDscrs.size());
    if (m_hshLabelDscrs.contains(strName)) {
        strName = i_strPrefix;
        if (strName.isEmpty()) {
            strName = "Label";
        }
        int iRow = 0;
        QString strNameTmp = strName + QString::number(iRow);
        while (m_hshLabelDscrs.contains(strNameTmp)) {
            ++iRow;
            strNameTmp = strName + QString::number(iRow);
        }
        strName = strNameTmp;
    }
    return strName;
}

//------------------------------------------------------------------------------
/*! @brief Returns the list of all existing label names.

    @return List of existing label names.
*/
QStringList CGraphObj::getLabelNames() const
//------------------------------------------------------------------------------
{
    return m_hshLabelDscrs.keys();
}

//------------------------------------------------------------------------------
/*! @brief Returns the list of the reserved (predefined) label names.

    For all object types "Name" is reserved to indicate the name of the object.

    To extend the list of predefined label names the derived classes should add
    the label names in their conto the member m_strlstPredefinedLabelNames
    This method may be overriden by derived classes if there are further reserved
    names beside "Name" (e.g. the "Line" object reserves "P1" and "P2" as names).
    If user defined labels got to be added the reserved names may not be used.

    @return List of reserved label names.
*/
QStringList CGraphObj::getPredefinedLabelNames() const
//------------------------------------------------------------------------------
{
    return m_strlstPredefinedLabelNames;
}

//------------------------------------------------------------------------------
/*! @brief Returns whether the passed name belongs to the predefined names.

    @param [in] i_strName
        Name of the label to be checked.

    @return true if the given name belongs to the list of predefined names, false otherwise.
*/
bool CGraphObj::isPredefinedLabelName(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    return m_strlstPredefinedLabelNames.contains(i_strName);
}

//------------------------------------------------------------------------------
/*! @brief Returns the label descriptor for the given label name.
*/
SLabelDscr CGraphObj::getLabelDescriptor(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    SLabelDscr labelDscr = m_hshLabelDscrs.value(i_strName, SLabelDscr());
    CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(i_strName, nullptr);
    if (pGraphObjLabel != nullptr) {
        labelDscr.m_polarCoorsToLinkedSelPt = pGraphObjLabel->polarCoorsToLinkedSelectionPoint();
    }
    return labelDscr;
}

//------------------------------------------------------------------------------
/*! @brief Returns the list of the possible selection points a label may be anchored to.

    The method has to be overridden by the specialized classes.

    Please note that the most common used selection points should be at the
    beginning of the list so that combo boxes to select the selection point
    start with those.

    @param [in] i_strName
        Unique name of the label. May be omitted (empty string).

    @return List of possbile selection points.
        If PolygonPoint is contained in the list of returned selection points the possible
        anchor points depend on the number of line points of the polygon or polyline.
*/
QList<SGraphObjSelectionPoint> CGraphObj::getPossibleLabelAnchorPoints(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    static QList<SGraphObjSelectionPoint> s_arSelPts;
    if (s_arSelPts.isEmpty()) {
        s_arSelPts.append(SGraphObjSelectionPoint(const_cast<CGraphObj*>(this), ESelectionPoint::Center));
    }
    static QHash<QString, QList<SGraphObjSelectionPoint>> s_hshSelPtsPredefined;
    if (s_hshSelPtsPredefined.isEmpty()) {
        s_hshSelPtsPredefined.insert(c_strLabelName, s_arSelPts);
    }
    return s_hshSelPtsPredefined.value(i_strName, QList<SGraphObjSelectionPoint>());
}

//------------------------------------------------------------------------------
/*! @brief Returns whether a label with the passed name has been added to the
           list of text labels.

    @param [in] i_strName
        Name of the label to be checked.

    @return true if the given name belongs to the list of text labels, false otherwise.
*/
bool CGraphObj::isLabelAdded(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    return m_hshLabelDscrs.contains(i_strName);
}

//------------------------------------------------------------------------------
/*! @brief Creates a new text label with the given name linked to a selection point
           at the bounding rectangle.

    The label is not added to the graphics scene and remains invisible.
    To add the label also to the graphics scene the label must be shown.

    The predefined labels (including "Name") got to be added in the constructor
    of the derived class. Adding labels in the constructor of the base class
    would lead to crashes as "QGraphicsItem" is not yet created and the constructor
    of the label object tries to access the parent (the linked) "QGraphicsItem".

    @param [in] i_strName
        Name of the label. The name must be unique otherwise no label is created.
    @param [in] i_strText (optional)
        If not empty defines the text to be shown.
    @param [in] i_selPt
        Selection point the label should be anchored to.

    @return true, if the label has been created and added, false otherwise.
*/
bool CGraphObj::addLabel(
    const QString& i_strName, const QString& i_strText, ESelectionPoint i_selPt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName + ", " + i_strText + ", " + CEnumSelectionPoint(i_selPt).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::addLabel",
        /* strAddInfo   */ strMthInArgs );

    bool bCanAdd = !m_hshLabelDscrs.contains(i_strName);
    if (bCanAdd) {
        QString strText = i_strText;
        if (i_strName == c_strLabelName) {
            strText = m_strName;
        }
        SLabelDscr labelDscr(EGraphObjTypeLabel, i_strName, strText, SGraphObjSelectionPoint(this, i_selPt));
        m_hshLabelDscrs.insert(i_strName, labelDscr);
        emit_labelAdded(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bCanAdd);
    }
    return bCanAdd;
}

//------------------------------------------------------------------------------
/*! @brief Creates a new text label with the given name linked to a polygon shape point.

    The label is not added to the graphics scene and remains invisible.
    To add the label also to the graphics scene the label must be shown.

    The predefined labels (including "Name") got to be added in the constructor
    of the derived class. Adding labels in the constructor of the base class
    would lead to crashes as "QGraphicsItem" is not yet created and the constructor
    of the label object tries to access the parent (the linked) "QGraphicsItem".

    @param [in] i_strName
        Name of the label. The name must be unique otherwise no label is created.
    @param [in] i_strText (optional)
        If not empty defines the text to be shown.
    @param [in] i_idxPt
        Selection point the label should be anchored to.

    @return true, if the label has been created and added, false otherwise.
*/
bool CGraphObj::addLabel(
    const QString& i_strName, const QString& i_strText, int i_idxPt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName + ", " + i_strText + ", " + QString::number(i_idxPt);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::addLabel",
        /* strAddInfo   */ strMthInArgs );

    bool bCanAdd = !m_hshLabelDscrs.contains(i_strName);
    if (bCanAdd) {
        QString strText = i_strText;
        if (i_strName == c_strLabelName) {
            strText = m_strName;
        }
        SLabelDscr labelDscr(EGraphObjTypeLabel, i_strName, strText, SGraphObjSelectionPoint(this, i_idxPt));
        m_hshLabelDscrs.insert(i_strName, labelDscr);
        emit_labelAdded(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bCanAdd);
    }
    return bCanAdd;
}

//------------------------------------------------------------------------------
/*! Removes the label with the given name.

    The label is destryoed and also removed from the graphics scene and becomes invisible.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
*/
bool CGraphObj::removeLabel(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::removeLabel",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    bool bCanRemove = !isPredefinedLabelName(i_strName);
    if (bCanRemove) {
        m_hshLabelDscrs.remove(i_strName);
        CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(i_strName, nullptr);
        if (pGraphObjLabel != nullptr) {
            // "onLabelAboutToBeDestroyed" is called which removes the label from the hash.
            delete pGraphObjLabel;
            pGraphObjLabel = nullptr;
        }
        emit_labelRemoved(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bCanRemove);
    }
    return bCanRemove;
}

//------------------------------------------------------------------------------
/*! Renames the label with the given name.

    If a label with passed new name already exists, the label is not renamed and
    the method returns false.

    @param [in] i_strName
        Current name of the label. If no label with the name exists an exception is thrown.
    @param [in] i_strNameNew
        New name of the label. If a label with the new name already exists, the
        label will not be renamed.

    @return true, if the label has been renamed, false otherwise.
*/
bool CGraphObj::renameLabel(const QString& i_strName, const QString& i_strNameNew)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName + ", " + i_strNameNew;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::renameLabel",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    bool bCanRename = !isPredefinedLabelName(i_strName);
    if (bCanRename) {
        SLabelDscr labelDscr = m_hshLabelDscrs[i_strName];
        m_hshLabelDscrs.remove(i_strName);
        labelDscr.m_strKey = i_strNameNew;
        m_hshLabelDscrs.insert(i_strNameNew, labelDscr);
        CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(i_strName, nullptr);
        if (pGraphObjLabel == nullptr) {
            m_hshpLabels.remove(i_strName);
            pGraphObjLabel->setKey(i_strNameNew);
            m_hshpLabels.insert(i_strNameNew, pGraphObjLabel);
            if (i_strNameNew == c_strLabelName) {
                pGraphObjLabel->setText(m_strName);
            }
        }
        emit_labelRenamed(i_strName, i_strNameNew);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bCanRename);
    }
    return bCanRename;
}

//------------------------------------------------------------------------------
/*! Sets the text to be shown by the label.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
    @param [in] i_strText
        Text to be shown.
*/
void CGraphObj::setLabelText(const QString& i_strName, const QString& i_strText)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName + ", " + i_strText;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setLabelText",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    if (m_hshLabelDscrs[i_strName].m_strText != i_strText) {
        m_hshLabelDscrs[i_strName].m_strText = i_strText;
        CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(i_strName, nullptr);
        if (pGraphObjLabel != nullptr) {
            pGraphObjLabel->setText(i_strText);
        }
        emit_labelChanged(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the text of the label.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
*/
QString CGraphObj::labelText(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }
    return m_hshLabelDscrs[i_strName].m_strText;
}

//------------------------------------------------------------------------------
/*! @brief Sets the anchor point of the label with the given name to a selection
           point at the bounding rectangle.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
    @param [in] i_selPt
        Selection point the label should be anchored to.
*/
void CGraphObj::setLabelAnchorPoint(const QString& i_strName, ESelectionPoint i_selPt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName + ", " + CEnumSelectionPoint(i_selPt).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setLabelAnchorPoint",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    SLabelDscr& labelDscr = m_hshLabelDscrs[i_strName];
    if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle || labelDscr.m_selPt1.m_selPt != i_selPt) {
        labelDscr.m_selPt1 = SGraphObjSelectionPoint(this, i_selPt);
        CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(i_strName, nullptr);
        if (pGraphObjLabel != nullptr) {
            pGraphObjLabel->setSelectionPoint1(SGraphObjSelectionPoint(this, i_selPt));
        }
        emit_labelChanged(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Sets the anchor point of the label with the given name to a shape point.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
    @param [in] i_idxPt
        Selection point the label should be anchored to.
*/
void CGraphObj::setLabelAnchorPoint(const QString& i_strName, int i_idxPt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName + ", " + QString::number(i_idxPt);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setLabelAnchorPoint",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    SLabelDscr& labelDscr = m_hshLabelDscrs[i_strName];
    if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::PolygonShapePoint || labelDscr.m_selPt1.m_idxPt != i_idxPt) {
        labelDscr.m_selPt1 = SGraphObjSelectionPoint(this, i_idxPt);
        CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(i_strName, nullptr);
        if (pGraphObjLabel != nullptr) {
            pGraphObjLabel->setSelectionPoint1(SGraphObjSelectionPoint(this, i_idxPt));
        }
        emit_labelChanged(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the anchor point of the label.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
*/
SGraphObjSelectionPoint CGraphObj::labelAnchorPoint(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }
    return m_hshLabelDscrs[i_strName].m_selPt1;
}

//------------------------------------------------------------------------------
/*! Shows the label (set visible) with the given name.

    The label is added to the graphics scene and becomes visible.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
*/
void CGraphObj::showLabel(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::showLabel",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    SLabelDscr& labelDscr = m_hshLabelDscrs[i_strName];
    CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(i_strName, nullptr);
    if (pGraphObjLabel == nullptr) {
        labelDscr.m_bLabelIsVisible = true;
        pGraphObjLabel = new CGraphObjLabel(
            m_pDrawingScene, i_strName, labelDscr.m_strText, labelDscr.m_selPt1);
        m_hshpLabels.insert(i_strName, pGraphObjLabel);
        QObject::connect(
            pGraphObjLabel, &CGraphObj::aboutToBeDestroyed,
            this, &CGraphObj::onLabelAboutToBeDestroyed);
        // Please note that labels should not belong as child to the graphics items
        // for which the labels are created. Otherwise the "boundingRect" call of groups
        // (which implicitly calls childrenBoundingRect) does not work as expected as
        // the labels would be included.
        // In addition the labels must be directly added to the graphics scene as they
        // should not be indicated in the index tree.
        m_pDrawingScene->addItem(pGraphObjLabel);
        pGraphObjLabel->setVisible(true);
        pGraphObjLabel->setPolarCoorsToLinkedSelectionPoint(labelDscr.m_polarCoorsToLinkedSelPt);
        labelDscr.m_bShowAnchorLine ? pGraphObjLabel->showAnchorLine() : pGraphObjLabel->hideAnchorLine();
        // The labels anchor line should be drawn before the object is drawn.
        // Otherwise the anchor lines may cover the painting of this object.
        emit_labelChanged(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
} // showLabel

//------------------------------------------------------------------------------
/*! Hides the label.

    The label is removed from the graphics scene and becomes invisible.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
*/
void CGraphObj::hideLabel(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::hideLabel",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    SLabelDscr& labelDscr = m_hshLabelDscrs[i_strName];
    CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(i_strName, nullptr);
    if (pGraphObjLabel != nullptr) {
        if (pGraphObjLabel->scene() == nullptr) {
            throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
        }
        // Save current distance of label to selection point.
        labelDscr = getLabelDescriptor(i_strName);
        labelDscr.m_bLabelIsVisible = false;

        // "onLabelAboutToBeDestroyed" is called which removes the label from the hash.
        // The destructor also removes the label from the graphics scene.
        delete pGraphObjLabel;
        pGraphObjLabel = nullptr;
        emit_labelChanged(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @Returns whether the visibility of the label with the given name.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.

    @return true, if the label is visible. false otherwise.
*/
bool CGraphObj::isLabelVisible(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }
    return m_hshpLabels.contains(i_strName);
}

//------------------------------------------------------------------------------
/*! @brief Sets the relative position of the label in polar coordinates
           (length in pixels, angle in degrees) of the labels position to
           the linked selection point.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
    @param [in] i_polarCoors
        Polar coordinates (length in pixels, angle in degrees)
        as a relative position of the label to the linked selection point.
*/
void CGraphObj::setLabelPolarCoorsToLinkedSelectionPoint(const QString& i_strName, const SPolarCoors& i_polarCoors)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName + ", {" + i_polarCoors.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setLabelPolarCoorsToLinkedSelectionPoint",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    SLabelDscr& labelDscr = m_hshLabelDscrs[i_strName];
    if (labelDscr.m_polarCoorsToLinkedSelPt != i_polarCoors) {
        labelDscr.m_polarCoorsToLinkedSelPt = i_polarCoors;
        CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(i_strName, nullptr);
        if (pGraphObjLabel != nullptr) {
            pGraphObjLabel->setPolarCoorsToLinkedSelectionPoint(i_polarCoors);
        }
        emit_labelChanged(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the polar coordinates (length in pixels, angle in degrees)
           as a relative position of the label to the linked selection point.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.

    @return Relative position of the label to the linked selection point.
*/
SPolarCoors CGraphObj::labelPolarCoorsToLinkedSelectionPoint(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }
    return m_hshLabelDscrs[i_strName].m_polarCoorsToLinkedSelPt;
}

//------------------------------------------------------------------------------
/*! Shows the line between the label and the selection point the label is aligned to.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
*/
void CGraphObj::showLabelAnchorLine(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::showLabelAnchorLine",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    SLabelDscr& labelDscr = m_hshLabelDscrs[i_strName];
    if (!labelDscr.m_bShowAnchorLine) {
        labelDscr.m_bShowAnchorLine = true;
        CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(i_strName, nullptr);
        if (pGraphObjLabel != nullptr) {
            pGraphObjLabel->showAnchorLine();
        }
        emit_labelChanged(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! Hides the line between the description label and the selection point the label is aligned to.

    The method just sets a flag. If the description label is not visible the method has no effect.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
*/
void CGraphObj::hideLabelAnchorLine(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::hideLabelAnchorLine",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    SLabelDscr& labelDscr = m_hshLabelDscrs[i_strName];
    if (labelDscr.m_bShowAnchorLine) {
        labelDscr.m_bShowAnchorLine = false;
        CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(i_strName, nullptr);
        if (pGraphObjLabel != nullptr) {
            pGraphObjLabel->hideAnchorLine();
        }
        emit_labelChanged(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! Returns the visibility of the line between the label and the selection point
    the label is aligned to.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.

    @return true, if the anchor line is visible, false otherwise.
*/
bool CGraphObj::isLabelAnchorLineVisible(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }
    return m_hshLabelDscrs[i_strName].m_bShowAnchorLine;
}

/*==============================================================================
public: // geometry labels
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief This method must be overridden by derived classes to return the names
           for all geometry values which can be edited or indicated.

    Usually for each geometry value a pair of X and Y values can be set.
    Most of the geometry values specify shape points with X and Y coordinates.
    For the "Size" value X means width and Y means height.

    But there are also specific geometry values with a special meaning like
    "Length" and "Angle". For those only one value can be set and indicated.
    For those the X value is used and the Y value is invalid.

    A Line object for example supports the following geometry values:

    - P1 (x/y), P2 (x/y), Center (x/y), Size (width/height), Length("x"), Angle ("x").

    @return Supported value names.
*/
QStringList CGraphObj::getGeometryLabelNames() const
//------------------------------------------------------------------------------
{
    return m_strlstGeometryLabelNames;
}

//------------------------------------------------------------------------------
/*! @brief Returns the label descriptor for the given label name.
*/
bool CGraphObj::isValidGeometryLabelName(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    return m_hshGeometryLabelDscrs.contains(i_strName);
}

//------------------------------------------------------------------------------
/*! @brief Returns the label descriptor for the given label name.
*/
SLabelDscr CGraphObj::getGeometryLabelDescriptor(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    SLabelDscr labelDscr = m_hshGeometryLabelDscrs.value(i_strName, SLabelDscr());
    CGraphObjLabel* pGraphObjLabel = m_hshpGeometryLabels.value(i_strName, nullptr);
    if (pGraphObjLabel != nullptr) {
        labelDscr.m_polarCoorsToLinkedSelPt = pGraphObjLabel->polarCoorsToLinkedSelectionPoint();
    }
    return labelDscr;
}

//------------------------------------------------------------------------------
/*! Shows the label (set visible) with the given name.

    The label is added to the graphics scene and becomes visible.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
*/
void CGraphObj::showGeometryLabel(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::showGeometryLabel",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshGeometryLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    SLabelDscr& labelDscr = m_hshGeometryLabelDscrs[i_strName];
    CGraphObjLabel* pGraphObjLabel = m_hshpGeometryLabels.value(i_strName, nullptr);
    if (pGraphObjLabel == nullptr) {
        CGraphObjLabel* pGraphObjLabel = nullptr;
        if (labelDscr.m_labelType == EGraphObjTypeLabelGeometryPosition) {
            pGraphObjLabel = new CGraphObjLabelGeometryPosition(
                m_pDrawingScene, i_strName, labelDscr.m_selPt1);
        }
        else if (labelDscr.m_labelType == EGraphObjTypeLabelGeometryDY) {
            pGraphObjLabel = new CGraphObjLabelGeometryDY(
                m_pDrawingScene, i_strName, labelDscr.m_selPt1, labelDscr.m_selPt2);
        }
        else if (labelDscr.m_labelType == EGraphObjTypeLabelGeometryDX) {
            pGraphObjLabel = new CGraphObjLabelGeometryDX(
                m_pDrawingScene, i_strName, labelDscr.m_selPt1, labelDscr.m_selPt2);
        }
        else if (labelDscr.m_labelType == EGraphObjTypeLabelGeometryLength) {
            pGraphObjLabel = new CGraphObjLabelGeometryLength(
                m_pDrawingScene, i_strName, labelDscr.m_selPt1, labelDscr.m_selPt2);
        }
        else if (labelDscr.m_labelType == EGraphObjTypeLabelGeometryAngle) {
            pGraphObjLabel = new CGraphObjLabelGeometryAngle(
                m_pDrawingScene, i_strName, labelDscr.m_selPt1, labelDscr.m_selPt2);
        }
        if (pGraphObjLabel != nullptr) {
            labelDscr.m_bLabelIsVisible = true;
            pGraphObjLabel->setVisible(false);
            pGraphObjLabel->setPolarCoorsToLinkedSelectionPoint(labelDscr.m_polarCoorsToLinkedSelPt);
            labelDscr.m_bShowAnchorLine ? pGraphObjLabel->showAnchorLine() : pGraphObjLabel->hideAnchorLine();
            m_hshpGeometryLabels.insert(i_strName, pGraphObjLabel);
            QObject::connect(
                pGraphObjLabel, &CGraphObj::aboutToBeDestroyed,
                this, &CGraphObj::onGeometryLabelAboutToBeDestroyed);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
        }
        // Please note that labels should not belong as child to the graphics items
        // for which the labels are created. Otherwise the "boundingRect" call of groups
        // (which implicitly calls childrenBoundingRect) does not work as expected as
        // the labels would be included.
        // In addition the labels must be directly added to the graphics scene as they
        // should not be indicated in the index tree.
        m_pDrawingScene->addItem(pGraphObjLabel);
        pGraphObjLabel->setVisible(true);
        // The labels anchor line should be drawn before the object is drawn.
        // Otherwise the anchor lines may cover the painting of this object.
        emit_geometryLabelChanged(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! Hides the label.

    The label is removed from the graphics scene and becomes invisible.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
*/
void CGraphObj::hideGeometryLabel(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::hideGeometryLabel",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshGeometryLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    SLabelDscr& labelDscr = m_hshGeometryLabelDscrs[i_strName];
    CGraphObjLabel* pGraphObjLabel = m_hshpGeometryLabels.value(i_strName, nullptr);
    if (pGraphObjLabel != nullptr) {
        if (pGraphObjLabel->scene() == nullptr) {
            throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
        }

        // Save current distance of label to selection point.
        labelDscr = getGeometryLabelDescriptor(i_strName);
        labelDscr.m_bLabelIsVisible = false;

        // "onGeometryLabelAboutToBeDestroyed" is called which removes the label from the hash.
        // The destructor also removes the label from the graphics scene.
        delete pGraphObjLabel;
        pGraphObjLabel = nullptr;
        emit_geometryLabelChanged(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @Returns whether the visibility of the label with the given name.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.

    @return true, if the label is visible. false otherwise.
*/
bool CGraphObj::isGeometryLabelVisible(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    if (!m_hshGeometryLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }
    return m_hshpGeometryLabels.contains(i_strName);
}

//------------------------------------------------------------------------------
/*! @brief Sets the relative position of the label in polar coordinates
           (length in pixels, angle in degrees) of the labels position to
           the linked selection point.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
    @param [in] i_polarCoors
        Polar coordinates (length in pixels, angle in degrees)
        as a relative position of the label to the linked selection point.
*/
void CGraphObj::setGeometryLabelPolarCoorsToLinkedSelectionPoint(const QString& i_strName, const SPolarCoors& i_polarCoors)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName + ", {" + i_polarCoors.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setGeometryLabelPolarCoorsToLinkedSelectionPoint",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshGeometryLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    SLabelDscr& labelDscr = m_hshGeometryLabelDscrs[i_strName];
    if (labelDscr.m_polarCoorsToLinkedSelPt != i_polarCoors) {
        labelDscr.m_polarCoorsToLinkedSelPt = i_polarCoors;
        CGraphObjLabel* pGraphObjLabel = m_hshpGeometryLabels.value(i_strName, nullptr);
        if (pGraphObjLabel != nullptr) {
            pGraphObjLabel->setPolarCoorsToLinkedSelectionPoint(i_polarCoors);
        }
        emit_geometryLabelChanged(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the polar coordinates (length in pixels, angle in degrees)
           as a relative position of the label to the linked selection point.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.

    @return Relative position of the label to the linked selection point.
*/
SPolarCoors CGraphObj::geometryLabelPolarCoorsToLinkedSelectionPoint(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    if (!m_hshGeometryLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }
    return m_hshGeometryLabelDscrs[i_strName].m_polarCoorsToLinkedSelPt;
}

//------------------------------------------------------------------------------
/*! Shows the line between the label and the selection point the label is aligned to.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
*/
void CGraphObj::showGeometryLabelAnchorLine(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::showGeometryLabelAnchorLine",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshGeometryLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    SLabelDscr& labelDscr = m_hshGeometryLabelDscrs[i_strName];
    if (!labelDscr.m_bShowAnchorLine) {
        labelDscr.m_bShowAnchorLine = true;
        CGraphObjLabel* pGraphObjLabel = m_hshpGeometryLabels.value(i_strName, nullptr);
        if (pGraphObjLabel != nullptr) {
            pGraphObjLabel->showAnchorLine();
        }
        emit_geometryLabelChanged(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! Hides the line between the description label and the selection point the label is aligned to.

    The method just sets a flag. If the description label is not visible the method has no effect.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
*/
void CGraphObj::hideGeometryLabelAnchorLine(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::hideGeometryLabelAnchorLine",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshGeometryLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    SLabelDscr& labelDscr = m_hshGeometryLabelDscrs[i_strName];
    if (labelDscr.m_bShowAnchorLine) {
        labelDscr.m_bShowAnchorLine = false;
        CGraphObjLabel* pGraphObjLabel = m_hshpGeometryLabels.value(i_strName, nullptr);
        if (pGraphObjLabel != nullptr) {
            pGraphObjLabel->hideAnchorLine();
        }
        emit_geometryLabelChanged(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! Returns the visibility of the line between the label and the selection point
    the label is aligned to.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.

    @return true, if the anchor line is visible, false otherwise.
*/
bool CGraphObj::isGeometryLabelAnchorLineVisible(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    if (!m_hshGeometryLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }
    return m_hshGeometryLabelDscrs[i_strName].m_bShowAnchorLine;
}

/*==============================================================================
public: // instance methods (simulation methods)
==============================================================================*/

////------------------------------------------------------------------------------
//void CGraphObj::addMousePressEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::addMousePressEventFunction",
//        /* strAddInfo   */ strMthInArgs );
//
//    for (int idx = 0; idx < m_arMousePressEventFunctions.size(); idx++) {
//        SGraphObjMouseEventFct fctEntry = m_arMousePressEventFunctions[idx];
//        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
//            throw ZS::System::CException(__FILE__, __LINE__, EResultObjAlreadyInList, "Event function already added");
//        }
//    }
//    m_arMousePressEventFunctions.append( SGraphObjMouseEventFct(i_pFct,i_pvThis,i_pvData) );
//}
//
////------------------------------------------------------------------------------
//void CGraphObj::removeMousePressEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::removeMousePressEventFunction",
//        /* strAddInfo   */ strMthInArgs );
//
//    int idxFct = -1;
//    for (int idx = 0; idx < m_arMousePressEventFunctions.size(); idx++) {
//        SGraphObjMouseEventFct fctEntry = m_arMousePressEventFunctions[idx];
//        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
//            idxFct = idx;
//            break;
//        }
//    }
//    if (idxFct < 0) {
//        throw ZS::System::CException(__FILE__, __LINE__, EResultObjNotInList, "Event function not added");
//    }
//    m_arMousePressEventFunctions.removeAt(idxFct);
//}
//
////------------------------------------------------------------------------------
//void CGraphObj::addMouseReleaseEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::addMouseReleaseEventFunction",
//        /* strAddInfo   */ strMthInArgs );
//
//    for (int idx = 0; idx < m_arMouseReleaseEventFunctions.size(); idx++) {
//        SGraphObjMouseEventFct fctEntry = m_arMouseReleaseEventFunctions[idx];
//        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
//            throw ZS::System::CException(__FILE__, __LINE__, EResultObjAlreadyInList, "Event function already added");
//        }
//    }
//    m_arMouseReleaseEventFunctions.append( SGraphObjMouseEventFct(i_pFct,i_pvThis,i_pvData) );
//}
//
////------------------------------------------------------------------------------
//void CGraphObj::removeMouseReleaseEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::removeMouseReleaseEventFunction",
//        /* strAddInfo   */ strMthInArgs );
//
//    int idxFct = -1;
//    for (int idx = 0; idx < m_arMouseReleaseEventFunctions.size(); idx++) {
//        SGraphObjMouseEventFct fctEntry = m_arMouseReleaseEventFunctions[idx];
//        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
//            idxFct = idx;
//            break;
//        }
//    }
//    if (idxFct < 0) {
//        throw ZS::System::CException(__FILE__, __LINE__, EResultObjNotInList, "Event function not added");
//    }
//    m_arMouseReleaseEventFunctions.removeAt(idxFct);
//}
//
////------------------------------------------------------------------------------
//void CGraphObj::addMouseDoubleClickEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::addMouseDoubleClickEventFunction",
//        /* strAddInfo   */ strMthInArgs );
//
//    for (int idx = 0; idx < m_arMouseDoubleClickEventFunctions.size(); idx++) {
//        SGraphObjMouseEventFct fctEntry = m_arMouseDoubleClickEventFunctions[idx];
//        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
//            throw ZS::System::CException(__FILE__, __LINE__, EResultObjAlreadyInList, "Event function already added");
//        }
//    }
//    m_arMouseDoubleClickEventFunctions.append( SGraphObjMouseEventFct(i_pFct,i_pvThis,i_pvData) );
//}
//
////------------------------------------------------------------------------------
//void CGraphObj::removeMouseDoubleClickEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::removeMouseDoubleClickEventFunction",
//        /* strAddInfo   */ strMthInArgs );
//
//    int idxFct = -1;
//    for (int idx = 0; idx < m_arMouseDoubleClickEventFunctions.size(); idx++) {
//        SGraphObjMouseEventFct fctEntry = m_arMouseDoubleClickEventFunctions[idxFct];
//        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
//            idxFct = idx;
//            break;
//        }
//    }
//    if (idxFct < 0) {
//        throw ZS::System::CException(__FILE__, __LINE__, EResultObjNotInList, "Event function not added");
//    }
//    m_arMouseDoubleClickEventFunctions.removeAt(idxFct);
//}
//
////------------------------------------------------------------------------------
//void CGraphObj::addMouseMoveEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::addMouseMoveEventFunction",
//        /* strAddInfo   */ strMthInArgs );
//
//    for (int idx = 0; idx < m_arMouseMoveEventFunctions.size(); idx++) {
//        SGraphObjMouseEventFct fctEntry = m_arMouseMoveEventFunctions[idx];
//        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
//            throw ZS::System::CException(__FILE__, __LINE__, EResultObjAlreadyInList, "Event function already added");
//        }
//    }
//    m_arMouseMoveEventFunctions.append( SGraphObjMouseEventFct(i_pFct,i_pvThis,i_pvData) );
//}
//
////------------------------------------------------------------------------------
//void CGraphObj::removeMouseMoveEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::removeMouseMoveEventFunction",
//        /* strAddInfo   */ strMthInArgs );
//
//    int idxFct = -1;
//    for (int idx = 0; idx < m_arMouseMoveEventFunctions.size(); idx++) {
//        SGraphObjMouseEventFct fctEntry = m_arMouseMoveEventFunctions[idx];
//        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
//            idxFct = idx;
//            break;
//        }
//    }
//    if (idxFct < 0) {
//        throw ZS::System::CException(__FILE__, __LINE__, EResultObjNotInList, "Event function not added");
//    }
//    m_arMouseMoveEventFunctions.removeAt(idxFct);
//}
//
////------------------------------------------------------------------------------
//void CGraphObj::addKeyPressEventFunction( TFctKeyEvent i_pFct, void* i_pvThis, void* i_pvData )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::addKeyPressEventFunction",
//        /* strAddInfo   */ strMthInArgs );
//
//    for (int idx = 0; idx < m_arKeyPressEventFunctions.size(); idx++) {
//        SGraphObjKeyEventFct fctEntry = m_arKeyPressEventFunctions[idx];
//        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
//            throw ZS::System::CException(__FILE__, __LINE__, EResultObjAlreadyInList, "Event function already added");
//        }
//    }
//    m_arKeyPressEventFunctions.append( SGraphObjKeyEventFct(i_pFct,i_pvThis,i_pvData) );
//}
//
////------------------------------------------------------------------------------
//void CGraphObj::removeKeyPressEventFunction( TFctKeyEvent i_pFct, void* i_pvThis, void* i_pvData )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::removeKeyPressEventFunction",
//        /* strAddInfo   */ strMthInArgs );
//
//    int idxFct = -1;
//    for (int idx = 0; idx < m_arKeyPressEventFunctions.size(); idx++) {
//        SGraphObjKeyEventFct fctEntry = m_arKeyPressEventFunctions[idx];
//        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
//            idxFct = idx;
//            break;
//        }
//    }
//    if (idxFct < 0) {
//        throw ZS::System::CException(__FILE__, __LINE__, EResultObjNotInList, "Event function not added");
//    }
//    m_arKeyPressEventFunctions.removeAt(idxFct);
//}
//
////------------------------------------------------------------------------------
//void CGraphObj::addKeyReleaseEventFunction( TFctKeyEvent i_pFct, void* i_pvThis, void* i_pvData )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::addKeyReleaseEventFunction",
//        /* strAddInfo   */ strMthInArgs );
//
//    for (int idx = 0; idx < m_arKeyReleaseEventFunctions.size(); idx++) {
//        SGraphObjKeyEventFct fctEntry = m_arKeyReleaseEventFunctions[idx];
//        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
//            throw ZS::System::CException(__FILE__, __LINE__, EResultObjAlreadyInList, "Event function already added");
//        }
//    }
//    m_arKeyReleaseEventFunctions.append( SGraphObjKeyEventFct(i_pFct,i_pvThis,i_pvData) );
//}
//
////------------------------------------------------------------------------------
//void CGraphObj::removeKeyReleaseEventFunction( TFctKeyEvent i_pFct, void* i_pvThis, void* i_pvData )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::removeKeyReleaseEventFunction",
//        /* strAddInfo   */ strMthInArgs );
//
//    int idxFct = -1;
//    for (int idx = 0; idx < m_arKeyReleaseEventFunctions.size(); idx++) {
//        SGraphObjKeyEventFct fctEntry = m_arKeyReleaseEventFunctions[idx];
//        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
//            idxFct = idx;
//            break;
//        }
//    }
//    if (idxFct < 0) {
//        throw ZS::System::CException(__FILE__, __LINE__, EResultObjNotInList, "Event function not added");
//    }
//    m_arKeyReleaseEventFunctions.removeAt(idxFct);
//}

/*==============================================================================
protected: // overridables (geometry labels)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates a new geometry label with the given name linked to one or two
           selection point at the bounding rectangle.

    The label is not added to the graphics scene and remains invisible.
    To add the label also to the graphics scene the label must be shown.

    @param [in] i_strName
        Name of the label. The name must be unique otherwise no label is created.
    @param [in] i_labelType
        Range [EGraphObjTypeLabelGeometryPosition,
               EGraphObjTypeLabelGeometryDX, EGraphObjTypeLabelGeometryDY,
               EGraphObjTypeLabelGeometryLength, EGraphObjTypeLabelGeometryAngle]
        If not empty defines the text to be shown.
    @param [in] i_selPt1
        First selection point the label should use to indicate the geometry.
    @param [in] i_selPt2
        Second selection point the label should use to indicate the geometry.
        For position label only one selection point is used and the second selection
        point is set to invalid.

    @return true, if the label has been created and added, false otherwise.
*/
bool CGraphObj::addGeometryLabel(
    const QString& i_strName, EGraphObjType i_labelType,
    ESelectionPoint i_selPt1, ESelectionPoint i_selPt2)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName + ", " + graphObjType2Str(i_labelType)
            + ", " + CEnumSelectionPoint(i_selPt1).toString()
            + ", " + CEnumSelectionPoint(i_selPt2).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::addGeometryLabel",
        /* strAddInfo   */ strMthInArgs );

    bool bCanAdd = !m_hshGeometryLabelDscrs.contains(i_strName);
    if (bCanAdd) {
        SLabelDscr labelDscr(i_labelType, i_strName);
        labelDscr.m_selPt1 = SGraphObjSelectionPoint(this, i_selPt1);
        labelDscr.m_selPt2 = SGraphObjSelectionPoint(this, i_selPt2);
        m_hshGeometryLabelDscrs.insert(i_strName, labelDscr);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bCanAdd);
    }
    return bCanAdd;
}

//------------------------------------------------------------------------------
/*! @brief Creates a new geometry label with the given name linked to one or two
           shape points.

    The label is not added to the graphics scene and remains invisible.
    To add the label also to the graphics scene the label must be shown.

    @param [in] i_strName
        Name of the label. The name must be unique otherwise no label is created.
    @param [in] i_labelType
        Range [EGraphObjTypeLabelGeometryPosition,
               EGraphObjTypeLabelGeometryDX, EGraphObjTypeLabelGeometryDY,
               EGraphObjTypeLabelGeometryLength, EGraphObjTypeLabelGeometryAngle]
        If not empty defines the text to be shown.
    @param [in] i_idxPt1
        First selection point the label should use to indicate the geometry.
    @param [in] i_idxPt2
        Second selection point the label should use to indicate the geometry.
        For position label only one selection point is used and the second selection
        point is set to invalid.

    @return true, if the label has been created and added, false otherwise.
*/
bool CGraphObj::addGeometryLabel(
    const QString& i_strName, EGraphObjType i_labelType,
    int i_idxPt1, int i_idxPt2)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName + ", " + graphObjType2Str(i_labelType)
            + ", " + QString::number(i_idxPt1) + ", " + QString::number(i_idxPt2);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::addGeometryLabel",
        /* strAddInfo   */ strMthInArgs );

    bool bCanAdd = !m_hshGeometryLabelDscrs.contains(i_strName);
    if (bCanAdd) {
        SLabelDscr labelDscr(i_labelType, i_strName);
        labelDscr.m_selPt1 = SGraphObjSelectionPoint(this, i_idxPt1);
        labelDscr.m_selPt2 = SGraphObjSelectionPoint(this, i_idxPt2);
        m_hshGeometryLabelDscrs.insert(i_strName, labelDscr);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bCanAdd);
    }
    return bCanAdd;
}

/*==============================================================================
protected slots: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Called by the drawing scene if the drawing size is changed.

    When changing the drawing size in metric unit dimension
    (e.g. on changing the Y Scale Orientation) the scene coordinates must be
    newly calculated even if the original values stored in metric units have not
    been changed. On changing the drawing size the the drawing scene will emit
    the signal "drawingSizeChanged" and the method MUST set the flag
    "m_bForceConversionToSceneCoors" to true before converting the coordinates
    and setting the converted values.
*/
void CGraphObj::onDrawingSizeChanged(const CDrawingSize& i_drawingSize)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_drawingSize.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::onDrawingSizeChanged",
        /* strAddInfo   */ strMthInArgs );

    m_bForceConversionToSceneCoors = true;

    // Here add code in your derived class to convert and recalculate the coordinates.
    // In the derived class the signal "geometryValuesUnitChanged" has to be emitted
    // if the unit of the drawing scene's size has been changed.

    m_bForceConversionToSceneCoors = false;
}

//------------------------------------------------------------------------------
void CGraphObj::onGraphObjParentScenePosChanged(CGraphObj* i_pGraphObjParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pGraphObjParent->keyInTree();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::onGraphObjParentScenePosChanged",
        /* strAddInfo   */ strMthInArgs );
}

//------------------------------------------------------------------------------
void CGraphObj::onGraphObjParentGeometryChanged(CGraphObj* i_pGraphObjParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pGraphObjParent->keyInTree();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::onGraphObjParentGeometryChanged",
        /* strAddInfo   */ strMthInArgs );
}

//------------------------------------------------------------------------------
void CGraphObj::onGraphObjParentZValueChanged(CGraphObj* i_pGraphObjParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pGraphObjParent->keyInTree();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::onGraphObjParentZValueChanged",
        /* strAddInfo   */ strMthInArgs );
}

//------------------------------------------------------------------------------
void CGraphObj::onSelectionPointGeometryChanged(CGraphObj* i_pSelectionPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pSelectionPoint->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::onSelectionPointGeometryChanged",
        /* strAddInfo   */ strMthInArgs );
}

//------------------------------------------------------------------------------
/*! Informs the grapical object that a selection point is going to be destroyed.

    If currently objects are selected their selection points are created.
    On clearing the graphics scene all items including selection points will
    be destroyed. Selection points may be destroyed before its parent object
    the selection points belong to. The parent object got to be informed if the
    selection point will be destroyed by someone else.

    @param i_pSelectionPoint [in]
        Pointer to selection point which will be destroyed.
*/
void CGraphObj::onSelectionPointAboutToBeDestroyed(CGraphObj* i_pSelectionPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pSelectionPoint == nullptr ? "nullptr" : i_pSelectionPoint->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::onSelectionPointAboutToBeDestroyed",
        /* strAddInfo   */ strMthInArgs );

    if (m_arpSelPtsBoundingRect.size() > 0) {
        for (int idxSelPt = m_arpSelPtsBoundingRect.size()-1; idxSelPt >= 0; idxSelPt--) {
            CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];
            if (pGraphObjSelPt == i_pSelectionPoint) {
                m_arpSelPtsBoundingRect[idxSelPt] = nullptr;
            }
        }
    }
    if (m_arpSelPtsPolygon.size() > 0) {
        for (int idxSelPt = m_arpSelPtsPolygon.size()-1; idxSelPt >= 0; idxSelPt--) {
            CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
            if (pGraphObjSelPt == i_pSelectionPoint) {
                m_arpSelPtsPolygon[idxSelPt] = nullptr;
            }
        }
    }
}

//------------------------------------------------------------------------------
/*! Informs the graphical object that one of its labels is going to be destroyed.

    On clearing the drawing scene all graphical objects will be destroyed.
    Labels may be destroyed before its parent object the labels belong to.
    The parent object got to be informed if the label will be destroyed by
    someone else.

    @param i_pLabel [in]
        Pointer to label which will be destroyed.
*/
void CGraphObj::onLabelAboutToBeDestroyed(CGraphObj* i_pLabel)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pLabel == nullptr ? "nullptr" : i_pLabel->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::onLabelAboutToBeDestroyed",
        /* strAddInfo   */ strMthInArgs );

    CGraphObjLabel* pGraphObjLabel = dynamic_cast<CGraphObjLabel*>(i_pLabel);
    if( m_hshpLabels.contains(pGraphObjLabel->key())) {
        m_hshpLabels.remove(pGraphObjLabel->key());
    }
}

//------------------------------------------------------------------------------
/*! Informs the graphical object that one of its labels is going to be destroyed.

    On clearing the drawing scene all graphical objects will be destroyed.
    Labels may be destroyed before its parent object the labels belong to.
    The parent object got to be informed if the label will be destroyed by
    someone else.

    @param i_pLabel [in]
        Pointer to label which will be destroyed.
*/
void CGraphObj::onGeometryLabelAboutToBeDestroyed(CGraphObj* i_pLabel)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pLabel == nullptr ? "nullptr" : i_pLabel->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::onGeometryLabelAboutToBeDestroyed",
        /* strAddInfo   */ strMthInArgs );

    CGraphObjLabel* pGraphObjLabel = dynamic_cast<CGraphObjLabel*>(i_pLabel);
    if (m_hshpGeometryLabels.contains(pGraphObjLabel->key())) {
        m_hshpGeometryLabels.remove(pGraphObjLabel->key());
    }
}

/*==============================================================================
protected: // overridables
==============================================================================*/

////------------------------------------------------------------------------------
//void CGraphObj::updateTransform()
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::updateTransform",
//        /* strAddInfo   */ "" );
//
//    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
//
//    if (pGraphicsItem != nullptr)
//    {
//        QTransform trs;
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//        trs.translate( m_ptRotOriginCurr.x(), m_ptRotOriginCurr.y() );
//        trs.rotate(-m_fRotAngleCurr_deg);
//        trs.translate( -m_ptRotOriginCurr.x(), -m_ptRotOriginCurr.y() );
//        //trsNew.scale( m_fScaleFacXCurr, m_fScaleFacYCurr );
//#endif
//        pGraphicsItem->resetTransform();
//        pGraphicsItem->setTransform(trs);
//    }
//} // updateTransform

////------------------------------------------------------------------------------
//void CGraphObj::updateToolTip()
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::updateToolTip",
//        /* strAddInfo   */ "" );
//
//    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
//
//    if (pGraphicsItem != nullptr)
//    {
//        m_strToolTip = "ObjPath:\t\t" + path();
//
//        // "scenePos" returns mapToScene(0,0). This is NOT equivalent to the
//        // position of the item's top left corner before applying the rotation
//        // transformation matrix but includes the transformation. What we want
//        // (or what I want) is the position of the item before rotating the item
//        // around the rotation origin point. In contrary it looks like "pos"
//        // always returns the top left corner before rotating the object.
//
//        QPointF ptPos;
//        if (pGraphicsItem->parentItem() != nullptr) {
//            ptPos = pGraphicsItem->pos();
//            m_strToolTip += "\nPos:\t\t" + point2Str(ptPos);
//        }
//        else {
//            ptPos = pGraphicsItem->pos(); // don't use "scenePos" here (see comment above)
//            m_strToolTip += "\nPos:\t\t" + point2Str(ptPos);
//        }
//
//        m_strToolTip += "\nSize:\t\t" + getSize(Units.Length.px).toString();
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//        m_strToolTip += "\nRotation:\t" + QString::number(m_fRotAngleCurr_deg,'f',1) + " " + c_strSymbolDegree;
//#endif
//        m_strToolTip += "\nZValue:\t\t" + QString::number(pGraphicsItem->zValue());
//
//        pGraphicsItem->setToolTip(m_strToolTip);
//    }
//} // updateToolTip

////------------------------------------------------------------------------------
//void CGraphObj::updateEditInfo()
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::updateEditInfo",
//        /* strAddInfo   */ "" );
//
//    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
//    if (pGraphicsItem != nullptr)
//    {
//        QString strAngleSymbol = QString(QChar(8738));
//        m_strEditInfo  = "C:" + point2Str( pGraphicsItem->mapToScene(pGraphicsItem->boundingRect().center()) );
//        m_strEditInfo += ", W:" + QString::number(pGraphicsItem->boundingRect().width(),'f',1);
//        m_strEditInfo += ", H:" + QString::number(pGraphicsItem->boundingRect().height(),'f',1);
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//        m_strEditInfo += ", " + strAngleSymbol + ":" + QString::number(m_fRotAngleCurr_deg,'f',1) + " " + c_strSymbolDegree;
//#endif
//    }
//}

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*!< @brief Emits the aboutToBeDestroyed signal if the signal has not already
            been emitted.

     Only the top most class in the hierachy of class inheritance should emit the signal.
     For this the signal may not be emitted "directly" by the derived classes but the class
     must call "emit_aboutToBeDestroyed" in their destructors.
*/
void CGraphObj::emit_aboutToBeDestroyed()
//------------------------------------------------------------------------------
{
    if (!m_bAboutToBeDestroyedEmitted) {
        CMethodTracer mthTracer(
            /* pAdminObj    */ m_pTrcAdminObjItemChange,
            /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strObjName   */ m_strName,
            /* strMethod    */ "CGraphObj::emit_aboutToBeDestroyed",
            /* strAddInfo   */ "" );
        emit aboutToBeDestroyed(this);
        m_bAboutToBeDestroyedEmitted = true;
    }
}

/*==============================================================================
protected: // auxiliary instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::emit_editModeChanged(const CEnumEditMode& i_eMode)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_eMode.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::emit_editModeChanged",
        /* strAddInfo   */ strMthInArgs );
    emit editModeChanged(this, i_eMode);
}

//------------------------------------------------------------------------------
void CGraphObj::emit_selectedChanged(bool i_bIsSelected)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bIsSelected);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::emit_selectedChanged",
        /* strAddInfo   */ strMthInArgs );
    emit selectedChanged(this, i_bIsSelected);
}

//------------------------------------------------------------------------------
void CGraphObj::emit_scenePosChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::emit_scenePosChanged",
        /* strAddInfo   */ "" );
    emit scenePosChanged(this);
}

//------------------------------------------------------------------------------
void CGraphObj::emit_geometryChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::emit_geometryChanged",
        /* strAddInfo   */ "" );
    emit geometryChanged(this);
}

//------------------------------------------------------------------------------
void CGraphObj::emit_geometryValuesUnitChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::emit_geometryValuesUnitChanged",
        /* strAddInfo   */ "" );
    emit geometryValuesUnitChanged(this);
}

//------------------------------------------------------------------------------
void CGraphObj::emit_zValueChanged(double i_fZValue)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fZValue);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::emit_zValueChanged",
        /* strAddInfo   */ strMthInArgs );
    emit zValueChanged(this, i_fZValue);
}

//------------------------------------------------------------------------------
void CGraphObj::emit_drawSettingsChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::emit_drawSettingsChanged",
        /* strAddInfo   */ "" );
    emit drawSettingsChanged(this);
}

//------------------------------------------------------------------------------
void CGraphObj::emit_labelAdded(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::emit_labelAdded",
        /* strAddInfo   */ strMthInArgs );
    emit labelAdded(this, i_strName);
}

//------------------------------------------------------------------------------
void CGraphObj::emit_labelRemoved(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::emit_labelRemoved",
        /* strAddInfo   */ strMthInArgs );
    emit labelRemoved(this, i_strName);
}

//------------------------------------------------------------------------------
void CGraphObj::emit_labelRenamed(const QString& i_strName, const QString& i_strNameNew)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName + ", " + i_strNameNew;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::emit_labelRenamed",
        /* strAddInfo   */ strMthInArgs );
    emit labelRenamed(this, i_strName, i_strNameNew);
}

//------------------------------------------------------------------------------
void CGraphObj::emit_labelChanged(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::emit_labelChanged",
        /* strAddInfo   */ strMthInArgs );
    emit labelChanged(this, i_strName);
}

//------------------------------------------------------------------------------
void CGraphObj::emit_geometryLabelChanged(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::emit_geometryLabelChanged",
        /* strAddInfo   */ strMthInArgs );
    emit geometryLabelChanged(this, i_strName);
}

/*==============================================================================
protected: // overridable auxiliary instance methods (method tracing)
==============================================================================*/

////------------------------------------------------------------------------------
///*! @brief Internal method to calculate and set the current scene position of the object.
//*/
//void CGraphObj::updateInternalScenePos()
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::updateInternalScenePos",
//        /* strAddInfo   */ "" );
//
//    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
//    if (pGraphicsItem != nullptr) {
//        QPointF ptScenePos = pGraphicsItem->scenePos();
//        setInternalScenePos(ptScenePos);
//    }
//}

////------------------------------------------------------------------------------
///*! @brief Internal method to set the current scene position of the object.
//
//    If the current scene position has been changed the scenePosChanged signal
//    is emitted.
//
//    @param [in] i_pos
//        New scene position of the object.
//*/
//void CGraphObj::setInternalScenePos(const QPointF& i_pos)
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = point2Str(i_pos);
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::setInternalScenePos",
//        /* strAddInfo   */ strMthInArgs );
//
//    if (m_ptScenePos != i_pos) {
//        m_ptScenePos = i_pos;
//        emit_scenePosChanged();
//    }
//}

//------------------------------------------------------------------------------
/*! @brief Internal method reimplementing the setPos call of graphics item.

    The setPos call is traced and in addition the current scene position of the
    object is tracked emitting the scenePosChanged signal if the scene position
    has been changed.

    @param [in] i_pos
        New position of the object in parent coordinates.
*/
void CGraphObj::QGraphicsItem_setPos(const QPointF& i_pos)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = point2Str(i_pos);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::QGraphicsItem_setPos",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr) {
        pGraphicsItem->setPos(i_pos);
    }
}

/*==============================================================================
protected: // overridable auxiliary instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::tracePositionInfo(
    CMethodTracer& i_mthTracer, EMethodDir i_mthDir, ELogDetailLevel i_detailLevel) const
//------------------------------------------------------------------------------
{
    traceThisPositionInfo(i_mthTracer, i_mthDir, i_detailLevel);
    traceParentGroupPositionInfo(i_mthTracer, i_mthDir, i_detailLevel);
}

//------------------------------------------------------------------------------
void CGraphObj::traceThisPositionInfo(
    CMethodTracer& i_mthTracer, EMethodDir i_mthDir, ELogDetailLevel i_detailLevel) const
//------------------------------------------------------------------------------
{
    if (i_mthTracer.isRuntimeInfoActive(i_detailLevel)) {
        const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
        if (pGraphicsItemThis != nullptr) {
            QPointF ptPos = pGraphicsItemThis->pos();
            QPointF ptScenePos = pGraphicsItemThis->scenePos();
            QRectF rectBounding = getBoundingRect();
            QPointF ptCenterPos = rectBounding.center();
            QString strRuntimeInfo;
            if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
            else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
            else strRuntimeInfo = "   ";
            strRuntimeInfo += "BoundingRect {" + qRect2Str(rectBounding) + "}, Center {" + qPoint2Str(ptCenterPos) + "}";
            i_mthTracer.trace(strRuntimeInfo);
            if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
            else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
            else strRuntimeInfo = "   ";
            strRuntimeInfo += "ParentPos {" + qPoint2Str(ptPos) + "}, ScenePos {" + qPoint2Str(ptScenePos) + "}";
            i_mthTracer.trace(strRuntimeInfo);
        }
    }
}

//------------------------------------------------------------------------------
void CGraphObj::traceParentGroupPositionInfo(
    CMethodTracer& i_mthTracer, EMethodDir i_mthDir, ELogDetailLevel i_detailLevel) const
//------------------------------------------------------------------------------
{
    if (i_mthTracer.isRuntimeInfoActive(i_detailLevel)) {
        const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
        if (pGraphicsItemThis != nullptr) {
            QGraphicsItem* pGraphicsItemParent = pGraphicsItemThis->parentItem();
            CGraphObjGroup* pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(pGraphicsItemParent);
            if (pGraphicsItemParent != nullptr && pGraphObjGroup != nullptr) {
                QPointF ptPos = pGraphicsItemParent->pos();
                QPointF ptScenePos = pGraphicsItemParent->scenePos();
                QRectF rectBounding = pGraphObjGroup->getBoundingRect();
                QPointF ptCenterPos = rectBounding.center();
                QString strRuntimeInfo;
                if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
                else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
                else strRuntimeInfo = "   ";
                strRuntimeInfo += "ParentGroup: " + QString(pGraphObjGroup == nullptr ? "null" : pGraphObjGroup->path());
                i_mthTracer.trace(strRuntimeInfo);
                if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
                else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
                else strRuntimeInfo = "   ";
                strRuntimeInfo += "  PhysValRect {" + pGraphObjGroup->getRect().toString() + "}";
                i_mthTracer.trace(strRuntimeInfo);
                if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
                else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
                else strRuntimeInfo = "   ";
                strRuntimeInfo += "  BoundingRect {" + qRect2Str(rectBounding) + "}, Center {" + qPoint2Str(ptCenterPos) + "}";
                i_mthTracer.trace(strRuntimeInfo);
                if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
                else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
                else strRuntimeInfo = "   ";
                strRuntimeInfo += "  ParentPos {" + qPoint2Str(ptPos) + "}, ScenePos {" + qPoint2Str(ptScenePos) + "}";
                i_mthTracer.trace(strRuntimeInfo);
            }
        }
    }
}

//------------------------------------------------------------------------------
void CGraphObj::traceGraphicsItemStates(
    CMethodTracer& i_mthTracer, EMethodDir i_mthDir, ELogDetailLevel i_detailLevel) const
//------------------------------------------------------------------------------
{
    if (i_mthTracer.isRuntimeInfoActive(i_detailLevel)) {
        const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
        if (pGraphicsItemThis != nullptr) {
            QString strRuntimeInfo;

            if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
            else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
            else strRuntimeInfo = "   ";
            strRuntimeInfo += "AcceptHoverEvents: " + bool2Str(pGraphicsItemThis->acceptHoverEvents()) +
                             ", IsSelected: " + bool2Str(pGraphicsItemThis->isSelected()) +
                             ", IsVisible: " + bool2Str(pGraphicsItemThis->isVisible()) +
                             ", IsEnabled: " + bool2Str(pGraphicsItemThis->isEnabled());
            i_mthTracer.trace(strRuntimeInfo);

            if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
            else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
            else strRuntimeInfo = "   ";
            strRuntimeInfo += "AcceptedMouseButtons {" + qMouseButtons2Str(pGraphicsItemThis->acceptedMouseButtons()) + "}";
            i_mthTracer.trace(strRuntimeInfo);

            if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
            else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
            else strRuntimeInfo = "   ";
            strRuntimeInfo += "Flags {" + qGraphicsItemFlags2Str(pGraphicsItemThis->flags()) + "}";
            i_mthTracer.trace(strRuntimeInfo);
        }
    }
}

//------------------------------------------------------------------------------
void CGraphObj::traceGraphObjStates(
    CMethodTracer& i_mthTracer, EMethodDir i_mthDir, ELogDetailLevel i_detailLevel) const
//------------------------------------------------------------------------------
{
    if (i_mthTracer.isRuntimeInfoActive(i_detailLevel)) {
        QString strRuntimeInfo;

        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = "   ";
        strRuntimeInfo += "IsHit: " + bool2Str(m_bIsHit) + ", IsHighlighted: " + bool2Str(m_bIsHighlighted);
        i_mthTracer.trace(strRuntimeInfo);

        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = "   ";
        strRuntimeInfo += "ItemChangeUpdateOriginalCoorsBlockedCounter: " + QString::number(m_iItemChangeUpdateOriginalCoorsBlockedCounter);
        i_mthTracer.trace(strRuntimeInfo);
    }
}
