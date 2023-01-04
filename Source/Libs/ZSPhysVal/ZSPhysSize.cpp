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

#include "ZSPhysVal/ZSPhysSize.h"
#include "ZSPhysVal/ZSPhysUnits.h"
#include "ZSPhysVal/ZSPhysSizesIdxTree.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysErrResult.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::Math;
using namespace ZS::PhysVal;


/*******************************************************************************
class CPhysSize : public CUnitGrp
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief

    @param [in] i_pIdxTree
    @param [in] i_scienceField
    @param [in] i_strName
        e.g. "Time", "Length", "Power", "Voltage", ...
    @param [in] i_strSIUnitName
        e.g. "Second" for Time, "Meter" for Length, "Watt" for el. Power, "Volt" for el. Voltage, ...
    @param [in] i_strSIUnitSymbol
        e.g. "s" for Second, "m" for Meter, "W" for "Watt", "V" for el. Volt, ...
    @param [in] i_strFormulaSymbol
        e.g. "t" for Time, "l" for Length, "P" for Power, "U" for Voltage, ...
    @param [in] i_bIsPowerRelated
        true if X/dB = 10*log(X/X0), false if e.g. X/dB = 20*log(X/X0)
*/
CPhysSize::CPhysSize(
    CIdxTree*         i_pIdxTree,
    EPhysScienceField i_scienceField,
    const QString&    i_strName,
    const QString&    i_strSIUnitName,
    const QString&    i_strSIUnitSymbol,
    const QString&    i_strFormulaSymbol,
    bool              i_bIsPowerRelated,
    CIdxTreeEntry*    i_pParentBranch ) :
//------------------------------------------------------------------------------
    CUnitGrp(
        /* pIdxTree      */ i_pIdxTree,
        /* type          */ EUnitClassTypePhysScienceFields,
        /* strName       */ i_strName,
        /* pParentBranch */ i_pParentBranch ),
    m_strSIUnitName(i_strSIUnitName),
    m_strSIUnitSymbol(i_strSIUnitSymbol),
    m_pPhysUnitSI(nullptr),
    m_strFormulaSymbol(i_strFormulaSymbol),
    m_bIsPowerRelated(i_bIsPowerRelated),
    m_bInitialized(false)
{
} // ctor

//------------------------------------------------------------------------------
/*! @brief

    @param [in] i_pIdxTree
    @param [in] i_scienceField
    @param [in] i_strName
        e.g. "Time", "Length", "Power", "Voltage", ...
    @param [in] i_strSIUnitName
        e.g. "Second" for Time, "Meter" for Length, "Watt" for el. Power, "Volt" for el. Voltage, ...
    @param [in] i_strSIUnitSymbol
        e.g. "s" for Second, "m" for Meter, "W" for "Watt", "V" for el. Volt, ...
    @param [in] i_strFormulaSymbol
        e.g. "t" for Time, "l" for Length, "P" for Power, "U" for Voltage, ...
    @param [in] i_bIsPowerRelated
        true if X/dB = 10*log(X/X0), false if e.g. X/dB = 20*log(X/X0)
*/
CPhysSize::CPhysSize(
    CIdxTreeEntry*    i_pParentBranch,
    EPhysScienceField i_scienceField,
    const QString&    i_strName,
    const QString&    i_strSIUnitName,
    const QString&    i_strSIUnitSymbol,
    const QString&    i_strFormulaSymbol,
    bool              i_bIsPowerRelated ) :
//------------------------------------------------------------------------------
    CUnitGrp(
        /* pParentBranch */ i_pParentBranch,
        /* type          */ EUnitClassTypePhysScienceFields,
        /* strName       */ i_strName ),
    m_strSIUnitName(i_strSIUnitName),
    m_strSIUnitSymbol(i_strSIUnitSymbol),
    m_pPhysUnitSI(nullptr),
    m_strFormulaSymbol(i_strFormulaSymbol),
    m_bIsPowerRelated(i_bIsPowerRelated),
    m_bInitialized(false)
{
} // ctor

