/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#include "ZSPhysVal/ZSPhysUnitsDataQuantity.h"
#include "ZSPhysVal/ZSPhysUnitsPool.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"
#include "ZSSys/ZSSysErrResult.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::PhysVal;


/*******************************************************************************
class CUnitDataQuantity : public CUnit
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CUnitDataQuantity::CUnitDataQuantity(
    CUnitGrp*      i_pUnitGrp,
    const QString& i_strName,
    const QString& i_strSymbol,
    double         i_fFactor ) :
//------------------------------------------------------------------------------
    CUnit(
        /* pUnitGrp       */ i_pUnitGrp,
        /* bIsLogarithmic */ false,
        /* fLogFactor     */ 1.0,
        /* strName        */ i_strName,
        /* strSymbol      */ i_strSymbol,
        /* strKey         */ i_pUnitGrp->getKey() + CUnitsPool::GetNameSeparator() + i_strSymbol ),
    m_fFactor(i_fFactor)
{
} // ctor

//------------------------------------------------------------------------------
CUnitDataQuantity::~CUnitDataQuantity()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
double CUnitDataQuantity::getFactor() const
//------------------------------------------------------------------------------
{
    return m_fFactor;
}

/*==============================================================================
public: // overridables (converting values)
==============================================================================*/

//------------------------------------------------------------------------------
bool CUnitDataQuantity::isConvertible( const CUnit* i_pUnitDst, double /*i_fVal*/ ) const
//------------------------------------------------------------------------------
{
    if( i_pUnitDst->classType() != EUnitClassTypeDataQuantity )
    {
        return false;
    }

    const CUnitDataQuantity* pUnitDst = dynamic_cast<const CUnitDataQuantity*>(i_pUnitDst);

    if( pUnitDst->m_fFactor == 0.0 )
    {
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
double CUnitDataQuantity::convertValue( double i_fVal, const CUnit* i_pUnitDst ) const
//------------------------------------------------------------------------------
{
    if( i_pUnitDst->classType() != EUnitClassTypeDataQuantity )
    {
        throw CException(__FILE__,__LINE__,EResultDifferentPhysSizes);
    }

    const CUnitDataQuantity* pUnitDst = dynamic_cast<const CUnitDataQuantity*>(i_pUnitDst);

    if( pUnitDst->m_fFactor == 0.0 )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultDivisionByZero);
    }

    double fVal = i_fVal;
    double fFactor = m_fFactor / pUnitDst->m_fFactor;

    fVal *= fFactor;

    return fVal;

} // convertValue


/*******************************************************************************
class CUnitGrpDataQuantity : public QObject
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CUnitGrpDataQuantity::CUnitGrpDataQuantity() :
//------------------------------------------------------------------------------
    CUnitGrp(
        /* type             */ EUnitClassTypeDataQuantity,
        /* strName          */ "DataQuantity",
        /* strKey           */ "DataQuantity",
        /* bIsNameSpaceNode */ false,
        /* pUnitGrpParent   */ CUnitsPool::GetUnitClassTypeGroup(EUnitClassTypeDataQuantity) ),
    m_physUnitByte(
        /* pUnitGrp  */ this,
        /* strName   */ "Byte",
        /* strSymbol */ "B",
        /* fFactor   */ 1.0 ),
    m_physUnitKiloByte(
        /* pUnitGrp  */ this,
        /* strName   */ "Kilo Byte",
        /* strSymbol */ "kB",
        /* fFactor   */ 1024.0 ),
    m_physUnitMegaByte(
        /* pUnitGrp  */ this,
        /* strName   */ "Mega Byte",
        /* strSymbol */ "MB",
        /* fFactor   */ 1024.0*1024.0 ),
    m_physUnitGigaByte(
        /* pUnitGrp  */ this,
        /* strName   */ "Giga Byte",
        /* strSymbol */ "GB",
        /* fFactor   */ 1024.0*1024.0*1024.0 )
{
} // ctor

//------------------------------------------------------------------------------
CUnitGrpDataQuantity::~CUnitGrpDataQuantity()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods to access the ratio units
==============================================================================*/

//------------------------------------------------------------------------------
CUnitDataQuantity* CUnitGrpDataQuantity::Byte()
//------------------------------------------------------------------------------
{
    return &m_physUnitByte;
}

//------------------------------------------------------------------------------
CUnitDataQuantity* CUnitGrpDataQuantity::KiloByte()
//------------------------------------------------------------------------------
{
    return &m_physUnitKiloByte;
}

//------------------------------------------------------------------------------
CUnitDataQuantity* CUnitGrpDataQuantity::MegaByte()
//------------------------------------------------------------------------------
{
    return &m_physUnitMegaByte;
}

//------------------------------------------------------------------------------
CUnitDataQuantity* CUnitGrpDataQuantity::GigaByte()
//------------------------------------------------------------------------------
{
    return &m_physUnitGigaByte;
}
