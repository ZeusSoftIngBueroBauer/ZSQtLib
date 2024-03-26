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
#include <cmath>

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;

//------------------------------------------------------------------------------
/*! @brief Returns the square of the given value.

    @param i_fVal [in]
        Value to be squared.
    @return Square of given value.
*/
double Math::sqr( double i_fVal )
//------------------------------------------------------------------------------
{
    return i_fVal*i_fVal;
}

//------------------------------------------------------------------------------
/*! @brief Returns the square root of the given value.

    @param i_fVal [in]
        Value whose square root is computed.
    @return Square of given value.
*/
double Math::sqrt( double i_fVal )
//------------------------------------------------------------------------------
{
    if (i_fVal < 0) {
        QString strAddErrInfo = "Math::sqrt(" + QString::number(i_fVal) + ")";
        throw ZS::System::CException(__FILE__, __LINE__, EResultValueOutOfRange, strAddErrInfo);
    }
    return ::sqrt(i_fVal);
}

//------------------------------------------------------------------------------
/*! @brief Returns the absolute value of the given value.

    @param i_fVal [in]
        Value for which the absolute value should be computed.
    @return Absolute value of the given value.
*/
double Math::abs( double i_fVal )
//------------------------------------------------------------------------------
{
    double fVal = i_fVal;

    if( i_fVal < 0.0 )
    {
        fVal *= -1.0;
    }
    return fVal;
}

//------------------------------------------------------------------------------
double Math::normalizeAngleInRad( double i_fAngle_rad )
//------------------------------------------------------------------------------
{
    double fAbsAngle_rad = fabs(i_fAngle_rad);
    if (fAbsAngle_rad >= c_f2PI) {
        int iFullCircles = static_cast<int>(fAbsAngle_rad / c_f2PI);
        if (iFullCircles > 0) {
            fAbsAngle_rad -= (static_cast<double>(iFullCircles) * c_f2PI);
        }
    }
    return fAbsAngle_rad;
}

//------------------------------------------------------------------------------
double Math::normalizeAngleInDegree( double i_fAngle_degree )
//------------------------------------------------------------------------------
{
    double fAbsAngle_degree = fabs(i_fAngle_degree);
    if (fAbsAngle_degree >= 360.0) {
        int iFullCircles = static_cast<int>(fAbsAngle_degree) / 360;
        if (iFullCircles > 0) {
            fAbsAngle_degree -= (static_cast<double>(iFullCircles) * 360.0);
        }
    }
    return fAbsAngle_degree;
}

//------------------------------------------------------------------------------
/*! @brief Converts the angle given in rads to an angle in degrees.

    When the normalize flag is set to true the result is adjusted to start
    from 0.0 degrees to 360.0 degrees.
    E.g. an angle of -PI/2 (-90 degrees) is normalized to 270.0 degrees.
    360.0 degrees will be normalized to 0.0 degrees.

    @param i_fVal_rad [in]
        Angle given in rads.
    @param i_bNormalize [in]
        Set to true to normalize the result to angles starting from 0.0 degrees to 360.0.
        360.0 degrees will be normalized to 0.0 degrees.

    @return Value converted into degrees.

    @Examples:
        rad2Degree(0.0, true) = 0.0
        rad2Degree(0.0, false) = 0.0
        rad2Degree(PI/4, true) = 45.0
        rad2Degree(PI/4, false) = 45.0
        rad2Degree(-PI/4, true) = 315.0
        rad2Degree(-PI/4, false) = -45.0
        rad2Degree(5PI/4, true) = 225.0
        rad2Degree(-5PI/4, false) = -135.0
        rad2Degree(7PI/4, true) = 315.0
        rad2Degree(-7PI/4, true) = 45.0
        rad2Degree(2PI, true) = 0.0
        rad2Degree(2PI, false) = 360.0
        rad2Degree(-2PI, true) = 0.0
        rad2Degree(-2PI, false) = -360.0
        rad2Degree(11PI/4, true) = 135.0
        rad2Degree(11PI/4, false) = 495.0
        rad2Degree(-11PI/4, true) = 225.0
        rad2Degree(-11PI/4, false) = -495.0
*/
double Math::rad2Degree( double i_fAngle_rad, bool i_bNormalize )
//------------------------------------------------------------------------------
{
    double fAngle_degree = 360.0 * i_fAngle_rad / Math::c_f2PI;
    if (i_bNormalize) {
        double fAbsAngle_degree = normalizeAngleInDegree(fAngle_degree);
        fAngle_degree = fAbsAngle_degree;
        if (i_fAngle_rad < 0.0 && fAbsAngle_degree > 0.0) {
            fAngle_degree = 360.0 - fAbsAngle_degree;
        }
    }
    return fAngle_degree;
}

