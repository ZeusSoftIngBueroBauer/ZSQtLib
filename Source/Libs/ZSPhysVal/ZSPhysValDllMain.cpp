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

#include <QtCore/qglobal.h>

#include "ZSPhysVal/ZSPhysValDllMain.h"
#include "ZSPhysVal/ZSPhysSize.h"
#include "ZSPhysVal/ZSPhysUnitsPool.h"
#include "ZSPhysVal/ZSPhysUnitsRatio.h"
#include "ZSPhysVal/ZSPhysUnitsDataQuantity.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysVersion.h"

#ifdef _WINDOWS
// As "min" will be defined as a macro with two arguments and qdatetime uses "min"
// as a function with no arguments "windows.h" must be included after qdatetime
// (which is included by ZSSysTime (which again is included by ZSSysAux)).
#include <windows.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;


/*******************************************************************************
Static variables
*******************************************************************************/

static CUnitsPool* s_pUnitsPool = nullptr;

static const SExponent c_arExponent[EExponentCount] = {
    //       { fFactor,        iExponent,        strExponent, strPrefix        }
    //---------------------------------------------------------------------------
    /*  0 */ { c_fFactorYokto, c_iExponentYokto, c_strYokto,  c_strPrefixYokto },
    /*  1 */ { c_fFactorZepto, c_iExponentZepto, c_strZepto,  c_strPrefixZepto },
    /*  2 */ { c_fFactorAtto,  c_iExponentAtto,  c_strAtto,   c_strPrefixAtto  },
    /*  3 */ { c_fFactorFemto, c_iExponentFemto, c_strFemto,  c_strPrefixFemto },
    /*  4 */ { c_fFactorPico,  c_iExponentPico,  c_strPico,   c_strPrefixPico  },
    /*  5 */ { c_fFactorNano,  c_iExponentNano,  c_strNano,   c_strPrefixNano  },
    /*  6 */ { c_fFactorMicro, c_iExponentMicro, c_strMicro,  c_strPrefixMicro },
    /*  7 */ { c_fFactorMilli, c_iExponentMilli, c_strMilli,  c_strPrefixMilli },
    /*  8 */ { c_fFactorCenti, c_iExponentCenti, c_strCenti,  c_strPrefixCenti },
    /*  9 */ { c_fFactorDezi,  c_iExponentDezi,  c_strDezi,   c_strPrefixDezi  },
    //---------------------------------------------------------------------------
    /* 10 */ { c_fFactor1,     c_iExponent0,     c_str0,      c_strPrefix0     },
    //---------------------------------------------------------------------------
    /* 11 */ { c_fFactorDeka , c_iExponentDeka,  c_strDeka,   c_strPrefixDeka  },
    /* 12 */ { c_fFactorHekto, c_iExponentHekto, c_strHekto,  c_strPrefixHekto },
    /* 13 */ { c_fFactorKilo,  c_iExponentKilo,  c_strKilo,   c_strPrefixKilo  },
    /* 14 */ { c_fFactorMega,  c_iExponentMega,  c_strMega,   c_strPrefixMega  },
    /* 15 */ { c_fFactorGiga,  c_iExponentGiga,  c_strGiga,   c_strPrefixGiga  },
    /* 16 */ { c_fFactorTera,  c_iExponentTera,  c_strTera,   c_strPrefixTera  },
    /* 17 */ { c_fFactorPeta,  c_iExponentPeta,  c_strPeta,   c_strPrefixPeta  },
    /* 18 */ { c_fFactorExa,   c_iExponentExa,   c_strExa,    c_strPrefixExa   },
    /* 19 */ { c_fFactorZetta, c_iExponentZetta, c_strZetta,  c_strPrefixZetta },
    /* 20 */ { c_fFactorYotta, c_iExponentYotta, c_strYotta,  c_strPrefixYotta }
};


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

/* InvalidValue
==============================================================================*/

static QString s_strInvalidValue = "---";

//------------------------------------------------------------------------------
QString ZS::PhysVal::invalidValueString()
//------------------------------------------------------------------------------
{
    return s_strInvalidValue;
}

//------------------------------------------------------------------------------
void ZS::PhysVal::setInvalidValueString( const QString& i_str )
//------------------------------------------------------------------------------
{
    s_strInvalidValue = i_str;
}

/* BitField EFormatResult
==============================================================================*/

//------------------------------------------------------------------------------
bool ZS::PhysVal::isErrorFormatResult( EFormatResult i_formatResult )
//------------------------------------------------------------------------------
{
    bool bIs = false;

    if( i_formatResult & EFormatResultError )
    {
        bIs = true;
    }
    return bIs;

} // isError

