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

#include "ZSPhysVal/ZSPhysVal.h"
#include "ZSPhysVal/ZSPhysSize.h"
#include "ZSPhysVal/ZSPhysSizesIdxTree.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysErrResult.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::Math;
using namespace ZS::PhysVal;


/*******************************************************************************
Exported methods
*******************************************************************************/

//------------------------------------------------------------------------------
CPhysVal ZS::PhysVal::minPhysVal( const QVector<CPhysVal>& i_arphysval )
//------------------------------------------------------------------------------
{
    if( i_arphysval.size() < 2 )
    {
        throw CException( __FILE__, __LINE__, EResultArgOutOfRange, "i_arphysval.size() == " + QString::number(i_arphysval.size()) );
    }

    int      iValIdx;
    CPhysVal physvalMin = i_arphysval[0];

    for( iValIdx = 1; iValIdx < i_arphysval.size(); iValIdx++ )
    {
        CPhysVal physval = i_arphysval[iValIdx];

        if( physval < physvalMin )
        {
            physvalMin = physval;
        }
    }

    return physvalMin;

} // minPhysVal

//------------------------------------------------------------------------------
CPhysVal ZS::PhysVal::minPhysVal( const CPhysVal& i_physval1, const CPhysVal& i_physval2 )
//------------------------------------------------------------------------------
{
    QVector<CPhysVal> arphysVal;

    arphysVal.append(i_physval1);
    arphysVal.append(i_physval2);

    return minPhysVal(arphysVal);

} // minPhysVal

//------------------------------------------------------------------------------
CPhysVal ZS::PhysVal::minPhysVal( const CPhysVal& i_physval1, const CPhysVal& i_physval2, const CPhysVal& i_physval3 )
//------------------------------------------------------------------------------
{
    QVector<CPhysVal> arphysVal;

    arphysVal.append(i_physval1);
    arphysVal.append(i_physval2);
    arphysVal.append(i_physval3);

    return minPhysVal(arphysVal);

} // minPhysVal

//------------------------------------------------------------------------------
CPhysVal ZS::PhysVal::minPhysVal( const CPhysVal& i_physval1, const CPhysVal& i_physval2, const CPhysVal& i_physval3, const CPhysVal& i_physval4 )
//------------------------------------------------------------------------------
{
    QVector<CPhysVal> arphysVal;

    arphysVal.append(i_physval1);
    arphysVal.append(i_physval2);
    arphysVal.append(i_physval3);
    arphysVal.append(i_physval4);

    return minPhysVal(arphysVal);

} // minPhysVal

//------------------------------------------------------------------------------
CPhysVal ZS::PhysVal::maxPhysVal( const QVector<CPhysVal>& i_arphysval )
//------------------------------------------------------------------------------
{
    if( i_arphysval.size() < 2 )
    {
        throw CException( __FILE__, __LINE__, EResultArgOutOfRange, "i_arphysval.size() == " + QString::number(i_arphysval.size()) );
    }

    int      iValIdx;
    CPhysVal physvalMax = i_arphysval[0];

    for( iValIdx = 1; iValIdx < i_arphysval.size(); iValIdx++ )
    {
        CPhysVal physval = i_arphysval[iValIdx];

        if( physval > physvalMax )
        {
            physvalMax = physval;
        }
    }

    return physvalMax;

} // maxPhysVal

//------------------------------------------------------------------------------
CPhysVal ZS::PhysVal::maxPhysVal( const CPhysVal& i_physval1, const CPhysVal& i_physval2 )
//------------------------------------------------------------------------------
{
    QVector<CPhysVal> arphysVal;

    arphysVal.append(i_physval1);
    arphysVal.append(i_physval2);

    return maxPhysVal(arphysVal);

} // maxPhysVal

//------------------------------------------------------------------------------
CPhysVal ZS::PhysVal::maxPhysVal( const CPhysVal& i_physval1, const CPhysVal& i_physval2, const CPhysVal& i_physval3 )
//------------------------------------------------------------------------------
{
    QVector<CPhysVal> arphysVal;

    arphysVal.append(i_physval1);
    arphysVal.append(i_physval2);
    arphysVal.append(i_physval3);

    return maxPhysVal(arphysVal);

} // maxPhysVal

//------------------------------------------------------------------------------
CPhysVal ZS::PhysVal::maxPhysVal( const CPhysVal& i_physval1, const CPhysVal& i_physval2, const CPhysVal& i_physval3, const CPhysVal& i_physval4 )
//------------------------------------------------------------------------------
{
    QVector<CPhysVal> arphysVal;

    arphysVal.append(i_physval1);
    arphysVal.append(i_physval2);
    arphysVal.append(i_physval3);
    arphysVal.append(i_physval4);

    return maxPhysVal(arphysVal);

} // maxPhysVal

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
EFormatResult ZS::PhysVal::formatString(
    const QString& i_strValue,
    int*           o_piDigitsLeading,
    int*           o_piDigitsTrailing,
    int*           o_piDigitsExponent )
//------------------------------------------------------------------------------
{
    int  idx;
    int  iDigitsLeading = 0;
    int  iDigitsTrailing = 0;
    int  iDigitsExponent = 0;
    bool bLeadingSection = true;
    bool bTrailingSection = false;
    bool bExponentSection = false;

    for( idx = 0; idx < i_strValue.length(); idx++ )
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
    return EFormatResultOk;

} // formatString

//------------------------------------------------------------------------------
EFormatResult ZS::PhysVal::formatValue(
    double       i_fVal,
    const CUnit* i_pUnitVal,
    double       i_fRes,
    const CUnit* i_pUnitRes,
    EResType     i_resType,
    int          i_iDigitsMantissa,
    int          i_iDigitsExponent,
    bool         i_bUseEngineeringFormat,
    double*      o_pfVal,
    QString*     o_pstrVal,
    CUnit**      o_ppUnitVal,
    int*         o_piDigitsLeading,
    int*         o_piDigitsTrailing,
    int*         o_piDigitsExponent )
//------------------------------------------------------------------------------
{
    /*
    Converts a physical value into its string representation and/or into
    its numerical format according to several format specifications.
    E.g. this methods may be used to convert a value from a given source unit
    into a unit which best fits according to the resolution and user readability.
    E.g. if the resolution (or better inaccuracy) of a frequency is 100.0 kHz
    and the value is defined as 36726372541.3 Hz, the value would be "best"
    output as 36.7264 GHz.

    @param      i_physSize:     Range [EPhysSizeMin ... EPhysSizeMax]
                Physical size of the values.

    @param      i_physUnitValue:    Range [IdEUnitMin ... IdEUnitMax]
                Physical unit of the value to be formatted.
                Must be one of the values of enum IdEUnit defined in "IdCommon.h".

    @param      i_fValue:       Range [DOUBLE_MIN .. DOUBLE_MAX]
                Value which got to be formatted.

    @param      i_physUnitRes:  Range [IdEUnitMin ... IdEUnitMax]
                Physical unit of the resolution.
                Must be one of the values of enum IdEUnit defined in "IdCommon.h".
                If set to IdEUnitNone it is assumed that the unit of the resolution is
                the same as the unit of the value to be formatted.

    @param      i_fRes:     Range [DOUBLE_MIN .. DOUBLE_MAX]
                Resolution (accuracy) of the value to be formatted.

    @param      i_iDigitsMantissa:   Range [-1, 1..N]
                If this value is set by the callee to a value greater than 0 this parameter
                defines the maximum number of mantissa digits (leading and trailing part of
                the string representation) to be used for formatting the value.
                Please note that "DigitsMantissa" determines the maximum allowed number of digits
                used to limit the length of the resulting string.

    @param      i_iDigitsExponent:  Range IN  [-1, 0..N]
                If this value is set by the callee to a value greater or equal to 0 this parameter
                defines the maximum number of exponent digits to be used for formatting the value.

    @param      i_bUseEngineeringFormat:    Range [false, true]
                Optional parameter which may be set to true if the value should be formatted with
                exactly one valid leading digit.

    @param      o_pPhysUnitValue:   Range IN  [nullptr, valid address]
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
                EFormatResultOk .... The value has been successfully converted.
                EFormatResult<..> .. Any other format result as "Ok" indicates that it
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

    EFormatResult formatResult = EFormatResultOk;
    const CUnit*  pUnitVal = i_pUnitVal;
    const CUnit*  pUnitRes = i_pUnitRes;
    double        fVal     = i_fVal;
    int           iValSign = i_fVal < 0 ? -1 : +1;
    double        fValAbs  = fabs(i_fVal);
    int           iValExp  = -999;
    bool          bIsLogRes;
    double        fResAbs  = fabs(i_fRes);
    int           iResMant = 0;
    int           iResExp  = 0;
    bool          bDigitsAccuracyLimitsMantissa = false;
    int           iDigitsAccuracy = -1;
    CUnit**       ppUnitValBest = o_ppUnitVal;
    int           iDigitsMantissa = i_iDigitsMantissa;

    // If a resolution should be taken into account ..
    if( i_fRes != 0.0 )
    {
        // The number of digits is limited by the resolution.
        bDigitsAccuracyLimitsMantissa = true;

        if( pUnitRes == nullptr )
        {
            pUnitRes = pUnitVal;
        }

        // If value has got a unit and the unit of the value and the unit of the resolution is different .
        if( pUnitVal != nullptr && pUnitRes != nullptr && pUnitVal != pUnitRes )
        {
            // If the accuracy is defined as a ratio number ..
            if( pUnitRes->unitGroup()->classType() == EUnitClassTypeRatios )
            {
                // .. the absolute resolution value need to be calculated in the unit
                // of the value.
                fResAbs = fResAbs * fValAbs * dynamic_cast<const CUnitRatio*>(pUnitRes)->getFactor();
            }
            else
            {
                // Try to convert the value into the unit of resolution ..
                try
                {
                    fValAbs = pUnitVal->convertValue(
                        /* fValueSrc    */ fValAbs,
                        /* pPhysUnitDst */ pUnitRes );
                    pUnitVal = pUnitRes;

                    bIsLogRes = pUnitRes->isLogarithmic();

                    // If the resolution is of logarithmic type ..
                    if( bIsLogRes )
                    {
                        // .. the resolution is considered to be relative. There will
                        // be no "best unit" the value can be converted to.
                        ppUnitValBest = nullptr;
                    }
                }
                catch( CException& )
                {
                    formatResult = EFormatResultUnitConversionFailed;
                }
            }
        }

        // If it was possible to convert the value into the unit of the resolution ...
        if( formatResult == EFormatResultOk )
        {
            // .. determine the number of accuracy digits for the mantissa.

            // Determine position of first valid digit of the value ..
            Math::normalize(fValAbs,nullptr,nullptr,&iValExp);

            // Determine position and first valid digit of the resolution ..
            Math::normalize(fResAbs,nullptr,&iResMant,&iResExp);

            if( i_resType == EResTypeAccuracy )
            {
                // If the first digit of the accuracy is either '1' or '2' ..
                if( iResMant == 1 || iResMant == 2 )
                {
                    // ... the value will be indicated with two insecure digits.
                    iResExp--;
                }
            }
            else if( i_resType == EResTypeResolution )
            {
                fValAbs = round2Res(fValAbs,fResAbs);
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

                if( ppUnitValBest != nullptr && pUnitVal != nullptr )
                {
                    pUnitVal = pUnitVal->findBestUnit(i_fRes);
                }
            }
        }
        fVal = iValSign * fValAbs;

        // The value need to be converted back to its original unit
        if( i_pUnitVal != nullptr && pUnitVal != nullptr && i_pUnitVal != pUnitVal )
        {
            // Try to convert the (maybe rounded) value into its original unit ..
            try
            {
                fVal = pUnitVal->convertValue(
                    /* fValueSrc    */ fVal,
                    /* pPhysUnitDst */ i_pUnitVal );
                pUnitVal = i_pUnitVal;
            }
            catch( CException& )
            {
                formatResult = EFormatResultUnitConversionFailed;
            }
        }

    } // if( physValRes.isValid() )

    if( formatResult == EFormatResultOk )
    {
        formatResult = formatValue(
            /* fVal                    */ fVal,
            /* pUnitVal                */ i_pUnitVal,
            /* iDigitsMantissaMax      */ iDigitsMantissa,
            /* bDigitsAccuracyLimitsM. */ bDigitsAccuracyLimitsMantissa,
            /* iDigitsAccuracy         */ iDigitsAccuracy,
            /* iDigitsExponent         */ i_iDigitsExponent,
            /* bUseEngineeringFormat   */ i_bUseEngineeringFormat,
            /* pfVal                   */ o_pfVal,
            /* pstrVal                 */ o_pstrVal,
            /* ppUnitVal               */ ppUnitValBest,
            /* piDigitsLeading         */ o_piDigitsLeading,
            /* piDigitsTrailing        */ o_piDigitsTrailing,
            /* piDigitsExponent        */ o_piDigitsExponent );
    }
    return formatResult;

} // formatValue

//------------------------------------------------------------------------------
EFormatResult ZS::PhysVal::formatValue(
    double         i_fVal,
    const CUnit*   i_pUnitVal,
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
    CUnit**        o_ppUnitVal,
    int*           o_piDigitsLeading,
    int*           o_piDigitsTrailing,
    int*           o_piDigitsExponent )
