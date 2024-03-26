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
/*! @brief Corresponds to QLineF but providing a physical unit and a resolution.

    This class is not part of ZS::PhysVal as the namespace ZS::PhysVal is abstract
    concerning units and the units "pixels" or any metric units is not known within
    the ZS::PhysVal library. The corresponding unit tree must be configured for the
    specific application or, as in this case, in the units tree of the ZS::Draw library.

    In addition to convert between metric and pixel dimensions (and vice versa)
    the screen resolution and the scale factor must be known. Both are properties
    of the physical size "Length" within the ZS::Draw library.
*/
class ZSDRAWDLL_API CPhysValLine
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CPhysValLine"; }
public: // ctors and dtor
    CPhysValLine();
    CPhysValLine(const CDrawingScene& i_drawingScene);
    CPhysValLine(const ZS::PhysVal::CUnit& i_unit, double i_fRes);
    CPhysValLine(double i_fX1, double i_fY1, double i_fX2, double i_fY2, double i_fRes, const ZS::PhysVal::CUnit& i_unit);
    CPhysValLine(const QPointF& i_p1, const QPointF& i_p2, double i_fRes, const ZS::PhysVal::CUnit& i_unit);
    CPhysValLine(const QLineF& i_line, double i_fRes, const ZS::PhysVal::CUnit& i_unit);
    CPhysValLine(const CPhysValPoint& i_physValPoint1, const CPhysValPoint& i_physValPoint2);
    CPhysValLine(const CPhysValLine& i_physValLineOther);
    virtual ~CPhysValLine();
public: // operators
    CPhysValLine& operator = (const CPhysValLine& i_physValLineOther);
    CPhysValLine& operator = (const QLineF& i_line);
public: // operators
    bool operator == (const CPhysValLine& i_physValLineOther) const;
    bool operator != (const CPhysValLine& i_physValLineOther) const;
public: // instance methods
    bool isValid() const;
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
    ZS::PhysVal::CPhysVal angle(const ZS::System::CEnumYScaleAxisOrientation& i_yScaleScaleOrientation) const;
    ZS::PhysVal::CPhysVal angleTo(const CPhysValLine& i_physValLineOther) const;
    CPhysValPoint center() const;
    double resolution() const;
    ZS::PhysVal::CUnit unit() const;
    QLineF::IntersectionType intersects(const CPhysValLine& i_physValLineOther, CPhysValPoint* i_physValPointIntersection) const;
    bool isNull() const;
public: // instance methods
    void setP1(const QPointF& i_p1);
    void setP1(const CPhysValPoint& i_physValPoint1);
    void setP2(const CPhysValPoint& i_physValPoint2);
    void setP2(const QPointF& i_p2);
    void setLine(double i_fX1, double i_fY1, double i_fX2, double i_fY2, double i_fRes, const ZS::PhysVal::CUnit& i_unit);
    void setLine(const QLineF& i_line, double i_fRes, const ZS::PhysVal::CUnit& i_unit);
    void setPoints(const QPointF& i_p1, const QPointF& i_p2, double i_fRes, const ZS::PhysVal::CUnit& i_unit);
    void setPoints(const CPhysValPoint& i_physValP1, const CPhysValPoint& i_physValP2);
    void setDX(const ZS::PhysVal::CPhysVal& i_physValDX);
    void setDY(const ZS::PhysVal::CPhysVal& i_physValDY);
    void setLength(double i_fLength);
    void setLength(const ZS::PhysVal::CPhysVal& i_physValLength);
    void setAngle(const ZS::PhysVal::CPhysVal& i_physValAngle, const ZS::System::CEnumYScaleAxisOrientation& i_yScaleAxisOrientation);
    void setCenter(const QPointF& i_pCenter);
    void setCenter(const CPhysValPoint& i_physValPointCenter);
    void setResolution(double i_fRes);
    void setUnit(const ZS::PhysVal::CUnit& i_unit);
public: // instance methods (to convert the values into another unit)
    QLineF toQLineF() const;
    QString toString(bool i_bAddUnit = false, const QString& i_strSeparator = ", ") const;
protected: // instance members
    /*!< Values of the line stored in "m_unit". */
    QLineF m_line;
    /*!< Resolution of line coordinates.
         Defines the number of decimals when converting the coordinates into a string. */
    double m_fRes;
    /*!< Unit the values are store in "m_line". */
    ZS::PhysVal::CUnit m_unit;

}; // class CPhysValLine

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_PhysValLine_h
