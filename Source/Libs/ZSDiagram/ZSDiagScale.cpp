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

#include "ZSDiagram/ZSDiagScale.h"
#include "ZSDiagram/ZSDiagramProcData.h"
#include "ZSSys/ZSSysAux.h"
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
    m_spacing(ESpacing::Linear),
    m_scale(i_scale),
    m_iMinVal_px(0),
    m_iMaxVal_px(0),
    m_ariDivLineDistMin_px(CEnumDivLineLayer::count(), 0),
    m_ariDivLineCount(CEnumDivLineLayer::count(), 0),
    m_fDivDistMinVal(0.0),
    m_ararfDivLineVal(CEnumDivLineLayer::count(), QVector<double>()),
    m_ararfDivLine_px(CEnumDivLineLayer::count(), QVector<double>()),
    m_bDivLinesCalculated(false),
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

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_strObjName;
        strMthInArgs += ", ScaleDir: " + CEnumScaleDir(i_scaleDir).toString();
        strMthInArgs += ", Scale: " + i_scale.toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    m_ariDivLineDistMin_px[static_cast<int>(EDivLineLayer::Main)] = 50;

    if( !s_bClassInitialised )
    {
        double fFacLin = 2.0;

        // In logarithmic scale each decade may be divided into 9 sections indicating
        // the values 1.0*10^x, 2.0*10^x, .... , 9.0*10^x, 10.0*10^x with x as an
        // signed integer value. The width of the sections is decreasing from beginning
        // of the first value of the decade to the last value of the decade (non
        // equidistant grid). The portion of each sector is the same for all visible decades.
        for( int idxSection = 0; idxSection < 9; idxSection++, fFacLin += 1.0 )
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

    //m_strObjName;
    m_pDiagram = nullptr;
    m_scaleDir = static_cast<EScaleDir>(0);
    m_spacing = static_cast<ESpacing>(0);
    //m_scale;
    m_iMinVal_px = 0;
    m_iMaxVal_px = 0;
    //m_ariDivLineDistMin_px.clear();
    //m_ariDivLineCount.clear();
    m_fDivDistMinVal = 0.0;
    //m_ararfDivLineVal.clear();
    //m_ararfDivLine_px.clear();
    m_bDivLinesCalculated = false;
    m_iZoomCount = 0;
    m_pZoomStackFirst = nullptr; // passierte zwar schon in clearScaleStack, weiss aber lint nicht
    m_pZoomStackLast = nullptr;  // passierte zwar schon in clearScaleStack, weiss aber lint nicht

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
void CDiagScale::setSpacing( const CEnumSpacing& i_spacing )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::EnterLeave)) {
        strMthInArgs = i_spacing.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setSpacing",
        /* strAddInfo   */ strMthInArgs );

    if (m_spacing != i_spacing.enumerator())
    {
        m_spacing = i_spacing.enumerator();

        m_bDivLinesCalculated = false;

        if (m_pDiagram != nullptr)
        {
            m_pDiagram->scaleChanged(this);
        }
        emit_spacingChanged(m_spacing);
    }
}

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
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::EnterLeave)) {
        strMthInArgs = i_scale.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setScale",
        /* strAddInfo   */ strMthInArgs );

    if (m_scale != i_scale) //lint !e1702
    {
        m_scale = i_scale;

        m_bDivLinesCalculated = false;

        if (m_pDiagram != nullptr)
        {
            m_pDiagram->scaleChanged(this);
        }
        emit_scaleChanged(m_scale);
    }
}

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

    if( getRangePix() < 0 )
    {
        bScaleValid = false;
    }
    else if( m_scale.m_fMax <= m_scale.m_fMin )
    {
        bScaleValid = false;
    }
    else if( m_spacing == ESpacing::Logarithmic )
    {
        if( m_scale.m_fMin <= 0.0 || m_scale.m_fMax <= 0.0 )
        {
            bScaleValid = false;
        }
    }
    return bScaleValid;
}

//------------------------------------------------------------------------------
void CDiagScale::setGeometry( int i_iMinVal_px, int i_iMaxVal_px )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjLayout, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iMinVal_px) + ", " + QString::number(i_iMaxVal_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDatailLevel */ EMethodTraceDetailLevel::ArgsNormal,
        /* strMethod    */ "setGeometry",
        /* strAddInfo   */ strMthInArgs );

    if ((m_iMinVal_px != i_iMinVal_px) || (m_iMaxVal_px != i_iMaxVal_px))
    {
        m_iMinVal_px = i_iMinVal_px;
        m_iMaxVal_px = i_iMaxVal_px;

        m_bDivLinesCalculated = false;

        if (m_pDiagram != nullptr)
        {
            m_pDiagram->scaleChanged(this);
        }
        emit_geometryChanged(m_iMinVal_px, m_iMaxVal_px);
    }
}

