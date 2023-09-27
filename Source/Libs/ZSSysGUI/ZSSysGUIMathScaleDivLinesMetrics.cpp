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

#include "ZSSysGUI/ZSSysGUIMathScaleDivLinesMetrics.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include <QtGui/qfontmetrics.h>

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI::Math;


/*******************************************************************************
class CScaleDivLinesMetrics
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates an instance of the class.

    @param i_strObjName [in]
        Name of the object.
    @param i_scaleAxis [in]
        Range [X, Y]
*/
CScaleDivLinesMetrics::CScaleDivLinesMetrics(const QString& i_strObjName, EScaleAxis i_scaleAxis) :
//------------------------------------------------------------------------------
    CScaleDivLines(NameSpace(), ClassName(), i_strObjName, i_scaleAxis),
    // config values
    m_fnt(),
    m_iDigitsCountMax(0),
    m_bUseEngineeringFormat(false),
    m_sizeMinTextExtent(),
    // calculated
    m_iTrailingDigits(1),
    m_iExponentDigits(0),
    m_sizeMaxTextExtent(),
    m_iSpacing_px(8),
    m_ararrectLabels(CEnumDivLineLayer::count(), QVector<QRect>(0)),
    m_ararstrLabels(CEnumDivLineLayer::count(), QVector<QString>(0)),
    m_ararbLabelsVisible(CEnumDivLineLayer::count(), QVector<bool>(0)),
    m_arstrScaleMinMaxVal(2),
    m_arrectScaleMinMaxVal(2)
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumScaleAxis(i_scaleAxis).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

} // ctor

//------------------------------------------------------------------------------
/*! @brief Assignment operator.

    @param i_other [in]
        Reference to base class object whose current internal data should be taken over.
*/
CScaleDivLinesMetrics::CScaleDivLinesMetrics(const QString& i_strObjName, const CScaleDivLinesMetrics& i_other) :
//------------------------------------------------------------------------------
    CScaleDivLines(i_strObjName, i_other),
    // config values
    m_fnt(i_other.m_fnt),
    m_iDigitsCountMax(i_other.m_iDigitsCountMax),
    m_bUseEngineeringFormat(i_other.m_bUseEngineeringFormat),
    m_sizeMinTextExtent(i_other.m_sizeMinTextExtent),
    // calculated
    m_iTrailingDigits(i_other.m_iTrailingDigits),
    m_iExponentDigits(i_other.m_iExponentDigits),
    m_sizeMaxTextExtent(i_other.m_sizeMaxTextExtent),
    m_iSpacing_px(i_other.m_iSpacing_px),
    m_ararrectLabels(i_other.m_ararrectLabels),
    m_ararstrLabels(i_other.m_ararstrLabels),
    m_ararbLabelsVisible(i_other.m_ararbLabelsVisible),
    m_arstrScaleMinMaxVal(i_other.m_arstrScaleMinMaxVal),
    m_arrectScaleMinMaxVal(i_other.m_arrectScaleMinMaxVal)
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_other.objectName();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "=",
        /* strAddInfo   */ i_other.NameSpace() + "::" + i_other.ClassName() + "::" + i_other.objectName() );

} // copy ctor

//------------------------------------------------------------------------------
/*! @brief Destructor.
*/
CScaleDivLinesMetrics::~CScaleDivLinesMetrics()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    // config values
    //m_fnt;
    m_iDigitsCountMax = 0;
    m_bUseEngineeringFormat = false;
    //m_sizeMinTextExtent;
    // calculated
    m_iTrailingDigits = 0;
    m_iExponentDigits = 0;
    //m_sizeMaxTextExtent;
    m_iSpacing_px = 0;
    //m_ararrectLabels.clear();
    //m_ararstrLabels.clear();
    //m_ararbLabelsVisible.clear();
    //m_arstrScaleMinMaxVal.clear();
    //m_arrectScaleMinMaxVal.clear();

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Assignment operator.

    @param i_other [in]
        Reference to base class object whose current internal data should be taken over.
*/
CScaleDivLinesMetrics& CScaleDivLinesMetrics::operator = (const ZS::System::Math::CScaleDivLines& i_other)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_other.objectName();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "=",
        /* strAddInfo   */ i_other.NameSpace() + "::" + i_other.ClassName() + "::" + i_other.objectName() );

    static_cast<CScaleDivLines&>(*this) = i_other;

    // Further data processing is necessary to calculate the bounding
    // rectangles of the division line labels depending on the used font.
    m_bDivLinesCalculated = false;

    return *this;
}

