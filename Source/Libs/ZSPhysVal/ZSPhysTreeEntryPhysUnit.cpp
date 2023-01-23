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

#include "ZSPhysVal/ZSPhysTreeEntryPhysUnit.h"
#include "ZSPhysVal/ZSPhysTreeEntryGrpPhysUnits.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"
#include "ZSSys/ZSSysIdxTree.h"
#include "ZSSys/ZSSysErrResult.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::PhysVal;


/*******************************************************************************
class CUnitsTreeEntryPhysUnit : public CUnitsTreeEntryUnitBase
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Constructor for linear units.

    @param i_pPhysSize [in]
    @param i_strPrefix [in]
        e.g. "" for SI-Base, "m", "k", "µ", "M", ...
*/
CUnitsTreeEntryPhysUnit::CUnitsTreeEntryPhysUnit(
    CUnitsTreeEntryGrpPhysUnits* i_pPhysSize, const QString& i_strFactorPrefix ) :
//------------------------------------------------------------------------------
    CUnitsTreeEntryUnitBase(
        /* pUnitGrp        */ i_pPhysSize,
        /* bIsLogarithmic  */ false,
        /* fLogFactor      */ 1.0,
        /* strName         */ getExponentStrFromPrefixStr(i_strFactorPrefix) + i_pPhysSize->getSIUnitName(),
        /* strSymbol       */ i_strFactorPrefix + i_pPhysSize->getSIUnitSymbol(),
        /* strFactorPrefix */ i_strFactorPrefix ),
    m_pPhysUnitSI(nullptr),
    m_bInitialized(false),
    //m_iPhysSizeRowIdx(-1),
    m_fctConvertFromSIUnit(),
    m_fctConvertIntoSIUnit(),
    m_arFctConvertsInternal(),
    m_arFctConvertsExternal()
{
    //--------------------------------------------------------------------------
    // This is the ctor for linear units
    //--------------------------------------------------------------------------

    // Please note: for "kilo" the factor is 10e3, for "milli" the factor is 10e-3.
    double fFactorIntoSIUnit = getFactorFromPrefixStr(i_strFactorPrefix);

    // We don't calculate the factor from SI unit by "1/FactorIntoSIUnit" as by
    // the floating point division the calculated factor may be inaccurate.
    // Better (more precise) is to get the inverted factor from the exponent table.
    double fFactorFromSIUnit = getFactorFromPrefixStr(i_strFactorPrefix,true);

    // Set function to convert values from the SI unit into this unit
    //--------------------------------------------------------------------------

    m_fctConvertFromSIUnit.m_pPhysUnitDst = this;
    m_fctConvertFromSIUnit.m_fctConvertType = EFctConvert_mMULxADDt;
    m_fctConvertFromSIUnit.m_physValM = fFactorFromSIUnit;

    // Set function to convert values from this unit into the SI unit
    //--------------------------------------------------------------------------

    // As mentioned a division is more inaccurate than multiplying
    // with the inverted factor.
    m_fctConvertIntoSIUnit.m_pPhysUnitSrc = this;
    m_fctConvertIntoSIUnit.m_fctConvertType = EFctConvert_mMULxADDt;
    m_fctConvertIntoSIUnit.m_physValM = fFactorIntoSIUnit;

} // ctor

//------------------------------------------------------------------------------
/*! @brief

    Depending on the input parameter "IsLogarithmic" this is the ctor for either
    - linear units whose factor is not one of the decade factors
    - logarithmic units

    @param i_pPhysSize [in]
    @param i_bIsLogarithmic [in]
        false for linear units (e.g. Degree), true for logarithmic units (e.g. dBm)
    @param i_strName [in]
        e.g. "Degree", "Inch", ... or "dBWatt", "dBMilliWatt", "dBu(0.775V)", ...
    @param i_strSymbol [in]
        e.g. "°", "in", ... or "dBW", "dBm", "dBu", ...
*/
CUnitsTreeEntryPhysUnit::CUnitsTreeEntryPhysUnit(
    CUnitsTreeEntryGrpPhysUnits* i_pPhysSize,
    bool           i_bIsLogarithmic,
    const QString& i_strName,
    const QString& i_strSymbol ) :
//------------------------------------------------------------------------------
    CUnitsTreeEntryUnitBase(
        /* pUnitGrp        */ i_pPhysSize,
        /* bIsLogarithmic  */ i_bIsLogarithmic,
        /* fLogFactor      */ 1.0,
        /* strName         */ i_strName,
        /* strSymbol       */ i_strSymbol,
        /* strFactorPrefix */ "" ),
    m_pPhysUnitSI(nullptr),
    m_bInitialized(false),
    //m_iPhysSizeRowIdx(-1),
    m_fctConvertFromSIUnit(),
    m_fctConvertIntoSIUnit(),
    m_arFctConvertsInternal(),
    m_arFctConvertsExternal()
{
    m_fctConvertFromSIUnit.m_pPhysUnitDst = this;
    m_fctConvertIntoSIUnit.m_pPhysUnitSrc = this;

} // ctor

