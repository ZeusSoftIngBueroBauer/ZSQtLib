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
//******************************************************************************
/*! @brief Corresponds to QPointF but providing a physical unit and a resolution.

    This class is not part of ZS::PhysVal as conversion into pixel values is
    necessary and for this the unit "Pixel" must be available.
    ZS::PhysVal is abstract concerning units and does not know about the unit "px".
*/
class ZSDRAWDLL_API CPhysValPoint
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CPhysValPoint"; }
public: // ctors and dtor
    CPhysValPoint( ZS::PhysVal::EResType i_resType = ZS::PhysVal::EResType::Resolution );
    CPhysValPoint( const ZS::PhysVal::CUnit& i_unit, double i_fRes = 0.0, ZS::PhysVal::EResType i_resType = ZS::PhysVal::EResType::Resolution );
    CPhysValPoint( const CPhysValPoint& i_physValPointOther );
    CPhysValPoint( const QPointF& i_pt );
    virtual ~CPhysValPoint();
public: // operators
    bool operator == ( const CPhysValPoint& i_physValPointOther ) const;
    bool operator != ( const CPhysValPoint& i_physValPointOther ) const;
public: // operators
    CPhysValPoint& operator = ( const CPhysValPoint& i_physValPointOther );
    CPhysValPoint& operator = ( const QPointF& i_pt );
public: // instance methods
    ZS::PhysVal::CUnit unit() const;
    ZS::PhysVal::CPhysVal x() const;
    ZS::PhysVal::CPhysVal y() const;
public: // instance methods
    void setX(const ZS::PhysVal::CPhysVal& i_physValX);
    void setY(const ZS::PhysVal::CPhysVal& i_physValY);
public: // instance methods (to convert the values into another unit)
    QPointF toQPointF() const;
    void convertValues(const ZS::PhysVal::CUnit& i_unitDst);
public: // instance methods
    QString toString() const;
protected: // instance members
    ZS::PhysVal::CUnit m_unit;
    ZS::PhysVal::CPhysValRes m_physValRes;
    QPointF m_pt;

}; // class CPhysValPoint

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_PhysValPoint_h
