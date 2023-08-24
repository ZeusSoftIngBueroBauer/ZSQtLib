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

#include "ZSPhysVal/ZSPhysTreeEntryGrpPhysUnits.h"
#include "ZSPhysVal/ZSPhysTreeEntryGrpScienceField.h"
#include "ZSPhysVal/ZSPhysTreeEntryPhysUnit.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysMath.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::Math;
using namespace ZS::PhysVal;


/*******************************************************************************
class CUnitsTreeEntryGrpPhysUnits : public CUnitsTreeEntryGrpBase
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief  Creates an instance of a group for physical units.

    @param [in] i_pIdxTree
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
CUnitsTreeEntryGrpPhysUnits::CUnitsTreeEntryGrpPhysUnits(
    CIdxTreeUnits* i_pIdxTree,
    const QString& i_strName,
    const QString& i_strSIUnitName,
    const QString& i_strSIUnitSymbol,
    const QString& i_strFormulaSymbol,
    bool           i_bIsPowerRelated ) :
//------------------------------------------------------------------------------
    CUnitsTreeEntryGrpBase(i_pIdxTree, EUnitClassType::PhysSize, i_strName),
    m_strSIUnitName(i_strSIUnitName),
    m_strSIUnitSymbol(i_strSIUnitSymbol),
    m_pPhysUnitSI(nullptr),
    m_strFormulaSymbol(i_strFormulaSymbol),
    m_bIsPowerRelated(i_bIsPowerRelated),
    m_bInitialized(false)
{
} // ctor

//------------------------------------------------------------------------------
/*! @brief  Creates an instance of a for with physical units.

    @param [in] i_pParentBranch
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
CUnitsTreeEntryGrpPhysUnits::CUnitsTreeEntryGrpPhysUnits(
    CUnitsTreeEntryGrpScienceField* i_pParentBranch,
    const QString& i_strName,
    const QString& i_strSIUnitName,
    const QString& i_strSIUnitSymbol,
    const QString& i_strFormulaSymbol,
    bool           i_bIsPowerRelated ) :
//------------------------------------------------------------------------------
    CUnitsTreeEntryGrpBase(i_pParentBranch, EUnitClassType::PhysSize, i_strName),
    m_strSIUnitName(i_strSIUnitName),
    m_strSIUnitSymbol(i_strSIUnitSymbol),
    m_pPhysUnitSI(nullptr),
    m_strFormulaSymbol(i_strFormulaSymbol),
    m_bIsPowerRelated(i_bIsPowerRelated),
    m_bInitialized(false)
{
} // ctor

//------------------------------------------------------------------------------
/*! @brief  Creates an instance of a for with physical units.

    @param [in] i_pParentBranch
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
CUnitsTreeEntryGrpPhysUnits::CUnitsTreeEntryGrpPhysUnits(
    CIdxTreeEntry* i_pParentBranch,
    const QString& i_strName,
    const QString& i_strSIUnitName,
    const QString& i_strSIUnitSymbol,
    const QString& i_strFormulaSymbol,
    bool           i_bIsPowerRelated ) :
//------------------------------------------------------------------------------
    CUnitsTreeEntryGrpBase(i_pParentBranch, EUnitClassType::PhysSize, i_strName),
    m_strSIUnitName(i_strSIUnitName),
    m_strSIUnitSymbol(i_strSIUnitSymbol),
    m_pPhysUnitSI(nullptr),
    m_strFormulaSymbol(i_strFormulaSymbol),
    m_bIsPowerRelated(i_bIsPowerRelated),
    m_bInitialized(false)
{
} // ctor

//------------------------------------------------------------------------------
/*! @brief Destroys the group of physical units.
*/
CUnitsTreeEntryGrpPhysUnits::~CUnitsTreeEntryGrpPhysUnits()
//------------------------------------------------------------------------------
{
    m_pPhysUnitSI = nullptr;

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CUnitsTreeEntryGrpPhysUnits::operator == ( const CUnitsTreeEntryGrpPhysUnits& i_physsizeOther ) const
//------------------------------------------------------------------------------
{
    if( this != &i_physsizeOther )
    {
        return false;
    }
    return true;

} // operator ==

//------------------------------------------------------------------------------
bool CUnitsTreeEntryGrpPhysUnits::operator != ( const CUnitsTreeEntryGrpPhysUnits& i_physsizeOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_physsizeOther );

} // operator ==

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Initialization function for the group of physical units which must
           be invoked after all units have been added to the group and before
           the group and any unit conversion routine is used.

    The physical size scans all its physical units and will set the default
    conversion routines "IntoSIUnit" and "FromSIUnit" of the physical units
    correspondingly (if no direct conversion routines into the SI unit have
    not yet been set).

    @param {in] i_bCreateFindBestChainedList
        The chained list of next higher and next lower units can only be
        automatically detected if the units are added starting with the
        smallest unit (like nm) to the bigggest unit (like km).
        If the unit group does not follow this rule the chained list must
        be created after invoking the initialize method by applying
        "setNextLower" and "setNextLower" to the units "manually" after
        "initialize" has been called.