//------------------------------------------------------------------------------
/*! @brief Assignment operator.

    @param i_other [in]
        Reference to base class object whose current internal data should be taken over.
*/
CScaleDivLinesMetrics& CScaleDivLinesMetrics::operator = (const CScaleDivLinesMetrics& i_other)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_other.objectName();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "=",
        /* strAddInfo   */ i_other.NameSpace() + "::" + i_other.ClassName() + "::" + i_other.objectName() );

    m_scaleAxis = i_other.m_scaleAxis;
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

    m_fnt = i_other.m_fnt;
    m_iDigitsCountMax = i_other.m_iDigitsCountMax;
    m_bUseEngineeringFormat = i_other.m_bUseEngineeringFormat;
    m_sizeMinTextExtent = i_other.m_sizeMinTextExtent;
    m_iTrailingDigits = i_other.m_iTrailingDigits;
    m_iExponentDigits = i_other.m_iExponentDigits;
    m_sizeMaxTextExtent = i_other.m_sizeMaxTextExtent;
    m_iSpacing_px = i_other.m_iSpacing_px;
    m_ararrectLabels = i_other.m_ararrectLabels;
    m_ararstrLabels = i_other.m_ararstrLabels;
    m_ararbLabelsVisible = i_other.m_ararbLabelsVisible;
    m_arstrScaleMinMaxVal = i_other.m_arstrScaleMinMaxVal;
    m_arrectScaleMinMaxVal = i_other.m_arrectScaleMinMaxVal;

    // Further data processing is necessary to calculate the bounding
    // rectangles of the division line labels depending on the used font.
    m_bDivLinesCalculated = false;

    return *this;
}

/*==============================================================================
public: // instance methods (setting properties)
==============================================================================*/

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
bool CScaleDivLinesMetrics::setFont(const QFont& i_fnt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_fnt.family();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setFont",
        /* strAddInfo   */ strMthInArgs );

    bool bChanged = false;
    if (m_fnt != i_fnt) {
        m_fnt = i_fnt;
        bChanged = true;
        m_bDivLinesCalculated = false;
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bChanged);
    }
    return bChanged;
}

//------------------------------------------------------------------------------
/*! @brief Sets the maximum number of digits for the division line labels.

    The method returns false if the value did not change and true
    if the value changed and the update method need to be called afterwards
    to recalculate the division lines.

    @param i_iDigitsCountMax [in]
        Range [0..N]

    @return true if the value has been changed and the division lines need to
            be recalculated, false otherwise.
*/
bool CScaleDivLinesMetrics::setDigitsCountMax(int i_iDigitsCountMax)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iDigitsCountMax);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setDigitsCountMax",
        /* strAddInfo   */ strMthInArgs );

    bool bChanged = false;
    if (m_iDigitsCountMax != i_iDigitsCountMax) {
        m_iDigitsCountMax = i_iDigitsCountMax;
        bChanged = true;
        m_bDivLinesCalculated = false;
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bChanged);
    }
    return bChanged;
}

//------------------------------------------------------------------------------
/*! @brief Sets whether engineering format should be used to indicate the labels.

    The method returns false if the value did not change and true
    if the value changed and the update method need to be called afterwards
    to recalculate the division lines.

    @param i_bUseEngineeringFormat [in]
        true, if engineering format should be used, false otherwise.

    @return true if the value has been changed and the division lines need to
            be recalculated, false otherwise.
*/
bool CScaleDivLinesMetrics::setUseEngineeringFormat(bool i_bUseEngineeringFormat)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bUseEngineeringFormat);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setUseEngineeringFormat",
        /* strAddInfo   */ strMthInArgs );

    bool bChanged = false;
    if (m_bUseEngineeringFormat != i_bUseEngineeringFormat) {
        m_bUseEngineeringFormat = i_bUseEngineeringFormat;
        bChanged = true;
        m_bDivLinesCalculated = false;
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bChanged);
    }
    return bChanged;
}

