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

#include "ZSSys/ZSSysMathScaleDivLines.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::Math;


/*******************************************************************************
class CScaleDivLines
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

QVector<double> CScaleDivLines::s_arfScaleRangeFacPixDivValLog;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Calculates the precision need to show each given value with a unique string.

    @param i_iExponentDigits [in]
        0 if the values should be indicated without exponent.
        > 0 if the values should be indicated in engineering format with the number
            of specified digits for the exponent.
    @param i_iPrecisionMin [in]
        Range [0..N]
        Minimum allowed precision.
    @param i_iPrecisionMax [in]
        Range [1..N]
        Maximum allowed precision.

    @return Calculated precision.
*/
int CScaleDivLines::getPrecision2ShowUniqueNumbers(
    const QVector<double> i_arfVals,
    int i_iExponentDigits,
    int i_iPrecisionMin,
    int i_iPrecisionMax,
    CTrcAdminObj* i_pTrcAdminObj)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(i_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ExpDigits: " + QString::number(i_iExponentDigits) +
                       ", PrecMin: " + QString::number(i_iPrecisionMin) +
                       ", PrecMax: " + QString::number(i_iPrecisionMax) +
                       ", Vals [" + QString::number(i_arfVals.size()) + "]";
        if (areMethodCallsActive(i_pTrcAdminObj, EMethodTraceDetailLevel::ArgsDetailed)) {
            if (i_arfVals.size() > 0) {
                strMthInArgs += "(";
                for (int idxVal = 0; idxVal < i_arfVals.size(); ++idxVal) {
                    if ((idxVal >= 5 && idxVal < i_arfVals.size() - 5) && !areMethodCallsActive(i_pTrcAdminObj, EMethodTraceDetailLevel::ArgsVerbose)) {
                        idxVal = i_arfVals.size() - 5;
                    }
                    strMthInArgs += QString::number(i_arfVals[idxVal]);
                }
                strMthInArgs += ")";
            }
        }
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ i_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "Math::getPrecision2ShowUniqueNumbers",
        /* strAddInfo   */ strMthInArgs );

    int iPrecision = i_iPrecisionMin;
    int idx1 = 0;
    while (idx1 < i_arfVals.size() && iPrecision < i_iPrecisionMax)
    {
        double fVal1 = i_arfVals[idx1];
        QString strVal1;
        if (i_iExponentDigits == 0) {
            strVal1 = QString::number(fVal1, 'f', iPrecision);
        }
        else {
            strVal1 = QString::number(fVal1, 'e', iPrecision);
        }

        for (int idx2 = idx1+1; idx2 < i_arfVals.size();  ++idx2)
        {
            double fVal2 = i_arfVals[idx2];
            QString strVal2;
            if (i_iExponentDigits == 0) {
                strVal2 = QString::number(fVal2, 'f', iPrecision);
            }
            else {
                strVal2 = QString::number(fVal2, 'e', iPrecision);
            }
            // Two successive labels may not be equal ...
            if( strVal1 == strVal2 )
            {
                ++iPrecision;
                idx1 = 0;
                break;
            }
        }
        ++idx1;
    }

    if (areMethodCallsActive(i_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(iPrecision);
    }
    return iPrecision;
}

//------------------------------------------------------------------------------
/*! @brief Calculates division lines for the given scale values for pixel
           coordinate systems only (no world coordinate transformations).

    To calculate division lines with world coordinate transformations use the
    getDivLines4LinSpacing method with the pixel range argument.

    For more information about the difference between calcuation of division lines
    with and without world transformations see documentation of class CScaleDivLines.

    @param i_fScaleMinVal [in]
        Minimum scale value in world coordinates (e.g. 0.0).
    @param i_fScaleMaxVal [in]
        Maximum scale value in world coordinates (e.g. 100.0).
    @param i_fDivLineDistMinVal [in]
        Minimum distance between two division lines in world coordinates (e.g. 10.0).
    @param i_iDivLineDistMinPix [in]
        Minimum distance between two division lines in pixels (e.g. 50).
    @param i_bUseDivLineDistValDecimalFactor25 [in]
        If false the division lines will be decimal factors at 10, 20, 50.
        If true also the decimall factor 25 is allowed.
    @param o_pfDivLineFirstVal [out]
        If != nullptr contains the world coordinate of the first division line.
    @param o_pfDivLineDistFirstPix [out]
        If != nullptr contains the pixel coordinate of the first division line.
    @param o_pfDivLineDistVal [out]
        If != nullptr contains the distance in world coordinate between two division lines.
    @param o_pfDivLineDistPix [out]
        If != nullptr contains the distance in pixels between two division lines.
    @param i_pTrcAdminObj [in]
        If != nullptr the admin object is used to trace the method call.

    @return Number of division lines for the given scale range.
*/
int CScaleDivLines::getDivLines4LinSpacing(
    double  i_fScaleMinPix,
    double  i_fScaleMaxPix,
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
    return getDivLines4LinSpacing(
        /* bUseWorldTransform    */ false,
        /* fScaleMinVal          */ i_fScaleMinPix,
        /* fScaleMaxVal          */ i_fScaleMaxPix,
        /* fScaleRangePix        */ 0.0,
        /* fDivLineDistMinVal    */ i_fDivLineDistMinVal,
        /* iDivLineDistMinPix    */ i_iDivLineDistMinPix,
        /* bUseDivLineDecFac25   */ i_bUseDivLineDistValDecimalFactor25,
        /* pfDivLineFirstVal     */ o_pfDivLineFirstVal,
        /* pfDivLineDistFirstPix */ o_pfDivLineDistFirstPix,
        /* pfDivLineDistVal      */ o_pfDivLineDistVal,
        /* pfDivLineDistPix      */ o_pfDivLineDistPix,
        /* pTrcAdminObj          */ i_pTrcAdminObj );
}

//------------------------------------------------------------------------------
/*! @brief Calculates division lines for the given scale values for coordinate
           systems where pixel values got to be converted into world coordinates
           and vice versa.

    To calculate division lines in systems with pure pixel coordinates without
    world transformations use the getDivLines4LinSpacing method without the
    pixel range argument.

    For more information about the difference between calcuation of division lines
    with and without world transformations see documentation of class CScaleDivLines.

    @param i_fScaleMinVal [in]
        Minimum scale value in world coordinates (e.g. 0.0).
    @param i_fScaleMaxVal [in]
        Maximum scale value in world coordinates (e.g. 100.0).
    @param i_fScaleRangePix [in]
        Number of pixels available for the scale (e.g. 1000.0).
    @param i_fDivLineDistMinVal [in]
        Minimum distance between two division lines in world coordinates (e.g. 10.0).
    @param i_iDivLineDistMinPix [in]
        Minimum distance between two division lines in pixels (e.g. 50).
    @param i_bUseDivLineDistValDecimalFactor25 [in]
        If false the division lines will be decimal factors at 10, 20, 50.
        If true also the decimall factor 25 is allowed.
    @param o_pfDivLineFirstVal [out]
        If != nullptr contains the world coordinate of the first division line.
    @param o_pfDivLineDistFirstPix [out]
        If != nullptr contains the pixel coordinate of the first division line.
    @param o_pfDivLineDistVal [out]
        If != nullptr contains the distance in world coordinate between two division lines.
    @param o_pfDivLineDistPix [out]
        If != nullptr contains the distance in pixels between two division lines.
    @param i_pTrcAdminObj [in]
        If != nullptr the admin object is used to trace the method call.

    @return Number of division lines for the given scale range.
*/
int CScaleDivLines::getDivLines4LinSpacing(
    double  i_fScaleMinVal,
    double  i_fScaleMaxVal,
    double  i_fScaleRangePix,
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
    return getDivLines4LinSpacing(
        /* bUseWorldTransform    */ true,
        /* fScaleMinVal          */ i_fScaleMinVal,
        /* fScaleMaxVal          */ i_fScaleMaxVal,
        /* fScaleRangePix        */ i_fScaleRangePix,
        /* fDivLineDistMinVal    */ i_fDivLineDistMinVal,
        /* iDivLineDistMinPix    */ i_iDivLineDistMinPix,
        /* bUseDivLineDecFac25   */ i_bUseDivLineDistValDecimalFactor25,
        /* pfDivLineFirstVal     */ o_pfDivLineFirstVal,
        /* pfDivLineDistFirstPix */ o_pfDivLineDistFirstPix,
        /* pfDivLineDistVal      */ o_pfDivLineDistVal,
        /* pfDivLineDistPix      */ o_pfDivLineDistPix,
        /* pTrcAdminObj          */ i_pTrcAdminObj );
}