//------------------------------------------------------------------------------
QString ZS::PhysVal::formatResult2Str( int i_formatResult )
//------------------------------------------------------------------------------
{
    QString strFormatResult = "?";

    switch( i_formatResult )
    {
        case EFormatResultOk:
        {
            strFormatResult = "Ok";
            break;
        }
        case EFormatResultAccuracyOverflow:
        {
            strFormatResult = "AccuracyOverflow";
            break;
        }
        case EFormatResultAccuracyUnderflow:
        {
            strFormatResult = "AccuracyUnderflow";
            break;
        }
        case EFormatResultValueOverflow:
        {
            strFormatResult = "ValueOverflow";
            break;
        }
        case EFormatResultValueUnderflow:
        {
            strFormatResult = "ValueUnderflow";
            break;
        }
        case EFormatResultUnitConversionFailed:
        {
            strFormatResult = "UnitConversionFailed";
            break;
        }
        default:
        {
            break;
        }
    }
    return strFormatResult;

} // formatResult2Str

/* enum EUnitClassType
==============================================================================*/

const SEnumEntry s_arEnumStrUnitClassTypes[EUnitClassTypeCount] =
{
    /* 0 */ SEnumEntry( EUnitClassTypeRatios,                "Ratios",            "Ratios"                  ),
    /* 1 */ SEnumEntry( EUnitClassTypeDataQuantity,          "DataQuantities",    "Data Quantities"         ),
    /* 2 */ SEnumEntry( EUnitClassTypePhysScienceFields,     "PhysScienceFields", "Physical Science Fields" ),
    /* 3 */ SEnumEntry( EUnitClassTypeUserDefinedQuantities, "UserDefQuantities", "User Defined Quantities" )
};

//------------------------------------------------------------------------------
QString ZS::PhysVal::unitClassType2Str( int i_iType )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str( s_arEnumStrUnitClassTypes, _ZSArrLen(s_arEnumStrUnitClassTypes), i_iType );
}

/* enum EPhysScienceField
==============================================================================*/

const SEnumEntry s_arEnumStrPhysScienceFields[EPhysScienceFieldCount] =
{
    /* 0 */ SEnumEntry( EPhysScienceFieldGeometry,       c_strPhysScienceFieldGeometry       ),
    /* 1 */ SEnumEntry( EPhysScienceFieldKinematics,     c_strPhysScienceFieldKinematics     ),
    /* 2 */ SEnumEntry( EPhysScienceFieldMechanics,      c_strPhysScienceFieldMechanics      ),
    /* 3 */ SEnumEntry( EPhysScienceFieldElectricity,    c_strPhysScienceFieldElectricity    ),
    /* 4 */ SEnumEntry( EPhysScienceFieldThermoDynamics, c_strPhysScienceFieldThermoDynamics ),
    /* 5 */ SEnumEntry( EPhysScienceFieldAtomics,        c_strPhysScienceFieldAtomics        ),
    /* 6 */ SEnumEntry( EPhysScienceFieldNucleonics,     c_strPhysScienceFieldNucleonics     ),
    /* 7 */ SEnumEntry( EPhysScienceFieldPhotometry,     c_strPhysScienceFieldPhotometry     )
};

//------------------------------------------------------------------------------
QString ZS::PhysVal::physScienceField2Str( int i_iField )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str( s_arEnumStrPhysScienceFields, _ZSArrLen(s_arEnumStrPhysScienceFields), i_iField );
}

//------------------------------------------------------------------------------
bool ZS::PhysVal::getPrefixStrFromFactor( double i_fFactor, QString* o_pStrPrefix )
//------------------------------------------------------------------------------
{
    if( o_pStrPrefix == nullptr )
    {
        return false;
    }

    bool             bOk = false;
    const SExponent* pExp;
    int              idxExponent;
    double           fMin = i_fFactor-2.0*DBL_EPSILON;
    double           fMax = i_fFactor+2.0*DBL_EPSILON ;

    for( pExp = c_arExponent, idxExponent = 0; idxExponent < EExponentCount; idxExponent++, pExp++ )
    {
        if( pExp->m_fFactor >= fMin && pExp->m_fFactor <= fMax )
        {
            *o_pStrPrefix = pExp->m_strPrefix;
            bOk = true;
            break;
        }
    }
    return bOk;

} // getPrefixStrFromFactor

