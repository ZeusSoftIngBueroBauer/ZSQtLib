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
#include "QtCore/qrect.h"

namespace ZS
{
namespace Draw
{
//******************************************************************************
/*! @brief Corresponds to QRectF but providing a physical unit and a resolution.

    This class is not part of ZS::PhysVal as the namespace ZS::PhysVal is abstract
    concerning units and the units "pixels" or any metric units is not known within
    the ZS::PhysVal library. The corresponding unit tree must be configured for the
    specific application or, as in this case, in the units tree of the ZS::Draw library.

    In addition to convert between metric and pixel dimensions (and vice versa)
    the screen resolution and the scale factor must be known. Both are properties
    of the physical size "Length" within the ZS::Draw library.
*/
class ZSDRAWDLL_API CPhysValRect
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CPhysValRect"; }
public: // ctors and dtor
    CPhysValRect();
    CPhysValRect(const ZS::PhysVal::CUnit& i_unit);
    CPhysValRect(const QRectF& i_rect, const ZS::PhysVal::CUnit& i_unit);
    CPhysValRect(const CPhysValRect& i_physValRectOther);
    virtual ~CPhysValRect();
public: // operators
    CPhysValRect& operator = (const CPhysValRect& i_physValRectOther);
    CPhysValRect& operator = (const QRectF& i_rect);
public: // operators
    bool operator == (const CPhysValRect& i_physValRectOther) const;
    bool operator != (const CPhysValRect& i_physValRectOther) const;
public: // instance methods
    ZS::PhysVal::CUnit unit() const;
    ZS::PhysVal::CPhysVal top() const;
    ZS::PhysVal::CPhysVal bottom() const;
    ZS::PhysVal::CPhysVal left() const;
    ZS::PhysVal::CPhysVal right() const;
    ZS::PhysVal::CPhysVal width() const;
    ZS::PhysVal::CPhysVal height() const;
public: // instance methods (to convert the values into another unit)
    QRectF toQRectF() const;
    QString toString() const;
protected: // instance members
    ZS::PhysVal::CUnit m_unit;
    QRectF m_rect;

}; // class CPhysValRect

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_PhysValRect_h