/*==============================================================================
protected: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Calculates division lines for the given scale values for both,
           pure pixel coordinate systems and coordinate systems with world
           transfomrations.

    The method returns false if the value did not change and true
    if the value changed and the update method need to be called afterwards
    to recalculate the division lines.

    @param i_bUseWorldCoordinateTransformation [in]
        Range [false, true]
        false for pure pixel coordinate systems.
        true for coordinate systems where pixels got to be converted to world
        coordinates and vice versa.
        For more information about the difference between calcuation of division lines
        with and without world transformations see documentation of class CScaleDivLines.
    @param i_fScaleMinVal [in]
        Minimum scale value in world coordinates (e.g. 0.0).
    @param i_fScaleMaxVal [in]
        Maximum scale value in world coordinates (e.g. 100.0).
    @param i_fScaleRangePix [in]
        Number of pixels available for the scale (e.g. 1000.0).
        Ignored if i_bUseWorldCoordinateTransformation is false.
    @param i_fDivLineDistMinVal [in]
        Minimum distance between two division lines in world coordinates (e.g. 10.0).
    @param i_iDivLineDistMinPix [in]
        Minimum distance between two division lines in pixels (e.g. 50).
    @param i_bUseDivLineDistValDecimalFactor25 [in]
        If false the division lines will be decimal factors at 10, 20, 50.
        If true also the decimall factor 25 is allowed.
    @param o_pfDivLineFirstVal [out]
        If != nullptr contains the world coordinate of the first division line.
    @param o_pfDivLineDistFirstPix [out]
        If != nullptr contains the pixel coordinate of the first division line.
    @param o_pfDivLineDistVal [out]
        If != nullptr contains the distance in world coordinate between two division lines.
    @param o_pfDivLineDistPix [out]
        If != nullptr contains the distance in pixels between two division lines.
    @param i_pTrcAdminObj [in]
        If != nullptr the admin object is used to trace the method call.

    @return Number of division lines for the given scale range.
*/
int CScaleDivLines::getDivLines4LinSpacing(
    bool    i_bUseWorldCoordinateTransformation,
    double  i_fScaleMinVal,
    double  i_fScaleMaxVal,
    double  i_fScaleRangePix,
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
        strMthInArgs =
            "UseWorldCoors: " + bool2Str(i_bUseWorldCoordinateTransformation) +
            ", Scale {Min: " + QString::number(i_fScaleMinVal) +
            ", Max: " + QString::number(i_fScaleMaxVal) +
            ", Range: " + QString::number(i_fScaleRangePix) + " px" +
            ", DivLineDist {MinVal: " + QString::number(i_fDivLineDistMinVal) +
            ", MinPix: " + QString::number(i_iDivLineDistMinPix) +
            ", UseDecFactor25: " + bool2Str(i_bUseDivLineDistValDecimalFactor25) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ i_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "Math::getDivLines4LinSpacing",
        /* strAddInfo   */ strMthInArgs );

    int iDivLineCount = 0;

    if (o_pfDivLineFirstVal != nullptr) {
        *o_pfDivLineFirstVal = i_fScaleMinVal;
    }
    if (o_pfDivLineDistFirstPix != nullptr) {
        *o_pfDivLineDistFirstPix = 0.0;
    }
    if (i_bUseWorldCoordinateTransformation) {
        if (o_pfDivLineDistVal != nullptr) {
            *o_pfDivLineDistVal = i_fScaleMaxVal - i_fScaleMinVal;
        }
        if (o_pfDivLineDistPix != nullptr) {
            *o_pfDivLineDistPix = i_fScaleRangePix;
        }
    }
    else {
        if (o_pfDivLineDistVal != nullptr) {
            *o_pfDivLineDistVal = i_fScaleMaxVal - i_fScaleMinVal + 1.0;
        }
        if (o_pfDivLineDistPix != nullptr) {
            *o_pfDivLineDistPix = i_fScaleMaxVal - i_fScaleMinVal + 1.0;
        }
    }

    double fScaleRangeVal = i_fScaleMaxVal - i_fScaleMinVal;
    double fScaleRangePix = i_fScaleRangePix;
    if (!i_bUseWorldCoordinateTransformation) {
        fScaleRangeVal += 1.0;
        fScaleRangePix = fScaleRangeVal;
    }

    if (fScaleRangeVal > 0.0 && fScaleRangePix > 1.0 && i_iDivLineDistMinPix > 1)
    {
        double fScaleRangeFacPixDivVal = fScaleRangePix / fScaleRangeVal;
        if (i_bUseWorldCoordinateTransformation) {
            // Scale factor for transformation from pixel coordinates into world coordinates:
            fScaleRangeFacPixDivVal = (fScaleRangePix - 1.0) / fScaleRangeVal;
        }

        // Maximum possible count of grid lines:
        int iDivLineCountMax = static_cast<int>((fScaleRangePix-1.0)/static_cast<double>(i_iDivLineDistMinPix)) + 1;

        // On dividing the pixel range by the maximum possible count of grid lines the
        // distance between two grid lines would be:
        double fDivLineDistMinVal = i_fDivLineDistMinVal;
        if (i_fDivLineDistMinVal <= 0.0) {
            fDivLineDistMinVal = fScaleRangeVal / iDivLineCountMax;
        }

        // But DivLineDistMinVal might have a fractional part. And thats not what we want.
        // We need to set the grid lines to "human readable" values like 0.1 .. 0.2 .. 0.3 or
        // 10.0 .. 20.0 ..30.0 or 1.25 .. 1.50 .. 1.75 and so on.

        // For this we are going to calculate "DivLineDistVal" which is less or equal to the distance
        // between two grid lines if the pixel range would be divided by the maximum possible count
        // of grid lines and with the first valid digit "1" (e.g. 100.0, 1.0, 0.001, etc.).
        double fDivLineDistVal = 1.0;
        if (fDivLineDistMinVal > 1.0) {
            double fAbortVal = fDivLineDistMinVal/10.0;
            while (fDivLineDistVal <= fAbortVal) {
                fDivLineDistVal *= 10.0;
            }
        }
        else if (fDivLineDistMinVal < 1.0) {
            double fAbortVal = fDivLineDistMinVal;
            while (fDivLineDistVal >= fAbortVal) {
                fDivLineDistVal /= 10.0;
            }
        }

        // Please note that at this point of execution "DivLineDistVal" is always less or equal
        // to the distance between two grid lines if the pixel range would be divided by the
        // maximum possible count of grid lines and its first valid digit is always 1
        // (e.g. 100.0, 1.0, 0.001, etc.). So definitely "DivLineDistPix" is less or equal to
        // "DistMinPix" (the user setting for the minimum distance in pixels between two grid lines).

        // In the following loop we are going to optimize "DivLineDistVal" by increasing the value by the
        // factors 1, 2, 2.5, and 5.0 until the "best human readable" division of the axis has been reached.
        // E.g.:  0.0  1.0  2.0  3.0  4.0  5.0  6.0  7.0  8.0  9.0  10.0 ...
        // or:    0.0       2.0       4.0       6.0       8.0       10.0 ...
        // or:    0.0          2.5         5.0          7.5         10.0 ... (optional depending on i_bUseDivLineDistValDecimalFactor25)
        // or:    0.0                      5.0                      10.0 ...
        int iDivLineDistValDecimalFactor = 10;
        int iDecimals = 1;
        double fDivLineDistValDecimalFactor = 1.0;
        double fDivLineDistValBeforeCorr = fDivLineDistVal;

        // As long as the grid lines are too close to each other ...
        double fDivLineDistPix = fScaleRangeFacPixDivVal * fDivLineDistVal;
        while (fDivLineDistPix < i_iDivLineDistMinPix) {
            switch (iDivLineDistValDecimalFactor)
            {
                case 10: {
                    iDivLineDistValDecimalFactor = 20;
                    fDivLineDistValDecimalFactor = iDecimals * 2.0;
                    break;
                }
                case 20: {
                    if (i_bUseDivLineDistValDecimalFactor25) {
                        iDivLineDistValDecimalFactor = 25;
                        fDivLineDistValDecimalFactor = iDecimals * 2.5;
                    }
                    else {
                        iDivLineDistValDecimalFactor = 50;
                        fDivLineDistValDecimalFactor = iDecimals * 5.0;
                    }
                    break;
                }
                case 25: {
                    iDivLineDistValDecimalFactor = 50;
                    fDivLineDistValDecimalFactor = iDecimals * 5.0;
                    break;
                }
                case 50: {
                    iDecimals *= 10;
                    iDivLineDistValDecimalFactor = 10;
                    fDivLineDistValDecimalFactor = iDecimals;
                    break;
                }
                default: {
                    break;
                }
            }
            fDivLineDistVal = fDivLineDistValDecimalFactor * fDivLineDistValBeforeCorr;
            fDivLineDistPix = fScaleRangeFacPixDivVal * fDivLineDistVal;
        }

        // The values of the first and last grid lines must be a whole number multiple of
        // the distance between two grid lines:
        double fDivLineFirstVal = 0.0;
        modf(i_fScaleMinVal/fDivLineDistVal, &fDivLineFirstVal);
        fDivLineFirstVal *= fDivLineDistVal;
        if (fDivLineFirstVal < (i_fScaleMinVal - DBL_EPSILON)) {
            fDivLineFirstVal += fDivLineDistVal;
        }
        double fDivLineDistFirstPix = fScaleRangeFacPixDivVal * (fDivLineFirstVal - i_fScaleMinVal);

        double fDivLineLastVal = 0.0;
        modf(i_fScaleMaxVal / fDivLineDistVal, &fDivLineLastVal);
        fDivLineLastVal *= fDivLineDistVal;
        if (fDivLineLastVal > (i_fScaleMaxVal + DBL_EPSILON)) {
            fDivLineLastVal -= fDivLineDistVal;
        }
        double fDivLineDistLastPix = fScaleRangeFacPixDivVal * (i_fScaleMaxVal - fDivLineLastVal);

        // The value of the first grid line must be a whole number multiple of the distance
        // between two grid lines:
        modf(i_fScaleMinVal / fDivLineDistVal, &fDivLineFirstVal);
        fDivLineFirstVal *= fDivLineDistVal;
        if (fDivLineFirstVal < i_fScaleMinVal-DBL_EPSILON) {
            fDivLineFirstVal += fDivLineDistVal;
        }
        fDivLineDistFirstPix = fScaleRangeFacPixDivVal * (fDivLineFirstVal - i_fScaleMinVal);

        // If the value 0.0 is included ...
        if (i_fScaleMinVal < 0.0 && i_fScaleMaxVal > 0.0) {
            // ... there should be always a line at 0.0 ...
            int iDivLineCountBelowZero = static_cast<int>(-i_fScaleMinVal/fDivLineDistVal);
            double fDivLineOffsetVal = -(i_fScaleMinVal + iDivLineCountBelowZero*fDivLineDistVal);
            double fDivLineOffsetPix = fScaleRangeFacPixDivVal*fDivLineOffsetVal;
            fDivLineFirstVal = i_fScaleMinVal + fDivLineOffsetVal;
            fDivLineDistFirstPix = fDivLineOffsetPix;
        }

        // Calculate number of division lines. Algorithm could be optimized ...
        iDivLineCount = 1;
        double fDivLineVal_px = fDivLineDistFirstPix; // please note that the first digit of "DivLineFirstVal" is "1".
        while (fDivLineVal_px <= fScaleRangePix) {
            iDivLineCount++;
            fDivLineVal_px += fDivLineDistPix;

            // If DivLineDistVal is very small (but still greater than DBL_EPSILON),
            // adding fDivLineDistVal to fDivLineVal may not change the value of fDivLineVal
            // (maybe bug in floating point library?). To avoid an endless loop ...
            // And note that the pixel value at ScaleRangePix (which is ScaleMaxPix + 1)
            // does not have visible division line.
            if (fDivLineDistFirstPix + iDivLineCount*fDivLineDistPix >= fScaleRangePix) {
                break;
            }
        }
        // The pixel value at ScaleRangePix (which is ScaleMaxPix + 1) does not have visible division line.
        if (fDivLineVal_px >= fScaleRangePix) {
            iDivLineCount--;
        }

        if (o_pfDivLineFirstVal != nullptr) {
            *o_pfDivLineFirstVal = fDivLineFirstVal;
        }
        if (o_pfDivLineDistFirstPix != nullptr) {
            *o_pfDivLineDistFirstPix = fDivLineDistFirstPix;
        }
        if (o_pfDivLineDistVal != nullptr) {
            *o_pfDivLineDistVal = fDivLineDistVal;
        }
        if (o_pfDivLineDistPix != nullptr) {
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

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates an instance of the class.

    @param i_strObjName [in]
        Name of the object.
    @param i_scaleAxis [in]
        Direction of the scale which could be either X or Y.
        3 dimensional scales with scaleAxis = Z are not supported.
        Please note that the scale direction cannot be changed during runtime.
*/
CScaleDivLines::CScaleDivLines(const QString& i_strObjName, EScaleAxis i_scaleAxis) :
//------------------------------------------------------------------------------
    CScaleDivLines(ClassName(), NameSpace(), i_strObjName, i_scaleAxis)
{
}

//------------------------------------------------------------------------------
/*! @brief Creates an instance of the class.

    This constructor should be used by classes derived from this class to adjust
    the name space and class name of the trace admin object correspondingly.

    @param i_strNameSpace [in]
        Name space of the derived class.
    @param i_strClassName [in]
        Class name of the derived class.
    @param i_strObjName [in]
        Name of the object.
    @param i_scaleAxis [in]
        Direction of the scale which could be either X or Y.
        3 dimensional scales with scaleAxis = Z are not supported.
        Please note that the scale direction cannot be changed during runtime.
*/
CScaleDivLines::CScaleDivLines(
    const QString& i_strNameSpace, const QString& i_strClassName,
    const QString& i_strObjName, EScaleAxis i_scaleAxis) :
//------------------------------------------------------------------------------
    m_strNameSpace(i_strNameSpace),
    m_strClassName(i_strClassName),
    m_strObjName(i_strObjName),
    m_scaleAxis(i_scaleAxis),
    m_bUseWorldCoordinateTransformation(true),
    m_yScaleAxisOrientation(EYScaleAxisOrientation::BottomUp),
    m_spacing(ESpacing::Linear),
    m_fScaleMin(0.0),
    m_fScaleMax(0.0),
    m_fScaleRes(0.0),
    m_fMin_px(0.0),
    m_fMax_px(0.0),
    m_ariDivLinesDistMin_px(CEnumDivLineLayer::count(), 0),
    m_bDivLinesCalculated(false),
    m_ariDivLinesCount(CEnumDivLineLayer::count(), 0),
    m_arfDivLinesDistMinVal(CEnumDivLineLayer::count(), 0.0),
    m_ararfDivLinesVals(CEnumDivLineLayer::count(), QVector<double>()),
    m_arfDivLinesValsSorted(),
    m_ararfDivLines_px(CEnumDivLineLayer::count(), QVector<double>()),
    m_pTrcAdminObj(nullptr)
{
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName(), objectName());

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumScaleAxis(i_scaleAxis).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    m_ariDivLinesDistMin_px[EDivLineLayerMain] = 50;

    if (s_arfScaleRangeFacPixDivValLog.size() == 0)
    {
        s_arfScaleRangeFacPixDivValLog.resize(9);

        // In logarithmic scale each decade may be divided into 9 sections indicating
        // the values 1.0*10^x, 2.0*10^x, .... , 9.0*10^x, 10.0*10^x with x as an
        // signed integer value. The width of the sections is decreasing from beginning
        // of the first value of the decade to the last value of the decade (non
        // equidistant grid). The portion of each sector is the same for all visible decades.
        double fFacLin = 2.0;
        for (int idxSection = 0; idxSection < s_arfScaleRangeFacPixDivValLog.size(); idxSection++, fFacLin += 1.0)
        {
            s_arfScaleRangeFacPixDivValLog[idxSection] = log10(fFacLin) - log10(fFacLin-1.0);
        }
    }
} // ctor

//------------------------------------------------------------------------------
/*! @brief Assignment operator.

    @param i_other [in]
        Reference to instance whose current internal data should be taken over.
*/
CScaleDivLines::CScaleDivLines(const QString& i_strObjName, const CScaleDivLines& i_other) :
//------------------------------------------------------------------------------
    m_strNameSpace(i_other.m_strNameSpace),
    m_strClassName(i_other.m_strClassName),
    m_strObjName(i_strObjName),
    m_scaleAxis(i_other.m_scaleAxis),
    m_bUseWorldCoordinateTransformation(i_other.m_bUseWorldCoordinateTransformation),
    m_yScaleAxisOrientation(i_other.m_yScaleAxisOrientation),
    m_spacing(i_other.m_spacing),
    m_fScaleMin(i_other.m_fScaleMin),
    m_fScaleMax(i_other.m_fScaleMax),
    m_fScaleRes(i_other.m_fScaleRes),
    m_fMin_px(i_other.m_fMin_px),
    m_fMax_px(i_other.m_fMax_px),
    m_ariDivLinesDistMin_px(i_other.m_ariDivLinesDistMin_px),
    m_bDivLinesCalculated(i_other.m_bDivLinesCalculated),
    m_ariDivLinesCount(i_other.m_ariDivLinesCount),
    m_arfDivLinesDistMinVal(i_other.m_arfDivLinesDistMinVal),
    m_ararfDivLinesVals(i_other.m_ararfDivLinesVals),
    m_arfDivLinesValsSorted(m_arfDivLinesValsSorted),
    m_ararfDivLines_px(i_other.m_ararfDivLines_px),
    m_pTrcAdminObj(nullptr)
{
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "copy ctor",
        /* strAddInfo   */ i_other.NameSpace() + "::" + i_other.ClassName() + "::" + i_other.objectName() );
}

