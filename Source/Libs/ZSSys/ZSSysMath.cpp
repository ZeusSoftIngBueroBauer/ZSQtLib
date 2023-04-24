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

#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;

//------------------------------------------------------------------------------
double Math::sqr( double i_fVal )
//------------------------------------------------------------------------------
{
    return i_fVal*i_fVal;
}

//------------------------------------------------------------------------------
double Math::abs( double i_fVal )
//------------------------------------------------------------------------------
{
    double fVal = i_fVal;

    if( i_fVal < 0 )
    {
        fVal *= -1.0;
    }
    return fVal;
}

//------------------------------------------------------------------------------
double Math::rad2Deg( double i_fVal_rad )
//------------------------------------------------------------------------------
{
    return 360.0*i_fVal_rad/Math::c_f2PI;
}

//------------------------------------------------------------------------------
double Math::deg2Rad( double i_fVal_deg )
//------------------------------------------------------------------------------
{
    return Math::c_f2PI*i_fVal_deg/360.0;
}

//------------------------------------------------------------------------------
double Math::round2Nearest( double i_fVal, int i_iTrailingDigits )
//------------------------------------------------------------------------------
{
    double fVal = 0.0;

    if( i_iTrailingDigits == 0 )
    {
        if( i_fVal < 0.0 )
        {
            fVal = ceil(i_fVal-0.5);
        }
        else if( i_fVal > 0.0 )
        {
            fVal = floor(i_fVal+0.5);
        }
    }
    else
    {
        int   iTrailingDigits = i_iTrailingDigits;
        int   iDecimalPos;
        int   iSign;
        char* szFcvt = nullptr;
        char* szVal  = nullptr;

        if( i_iTrailingDigits == -1 )
        {
            iTrailingDigits = c_iCalculationAccuracyTrailingDigits;
        }
        #ifdef _MSC_VER
        #pragma warning(disable:4996)
        szFcvt = _fcvt(i_fVal, iTrailingDigits, &iDecimalPos, &iSign);
        #pragma warning(default:4996)
        #else
        szFcvt = fcvt(i_fVal, iTrailingDigits, &iDecimalPos, &iSign);
        #endif

        if( iDecimalPos > 0 )
        {
            szVal = new char[strlen(szFcvt)+2]; // incl. decimal point and '\0'
            memset( szVal, 0x00, strlen(szFcvt)+2 );

            #ifdef _WINDOWS
            #pragma warning(disable:4996)
            #endif
            strncpy( szVal, szFcvt, iDecimalPos );
            strcat( szVal, "." );
            strcat( szVal, &szFcvt[iDecimalPos] );
            #ifdef _WINDOWS
            #pragma warning(default:4996)
            #endif
        }
        else
        {
            szVal = new char[-iDecimalPos+strlen(szFcvt)+2]; // incl. decimal point and '\0'
            memset( szVal, 0x00, -iDecimalPos+strlen(szFcvt)+2 );

            memset( szVal, '0', -iDecimalPos );
            #ifdef _WINDOWS
            #pragma warning(disable:4996)
            #endif
            strcat( szVal, "." );
            strcat( szVal, szFcvt );
            #ifdef _WINDOWS
            #pragma warning(default:4996)
            #endif
        }
        fVal = atof(szVal);
        if( i_fVal < 0.0 )
        {
            fVal *= -1.0;
        }
        delete [] szVal;
        szVal = nullptr;
    }
    return fVal;

} // round2Nearest

//------------------------------------------------------------------------------
double Math::round2Lower( double i_fVal, int i_iTrailingDigits )
//------------------------------------------------------------------------------
{
    double fVal = 0.0;

    if( i_iTrailingDigits == 0 )
    {
        fVal = floor(i_fVal);
    }
    else
    {
        fVal = floor(i_fVal);

        #ifdef _WINDOWS

        #pragma message(__TODO__"Support Trailing Digits")

        //int iTrailingDigits = i_iTrailingDigits;

        //if( i_iTrailingDigits == -1 )
        //{
        //    iTrailingDigits = c_iCalculationAccuracyTrailingDigits;
        //}
        #endif // #ifdef _WINDOWS
    }

    return fVal;

} // round2Lower