//------------------------------------------------------------------------------
/*! @brief Sets the minimum size of the bounding rectangle for the division line labels.

    It might be that additional strings like the unit string have to
    be indicated together with the division line labels.

    For example if the division line labels are bound to the Y-Scale this may
    require that the bounding rectangles of all division line labels must have at
    least of the width of the unit strings bounding rectangle.

    The method returns false if the value did not change and true
    if the value changed and the update method need to be called afterwards
    to recalculate the division lines.

    @param i_rect [in]
        Rectangle with the minimum width and height.

    @return true if the value has been changed and the division lines need to
            be recalculated, false otherwise.
*/
bool CScaleDivLinesMetrics::setDivLineLabelsMinTextExtent(const QSize& i_sizeMin)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qSize2Str(i_sizeMin);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setDivLineLabelsMinTextExtent",
        /* strAddInfo   */ strMthInArgs );

    bool bChanged = false;
    if (m_sizeMinTextExtent != i_sizeMin) {
        m_sizeMinTextExtent = i_sizeMin;
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
/*! @brief Returns the font.

    @Return Font.
*/
QFont CScaleDivLinesMetrics::font() const
//------------------------------------------------------------------------------
{
    return m_fnt;
}

//------------------------------------------------------------------------------
/*! @brief Returns the maximum number of digits.

    @Return Maximum number of digits.
*/
int CScaleDivLinesMetrics::digitsCountMax() const
//------------------------------------------------------------------------------
{
    return m_iDigitsCountMax;
}

//------------------------------------------------------------------------------
/*! @brief Returns whether engineering format should be used.

    @Return true, if engineering format should be used, false otherwise.
*/
bool CScaleDivLinesMetrics::useEngineeringFormat() const
//------------------------------------------------------------------------------
{
    return m_bUseEngineeringFormat;
}

//------------------------------------------------------------------------------
/*! @brief Returns the minimum size for the division line labels.

    @Return Minimum size of division line labels.
*/
QSize CScaleDivLinesMetrics::divLineLabelsMinTextExtent() const
//------------------------------------------------------------------------------
{
    return m_sizeMinTextExtent;
}

/*==============================================================================
public: // overridables of base class CScaleDivLines (to recalculate division lines after changing settings)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief When changing settings the update method has to be called to
           recalcuate (update) the division line values.

    Sets the flag m_bDivLinesCalculated to true.

    @return true if the division lines have been newly calculated,
            false if the division lines were already up to date.
*/
bool CScaleDivLinesMetrics::update()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "update",
        /* strAddInfo   */ "" );

    // The base class will set the flag to true. But to calculate the metrics
    // also font changes etc. need to be taken into account.
    if (!m_bDivLinesCalculated)
    {
        // The base class calculates the pixel position of the division lines
        // so that the values are a whole number multiple of ten.
        CScaleDivLines::update();

        // Calculate number formatting (needed leading, trailing and exponent digits)
        int iLeadingDigits = updateNumberFormatting();

        // Calculate maximum extent of division line labels
        updateMaxTextExtentAndSpacing(iLeadingDigits);

        // Calculate bounding rectangles of division line labels
        updateDivLineLabelsBoundingRects();

        // Calculate bounding rectangles of minimum and maximum scale values
        updateScaleMinMaxBoundingRects();

        // Hide those division line labels which would overlap other division line labels.
        updateDivLineLabelsVisibilities();

        m_bDivLinesCalculated = true;

    } // if (!m_bDivLinesCalculated)

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(m_bDivLinesCalculated);
    }
    return m_bDivLinesCalculated;

} // update

/*==============================================================================
public: // instance methods (returning calculated values)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the calculated number of trailing digits for the division line labels.

    @return Calculated number of trailing digits.
*/
int CScaleDivLinesMetrics::getTrailingDigits() const
//------------------------------------------------------------------------------
{
    return m_iTrailingDigits;
}

//------------------------------------------------------------------------------
/*! @brief Returns the calculated number of exponent digits for the division line labels.

    @return Calculated number of exponent digits.
*/
int CScaleDivLinesMetrics::getExponentDigits() const
//------------------------------------------------------------------------------
{
    return m_iExponentDigits;
}

//------------------------------------------------------------------------------
/*! @brief Returns the calculated size of the "longest" division line label.

    @return Calculated size of the "longest" division line label.
*/
QSize CScaleDivLinesMetrics::getDivLineLabelsMaxTextExtent() const
//------------------------------------------------------------------------------
{
    return m_sizeMaxTextExtent;
}

//------------------------------------------------------------------------------
/*! @brief Returns the calculated spacing between two division line labels.

    @return Calculated spacing between two division line labels.
*/
int CScaleDivLinesMetrics::getDivLineLabelsSpacingInPix() const
//------------------------------------------------------------------------------
{
    return m_iSpacing_px;
}

//------------------------------------------------------------------------------
/*! @brief Returns the visibility flag for the division line label.

    @param i_eLayer [in]
        Range [Main, Sub]
        Layer for which the value should be returned.
    @param i_idxDivLine [in]
        Range [0..DivLineCount-1]
        Index of the division line the value should be returned.

    @return String to label the division line.
*/
bool CScaleDivLinesMetrics::isDivLineLabelVisible(const CEnumDivLineLayer& i_eLayer, int i_idxDivLine) const
//------------------------------------------------------------------------------
{
    return m_ararbLabelsVisible[i_eLayer.enumeratorAsInt()][i_idxDivLine];
}

//------------------------------------------------------------------------------
/*! @brief Returns the formatted text to label the specified division line.

    @param i_eLayer [in]
        Range [Main, Sub]
        Layer for which the value should be returned.
    @param i_idxDivLine [in]
        Range [0..DivLineCount-1]
        Index of the division line the value should be returned.

    @return String to label the division line.
*/
QString CScaleDivLinesMetrics::getDivLineLabelText(
    const CEnumDivLineLayer& i_eLayer, int i_idxDivLine) const
//------------------------------------------------------------------------------
{
    return m_ararstrLabels[i_eLayer.enumeratorAsInt()][i_idxDivLine];
}

//------------------------------------------------------------------------------
/*! @brief Returns the bounding rectangle for the text to label the specified division line.

    @param i_eLayer [in]
        Range [Main, Sub]
        Layer for which the value should be returned.
    @param i_idxDivLine [in]
        Range [0..DivLineCount-1]
        Index of the division line the value should be returned.

    @return Bounding rectangle of the label text.
*/
QRect CScaleDivLinesMetrics::getDivLineLabelBoundingRect(
    const CEnumDivLineLayer& i_eLayer, int i_idxDivLine) const
