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

#include "ZSPhysVal/ZSPhysTreeEntryUnitBase.h"
#include "ZSPhysVal/ZSPhysTreeEntryGrpBase.h"
#include "ZSSys/ZSSysIdxTree.h"
#include "ZSSys/ZSSysErrResult.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::PhysVal;


/*******************************************************************************
class CUnitsTreeEntryUnitBase
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CUnitsTreeEntryUnitBase::CUnitsTreeEntryUnitBase(
    CUnitsTreeEntryGrpBase* i_pParentBranch,
    bool i_bIsLogarithmic,
    double i_fLogFactor,
    const QString& i_strName,
    const QString& i_strSymbol ) :
//------------------------------------------------------------------------------
    CIdxTreeEntry(EIdxTreeEntryType::Leave, i_strName),
    m_classType(i_pParentBranch->classType()),
    m_strSymbol(i_strSymbol),
    m_bIsLogarithmic(i_bIsLogarithmic),
    m_fLogFactor(i_fLogFactor),
    m_pNextLower(nullptr),
    m_pNextHigher(nullptr)
{
    if( m_strName.isEmpty() )
    {
        m_strName = m_strSymbol;
    }

    if( i_pParentBranch != nullptr ) {
        CIdxTree* pIdxTree = i_pParentBranch->tree();
        if( pIdxTree != nullptr ) {
            pIdxTree->add(this, i_pParentBranch);
        }
    }

} // ctor

//------------------------------------------------------------------------------
CUnitsTreeEntryUnitBase::~CUnitsTreeEntryUnitBase()
//------------------------------------------------------------------------------
{
    // Remove this entry from the chain of next lower and next higher units.
    if( m_pNextLower != nullptr && m_pNextLower->m_pNextHigher == this )
    {
        m_pNextLower->m_pNextHigher = m_pNextHigher;
    }
    if( m_pNextHigher != nullptr && m_pNextHigher->m_pNextLower == this )
    {
        m_pNextHigher->m_pNextLower = m_pNextLower;
    }

    m_classType = static_cast<EUnitClassType>(0);
    //m_strSymbol;
    m_bIsLogarithmic = false;
    m_fLogFactor = 0.0;
    m_pNextLower = nullptr;
    m_pNextHigher = nullptr;

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CUnitsTreeEntryUnitBase::operator == ( const CUnitsTreeEntryUnitBase& i_other ) const
//------------------------------------------------------------------------------
{
    if( this != &i_other )
    {
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
bool CUnitsTreeEntryUnitBase::operator != ( const CUnitsTreeEntryUnitBase& i_other ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/*==============================================================================
public: // instance methods (configuration)
==============================================================================*/

//------------------------------------------------------------------------------
QString CUnitsTreeEntryUnitBase::classType2Str() const
//------------------------------------------------------------------------------
{
    return CEnumUnitClassType(m_classType).toString();
}

//------------------------------------------------------------------------------
bool CUnitsTreeEntryUnitBase::isRatio() const
//------------------------------------------------------------------------------
{
    return m_classType == EUnitClassType::Ratio;
}

//------------------------------------------------------------------------------
bool CUnitsTreeEntryUnitBase::isQuantity() const
//------------------------------------------------------------------------------
{
    return m_classType == EUnitClassType::Quantity;
}

//------------------------------------------------------------------------------
bool CUnitsTreeEntryUnitBase::isRatioOrQuantity() const
//------------------------------------------------------------------------------
{
    return (isRatio() || isQuantity());
}

//------------------------------------------------------------------------------
bool CUnitsTreeEntryUnitBase::isPhysSize() const
//------------------------------------------------------------------------------
{
    return m_classType == EUnitClassType::PhysSize;
}

//------------------------------------------------------------------------------
CUnitsTreeEntryGrpBase* CUnitsTreeEntryUnitBase::unitGroup() const
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitsTreeEntryGrpBase*>(m_pParentBranch);
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CUnitsTreeEntryUnitBase::setNextLowerHigherUnits( CUnitsTreeEntryUnitBase* i_pNextLower, CUnitsTreeEntryUnitBase* i_pNextHigher )
//------------------------------------------------------------------------------
{
    m_pNextLower = i_pNextLower;
    m_pNextHigher = i_pNextHigher;
}

//------------------------------------------------------------------------------
CUnitsTreeEntryUnitBase* CUnitsTreeEntryUnitBase::findBestUnit(
    double  i_fVal,
    double* o_pfVal,
    int     i_iDigitsLeadingMax ) const