//------------------------------------------------------------------------------
/*! @brief Converts the angle given in degrees to an angle in rad.

    When the normalize flag is set to true the result is adjusted to start
    from 0.0 degrees to 360.0 degrees.
    E.g. an angle of -PI/2 (-90 degrees) is normalized to 270.0 degrees.
    360.0 degrees will be normalized to 0.0 degrees.

    @param i_fVal_deg [in]
        Angle given in degrees.
    @param i_bNormalize [in]
        Set to true to normalize the result to angles starting from 0.0 rad to 2PI.
        2PI (360.0 degrees) will be normalized to 0.0 rad (0.0 degrees).

    @return Value converted into rad.

    @Examples:
        degree2Rad(0.0, true) = 0.0
        degree2Rad(0.0, false) = 0.0
        degree2Rad(45.0, true) = PI/4
        degree2Rad(45.0, false) = PI/4
        degree2Rad(-45.0, true) = 7PI/4
        degree2Rad(-45.0, false) = -PI/4
        degree2Rad(225.0, true) = 5PI/4
        degree2Rad(-225.0, false) = -5PI/4
        degree2Rad(315.0, true) = 7PI/4
        degree2Rad(-315.0, true) = PI/4
        degree2Rad(360.0, true) = 0.0
        degree2Rad(360.0, false) = 2PI
        degree2Rad(-360.0, true) = 0.0
        degree2Rad(-360.0, false) = -2PI
        degree2Rad(495.0, true) = 3PI/4
        degree2Rad(495.0, false) = 11PI/4
        degree2Rad(-495.0, true) = 3PI/4
        degree2Rad(-495.0, false) = -11PI/4
*/
double Math::degree2Rad( double i_fAngle_degree, bool i_bNormalize )
//------------------------------------------------------------------------------
{
    double fAngle_rad = Math::c_f2PI * i_fAngle_degree / 360.0;
    if (i_bNormalize) {
        double fAbsAngle_rad = normalizeAngleInRad(fAngle_rad);
        fAngle_rad = fAbsAngle_rad;
        if (i_fAngle_degree < 0.0 && fAbsAngle_rad > 0.0) {
            fAngle_rad = c_f2PI - fAbsAngle_rad;
        }
    }
    return fAngle_rad;
}

//------------------------------------------------------------------------------
/*! @brief Rounds the given value to the nearest absolute value at the given trailing digit.

    @param i_fVal [in]
        Value to be rounded.
    @param i_iTrailingDigits [in]
        Trailing digits to be where the rounding should take place.
        If set to -1 the maximum digits according computation accuracy of floating point unit is used.

    @return Rounded value.

    @Examples
        round2Nearest(1.15, 0) = 1.0
        round2Nearest(1.14, 1) = 1.1
        round2Nearest(1.16, 1) = 1.2
        round2Nearest(-1.15, 0) = -1.0
        round2Nearest(-1.14, 1) = -1.1
        round2Nearest(-1.16, 1) = -1.2
        round2Nearest(1.65, 0) = 2.0
        round2Nearest(1.64, 1) = 1.6
        round2Nearest(1.66, 1) = 1.7
        round2Nearest(-1.65, 0) = -2.0
        round2Nearest(-1.64, 1) = -1.6
        round2Nearest(-1.66, 1) = -1.7

    @note The function uses the following library functions:
        - fcvt(double value, int count, int *dec, int *sign)
          The function converts the floating point value to NULL-terminated ASCII string.
          A single, statically allocated buffer is used for the conversion.
          Each call destroys the result of the previous call.
          The value parameter is the floating point number to convert.
          fcvt stores the digits of value as a string, appending the character NULL ('\0').
          The count parameter specifies the number of digits to store after the decimal point.
          Excess digits are rounded to count digits. If there are fewer than count decimal places,
          the string is padded with zeros.
          Only digits are stored in the string. The decimal place and sign of value can be
          retrieved and signed from dec after the call. The dec parameter points to an integer
          value; this integer value indicates the position of the decimal relative to the
          beginning of the string. A value of zero or a negative integer value indicates that
          the decimal is to the left of the first digit.
          The sign parameter refers to an integer that indicates the sign of value.
          The integer is set to 0 if value is positive and is set to a non-zero number if value
          is negative.
        - ceil(double value)
          Computes the smallest integer value not less than value.
        - floor(double value)
          Computes the largest integer value not greater than value.
*/
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
        int iTrailingDigits = i_iTrailingDigits;
        if( i_iTrailingDigits == -1 )
        {
            iTrailingDigits = c_iCalculationAccuracyTrailingDigits;
        }

        int iDecimalPos = 0;
        int iSign = 0;

        #ifdef _MSC_VER
        #pragma warning(disable:4996)
        char* szFcvt = _fcvt(i_fVal, iTrailingDigits+1, &iDecimalPos, &iSign);
        #pragma warning(default:4996)
        #else
        char* szFcvt = fcvt(i_fVal, iTrailingDigits+1, &iDecimalPos, &iSign);
        #endif

        char* szVal = nullptr;

        if( iDecimalPos > 0 )
        {
            szVal = new char[strlen(szFcvt)+2]; // incl. decimal point and '\0'
            memset(szVal, 0x00, strlen(szFcvt)+2);

            #ifdef _WINDOWS
            #pragma warning(disable:4996)
            #endif
            strncpy(szVal, szFcvt, iDecimalPos);
            strcat(szVal, "." );
            strcat(szVal, &szFcvt[iDecimalPos]);
            #ifdef _WINDOWS
            #pragma warning(default:4996)
            #endif
            if (szVal[strlen(szVal)-1] >= '5')
            {
                ++szVal[strlen(szVal)-2];
            }
            szVal[strlen(szVal)-1] = 0x00;
        }
        else
        {
            szVal = new char[-iDecimalPos+strlen(szFcvt)+3]; // incl. '0.' and '\0'
            memset(szVal, 0x00, -iDecimalPos+strlen(szFcvt)+3);

            memset(szVal, '0', -iDecimalPos+2); // starting with '0.'
            szVal[1] = '.';
            #ifdef _WINDOWS
            #pragma warning(disable:4996)
            #endif
            strcat(szVal, szFcvt);
            #ifdef _WINDOWS
            #pragma warning(default:4996)
            #endif
            if (szVal[strlen(szVal)-1] >= '5')
            {
                ++szVal[strlen(szVal)-2];
            }
            szVal[strlen(szVal)-1] = 0x00;
        }
        fVal = atof(szVal);
        if( i_fVal < 0.0 )
        {
            fVal *= -1.0;
        }
        delete [] szVal;
        szVal = nullptr;
        szFcvt = nullptr;
    }
    return fVal;

} // round2Nearest

