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

#ifndef ZSPhysVal_DllMain_h
#define ZSPhysVal_DllMain_h

#include <QtCore/qstring.h>

// generic export defines
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #define __API_EXPORT __declspec(dllexport)
    #define __API_IMPORT __declspec(dllimport)
#elif defined(__linux__)
    #if (__GNUC__ >= 4)
        #define __API_EXPORT __attribute((visibility("default")))
    #else
        #define __API_EXPORT
    #endif
    #define  __API_IMPORT
#else
    #error Missing export macro
#endif

// specific export defines
#ifdef ZSPHYSVALDLL_EXPORTS
    #define ZSPHYSVALDLL_API __API_EXPORT
#else
    #define ZSPHYSVALDLL_API __API_IMPORT
#endif


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

namespace ZS
{
namespace PhysVal
{
class CUnit;
class CUnitGrp;
class CUnitsPool;
class CPhysUnit;
class CPhysSize;
class CUnitGrpRatio;
class CUnitGrpDataQuantity;

ZSPHYSVALDLL_API QString invalidValueString();      // "---" as default
ZSPHYSVALDLL_API void setInvalidValueString( const QString& i_str );

const QString c_strSymbolDegree = QString::fromLatin1("°");     // "\u00b0"

typedef enum
{   // Please note that the bit number number 8 (0x80) indicates an error.
    // If this bit is not set the result is either Ok or is just a warning.
    EFormatResultOk                   = 0x00,
    EFormatResultAccuracyOverflow     = 0x01,
    EFormatResultAccuracyUnderflow    = 0x02,
    EFormatResultError                = 0x80,
    EFormatResultValueOverflow        = 0x81,
    EFormatResultValueUnderflow       = 0x82,
    EFormatResultUnitConversionFailed = 0x83
}   EFormatResult;

ZSPHYSVALDLL_API bool isErrorFormatResult( EFormatResult i_formatResult );
ZSPHYSVALDLL_API QString formatResult2Str( int i_formatResult );

typedef enum
{
    EUnitClassTypeRatios                = 0,
    EUnitClassTypeDataQuantity          = 1,
    EUnitClassTypePhysScienceFields     = 2,
    EUnitClassTypeUserDefinedQuantities = 3,
    EUnitClassTypeCount,
    EUnitClassTypeUndefined
}   EUnitClassType;

ZSPHYSVALDLL_API QString unitClassType2Str( int i_iType );

const QString c_strPhysScienceFieldGeometry       = "Geometry";
const QString c_strPhysScienceFieldKinematics     = "Kinematics";
const QString c_strPhysScienceFieldMechanics      = "Mechanics";
const QString c_strPhysScienceFieldElectricity    = "Electricity";
const QString c_strPhysScienceFieldThermoDynamics = "ThermoDynamics";
const QString c_strPhysScienceFieldAtomics        = "Atomics";
const QString c_strPhysScienceFieldNucleonics     = "Nucleonics";
const QString c_strPhysScienceFieldPhotometry     = "Photometry";

typedef enum {
    EPhysScienceFieldGeometry       = 0,
    EPhysScienceFieldKinematics     = 1,
    EPhysScienceFieldMechanics      = 2,
    EPhysScienceFieldElectricity    = 3,
    EPhysScienceFieldThermoDynamics = 4,
    EPhysScienceFieldAtomics        = 5,
    EPhysScienceFieldNucleonics     = 6,
    EPhysScienceFieldPhotometry     = 7,
    EPhysScienceFieldCount,
    EPhysScienceFieldUndefined
}   EPhysScienceField;

ZSPHYSVALDLL_API QString physScienceField2Str( int i_iField );

typedef enum {
    EUnitFindNone = 0,
    EUnitFindBest = 1,
}   EUnitFind;

typedef enum {
    EPhysValSubStrNone       = 0x0000,
    EPhysValSubStrVal        = 0x0001,
    EPhysValSubStrUnitGrp    = 0x0002,
    EPhysValSubStrUnitPrefix = 0x0004,
    EPhysValSubStrUnitSymbol = 0x0008,
    EPhysValSubStrUnitName   = 0x0010,
    EPhysValSubStrUnitMask   = 0x001e
}   EPhysValSubStr;

typedef enum
{
    ESubStrVal         = 0,
    ESubStrValUnitGrp  = 1,
    ESubStrValUnit     = 2,
    ESubStrRes         = 3,
    ESubStrResUnitGrp  = 4,
    ESubStrResUnit     = 5,
    ESubStrCount
}   ESubStr;

const double c_fFactorYokto  = 1.0e-24;
const double c_fFactorZepto  = 1.0e-21;
const double c_fFactorAtto   = 1.0e-18;
const double c_fFactorFemto  = 1.0e-15;
const double c_fFactorPico   = 1.0e-12;
const double c_fFactorNano   = 1.0e-9;
const double c_fFactorMicro  = 1.0e-6;
const double c_fFactorMilli  = 1.0e-3;
const double c_fFactorCenti  = 1.0e-2;
const double c_fFactorDezi   = 1.0e-1;
const double c_fFactor1      = 1.0;
const double c_fFactorDeka   = 1.0e1;
const double c_fFactorHekto  = 1.0e2;
const double c_fFactorKilo   = 1.0e3;
const double c_fFactorMega   = 1.0e6;
const double c_fFactorGiga   = 1.0e9;
const double c_fFactorTera   = 1.0e12;
const double c_fFactorPeta   = 1.0e15;
const double c_fFactorExa    = 1.0e18;
const double c_fFactorZetta  = 1.0e21;
const double c_fFactorYotta  = 1.0e24;

const int c_iExponentYokto  = -24;
const int c_iExponentZepto  = -21;
const int c_iExponentAtto   = -18;
const int c_iExponentFemto  = -15;
const int c_iExponentPico   = -12;
const int c_iExponentNano   = -9;
const int c_iExponentMicro  = -6;
const int c_iExponentMilli  = -3;
const int c_iExponentCenti  = -2;
const int c_iExponentDezi   = -1;
const int c_iExponent0      = 0;
const int c_iExponentDeka   = 1;
const int c_iExponentHekto  = 2;
const int c_iExponentKilo   = 3;
const int c_iExponentMega   = 6;
const int c_iExponentGiga   = 9;
const int c_iExponentTera   = 12;
const int c_iExponentPeta   = 15;
const int c_iExponentExa    = 18;
const int c_iExponentZetta  = 21;
const int c_iExponentYotta  = 24;

const QString c_strYokto  = "Yokto";
const QString c_strZepto  = "Zepto";
const QString c_strAtto   = "Atto";
const QString c_strFemto  = "Femto";
const QString c_strPico   = "Pico";
const QString c_strNano   = "Nano";
const QString c_strMicro  = "Micro";
const QString c_strMilli  = "Milli";
const QString c_strCenti  = "Centi";
const QString c_strDezi   = "Dezi";
const QString c_str0      = "";
const QString c_strDeka   = "Deka";
const QString c_strHekto  = "Hekto";
const QString c_strKilo   = "Kilo";
const QString c_strMega   = "Mega";
const QString c_strGiga   = "Giga";
const QString c_strTera   = "Tera";
const QString c_strPeta   = "Peta";
const QString c_strExa    = "Exa";
const QString c_strZetta  = "Zetta";
const QString c_strYotta  = "Yotta";

const QString c_strPrefixYokto  = "y";
const QString c_strPrefixZepto  = "z";
const QString c_strPrefixAtto   = "a";
const QString c_strPrefixFemto  = "f";
const QString c_strPrefixPico   = "p";
const QString c_strPrefixNano   = "n";
const QString c_strPrefixMicro  = QString::fromLatin1("µ"); // "\u00b5"
const QString c_strPrefixMilli  = "m";
const QString c_strPrefixCenti  = "c";
const QString c_strPrefixDezi   = "d";
const QString c_strPrefix0      = "";
const QString c_strPrefixDeka   = "da";
const QString c_strPrefixHekto  = "h";
const QString c_strPrefixKilo   = "k";
const QString c_strPrefixMega   = "M";
const QString c_strPrefixGiga   = "G";
const QString c_strPrefixTera   = "T";
const QString c_strPrefixPeta   = "P";
const QString c_strPrefixExa    = "E";
const QString c_strPrefixZetta  = "Z";
const QString c_strPrefixYotta  = "Y";

typedef enum
{
    EExponentYokto =  0,
    EExponentZepto =  1,
    EExponentAtto  =  2,
    EExponentFemto =  3,
    EExponentPico  =  4,
    EExponentNano  =  5,
    EExponentMicro =  6,
    EExponentMilli =  7,
    EExponentCenti =  8,
    EExponentDezi  =  9,
    EExponent0     = 10,
    EExponentDeka  = 11,
    EExponentHekto = 12,
    EExponentKilo  = 13,
    EExponentMega  = 14,
    EExponentGiga  = 15,
    EExponentTera  = 16,
    EExponentPeta  = 17,
    EExponentExa   = 18,
    EExponentZetta = 19,
    EExponentYotta = 20,
    EExponentCount
}   EExponent;

struct SExponent
{
    double  m_fFactor;
    int     m_iExponent;
    QString m_strExponent;
    QString m_strPrefix;
};

ZSPHYSVALDLL_API bool    getPrefixStrFromFactor( double i_fFactor, QString* o_pStrPrefix );
ZSPHYSVALDLL_API bool    getExponentFromFactor( double i_fFactor, int* o_piExponent );
ZSPHYSVALDLL_API bool    getFactorInverted( double i_fFactor, double* o_pfFactorInverted );
ZSPHYSVALDLL_API double  getFactorFromPrefixStr( const QString& i_strPrefix, bool i_bInverted = false );
ZSPHYSVALDLL_API int     getExponentFromPrefixStr( const QString& i_strPrefix );
ZSPHYSVALDLL_API QString getExponentStrFromPrefixStr( const QString& i_strPrefix );


/*******************************************************************************
Auxiliary methods
*******************************************************************************/

ZSPHYSVALDLL_API QString getUnitName( const CUnit* i_pUnit, bool i_bInsertParentNames = false );
ZSPHYSVALDLL_API QString getUnitGroupName( const CUnit* i_pUnit, bool i_bInsertParentNames = false  );
ZSPHYSVALDLL_API QString getUnitGroupName( const CUnitGrp* i_pUnitGrp, bool i_bInsertParentNames = false  );

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_DllMain_h
