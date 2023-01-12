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

#include "ZSPhysVal/ZSPhysUnitsRatio.h"
#include "ZSPhysVal/ZSPhysSizesIdxTree.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"
#include "ZSSys/ZSSysErrResult.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::PhysVal;


/*******************************************************************************
class CUnitRatioTreeEntry : public CUnitTreeEntry
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CUnitRatioTreeEntry::CUnitRatioTreeEntry(
    CUnitGrpTreeEntry* i_pUnitGrp,
    const QString&     i_strName,
    const QString&     i_strSymbol,
    double             i_fFactor ) :
//------------------------------------------------------------------------------
    CUnitTreeEntry(
        /* pUnitGrp       */ i_pUnitGrp,
        /* bIsLogarithmic */ false,
        /* fLogFactor     */ 1.0,
        /* strName        */ i_strName,
        /* strSymbol      */ i_strSymbol ),
    m_fFactor(i_fFactor)
{
} // ctor

//------------------------------------------------------------------------------
CUnitRatioTreeEntry::~CUnitRatioTreeEntry()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
double CUnitRatioTreeEntry::factor() const
//------------------------------------------------------------------------------
{
    return m_fFactor;
}

/*==============================================================================
public: // overridables (converting values)
==============================================================================*/

//------------------------------------------------------------------------------
bool CUnitRatioTreeEntry::isConvertible( const CUnitTreeEntry* i_pUnitDst, double /*i_fVal*/ ) const
//------------------------------------------------------------------------------
{
    if( i_pUnitDst->classType() != EUnitClassType::Ratios )
    {
        return false;
    }

    const CUnitRatioTreeEntry* pUnitRatioDst = dynamic_cast<const CUnitRatioTreeEntry*>(i_pUnitDst);

    if( pUnitRatioDst->m_fFactor == 0.0 )
    {
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
double CUnitRatioTreeEntry::convertValue( double i_fVal, const CUnitTreeEntry* i_pUnitDst ) const
//------------------------------------------------------------------------------
{
    if( i_pUnitDst->classType() != EUnitClassType::Ratios )
    {
        QString strAddErrInfo = "Src:" + unitGroup()->name() + ", Dst:" + i_pUnitDst->unitGroup()->name();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    const CUnitRatioTreeEntry* pUnitRatioDst = dynamic_cast<const CUnitRatioTreeEntry*>(i_pUnitDst);

    if( pUnitRatioDst->m_fFactor == 0.0 )
    {
        throw CUnitConversionException( __FILE__, __LINE__, EResultDivisionByZero, "Division By Zero" );
    }

    double fVal = i_fVal;
    double fFactor = m_fFactor / pUnitRatioDst->m_fFactor;

    fVal *= fFactor;

    return fVal;

} // convertValue


/*******************************************************************************
class CUnitRatio : public CUnit
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CUnitRatio::CUnitRatio() :
//------------------------------------------------------------------------------
    CUnit()
{
}

//------------------------------------------------------------------------------
CUnitRatio::CUnitRatio(CUnitRatio* i_pUnitRatio) :
//------------------------------------------------------------------------------
    CUnit(i_pUnitRatio)
{
}

//------------------------------------------------------------------------------
CUnitRatio::CUnitRatio(const CUnitRatio* i_pUnitRatio) :
//------------------------------------------------------------------------------
    CUnit(i_pUnitRatio)
{
}

//------------------------------------------------------------------------------
CUnitRatio::CUnitRatio(CUnitRatio& i_unitRatio) :
//------------------------------------------------------------------------------
    CUnit(i_unitRatio)
{
}

//------------------------------------------------------------------------------
CUnitRatio::CUnitRatio(const CUnitRatio& i_unitRatio) :
//------------------------------------------------------------------------------
    CUnit(i_unitRatio)
{
}

//------------------------------------------------------------------------------
CUnitRatio::CUnitRatio(CUnitRatioTreeEntry* i_pUnitRatio) :
//------------------------------------------------------------------------------
    CUnit(i_pUnitRatio)
{
}

//------------------------------------------------------------------------------
CUnitRatio::CUnitRatio(const CUnitRatioTreeEntry* i_pUnitRatio) :
//------------------------------------------------------------------------------
    CUnit(i_pUnitRatio)
{
}

//------------------------------------------------------------------------------
CUnitRatio::CUnitRatio(CUnitRatioTreeEntry& i_unitRatio) :
//------------------------------------------------------------------------------
    CUnit(&i_unitRatio)
{
}

//------------------------------------------------------------------------------
CUnitRatio::CUnitRatio(const CUnitRatioTreeEntry& i_unitRatio) :
//------------------------------------------------------------------------------
    CUnit(&i_unitRatio)
{
}

//------------------------------------------------------------------------------
CUnitRatio::CUnitRatio(const QString& i_strUniqueName) :
//------------------------------------------------------------------------------
    CUnit(i_strUniqueName)
{
}

//------------------------------------------------------------------------------
CUnitRatio::~CUnitRatio()
//------------------------------------------------------------------------------
{
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CUnitRatio::operator == ( const CUnitRatio& i_other ) const
//------------------------------------------------------------------------------
{
    bool bEqual = (m_pTreeEntry == i_other.m_pTreeEntry);
    if( bEqual ) bEqual = (m_strUniqueName.compare(i_other.m_strUniqueName) == 0);
    return bEqual;
}

//------------------------------------------------------------------------------
bool CUnitRatio::operator != ( const CUnitRatio& i_other ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
double CUnitRatio::factor() const
//------------------------------------------------------------------------------
{
    CUnitRatioTreeEntry* pUnitRatio = dynamic_cast<CUnitRatioTreeEntry*>(m_pTreeEntry);
    return pUnitRatio == nullptr ? 0.0 : pUnitRatio->factor();
}


#if 0

/*******************************************************************************
class CUnitGrpRatio : public QObject
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CUnitGrpRatio::CUnitGrpRatio( CIdxTree* i_pIdxTree ) :
//------------------------------------------------------------------------------
    CUnitGrp(
        /* pIdxTree       */ i_pIdxTree,
        /* type           */ EUnitClassType::Ratios,
        /* strName        */ "Ratio",
        /* pUnitGrpParent */ nullptr ),
    m_unitRatioPerOne(
        /* pUnitGrp  */ this,
        /* strName   */ "Per One",
        /* strSymbol */ "",
        /* fFactor   */ 1.0 ),
    m_unitRatioPerCent(
        /* pUnitGrp  */ this,
        /* strName   */ "Per Cent",
        /* strSymbol */ "%",
        /* fFactor   */ c_fFactorCenti ),
    m_unitRatioPerMille(
        /* pUnitGrp  */ this,
        /* strName   */ "Per Mille",
        /* strSymbol */ "%%",
        /* fFactor   */ c_fFactorMilli ),
    m_unitRatioDezibel(
        /* pUnitGrp  */ this,
        /* strName   */ "Dezibel",
        /* strSymbol */ "dB",
        /* fFactor   */ c_fFactor1 )
{
} // ctor

//------------------------------------------------------------------------------
CUnitGrpRatio::~CUnitGrpRatio()
//------------------------------------------------------------------------------
{
} // dtor

#endif