//------------------------------------------------------------------------------
/*! @brief Rounds the given value to the nearest lower absolute value at the given
           trailing digit.

    @param i_fVal [in]
        Value to be rounded.
    @param i_iTrailingDigits [in]
        Trailing digits where the rounding should take place.
        If set to -1 the maximum digits according computation accuracy of floating point unit is used.

    @return Rounded value.

    @Examples
        round2Lower(1.15, 0) = 1.0
        round2Lower(1.14, 1) = 1.1
        round2Lower(1.16, 1) = 1.1
        round2Lower(-1.15, 0) = -1.0
        round2Lower(-1.14, 1) = -1.1
        round2Lower(-1.16, 1) = -1.1
        round2Lower(1.65, 0) = 1.0
        round2Lower(1.64, 1) = 1.6
        round2Lower(1.66, 1) = 1.6
        round2Lower(-1.65, 0) = -1.0
        round2Lower(-1.64, 1) = -1.6
        round2Lower(-1.66, 1) = -1.6

    @note The function uses the following library functions:
        - fcvt(double value, int count, int *dec, int *sign)
          see "round2Nearest".
        - floor(double value)
          Computes the largest integer value not greater than value.

*/
double Math::round2Lower( double i_fVal, int i_iTrailingDigits )
//------------------------------------------------------------------------------
{
    double fVal = 0.0;

    if( i_iTrailingDigits == 0 )
    {
        fVal = floor(fabs(i_fVal));
        if (i_fVal < 0.0)
        {
            fVal *= -1.0;
        }
    }
    else
    {
        int iTrailingDigits = i_iTrailingDigits;
        if( i_iTrailingDigits == -1 )
        {
            iTrailingDigits = c_iCalculationAccuracyTrailingDigits;
        }

        int iDecimalPos = 0;
        int iSign = 0;

        #ifdef _MSC_VER
        #pragma warning(disable:4996)
        char* szFcvt = _fcvt(i_fVal, iTrailingDigits+2, &iDecimalPos, &iSign);
        #pragma warning(default:4996)
        #else
        char* szFcvt = fcvt(i_fVal, iTrailingDigits+2, &iDecimalPos, &iSign);
        #endif

        char* szVal = nullptr;

        if( iDecimalPos > 0 )
        {
            szVal = new char[strlen(szFcvt)+2]; // incl. decimal point and '\0'
            memset(szVal, 0x00, strlen(szFcvt)+2);

            #ifdef _WINDOWS
            #pragma warning(disable:4996)
            #endif
            strncpy(szVal, szFcvt, iDecimalPos);
            strcat(szVal, ".");
            strcat(szVal, &szFcvt[iDecimalPos]);
            #ifdef _WINDOWS
            #pragma warning(default:4996)
            #endif
            szVal[strlen(szVal)-2] = 0x00;
        }
        else
        {
            szVal = new char[-iDecimalPos+strlen(szFcvt)+3]; // incl. '0.' and '\0'
            memset(szVal, 0x00, -iDecimalPos+strlen(szFcvt)+3);

            memset(szVal, '0', -iDecimalPos+2); // starting with '0.'
            szVal[1] = '.';
            #ifdef _WINDOWS
            #pragma warning(disable:4996)
            #endif
            strcat(szVal, szFcvt);
            #ifdef _WINDOWS
            #pragma warning(default:4996)
            #endif
            szVal[strlen(szVal)-2] = 0x00;
        }
        fVal = atof(szVal);
        if (i_fVal < 0.0)
        {
            fVal *= -1.0;
        }
        delete [] szVal;
        szVal = nullptr;
    }

    return fVal;

} // round2Lower