//------------------------------------------------------------------------------
int CDiagScale::getMinValPix() const
//------------------------------------------------------------------------------
{
    return m_iMinVal_px;
}

//------------------------------------------------------------------------------
int CDiagScale::getMaxValPix() const
//------------------------------------------------------------------------------
{
    return m_iMaxVal_px;
}

//------------------------------------------------------------------------------
int CDiagScale::getRangePix() const
//------------------------------------------------------------------------------
{
    int iScaleRangePix = 0;

    if( m_iMaxVal_px > m_iMinVal_px )
    {
        iScaleRangePix = m_iMaxVal_px - m_iMinVal_px + 1; //lint !e834
    }
    else if( m_iMinVal_px > m_iMaxVal_px )
    {
        iScaleRangePix = m_iMinVal_px - m_iMaxVal_px + 1; //lint !e834
    }
    return iScaleRangePix;
}

//------------------------------------------------------------------------------
void CDiagScale::setDivLineDistMinPix( const CEnumDivLineLayer& i_eLayer, int i_iDistMinPix )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::EnterLeave))
    {
        strTrcMsg = "Layer: " + i_eLayer.toString();
        strTrcMsg += ", DistMinPix: " + QString::number(i_iDistMinPix);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setDivLineDistMinPix",
        /* strAddInfo   */ strTrcMsg );

    if( m_ariDivLineDistMin_px[i_eLayer.enumeratorAsInt()] != i_iDistMinPix )
    {
        m_ariDivLineDistMin_px[i_eLayer.enumeratorAsInt()] = i_iDistMinPix;

        m_bDivLinesCalculated = false;

        if( m_pDiagram != nullptr )
        {
            m_pDiagram->scaleChanged(this);
        }
        emit_divLineDistMinPixChanged(i_eLayer.enumerator(), i_iDistMinPix);
    }
}

//------------------------------------------------------------------------------
int CDiagScale::getDivLineDistMinPix( const CEnumDivLineLayer& i_eLayer ) const
//------------------------------------------------------------------------------
{
    return m_ariDivLineDistMin_px[i_eLayer.enumeratorAsInt()];
}

//------------------------------------------------------------------------------
bool CDiagScale::areDivLinesCalculated() const
//------------------------------------------------------------------------------
{
    return m_bDivLinesCalculated;
}

//------------------------------------------------------------------------------
/*! @brief Returns the resolution of the scale.

    The scale resolution depends on the one hand on the resolution per pixel
    (pixel range / value range) and on the other hand the minumum and maximum value.
    The absolute lowest value determines the scale resolution.

    The resolution is rounded to the next lower decade.

    @note The division line distance don't need to be taken into account
          as the distance is always greater than one pixel and therefore
          greater than (pixel range / value range).

    @param i_pUnit [in]
        Unit in which the resolution should be returned.
        If nullptr the resolution is returned in the unit defined for the scale.

    @return Resolution.
*/
double CDiagScale::getScaleRes( const CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    CUnit unit = m_scale.m_unit;

    if (i_pUnit != nullptr)
    {
        unit = *i_pUnit;
    }
    if (!areOfSameUnitGroup(m_scale.m_unit,unit))
    {
        throw CException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }

    double fScaleRes = 0.1;

    if (isScaleValid())
    {
        double fScaleRangePix = getRangePix();
        double fScaleMin = m_scale.m_unit.convertValue(m_scale.m_fMin, unit);
        double fScaleMax = m_scale.m_unit.convertValue(m_scale.m_fMax, unit);

        if (m_spacing == ESpacing::Linear)
        {
            double fScaleRangeVal = fScaleMax - fScaleMin;
            if (fScaleRangePix > 1.0) {
                fScaleRes = fScaleRangeVal / (fScaleRangePix-1.0);
            }
            else {
                fScaleRes = fScaleRangeVal / 1000.0;
            }
        }
        else
        {
            double fScaleMinLog = log10(fScaleMin);
            double fScaleMaxLog = log10(fScaleMax);
            double fScaleRangeValLog = fScaleMaxLog - fScaleMinLog;
            if (fScaleRangePix > 1.0) {
                fScaleRes = fScaleRangeValLog / (fScaleRangePix-1.0);
            }
            else {
                fScaleRes = fScaleRangeValLog / 1000.0;
            }
        }
        if (fScaleMin != 0.0 && fScaleRes > fabs(fScaleMin)) {
            fScaleRes = fabs(fScaleMin);
        }
        if (fScaleMax != 0.0 && fScaleRes > fabs(fScaleMax)) {
            fScaleRes = fabs(fScaleMax);
        }
    }
    return Math::round2LowerDecade(fScaleRes);
}

