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

#include "ZSPhysVal/ZSPhysTreeEntryUnitQuantity.h"
#include "ZSPhysVal/ZSPhysTreeEntryGrpBase.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"
#include "ZSSys/ZSSysErrResult.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::PhysVal;


/*******************************************************************************
class CUnitsTreeEntryUnitQuantity : public CUnitsTreeEntryUnitBase
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CUnitsTreeEntryUnitQuantity::CUnitsTreeEntryUnitQuantity(
    CUnitsTreeEntryGrpBase* i_pUnitGrp,
    const QString& i_strName,
    const QString& i_strSymbol,
    double         i_fFactor ) :
//------------------------------------------------------------------------------
    CUnitsTreeEntryUnitBase(
        /* pUnitGrp       */ i_pUnitGrp,
        /* bIsLogarithmic */ false,
        /* fLogFactor     */ 1.0,
        /* strName        */ i_strName,
        /* strSymbol      */ i_strSymbol ),
    m_fFactor(i_fFactor)
{
} // ctor

//------------------------------------------------------------------------------
CUnitsTreeEntryUnitQuantity::~CUnitsTreeEntryUnitQuantity()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
double CUnitsTreeEntryUnitQuantity::factor() const
//------------------------------------------------------------------------------
{
    return m_fFactor;
}

/*==============================================================================
public: // overridables (converting values)
==============================================================================*/

//------------------------------------------------------------------------------
bool CUnitsTreeEntryUnitQuantity::isConvertible( const CUnitsTreeEntryUnitBase* i_pUnitDst, double /*i_fVal*/ ) const
//------------------------------------------------------------------------------
{
    if( i_pUnitDst->classType() != EUnitClassType::Quantity )
    {
        return false;
    }

    const CUnitsTreeEntryUnitQuantity* pUnitDst = dynamic_cast<const CUnitsTreeEntryUnitQuantity*>(i_pUnitDst);

    if( pUnitDst->m_fFactor == 0.0 )
    {
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
double CUnitsTreeEntryUnitQuantity::convertValue( double i_fVal, const CUnitsTreeEntryUnitBase* i_pUnitDst ) const
//------------------------------------------------------------------------------
{
    if( i_pUnitDst->classType() != EUnitClassType::Quantity )
    {
        throw CException(__FILE__,__LINE__,EResultDifferentPhysSizes);
    }

    const CUnitsTreeEntryUnitQuantity* pUnitDst = dynamic_cast<const CUnitsTreeEntryUnitQuantity*>(i_pUnitDst);

    if( pUnitDst->m_fFactor == 0.0 )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultDivisionByZero);
    }

    double fVal = i_fVal;
    double fFactor = m_fFactor / pUnitDst->m_fFactor;

    fVal *= fFactor;

    return fVal;

} // convertValue