//------------------------------------------------------------------------------
{
    double       fValAbs       = fabs(i_fVal);
    double       fValAbsPrev   = fValAbs;
    double       fValSign      = i_fVal >= 0.0 ? 1.0 : -1.0;
    const CUnitsTreeEntryUnitBase* pUnitBest = this;
    const CUnitsTreeEntryUnitBase* pUnitPrev = this;
    const CUnitsTreeEntryUnitBase* pUnitNext = nullptr;
    int          iNextUnitDir; // 0 = AbortSearch, -1 = LowerUnit, +1 = HigherUnit
    int          iDigitsLeadingMax = i_iDigitsLeadingMax;
    int          iDigitsLeadingPrev;
    int          iDigitsLeading;
    int          iDigitsTrailing;
    double       fDigitsTrailing;
    double       fFract;
    double       fInt;

    if( iDigitsLeadingMax < 1 || iDigitsLeadingMax > 3 )
    {
        iDigitsLeadingMax = 3;
    }

    try
    {
        if( fValAbs >= 1.0 )
        {
            iDigitsLeading  = static_cast<int>(log10(fValAbs))+1;
            iDigitsTrailing = 0;
        }
        else if( fValAbs < 1.0 && fValAbs > 0.0 )
        {
            iDigitsLeading  = 0;
            fDigitsTrailing = log10(fValAbs);
            iDigitsTrailing = -static_cast<int>(fDigitsTrailing);

            fFract = modf(fDigitsTrailing,&fInt);
            if( fFract != 0.0 )
            {
                iDigitsTrailing += 1;
            }
        }
        else // if( fValAbs == 0.0 )
        {
            iDigitsLeading  = 1;
            iDigitsTrailing = 0;
        }
        iDigitsLeadingPrev = iDigitsLeading;

        if( iDigitsLeading > iDigitsLeadingMax )
        {
            pUnitNext = pUnitPrev->nextHigherUnit();
            iNextUnitDir = 1;
        }
        else if( iDigitsTrailing > 0 )
        {
            pUnitNext = pUnitPrev->nextLowerUnit();
            iNextUnitDir = -1;
        }
        else // if( iDigitsLeading <= iDigitsLeadingMax && iDigitsTrailing <= 0 )
        {
            pUnitNext = nullptr;
            iNextUnitDir = 0;
        }

        while( pUnitNext != nullptr )
        {
            fValAbs = pUnitPrev->convertValue( fValAbsPrev, pUnitNext );

            if( fValAbs >= 1.0 )
            {
                iDigitsLeading  = static_cast<int>(log10(fValAbs))+1;
                iDigitsTrailing = 0;
            }
            else if( fValAbs < 1.0 && fValAbs > 0.0 )
            {
                iDigitsLeading  = 0;
                fDigitsTrailing = log10(fValAbs);
                iDigitsTrailing = -static_cast<int>(fDigitsTrailing);

                fFract = modf(fDigitsTrailing,&fInt);
                if( fFract != 0.0 )
                {
                    iDigitsTrailing += 1;
                }
            }
            if( iDigitsLeading > iDigitsLeadingMax && iNextUnitDir > 0 )
            {
                pUnitPrev = pUnitNext;
                pUnitNext = pUnitPrev->nextHigherUnit();
                if( pUnitNext == nullptr )
                {
                    // There is no next higher unit. Previous unit is best.
                    pUnitBest = pUnitPrev;
                }
            }
            else if( iDigitsTrailing > 0 && iNextUnitDir < 0 )
            {
                pUnitPrev = pUnitNext;
                pUnitNext = pUnitPrev->nextLowerUnit();
                if( pUnitNext == nullptr )
                {
                    // There is no next lower unit. Previous unit is best.
                    pUnitBest = pUnitPrev;
                }
            }
            else if( iDigitsLeading <= iDigitsLeadingMax && iDigitsTrailing <= 0 )
            {
                // Value can be formatted with minimum one leading and maximum
                // accuracy leading digits. Best unit has been found.
                pUnitBest = pUnitNext;
                pUnitNext = nullptr;
            }
            else // if( UnitDirChanged )
            {
                // Skipped from leading into trailing digits or vice versa.
                // Previous unit is the best. But not if the number of leading
                // digits would exceed the number of accuracy leading digits.
                if( iDigitsLeadingPrev < iDigitsLeadingMax )
                {
                    pUnitBest = pUnitPrev;
                    fValAbs = fValAbsPrev;
                }
                else
                {
                    pUnitBest = pUnitNext;
                }
                pUnitNext = nullptr;
            }
            fValAbsPrev = fValAbs;
            iDigitsLeadingPrev = iDigitsLeading;

        } // while( iNextUnitDir != 0 )
    }
    catch( CException* )
    {
        pUnitBest = this;
        fValAbs  = fabs(i_fVal);
    }
    if( o_pfVal != nullptr )
    {
        *o_pfVal = fValSign*fValAbs;
    }
    return const_cast<CUnitsTreeEntryUnitBase*>(pUnitBest);

} // findBestUnit