//------------------------------------------------------------------------------
/*! @brief Returns the resolution of the scale.

    The scale resolution depends on the one hand on the resolution per pixel
    (pixel range / value range) and on the other hand the minumum and maximum value.
    The absolute lowest value determines the scale resolution.

    The resolution is rounded to the next lower decade.

    @note The division line distance don't need to be taken into account
          as the distance is always greater than one pixel and therefore
          greater than (pixel range / value range).

    @note In linear spacing the resolution is constant through the whole axis range.
          In logarithmic spacing the resolution is not constant but decreases to higher values.

    @param i_fVal [in]
    @param i_pUnit [in]
        Unit in which the resolution should be returned.
        If nullptr the resolution is returned in the unit defined for the scale.

    @return Resolution.
*/
double CDiagScale::getScaleRes( double i_fVal, const CUnit* i_pUnit ) const
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
    double fScaleRes = 0.1;

    if (isScaleValid())
    {
        double fScaleRangePix = getRangePix();
        double fScaleMin = m_scale.m_unit.convertValue(m_scale.m_fMin, unit);
        double fScaleMax = m_scale.m_unit.convertValue(m_scale.m_fMax, unit);

        if( m_spacing == ESpacing::Linear )
        {
            double fScaleRangeVal = fScaleMax - fScaleMin;
            if (fScaleRangePix > 1.0) {
                fScaleRes = fScaleRangeVal / (fScaleRangePix-1.0);
            }
            else {
                fScaleRes = fScaleRangeVal / 1000.0;
            }
        }
        else
        {
            if( unit != m_scale.m_unit ) {
                fVal = unit.convertValue(fVal, m_scale.m_unit);
            }
            fScaleRes = Math::logRes2LinRes(fScaleRes, fVal);
        }
        if (fScaleMin != 0.0 && fScaleRes > fabs(fScaleMin)) {
            fScaleRes = fabs(fScaleMin);
        }
        if (fScaleMax != 0.0 && fScaleRes > fabs(fScaleMax)) {
            fScaleRes = fabs(fScaleMax);
        }
    }
    return Math::round2LowerDecade(fScaleRes);
}

//------------------------------------------------------------------------------
double CDiagScale::round2ScaleRes( double i_fVal, const CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    double fRes = getScaleRes(i_fVal, i_pUnit);
    double fVal = Math::round2Resolution(i_fVal, fRes);
    return fVal;
}

//------------------------------------------------------------------------------
int CDiagScale::getDivLineCount( const CEnumDivLineLayer& i_eLayer ) const
//------------------------------------------------------------------------------
{
    int iDivLineCount = 0;

    if( getRangePix() > 1 && isScaleValid() && areDivLinesCalculated() )
    {
        iDivLineCount = m_ariDivLineCount[i_eLayer.enumeratorAsInt()];
    }
    return iDivLineCount;
}

//------------------------------------------------------------------------------
double CDiagScale::getDivLineDistMin( const CEnumDivLineLayer& i_eLayer, const PhysVal::CUnit* i_pUnit ) const
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

    double fDivLineDistMin = m_scale.physValRange().getVal();

    int idxLayerMin = 0;
    int idxLayerMax = CEnumDivLineLayer::count()-1;

    if (i_eLayer.isValid())
    {
        idxLayerMin = i_eLayer.enumeratorAsInt();
        idxLayerMax = i_eLayer.enumeratorAsInt();
    }
    for (int idxLayer = idxLayerMin; idxLayer <= idxLayerMax; ++idxLayer)
    {
        for (int idxDivLine = 1; idxDivLine < m_ariDivLineCount[idxLayer]; ++idxDivLine)
        {
            double fDivLineValPrev = m_ararfDivLineVal[idxLayer][idxDivLine-1];
            double fDivLineValCurr = m_ararfDivLineVal[idxLayer][idxDivLine];
            if ((fDivLineValCurr - fDivLineValPrev) < fDivLineDistMin)
            {
                fDivLineDistMin = fDivLineValCurr - fDivLineValPrev;
                if (m_spacing == ESpacing::Linear)
                {
                    break;
                }
            }
        }
    }
    return m_scale.m_unit.convertValue(fDivLineDistMin, unit);
}

//------------------------------------------------------------------------------
double CDiagScale::getDivLineVal( const CEnumDivLineLayer& i_eLayer, int i_idxDivLine, const CUnit* i_pUnit ) const
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

    if( getRangePix() > 1 && isScaleValid() && areDivLinesCalculated() )
    {
        if( i_idxDivLine < m_ariDivLineCount[i_eLayer.enumeratorAsInt()] )
        {
            fDivLineVal = m_ararfDivLineVal[i_eLayer.enumeratorAsInt()][i_idxDivLine];
        }
        if( unit != m_scale.m_unit )
        {
            fDivLineVal = m_scale.m_unit.convertValue(fDivLineVal, unit);
        }
    }
    return fDivLineVal;
}

