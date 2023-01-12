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
#include "ZSPhysVal/ZSPhysUnit.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysMath.h"

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

/* BitField TFormatResult
==============================================================================*/

//------------------------------------------------------------------------------
bool ZS::PhysVal::FormatResult::isErrorResult( TFormatResult i_formatResult )
//------------------------------------------------------------------------------
{
    return (i_formatResult & FormatResult::Error);
}

//------------------------------------------------------------------------------
QString ZS::PhysVal::FormatResult::result2Str( TFormatResult i_formatResult )
//------------------------------------------------------------------------------
{
    QString str = "?";

    if( i_formatResult == FormatResult::Ok )
    {
    }
    switch( i_formatResult )
    {
        case FormatResult::Ok:
        {
            str = "Ok";
            break;
        }
        case FormatResult::AccuracyOverflow:
        {
            str = "AccuracyOverflow";
            break;
        }
        case FormatResult::AccuracyUnderflow:
        {
            str = "AccuracyUnderflow";
            break;
        }
        case FormatResult::Error:
        {
            str = "ValueOverflow";
            break;
        }
        case FormatResult::ValueOverflow:
        {
            str = "ValueOverflow";
            break;
        }
        case FormatResult::ValueUnderflow:
        {
            str = "ValueUnderflow";
            break;
        }
        case FormatResult::UnitConversionFailed:
        {
            str = "UnitConversionFailed";
            break;
        }
        default:
        {
            break;
        }
    }
    return str;
}

/* enum EUnitClassType
==============================================================================*/

template<> QMutex CEnum<EUnitClassType>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> CEnum<EUnitClassType>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

template<> const QVector<SEnumEntry> CEnum<EUnitClassType>::s_arEnumEntries(
{
    /* 0 */ SEnumEntry( static_cast<int>(EUnitClassType::Undefined), "Undefined" ),
    /* 1 */ SEnumEntry( static_cast<int>(EUnitClassType::Ratio),     "Ratios" ),
    /* 2 */ SEnumEntry( static_cast<int>(EUnitClassType::Quantity),  "Quantities" ),
    /* 3 */ SEnumEntry( static_cast<int>(EUnitClassType::PhysSize),  "PhysSize" )
});

/* enum EPhysScienceField
==============================================================================*/

template<> QMutex CEnum<EPhysScienceField>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> CEnum<EPhysScienceField>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

template<> const QVector<SEnumEntry> CEnum<EPhysScienceField>::s_arEnumEntries(
{
    /* 0 */ SEnumEntry( static_cast<int>(EPhysScienceField::Undefined),      "Undefined" ),
    /* 1 */ SEnumEntry( static_cast<int>(EPhysScienceField::Geometry),       "Geometry" ),
    /* 2 */ SEnumEntry( static_cast<int>(EPhysScienceField::Kinematics),     "Kinematics" ),
    /* 3 */ SEnumEntry( static_cast<int>(EPhysScienceField::Mechanics),      "Mechanics" ),
    /* 4 */ SEnumEntry( static_cast<int>(EPhysScienceField::Electricity),    "Electricity" ),
    /* 5 */ SEnumEntry( static_cast<int>(EPhysScienceField::ThermoDynamics), "ThermoDynamics" ),
    /* 6 */ SEnumEntry( static_cast<int>(EPhysScienceField::Atomics),        "Atomics" ),
    /* 7 */ SEnumEntry( static_cast<int>(EPhysScienceField::Nucleonics),     "Nucleonics" ),
    /* 8 */ SEnumEntry( static_cast<int>(EPhysScienceField::Photometry),     "Photometry" )
});

/* enum EUnitFind
==============================================================================*/

template<> QMutex CEnum<EUnitFind>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> CEnum<EUnitFind>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

template<> const QVector<SEnumEntry> CEnum<EUnitFind>::s_arEnumEntries(
{
    /* 0 */ SEnumEntry( static_cast<int>(EUnitFind::None), "None" ),
    /* 1 */ SEnumEntry( static_cast<int>(EUnitFind::Best), "Best" )
});

/* enum EResType
==============================================================================*/

template<> QMutex CEnum<EResType>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> CEnum<EResType>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

template<> const QVector<SEnumEntry> CEnum<EResType>::s_arEnumEntries(
{
    /* 0 */ SEnumEntry( static_cast<int>(EResType::Undefined), "Undefined" ),
    /* 1 */ SEnumEntry( static_cast<int>(EResType::Resolution), "Resolution" ),
    /* 2 */ SEnumEntry( static_cast<int>(EResType::Accuracy), "Accuracy" )
});

///* enum ESubStr
//==============================================================================*/
//
//template<> QMutex CEnum<EPhysScienceField>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
//template<> QVector<QHash<QString, int>> CEnum<EPhysScienceField>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();
//
//template<> const QVector<SEnumEntry> CEnum<ESubStr>::s_arEnumEntries(
//{
//    /* 0 */ SEnumEntry( static_cast<int>(ESubStrVal),        "Val" ),
//    /* 1 */ SEnumEntry( static_cast<int>(ESubStrValUnitGrp), "ValUnitGrp" ),
//    /* 2 */ SEnumEntry( static_cast<int>(ESubStrValUnit),    "ValUnit" ),
//    /* 3 */ SEnumEntry( static_cast<int>(ESubStrRes),        "Res" ),
//    /* 4 */ SEnumEntry( static_cast<int>(ESubStrResUnitGrp), "ResUnitGrp" ),
//    /* 5 */ SEnumEntry( static_cast<int>(ESubStrResUnit),    "ResUnit" )
//});

/* Exponents
==============================================================================*/

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
Auxiliary methods
*******************************************************************************/

//------------------------------------------------------------------------------
void ZS::PhysVal::insertDelimiter(
    int            i_iDigitsPerDigitGroup,
    const QString& i_strDelimiter,
    QString*       io_pstrValue,
    int            i_iDigitsLeading,
    int            i_iDigitsTrailing )
//------------------------------------------------------------------------------
{
    int iDelimiterCount;
    int idxLeadingDelimiterPos;
    int idxTrailingDelimiterPos;
    int idx;

    if( i_iDigitsPerDigitGroup > 0 )
    {
        idxLeadingDelimiterPos  = i_iDigitsLeading-i_iDigitsPerDigitGroup;
        idxTrailingDelimiterPos = i_iDigitsLeading+i_iDigitsPerDigitGroup+1;

        // There might be a "-" or "+" sign at the beginning of the string ...
        if( i_iDigitsLeading > 0 )
        {
            idx = 0;
            while( ((*io_pstrValue)[idx] < '0' || (*io_pstrValue)[idx] > '9') && (idx < static_cast<int>(io_pstrValue->length())) )
            {
                idx++;
                idxLeadingDelimiterPos++;
                idxTrailingDelimiterPos++;
            }
        }

        // Insert delimiters into leading part:
        if( i_iDigitsLeading > i_iDigitsPerDigitGroup )
        {
            iDelimiterCount = (i_iDigitsLeading-1)/i_iDigitsPerDigitGroup;
            for( idx = 0; idx < iDelimiterCount; idx++ )
            {
                io_pstrValue->insert(idxLeadingDelimiterPos,i_strDelimiter);
                idxTrailingDelimiterPos++;
                idxLeadingDelimiterPos -= i_iDigitsPerDigitGroup;
            }
        }

        // Insert delimiters into trailing part:
        if( i_iDigitsTrailing > i_iDigitsPerDigitGroup )
        {
            iDelimiterCount = (i_iDigitsTrailing-1)/i_iDigitsPerDigitGroup;
            for( idx = 0; idx < iDelimiterCount; idx++ )
            {
                io_pstrValue->insert(idxTrailingDelimiterPos,i_strDelimiter);
                idxTrailingDelimiterPos++;
                idxTrailingDelimiterPos += i_iDigitsPerDigitGroup;
            }
        }
    }

} // insertDelimiter

//------------------------------------------------------------------------------
void ZS::PhysVal::removeTrailingZeros(
    QString* io_pstrValue,
    int      i_iDigitsTrailingMin,
    QChar    i_charDecPoint )
//------------------------------------------------------------------------------
{
    // String:               "123.45600e-12"
    // idxChar:               0123456789012
    // idxCharExp          =  9
    // idxCharDecPt        =  3
    // uStrLen             = 13
    // iMantissaSubStrLen  =  9
    // iExponentSubStrLen  =  4
    // uTrailingSubStrLen  =  5
    // uLeadingSubStrLen   =  3
    // iTrailingZeros      =  2

    QString strValue = *io_pstrValue;
    int     idxChar;
    int     idxCharDecPt;
    int     iMantissaSubStrLen = 0;
    int     iExponentSubStrLen = 0;
    int     iTrailingZeros = 0;

    if( strValue.length() > 0 )
    {
        idxCharDecPt = strValue.indexOf(i_charDecPoint,0,Qt::CaseInsensitive);

        if( idxCharDecPt >= 0 )
        {
            idxChar = strValue.indexOf('e',-1,Qt::CaseInsensitive);
            if( idxChar >= 0 )
            {
                iExponentSubStrLen = strValue.length()-idxChar;
            }
            iMantissaSubStrLen = strValue.length() - iExponentSubStrLen;

            if( iMantissaSubStrLen > 0 )
            {
                for( idxChar = iMantissaSubStrLen-1; idxChar >= 0; idxChar-- )
                {
                    if( strValue[idxChar] == i_charDecPoint )
                    {
                        if( i_iDigitsTrailingMin > 0 && idxChar + i_iDigitsTrailingMin < iMantissaSubStrLen )
                        {
                            idxChar += i_iDigitsTrailingMin;
                        }
                        break;
                    }
                    #if QT_VERSION < 0x050000
                    else if( strValue.toAscii()[idxChar] != '0' )
                    #else
                    else if( strValue.toLatin1()[idxChar] != '0' )
                    #endif
                    {
                        break;
                    }
                    iTrailingZeros++;
                }
                if( idxChar < static_cast<int>(iMantissaSubStrLen-1) )
                {
                    strValue.remove(idxChar+1,iTrailingZeros);
                }
            }
            *io_pstrValue = strValue;
        }
    }

} // removeTrailingZeros