//------------------------------------------------------------------------------
/*! @brief Rounds the given value to the nearest upper absolute value at the given
           trailing digit.

    @param i_fVal [in]
        Value to be rounded.
    @param i_iTrailingDigits [in]
        Trailing digits where the rounding should take place.
        If set to -1 the maximum digits according computation accuracy of floating point unit is used.

    @return Rounded value.

    @Examples
        round2Upper(1.15, 0) = 2.0
        round2Upper(1.14, 1) = 1.2
        round2Upper(1.16, 1) = 1.2
        round2Upper(-1.15, 0) = -2.0
        round2Upper(-1.14, 1) = -1.2
        round2Upper(-1.16, 1) = -1.2
        round2Upper(1.65, 0) = 2.0
        round2Upper(1.64, 1) = 1.7
        round2Upper(1.66, 1) = 1.7
        round2Upper(-1.65, 0) = -2.0
        round2Upper(-1.64, 1) = -1.7
        round2Upper(-1.66, 1) = -1.7

    @note The function uses the following library functions:
        - fcvt(double value, int count, int *dec, int *sign)
          see "round2Nearest".
        - ceil(double value)
          Computes the smallest integer value not less than value.
*/
double Math::round2Upper( double i_fVal, int i_iTrailingDigits )
//------------------------------------------------------------------------------
{
    double fVal = 0.0;

    if( i_iTrailingDigits == 0 )
    {
        fVal = ceil(fabs(i_fVal));
        if (i_fVal < 0.0)
        {
            fVal *= -1.0;
        }
    }
    else
    {
        int iTrailingDigits = i_iTrailingDigits;
        if( i_iTrailingDigits == -1 )
        {
            iTrailingDigits = c_iCalculationAccuracyTrailingDigits;
        }

        int iDecimalPos = 0;
        int iSign = 0;

        #ifdef _MSC_VER
        #pragma warning(disable:4996)
        char* szFcvt = _fcvt(i_fVal, iTrailingDigits+2, &iDecimalPos, &iSign);
        #pragma warning(default:4996)
        #else
        char* szFcvt = fcvt(i_fVal, iTrailingDigits+2, &iDecimalPos, &iSign);
        #endif

        char* szVal = nullptr;

        if( iDecimalPos > 0 )
        {
            szVal = new char[strlen(szFcvt)+2]; // incl. decimal point and '\0'
            memset(szVal, 0x00, strlen(szFcvt)+2);

            #ifdef _WINDOWS
            #pragma warning(disable:4996)
            #endif
            strncpy(szVal, szFcvt, iDecimalPos);
            strcat(szVal, ".");
            strcat(szVal, &szFcvt[iDecimalPos]);
            #ifdef _WINDOWS
            #pragma warning(default:4996)
            #endif
            if (szVal[strlen(szVal)-2] != '0')
            {
                ++szVal[strlen(szVal)-3];
            }
            szVal[strlen(szVal)-2] = 0x00;
        }
        else
        {
            szVal = new char[-iDecimalPos+strlen(szFcvt)+3]; // incl. '0.' and '\0'
            memset(szVal, 0x00, -iDecimalPos+strlen(szFcvt)+3);

            memset(szVal, '0', -iDecimalPos+2); // starting with '0.'
            szVal[1] = '.';
            #ifdef _WINDOWS
            #pragma warning(disable:4996)
            #endif
            strcat(szVal, szFcvt);
            #ifdef _WINDOWS
            #pragma warning(default:4996)
            #endif
            if (szVal[strlen(szVal)-2] != '0')
            {
                ++szVal[strlen(szVal)-3];
            }
            szVal[strlen(szVal)-2] = 0x00;
        }
        fVal = atof(szVal);
        if (i_fVal < 0.0)
        {
            fVal *= -1.0;
        }
        delete [] szVal;
        szVal = nullptr;
    }

    return fVal;

} // round2Upper

