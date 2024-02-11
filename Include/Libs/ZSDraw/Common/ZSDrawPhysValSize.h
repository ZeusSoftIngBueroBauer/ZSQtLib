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

#ifndef ZSDraw_PhysValSize_h
#define ZSDraw_PhysValSize_h

#include "ZSDrawDllMain.h"
#include "ZSPhysVal/ZSPhysVal.h"
#include "QtCore/qsize.h"

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
/*! @brief Corresponds to QSizeF but providing a physical unit and a resolution.

    This class is not part of ZS::PhysVal as the namespace ZS::PhysVal is abstract
    concerning units and the units "pixels" or any metric units is not known within
    the ZS::PhysVal library. The corresponding unit tree must be configured for the
    specific application or, as in this case, in the units tree of the ZS::Draw library.

    In addition to convert between metric and pixel dimensions (and vice versa)
    the screen resolution and the scale factor must be known. Both are properties
    of the physical size "Length" within the ZS::Draw library.
*/
class ZSDRAWDLL_API CPhysValSize
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CPhysValSize"; }
public: // ctors and dtor
    CPhysValSize();
    CPhysValSize(const CDrawingScene& i_drawingScene);
    CPhysValSize(const ZS::PhysVal::CUnit& i_unit, double i_fRes);
    CPhysValSize(double i_fWidth, double i_fHeight, double i_fRes, const ZS::PhysVal::CUnit& i_unit);
    CPhysValSize(const QSizeF& i_size, double i_fRes, const ZS::PhysVal::CUnit& i_unit);
    CPhysValSize(const ZS::PhysVal::CPhysVal& i_physValWidth, const ZS::PhysVal::CPhysVal& i_physValHeight);
    CPhysValSize(const CPhysValSize& i_physValSizeOther);
    virtual ~CPhysValSize();
public: // operators
    CPhysValSize& operator = (const CPhysValSize& i_physValSizeOther);
    CPhysValSize& operator = (const QSizeF& i_size);
public: // operators
    bool operator == (const CPhysValSize& i_physValSizeOther) const;
    bool operator != (const CPhysValSize& i_physValSizeOther) const;
public: // instance methods
    ZS::PhysVal::CPhysVal width() const;
    ZS::PhysVal::CPhysVal height() const;
    double resolution() const;
    ZS::PhysVal::CUnit unit() const;
    bool isValid() const;
public: // instance methods
    void setWidth(const ZS::PhysVal::CPhysVal& i_physValWidth);
    void setHeight(const ZS::PhysVal::CPhysVal& i_physValHeight);
    void setResolution(double i_fRes);
    void setUnit(const ZS::PhysVal::CUnit& unit);
public: // instance methods (to convert the values into another unit)
    QSizeF toQSizeF() const;
    QString toString(bool i_bAddUnit = false, const QString& i_strSeparator = ",") const;
protected: // instance members
    /*!< The size in width and height in the unit 'm_unit'. */
    QSizeF m_size;
    /*!< Resolution of both the width and height value.
         Defines the number of decimals when converting the coordinates into a string. */
    double m_fRes;
    /*!< Unit (either metric or pixels) in which the size values are in 'm_size'. */
    ZS::PhysVal::CUnit m_unit;

}; // class CPhysValSize

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_PhysValSize_h
