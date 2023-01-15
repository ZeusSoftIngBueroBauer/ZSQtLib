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

#include <float.h>
#include <math.h>

#include "ZSDiagram/ZSDiagScale.h"
#include "ZSDiagram/ZSDiagram.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Diagram;
using namespace ZS::PhysVal;


/*******************************************************************************
class CDiagScale
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

bool   CDiagScale::s_bClassInitialised = false;
double CDiagScale::s_arfScaleRangeFacPixDivValLog[9];

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
int CDiagScale::CalculateDivLines4LinSpacing(
    double  i_fScaleMinVal,
    double  i_fScaleMaxVal,
    int     i_iScaleRangePix,
    double  i_fDivLineDistMinVal,
    int     i_iDivLineDistMinPix,
    bool    i_bUseDivLineDistValDecimalFactor25,
    double* o_pfDivLineFirstVal,
    double* o_pfDivLineDistFirstPix,
    double* o_pfDivLineDistVal,
    double* o_pfDivLineDistPix )
//------------------------------------------------------------------------------
{
    double fScaleRangeVal;
    double fScaleRangeFacPixDivVal;
    double fDivLineDistMinVal;
    double fDivLineDistVal;
    double fDivLineDistPix;
    double fDivLineVal;
    double fAbortVal;
    double fDivLineFirstVal;
    double fDivLineDistFirstPix;
    double fDivLineLastVal;
    double fDivLineDistLastPix;
    double fDivLineRangeVal;
    double fDivLineRangePix;
    int    iDivLineCountMax;
    int    iDivLineCount;
    int    iDivLineDistValDecimalFactor;
    double fDivLineDistValDecimalFactor;
    double fDivLineDistValBeforeCorr;
    double fDivLineDistPixBeforeCorr;

    if( o_pfDivLineFirstVal != nullptr )
    {
        *o_pfDivLineFirstVal = i_fScaleMinVal;
    }
    if( o_pfDivLineDistFirstPix != nullptr )
    {
        *o_pfDivLineDistFirstPix = 0.0;
    }
    if( o_pfDivLineDistVal != nullptr )
    {
        *o_pfDivLineDistVal = i_fScaleMaxVal - i_fScaleMinVal;
    }
    if( o_pfDivLineDistPix != nullptr )
    {
        *o_pfDivLineDistPix = i_iScaleRangePix;
    }

    fScaleRangeVal = i_fScaleMaxVal - i_fScaleMinVal;

    if( fScaleRangeVal <= 0.0 )
    {
        return 0;
    }
    if( i_iScaleRangePix <= 1 )
    {
        return 0;
    }
    if( i_iDivLineDistMinPix <= 1 )
    {
        return 0;
    }

    fScaleRangeFacPixDivVal = i_iScaleRangePix/fScaleRangeVal;

    // Maximum possible count of grid lines:
    iDivLineCountMax = static_cast<int>(static_cast<double>(i_iScaleRangePix)/static_cast<double>(i_iDivLineDistMinPix)) + 1;

    // On dividing the pixel range by the maximum possible count of grid lines the
    // distance between two grid lines would be:
    if( i_fDivLineDistMinVal > 0.0 )
    {
        fDivLineDistMinVal = i_fDivLineDistMinVal;
    }
    else
    {
        fDivLineDistMinVal = fScaleRangeVal / iDivLineCountMax;
    }

    // But DivLineDistMinVal might have a fractional part. And thats not what we want.
    // We need to set the grid lines to "human readable" values like 0.1 .. 0.2 .. 0.3 or
    // 10.0 .. 20.0 ..30.0 or 1.25 .. 1.50 .. 1.75 and so on.

    // For this we are going to calculate "DivLineDistVal" which is less or equal to the distance
    // between two grid lines if the pixel range would be divided by the maximum possible count
    // of grid lines and with the first valid digit "1" (e.g. 100.0, 1.0, 0.001, etc.).
    fDivLineDistVal = 1.0;
    if( fDivLineDistMinVal > 1.0 )
    {
        fAbortVal = fDivLineDistMinVal/10.0;
        while( fDivLineDistVal <= fAbortVal )
        {
            fDivLineDistVal *= 10.0;
        }
    }
    else if( fDivLineDistMinVal < 1.0 )
    {
        fAbortVal = fDivLineDistMinVal;
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
    modf(i_fScaleMinVal/fDivLineDistVal,&fDivLineFirstVal);
    fDivLineFirstVal *= fDivLineDistVal;
    if( fDivLineFirstVal < i_fScaleMinVal-DBL_EPSILON )
    {
        fDivLineFirstVal += fDivLineDistVal;
    }
    fDivLineDistFirstPix = fScaleRangeFacPixDivVal*(fDivLineFirstVal-i_fScaleMinVal);
    modf(i_fScaleMaxVal/fDivLineDistVal,&fDivLineLastVal);
    fDivLineLastVal *= fDivLineDistVal;
    if( fDivLineLastVal > i_fScaleMaxVal+DBL_EPSILON )
    {
        fDivLineLastVal -= fDivLineDistVal;
    }
    fDivLineDistLastPix = fScaleRangeFacPixDivVal*(i_fScaleMaxVal-fDivLineLastVal);

    // Range between first and last grid line:
    fDivLineRangeVal = fDivLineLastVal - fDivLineFirstVal;
    fDivLineRangePix = i_iScaleRangePix - fDivLineDistFirstPix - fDivLineDistLastPix; //lint !e834
    iDivLineCount = static_cast<int>(fDivLineRangeVal/fDivLineDistVal) + 1;

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
        iDivLineDistValDecimalFactor = 10;
        fDivLineDistValDecimalFactor = 1.0;
        fDivLineDistValBeforeCorr = fDivLineDistVal;
        fDivLineDistPixBeforeCorr = fDivLineDistPix;

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
    modf(i_fScaleMinVal/fDivLineDistVal,&fDivLineFirstVal);
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
    fDivLineVal = fDivLineFirstVal; // please note that the first digit of "DivLineFirstVal" is "1".
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
    return iDivLineCount;

} // CalculateDivLines4LinSpacing

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDiagScale::CDiagScale(
    const QString& i_strObjName,
    EScaleDir      i_scaleDir,
    const SScale&  i_scale ) :
//------------------------------------------------------------------------------
    QObject(),
    m_strObjName(i_strObjName),
    m_pDiagram(nullptr),
    m_scaleDir(i_scaleDir),
    m_spacing(ESpacingLinear),
    m_scale(i_scale),
    m_fScaleRes(0.0),
    m_bScaleResCalculated(false),
    m_iScaleMinValPix(0),
    m_iScaleMaxValPix(0),
    m_fPixRes(0.0),
    //m_ariDivLineDistMinPix[EDivLineLayerCount],
    //m_ariDivLineCount[EDivLineLayerCount],
    //m_ararfDivLineVal[EDivLineLayerCount],
    //m_ararfDivLinePix[EDivLineLayerCount],
    m_bDivLinesCalculated(false),
    m_pDiagScaleNext(nullptr),
    m_pDiagScalePrev(nullptr),
    m_iZoomCount(0),
    m_pZoomStackFirst(nullptr),
    m_pZoomStackLast(nullptr),
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjUpdate(nullptr),
    m_pTrcAdminObjLayout(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName(), objectName());
    m_pTrcAdminObjUpdate = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::Update", objectName());
    m_pTrcAdminObjLayout = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::Layout", objectName());

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_strObjName;
        strMthInArgs += ", ScaleDir: " + scaleDir2Str(i_scaleDir);
        strMthInArgs += ", Scale: " + i_scale.toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    int iLayer;

    for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
    {
        m_ariDivLineDistMinPix[iLayer] = 0;
        m_ariDivLineCount[iLayer] = 0;
        m_ararfDivLineVal[iLayer] = nullptr;
        m_ararfDivLinePix[iLayer] = nullptr;
    }
    m_ariDivLineDistMinPix[EDivLineLayerMain] = 50;

    if( !s_bClassInitialised )
    {
        int    idxSection;
        double fFacLin;

        // In logarithmic scale each decade may be divided into 9 sections indicating
        // the values 1.0*10^x, 2.0*10^x, .... , 9.0*10^x, 10.0*10^x with x as an
        // signed integer value. The width of the sections is decreasing from beginning
        // of the first value of the decade to the last value of the decade (non
        // equidistant grid). The portion of each sector is the same for all visible decades.
        for( idxSection = 0, fFacLin = 2.0; idxSection < 9; idxSection++, fFacLin += 1.0 )
        {
            s_arfScaleRangeFacPixDivValLog[idxSection] = log10(fFacLin) - log10(fFacLin-1.0);
        }
        s_bClassInitialised = true;
    }

} // ctor

//------------------------------------------------------------------------------
CDiagScale::~CDiagScale()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    int iLayer;

    for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
    {
        delete [] m_ararfDivLineVal[iLayer];
        m_ararfDivLineVal[iLayer] = nullptr;
        delete [] m_ararfDivLinePix[iLayer];
        m_ararfDivLinePix[iLayer] = nullptr;
    }

    if( m_iZoomCount > 0 )
    {
        try
        {
            clearScaleStack();
        }
        catch(...)
        {
        }
    }
    m_pZoomStackFirst = nullptr; // passierte zwar schon in clearScaleStack, weiss aber lint nicht
    m_pZoomStackLast = nullptr;  // passierte zwar schon in clearScaleStack, weiss aber lint nicht
    m_pDiagram = nullptr;
    m_pDiagScaleNext = nullptr;
    m_pDiagScalePrev = nullptr;

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjUpdate);
    m_pTrcAdminObjUpdate = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjLayout);
    m_pTrcAdminObjLayout = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CDiagScale::getObjName() const
//------------------------------------------------------------------------------
{
    return m_strObjName;
}

//------------------------------------------------------------------------------
EScaleDir CDiagScale::getScaleDir() const
//------------------------------------------------------------------------------
{
    return m_scaleDir;
}

//------------------------------------------------------------------------------
CDataDiagram* CDiagScale::getDiagram()
//------------------------------------------------------------------------------
{
    return m_pDiagram;
}

//------------------------------------------------------------------------------
void CDiagScale::setSpacing( ESpacing i_spacing )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave) )
    {
        strTrcMsg = spacing2Str(i_spacing);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setSpacing",
        /* strAddInfo   */ strTrcMsg );

    if( m_spacing != i_spacing )
    {
        m_spacing = i_spacing;
        m_bScaleResCalculated = false;
        m_bDivLinesCalculated = false;

        if( m_pDiagram != nullptr )
        {
            m_pDiagram->scaleChanged(this);
        }
    }

} // setSpacing