//------------------------------------------------------------------------------
/*! @brief Destructor.
*/
CScaleDivLines::~CScaleDivLines()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    //m_strNameSpace;
    //m_strClassName;
    //m_strObjName;
    m_scaleAxis = static_cast<EScaleAxis>(0);
    m_bUseWorldCoordinateTransformation = false;
    m_yScaleAxisOrientation = static_cast<EYScaleAxisOrientation>(0);
    m_spacing = static_cast<ESpacing>(0);
    m_fScaleMin = 0.0;
    m_fScaleMax = 0.0;
    m_fScaleRes = 0.0;
    m_fMin_px = 0.0;
    m_fMax_px = 0.0;
    //m_ariDivLinesDistMin_px.clear();
    m_bDivLinesCalculated = false;
    //m_ariDivLinesCount.clear();
    //m_arfDivLinesDistMinVal.clear();
    //m_ararfDivLinesVals.clear();
    //m_arfDivLinesValsSorted.clear();
    //m_ararfDivLines_px.clear();

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Assignment operator.

    @param i_other [in]
        Reference to instance whose current internal data should be taken over.
*/
CScaleDivLines& CScaleDivLines::operator = (const CScaleDivLines& i_other)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "operator =",
        /* strAddInfo   */ i_other.NameSpace() + "::" + i_other.ClassName() + "::" + objectName() );

    m_scaleAxis = i_other.m_scaleAxis;
    m_bUseWorldCoordinateTransformation = i_other.m_bUseWorldCoordinateTransformation;
    m_spacing = i_other.m_spacing;
    m_fScaleMin = i_other.m_fScaleMin;
    m_fScaleMax = i_other.m_fScaleMax;
    m_fScaleRes = i_other.m_fScaleRes;
    m_fMin_px = i_other.m_fMin_px;
    m_fMax_px = i_other.m_fMax_px;
    m_ariDivLinesDistMin_px = i_other.m_ariDivLinesDistMin_px;
    m_bDivLinesCalculated = i_other.m_bDivLinesCalculated;
    m_ariDivLinesCount = i_other.m_ariDivLinesCount;
    m_arfDivLinesDistMinVal = i_other.m_arfDivLinesDistMinVal;
    m_ararfDivLinesVals = i_other.m_ararfDivLinesVals;
    m_arfDivLinesValsSorted = i_other.m_arfDivLinesValsSorted;
    m_ararfDivLines_px = i_other.m_ararfDivLines_px;

    return *this;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the name of the instance.

    @return Name of the instance.
*/
QString CScaleDivLines::objectName() const
//------------------------------------------------------------------------------
{
    return m_strObjName;
}

/*==============================================================================
public: // instance methods (setting properties)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the orientation of the Y scale axis.

    Y scales can be either orientated from top to bottom (like in papers)
    or from bottom to top (e.g. for technical drawings or diagrams).

    The method returns false if the value did not change and true
    if the value changed and the update method need to be called afterwards
    to recalculate the division lines.

    @param i_eOrientation [in]
        Range [TopDown, BottomUp]

    @return true if the value has been changed and the division lines need to
            be recalculated, false otherwise.
*/
bool CScaleDivLines::setYScaleAxisOrientation(const CEnumYScaleAxisOrientation& i_eOrientation)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_eOrientation.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setYScaleAxisOrientation",
        /* strAddInfo   */ strMthInArgs );

    bool bChanged = false;
    if (m_yScaleAxisOrientation != i_eOrientation.enumerator()) {
        m_yScaleAxisOrientation = i_eOrientation.enumerator();
        if (m_scaleAxis == EScaleAxis::Y && m_yScaleAxisOrientation == EYScaleAxisOrientation::BottomUp) {
            if (m_fMax_px < m_fMin_px) {
                double fMinTmp_px = m_fMin_px;
                m_fMin_px = m_fMax_px;
                m_fMax_px = fMinTmp_px;
            }
        }
        bChanged = true;
        m_bDivLinesCalculated = false;
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bChanged);
    }
    return bChanged;
}

//------------------------------------------------------------------------------
/*! @brief Sets the spacing.

    The method returns false if the value did not change and true
    if the value changed and the update method need to be called afterwards
    to recalculate the division lines.

    @param i_spacing [in]
        Range [Linear, Logarithmic]

    @return true if the value has been changed and the division lines need to
            be recalculated, false otherwise.
*/
bool CScaleDivLines::setSpacing(const CEnumSpacing& i_eSpacing)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_eSpacing.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setSpacing",
        /* strAddInfo   */ strMthInArgs );

    bool bChanged = false;
    if (m_spacing != i_eSpacing.enumerator()) {
        m_spacing = i_eSpacing.enumerator();
        bChanged = true;
        m_bDivLinesCalculated = false;
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bChanged);
    }
    return bChanged;
}

//------------------------------------------------------------------------------
/*! @brief Sets whether world coordinate transformation should be used.

    For more information about the difference between calcuation of division lines
    with and without world transformations see documentation of class CScaleDivLines.

    The method returns false if the value did not change and true
    if the value changed and the update method need to be called afterwards
    to recalculate the division lines.

    @param i_bUseTransformation [in]
        Range [false, true]
        false for pure pixel coordinate systems.
        true for coordinate systems where pixels got to be converted to world
        coordinates and vice versa.

    @return true if the value has been changed and the division lines need to
            be recalculated, false otherwise.
*/
bool CScaleDivLines::setUseWorldCoordinateTransformation(bool i_bUseTransformation)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bUseTransformation);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setUseWorldCoordinateTransformation",
        /* strAddInfo   */ strMthInArgs );

    bool bChanged = false;
    if (m_bUseWorldCoordinateTransformation != i_bUseTransformation) {
        m_bUseWorldCoordinateTransformation = i_bUseTransformation;
        bChanged = true;
        m_bDivLinesCalculated = false;
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bChanged);
    }
    return bChanged;
}

//------------------------------------------------------------------------------
/*! @brief Sets the scale values.

    Before invoking this method the instance must be setup to use either pure
    pixel coordinate system or a coordinate system with world transformation.

    If the scale division line instance is setup for pure pixel coordinate system
    the pixel scale values are set.
    If the scale division line instance is setup for world transformations the
    the world scale values are set.

    For more information about the difference between calcuation of division lines
    with and without world transformations see documentation of class CScaleDivLines.

    The method returns false if none of the values changed and true if at
    least one value changed and the update method need to be called afterwards
    to recalculate the division lines.

    @param i_fMin [in]
        Minimum scale value (either in pixels or world coordinates).
    @param i_fMax [in]
        Maximum scale value (either in pixels or world coordinates)
    @param i_fRes [in]
        Resolution of the scale values in pixels.
        The resolution defines the number of leading and trailing digits to indicate
        the minimum and maximum values.
        For pixels the resolution is usually 1.0.
        - A resolution of 0.001 sets the number of leading digits to 1
          and the trailing digits to 3.
        - A resolution of 10.0 sets the number of leading digits to 2
          and the trailing digits to 1.

    @return true if the value has been changed and the division lines need to
            be recalculated, false otherwise.
*/
bool CScaleDivLines::setScale(double i_fMin, double i_fMax, double i_fRes)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Min: " + QString::number(i_fMin) +
                     ", Max: " + QString::number(i_fMax) +
                     ", Res: " + QString::number(i_fRes);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setScale",
        /* strAddInfo   */ strMthInArgs );

    bool bChanged = false;
    if (m_bUseWorldCoordinateTransformation) {
        if (m_fScaleMin != i_fMin || m_fScaleMax != i_fMax || m_fScaleRes != i_fRes) {
            m_fScaleMin = i_fMin;
            m_fScaleMax = i_fMax;
            m_fScaleRes = i_fRes;
            bChanged = true;
            m_bDivLinesCalculated = false;
        }
    }
    else {
        double fMin_px = i_fMin;
        double fMax_px = i_fMax;
        if (m_scaleAxis == EScaleAxis::Y && m_yScaleAxisOrientation == EYScaleAxisOrientation::BottomUp) {
            if (fMax_px < fMin_px) {
                double fMinTmp_px = fMin_px;
                fMin_px = fMax_px;
                fMax_px = fMinTmp_px;
            }
        }
        if (m_fMin_px != fMin_px || m_fMax_px != fMax_px || m_fScaleRes != i_fRes) {
            m_fMin_px = fMin_px;
            m_fMax_px = fMax_px;
            m_fScaleRes = i_fRes;
            bChanged = true;
            m_bDivLinesCalculated = false;
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bChanged);
    }
    return bChanged;
}

//------------------------------------------------------------------------------
/*! @brief Sets both the world coordinates (phyiscal values) of the scale and the
           pixel range of the scale.

    The method throws an exception if the instance is setup to calculate
    division lines for pixel coordinate systems only.

    For a pure pixel coordinate system the setScale method with three arguments
    must be used.

    For more information about the difference between calcuation of division lines
    with and without world transformations see documentation of class CScaleDivLines.

    The method returns false if none of the values changed and true if at
    least one value changed and the update method need to be called afterwards
    to recalculate the division lines.

    @param i_fMin [in]
        Minimum scale value ("world" coordinate).
    @param i_fMax [in]
        Maximum scale value ("world" coordinate). Must be greater than the minimum value.
    @param i_fRes [in]
        Resolution of the scale values ("world" coordinate).
        The resolution defines the number of leading and trailing digits to indicate
        the minimum and maximum values.
        - A resolution of 0.001 sets the number of leading digits to 1
          and the trailing digits to 3.
        - A resolution of 10.0 sets the number of leading digits to 2
          and the trailing digits to 1.
    @param i_fMin_px [in]
        Minimum of the scale in pixels.
    @param i_fMax_px [in]
        Maximum of the scale in pixels.

    @return true if the value has been changed and the division lines need to
            be recalculated, false otherwise.
*/
bool CScaleDivLines::setScale(
    double i_fMin, double i_fMax, double i_fRes, double i_fMin_px, double i_fMax_px)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Min: " + QString::number(i_fMin) +
                     ", Max: " + QString::number(i_fMax) +
                     ", Res: " + QString::number(i_fRes) +
                     ", Min: " + QString::number(i_fMin_px) + " px" +
                     ", Max: " + QString::number(i_fMax_px) + " px";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setScale",
        /* strAddInfo   */ strMthInArgs );

    if (!m_bUseWorldCoordinateTransformation) {
        throw CException(__FILE__, __LINE__, EResultInvalidMethodCall);
    }

    bool bChanged = false;
    double fMin_px = i_fMin_px;
    double fMax_px = i_fMax_px;
    if (m_scaleAxis == EScaleAxis::Y && m_yScaleAxisOrientation == EYScaleAxisOrientation::BottomUp) {
        if (fMax_px < fMin_px) {
            double fMinTmp_px = fMin_px;
            fMin_px = fMax_px;
            fMax_px = fMinTmp_px;
        }
    }
    if (m_fScaleMin != i_fMin || m_fScaleMax != i_fMax || m_fScaleRes != i_fRes
     || m_fMin_px != fMin_px || m_fMax_px != fMax_px)
    {
        m_fScaleMin = i_fMin;
        m_fScaleMax = i_fMax;
        m_fScaleRes = i_fRes;
        m_fMin_px = i_fMin_px;
        m_fMax_px = i_fMax_px;
        bChanged = true;
        m_bDivLinesCalculated = false;
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bChanged);
    }
    return bChanged;
}

