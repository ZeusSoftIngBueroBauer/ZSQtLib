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

#include "ZSDraw/Common/ZSDrawPhysValPolygon.h"
#include "ZSDraw/Common/ZSDrawUnits.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysMath.h"

#include <QtGui/qpainter.h>

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::Draw;


/*******************************************************************************
class CPhysValPolygon
*******************************************************************************/

/*==============================================================================
public: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates a physical polyline on the drawing scene in the current unit
           and current resolution of the drawing scene.
*/
CPhysValPolygon::CPhysValPolygon(const CDrawingScene& i_drawingScene) :
//------------------------------------------------------------------------------
    CPhysValPolyline(i_drawingScene)
{
}

//------------------------------------------------------------------------------
/*! @brief Creates a physical polyline on the drawing scene in the current unit
           and current resolution of the drawing scene.
*/
CPhysValPolygon::CPhysValPolygon(const CDrawingScene& i_drawingScene, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    CPhysValPolyline(i_drawingScene, i_unit)
{
}

//------------------------------------------------------------------------------
CPhysValPolygon::CPhysValPolygon(const CDrawingScene& i_drawingScene, QPolygonF i_polygon) :
//------------------------------------------------------------------------------
    CPhysValPolyline(i_drawingScene, std::move(i_polygon))
{
}

//------------------------------------------------------------------------------
CPhysValPolygon::CPhysValPolygon(
    const CDrawingScene& i_drawingScene, QPolygonF i_polygon, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    CPhysValPolyline(i_drawingScene, std::move(i_polygon), i_unit)
{
}

/*==============================================================================
public: // copy ctor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValPolygon::CPhysValPolygon(const CPhysValPolygon& i_physValPolygonOther) :
//------------------------------------------------------------------------------
    CPhysValPolyline(i_physValPolygonOther)
{
}

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValPolygon::~CPhysValPolygon()
//------------------------------------------------------------------------------
{
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValPolygon& CPhysValPolygon::operator = ( const QPolygonF& i_polyline )
//------------------------------------------------------------------------------
{
    m_polygonOrig = i_polyline;
    m_polygonModified = i_polyline;
    m_physValRect.setSize(CPhysValSize(*m_pDrawingScene, m_polygonOrig.boundingRect().size(), m_unit));
    m_physValRect.setCenter(CPhysValPoint(*m_pDrawingScene, m_polygonOrig.boundingRect().center(), m_unit));
    m_ptCenterOrig = m_physValRect.center().toQPointF();
    m_sizeOrig = m_physValRect.size().toQSizeF();
    return *this;
}

/*==============================================================================
public: // must overridable operators of base class CPhysValShape
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValShape& CPhysValPolygon::operator = ( const CPhysValShape& i_physValPolylineOther )
//------------------------------------------------------------------------------
{
    const CPhysValPolygon& physValPolygonOther = dynamic_cast<const CPhysValPolygon&>(i_physValPolylineOther);
    m_bYAxisTopDown = physValPolygonOther.m_bYAxisTopDown;
    m_unit = physValPolygonOther.m_unit;
    m_physValRect = physValPolygonOther.m_physValRect;
    m_ptCenterOrig = physValPolygonOther.m_ptCenterOrig;
    m_sizeOrig = physValPolygonOther.m_sizeOrig;
    m_polygonOrig = physValPolygonOther.m_polygonOrig;
    m_polygonModified = physValPolygonOther.m_polygonModified;
    return *this;
}

/*==============================================================================
public: // must overridables of base class CPhysValShape
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Draws the shape on the drawing scene.
*/
void CPhysValPolygon::draw(QPainter* i_pPainter, const QRectF& i_rect, const CDrawSettings& i_drawSettings)
//------------------------------------------------------------------------------
{
    i_pPainter->save();
    QPen pen(i_drawSettings.getPenColor());
    pen.setStyle(Qt::SolidLine);
    i_pPainter->setPen(pen);
    QPolygonF polygon_px = m_polygonModified;
    if (m_unit != Units.Length.px) {
        for (int idxPt = 0; idxPt < polygon_px.size(); ++idxPt) {
            polygon_px[idxPt] = m_pDrawingScene->convert(at(idxPt), Units.Length.px).toQPointF();
        }
    }
    if (polygon_px.size() > 1) {
        for (int idxPt = 0; idxPt < polygon_px.size()-1; ++idxPt) {
            i_pPainter->drawLine(polygon_px[idxPt], polygon_px[idxPt+1]);
        }
        i_pPainter->drawLine(polygon_px[polygon_px.size()-1], polygon_px[0]);
    }
    for (int idxPt = 0; idxPt < polygon_px.size(); ++idxPt) {
        i_pPainter->drawText(polygon_px[idxPt], "P" + QString::number(idxPt));
        break;
    }
    QPointF ptCenter = m_pDrawingScene->convert(m_physValRect.center(), Units.Length.px).toQPointF();
    QPointF ptTL = m_pDrawingScene->convert(m_physValRect.topLeft(), Units.Length.px).toQPointF();
    QPointF ptTR = m_pDrawingScene->convert(m_physValRect.topRight(), Units.Length.px).toQPointF();
    QPointF ptBR = m_pDrawingScene->convert(m_physValRect.bottomRight(), Units.Length.px).toQPointF();
    QPointF ptBL = m_pDrawingScene->convert(m_physValRect.bottomLeft(), Units.Length.px).toQPointF();
    QPointF ptTC = m_pDrawingScene->convert(m_physValRect.topCenter(), Units.Length.px).toQPointF();
    QPointF ptRC = m_pDrawingScene->convert(m_physValRect.rightCenter(), Units.Length.px).toQPointF();
    QPointF ptBC = m_pDrawingScene->convert(m_physValRect.bottomCenter(), Units.Length.px).toQPointF();
    QPointF ptLC = m_pDrawingScene->convert(m_physValRect.leftCenter(), Units.Length.px).toQPointF();
    pen.setStyle(Qt::DotLine);
    i_pPainter->setPen(pen);
    i_pPainter->drawLine(ptTL, ptTR);
    i_pPainter->drawLine(ptTR, ptBR);
    i_pPainter->drawLine(ptBR, ptBL);
    i_pPainter->drawLine(ptBL, ptTL);
    pen.setStyle(Qt::DashDotDotLine);
    i_pPainter->setPen(pen);
    i_pPainter->drawLine(ptTL, ptBR);
    i_pPainter->drawLine(ptTR, ptBL);
    pen.setStyle(Qt::DashLine);
    i_pPainter->setPen(pen);
    i_pPainter->drawLine(ptLC, ptRC);
    i_pPainter->drawLine(ptTC, ptBC);
    if (pen.color() == Qt::red) {
        i_pPainter->drawText(ptTL, "TL");
        i_pPainter->drawText(ptTR, "TR");
        i_pPainter->drawText(ptBR, "BR");
        i_pPainter->drawText(ptBL, "BL");
    }
    i_pPainter->restore();
}