//------------------------------------------------------------------------------
void ZS::PhysVal::removeLeadingZerosFromExponent( QString* io_pstrValue )
//------------------------------------------------------------------------------
{
    // String:               "123.45600e-02"
    // idxChar:               0123456789012
    // idxCharExp          =  9
    // idxCharDecPt        =  3
    // uStrLen             = 13
    // iMantissaSubStrLen  =  9
    // iExponentSubStrLen  =  4
    // uTrailingSubStrLen  =  5
    // uLeadingSubStrLen   =  3
    // iTrailingZeros      =  2

    QString      strValue = *io_pstrValue;
    int          idxChar;
    int          idxCharExp;
    int          idxCharFirstZero = -1;
    int          idxCharFirstDigit = -1;

    if( strValue.length() > 0 )
    {
        idxCharExp = strValue.indexOf("e",0,Qt::CaseInsensitive);

        if( idxCharExp > 0 )
        {
            for( idxChar = idxCharExp+1; idxChar < strValue.length(); idxChar++ )
            {
                if( idxCharFirstZero < 0 && strValue[idxChar] == '0' )
                {
                    idxCharFirstZero = idxChar;
                }
                if( idxCharFirstDigit < 0 && isDigitChar(strValue[idxChar]) && strValue[idxChar] != '0' )
                {
                    idxCharFirstDigit = idxChar;
                }
            }
            if( idxCharFirstZero < idxCharFirstDigit )
            {
                strValue.remove(idxCharFirstZero,idxCharFirstDigit-idxCharFirstZero);
                *io_pstrValue = strValue;
            }
        }
    }

} // removeLeadingZerosFromExponent

//------------------------------------------------------------------------------
TFormatResult ZS::PhysVal::formatString(
    const QString& i_strValue,
    int*           o_piDigitsLeading,
    int*           o_piDigitsTrailing,
    int*           o_piDigitsExponent )
//------------------------------------------------------------------------------
{
    int  iDigitsLeading = 0;
    int  iDigitsTrailing = 0;
    int  iDigitsExponent = 0;
    bool bLeadingSection = true;
    bool bTrailingSection = false;
    bool bExponentSection = false;

    for( int idx = 0; idx < i_strValue.length(); idx++ )
    {
        if( bLeadingSection )
        {
            if( i_strValue.at(idx) == QChar('-') )
            {
            }
            else if( i_strValue.at(idx) == QChar('+') )
            {
            }
            else if( i_strValue.at(idx) >= QChar('0') && i_strValue.at(idx) <= QChar('9') )
            {
                iDigitsLeading++;
            }
            else if( i_strValue.at(idx) == QChar('.') )
            {
                bLeadingSection = false;
                bTrailingSection = true;
            }
        }
        else if( bTrailingSection )
        {
            if( i_strValue.at(idx) >= QChar('0') && i_strValue.at(idx) <= QChar('9') )
            {
                iDigitsTrailing++;
            }
            else if( i_strValue.at(idx) == QChar('e') || i_strValue.at(idx) == QChar('E') )
            {
                bTrailingSection = false;
                bExponentSection = true;
            }
        }
        else if( bExponentSection )
        {
            if( i_strValue.at(idx) == QChar('-') )
            {
            }
            else if( i_strValue.at(idx) == QChar('+') )
            {
            }
            else if( i_strValue.at(idx) >= QChar('0') && i_strValue.at(idx) <= QChar('9') )
            {
                iDigitsExponent++;
            }
        }
    }
    if( o_piDigitsLeading != nullptr )
    {
        *o_piDigitsLeading = iDigitsLeading;
    }
    if( o_piDigitsTrailing != nullptr )
    {
        *o_piDigitsTrailing = iDigitsTrailing;
    }
    if( o_piDigitsExponent != nullptr )
    {
        *o_piDigitsExponent = iDigitsExponent;
    }
    return FormatResult::Ok;

} // formatString

//------------------------------------------------------------------------------
/*! @brief Converts a physical value into its string representation and/or into
    its numerical format according to several format specifications.

    This method may be used to convert a value from a given source unit
    into a unit which best fits according to the resolution and user readability.
    E.g. if the resolution (or better inaccuracy) of a frequency is 100.0 kHz
    and the value is defined as 36726372541.3 Hz, the value would be "best"
    output as 36.7264 GHz.

    @param i_fVal: Range [DOUBLE_MIN .. DOUBLE_MAX]
       Value which got to be formatted.

    @param i_unitVal
       Physical unit of the value to be formatted.

    @param i_fRes:     Range [DOUBLE_MIN .. DOUBLE_MAX]
       Resolution (accuracy) of the value to be formatted.

    @param i_unitRes
       Physical unit of the resolution.
       If invalid it is assumed that the unit of the resolution is
       the same as the unit of the value to be formatted.

    @param i_resType

    @param i_iDigitsMantissa:   Range [-1, 1..N]
        If this value is set by the callee to a value greater than 0 this parameter
        defines the maximum number of mantissa digits (leading and trailing part of
        the string representation) to be used for formatting the value.
        Please note that "DigitsMantissa" determines the maximum allowed number of digits
        used to limit the length of the resulting string.

    @param i_iDigitsExponent:  Range IN  [-1, 0..N]
        If this value is set by the callee to a value greater or equal to 0 this parameter
        defines the maximum number of exponent digits to be used for formatting the value.

    @param i_bUseEngineeringFormat:    Range [false, true]
        Optional parameter which may be set to true if the value should be formatted with
        exactly one valid leading digit.

    @param o_pfVal:      Range IN  [nullptr, valid address]
                               OUT [DOUBLE_MIN .. DOUBLE_MAX]
        Optional parameter which may be used if the callee is interested in the numerical
        representation of the converted value.

    @param o_pstrValue:    Range IN  [nullptr, valid address]
                                 OUT [string representation of the converted value]
        Optional parameter which may be used if the value may be converted into its
        string representation returing the converted value as its string representation.

    @param o_pUnitVal:   Range IN  [nullptr, valid address]
                               OUT [IdEUnitMin ... IdEUnitMax]
        Optional parameter which may be used if the value may be converted into its
        string representation using the best fitting unit returning the physical unit
        into which the value has been converted.
        If this parameter is set to nullptr the value will be converted without finding
        the best fitting unit.

    @param o_piDigitsLeading:      Range IN  [nullptr, valid address]
                                         OUT [1..N] (always at least on leading digit will be used)
        Optional parameter which may be used if the callee is interested in how
        the value has been converted into its string representation returning
        the number of leading digits used to format the value.

    @param o_piDigitsTrailing:     Range IN  [nullptr, valid address]
                                         OUT [0..N]
        Optional parameter which may be used if the callee is interested in how
        the value has been converted into its string representation returning
        the number of trailing digits used to format the value.

    @param o_piDigitsExponent:     Range IN  [nullptr, valid address]
                                         OUT [0..N]
        Optional parameter which may be used if the callee is interested in how
        the value has been converted into its string representation returning
        the number of exponent digits used to format the value.

    @returnExamine the result value to get information if the conversion was successfull.
        FormatResult::Ok .... The value has been successfully converted.
        TFormatResult<..> ... Any other format result as "Ok" indicates that it
                              was not possible to convert the value according to the
                              defined format specifications. Some of the results
                              indicate an error, some others are used as warnings.
                              Please note that the bit number number 8 (0x80) of the
                              format result indicates an error. If this bit is not set
                              the result is either Ok or is just a warning.
            AccuracyOverflow ... Warning: the value has been converted but is indicated
                                            with too many digits.
            AccuracyUnderflow .. Warning: the value has been converted but is indicated
                                            with too less digits.
            UnitConversionFailed Error:   it was not possible to convert the resolution
                                            into the unit of the value
            ValueOverflow ...... Error:   the value has not been converted. An invalid
                                            string "---" will be returned.
            ValueUnderflow ..... Error:   the value has not been converted. But the
                                            value "0" is returned.
*/
TFormatResult ZS::PhysVal::formatValue(
    double       i_fVal,
    const CUnit& i_unitVal,
    double       i_fRes,
    const CUnit* i_pUnitRes,
    EResType     i_resType,
    int          i_iDigitsMantissa,
    int          i_iDigitsExponent,
    bool         i_bUseEngineeringFormat,
    double*      o_pfVal,
    QString*     o_pstrVal,
    CUnit*       o_pUnitVal,
    int*         o_piDigitsLeading,
    int*         o_piDigitsTrailing,
    int*         o_piDigitsExponent )