//------------------------------------------------------------------------------
double Math::round2Upper( double i_fVal, int i_iTrailingDigits )
//------------------------------------------------------------------------------
{
    double fVal = 0.0;

    if( i_iTrailingDigits == 0 )
    {
        fVal = ceil(i_fVal);
    }
    else
    {
        fVal = ceil(i_fVal);

        #ifdef _WINDOWS

        #pragma message(__TODO__"Support Trailing Digits")

        //int iTrailingDigits = i_iTrailingDigits;

        //if( i_iTrailingDigits == -1 )
        //{
        //    iTrailingDigits = c_iCalculationAccuracyTrailingDigits;
        //}
        #endif // #ifdef _WINDOWS
    }

    return fVal;

} // round2Upper

//------------------------------------------------------------------------------
double Math::round2Resolution( double i_fVal, double i_fRes )
//------------------------------------------------------------------------------
{
    double fVal = i_fVal;
    double fRes = fabs(i_fRes);

    if( fRes > 0.0 )
    {
        fVal = i_fVal / fRes;
        fVal = floor(fVal+0.5);
        fVal *= fRes;
    }
    return fVal;

} // round2Resolution

//------------------------------------------------------------------------------
void Math::normalize( double i_fVal, int* o_piSign, int* o_piMant, int* o_piExp )
//------------------------------------------------------------------------------
{
    double fValAbs  = fabs(i_fVal);
    int    iValSign = i_fVal < 0.0 ? -1 : 1;
    int    iValMant = 0;
    int    iValExp  = 0;

    if( fValAbs >= 1.0 )
    {
        iValExp = static_cast<int>(log10(fValAbs));
    }
    else if( fValAbs < 1.0 && fValAbs > 0.0 )
    {
        double fValTmp;
        double fFractTmp;
        double fIntTmp;

        fValTmp = log10(fValAbs);
        iValExp = static_cast<int>(fValTmp);

        fFractTmp = modf(fValTmp,&fIntTmp);
        if( fFractTmp != 0.0 )
        {
            iValExp--;
        }
    }
    if( o_piSign != nullptr )
    {
        *o_piSign = iValSign;
    }
    if( o_piMant != nullptr )
    {
        if( iValExp != 0 )
        {
            double fFactTmp = pow(10.0,iValExp);
            iValMant = static_cast<int>(fValAbs/fFactTmp+0.05); // not the first but the second valid digit will be rounded
        }
        else
        {
            iValMant = static_cast<int>(fValAbs+0.05); // not the first but the second valid digit will be rounded
        }
        *o_piMant = iValMant;
    }
    if( o_piExp != nullptr )
    {
        *o_piExp = iValExp;
    }

} // normalize

//------------------------------------------------------------------------------
double Math::round2LowerDecade( double i_fVal )
//------------------------------------------------------------------------------
{
    int    iSign = 1;
    double fVal  = fabs(i_fVal);

    if( i_fVal < 0.0 )
    {
        iSign = -1;
    }

    if( fVal > 0.0 )
    {
        double fLog;
        double fFrac;

        fLog  = log10(fVal);
        fFrac = modf(fLog,&fLog);
        fLog  = static_cast<int>(fLog);

        if( fVal < 1.0 )
        {
            if( fFrac != 0.0 )
            {
                fLog -= 1.0;
            }
        }
        else
        {
            if( iSign < 0 )
            {
                fLog += 1.0;
            }
        }
        fVal = pow(10.0,fLog);
    }
    return iSign*fVal;

} // round2LowerDecade

//------------------------------------------------------------------------------
double Math::round2UpperDecade( double i_fVal )
//------------------------------------------------------------------------------
{
    int    iSign = 1;
    double fVal = fabs(i_fVal);

    if( i_fVal < 0.0 )
    {
        iSign = -1;
    }

    if( fVal > 0.0 )
    {
        double fLog;
        double fFrac;

        fLog  = log10(fVal);
        fFrac = modf(fLog,&fLog);
        fLog  = static_cast<int>(fLog);

        if( fVal < 1.0 )
        {
            if( fFrac != 0.0 )
            {
                fLog += 1.0;
            }
        }
        else
        {
            if( iSign > 0 )
            {
                fLog += 1.0;
            }
        }
        fVal = pow(10.0,fLog);
    }
    return iSign*fVal;

} // round2UpperDecade

