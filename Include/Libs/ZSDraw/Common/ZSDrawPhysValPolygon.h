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

#ifndef ZSDraw_PhysValPolygon_h
#define ZSDraw_PhysValPolygon_h

#include "ZSDraw/Common/ZSDrawPhysValPolyline.h"
#include "QtCore/qline.h"

namespace ZS
{
namespace Draw
{
//******************************************************************************
/*! @brief Represents a polygon on the drawing scene.

    Corresponds to CPhysValPolyline but connects the last point of the polygon
    with the first point of the polygon (closed polyline).

    Converting points from pixels to metric units is not supported.
    This conversion would have to be done by the parent group or scene to take
    the different Y-Scale-Axis orientation into account.
    But the polyline does not know about groups.
*/
class ZSDRAWDLL_API CPhysValPolygon : public CPhysValPolyline
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CPhysValPolygon"; }
public: // ctors
    CPhysValPolygon(const CDrawingScene& i_drawingScene);
    CPhysValPolygon(const CDrawingScene& i_drawingScene, const ZS::PhysVal::CUnit& i_unit);
    CPhysValPolygon(const CDrawingScene& i_drawingScene, QPolygonF i_polygon);
    CPhysValPolygon(const CDrawingScene& i_drawingScene, QPolygonF i_polygon, const ZS::PhysVal::CUnit& i_unit);
public: // copy ctor
    CPhysValPolygon(const CPhysValPolygon& i_physValPolygonOther);
public: // dtor
    virtual ~CPhysValPolygon();
public: // must overridables of base class CPhysValShape
    void draw(QPainter* i_pPainter, const QRectF& i_rect, const CDrawSettings& i_drawSettings) override;

}; // class CPhysValPolygon

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_PhysValPolygon_h