//------------------------------------------------------------------------------
double CDiagScale::getDivLinePix( const CEnumDivLineLayer& i_eLayer, int i_idxDivLine ) const
//------------------------------------------------------------------------------
{
    double fDivLine_px = 0.0;

    if( getRangePix() > 1 && isScaleValid() && areDivLinesCalculated() )
    {
        if( i_idxDivLine < m_ariDivLineCount[i_eLayer.enumeratorAsInt()] )
        {
            fDivLine_px = m_ararfDivLine_px[i_eLayer.enumeratorAsInt()][i_idxDivLine];
        }
    }
    return fDivLine_px;
}

//------------------------------------------------------------------------------
double CDiagScale::getDivLineDistPix( const CEnumDivLineLayer& i_eLayer, int i_idxDivLine1, int i_idxDivLine2 ) const
//------------------------------------------------------------------------------
{
    double fDivLineDistPix = 0.0;

    if( i_idxDivLine1 < m_ariDivLineCount[i_eLayer.enumeratorAsInt()]
     && i_idxDivLine2 < m_ariDivLineCount[i_eLayer.enumeratorAsInt()] )
    {
        fDivLineDistPix = m_ararfDivLine_px[i_eLayer.enumeratorAsInt()][i_idxDivLine2]
                        - m_ararfDivLine_px[i_eLayer.enumeratorAsInt()][i_idxDivLine1];
    }
    return fDivLineDistPix;
}