//------------------------------------------------------------------------------
/*! @brief

    Depending on the input parameter "IsLogarithmic" this is the ctor for either
    - linear units whose factor is not one of the decade factors
    - logarithmic units

    @param i_pPhysSize [in]
    @param i_bIsLogarithmic [in]
        false for linear units (e.g. Degree), true for logarithmic units (e.g. dBm)
    @param i_strName [in]
        e.g. "Degree", "Inch", ... or "dBWatt", "dBMilliWatt", "dBu(0.775V)", ...
    @param i_strSymbol [in]
        e.g. "°", "in", ... or "dBW", "dBm", "dBu", ...
    @param i_fMFromSI [in]
        e.g. "180/PI", 0.0254, ... or 1.0 for dBW, 1.0e-3 for dBm, 0.775 for 0.775 V, ...
*/
CUnitsTreeEntryPhysUnit::CUnitsTreeEntryPhysUnit(
    CUnitsTreeEntryGrpPhysUnits* i_pPhysSize,
    bool           i_bIsLogarithmic,
    const QString& i_strName,
    const QString& i_strSymbol,
    const double   i_fMFromBaseOrRefVal ) :
//------------------------------------------------------------------------------
    CUnitsTreeEntryUnitBase(
        /* pUnitGrp        */ i_pPhysSize,
        /* bIsLogarithmic  */ i_bIsLogarithmic,
        /* fLogFactor      */ 1.0,
        /* strName         */ i_strName,
        /* strSymbol       */ i_strSymbol,
        /* strFactorPrefix */ "" ),
    m_pPhysUnitSI(nullptr),
    m_bInitialized(false),
    //m_iPhysSizeRowIdx(-1),
    m_fctConvertFromSIUnit(),
    m_fctConvertIntoSIUnit(),
    m_arFctConvertsInternal(),
    m_arFctConvertsExternal()
{
    // Set function to convert values from the SI unit into this unit
    //--------------------------------------------------------------------------

    m_fctConvertFromSIUnit.m_pPhysUnitDst = this;

    if( i_bIsLogarithmic ) {
        // y = m*log10(x) + t (with t = SSI+SRel)
        // (for the convert into SI unit function SSI is equal to 0)
        m_fctConvertFromSIUnit.m_fctConvertType = EFctConvert_mLOGxADDt;

        if( physSize()->isPowerRelated() ) {
            m_fctConvertFromSIUnit.m_physValM = 10.0;
            m_fLogFactor = 10.0;
        }
        else {
            m_fctConvertFromSIUnit.m_physValM = 20.0;
            m_fLogFactor = 20.0;
        }
        if( i_fMFromBaseOrRefVal != 0.0 ) {
            m_fctConvertFromSIUnit.m_physValT =
                m_fctConvertFromSIUnit.m_physValM.getVal() * log10(1.0/i_fMFromBaseOrRefVal);
        }
    }
    else {
        m_fctConvertFromSIUnit.m_fctConvertType = EFctConvert_mMULxADDt;
        m_fctConvertFromSIUnit.m_physValM = i_fMFromBaseOrRefVal;
    }

    // Set function to convert values from this unit into the SI unit
    //--------------------------------------------------------------------------

    m_fctConvertIntoSIUnit.m_pPhysUnitSrc = this;

    if( i_bIsLogarithmic ) {
        // y = 10exp((x+t)/m) (with t = -(SSI+SRel))
        // (for the convert from SI unit function SSI is equal to 0)
        m_fctConvertIntoSIUnit.m_fctConvertType = EFctConvert_EXP__xADDt_DIVm_;
        m_fctConvertIntoSIUnit.m_physValM = m_fctConvertFromSIUnit.m_physValM;
        m_fctConvertIntoSIUnit.m_physValT = -1.0 * m_fctConvertFromSIUnit.m_physValT.getVal();
    }
    else {
        m_fctConvertIntoSIUnit.m_fctConvertType = EFctConvert_mMULxADDt;
        if( i_fMFromBaseOrRefVal != 0.0 ) {
            m_fctConvertIntoSIUnit.m_physValM = 1.0 / i_fMFromBaseOrRefVal;
        }
    }

} // ctor

//------------------------------------------------------------------------------
CUnitsTreeEntryPhysUnit::~CUnitsTreeEntryPhysUnit()
//------------------------------------------------------------------------------
{
    m_pPhysUnitSI = nullptr;
    m_bInitialized = false;
    //m_fctConvertFromSIUnit;
    //m_fctConvertIntoSIUnit;
    m_arFctConvertsInternal.clear();
    m_arFctConvertsExternal.clear();

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CUnitsTreeEntryPhysUnit::operator == ( const CUnitsTreeEntryPhysUnit& i_physUnitOther ) const
//------------------------------------------------------------------------------
{
    if( this != &i_physUnitOther )
    {
        return false;
    }
    return true;

} // operator ==

//------------------------------------------------------------------------------
bool CUnitsTreeEntryPhysUnit::operator != ( const CUnitsTreeEntryPhysUnit& i_physUnitOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_physUnitOther);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CUnitsTreeEntryGrpPhysUnits* CUnitsTreeEntryPhysUnit::physSize() const
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitsTreeEntryGrpPhysUnits*>(m_pParentBranch);
};

/*==============================================================================
public: // overridables of base class CUnitsTreeEntryUnitBase (converting values)
==============================================================================*/

//------------------------------------------------------------------------------
bool CUnitsTreeEntryPhysUnit::isConvertible(
    const CUnitsTreeEntryUnitBase* i_pUnitDst, double /*i_fVal*/ ) const
