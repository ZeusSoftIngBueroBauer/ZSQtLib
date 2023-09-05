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

    This class is not part of ZS::PhysVal as conversion into pixel values is
    necessary and for this the unit "Pixel" must be available.
    ZS::PhysVal is abstract concerning units and does not know about the unit "px".
*/
class ZSDRAWDLL_API CPhysValLine
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CPhysValLine"; }
public: // ctors and dtor
    CPhysValLine( ZS::PhysVal::EResType i_resType = ZS::PhysVal::EResType::Resolution );
    CPhysValLine( const ZS::PhysVal::CUnit& i_unit, double i_fRes = 0.0, ZS::PhysVal::EResType i_resType = ZS::PhysVal::EResType::Resolution );
    CPhysValLine( const CPhysValPoint& i_physValPoint1, const CPhysValPoint& i_physValPoint2 );
    CPhysValLine( const ZS::PhysVal::CPhysVal& i_physValX1, const ZS::PhysVal::CPhysVal& i_physValY1,
                  const ZS::PhysVal::CPhysVal& i_physValX2, const ZS::PhysVal::CPhysVal& i_physValY2 );
    CPhysValLine( const CPhysValLine& i_physValLineOther );
    CPhysValLine( const QLineF& i_line );
    virtual ~CPhysValLine();
public: // operators
    bool operator == ( const CPhysValLine& i_physValLineOther ) const;
    bool operator != ( const CPhysValLine& i_physValLineOther ) const;
public: // operators
    CPhysValLine& operator = ( const CPhysValLine& i_physValLineOther );
    CPhysValLine& operator = ( const QLineF& i_line );
public: // instance methods
    ZS::PhysVal::CUnit unit() const;
    CPhysValPoint p1() const;
    CPhysValPoint p2() const;
    ZS::PhysVal::CPhysVal x1() const;
    ZS::PhysVal::CPhysVal x2() const;
    ZS::PhysVal::CPhysVal y1() const;
    ZS::PhysVal::CPhysVal y2() const;
    CPhysValPoint center() const;
public: // instance methods (to convert the values into another unit)
    QLineF toQLineF() const;
    void convertValues( const ZS::PhysVal::CUnit& i_unitDst );
public: // instance methods
    QString toString() const;
protected: // instance members
    ZS::PhysVal::CUnit m_unit;
    ZS::PhysVal::CPhysValRes m_physValRes;
    QLineF m_line;

}; // class CPhysValLine

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_PhysValLine_h
