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

#include "ZSDrawDllMain.h"
#include "ZSPhysVal/ZSPhysVal.h"
#include "QtCore/qpoint.h"

namespace ZS
{
namespace PhysVal
{
class CPhysVal;
}
namespace Draw
{
class CDrawingScene;

//******************************************************************************
/*! @brief Corresponds to QPointF but providing a physical unit and a resolution.

    This class is not part of ZS::PhysVal as the namespace ZS::PhysVal is abstract
    concerning units and the units "pixels" or any metric units is not known within
    the ZS::PhysVal library. The corresponding unit tree must be configured for the
    specific application or, as in this case, in the units tree of the ZS::Draw library.

    In addition to convert between metric and pixel dimensions (and vice versa)
    the screen resolution and the scale factor must be known. Both are properties
    of the physical size "Length" within the ZS::Draw library.
*/
class ZSDRAWDLL_API CPhysValPoint
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CPhysValPoint"; }
public: // ctors and dtor
    CPhysValPoint();
    CPhysValPoint(const CDrawingScene& i_drawingScene);
    CPhysValPoint(const ZS::PhysVal::CUnit& i_unit, double i_fRes);
    CPhysValPoint(double i_fX, double i_fY, double i_fRes, const ZS::PhysVal::CUnit& i_unit);
    CPhysValPoint(const QPointF& i_pt, double i_fRes, const ZS::PhysVal::CUnit& i_unit);
    CPhysValPoint(const ZS::PhysVal::CPhysVal& i_physValX, const ZS::PhysVal::CPhysVal& i_physValY);
    CPhysValPoint(const CPhysValPoint& i_physValPointOther);
    ~CPhysValPoint();
public: // operators
    CPhysValPoint& operator = (const CPhysValPoint& i_physValPointOther);
    CPhysValPoint& operator = (const QPointF& i_ptOther);
    CPhysValPoint& operator = ( const QString& i_strValOther );
public: // operators
    bool operator == (const CPhysValPoint& i_physValPointOther) const;
    bool operator != (const CPhysValPoint& i_physValPointOther) const;
public: // instance methods
    ZS::PhysVal::CPhysVal x() const;
    ZS::PhysVal::CPhysVal y() const;
    double resolution() const;
    ZS::PhysVal::CUnit unit() const;
public: // instance methods
    void setX(const ZS::PhysVal::CPhysVal& i_physValX);
    void setY(const ZS::PhysVal::CPhysVal& i_physValY);
    void setResolution(double i_fRes);
    void setUnit(const ZS::PhysVal::CUnit& i_unit);
public: // instance methods (to convert the values into another unit)
    QPointF toQPointF() const;
    QString toString(bool i_bAddUnit = false, const QString& i_strSeparator = "/") const;
protected: // instance members
    /*!< The point coordinates stored in the unit 'm_unit'. */
    QPointF m_pt;
    /*!< Resolution of both the X and Y value.
         Defines the number of decimals when converting the coordinates into a string. */
    double m_fRes;
    /*!< Unit (either metric or pixels) in which the line coordinates are internally stored in 'm_pt'. */
    ZS::PhysVal::CUnit m_unit;

}; // class CPhysValPoint

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_PhysValPoint_h