//------------------------------------------------------------------------------
{
    if( !m_bInitialized )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultClassNotInitialised );
    }
    if( m_idxInParentBranch < 0 || m_idxInParentBranch >= m_arFctConvertsInternal.size() )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultPhysSizeNotInitialized );
    }
    if( m_pPhysUnitSI == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSIUnitNotDefined );
    }
    if( i_pUnitDst->classType() != EUnitClassType::PhysSize )
    {
        return false;
    }

    const CUnitsTreeEntryPhysUnit* pPhysUnitDst = dynamic_cast<const CUnitsTreeEntryPhysUnit*>(i_pUnitDst);

    if( !pPhysUnitDst->m_bInitialized )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultPhysSizeNotInitialized );
    }
    if( pPhysUnitDst->m_idxInParentBranch < 0 || pPhysUnitDst->m_idxInParentBranch >= m_arFctConvertsInternal.size() )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultPhysSizeNotInitialized );
    }
    if( pPhysUnitDst->m_pPhysUnitSI == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSIUnitNotDefined );
    }

    bool bConvertible = false;

    // If this and the destination unit are belonging to the same physical size ...
    if( physSize() == pPhysUnitDst->physSize() )
    {
        // .. the unit is convertible if it has been initialized by the physical size
        // creating the array with internal conversion routines and setting the
        // internal conversion routines (see exceptions above).
        bConvertible = true;
    }
    // If this and the destination unit are belonging to different physical sizes ...
    else
    {
        // Check if a direct conversion routine has been set.
        const SFctConvert* pFctConvert = findFctConvertExternal(pPhysUnitDst);

        // If a direct conversion routine has been set ..
        if( pFctConvert != nullptr )
        {
            bConvertible = true;
        }
        // If no direct conversion routine has been set ..
        else
        {
            // .. maybe a direct conversion between both SI units has been set.
            CUnitsTreeEntryPhysUnit* pPhysUnitSrcSI = physSize()->getSIUnit();
            CUnitsTreeEntryPhysUnit* pPhysUnitDstSI = pPhysUnitDst->getSIUnit();

            if( pPhysUnitSrcSI != nullptr && pPhysUnitDstSI != nullptr )
            {
                // Check if a direct conversion from this SI unit into the
                // SI unit of the destination unit has been set.
                pFctConvert = pPhysUnitSrcSI->findFctConvertExternal(pPhysUnitDstSI);

                // If a direct conversion routine between both SI units has been set.
                if( pFctConvert != nullptr )
                {
                    bConvertible = true;
                }
            }

            //------------------------------------------------------------------
            // Please note that a conversion between this and the destination unit
            // may still be possible if for any other unit of this phyisical size
            // a direct conversion routine into any unit of the destination physical
            // size has been set and if that unit is convertible into the destination
            // unit. But this feature is not yet supported as the checks above already
            // need a lot of time. And if the conversion is needed the conversion
            // routine should be implemented and added.
            //------------------------------------------------------------------
        }
    }
    return bConvertible;

} // isConvertible

//------------------------------------------------------------------------------
double CUnitsTreeEntryPhysUnit::convertValue(
    double i_fVal, const CUnitsTreeEntryUnitBase* i_pUnitDst ) const
