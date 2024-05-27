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

#ifndef ZSDraw_PhysValRect_h
#define ZSDraw_PhysValRect_h

#include "ZSDraw/Common/ZSDrawPhysValPoint.h"
#include "ZSDraw/Common/ZSDrawPhysValLine.h"
#include "ZSDraw/Common/ZSDrawPhysValSize.h"
#include "ZSDraw/Common/ZSDrawCommon.h"
#include "QtCore/qrect.h"

namespace ZS
{
namespace Draw
{
//******************************************************************************
/*! @brief Represents a rectangle on the drawing scene.

    Corresponds to QRectF but providing a physical unit and a resolution and
    the possibility to rotate the rectangle.

    Converting points (corners and center of the rectangle) from pixels to metric
    units is not supported. This conversion would have to be done by the parent
    group or scene to take the different Y-Scale-Axis orientation into account.
    But the line does not know about groups.
*/
class ZSDRAWDLL_API CPhysValRect : public CPhysValShape
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CPhysValRect"; }
public: // ctors
    CPhysValRect(const CDrawingScene& i_drawingScene);
    CPhysValRect(const CDrawingScene& i_drawingScene, const ZS::PhysVal::CUnit& i_unit);
    CPhysValRect(
        const CDrawingScene& i_drawingScene,
        const QPointF& i_ptTL, const QPointF& i_ptBR);
    CPhysValRect(
        const CDrawingScene& i_drawingScene,
        const QPointF& i_ptTL, const QPointF& i_ptBR,
        const ZS::PhysVal::CUnit& i_unit);
    CPhysValRect(
        const CDrawingScene& i_drawingScene,
        const QPointF& i_ptTL, const QSizeF& i_size);
    CPhysValRect(
        const CDrawingScene& i_drawingScene,
        const QPointF& i_ptTL, const QSizeF& i_size,
        const ZS::PhysVal::CUnit& i_unit);
public: // ctors
    CPhysValRect(
        const CDrawingScene& i_drawingScene,
        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY);
    CPhysValRect(
        const CDrawingScene& i_drawingScene,
        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
        const ZS::PhysVal::CUnit& i_unit);
    CPhysValRect(
        const CDrawingScene& i_drawingScene,
        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
        const QPointF& i_ptTL, const QPointF& i_ptBR);
    CPhysValRect(
        const CDrawingScene& i_drawingScene,
        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
        const QPointF& i_ptTL, const QPointF& i_ptBR,
        const ZS::PhysVal::CUnit& i_unit);
    CPhysValRect(
        const CDrawingScene& i_drawingScene,
        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
        const QPointF& i_ptTL, const QSizeF& i_size);
    CPhysValRect(
        const CDrawingScene& i_drawingScene,
        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
        const QPointF& i_ptTL, const QSizeF& i_size,
        const ZS::PhysVal::CUnit& i_unit);
public: // ctors
    CPhysValRect(const CPhysValPoint& i_physValTopLeft, const CPhysValPoint& i_physValBottomRight);
    CPhysValRect(const CPhysValPoint& i_physValTopLeft, const CPhysValSize& i_physValSize);
public: // copy ctor
    CPhysValRect(const CPhysValRect& i_physValRectOther);
public: // dtor
    virtual ~CPhysValRect();
public: // must overridable operators of base class CPhysValShape
    CPhysValShape& operator = (const CPhysValShape& i_physValRectOther) override;
public: // must overridables of base class CPhysValShape
    bool operator == (const CPhysValShape& i_physValRectOther) const override;
    bool operator != (const CPhysValShape& i_physValRectOther) const override;
public: // must overridables of base class CPhysValShape
    bool isValid() const override;
    bool isNull() const override;
    void draw(QPainter* i_pPainter, const QRectF& i_rect, const CDrawSettings& i_drawSettings) override;
    QString toString(bool i_bAddUnit = false, const QString& i_strSeparator = ", ") const override;
public: // instance methods
    CPhysValPoint center() const;
    CPhysValSize size() const;
    ZS::PhysVal::CPhysVal width() const;
    ZS::PhysVal::CPhysVal height() const;
    ZS::PhysVal::CPhysVal angle() const;
    CPhysValPoint topLeft() const;
    CPhysValPoint topRight() const;
    CPhysValPoint bottomRight() const;
    CPhysValPoint bottomLeft() const;
    CPhysValPoint topCenter() const;
    CPhysValPoint rightCenter() const;
    CPhysValPoint bottomCenter() const;
    CPhysValPoint leftCenter() const;
    double resolution() const;
    ZS::PhysVal::CUnit unit() const;
public: // instance methods
    void setCenter(const QPointF& i_pt);
    void setCenter(const CPhysValPoint& i_physValPoint);
    void setSize(const QSizeF& i_size);
    void setSize(const CPhysValSize& i_physValSize);
    void setWidth(double i_fWidth);
    void setWidth(const ZS::PhysVal::CPhysVal& i_physValWidth);
    void setWidthByMovingLeftCenter(const QPointF& i_pt);
    void setWidthByMovingLeftCenter(const CPhysValPoint& i_physValPoint);
    void setWidthByMovingRightCenter(const QPointF& i_pt);
    void setWidthByMovingRightCenter(const CPhysValPoint& i_physValPoint);
    void setHeight(double i_fHeight);
    void setHeight(const ZS::PhysVal::CPhysVal& i_physValHeight);
    void setHeightByMovingTopCenter(const QPointF& i_pt);
    void setHeightByMovingTopCenter(const CPhysValPoint& i_physValPoint);
    void setHeightByMovingBottomCenter(const QPointF& i_pt);
    void setHeightByMovingBottomCenter(const CPhysValPoint& i_physValPoint);
    void setAngle(const ZS::PhysVal::CPhysVal& i_physValAngle);
    void setTopLeft(const QPointF& i_pt);
    void setTopLeft(const CPhysValPoint& i_physValPoint);
    void setTopRight(const QPointF& i_pt);
    void setTopRight(const CPhysValPoint& i_physValPoint);
    void setBottomRight(const QPointF& i_pt);
    void setBottomRight(const CPhysValPoint& i_physValPoint);
    void setBottomLeft(const QPointF& i_pt);
    void setBottomLeft(const CPhysValPoint& i_physValPoint);
protected: // auxiliary functions
    void initSelectionPoints();
    void invalidateSelectionPoints(quint16 i_uSelectionPointsToExclude = 0x0000);
    QPointF getCornerPoint(ESelectionPoint i_selPt) const;
protected: // instance members
    /*!< The rectangles center coordinates in the unit 'm_unit'. */
    QPointF m_ptCenter;
    /*!< The rectangles width and height in the unit 'm_unit'. */
    QSizeF m_size;
    /*!< The rotation angle (counted clockwise). */
    ZS::PhysVal::CPhysVal m_physValAngle;
    /*!< Corner and other selection points of a rotated rectangle can be calculated from the
         center point, the size (width and height) and the rotation angle.
         As time consuming trigonometric functions and the mathematical root function must be
         used to calculate the corner points, the corner points (and other selection points)
         are only calculated if needed. A dirty flag indicates for each point whether calculation
         is needed. */
    mutable QVector<CPhysValPoint> m_arphysValPoints;
    mutable QVector<bool> m_arbPointsCalculated;

}; // class CPhysValRect

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_PhysValRect_h
