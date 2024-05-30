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

#ifndef ZSDraw_PhysValPoint_h
#define ZSDraw_PhysValPoint_h

#include "ZSDraw/Common/ZSDrawPhysValShape.h"
#include "QtCore/qpoint.h"

namespace ZS
{
namespace Draw
{
//******************************************************************************
/*! @brief Represents a point on the drawing scene.

    Corresponds to QPointF but providing a physical unit and a resolution.

    Converting points from pixels to metric units is not supported.
    This conversion would have to be done by the parent group or scene to take
    the different Y-Scale-Axis orientation into account.
    But the point does not know about groups.
*/
class ZSDRAWDLL_API CPhysValPoint : public CPhysValShape
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CPhysValPoint"; }
public: // ctors
    CPhysValPoint(const CDrawingScene& i_drawingScene);
    CPhysValPoint(const CDrawingScene& i_drawingScene, const ZS::PhysVal::CUnit& i_unit);
    CPhysValPoint(
        const CDrawingScene& i_drawingScene,
        double i_fX, double i_fY);
    CPhysValPoint(
        const CDrawingScene& i_drawingScene,
        double i_fX, double i_fY,
        const ZS::PhysVal::CUnit& i_unit);
    CPhysValPoint(
        const CDrawingScene& i_drawingScene,
        const QPointF& i_pt);
    CPhysValPoint(
        const CDrawingScene& i_drawingScene,
        const QPointF& i_pt,
        const ZS::PhysVal::CUnit& i_unit);
    CPhysValPoint(
        const CDrawingScene& i_drawingScene,
        const ZS::PhysVal::CPhysVal& i_physValX, const ZS::PhysVal::CPhysVal& i_physValY);
//public: // ctors
//    CPhysValPoint(
//        const CDrawingScene& i_drawingScene,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY);
//    CPhysValPoint(
//        const CDrawingScene& i_drawingScene,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
//        const ZS::PhysVal::CUnit& i_unit);
//    CPhysValPoint(
//        const CDrawingScene& i_drawingScene,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
//        double i_fX, double i_fY);
//    CPhysValPoint(
//        const CDrawingScene& i_drawingScene,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
//        double i_fX, double i_fY,
//        const ZS::PhysVal::CUnit& i_unit);
//    CPhysValPoint(
//        const CDrawingScene& i_drawingScene,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
//        const QPointF& i_pt);
//    CPhysValPoint(
//        const CDrawingScene& i_drawingScene,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
//        const QPointF& i_pt,
//        const ZS::PhysVal::CUnit& i_unit);
//    CPhysValPoint(
//        const CDrawingScene& i_drawingScene,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
//        const ZS::PhysVal::CPhysVal& i_physValX, const ZS::PhysVal::CPhysVal& i_physValY);
public: // copy ctor
    CPhysValPoint(const CPhysValPoint& i_physValPointOther);
public: // dtor
    ~CPhysValPoint();
public: // operators
    CPhysValPoint& operator = (const QPointF& i_ptOther);
    CPhysValPoint& operator = (const QString& i_strValOther);
public: // must overridable operators of base class CPhysValShape
    CPhysValShape& operator = (const CPhysValShape& i_physValPointOther) override;
public: // must overridables of base class CPhysValShape
    bool operator == (const CPhysValShape& i_physValPointOther) const override;
    bool operator != (const CPhysValShape& i_physValPointOther) const override;
public: // must overridables of base class CPhysValShape
    bool isValid() const override;
    bool isNull() const override;
    void draw(QPainter* i_pPainter, const QRectF& i_rect, const CDrawSettings& i_drawSettings) override;
    QString toString(bool i_bAddUnit = false, const QString& i_strSeparator = ", ") const override;
public: // instance methods
    ZS::PhysVal::CPhysVal x() const;
    ZS::PhysVal::CPhysVal y() const;
    double resolution() const;
    ZS::PhysVal::CUnit unit() const;
public: // instance methods
    void setX(const ZS::PhysVal::CPhysVal& i_physValX);
    void setY(const ZS::PhysVal::CPhysVal& i_physValY);
public: // instance methods (to convert the values into another unit)
    QPointF toQPointF() const;
    QPointF toQPointF(const ZS::PhysVal::CUnit& i_unit) const;
protected: // instance members
    /*!< The point coordinates stored in the unit 'm_unit'. */
    QPointF m_pt;

}; // class CPhysValPoint

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_PhysValPoint_h