//------------------------------------------------------------------------------
{
    return m_ararrectLabels[i_eLayer.enumeratorAsInt()][i_idxDivLine];
}

//------------------------------------------------------------------------------
/*! @brief Returns the calculated string the minimum scale value.

    @return Minimum scale value as string.
*/
//------------------------------------------------------------------------------
QString CScaleDivLinesMetrics::getScaleMinValText() const
{
    return m_arstrScaleMinMaxVal[0];
}

//------------------------------------------------------------------------------
/*! @brief Returns the calculated string the maximum scale value.

    @return Maximum scale value as string.
*/
//------------------------------------------------------------------------------
QString CScaleDivLinesMetrics::getScaleMaxValText() const
{
    return m_arstrScaleMinMaxVal[1];
}

//------------------------------------------------------------------------------
/*! @brief Returns the calculated bounding rectangles of the minimum scale value.

    @return Bounding rectangle of the minimum scale value.
*/
//------------------------------------------------------------------------------
QRect CScaleDivLinesMetrics::getScaleMinValBoundingRect() const
{
    return m_arrectScaleMinMaxVal[0];
}

//------------------------------------------------------------------------------
/*! @brief Returns the calculated bounding rectangles of the maximum scale value.

    @return Bounding rectangle of the maximum scale value.
*/
QRect CScaleDivLinesMetrics::getScaleMaxValBoundingRect() const
//------------------------------------------------------------------------------
{
    return m_arrectScaleMinMaxVal[1];
}

//------------------------------------------------------------------------------
/*! @brief Formats the value into a string according to the internally
           calculated number of trailing and exponent digits.

    @param i_fVal [in]
        Value to be formatted.

    @return String containing the formatted value.
*/
QString CScaleDivLinesMetrics::formatValue(double i_fVal) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fVal);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "formatValue",
        /* strAddInfo   */ strMthInArgs );

    QString strVal;
    if( m_iExponentDigits <= 0 ) {
        strVal = QString::number(i_fVal, 'f', m_iTrailingDigits);
    }
    else {
        strVal = QString::number(i_fVal, 'e', m_iTrailingDigits);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(strVal);
    }
    return strVal;

} // formatValue

/*==============================================================================
protected: // overridable auxiliary instance methods of base class
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Invalidates the result member variables.

    Called before calculating the results in update method.
*/
void CScaleDivLinesMetrics::invalidateResults()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "invalidateResults",
        /* strAddInfo   */ "" );

    CScaleDivLines::invalidateResults();

    m_iTrailingDigits = 0;
    m_iExponentDigits = 0;
    m_sizeMaxTextExtent = QSize();
    m_iSpacing_px = 0;

    for (int iLayer = 0; iLayer < CEnumDivLineLayer::count(); ++iLayer) {
        m_ararrectLabels[iLayer].clear();
        m_ararstrLabels[iLayer].clear();
        m_ararbLabelsVisible[iLayer].clear();
    }
    for (int idxMinMax = 0; idxMinMax < 2; ++idxMinMax) {
        m_arstrScaleMinMaxVal[idxMinMax].clear();
        m_arrectScaleMinMaxVal[idxMinMax] = QRect();
    }
} // invalidateResults

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to calculate number formatting
           (needed leading, trailing and exponent digits).

    The calculated trailing and exponent digits are store in member variables.
    The number of leading digits are only temporarily needed to calculate
    the maximum text extent of the division line labels and is not stored in
    member variables but will be returend by the method.

    @return Calculated number of leading digits.