//------------------------------------------------------------------------------
{
    TFormatResult formatResult = FormatResult::Ok;

    double fVal = i_fVal;
    bool   bDigitsAccuracyLimitsMantissa = false;
    int    iDigitsAccuracy = -1;
    int    iDigitsMantissa = i_iDigitsMantissa;

    // If a resolution should be taken into account ..
    if( i_fRes != 0.0 )
    {
        CUnit  unitVal = i_unitVal;
        CUnit  unitRes = i_pUnitRes == nullptr ? i_unitVal : i_pUnitRes->isValid() ? *i_pUnitRes : i_unitVal;
        int    iValSign = i_fVal < 0 ? -1 : +1;
        double fValAbs  = fabs(i_fVal);
        int    iValExp  = -999;
        bool   bIsLogRes;
        double fResAbs  = fabs(i_fRes);
        int    iResMant = 0;
        int    iResExp  = 0;

        // The number of digits is limited by the resolution.
        bDigitsAccuracyLimitsMantissa = true;

        // If value has got a unit and the unit of the value and the unit of the resolution is different .
        if( i_unitVal.isValid() && i_unitVal != unitRes )
        {
            // If the accuracy is defined as a ratio number ..
            if( unitRes.isRatio() || unitRes.isQuantity() )
            {
                // .. the absolute resolution value need to be calculated in the unit
                // of the value.
                fResAbs = fResAbs * fValAbs * unitRes.factor();
            }
            else
            {
                // Try to convert the value into the unit of resolution ..
                try
                {
                    fValAbs = i_unitVal.convertValue(fValAbs, unitRes);
                    unitVal = unitRes;

                    bIsLogRes = unitRes.isLogarithmic();

                    // If the resolution is of logarithmic type ..
                    if( bIsLogRes )
                    {
                        // .. the resolution is considered to be relative. There will
                        // be no "best unit" the value can be converted to.
                        //pUnitValBest = nullptr;
                    }
                }
                catch( CException& )
                {
                    formatResult = FormatResult::UnitConversionFailed;
                }
            }
        }

        // If it was possible to convert the value into the unit of the resolution ...
        if( formatResult == FormatResult::Ok )
        {
            // .. determine the number of accuracy digits for the mantissa.

            // Determine position of first valid digit of the value ..
            Math::normalize(fValAbs, nullptr, nullptr, &iValExp);

            // Determine position and first valid digit of the resolution ..
            Math::normalize(fResAbs, nullptr, &iResMant, &iResExp);

            if( i_resType == EResType::Accuracy )
            {
                // If the first digit of the accuracy is either '1' or '2' ..
                if( iResMant == 1 || iResMant == 2 )
                {
                    // ... the value will be indicated with two insecure digits.
                    iResExp--;
                }
            }
            else if( i_resType == EResType::Resolution )
            {
                fValAbs = Math::round2Res(fValAbs, fResAbs);
            }

            // If the value was greater than the resolution ...
            if( fValAbs != 0.0 && iValExp > iResExp )
            {
                iDigitsAccuracy = iValExp - iResExp + 1;
            }
            // If the value was 0.0 (or if the resolution was greater or equal than the value) ...
            else
            {
                // ... indicate the value 0 with just one digit in the "best unit" of the resolution.
                iDigitsAccuracy = 1;

                if( o_pUnitVal != nullptr && unitVal.isValid() )
                {
                    unitVal = unitVal.findBestUnit(i_fRes);
                }
            }
        }

        fVal = iValSign * fValAbs;

        // The value need to be converted back to its original unit
        if( unitVal.isValid() && unitVal != i_unitVal )
        {
            // Try to convert the (maybe rounded) value into its original unit ..
            try
            {
                fVal = unitVal.convertValue(fVal, i_unitVal);
                unitVal = i_unitVal;
            }
            catch( CException& )
            {
                formatResult = FormatResult::UnitConversionFailed;
            }
        }
    } // if( physValRes.isValid() )

    if( formatResult == FormatResult::Ok )
    {
        formatResult = formatValue(
            /* fVal                    */ fVal,
            /* unitVal                 */ i_unitVal,
            /* iDigitsMantissaMax      */ iDigitsMantissa,
            /* bDigitsAccuracyLimitsM. */ bDigitsAccuracyLimitsMantissa,
            /* iDigitsAccuracy         */ iDigitsAccuracy,
            /* iDigitsExponent         */ i_iDigitsExponent,
            /* bUseEngineeringFormat   */ i_bUseEngineeringFormat,
            /* pfVal                   */ o_pfVal,
            /* pstrVal                 */ o_pstrVal,
            /* pUnitVal                */ o_pUnitVal,
            /* piDigitsLeading         */ o_piDigitsLeading,
            /* piDigitsTrailing        */ o_piDigitsTrailing,
            /* piDigitsExponent        */ o_piDigitsExponent );
    }
    return formatResult;

} // formatValue

//------------------------------------------------------------------------------
/*! @brief Converts a physical value into its string representation and/or into
    its numerical format according to several format specifications.

    This is an overloaded function and behaves essentially like the 
    "formatValue" function above.
    In addition digits may be grouped into digit groups delimited with a user
    defined group delimiter.

    @param      i_iDigitsPerDigitGroup:     Range [0, 1..N]
                If set to a value greater than 0 this parameter defines the number of digits
                per digit group. The digit group will be delimited by the "DigitsGroupDelimiter"
                string as defined by the next input parameter. The leading and trailing part
                of the value will be treated separately.

    @param      i_pstrDigitsGroupDelimiter: Range [nullptr, valid address]
                If set to a valid address and if "DigitsPerDigitGroup" is greater than 0 this
                parameter defines the character (or characters) which will be inserted between
                each digit group. If the digits should be grouped ("DigitsPerDigitGroup" is set
                to a value greater than 0) and if a nullptr pointer is provided for the group
                delimiter string blanks will be inserted between each group.
                Examples for the value = 1234567.8901234:
                    - DigitsPerDigitGroup = 3
                        DigitsGroupDelimiter = nullptr
                        strVal = "1 234 567.890 123 4"
                    - DigitsPerDigitGroup = 3
                        DigitsGroupDelimiter = "#"
                        strVal = "1#234#567.890#123#4"

    @param      i_pstrDecimalPoint: Range [nullptr, valid address]
                The default character for the decimal point is the dot ".".
                On transferring a valid QString reference this default character may
                be changed e.g. into a comma ",".

    @return     see "formatValue" method above
*/
TFormatResult ZS::PhysVal::formatValue(
    double         i_fVal,
    const CUnit&   i_unitVal,
    double         i_fRes,
    const CUnit*   i_pUnitRes,
    EResType       i_resType,
    int            i_iDigitsMantissa,
    int            i_iDigitsExponent,
    int            i_iDigitsPerDigitGroup,
    const QString* i_pstrDigitsGroupDelimiter,
    const QString* i_pstrDecimalPoint,
    bool           i_bUseEngineeringFormat,
    double*        o_pfVal,
    QString*       o_pstrVal,
    CUnit*         o_pUnitVal,
    int*           o_piDigitsLeading,
    int*           o_piDigitsTrailing,
    int*           o_piDigitsExponent )
//------------------------------------------------------------------------------
{
    TFormatResult formatResult;
    int           iDigitsLeading;
    int           iDigitsTrailing;
    QString       strDelimiter = " ";

    formatResult = formatValue(
        /* fVal                  */ i_fVal,
        /* unitVal               */ i_unitVal,
        /* fRes                  */ i_fRes,
        /* pUnitRes              */ i_pUnitRes,
        /* resType               */ i_resType,
        /* iDigitsMantissa       */ i_iDigitsMantissa,
        /* iDigitsExponent       */ i_iDigitsExponent,
        /* bUseEngineeringFormat */ i_bUseEngineeringFormat,
        /* pfVal                 */ o_pfVal,
        /* pstrVal               */ o_pstrVal,
        /* pUnitVal              */ o_pUnitVal,
        /* piDigitsLeading       */ &iDigitsLeading,
        /* piDigitsTrailing      */ &iDigitsTrailing,
        /* piDigitsExponent      */ o_piDigitsExponent );

    if( !(formatResult & FormatResult::Error) && o_pstrVal != nullptr )  //lint !e655
    {
        if( i_iDigitsPerDigitGroup > 0 )
        {
            if( i_pstrDigitsGroupDelimiter != nullptr )
            {
                strDelimiter = *i_pstrDigitsGroupDelimiter;
            }
            insertDelimiter(
                /* iDigitsPerDigitGroup */ i_iDigitsPerDigitGroup,
                /* strDelimiter         */ strDelimiter,
                /* pstrValue            */ o_pstrVal,
                /* iDigitsLeading       */ iDigitsLeading,
                /* iDigitsTrailing      */ iDigitsTrailing );
        }
        if( i_pstrDecimalPoint != nullptr )
        {
            o_pstrVal->replace( QChar('.'), *i_pstrDecimalPoint );
        }
    }
    if( o_piDigitsLeading != nullptr )
    {
        *o_piDigitsLeading = iDigitsLeading;
    }
    if( o_piDigitsTrailing != nullptr )
    {
        *o_piDigitsTrailing = iDigitsTrailing;
    }
    return formatResult;

} // formatValue