//------------------------------------------------------------------------------
{
    /*
    Converts a physical value into its string representation and/or into
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

    EFormatResult formatResult;
    int           iDigitsLeading;
    int           iDigitsTrailing;
    QString       strDelimiter = " ";

    formatResult = formatValue(
        /* fVal                  */ i_fVal,
        /* pUnitVal              */ i_pUnitVal,
        /* fRes                  */ i_fRes,
        /* physUnitRes           */ i_pUnitRes,
        /* resType               */ i_resType,
        /* iDigitsMantissa       */ i_iDigitsMantissa,
        /* iDigitsExponent       */ i_iDigitsExponent,
        /* bUseEngineeringFormat */ i_bUseEngineeringFormat,
        /* pfVal                 */ o_pfVal,
        /* pstrVal               */ o_pstrVal,
        /* ppUnitVal             */ o_ppUnitVal,
        /* piDigitsLeading       */ &iDigitsLeading,
        /* piDigitsTrailing      */ &iDigitsTrailing,
        /* piDigitsExponent      */ o_piDigitsExponent );

    if( !(formatResult & EFormatResultError) && o_pstrVal != nullptr )  //lint !e655
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
EFormatResult ZS::PhysVal::formatValue(
    double       i_fVal,
    const CUnit* i_pUnitVal,
    int          i_iDigitsMantissaMax,
    bool         i_bDigitsAccuracyLimitsMantissa,
    int          i_iDigitsAccuracy,
    int          i_iDigitsExponent,
    bool         i_bUseEngineeringFormat,
    double*      o_pfVal,
    QString*     o_pstrVal,
    CUnit**      o_ppUnitVal,
    int*         o_piDigitsLeading,
    int*         o_piDigitsTrailing,
    int*         o_piDigitsExponent )