//------------------------------------------------------------------------------
int CDiagScale::getValPix( double i_fVal, const CUnit* i_pUnit ) const
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

    double fPix = m_iMinVal_px;

    if( getRangePix() > 1 && isScaleValid() )
    {
        double fVal = i_fVal;
        double fScalePixMin   = static_cast<double>(m_iMinVal_px);
        double fScalePixMax   = static_cast<double>(m_iMaxVal_px);
        double fScalePixRange  = static_cast<double>(getRangePix());
        double fScaleValMin   = m_scale.m_fMin;
        double fScaleValMax   = m_scale.m_fMax;
        double fScaleValRange = fabs(fScaleValMax-fScaleValMin);
        double fPixRes;
        bool   bDivLineHit = false;
        double fDivLineVal1;
        double fDivLineVal2;
        int    idxDivLine;

        // Calculate range and resolution of one pixel:
        if( m_spacing == ESpacing::Logarithmic )
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
            fPix = m_iMinVal_px;
        }
        // At the maximum scale value ..
        else if( fVal == m_scale.m_fMax )
        {
            fPix = m_iMaxVal_px;
        }

        // Somewhere between minimum and maximum scale ..
        else
        {
            // Get the nearest division line.
            for( int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
            {
                if( m_ariDivLineCount[iLayer] > 1 )
                {
                    for( idxDivLine = 0; idxDivLine < m_ariDivLineCount[iLayer]-1; idxDivLine++ )
                    {
                        fDivLineVal1 = m_ararfDivLineVal[iLayer][idxDivLine];
                        fDivLineVal2 = m_ararfDivLineVal[iLayer][idxDivLine+1];

                        if( fVal >= (fDivLineVal1-DBL_EPSILON) && fVal <= (fDivLineVal1+DBL_EPSILON) )
                        {
                            fPix = static_cast<int>(m_ararfDivLine_px[iLayer][idxDivLine]);
                            bDivLineHit = true;
                            break;
                        }
                        else if( fVal >= (fDivLineVal2-DBL_EPSILON) && fVal <= (fDivLineVal2+DBL_EPSILON) )
                        {
                            fPix = static_cast<int>(m_ararfDivLine_px[iLayer][idxDivLine+1]);
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
                        fPix = static_cast<int>(m_ararfDivLine_px[iLayer][0]);
                        bDivLineHit = true;
                    }
                }
                if( bDivLineHit )
                {
                    break;
                }
            } // for( iLayer < CEnumDivLineLayer::count() )

            // Did not hit a division line ..
            if( !bDivLineHit )
            {
                if( m_spacing == ESpacing::Logarithmic )
                {
                    fVal = log10(fVal);
                }

                // Calculate value as distance to left grid line:
                switch( m_scaleDir )
                {
                    case EScaleDir::X:
                    {
                        fPix = fScalePixMin + fPixRes*(fVal-fScaleValMin); //lint !e834
                        break;
                    }
                    case EScaleDir::Y:
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
    double i_fVal, const CUnit* i_pUnit,
    int i_iDigitsCountMax, bool i_bUseEngineeringFormat ) const
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
double CDiagScale::getVal( double i_fPix, const CUnit* i_pUnit ) const
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

    if( getRangePix() <= 1 )
    {
        return m_scale.m_fMin;
    }
    if( isScaleValid() )
    {
        double fScalePixMin   = static_cast<double>(m_iMinVal_px);
        double fScalePixMax   = static_cast<double>(m_iMaxVal_px);
        double fScalePixRange = static_cast<double>(getRangePix());
        double fScaleValMin   = m_scale.m_fMin;
        double fScaleValMax   = m_scale.m_fMax;
        double fScaleValRange = fabs(fScaleValMax-fScaleValMin);
        double fPixRes;
        bool   bDivLineHit = false;
        double fDivLinePix1;
        double fDivLinePix2;
        int    idxDivLine;

        // Calculate range and resolution of one pixel:
        if( m_spacing == ESpacing::Logarithmic )
        {
            fScaleValMin = log10(fScaleValMin);
            fScaleValMax = log10(fScaleValMax);
        }
        fScaleValRange = fabs(fScaleValMax-fScaleValMin);
        fScalePixRange = fabs(fScalePixMax-fScalePixMin)+1;
        fPixRes        = fScaleValRange / fScalePixRange;

        // At the minimum scale value ..
        if( i_fPix == m_iMinVal_px )
        {
            fVal = m_scale.m_fMin;
        }
        // At the maximum scale value ..
        else if( i_fPix == m_iMaxVal_px )
        {
            fVal = m_scale.m_fMax;
        }

        // Somewhere between minimum and maximum scale ..
        else
        {
            // Get the nearest division line. This is necessary to exactly return the value
            // at a grid line if the mouse cursor is positioned on a grid line.
            for( int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
            {
                if( m_ariDivLineCount[iLayer] > 1 )
                {
                    for( idxDivLine = 0; idxDivLine < m_ariDivLineCount[iLayer]-1; idxDivLine++ )
                    {
                        fDivLinePix1 = m_ararfDivLine_px[iLayer][idxDivLine];
                        fDivLinePix2 = m_ararfDivLine_px[iLayer][idxDivLine+1];

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
                    fDivLinePix1 = m_ararfDivLine_px[iLayer][0];

                    if( i_fPix == fDivLinePix1 )
                    {
                        fVal = static_cast<int>(m_ararfDivLineVal[iLayer][0]);
                        bDivLineHit = true;
                    }
                }
                if( bDivLineHit )
                {
                    break;
                }
            } // for( iLayer < CEnumDivLineLayer::count() )

            // Did not hit a division line ..
            if( !bDivLineHit )
            {
                // Calculate value as distance to left grid line:
                switch( m_scaleDir )
                {
                    case EScaleDir::X:
                    {
                        fVal = fScaleValMin + fPixRes*(i_fPix-fScalePixMin+1); //lint !e834
                        break;
                    }
                    case EScaleDir::Y:
                    {
                        fVal = fScaleValMin + fPixRes*(fScalePixMin-i_fPix+1); //lint !e834
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
                if( m_spacing == ESpacing::Logarithmic )
                {
                    fVal = pow(10.0,fVal);
                }
            } // Neither hit a main nor a sub division line ..

        } // Somewhere between minimum and maximum scale ..

        if( m_spacing == ESpacing::Linear )
        {
            fPixRes = Math::round2LowerDecade(fPixRes);
        }
        else
        {
            fPixRes = getScaleRes(fVal);
        }
        fVal = Math::round2Resolution(fVal, fPixRes);

    } // if( isScaleValid() )

    if( unit != m_scale.m_unit )
    {
        fVal = m_scale.m_unit.convertValue(fVal, unit);
    }
    return fVal;

} // getVal

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDiagScale::zoomIn( int i_iZoomRectMinValPix, int i_iZoomRectMaxValPix )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iZoomRectMinValPix) + ", " + QString::number(i_iZoomRectMaxValPix);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "zoomIn",
        /* strAddInfo   */ strMthInArgs );

    SScale scaleZoom = m_scale;
    scaleZoom.m_fMin = getVal(static_cast<double>(i_iZoomRectMinValPix));
    scaleZoom.m_fMax = getVal(static_cast<double>(i_iZoomRectMaxValPix));
    pushScale(m_scale);
    setScale(scaleZoom); // sets the flag calculated to false and calls "scaleChanged" of diagram
}

//------------------------------------------------------------------------------
void CDiagScale::zoomIn( double i_fMinVal, double i_fMaxVal, CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fMinVal) + ", " + QString::number(i_fMaxVal);
        if (i_pUnit != nullptr ) strMthInArgs += ", " + i_pUnit->symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "zoomIn",
        /* strAddInfo   */ strMthInArgs );

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
}

//------------------------------------------------------------------------------
void CDiagScale::zoomOut( bool i_bComplete )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Complete: " + bool2Str(i_bComplete);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "zoomOut",
        /* strAddInfo   */ strMthInArgs );

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
}

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
        QString strMthRuntimeInfo =
            "ScaleIsValid: " + bool2Str(isScaleValid()) +
            ", DivLinesCalculated: " + bool2Str(m_bDivLinesCalculated);
    }

    // Division lines:
    //----------------

    if( !m_bDivLinesCalculated )
    {
        for( int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
        {
            m_ariDivLineCount[iLayer] = 0;
            m_ararfDivLineVal[iLayer].clear();
            m_ararfDivLine_px[iLayer].clear();
        }
        m_fDivDistMinVal = 0.0;

        int iScaleRangePix = getRangePix();

        if( iScaleRangePix <= 1 || m_ariDivLineDistMin_px[static_cast<int>(EDivLineLayer::Main)] < 2 )
        {
        }
        else if( !isScaleValid() )
        {
            m_ariDivLineCount[static_cast<int>(EDivLineLayer::Main)] = 2;
            m_ararfDivLineVal[static_cast<int>(EDivLineLayer::Main)].resize(
                m_ariDivLineCount[static_cast<int>(EDivLineLayer::Main)]);
            m_ararfDivLine_px[static_cast<int>(EDivLineLayer::Main)].resize(
                m_ariDivLineCount[static_cast<int>(EDivLineLayer::Main)]);
            m_ararfDivLineVal[static_cast<int>(EDivLineLayer::Main)][0] = m_scale.m_fMin;
            m_ararfDivLineVal[static_cast<int>(EDivLineLayer::Main)][1] = m_scale.m_fMax;
            m_ararfDivLine_px[static_cast<int>(EDivLineLayer::Main)][0] = m_iMinVal_px;
            m_ararfDivLine_px[static_cast<int>(EDivLineLayer::Main)][1] = m_iMaxVal_px;
        }

        // Calculate main and sub division lines
        //--------------------------------------

        else
        {
            // Calculate division lines for linear spacing
            //--------------------------------------------

            QVector<int> ariDivLineCount(CEnumDivLineLayer::count());

            for( int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
            {
                ariDivLineCount[iLayer] = 0;
            }

            m_fDivDistMinVal = m_scale.physValRange().getVal();

            if( m_spacing == ESpacing::Linear )
            {
                double fDivLineDistValMin = m_scale.m_fMin;
                double fDivLineDistValMax = m_scale.m_fMax;
                QVector<int> ariDivLineCountTmp(CEnumDivLineLayer::count());
                QVector<double> arfDivLineFirstVal(CEnumDivLineLayer::count());
                QVector<double> arfDivLineDistFirstPix(CEnumDivLineLayer::count());
                QVector<double> arfDivLineDistVal(CEnumDivLineLayer::count());
                QVector<double> arfDivLineDistPix(CEnumDivLineLayer::count());
                double fPrevLayerDivLineVal;
                double fCurrLayerDivLineVal;
                int    idxDivLine;
                int    idxDivLineTmp;

                for( int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
                {
                    if (m_ariDivLineDistMin_px[iLayer] > 0)
                    {
                        // Calculate optimized distance between two grid lines:
                        ariDivLineCountTmp[iLayer] = Math::calculateDivLines4LinSpacing(
                            /* fScaleMinVal          */ fDivLineDistValMin,
                            /* fScaleMaxVal          */ fDivLineDistValMax,
                            /* iScaleRangePix        */ iScaleRangePix,
                            /* fDivLineDistMinVal    */ 0.0,
                            /* iDivLineDistMinPix    */ m_ariDivLineDistMin_px[iLayer],
                            /* bUseDivLineDecFac25   */ true,
                            /* pfDivLineFirstVal     */ &arfDivLineFirstVal[iLayer],
                            /* pfDivLineDistFirstPix */ &arfDivLineDistFirstPix[iLayer],
                            /* pfDivLineDistVal      */ &arfDivLineDistVal[iLayer],
                            /* pfDivLineDistPix      */ &arfDivLineDistPix[iLayer],
                            /* pTrcAdminObj          */ m_pTrcAdminObjUpdate);

                        if (m_fDivDistMinVal > arfDivLineDistVal[iLayer] )
                        {
                            m_fDivDistMinVal = arfDivLineDistVal[iLayer];
                        }

                        // Store the calculated results:
                        if( ariDivLineCountTmp[iLayer] > 0 )
                        {
                            if( iLayer == 0 )
                            {
                                // Count visible division lines for the main grid lines:
                                ariDivLineCount[iLayer] = ariDivLineCountTmp[iLayer];

                                m_ararfDivLineVal[iLayer].resize(ariDivLineCount[iLayer]);
                                m_ararfDivLine_px[iLayer].resize(ariDivLineCount[iLayer]);

                                // Calculate pixel values and store the calculated results of the main grid lines:
                                for( idxDivLine = 0; idxDivLine < ariDivLineCount[iLayer]; idxDivLine++ )
                                {
                                    double fVal = arfDivLineFirstVal[iLayer] + idxDivLine*arfDivLineDistVal[iLayer];
                                    if( fabs(fVal) < DBL_EPSILON )
                                    {
                                        fVal = 0.0;
                                    }
                                    m_ararfDivLineVal[iLayer][idxDivLine] = fVal;
                                    m_ararfDivLine_px[iLayer][idxDivLine] = getValPix(fVal);
                                }
                            }
                            else // if (iLayer != 0)
                            {
                                // Count visible sub division lines:
                                modf(m_scale.m_fMin/arfDivLineDistVal[iLayer-1], &fPrevLayerDivLineVal);
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
                                    m_ararfDivLineVal[iLayer].resize(ariDivLineCount[iLayer]);
                                    m_ararfDivLine_px[iLayer].resize(ariDivLineCount[iLayer]);

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
                                                    m_ararfDivLine_px[iLayer][idxDivLine] = getValPix(fCurrLayerDivLineVal);
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
                        if( iLayer < CEnumDivLineLayer::count()-1 )
                        {
                            if( m_ariDivLineDistMin_px[iLayer+1] > 1 && m_ariDivLineDistMin_px[iLayer] > 2*m_ariDivLineDistMin_px[iLayer+1] )
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
                    } // if (m_ariDivLineDistMin_px[iLayer] > 0)
                } // for( iLayer < CEnumDivLineLayer::count() )
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
                QVector<int> ariDivLineCountMax(CEnumDivLineLayer::count());
                QVector<int> ariDivLineDistMinPix(CEnumDivLineLayer::count());
                QVector<double> arfDivLineFirstValLog(CEnumDivLineLayer::count());
                QVector<double> arfDivLineDistFirstPix(CEnumDivLineLayer::count());
                QVector<double> arfDivLineDistValLog(CEnumDivLineLayer::count());
                QVector<double> arfDivLineDistPix(CEnumDivLineLayer::count());
                QVector<double> arfDivLineValLin(CEnumDivLineLayer::count());
                QVector<double> arfDivLineValLog(CEnumDivLineLayer::count());
                int    idxDivLine;
                int    idxDivLineTmp;
                QVector<int> ariDivLineCountTmp(CEnumDivLineLayer::count());
                int    iLayerMain = static_cast<int>(EDivLineLayer::Main);
                int    iLayerSub = static_cast<int>(EDivLineLayer::Sub);
                double fFacLin;
                int    idxSection;

                for( int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
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
                //fScaleMinPixExt = m_iMinVal_px - (fScaleMinValLog-fScaleMinValLogExt) * (iScaleRangePix/fScaleRangeValLog);

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
                    if( ariDivLineDistMinPix[iLayerMain] < m_ariDivLineDistMin_px[iLayerMain] )
                    {
                        ariDivLineDistMinPix[iLayerMain] = m_ariDivLineDistMin_px[iLayerMain];
                    }
                    // If the distance of 1.0 between the main grid lines is too big ..
                    else if( ariDivLineDistMinPix[iLayerMain] > iScaleRangePix / 2 )
                    {
                        ariDivLineDistMinPix[iLayerMain] = iScaleRangePix / 2;
                    }

                    // Calculate optimized distance between two main grid lines:
                    ariDivLineCount[iLayerMain] = Math::calculateDivLines4LinSpacing(
                        /* fScaleMinVal          */ fScaleMinValLog,
                        /* fScaleMaxVal          */ fScaleMaxValLog,
                        /* iScaleRangePix        */ iScaleRangePix,
                        /* fDivLineDistMinVal    */ 1.0,
                        /* iDivLineDistMinPix    */ ariDivLineDistMinPix[iLayerMain],
                        /* bUseDivLineDecFac25   */ false,
                        /* pfDivLineFirstVal     */ &arfDivLineFirstValLog[iLayerMain],
                        /* pfDivLineDistFirstPix */ &arfDivLineDistFirstPix[iLayerMain],
                        /* pfDivLineDistVal      */ &arfDivLineDistValLog[iLayerMain],
                        /* pfDivLineDistPix      */ &arfDivLineDistPix[iLayerMain],
                        /* pTrcAdminObj          */ m_pTrcAdminObjUpdate);
                }

                // Store the calculated results:
                if( ariDivLineCount[iLayerMain] > 0 )
                {
                    m_ararfDivLineVal[iLayerMain].resize(ariDivLineCount[iLayerMain]);
                    m_ararfDivLine_px[iLayerMain].resize(ariDivLineCount[iLayerMain]);

                    for( idxDivLine = 0; idxDivLine < ariDivLineCount[iLayerMain]; idxDivLine++ )
                    {
                        arfDivLineValLog[iLayerMain] = arfDivLineFirstValLog[iLayerMain] + idxDivLine*arfDivLineDistValLog[iLayerMain];
                        arfDivLineValLin[iLayerMain] = pow(10.0,arfDivLineValLog[iLayerMain]);
                        m_ararfDivLineVal[iLayerMain][idxDivLine] = arfDivLineValLin[iLayerMain];
                        m_ararfDivLine_px[iLayerMain][idxDivLine] = getValPix(arfDivLineValLin[iLayerMain]);
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
                    if( m_ariDivLineDistMin_px[iLayerSub] > 1 && m_ariDivLineDistMin_px[iLayerMain] > 2*m_ariDivLineDistMin_px[iLayerSub] )
                    {
                        ariDivLineCountTmp[iLayerSub] = Math::calculateDivLines4LinSpacing(
                            /* fScaleMinVal          */ arfDivLineFirstValLog[iLayerMain],
                            /* fScaleMaxVal          */ arfDivLineFirstValLog[iLayerMain]+arfDivLineDistValLog[iLayerMain],
                            /* iScaleRangePix        */ static_cast<int>(arfDivLineDistPix[iLayerMain]),
                            /* fDivLineDistMinVal    */ 1.0,
                            /* iDivLineDistMinPix    */ m_ariDivLineDistMin_px[iLayerSub],
                            /* bUseDivLineDecFac25   */ false,
                            /* pfDivLineFirstVal     */ &arfDivLineFirstValLog[iLayerSub],
                            /* pfDivLineDistFirstPix */ &arfDivLineDistFirstPix[iLayerSub],
                            /* pfDivLineDistVal      */ &arfDivLineDistValLog[iLayerSub],
                            /* pfDivLineDistPix      */ nullptr,
                            /* pTrcAdminObj          */ m_pTrcAdminObjUpdate);

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
                                m_ararfDivLineVal[iLayerSub].resize(ariDivLineCount[iLayerSub]);
                                m_ararfDivLine_px[iLayerSub].resize(ariDivLineCount[iLayerSub]);

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
                                                m_ararfDivLine_px[iLayerSub][idxDivLine] = getValPix(arfDivLineValLin[iLayerSub]);
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
                    if( m_ariDivLineDistMin_px[iLayerSub] > 1 && s_arfScaleRangeFacPixDivValLog[2] * fScaleRangePixDec >= m_ariDivLineDistMin_px[iLayerSub] )
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
                            m_ararfDivLineVal[iLayerSub].resize(ariDivLineCount[iLayerSub]);
                            m_ararfDivLine_px[iLayerSub].resize(ariDivLineCount[iLayerSub]);

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
                                            m_ararfDivLine_px[iLayerSub][idxDivLine] = getValPix(arfDivLineValLin[iLayerSub]);
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

            for( int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
            {
                m_ariDivLineCount[iLayer] = ariDivLineCount[iLayer];
            }

        } // if( validScale() && ScaleRangePix > 0 )

        m_bDivLinesCalculated = true;

    } // if( !m_bDivLinesCalculated )

    if( m_pDiagram != nullptr )
    {
        m_pDiagram->scaleChanged(this);
    }

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
    pDiagScale->m_iMinVal_px = m_iMinVal_px;
    pDiagScale->m_iMaxVal_px = m_iMaxVal_px;

    for( iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
    {
        pDiagScale->m_ariDivLineDistMin_px[iLayer] = m_ariDivLineDistMin_px[iLayer];
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
protected: // auxiliary instance methods
==============================================================================*/

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
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_scale.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::ArgsNormal,
        /* strMethod    */ "pushScale",
        /* strAddInfo   */ strMthInArgs );

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
}

//------------------------------------------------------------------------------
SScale CDiagScale::popScale()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::ArgsNormal,
        /* strMethod    */ "popScale",
        /* strAddInfo   */ "" );

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

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(scale.toString());
    }
    return scale;
}

//------------------------------------------------------------------------------
SScale CDiagScale::clearScaleStack()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::ArgsNormal,
        /* strMethod    */ "clearScaleStack",
        /* strAddInfo   */ "" );

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

/*==============================================================================
protected: // instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CDiagScale::emit_spacingChanged( ESpacing i_spacing )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumSpacing(i_spacing).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_spacingChanged",
        /* strAddInfo   */ strMthInArgs );

    emit spacingChanged(i_spacing);
}

//------------------------------------------------------------------------------
void CDiagScale::emit_scaleChanged( const SScale& i_scale )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_scale.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_scaleChanged",
        /* strAddInfo   */ strMthInArgs );

    emit scaleChanged(i_scale);
}

//------------------------------------------------------------------------------
void CDiagScale::emit_geometryChanged( int i_iMinVal_px, int i_iMaxVal_px )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iMinVal_px) + ", " + QString::number(i_iMaxVal_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_geometryChanged",
        /* strAddInfo   */ strMthInArgs );

    emit geometryChanged(i_iMinVal_px, i_iMaxVal_px);
}

//------------------------------------------------------------------------------
void CDiagScale::emit_divLineDistMinPixChanged( EDivLineLayer i_layer, int i_iDivLineDistMin_px )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumDivLineLayer(i_layer).toString() + ", " + QString::number(i_iDivLineDistMin_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_divLineDistMinPixChanged",
        /* strAddInfo   */ strMthInArgs );

    emit divLineDistMinPixChanged(i_layer, i_iDivLineDistMin_px);
}