//------------------------------------------------------------------------------
CPhysSize::~CPhysSize()
//------------------------------------------------------------------------------
{
    m_pPhysUnitSI = nullptr;

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CPhysSize::operator == ( const CPhysSize& i_physsizeOther ) const
//------------------------------------------------------------------------------
{
    if( this != &i_physsizeOther )
    {
        return false;
    }
    return true;

} // operator ==

//------------------------------------------------------------------------------
bool CPhysSize::operator != ( const CPhysSize& i_physsizeOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_physsizeOther );

} // operator ==

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CPhysSize::initialize( bool i_bCreateFindBestChainedList )
//------------------------------------------------------------------------------
{
    // The physical size scans all its physical units and will set the default
    // conversion routines "IntoSIUnit" and "FromSIUnit" of the physical units
    // correspondingly (if no direct conversion routines into the SI unit have
    // not yet been set).

    //CUnit*       pUnit;
    //CPhysUnit*   pPhysUnitPrev = nullptr;
    //CPhysUnit*   pPhysUnitCurr = nullptr;
    //CPhysUnit*   pPhysUnitNext = nullptr;
    //CPhysUnit*   pPhysUnitDst = nullptr;
    //CFctConvert* pFctConvertIntoSIUnitSrc = nullptr;
    //CFctConvert* pFctConvertFromSIUnitDst = nullptr;
    //double       fMSrcInto;
    //double       fTSrcInto;
    //double       fMDstFrom;
    //double       fTDstFrom;
    //int          idxUnit;
    //int          idxUnitDst;

    // Find the SI unit of this physical size
    //---------------------------------------

    int iUnitsCount = 0;
    for( int idxUnit = 0; idxUnit < m_arpTreeEntries.size(); idxUnit++ ) {
        CPhysUnit* pUnit = dynamic_cast<CPhysUnit*>(m_arpTreeEntries[idxUnit]);
        if( pUnit != nullptr ) {
            iUnitsCount++;
            if( pUnit->name() == m_strSIUnitName && pUnit->symbol() == m_strSIUnitSymbol ) {
                m_pPhysUnitSI = pUnit;
                break;
            }
        }
    }
    if( iUnitsCount== 0 ) {
        throw CException(
            __FILE__, __LINE__, EResultInvalidPhysSize,
            "Physical size does not have any physical units." );
    }
    if( m_pPhysUnitSI == nullptr )
    {
        throw CException(
            __FILE__, __LINE__, EResultSIUnitNotDefined,
            "SI Unit of " + keyInTree() + " not defined." );
    }

    // Set the SI unit for all units of this physical size
    //----------------------------------------------------

    for( int idxUnit = 0; idxUnit < m_arpTreeEntries.size(); idxUnit++ )
    {
        CPhysUnit* pUnit = dynamic_cast<CPhysUnit*>(m_arpTreeEntries[idxUnit]);
        if( pUnit != nullptr ) {
            pUnit->m_pPhysUnitSI = m_pPhysUnitSI;
            pUnit->m_fctConvertFromSIUnit.m_pPhysUnitSrc = m_pPhysUnitSI;
            pUnit->m_fctConvertFromSIUnit.buildFctConvertName();
            pUnit->m_fctConvertIntoSIUnit.m_pPhysUnitDst = m_pPhysUnitSI;
            pUnit->m_fctConvertIntoSIUnit.buildFctConvertName();
        }
    }

    // Create chained list through NextLower and NextHigher units
    //-----------------------------------------------------------

    if( i_bCreateFindBestChainedList ) {
        CPhysUnit* pPhysUnitCurr = nullptr;
        CPhysUnit* pPhysUnitNext = nullptr;
        int idxUnit = 0;
        for( CPhysUnit* pPhysUnitPrev = nullptr; idxUnit < m_arpTreeEntries.size(); idxUnit++ ) {
            CPhysUnit* pUnit = dynamic_cast<CPhysUnit*>(m_arpTreeEntries[idxUnit]);
            if( idxUnit == 0 ) {
                pPhysUnitCurr = pUnit;
            }
            else {
                pPhysUnitCurr = pPhysUnitNext;
            }
            if( pPhysUnitCurr->m_bIsLogarithmic ) {
                break;
            }
            if( idxUnit < m_arpTreeEntries.size()-1 ) {
                pPhysUnitNext = dynamic_cast<CPhysUnit*>(m_arpTreeEntries[idxUnit+1]);
            }
            else {
                pPhysUnitNext = nullptr;
            }
            pPhysUnitCurr->m_pNextLower = pPhysUnitPrev;

            if( pPhysUnitNext == nullptr || pPhysUnitNext->m_bIsLogarithmic ) {
                pPhysUnitCurr->m_pNextHigher = nullptr;
            }
            else {
                pPhysUnitCurr->m_pNextHigher = pPhysUnitNext;
            }
            pPhysUnitPrev = pPhysUnitCurr;
        }

        for( ; idxUnit < m_arpTreeEntries.size(); idxUnit++ )
        {
            CPhysUnit* pUnit = dynamic_cast<CPhysUnit*>(m_arpTreeEntries[idxUnit]);
            if( !pUnit->m_bIsLogarithmic )
            {
                throw ZS::System::CException(
                    __FILE__, __LINE__, EResultInvalidPhysSize,
                    "Unexpected order of physical units. "
                    "Logarithmic units must be added at the end of the list." );
            }
        }
    }

    // For all units of this physical size allocate array for routines to
    // convert units within this physical size (internal conversion routines)
    //--------------------------------------------------------------------------

    for( int idxUnit = 0; idxUnit < m_arpTreeEntries.size(); idxUnit++ )
    {
        CPhysUnit* pUnit = dynamic_cast<CPhysUnit*>(m_arpTreeEntries[idxUnit]);
        pUnit->m_arFctConvertsInternal.resize(m_arpTreeEntries.size());
    }

    // For all units of this physical size set the internal conversion routines.
    //--------------------------------------------------------------------------

    for( int idxUnitSrc = 0; idxUnitSrc < m_arpTreeEntries.size(); idxUnitSrc++ )
    {
        CPhysUnit* pUnitSrc = dynamic_cast<CPhysUnit*>(m_arpTreeEntries[idxUnitSrc]);

        CFctConvert* pFctConvertIntoSIUnitSrc = &pUnitSrc->m_fctConvertIntoSIUnit;

        double fMSrcInto = pFctConvertIntoSIUnitSrc->m_fM;
        double fTSrcInto = pFctConvertIntoSIUnitSrc->m_fT;

        for( int idxUnitDst = 0; idxUnitDst < m_arpTreeEntries.size(); idxUnitDst++ )
        {
            CPhysUnit* pUnitDst = dynamic_cast<CPhysUnit*>(m_arpTreeEntries[idxUnitDst]);

            CFctConvert* pFctConvertFromSIUnitDst = &pUnitDst->m_fctConvertFromSIUnit;

            double fMDstFrom = pFctConvertFromSIUnitDst->m_fM;
            double fTDstFrom = pFctConvertFromSIUnitDst->m_fT;

            pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_pPhysUnitSrc = pUnitSrc;
            pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_pPhysUnitDst = pUnitDst;

            if( pFctConvertIntoSIUnitSrc->m_fctConvertType == EFctConvert_mMULxADDt )
            {
                // Conversion from linear to linear unit:
                if( pFctConvertFromSIUnitDst->m_fctConvertType == EFctConvert_mMULxADDt )
                {
                    pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_fctConvertType = EFctConvert_mMULxADDt;
                    pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_fM = fMDstFrom*fMSrcInto;
                    pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_fT = fMDstFrom*fTSrcInto + fTDstFrom;
                }
                // Conversion from linear to logarithmic unit:
                else if( pFctConvertFromSIUnitDst->m_fctConvertType == EFctConvert_mLOGxADDt )
                {
                    pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_fctConvertType = EFctConvert_mLOGxADDt;
                    pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_fM = fMDstFrom;
                    pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_fT = fMDstFrom*log10(fMSrcInto) + fTDstFrom;
                }
                else
                {
                    throw CException(__FILE__,__LINE__,EResultFctConvertInvalid);
                }
            }
            else if( pFctConvertIntoSIUnitSrc->m_fctConvertType == EFctConvert_EXP__xADDt_DIVm_ )
            {
                // Conversion from logarithmic to linear unit:
                if( pFctConvertFromSIUnitDst->m_fctConvertType == EFctConvert_mMULxADDt )
                {
                    pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_fctConvertType = EFctConvert_EXP__xADDt_DIVm_;
                    pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_fM = fMSrcInto;
                    pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_fT = fMSrcInto*log10(fMDstFrom) + fTSrcInto;
                }
                // Conversion from logarithmic to logarithmic unit:
                else if( pFctConvertFromSIUnitDst->m_fctConvertType == EFctConvert_mLOGxADDt )
                {
                    pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_fctConvertType = EFctConvert_mMULxADDt;
                    pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_fM = fMDstFrom/fMSrcInto;
                    pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_fT = (fMDstFrom*fTSrcInto)/fMSrcInto + fTDstFrom;
                }
                else
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultFctConvertInvalid );
                }
            }
            else
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultFctConvertInvalid );
            }

            // For test and debugging purposes: build an expressive function name.
            pUnitSrc->m_arFctConvertsInternal[idxUnitDst].buildFctConvertName();

        } // for( idxUnitDst = 0; idxUnitDst < m_arpTreeEntries.size(); idxUnitDst++ )
    } // for( idxUnitSrc = 0; idxUnitSrc < m_arpTreeEntries.size(); idxUnitSrc++ )

    // Set flag that this physical size has been initialized
    //------------------------------------------------------

    for( int idxUnit = 0; idxUnit < m_arpTreeEntries.size(); idxUnit++ )
    {
        CPhysUnit* pUnit = dynamic_cast<CPhysUnit*>(m_arpTreeEntries[idxUnit]);
        pUnit->m_bInitialized = true;
    }

    m_bInitialized = true;

    return m_bInitialized;

} // initialize