*/
bool CUnitsTreeEntryGrpPhysUnits::initialize( bool i_bCreateFindBestChainedList )
//------------------------------------------------------------------------------
{
    // For all units of this physical size allocate array for routines to
    // convert units within this physical size (internal conversion routines)
    for( int idxUnit = 0; idxUnit < m_arpTreeEntries.size(); idxUnit++ )
    {
        CUnitsTreeEntryPhysUnit* pUnit = dynamic_cast<CUnitsTreeEntryPhysUnit*>(m_arpTreeEntries[idxUnit]);
        pUnit->m_arFctConvertsInternal.resize(m_arpTreeEntries.size());
    }

    findAndSetSIUnit();

    if( i_bCreateFindBestChainedList ) {
        createFindBestUnitChainedList();
    }

    updateInternalConversionRoutines();

    // Set flag that this physical size has been initialized
    //------------------------------------------------------

    for( int idxUnit = 0; idxUnit < m_arpTreeEntries.size(); idxUnit++ )
    {
        CUnitsTreeEntryPhysUnit* pUnit = dynamic_cast<CUnitsTreeEntryPhysUnit*>(m_arpTreeEntries[idxUnit]);
        pUnit->m_bInitialized = true;
    }

    m_bInitialized = true;

    return m_bInitialized;

} // initialize

//------------------------------------------------------------------------------
/*! @brief Set the internal conversion for the specified unit.
*/
void CUnitsTreeEntryGrpPhysUnits::updateInternalConversionRoutines()
//------------------------------------------------------------------------------
{
    for( int idxUnitSrc = 0; idxUnitSrc < m_arpTreeEntries.size(); idxUnitSrc++ )
    {
        CUnitsTreeEntryPhysUnit* pUnitSrc = dynamic_cast<CUnitsTreeEntryPhysUnit*>(m_arpTreeEntries[idxUnitSrc]);

        SFctConvert* pFctConvertIntoSIUnitSrc = &pUnitSrc->m_fctConvertIntoSIUnit;

        pUnitSrc->m_fctConvertFromSIUnit.buildFctConvertName();
        pUnitSrc->m_fctConvertIntoSIUnit.buildFctConvertName();

        double fMSrcInto = pFctConvertIntoSIUnitSrc->m_physValM.getVal();
        double fTSrcInto = pFctConvertIntoSIUnitSrc->m_physValT.getVal();

        // Update conversion table of the given source unit
        //-------------------------------------------------

        for( int idxUnitDst = 0; idxUnitDst < m_arpTreeEntries.size(); idxUnitDst++ )
        {
            CUnitsTreeEntryPhysUnit* pUnitDst = dynamic_cast<CUnitsTreeEntryPhysUnit*>(m_arpTreeEntries[idxUnitDst]);

            SFctConvert* pFctConvertFromSIUnitDst = &pUnitDst->m_fctConvertFromSIUnit;

            double fMDstFrom = pFctConvertFromSIUnitDst->m_physValM.getVal();
            double fTDstFrom = pFctConvertFromSIUnitDst->m_physValT.getVal();

            pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_pPhysUnitSrc = pUnitSrc;
            pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_pPhysUnitDst = pUnitDst;

            if( pUnitSrc == pUnitDst )
            {
                pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_fctConvertType = EFctConvert_mMULxADDt;
                pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_physValM = 1.0;
                pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_physValT = 0.0;
            }
            else if( pFctConvertIntoSIUnitSrc->m_fctConvertType == EFctConvert_mMULxADDt )
            {
                // Conversion from linear to linear unit:
                if( pFctConvertFromSIUnitDst->m_fctConvertType == EFctConvert_mMULxADDt )
                {
                    pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_fctConvertType = EFctConvert_mMULxADDt;
                    pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_physValM = fMDstFrom*fMSrcInto;
                    pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_physValT = fMDstFrom*fTSrcInto + fTDstFrom;
                }
                // Conversion from linear to logarithmic unit:
                else if( pFctConvertFromSIUnitDst->m_fctConvertType == EFctConvert_mLOGxADDt )
                {
                    pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_fctConvertType = EFctConvert_mLOGxADDt;
                    pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_physValM = fMDstFrom;
                    pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_physValT = fMDstFrom*log10(fMSrcInto) + fTDstFrom;
                }
                else
                {
                    pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_fctConvertType = EFctConvert_Undefined;
                }
            }
            else if( pFctConvertIntoSIUnitSrc->m_fctConvertType == EFctConvert_EXP__xADDt_DIVm_ )
            {
                // Conversion from logarithmic to linear unit:
                if( pFctConvertFromSIUnitDst->m_fctConvertType == EFctConvert_mMULxADDt )
                {
                    pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_fctConvertType = EFctConvert_EXP__xADDt_DIVm_;
                    pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_physValM = fMSrcInto;
                    pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_physValT = fMSrcInto*log10(fMDstFrom) + fTSrcInto;
                }
                // Conversion from logarithmic to logarithmic unit:
                else if( pFctConvertFromSIUnitDst->m_fctConvertType == EFctConvert_mLOGxADDt )
                {
                    pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_fctConvertType = EFctConvert_mMULxADDt;
                    pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_physValM = fMDstFrom/fMSrcInto;
                    pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_physValT = (fMDstFrom*fTSrcInto)/fMSrcInto + fTDstFrom;
                }
                else
                {
                    pUnitSrc->m_arFctConvertsInternal[idxUnitDst].m_fctConvertType = EFctConvert_Undefined;
                }
            }

            // For test and debugging purposes: build an expressive function name.
            pUnitSrc->m_arFctConvertsInternal[idxUnitDst].buildFctConvertName();

        } // for( idxUnitDst = 0; idxUnitDst < m_arpTreeEntries.size(); idxUnitDst++ )
    } // for( idxUnitSrc = 0; idxUnitSrc < m_arpTreeEntries.size(); idxUnitSrc++ )

} // updateInternalConversionRoutine

////------------------------------------------------------------------------------
//void CUnitsTreeEntryGrpPhysUnits::addFctConvert(
//    CUnitsTreeEntryGrpPhysUnits*  i_pPhysSizeDst,
//    CUnitsTreeEntryGrpPhysUnits*  i_pPhysSizeRef,
//    EFctConvert i_fctConvert )
////------------------------------------------------------------------------------
//{
//    if( m_pPhysUnitSI == nullptr )
//    {
//        throw CException(__FILE__,__LINE__,EResultSIUnitNotDefined);
//    }
//    if( i_pPhysSizeDst->getSIUnit() == nullptr )
//    {
//        throw CException(__FILE__,__LINE__,EResultSIUnitNotDefined);
//    }
//    if( i_pPhysSizeRef->getSIUnit() == nullptr )
//    {
//        throw CException(__FILE__,__LINE__,EResultSIUnitNotDefined);
//    }
//
//    // First step: add an external conversion function to the SI unit
//    //---------------------------------------------------------------
//
//    m_pPhysUnitSI->addFctConvertExternal(
//        /* physUnitDst     */ i_pPhysSizeDst->getSIUnit(),
//        /* physUnitRef     */ i_pPhysSizeRef->getSIUnit(),
//        /* fctConvertUser1 */ i_fctConvert );
//
//    // Second step: add external conversion function to the logarithmic units of this physical size.
//    //----------------------------------------------------------------------------------------------
//
//    // For each logarithmic unit within the other physical size an external conversion
//    // routine will be added to each logarithmic unit of this physical size.
//
//    for( int idxChildSrc = 0; idxChildSrc < m_arpTreeEntries.size(); idxChildSrc++ )
//    {
//        CUnitsTreeEntryPhysUnit* pPhysUnitSrc = getPhysUnit(idxChildSrc);
//
//        if( pPhysUnitSrc != nullptr && pPhysUnitSrc->isLogarithmic() )
//        {
//            for( int idxChildDst = 0; idxChildDst < i_pPhysSizeDst->m_arpTreeEntries.size(); idxChildDst++ )
//            {
//                CUnitsTreeEntryPhysUnit* pPhysUnitDst = i_pPhysSizeDst->getPhysUnit(idxChildDst);
//
//                if( pPhysUnitDst != nullptr && pPhysUnitDst->isLogarithmic() )
//                {
//                    pPhysUnitSrc->addFctConvertExternal(
//                        /* physUnitDst     */ pPhysUnitDst,
//                        /* physUnitRef     */ i_pPhysSizeRef->getSIUnit(),
//                        /* fctConvertUser1 */ i_fctConvert );
//                }
//            }
//        }
//    }
//
//} // addFctConvert

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CUnitsTreeEntryPhysUnit* CUnitsTreeEntryGrpPhysUnits::getPhysUnit( int i_idx ) const
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitsTreeEntryPhysUnit*>(at(i_idx));
}

//------------------------------------------------------------------------------
CUnitsTreeEntryPhysUnit* CUnitsTreeEntryGrpPhysUnits::findPhysUnit( const QString& i_strSymbolOrName ) const
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitsTreeEntryPhysUnit*>(findUnit(i_strSymbolOrName));
}

//------------------------------------------------------------------------------
CUnitsTreeEntryPhysUnit* CUnitsTreeEntryGrpPhysUnits::findPhysUnitBySymbol( const QString& i_strSymbol ) const
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitsTreeEntryPhysUnit*>(findUnitBySymbol(i_strSymbol));
}

//------------------------------------------------------------------------------
CUnitsTreeEntryPhysUnit* CUnitsTreeEntryGrpPhysUnits::findPhysUnitByName( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitsTreeEntryPhysUnit*>(findUnitByName(i_strName));
}

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CUnitsTreeEntryGrpPhysUnits::findAndSetSIUnit()
//------------------------------------------------------------------------------
{
    // Find the SI unit of this physical size
    //---------------------------------------

    int iUnitsCount = 0;
    for( int idxUnit = 0; idxUnit < m_arpTreeEntries.size(); idxUnit++ ) {
        CUnitsTreeEntryPhysUnit* pUnit = dynamic_cast<CUnitsTreeEntryPhysUnit*>(m_arpTreeEntries[idxUnit]);
        if( pUnit != nullptr ) {
            iUnitsCount++;
            if( pUnit->name() == m_strSIUnitName && pUnit->symbol() == m_strSIUnitSymbol ) {
                m_pPhysUnitSI = pUnit;
                break;
            }
        }
    }
    if( iUnitsCount== 0 ) {
        SErrResultInfo errResultInfo(
            NameSpace(), ClassName(), name(), "initialize",
            EResultListIsEmpty, EResultSeverityError,
            "Physical size " + keyInTree() + " does not have any physical units.");
        if( CErrLog::GetInstance() != nullptr ) {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
        else {
            throw CException(__FILE__, __LINE__, errResultInfo);
        }
    }
    if( m_pPhysUnitSI == nullptr )
    {
        SErrResultInfo errResultInfo(
            NameSpace(), ClassName(), name(), "initialize",
            EResultSIUnitNotDefined, EResultSeverityError,
            "SI Unit of " + keyInTree() + " not defined.");
        if( CErrLog::GetInstance() != nullptr ) {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
        else {
            throw CException(__FILE__, __LINE__, errResultInfo);
        }
    }

    // Set the SI unit for all units of this physical size
    //----------------------------------------------------

    for( int idxUnit = 0; idxUnit < m_arpTreeEntries.size(); idxUnit++ )
    {
        CUnitsTreeEntryPhysUnit* pUnit = dynamic_cast<CUnitsTreeEntryPhysUnit*>(m_arpTreeEntries[idxUnit]);
        if( pUnit != nullptr ) {
            pUnit->m_pPhysUnitSI = m_pPhysUnitSI;
            pUnit->m_fctConvertFromSIUnit.m_pPhysUnitSrc = m_pPhysUnitSI;
            pUnit->m_fctConvertIntoSIUnit.m_pPhysUnitDst = m_pPhysUnitSI;
        }
    }
} // findAndSetSIUnit

//------------------------------------------------------------------------------
/*! @brief Create chained list through NextLower and NextHigher units.
*/
void CUnitsTreeEntryGrpPhysUnits::createFindBestUnitChainedList()
//------------------------------------------------------------------------------
{
    CUnitsTreeEntryPhysUnit* pPhysUnitCurr = nullptr;
    CUnitsTreeEntryPhysUnit* pPhysUnitNext = nullptr;
    int idxUnit = 0;
    for( CUnitsTreeEntryPhysUnit* pPhysUnitPrev = nullptr; idxUnit < m_arpTreeEntries.size(); idxUnit++ ) {
        CUnitsTreeEntryPhysUnit* pUnit = dynamic_cast<CUnitsTreeEntryPhysUnit*>(m_arpTreeEntries[idxUnit]);
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
            pPhysUnitNext = dynamic_cast<CUnitsTreeEntryPhysUnit*>(m_arpTreeEntries[idxUnit+1]);
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
        CUnitsTreeEntryPhysUnit* pUnit = dynamic_cast<CUnitsTreeEntryPhysUnit*>(m_arpTreeEntries[idxUnit]);
        if( !pUnit->m_bIsLogarithmic )
        {
            throw ZS::System::CException(
                __FILE__, __LINE__, EResultInvalidPhysSize,
                "Unexpected order of physical units. "
                "Logarithmic units must be added at the end of the list." );
        }
    }
} // createFindBestUnitChainedList