//------------------------------------------------------------------------------
/*! @brief Rounds the given value to the defined resolution value.

    @param i_fVal [in]
        Value to be rounded.
    @param i_fRes [in]
        Resolution the value should be rounded to.
        If 0.0 is passed here the value is not rounded.

    @return Rounded value.

    @examples
        round2Resolution(0.0, 0.0) = 0.0
        round2Resolution(5.15, 0.0) = 5.15
        round2Resolution(5.15, 10.0) = 10.0
        round2Resolution(5.45, 0.1) = 5.5
        round2Resolution(5.55, 0.1) = 5.6
        round2Resolution(5.65, 0.1) = 5.7
        round2Resolution(5.45, 0.01) = 5.45
        round2Resolution(5.55, 0.01) = 5.55
        round2Resolution(5.65, 0.01) = 5.65

    @note The function uses the following library functions:
        - floor(double value)
          Computes the largest integer value not greater than value.
*/
double Math::round2Resolution( double i_fVal, double i_fRes )
//------------------------------------------------------------------------------
{
    double fVal = i_fVal;
    double fRes = fabs(i_fRes);

    if( fRes > 0.0 )
    {
        fVal = i_fVal / fRes;
        if (fVal >= 0.0)
        {
            fVal = fVal + 0.5;
            if (fRes < 1.0)
            {
                fVal += fRes/10.0;
            }
            fVal = floor(fVal);
        }
        else
        {
            fVal = fVal - 0.5;
            if (fRes < 1.0)
            {
                fVal -= fRes/10.0;
            }
            fVal = ceil(fVal);
        }
        fVal *= fRes;
    }
    return fVal;
}

//------------------------------------------------------------------------------
/*! @brief Rounds the value to the lower decade if the value is not a whole
           number multiple of a decade.

    This means that the values 10.0, 10000.0, 0.001 etc. are not rounded.

    @param i_fVal [in]
        Value to be rounded.

    @return Value rounded to the lower decade.

    @examples
        round2LowerDecade(0.01) = 0.01
        round2LowerDecade(0.056734) = 0.01
        round2LowerDecade(10.0) = 10.0
        round2LowerDecade(56734.45) = 10000
*/
double Math::round2LowerDecade( double i_fVal )
//------------------------------------------------------------------------------
{
    int iSign = i_fVal < 0.0 ? -1 : 1;
    double fVal  = fabs(i_fVal);

    if( fVal > 0.0 )
    {
        double fLog  = log10(fVal);
        double fFrac = modf(fLog, &fLog);

        if( fVal < 1.0 )
        {
            if( fFrac != 0.0 )
            {
                fLog -= 1.0;
            }
        }
        fVal = pow(10.0, fLog);
    }
    return iSign*fVal;
}

//------------------------------------------------------------------------------
/*! @brief Rounds the value to the upper decade if the value is not a whole
           number multiple of a decade.

    This means that the values 10.0, 10000.0, 0.001 etc. are not rounded.

    @param i_fVal [in]
        Value to be rounded.

    @return Value rounded to the upper decade.

    @examples
        round2UpperDecade(0.01) = 0.01
        round2UpperDecade(0.056734) = 0.1
        round2UpperDecade(10.0) = 10.0
        round2UpperDecade(56734.45) = 100000
*/
double Math::round2UpperDecade( double i_fVal )
//------------------------------------------------------------------------------
{
    int iSign = i_fVal < 0.0 ? -1 : 1;
    double fVal = fabs(i_fVal);

    if( fVal > 0.0 )
    {
        double fLog  = log10(fVal);
        double fFrac = modf(fLog, &fLog);

        if( fVal >= 1.0 )
        {
            if( fFrac != 0.0 )
            {
                fLog += 1.0;
            }
        }
        fVal = pow(10.0, fLog);
    }
    return iSign*fVal;
}

//------------------------------------------------------------------------------
/*! @brief Rounds the value to next lower value which will be a whole number
           multiple of a decade.

    This means that the values 10.0, 10000.0, 0.001 etc. are not rounded.

    @param i_fVal [in]
        Value to be rounded.

    @return Rounded value.

    @examples
        round2LowerIntMultOfTen(0.01) = 0.01
        round2LowerIntMultOfTen(0.056734) = 0.05
        round2LowerIntMultOfTen(10.0) = 10.0
        round2LowerIntMultOfTen(56734.45) = 50000
*/
double Math::round2LowerIntMultOfTen( double i_fVal )
//------------------------------------------------------------------------------
{
    double fVal = fabs(i_fVal);

    if( fVal > 0.0 )
    {
        int iSign = 1;
        int iMant = 0;
        int iExp  = 0;

        normalize(i_fVal, &iSign, &iMant, &iExp);

        fVal = iSign * iMant * pow(10.0, iExp);
    }
    return fVal;
}