//------------------------------------------------------------------------------
{
    /*
    Converts a physical value into its string representation and/or into
    its numerical format according to several format specifications.
    This is an overloaded function and behaves essentially like the 
    "formatValue" functions above.
    But whereas the "formatValue" functions above are using a resolution value
    to find the correct number of accuracy digits this function may be used
    to format the value with a predefined number of accuracy digits either
    defined for the whole mantissa or just for the number of trailing digits.

    @param      i_physSize:     Range [EPhysSizeMin ... EPhysSizeMax]
                Physical size of the values.
    *
    @param      i_physUnit:     Range [IdEUnitMin ... IdEUnitMax]
                Physical unit of the value to be formatted.
                Must be one of the values of enum IdEUnit defined in "IdCommon.h".
    *
    @param      i_fValue:       Range [DOUBLE_MIN .. DOUBLE_MAX]
                Value which got to be formatted.
    *
    @param      i_iDigitsMantissaMax:   Range [-1, 1..N]
                If this value is set by the callee to a value greater than 0 this parameter
                defines the maximum number of mantissa digits (leading and trailing part of
                the string representation) to be used for formatting the value.
                Please note that "DigitsMantissaMax" determines the maximum allowed number of digits
                used to limit the length of the resulting string.
    *
    @param      i_bDigitsAccuracyLimitsMantissa:    Range [true, false]
                Specifies how the following input parameter "DigitsAccuracy" will be treated.
                false ... "DigitsAccuracy" is used to define the number of valid trailing digits.
                true .... "DigitsAccuracy" is used to define the number of valid digits for the
                          whole mantissa.
    *
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
                EFormatResultOk .... The value has been successfully converted.
                EFormatResult<..> .. Any other format result as "Ok" indicates that it
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

    EFormatResult formatResult    = EFormatResultOk;
    const CUnit*  pUnitVal        = i_pUnitVal;
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
            if( pUnitVal != nullptr && o_ppUnitVal != nullptr && fValAbs != 0.0 )
            {
                if( i_iDigitsAccuracy < 3 )
                {
                    pUnitVal = pUnitVal->findBestUnit(fValAbs,&fValAbs,i_iDigitsAccuracy);
                }
                else
                {
                    pUnitVal = pUnitVal->findBestUnit(fValAbs,&fValAbs);
                }
            }
        }
        else
        {
            iDigitsAccuracyTrailing = i_iDigitsAccuracy;

            // Try to find "best" unit for readability (at most three leading digits, at least one leading digit).
            if( pUnitVal != nullptr && o_ppUnitVal != nullptr && fValAbs != 0.0 )
            {
                pUnitVal = pUnitVal->findBestUnit(fValAbs,&fValAbs);
            }
        }
    }
    else
    {
        // Try to find "best" unit for readability (at most three leading digits, at least one leading digit).
        if( pUnitVal != nullptr && o_ppUnitVal != nullptr && fValAbs != 0.0 )
        {
            pUnitVal = pUnitVal->findBestUnit(fValAbs,&fValAbs);
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
                formatResult = EFormatResultValueOverflow;
            }
            else
            {
                formatResult = EFormatResultValueUnderflow;
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
    if( formatResult == EFormatResultOk )
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
                    formatResult = EFormatResultAccuracyOverflow;
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
                formatResult = EFormatResultAccuracyUnderflow;

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
                    formatResult = EFormatResultAccuracyUnderflow;
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

    } // if( formatResult == EFormatResultOk )

    // At this point the pair "iDigitsLeading" and "iDigitsTrailing" determine the
    // position of the first valid digit of the value to be formatted taking into
    // account the accuracy. But "iDigitsMantissa" is still not yet calculated.

    // If the value cannot be shown with the defined format specification
    // (format overflow or format underflow) ...
    if( formatResult & EFormatResultError )  //lint !e655
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
        if( formatResult & EFormatResultError )  //lint !e655
        {
            if( formatResult == EFormatResultValueUnderflow )
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
                strVal = QString::number(
                    /* fVal       */ fValSign*fValAbs,
                    /* chFormat   */ 'e',
                    /* iPrecision */ iDigitsTrailing );
            }
            else
            {
                strVal = QString::number(
                    /* fVal       */ fValSign*fValAbs,
                    /* chFormat   */ 'f',
                    /* iPrecision */ iDigitsTrailing );
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
                if( formatResult == EFormatResultOk )
                {
                    if( iDigitsLeadingTmp + iDigitsTrailingTmp > iDigitsLeading + iDigitsTrailing )
                    {
                        if( iDigitsExponent > 0 )
                        {
                            strVal = QString::number(
                                /* fVal       */ fValSign*fValAbs,
                                /* chFormat   */ 'e',
                                /* iPrecision */ iDigitsTrailing-1 );
                        }
                        else
                        {
                            strVal = QString::number(
                                /* fVal       */ fValSign*fValAbs,
                                /* chFormat   */ 'f',
                                /* iPrecision */ iDigitsTrailing-1 );
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
    if( o_ppUnitVal != nullptr )
    {
        *o_ppUnitVal = const_cast<CUnit*>(pUnitVal);
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
EFormatResult ZS::PhysVal::formatValue(
    double         i_fVal,
    const CUnit*   i_pUnitVal,
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
    CUnit**        o_ppUnitVal,
    int*           o_piDigitsLeading,
    int*           o_piDigitsTrailing,
    int*           o_piDigitsExponent )
//------------------------------------------------------------------------------
{
    /*
    Converts a physical value into its string representation and/or into
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

    EFormatResult formatResult;
    int           iDigitsLeading;
    int           iDigitsTrailing;
    QString       strDelimiter = " ";

    formatResult = formatValue(
        /* fVal                    */ i_fVal,
        /* pUnitVal                */ i_pUnitVal,
        /* iDigitsMantissaMax      */ i_iDigitsMantissaMax,
        /* bDigitsAccuracyLimitsM. */ i_bDigitsAccuracyLimitsMantissa,
        /* iDigitsAccuracy         */ i_iDigitsAccuracy,
        /* iDigitsExponent         */ i_iDigitsExponent,
        /* bUseEngineeringFormat   */ i_bUseEngineeringFormat,
        /* pfVal                   */ o_pfVal,
        /* pstrVal                 */ o_pstrVal,
        /* ppUnitVal               */ o_ppUnitVal,
        /* piDigitsLeading         */ &iDigitsLeading,
        /* piDigitsTrailing        */ &iDigitsTrailing,
        /* piDigitsExponent        */ o_piDigitsExponent );

    if( formatResult == EFormatResultOk && o_pstrVal != nullptr )
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
EFormatResult ZS::PhysVal::parseValStr(
    const QString&     i_strVal,
    bool*              o_pbValOk,
    double*            o_pfVal,
    CUnitGrp**         io_ppUnitGrpVal,
    CUnit**            io_ppUnitVal,
    bool*              o_pbResOk,
    double*            o_pfRes,
    CUnitGrp**         io_ppUnitGrpRes,
    CUnit**            io_ppUnitRes,
    CIdxTreePhysSizes* i_pIdxTree )
//------------------------------------------------------------------------------
{
    EFormatResult formatResult = EFormatResultOk;
    QString       arSubStr[ESubStrCount];
    double        fVal;
    CUnit*        pUnitVal    = nullptr;
    CUnitGrp*     pUnitGrpVal = nullptr;
    double        fRes        = 0.0;
    CUnit*        pUnitRes    = nullptr;
    CUnitGrp*     pUnitGrpRes = nullptr;
    bool          bOk;

    if( o_pbValOk != nullptr )
    {
        *o_pbValOk = false;
    }
    if( o_pbResOk != nullptr )
    {
        *o_pbResOk = false;
    }

    // Extract sub strings from input string:
    formatResult = getSubStrings(i_strVal, arSubStr, i_pIdxTree);
    if( formatResult != EFormatResultOk )
    {
        return formatResult;
    }

    // Get value from sub strings:
    if( arSubStr[ESubStrVal].isEmpty() )
    {
        return EFormatResultError;
    }
    fVal = arSubStr[ESubStrVal].toDouble(&bOk);
    if( !bOk )
    {
        return EFormatResultError;
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
            if( io_ppUnitGrpVal == nullptr )
            {
                return EFormatResultError;
            }
            pUnitGrpVal = *io_ppUnitGrpVal;

            if( pUnitGrpVal != nullptr )
            {
                // Use "keyInTree" instead of "getName(IncludingParentNames)" as the path contains the
                // root nodes (UnitClassType) whereas the key starts with the name of the science field.
                arSubStr[ESubStrValUnitGrp] = pUnitGrpVal->keyInTree();

                // For historical reasons keep the comment:

                //// Please note that the following code is not really needed as the
                //// known reference to the unit group is used later on to find the
                //// unit instance. But the code and comments should state the facts
                //// around the main group name of the units.
                //if( pUnitGrpVal->classType() == EUnitClassTypePhysScienceFields )
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
        if( pUnitGrpVal != nullptr )
        {
            // .. the unit must be a child of the already known unit group.
            pUnitVal = pUnitGrpVal->findUnit(arSubStr[ESubStrValUnit]);
        }
        // If the group name of the unit was specified within the input string ...
        else if( i_pIdxTree != nullptr )
        {
            // .. we need to find both, the unit group and the unit by their names.
            pUnitVal = i_pIdxTree->findUnit( arSubStr[ESubStrValUnitGrp], arSubStr[ESubStrValUnit] );
        }
        if( pUnitVal == nullptr )
        {
            return EFormatResultError;
        }
        pUnitGrpVal = pUnitVal->unitGroup();
    }
    if( pUnitVal != nullptr && io_ppUnitVal != nullptr )
    {
        *io_ppUnitVal = pUnitVal;
        pUnitGrpVal = pUnitVal->unitGroup();
    }
    if( pUnitGrpVal != nullptr && io_ppUnitGrpVal != nullptr )
    {
        *io_ppUnitGrpVal = pUnitGrpVal;
    }

    // Get resolution (inaccuracy) from sub strings:
    if( !arSubStr[ESubStrRes].isEmpty() )
    {
        fRes = arSubStr[ESubStrRes].toDouble(&bOk);
        if( !bOk )
        {
            return EFormatResultError;
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
                if( io_ppUnitGrpRes != nullptr && (*io_ppUnitGrpRes)->classType() == EUnitClassTypeRatios )
                {
                    arSubStr[ESubStrResUnitGrp] = (*io_ppUnitGrpRes)->keyInTree();
                }
                else
                {
                    arSubStr[ESubStrResUnitGrp] = arSubStr[ESubStrValUnitGrp];
                }
            }
            if( arSubStr[ESubStrResUnitGrp].isEmpty() )
            {
                if( io_ppUnitGrpRes == nullptr )
                {
                    if( pUnitGrpVal == nullptr )
                    {
                        return EFormatResultError;
                    }
                    pUnitGrpRes = pUnitGrpVal;
                }
                else if( *io_ppUnitGrpRes == nullptr )
                {
                    pUnitGrpRes = pUnitGrpVal;
                }
                else
                {
                    pUnitGrpRes = *io_ppUnitGrpRes;
                }
                if( pUnitGrpRes != nullptr )
                {
                    // Use "keyInTree" instead of "getName(IncludingParentNames)" as the path contains the
                    // root nodes (UnitClassType) whereas the key starts with the name of the science field.
                    arSubStr[ESubStrResUnitGrp] = pUnitGrpRes->keyInTree();

                    // For historical reasons keep the comment:

                    //// Please note that the following code is not really needed as the
                    //// known reference to the unit group is used later on to find the
                    //// unit instance. But the code and comments should state the facts
                    //// around the main group name of the units.
                    //if( pUnitGrpRes->classType() == EUnitClassTypePhysScienceFields )
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
            if( pUnitGrpRes != nullptr )
            {
                // .. the unit must be a child of the already known unit group.
                pUnitRes = pUnitGrpRes->findUnit(arSubStr[ESubStrResUnit]);
            }
            // If the group name of the unit was specified within the input string ...
            else if( i_pIdxTree != nullptr )
            {
                // .. we need to find both, the unit group and the unit by their names.
                pUnitRes = i_pIdxTree->findUnit( arSubStr[ESubStrResUnitGrp], arSubStr[ESubStrResUnit] );
            }
            if( pUnitRes == nullptr )
            {
                return EFormatResultError;
            }
            pUnitGrpRes = pUnitRes->unitGroup();
        }
        if( pUnitRes != nullptr && io_ppUnitRes != nullptr )
        {
            *io_ppUnitRes = pUnitRes;
            pUnitGrpRes = pUnitRes->unitGroup();
        }
        if( pUnitGrpRes != nullptr && io_ppUnitGrpRes != nullptr )
        {
            *io_ppUnitGrpRes = pUnitGrpRes;
        }
    }

    return EFormatResultOk;

} // parseValStr

//------------------------------------------------------------------------------
EFormatResult ZS::PhysVal::getSubStrings(
    const QString&     i_strVal,
    QString*           io_arSubStr/*[ESubStrCount]*/,
    CIdxTreePhysSizes* i_pIdxTree )
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

    EFormatResult formatResult = EFormatResultOk;
    int           iSubStr = ESubStrVal;
    int           iParanthesisLeft = 0;
    int           iParanthesisRight = 0;
    int           ariSubStrBeg[ESubStrCount];
    int           ariSubStrLen[ESubStrCount];
    int           idxChar;
    QVector<int>  aridxSubStrUnitBegTmp; // Last entry for unit, previous entries for unit group
    QVector<int>  aridxSubStrUnitLenTmp; // Last entry for unit, previous entries for unit group
    int           idxTmp;

    memset( ariSubStrBeg, 0x00, ESubStrCount*sizeof(ariSubStrBeg[0]) );
    memset( ariSubStrLen, 0x00, ESubStrCount*sizeof(ariSubStrLen[0]) );

    // The numerical value will be extracted scanning the input string
    // starting with the first character.
    for( iSubStr = 0, idxChar = 0;
         (idxChar < i_strVal.length()) && (iSubStr < ESubStrCount) && (formatResult == EFormatResultOk);
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
                    for( ; idxChar < i_strVal.length(); idxChar++ )
                    {
                        if( isWhiteSpace(i_strVal[idxChar]) )
                        {
                            break;
                        }
                        if( i_strVal[idxChar] == c_chSymbolPlusMinus )
                        {
                            break;
                        }
                        #pragma message(__TODO__ BEGIN)
                        //if( i_strVal[idxChar] == QChar(i_pIdxTree == nullptr ? '.' : i_pIdxTree->nodeSeparator()) )
                        //{
                        //    aridxSubStrUnitLenTmp[idxTmp] = idxChar-aridxSubStrUnitBegTmp[idxTmp];
                        //    idxChar++;
                        //    idxTmp++;
                        //    aridxSubStrUnitBegTmp.resize(idxTmp+1);
                        //    aridxSubStrUnitLenTmp.resize(idxTmp+1);
                        //    aridxSubStrUnitBegTmp[idxTmp] = idxChar;
                        //}
                        #pragma message(__TODO__ END)
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
                        formatResult = EFormatResultError;
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
                    #pragma message(__TODO__ BEGIN)
                    //if( i_strVal[idxChar] == QChar(i_pIdxTree == nullptr ? '.' : i_pIdxTree->nodeSeparator()) )
                    //{
                    //    aridxSubStrUnitLenTmp[idxTmp] = idxChar-aridxSubStrUnitBegTmp[idxTmp];
                    //    idxChar++;
                    //    idxTmp++;
                    //    aridxSubStrUnitBegTmp.resize(idxTmp+1);
                    //    aridxSubStrUnitLenTmp.resize(idxTmp+1);
                    //    aridxSubStrUnitBegTmp[idxTmp] = idxChar;
                    //}
                    #pragma message(__TODO__ END)
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
struct SValueFormatProvider
*******************************************************************************/

/*==============================================================================
public: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
SValueFormatProvider::SValueFormatProvider() :
//------------------------------------------------------------------------------
    m_pUnitVal(nullptr),
    m_unitFindVal(EUnitFindNone),
    m_iValSubStrVisibility(EPhysValSubStrUnitSymbol),
    m_iDigitsMantissa(0),
    m_iDigitsExponent(0),
    m_bUseEngineeringFormat(false),
    m_iDigitsPerDigitGroup(0),
    m_pstrDigitsGroupDelimiter(nullptr),
    m_pstrDecimalPoint(nullptr),
    m_fRes(0.0),
    m_pUnitRes(nullptr),
    m_resType(EResTypeUndefined),
    m_unitFindRes(EUnitFindNone),
    m_iResSubStrVisibility(EPhysValSubStrNone),
    m_iDigitsAccuracy(0),
    m_bDigitsAccuracyLimitsMantissa(false)
{
} // default ctor

//------------------------------------------------------------------------------
SValueFormatProvider::SValueFormatProvider( const SValueFormatProvider& i_other ) :
//------------------------------------------------------------------------------
    m_pUnitVal(i_other.m_pUnitVal),
    m_unitFindVal(i_other.m_unitFindVal),
    m_iValSubStrVisibility(i_other.m_iValSubStrVisibility),
    m_iDigitsMantissa(i_other.m_iDigitsMantissa),
    m_iDigitsExponent(i_other.m_iDigitsExponent),
    m_bUseEngineeringFormat(i_other.m_bUseEngineeringFormat),
    m_iDigitsPerDigitGroup(i_other.m_iDigitsPerDigitGroup),
    m_pstrDigitsGroupDelimiter(nullptr),
    m_pstrDecimalPoint(nullptr),
    m_fRes(i_other.m_fRes),
    m_pUnitRes(i_other.m_pUnitRes),
    m_resType(i_other.m_resType),
    m_unitFindRes(i_other.m_unitFindRes),
    m_iResSubStrVisibility(i_other.m_iResSubStrVisibility),
    m_iDigitsAccuracy(i_other.m_iDigitsAccuracy),
    m_bDigitsAccuracyLimitsMantissa(i_other.m_bDigitsAccuracyLimitsMantissa)
{
    if( i_other.m_pstrDigitsGroupDelimiter != nullptr )
    {
        m_pstrDigitsGroupDelimiter = new QString(*i_other.m_pstrDigitsGroupDelimiter);
    }
    if( i_other.m_pstrDecimalPoint != nullptr )
    {
        m_pstrDecimalPoint = new QString(*i_other.m_pstrDecimalPoint);
    }

} // copy ctor

//------------------------------------------------------------------------------
SValueFormatProvider::SValueFormatProvider(
    CUnit*         i_pUnitVal,    // nullptr to indicate the value with the current or the "best unit".
    EUnitFind      i_unitFindVal, // Only evaluated if pUnit == nullptr
    int            i_iValSubStrVisibility,
    int            i_iDigitsMantissa,
    int            i_iDigitsExponent,
    bool           i_bUseEngineeringFormat,
    int            i_iDigitsPerDigitGroup,
    const QString* i_pstrDigitsGroupDelimiter,
    const QString* i_pstrDecimalPoint ) :
//------------------------------------------------------------------------------
    m_pUnitVal(i_pUnitVal),
    m_unitFindVal(i_unitFindVal),
    m_iValSubStrVisibility(i_iValSubStrVisibility),
    m_iDigitsMantissa(i_iDigitsMantissa),
    m_iDigitsExponent(i_iDigitsExponent),
    m_bUseEngineeringFormat(i_bUseEngineeringFormat),
    m_iDigitsPerDigitGroup(i_iDigitsPerDigitGroup),
    m_pstrDigitsGroupDelimiter(nullptr),
    m_pstrDecimalPoint(nullptr),
    m_fRes(0.0),
    m_pUnitRes(nullptr),
    m_resType(EResTypeUndefined),
    m_unitFindRes(EUnitFindNone),
    m_iResSubStrVisibility(EPhysValSubStrNone),
    m_iDigitsAccuracy(0),
    m_bDigitsAccuracyLimitsMantissa(false)
{
    if( i_pstrDigitsGroupDelimiter != nullptr )
    {
        m_pstrDigitsGroupDelimiter = new QString(*i_pstrDigitsGroupDelimiter);
    }
    if( i_pstrDecimalPoint != nullptr )
    {
        m_pstrDecimalPoint = new QString(*i_pstrDecimalPoint);
    }

} // ctor

//------------------------------------------------------------------------------
SValueFormatProvider::SValueFormatProvider(
    CUnit*         i_pUnitVal,    // nullptr to indicate the value with the current or the "best unit".
    EUnitFind      i_unitFindVal, // Only evaluated if pUnit == nullptr
    int            i_iValSubStrVisibility,
    int            i_iDigitsMantissa,
    int            i_iDigitsExponent,
    bool           i_bUseEngineeringFormat,
    int            i_iDigitsPerDigitGroup,
    const QString* i_pstrDigitsGroupDelimiter,
    const QString* i_pstrDecimalPoint,
    double         i_fRes,        // If set to != 0.0 this resolution is used instead of the physical values current resolution
    CUnit*         i_pUnitRes,    // nullptr to indicate the value with the current or the "best unit".
    EResType       i_resType,
    EUnitFind      i_unitFindRes, // Only evaluated if pUnit == nullptr
    int            i_iResSubStrVisibility ) :
//------------------------------------------------------------------------------
    m_pUnitVal(i_pUnitVal),
    m_unitFindVal(i_unitFindVal),
    m_iValSubStrVisibility(i_iValSubStrVisibility),
    m_iDigitsMantissa(i_iDigitsMantissa),
    m_iDigitsExponent(i_iDigitsExponent),
    m_bUseEngineeringFormat(i_bUseEngineeringFormat),
    m_iDigitsPerDigitGroup(i_iDigitsPerDigitGroup),
    m_pstrDigitsGroupDelimiter(nullptr),
    m_pstrDecimalPoint(nullptr),
    m_fRes(i_fRes),
    m_pUnitRes(i_pUnitRes),
    m_resType(i_resType),
    m_unitFindRes(i_unitFindRes),
    m_iResSubStrVisibility(i_iResSubStrVisibility),
    m_iDigitsAccuracy(0),
    m_bDigitsAccuracyLimitsMantissa(false)
{
    if( i_pstrDigitsGroupDelimiter != nullptr )
    {
        m_pstrDigitsGroupDelimiter = new QString(*i_pstrDigitsGroupDelimiter);
    }
    if( i_pstrDecimalPoint != nullptr )
    {
        m_pstrDecimalPoint = new QString(*i_pstrDecimalPoint);
    }

} // ctor

//------------------------------------------------------------------------------
SValueFormatProvider::SValueFormatProvider(
    CUnit*         i_pUnitVal,    // nullptr to indicate the value with the current or the "best unit".
    EUnitFind      i_unitFindVal, // Only evaluated if pUnit == nullptr
    int            i_iValSubStrVisibility,
    int            i_iDigitsMantissa,
    int            i_iDigitsExponent,
    bool           i_bUseEngineeringFormat,
    int            i_iDigitsPerDigitGroup,
    const QString* i_pstrDigitsGroupDelimiter,
    const QString* i_pstrDecimalPoint,
    int            i_iDigitsAccuracy,
    bool           i_bDigitsAccuracyLimitsMantissa ) :
//------------------------------------------------------------------------------
    m_pUnitVal(i_pUnitVal),
    m_unitFindVal(i_unitFindVal),
    m_iValSubStrVisibility(i_iValSubStrVisibility),
    m_iDigitsMantissa(i_iDigitsMantissa),
    m_iDigitsExponent(i_iDigitsExponent),
    m_bUseEngineeringFormat(i_bUseEngineeringFormat),
    m_iDigitsPerDigitGroup(i_iDigitsPerDigitGroup),
    m_pstrDigitsGroupDelimiter(nullptr),
    m_pstrDecimalPoint(nullptr),
    m_fRes(0.0),
    m_pUnitRes(nullptr),
    m_resType(EResTypeUndefined),
    m_unitFindRes(EUnitFindNone),
    m_iResSubStrVisibility(EPhysValSubStrNone),
    m_iDigitsAccuracy(i_iDigitsAccuracy),
    m_bDigitsAccuracyLimitsMantissa(i_bDigitsAccuracyLimitsMantissa)
{
    if( i_pstrDigitsGroupDelimiter != nullptr )
    {
        m_pstrDigitsGroupDelimiter = new QString(*i_pstrDigitsGroupDelimiter);
    }
    if( i_pstrDecimalPoint != nullptr )
    {
        m_pstrDecimalPoint = new QString(*i_pstrDecimalPoint);
    }

} // ctor

//------------------------------------------------------------------------------
SValueFormatProvider::~SValueFormatProvider()
//------------------------------------------------------------------------------
{
    try
    {
        delete m_pstrDigitsGroupDelimiter;
    }
    catch(...)
    {
    }
    m_pstrDigitsGroupDelimiter = nullptr;

    try
    {
        delete m_pstrDecimalPoint;
    }
    catch(...)
    {
    }
    m_pstrDecimalPoint = nullptr;

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
SValueFormatProvider& SValueFormatProvider::operator = ( const SValueFormatProvider& i_other )
//------------------------------------------------------------------------------
{
    if( &i_other != this )
    {
        delete m_pstrDigitsGroupDelimiter;
        m_pstrDigitsGroupDelimiter = nullptr;
        delete m_pstrDecimalPoint;
        m_pstrDecimalPoint = nullptr;

        m_pUnitVal = i_other.m_pUnitVal;
        m_unitFindVal = i_other.m_unitFindVal;
        m_iValSubStrVisibility = i_other.m_iValSubStrVisibility;
        m_iDigitsMantissa = i_other.m_iDigitsMantissa;
        m_iDigitsExponent = i_other.m_iDigitsExponent;
        m_bUseEngineeringFormat = i_other.m_bUseEngineeringFormat;
        m_iDigitsPerDigitGroup = i_other.m_iDigitsPerDigitGroup;
        m_fRes = i_other.m_fRes;
        m_pUnitRes = i_other.m_pUnitRes;
        m_resType = i_other.m_resType;
        m_unitFindRes = i_other.m_unitFindRes;
        m_iResSubStrVisibility = i_other.m_iResSubStrVisibility;
        m_iDigitsAccuracy = i_other.m_iDigitsAccuracy;
        m_bDigitsAccuracyLimitsMantissa = i_other.m_bDigitsAccuracyLimitsMantissa;

        if( i_other.m_pstrDigitsGroupDelimiter != nullptr )
        {
            m_pstrDigitsGroupDelimiter = new QString(*i_other.m_pstrDigitsGroupDelimiter);
        }
        if( i_other.m_pstrDecimalPoint != nullptr )
        {
            m_pstrDecimalPoint = new QString(*i_other.m_pstrDecimalPoint);
        }
    }

    return *this;

} // operator =

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool SValueFormatProvider::hasRes() const
//------------------------------------------------------------------------------
{
    if( m_fRes > 0.0 || m_iDigitsAccuracy > 0 )
    {
        return true;
    }
    return false;
}


/*******************************************************************************
class CPhysVal
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_resType)
{
} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( CUnitGrp* i_pUnitGrp, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pUnitGrp),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_pUnitGrp,i_resType)
{
    if( m_pUnitGrp != nullptr && m_pUnitGrp->classType() == EUnitClassTypePhysScienceFields )
    {
        m_pUnit = dynamic_cast<CPhysSize*>(m_pUnitGrp)->getSIUnit();
    }
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( CUnit* i_pUnit, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_pUnit,i_resType)
{
    if( i_pUnit != nullptr )
    {
        m_pUnitGrp = i_pUnit->unitGroup();
        m_pUnit = i_pUnit;
    }
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( double i_fVal, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Valid),
    m_fVal(i_fVal),
    m_physValRes(i_resType)
{
} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( double i_fVal, double i_fResVal, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Valid),
    m_fVal(i_fVal),
    m_physValRes(i_fResVal,i_resType)
{
} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( double i_fVal, CUnitGrp* i_pUnitGrp, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pUnitGrp),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Valid),
    m_fVal(i_fVal),
    m_physValRes(i_pUnitGrp,i_resType)
{
    if( i_pUnitGrp->classType() == EUnitClassTypePhysScienceFields )
    {
        m_pUnit = dynamic_cast<CPhysSize*>(i_pUnitGrp)->getSIUnit();
    }
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( double i_fVal, CUnit* i_pUnit, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Valid),
    m_fVal(i_fVal),
    m_physValRes(i_pUnit,i_resType)
{
    if( i_pUnit != nullptr )
    {
        m_pUnitGrp = i_pUnit->unitGroup();
        m_pUnit = i_pUnit;
    }
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( double i_fVal, CUnit* i_pUnit, double i_fResVal, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Valid),
    m_fVal(i_fVal),
    m_physValRes(i_fResVal,i_pUnit,i_resType)
{
    if( i_pUnit != nullptr )
    {
        m_pUnitGrp = i_pUnit->unitGroup();
        m_pUnit = i_pUnit;
    }
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( double i_fVal, CUnit* i_pUnitVal, double i_fResVal, CUnit* i_pUnitRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Valid),
    m_fVal(i_fVal),
    m_physValRes(i_fResVal,i_pUnitRes,i_resType)
{
    if( i_pUnitVal != nullptr )
    {
        m_pUnitGrp = i_pUnitVal->unitGroup();
        m_pUnit = i_pUnitVal;
    }
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( double i_fVal, CUnit* i_pUnitVal, const CPhysValRes& i_physValRes ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Valid),
    m_fVal(i_fVal),
    m_physValRes(i_physValRes)
{
    if( i_pUnitVal != nullptr )
    {
        m_pUnitGrp = i_pUnitVal->unitGroup();
        m_pUnit = i_pUnitVal;
    }
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( double i_fVal, CUnitRatio* i_pUnitRatio, double i_fResVal, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pUnitRatio->unitGroup()),
    m_pUnit(i_pUnitRatio),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Valid),
    m_fVal(i_fVal),
    m_physValRes(i_fResVal,i_pUnitRatio,i_resType)
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal(  double i_fVal, CUnitRatio* i_pUnitRatioVal, double i_fResVal, CUnitRatio* i_pUnitRatioRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pUnitRatioVal->unitGroup()),
    m_pUnit(i_pUnitRatioVal),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Valid),
    m_fVal(i_fVal),
    m_physValRes(i_fResVal,i_pUnitRatioRes,i_resType)
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( double i_fVal, CUnitRatio* i_pUnitRatioVal, const CPhysValRes& i_physValRes ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pUnitRatioVal->unitGroup()),
    m_pUnit(i_pUnitRatioVal),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Valid),
    m_fVal(i_fVal),
    m_physValRes(i_physValRes)
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( double i_fVal, CUnitDataQuantity* i_pUnitDataQuantity, double i_fResVal, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pUnitDataQuantity->unitGroup()),
    m_pUnit(i_pUnitDataQuantity),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Valid),
    m_fVal(i_fVal),
    m_physValRes(i_fResVal,i_pUnitDataQuantity,i_resType)
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( double i_fVal, CUnitDataQuantity* i_pUnitDataQuantity, double i_fResVal, CUnitRatio* i_pUnitRatioRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pUnitDataQuantity->unitGroup()),
    m_pUnit(i_pUnitDataQuantity),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Valid),
    m_fVal(i_fVal),
    m_physValRes(i_fResVal,i_pUnitRatioRes,i_resType)
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( double i_fVal, CUnitDataQuantity* i_pUnitDataQuantity, const CPhysValRes& i_physValRes ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pUnitDataQuantity->unitGroup()),
    m_pUnit(i_pUnitDataQuantity),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Valid),
    m_fVal(i_fVal),
    m_physValRes(i_physValRes)
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( double i_fVal, CPhysUnit* i_pPhysUnit, double i_fResVal, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pPhysUnit->getPhysSize()),
    m_pUnit(i_pPhysUnit),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Valid),
    m_fVal(i_fVal),
    m_physValRes(i_fResVal,i_pPhysUnit,i_resType)
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( double i_fVal, CPhysUnit* i_pPhysUnitVal, double i_fResVal, CUnitRatio* i_pUnitRatioRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pPhysUnitVal->getPhysSize()),
    m_pUnit(i_pPhysUnitVal),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Valid),
    m_fVal(i_fVal),
    m_physValRes(i_fResVal,i_pUnitRatioRes,i_resType)
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( double i_fVal, CPhysUnit* i_pPhysUnitVal, double i_fResVal, CPhysUnit* i_pPhysUnitRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pPhysUnitVal->getPhysSize()),
    m_pUnit(i_pPhysUnitVal),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Valid),
    m_fVal(i_fVal),
    m_physValRes(i_fResVal,i_pPhysUnitRes,i_resType)
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( double i_fVal, CPhysUnit* i_pPhysUnitVal, const CPhysValRes& i_physValRes ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pPhysUnitVal->getPhysSize()),
    m_pUnit(i_pPhysUnitVal),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Valid),
    m_fVal(i_fVal),
    m_physValRes(i_physValRes)
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_resType)
{
    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, double i_fResVal, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_fResVal,i_resType)
{
    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, CUnit* i_pUnit, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_pUnit,i_resType)
{
    if( i_pUnit != nullptr )
    {
        m_pUnitGrp = i_pUnit->unitGroup();
        m_pUnit = i_pUnit;
    }
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, CUnit* i_pUnit, double i_fResVal, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_fResVal,i_pUnit,i_resType)
{
    if( i_pUnit != nullptr )
    {
        m_pUnitGrp = i_pUnit->unitGroup();
        m_pUnit = i_pUnit;
    }
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, CUnit* i_pUnitVal, double i_fResVal, CUnit* i_pUnitRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_fResVal,i_pUnitRes,i_resType)
{
    if( i_pUnitVal != nullptr )
    {
        m_pUnitGrp = i_pUnitVal->unitGroup();
        m_pUnit = i_pUnitVal;
    }
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, CUnit* i_pUnitVal, const CPhysValRes& i_physValRes ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_physValRes)
{
    if( i_pUnitVal != nullptr )
    {
        m_pUnitGrp = i_pUnitVal->unitGroup();
        m_pUnit = i_pUnitVal;
    }
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, CUnitGrp* i_pUnitGrp, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pUnitGrp),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_resType)
{
    if( i_pUnitGrp->classType() == EUnitClassTypePhysScienceFields )
    {
        m_pUnit = dynamic_cast<CPhysSize*>(i_pUnitGrp)->getSIUnit();
    }
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    m_physValRes.setUnitGroup(m_pUnitGrp);

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, CUnitGrp* i_pUnitGrp, double i_fResVal, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pUnitGrp),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_fResVal,i_resType)
{
    if( i_pUnitGrp->classType() == EUnitClassTypePhysScienceFields )
    {
        m_pUnit = dynamic_cast<CPhysSize*>(i_pUnitGrp)->getSIUnit();
    }
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    m_physValRes.setUnitGroup(m_pUnitGrp);

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, CUnitGrp* i_pUnitGrpVal, CUnitGrp* i_pUnitGrpRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pUnitGrpVal),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_pUnitGrpRes,i_resType)
{
    if( i_pUnitGrpVal->classType() == EUnitClassTypePhysScienceFields )
    {
        m_pUnit = dynamic_cast<CPhysSize*>(i_pUnitGrpVal)->getSIUnit();
    }
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, CUnitGrp* i_pUnitGrpVal, double i_fResVal, CUnitRatio* i_pUnitRatioRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pUnitGrpVal),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_fResVal,i_pUnitRatioRes,i_resType)
{
    if( i_pUnitGrpVal->classType() == EUnitClassTypePhysScienceFields )
    {
        m_pUnit = dynamic_cast<CPhysSize*>(i_pUnitGrpVal)->getSIUnit();
    }
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, CUnitGrp* i_pUnitGrpVal, double i_fResVal, CUnitDataQuantity* i_pUnitDataQuantityRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pUnitGrpVal),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_fResVal,i_pUnitDataQuantityRes,i_resType)
{
    if( i_pUnitGrpVal->classType() == EUnitClassTypePhysScienceFields )
    {
        m_pUnit = dynamic_cast<CPhysSize*>(i_pUnitGrpVal)->getSIUnit();
    }
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, CUnitGrp* i_pUnitGrpVal, double i_fResVal, CPhysUnit* i_pPhysUnitRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pUnitGrpVal),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_fResVal,i_pPhysUnitRes,i_resType)
{
    if( i_pUnitGrpVal->classType() == EUnitClassTypePhysScienceFields )
    {
        m_pUnit = dynamic_cast<CPhysSize*>(i_pUnitGrpVal)->getSIUnit();
    }
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, CUnitGrp* i_pUnitGrpVal, const CPhysValRes& i_physValRes ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pUnitGrpVal),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_physValRes)
{
    if( i_pUnitGrpVal->classType() == EUnitClassTypePhysScienceFields )
    {
        m_pUnit = dynamic_cast<CPhysSize*>(i_pUnitGrpVal)->getSIUnit();
    }
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, CUnitRatio* i_pUnitRatio, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pUnitRatio->unitGroup()),
    m_pUnit(i_pUnitRatio),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_pUnitRatio,i_resType)
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, CUnitRatio* i_pUnitRatio, double i_fResVal, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pUnitRatio->unitGroup()),
    m_pUnit(i_pUnitRatio),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_fResVal,i_pUnitRatio,i_resType)
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, CUnitRatio* i_pUnitRatioVal, double i_fResVal, CUnitRatio* i_pUnitRatioRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pUnitRatioVal->unitGroup()),
    m_pUnit(i_pUnitRatioVal),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_fResVal,i_pUnitRatioRes,i_resType)
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, CUnitRatio* i_pUnitRatioVal, const CPhysValRes& i_physValRes ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pUnitRatioVal->unitGroup()),
    m_pUnit(i_pUnitRatioVal),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_physValRes)
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, CUnitDataQuantity* i_pUnitDataQuantity, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pUnitDataQuantity->unitGroup()),
    m_pUnit(i_pUnitDataQuantity),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_pUnitDataQuantity,i_resType)
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, CUnitDataQuantity* i_pUnitDataQuantity, double i_fResVal, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pUnitDataQuantity->unitGroup()),
    m_pUnit(i_pUnitDataQuantity),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_fResVal,i_pUnitDataQuantity,i_resType)
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, CUnitDataQuantity* i_pUnitDataQuantity, double i_fResVal, CUnitRatio* i_pUnitRatioRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pUnitDataQuantity->unitGroup()),
    m_pUnit(i_pUnitDataQuantity),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_fResVal,i_pUnitRatioRes,i_resType)
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, CUnitDataQuantity* i_pUnitDataQuantity, double i_fResVal, CUnitDataQuantity* i_pUnitDataQuantityRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pUnitDataQuantity->unitGroup()),
    m_pUnit(i_pUnitDataQuantity),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_fResVal,i_pUnitDataQuantityRes,i_resType)
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, CUnitDataQuantity* i_pUnitDataQuantity, const CPhysValRes& i_physValRes ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pUnitDataQuantity->unitGroup()),
    m_pUnit(i_pUnitDataQuantity),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_physValRes)
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, CPhysUnit* i_pPhysUnit, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pPhysUnit->getPhysSize()),
    m_pUnit(i_pPhysUnit),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_pPhysUnit,i_resType)
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, CPhysUnit* i_pPhysUnit, double i_fResVal, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pPhysUnit->getPhysSize()),
    m_pUnit(i_pPhysUnit),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_fResVal,i_pPhysUnit,i_resType)
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, CPhysUnit* i_pPhysUnitVal, double i_fResVal, CUnitRatio* i_pUnitRatioRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pPhysUnitVal->getPhysSize()),
    m_pUnit(i_pPhysUnitVal),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_fResVal,i_pUnitRatioRes,i_resType)
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, CPhysUnit* i_pPhysUnitVal, double i_fResVal, CPhysUnit* i_pPhysUnitRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pPhysUnitVal->getPhysSize()),
    m_pUnit(i_pPhysUnitVal),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_fResVal,i_pPhysUnitRes,i_resType)
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, CPhysUnit* i_pPhysUnitVal, const CPhysValRes& i_physValRes ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pPhysUnitVal->getPhysSize()),
    m_pUnit(i_pPhysUnitVal),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_physValRes)
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const CPhysVal& i_physVal ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_physVal.m_pUnitGrp),
    m_pUnit(i_physVal.m_pUnit),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(i_physVal.m_validity),
    m_fVal(i_physVal.m_fVal),
    m_physValRes(i_physVal.m_physValRes)
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

} // copy ctor

//------------------------------------------------------------------------------
CPhysVal::~CPhysVal()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CPhysVal::operator == ( const CPhysVal& i_physValOther ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    if( m_strUnitGrpKey != i_physValOther.m_strUnitGrpKey )
    {
        bEqual = false;
    }
    else if( m_validity != i_physValOther.m_validity )
    {
        bEqual = false;
    }
    else
    {
        double fValOther = i_physValOther.getVal();
        double fValThis  = getVal(i_physValOther.m_pUnit);

        //if( i_physValOther.m_pUnit == nullptr )
        //{
        //    fValThis = getVal();
        //}
        //else
        //{
        //    fValThis = getVal(i_physValOther.m_pUnit);
        //}

        if( fValThis != fValOther )
        {
            bEqual = false;
        }
    }
    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
bool CPhysVal::operator != ( const CPhysVal& i_physValOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_physValOther);
}

//------------------------------------------------------------------------------
bool CPhysVal::operator < ( const CPhysVal& i_physValOther ) const
//------------------------------------------------------------------------------
{
    if( m_pUnitGrp != i_physValOther.m_pUnitGrp )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_physValOther);
        QString strAddErrInfo = physValThis.toString() + " < " + physValOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    double fValOther = i_physValOther.getVal();
    double fValThis  = getVal(i_physValOther.m_pUnit);

    if( fValThis < fValOther )
    {
        return true;
    }
    return false;

} // operator <

//------------------------------------------------------------------------------
bool CPhysVal::operator > ( const CPhysVal& i_physValOther ) const
//------------------------------------------------------------------------------
{
    if( m_pUnitGrp != i_physValOther.m_pUnitGrp )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_physValOther);
        QString strAddErrInfo = physValThis.toString() + " > " + physValOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    double fValOther = i_physValOther.getVal();
    double fValThis  = getVal(i_physValOther.m_pUnit);

    if( fValThis > fValOther )
    {
        return true;
    }
    return false;

} // operator >

//------------------------------------------------------------------------------
bool CPhysVal::operator <= ( const CPhysVal& i_physValOther ) const
//------------------------------------------------------------------------------
{
    if( m_pUnitGrp != i_physValOther.m_pUnitGrp )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_physValOther);
        QString strAddErrInfo = physValThis.toString() + " <= " + physValOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    double fValOther = i_physValOther.getVal();
    double fValThis  = getVal(i_physValOther.m_pUnit);

    if( fValThis <= fValOther )
    {
        return true;
    }
    return false;

} // operator <=

//------------------------------------------------------------------------------
bool CPhysVal::operator >= ( const CPhysVal& i_physValOther ) const
//------------------------------------------------------------------------------
{
    if( m_pUnitGrp != i_physValOther.m_pUnitGrp )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_physValOther);
        QString strAddErrInfo = physValThis.toString() + " >= " + physValOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    double fValOther = i_physValOther.getVal();
    double fValThis  = getVal(i_physValOther.m_pUnit);

    if( fValThis >= fValOther )
    {
        return true;
    }
    return false;

} // operator >=

//------------------------------------------------------------------------------
CPhysVal& CPhysVal::operator = ( const CPhysVal& i_physValNew )
//------------------------------------------------------------------------------
{
    m_pUnitGrp      = i_physValNew.m_pUnitGrp;
    m_pUnit         = i_physValNew.m_pUnit;
    m_strUnitGrpKey = i_physValNew.m_strUnitGrpKey;
    m_strUnitKey    = i_physValNew.m_strUnitKey;
    m_validity      = i_physValNew.m_validity;
    m_fVal          = i_physValNew.m_fVal;
    m_physValRes    = i_physValNew.m_physValRes;

    return *this;

} // operator =

//------------------------------------------------------------------------------
CPhysVal CPhysVal::operator + ( const CPhysVal& i_physValOp ) const
//------------------------------------------------------------------------------
{
    if( m_pUnitGrp != i_physValOp.m_pUnitGrp )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_physValOp);
        QString strAddErrInfo = physValThis.toString() + " + " + physValOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    CPhysVal physValSum(*this);

    physValSum += i_physValOp;

    return physValSum;

} // operator +

//------------------------------------------------------------------------------
CPhysVal& CPhysVal::operator += ( const CPhysVal& i_physValOp )
//------------------------------------------------------------------------------
{
    if( m_pUnitGrp != i_physValOp.m_pUnitGrp )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_physValOp);
        QString strAddErrInfo = physValThis.toString() + " += " + physValOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    // Same unit group, so both units are either nullptr or not.
    if( m_pUnit == nullptr )
    {
        m_fVal += i_physValOp.getVal();

        // TODO: operator for resolution missing ...
        if( m_physValRes.isValid() )
        {
            m_physValRes += i_physValOp.m_physValRes;
        }
    }
    else
    {
        CUnit* pUnitOp = i_physValOp.getUnit();

        if( m_pUnit->isLogarithmic() && !pUnitOp->isLogarithmic() )
        {
            QString strAddErrInfo = "Val:" + m_pUnit->keyInTree() + ", Res:" + pUnitOp->keyInTree();
            throw CUnitConversionException( __FILE__, __LINE__, EResultMixedLinLogInMathOp, strAddErrInfo );
        }
        if( pUnitOp->isLogarithmic() )
        {
            double fFactor = pow( 10.0, i_physValOp.getVal()/pUnitOp->logarithmicFactor() );

            m_fVal *= fFactor;

            if( m_physValRes.isValid() )
            {
                m_physValRes = m_physValRes * fFactor;
            }
        }
        else
        {
            m_fVal += i_physValOp.getVal(m_pUnit);

            if( m_physValRes.isValid() )
            {
                m_physValRes += m_physValRes;
            }
        }
    }
    return *this;

} // operator +=

//------------------------------------------------------------------------------
CPhysVal CPhysVal::operator - ( const CPhysVal& i_physValOp ) const
//------------------------------------------------------------------------------
{
    if( m_pUnitGrp != i_physValOp.m_pUnitGrp )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_physValOp);
        QString  strAddErrInfo = physValThis.toString() + " - " + physValOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    CPhysVal physValSum(*this);

    physValSum -= i_physValOp;

    return physValSum;

} // operator -

//------------------------------------------------------------------------------
CPhysVal& CPhysVal::operator -= ( const CPhysVal& i_physValOp )
//------------------------------------------------------------------------------
{
    if( m_pUnitGrp != i_physValOp.m_pUnitGrp )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_physValOp);
        QString  strAddErrInfo = physValThis.toString() + " - " + physValOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    // Same unit group, so both units are either nullptr or not.
    if( m_pUnit == nullptr )
    {
        m_fVal -= i_physValOp.getVal();

        if( m_physValRes.isValid() )
        {
            m_physValRes += i_physValOp.m_physValRes;
        }
    }
    else
    {
        CUnit* pUnitOp = i_physValOp.getUnit();

        if( m_pUnit->isLogarithmic() && !pUnitOp->isLogarithmic() )
        {
            QString strAddErrInfo = "Val:" + m_pUnit->keyInTree() + ", Res:" + pUnitOp->keyInTree();
            throw CUnitConversionException( __FILE__, __LINE__, EResultMixedLinLogInMathOp, strAddErrInfo );
        }
        if( pUnitOp->isLogarithmic() )
        {
            double fFactor = pow( 10.0, i_physValOp.getVal()/pUnitOp->logarithmicFactor() );

            m_fVal /= fFactor;

            if( m_physValRes.isValid() )
            {
                m_physValRes = m_physValRes * fFactor;
            }
        }
        else
        {
            m_fVal -= i_physValOp.getVal(m_pUnit);

            if( m_physValRes.isValid() )
            {
                m_physValRes += m_physValRes;
            }
        }
    }
    return *this;

} // operator -=

//------------------------------------------------------------------------------
bool CPhysVal::operator == ( double i_fValOther ) const
//------------------------------------------------------------------------------
{
    if( m_pUnit == nullptr && m_fVal == i_fValOther )
    {
        return true;
    }
    return false;

} // operator ==

//------------------------------------------------------------------------------
bool CPhysVal::operator != ( double i_fValOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_fValOther);

} // operator !=

//------------------------------------------------------------------------------
bool CPhysVal::operator < ( double i_fValOther ) const
//------------------------------------------------------------------------------
{
    if( m_pUnit != nullptr )
    {
        QString strAddErrInfo = "Src:" + getUnitGroupName() + ", Dst:NoUnit";
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    if( m_fVal < i_fValOther )
    {
        return true;
    }
    return false;

} // operator <

//------------------------------------------------------------------------------
bool CPhysVal::operator > ( double i_fValOther ) const
//------------------------------------------------------------------------------
{
    if( m_pUnit != nullptr )
    {
        QString strAddErrInfo = "Src:" + getUnitGroupName() + ", Dst:NoUnit";
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    if( m_fVal > i_fValOther )
    {
        return true;
    }
    return false;

} // operator >

//------------------------------------------------------------------------------
bool CPhysVal::operator <= ( double i_fValOther ) const
//------------------------------------------------------------------------------
{
    if( m_pUnit != nullptr )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_fValOther);
        QString  strAddErrInfo = physValThis.toString() + " <= " + physValOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    if( m_fVal <= i_fValOther )
    {
        return true;
    }
    return false;

} // operator <=

//------------------------------------------------------------------------------
bool CPhysVal::operator >= ( double i_fValOther ) const
//------------------------------------------------------------------------------
{
    if( m_pUnit != nullptr )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_fValOther);
        QString  strAddErrInfo = physValThis.toString() + " >= " + physValOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    if( m_fVal >= i_fValOther )
    {
        return true;
    }
    return false;

} // operator >=

//------------------------------------------------------------------------------
CPhysVal& CPhysVal::operator = ( const double i_fValNew )
//------------------------------------------------------------------------------
{
    setVal(i_fValNew);

    return *this;

} // operator =

//------------------------------------------------------------------------------
CPhysVal CPhysVal::operator + ( double i_fOp ) const
//------------------------------------------------------------------------------
{
    if( m_pUnit != nullptr )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_fOp);
        QString  strAddErrInfo = physValThis.toString() + " + " + physValOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    CPhysVal physValSum(*this);

    physValSum.m_fVal += i_fOp;

    return physValSum;

} // operator +

//------------------------------------------------------------------------------
CPhysVal& CPhysVal::operator += ( double i_fOp )
//------------------------------------------------------------------------------
{
    if( m_pUnit != nullptr )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_fOp);
        QString  strAddErrInfo = physValThis.toString() + " += " + physValOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    m_fVal += i_fOp;

    return *this;

} // operator +=

//------------------------------------------------------------------------------
CPhysVal CPhysVal::operator - ( double i_fOp ) const
//------------------------------------------------------------------------------
{
    if( m_pUnit != nullptr )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_fOp);
        QString  strAddErrInfo = physValThis.toString() + " - " + physValOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    CPhysVal physValSum(*this);

    physValSum.m_fVal -= i_fOp;

    return physValSum;

} // operator -

//------------------------------------------------------------------------------
CPhysVal& CPhysVal::operator -= ( double i_fOp )
//------------------------------------------------------------------------------
{
    if( m_pUnit != nullptr )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_fOp);
        QString  strAddErrInfo = physValThis.toString() + " -= " + physValOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    m_fVal -= i_fOp;

    return *this;

} // operator -=

//------------------------------------------------------------------------------
CPhysVal CPhysVal::operator * ( double i_fOp ) const
//------------------------------------------------------------------------------
{
    CPhysVal physValMul(*this);

    physValMul.m_fVal *= i_fOp;

    return physValMul;

} // operator *

//------------------------------------------------------------------------------
CPhysVal& CPhysVal::operator *= ( double i_fOp )
//------------------------------------------------------------------------------
{
    m_fVal *= i_fOp;

    return *this;

} // operator *=

//------------------------------------------------------------------------------
CPhysVal CPhysVal::operator / ( double i_fOp ) const
//------------------------------------------------------------------------------
{
    CPhysVal physValDif(*this);

    physValDif.m_fVal /= i_fOp;

    return physValDif;

} // operator /

//------------------------------------------------------------------------------
CPhysVal& CPhysVal::operator /= ( double i_fOp )
//------------------------------------------------------------------------------
{
    m_fVal /= i_fOp;

    return *this;

} // operator /=

//------------------------------------------------------------------------------
bool CPhysVal::operator == ( const QString& i_valOther ) const
//------------------------------------------------------------------------------
{
    CPhysVal physValOther(*this);
    physValOther.setVal(i_valOther);
    return (*this == physValOther);
}

//------------------------------------------------------------------------------
bool CPhysVal::operator != ( const QString& i_valOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_valOther);

} // operator !=

//------------------------------------------------------------------------------
bool CPhysVal::operator < ( const QString& i_valOther ) const
//------------------------------------------------------------------------------
{
    CPhysVal physValOther(*this);
    physValOther.setVal(i_valOther);
    return (*this < physValOther);

} // operator <

//------------------------------------------------------------------------------
bool CPhysVal::operator > ( const QString& i_valOther ) const
//------------------------------------------------------------------------------
{
    CPhysVal physValOther(*this);
    physValOther.setVal(i_valOther);
    return (*this > physValOther);

} // operator >

//------------------------------------------------------------------------------
bool CPhysVal::operator <= ( const QString& i_valOther ) const
//------------------------------------------------------------------------------
{
    CPhysVal physValOther(*this);
    physValOther.setVal(i_valOther);
    return (*this <= physValOther);

} // operator <=

//------------------------------------------------------------------------------
bool CPhysVal::operator >= ( const QString& i_valOther ) const
//------------------------------------------------------------------------------
{
    CPhysVal physValOther(*this);
    physValOther.setVal(i_valOther);
    return (*this >= physValOther);

} // operator >=

//------------------------------------------------------------------------------
CPhysVal& CPhysVal::operator = ( const QString& i_valNew )
//------------------------------------------------------------------------------
{
    setVal(i_valNew);

    return *this;

} // operator =

//------------------------------------------------------------------------------
CPhysVal CPhysVal::operator + ( const QString& i_valOp ) const
//------------------------------------------------------------------------------
{
    CPhysVal physValSum(*this);
    CPhysVal physValOp;

    physValOp.setUnitGroup(physValSum.unitGroup());
    physValOp.setVal(i_valOp);

    physValSum += physValOp;

    return physValSum;

} // operator =

//------------------------------------------------------------------------------
CPhysVal& CPhysVal::operator += ( const QString& i_valOp )
//------------------------------------------------------------------------------
{
    CPhysVal physValOp;

    physValOp.setUnitGroup(m_pUnitGrp);
    physValOp.setVal(i_valOp);

    *this += physValOp;

    return *this;

} // operator +=

//------------------------------------------------------------------------------
CPhysVal CPhysVal::operator - ( const QString& i_valOp ) const
//------------------------------------------------------------------------------
{
    CPhysVal physValSum(*this);
    CPhysVal physValOp;

    physValOp.setUnitGroup(physValSum.unitGroup());
    physValOp.setVal(i_valOp);

    physValSum -= physValOp;

    return physValSum;

} // operator =

//------------------------------------------------------------------------------
CPhysVal& CPhysVal::operator -= ( const QString& i_valOp )
//------------------------------------------------------------------------------
{
    CPhysVal physValOp;

    physValOp.setUnitGroup(m_pUnitGrp);
    physValOp.setVal(i_valOp);

    *this -= physValOp;

    return *this;

} // operator +=

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysVal::setValidity( EValueValidity i_validity )
//------------------------------------------------------------------------------
{
    m_validity = i_validity;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysVal::setUnitGroup( CUnitGrp* i_pUnitGrp )
//------------------------------------------------------------------------------
{
    m_pUnitGrp = i_pUnitGrp;

    if( m_pUnitGrp == nullptr )
    {
        m_pUnit = nullptr;
    }
    else if( m_pUnitGrp->classType() == EUnitClassTypePhysScienceFields )
    {
        m_pUnit = dynamic_cast<CPhysSize*>(m_pUnitGrp)->getSIUnit();
    }

    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    m_physValRes.setUnitGroup(i_pUnitGrp);

} // setUnitGroup

//------------------------------------------------------------------------------
void CPhysVal::setUnitGroupKey( const QString& i_strUnitGrpKey )
//------------------------------------------------------------------------------
{
    if( m_pUnitGrp != nullptr && m_pUnitGrp->keyInTree() != i_strUnitGrpKey )
    {
        QString strMethod, strArgs, strErr;
        strMethod = "CPhysVal::setUnitGroupKey";
        strArgs   = "UnitGrpKey: " + i_strUnitGrpKey;
        strErr    = "UnitGrpKey is different from key of already set unit group " + m_pUnitGrp->keyInTree();
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, strMethod + "( " + strArgs + " ): " + strErr );
    }

    m_strUnitGrpKey = i_strUnitGrpKey;

    m_physValRes.setUnitGroupKey(i_strUnitGrpKey);

} // setUnitGroupKey

//------------------------------------------------------------------------------
QString CPhysVal::getUnitGroupName( bool i_bInsertParentNames ) const
//------------------------------------------------------------------------------
{
    QString strName;

    if( m_pUnitGrp == nullptr )
    {
        strName = "NoUnit";
    }
    else
    {
        strName = m_pUnitGrp->keyInTree();
    }
    return strName;

} // getUnitGroupName

//------------------------------------------------------------------------------
CPhysSize* CPhysVal::getPhysSize() const
//------------------------------------------------------------------------------
{
    return dynamic_cast<CPhysSize*>(m_pUnitGrp);
}

//------------------------------------------------------------------------------
void CPhysVal::setUnit( CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    m_pUnit = i_pUnit;

    if( m_pUnit == nullptr )
    {
        m_pUnitGrp = nullptr;
    }
    else if( m_pUnit->classType() == EUnitClassTypeRatios )
    {
        m_pUnitGrp = dynamic_cast<CUnitGrpRatio*>(m_pUnit->unitGroup());
    }
    else if( m_pUnit->classType() == EUnitClassTypeDataQuantity )
    {
        m_pUnitGrp = dynamic_cast<CUnitGrpDataQuantity*>(m_pUnit->unitGroup());
    }
    else if( m_pUnit->classType() == EUnitClassTypePhysScienceFields )
    {
        m_pUnitGrp = dynamic_cast<CPhysUnit*>(m_pUnit)->getPhysSize();
    }

    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    m_physValRes.setUnit(i_pUnit);

} // setUnit

//------------------------------------------------------------------------------
void CPhysVal::setUnitKey( const QString& i_strUnitKey )
//------------------------------------------------------------------------------
{
    if( m_pUnit != nullptr && m_pUnit->keyInTree() != i_strUnitKey )
    {
        QString strMethod, strArgs, strErr;
        strMethod = "CPhysVal::setUnitKey";
        strArgs   = "UnitKey: " + i_strUnitKey;
        strErr    = "UnitKey is different from key of already set unit " + m_pUnit->keyInTree();
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, strMethod + "( " + strArgs + " ): " + strErr );
    }

    m_strUnitKey = i_strUnitKey;

    m_physValRes.setUnitKey(i_strUnitKey);

} // setUnitKey

//------------------------------------------------------------------------------
void CPhysVal::setUnitRatio( CUnitRatio* i_pUnitRatio )
//------------------------------------------------------------------------------
{
    m_pUnit = i_pUnitRatio;

    if( m_pUnit == nullptr )
    {
        m_pUnitGrp = nullptr;
    }
    else
    {
        m_pUnitGrp = m_pUnit->unitGroup();
    }

    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    m_physValRes.setUnitRatio(i_pUnitRatio);

} // setUnitRatio

//------------------------------------------------------------------------------
void CPhysVal::setUnitDataQuantity( CUnitDataQuantity* i_pUnitDataQuantity )
//------------------------------------------------------------------------------
{
    m_pUnit = i_pUnitDataQuantity;

    if( m_pUnit == nullptr )
    {
        m_pUnitGrp = nullptr;
    }
    else
    {
        m_pUnitGrp = i_pUnitDataQuantity->unitGroup();
    }

    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    m_physValRes.setUnitDataQuantity(i_pUnitDataQuantity);

} // setUnitDataQuantity

//------------------------------------------------------------------------------
void CPhysVal::setPhysUnit( CPhysUnit* i_pPhysUnit )
//------------------------------------------------------------------------------
{
    m_pUnit = i_pPhysUnit;

    if( m_pUnit == nullptr )
    {
        m_pUnitGrp = nullptr;
    }
    else
    {
        m_pUnitGrp = i_pPhysUnit->getPhysSize();
    }

    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    m_physValRes.setPhysUnit(i_pPhysUnit);

} // setPhysUnit

//------------------------------------------------------------------------------
CUnitRatio* CPhysVal::getUnitRatio() const
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitRatio*>(m_pUnit);
}

//------------------------------------------------------------------------------
CUnitDataQuantity* CPhysVal::getUnitDataQuantity() const
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitDataQuantity*>(m_pUnit);
}

//------------------------------------------------------------------------------
CPhysUnit* CPhysVal::getPhysUnit() const
//------------------------------------------------------------------------------
{
    return dynamic_cast<CPhysUnit*>(m_pUnit);
}

//------------------------------------------------------------------------------
QString CPhysVal::getUnitName( bool i_bInsertParentNames ) const
//------------------------------------------------------------------------------
{
    QString strName;

    if( m_pUnit == nullptr )
    {
        strName = "NoUnit";
    }
    else
    {
        strName = m_pUnit->keyInTree();
    }
    return strName;

} // getUnitName

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysVal::invalidateObjectReferences()
//------------------------------------------------------------------------------
{
    m_pUnitGrp = nullptr;
    m_pUnit = nullptr;

    m_physValRes.invalidateObjectReferences();

} // invalidateObjectReferences

/*==============================================================================
public: // instance methods (to set the value)
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysVal::setVal( double i_fVal )
//------------------------------------------------------------------------------
{
    m_fVal = i_fVal;
    m_validity = EValueValidity::Valid;
}

//------------------------------------------------------------------------------
void CPhysVal::setVal( double i_fVal, CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    if( i_pUnit != nullptr )
    {
        setUnit(i_pUnit);
    }
    setVal(i_fVal);

} // setVal

//------------------------------------------------------------------------------
void CPhysVal::setVal( double i_fVal, CUnitRatio* i_pUnitRatio )
//------------------------------------------------------------------------------
{
    setUnitRatio(i_pUnitRatio);
    setVal(i_fVal);
}

//------------------------------------------------------------------------------
void CPhysVal::setVal( double i_fVal, CUnitDataQuantity* i_pUnitDataQuantity )
//------------------------------------------------------------------------------
{
    setUnitDataQuantity(i_pUnitDataQuantity);
    setVal(i_fVal);
}

//------------------------------------------------------------------------------
void CPhysVal::setVal( double i_fVal, CPhysUnit* i_pPhysUnit )
//------------------------------------------------------------------------------
{
    setPhysUnit(i_pPhysUnit);
    setVal(i_fVal);
}

/*==============================================================================
public: // instance methods (to set the value)
==============================================================================*/

//------------------------------------------------------------------------------
EFormatResult CPhysVal::setVal( const QString& i_strVal )
//------------------------------------------------------------------------------
{
    EFormatResult formatResult = EFormatResultError;
    bool          bValOk = false;
    double        fVal = 0.0;
    CUnitGrp*     pUnitGrpVal = m_pUnitGrp;
    CUnit*        pUnitVal = m_pUnit;
    bool          bResOk = false;
    double        fResVal = 0.0;
    CUnitGrp*     pUnitGrpRes = m_physValRes.unitGroup();
    CUnit*        pUnitRes = m_physValRes.getUnit();

    formatResult = parseValStr(i_strVal, &bValOk, &fVal, &pUnitGrpVal, &pUnitVal, &bResOk, &fResVal, &pUnitGrpRes, &pUnitRes, nullptr);

    if( !(formatResult & EFormatResultError) && bValOk )
    {
        m_validity = EValueValidity::Valid;
        m_fVal = fVal;
        m_pUnitGrp = pUnitGrpVal;
        m_pUnit = pUnitVal;

        if( m_pUnitGrp != nullptr )
        {
            m_strUnitGrpKey = m_pUnitGrp->keyInTree();
        }
        if( m_pUnit != nullptr )
        {
            m_strUnitKey = m_pUnit->keyInTree();
        }

        if( bResOk && fResVal != 0.0 )
        {
            if( pUnitRes == nullptr )
            {
                m_physValRes.setVal(fResVal);
            }
            else
            {
                if( pUnitVal != nullptr && (pUnitVal->isLogarithmic() != pUnitRes->isLogarithmic()) )
                {
                    QString strAddErrInfo = "Val:" + pUnitVal->keyInTree() + ", Res:" + pUnitRes->keyInTree();
                    throw CUnitConversionException( __FILE__, __LINE__, EResultMixedLinLogInValAndRes, strAddErrInfo );
                }
                m_physValRes.setVal(fResVal,pUnitRes);
            }
        }
    }

    return formatResult;

} // setVal(QString)

//------------------------------------------------------------------------------
EFormatResult CPhysVal::setVal( const QString& i_strVal, CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    if( i_pUnit != nullptr )
    {
        setUnit(i_pUnit);
    }
    return setVal(i_strVal);
}

//------------------------------------------------------------------------------
EFormatResult CPhysVal::setVal( const QString& i_strVal, CUnitRatio* i_pUnitRatio )
//------------------------------------------------------------------------------
{
    setUnitRatio(i_pUnitRatio);
    return setVal(i_strVal);
}

//------------------------------------------------------------------------------
EFormatResult CPhysVal::setVal( const QString& i_strVal, CUnitDataQuantity* i_pUnitDataQuantity )
//------------------------------------------------------------------------------
{
    setUnitDataQuantity(i_pUnitDataQuantity);
    return setVal(i_strVal);
}

//------------------------------------------------------------------------------
EFormatResult CPhysVal::setVal( const QString& i_strVal, CPhysUnit* i_pPhysUnit )
//------------------------------------------------------------------------------
{
    setPhysUnit(i_pPhysUnit);
    return setVal(i_strVal);
}

/*==============================================================================
public: // instance methods (to get the value as double)
==============================================================================*/

//------------------------------------------------------------------------------
double CPhysVal::getVal( const CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    double fVal = m_fVal;

    if( i_pUnit != nullptr )
    {
        if( !areOfSameUnitGroup(m_pUnit,i_pUnit) )
        {
            QString strAddErrInfo = "Src:" + getUnitGroupName() + ", Dst:" + ZS::PhysVal::getUnitGroupName(i_pUnit);
            throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
        }
    }
    if( i_pUnit != nullptr && m_pUnit != nullptr && i_pUnit != m_pUnit )
    {
        fVal = m_pUnit->convertValue(fVal,i_pUnit);
    }
    return fVal;
}

//------------------------------------------------------------------------------
double CPhysVal::getVal( const CUnitRatio* i_pUnitRatio ) const
//------------------------------------------------------------------------------
{
    double fVal = m_fVal;

    if( m_pUnit == nullptr || m_pUnit->classType() != EUnitClassTypeRatios )
    {
        QString strAddErrInfo = "Src:" + getUnitGroupName() + ", Dst:" + i_pUnitRatio->unitGroup()->keyInTree();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    fVal = dynamic_cast<CUnitRatio*>(m_pUnit)->convertValue(fVal,i_pUnitRatio);

    return fVal;
}

//------------------------------------------------------------------------------
double CPhysVal::getVal( const CUnitDataQuantity* i_pUnitDataQuantity ) const
//------------------------------------------------------------------------------
{
    double fVal = m_fVal;

    if( m_pUnit == nullptr || m_pUnit->classType() != EUnitClassTypeDataQuantity )
    {
        QString strAddErrInfo = "Src:" + getUnitGroupName() + ", Dst:" + i_pUnitDataQuantity->unitGroup()->keyInTree();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    fVal = dynamic_cast<CUnitDataQuantity*>(m_pUnit)->convertValue(fVal,i_pUnitDataQuantity);

    return fVal;
}

//------------------------------------------------------------------------------
double CPhysVal::getVal( const CPhysUnit* i_pPhysUnit ) const
//------------------------------------------------------------------------------
{
    double fVal = m_fVal;

    if( m_pUnit == nullptr || m_pUnit->classType() != EUnitClassTypePhysScienceFields )
    {
        QString strAddErrInfo = "Src:" + getUnitGroupName() + ", Dst:" + i_pPhysUnit->unitGroup()->keyInTree();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    fVal = dynamic_cast<CPhysUnit*>(m_pUnit)->convertValue(fVal,i_pPhysUnit);

    return fVal;
}

/*==============================================================================
public: // instance methods (to convert the value into a string)
==============================================================================*/

//------------------------------------------------------------------------------
QString CPhysVal::toString(
    EUnitFind i_unitFindVal,
    int       i_iValSubStrVisibility,
    EUnitFind i_unitFindRes,
    int       i_iResSubStrVisibility ) const
//------------------------------------------------------------------------------
{
    SValueFormatProvider valueFormat;

    valueFormat.m_unitFindVal = i_unitFindVal;
    valueFormat.m_iValSubStrVisibility = i_iValSubStrVisibility;
    valueFormat.m_unitFindRes = i_unitFindRes;
    valueFormat.m_iResSubStrVisibility = i_iResSubStrVisibility;

    return toString(valueFormat);

} // toString

//------------------------------------------------------------------------------
QString CPhysVal::toString(
    EUnitFind    i_unitFindVal,
    int          i_iValSubStrVisibility,
    const CUnit* i_pUnitRes,
    int          i_iResSubStrVisibility ) const
//------------------------------------------------------------------------------
{
    SValueFormatProvider valueFormat;

    valueFormat.m_unitFindVal = i_unitFindVal;
    valueFormat.m_iValSubStrVisibility = i_iValSubStrVisibility;
    valueFormat.m_pUnitRes = const_cast<CUnit*>(i_pUnitRes);
    valueFormat.m_iResSubStrVisibility = i_iResSubStrVisibility;

    return toString(valueFormat);

} // toString

//------------------------------------------------------------------------------
QString CPhysVal::toString(
    const CUnit* i_pUnitVal,
    int          i_iValSubStrVisibility,
    EUnitFind    i_unitFindRes,
    int          i_iResSubStrVisibility ) const
//------------------------------------------------------------------------------
{
    SValueFormatProvider valueFormat;

    valueFormat.m_pUnitVal = const_cast<CUnit*>(i_pUnitVal);
    valueFormat.m_iValSubStrVisibility = i_iValSubStrVisibility;
    valueFormat.m_unitFindRes = i_unitFindRes;
    valueFormat.m_iResSubStrVisibility = i_iResSubStrVisibility;

    return toString(valueFormat);

} // toString

//------------------------------------------------------------------------------
QString CPhysVal::toString(
    const CUnit* i_pUnitVal,
    int          i_iValSubStrVisibility,
    const CUnit* i_pUnitRes,
    int          i_iResSubStrVisibility ) const
//------------------------------------------------------------------------------
{
    SValueFormatProvider valueFormat;

    valueFormat.m_pUnitVal = const_cast<CUnit*>(i_pUnitVal);
    valueFormat.m_iValSubStrVisibility = i_iValSubStrVisibility;
    valueFormat.m_pUnitRes = const_cast<CUnit*>(i_pUnitRes);
    valueFormat.m_iResSubStrVisibility = i_iResSubStrVisibility;

    return toString(valueFormat);

} // toString

//------------------------------------------------------------------------------
QString CPhysVal::toString( const SValueFormatProvider& i_valueFormat ) const
//------------------------------------------------------------------------------
{
    EFormatResult formatResult;
    QString       strValTmp;
    QString       arSubStr[ESubStrCount];
    QString       strPhysVal;
    double        fVal = m_fVal;
    CUnit*        pUnitVal = m_pUnit;
    EUnitFind     unitFindVal = i_valueFormat.m_unitFindVal;
    CPhysValRes   physValRes = m_physValRes;
    EUnitFind     unitFindRes = i_valueFormat.m_unitFindRes;

    if( i_valueFormat.hasRes() )
    {
        physValRes = CPhysValRes(i_valueFormat.m_fRes,i_valueFormat.m_pUnitRes,i_valueFormat.m_resType);
    }

    arSubStr[ESubStrVal] = invalidValueString();

    if( !isValid() )
    {
        strPhysVal = invalidValueString();
    }
    else
    {
        if( i_valueFormat.m_pUnitVal != nullptr )
        {
            if( i_valueFormat.m_pUnitVal != pUnitVal && pUnitVal != nullptr && i_valueFormat.m_pUnitVal != nullptr )
            {
                fVal = pUnitVal->convertValue(fVal,i_valueFormat.m_pUnitVal);
            }
            pUnitVal = i_valueFormat.m_pUnitVal;
            unitFindVal = EUnitFindNone;
        }
        if( unitFindVal == EUnitFindBest )
        {
            if( physValRes.isValid() )
            {
                formatResult = formatValue(
                    /* fVal                  */ fVal,
                    /* pUnitVal              */ pUnitVal,
                    /* fRes                  */ physValRes.getVal(),
                    /* pUnitRes              */ physValRes.getUnit(),
                    /* resType               */ physValRes.type(),
                    /* iDigitsMantissa       */ i_valueFormat.m_iDigitsMantissa,
                    /* iDigitsExponent       */ i_valueFormat.m_iDigitsExponent,
                    /* iDigitsPerDigitGroup  */ i_valueFormat.m_iDigitsPerDigitGroup,
                    /* pstrDigitsGroupDelim. */ i_valueFormat.m_pstrDigitsGroupDelimiter,
                    /* pstrDecimalPoint      */ i_valueFormat.m_pstrDecimalPoint,
                    /* bUseEngineeringFormat */ i_valueFormat.m_bUseEngineeringFormat,
                    /* pfVal                 */ &fVal,
                    /* pstr                  */ &strValTmp,
                    /* ppUnitVal             */ &pUnitVal );
            }
            else
            {
                formatResult = formatValue(
                    /* fVal                  */ fVal,
                    /* pUnitVal              */ pUnitVal,
                    /* iDigitsMantissaMax    */ i_valueFormat.m_iDigitsMantissa,
                    /* bResolutionLimitsMant.*/ i_valueFormat.m_bDigitsAccuracyLimitsMantissa,
                    /* iDigitsResolution     */ i_valueFormat.m_iDigitsAccuracy,
                    /* iDigitsExponent       */ i_valueFormat.m_iDigitsExponent,
                    /* iDigitsPerDigitGroup  */ i_valueFormat.m_iDigitsPerDigitGroup,
                    /* pstrDigitsGroupDelim. */ i_valueFormat.m_pstrDigitsGroupDelimiter,
                    /* pstrDecimalPoint      */ i_valueFormat.m_pstrDecimalPoint,
                    /* bUseEngineeringFormat */ i_valueFormat.m_bUseEngineeringFormat,
                    /* pfVal                 */ &fVal,
                    /* pstr                  */ &strValTmp,
                    /* ppUnitVal             */ &pUnitVal );
            }
        }
        else
        {
            if( physValRes.isValid() )
            {
                formatResult = formatValue(
                    /* fVal                  */ fVal,
                    /* pUnitVal              */ pUnitVal,
                    /* fRes                  */ physValRes.getVal(),
                    /* pUnitRes              */ physValRes.getUnit(),
                    /* resType               */ physValRes.type(),
                    /* iDigitsMantissa       */ i_valueFormat.m_iDigitsMantissa,
                    /* iDigitsExponent       */ i_valueFormat.m_iDigitsExponent,
                    /* iDigitsPerDigitGroup  */ i_valueFormat.m_iDigitsPerDigitGroup,
                    /* pstrDigitsGroupDelim. */ i_valueFormat.m_pstrDigitsGroupDelimiter,
                    /* pstrDecimalPoint      */ i_valueFormat.m_pstrDecimalPoint,
                    /* bUseEngineeringFormat */ i_valueFormat.m_bUseEngineeringFormat,
                    /* pfVal                 */ nullptr,
                    /* pstr                  */ &strValTmp );
            }
            else
            {
                formatResult = formatValue(
                    /* fVal                   */ fVal,
                    /* pUnitVal               */ pUnitVal,
                    /* iDigitsMantissaMax     */ i_valueFormat.m_iDigitsMantissa,
                    /* bResolutionLimitsMant. */ i_valueFormat.m_bDigitsAccuracyLimitsMantissa,
                    /* iDigitsResolution      */ i_valueFormat.m_iDigitsAccuracy,
                    /* iDigitsExponent        */ i_valueFormat.m_iDigitsExponent,
                    /* iDigitsPerDigitGroup   */ i_valueFormat.m_iDigitsPerDigitGroup,
                    /* pstrDigitsGroupDelim.  */ i_valueFormat.m_pstrDigitsGroupDelimiter,
                    /* pstrDecimalPoint       */ i_valueFormat.m_pstrDecimalPoint,
                    /* bUseEngineeringFormat  */ i_valueFormat.m_bUseEngineeringFormat,
                    /* pfVal                  */ nullptr,
                    /* pstr                   */ &strValTmp );
            }
        }

        switch( formatResult )
        {
            case EFormatResultOk:
            case EFormatResultAccuracyOverflow:
            case EFormatResultAccuracyUnderflow:
            {
                if( i_valueFormat.m_iValSubStrVisibility & EPhysValSubStrVal )
                {
                    arSubStr[ESubStrVal] = strValTmp;
                }
                if( pUnitVal != nullptr )
                {
                    if( i_valueFormat.m_iValSubStrVisibility & EPhysValSubStrUnitGrp )
                    {
                        // Use "key" instead of "name(IncludingParentNames)" as the path contains the
                        // root nodes (UnitClassType) whereas the key starts with the name of the science field.
                        arSubStr[ESubStrValUnitGrp] = pUnitVal->unitGroup()->keyInTree();
                    }
                    if( i_valueFormat.m_iValSubStrVisibility & EPhysValSubStrUnitSymbol )
                    {
                        arSubStr[ESubStrValUnit] = pUnitVal->symbol();
                    }
                    else if( i_valueFormat.m_iValSubStrVisibility & EPhysValSubStrUnitName )
                    {
                        arSubStr[ESubStrValUnit] = pUnitVal->name();
                    }
                }
                if( physValRes.isValid() && !(i_valueFormat.m_iResSubStrVisibility & EPhysValSubStrNone) )
                {
                    double fRes     = physValRes.getVal();
                    CUnit* pUnitRes = physValRes.getUnit();

                    // If the resolution is defined as a ratio value but the value is not ...
                    if( pUnitVal != nullptr && pUnitVal->classType() != EUnitClassTypeRatios && pUnitRes != nullptr && pUnitRes->classType() == EUnitClassTypeRatios )
                    {
                        // If the unit of the resolution has been explicitly specified not as ratio
                        // or if the resolution should be indicated in the unit of the value ..
                        if( (i_valueFormat.m_pUnitRes != nullptr && i_valueFormat.m_pUnitRes->classType() != EUnitClassTypeRatios)
                         || (!(i_valueFormat.m_iResSubStrVisibility & EPhysValSubStrUnitSymbol) && !(i_valueFormat.m_iResSubStrVisibility & EPhysValSubStrUnitName)) )
                        {
                            // .. calculate the absolute value of the resolution in the unit of the value.
                            fRes = fRes * fVal * dynamic_cast<CUnitRatio*>(pUnitRes)->getFactor();
                            pUnitRes = pUnitVal;

                            // If the unit of the resolution has been explicitly specified ..
                            if( i_valueFormat.m_pUnitRes != nullptr )
                            {
                                fRes = pUnitRes->convertValue(fRes,i_valueFormat.m_pUnitRes);
                                pUnitRes = i_valueFormat.m_pUnitRes;
                            }
                            else if( i_valueFormat.m_pUnitVal != nullptr )
                            {
                                fRes = pUnitVal->convertValue(fRes,i_valueFormat.m_pUnitVal);
                                pUnitRes = i_valueFormat.m_pUnitVal;
                            }
                        }
                    }
                    // If the unit of the resolution should not be indicated separately ..
                    if( !(i_valueFormat.m_iResSubStrVisibility & EPhysValSubStrUnitSymbol) && !(i_valueFormat.m_iResSubStrVisibility & EPhysValSubStrUnitName) )
                    {
                        // ... the resolution must be indicated in the same unit as the value.
                        if( pUnitRes != nullptr && pUnitRes != pUnitVal )
                        {
                            fRes = pUnitRes->convertValue(fRes,pUnitVal);
                            pUnitRes = pUnitVal;
                        }
                        unitFindRes = EUnitFindNone;
                    }
                    if( unitFindRes == EUnitFindBest )
                    {
                        formatResult = formatValue(
                            /* fVal                       */ fRes,
                            /* pUnitVal                   */ pUnitRes,
                            /* iDigitsMantissaMax         */ -1,
                            /* bDigitsAccuracyLimitsMant. */ true,
                            /* iDigitsAccuracy            */ 2,
                            /* iDigitsExponent            */ 0,
                            /* bUseEngineeringFormat      */ false,
                            /* pfVal                      */ &fRes,
                            /* pstr                       */ &strValTmp,
                            /* ppUnitVal                  */ &pUnitRes );
                    }
                    else
                    {
                        formatResult = formatValue(
                            /* fVal                       */ fRes,
                            /* pUnitVal                   */ pUnitRes,
                            /* iDigitsMantissaMax         */ -1,
                            /* bDigitsAccuracyLimitsMant. */ true,
                            /* iDigitsAccuracy            */ 2,
                            /* iDigitsExponent            */ 0,
                            /* bUseEngineeringFormat      */ false,
                            /* pfVal                      */ nullptr,
                            /* pstr                       */ &strValTmp );
                    }
                    if( formatResult != EFormatResultOk )
                    {
                        strValTmp = "";
                    }
                    removeTrailingZeros(&strValTmp);

                    if( i_valueFormat.m_iResSubStrVisibility & EPhysValSubStrVal )
                    {
                        arSubStr[ESubStrRes] = strValTmp;
                    }
                    if( pUnitRes != nullptr )
                    {
                        if( i_valueFormat.m_iResSubStrVisibility & EPhysValSubStrUnitGrp )
                        {
                            // Use "key" instead of "name(IncludingParentNames)" as the path contains the
                            // root nodes (UnitClassType) whereas the key starts with the name of the science field.
                            arSubStr[ESubStrResUnitGrp] = pUnitRes->unitGroup()->keyInTree();
                        }
                        if( i_valueFormat.m_iResSubStrVisibility & EPhysValSubStrUnitSymbol )
                        {
                            arSubStr[ESubStrResUnit] = pUnitRes->symbol();
                        }
                        else if( i_valueFormat.m_iResSubStrVisibility & EPhysValSubStrUnitName )
                        {
                            arSubStr[ESubStrResUnit] = pUnitRes->name();
                        }
                    }
                } // if( physValRes.isValid() && !(i_iResSubStrVisibility & EPhysValSubStrNone) )

                // If the value should be indicated without the resolution or if the value has no resolution ..
                if( i_valueFormat.m_iValSubStrVisibility != EPhysValSubStrNone && (i_valueFormat.m_iResSubStrVisibility == EPhysValSubStrNone || !physValRes.isValid()) )
                {
                    bool bUnitSpaceInserted = false;

                    if( (i_valueFormat.m_iValSubStrVisibility & EPhysValSubStrVal) && !arSubStr[ESubStrVal].isEmpty() )
                    {
                        strPhysVal += arSubStr[ESubStrVal];
                    }
                    if( (i_valueFormat.m_iValSubStrVisibility & EPhysValSubStrUnitGrp) && !arSubStr[ESubStrValUnitGrp].isEmpty() )
                    {
                        strPhysVal += " ";
                        bUnitSpaceInserted = true;
                        strPhysVal += arSubStr[ESubStrValUnitGrp];
                    }
                    if( ((i_valueFormat.m_iValSubStrVisibility & EPhysValSubStrUnitSymbol) || (i_valueFormat.m_iValSubStrVisibility & EPhysValSubStrUnitName)) && !arSubStr[ESubStrValUnit].isEmpty() )
                    {
                        if( !bUnitSpaceInserted )
                        {
                            strPhysVal += " ";
                            bUnitSpaceInserted = true;
                        }
                        strPhysVal += arSubStr[ESubStrValUnit];
                    }
                }

                // If the value should be indicated together with the resolution ..
                else if( i_valueFormat.m_iValSubStrVisibility != EPhysValSubStrNone && i_valueFormat.m_iResSubStrVisibility != EPhysValSubStrNone )
                {
                    // If the unit of the value and the unit of the resolution are the same and should 
                    // be indicated for both values together ...
                    if( !arSubStr[ESubStrValUnit].isEmpty() && arSubStr[ESubStrResUnit].isEmpty() )
                    {
                        bool bUnitSpaceInserted = false;

                        strPhysVal += "(";

                        if( (i_valueFormat.m_iValSubStrVisibility & EPhysValSubStrVal) && !arSubStr[ESubStrVal].isEmpty() )
                        {
                            strPhysVal += arSubStr[ESubStrVal];
                        }
                        if( (i_valueFormat.m_iResSubStrVisibility & EPhysValSubStrVal) && !arSubStr[ESubStrRes].isEmpty() )
                        {
                            strPhysVal += " " + c_strSymbolPlusMinus + " " + arSubStr[ESubStrRes];
                        }
                        strPhysVal += ")";

                        if( (i_valueFormat.m_iValSubStrVisibility & EPhysValSubStrUnitGrp) && !arSubStr[ESubStrValUnitGrp].isEmpty() )
                        {
                            strPhysVal += " ";
                            bUnitSpaceInserted = true;
                            strPhysVal += arSubStr[ESubStrValUnitGrp];
                        }
                        if( ((i_valueFormat.m_iValSubStrVisibility & EPhysValSubStrUnitSymbol) || (i_valueFormat.m_iValSubStrVisibility & EPhysValSubStrUnitName)) && !arSubStr[ESubStrValUnit].isEmpty() )
                        {
                            if( !bUnitSpaceInserted )
                            {
                                strPhysVal += " ";
                                bUnitSpaceInserted = true;
                            }
                            strPhysVal += arSubStr[ESubStrValUnit];
                        }
                    }

                    // If the unit of the value and the unit of the resolution are different or should 
                    // be indicated for both values separately ...
                    else
                    {
                        bool bUnitSpaceInserted = false;

                        if( (i_valueFormat.m_iValSubStrVisibility & EPhysValSubStrVal) && !arSubStr[ESubStrVal].isEmpty() )
                        {
                            strPhysVal += arSubStr[ESubStrVal];
                        }
                        if( (i_valueFormat.m_iValSubStrVisibility & EPhysValSubStrUnitGrp) && !arSubStr[ESubStrValUnitGrp].isEmpty() )
                        {
                            strPhysVal += " ";
                            bUnitSpaceInserted = true;
                            strPhysVal += arSubStr[ESubStrValUnitGrp];
                        }
                        if( ((i_valueFormat.m_iValSubStrVisibility & EPhysValSubStrUnitSymbol) || (i_valueFormat.m_iValSubStrVisibility & EPhysValSubStrUnitName)) && !arSubStr[ESubStrValUnit].isEmpty() )
                        {
                            if( !bUnitSpaceInserted )
                            {
                                strPhysVal += " ";
                                bUnitSpaceInserted = true;
                            }
                            strPhysVal += arSubStr[ESubStrValUnit];
                        }

                        // Append the resolution with its unit
                        bUnitSpaceInserted = false;
                        if( (i_valueFormat.m_iResSubStrVisibility & EPhysValSubStrVal) && !arSubStr[ESubStrRes].isEmpty() )
                        {
                            strPhysVal += " " + c_strSymbolPlusMinus + " " + arSubStr[ESubStrRes];
                        }
                        if( (i_valueFormat.m_iResSubStrVisibility & EPhysValSubStrUnitGrp) && !arSubStr[ESubStrResUnitGrp].isEmpty() )
                        {
                            strPhysVal += " ";
                            bUnitSpaceInserted = true;
                            strPhysVal += arSubStr[ESubStrResUnitGrp];
                        }
                        if( ((i_valueFormat.m_iResSubStrVisibility & EPhysValSubStrUnitSymbol) || (i_valueFormat.m_iResSubStrVisibility & EPhysValSubStrUnitName)) && !arSubStr[ESubStrResUnit].isEmpty() )
                        {
                            if( !bUnitSpaceInserted )
                            {
                                strPhysVal += " ";
                                bUnitSpaceInserted = true;
                            }
                            strPhysVal += arSubStr[ESubStrResUnit];
                        }
                    }
                }
                break;
            }
            case EFormatResultValueOverflow:
            {
                strPhysVal = "Overflow";
                break;
            }
            case EFormatResultValueUnderflow:
            {
                strPhysVal = "Underflow";
                break;
            }
            default:
            {
                strPhysVal = "Format Error";
                break;
            }
        }
    }
    return strPhysVal;

} // toString

/*==============================================================================
public: // instance methods (to get and set the resolution of the value)
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysVal::setRes( double i_fVal )
//------------------------------------------------------------------------------
{
    m_physValRes.setVal(i_fVal);
}

//------------------------------------------------------------------------------
void CPhysVal::setRes( double i_fRes, CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    m_physValRes.setVal(i_fRes,i_pUnit);
}

//------------------------------------------------------------------------------
void CPhysVal::setRes( const CPhysValRes& i_physValRes )
//------------------------------------------------------------------------------
{
    m_physValRes = i_physValRes;
}

/*==============================================================================
public: // instance methods (to convert the unit)
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysVal::convertValue( CUnit* i_pUnitDst )
//------------------------------------------------------------------------------
{
    if( !areOfSameUnitGroup(m_pUnit,i_pUnitDst) )
    {
        QString strAddErrInfo = "Src:" + getUnitGroupName() + ", Dst:" + ZS::PhysVal::getUnitGroupName(i_pUnitDst);
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    if( isValid() && m_pUnit != nullptr && i_pUnitDst != nullptr && m_pUnit != i_pUnitDst )
    {
        m_fVal = m_pUnit->convertValue(m_fVal,i_pUnitDst);
        m_pUnit = i_pUnitDst;
        m_pUnitGrp = i_pUnitDst->unitGroup();

        if( m_pUnitGrp != nullptr )
        {
            m_strUnitGrpKey = m_pUnitGrp->keyInTree();
        }
        if( m_pUnit != nullptr )
        {
            m_strUnitKey = m_pUnit->keyInTree();
        }
    }

} // convertValue