//------------------------------------------------------------------------------
void CPhysSize::addFctConvert(
    CPhysSize*  i_pPhysSizeDst,
    CPhysSize*  i_pPhysSizeRef,
    EFctConvert i_fctConvert )
//------------------------------------------------------------------------------
{
    if( m_pPhysUnitSI == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultSIUnitNotDefined);
    }
    if( i_pPhysSizeDst->getSIUnit() == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultSIUnitNotDefined);
    }
    if( i_pPhysSizeRef->getSIUnit() == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultSIUnitNotDefined);
    }

    // First step: add an external conversion function to the SI unit
    //---------------------------------------------------------------

    m_pPhysUnitSI->addFctConvertExternal(
        /* physUnitDst     */ i_pPhysSizeDst->getSIUnit(),
        /* physUnitRef     */ i_pPhysSizeRef->getSIUnit(),
        /* fctConvertUser1 */ i_fctConvert );

    // Second step: add external conversion function to the logarithmic units of this physical size.
    //----------------------------------------------------------------------------------------------

    // For each logarithmic unit within the other physical size an external conversion
    // routine will be added to each logarithmic unit of this physical size.

    for( int idxChildSrc = 0; idxChildSrc < m_arpTreeEntries.size(); idxChildSrc++ )
    {
        CPhysUnit* pPhysUnitSrc = getPhysUnit(idxChildSrc);

        if( pPhysUnitSrc != nullptr && pPhysUnitSrc->isLogarithmic() )
        {
            for( int idxChildDst = 0; idxChildDst < i_pPhysSizeDst->m_arpTreeEntries.size(); idxChildDst++ )
            {
                CPhysUnit* pPhysUnitDst = i_pPhysSizeDst->getPhysUnit(idxChildDst);

                if( pPhysUnitDst != nullptr && pPhysUnitDst->isLogarithmic() )
                {
                    pPhysUnitSrc->addFctConvertExternal(
                        /* physUnitDst     */ pPhysUnitDst,
                        /* physUnitRef     */ i_pPhysSizeRef->getSIUnit(),
                        /* fctConvertUser1 */ i_fctConvert );
                }
            }
        }
    }

} // addFctConvert

//------------------------------------------------------------------------------
CPhysUnit* CPhysSize::getPhysUnit( int i_idx )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CPhysUnit*>(at(i_idx));
}

//------------------------------------------------------------------------------
CPhysUnit* CPhysSize::findPhysUnit( const QString& i_strSymbolOrName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CPhysUnit*>(findUnit(i_strSymbolOrName));
}

//------------------------------------------------------------------------------
CPhysUnit* CPhysSize::findPhysUnitBySymbol( const QString& i_strSymbol )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CPhysUnit*>(findUnitBySymbol(i_strSymbol));
}

//------------------------------------------------------------------------------
CPhysUnit* CPhysSize::findPhysUnitByName( const QString& i_strName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CPhysUnit*>(findUnitByName(i_strName));
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief

    @param i_pPhysUnitRef [in]
        nullptr to specify the SI unit.
*/
double CPhysSize::getRefVal( CPhysUnit* /*i_pPhysUnitRef*/ ) const
//------------------------------------------------------------------------------
{
    QString strAddErrInfo = "No reference value defined for physical size " + keyInTree();
    throw CException( __FILE__, __LINE__, EResultInvalidRefVal, strAddErrInfo );
}