//------------------------------------------------------------------------------
/*! @brief Rounds the value to next higher value which will be a whole number
           multiple of a decade.

    This means that the values 10.0, 10000.0, 0.001 etc. are not rounded.

    @param i_fVal [in]
        Value to be rounded.

    @return Rounded value.

    @examples
        round2LowerIntMultOfTen(0.01) = 0.01
        round2LowerIntMultOfTen(0.056734) = 0.06
        round2LowerIntMultOfTen(10.0) = 10.0
        round2LowerIntMultOfTen(56734.45) = 60000
*/
double Math::round2UpperIntMultOfTen( double i_fVal )
//------------------------------------------------------------------------------
{
    double fVal = fabs(i_fVal);

    if( fVal > 0.0 )
    {
        double fLog  = log10(fVal);
        double fFrac = modf(fLog, &fLog);

        int iSign = 1;
        int iMant = 0;
        int iExp  = 0;

        normalize(i_fVal, &iSign, &iMant, &iExp);

        if( fFrac != 0.0 )
        {
            iMant += 1;
        }
        fVal = iSign * iMant * pow(10.0, iExp);
    }
    return fVal;
}

//------------------------------------------------------------------------------
/*! @brief Returns the minimum value.

    @param i_iValCount [in]
        Number of passed values.
    @param i_iVal1 ... [in]
        Variadic list of values for which the miniumum value should be calculated.

    @return Minimum value.
*/
int Math::minVal( int i_iValCount, int i_iVal1 ... )
//------------------------------------------------------------------------------
{
    int iMinVal = i_iVal1;

    va_list ap;
    va_start(ap,i_iVal1);

    for( int iValIdx = 1; iValIdx < i_iValCount; iValIdx++ )
    {
        int iVal = va_arg(ap,int);

        if( iVal < iMinVal )
        {
            iMinVal = iVal;
        }
    }
    va_end(ap);

    return iMinVal;
}

//------------------------------------------------------------------------------
/*! @brief Returns the minimum value.

    @param i_iValCount [in]
        Number of passed values.
    @param i_fVal1 ... [in]
        Variadic list of values for which the miniumum value should be calculated.

    @return Minimum value.
*/
double Math::minVal( int i_iValCount, double i_fVal1 ... )
//------------------------------------------------------------------------------
{
    double fMinVal = i_fVal1;

    va_list ap;
    va_start(ap, i_fVal1);

    for( int iValIdx = 1; iValIdx < i_iValCount; iValIdx++ )
    {
        double fVal = va_arg(ap, double);

        if( fVal < fMinVal )
        {
            fMinVal = fVal;
        }
    }
    va_end(ap);

    return fMinVal;
}

//------------------------------------------------------------------------------
/*! @brief Returns the maximum value.

    @param i_iValCount [in]
        Number of passed values.
    @param i_iVal1 ... [in]
        Variadic list of values for which the maximum value should be calculated.

    @return Maximum value.
*/
int Math::maxVal( int i_iValCount, int i_iVal1 ... )
//------------------------------------------------------------------------------
{
    int iMaxVal = i_iVal1;

    va_list ap;
    va_start(ap,i_iVal1);

    for( int iValIdx = 1; iValIdx < i_iValCount; iValIdx++ )
    {
        int iVal = va_arg(ap,int);

        if( iVal > iMaxVal )
        {
            iMaxVal = iVal;
        }
    }
    va_end(ap);

    return iMaxVal;
}

//------------------------------------------------------------------------------
/*! @brief Returns the maximum value.

    @param i_iValCount [in]
        Number of passed values.
    @param i_fVal1 ... [in]
        Variadic list of values for which the maximum value should be calculated.

    @return Maximum value.
*/
double Math::maxVal( int i_iValCount, double i_fVal1 ... )
//------------------------------------------------------------------------------
{
    double fMaxVal = i_fVal1;

    va_list ap;
    va_start(ap,i_fVal1);

    for( int iValIdx = 1; iValIdx < i_iValCount; iValIdx++ )
    {
        double fVal = va_arg(ap,double);

        if( fVal > fMaxVal )
        {
            fMaxVal = fVal;
        }
    }
    va_end(ap);

    return fMaxVal;
}

//------------------------------------------------------------------------------
/*! @brief Returns the absolute minimum and absolute maximum values of the
           values passed in the array of double values.

    @param i_arVals [in]
        Array with double values for which the absolute minimum and absolute
        maximum value should be returned.

    @return Tuple with minimum and maximum value.
        std::tuple<double, double> minMax = Math::getAbsMinMax(arVals);
        double fMin = std::get<0>(minMax);
        double fMax = std::get<1>(minMax);
*/
std::tuple<double, double> Math::getAbsMinMax(const QVector<double>& i_arVals)
//------------------------------------------------------------------------------
{
    double fMin = DBL_MAX;
    double fMax = DBL_EPSILON;
    for (int idx = 0; idx < i_arVals.size(); ++idx)
    {
        double fTmp = fabs(i_arVals[idx]);
        if( fTmp > fMax ) {
            fMax = fTmp;
        }
        if( fTmp < fMin && fTmp > 0.0 ) {
            fMin = fTmp;
        }
    }
    return std::tuple<double, double>{fMin, fMax};
}