//------------------------------------------------------------------------------
/*! @brief Sets the scale range in pixels for a coordinate system where the
           coordinate system used world transformations.

    The method throws an exception if the instance is setup to calculate
    division lines for pixel dimension units.

    For pixel dimension the setScale methods without InPix in the name must be used.

    For more information about the difference between calcuation of division lines
    with and without world transformations see documentation of class CScaleDivLines.

    The method returns false if none of the values changed and true if at
    least one value changed and the update method need to be called afterwards
    to recalculate the division lines.

    @param i_fMin_px [in]
        Minimum of the scale in pixels.
    @param i_fMax_px [in]
        Maximum of the scale in pixels.

    @return true if the value has been changed and the division lines need to
            be recalculated, false otherwise.
*/
bool CScaleDivLines::setScaleInPix(double i_fMin_px, double i_fMax_px)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Min: " + QString::number(i_fMin_px) + " px" +
                     ", Max: " + QString::number(i_fMax_px) + " px";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setScale",
        /* strAddInfo   */ strMthInArgs );

    if (!m_bUseWorldCoordinateTransformation) {
        throw CException(__FILE__, __LINE__, EResultInvalidMethodCall);
    }

    bool bChanged = false;
    double fMin_px = i_fMin_px;
    double fMax_px = i_fMax_px;
    if (m_scaleAxis == EScaleAxis::Y && m_yScaleAxisOrientation == EYScaleAxisOrientation::BottomUp) {
        if (fMax_px < fMin_px) {
            double fMinTmp_px = fMin_px;
            fMin_px = fMax_px;
            fMax_px = fMinTmp_px;
        }
    }
    if (m_fMin_px != fMin_px || m_fMax_px != fMax_px)
    {
        m_fMin_px = fMin_px;
        m_fMax_px = fMax_px;
        bChanged = true;
        m_bDivLinesCalculated = false;
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bChanged);
    }
    return bChanged;
}

//------------------------------------------------------------------------------
/*! @brief Sets the resolution of the minimum and maximu scale values
           in world coordinates (phyiscal values).

    The method returns false if the value did not change and true
    if the value changed and the update method need to be called afterwards
    to recalculate the division lines.

    @param i_fRes [in]
        Resolution of the scale values. The resolution defines the number of
        leading and trailing digits to indicate the minimum and maximum values.
        - A resolution of 0.001 sets the number of leading digits to 1
          and the trailing digits to 3.
        - A resolution of 10.0 sets the number of leading digits to 2
          and the trailing digits to 1.

    @return true if the value has been changed and the division lines need to
            be recalculated, false otherwise.
*/
bool CScaleDivLines::setScaleRes(double i_fRes)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fRes);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setScaleRes",
        /* strAddInfo   */ strMthInArgs );

    bool bChanged = false;
    if (m_fScaleRes != i_fRes) {
        m_fScaleRes = i_fRes;
        bChanged = true;
        m_bDivLinesCalculated = false;
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bChanged);
    }
    return bChanged;
}

/*==============================================================================
public: // instance methods (setting properties)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the minimum distance between two division lines in pixels.

    The final distance between two division lines is calculated based on this value
    so that the division line values become a whole number of a decimal power.

    The method returns false if the value did not change and true
    if the value changed and the update method need to be called afterwards
    to recalculate the division lines.

    @param i_eLayer [in]
        Range [Main, Sub]
        Layer for which the value should be applied.
    @param i_iDistMinPix [in]
        Minimum value in pixels.

    @return true if the value has been changed and the division lines need to
            be recalculated, false otherwise.
*/
bool CScaleDivLines::setDivLinesDistMinInPix(const CEnumDivLineLayer& i_eLayer, int i_iDist_px)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_eLayer.toString() + ", " + QString::number(i_iDist_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setDivLinesDistMinInPix",
        /* strAddInfo   */ strMthInArgs );

    bool bChanged = false;
    if (m_ariDivLinesDistMin_px[i_eLayer.enumeratorAsInt()] != i_iDist_px) {
        m_ariDivLinesDistMin_px[i_eLayer.enumeratorAsInt()] = i_iDist_px;
        bChanged = true;
        m_bDivLinesCalculated = false;
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bChanged);
    }
    return bChanged;
}

/*==============================================================================
public: // instance methods (getting properties)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Checks whether instance has valid values.

    The range in pixels must be greater than 0,
    the scales maximum value must be greater than the minimum,
    the scale resolution must be greater than 0.0,
    and for logarithmic spacing both the minimum and maximum scales values
    must be greater than 0.0.

    @return true, if the values are valid, false otherwise.
*/
bool CScaleDivLines::isValid() const
//------------------------------------------------------------------------------
{
    bool bScaleValid = true;

    if (scaleRangeInPix() <= 0) {
        bScaleValid = false;
    }
    else if (m_fScaleRes < 0.0) {
        bScaleValid = false;
    }
    else if (m_bUseWorldCoordinateTransformation) {
        if (m_fScaleMax <= m_fScaleMin) {
            bScaleValid = false;
        }
        else if (m_spacing == ESpacing::Logarithmic) {
            if (m_fScaleMin <= 0.0 || m_fScaleMax <= 0.0) {
                bScaleValid = false;
            }
        }
    }
    else if (m_spacing == ESpacing::Logarithmic) {
        bScaleValid = false;
    }
    return bScaleValid;
}

//------------------------------------------------------------------------------
/*! @brief Returns the spacing which could be either Linear or Logarithmic.

    @return Spacing.
*/
EScaleAxis CScaleDivLines::scaleAxis() const
//------------------------------------------------------------------------------
{
    return m_scaleAxis;
}

//------------------------------------------------------------------------------
/*! @brief Returns the orientation of the Y scale axis which could be either
           TopDown or ButtomUp.

    @return Spacing.
*/
EYScaleAxisOrientation CScaleDivLines::yScaleAxisOrientation() const
//------------------------------------------------------------------------------
{
    return m_yScaleAxisOrientation;
}

//------------------------------------------------------------------------------
/*! @brief Returns the spacing which could be either Linear or Logarithmic.

    @return Spacing.
*/
ESpacing CScaleDivLines::spacing() const
//------------------------------------------------------------------------------
{
    return m_spacing;
}

//------------------------------------------------------------------------------
/*! @brief Returns whether the instance has to be setup for a pure pixel coordinate
           system or a coordinate system with world coordinate transformations.

    For more information about the difference between calcuation of division lines
    with and without world transformations see documentation of class CScaleDivLines.

    @return Range [false, true]
        false for pure pixel coordinate systems.
        true for coordinate systems where pixels got to be converted to world
        coordinates and vice versa.
*/
bool CScaleDivLines::useWorldCoordinateTransformation() const
//------------------------------------------------------------------------------
{
    return m_bUseWorldCoordinateTransformation;
}

//------------------------------------------------------------------------------
/*! @brief Returns the minimum scale value (either pixel or world coordinates
           depending on the dimension unit the scale division line instance is setup).

    @return Minimum scale value.
*/
double CScaleDivLines::scaleMin() const
//------------------------------------------------------------------------------
{
    if (m_bUseWorldCoordinateTransformation) {
        return m_fScaleMin;
    }
    return scaleMinInPix();
}

//------------------------------------------------------------------------------
/*! @brief Returns the maxim scale value (either pixel or world coordinates
           depending on the dimension unit the scale division line instance is setup).

    @return Maximum scale value.
*/
double CScaleDivLines::scaleMax() const
//------------------------------------------------------------------------------
{
    if (m_bUseWorldCoordinateTransformation) {
        return m_fScaleMax;
    }
    return scaleMaxInPix();
}

//------------------------------------------------------------------------------
/*! @brief Returns the range of the scale (world coordinates).

    @return Range of scale (maxVal - minVal).
*/
double CScaleDivLines::scaleRange() const
//------------------------------------------------------------------------------
{
    if (m_bUseWorldCoordinateTransformation) {
        return fabs(m_fScaleMax - m_fScaleMin);
    }
    return scaleRangeInPix();
}

//------------------------------------------------------------------------------
/*! @brief Returns the resolution of the scale.

    The resolution defines the number of leading and trailing digits to indicate
    the minimum and maximum values.
    - A resolution of 0.001 sets the number of leading digits to 1
        and the trailing digits to 3.
    - A resolution of 10.0 sets the number of leading digits to 2
        and the trailing digits to 1.

    @return Resolution.
*/
double CScaleDivLines::scaleRes() const
//------------------------------------------------------------------------------
{
    return m_fScaleRes;
}

//------------------------------------------------------------------------------
/*! @brief Returns the scales minimum in pixels.

    @Return Minimum scale in pixels.
*/
double CScaleDivLines::scaleMinInPix() const
//------------------------------------------------------------------------------
{
    return m_fMin_px;
}

//------------------------------------------------------------------------------
/*! @brief Returns the scales maximum in pixels.

    @Return Maximum scale in pixels.
*/
double CScaleDivLines::scaleMaxInPix() const
//------------------------------------------------------------------------------
{
    return m_fMax_px;
}

//------------------------------------------------------------------------------
/*! @brief Returns the range in pixels.

    @Return Range in pixels.
*/
double CScaleDivLines::scaleRangeInPix() const
//------------------------------------------------------------------------------
{
    return abs(m_fMax_px - m_fMin_px + 1);
}

/*==============================================================================
public: // instance methods (getting properties)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the minimum distance between two division lines in pixels.

    The final distance between two division lines is calculated based on this value
    so that the division line values become a whole number of a decimal power.

    @param i_eLayer [in]
        Range [Main, Sub]
        Layer for which the value should be applied.

    @return Minimum distance between two division lines in pixels.

*/
int CScaleDivLines::divLinesDistMinInPix(const CEnumDivLineLayer& i_eLayer) const
//------------------------------------------------------------------------------
{
    return m_ariDivLinesDistMin_px[i_eLayer.enumeratorAsInt()];
}

/*==============================================================================
public: // overridables (to recalculate division lines after changing settings)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns whether the division line values have been calculated.

    @return true, if calculated, false otherwise.
*/
bool CScaleDivLines::divLinesCalculated() const
//------------------------------------------------------------------------------
{
    return m_bDivLinesCalculated;
}

//------------------------------------------------------------------------------
/*! @brief When changing settings the update method has to be called to
           recalcuate (update) the division line values.

    Sets the flag m_bDivLinesCalculated to true.

    @return true if the division lines have been newly calculated,
            false if the division lines were already up to date.
*/
bool CScaleDivLines::update()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "update",
        /* strAddInfo   */ "" );

    bool bChanged = false;

    if (!m_bDivLinesCalculated)
    {
        // Reset internal data
        //--------------------

        invalidateResults();

        // Calculate main and sub division lines
        //--------------------------------------

        double fRange_px = scaleRangeInPix();

        if (fRange_px <= 1.0 || m_ariDivLinesDistMin_px[EDivLineLayerMain] < 2) {
        }
        else if (!isValid()) {
            m_ariDivLinesCount[EDivLineLayerMain] = 2;
            m_ararfDivLinesVals[EDivLineLayerMain].resize(m_ariDivLinesCount[EDivLineLayerMain]);
            m_ararfDivLinesVals[EDivLineLayerMain][0] = m_fScaleMin;
            m_ararfDivLinesVals[EDivLineLayerMain][1] = m_fScaleMax;
            m_ararfDivLines_px[EDivLineLayerMain].resize(m_ariDivLinesCount[EDivLineLayerMain]);
            m_ararfDivLines_px[EDivLineLayerMain][0] = 0;
            m_ararfDivLines_px[EDivLineLayerMain][1] = fRange_px;
        }
        else {
            if( m_spacing == ESpacing::Linear ) {
                updateLinearSpacing();
            }
            else {
                updateLogarithmicSpacing();
            }
        }

        // Update sorted division lines.
        updateDivLinesValsSorted();

        // Minimum distance between two division lines
        //--------------------------------------------

        for (int idxLayer = 0; idxLayer < CEnumDivLineLayer::count(); ++idxLayer)
        {
            double fDivLineDistMin = scaleRange();
            for (int idxDivLine = 1; idxDivLine < m_ariDivLinesCount[idxLayer]; ++idxDivLine)
            {
                double fDivLineValPrev = m_ararfDivLinesVals[idxLayer][idxDivLine-1];
                double fDivLineValCurr = m_ararfDivLinesVals[idxLayer][idxDivLine];
                if ((fDivLineValCurr - fDivLineValPrev) < fDivLineDistMin) {
                    fDivLineDistMin = fDivLineValCurr - fDivLineValPrev;
                    if (m_spacing == ESpacing::Linear) {
                        break;
                    }
                }
            }
            m_arfDivLinesDistMinVal[idxLayer] = fDivLineDistMin;
        }

        m_bDivLinesCalculated = true;
        bChanged = true;
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bChanged);
    }
    return bChanged;
}