*/
int CScaleDivLinesMetrics::updateNumberFormatting()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateNumberFormatting",
        /* strAddInfo   */ "" );

    int iLeadingDigits = 1;

    // The number of trailing digits is defined by the mimimum distance between two
    // division lines to indicate unique values. If the distance is greater than 1.0
    // at least one trailing digit will be shown.
    m_iTrailingDigits = 1;
    double fDivLineDistMinVal = getDivLinesDistMin(CEnumDivLineLayer());
    if (fDivLineDistMinVal < 1.0) {
        m_iTrailingDigits = abs(ZS::System::Math::getFirstSignificantDigit(fDivLineDistMinVal));
    }

    // The number of leading digits is defined by the absolute maximum value
    // of the scales division lines. If the value is less than 1.0 at least
    // one leading digit will be shown.
    std::tuple<double, double> minMax = ZS::System::Math::getAbsMinMax(m_arfDivLinesValsSorted);
    double fDivLineValAbsMax = std::get<1>(minMax);
    if (fDivLineValAbsMax > 1.0) {
        iLeadingDigits = ZS::System::Math::getFirstSignificantDigit(fDivLineValAbsMax);
    }

    // The absolute minimum value of the division lines may be lower than
    // the distance between two division lines.
    double fDivLineValAbsMin = std::get<0>(minMax);
    if (fDivLineValAbsMin < 1.0) {
        int iTrailingDigits = abs(ZS::System::Math::getFirstSignificantDigit(fDivLineValAbsMin));
        if (iTrailingDigits > m_iTrailingDigits) {
            m_iTrailingDigits = iTrailingDigits;
        }
    }

    // As the absolute maximum value of the division lines may not be lower than
    // the absolute minimum value this value don't need to be taken into account
    // when calculating the number of trailing digits.

    // Next calculate how many leading and trailing digits would be necessary to
    // indicate the absolute minimum and maximum values and decide, whether
    // an exponent is necessary to indicate the values.

    // If engineering format is forced or if the number of leading and trailing digits
    // exceeds the maximum number of digits configured for the mantissa the labels must
    // be indicated in engineering format.
    // In case the labels got to be shown in engineering format the number of trailing
    // digits are no longer defined by the distance of the division lines. The necessary
    // precision to indicate unique labels is calculated further below by converting each
    // division line value into its string representation using a precision, compare all
    // the strings and increase the precision until the strings are unique.
    // If engineering format is chosen because the number of digits for the mantissa is
    // limited it may happen that the labels become ambiguous.

    m_iExponentDigits = 0;

    // If engineering format is forced ...
    if (m_bUseEngineeringFormat) {
        m_iExponentDigits = static_cast<int>(log10(static_cast<double>(iLeadingDigits)))+1;
        int iTmp = static_cast<int>(log10(static_cast<double>(m_iTrailingDigits)))+1;
        if (iTmp > m_iExponentDigits) {
            m_iExponentDigits = iTmp;
        }
        iLeadingDigits = 1;
        m_iTrailingDigits = 1;
    }
    // If the maximum number of digits is set ...
    else if (m_iDigitsCountMax > 0) {
        // Engineering format (x.y) has to be used if the number of digits necessary to
        // indicate the value is greater than the maximum number of digits set.
        if ((iLeadingDigits + m_iTrailingDigits) > m_iDigitsCountMax)
        {
            m_iExponentDigits = static_cast<int>(log10(static_cast<double>(iLeadingDigits)))+1;
            int iTmp = static_cast<int>(log10(static_cast<double>(m_iTrailingDigits)))+1;
            if (iTmp > m_iExponentDigits) {
                m_iExponentDigits = iTmp;
            }
            iLeadingDigits = 1;
            m_iTrailingDigits = 1;
        }
    }

    // Already mentioned above:
    // If engineering format is forced or if the number of leading and trailing digits
    // exceeds the maximum number of digits,
    // but also if the scale uses logarithmic spacing ...
    if (m_iExponentDigits > 0 || m_spacing == ESpacing::Logarithmic) {
        // The division lines are rounded to a whole number of a decimal power.
        // To output the axis labels it is "just" necessary to ensure that the
        // indicated values are different.
        int iPrecisionMax = 10;
        if (m_iDigitsCountMax > 0) {
            iPrecisionMax = m_iDigitsCountMax - iLeadingDigits;
        }
        m_iTrailingDigits = getPrecision2ShowUniqueNumbers(
            m_arfDivLinesValsSorted, m_iExponentDigits,
            m_iTrailingDigits, iPrecisionMax, m_pTrcAdminObj);
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs =
            "Leading: " + QString::number(iLeadingDigits) +
            ", Trailing: " + QString::number(m_iTrailingDigits) +
            ", Exponent: " + QString::number(m_iExponentDigits);
        mthTracer.setMethodOutArgs(strMthOutArgs);
    }
    return iLeadingDigits;

} // updateNumberFormatting

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to calculate the maximum extent of
           division line labels and the spacing used between two labels.

    The calculated trailing and exponent digits are store in member variables.
    The number of leading digits are only temporarily needed to calculate
    the maximum text extent of the division line labels and is not stored in
    member variables but will be passed by argument to the method.

    @param i_iLeadingDigits [in]
        Calculated number of leading digits.
*/
void CScaleDivLinesMetrics::updateMaxTextExtentAndSpacing(int i_iLeadingDigits)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "LeadingDigits: " + QString::number(i_iLeadingDigits);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateMaxTextExtentAndSpacing",
        /* strAddInfo   */ strMthInArgs );

    // Build string with maximum count of necessary digits and calculate
    // the maximum extent of the division line labels:
    QString strDivLineLabel("+");
    for (int idx = 0; idx < i_iLeadingDigits; idx++) {
        strDivLineLabel += "0";
    }
    if (m_iTrailingDigits > 0) {
        strDivLineLabel += ".";
        for (int idx = 0; idx < m_iTrailingDigits; idx++) {
            strDivLineLabel += "0";
        }
    }
    if (m_iExponentDigits > 0) {
        strDivLineLabel += "e+";
        for (int idx = 0; idx < m_iExponentDigits; idx++) {
            strDivLineLabel += "0";
        }
    }

    // I'm not sure if "0" is the most "wide" character ...
    strDivLineLabel += "0"; // .. so we add one additional character.
    QFontMetrics fntmtr(m_fnt);
    m_sizeMaxTextExtent = fntmtr.boundingRect(strDivLineLabel).size();
    if (m_sizeMaxTextExtent.width() < m_sizeMinTextExtent.width()) {
        m_sizeMaxTextExtent.setWidth(m_sizeMinTextExtent.width());
    }
    if (m_sizeMaxTextExtent.height() < m_sizeMinTextExtent.height()) {
        m_sizeMaxTextExtent.setHeight(m_sizeMinTextExtent.height());
    }

    // There must be a distance (space) between two successive division line labels on the
    // horizontal (X) axis. One and a half of the width of the character '0' is a good choice.
    m_iSpacing_px = fntmtr.boundingRect("0").width();
    m_iSpacing_px += m_iSpacing_px/2;

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs =
            "MaxTextExtent {" + qSize2Str(m_sizeMaxTextExtent) + "}" +
            ", Spacing: " + QString::number(m_iSpacing_px);
        mthTracer.setMethodOutArgs(strMthOutArgs);
    }
} // updateMaxTextExtentAndSpacing

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to calculate the bounding rectangles of
           the division line labels.