//------------------------------------------------------------------------------
{
    if( i_pUnitDst->classType() != EUnitClassType::PhysSize ) {
        return CUnitsTreeEntryUnitBase::convertValue(i_fVal, i_pUnitDst);
    }

    const CUnitsTreeEntryPhysUnit* pPhysUnitDst =
        dynamic_cast<const CUnitsTreeEntryPhysUnit*>(i_pUnitDst);

    if( !m_bInitialized )
    {
        throw CException(__FILE__,__LINE__,EResultPhysSizeNotInitialized);
    }
    if( m_pPhysUnitSI == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultSIUnitNotDefined);
    }
    if( !pPhysUnitDst->m_bInitialized )
    {
        throw CException(__FILE__,__LINE__,EResultPhysSizeNotInitialized);
    }
    if( pPhysUnitDst->m_pPhysUnitSI == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultSIUnitNotDefined);
    }

    const SFctConvert* pFctConvert = nullptr;
    const SFctConvert* pFctConvertSrcIntoSI = nullptr;
    const SFctConvert* pFctConvertDstFromSI = nullptr;

    // If this and the destination unit are belonging to the same physical size ...
    if( physSize() == pPhysUnitDst->physSize() )
    {
        if( pPhysUnitDst->m_idxInParentBranch < 0 || pPhysUnitDst->m_idxInParentBranch >= m_arFctConvertsInternal.size() )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultPhysSizeNotInitialized );
        }
        pFctConvert = &m_arFctConvertsInternal.data()[pPhysUnitDst->m_idxInParentBranch];
    }
    // If this and the destination unit are not belonging to the same physical size ...
    else
    {
        pFctConvert = findFctConvertExternal(pPhysUnitDst);

        // If no direct conversion routine has been set ...
        if( pFctConvert == nullptr )
        {
            // ... the source unit must be first converted into the SI unit of this physical size ..
            pFctConvertSrcIntoSI = &m_fctConvertIntoSIUnit;

            // ... and must be afterwards converted into the SI unit of the destinations physical size ...
            pFctConvert = m_pPhysUnitSI->findFctConvertExternal(pPhysUnitDst->m_pPhysUnitSI);

            // ... and must finally be converted into the destination unit.
            pFctConvertDstFromSI = &pPhysUnitDst->m_fctConvertFromSIUnit;

            if( pFctConvertSrcIntoSI == nullptr )
            {
                throw CException(__FILE__,__LINE__,EResultFctConvertNotDefined);
            }
            if( pFctConvertDstFromSI == nullptr )
            {
                throw CException(__FILE__,__LINE__,EResultFctConvertNotDefined);
            }
        }
    }
    if( pFctConvert == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultFctConvertNotDefined);
    }

    // If this and the destination unit are not belonging to the same physical
    // size but if no direct conversion routine has been set ...
    //--------------------------------------------------------------------------

    double fVal = i_fVal;

    if( pFctConvertSrcIntoSI != nullptr )
    {
        // ... the source unit must be first converted into the SI unit of this physical size.
        fVal = convertIntoSIUnit(fVal);
    }

    // Directly convert the source unit into the destination unit
    //--------------------------------------------------------------------------

    double fM = pFctConvert->m_physValM.getVal();
    double fT = pFctConvert->m_physValT.getVal();
    double fK = pFctConvert->m_physValK.getVal();

    switch( pFctConvert->m_fctConvertType )
    {
        // Conversion routines to convert units within this physical size:
        case EFctConvert_mMULxADDt: // y = m*x + t
        {
            fVal = fM*fVal + fT;
            break;
        }
        case EFctConvert_mLOGxADDt: // y = m*log10(x) + t
        {
            if( fVal <= 0.0 )
            {
                QString strAddErrInfo = "Log10(" + QString::number(fVal) + "): Value is less or equal zero";
                throw CUnitConversionException( __FILE__, __LINE__, EResultConversionFailed, strAddErrInfo );
            }
            fVal = fM*log10(fVal) + fT;
            break;
        }
        case EFctConvert_EXP__xADDt_DIVm_:  // y = 10exp((x+t))/m) ... inverse function of y = m*log10(x) + t
        {
            fVal = pow(10.0,(fVal+fT)/fM);
            break;
        }

        // Conversion routines to convert units to another physical size
        //case EFctConvert_xMULr:
        //{
        //    fVal *= fValRef;
        //    break;
        //}
        //case EFctConvert_xDIVr:
        //{
        //    if( fVal <= 0.0 )
        //    {
        //        throw CUnitConversionException( __FILE__, __LINE__, EResultConversionFailed, "Division By Zero" );
        //    }
        //    fVal /= fValRef;
        //    break;
        //}
        //case EFctConvert_SQRxDIVr:
        //{
        //    if( fVal <= 0.0 )
        //    {
        //        throw CUnitConversionException( __FILE__, __LINE__, EResultConversionFailed, "Division By Zero" );
        //    }
        //    fVal = (fVal*fVal)/fValRef;
        //    break;
        //}
        //case EFctConvert_SQRT_xMULr_:
        //{
        //    fVal = sqrt(fVal*fValRef);
        //    break;
        //}
        //case EFctConvert_SQRxMULr:
        //{
        //    fVal = (fVal*fVal)*fValRef;
        //    break;
        //}
        //case EFctConvert_SQRT_xDIVr_:
        //{
        //    if( fVal <= 0.0 )
        //    {
        //        throw CUnitConversionException( __FILE__, __LINE__, EResultConversionFailed, "Division By Zero" );
        //    }
        //    fVal = sqrt(fVal/fValRef);
        //    break;
        //}
        //case EFctConvert_mMULxADDtADDkLOGr:
        //{
        //    fVal = fM*fVal+fT+fK*log10(fValRef);
        //    break;
        //}
        case EFctConvert_Undefined:
        default:
        {
            break;
        }
    } // switch( pFctConvert->m_fctConvert )

    // If this and the destination unit are not belonging to the same physical
    // size but if no direct conversion routine has been set ...
    //--------------------------------------------------------------------------

    if( pFctConvertDstFromSI != nullptr )
    {
        // ... the value was converted into the SI unit of the destinations physical size
        // und must now be converted from the SI unit into the destination unit.
        fVal = pPhysUnitDst->convertFromSIUnit(fVal);
    }

    return fVal;

} // convertValue

/*==============================================================================
public: // instance methods (converting values)
==============================================================================*/

//------------------------------------------------------------------------------
double CUnitsTreeEntryPhysUnit::convertFromSIUnit( double i_fVal ) const
//------------------------------------------------------------------------------
{
    if( m_pPhysUnitSI == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSIUnitNotDefined );
    }
    return m_pPhysUnitSI->convertValue(i_fVal,this);
}

//------------------------------------------------------------------------------
double CUnitsTreeEntryPhysUnit::convertIntoSIUnit( double i_fVal ) const
//------------------------------------------------------------------------------
{
    if( m_pPhysUnitSI == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSIUnitNotDefined );
    }
    return convertValue(i_fVal,m_pPhysUnitSI);
}