//------------------------------------------------------------------------------
/*! @brief Returns the first significant digit necessary to indicate the value.

    If the given value is greater than 1.0 the first significant digit is a leading digit.
    If the given value is less than 1.0 the first significant digit is a trailing digit.

    @Examples
        - getFirstSignificantDigit(0.0): 0
        - getFirstSignificantDigit(2.0): 1
        - getFirstSignificantDigit(10.0): 2
        - getFirstSignificantDigit(3452.78): 4
        - getFirstSignificantDigit(0.00056): -4

    @param i_fVal [in]
        Value for which the first significant digit should be detected.
        The value 0.0 makes no sense and 0 is returned.

    @return Index of the first significant digit.
        If the given value is greater than 1.0 the first significant digit
        is a leading digit and a positive value is returned.
        If the given value is less than 1.0 the first significant digit
        is a trailing digit and a negative value is returned.
        If the value is 0.0 there is no first significant digit
        and 0 is returned.
*/
int Math::getFirstSignificantDigit( double i_fVal )
//------------------------------------------------------------------------------
{
    int iFirstDigit = 0;

    if (i_fVal != 0.0)
    {
        double fAbsVal = fabs(i_fVal);

        if (fAbsVal >= 1.0 && fAbsVal < 10.0)
        {
            iFirstDigit = 1;
        }
        else // if (fAbsVal < 1.0 || fAbsVal >= 10.0)
        {
            double fLogInt = 0.0;
            double fLog = log10(fAbsVal);
            double fLogFrac = modf(fLog, &fLogInt);

            if (fAbsVal >= 10.0)
            {
                iFirstDigit = static_cast<int>(fLogInt) + 1;
            }
            else // if (fAbsVal < 1.0)
            {
                iFirstDigit = static_cast<int>(fLogInt);
                if (fLogFrac < 0.0) {
                    --iFirstDigit;
                }
            }
        }
    }
    return iFirstDigit;
}

//------------------------------------------------------------------------------
/*! @brief Normalizes the given to a value between 1.0 and 10.0 and returns its
           parts sign, the first significant digit of the mantissa and its exponent.

    @param i_fVal [in]
        Value to be normalized.
    @param o_piSign [out]
        If != nullptr the sign of the given value is returned.
    @param o_piMant [out]
        If != nullptr the sign of the first significant digit of the normalized
        value is returned.
    @param o_piExp [out]
        If != nullptr the exponent of the normalized value is returned.

    @example
        int iSign = 1;
        int iMant = 0;
        int iExp  = 0;
        normalize(0.054734, &iSign, &iMant, &iExp);
        -> iSign = 1; iMant = 5; iExp = -2
*/
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
        double fValTmp = log10(fValAbs);
        iValExp = static_cast<int>(fValTmp);

        double fIntTmp = 0.0;
        double fFractTmp = modf(fValTmp, &fIntTmp);
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
/*! @brief Window function to be applied for FFTs.

    @param i_fVal [in]
        Value to be modified.
    @param i_iFFTSegmentLen [in]

    @return Modified value.
*/
double Math::fctFFTWindowRect( double /*i_fVal*/, int /*i_iFFTSegmentLen*/ )
//------------------------------------------------------------------------------
{
    return 1.0;
}

//------------------------------------------------------------------------------
/*! @brief Window function to be applied for FFTs.

    @param i_fVal [in]
        Value to be modified.
    @param i_iFFTSegmentLen [in]

    @return Modified value.
*/
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
/*! @brief Window function to be applied for FFTs.

    @param i_fVal [in]
        Value to be modified.
    @param i_iFFTSegmentLen [in]

    @return Modified value.
*/
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
/*! @brief Window function to be applied for FFTs.

    @param i_fVal [in]
        Value to be modified.
    @param i_iFFTSegmentLen [in]

    @return Modified value.
*/
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
/*! @brief Window function to be applied for FFTs.

    @param i_fVal [in]
        Value to be modified.
    @param i_iFFTSegmentLen [in]

    @return Modified value.
*/
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
/*! @brief Returns the given logarithmic resolution of the given linear
           value into a linear resolution.

    @param i_fResLog [in]
        Logarithmic value of the resolution of the linear value.
    @param i_fValLin [in]
        The linear value for which the logarithmic resolution is defined.

    @return Linear resolution value.
*/
double Math::logRes2LinRes( double i_fResLog, double i_fValLin )
//------------------------------------------------------------------------------
{
    //fValLin = i_fValLin/11.0;
    //fValLin = round2LowerDecade(fValLin);
    //fResLin = fValLin * pow(10.0,fResLog);
    //fResLin = round2LowerDecade(fResLin);

    double fValLog = log10(i_fValLin);
    //fValLog1 = fValLog - fResLog/2.0;
    //fValLog2 = fValLog + fResLog/2.0;
    double fValLog1 = fValLog;
    double fValLog2 = fValLog + i_fResLog;

    double fValLin1 = pow(10.0,fValLog1);
    double fValLin2 = pow(10.0,fValLog2);
    double fValLinDiff = fValLin2 - fValLin1;

    return Math::round2LowerDecade(fValLinDiff);
}