//------------------------------------------------------------------------------
ESpacing CDiagScale::getSpacing() const
//------------------------------------------------------------------------------
{
    return m_spacing;
}

//------------------------------------------------------------------------------
void CDiagScale::setScale( const SScale& i_scale )
//------------------------------------------------------------------------------
{
    if( m_scale != i_scale ) //lint !e1702
    {
        QString strTrcMsg;

        if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave) )
        {
            strTrcMsg = i_scale.toString();
        }

        CMethodTracer mthTracer(
            /* pAdminObj    */ m_pTrcAdminObj,
            /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strMethod    */ "setScale",
            /* strAddInfo   */ strTrcMsg );

        m_scale = i_scale;
        m_bScaleResCalculated = false;
        m_bDivLinesCalculated = false;

        if( m_pDiagram != nullptr )
        {
            m_pDiagram->scaleChanged(this);
        }
    }

} // setScale

//------------------------------------------------------------------------------
SScale CDiagScale::getScale() const
//------------------------------------------------------------------------------
{
    return m_scale;
}

//------------------------------------------------------------------------------
bool CDiagScale::isScaleValid() const
//------------------------------------------------------------------------------
{
    bool bScaleValid = true;

    if( getScaleRangePix() < 0 )
    {
        bScaleValid = false;
    }
    else if( m_scale.m_fMax <= m_scale.m_fMin )
    {
        bScaleValid = false;
    }
    else if( m_spacing == ESpacingLogarithmic )
    {
        if( m_scale.m_fMin <= 0.0 || m_scale.m_fMax <= 0.0 )
        {
            bScaleValid = false;
        }
    }
    return bScaleValid;

} // isScaleValid

//------------------------------------------------------------------------------
void CDiagScale::setDivLineDistMinPix( int i_iLayer, int i_iDistMinPix )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave) )
    {
        strTrcMsg = "Layer: " + divLineLayer2Str(i_iLayer);
        strTrcMsg += ", DistMinPix: " + QString::number(i_iDistMinPix);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setDivLineDistMinPix",
        /* strAddInfo   */ strTrcMsg );

    if( i_iLayer >= 0 && i_iLayer < EDivLineLayerCount )
    {
        if( m_ariDivLineDistMinPix[i_iLayer] != i_iDistMinPix )
        {
            m_ariDivLineDistMinPix[i_iLayer] = i_iDistMinPix;

            m_bDivLinesCalculated = false;

            if( m_pDiagram != nullptr )
            {
                m_pDiagram->scaleChanged(this);
            }
        }
    }

} // setMainDivLineDistMinPix

//------------------------------------------------------------------------------
int CDiagScale::getDivLineDistMinPix( int i_iLayer ) const
//------------------------------------------------------------------------------
{
    int iDivLineDistMinPix = 0;

    if( i_iLayer >= 0 && i_iLayer < EDivLineLayerCount )
    {
        iDivLineDistMinPix = m_ariDivLineDistMinPix[i_iLayer];
    }
    return iDivLineDistMinPix;
}

//------------------------------------------------------------------------------
void CDiagScale::zoomIn( int i_iZoomRectMinValPix, int i_iZoomRectMaxValPix )
//------------------------------------------------------------------------------
{
    SScale scaleZoom = m_scale;
    scaleZoom.m_fMin = getVal(static_cast<double>(i_iZoomRectMinValPix));
    scaleZoom.m_fMax = getVal(static_cast<double>(i_iZoomRectMaxValPix));
    pushScale(m_scale);
    setScale(scaleZoom); // sets the flag calculated to false and calls "scaleChanged" of diagram

} // zoomIn

//------------------------------------------------------------------------------
void CDiagScale::zoomIn( double i_fMinVal, double i_fMaxVal, CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    CUnit unit = m_scale.m_unit;

    if( i_pUnit != nullptr )
    {
        unit = *i_pUnit;
    }

    if( !areOfSameUnitGroup(m_scale.m_unit, unit) )
    {
        throw CException(__FILE__,__LINE__,EResultDifferentPhysSizes);
    }

    bool bConvertible = true;

    if( unit != m_scale.m_unit )
    {
        // I want to see if the physical unit is convertible before pushing
        // the new scale onto the stack. Otherwise the "unit not convertible"
        // exception would be thrown on poping the scale from the stack.
        // And thats too late to find the cause and source of the exception.
        bConvertible = unit.isConvertible(m_scale.m_unit, 0.0);
    }
    if( bConvertible )
    {
        SScale scaleZoom = m_scale;

        scaleZoom.m_fMin  = i_fMinVal;
        scaleZoom.m_fMax  = i_fMaxVal;
        scaleZoom.m_unit = unit;

        pushScale(m_scale);
        setScale(scaleZoom); // sets the flag calculated to false and calls "scaleChanged" of diagram
    }

} // zoomIn

//------------------------------------------------------------------------------
void CDiagScale::zoomOut( bool i_bComplete )
//------------------------------------------------------------------------------
{
    SScale scale = m_scale;

    if( i_bComplete )
    {
        if( m_iZoomCount > 0 && m_pZoomStackFirst != nullptr )
        {
            scale = m_pZoomStackFirst->m_scale;

            try
            {
                clearScaleStack();
            }
            catch(...)
            {
            }
        }
        m_pZoomStackFirst = nullptr; // passierte zwar schon in clearScaleStack, weiss aber lint nicht
        m_pZoomStackLast = nullptr;  // passierte zwar schon in clearScaleStack, weiss aber lint nicht
    }
    else if( m_iZoomCount > 0 )
    {
        scale = popScale();
    }
    if( scale != m_scale )
    {
        setScale(scale); // sets the flag calculated to false and calls "scaleChanged" of diagram
    }

} // zoomOut

//------------------------------------------------------------------------------
void CDiagScale::setGeometry( int i_iScaleMinValPix, int i_iScaleMaxValPix )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObjLayout != nullptr && m_pTrcAdminObjLayout->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strTrcMsg  = "MinValPix: " + QString::number(i_iScaleMinValPix);
        strTrcMsg += ", MaxValPix: " + QString::number(i_iScaleMaxValPix);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDatailLevel */ EMethodTraceDetailLevel::ArgsNormal,
        /* strMethod    */ "setGeometry",
        /* strAddInfo   */ strTrcMsg );

    if( m_iScaleMinValPix != i_iScaleMinValPix || m_iScaleMaxValPix != i_iScaleMaxValPix )
    {
        m_iScaleMinValPix = i_iScaleMinValPix;
        m_iScaleMaxValPix = i_iScaleMaxValPix;
        m_bScaleResCalculated = false;
        m_bDivLinesCalculated = false;

        if( m_pDiagram != nullptr )
        {
            m_pDiagram->scaleChanged(this);
        }
    }

} // setGeometry

//------------------------------------------------------------------------------
bool CDiagScale::areDivLinesCalculated() const
//------------------------------------------------------------------------------
{
    return m_bDivLinesCalculated;
}