//------------------------------------------------------------------------------
/*! @brief Converts a physical value into its string representation and/or into
    its numerical format according to several format specifications.

    This is an overloaded function and behaves essentially like the 
    "formatValue" functions above.
    But whereas the "formatValue" functions above are using a resolution value
    to find the correct number of accuracy digits this function may be used
    to format the value with a predefined number of accuracy digits either
    defined for the whole mantissa or just for the number of trailing digits.

    @param      i_physSize:     Range [EPhysSizeMin ... EPhysSizeMax]
                Physical size of the values.

    @param      i_physUnit:     Range [IdEUnitMin ... IdEUnitMax]
                Physical unit of the value to be formatted.
                Must be one of the values of enum IdEUnit defined in "IdCommon.h".

    @param      i_fValue:       Range [DOUBLE_MIN .. DOUBLE_MAX]
                Value which got to be formatted.

    @param      i_iDigitsMantissaMax:   Range [-1, 1..N]
                If this value is set by the callee to a value greater than 0 this parameter
                defines the maximum number of mantissa digits (leading and trailing part of
                the string representation) to be used for formatting the value.
                Please note that "DigitsMantissaMax" determines the maximum allowed number of digits
                used to limit the length of the resulting string.

    @param      i_bDigitsAccuracyLimitsMantissa:    Range [true, false]
                Specifies how the following input parameter "DigitsAccuracy" will be treated.
                false ... "DigitsAccuracy" is used to define the number of valid trailing digits.
                true .... "DigitsAccuracy" is used to define the number of valid digits for the
                            whole mantissa.

    @param      i_iDigitsAccuracy:  Range [-1, 1..N]
                If this value is set by the callee to a value greater than 0 this parameter
                defines either the number of valid trailing or the number of valid digits
                for the whole mantissa.
                Please note that leading zeroes are not "valid" digits and will not be taken
                into account. E.g. if a value should always be indicated with at least 5 valid
                digits for the mantissa the value 0.0003456789 will be converted into "0.00034567".
                The input parameter "DigitsMantissaMax" has priority over "DigitsAccuracy" and
                may still limit the length of the resulting string.

    @param      i_iDigitsExponent:  Range IN  [-1, 0..N]
                If this value is set by the callee to a value greater or equal to 0 this parameter
                defines the maximum number of exponent digits to be used for formatting the value.

    @param      i_bUseEngineeringFormat:    Range [false, true]
                Optional parameter which may be set to true if the value should be formatted with
                exactly one valid leading digit.

    @param      o_pPhysUnit:    Range IN  [nullptr, valid address]
                                            OUT [IdEUnitMin ... IdEUnitMax]
                Optional parameter which may be used if the value may be converted into its
                string representation using the best fitting unit returning the physical unit
                into which the value has been converted.
                If this parameter is set to nullptr the value will be converted without finding
                the best fitting unit.

    @param      o_pfValue:      Range IN  [nullptr, valid address]
                                        OUT [DOUBLE_MIN .. DOUBLE_MAX]
                Optional parameter which may be used if the callee is interested in the numerical
                representation of the converted value.

    @param      o_pstrValue:    Range IN  [nullptr, valid address]
                                        OUT [string representation of the converted value]
                Optional parameter which may be used if the value may be converted into its
                string representation returing the converted value as its string representation.

    @param      o_piDigitsLeading:      Range IN  [nullptr, valid address]
                                                OUT [1..N] (always at least on leading digit will be used)
                Optional parameter which may be used if the callee is interested in how
                the value has been converted into its string representation returning
                the number of leading digits used to format the value.

    @param      o_piDigitsTrailing:     Range IN  [nullptr, valid address]
                                                OUT [0..N]
                Optional parameter which may be used if the callee is interested in how
                the value has been converted into its string representation returning
                the number of trailing digits used to format the value.

    @param      o_piDigitsExponent:     Range IN  [nullptr, valid address]
                                                OUT [0..N]
                Optional parameter which may be used if the callee is interested in how
                the value has been converted into its string representation returning
                the number of exponent digits used to format the value.

    @return     Examine the result value to get information if the conversion was successfull.
                FormatResult::Ok .... The value has been successfully converted.
                TFormatResult<..> .. Any other format result as "Ok" indicates that it
                                        was not possible to convert the value according to the
                                        defined format specifications. Some of the results
                                        indicate an error, some others are used as warnings.
                                        Please note that the bit number number 8 (0x80) of the
                                        format result indicates an error. If this bit is not set
                                        the result is either Ok or is just a warning.
                    AccuracyOverflow ... Warning: the value has been converted but is indicated
                                                    with too many digits.
                    AccuracyUnderflow .. Warning: the value has been converted but is indicated
                                                    with too less digits.
                    ValueOverflow ...... Error:   the value has not been converted. An invalid
                                                    string "---" will be returned.
                    ValueUnderflow ..... Error:   the value has not been converted. But the
                                                    value "0" is returned.
*/
TFormatResult ZS::PhysVal::formatValue(
    double       i_fVal,
    const CUnit& i_unitVal,
    int          i_iDigitsMantissaMax,
    bool         i_bDigitsAccuracyLimitsMantissa,
    int          i_iDigitsAccuracy,
    int          i_iDigitsExponent,
    bool         i_bUseEngineeringFormat,
    double*      o_pfVal,
    QString*     o_pstrVal,
    CUnit*       o_pUnitVal,
    int*         o_piDigitsLeading,
    int*         o_piDigitsTrailing,
    int*         o_piDigitsExponent )