/*==============================================================================
public: // instance methods (returning calculated values)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the number of calculated division lines for the given layer.

    @param i_eLayer [in]
        Range [Main, Sub]
        Layer for which the value should be returned.

    @return Number of division lines.
*/
int CScaleDivLines::getDivLinesCount(const CEnumDivLineLayer& i_eLayer) const
//------------------------------------------------------------------------------
{
    return m_ariDivLinesCount[i_eLayer.enumeratorAsInt()];
}

//------------------------------------------------------------------------------
/*! @brief Returns the calculated distance between two division lines for the
           given layer.

    For linear spaced scales the distance between two successive division lines
    is the same. For logarithmic scales the minimum value is returned.

    @param i_eLayer [in]
        Range [Main, Sub, Invalid]
        Layer for which the value should be returned.
        You may pass an invalid enum value if you want to consider all layers.

    @return Number of division lines.
*/
double CScaleDivLines::getDivLinesDistMin(const CEnumDivLineLayer& i_eLayer) const
//------------------------------------------------------------------------------
{
    double fDistMin = scaleRange();
    int idxLayerMin = 0;
    int idxLayerMax = CEnumDivLineLayer::count()-1;
    if (i_eLayer.isValid()) {
        idxLayerMin = i_eLayer.enumeratorAsInt();
        idxLayerMax = i_eLayer.enumeratorAsInt();
    }
    for (int idxLayer = idxLayerMin; idxLayer <= idxLayerMax; ++idxLayer) {
        if (m_arfDivLinesDistMinVal[idxLayer] < fDistMin) {
            fDistMin = m_arfDivLinesDistMinVal[idxLayer];
        }
    }
    return fDistMin;
}

//------------------------------------------------------------------------------
/*! @brief Returns the value at the specified division line.

    @param i_eLayer [in]
        Range [Main, Sub]
        Layer for which the value should be returned.
    @param i_idxDivLine [in]
        Range [0..DivLineCount-1]
        Index of the division line the value should be returned.

    @return Value in world coordinates of the division line.
*/
double CScaleDivLines::getDivLineVal(
    const CEnumDivLineLayer& i_eLayer, int i_idxDivLine) const
//------------------------------------------------------------------------------
{
    if (i_idxDivLine < 0 || i_idxDivLine >= m_ararfDivLinesVals[i_eLayer.enumeratorAsInt()].size()) {
        throw CException(__FILE__, __LINE__, EResultIdxOutOfRange);
    }
    return m_ararfDivLinesVals[i_eLayer.enumeratorAsInt()][i_idxDivLine];
}

//------------------------------------------------------------------------------
/*! @brief Returns the value in pixels at the specified division line.

    The returned value is rounded to the nearest integer value.

    @param i_eLayer [in]
        Range [Main, Sub]
        Layer for which the value should be returned.
    @param i_idxDivLine [in]
        Range [0..DivLineCount-1]
        Index of the division line the value should be returned.

    @return Value in pixel coordinates of the division line.
*/
double CScaleDivLines::getDivLineInPix(
    const CEnumDivLineLayer& i_eLayer, int i_idxDivLine) const
//------------------------------------------------------------------------------
{
    if (i_idxDivLine < 0 || i_idxDivLine >= m_ararfDivLinesVals[i_eLayer.enumeratorAsInt()].size()) {
        throw CException(__FILE__, __LINE__, EResultIdxOutOfRange);
    }
    return Math::round2Nearest(m_ararfDivLines_px[i_eLayer.enumeratorAsInt()][i_idxDivLine], 0);
}

//------------------------------------------------------------------------------
/*! @brief Returns the distance in world coordinates (physical values)
           between the specified division lines.

    @param i_eLayer [in]
        Range [Main, Sub]
        Layer for which the value should be returned.
    @param i_idxDivLine1 [in]
        Range [0..DivLineCount-1]
        Index of the first division line to be used.
    @param i_idxDivLine2 [in]
        Range [0..DivLineCount-1]
        Index of the second division line to be used.

    @return Distance in world coordinates between the two division lines.
*/
double CScaleDivLines::getDivLineDistVal(
    const CEnumDivLineLayer& i_eLayer, int i_idxDivLine1, int i_idxDivLine2) const
//------------------------------------------------------------------------------
{
    if (i_idxDivLine1 < 0 && i_idxDivLine1 >= m_ararfDivLinesVals[i_eLayer.enumeratorAsInt()].size()
     && i_idxDivLine2 < 0 && i_idxDivLine2 >= m_ararfDivLinesVals[i_eLayer.enumeratorAsInt()].size()) {
        throw CException(__FILE__, __LINE__, EResultIdxOutOfRange);
    }
    return m_ararfDivLinesVals[i_eLayer.enumeratorAsInt()][i_idxDivLine2]
         - m_ararfDivLinesVals[i_eLayer.enumeratorAsInt()][i_idxDivLine1];
}

//------------------------------------------------------------------------------
/*! @brief Returns the distance in pixels between the specified division lines.

    The returned value is rounded to the nearest integer value.

    @param i_eLayer [in]
        Range [Main, Sub]
        Layer for which the value should be returned.
    @param i_idxDivLine1 [in]
        Range [0..DivLineCount-1]
        Index of the first division line to be used.
    @param i_idxDivLine2 [in]
        Range [0..DivLineCount-1]
        Index of the second division line to be used.

    @return Distance in pixel coordinates between the two division lines.
*/
double CScaleDivLines::getDivLineDistInPix(
    const CEnumDivLineLayer& i_eLayer, int i_idxDivLine1, int i_idxDivLine2) const
//------------------------------------------------------------------------------
{
    if (i_idxDivLine1 < 0 && i_idxDivLine1 >= m_ararfDivLines_px[i_eLayer.enumeratorAsInt()].size()
     && i_idxDivLine2 < 0 && i_idxDivLine2 >= m_ararfDivLines_px[i_eLayer.enumeratorAsInt()].size()) {
        throw CException(__FILE__, __LINE__, EResultIdxOutOfRange);
    }
    double fVal = m_ararfDivLines_px[i_eLayer.enumeratorAsInt()][i_idxDivLine2]
                - m_ararfDivLines_px[i_eLayer.enumeratorAsInt()][i_idxDivLine1];
    return Math::round2Nearest(fVal, 0);
}

/*==============================================================================
public: // instance methods (converting values)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the value in pixels for the given value.

    @param i_fVal [in]
        Value in world coordinates (physical value)

    @return Value in pixel coordinates of the given value.
*/
double CScaleDivLines::getValInPix(double i_fVal) const
//------------------------------------------------------------------------------
{
    double fPix = i_fVal;

    if (isValid())
    {
        // Pure pixel coordinate system: Just return the value as is.
        // Conversion needed if scaling usess world transformation.
        if (m_bUseWorldCoordinateTransformation)
        {
            // At the minimum scale value ..
            if (i_fVal == m_fScaleMin) {
                if (m_scaleAxis == EScaleAxis::Y && (m_yScaleAxisOrientation == EYScaleAxisOrientation::BottomUp)) {
                    fPix = m_fMax_px;
                }
                else {
                    fPix = m_fMin_px;
                }
            }
            // At the maximum scale value ..
            else if (i_fVal == m_fScaleMax) {
                if (m_scaleAxis == EScaleAxis::Y && (m_yScaleAxisOrientation == EYScaleAxisOrientation::BottomUp)) {
                    fPix = m_fMin_px;
                }
                else {
                    fPix = m_fMax_px;
                }
            }
            // Somewhere between minimum and maximum scale ..
            else {
                bool bDivLineHit = false;

                // Get the nearest division line.
                for (int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++)
                {
                    if (m_ariDivLinesCount[iLayer] > 1)
                    {
                        for (int idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]-1; idxDivLine++)
                        {
                            double fDivLineVal1 = m_ararfDivLinesVals[iLayer][idxDivLine];
                            double fDivLineVal2 = m_ararfDivLinesVals[iLayer][idxDivLine+1];

                            if (i_fVal >= (fDivLineVal1-DBL_EPSILON) && i_fVal <= (fDivLineVal1+DBL_EPSILON)) {
                                fPix = m_ararfDivLines_px[iLayer][idxDivLine];
                                bDivLineHit = true;
                                break;
                            }
                            else if (i_fVal >= (fDivLineVal2-DBL_EPSILON) && i_fVal <= (fDivLineVal2+DBL_EPSILON)) {
                                fPix = m_ararfDivLines_px[iLayer][idxDivLine+1];
                                bDivLineHit = true;
                                break;
                            }
                            else if (i_fVal > fDivLineVal1 && i_fVal < fDivLineVal2) {
                                break;
                            }
                        }
                    }
                    else if (m_ariDivLinesCount[iLayer] == 1) {
                        double fDivLineVal1 = m_ararfDivLinesVals[iLayer][0];
                        if (i_fVal == fDivLineVal1) {
                            fPix = m_ararfDivLines_px[iLayer][0];
                            bDivLineHit = true;
                        }
                    }
                    if (bDivLineHit) {
                        break;
                    }
                } // for( iLayer < CEnumDivLineLayer::count() )

                // Did not hit a division line ..
                if (!bDivLineHit) {
                    double fScaleValMin = m_fScaleMin;
                    double fScaleValMax = m_fScaleMax;
                    if (m_spacing == ESpacing::Logarithmic) {
                        fScaleValMin = log10(fScaleValMin);
                        fScaleValMax = log10(fScaleValMax);
                    }

                    // Transformation from world coordinates into pixel coordinates:
                    // Val_px = Min_px + ((Range_px - 1 px) / ScaleRange_mm) * (Val_mm - ScaleMin_mm)
                    double fScaleRangeVal = fabs(fScaleValMax - fScaleValMin);
                    double fScaleRangePix = scaleRangeInPix();
                    double fPixRes = (fScaleRangePix - 1.0) / fScaleRangeVal;

                    double fVal = i_fVal;
                    if (m_spacing == ESpacing::Logarithmic) {
                        fVal = log10(fVal);
                    }
                    // Calculate value as distance to left grid line:
                    if (m_scaleAxis == EScaleAxis::X) {
                        fPix = m_fMin_px + fPixRes * (fVal - fScaleValMin);
                    }
                    else /*if (m_scaleAxis == EScaleAxis::Y)*/ {
                        if (m_yScaleAxisOrientation == EYScaleAxisOrientation::TopDown) {
                            // Orientation from top to bottom like in pixel drawing:
                            // The origin is at the top left corner.
                            // XScaleMin = XMin_px, XScaleMax = XMax_px
                            // YScaleMin = XMin_px, YScaleMax = XMax_px
                            // The greater the value, the greater the pixel coordinate on the screen.
                            fPix = m_fMin_px + fPixRes * (fVal - fScaleValMin);
                        }
                        else /*if (m_yScaleAxisOrientation == EYScaleAxisOrientation::BottomUp)*/ {
                            // Orientation from bottom to top like in diagrams or in technical drawings
                            // based on metric units:
                            // The origin is at the bottom left corner.
                            // XScaleMin = XMin_px, XScaleMax = XMax_px
                            // YScaleMin = XMax_px, YScaleMax = XMin_px
                            // The greater the value, the less the pixel coordinate on the screen.
                            fPix = m_fMax_px - fPixRes * (fVal - fScaleValMin);
                        }
                    }
                }
            } // Somewhere between minimum and maximum scale ..
        } // if (m_bUseWorldCoordinateTransformation)
    } // isValid()

    return fPix;

} // getValInPix