//------------------------------------------------------------------------------
double CDiagScale::getPixRes( CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    CUnit unit = m_scale.m_unit;

    if( i_pUnit != nullptr )
    {
        unit = *i_pUnit;
    }
    if( !areOfSameUnitGroup(m_scale.m_unit,unit) )
    {
        throw CException(__FILE__,__LINE__,EResultDifferentPhysSizes);
    }

    double fPixRes = m_fPixRes;

    if( unit != m_scale.m_unit )
    {
        fPixRes = m_scale.m_unit.convertValue(fPixRes, unit);
    }
    return fPixRes;

} // getPixRes

//------------------------------------------------------------------------------
double CDiagScale::getScaleRes( CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    CUnit unit = m_scale.m_unit;

    if( i_pUnit != nullptr )
    {
        unit = *i_pUnit;
    }
    if( !areOfSameUnitGroup(m_scale.m_unit,unit) )
    {
        throw CException(__FILE__,__LINE__,EResultDifferentPhysSizes);
    }

    double fRes;

    if( m_spacing == ESpacingLinear )
    {
        fRes = m_fScaleRes;
    }
    else
    {
        fRes = pow(10.0,m_fScaleRes);
    }
    if( unit != m_scale.m_unit )
    {
        fRes = m_scale.m_unit.convertValue(fRes, unit);
    }
    return fRes;

} // getScaleRes

//------------------------------------------------------------------------------
double CDiagScale::getScaleRes( double i_fVal, CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    CUnit unit = m_scale.m_unit;

    if( i_pUnit != nullptr )
    {
        unit = *i_pUnit;
    }
    if( !areOfSameUnitGroup(m_scale.m_unit,unit) )
    {
        throw CException(__FILE__,__LINE__,EResultDifferentPhysSizes);
    }

    // In linear spacing the resolution is constant through the whole axis range.
    // In logarithmic spacing the resolution is not constant but decreases to
    // higher values.

    double fVal = i_fVal;
    double fRes;

    if( m_spacing == ESpacingLinear )
    {
        fRes = m_fScaleRes;
    }
    else
    {
        if( unit != m_scale.m_unit )
        {
            fVal = unit.convertValue(fVal, m_scale.m_unit);
        }
        fRes = logRes2LinRes(m_fScaleRes, fVal);
    }
    if( unit != m_scale.m_unit )
    {
        fRes = m_scale.m_unit.convertValue(fRes, unit);
    }
    return fRes;

} // getScaleRes

//------------------------------------------------------------------------------
double CDiagScale::round2ScaleRes( double i_fVal, CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    double fRes = getScaleRes(i_fVal, i_pUnit);
    double fVal = Math::round2Res(i_fVal, fRes);
    return fVal;
}

//------------------------------------------------------------------------------
int CDiagScale::getScaleMinValPix() const
//------------------------------------------------------------------------------
{
    return m_iScaleMinValPix;
}

//------------------------------------------------------------------------------
int CDiagScale::getScaleMaxValPix() const
//------------------------------------------------------------------------------
{
    return m_iScaleMaxValPix;
}

//------------------------------------------------------------------------------
int CDiagScale::getScaleRangePix() const
//------------------------------------------------------------------------------
{
    int iScaleRangePix = 0;

    if( m_iScaleMaxValPix > m_iScaleMinValPix )
    {
        iScaleRangePix = m_iScaleMaxValPix - m_iScaleMinValPix + 1; //lint !e834
    }
    else if( m_iScaleMinValPix > m_iScaleMaxValPix )
    {
        iScaleRangePix = m_iScaleMinValPix - m_iScaleMaxValPix + 1; //lint !e834
    }
    return iScaleRangePix;
}

//------------------------------------------------------------------------------
int CDiagScale::getDivLineCount( int i_iLayer ) const
//------------------------------------------------------------------------------
{
    int iDivLineCount = 0;

    if( i_iLayer >= 0 && i_iLayer < EDivLineLayerCount )
    {
        if( getScaleRangePix() > 1 && isScaleValid() && areDivLinesCalculated() )
        {
            iDivLineCount = m_ariDivLineCount[i_iLayer];
        }
    }
    return iDivLineCount;
}

//------------------------------------------------------------------------------
double CDiagScale::getDivLineVal( int i_iLayer, int i_idxDivLine, CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    CUnit unit = m_scale.m_unit;

    if( i_pUnit != nullptr )
    {
        unit = *i_pUnit;
    }
    if( !areOfSameUnitGroup(m_scale.m_unit,unit) )
    {
        throw CException(__FILE__,__LINE__,EResultDifferentPhysSizes);
    }

    double fDivLineVal = 0.0;

    if( i_iLayer >= 0 && i_iLayer < EDivLineLayerCount )
    {
        if( getScaleRangePix() > 1 && isScaleValid() && areDivLinesCalculated() )
        {
            if( i_idxDivLine < m_ariDivLineCount[i_iLayer] && m_ararfDivLineVal[i_iLayer] != nullptr )
            {
                fDivLineVal = m_ararfDivLineVal[i_iLayer][i_idxDivLine];
            }
            if( unit != m_scale.m_unit )
            {
                fDivLineVal = m_scale.m_unit.convertValue(fDivLineVal, unit);
            }
        }
    }
    return fDivLineVal;

} // getDivLineVal

//------------------------------------------------------------------------------
double CDiagScale::getDivLinePix( int i_iLayer, int i_idxDivLine ) const
//------------------------------------------------------------------------------
{
    double fDivLinePix = 0.0;

    if( i_iLayer >= 0 && i_iLayer < EDivLineLayerCount )
    {
        if( getScaleRangePix() > 1 && isScaleValid() && areDivLinesCalculated() )
        {
            if( i_idxDivLine < m_ariDivLineCount[i_iLayer] && m_ararfDivLinePix[i_iLayer] != nullptr )
            {
                fDivLinePix = m_ararfDivLinePix[i_iLayer][i_idxDivLine];
            }
        }
    }
    return fDivLinePix;
}

//------------------------------------------------------------------------------
double CDiagScale::getDivLineDistPix( int i_iLayer, int i_idxDivLine1, int i_idxDivLine2 ) const
//------------------------------------------------------------------------------
{
    double fDivLineDistPix = 0.0;

    if( i_iLayer >= 0 && i_iLayer < EDivLineLayerCount )
    {
        if( i_idxDivLine1 < m_ariDivLineCount[i_iLayer]
         && i_idxDivLine2 < m_ariDivLineCount[i_iLayer]
         && m_ararfDivLinePix[i_iLayer] != nullptr )
        {
            fDivLineDistPix = m_ararfDivLinePix[i_idxDivLine2] - m_ararfDivLinePix[i_idxDivLine1];
        }
    }
    return fDivLineDistPix;
}