////------------------------------------------------------------------------------
//double CUnitsTreeEntryPhysUnit::convertValue(
//    double i_fVal, const CUnitsTreeEntryPhysUnit* i_pPhysUnitDst,
//    double i_fValRef, const CUnitsTreeEntryPhysUnit* i_pPhysUnitRef ) const
////------------------------------------------------------------------------------
//{
//    if( !m_bInitialized )
//    {
//        throw CException(__FILE__,__LINE__,EResultPhysSizeNotInitialized);
//    }
//    if( m_pPhysUnitSI == nullptr )
//    {
//        throw CException(__FILE__,__LINE__,EResultSIUnitNotDefined);
//    }
//    if( !i_pPhysUnitDst->m_bInitialized )
//    {
//        throw CException(__FILE__,__LINE__,EResultPhysSizeNotInitialized);
//    }
//    if( i_pPhysUnitDst->m_pPhysUnitSI == nullptr )
//    {
//        throw CException(__FILE__,__LINE__,EResultSIUnitNotDefined);
//    }
//
//    double fVal = i_fVal;
//    double fValRef = i_fValRef;
//    double fM = 1.0;
//    double fT = 0.0;
//    double fK = 0.0;
//
//    const SFctConvert* pFctConvert = nullptr;
//    const SFctConvert* pFctConvertSrcIntoSI = nullptr;
//    const SFctConvert* pFctConvertDstFromSI = nullptr;
//
//    // If this and the destination unit are belonging to the same physical size ...
//    if( physSize() == i_pPhysUnitDst->physSize() )
//    {
//        if( i_pPhysUnitDst->m_idxInParentBranch < 0 || i_pPhysUnitDst->m_idxInParentBranch >= m_arFctConvertsInternal.size() )
//        {
//            throw ZS::System::CException( __FILE__, __LINE__, EResultPhysSizeNotInitialized );
//        }
//        pFctConvert = &m_arFctConvertsInternal.data()[i_pPhysUnitDst->m_idxInParentBranch];
//    }
//    // If this and the destination unit are not belonging to the same physical size ...
//    else
//    {
//        pFctConvert = findFctConvertExternal(i_pPhysUnitDst);
//
//        // If no direct conversion routine has been set ...
//        if( pFctConvert == nullptr )
//        {
//            // ... the source unit must be first converted into the SI unit of this physical size ..
//            pFctConvertSrcIntoSI = &m_fctConvertIntoSIUnit;
//
//            // ... and must be afterwards converted into the SI unit of the destinations physical size ...
//            pFctConvert = m_pPhysUnitSI->findFctConvertExternal(i_pPhysUnitDst->m_pPhysUnitSI);
//
//            // ... and must finally be converted into the destination unit.
//            pFctConvertDstFromSI = &i_pPhysUnitDst->m_fctConvertFromSIUnit;
//
//            if( pFctConvertSrcIntoSI == nullptr )
//            {
//                throw CException(__FILE__,__LINE__,EResultFctConvertNotDefined);
//            }
//            if( pFctConvertDstFromSI == nullptr )
//            {
//                throw CException(__FILE__,__LINE__,EResultFctConvertNotDefined);
//            }
//        }
//    }
//    if( pFctConvert == nullptr )
//    {
//        throw CException(__FILE__,__LINE__,EResultFctConvertNotDefined);
//    }
//
//    // If this and the destination unit are not belonging to the same physical
//    // size but if no direct conversion routine has been set ...
//    //--------------------------------------------------------------------------
//
//    if( pFctConvertSrcIntoSI != nullptr )
//    {
//        // ... the source unit must be first converted into the SI unit of this physical size.
//        fVal = convertIntoSIUnit(fVal);
//    }
//
//    // Directly convert the source unit into the destination unit
//    //--------------------------------------------------------------------------
//
//    fM = pFctConvert->m_fM;
//    fT = pFctConvert->m_fT;
//    fK = pFctConvert->m_fK;
//
//    switch( pFctConvert->m_fctConvertType )
//    {
//        // Conversion routines to convert units within this physical size:
//        case EFctConvert_mMULxADDt: // y = m*x + t
//        {
//            fVal = fM*fVal + fT;
//            break;
//        }
//        case EFctConvert_mLOGxADDt: // y = m*log10(x) + t
//        {
//            if( fVal <= 0.0 )
//            {
//                QString strAddErrInfo = "Log10(" + QString::number(fVal) + "): Value is less or equal zero";
//                throw CUnitConversionException( __FILE__, __LINE__, EResultConversionFailed, strAddErrInfo );
//            }
//            fVal = fM*log10(fVal) + fT;
//            break;
//        }
//        case EFctConvert_EXP__xADDt_DIVm_:  // y = 10exp((x+t))/m) ... inverse function of y = m*log10(x) + t
//        {
//            fVal = pow(10.0,(fVal+fT)/fM);
//            break;
//        }
//
//        // Conversion routines to convert units to another physical size
//        case EFctConvert_xMULr:
//        {
//            fVal *= fValRef;
//            break;
//        }
//        case EFctConvert_xDIVr:
//        {
//            if( fVal <= 0.0 )
//            {
//                throw CUnitConversionException( __FILE__, __LINE__, EResultConversionFailed, "Division By Zero" );
//            }
//            fVal /= fValRef;
//            break;
//        }
//        case EFctConvert_SQRxDIVr:
//        {
//            if( fVal <= 0.0 )
//            {
//                throw CUnitConversionException( __FILE__, __LINE__, EResultConversionFailed, "Division By Zero" );
//            }
//            fVal = (fVal*fVal)/fValRef;
//            break;
//        }
//        case EFctConvert_SQRT_xMULr_:
//        {
//            fVal = sqrt(fVal*fValRef);
//            break;
//        }
//        case EFctConvert_SQRxMULr:
//        {
//            fVal = (fVal*fVal)*fValRef;
//            break;
//        }
//        case EFctConvert_SQRT_xDIVr_:
//        {
//            if( fVal <= 0.0 )
//            {
//                throw CUnitConversionException( __FILE__, __LINE__, EResultConversionFailed, "Division By Zero" );
//            }
//            fVal = sqrt(fVal/fValRef);
//            break;
//        }
//        case EFctConvert_mMULxADDtADDkLOGr:
//        {
//            fVal = fM*fVal+fT+fK*log10(fValRef);
//            break;
//        }
//        case EFctConvert_Undefined:
//        default:
//        {
//            break;
//        }
//    } // switch( pFctConvert->m_fctConvert )
//
//    // If this and the destination unit are not belonging to the same physical
//    // size but if no direct conversion routine has been set ...
//    //--------------------------------------------------------------------------
//
//    if( pFctConvertDstFromSI != nullptr )
//    {
//        // ... the value has been converted into the SI unit of the destinations physical size
//        // und must now be converted from the SI unit into the destination unit.
//        fVal = i_pPhysUnitDst->convertFromSIUnit(fVal);
//    }
//
//    return fVal;
//
//} // convertValue