//------------------------------------------------------------------------------
bool ZS::PhysVal::getExponentFromFactor( double i_fFactor, int* o_piExponent )
//------------------------------------------------------------------------------
{
    if( o_piExponent == nullptr )
    {
        return false;
    }

    bool             bOk = false;
    const SExponent* pExp;
    int              idxExponent;
    double           fMin = i_fFactor-2.0*DBL_EPSILON;
    double           fMax = i_fFactor+2.0*DBL_EPSILON;

    for( pExp = c_arExponent, idxExponent = 0; idxExponent < EExponentCount; idxExponent++, pExp++ )
    {
        if( pExp->m_fFactor >= fMin && pExp->m_fFactor <= fMax )
        {
            *o_piExponent = pExp->m_iExponent;
            bOk = true;
            break;
        }
    }
    return bOk;

} // getExponentFromFactor

//------------------------------------------------------------------------------
bool ZS::PhysVal::getFactorInverted( double i_fFactor, double* o_pfFactorInverted )
//------------------------------------------------------------------------------
{
    if( o_pfFactorInverted == nullptr )
    {
        return false;
    }

    bool             bOk = false;
    const SExponent* pExp;
    int              idxExponent;
    double           fMin = i_fFactor-2.0*DBL_EPSILON;
    double           fMax = i_fFactor+2.0*DBL_EPSILON;

    for( pExp = c_arExponent, idxExponent = 0; idxExponent < EExponentCount; idxExponent++, pExp++ )
    {
        if( pExp->m_fFactor >= fMin && pExp->m_fFactor <= fMax )
        {
            if( idxExponent < EExponent0 )
            {
                *o_pfFactorInverted = c_arExponent[2*EExponent0-idxExponent].m_fFactor;
            }
            else if( idxExponent > EExponent0 )
            {
                *o_pfFactorInverted = c_arExponent[2*EExponent0-idxExponent].m_fFactor;
            }
            else
            {
                *o_pfFactorInverted = pExp->m_fFactor;
            }
            bOk = true;
            break;
        }
    }
    return bOk;

} // getFactorInverted

//------------------------------------------------------------------------------
double ZS::PhysVal::getFactorFromPrefixStr( const QString& i_strPrefix, bool i_bInverted )
//------------------------------------------------------------------------------
{
    double           fFactor = 1.0;
    const SExponent* pExp;
    int              idxExponent;

    for( pExp = c_arExponent, idxExponent = 0; idxExponent < EExponentCount; idxExponent++, pExp++ )
    {
        if( pExp->m_strPrefix == i_strPrefix )
        {
            if( i_bInverted )
            {
                if( idxExponent < EExponent0 )
                {
                    fFactor = c_arExponent[2*EExponent0-idxExponent].m_fFactor;
                }
                else if( idxExponent > EExponent0 )
                {
                    fFactor = c_arExponent[2*EExponent0-idxExponent].m_fFactor;
                }
                else
                {
                    fFactor = pExp->m_fFactor;
                }
            }
            else
            {
                fFactor = pExp->m_fFactor;
            }
            break;
        }
    }
    return fFactor;

} // getFactorFromPrefixStr

//------------------------------------------------------------------------------
int ZS::PhysVal::getExponentFromPrefixStr( const QString& i_strPrefix )
//------------------------------------------------------------------------------
{
    int              iExponent = 0;
    const SExponent* pExp;
    int              idxExponent;

    for( pExp = c_arExponent, idxExponent = 0; idxExponent < EExponentCount; idxExponent++, pExp++ )
    {
        if( pExp->m_strPrefix == i_strPrefix )
        {
            iExponent = pExp->m_iExponent;
            break;
        }
    }
    return iExponent;

} // getExponentFromPrefixStr

//------------------------------------------------------------------------------
QString ZS::PhysVal::getExponentStrFromPrefixStr( const QString& i_strPrefix )
//------------------------------------------------------------------------------
{
    QString          strExponent = "?";
    const SExponent* pExp;
    int              idxExponent;

    for( pExp = c_arExponent, idxExponent = 0; idxExponent < EExponentCount; idxExponent++, pExp++ )
    {
        if( pExp->m_strPrefix == i_strPrefix )
        {
            strExponent = pExp->m_strExponent;
            break;
        }
    }
    return strExponent;

} // getExponentStrFromPrefixStr


/*******************************************************************************
Initialization of the DLL
*******************************************************************************/

//------------------------------------------------------------------------------
void ZS::PhysVal::createUnitsPool()
//------------------------------------------------------------------------------
{
    if( s_pUnitsPool != nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSingletonClassAlreadyInstantiated );
    }
    s_pUnitsPool = new CUnitsPool();

} // createUnitsPool

