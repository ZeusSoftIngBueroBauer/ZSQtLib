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

#include "ZSSys/ZSSysEnumTemplate.h"

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

// The static arrays "CEnum<>::s_arEnumEntries" are defined in the cpp file.
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4661 )
#elif defined __linux__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
#pragma GCC diagnostic pop
#endif


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

namespace ZS
{
namespace PhysVal
{
class CUnit;

ZSPHYSVALDLL_API QString invalidValueString();      // "---" as default
ZSPHYSVALDLL_API void setInvalidValueString( const QString& i_str );

const QString c_strSymbolDegree = QString::fromLatin1("°");     // "\u00b0"

// Please note that the bit number number 8 (0x80) indicates an error.
// If this bit is not set the result is either Ok or is just a warning.
typedef quint8 TFormatResult;
namespace FormatResult {
    const TFormatResult Ok                   = 0x00;
    const TFormatResult AccuracyOverflow     = 0x01;
    const TFormatResult AccuracyUnderflow    = 0x02;
    const TFormatResult Error                = 0x80;
    const TFormatResult ValueOverflow        = 0x81;
    const TFormatResult ValueUnderflow       = 0x82;
    const TFormatResult UnitConversionFailed = 0x83;
    ZSPHYSVALDLL_API bool isErrorResult( TFormatResult i_formatResult );
    ZSPHYSVALDLL_API QString result2Str( TFormatResult i_formatResult );
}

enum class EUnitClassType
{
    Undefined = 0,
    Ratio     = 1,
    Quantity  = 2,
    PhysSize  = 3
};
template class ZSPHYSVALDLL_API ZS::System::CEnum<EUnitClassType>;
typedef ZS::System::CEnum<EUnitClassType> CEnumUnitClassType;

enum class EPhysScienceField {
    Undefined      = 0,
    Geometry       = 1,
    Kinematics     = 2,
    Mechanics      = 3,
    Electricity    = 4,
    ThermoDynamics = 5,
    Atomics        = 6,
    Nucleonics     = 7,
    Photometry     = 8
};
template class ZSPHYSVALDLL_API ZS::System::CEnum<EPhysScienceField>;
typedef ZS::System::CEnum<EPhysScienceField> CEnumPhysScienceField;

enum class EUnitFind {
    None = 0,
    Best = 1
};
template class ZSPHYSVALDLL_API ZS::System::CEnum<EUnitFind>;
typedef ZS::System::CEnum<EUnitFind> CEnumUnitFind;

enum class EResType {
    Undefined  = 0,
    Resolution = 1,
    Accuracy   = 2
};

template class ZSPHYSVALDLL_API ZS::System::CEnum<EResType>;
typedef ZS::System::CEnum<EResType> CEnumResType;

typedef quint16 TPhysValSubStr;
namespace PhysValSubStr {
    static TPhysValSubStr None       = 0x0000;
    static TPhysValSubStr Val        = 0x0001;
    static TPhysValSubStr UnitGrp    = 0x0002;
    static TPhysValSubStr UnitPrefix = 0x0004;
    static TPhysValSubStr UnitSymbol = 0x0008;
    static TPhysValSubStr UnitName   = 0x0010;
    static TPhysValSubStr UnitMask   = 0x001e;
}

enum ESubStr {
    ESubStrVal         = 0,
    ESubStrValUnitGrp  = 1,
    ESubStrValUnit     = 2,
    ESubStrRes         = 3,
    ESubStrResUnitGrp  = 4,
    ESubStrResUnit     = 5,
    ESubStrCount
};

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

const QChar   c_chSymbolPlusMinus(0x00B1);
const QString c_strSymbolPlusMinus(c_chSymbolPlusMinus);

static const char* c_strYokto  = "Yokto";
static const char* c_strZepto  = "Zepto";
static const char* c_strAtto   = "Atto";
static const char* c_strFemto  = "Femto";
static const char* c_strPico   = "Pico";
static const char* c_strNano   = "Nano";
static const char* c_strMicro  = "Micro";
static const char* c_strMilli  = "Milli";
static const char* c_strCenti  = "Centi";
static const char* c_strDezi   = "Dezi";
static const char* c_str0      = "";
static const char* c_strDeka   = "Deka";
static const char* c_strHekto  = "Hekto";
static const char* c_strKilo   = "Kilo";
static const char* c_strMega   = "Mega";
static const char* c_strGiga   = "Giga";
static const char* c_strTera   = "Tera";
static const char* c_strPeta   = "Peta";
static const char* c_strExa    = "Exa";
static const char* c_strZetta  = "Zetta";
static const char* c_strYotta  = "Yotta";

static const char* c_strPrefixYokto  = "y";
static const char* c_strPrefixZepto  = "z";
static const char* c_strPrefixAtto   = "a";
static const char* c_strPrefixFemto  = "f";
static const char* c_strPrefixPico   = "p";
static const char* c_strPrefixNano   = "n";
static const char* c_strPrefixMicro  = "u"; // QString::fromLatin1("µ"); // "\u00b5"
static const char* c_strPrefixMilli  = "m";
static const char* c_strPrefixCenti  = "c";
static const char* c_strPrefixDezi   = "d";
static const char* c_strPrefix0      = "";
static const char* c_strPrefixDeka   = "da";
static const char* c_strPrefixHekto  = "h";
static const char* c_strPrefixKilo   = "k";
static const char* c_strPrefixMega   = "M";
static const char* c_strPrefixGiga   = "G";
static const char* c_strPrefixTera   = "T";
static const char* c_strPrefixPeta   = "P";
static const char* c_strPrefixExa    = "E";
static const char* c_strPrefixZetta  = "Z";
static const char* c_strPrefixYotta  = "Y";

enum EExponent
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
};

struct SExponent
{
    double  m_fFactor;
    int     m_iExponent;
    QString m_strExponent;
    QString m_strPrefix;
};

ZSPHYSVALDLL_API QString getPrefixStrFromFactor( double i_fFactor, bool* o_pbOk = nullptr );
ZSPHYSVALDLL_API int getExponentFromFactor( double i_fFactor, bool* o_pbOk = nullptr );
ZSPHYSVALDLL_API double getFactorInverted( double i_fFactor, bool* o_pbOk = nullptr );
ZSPHYSVALDLL_API double getFactorFromPrefixStr( const QString& i_strPrefix, bool i_bInverted = false, bool* o_pbOk = nullptr );
ZSPHYSVALDLL_API int getExponentFromPrefixStr( const QString& i_strPrefix, bool* o_pbOk = nullptr );
ZSPHYSVALDLL_API QString getExponentStrFromPrefixStr( const QString& i_strPrefix, bool* o_pbOk = nullptr );
ZSPHYSVALDLL_API QString getFactorPrefixFromSymbol( const QString& i_strSymbol, bool* o_pbOk = nullptr );
ZSPHYSVALDLL_API QString getFactorPrefixFromSymbol( const QString& i_strSymbol, double i_fFactor, bool* o_pbOk = nullptr );


/*******************************************************************************
Auxiliary methods
*******************************************************************************/

ZSPHYSVALDLL_API void insertDelimiter(
    int            i_iDigitsPerDigitGroup,
    const QString& i_strDelimiter,
    QString*       io_pstrValue,
    int            i_iDigitsLeading,
    int            i_iDigitsTrailing );

ZSPHYSVALDLL_API void removeTrailingZeros(
    QString* io_pstrValue,
    int      i_iDigitsTrailingMin = 1,
    QChar    i_charDecPoint = '.' );

ZSPHYSVALDLL_API void removeLeadingZerosFromExponent( QString* io_pstrValue );

ZSPHYSVALDLL_API TFormatResult formatString(
    const QString& i_strValue,
    int*           o_piDigitsLeading,
    int*           o_piDigitsTrailing,
    int*           o_piDigitsExponent );

ZSPHYSVALDLL_API TFormatResult formatValue(
    double       i_fVal,
    const CUnit& i_unitVal,
    double       i_fRes,
    const CUnit* i_pUnitRes,
    EResType     i_resType,
    int          i_iDigitsMantissa,
    int          i_iDigitsExponent,
    bool         i_bUseEngineeringFormat = false,
    double*      o_pfVal = nullptr,
    QString*     o_pstrVal = nullptr,
    CUnit*       o_pUnitVal = nullptr,
    int*         o_piDigitsLeading = nullptr,
    int*         o_piDigitsTrailing = nullptr,
    int*         o_piDigitsExponent = nullptr );

ZSPHYSVALDLL_API TFormatResult formatValue(
    double         i_fVal,
    const CUnit&   i_unitVal,
    double         i_fRes,
    const CUnit*   i_pUnitRes,
    EResType       i_resType,
    int            i_iDigitsMantissa,
    int            i_iDigitsExponent,
    int            i_iDigitsPerDigitGroup,
    const QString* i_pstrDigitsGroupDelimiter,
    const QString* i_pstrDecimalPoint = nullptr,
    bool           i_bUseEngineeringFormat = false,
    double*        o_pfVal = nullptr,
    QString*       o_pstrVal = nullptr,
    CUnit*         o_pUnitVal = nullptr,
    int*           o_piDigitsLeading = nullptr,
    int*           o_piDigitsTrailing = nullptr,
    int*           o_piDigitsExponent = nullptr );

ZSPHYSVALDLL_API TFormatResult formatValue(
    double       i_fVal,
    const CUnit& i_unitVal,
    int          i_iDigitsMantissaMax,
    bool         i_bDigitsAccuracyLimitsMantissa,
    int          i_iDigitsAccuracy,
    int          i_iDigitsExponent,
    bool         i_bUseEngineeringFormat = false,
    double*      o_pfVal = nullptr,
    QString*     o_pstrVal = nullptr,
    CUnit*       o_pUnitVal = nullptr,
    int*         o_piDigitsLeading = nullptr,
    int*         o_piDigitsTrailing = nullptr,
    int*         o_piDigitsExponent = nullptr );

ZSPHYSVALDLL_API TFormatResult formatValue(
    double         i_fVal,
    const CUnit&   i_unitVal,
    int            i_iDigitsMantissaMax,
    bool           i_bDigitsAccuracyLimitsMantissa,
    int            i_iDigitsAccuracy,
    int            i_iDigitsExponent,
    int            i_iDigitsPerDigitGroup,
    const QString* i_pstrDigitsGroupDelimiter,
    const QString* i_pstrDecimalPoint = nullptr,
    bool           i_bUseEngineeringFormat = false,
    double*        o_pfVal = nullptr,
    QString*       o_pstrVal = nullptr,
    CUnit*         o_pUnitVal = nullptr,
    int*           o_piDigitsLeading = nullptr,
    int*           o_piDigitsTrailing = nullptr,
    int*           o_piDigitsExponent = nullptr );

ZSPHYSVALDLL_API TFormatResult parseValStr(
    const QString& i_strVal,
    bool*          o_pbValOk,
    double*        o_pfVal,
    CUnit*         o_pUnitVal,
    bool*          o_pbResOk,
    double*        o_pfResVal,
    CUnit*         o_pUnitRes );

ZSPHYSVALDLL_API TFormatResult getSubStrings( const QString& i_strVal, QString* io_arSubStr/*[ESubStrCount]*/ );

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_DllMain_h