*/
void CScaleDivLinesMetrics::updateDivLineLabelsBoundingRects()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateDivLineLabelsBoundingRects",
        /* strAddInfo   */ "" );

    for (int iLayer = 0; iLayer < CEnumDivLineLayer::count(); ++iLayer) {
        if (m_ariDivLinesCount[iLayer] > 0) {
            m_ararrectLabels[iLayer] = QVector<QRect>(m_ariDivLinesCount[iLayer]);
            m_ararstrLabels[iLayer] = QVector<QString>(m_ariDivLinesCount[iLayer]);
        }
    }

    if (m_scaleAxis == EScaleAxis::X)
    {
        QFontMetrics fntmtr(m_fnt);

        for (int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++)
        {
            EDivLineLayer eLayer = static_cast<EDivLineLayer>(iLayer);
            QRect rectDivLineLabel;

            rectDivLineLabel.setHeight(m_sizeMaxTextExtent.height());

            for (int idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]; idxDivLine++)
            {
                int xDivLine_px = getDivLineInPix(eLayer, idxDivLine);
                QString strDivLineLabel = formatValue(m_ararfDivLinesVals[iLayer][idxDivLine]);

                // At the horizontal (X) axis the labels will be output centered below the division lines
                // and we can remove trailing zeros:
                removeTrailingZeros(&strDivLineLabel);
                rectDivLineLabel = fntmtr.boundingRect(strDivLineLabel);
                rectDivLineLabel.setWidth(rectDivLineLabel.width() + m_iSpacing_px);
                rectDivLineLabel.moveLeft(xDivLine_px - rectDivLineLabel.width()/2);

                // To discover overlapping division line labels the rectangle extents of the
                // division line labels will be stored in an array.
                m_ararrectLabels[iLayer][idxDivLine] = rectDivLineLabel;
                m_ararstrLabels[iLayer][idxDivLine] = strDivLineLabel;
            }
        }
    }
    else // if (m_scaleAxis == EScaleAxis::Y)
    {
        for (int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++)
        {
            EDivLineLayer eLayer = static_cast<EDivLineLayer>(iLayer);
            QRect rectDivLineLabel;

            rectDivLineLabel.setWidth(m_sizeMaxTextExtent.width());
            rectDivLineLabel.setHeight(m_sizeMaxTextExtent.height());

            for (int idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]; idxDivLine++)
            {
                int yDivLine = getDivLineInPix(eLayer, idxDivLine);
                QString strDivLineLabel = formatValue(m_ararfDivLinesVals[iLayer][idxDivLine]);
                rectDivLineLabel.moveTop(yDivLine - m_sizeMaxTextExtent.height()/2);

                // To discover overlapping division line labels the rectangle extents of the
                // division line labels will be stored in an array.
                m_ararrectLabels[iLayer][idxDivLine] = rectDivLineLabel;
                m_ararstrLabels[iLayer][idxDivLine] = strDivLineLabel;
            }
        }
    }
} // updateDivLineLabelsBoundingRects

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to set the visibility flag to false for
           those division line labels which would overlap other division line labels.

    The following picture shows an X-Scale with division lines for a main and a sub layer.
    The labels of the division lines for the sub layer are not all indicated. The labels
    for 0.0, 10.0, .., 60.0 are not shown but belong to the sub layer. But they are the
    same as divison lines in the main layer and are not shown for better readability of
    the graphic.

    For division line labels which would overlap each other a visibility flag is calculated.
    This visibility flag can be read by method "isDivLineLabelVisible".

     0123456789012345678901234567890123456789012345678901234567890
     +----+----+----+----+----+----+----+----+----+----+----+----+  ("+" Sub Layer division lines)
     |   5.0   |  15.0   |  25.0   |  35.0   |  45.0   |  55.0   |  ("|" Main Layer division lines)
    0.0      10.0      20.0      30.0      40.0      50.0      60.0

    If sub division lines should be indicated the labels at the sub division lines
    may not overlap labels at main division line labels. The visibility flag
    for the values 0.0, 10.0, .., 60.0 is therefore set to false for the sub layer.