//------------------------------------------------------------------------------
double Math::round2LowerIntMultOfTen( double i_fVal )
//------------------------------------------------------------------------------
{
    int    iSign = 1;
    int    iMant = 0;
    int    iExp  = 0;
    double fVal  = fabs(i_fVal);

    if( fVal > 0.0 )
    {
        double fLog  = log10(fVal);
        double fFrac = modf(fLog,&fLog);

        normalize(i_fVal,&iSign,&iMant,&iExp);

        if( fFrac != 0.0 )
        {
            iMant -= 1;
        }
    }
    return iSign*iMant*pow(10.0,iExp);

} // round2LowerIntMultOfTen

//------------------------------------------------------------------------------
double Math::round2UpperIntMultOfTen( double i_fVal )
//------------------------------------------------------------------------------
{
    int    iSign = 1;
    int    iMant = 0;
    int    iExp  = 0;
    double fVal  = fabs(i_fVal);

    if( fVal > 0.0 )
    {
        double fLog  = log10(fVal);
        double fFrac = modf(fLog,&fLog);

        normalize(i_fVal,&iSign,&iMant,&iExp);

        if( fFrac != 0.0 )
        {
            iMant += 1;
        }
    }
    return iSign*iMant*pow(10.0,iExp);

} // round2UpperIntMultOfTen

//------------------------------------------------------------------------------
int Math::minVal( int i_iValCount, int i_iVal1 ... )
//------------------------------------------------------------------------------
{
    int iValIdx;
    int iMinVal = i_iVal1;

    va_list ap;
    va_start(ap,i_iVal1);

    for( iValIdx = 1; iValIdx < i_iValCount; iValIdx++ )
    {
        int iVal = va_arg(ap,int);

        if( iVal < iMinVal )
        {
            iMinVal = iVal;
        }
    }
    va_end(ap);

    return iMinVal;

} // minVal

//------------------------------------------------------------------------------
double Math::minVal( int i_iValCount, double i_fVal1 ... )
//------------------------------------------------------------------------------
{
    int    iValIdx;
    double fMinVal = i_fVal1;

    va_list ap;
    va_start(ap,i_fVal1);

    for( iValIdx = 1; iValIdx < i_iValCount; iValIdx++ )
    {
        double fVal = va_arg(ap,double);

        if( fVal < fMinVal )
        {
            fMinVal = fVal;
        }
    }
    va_end(ap);

    return fMinVal;

} // minVal

//------------------------------------------------------------------------------
int Math::maxVal( int i_iValCount, int i_iVal1 ... )
//------------------------------------------------------------------------------
{
    int iValIdx;
    int iMaxVal = i_iVal1;

    va_list ap;
    va_start(ap,i_iVal1);

    for( iValIdx = 1; iValIdx < i_iValCount; iValIdx++ )
    {
        int iVal = va_arg(ap,int);

        if( iVal > iMaxVal )
        {
            iMaxVal = iVal;
        }
    }
    va_end(ap);

    return iMaxVal;

} // maxVal

//------------------------------------------------------------------------------
double Math::maxVal( int i_iValCount, double i_fVal1 ... )
//------------------------------------------------------------------------------
{
    int    iValIdx;
    double fMaxVal = i_fVal1;

    va_list ap;
    va_start(ap,i_fVal1);

    for( iValIdx = 1; iValIdx < i_iValCount; iValIdx++ )
    {
        double fVal = va_arg(ap,double);

        if( fVal > fMaxVal )
        {
            fMaxVal = fVal;
        }
    }
    va_end(ap);

    return fMaxVal;

} // maxVal

//------------------------------------------------------------------------------
double Math::fctFFTWindowRect( double /*i_fVal*/, int /*i_iFFTSegmentLen*/ )
//------------------------------------------------------------------------------
{
    return 1.0;
}