//------------------------------------------------------------------------------
void ZS::PhysVal::deleteUnitsPool()
//------------------------------------------------------------------------------
{
    try
    {
        delete s_pUnitsPool;
    }
    catch(...)
    {
    }
    s_pUnitsPool = nullptr;

} // removeDll


/*******************************************************************************
Auxiliary methods
*******************************************************************************/

//------------------------------------------------------------------------------
CUnitsPool* ZS::PhysVal::getUnitsPool()
//------------------------------------------------------------------------------
{
    return s_pUnitsPool;
}

//------------------------------------------------------------------------------
CUnitGrpRatio* ZS::PhysVal::Ratio()
//------------------------------------------------------------------------------
{
    CUnitGrpRatio* pRatio = dynamic_cast<CUnitGrpRatio*>(CUnitsPool::GetUnitClassTypeGroup(EUnitClassTypeRatios));
    return pRatio;
}

//------------------------------------------------------------------------------
CUnitGrpDataQuantity* ZS::PhysVal::DataQuantity()
//------------------------------------------------------------------------------
{
    CUnitGrpDataQuantity* pDataQuantity = dynamic_cast<CUnitGrpDataQuantity*>(CUnitsPool::GetUnitClassTypeGroup(EUnitClassTypeDataQuantity));
    return pDataQuantity;
}


/*******************************************************************************
Public methods
*******************************************************************************/

//------------------------------------------------------------------------------
QString ZS::PhysVal::getUnitName( const CUnit* i_pUnit, bool i_bInsertParentNames )
//------------------------------------------------------------------------------
{
    QString strName;

    if( i_pUnit == nullptr )
    {
        strName = "NoUnit";
    }
    else
    {
        strName = i_pUnit->getName(i_bInsertParentNames);
    }
    return strName;

} // getUnitName

//------------------------------------------------------------------------------
QString ZS::PhysVal::getUnitGroupName( const CUnit* i_pUnit, bool i_bInsertParentNames )
//------------------------------------------------------------------------------
{
    QString strName;

    if( i_pUnit == nullptr )
    {
        strName = "NoUnit";
    }
    else
    {
        strName = i_pUnit->getUnitGroup()->getName(i_bInsertParentNames);
    }
    return strName;

} // getUnitGroupName

//------------------------------------------------------------------------------
QString ZS::PhysVal::getUnitGroupName( const CUnitGrp* i_pUnitGrp, bool i_bInsertParentNames )
//------------------------------------------------------------------------------
{
    QString strName;

    if( i_pUnitGrp == nullptr )
    {
        strName = "NoUnit";
    }
    else
    {
        strName = i_pUnitGrp->getName(i_bInsertParentNames);
    }
    return strName;

} // getUnitGroupName


/*******************************************************************************
Libraries depending on build configuration and used Qt version
*******************************************************************************/

#ifdef _WINDOWS

#ifdef USE_PRAGMA_COMMENT_LIB_INCLUDE_IN_MAIN_MODULES

#pragma message(__FILE__ ": Linking against = " QTCORELIB)
#pragma comment(lib, QTCORELIB)
#pragma message(__FILE__ ": Linking against = " QTNETWORKLIB)
#pragma comment(lib, QTNETWORKLIB)
#pragma message(__FILE__ ": Linking against = " QTXMLLIB)
#pragma comment(lib, QTXMLLIB)

#pragma message(__FILE__ ": Linking against = " ZSSYSLIB)
#pragma comment(lib, ZSSYSLIB)

#endif // #ifdef USE_PRAGMA_COMMENT_LIB_INCLUDE_IN_MAIN_MODULES

#endif // #ifdef _WINDOWS


/*******************************************************************************
Entry point for the DLL application.
*******************************************************************************/

#ifdef _WINDOWS
//------------------------------------------------------------------------------
BOOL APIENTRY DllMain(
    HANDLE /*i_hModule*/,
    DWORD  i_ul_reason_for_call,
    LPVOID /*i_lpReserved*/ )
//------------------------------------------------------------------------------
{
    switch( i_ul_reason_for_call )
    {
        case DLL_PROCESS_ATTACH:
        {
            break;
        }
        case DLL_THREAD_ATTACH:
        {
            break;
        }
        case DLL_THREAD_DETACH:
        {
            break;
        }
        case DLL_PROCESS_DETACH:
        {
            break;
        }
        default:
        {
            break;
        }
    }
    return TRUE;

} // DllMain
#endif
