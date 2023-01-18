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

#ifndef ZSPhysVal_PhysTreeEntryUnitRatio_h
#define ZSPhysVal_PhysTreeEntryUnitRatio_h

#include "ZSPhysVal/ZSPhysTreeEntryUnitBase.h"

namespace ZS
{
namespace PhysVal
{
//******************************************************************************
class ZSPHYSVALDLL_API CUnitsTreeEntryUnitRatio : public CUnitsTreeEntryUnitBase
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CUnitsTreeEntryUnitRatio"; }
public: // ctors and dtor
    CUnitsTreeEntryUnitRatio(
        CUnitsTreeEntryGrpBase* i_pUnitGrp,
        const QString& i_strName,
        const QString& i_strSymbol,
        double         i_fFactor );
    virtual ~CUnitsTreeEntryUnitRatio();
public: // instance methods
    double factor() const;
public: // overridables of base class CUnitsTreeEntryUnitBase (converting values)
    virtual bool isConvertible( const CUnitsTreeEntryUnitBase* i_pUnitDst, double i_fVal = 1.0 ) const override;
    virtual double convertValue( double i_fVal, const CUnitsTreeEntryUnitBase* i_pUnitDst ) const override;
protected: // instance members
    double m_fFactor;

}; // class CUnitsTreeEntryUnitRatio

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysTreeEntryUnitRatio_h
