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
#include "ZSPhysVal/ZSPhysUnitsPool.h"
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
CPhysSize::CPhysSize(
    EPhysScienceField i_scienceField,
    const QString&    i_strName,
    const QString&    i_strSIUnitName,
    const QString&    i_strSIUnitSymbol,
    const QString&    i_strFormulaSymbol,
    bool              i_bIsPowerRelated ) :
//------------------------------------------------------------------------------
    CUnitGrp(
        /* type             */ EUnitClassTypePhysScienceFields,
        /* strName          */ i_strName,
        /* strKey           */ physScienceField2Str(i_scienceField) + CUnitsPool::GetNameSeparator() + i_strName,
        /* bIsNameSpaceNode */ false,
        /* pUnitGrpParent   */ CUnitsPool::GetPhysScienceFieldUnitGroup(i_scienceField) ),
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

    CUnit*       pUnit;
    CPhysUnit*   pPhysUnitPrev = nullptr;
    CPhysUnit*   pPhysUnitCurr = nullptr;
    CPhysUnit*   pPhysUnitNext = nullptr;
    CPhysUnit*   pPhysUnitDst = nullptr;
    CFctConvert* pFctConvertIntoSIUnitSrc = nullptr;
    CFctConvert* pFctConvertFromSIUnitDst = nullptr;
    double       fMSrcInto;
    double       fTSrcInto;
    double       fMDstFrom;
    double       fTDstFrom;
    int          idxUnit;
    int          idxUnitDst;

    if( m_vecpUnits.size() == 0 )
    {
        QString strAddErrInfo = "Physical size does not have any physical units.";
        throw CException( __FILE__, __LINE__, EResultInvalidPhysSize, strAddErrInfo );
    }

    // First step: set the row index of the physical units
    //--------------------------------------------------------------------------

    for( idxUnit = 0; idxUnit < m_vecpUnits.size(); idxUnit++ )
    {
        pUnit = m_vecpUnits[idxUnit];

        pPhysUnitCurr = dynamic_cast<CPhysUnit*>(pUnit);

        if( pPhysUnitCurr == nullptr )
        {
            QString strAddErrInfo = pUnit->getName(true) + " is not a physical unit.";
            throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidPhysUnit, strAddErrInfo );
        }

        pPhysUnitCurr->m_iPhysSizeRowIdx = idxUnit;
    }

    // Second step: find the SI unit of this physical size
    //--------------------------------------------------------------------------

    for( idxUnit = 0; idxUnit < m_vecpUnits.size(); idxUnit++ )
    {
        pUnit = m_vecpUnits[idxUnit];

        pPhysUnitCurr = dynamic_cast<CPhysUnit*>(pUnit);

        if( pPhysUnitCurr->getName() == m_strSIUnitName && pPhysUnitCurr->getSymbol() == m_strSIUnitSymbol )
        {
            m_pPhysUnitSI = pPhysUnitCurr;
            break;
        }
    }

    if( m_pPhysUnitSI == nullptr )
    {
        QString strAddErrInfo = "SI Unit of " + getName(true) + " not defined.";
        throw ZS::System::CException( __FILE__, __LINE__, EResultSIUnitNotDefined, strAddErrInfo );
    }

    // Third step: set the SI unit for all units of this physical size
    //--------------------------------------------------------------------------

    for( idxUnit = 0; idxUnit < m_vecpUnits.size(); idxUnit++ )
    {
        pUnit = m_vecpUnits[idxUnit];

        pPhysUnitCurr = dynamic_cast<CPhysUnit*>(pUnit);

        pPhysUnitCurr->m_pPhysUnitSI = m_pPhysUnitSI;

        pPhysUnitCurr->m_fctConvertFromSIUnit.m_pPhysUnitSrc = m_pPhysUnitSI;
        pPhysUnitCurr->m_fctConvertFromSIUnit.buildFctConvertName();

        pPhysUnitCurr->m_fctConvertIntoSIUnit.m_pPhysUnitDst = m_pPhysUnitSI;
        pPhysUnitCurr->m_fctConvertIntoSIUnit.buildFctConvertName();
    }

    // Fourth step: create chained list through NextLower and NextHigher units
    //--------------------------------------------------------------------------

    if( i_bCreateFindBestChainedList )
    {
        for( pPhysUnitPrev = nullptr, idxUnit = 0; idxUnit < m_vecpUnits.size(); idxUnit++ )
        {
            pUnit = m_vecpUnits[idxUnit];

            if( idxUnit == 0 )
            {
                pPhysUnitCurr = dynamic_cast<CPhysUnit*>(pUnit);
            }
            else
            {
                pPhysUnitCurr = pPhysUnitNext;
            }
            if( pPhysUnitCurr->m_bIsLogarithmic )
            {
                break;
            }
            if( idxUnit < m_vecpUnits.size()-1 )
            {
                pPhysUnitNext = dynamic_cast<CPhysUnit*>(m_vecpUnits[idxUnit+1]);
            }
            else
            {
                pPhysUnitNext = nullptr;
            }
            pPhysUnitCurr->m_pNextLower = pPhysUnitPrev;

            if( pPhysUnitNext == nullptr || pPhysUnitNext->m_bIsLogarithmic )
            {
                pPhysUnitCurr->m_pNextHigher = nullptr;
            }
            else
            {
                pPhysUnitCurr->m_pNextHigher = pPhysUnitNext;
            }
            pPhysUnitPrev = pPhysUnitCurr;
        }

        for( ; idxUnit < m_vecpUnits.size(); idxUnit++ )
        {
            pUnit = m_vecpUnits[idxUnit];

            pPhysUnitCurr = dynamic_cast<CPhysUnit*>(pUnit);

            if( !pPhysUnitCurr->m_bIsLogarithmic )
            {
                QString strAddErrInfo = "Unexpected order of physical units. Logarithmic units must be added at the end of the list.";
                throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidPhysSize, strAddErrInfo );
            }
        }
    }

    // Fifth step: for all units of this physical size allocate array for routines
    // to convert units within this physical size (internal conversion routines)
    //--------------------------------------------------------------------------

    for( idxUnit = 0; idxUnit < m_vecpUnits.size(); idxUnit++ )
    {
        pUnit = m_vecpUnits[idxUnit];
        pPhysUnitCurr = dynamic_cast<CPhysUnit*>(pUnit);
        pPhysUnitCurr->m_arFctConvertsInternal.resize(m_vecpUnits.size());
    }

    // Sixth step: for all units of this physical size set the internal conversion routines.
    //--------------------------------------------------------------------------

    for( idxUnit = 0; idxUnit < m_vecpUnits.size(); idxUnit++ )
    {
        pUnit = m_vecpUnits[idxUnit];

        pPhysUnitCurr = dynamic_cast<CPhysUnit*>(pUnit);

        pFctConvertIntoSIUnitSrc = &pPhysUnitCurr->m_fctConvertIntoSIUnit;
        fMSrcInto = pFctConvertIntoSIUnitSrc->m_fM;
        fTSrcInto = pFctConvertIntoSIUnitSrc->m_fT;

        for( idxUnitDst = 0; idxUnitDst < m_vecpUnits.size(); idxUnitDst++ )
        {
            pUnit = m_vecpUnits[idxUnitDst];

            pPhysUnitDst = dynamic_cast<CPhysUnit*>(pUnit);

            pFctConvertFromSIUnitDst = &pPhysUnitDst->m_fctConvertFromSIUnit;
            fMDstFrom = pFctConvertFromSIUnitDst->m_fM;
            fTDstFrom = pFctConvertFromSIUnitDst->m_fT;

            pPhysUnitCurr->m_arFctConvertsInternal[idxUnitDst].m_pPhysUnitSrc = pPhysUnitCurr;
            pPhysUnitCurr->m_arFctConvertsInternal[idxUnitDst].m_pPhysUnitDst = pPhysUnitDst;

            if( pFctConvertIntoSIUnitSrc->m_fctConvertType == EFctConvert_mMULxADDt )
            {
                // Conversion from linear to linear unit:
                if( pFctConvertFromSIUnitDst->m_fctConvertType == EFctConvert_mMULxADDt )
                {
                    pPhysUnitCurr->m_arFctConvertsInternal[idxUnitDst].m_fctConvertType = EFctConvert_mMULxADDt;
                    pPhysUnitCurr->m_arFctConvertsInternal[idxUnitDst].m_fM = fMDstFrom*fMSrcInto;
                    pPhysUnitCurr->m_arFctConvertsInternal[idxUnitDst].m_fT = fMDstFrom*fTSrcInto + fTDstFrom;
                }
                // Conversion from linear to logarithmic unit:
                else if( pFctConvertFromSIUnitDst->m_fctConvertType == EFctConvert_mLOGxADDt )
                {
                    pPhysUnitCurr->m_arFctConvertsInternal[idxUnitDst].m_fctConvertType = EFctConvert_mLOGxADDt;
                    pPhysUnitCurr->m_arFctConvertsInternal[idxUnitDst].m_fM = fMDstFrom;
                    pPhysUnitCurr->m_arFctConvertsInternal[idxUnitDst].m_fT = fMDstFrom*log10(fMSrcInto) + fTDstFrom;
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
                    pPhysUnitCurr->m_arFctConvertsInternal[idxUnitDst].m_fctConvertType = EFctConvert_EXP__xADDt_DIVm_;
                    pPhysUnitCurr->m_arFctConvertsInternal[idxUnitDst].m_fM = fMSrcInto;
                    pPhysUnitCurr->m_arFctConvertsInternal[idxUnitDst].m_fT = fMSrcInto*log10(fMDstFrom) + fTSrcInto;
                }
                // Conversion from logarithmic to logarithmic unit:
                else if( pFctConvertFromSIUnitDst->m_fctConvertType == EFctConvert_mLOGxADDt )
                {
                    pPhysUnitCurr->m_arFctConvertsInternal[idxUnitDst].m_fctConvertType = EFctConvert_mMULxADDt;
                    pPhysUnitCurr->m_arFctConvertsInternal[idxUnitDst].m_fM = fMDstFrom/fMSrcInto;
                    pPhysUnitCurr->m_arFctConvertsInternal[idxUnitDst].m_fT = (fMDstFrom*fTSrcInto)/fMSrcInto + fTDstFrom;
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
            pPhysUnitCurr->m_arFctConvertsInternal[idxUnitDst].buildFctConvertName();

        } // for( idxUnitDst = 0; idxUnitDst < m_vecpUnits.size(); idxUnitDst++ )
    } // for( idxUnit = 0; idxUnit < m_vecpUnits.size(); idxUnit++ )

    // Last step: Set flag that this physical size has been initialized
    //--------------------------------------------------------------------------

    for( idxUnit = 0; idxUnit < m_vecpUnits.size(); idxUnit++ )
    {
        pUnit = m_vecpUnits[idxUnit];
        pPhysUnitCurr = dynamic_cast<CPhysUnit*>(pUnit);
        pPhysUnitCurr->m_bInitialized = true;
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

    int        idxChildSrc;
    int        idxChildDst;
    CPhysUnit* pPhysUnitSrc;
    CPhysUnit* pPhysUnitDst;

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

    for( idxChildSrc = 0; idxChildSrc < getPhysUnitCount(); idxChildSrc++ )
    {
        pPhysUnitSrc = getPhysUnit(idxChildSrc);

        if( pPhysUnitSrc != nullptr && pPhysUnitSrc->isLogarithmic() )
        {
            for( idxChildDst = 0; idxChildDst < i_pPhysSizeDst->getPhysUnitCount(); idxChildDst++ )
            {
                pPhysUnitDst = i_pPhysSizeDst->getPhysUnit(idxChildDst);

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
    return dynamic_cast<CPhysUnit*>(getUnit(i_idx));
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
double CPhysSize::getRefVal( CPhysUnit* /*i_pPhysUnitRef*/ ) const
//------------------------------------------------------------------------------
{
    QString strAddErrInfo = "No reference value defined for physical size " + getName(true);
    throw CException( __FILE__, __LINE__, EResultInvalidRefVal, strAddErrInfo );
}
