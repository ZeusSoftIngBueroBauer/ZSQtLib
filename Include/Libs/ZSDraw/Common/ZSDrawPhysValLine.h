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

#ifndef ZSDraw_PhysValLine_h
#define ZSDraw_PhysValLine_h

#include "ZSDraw/Common/ZSDrawPhysValPoint.h"
#include "QtCore/qline.h"

namespace ZS
{
namespace Draw
{
//******************************************************************************
/*! @brief Represents a line on the drawing scene.

    Corresponds to QLineF but providing a physical unit and a resolution.

    Converting points (start and end point of the line) from pixels to metric
    units is not supported. This conversion would have to be done by the parent
    group or scene to take the different Y-Scale-Axis orientation into account.
    But the line does not know about groups.
*/
class ZSDRAWDLL_API CPhysValLine : public CPhysValShape
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CPhysValLine"; }
public: // ctors
    CPhysValLine(const CDrawingScene& i_drawingScene);
    CPhysValLine(const CDrawingScene& i_drawingScene, const ZS::PhysVal::CUnit& i_unit);
    CPhysValLine(
        const CDrawingScene& i_drawingScene,
        double i_fX1, double i_fY1, double i_fX2, double i_fY2);
    CPhysValLine(
        const CDrawingScene& i_drawingScene,
        double i_fX1, double i_fY1, double i_fX2, double i_fY2,
        const ZS::PhysVal::CUnit& i_unit);
    CPhysValLine(
        const CDrawingScene& i_drawingScene,
        const QPointF& i_p1, const QPointF& i_p2);
    CPhysValLine(
        const CDrawingScene& i_drawingScene,
        const QPointF& i_p1, const QPointF& i_p2,
        const ZS::PhysVal::CUnit& i_unit);
    CPhysValLine(
        const CDrawingScene& i_drawingScene,
        const QLineF& i_line);
    CPhysValLine(
        const CDrawingScene& i_drawingScene,
        const QLineF& i_line,
        const ZS::PhysVal::CUnit& i_unit);
//public: // ctors
//    CPhysValLine(
//        const CDrawingScene& i_drawingScene,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY);
//    CPhysValLine(
//        const CDrawingScene& i_drawingScene,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
//        const ZS::PhysVal::CUnit& i_unit);
//    CPhysValLine(
//        const CDrawingScene& i_drawingScene,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
//        double i_fX1, double i_fY1, double i_fX2, double i_fY2);
//    CPhysValLine(
//        const CDrawingScene& i_drawingScene,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
//        double i_fX1, double i_fY1, double i_fX2, double i_fY2,
//        const ZS::PhysVal::CUnit& i_unit);
//    CPhysValLine(
//        const CDrawingScene& i_drawingScene,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
//        const QPointF& i_p1, const QPointF& i_p2);
//    CPhysValLine(
//        const CDrawingScene& i_drawingScene,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
//        const QPointF& i_p1, const QPointF& i_p2,
//        const ZS::PhysVal::CUnit& i_unit);
//    CPhysValLine(
//        const CDrawingScene& i_drawingScene,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
//        const QLineF& i_line);
//    CPhysValLine(
//        const CDrawingScene& i_drawingScene,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
//        const QLineF& i_line,
//        const ZS::PhysVal::CUnit& i_unit);
public: // ctors
    CPhysValLine(const CPhysValPoint& i_physValPoint1, const CPhysValPoint& i_physValPoint2);
public: // copy ctor
    CPhysValLine(const CPhysValLine& i_physValLineOther);
public: // dtor
    virtual ~CPhysValLine();
public: // operators
    CPhysValLine& operator = (const QLineF& i_line);
public: // must overridable operators of base class CPhysValShape
    CPhysValShape& operator = (const CPhysValShape& i_physValLineOther) override;
public: // must overridables of base class CPhysValShape
    bool operator == (const CPhysValShape& i_physValLineOther) const override;
    bool operator != (const CPhysValShape& i_physValLineOther) const override;
public: // must overridables of base class CPhysValShape
    void invalidate() override;
    bool isValid() const override;
    bool isNull() const override;
    void draw(QPainter* i_pPainter, const QRectF& i_rect, const CDrawSettings& i_drawSettings) override;
    QString toString(bool i_bAddUnit = false, const QString& i_strSeparator = ", ") const override;
public: // instance methods
    CPhysValPoint p1() const;
    CPhysValPoint p2() const;
    ZS::PhysVal::CPhysVal x1() const;
    ZS::PhysVal::CPhysVal x2() const;
    ZS::PhysVal::CPhysVal y1() const;
    ZS::PhysVal::CPhysVal y2() const;
    ZS::PhysVal::CPhysVal dx() const;
    ZS::PhysVal::CPhysVal dy() const;
    ZS::PhysVal::CPhysVal length() const;
    CPhysValPoint center() const;
    ZS::PhysVal::CPhysVal angle() const;
    ZS::PhysVal::CPhysVal angleTo(const CPhysValLine& i_physValLineOther) const;
    double resolution() const;
    ZS::PhysVal::CUnit unit() const;
    QLineF::IntersectionType intersects(const CPhysValLine& i_physValLineOther, CPhysValPoint* i_physValPointIntersection) const;
public: // instance methods
    void setP1(const QPointF& i_p1);
    void setP1(const CPhysValPoint& i_physValPoint1);
    void setP2(const CPhysValPoint& i_physValPoint2);
    void setP2(const QPointF& i_p2);
    void setLine(double i_fX1, double i_fY1, double i_fX2, double i_fY2, const ZS::PhysVal::CUnit& i_unit);
    void setLine(const QLineF& i_line, const ZS::PhysVal::CUnit& i_unit);
    void setPoints(const QPointF& i_p1, const QPointF& i_p2, const ZS::PhysVal::CUnit& i_unit);
    void setPoints(const CPhysValPoint& i_physValP1, const CPhysValPoint& i_physValP2);
    void setDX(double i_fDX);
    void setDX(const ZS::PhysVal::CPhysVal& i_physValDX);
    void setDY(double i_fDY);
    void setDY(const ZS::PhysVal::CPhysVal& i_physValDY);
    void setLength(double i_fLength);
    void setLength(const ZS::PhysVal::CPhysVal& i_physValLength);
    void setCenter(const QPointF& i_pCenter);
    void setCenter(const CPhysValPoint& i_physValPointCenter);
    void setAngle(const ZS::PhysVal::CPhysVal& i_physValAngle);
public: // instance methods (to convert the values into another unit)
    QLineF toQLineF() const;
    QLineF toQLineF(const ZS::PhysVal::CUnit& i_unit) const;
protected: // instance members
    /*!< Values of the line stored in "m_unit". */
    QLineF m_line;

}; // class CPhysValLine

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_PhysValLine_h