/*==============================================================================
public: // instance methods (conversion routine into SI unit)
==============================================================================*/

//------------------------------------------------------------------------------
void CUnitsTreeEntryPhysUnit::setFctConvertFromSIUnit(const SFctConvert& i_fctConvert )
//------------------------------------------------------------------------------
{
    m_fctConvertFromSIUnit = i_fctConvert;

    // Set function to convert values from this unit into the SI unit
    //--------------------------------------------------------------------------

    m_fctConvertIntoSIUnit.m_pPhysUnitSrc = this;

    if( m_bIsLogarithmic ) {
        // y = 10exp((x+t)/m) (with t = -(SSI+SRel))
        // (for the convert from SI unit function SSI is equal to 0)
        m_fctConvertIntoSIUnit.m_fctConvertType = EFctConvert_EXP__xADDt_DIVm_;
        m_fctConvertIntoSIUnit.m_physValM = m_fctConvertFromSIUnit.m_physValM;
        m_fctConvertIntoSIUnit.m_physValT = -m_fctConvertFromSIUnit.m_physValM.getVal();
    }
    else {
        double fM = m_fctConvertFromSIUnit.m_physValM.getVal();
        m_fctConvertIntoSIUnit.m_fctConvertType = EFctConvert_mMULxADDt;
        if( fM != 0.0 ) {
            m_fctConvertIntoSIUnit.m_physValM = 1.0 / fM;
        }
    }

    CUnitsTreeEntryGrpPhysUnits* pPhysSize = physSize();
    pPhysSize->updateInternalConversionRoutines();
}

//------------------------------------------------------------------------------
double CUnitsTreeEntryPhysUnit::getFactorConvertFromSIUnit() const
//------------------------------------------------------------------------------
{
    return m_fctConvertFromSIUnit.m_physValM.getVal();
}

/*==============================================================================
public: // instance methods (conversion routines from SI unit)
Inverse functions of convert from SI unit. Calculated when setting
convertFromSI unit function.
==============================================================================*/

//------------------------------------------------------------------------------
double CUnitsTreeEntryPhysUnit::getFactorConvertIntoSIUnit() const
//------------------------------------------------------------------------------
{
    return m_fctConvertIntoSIUnit.m_physValM.getVal();
}

/*==============================================================================
public: // instance methods (conversion routines to convert into units of same quantity)
==============================================================================*/

//------------------------------------------------------------------------------
int CUnitsTreeEntryPhysUnit::findFctConvertInternalIdx( const CUnitsTreeEntryPhysUnit* i_pPhysUnitDst ) const
//------------------------------------------------------------------------------
{
    int idxFct = -1;
    for( int idx = 0; idx < m_arFctConvertsInternal.size(); idx++ )
    {
        if( m_arFctConvertsInternal[idx].m_pPhysUnitDst == i_pPhysUnitDst )
        {
            idxFct = static_cast<int>(idx);
            break;
        }
    }
    return idxFct;
}

//------------------------------------------------------------------------------
SFctConvert* CUnitsTreeEntryPhysUnit::getFctConvertInternal( int i_idx ) const
//------------------------------------------------------------------------------
{
    SFctConvert* pFctConvert = nullptr;
    if( i_idx < m_arFctConvertsInternal.size() )
    {
        pFctConvert = const_cast<SFctConvert*>(&m_arFctConvertsInternal.data()[i_idx]);
    }
    return pFctConvert;
}

//------------------------------------------------------------------------------
SFctConvert* CUnitsTreeEntryPhysUnit::findFctConvertInternal( const CUnitsTreeEntryPhysUnit* i_pPhysUnitDst ) const
//------------------------------------------------------------------------------
{
    SFctConvert* pFctConvert = nullptr;
    int idxFct = findFctConvertInternalIdx(i_pPhysUnitDst);
    if( idxFct >= 0 && idxFct < m_arFctConvertsInternal.size() )
    {
        pFctConvert = const_cast<SFctConvert*>(&m_arFctConvertsInternal.data()[idxFct]);
    }
    return pFctConvert;
}

//------------------------------------------------------------------------------
QString CUnitsTreeEntryPhysUnit::findFctConvertInternalName( const CUnitsTreeEntryPhysUnit* i_pPhysUnitDst ) const
//------------------------------------------------------------------------------
{
    QString strFctConvertName = "?";

    int idxFct = findFctConvertInternalIdx(i_pPhysUnitDst);

    // If a special (direct) conversion function has been set ..
    if( idxFct >= 0 && idxFct < m_arFctConvertsInternal.size() )
    {
        const SFctConvert* pFctConvert = getFctConvertInternal(idxFct);
        if( pFctConvert != nullptr )
        {
            strFctConvertName = pFctConvert->m_strFctConvertName;
        }
    }
    return strFctConvertName;
}