//------------------------------------------------------------------------------
/*! @brief Returns the value in world coordinates (physical value) for the
           given position in pixels.

    @param i_fPix [in]
        Value in pixel coordinates.

    @return Value in world coordinates.
*/
double CScaleDivLines::getVal(double i_fPix) const
//------------------------------------------------------------------------------
{
    double fVal = i_fPix;

    if (isValid())
    {
        // Pure pixel coordinate system: Just return the value as is.
        // Conversion needed if scaling usess world transformation.
        if (m_bUseWorldCoordinateTransformation)
        {
            // At the minimum scale value ..
            if (i_fPix == m_fMin_px) {
                if (m_scaleAxis == EScaleAxis::Y && (m_yScaleAxisOrientation == EYScaleAxisOrientation::BottomUp)) {
                    fVal = m_fScaleMax;
                }
                else {
                    fVal = m_fScaleMin;
                }
            }
            // At the maximum scale value ..
            else if (i_fPix == m_fMax_px) {
                if (m_scaleAxis == EScaleAxis::Y && (m_yScaleAxisOrientation == EYScaleAxisOrientation::BottomUp)) {
                    fVal = m_fScaleMin;
                }
                else {
                    fVal = m_fScaleMax;
                }
            }
            // Somewhere between minimum and maximum scale ..
            else {
                bool bDivLineHit = false;

                // Get the nearest division line. This is necessary to exactly return the value
                // at a grid line if the mouse cursor is positioned on a grid line.
                for (int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++) {
                    if (m_ariDivLinesCount[iLayer] > 1) {
                        for (int idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]-1; idxDivLine++) {
                            double fDivLinePix1 = m_ararfDivLines_px[iLayer][idxDivLine];
                            double fDivLinePix2 = m_ararfDivLines_px[iLayer][idxDivLine+1];

                            if (i_fPix >= (fDivLinePix1-DBL_EPSILON) && i_fPix <= (fDivLinePix1+DBL_EPSILON)) {
                                fVal = m_ararfDivLinesVals[iLayer][idxDivLine];
                                bDivLineHit = true;
                                break;
                            }
                            else if (i_fPix >= (fDivLinePix2-DBL_EPSILON) && i_fPix <= (fDivLinePix2+DBL_EPSILON)) {
                                fVal = m_ararfDivLinesVals[iLayer][idxDivLine+1];
                                bDivLineHit = true;
                                break;
                            }
                            else if (i_fPix > fDivLinePix1 && i_fPix < fDivLinePix2) {
                                break;
                            }
                        }
                    }
                    else if (m_ariDivLinesCount[iLayer] == 1) {
                        double fDivLinePix1 = m_ararfDivLines_px[iLayer][0];
                        if (i_fPix == fDivLinePix1) {
                            fVal = m_ararfDivLinesVals[iLayer][0];
                            bDivLineHit = true;
                        }
                    }
                    if (bDivLineHit) {
                        break;
                    }
                } // for( iLayer < CEnumDivLineLayer::count() )

                // Did not hit a division line ..
                if (!bDivLineHit) {
                    double fScaleValMin = m_fScaleMin;
                    double fScaleValMax = m_fScaleMax;
                    if (m_spacing == ESpacing::Logarithmic) {
                        fScaleValMin = log10(fScaleValMin);
                        fScaleValMax = log10(fScaleValMax);
                    }

                    // Transformation from pixel coordinates into world coordinates:
                    // Val_mm = Min_mm + (ScaleRange_mm / (Range_px - 1 px)) * (Val_px - Min_px)
                    double fScaleValRange = fabs(fScaleValMax-fScaleValMin);
                    double fScaleRangePix = scaleRangeInPix();
                    double fValRes = (fScaleValRange / (fScaleRangePix - 1.0));
                    double fPixRes = (fScaleRangePix - 1.0) / fScaleValRange;

                    // Calculate value as distance to left grid line:
                    if (m_scaleAxis == EScaleAxis::X) {
                        fVal = fScaleValMin + fValRes * (i_fPix - m_fMin_px);
                    }
                    else /*if (m_scaleAxis == EScaleAxis::Y)*/ {
                        if (m_yScaleAxisOrientation == EYScaleAxisOrientation::TopDown) {
                            // Orientation from top to bottom like in pixel drawing:
                            // The origin is at the top left corner.
                            // XScaleMin = XMin_px, XScaleMax = XMax_px
                            // YScaleMin = XMin_px, YScaleMax = XMax_px
                            // The greater the value, the greater the pixel coordinate on the screen.
                            fVal = fScaleValMin + fValRes * (m_fMin_px + i_fPix);
                        }
                        else /*if (m_yScaleAxisOrientation == EYScaleAxisOrientation::BottomUp)*/ {
                            // Orientation from bottom to top like in diagrams or in technical drawings
                            // based on metric units:
                            // The origin is at the bottom left corner.
                            // XScaleMin = XMin_px, XScaleMax = XMax_px
                            // YScaleMin = XMax_px, YScaleMax = XMin_px
                            // The greater the value, the less the pixel coordinate on the screen.
                            fVal = fScaleValMin + fValRes * (m_fMax_px - i_fPix);
                        }
                    }
                    if (m_spacing == ESpacing::Logarithmic) {
                        fVal = pow(10.0, fVal);
                    }
                } // Neither hit a main nor a sub division line ..
            } // Somewhere between minimum and maximum scale ..

            fVal = Math::round2Resolution(fVal, m_fScaleRes);

        } // if (m_bUseWorldCoordinateTransformation)
    } // if( isValid() )

    return fVal;

} // getVal

/*==============================================================================
protected: // overridable auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Invalidates the result member variables.

    Called before calculating the results in update method.
*/
void CScaleDivLines::invalidateResults()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "invalidateResults",
        /* strAddInfo   */ "" );

    for (int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++) {
        m_ariDivLinesCount[iLayer] = 0;
        m_arfDivLinesDistMinVal[iLayer] = 0.0;
        m_ararfDivLinesVals[iLayer].clear();
        m_arfDivLinesValsSorted.clear();
        m_ararfDivLines_px[iLayer].clear();
    }
}

/*==============================================================================
protected: // auxiliary instance methods (to recalculate division lines after changing settings)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Internal method called by the update method to calculate
           the division lines if linear spacing is set.
*/
void CScaleDivLines::updateLinearSpacing()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateLinearSpacing",
        /* strAddInfo   */ "" );

    m_arfDivLinesDistMinVal[EDivLineLayerMain] = scaleRange();

    double fDivLineDistValMin = m_fMin_px;
    double fDivLineDistValMax = m_fMax_px;
    if (m_bUseWorldCoordinateTransformation) {
        fDivLineDistValMin = m_fScaleMin;
        fDivLineDistValMax = m_fScaleMax;
    }

    QVector<int> ariDivLineCount(CEnumDivLineLayer::count(), 0);
    QVector<int> ariDivLineCountTmp(CEnumDivLineLayer::count(), 0);
    QVector<double> arfDivLineFirstVal(CEnumDivLineLayer::count(), 0.0);
    QVector<double> arfDivLineDistFirstPix(CEnumDivLineLayer::count(), 0.0);
    QVector<double> arfDivLineDistVal(CEnumDivLineLayer::count(), 0.0);
    QVector<double> arfDivLineDistPix(CEnumDivLineLayer::count(), 0.0);

    double fRange_px = 0.0;
    if (m_bUseWorldCoordinateTransformation) {
        fRange_px = scaleRangeInPix();
    }

    for (int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++)
    {
        if (m_ariDivLinesDistMin_px[iLayer] > 0)
        {
            // Calculate optimized distance between two grid lines:
            ariDivLineCountTmp[iLayer] = getDivLines4LinSpacing(
                /* bUseWorldTransform    */ m_bUseWorldCoordinateTransformation,
                /* fScaleMinVal          */ fDivLineDistValMin,
                /* fScaleMaxVal          */ fDivLineDistValMax,
                /* fScaleRangePix        */ fRange_px,
                /* fDivLineDistMinVal    */ 0.0,
                /* iDivLineDistMinPix    */ m_ariDivLinesDistMin_px[iLayer],
                /* bUseDivLineDecFac25   */ true,
                /* pfDivLineFirstVal     */ &arfDivLineFirstVal[iLayer],
                /* pfDivLineDistFirstPix */ &arfDivLineDistFirstPix[iLayer],
                /* pfDivLineDistVal      */ &arfDivLineDistVal[iLayer],
                /* pfDivLineDistPix      */ &arfDivLineDistPix[iLayer],
                /* pTrcAdminObj          */ m_pTrcAdminObj);

            if (m_arfDivLinesDistMinVal[iLayer] > arfDivLineDistVal[iLayer]) {
                m_arfDivLinesDistMinVal[iLayer] = arfDivLineDistVal[iLayer];
            }

            // Store the calculated results:
            if (ariDivLineCountTmp[iLayer] > 0) {
                if (iLayer == EDivLineLayerMain) {
                    // Count visible division lines for the main grid lines:
                    ariDivLineCount[iLayer] = ariDivLineCountTmp[iLayer];

                    m_ararfDivLinesVals[iLayer].resize(ariDivLineCount[iLayer]);
                    m_ararfDivLines_px[iLayer].resize(ariDivLineCount[iLayer]);

                    // Calculate pixel values and store the calculated results of the main grid lines:
                    for (int idxDivLine = 0; idxDivLine < ariDivLineCount[iLayer]; idxDivLine++) {
                        double fVal = arfDivLineFirstVal[iLayer] + idxDivLine*arfDivLineDistVal[iLayer];
                        if (fabs(fVal) < DBL_EPSILON) {
                            fVal = 0.0;
                        }
                        m_ararfDivLinesVals[iLayer][idxDivLine] = fVal;
                        m_ararfDivLines_px[iLayer][idxDivLine] = getValInPix(fVal);
                    }
                }
                else /*if (iLayer == EDivLineLayerSub)*/ {
                    // Count visible sub division lines:
                    double fPrevLayerDivLineVal = 0.0;
                    modf(m_fScaleMin/arfDivLineDistVal[iLayer-1], &fPrevLayerDivLineVal);
                    fPrevLayerDivLineVal *= arfDivLineDistVal[iLayer-1];
                    double fCurrLayerDivLineVal = fPrevLayerDivLineVal;
                    while (fCurrLayerDivLineVal <= m_fScaleMax) {
                        for (int idxDivLineTmp = 1; idxDivLineTmp < ariDivLineCountTmp[iLayer]-1; idxDivLineTmp++) {
                            fCurrLayerDivLineVal = fPrevLayerDivLineVal + idxDivLineTmp*arfDivLineDistVal[iLayer];
                            if (fabs(fCurrLayerDivLineVal) < DBL_EPSILON) {
                                fCurrLayerDivLineVal = 0.0;
                            }
                            if (fCurrLayerDivLineVal >= m_fScaleMin && fCurrLayerDivLineVal <= m_fScaleMax) {
                                ariDivLineCount[iLayer]++;
                            }
                        }
                        fPrevLayerDivLineVal += arfDivLineDistVal[iLayer-1];
                    }

                    // Calculate pixel values and store the calculated results of the sub grid lines:
                    if (ariDivLineCount[iLayer] > 0) {
                        m_ararfDivLinesVals[iLayer].resize(ariDivLineCount[iLayer]);
                        m_ararfDivLines_px[iLayer].resize(ariDivLineCount[iLayer]);

                        modf(m_fScaleMin/arfDivLineDistVal[iLayer-1], &fPrevLayerDivLineVal);
                        fPrevLayerDivLineVal *= arfDivLineDistVal[iLayer-1];
                        fCurrLayerDivLineVal = fPrevLayerDivLineVal;
                        int idxDivLine = 0;
                        while (fCurrLayerDivLineVal <= m_fScaleMax) {
                            for (int idxDivLineTmp = 1; idxDivLineTmp < ariDivLineCountTmp[iLayer]-1; idxDivLineTmp++) {
                                fCurrLayerDivLineVal = fPrevLayerDivLineVal + idxDivLineTmp*arfDivLineDistVal[iLayer];
                                if (fabs(fCurrLayerDivLineVal) < DBL_EPSILON) {
                                    fCurrLayerDivLineVal = 0.0;
                                }
                                if (fCurrLayerDivLineVal >= m_fScaleMin && fCurrLayerDivLineVal <= m_fScaleMax) {
                                    if (idxDivLine < ariDivLineCount[iLayer]) {
                                        m_ararfDivLinesVals[iLayer][idxDivLine] = fCurrLayerDivLineVal;
                                        m_ararfDivLines_px[iLayer][idxDivLine] = getValInPix(fCurrLayerDivLineVal);
                                        idxDivLine++;
                                    }
                                }
                            }
                            fPrevLayerDivLineVal += arfDivLineDistVal[iLayer-1];
                        }
                    }
                } // if( iLayer != 0 )
            } // if( ariDivLineCount[iLayer] > 0 )

            // If there is a next layer ...
            if (iLayer < CEnumDivLineLayer::count()-1) {
                if (m_ariDivLinesDistMin_px[iLayer+1] > 1 && m_ariDivLinesDistMin_px[iLayer] > 2*m_ariDivLinesDistMin_px[iLayer+1]) {
                    fDivLineDistValMin = 0.0;
                    fDivLineDistValMax = arfDivLineDistVal[iLayer];
                }
                else {
                    // Next layer is empty ...
                    break;
                }
            }
        } // if (m_ariDivLinesDistMin_px[iLayer] > 0)
    } // for( iLayer < CEnumDivLineLayer::count() )

    for (int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++) {
        m_ariDivLinesCount[iLayer] = ariDivLineCount[iLayer];
    }
} // updateLinearSpacing