//------------------------------------------------------------------------------
int CDiagScale::getValPix( double i_fVal, CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    CUnit unit = m_scale.m_unit;

    if( i_pUnit != nullptr )
    {
        unit = *i_pUnit;
    }
    if( !areOfSameUnitGroup(m_scale.m_unit,unit) )
    {
        throw CException(__FILE__,__LINE__,EResultDifferentPhysSizes);
    }

    double fPix = m_iScaleMinValPix;

    if( getScaleRangePix() > 1 && isScaleValid() )
    {
        double fVal = i_fVal;
        double fScalePixMin   = static_cast<double>(m_iScaleMinValPix);
        double fScalePixMax   = static_cast<double>(m_iScaleMaxValPix);
        double fScalePixRange  = static_cast<double>(getScaleRangePix());
        double fScaleValMin   = m_scale.m_fMin;
        double fScaleValMax   = m_scale.m_fMax;
        double fScaleValRange = fabs(fScaleValMax-fScaleValMin);
        double fPixRes;
        bool   bDivLineHit = false;
        double fDivLineVal1;
        double fDivLineVal2;
        int    iLayer;
        int    idxDivLine;

        // Calculate range and resolution of one pixel:
        if( m_spacing == ESpacingLogarithmic )
        {
            fScaleValMin = log10(fScaleValMin);
            fScaleValMax = log10(fScaleValMax);
        }
        fScaleValRange = fabs(fScaleValMax-fScaleValMin);
        fScalePixRange = fabs(fScalePixMax-fScalePixMin)+1;
        fPixRes        = (fScalePixRange-1) / fScaleValRange;

        // Convert value into unit of division line values.
        if( unit != m_scale.m_unit )
        {
            fVal = unit.convertValue(i_fVal, m_scale.m_unit);
        }

        // At the minimum scale value ..
        if( fVal == m_scale.m_fMin )
        {
            fPix = m_iScaleMinValPix;
        }
        // At the maximum scale value ..
        else if( fVal == m_scale.m_fMax )
        {
            fPix = m_iScaleMaxValPix;
        }

        // Somewhere between minimum and maximum scale ..
        else
        {
            // Get the nearest division line.
            for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
            {
                if( m_ararfDivLinePix[iLayer] != nullptr && m_ararfDivLineVal[iLayer] != nullptr )
                {
                    if( m_ariDivLineCount[iLayer] > 1 )
                    {
                        for( idxDivLine = 0; idxDivLine < m_ariDivLineCount[iLayer]-1; idxDivLine++ )
                        {
                            fDivLineVal1 = m_ararfDivLineVal[iLayer][idxDivLine];
                            fDivLineVal2 = m_ararfDivLineVal[iLayer][idxDivLine+1];

                            if( fVal >= (fDivLineVal1-DBL_EPSILON) && fVal <= (fDivLineVal1+DBL_EPSILON) )
                            {
                                fPix = static_cast<int>(m_ararfDivLinePix[iLayer][idxDivLine]);
                                bDivLineHit = true;
                                break;
                            }
                            else if( fVal >= (fDivLineVal2-DBL_EPSILON) && fVal <= (fDivLineVal2+DBL_EPSILON) )
                            {
                                fPix = static_cast<int>(m_ararfDivLinePix[iLayer][idxDivLine+1]);
                                bDivLineHit = true;
                                break;
                            }
                            else if( fVal > fDivLineVal1 && fVal < fDivLineVal2 )
                            {
                                break;
                            }
                        }
                    }
                    else if( m_ariDivLineCount[iLayer] == 1 )
                    {
                        fDivLineVal1 = m_ararfDivLineVal[iLayer][0];

                        if( fVal == fDivLineVal1 )
                        {
                            fPix = static_cast<int>(m_ararfDivLinePix[iLayer][0]);
                            bDivLineHit = true;
                        }
                    }
                } // if( m_ararfDivLinePix[iLayer] != nullptr && m_ararfDivLineVal[iLayer] != nullptr )

                if( bDivLineHit )
                {
                    break;
                }
            } // for( iLayer < EDivLineLayerCount )

            // Did not hit a division line ..
            if( !bDivLineHit )
            {
                if( m_spacing == ESpacingLogarithmic )
                {
                    fVal = log10(fVal);
                }

                // Calculate value as distance to left grid line:
                switch( m_scaleDir )
                {
                    case EScaleDirX:
                    {
                        fPix = fScalePixMin + fPixRes*(fVal-fScaleValMin); //lint !e834
                        break;
                    }
                    case EScaleDirY:
                    {
                        fPix = fScalePixMin - fPixRes*(fVal-fScaleValMin); //lint !e834
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            } // Neither hit a main nor a sub division line ..
        } // Somewhere between minimum and maximum scale ..
    } // if( isScaleValid() )

    return static_cast<int>(fPix);

} // getValPix

//------------------------------------------------------------------------------
QString CDiagScale::getValString(
    double i_fVal,
    CUnit* i_pUnit,
    int    i_iDigitsCountMax,
    bool   i_bUseEngineeringFormat ) const
//------------------------------------------------------------------------------
{
    CUnit unit = m_scale.m_unit;

    if( i_pUnit != nullptr )
    {
        unit = *i_pUnit;
    }
    if( !areOfSameUnitGroup(m_scale.m_unit,unit) )
    {
        throw CException(__FILE__,__LINE__,EResultDifferentPhysSizes);
    }

    QString strVal = "---";

    if( isScaleValid() )
    {
        // The scale resolution defines the number of valid digits.

        double fVal = i_fVal;
        int    iLeadingDigits = 1;
        int    iTrailingDigits = 1;
        int    iExponentDigits = 0;
        double fScaleRes = getScaleRes(i_fVal, &unit);

        if( fVal >= 10.0 )
        {
            iLeadingDigits = static_cast<int>(log10(fabs(fVal)))+1;
        }
        if( fScaleRes < 0.1 )
        {
            double fScaleResLog = log10(fabs(fScaleRes));
            double fScaleResLogInt;
            double fScaleResLogFrac = modf(fScaleResLog,&fScaleResLogInt);
            iTrailingDigits = static_cast<int>(-fScaleResLog);
            if( fScaleResLogFrac > 0.0 )
            {
                iTrailingDigits += 1;
            }
        }
        if( i_iDigitsCountMax > 0 || i_bUseEngineeringFormat )
        {
            if( ( (iLeadingDigits + iTrailingDigits) > i_iDigitsCountMax )
             || ( i_bUseEngineeringFormat && (iLeadingDigits > 1) ) )
            {
                if( fVal > 1.0 )
                {
                    iExponentDigits = static_cast<int>(log10(static_cast<double>(iLeadingDigits)))+1;
                }
                else if( fVal < 1.0 )
                {
                    iExponentDigits = static_cast<int>(log10(static_cast<double>(iTrailingDigits)))+1;
                }
                iTrailingDigits += iLeadingDigits-1;
                iLeadingDigits = 1;
                if( (iLeadingDigits + iTrailingDigits) > i_iDigitsCountMax )
                {
                    iTrailingDigits = i_iDigitsCountMax - iLeadingDigits;
                }
            }
        }
        if( iExponentDigits > 0 )
        {
            strVal = QString::number(
                /* fVal       */ fVal,
                /* chFormat   */ 'e',
                /* iPrecision */ iTrailingDigits );
        }
        else
        {
            strVal = QString::number(
                /* fVal       */ fVal,
                /* chFormat   */ 'f',
                /* iPrecision */ iTrailingDigits );
        }
    }
    return strVal;

} // getValString

//------------------------------------------------------------------------------
double CDiagScale::getVal( double i_fPix, CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    CUnit unit = m_scale.m_unit;

    if( i_pUnit != nullptr )
    {
        unit = *i_pUnit;
    }
    if( !areOfSameUnitGroup(m_scale.m_unit,unit) )
    {
        throw CException(__FILE__,__LINE__,EResultDifferentPhysSizes);
    }

    double fVal = m_scale.m_fMin;

    if( getScaleRangePix() <= 1 )
    {
        return m_scale.m_fMin;
    }
    if( isScaleValid() )
    {
        double fScalePixMin   = static_cast<double>(m_iScaleMinValPix);
        double fScalePixMax   = static_cast<double>(m_iScaleMaxValPix);
        double fScalePixRange = static_cast<double>(getScaleRangePix());
        double fScaleValMin   = m_scale.m_fMin;
        double fScaleValMax   = m_scale.m_fMax;
        double fScaleValRange = fabs(fScaleValMax-fScaleValMin);
        double fPixRes;
        bool   bDivLineHit = false;
        double fDivLinePix1;
        double fDivLinePix2;
        int    iLayer;
        int    idxDivLine;

        // Calculate range and resolution of one pixel:
        if( m_spacing == ESpacingLogarithmic )
        {
            fScaleValMin = log10(fScaleValMin);
            fScaleValMax = log10(fScaleValMax);
        }
        fScaleValRange = fabs(fScaleValMax-fScaleValMin);
        fScalePixRange = fabs(fScalePixMax-fScalePixMin)+1;
        fPixRes        = fScaleValRange / fScalePixRange;

        // At the minimum scale value ..
        if( i_fPix == m_iScaleMinValPix )
        {
            fVal = m_scale.m_fMin;
        }
        // At the maximum scale value ..
        else if( i_fPix == m_iScaleMaxValPix )
        {
            fVal = m_scale.m_fMax;
        }

        // Somewhere between minimum and maximum scale ..
        else
        {
            // Get the nearest division line. This is necessary to exactly return the value
            // at a grid line if the mouse cursor is positioned on a grid line.
            for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
            {
                if( m_ararfDivLinePix[iLayer] != nullptr && m_ararfDivLineVal[iLayer] != nullptr )
                {
                    if( m_ariDivLineCount[iLayer] > 1 )
                    {
                        for( idxDivLine = 0; idxDivLine < m_ariDivLineCount[iLayer]-1; idxDivLine++ )
                        {
                            fDivLinePix1 = m_ararfDivLinePix[iLayer][idxDivLine];
                            fDivLinePix2 = m_ararfDivLinePix[iLayer][idxDivLine+1];

                            if( i_fPix >= (fDivLinePix1-DBL_EPSILON) && i_fPix <= (fDivLinePix1+DBL_EPSILON) )
                            {
                                fVal = m_ararfDivLineVal[iLayer][idxDivLine];
                                bDivLineHit = true;
                                break;
                            }
                            else if( i_fPix >= (fDivLinePix2-DBL_EPSILON) && i_fPix <= (fDivLinePix2+DBL_EPSILON) )
                            {
                                fVal = m_ararfDivLineVal[iLayer][idxDivLine+1];
                                bDivLineHit = true;
                                break;
                            }
                            else if( i_fPix > fDivLinePix1 && i_fPix < fDivLinePix2 )
                            {
                                break;
                            }
                        }
                    }
                    else if( m_ariDivLineCount[iLayer] == 1 )
                    {
                        fDivLinePix1 = m_ararfDivLinePix[iLayer][0];

                        if( i_fPix == fDivLinePix1 )
                        {
                            fVal = static_cast<int>(m_ararfDivLineVal[iLayer][0]);
                            bDivLineHit = true;
                        }
                    }
                } // if( m_ararfDivLinePix[iLayer] != nullptr && m_ararfDivLineVal[iLayer] != nullptr )

                if( bDivLineHit )
                {
                    break;
                }
            } // for( iLayer < EDivLineLayerCount )

            // Did not hit a division line ..
            if( !bDivLineHit )
            {
                // Calculate value as distance to left grid line:
                switch( m_scaleDir )
                {
                    case EScaleDirX:
                    {
                        fVal = fScaleValMin + fPixRes*(i_fPix-fScalePixMin+1); //lint !e834
                        break;
                    }
                    case EScaleDirY:
                    {
                        fVal = fScaleValMin + fPixRes*(fScalePixMin-i_fPix+1); //lint !e834
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
                if( m_spacing == ESpacingLogarithmic )
                {
                    fVal = pow(10.0,fVal);
                }
            } // Neither hit a main nor a sub division line ..

        } // Somewhere between minimum and maximum scale ..

        if( m_spacing == ESpacingLinear )
        {
            fPixRes = Math::round2LowerDecade(fPixRes);
        }
        else
        {
            fPixRes = getScaleRes(fVal);
        }
        fVal = Math::round2Res(fVal,fPixRes);

    } // if( isScaleValid() )

    if( unit != m_scale.m_unit )
    {
        fVal = m_scale.m_unit.convertValue(fVal, unit);
    }
    return fVal;

} // getVal

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CDiagScale::update()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "update",
        /* strAddInfo   */ "" );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        QString strTrcMsg;
        strTrcMsg  = "ScaleIsValid=";
        strTrcMsg += QString::number(isScaleValid()) + ", ";
        strTrcMsg += "DivLinesCalculated=";
        strTrcMsg += QString::number(m_bDivLinesCalculated) + ", ";
        strTrcMsg += "ScaleResCalculated=";
        strTrcMsg += QString::number(m_bScaleResCalculated);
        mthTracer.trace(strTrcMsg);
    }

    if( m_bDivLinesCalculated && m_bScaleResCalculated )
    {
        return;
    }

    int    iScaleRangePix = getScaleRangePix();
    double fScaleRangePix = iScaleRangePix;
    double fScaleRangeVal = m_scale.m_fMax - m_scale.m_fMin;
    int    iLayer;

    // Division lines:
    //----------------

    if( !m_bDivLinesCalculated )
    {
        for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
        {
            m_ariDivLineCount[iLayer] = 0;
            delete [] m_ararfDivLineVal[iLayer];
            m_ararfDivLineVal[iLayer] = nullptr;
            delete [] m_ararfDivLinePix[iLayer];
            m_ararfDivLinePix[iLayer] = nullptr;
        }
        if( iScaleRangePix <= 1 || m_ariDivLineDistMinPix[EDivLineLayerMain] < 2 )
        {
            m_fPixRes = m_scale.m_fMax - m_scale.m_fMin;
        }
        else if( !isScaleValid() )
        {
            m_fPixRes = m_scale.m_fMax - m_scale.m_fMin;
            m_ariDivLineCount[EDivLineLayerMain] = 2;
            m_ararfDivLineVal[EDivLineLayerMain] = new double[m_ariDivLineCount[EDivLineLayerMain]];
            m_ararfDivLinePix[EDivLineLayerMain] = new double[m_ariDivLineCount[EDivLineLayerMain]];
            m_ararfDivLineVal[EDivLineLayerMain][0] = m_scale.m_fMin;
            m_ararfDivLineVal[EDivLineLayerMain][1] = m_scale.m_fMax;
            m_ararfDivLinePix[EDivLineLayerMain][0] = m_iScaleMinValPix;
            m_ararfDivLinePix[EDivLineLayerMain][1] = m_iScaleMaxValPix;
        }

        // Calculate main and sub division lines
        //--------------------------------------

        else
        {
            // Calculate division lines for linear spacing
            //--------------------------------------------

            int ariDivLineCount[EDivLineLayerCount];

            for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
            {
                ariDivLineCount[iLayer] = 0;
            }

            if( m_spacing == ESpacingLinear )
            {
                double fDivLineDistValMin = m_scale.m_fMin;
                double fDivLineDistValMax = m_scale.m_fMax;
                int    ariDivLineCountTmp[EDivLineLayerCount];
                double arfDivLineFirstVal[EDivLineLayerCount];
                double arfDivLineDistFirstPix[EDivLineLayerCount];
                double arfDivLineDistVal[EDivLineLayerCount];
                double arfDivLineDistPix[EDivLineLayerCount];
                double fPrevLayerDivLineVal;
                double fCurrLayerDivLineVal;
                int    idxDivLine;
                int    idxDivLineTmp;

                m_fPixRes = fScaleRangeVal / (fScaleRangePix-1.0);

                for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
                {
                    // Calculate optimized distance between two grid lines:
                    ariDivLineCountTmp[iLayer] = CalculateDivLines4LinSpacing(
                        /* fScaleMinVal          */ fDivLineDistValMin,
                        /* fScaleMaxVal          */ fDivLineDistValMax,
                        /* iScaleRangePix        */ iScaleRangePix,
                        /* fDivLineDistMinVal    */ 0.0,
                        /* iDivLineDistMinPix    */ m_ariDivLineDistMinPix[iLayer],
                        /* bUseDivLineDecFac25   */ true,
                        /* pfDivLineFirstVal     */ &arfDivLineFirstVal[iLayer],
                        /* pfDivLineDistFirstPix */ &arfDivLineDistFirstPix[iLayer],
                        /* pfDivLineDistVal      */ &arfDivLineDistVal[iLayer],
                        /* pfDivLineDistPix      */ &arfDivLineDistPix[iLayer] );

                    // Store the calculated results:
                    if( ariDivLineCountTmp[iLayer] > 0 )
                    {
                        if( iLayer == 0 )
                        {
                            // Count visible sub division lines (thats eays for the main grid lines):
                            ariDivLineCount[iLayer] = ariDivLineCountTmp[iLayer];

                            m_ararfDivLineVal[iLayer] = new double[ariDivLineCount[iLayer]];
                            m_ararfDivLinePix[iLayer] = new double[ariDivLineCount[iLayer]];

                            // Calculate pixel values and store the calculated results of the main grid lines:
                            for( idxDivLine = 0; idxDivLine < ariDivLineCount[iLayer]; idxDivLine++ )
                            {
                                double fVal = arfDivLineFirstVal[iLayer] + idxDivLine*arfDivLineDistVal[iLayer];
                                if( fabs(fVal) < DBL_EPSILON )
                                {
                                    fVal = 0.0;
                                }
                                m_ararfDivLineVal[iLayer][idxDivLine] = fVal;
                                m_ararfDivLinePix[iLayer][idxDivLine] = getValPix(fVal);
                            }
                        }
                        else
                        {
                            // Count visible sub division lines:
                            modf(m_scale.m_fMin/arfDivLineDistVal[iLayer-1],&fPrevLayerDivLineVal);
                            fPrevLayerDivLineVal *= arfDivLineDistVal[iLayer-1];
                            fCurrLayerDivLineVal = fPrevLayerDivLineVal;
                            while( fCurrLayerDivLineVal <= m_scale.m_fMax )
                            {
                                for( idxDivLineTmp = 1; idxDivLineTmp < ariDivLineCountTmp[iLayer]-1; idxDivLineTmp++ )
                                {
                                    fCurrLayerDivLineVal = fPrevLayerDivLineVal + idxDivLineTmp*arfDivLineDistVal[iLayer];
                                    if( fabs(fCurrLayerDivLineVal) < DBL_EPSILON )
                                    {
                                        fCurrLayerDivLineVal = 0.0;
                                    }
                                    if( fCurrLayerDivLineVal >= m_scale.m_fMin && fCurrLayerDivLineVal <= m_scale.m_fMax )
                                    {
                                        ariDivLineCount[iLayer]++;
                                    }
                                }
                                fPrevLayerDivLineVal += arfDivLineDistVal[iLayer-1];
                            }

                            // Calculate pixel values and store the calculated results of the sub grid lines:
                            if( ariDivLineCount[iLayer] > 0 )
                            {
                                m_ararfDivLineVal[iLayer] = new double[ariDivLineCount[iLayer]];
                                m_ararfDivLinePix[iLayer] = new double[ariDivLineCount[iLayer]];

                                modf(m_scale.m_fMin/arfDivLineDistVal[iLayer-1],&fPrevLayerDivLineVal);
                                fPrevLayerDivLineVal *= arfDivLineDistVal[iLayer-1];
                                fCurrLayerDivLineVal = fPrevLayerDivLineVal;
                                idxDivLine = 0;
                                while( fCurrLayerDivLineVal <= m_scale.m_fMax )
                                {
                                    for( idxDivLineTmp = 1; idxDivLineTmp < ariDivLineCountTmp[iLayer]-1; idxDivLineTmp++ )
                                    {
                                        fCurrLayerDivLineVal = fPrevLayerDivLineVal + idxDivLineTmp*arfDivLineDistVal[iLayer];
                                        if( fabs(fCurrLayerDivLineVal) < DBL_EPSILON )
                                        {
                                            fCurrLayerDivLineVal = 0.0;
                                        }
                                        if( fCurrLayerDivLineVal >= m_scale.m_fMin && fCurrLayerDivLineVal <= m_scale.m_fMax )
                                        {
                                            if( idxDivLine < ariDivLineCount[iLayer] )
                                            {
                                                m_ararfDivLineVal[iLayer][idxDivLine] = fCurrLayerDivLineVal;
                                                m_ararfDivLinePix[iLayer][idxDivLine] = getValPix(fCurrLayerDivLineVal);
                                                idxDivLine++;
                                            }
                                        }
                                    }
                                    fPrevLayerDivLineVal += arfDivLineDistVal[iLayer-1];
                                }
                            }
                        } // if( iLayer > 0 )
                    } // if( ariDivLineCount[iLayer] > 0 )

                    // If there is a next layer ...
                    if( iLayer < EDivLineLayerCount-1 )
                    {
                        if( m_ariDivLineDistMinPix[iLayer+1] > 1 && m_ariDivLineDistMinPix[iLayer] > 2*m_ariDivLineDistMinPix[iLayer+1] )
                        {
                            fDivLineDistValMin = 0.0;
                            fDivLineDistValMax = arfDivLineDistVal[iLayer];
                        }
                        else
                        {
                            // Next layer is empty ...
                            break;
                        }
                    }

                } // for( iLayer < EDivLineLayerCount )
            } // if( m_spacing == ESpacingLinear )

            // Calculate division lines for logarithmic spacing
            //-------------------------------------------------

            else // if( m_spacing == ESpacingLogarithmic )
            {
                // Sub grid (non equidistant) within main grid (equidistant) NOT including "left" and
                // "right" main grid lines. Please note that in a logarithmic scale one decade comprises
                // 8 sub grid lines, e.g. 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 (the values 1.0 and
                // 10.0 belong to the main grid):

                double fScaleMinValLinNorm;
                double fScaleMinValLinNormLeading;
                double fScaleMinValLinNormFract;
                int    iScaleMinValLinNormExp;
                double fScaleMinValLinExt;
                double fScaleMinValLinDec;
                double fScaleMinValLog;
                double fScaleMinValLogExt;
                double fScaleMaxValLinNorm;
                double fScaleMaxValLinNormLeading;
                double fScaleMaxValLinNormFract;
                int    iScaleMaxValLinNormExp;
                double fScaleMaxValLog;
                double fScaleMaxValLogExt;
                double fScaleRangeValLog;
                double fScaleRangePixDec;
                int    ariDivLineCountMax[EDivLineLayerCount];
                int    ariDivLineDistMinPix[EDivLineLayerCount];
                double arfDivLineFirstValLog[EDivLineLayerCount];
                double arfDivLineDistFirstPix[EDivLineLayerCount];
                double arfDivLineDistValLog[EDivLineLayerCount];
                double arfDivLineDistPix[EDivLineLayerCount];
                double arfDivLineValLin[EDivLineLayerCount];
                double arfDivLineValLog[EDivLineLayerCount];
                int    idxDivLine;
                int    idxDivLineTmp;
                int    ariDivLineCountTmp[EDivLineLayerCount];
                int    iLayerMain = EDivLineLayerMain;
                int    iLayerSub = EDivLineLayerSub;
                double fFacLin;
                int    idxSection;

                for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
                {
                    ariDivLineCountMax[iLayer] = 0;
                    ariDivLineDistMinPix[iLayer] = 0;
                    arfDivLineFirstValLog[iLayer] = 0.0;
                    arfDivLineDistFirstPix[iLayer] = 0.0;
                    arfDivLineDistValLog[iLayer] = 0.0;
                    arfDivLineDistPix[iLayer] = 0.0;
                    arfDivLineValLin[iLayer] = 0.0;
                    arfDivLineValLog[iLayer] = 0.0;
                    ariDivLineCountTmp[iLayer] = 0;
                }

                fScaleMinValLog = log10(m_scale.m_fMin);
                fScaleMaxValLog = log10(m_scale.m_fMax);
                fScaleRangeValLog = fScaleMaxValLog - fScaleMinValLog;

                m_fPixRes = fScaleRangeValLog / (fScaleRangePix-1.0);

                // Normalize the axis range to engineering format (e.g. 4.234e9):
                iScaleMinValLinNormExp = static_cast<int>(fScaleMinValLog);
                fScaleMinValLinNorm = m_scale.m_fMin/pow(10.0,iScaleMinValLinNormExp);
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
                fScaleMinValLinNormFract = modf(fScaleMinValLinNorm,&fScaleMinValLinNormLeading);

                iScaleMaxValLinNormExp = static_cast<int>(fScaleMaxValLog);
                fScaleMaxValLinNorm = m_scale.m_fMax/pow(10.0,iScaleMaxValLinNormExp);
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
                fScaleMaxValLinNormFract = modf(fScaleMaxValLinNorm,&fScaleMaxValLinNormLeading);

                // Extend the axis range by rounding down the scale minimum value and rounding up the
                // scale maximum value so that the following applies:
                // - log(ScaleMinVal) MOD 1 = 0
                // - log(ScaleMaxVal) MOD 1 = 0

                // Round down minimum scale value:
                fScaleMinValLogExt = iScaleMinValLinNormExp;
                fScaleMinValLinExt = pow(10.0,fScaleMinValLogExt);

                // Round up maximum scale value:
                fScaleMaxValLogExt = iScaleMaxValLinNormExp;
                if( fScaleMaxValLinNorm > 1.0 || fScaleMaxValLinNormFract > 0.0 || fScaleMaxValLogExt == fScaleMinValLogExt ) //lint !e777
                {
                    fScaleMaxValLogExt += 1.0;
                }

                // Get extended range:
                //fScaleMinPixExt = m_iScaleMinValPix - (fScaleMinValLog-fScaleMinValLogExt) * (iScaleRangePix/fScaleRangeValLog);

                // Calculate "main" division lines.
                //---------------------------------

                // For the values of the main division lines the following constraint must apply:
                // - log(MainDivLineVal) MOD 1 = 0
                // The distances between the main division lines are the same (equidistant grid).

                // Calculate maximum number of visible main grid lines:
                ariDivLineCountMax[iLayerMain] = static_cast<unsigned int>(iScaleMaxValLinNormExp-iScaleMinValLinNormExp)+1;
                if( ariDivLineCountMax[iLayerMain] > 0 && (fScaleMinValLinNorm > 1.0 || fScaleMinValLinNormFract >= DBL_EPSILON) )
                {
                    ariDivLineCountMax[iLayerMain]--;
                }

                // If at least one main grid line is visible ...
                if( ariDivLineCountMax[iLayerMain] > 0 )
                {
                    // Prepare logarithmic scale values for linear spacing:
                    // First assume that the value distance between the main grid lines is 1.0
                    // (the lowest possible value for main grid lines).
                    ariDivLineDistMinPix[iLayerMain] = static_cast<int>(static_cast<double>(iScaleRangePix) / fScaleRangeValLog);
                    // If the distance of 1.0 between the main grid lines is too small ..
                    if( ariDivLineDistMinPix[iLayerMain] < m_ariDivLineDistMinPix[iLayerMain] )
                    {
                        ariDivLineDistMinPix[iLayerMain] = m_ariDivLineDistMinPix[iLayerMain];
                    }
                    // If the distance of 1.0 between the main grid lines is too big ..
                    else if( ariDivLineDistMinPix[iLayerMain] > iScaleRangePix / 2 )
                    {
                        ariDivLineDistMinPix[iLayerMain] = iScaleRangePix / 2;
                    }

                    // Calculate optimized distance between two main grid lines:
                    ariDivLineCount[iLayerMain] = CalculateDivLines4LinSpacing(
                        /* fScaleMinVal          */ fScaleMinValLog,
                        /* fScaleMaxVal          */ fScaleMaxValLog,
                        /* iScaleRangePix        */ iScaleRangePix,
                        /* fDivLineDistMinVal    */ 1.0,
                        /* iDivLineDistMinPix    */ ariDivLineDistMinPix[iLayerMain],
                        /* bUseDivLineDecFac25   */ false,
                        /* pfDivLineFirstVal     */ &arfDivLineFirstValLog[iLayerMain],
                        /* pfDivLineDistFirstPix */ &arfDivLineDistFirstPix[iLayerMain],
                        /* pfDivLineDistVal      */ &arfDivLineDistValLog[iLayerMain],
                        /* pfDivLineDistPix      */ &arfDivLineDistPix[iLayerMain] );
                }

                // Store the calculated results:
                if( ariDivLineCount[iLayerMain] > 0 )
                {
                    m_ararfDivLineVal[iLayerMain] = new double[ariDivLineCount[iLayerMain]];
                    m_ararfDivLinePix[iLayerMain] = new double[ariDivLineCount[iLayerMain]];

                    for( idxDivLine = 0; idxDivLine < ariDivLineCount[iLayerMain]; idxDivLine++ )
                    {
                        arfDivLineValLog[iLayerMain] = arfDivLineFirstValLog[iLayerMain] + idxDivLine*arfDivLineDistValLog[iLayerMain];
                        arfDivLineValLin[iLayerMain] = pow(10.0,arfDivLineValLog[iLayerMain]);
                        m_ararfDivLineVal[iLayerMain][idxDivLine] = arfDivLineValLin[iLayerMain];
                        m_ararfDivLinePix[iLayerMain][idxDivLine] = getValPix(arfDivLineValLin[iLayerMain]);
                    }
                }

                // Calculate "sub" division lines.
                //---------------------------------

                // If not each main division line is visible (e.g. if the scale range enclosed
                // a huge range (many decades) and not each main grid line may be drawn because
                // the distance between two main grid lines would be below the minimum distance
                // between the main grid lines) ..
                if( ariDivLineCount[iLayerMain] < ariDivLineCountMax[iLayerMain] )
                {
                    // .. some (or all) of the not yet visible main division lines
                    // become the sub division lines (equidistant grid).

                    // Calculate optimized distance between two sub grid lines:
                    if( m_ariDivLineDistMinPix[iLayerSub] > 1 && m_ariDivLineDistMinPix[iLayerMain] > 2*m_ariDivLineDistMinPix[iLayerSub] )
                    {
                        ariDivLineCountTmp[iLayerSub] = CalculateDivLines4LinSpacing(
                            /* fScaleMinVal          */ arfDivLineFirstValLog[iLayerMain],
                            /* fScaleMaxVal          */ arfDivLineFirstValLog[iLayerMain]+arfDivLineDistValLog[iLayerMain],
                            /* iScaleRangePix        */ static_cast<int>(arfDivLineDistPix[iLayerMain]),
                            /* fDivLineDistMinVal    */ 1.0,
                            /* iDivLineDistMinPix    */ m_ariDivLineDistMinPix[iLayerSub],
                            /* bUseDivLineDecFac25   */ false,
                            /* pfDivLineFirstVal     */ &arfDivLineFirstValLog[iLayerSub],
                            /* pfDivLineDistFirstPix */ &arfDivLineDistFirstPix[iLayerSub],
                            /* pfDivLineDistVal      */ &arfDivLineDistValLog[iLayerSub],
                            /* pfDivLineDistPix      */ nullptr );

                        // Store the calculated results:
                        if( ariDivLineCountTmp[iLayerSub] > 2 )
                        {
                            // Count visible sub division lines:
                            arfDivLineValLog[iLayerMain] = arfDivLineFirstValLog[iLayerMain];
                            if( arfDivLineValLog[iLayerMain] > fScaleMinValLog )
                            {
                                arfDivLineValLog[iLayerMain] -= arfDivLineDistValLog[iLayerMain];
                            }
                            arfDivLineValLog[iLayerSub] = arfDivLineValLog[iLayerMain];
                            arfDivLineValLin[iLayerSub] = pow(10.0,arfDivLineValLog[iLayerSub]);
                            while( arfDivLineValLin[iLayerSub] <= m_scale.m_fMax )
                            {
                                for( idxDivLineTmp = 1; idxDivLineTmp < ariDivLineCountTmp[iLayerSub]-1; idxDivLineTmp++ )
                                {
                                    arfDivLineValLog[iLayerSub] = arfDivLineValLog[iLayerMain] + idxDivLineTmp*arfDivLineDistValLog[iLayerSub];
                                    arfDivLineValLin[iLayerSub] = pow(10.0,arfDivLineValLog[iLayerSub]);
                                    if( arfDivLineValLin[iLayerSub] >= m_scale.m_fMin && arfDivLineValLin[iLayerSub] <= m_scale.m_fMax )
                                    {
                                        ariDivLineCount[iLayerSub]++;
                                    }
                                }
                                arfDivLineValLog[iLayerMain] += arfDivLineDistValLog[iLayerMain];
                            }

                            // Calculate pixel values and store the calculated results of the sub grid lines:
                            if( ariDivLineCount[iLayerSub] > 0 )
                            {
                                m_ararfDivLineVal[iLayerSub] = new double[ariDivLineCount[iLayerSub]];
                                m_ararfDivLinePix[iLayerSub] = new double[ariDivLineCount[iLayerSub]];

                                arfDivLineValLog[iLayerMain] = arfDivLineFirstValLog[iLayerMain];
                                if( arfDivLineValLog[iLayerMain] > fScaleMinValLog )
                                {
                                    arfDivLineValLog[iLayerMain] -= arfDivLineDistValLog[iLayerMain];
                                }
                                arfDivLineValLog[iLayerSub] = arfDivLineValLog[iLayerMain];
                                arfDivLineValLin[iLayerSub] = pow(10.0,arfDivLineValLog[iLayerSub]);
                                idxDivLine = 0;
                                while( arfDivLineValLin[iLayerSub] <= m_scale.m_fMax )
                                {
                                    for( idxDivLineTmp = 1; idxDivLineTmp < ariDivLineCountTmp[iLayerSub]-1; idxDivLineTmp++ )
                                    {
                                        arfDivLineValLog[iLayerSub] = arfDivLineValLog[iLayerMain] + idxDivLineTmp*arfDivLineDistValLog[iLayerSub];
                                        arfDivLineValLin[iLayerSub] = pow(10.0,arfDivLineValLog[iLayerSub]);
                                        if( arfDivLineValLin[iLayerSub] >= m_scale.m_fMin && arfDivLineValLin[iLayerSub] <= m_scale.m_fMax )
                                        {
                                            if( idxDivLineTmp < ariDivLineCount[iLayerSub] )
                                            {
                                                m_ararfDivLineVal[iLayerSub][idxDivLine] = arfDivLineValLin[iLayerSub];
                                                m_ararfDivLinePix[iLayerSub][idxDivLine] = getValPix(arfDivLineValLin[iLayerSub]);
                                                idxDivLine++;
                                            }
                                        }
                                    }
                                    arfDivLineValLog[iLayerMain] += arfDivLineDistValLog[iLayerMain];
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

                    // It at least two main division lines are visible ...
                    if( ariDivLineCount[iLayerMain] > 1 )
                    {
                        fScaleRangePixDec = arfDivLineDistPix[iLayerMain] / arfDivLineDistValLog[iLayerMain];
                    }
                    // If no or just one main division line is visible ...
                    else // if( ariDivLineCount[iLayerMain] == 0 )
                    {
                        // If one main division line is visible it could be that more than one
                        // decade is visible (e.g ranging from 4.0 e9 to 6.0 e10 with one grid
                        // line at 1.0 e10) but it could also be that less than one decade is
                        // visible (e.g. ranging from 6.0 e9 to 4.0 e10 with one grid line at 1.0 e10).

                        // If no division line is visible the axis range is definitely less than
                        // one decade.

                        // Get the extended range:
                        fScaleRangePixDec = iScaleRangePix / fScaleRangeValLog;
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
                    if( m_ariDivLineDistMinPix[iLayerSub] > 1 && s_arfScaleRangeFacPixDivValLog[2] * fScaleRangePixDec >= m_ariDivLineDistMinPix[iLayerSub] )
                    {
                        // Count the visible sub division lines
                        // (not including main grid line at start and end of section):
                        fScaleMinValLinDec = fScaleMinValLinExt;
                        arfDivLineValLin[iLayerSub] = fScaleMinValLinDec;
                        while( arfDivLineValLin[iLayerSub] <= m_scale.m_fMax )
                        {
                            // Not including main grid line at start and end of section:
                            for( idxSection = 0, fFacLin = 2.0; idxSection < 8; idxSection++, fFacLin += 1.0 )
                            {
                                arfDivLineValLin[iLayerSub] = fFacLin*fScaleMinValLinDec;
                                if( arfDivLineValLin[iLayerSub] >= m_scale.m_fMin && arfDivLineValLin[iLayerSub] <= m_scale.m_fMax )
                                {
                                    ariDivLineCount[iLayerSub]++;
                                }
                                if( arfDivLineValLin[iLayerSub] > m_scale.m_fMax )
                                {
                                    break;
                                }
                            }
                            fScaleMinValLinDec *= 10.0;
                        }

                        // Calculate pixel values and store the calculated results of the sub grid lines:
                        if( ariDivLineCount[iLayerSub] > 0 )
                        {
                            m_ararfDivLineVal[iLayerSub] = new double[ariDivLineCount[iLayerSub]];
                            m_ararfDivLinePix[iLayerSub] = new double[ariDivLineCount[iLayerSub]];

                            fScaleMinValLinDec = fScaleMinValLinExt;
                            arfDivLineValLin[iLayerSub] = fScaleMinValLinDec;
                            //fScaleMinPixDec = fScaleMinPixExt;
                            idxDivLine = 0;
                            while( arfDivLineValLin[iLayerSub] <= m_scale.m_fMax )
                            {
                                // Not including main grid line at start and end of section:
                                for( idxSection = 0, fFacLin = 2.0; idxSection < 8; idxSection++, fFacLin += 1.0 )
                                {
                                    arfDivLineValLin[iLayerSub] = fFacLin*fScaleMinValLinDec;
                                    if( arfDivLineValLin[iLayerSub] >= m_scale.m_fMin && arfDivLineValLin[iLayerSub] <= m_scale.m_fMax )
                                    {
                                        if( idxDivLine < ariDivLineCount[iLayerSub] )
                                        {
                                            m_ararfDivLineVal[iLayerSub][idxDivLine] = arfDivLineValLin[iLayerSub];
                                            m_ararfDivLinePix[iLayerSub][idxDivLine] = getValPix(arfDivLineValLin[iLayerSub]);
                                            idxDivLine++;
                                        }
                                    }
                                    if( arfDivLineValLin[iLayerSub] > m_scale.m_fMax )
                                    {
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
                //while( fSubDivLineValLin < m_scale.m_fMax )
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
                    //    if( fSubDivLineValLin >= m_scale.m_fMin && fSubDivLineValLin <= m_scale.m_fMax )
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

            } // if( m_spacing == ESpacingLogarithmic )

            for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
            {
                m_ariDivLineCount[iLayer] = ariDivLineCount[iLayer];
            }

        } // if( validScale() && ScaleRangePix > 0 )

        m_bDivLinesCalculated = true;

    } // if( !m_bDivLinesCalculated )

    // Resolution:
    //------------

    // The scale resolution depends on the scale range (pixel range / value range)

    if( !m_bScaleResCalculated )
    {
        double fValRes = fScaleRangeVal;

        if( isScaleValid() )
        {
            if( m_spacing == ESpacingLinear )
            {
                if( fScaleRangePix > 1.0 )
                {
                    fValRes = fScaleRangeVal / (fScaleRangePix-1.0);
                }
                else
                {
                    fValRes = fScaleRangeVal / 1000.0;
                }
            }
            else
            {
                double fScaleRangeValLog = log10(m_scale.m_fMax) - log10(m_scale.m_fMin);

                if( fScaleRangePix > 1.0 )
                {
                    fValRes = fScaleRangeValLog / (fScaleRangePix-1.0);
                }
                else
                {
                    fValRes = fScaleRangeValLog / 1000.0;
                }
            }
            fValRes = Math::round2LowerDecade(fValRes);
        }
        m_fScaleRes = fValRes;
        m_bScaleResCalculated = true;

    } // if( !m_bScaleResCalculated )

    if( m_pDiagram != nullptr )
    {
        m_pDiagram->scaleChanged(this);
    }
    emit scaleChanged(this);

} // update

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
CDiagScale* CDiagScale::clone( CDataDiagram* i_pDiagramTrg ) const
//------------------------------------------------------------------------------
{
    if( i_pDiagramTrg == nullptr )
    {
        return nullptr;
    }

    int iLayer;

    CDiagScale* pDiagScale = new CDiagScale(
        /* strObjName */ m_strObjName,
        /* scaleDir   */ m_scaleDir,
        /* scale      */ m_scale );

    if( m_pZoomStackFirst != nullptr )
    {
        pDiagScale->m_scale = m_pZoomStackFirst->m_scale;
    }

    pDiagScale->m_spacing = m_spacing;
    //pDiagScale->m_fScaleRes = m_fScaleRes;
    //pDiagScale->m_bScaleResCalculated = m_bScaleResCalculated;
    pDiagScale->m_iScaleMinValPix = m_iScaleMinValPix;
    pDiagScale->m_iScaleMaxValPix = m_iScaleMaxValPix;
    //pDiagScale->m_fPixRes = m_fPixRes;

    for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
    {
        pDiagScale->m_ariDivLineDistMinPix[iLayer] = m_ariDivLineDistMinPix[iLayer];
    }

    //pDiagScale->m_uMainDivLineCount = m_uMainDivLineCount;
    //if( pDiagScale->m_uMainDivLineCount > 0 )
    //{
    //    pDiagScale->m_arfMainDivLineVal = new double[pDiagScale->m_uMainDivLineCount];
    //    pDiagScale->m_arfMainDivLinePix = new double[pDiagScale->m_uMainDivLineCount];

    //    for( idxDivLine = 0; idxDivLine < pDiagScale->m_uMainDivLineCount; idxDivLine++ )
    //    {
    //        pDiagScale->m_arfMainDivLineVal[idxDivLine] = m_arfMainDivLineVal[idxDivLine];
    //        pDiagScale->m_arfMainDivLinePix[idxDivLine] = m_arfMainDivLinePix[idxDivLine];
    //    }
    //}

    //pDiagScale->m_uSubDivLineCount = m_uSubDivLineCount;
    //if( pDiagScale->m_uSubDivLineCount > 0 )
    //{
    //    pDiagScale->m_arfSubDivLineVal = new double[pDiagScale->m_uSubDivLineCount];
    //    pDiagScale->m_arfSubDivLinePix = new double[pDiagScale->m_uSubDivLineCount];

    //    for( idxDivLine = 0; idxDivLine < pDiagScale->m_uSubDivLineCount; idxDivLine++ )
    //    {
    //        pDiagScale->m_arfSubDivLineVal[idxDivLine] = m_arfSubDivLineVal[idxDivLine];
    //        pDiagScale->m_arfSubDivLinePix[idxDivLine] = m_arfSubDivLinePix[idxDivLine];
    //    }
    //}

    i_pDiagramTrg->addDiagScale(pDiagScale);

    return pDiagScale;

} // clone

/*==============================================================================
protected: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
CDiagScale::SZoomStackEntry::SZoomStackEntry( const SScale& i_scale ) :
//------------------------------------------------------------------------------
    m_scale(i_scale),
    m_pStackEntryNext(nullptr),
    m_pStackEntryPrev(nullptr)
{
} // ctor

/*==============================================================================
protected:  // instance memthods
==============================================================================*/

//------------------------------------------------------------------------------
void CDiagScale::pushScale( const SScale& i_scale )
//------------------------------------------------------------------------------
{
    SZoomStackEntry* pEntry = new SZoomStackEntry(i_scale);

    // First object ..
    if( m_pZoomStackFirst == nullptr )
    {
        pEntry->m_pStackEntryPrev = nullptr;
        pEntry->m_pStackEntryNext = nullptr;

        m_pZoomStackFirst = pEntry;
        m_pZoomStackLast  = pEntry;
    }
    // Not the first object ...
    else if( m_pZoomStackLast != nullptr )
    {
        m_pZoomStackLast->m_pStackEntryNext = pEntry;

        pEntry->m_pStackEntryPrev = m_pZoomStackLast;
        pEntry->m_pStackEntryNext = nullptr;

        m_pZoomStackLast = pEntry;
    }
    m_iZoomCount++;

} // pushScale

//------------------------------------------------------------------------------
SScale CDiagScale::popScale()
//------------------------------------------------------------------------------
{
    if( m_iZoomCount == 0 || m_pZoomStackFirst == nullptr || m_pZoomStackLast == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultListIsEmpty);
    }

    SZoomStackEntry* pEntry = m_pZoomStackLast;
    SScale           scale = pEntry->m_scale;

    // Just one object on stack ...
    if( m_pZoomStackFirst == m_pZoomStackLast )
    {
        m_pZoomStackFirst = nullptr;
        m_pZoomStackLast  = nullptr;
    }
    // Not the first object ...
    else
    {
        pEntry->m_pStackEntryPrev->m_pStackEntryNext = nullptr;
        m_pZoomStackLast = pEntry->m_pStackEntryPrev;
    }
    delete pEntry;
    pEntry = nullptr;
    m_iZoomCount--;

    return scale;

} // popScale

//------------------------------------------------------------------------------
SScale CDiagScale::clearScaleStack()
//------------------------------------------------------------------------------
{
    if( m_iZoomCount == 0 || m_pZoomStackFirst == nullptr || m_pZoomStackLast == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultListIsEmpty);
    }

    SZoomStackEntry* pEntry     = m_pZoomStackFirst;
    SZoomStackEntry* pEntryPrev = nullptr;
    SScale           scale = pEntry->m_scale;

    pEntry = m_pZoomStackLast;
    while( pEntry != nullptr )
    {
        pEntryPrev = pEntry->m_pStackEntryPrev;
        delete pEntry;
        pEntry = nullptr;
        pEntry = pEntryPrev;
    }
    m_pZoomStackFirst = nullptr;
    m_pZoomStackLast  = nullptr;
    m_iZoomCount = 0;

    return scale;

} // clearScaleStack