*/
void CScaleDivLinesMetrics::updateDivLineLabelsVisibilities()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateDivLineLabelsVisibilities",
        /* strAddInfo   */ "" );
    
    for (int iLayer = 0; iLayer < CEnumDivLineLayer::count(); ++iLayer) {
        if (m_ariDivLinesCount[iLayer] > 0) {
            m_ararbLabelsVisible[iLayer] = QVector<bool>(m_ariDivLinesCount[iLayer], true);
        }
    }

    // First hide all sub division line labels which would overlap main division line labels.
    for (int idxDivLine = 0; idxDivLine < m_ariDivLinesCount[EDivLineLayerSub]; ++idxDivLine) {
        QRect rect = m_ararrectLabels[EDivLineLayerSub][idxDivLine];
        if (intersectsWithDivLineLabelsRects(rect, EDivLineLayerMain, 0, ESearchDirection::Ascending)) {
            m_ararbLabelsVisible[EDivLineLayerSub][idxDivLine] = false;
        }
    }

    // Next hide sub division line labels which would overlap other labels in the sub layer.
    for (int idxDivLine = 1; idxDivLine < m_ariDivLinesCount[EDivLineLayerSub]; ++idxDivLine) {
        QRect rect = m_ararrectLabels[EDivLineLayerSub][idxDivLine];
        if (intersectsWithDivLineLabelsRects(rect, EDivLineLayerSub, idxDivLine-1, ESearchDirection::Descending)) {
            m_ararbLabelsVisible[EDivLineLayerSub][idxDivLine] = false;
        }
    }

    // Next hide main division line labels which would overlap other labels in the main layer.
    // The first division line label is always visible. The following division line labels are
    // visible if they do not overlap any of the previous visible labels.
    for (int idxDivLine = 0; idxDivLine < m_ariDivLinesCount[EDivLineLayerMain]; ++idxDivLine) {
        QRect rect = m_ararrectLabels[EDivLineLayerMain][idxDivLine];
        if (intersectsWithDivLineLabelsRects(rect, EDivLineLayerMain, idxDivLine-1, ESearchDirection::Descending)) {
            m_ararbLabelsVisible[EDivLineLayerMain][idxDivLine] = false;
        }
    }

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        for( int iLayer = 1; iLayer < CEnumDivLineLayer::count(); ++iLayer) {
            QString strMthAddInfo =
                "LabelsVisible[" + CEnumDivLineLayer(iLayer).toString() + "][" +
                QString::number(m_ararbLabelsVisible[iLayer].size()) + "]";
            if (m_ararbLabelsVisible[iLayer].size() > 0) {
                strMthAddInfo += "(";
                for (int idxDivLine = 0; idxDivLine < m_ararbLabelsVisible[iLayer].size(); ++idxDivLine) {
                    if (strMthAddInfo.endsWith("(")) strMthAddInfo += ", ";
                    strMthAddInfo += QString(m_ararbLabelsVisible[iLayer][idxDivLine] ? "1" : "0");
                }
                strMthAddInfo += ")";
            }
            mthTracer.trace(strMthAddInfo);
        }
    }
} // updateDivLineLabelsVisibilities

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to calculate the bounding rectangles of
           the minimum and maximum scale values.