//------------------------------------------------------------------------------
{
    TFormatResult formatResult    = FormatResult::Ok;
    CUnit         unitVal         = i_unitVal;
    double        fValSign        = i_fVal >= 0.0 ? 1.0 : -1.0;
    double        fValAbs         = fabs(i_fVal);
    int           iDigitsLeading  = 0;
    int           iDigitsTrailing = 0;
    int           iDigitsExponent = 0;
    int           iDigitsMantissa = -1;
    int           iDigitsAccuracyMantissa = -1;
    int           iDigitsAccuracyTrailing = -1;
    QString       strVal;
    double        fFract;
    double        fInt;
    double        fDigitsTrailing;

    if( i_iDigitsAccuracy > 0 )
    {
        if( i_bDigitsAccuracyLimitsMantissa)
        {
            iDigitsAccuracyMantissa = i_iDigitsAccuracy;

            // Try to find "best" unit for readability (at most three leading digits, at least one leading digit).
            // As the number of digits for the mantissa is limited by the accuracy the number of leading digits
            // should not exceed the number of accuracy digits.
            if( unitVal.isValid() && o_pUnitVal != nullptr && fValAbs != 0.0 )
            {
                if( i_iDigitsAccuracy < 3 )
                {
                    unitVal = unitVal.findBestUnit(fValAbs, &fValAbs, i_iDigitsAccuracy);
                }
                else
                {
                    unitVal = unitVal.findBestUnit(fValAbs, &fValAbs);
                }
            }
        }
        else
        {
            iDigitsAccuracyTrailing = i_iDigitsAccuracy;

            // Try to find "best" unit for readability (at most three leading digits, at least one leading digit).
            if( unitVal.isValid() && o_pUnitVal != nullptr && fValAbs != 0.0 )
            {
                unitVal = unitVal.findBestUnit(fValAbs, &fValAbs);
            }
        }
    }
    else
    {
        // Try to find "best" unit for readability (at most three leading digits, at least one leading digit).
        if( unitVal.isValid() && o_pUnitVal != nullptr && fValAbs != 0.0 )
        {
            unitVal = unitVal.findBestUnit(fValAbs, &fValAbs);
        }
    }

    // Determine the position of the first valid digit needed to indicate the value:
    if( fValAbs >= 10.0-DBL_EPSILON )
    {
        iDigitsLeading  = static_cast<int>(log10(fValAbs))+1;
        iDigitsTrailing = 0;
    }
    else if( fValAbs >= 1.0-DBL_EPSILON )
    {
        iDigitsLeading  = 1;
        iDigitsTrailing = 0;
    }
    else if( fValAbs >= DBL_EPSILON )
    {
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

    // Calculate how many leading and trailing digits would be necessary to
    // indicate the value to be converted and decide, whether an exponent is
    // necessary to indicate the value ...

    // If the value should be formatted using engineering format
    // or if the number of digits for the mantissa is limited and if the number
    // of leading or trailing digits necessary to indicate the first valid digit
    // exceeds the given maximum number of digits for the mantissa ...
    if( i_bUseEngineeringFormat || ( i_iDigitsMantissaMax > 0 && ( iDigitsLeading + iDigitsTrailing > i_iDigitsMantissaMax ) ) )
    {
        // ... the value will be formatted with one leading and as many
        // trailing digits as necessary (and possible).

        iDigitsExponent = 0;
        if( iDigitsLeading > 1 )
        {
            iDigitsExponent = static_cast<int>(log10(static_cast<double>(iDigitsLeading)))+1;
        }
        else if( iDigitsTrailing > 0 )
        {
            iDigitsExponent = static_cast<int>(log10(static_cast<double>(iDigitsTrailing)))+1;
        }

        // If the number of digits for the exponent is limited and the number of
        // necessary exponent digits exceeds this limit ...
        if( i_iDigitsExponent >= 0 && iDigitsExponent > i_iDigitsExponent )
        {
            // ... the value cannot be shown with the defined format specification.
            if( fValAbs >= 1.0 )
            {
                formatResult = FormatResult::ValueOverflow;
            }
            else
            {
                formatResult = FormatResult::ValueUnderflow;
            }
        }
        // If the number of digits for the exponent is not limited or if the number of
        // necessary exponent digits does not exceed the limit ...
        else
        {
            iDigitsLeading = 1;
            iDigitsTrailing = 0;
        }
    } // if( UseEngineeringFormat || DigitsNecessary > DigitsMantissa )

    // At this point the pair "iDigitsLeading" and "iDigitsTrailing" determine the
    // position of the first valid digit of the value to be formatted. "iDigitsMantissa"
    // is still not yet calculated. And the accuracy has not yet been taken into account.

    // If the value can be formatted according to the defined format specifications ...
    if( formatResult == FormatResult::Ok )
    {
        // If the number of digits for the mantissa should be adjusted to a minimum
        // number according to the specified accuracy ...
        if( iDigitsAccuracyMantissa > 0 )
        {
            // If the number of leading digits of the converted value exceeds the
            // number of accuracy digits for the mantissa ..
            if( iDigitsLeading > iDigitsAccuracyMantissa )
            {
                // .. the value need to be shortened (the decimal point need to be moved left)
                // and an exponent is necessary to indicate the value with the correct accuracy.
                iDigitsExponent = static_cast<int>(log10(static_cast<double>(iDigitsLeading-iDigitsAccuracyMantissa))+1);

                // If there are not enough exponent digits available ..
                if( i_iDigitsExponent != -1 && iDigitsExponent > i_iDigitsExponent )
                {
                    // .. the value must be indicated with more leading digits
                    // as the accuracy would allow.
                    formatResult = FormatResult::AccuracyOverflow;
                    iDigitsExponent = i_iDigitsExponent;
                }
                // If there are enough exponent digits available ..
                else
                {
                    // .. move the decimal point to the left (implicitly use engineering format).
                    iDigitsLeading = 1;
                    iDigitsTrailing = iDigitsAccuracyMantissa-1;
                }
            }

            // If the number of trailing digits to indicate the first digit of the value
            // plus the number of accuracy digits appended to the first trailing digit
            // would exceed the maximum allowed number for the mantissa ...
            // Please note to indicate the first trailing digit e.g. at position 1 with
            // e.g. five accuracy mantissa digits 6 mantissa digits would be necessary
            // calculated as (iDigitsTrailing-1)+iDigitsAccuracy+iDigitLeading = (1-1)+5+1 = 6.
            // This means (just for the sake of clarification) that its not necessary first to
            // subtract one and afterwards add one for the leading digit.
            else if( i_iDigitsMantissaMax > 0 && (iDigitsTrailing + iDigitsAccuracyMantissa) > i_iDigitsMantissaMax )
            {
                // .. the value must be indicated with less digits as the accuracy would demand.
                formatResult = FormatResult::AccuracyUnderflow;

                //// The value need to be shortened (the decimal point need to be moved right)
                //// and an exponent is necessary to indicate the value with the correct accuracy.
                //iDigitsExponent = static_cast<int>(log10(static_cast<double>(iDigitsTrailing+iDigitsAccuracyMantissa-i_iDigitsMantissaMax))+1);

                //// If there are not enough exponent digits available ..
                //if( i_iDigitsExponent != -1 && iDigitsExponent > i_iDigitsExponent )
                //{
                //    iDigitsExponent = i_iDigitsExponent;
                //}
                //// If there are enough exponent digits available ..
                //else
                //{
                //    // .. move the decimal point to the left (implicitly use engineering format).
                //    iDigitsLeading = 1;
                //    iDigitsTrailing = iDigitsAccuracyMantissa-1;
                //}
            }
        } // if( iDigitsAccuracyMantissa > 0 )

        // If the number of trailing digits should be adjusted to a minimum
        // number according to the specified accuracy ...
        else if( iDigitsAccuracyTrailing > 0 )
        {
            // If the number of trailing digits to indicate the first digit of the value
            // to be converted plus the number of trailing accuracy digits plus the
            // one leading digit always visible (e.g. "0.03736") exceeds the maximum allowed
            // number for the mantissa ...
            if( i_iDigitsMantissaMax > 0 && (iDigitsLeading + iDigitsAccuracyTrailing) > i_iDigitsMantissaMax )
            {
                // .. the value need to be shortened (the decimal point need to be moved right)
                // and an exponent is necessary to indicate the value with the correct accuracy.
                iDigitsExponent = static_cast<int>(log10(static_cast<double>(iDigitsTrailing+iDigitsAccuracyMantissa-i_iDigitsMantissaMax))+1);

                // If there are not enough exponent digits available ..
                if( i_iDigitsExponent != -1 && iDigitsExponent > i_iDigitsExponent )
                {
                    // .. the value must be indicated with less trailing digits
                    // as the accuracy would demand.
                    formatResult = FormatResult::AccuracyUnderflow;
                    iDigitsExponent = i_iDigitsExponent;
                }
                // If there are enough exponent digits available ..
                else
                {
                    // .. move the decimal point to the left (implicitly use engineering format).
                    iDigitsLeading = 1;
                    iDigitsTrailing = 0;
                }
            }
        } // if( iDigitsAccuracyTrailing > 0 )

        // If neither the number of digits for the mantissa nor the number of trailing digits
        // should be adjusted to a minimum number according to the specified accuracy ...
        else
        {
            iDigitsTrailing = 6;

            if( i_iDigitsMantissaMax > 0 )
            {
                if( iDigitsLeading > 0 )
                {
                    iDigitsTrailing = i_iDigitsMantissaMax - iDigitsLeading;
                }
                else if( iDigitsTrailing > i_iDigitsMantissaMax )
                {
                    iDigitsTrailing = i_iDigitsMantissaMax;
                }
            }
        }

    } // if( formatResult == FormatResult::Ok )

    // At this point the pair "iDigitsLeading" and "iDigitsTrailing" determine the
    // position of the first valid digit of the value to be formatted taking into
    // account the accuracy. But "iDigitsMantissa" is still not yet calculated.

    // If the value cannot be shown with the defined format specification
    // (format overflow or format underflow) ...
    if( formatResult & FormatResult::Error )  //lint !e655
    {
        iDigitsLeading = 0;
        iDigitsTrailing = 0;
        iDigitsMantissa = 0;
    }

    // If the value can be shown with the defined format specification
    // (but maybe with to less or to much digits according to the accuracy) ...
    else
    {
        // Take accuracy into account to determine the number of mantissa digits:
        if( iDigitsAccuracyMantissa > 0 )
        {
            // At least the leading digits need to be output (accuracy overflow) ..
            if( iDigitsAccuracyMantissa < iDigitsLeading )
            {
                iDigitsMantissa = iDigitsLeading;
            }
            // Always at least one digit before the decimal point:
            else if( iDigitsLeading == 0 )
            {
                iDigitsLeading = 1;
                iDigitsMantissa = iDigitsLeading + iDigitsTrailing + iDigitsAccuracyMantissa - 1;
            }
            else
            {
                iDigitsMantissa = iDigitsAccuracyMantissa;
            }
        }
        else if( iDigitsAccuracyTrailing > 0 )
        {
            iDigitsMantissa = iDigitsLeading + iDigitsAccuracyTrailing;

            // Always at least one digit before the decimal point:
            if( iDigitsLeading == 0 )
            {
                iDigitsLeading   = 1;
                iDigitsMantissa += 1;
            }
        }
        else
        {
            // Always at least one digit before the decimal point:
            if( iDigitsLeading == 0 )
            {
                iDigitsLeading = 1;
            }
            iDigitsMantissa = iDigitsLeading + iDigitsTrailing;
        }

        // Take maximum allowed number of mantissa digits into account:
        if( i_iDigitsMantissaMax > 0 && iDigitsMantissa > i_iDigitsMantissaMax )
        {
            iDigitsMantissa = i_iDigitsMantissaMax;
        }

        // Now determine the number of trailing digits. The number of trailing digits is
        // used as the "precision" for converting the value into its string representation.
        iDigitsTrailing = iDigitsMantissa - iDigitsLeading;
    }

    if( o_pstrVal != nullptr )
    {
        if( formatResult & FormatResult::Error )  //lint !e655
        {
            if( formatResult == FormatResult::ValueUnderflow )
            {
                strVal = "0";
                iDigitsLeading = 1;
                iDigitsTrailing = 0;
                iDigitsMantissa = 1;
            }
            else
            {
                strVal = invalidValueString();
            }
        }
        else
        {
            if( iDigitsExponent > 0 )
            {
                strVal = QString::number(fValSign*fValAbs, 'e',iDigitsTrailing);
            }
            else
            {
                strVal = QString::number(fValSign*fValAbs, 'f', iDigitsTrailing);
            }

            // The value could have been rounded up and there might have been
            // added an additional leading digit (e.g. 9.9999999 with three
            // accuracy digits would have been converted into "10.00").
            if( iDigitsTrailing > 1 )
            {
                int iDigitsLeadingTmp, iDigitsTrailingTmp, iDigitsExponentTmp;
                formatResult = formatString(
                    /* strVal           */ strVal,
                    /* piDigitsLeading  */ &iDigitsLeadingTmp,
                    /* piDigitsTrailing */ &iDigitsTrailingTmp,
                    /* piDigitsExponent */ &iDigitsExponentTmp );
                if( formatResult == FormatResult::Ok )
                {
                    if( iDigitsLeadingTmp + iDigitsTrailingTmp > iDigitsLeading + iDigitsTrailing )
                    {
                        if( iDigitsExponent > 0 )
                        {
                            strVal = QString::number(fValSign*fValAbs, 'e', iDigitsTrailing-1);
                        }
                        else
                        {
                            strVal = QString::number(fValSign*fValAbs, 'f', iDigitsTrailing-1);
                        }
                    }
                }
            }
            if( i_iDigitsAccuracy <= 0 )
            {
                removeTrailingZeros(&strVal,1);
            }
        }
        if( iDigitsExponent > 0 )
        {
            removeLeadingZerosFromExponent(&strVal);
        }
        *o_pstrVal = strVal;
    }
    if( o_pfVal != nullptr )
    {
        *o_pfVal = fValSign*fValAbs;
    }
    if( o_pUnitVal != nullptr )
    {
        *o_pUnitVal = unitVal;
    }
    if( o_piDigitsLeading != nullptr )
    {
        *o_piDigitsLeading = iDigitsLeading;
    }
    if( o_piDigitsTrailing != nullptr )
    {
        *o_piDigitsTrailing = iDigitsTrailing;
    }
    if( o_piDigitsExponent != nullptr )
    {
        *o_piDigitsExponent = iDigitsExponent;
    }
    return formatResult;

} // formatValue

//------------------------------------------------------------------------------
/*! @brief Converts a physical value into its string representation and/or into
    its numerical format according to several format specifications.

    This is an overloaded function and behaves essentially like the 
    "formatValue" functions above.
    In addition digits may be grouped into digit groups delimited with a user
    defined group delimiter.

    @param      i_iDigitsPerDigitGroup:     Range [0, 1..N]
                If set to a value greater than 0 this parameter defines the number of digits
                per digit group. The digit group will be delimited by the "DigitsGroupDelimiter"
                string as defined by the next input parameter. The leading and trailing part
                of the value will be treated separately.

    @param      i_pstrDigitsGroupDelimiter: Range [nullptr, valid address]
                If set to a valid address and if "DigitsPerDigitGroup" is greater than 0 this
                parameter defines the character (or characters) which will be inserted between
                each digit group. If the digits should be grouped ("DigitsPerDigitGroup" is set
                to a value greater than 0) and if a nullptr pointer is provided for the group
                delimiter string blanks will be inserted between each group.
                Examples for the value = 1234567.8901234:
                    - DigitsPerDigitGroup = 3
                        DigitsGroupDelimiter = nullptr
                        strVal = "1 234 567.890 123 4"
                    - DigitsPerDigitGroup = 3
                        DigitsGroupDelimiter = "#"
                        strVal = "1#234#567.890#123#4"

    @param      i_pstrDecimalPoint: Range [nullptr, valid address]
                The default character for the decimal point is the dot ".".
                On transferring a valid QString reference this default character may
                be changed e.g. into a comma ",".

    @return     see "formatValue" method above
*/
TFormatResult ZS::PhysVal::formatValue(
    double         i_fVal,
    const CUnit&   i_unitVal,
    int            i_iDigitsMantissaMax,
    bool           i_bDigitsAccuracyLimitsMantissa,
    int            i_iDigitsAccuracy,
    int            i_iDigitsExponent,
    int            i_iDigitsPerDigitGroup,
    const QString* i_pstrDigitsGroupDelimiter,
    const QString* i_pstrDecimalPoint,
    bool           i_bUseEngineeringFormat,
    double*        o_pfVal,
    QString*       o_pstrVal,
    CUnit*         o_pUnitVal,
    int*           o_piDigitsLeading,
    int*           o_piDigitsTrailing,
    int*           o_piDigitsExponent )
//------------------------------------------------------------------------------
{
    TFormatResult formatResult;
    int           iDigitsLeading;
    int           iDigitsTrailing;
    QString       strDelimiter = " ";

    formatResult = formatValue(
        /* fVal                    */ i_fVal,
        /* unitVal                 */ i_unitVal,
        /* iDigitsMantissaMax      */ i_iDigitsMantissaMax,
        /* bDigitsAccuracyLimitsM. */ i_bDigitsAccuracyLimitsMantissa,
        /* iDigitsAccuracy         */ i_iDigitsAccuracy,
        /* iDigitsExponent         */ i_iDigitsExponent,
        /* bUseEngineeringFormat   */ i_bUseEngineeringFormat,
        /* pfVal                   */ o_pfVal,
        /* pstrVal                 */ o_pstrVal,
        /* ppUnitVal               */ o_pUnitVal,
        /* piDigitsLeading         */ &iDigitsLeading,
        /* piDigitsTrailing        */ &iDigitsTrailing,
        /* piDigitsExponent        */ o_piDigitsExponent );

    if( formatResult == FormatResult::Ok && o_pstrVal != nullptr )
    {
        if( i_iDigitsPerDigitGroup > 0 )
        {
            if( i_pstrDigitsGroupDelimiter != nullptr )
            {
                strDelimiter = *i_pstrDigitsGroupDelimiter;
            }
            insertDelimiter(
                /* iDigitsPerDigitGroup */ i_iDigitsPerDigitGroup,
                /* strDelimiter         */ strDelimiter,
                /* pstrValue            */ o_pstrVal,
                /* iDigitsLeading       */ iDigitsLeading,
                /* iDigitsTrailing      */ iDigitsTrailing );
        }
        if( i_pstrDecimalPoint != nullptr )
        {
            o_pstrVal->replace( QChar('.'), *i_pstrDecimalPoint );
        }
    }
    if( o_piDigitsLeading != nullptr )
    {
        *o_piDigitsLeading = iDigitsLeading;
    }
    if( o_piDigitsTrailing != nullptr )
    {
        *o_piDigitsTrailing = iDigitsTrailing;
    }
    return formatResult;

} // formatValue

//------------------------------------------------------------------------------
TFormatResult ZS::PhysVal::parseValStr(
    const QString& i_strVal,
    bool*          o_pbValOk,
    double*        o_pfVal,
    CUnit*         io_pUnitVal,
    bool*          o_pbResOk,
    double*        o_pfRes,
    CUnit*         io_pUnitRes )
//------------------------------------------------------------------------------
{
    TFormatResult formatResult = FormatResult::Ok;

    QString  arSubStr[ESubStrCount];
    double   fVal = 0.0;
    CUnit    unitVal(io_pUnitVal);
    double   fRes = 0.0;
    CUnit    unitRes;
    bool     bOk;

    if( o_pbValOk != nullptr )
    {
        *o_pbValOk = false;
    }
    if( o_pbResOk != nullptr )
    {
        *o_pbResOk = false;
    }

    // Extract sub strings from input string:
    formatResult = getSubStrings(i_strVal, arSubStr);
    if( formatResult != FormatResult::Ok )
    {
        return formatResult;
    }

    // Get value from sub strings:
    if( arSubStr[ESubStrVal].isEmpty() )
    {
        return FormatResult::Error;
    }
    fVal = arSubStr[ESubStrVal].toDouble(&bOk);
    if( !bOk )
    {
        return FormatResult::Error;
    }
    if( o_pbValOk != nullptr )
    {
        *o_pbValOk = bOk;
    }
    if( bOk && o_pfVal != nullptr )
    {
        *o_pfVal = fVal;
    }
    if( !arSubStr[ESubStrValUnit].isEmpty() )
    {
        if( arSubStr[ESubStrValUnitGrp].isEmpty() )
        {
            if( unitVal.isValidGroup() )
            {
                // Use "keyInTree" instead of "getName(IncludingParentNames)" as the path contains the
                // root nodes (UnitClassType) whereas the key starts with the name of the science field.
                arSubStr[ESubStrValUnitGrp] = unitVal.groupPath();

                // For historical reasons keep the comment:

                //// Please note that the following code is not really needed as the
                //// known reference to the unit group is used later on to find the
                //// unit instance. But the code and comments should state the facts
                //// around the main group name of the units.
                //if( pUnitGrpVal->classType() == EUnitClassType::PhysSize )
                //{
                //    // For physical units the group name of the unit group corresponds
                //    // to the science field of the unit and is considered to be the
                //    // main group of the physical unit. The name of the unit group
                //    // corresponds to the physical size of the unit and is considered
                //    // to be the sub group of the physical unit.
                //    arSubStr[ESubStrValUnitGrp] = pUnitGrpVal->keyInTree();
                //}
                //else
                //{
                //    // SI base, ratio units and data quantity units are not children of a
                //    // parent unit group and the group name of the group is empty. The
                //    // name of the unit group corresponds to "SI-Base", "Ratio" or
                //    // "DataQuantity" and is considered to be the main group of the
                //    // SI base, data quantity and ratio units.
                //    arSubStr[ESubStrValUnitGrp] = pUnitGrpVal->keyInTree(); // for historical reasons keep the comment
                //}
            }
        }

        // If the group name of the unit was not specified within the input string but
        // if the unit group has been defined by the corresponding input parameter ...
        if( unitVal.isValidGroup() )
        {
            // .. the unit must be a child of the already known unit group.
            unitVal.setUnitName(arSubStr[ESubStrValUnit]);
        }
        // If the group name of the unit was specified within the input string ...
        else if( CIdxTreeUnits::GetInstance() != nullptr )
        {
            // .. we need to find both, the unit group and the unit by their names.
            unitVal = CIdxTreeUnits::GetInstance()->findUnit(
                arSubStr[ESubStrValUnitGrp], arSubStr[ESubStrValUnit]);
        }
        if( !unitVal.isValid() )
        {
            return FormatResult::Error;
        }
    }
    if( unitVal.isValid() && io_pUnitVal != nullptr )
    {
        *io_pUnitVal = unitVal;
    }

    // Get resolution (inaccuracy) from sub strings:
    if( !arSubStr[ESubStrRes].isEmpty() )
    {
        fRes = arSubStr[ESubStrRes].toDouble(&bOk);
        if( !bOk )
        {
            return FormatResult::Error;
        }
        if( o_pbResOk != nullptr )
        {
            *o_pbResOk = bOk;
        }
        if( bOk && o_pfRes != nullptr )
        {
            *o_pfRes = fRes;
        }
        if( !arSubStr[ESubStrResUnit].isEmpty() )
        {
            if( arSubStr[ESubStrResUnitGrp].isEmpty() )
            {
                if( io_pUnitRes != nullptr && (io_pUnitRes->isRatio() || io_pUnitRes->isQuantity()) )
                {
                    arSubStr[ESubStrResUnitGrp] = io_pUnitRes->groupPath();
                }
                else
                {
                    arSubStr[ESubStrResUnitGrp] = arSubStr[ESubStrValUnitGrp];
                }
            }
            if( arSubStr[ESubStrResUnitGrp].isEmpty() )
            {
                if( io_pUnitRes == nullptr )
                {
                    if( !unitVal.isValidGroup() )
                    {
                        return FormatResult::Error;
                    }
                    unitRes = unitVal;
                }
                else
                {
                    unitRes = *io_pUnitRes;
                }
                if( unitRes.isValidGroup() )
                {
                    arSubStr[ESubStrResUnitGrp] = unitRes.groupPath();

                    // For historical reasons keep the comment:

                    //// Please note that the following code is not really needed as the
                    //// known reference to the unit group is used later on to find the
                    //// unit instance. But the code and comments should state the facts
                    //// around the main group name of the units.
                    //if( pUnitGrpRes->classType() == EUnitClassType::PhysSize )
                    //{
                    //    // For physical units the group name of the unit group corresponds
                    //    // to the science field of the unit and is considered to be the
                    //    // main group of the physical unit. The name of the unit group
                    //    // corresponds to the physical size of the unit and is considered
                    //    // to be the sub group of the physical unit.
                    //    //arSubStr[ESubStrResUnitGrp] = pUnitGrpRes->getGroupName();
                    //}
                    //else
                    //{
                    //    // SI base, ratio and data quantity units are not chils of a parent
                    //    // unit group and the group name of the group is empty. The name of
                    //    // the unit group corresponds to "SI-Base", "DataQuantity" or "Ratio"
                    //    // and is considered to be the main group of the SI base, data quantity
                    //    // and ratio units.
                    //    //arSubStr[ESubStrResUnitGrp] = pUnitGrpRes->getName();
                    //}
                }
            }

            // If the group name of the unit was not specified within the input string but
            // if the unit group has been defined by the corresponding input parameter ...
            if( unitRes.isValidGroup() )
            {
                // .. the unit must be a child of the already known unit group.
                unitRes.setUnitName(arSubStr[ESubStrResUnit]);
            }
            // If the group name of the unit was specified within the input string ...
            else if( CIdxTreeUnits::GetInstance() != nullptr )
            {
                // .. we need to find both, the unit group and the unit by their names.
                unitRes = CIdxTreeUnits::GetInstance()->findUnit(
                    arSubStr[ESubStrResUnitGrp], arSubStr[ESubStrResUnit]);
            }
            if( !unitRes.isValid() )
            {
                return FormatResult::Error;
            }
        }
        if( unitRes.isValid() && io_pUnitRes != nullptr )
        {
            *io_pUnitRes = unitRes;
        }
    }

    return FormatResult::Ok;

} // parseValStr

//------------------------------------------------------------------------------
TFormatResult ZS::PhysVal::getSubStrings(
    const QString& i_strVal,
    QString*       io_arSubStr/*[ESubStrCount]*/ )
//------------------------------------------------------------------------------
{
    if( io_arSubStr == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }

    // Possible strings may look like       | And will be extracted to substrings as                            |
    //--------------------------------------+-------------------------------------------------------------------+
    //                                      |                Value                  |         Resolution        |
    //                                      | Value    |             Unit           | Value  |        Unit      |
    //                                      |          | Group              | Name  |        | Group    | Name  |
    //--------------------------------------+----------+--------------------+-------+--------+----------+-------+
    // "1.2e-3"                             | "1.2e-3" |                    |       |        |          |       |
    // "1.234"                              | "1.234"  |                    |       |        |          |       |
    // "-.7e-1 dBm"                         | ".7e-1"  |                    | "dBm" |        |          |       |
    // "5V"                                 | "5"      |                    | "V"   |        |          |       |
    // "(1.234 +/- 0.56) Kinematics.Time.s" | "1.234"  | "Kinematics.Time"  | "s"   | "0.56" |          |       |
    // "1.234 Geometry.km +/- 56 m"         | "1.234"  | "Geometry"         | "km"  | "56"   | "Length" | "m"   |

    CIdxTreeUnits* pIdxTree = CIdxTreeUnits::GetInstance();

    TFormatResult formatResult = FormatResult::Ok;
    int           iSubStr = ESubStrVal;
    int           iParanthesisLeft = 0;
    int           iParanthesisRight = 0;
    int           ariSubStrBeg[ESubStrCount];
    int           ariSubStrLen[ESubStrCount];
    int           idxChar;
    QString       strNodeSeparator = QString(pIdxTree == nullptr ? "." : pIdxTree->nodeSeparator());;
    QVector<int>  aridxSubStrUnitBegTmp; // Last entry for unit, previous entries for unit group
    QVector<int>  aridxSubStrUnitLenTmp; // Last entry for unit, previous entries for unit group
    int           idxTmp;

    memset( ariSubStrBeg, 0x00, ESubStrCount*sizeof(ariSubStrBeg[0]) );
    memset( ariSubStrLen, 0x00, ESubStrCount*sizeof(ariSubStrLen[0]) );

    // The numerical value will be extracted scanning the input string
    // starting with the first character.
    for( iSubStr = 0, idxChar = 0;
         (idxChar < i_strVal.length()) && (iSubStr < ESubStrCount) && (formatResult == FormatResult::Ok);
         idxChar++ )
    {
        switch( iSubStr )
        {
            case ESubStrVal:
            {
                // "1.2e-3" or "(1.234 +/- 0.56) Kinematics.Time.s" or  "1.234 Geometry.km +/- 56 m"
                // ^           ^                                        ^                           

                // Skip white spaces and left paranthesis ...
                for( ; idxChar < i_strVal.length(); idxChar++ )
                {
                    if( i_strVal[idxChar] == '(' )
                    {
                        iParanthesisLeft++;
                        idxChar++;
                        break;
                    }
                    if( !isWhiteSpace(i_strVal[idxChar]) )
                    {
                        break;
                    }
                }

                // "1.2e-3" or "(1.234 +/- 0.56) Kinematics.Time.s" or  "1.234 Geometry.km +/- 56 m"
                //  ^            ^                                       ^                          

                // Count number of digits for the numerical value.
                ariSubStrBeg[iSubStr] = idxChar;
                for( ; idxChar < i_strVal.length(); idxChar++ )
                {
                    if( !isNumChar(i_strVal[idxChar]) )
                    {
                        break;
                    }
                }
                ariSubStrLen[iSubStr] = idxChar - ariSubStrBeg[iSubStr];

                if( idxChar > 0 )
                {
                    // Points after the last digit. But will be incremented at the beginning of the for loop.
                    idxChar--;
                }

                // "1.2e-3" or "(1.234 +/- 0.56) Kinematics.Time.s" or  "1.234 Geometry.km +/- 56 m"
                //        ^           ^                                       ^                     
                if( iParanthesisLeft == 1 )
                {
                    iSubStr = ESubStrRes;
                }
                else
                {
                    iSubStr = ESubStrValUnitGrp;
                }
                break;
            } // case ESubStrVal

            case ESubStrValUnitGrp:
            case ESubStrValUnit:
            {
                // "1.2e-3" or "(1.234 +/- 0.56) Kinematics.Time.s" or  "1.234 Geometry.km +/- 56 m"
                //        ^                     ^                             ^                     

                // Skip white spaces ...
                for( ; idxChar < i_strVal.length(); idxChar++ )
                {
                    if( !isWhiteSpace(i_strVal[idxChar]) )
                    {
                        break;
                    }
                }

                //"1.2e-3" or  "(1.234 +/- 0.56) Kinematics.Time.s" or  "1.234 Geometry.km +/- 56 m"
                //       ^                       ^                             ^                    

                // If a unit for the value has been defined
                // (if not the resolution (inaccuracy) follows after the value) ...
                if( i_strVal[idxChar] != c_chSymbolPlusMinus )
                {
                    // Count number of digits for the unit string.
                    idxTmp = 0;
                    aridxSubStrUnitBegTmp.resize(idxTmp+1);
                    aridxSubStrUnitLenTmp.resize(idxTmp+1);
                    aridxSubStrUnitBegTmp[idxTmp] = idxChar;
                    for( ; idxChar < i_strVal.length(); ++idxChar )
                    {
                        if( isWhiteSpace(i_strVal[idxChar]) )
                        {
                            break;
                        }
                        if( i_strVal[idxChar] == c_chSymbolPlusMinus )
                        {
                            break;
                        }
                        if( i_strVal.mid(idxChar, strNodeSeparator.length()) == strNodeSeparator )
                        {
                            aridxSubStrUnitLenTmp[idxTmp] = idxChar-aridxSubStrUnitBegTmp[idxTmp];
                            idxChar += strNodeSeparator.length();
                            ++idxTmp;
                            aridxSubStrUnitBegTmp.resize(idxTmp+1);
                            aridxSubStrUnitLenTmp.resize(idxTmp+1);
                            aridxSubStrUnitBegTmp[idxTmp] = idxChar;
                        }
                    }
                    aridxSubStrUnitLenTmp[idxTmp] = idxChar-aridxSubStrUnitBegTmp[idxTmp];

                    if( idxTmp == 0 )
                    {
                        // If just one unit sub string was defined this must have been
                        // the name or symbol of the unit.
                        ariSubStrBeg[ESubStrValUnit] = aridxSubStrUnitBegTmp[0];
                        ariSubStrLen[ESubStrValUnit] = aridxSubStrUnitLenTmp[0];
                    }
                    else // if( idxTmp > 0 )
                    {
                        // If more than one unit sub strings have been defined the last one
                        // must have been the symbol or name of the unit and the previous ones
                        // must have been the group name.
                        ariSubStrBeg[ESubStrValUnitGrp] = aridxSubStrUnitBegTmp[0];
                        for( int _idx = 0; _idx < idxTmp; _idx++ )
                        {
                            ariSubStrLen[ESubStrValUnitGrp] += aridxSubStrUnitLenTmp[_idx];
                            if( _idx < idxTmp-1 )
                            {
                                ++ariSubStrLen[ESubStrValUnitGrp]; // separator
                            }
                        }
                        ariSubStrBeg[ESubStrValUnit] = aridxSubStrUnitBegTmp[idxTmp];
                        ariSubStrLen[ESubStrValUnit] = aridxSubStrUnitLenTmp[idxTmp];
                    }
                }

                if( iParanthesisLeft == 0 )
                {
                    iSubStr = ESubStrRes;
                    if( idxChar < i_strVal.length() && i_strVal[idxChar] == c_chSymbolPlusMinus )
                    {
                        idxChar--;
                    }
                }
                else
                {
                    ariSubStrBeg[ESubStrResUnitGrp] = ariSubStrBeg[ESubStrValUnitGrp];
                    ariSubStrLen[ESubStrResUnitGrp] = ariSubStrLen[ESubStrValUnitGrp];
                    ariSubStrBeg[ESubStrResUnit]    = ariSubStrBeg[ESubStrValUnit];
                    ariSubStrLen[ESubStrResUnit]    = ariSubStrLen[ESubStrValUnit];

                    iSubStr = ESubStrCount; // that's the end ..
                }

                //"1.2e-3" or  "(1.234 +/- 0.56) Kinematics.Time.s" or  "1.234 Geometry.km +/- 56 m"
                //       ^                                        ^                       ^         
                break;
            } // case ESubStrValUnitGrp | ESubStrValUnit

            case ESubStrRes:
            {
                //"1.2e-3" or  "(1.234 +/- 0.56) Kinematics.Time.s" or  "1.234 Geometry.km +/- 56 m"
                //       ^             ^                                                  ^         

                // Skip white spaces ...
                for( ; idxChar < i_strVal.length(); idxChar++ )
                {
                    if( !isWhiteSpace(i_strVal[idxChar]) )
                    {
                        break;
                    }
                }

                //"1.2e-3" or  "(1.234 +/- 0.56) Kinematics.Time.s" or  "1.234 Geometry.km +/- 56 m"
                //       ^             ^                                                   ^        

                // If resolution (inaccuracy) follows ...
                if( i_strVal[idxChar] == c_chSymbolPlusMinus )
                {
                    idxChar++;

                    // Skip white spaces ...
                    for( ; idxChar < i_strVal.length(); idxChar++ )
                    {
                        if( !isWhiteSpace(i_strVal[idxChar]) )
                        {
                            break;
                        }
                    }

                    //"1.2e-3" or  "(1.234 +/- 0.56) Kinematics.Time.s" or  "1.234 Geometry.km +/- 56 m"
                    //       ^                 ^                                                   ^    

                    // Count number of digits for the numerical value.
                    ariSubStrBeg[iSubStr] = idxChar;
                    for( ; idxChar < i_strVal.length(); idxChar++ )
                    {
                        if( !isNumChar(i_strVal[idxChar]) )
                        {
                            break;
                        }
                    }
                    ariSubStrLen[iSubStr] = idxChar-ariSubStrBeg[iSubStr];
                }

                if( iParanthesisLeft == 0 )
                {
                    iSubStr = ESubStrResUnitGrp;
                }
                else
                {
                    // Skip white spaces ...
                    for( ; idxChar < i_strVal.length(); idxChar++ )
                    {
                        if( !isWhiteSpace(i_strVal[idxChar]) )
                        {
                            break;
                        }
                    }
                    if( idxChar < i_strVal.length() && i_strVal[idxChar] == ')' )
                    {
                        iParanthesisRight++;
                        idxChar++;
                    }
                    if( iParanthesisLeft != 1 || iParanthesisRight != 1 )
                    {
                        formatResult = FormatResult::Error;
                    }
                    else
                    {
                        iSubStr = ESubStrValUnitGrp;
                    }
                }

                //"1.2e-3" or  "(1.234 +/- 0.56) Kinematics.Time.s" or  "1.234 Geometry.km +/- 56 m"
                //       ^                      ^                                              ^    
                break;
            } // case ESubStrRes

            case ESubStrResUnitGrp:
            case ESubStrResUnit:
            {
                // "1.2e-3" or "(1.234 +/- 0.56) Kinematics.Time.s" or  "1.234 Geometry.km +/- 56 m"
                //        ^                     ^                             ^                     

                // Skip white spaces ...
                for( ; idxChar < i_strVal.length(); idxChar++ )
                {
                    if( !isWhiteSpace(i_strVal[idxChar]) )
                    {
                        break;
                    }
                }

                //"1.2e-3" or  "(1.234 +/- 0.56) Kinematics.Time.s" or  "1.234 Geometry.km +/- 56 m"
                //       ^                     ^                               ^                    

                // Count number of digits for the unit string.
                idxTmp = 0;
                aridxSubStrUnitBegTmp.resize(idxTmp+1);
                aridxSubStrUnitLenTmp.resize(idxTmp+1);
                aridxSubStrUnitBegTmp[idxTmp] = idxChar;
                for( ; idxChar < i_strVal.length(); idxChar++ )
                {
                    if( isWhiteSpace(i_strVal[idxChar]) )
                    {
                        break;
                    }
                    if( i_strVal.mid(idxChar, strNodeSeparator.length()) == strNodeSeparator )
                    {
                        aridxSubStrUnitLenTmp[idxTmp] = idxChar-aridxSubStrUnitBegTmp[idxTmp];
                        idxChar += strNodeSeparator.length();
                        ++idxTmp;
                        aridxSubStrUnitBegTmp.resize(idxTmp+1);
                        aridxSubStrUnitLenTmp.resize(idxTmp+1);
                        aridxSubStrUnitBegTmp[idxTmp] = idxChar;
                    }
                }
                aridxSubStrUnitLenTmp[idxTmp] = idxChar-aridxSubStrUnitBegTmp[idxTmp];

                if( idxTmp == 0 )
                {
                    // If just one unit sub string was defined this must have been
                    // the name or symbol of the unit.
                    ariSubStrBeg[ESubStrResUnit] = aridxSubStrUnitBegTmp[0];
                    ariSubStrLen[ESubStrResUnit] = aridxSubStrUnitLenTmp[0];
                }
                else // if( idxTmp > 0 )
                {
                    // If more than one unit sub strings have been defined the last one
                    // must have been the symbol or name of the unit and the previous ones
                    // must have been the group name.
                    ariSubStrBeg[ESubStrResUnitGrp] = aridxSubStrUnitBegTmp[0];
                    for( int _idx = 0; _idx < idxTmp; _idx++ )
                    {
                        ariSubStrLen[ESubStrResUnitGrp] += aridxSubStrUnitLenTmp[_idx];
                        if( _idx < idxTmp-1 )
                        {
                            ++ariSubStrLen[ESubStrResUnitGrp]; // separator
                        }
                    }
                    ariSubStrBeg[ESubStrResUnit] = aridxSubStrUnitBegTmp[idxTmp];
                    ariSubStrLen[ESubStrResUnit] = aridxSubStrUnitLenTmp[idxTmp];
                }

                //"1.2e-3" or  "(1.234 +/- 0.56) Kinematics.Time.s" or  "1.234 Geometry.km +/- 56 m"
                //       ^                                        ^                       ^         
                iSubStr = ESubStrCount; // that's the end ..
                break;
            } // case ESubStrResUnitGrp | ESubStrResUnit

            default:
            {
                break;
            }
        }
    } // for( idxChar < i_strVal.length() )

    for( iSubStr = 0; iSubStr < ESubStrCount; iSubStr++ )
    {
        if( ariSubStrLen[iSubStr] > 0 )
        {
            io_arSubStr[iSubStr] = i_strVal.mid(ariSubStrBeg[iSubStr],ariSubStrLen[iSubStr]);
        }
    }
    return formatResult;

} // getSubStrings


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