//------------------------------------------------------------------------------
double Math::fctFFTWindowHanning( double i_fVal, int i_iFFTSegmentLen )
//------------------------------------------------------------------------------
{
    if( i_fVal > i_iFFTSegmentLen || i_iFFTSegmentLen <= 1 || i_fVal < 0 )
    {
        return 0.0;
    }
    double fn = static_cast<double>(i_fVal);
    double fN = static_cast<double>(i_iFFTSegmentLen);
    return 0.5+0.5*cos(c_f2PI*(fn-fN/2.0)/fN);
}

//------------------------------------------------------------------------------
double Math::fctFFTWindowFlatTop( double i_fVal, int i_iFFTSegmentLen )
//------------------------------------------------------------------------------
{
    if( i_fVal > i_iFFTSegmentLen || i_iFFTSegmentLen <= 1 || i_fVal < 0 )
    {
        return 0.0;
    }
    double fn = static_cast<double>(i_fVal);
    double fN = static_cast<double>(i_iFFTSegmentLen);
    return 1.0-1.93*cos(c_f2PI*fn/(fN-1.0))+1.29*cos(c_f4PI*fn/(fN-1.0))-0.388*cos(c_f6PI*fn/(fN-1.0))+0.032*cos(c_f8PI*fn/(fN-1.0));
}

//------------------------------------------------------------------------------
double Math::fctFFTWindowBlackman( double i_fVal, int i_iFFTSegmentLen )
//------------------------------------------------------------------------------
{
    if( i_fVal > i_iFFTSegmentLen || i_iFFTSegmentLen <= 1 || i_fVal < 0 )
    {
        return 0.0;
    }
    double fn = static_cast<double>(i_fVal);
    double fN = static_cast<double>(i_iFFTSegmentLen);
    return 0.42-0.5*cos(c_f2PI*fn/(fN-1.0))+0.08*cos(c_f4PI*fn/(fN-1.0));
}

//------------------------------------------------------------------------------
double Math::fctFFTWindowWelch( double i_fVal, int i_iFFTSegmentLen )
//------------------------------------------------------------------------------
{
    if( i_fVal > i_iFFTSegmentLen || i_iFFTSegmentLen <= 1 || i_fVal < 0 )
    {
        return 0.0;
    }
    double fn = static_cast<double>(i_fVal);
    double fN = static_cast<double>(i_iFFTSegmentLen);
    return 1.0-sqr((fn-fN/2.0)/(fN/2.0));
}

