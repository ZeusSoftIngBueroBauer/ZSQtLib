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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysException.h"
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
double Math::round2nearest( double i_fVal, int i_iTrailingDigits )
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
        #ifdef _WINDOWS
        #pragma warning(disable:4996)
        szFcvt = _fcvt(i_fVal, iTrailingDigits, &iDecimalPos, &iSign);
        #pragma warning(default:4996)
        #elif defined __linux__
        szFcvt = fcvt(i_fVal, iTrailingDigits, &iDecimalPos, &iSign);
        #else
        #error Neither _WINDOWS nor __linux__ defined.
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

} // round2nearest

//------------------------------------------------------------------------------
double Math::round2lower( double i_fVal, int i_iTrailingDigits )
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

} // round2lower

//------------------------------------------------------------------------------
double Math::round2upper( double i_fVal, int i_iTrailingDigits )
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

} // round2upper

//------------------------------------------------------------------------------
double Math::round2Res( double i_fVal, double i_fRes )
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

} // round2Res

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

