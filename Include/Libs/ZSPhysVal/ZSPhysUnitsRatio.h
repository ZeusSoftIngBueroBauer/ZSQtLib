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

#ifndef ZSPhysVal_PhysUnitsRatio_h
#define ZSPhysVal_PhysUnitsRatio_h

#include "ZSPhysVal/ZSPhysUnits.h"
#include "ZSPhysVal/ZSPhysUnitsGrp.h"

namespace ZS
{
namespace PhysVal
{
//******************************************************************************
class ZSPHYSVALDLL_API CUnitRatio : public CUnit
//******************************************************************************
{
public: // ctors and dtor
    CUnitRatio(
        CUnitGrp*      i_pUnitGrp,
        const QString& i_strName,
        const QString& i_strSymbol,
        double         i_fFactor );
    virtual ~CUnitRatio();
public: // instance methods
    double getFactor() const;
public: // overridables of base class CUnit (converting values)
    virtual bool isConvertible( const CUnit* i_pUnitDst, double i_fVal = 1.0 ) const;
    virtual double convertValue( double i_fVal, const CUnit* i_pUnitDst ) const;
protected: // instance members
    double m_fFactor;

}; // class CUnitRatio

//******************************************************************************
class ZSPHYSVALDLL_API CUnitGrpRatio : public CUnitGrp
//******************************************************************************
{
public: // ctors and dtor
    CUnitGrpRatio( CUnitsPool* i_pUnitsPool );
    virtual ~CUnitGrpRatio();
public: // instance methods to access the ratio units
    CUnitRatio* PerOne() { return &m_unitRatioPerOne; }
    CUnitRatio* PerCent() { return &m_unitRatioPerCent; }
    CUnitRatio* PerMille() { return &m_unitRatioPerMille; }
    CUnitRatio* dB() { return &m_unitRatioDezibel; }
    CUnitRatio* Dezibel() { return &m_unitRatioDezibel; }
public: // instance methods
    CUnitRatio* getUnit( int i_idx );
    CUnitRatio* findUnitBySymbol( const QString& i_strSymbol );
    CUnitRatio* findUnitByName( const QString& i_strName );
private: // copy ctor not allowed
    CUnitGrpRatio( const CUnitGrpRatio& );
private: // assignment operator not allowed
    CUnitGrpRatio& operator = ( const CUnitGrpRatio& );
protected: // instance members
    CUnitRatio  m_unitRatioPerOne;
    CUnitRatio  m_unitRatioPerCent;
    CUnitRatio  m_unitRatioPerMille;
    CUnitRatio  m_unitRatioDezibel;

}; // class CUnitGrpRatio

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysUnitsRatio_h
