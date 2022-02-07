/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSPhysUnitsDataQuantity_h
#define ZSPhysUnitsDataQuantity_h

#include "ZSPhysVal/ZSPhysUnits.h"
#include "ZSPhysVal/ZSPhysUnitsGrp.h"

namespace ZS
{
namespace PhysVal
{
//******************************************************************************
class ZSPHYSVALDLL_API CUnitDataQuantity : public CUnit
//******************************************************************************
{
public: // ctors and dtor
    CUnitDataQuantity(
        CUnitGrp*      i_pUnitGrp,
        const QString& i_strName,
        const QString& i_strSymbol,
        double         i_fFactor );
    virtual ~CUnitDataQuantity();
public: // instance methods
    double getFactor() const;
public: // overridables of base class CUnit (converting values)
    virtual bool isConvertible( const CUnit* i_pUnitDst, double i_fVal = 1.0 ) const;
    virtual double convertValue( double i_fVal, const CUnit* i_pUnitDst ) const;
protected: // instance members
    double m_fFactor;

}; // class CUnitDataQuantity

//******************************************************************************
class ZSPHYSVALDLL_API CUnitGrpDataQuantity : public CUnitGrp
//******************************************************************************
{
public: // ctors and dtor
    CUnitGrpDataQuantity( CUnitsPool* i_pUnitsPool );
    virtual ~CUnitGrpDataQuantity();
public: // instance methods to access the ratio units
    CUnitDataQuantity* Byte();
    CUnitDataQuantity* KiloByte();
    CUnitDataQuantity* MegaByte();
    CUnitDataQuantity* GigaByte();
private: // copy ctor not allowed
    CUnitGrpDataQuantity( const CUnitGrpDataQuantity& );
private: // assignment operator not allowed
    void operator = ( const CUnitGrpDataQuantity& );
protected: // instance members
    CUnitDataQuantity  m_physUnitByte;
    CUnitDataQuantity  m_physUnitKiloByte;
    CUnitDataQuantity  m_physUnitMegaByte;
    CUnitDataQuantity  m_physUnitGigaByte;

}; // class CUnitGrpDataQuantity

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysUnitsDataQuantity_h