//------------------------------------------------------------------------------
QString CUnitsTreeEntryPhysUnit::getFctConvertInternalName( int i_idx ) const
//------------------------------------------------------------------------------
{
    QString strFctConvertName = "?";
    const SFctConvert* pFctConvert = getFctConvertInternal(i_idx);
    if( pFctConvert != nullptr )
    {
        strFctConvertName = pFctConvert->m_strFctConvertName;
    }
    return strFctConvertName;
}

/*==============================================================================
public: // instance methods (conversion routines to convert into units of other quantities)
==============================================================================*/

////------------------------------------------------------------------------------
//void CUnitsTreeEntryPhysUnit::addFctConvertExternal(
//    CUnitsTreeEntryPhysUnit*  i_pPhysUnitDst,
//    CUnitsTreeEntryPhysUnit*  i_pPhysUnitRef,
//    EFctConvert i_fctConvert )
////------------------------------------------------------------------------------
//{
//    //--------------------------------------------------------------------------
//    // This function may only be used to add a conversion function
//    // - to convert from the SI unit of this physical size to the SI unit
//    //   of the destination physical size as for those conversion the parameters
//    //   m and t are not supported or
//    // - to convert between logarithmic units as only in this case the resulting
//    //   conversion function will be a linear equation where the parameters
//    //   m and t may be evaluated through the configured conversion function
//    //   FctIntoSIUnit of this unit and FctFromSIUnit of the destination unit.
//    //--------------------------------------------------------------------------
//
//    // If this is the SI unit of the physical size ..
//    if( this == m_pPhysUnitSI )
//    {
//        // .. only conversion functions without the parameters m and t may be used.
//        switch( i_fctConvert )
//        {
//            case EFctConvert_xMULr:             // y = x*r
//            case EFctConvert_xDIVr:             // y = x/r
//            case EFctConvert_SQRxDIVr:          // y = x²/r
//            case EFctConvert_SQRT_xMULr_:       // y = sqrt(x*r)
//            case EFctConvert_SQRxMULr:          // y = x²*r
//            case EFctConvert_SQRT_xDIVr_:       // y = sqrt(x/r)
//            {
//                break;
//            }
//            case EFctConvert_mMULxADDt:         // y = m*x + t
//            case EFctConvert_mLOGxADDt:         // y = m*log10(x) + t
//            case EFctConvert_EXP__xADDt_DIVm_:  // y = 10exp((x+t)/m)
//            case EFctConvert_mMULxADDtADDkLOGr: // y = m*x + t + k*log10(r)
//            default:
//            {
//                throw CException(__FILE__,__LINE__,EResultFctConvertInvalid);
//            }
//        }
//    }
//    // If this is not the SI unit of the physical size ..
//    else
//    {
//        // .. this and the destination unit must be logarithmic units as only for those
//        // the resulting conversion function is a linear equation where m and t may be
//        // evaluated through the FctConvertIntoSI and FctConvertFromSI routines.
//        if( !isLogarithmic() || !i_pPhysUnitDst->isLogarithmic() )
//        {
//            throw CException(__FILE__,__LINE__,EResultFctConvertInvalid);
//        }
//    }
//
//    // Calculate and set the resulting conversion routine
//    //---------------------------------------------------
//
//    EFctConvert fctConvertType = i_fctConvert;
//    int         idxFct = m_arFctConvertsExternal.size();
//    double      fM = 1.0;
//    double      fT = 0.0;
//    double      fK = 0.0;
//
//    m_arFctConvertsExternal.append( SFctConvert() );
//
//    m_arFctConvertsExternal[idxFct].m_pPhysUnitSrc = this;
//    m_arFctConvertsExternal[idxFct].m_pPhysUnitDst = i_pPhysUnitDst;
//    m_arFctConvertsExternal[idxFct].m_pPhysUnitRef = i_pPhysUnitRef;
//
//    if( this != m_pPhysUnitSI )
//    {
//        SFctConvert* pFctConvertIntoSIUnitSrc = &m_fctConvertIntoSIUnit;
//        double       fMSrcInto = pFctConvertIntoSIUnitSrc->m_fM;
//        double       fTSrcInto = pFctConvertIntoSIUnitSrc->m_fT;
//
//        SFctConvert* pFctConvertFromSIUnitDst = &i_pPhysUnitDst->m_fctConvertFromSIUnit;
//        double       fMDstFrom = pFctConvertFromSIUnitDst->m_fM;
//        double       fTDstFrom = pFctConvertFromSIUnitDst->m_fT;
//
//        if( pFctConvertIntoSIUnitSrc->m_fctConvertType != EFctConvert_EXP__xADDt_DIVm_ )
//        {
//            throw CException(__FILE__,__LINE__,EResultFctConvertInvalid);
//        }
//        if( pFctConvertFromSIUnitDst->m_fctConvertType != EFctConvert_mLOGxADDt )
//        {
//            throw CException(__FILE__,__LINE__,EResultFctConvertInvalid);
//        }
//
//        switch( i_fctConvert )
//        {
//            case EFctConvert_xMULr: // 1. Fall: y = x*r
//            {
//                fctConvertType = EFctConvert_mMULxADDtADDkLOGr;
//                fM = fMDstFrom/fMSrcInto;
//                fT = fMDstFrom/fMSrcInto*fTSrcInto + fTDstFrom;
//                fK = fMDstFrom;
//                break;
//            }
//            case EFctConvert_xDIVr: // 2. Fall: y = x/r
//            {
//                fctConvertType = EFctConvert_mMULxADDtADDkLOGr;
//                fM = fMDstFrom/fMSrcInto;
//                fT = fMDstFrom/fMSrcInto*fTSrcInto + fTDstFrom;
//                fK = -fMDstFrom;
//                break;
//            }
//            case EFctConvert_SQRxDIVr: // 3. Fall: y = x²/r
//            {
//                fctConvertType = EFctConvert_mMULxADDtADDkLOGr;
//                fM = 2.0*fMDstFrom/fMSrcInto;
//                fT = 2.0*fMDstFrom/fMSrcInto*fTSrcInto + fTDstFrom;
//                fK = -fMDstFrom;
//                break;
//            }
//            case EFctConvert_SQRT_xMULr_: // 4. Fall: y = sqrt(x*r)
//            {
//                fctConvertType = EFctConvert_mMULxADDtADDkLOGr;
//                fM = 0.5*fMDstFrom/fMSrcInto;
//                fT = 0.5*fMDstFrom/fMSrcInto*fTSrcInto + fTDstFrom;
//                fK = 0.5*fMDstFrom;
//                break;
//            }
//            case EFctConvert_SQRxMULr: // 5. Fall: y = x²*r
//            {
//                fctConvertType = EFctConvert_mMULxADDtADDkLOGr;
//                fM = 2.0*fMDstFrom/fMSrcInto;
//                fT = 2.0*fMDstFrom/fMSrcInto*fTSrcInto + fTDstFrom;
//                fK = fMDstFrom;
//                break;
//            }
//            case EFctConvert_SQRT_xDIVr_: // 6. Fall: y = sqrt(x/r)
//            {
//                fctConvertType = EFctConvert_mMULxADDtADDkLOGr;
//                fM = 0.5*fMDstFrom/fMSrcInto;
//                fT = 0.5*fMDstFrom/fMSrcInto*fTSrcInto + fTDstFrom;
//                fK = -0.5*fMDstFrom;
//                break;
//            }
//            default:
//            {
//                break;
//            }
//        } // switch( i_fctConvert )
//
//    } // if( this != m_pPhysUnitSI )
//
//    m_arFctConvertsExternal[idxFct].m_fctConvertType = fctConvertType;
//    m_arFctConvertsExternal[idxFct].m_fM = fM;
//    m_arFctConvertsExternal[idxFct].m_fT = fT;
//    m_arFctConvertsExternal[idxFct].m_fK = fK;
//
//    // For test and debugging purposes: build an expressive function name.
//    m_arFctConvertsExternal[idxFct].buildFctConvertName();
//
//} // addFctConvertExternal