//------------------------------------------------------------------------------
/*! @brief Internal method called by the update method to calculate
           the division lines if logarithmic spacing is set.
*/
void CScaleDivLines::updateLogarithmicSpacing()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateLogarithmicSpacing",
        /* strAddInfo   */ "" );

    m_arfDivLinesDistMinVal[EDivLineLayerMain] = scaleRange();

    // Sub grid (non equidistant) within main grid (equidistant) NOT including "left" and
    // "right" main grid lines. Please note that in a logarithmic scale one decade comprises
    // 8 sub grid lines, e.g. 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 (the values 1.0 and
    // 10.0 belong to the main grid):

    QVector<int> ariDivLineCount(CEnumDivLineLayer::count(), 0);
    QVector<int> ariDivLineCountMax(CEnumDivLineLayer::count(), 0);
    QVector<int> ariDivLineDistMinPix(CEnumDivLineLayer::count(), 0);
    QVector<double> arfDivLineFirstValLog(CEnumDivLineLayer::count(), 0.0);
    QVector<double> arfDivLineDistFirstPix(CEnumDivLineLayer::count(), 0.0);
    QVector<double> arfDivLineDistValLog(CEnumDivLineLayer::count(), 0.0);
    QVector<double> arfDivLineDistPix(CEnumDivLineLayer::count(), 0.0);
    QVector<double> arfDivLineValLin(CEnumDivLineLayer::count(), 0.0);
    QVector<double> arfDivLineValLog(CEnumDivLineLayer::count(), 0.0);
    QVector<int> ariDivLineCountTmp(CEnumDivLineLayer::count(), 0);

    double fScaleMinValLog = log10(m_fScaleMin);
    double fScaleMaxValLog = log10(m_fScaleMax);
    double fScaleRangeValLog = fScaleMaxValLog - fScaleMinValLog;

    // Normalize the axis range to engineering format (e.g. 4.234e9):
    int iScaleMinValLinNormExp = static_cast<int>(fScaleMinValLog);
    double fScaleMinValLinNorm = m_fScaleMin/pow(10.0,iScaleMinValLinNormExp);
    while( fScaleMinValLinNorm < 1.0 )
    {
        iScaleMinValLinNormExp--;
        fScaleMinValLinNorm *= 10.0;
    }
    while( fScaleMinValLinNorm >= 10.0 )
    {
        iScaleMinValLinNormExp++;
        fScaleMinValLinNorm /= 10.0;
    }
    double fScaleMinValLinNormLeading = 0.0;
    double fScaleMinValLinNormFract = modf(fScaleMinValLinNorm, &fScaleMinValLinNormLeading);

    int iScaleMaxValLinNormExp = static_cast<int>(fScaleMaxValLog);
    double fScaleMaxValLinNorm = m_fScaleMax/pow(10.0,iScaleMaxValLinNormExp);
    while( fScaleMaxValLinNorm < 1.0 )
    {
        iScaleMaxValLinNormExp--;
        fScaleMaxValLinNorm *= 10.0;
    }
    while( fScaleMaxValLinNorm >= 10.0 )
    {
        iScaleMaxValLinNormExp++;
        fScaleMaxValLinNorm /= 10.0;
    }
    double fScaleMaxValLinNormLeading = 0.0;
    double fScaleMaxValLinNormFract = modf(fScaleMaxValLinNorm, &fScaleMaxValLinNormLeading);

    // Extend the axis range by rounding down the scale minimum value and rounding up the
    // scale maximum value so that the following applies:
    // - log(ScaleMinVal) MOD 1 = 0
    // - log(ScaleMaxVal) MOD 1 = 0

    // Round down minimum scale value:
    double fScaleMinValLogExt = iScaleMinValLinNormExp;
    double fScaleMinValLinExt = pow(10.0, fScaleMinValLogExt);

    // Round up maximum scale value:
    double fScaleMaxValLogExt = iScaleMaxValLinNormExp;
    if( fScaleMaxValLinNorm > 1.0 || fScaleMaxValLinNormFract > 0.0 || fScaleMaxValLogExt == fScaleMinValLogExt ) //lint !e777
    {
        fScaleMaxValLogExt += 1.0;
    }

    // Get extended range:
    //fScaleMinPixExt = m_iMinVal_px - (fScaleMinValLog-fScaleMinValLogExt) * (iScaleRangePix/fScaleRangeValLog);

    // Calculate "main" division lines.
    //---------------------------------

    // For the values of the main division lines the following constraint must apply:
    // - log(MainDivLineVal) MOD 1 = 0
    // The distances between the main division lines are the same (equidistant grid).

    // Calculate maximum number of visible main grid lines:
    ariDivLineCountMax[EDivLineLayerMain] = static_cast<unsigned int>(iScaleMaxValLinNormExp-iScaleMinValLinNormExp)+1;
    if( ariDivLineCountMax[EDivLineLayerMain] > 0 && (fScaleMinValLinNorm > 1.0 || fScaleMinValLinNormFract >= DBL_EPSILON) )
    {
        ariDivLineCountMax[EDivLineLayerMain]--;
    }

    double fRange_px = scaleRangeInPix();
    if (m_bUseWorldCoordinateTransformation) {
        fRange_px -= 1;
    }

    // If at least one main grid line is visible ...
    if (ariDivLineCountMax[EDivLineLayerMain] > 0)
    {
        // Prepare logarithmic scale values for linear spacing:
        // First assume that the value distance between the main grid lines is 1.0
        // (the lowest possible value for main grid lines).
        ariDivLineDistMinPix[EDivLineLayerMain] = static_cast<int>(fRange_px / fScaleRangeValLog);
        // If the distance of 1.0 between the main grid lines is too small ..
        if (ariDivLineDistMinPix[EDivLineLayerMain] < m_ariDivLinesDistMin_px[EDivLineLayerMain]) {
            ariDivLineDistMinPix[EDivLineLayerMain] = m_ariDivLinesDistMin_px[EDivLineLayerMain];
        }
        // If the distance of 1.0 between the main grid lines is too big ..
        else if (ariDivLineDistMinPix[EDivLineLayerMain] > fRange_px / 2.0) {
            ariDivLineDistMinPix[EDivLineLayerMain] = fRange_px / 2.0;
        }

        // Calculate optimized distance between two main grid lines:
        ariDivLineCount[EDivLineLayerMain] = getDivLines4LinSpacing(
            /* bUseWorldTransform    */ m_bUseWorldCoordinateTransformation,
            /* fScaleMinVal          */ fScaleMinValLog,
            /* fScaleMaxVal          */ fScaleMaxValLog,
            /* iScaleRangePix        */ fRange_px,
            /* fDivLineDistMinVal    */ 1.0,
            /* iDivLineDistMinPix    */ ariDivLineDistMinPix[EDivLineLayerMain],
            /* bUseDivLineDecFac25   */ false,
            /* pfDivLineFirstVal     */ &arfDivLineFirstValLog[EDivLineLayerMain],
            /* pfDivLineDistFirstPix */ &arfDivLineDistFirstPix[EDivLineLayerMain],
            /* pfDivLineDistVal      */ &arfDivLineDistValLog[EDivLineLayerMain],
            /* pfDivLineDistPix      */ &arfDivLineDistPix[EDivLineLayerMain],
            /* pTrcAdminObj          */ m_pTrcAdminObj);
    }

    // Store the calculated results:
    if (ariDivLineCount[EDivLineLayerMain] > 0) {
        m_ararfDivLinesVals[EDivLineLayerMain].resize(ariDivLineCount[EDivLineLayerMain]);
        m_ararfDivLines_px[EDivLineLayerMain].resize(ariDivLineCount[EDivLineLayerMain]);

        for (int idxDivLine = 0; idxDivLine < ariDivLineCount[EDivLineLayerMain]; idxDivLine++) {
            arfDivLineValLog[EDivLineLayerMain] = arfDivLineFirstValLog[EDivLineLayerMain] + idxDivLine*arfDivLineDistValLog[EDivLineLayerMain];
            arfDivLineValLin[EDivLineLayerMain] = pow(10.0,arfDivLineValLog[EDivLineLayerMain]);
            m_ararfDivLinesVals[EDivLineLayerMain][idxDivLine] = arfDivLineValLin[EDivLineLayerMain];
            m_ararfDivLines_px[EDivLineLayerMain][idxDivLine] = getValInPix(arfDivLineValLin[EDivLineLayerMain]);
        }
    }

    // Calculate "sub" division lines.
    //---------------------------------

    // If not each main division line is visible (e.g. if the scale range enclosed
    // a huge range (many decades) and not each main grid line may be drawn because
    // the distance between two main grid lines would be below the minimum distance
    // between the main grid lines) ..
    if (ariDivLineCount[EDivLineLayerMain] < ariDivLineCountMax[EDivLineLayerMain])
    {
        // .. some (or all) of the not yet visible main division lines
        // become the sub division lines (equidistant grid).

        // Calculate optimized distance between two sub grid lines:
        if (m_ariDivLinesDistMin_px[EDivLineLayerSub] > 1 && m_ariDivLinesDistMin_px[EDivLineLayerMain] > 2*m_ariDivLinesDistMin_px[EDivLineLayerSub])
        {
            ariDivLineCountTmp[EDivLineLayerSub] = getDivLines4LinSpacing(
                /* bUseWorldTransform    */ m_bUseWorldCoordinateTransformation,
                /* fScaleMinVal          */ arfDivLineFirstValLog[EDivLineLayerMain],
                /* fScaleMaxVal          */ arfDivLineFirstValLog[EDivLineLayerMain]+arfDivLineDistValLog[EDivLineLayerMain],
                /* fScaleRangePix        */ arfDivLineDistPix[EDivLineLayerMain],
                /* fDivLineDistMinVal    */ 1.0,
                /* iDivLineDistMinPix    */ m_ariDivLinesDistMin_px[EDivLineLayerSub],
                /* bUseDivLineDecFac25   */ false,
                /* pfDivLineFirstVal     */ &arfDivLineFirstValLog[EDivLineLayerSub],
                /* pfDivLineDistFirstPix */ &arfDivLineDistFirstPix[EDivLineLayerSub],
                /* pfDivLineDistVal      */ &arfDivLineDistValLog[EDivLineLayerSub],
                /* pfDivLineDistPix      */ nullptr,
                /* pTrcAdminObj          */ m_pTrcAdminObj);

            // Store the calculated results:
            if (ariDivLineCountTmp[EDivLineLayerSub] > 2)
            {
                // Count visible sub division lines:
                arfDivLineValLog[EDivLineLayerMain] = arfDivLineFirstValLog[EDivLineLayerMain];
                if (arfDivLineValLog[EDivLineLayerMain] > fScaleMinValLog) {
                    arfDivLineValLog[EDivLineLayerMain] -= arfDivLineDistValLog[EDivLineLayerMain];
                }
                arfDivLineValLog[EDivLineLayerSub] = arfDivLineValLog[EDivLineLayerMain];
                arfDivLineValLin[EDivLineLayerSub] = pow(10.0,arfDivLineValLog[EDivLineLayerSub]);
                while (arfDivLineValLin[EDivLineLayerSub] <= m_fScaleMax) {
                    for (int idxDivLineTmp = 1; idxDivLineTmp < ariDivLineCountTmp[EDivLineLayerSub]-1; idxDivLineTmp++) {
                        arfDivLineValLog[EDivLineLayerSub] = arfDivLineValLog[EDivLineLayerMain] + idxDivLineTmp*arfDivLineDistValLog[EDivLineLayerSub];
                        arfDivLineValLin[EDivLineLayerSub] = pow(10.0,arfDivLineValLog[EDivLineLayerSub]);
                        if (arfDivLineValLin[EDivLineLayerSub] >= m_fScaleMin && arfDivLineValLin[EDivLineLayerSub] <= m_fScaleMax) {
                            ariDivLineCount[EDivLineLayerSub]++;
                        }
                    }
                    arfDivLineValLog[EDivLineLayerMain] += arfDivLineDistValLog[EDivLineLayerMain];
                }

                // Calculate pixel values and store the calculated results of the sub grid lines:
                if (ariDivLineCount[EDivLineLayerSub] > 0)
                {
                    m_ararfDivLinesVals[EDivLineLayerSub].resize(ariDivLineCount[EDivLineLayerSub]);
                    m_ararfDivLines_px[EDivLineLayerSub].resize(ariDivLineCount[EDivLineLayerSub]);

                    arfDivLineValLog[EDivLineLayerMain] = arfDivLineFirstValLog[EDivLineLayerMain];
                    if (arfDivLineValLog[EDivLineLayerMain] > fScaleMinValLog) {
                        arfDivLineValLog[EDivLineLayerMain] -= arfDivLineDistValLog[EDivLineLayerMain];
                    }
                    arfDivLineValLog[EDivLineLayerSub] = arfDivLineValLog[EDivLineLayerMain];
                    arfDivLineValLin[EDivLineLayerSub] = pow(10.0,arfDivLineValLog[EDivLineLayerSub]);
                    int idxDivLine = 0;
                    while (arfDivLineValLin[EDivLineLayerSub] <= m_fScaleMax)
                    {
                        for (int idxDivLineTmp = 1; idxDivLineTmp < ariDivLineCountTmp[EDivLineLayerSub]-1; idxDivLineTmp++)
                        {
                            arfDivLineValLog[EDivLineLayerSub] = arfDivLineValLog[EDivLineLayerMain] + idxDivLineTmp*arfDivLineDistValLog[EDivLineLayerSub];
                            arfDivLineValLin[EDivLineLayerSub] = pow(10.0,arfDivLineValLog[EDivLineLayerSub]);
                            if (arfDivLineValLin[EDivLineLayerSub] >= m_fScaleMin && arfDivLineValLin[EDivLineLayerSub] <= m_fScaleMax) {
                                if (idxDivLineTmp < ariDivLineCount[EDivLineLayerSub]) {
                                    m_ararfDivLinesVals[EDivLineLayerSub][idxDivLine] = arfDivLineValLin[EDivLineLayerSub];
                                    m_ararfDivLines_px[EDivLineLayerSub][idxDivLine] = getValInPix(arfDivLineValLin[EDivLineLayerSub]);
                                    idxDivLine++;
                                }
                            }
                        }
                        arfDivLineValLog[EDivLineLayerMain] += arfDivLineDistValLog[EDivLineLayerMain];
                    }
                }
            }
        }
    }

    // If all main division lines are visible ...
    else
    {
        //   the decade between the main division lines will be divided into
        // sub division lines (non equidistant grid).

        double fScaleRangePixDec = 0.0;

        // It at least two main division lines are visible ...
        if (ariDivLineCount[EDivLineLayerMain] > 1) {
            fScaleRangePixDec = arfDivLineDistPix[EDivLineLayerMain] / arfDivLineDistValLog[EDivLineLayerMain];
        }
        // If no or just one main division line is visible ...
        else /*if (ariDivLineCount[EDivLineLayerMain] == 0)*/ {
            // If one main division line is visible it could be that more than one
            // decade is visible (e.g ranging from 4.0 e9 to 6.0 e10 with one grid
            // line at 1.0 e10) but it could also be that less than one decade is
            // visible (e.g. ranging from 6.0 e9 to 4.0 e10 with one grid line at 1.0 e10).

            // If no division line is visible the axis range is definitely less than
            // one decade.

            // Get the extended range:
            fScaleRangePixDec = fRange_px / fScaleRangeValLog;
        }

        // Please note that we cannot be sure that if we would just draw part of
        // the sub division lines that each of the sub division line will be labeled.
        // Even worse we don't know which (if any) division line will be labeled.
        // We could assume that the user agrees in that the division lines will be drawn
        // as long as the distance between the sub division lines is wide enough and
        // that always the last division lines will not be drawn. But in this case it could
        // happen that the distance between the last visible sub division line and the
        // following main grid line is wider than between the last visible sub division line
        // and the previous sub division line. And this would look strange ...
        // So we are just drawing all or none of the sub division lines:

        // If there is enough space for sub division lines ...
        if (m_ariDivLinesDistMin_px[EDivLineLayerSub] > 1 && s_arfScaleRangeFacPixDivValLog[2] * fScaleRangePixDec >= m_ariDivLinesDistMin_px[EDivLineLayerSub])
        {
            // Count the visible sub division lines
            // (not including main grid line at start and end of section):
            double fScaleMinValLinDec = fScaleMinValLinExt;
            arfDivLineValLin[EDivLineLayerSub] = fScaleMinValLinDec;
            while (arfDivLineValLin[EDivLineLayerSub] <= m_fScaleMax) {
                // Not including main grid line at start and end of section:
                double fFacLin = 2.0;
                for (int idxSection = 0; idxSection < 8; idxSection++, fFacLin += 1.0) {
                    arfDivLineValLin[EDivLineLayerSub] = fFacLin*fScaleMinValLinDec;
                    if (arfDivLineValLin[EDivLineLayerSub] >= m_fScaleMin && arfDivLineValLin[EDivLineLayerSub] <= m_fScaleMax) {
                        ariDivLineCount[EDivLineLayerSub]++;
                    }
                    if (arfDivLineValLin[EDivLineLayerSub] > m_fScaleMax) {
                        break;
                    }
                }
                fScaleMinValLinDec *= 10.0;
            }

            // Calculate pixel values and store the calculated results of the sub grid lines:
            if (ariDivLineCount[EDivLineLayerSub] > 0) {
                m_ararfDivLinesVals[EDivLineLayerSub].resize(ariDivLineCount[EDivLineLayerSub]);
                m_ararfDivLines_px[EDivLineLayerSub].resize(ariDivLineCount[EDivLineLayerSub]);

                fScaleMinValLinDec = fScaleMinValLinExt;
                arfDivLineValLin[EDivLineLayerSub] = fScaleMinValLinDec;
                //fScaleMinPixDec = fScaleMinPixExt;
                int idxDivLine = 0;
                while (arfDivLineValLin[EDivLineLayerSub] <= m_fScaleMax)
                {
                    // Not including main grid line at start and end of section:
                    double fFacLin = 2.0;
                    for (int idxSection = 0; idxSection < 8; idxSection++, fFacLin += 1.0)
                    {
                        arfDivLineValLin[EDivLineLayerSub] = fFacLin*fScaleMinValLinDec;
                        if (arfDivLineValLin[EDivLineLayerSub] >= m_fScaleMin
                         && arfDivLineValLin[EDivLineLayerSub] <= m_fScaleMax) {
                            if (idxDivLine < ariDivLineCount[EDivLineLayerSub]) {
                                m_ararfDivLinesVals[EDivLineLayerSub][idxDivLine] = arfDivLineValLin[EDivLineLayerSub];
                                m_ararfDivLines_px[EDivLineLayerSub][idxDivLine] = getValInPix(arfDivLineValLin[EDivLineLayerSub]);
                                idxDivLine++;
                            }
                        }
                        if (arfDivLineValLin[EDivLineLayerSub] > m_fScaleMax) {
                            break;
                        }
                    }
                    fScaleMinValLinDec *= 10.0;
                    //fScaleMinPixDec += fScaleRangePixDec;
                }
            }
        } // If there is enough space for sub division lines ...
    } // If all main division lines are visible ..


    //    // In logarithmic scale each decade may be divided into 1, 2, 4, 5, 10, 20, 40, 50, .. section(s).
    //    // The following algorithm assumes an equidistant division of the decade. As a
    //    // result the distance between the first division lines will be greater than
    //    // SubDivLineDistMinPix and the distance between the last division lines will
    //    // be less than SubDivLineDistMinPix.
    //    iSectionCount = 1;
    //    iSectionCountTmp = 1;
    //    iSectionCountNorm = 1;
    //    iSectionCountNormTmp = 1;
    //    iSectionCountNormExp = 0;
    //    iSectionCountNormExpTmp = 0;
    //    while( static_cast<int>(fScaleRangePixDec/iSectionCountTmp) > m_uSubDivLineDistMinPix )
    //    {
    //        iSectionCount = iSectionCountTmp;
    //        iSectionCountNorm = iSectionCountNormTmp;
    //        iSectionCountNormExp = iSectionCountNormExpTmp;

    //        switch( iSectionCountNorm )
    //        {
    //            case 1:
    //            {
    //                iSectionCountNormTmp = 2;
    //                break;
    //            }
    //            case 2:
    //            {
    //                iSectionCountNormTmp = 4;
    //                break;
    //            }
    //            case 4:
    //            {
    //                iSectionCountNormTmp = 5;
    //                break;
    //            }
    //            case 5:
    //            {
    //                iSectionCountNormExpTmp++;
    //                iSectionCountNormTmp = 1;
    //                break;
    //            }
    //        }
    //        iSectionCountTmp = iSectionCountNormTmp * pow(10.0,static_cast<int>(iSectionCountNormExpTmp));
    //    }

    //    // Count the visible sub division lines:

    //} // If all main division lines are visible ..

    //fSubDivLineDistValLinFact = 1.0 / (iSectionCountNorm*pow(10.0,static_cast<int>(iSectionCountNormExp)));

    //// Calculate count of visible sub division lines:
    //fSubDivLineValLinDecFirst = fScaleMinValLinExt;
    //fSubDivLineValLin = fSubDivLineValLinDecFirst + fSubDivLineDistValLinFact*fSubDivLineValLinDecFirst;
    //while( fSubDivLineValLin < m_fScaleMax )
    //{
    //    fSubDivLineValLin = 
    //}

    //// Calculate pixel values and store the calculated results of the sub grid lines:
    //if( uSubDivLineCount > 0 )
    //{
    //    m_arfSubDivLineVal = new double[uSubDivLineCount];
    //    m_arfSubDivLinePix = new double[uSubDivLineCount];

        //udxSubDivLine = 0;
        //fSubDivLineValLog = static_cast<int>(fScaleMinValLog/fSubDivLineDistValLog)*fSubDivLineDistValLog;
        //while( fSubDivLineValLog <= fScaleMaxValLog )
        //{
        //    fSubDivLineValLin = pow(10.0,fSubDivLineValLog);
        //    if( fSubDivLineValLin >= m_fScaleMin && fSubDivLineValLin <= m_fScaleMax )
        //    {
        //        if( udxSubDivLine < uSubDivLineCount )
        //        {
        //            m_arfSubDivLineVal[udxSubDivLine] = fSubDivLineValLin;
        //            m_arfSubDivLinePix[udxSubDivLine] = fSubDivLineDistValLog;
        //            udxSubDivLine++;
        //        }
        //    }
        //    fSubDivLineValLog += fSubDivLineDistValLog;
        //}
    //}

    for (int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++) {
        m_ariDivLinesCount[iLayer] = ariDivLineCount[iLayer];
    }
} // updateLogarithmicSpacing

//------------------------------------------------------------------------------
/*! @brief Merges the main and sub division lines.

    Duplicates are removed (sub division lines overlapping main division lines
    are not added to the resulting array).
    The division lines are sorted in ascending order (lowest values first).
*/
void CScaleDivLines::updateDivLinesValsSorted()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateDivLinesValsSorted",
        /* strAddInfo   */ "" );

    QVector<double> arfValsMain;
    int iValCountMain = m_ariDivLinesCount[EDivLineLayerMain];
    for (int idxVal = 0; idxVal < iValCountMain; ++idxVal) {
        double fVal = m_ararfDivLinesVals[EDivLineLayerMain][idxVal];
        arfValsMain.append(fVal);
    }
    QVector<double> arfValsSub;
    int iValCountSub = m_ariDivLinesCount[EDivLineLayerSub];
    for (int idxVal = 0; idxVal < iValCountSub; ++idxVal) {
        double fVal = m_ararfDivLinesVals[EDivLineLayerSub][idxVal];
        arfValsSub.append(fVal);
    }
    m_arfDivLinesValsSorted = Math::merge(arfValsMain, arfValsSub);
}