*/
void CScaleDivLinesMetrics::updateScaleMinMaxBoundingRects()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateScaleMinMaxBoundingRects",
        /* strAddInfo   */ "" );

    for (int idxMinMax = 0; idxMinMax < 2; ++idxMinMax) {
        m_arstrScaleMinMaxVal[idxMinMax] = "";
        m_arrectScaleMinMaxVal[idxMinMax] = QRect();
    }

    QFontMetrics fntmtr(m_fnt);

    if (m_scaleAxis == EScaleAxis::X)
    {
        for (int idxMinMax = 0; idxMinMax < 2; ++idxMinMax)
        {
            m_arstrScaleMinMaxVal[idxMinMax] = "";
            m_arrectScaleMinMaxVal[idxMinMax].setWidth(0);
            m_arrectScaleMinMaxVal[idxMinMax].setHeight(0);

            int xDivLine = 0;
            if (idxMinMax == 0) {
                m_arstrScaleMinMaxVal[idxMinMax] = formatValue(m_fScaleMin);
                xDivLine = scaleMinInPix();
            }
            else {
                m_arstrScaleMinMaxVal[idxMinMax] = formatValue(m_fScaleMax);
                xDivLine = scaleMaxInPix();
            }
            m_arrectScaleMinMaxVal[idxMinMax] = fntmtr.boundingRect(m_arstrScaleMinMaxVal[idxMinMax]);
            m_arrectScaleMinMaxVal[idxMinMax].setWidth(m_arrectScaleMinMaxVal[idxMinMax].width() + 2);
            m_arrectScaleMinMaxVal[idxMinMax].setHeight(m_arrectScaleMinMaxVal[idxMinMax].height() + 2);
            m_arrectScaleMinMaxVal[idxMinMax].moveLeft(xDivLine - m_arrectScaleMinMaxVal[idxMinMax].width()/2);

            //if (m_arrectScaleMinMaxVal[idxMinMax].left() <= m_fMin_px) {
            //    int cxOffset = m_fMin_px - m_arrectScaleMinMaxVal[idxMinMax].left() + 1;
            //    m_arrectScaleMinMaxVal[idxMinMax].moveLeft(m_arrectScaleMinMaxVal[idxMinMax].left() + cxOffset);
            //}
            //else if (m_arrectScaleMinMaxVal[idxMinMax].right() >= m_fMax_px) {
            //    int cxOffset = m_arrectScaleMinMaxVal[idxMinMax].right() - m_fMax_px + 1;
            //    m_arrectScaleMinMaxVal[idxMinMax].moveRight(m_arrectScaleMinMaxVal[idxMinMax].right() - cxOffset);
            //}
        }
    }
    else // if (m_scaleAxis == EScaleAxis::Y)
    {
        for (int idxMinMax = 0; idxMinMax < 2; ++idxMinMax)
        {
            m_arstrScaleMinMaxVal[idxMinMax] = "";
            m_arrectScaleMinMaxVal[idxMinMax].setWidth(0);
            m_arrectScaleMinMaxVal[idxMinMax].setHeight(0);

            int yDivLine = 0;
            if (idxMinMax == 0) {
                m_arstrScaleMinMaxVal[idxMinMax] = formatValue(m_fScaleMin);
                yDivLine = scaleMinInPix();
            }
            else {
                m_arstrScaleMinMaxVal[idxMinMax] = formatValue(m_fScaleMax);
                yDivLine = scaleMaxInPix();
            }
            m_arrectScaleMinMaxVal[idxMinMax] = fntmtr.boundingRect(m_arstrScaleMinMaxVal[idxMinMax]);
            m_arrectScaleMinMaxVal[idxMinMax].setWidth(m_arrectScaleMinMaxVal[idxMinMax].width() + 2);
            m_arrectScaleMinMaxVal[idxMinMax].setHeight(m_arrectScaleMinMaxVal[idxMinMax].height() + 2);
            m_arrectScaleMinMaxVal[idxMinMax].moveTop(yDivLine - m_arrectScaleMinMaxVal[idxMinMax].height()/2);

            //if (m_arrectScaleMinMaxVal[idxMinMax].bottom() <= m_fMin_px)
            //{
            //    int cyOffset = m_arrectScaleMinMaxVal[idxMinMax].bottom() - m_fMin_px + 1;
            //    m_arrectScaleMinMaxVal[idxMinMax].moveBottom(m_arrectScaleMinMaxVal[idxMinMax].bottom() - cyOffset);
            //}
            //else if (m_arrectScaleMinMaxVal[idxMinMax].top() >= m_fMax_px)
            //{
            //    int cyOffset = m_fMax_px - m_arrectScaleMinMaxVal[idxMinMax].top() + 1;
            //    m_arrectScaleMinMaxVal[idxMinMax].moveTop(m_arrectScaleMinMaxVal[idxMinMax].top() + cyOffset);
            //}
        }
    }
} // updateScaleMinMaxBoundingRects

//------------------------------------------------------------------------------
/*! @brief Checks whether the given rectangle intersects with any of the existing
           rectangles used to label the axis.

    Only those division line labels are taken into account whose visibility flag
    is not already set to false.

    @param i_rect [in]
        Rectangle to be checked whether it intersects any of the existing label rectangles.
    @param i_eLayer [in]
        Div line layer to be checked.
        Range [Main, Sub]
    @param i_idxDivLineStart [in]
        Index of the division line to be checked.
    @param i_eSearchDirection [in]
        Search list upwards (Ascending) or downwards (descending).

    @return true if the passed rectangle intersects any of the visible div line labels,
            false otherwise.
*/
bool CScaleDivLinesMetrics::intersectsWithDivLineLabelsRects(
    const QRect& i_rect, const CEnumDivLineLayer& i_eLayer,
    int i_idxDivLineStart, const CEnumSearchDirection& i_eSearchDirection)
//------------------------------------------------------------------------------
{
    bool bIntersects = false;

    int iLayer = i_eLayer.enumeratorAsInt();

    if (m_ariDivLinesCount[iLayer] == 0) {
        bIntersects = false;
    }
    else {
        int idxDivLine = i_idxDivLineStart;
        while (idxDivLine >= 0 && idxDivLine < m_ariDivLinesCount[iLayer]) {
            if (m_ararbLabelsVisible[iLayer][idxDivLine]) {
                // If the label is visible and the rectangle overlaps the rectangle
                // no need to check other rectangles. Same applies if it does not
                // overlap the visible rectangle as than the rectangle does not overlap
                // any other rectangle in the search direction.
                if (i_rect.intersects(m_ararrectLabels[iLayer][idxDivLine])) {
                    bIntersects = true;
                }
                break;
            }
            if (i_eSearchDirection == ESearchDirection::Ascending) {
                ++idxDivLine;
            } else {
                --idxDivLine;
            }
        }
    }
    return bIntersects;

} // intersectsWithDivLineLabelsRects