//------------------------------------------------------------------------------
int CUnitsTreeEntryPhysUnit::findFctConvertExternalIdx( const CUnitsTreeEntryPhysUnit* i_pPhysUnitDst ) const
//------------------------------------------------------------------------------
{
    int idxFct = -1;
    for( int idx = 0; idx < m_arFctConvertsExternal.size(); idx++ )
    {
        if( m_arFctConvertsExternal[idx].m_pPhysUnitDst == i_pPhysUnitDst )
        {
            idxFct = static_cast<int>(idx);
            break;
        }
    }
    return idxFct;
}

//------------------------------------------------------------------------------
SFctConvert* CUnitsTreeEntryPhysUnit::getFctConvertExternal( int i_idx ) const
//------------------------------------------------------------------------------
{
    SFctConvert* pFctConvert = nullptr;
    if( i_idx < m_arFctConvertsExternal.size() )
    {
        pFctConvert = const_cast<SFctConvert*>(&m_arFctConvertsExternal.data()[i_idx]);
    }
    return pFctConvert;
}

//------------------------------------------------------------------------------
SFctConvert* CUnitsTreeEntryPhysUnit::findFctConvertExternal( const CUnitsTreeEntryPhysUnit* i_pPhysUnitDst ) const
//------------------------------------------------------------------------------
{
    SFctConvert* pFctConvert = nullptr;
    int idxFct = findFctConvertExternalIdx(i_pPhysUnitDst);
    if( idxFct >= 0 && idxFct < m_arFctConvertsExternal.size() )
    {
        pFctConvert = const_cast<SFctConvert*>(&m_arFctConvertsExternal.data()[idxFct]);
    }
    return pFctConvert;
}

//------------------------------------------------------------------------------
QString CUnitsTreeEntryPhysUnit::findFctConvertExternalName( const CUnitsTreeEntryPhysUnit* i_pPhysUnitDst ) const
//------------------------------------------------------------------------------
{
    QString strFctConvertName = "?";
    int idxFct = findFctConvertExternalIdx(i_pPhysUnitDst);
    // If a special (direct) conversion function has been set ..
    if( idxFct >= 0 && idxFct < m_arFctConvertsExternal.size() )
    {
        const SFctConvert* pFctConvert = getFctConvertExternal(idxFct);
        if( pFctConvert != nullptr )
        {
            strFctConvertName = pFctConvert->m_strFctConvertName;
        }
    }
    return strFctConvertName;
}

//------------------------------------------------------------------------------
QString CUnitsTreeEntryPhysUnit::getFctConvertExternalName( int i_idx ) const
//------------------------------------------------------------------------------
{
    QString strFctConvertName = "?";
    SFctConvert* pFctConvert = getFctConvertExternal(i_idx);
    if( pFctConvert != nullptr )
    {
        strFctConvertName = pFctConvert->m_strFctConvertName;
    }
    return strFctConvertName;
}