//------------------------------------------------------------------------------
/*! @brief Returns the rectangle for the given start and end points.

    @param i_ptStart [in] Start point.
    @param i_ptEnd [in] End point.

    @return Covered rectangle.
*/
QRect Math::calcRect( const QPoint& i_ptStart, const QPoint& i_ptEnd )
//------------------------------------------------------------------------------
{
    QRect rct;

    // Previously zoomed area:
    if( i_ptStart.x() < i_ptEnd.x() )
    {
        rct.setX(i_ptStart.x());
        rct.setWidth(i_ptEnd.x()-i_ptStart.x()+1); //lint !e834
    }
    else
    {
        rct.setX(i_ptEnd.x());
        rct.setWidth(i_ptStart.x()-i_ptEnd.x()+1); //lint !e834
    }
    if( i_ptStart.y() < i_ptEnd.y() )
    {
        rct.setY(i_ptStart.y());
        rct.setHeight(i_ptEnd.y()-i_ptStart.y()+1); //lint !e834
    }
    else
    {
        rct.setY(i_ptEnd.y());
        rct.setHeight(i_ptStart.y()-i_ptEnd.y()+1); //lint !e834
    }
    return rct;
}

//------------------------------------------------------------------------------
/*! @brief Merges the given arrays into one array.

    Each of the passed arrays must have already been sorted in ascending order
    (lowest values first).

    The resulting array is sorted in ascending order (lowest values first)
    and duplicates are removed.

    @param i_arfVals1 [in]
        Array to be merged with i_arVals2. The values must be in ascending order.
    @param i_arfVals2 [in]
        Array to be merged with i_arVals1. The values must be in ascending order.

    @return Merged and sorted array (duplicates removed).
*/
QVector<double> Math::merge(const QVector<double>& i_arfVals1, const QVector<double>& i_arfVals2)
//------------------------------------------------------------------------------
{
    QVector<double> arfValsMerged;
    if (i_arfVals1.size() > 0 && i_arfVals2.size() == 0) {
        double fVal1 = i_arfVals1[0];
        arfValsMerged.append(fVal1);
        for (int idx1 = 0; idx1 < i_arfVals1.size()-1; ++idx1) {
            fVal1 = i_arfVals1[idx1];
            double fVal1Next = i_arfVals1[idx1+1];
            if (fVal1Next > fVal1) {
                arfValsMerged.append(fVal1Next);
            }
        }
    }
    else if (i_arfVals1.size() == 0 && i_arfVals2.size() > 0) {
        double fVal2 = i_arfVals2[0];
        arfValsMerged.append(fVal2);
        for (int idx2 = 0; idx2 < i_arfVals2.size()-1; ++idx2) {
            fVal2 = i_arfVals2[idx2];
            double fVal2Next = i_arfVals2[idx2+1];
            if (fVal2Next > fVal2) {
                arfValsMerged.append(fVal2Next);
            }
        }
    }
    else if (i_arfVals1.size() > 0 && i_arfVals2.size() > 0)
    {
        // First add all values from array 2 which are less than the first value from array 1.
        int idx2 = 0;
        double fVal1 = i_arfVals1[0];
        for (; idx2 < i_arfVals2.size(); ++idx2) {
            double fVal2 = i_arfVals2[idx2];
            if (fVal2 < fVal1) {
                arfValsMerged.append(fVal2);
            }
            else {
                break;
            }
        }
        arfValsMerged.append(fVal1);

        // Second add all values between first and last main value.
        for (int idx1 = 0; idx1 < i_arfVals1.size()-1; ++idx1) {
            fVal1 = i_arfVals1[idx1];
            double fVal1Next = i_arfVals1[idx1+1];
            for (; idx2 < i_arfVals2.size(); ++idx2) {
                double fVal2 = i_arfVals2[idx2];
                if (fVal2 >= fVal1Next) {
                    break;
                }
                else if (fVal2 == fVal1) {
                    // Duplicates will be ignored.
                }
                else {
                    arfValsMerged.append(fVal2);
                }
            }
            if (fVal1Next > fVal1) {
                arfValsMerged.append(fVal1Next);
            }
        }

        // Last add all values from array 2 which are greater than the last value from array 2.
        fVal1 = i_arfVals1[i_arfVals1.size()-1];
        for (; idx2 < i_arfVals2.size(); ++idx2) {
            double fVal2 = i_arfVals2[idx2];
            if (fVal2 > fVal1) {
                arfValsMerged.append(fVal2);
            }
        }
    }
    return arfValsMerged;
}