//------------------------------------------------------------------------------
/*! @brief Calculates division lines for the given scale values.

    @param i_fScaleMinVal
        Minimum scale value in world coordinates (e.g. 0.0).
    @param i_fScaleMaxVal
        Maximum scale value in world coordinates (e.g. 100.0).
    @param i_iScaleRangePix
        Number of pixels available for the scale (e.g. 1000).
    @param i_fDivLineDistMinVal
        Minimum distance between two division lines in world coordinates (e.g. 10.0).
    @param i_iDivLineDistMinPix
        Minimum distance between two division lines in pixels (e.g. 50).
    @param i_bUseDivLineDistValDecimalFactor25
        If false the division lines will be decimal factors at 10, 20, 50.
        If true also the decimall factor 25 is allowed.
    @param o_pfDivLineFirstVal
        If != nullptr contains the world coordinate of the first division line.
    @param o_pfDivLineDistFirstPix
        If != nullptr contains the pixel coordinate of the first division line.
    @param o_pfDivLineDistVal
        If != nullptr contains the distance in world coordinate between two division lines.
    @param o_pfDivLineDistPix
        If != nullptr contains the distance in pixels between two division lines.

    @return Number of division lines for the given scale range.

    @Example:

    Input:

                  DistMinVal
        |         |<--   -->|                                                                               |
        |         |<- 10.0->|                                                                               |
    ScaleMinVal                                                                                        ScaleMaxVal
       0.0                                                                                                100.0
        +---------------------------------------------------------------------------------------------------+
        |<-------------------------------------------------- 1000 ----------------------------------------->|
        |<----------------------------------------------- ScaleRangePix ----------------------------------->|
        |       |<50>|                                                                                      |
        |       |<-->|                                                                                      |
        |       DistMinPix                                                                                  |

    Output:

        |<DistVal>|
        |<- 10.0->|
     FirstVal
       0.0                                                                                                  |
        +---------|---------|---------|---------|---------|---------|---------|---------|---------|---------+
        0                                                                                                   |
     FirstPix                                                                                               |
        |<- 100 ->|                                                                                         |
        |<DistPix>|                                                                                         |

    Return: 11
*/
int Math::calculateDivLines4LinSpacing(
    double  i_fScaleMinVal,
    double  i_fScaleMaxVal,
    int     i_iScaleRangePix,
    double  i_fDivLineDistMinVal,
    int     i_iDivLineDistMinPix,
    bool    i_bUseDivLineDistValDecimalFactor25,
    double* o_pfDivLineFirstVal,
    double* o_pfDivLineDistFirstPix,
    double* o_pfDivLineDistVal,
    double* o_pfDivLineDistPix,
    CTrcAdminObj* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(i_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Scale {Min: " + QString::number(i_fScaleMinVal)
                     + ", Max: " + QString::number(i_fScaleMaxVal)
                     + ", RangePix: " + QString::number(i_iScaleRangePix) + "}"
                     + ", DivLineDist {MinVal: " + QString::number(i_fDivLineDistMinVal)
                     + ", MinPix: " + QString::number(i_iDivLineDistMinPix)
                     + ", UseDecFactor25: " + bool2Str(i_bUseDivLineDistValDecimalFactor25) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ i_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "Math::calculateDivLines4LinSpacing",
        /* strAddInfo   */ strMthInArgs );

    int iDivLineCount = 0;

    if (o_pfDivLineFirstVal != nullptr)
    {
        *o_pfDivLineFirstVal = i_fScaleMinVal;
    }
    if (o_pfDivLineDistFirstPix != nullptr)
    {
        *o_pfDivLineDistFirstPix = 0.0;
    }
    if (o_pfDivLineDistVal != nullptr)
    {
        *o_pfDivLineDistVal = i_fScaleMaxVal - i_fScaleMinVal;
    }
    if (o_pfDivLineDistPix != nullptr)
    {
        *o_pfDivLineDistPix = i_iScaleRangePix;
    }

    double fScaleRangeVal = i_fScaleMaxVal - i_fScaleMinVal;

    if (fScaleRangeVal > 0.0 && i_iScaleRangePix > 1 && i_iDivLineDistMinPix > 1)
    {
        double fScaleRangeFacPixDivVal = i_iScaleRangePix/fScaleRangeVal;

        // Maximum possible count of grid lines:
        int iDivLineCountMax = static_cast<int>(static_cast<double>(i_iScaleRangePix)/static_cast<double>(i_iDivLineDistMinPix)) + 1;

        // On dividing the pixel range by the maximum possible count of grid lines the
        // distance between two grid lines would be:
        double fDivLineDistMinVal = i_fDivLineDistMinVal;
        if( i_fDivLineDistMinVal < 0.0 )
        {
            fDivLineDistMinVal = fScaleRangeVal / iDivLineCountMax;
        }

        // But DivLineDistMinVal might have a fractional part. And thats not what we want.
        // We need to set the grid lines to "human readable" values like 0.1 .. 0.2 .. 0.3 or
        // 10.0 .. 20.0 ..30.0 or 1.25 .. 1.50 .. 1.75 and so on.

        // For this we are going to calculate "DivLineDistVal" which is less or equal to the distance
        // between two grid lines if the pixel range would be divided by the maximum possible count
        // of grid lines and with the first valid digit "1" (e.g. 100.0, 1.0, 0.001, etc.).
        double fDivLineDistVal = 1.0;
        if( fDivLineDistMinVal > 1.0 )
        {
            double fAbortVal = fDivLineDistMinVal/10.0;
            while( fDivLineDistVal <= fAbortVal )
            {
                fDivLineDistVal *= 10.0;
            }
        }
        else if( fDivLineDistMinVal < 1.0 )
        {
            double fAbortVal = fDivLineDistMinVal;
            while( fDivLineDistVal >= fAbortVal )
            {
                fDivLineDistVal /= 10.0;
            }
        }

        // Now calculate the distance between grid lines if the current "DivLineDistVal"
        // would be used. For this we calculate the values and pixel positions of the
        // first and last grid lines, calculate the resulting count of grid lines and the
        // resulting distance in pixels between two grid lines.

        // The values of the first and last grid lines must be a whole number multiple of
        // the distance between two grid lines:
        double fDivLineFirstVal = 0.0;
        modf(i_fScaleMinVal/fDivLineDistVal, &fDivLineFirstVal);
        fDivLineFirstVal *= fDivLineDistVal;
        if( fDivLineFirstVal < i_fScaleMinVal-DBL_EPSILON )
        {
            fDivLineFirstVal += fDivLineDistVal;
        }
        double fDivLineDistFirstPix = fScaleRangeFacPixDivVal*(fDivLineFirstVal-i_fScaleMinVal);
        double fDivLineLastVal = 0.0;
        modf(i_fScaleMaxVal/fDivLineDistVal, &fDivLineLastVal);
        fDivLineLastVal *= fDivLineDistVal;
        if( fDivLineLastVal > i_fScaleMaxVal+DBL_EPSILON )
        {
            fDivLineLastVal -= fDivLineDistVal;
        }
        double fDivLineDistLastPix = fScaleRangeFacPixDivVal*(i_fScaleMaxVal-fDivLineLastVal);

        // Range between first and last grid line:
        double fDivLineRangeVal = fDivLineLastVal - fDivLineFirstVal;
        double fDivLineRangePix = i_iScaleRangePix - fDivLineDistFirstPix - fDivLineDistLastPix; //lint !e834
        iDivLineCount = static_cast<int>(fDivLineRangeVal/fDivLineDistVal) + 1;

        double fDivLineDistPix = 0.0;

        // Distance between two grid lines between the first and last grid line:
        if( iDivLineCount < 2 )
        {
            fDivLineDistPix = 0.0;
        }
        else
        {
            fDivLineDistPix = fDivLineRangePix/(iDivLineCount-1);

            // Please note that at this point of execution "DivLineDistVal" is always less or equal
            // to the distance between two grid lines if the pixel range would be divided by the
            // maximum possible count of grid lines and its first valid digit is always 1
            // (e.g. 100.0, 1.0, 0.001, etc.). So definitely "DivLineDistPix" is less or equal to
            // "DistMinPix" (the user setting for the minimum distance in pixels between two grid lines).

            // In the following loop we are going to optimize "DivLineDistVal" by increasing the value
            // and the distance between by the factor of 2, 2.5, 4.0, and 5.0 until the "best human readable"
            // division of the axis has been reached.
            int iDivLineDistValDecimalFactor = 10;
            double fDivLineDistValDecimalFactor = 1.0;
            double fDivLineDistValBeforeCorr = fDivLineDistVal;
            double fDivLineDistPixBeforeCorr = fDivLineDistPix;

            // As long as the grid lines are too close to each other ...
            while( fDivLineDistPix < i_iDivLineDistMinPix )
            {
                switch( iDivLineDistValDecimalFactor )
                {
                    case 10:
                    {
                        iDivLineDistValDecimalFactor = 20;
                        fDivLineDistValDecimalFactor *= (20.0/10.0);
                        break;
                    }
                    case 20:
                    {
                        if( i_bUseDivLineDistValDecimalFactor25 )
                        {
                            iDivLineDistValDecimalFactor = 25;
                            fDivLineDistValDecimalFactor *= (25.0/20.0);
                        }
                        else
                        {
                            iDivLineDistValDecimalFactor = 50;
                            fDivLineDistValDecimalFactor *= (50.0/25.0);
                        }
                        break;
                    }
                    case 25:
                    {
                        iDivLineDistValDecimalFactor = 50;
                        fDivLineDistValDecimalFactor *= (50.0/25.0);
                        break;
                    }
                    case 50:
                    {
                        iDivLineDistValDecimalFactor = 10;
                        fDivLineDistValDecimalFactor *= (100.0/50.0);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
                fDivLineDistVal = fDivLineDistValDecimalFactor * fDivLineDistValBeforeCorr;
                fDivLineDistPix = fDivLineDistValDecimalFactor * fDivLineDistPixBeforeCorr;
            }
        }

        // The value of the first grid line must be a whole number multiple of the distance
        // between two grid lines:
        modf(i_fScaleMinVal/fDivLineDistVal, &fDivLineFirstVal);
        fDivLineFirstVal *= fDivLineDistVal;
        if( fDivLineFirstVal < i_fScaleMinVal-DBL_EPSILON )
        {
            fDivLineFirstVal += fDivLineDistVal;
        }
        fDivLineDistFirstPix = fScaleRangeFacPixDivVal*(fDivLineFirstVal-i_fScaleMinVal);

        // Calculate number of division lines. Algorithm could be optimized ...

        // If the value 0.0 is included ...
        if( i_fScaleMinVal < 0.0 && i_fScaleMaxVal > 0.0 )
        {
            // ... there should be always a line at 0.0 ...
            int    iDivLineCountBelowZero;
            double fDivLineOffsetVal;
            double fDivLineOffsetPix;

            iDivLineCountBelowZero = static_cast<int>(-i_fScaleMinVal/fDivLineDistVal);
            fDivLineOffsetVal      = -(i_fScaleMinVal + iDivLineCountBelowZero*fDivLineDistVal);
            fDivLineOffsetPix      = fScaleRangeFacPixDivVal*fDivLineOffsetVal;
            fDivLineFirstVal       = i_fScaleMinVal + fDivLineOffsetVal;
            fDivLineDistFirstPix   = fDivLineOffsetPix;
        }

        iDivLineCount = 0;
        double fDivLineVal = fDivLineFirstVal; // please note that the first digit of "DivLineFirstVal" is "1".
        if( fDivLineVal >= i_fScaleMinVal )
        {
            iDivLineCount++;
        }
        while( fDivLineVal <= i_fScaleMaxVal )
        {
            iDivLineCount++;
            fDivLineVal += fDivLineDistVal;

            // If DivLineDistVal is very small (but still greater than DBL_EPSILON),
            // adding fDivLineDistVal to fDivLineVal may not change the value of fDivLineVal
            // (maybe bug in floating point library?). To avoid an endless loop ...
            if( fDivLineFirstVal + iDivLineCount*fDivLineDistVal > i_fScaleMaxVal )
            {
                break;
            }
        }
        if( fDivLineVal > i_fScaleMaxVal )
        {
            iDivLineCount--;
        }
        if( o_pfDivLineFirstVal != nullptr )
        {
            *o_pfDivLineFirstVal = fDivLineFirstVal;
        }
        if( o_pfDivLineDistFirstPix != nullptr )
        {
            *o_pfDivLineDistFirstPix = fDivLineDistFirstPix;
        }
        if( o_pfDivLineDistVal != nullptr )
        {
            *o_pfDivLineDistVal = fDivLineDistVal;
        }
        if( o_pfDivLineDistPix != nullptr )
        {
            *o_pfDivLineDistPix = fDivLineDistPix;
        }
    }

    if (areMethodCallsActive(i_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs =
            "DivLines {FirstVal: " + QString(o_pfDivLineFirstVal == nullptr ? "null" : QString::number(*o_pfDivLineFirstVal)) +
            ", FirstPix: " + QString(o_pfDivLineDistFirstPix == nullptr ? "null" : QString::number(*o_pfDivLineDistFirstPix)) +
            ", DistVal: " + QString(o_pfDivLineDistVal == nullptr ? "null" : QString::number(*o_pfDivLineDistVal)) +
            ", DistPix: " + QString(o_pfDivLineDistPix == nullptr ? "null" : QString::number(*o_pfDivLineDistPix)) + "}";
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(iDivLineCount);
    }
    return iDivLineCount;
}
