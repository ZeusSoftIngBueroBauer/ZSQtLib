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

#ifndef ZSSysGUI_MathScaleDivLinesMetrics_h
#define ZSSysGUI_MathScaleDivLinesMetrics_h

#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSSys/ZSSysMathScaleDivLines.h"

#include <QtGui/qfont.h>

namespace ZS
{
namespace System
{
namespace GUI
{
namespace Math
{
//******************************************************************************
/*! @brief Class calculating metric informations about division lines for a given scale.

    The following picture shows an X-Scale with division lines for a main and a sub layer.
    The labels of the division lines for the sub layer are not all indicated. The labels
    for 0.0, 10.0, .. 60.0 are not shown but belong to the sub layer. But they are the
    same as divison lines in the main layer and are not shown for better readability of
    the graphic.

    For division line labels which would overlap each other a visibility flag is calculated.
    This visibility flag can be read by method "isDivLineLabelVisible".

     0123456789012345678901234567890123456789012345678901234567890
     +----+----+----+----+----+----+----+----+----+----+----+----+  ("+" Sub Layer division lines)
     |   5.0   |  15.0   |  25.0   |  35.0   |  45.0   |  55.0   |  ("|" Main Layer division lines)
    0.0      10.0      20.0      30.0      40.0      50.0      60.0
*/
class ZSSYSGUIDLL_API CScaleDivLinesMetrics : public ZS::System::Math::CScaleDivLines
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI::Math"; }
    static QString ClassName() { return "CScaleDivLinesMetrics"; }
public: // ctors and dtor
    CScaleDivLinesMetrics(EScaleAxis i_scaleAxis);
    CScaleDivLinesMetrics(const CScaleDivLinesMetrics& i_other);
    ~CScaleDivLinesMetrics();
public: // operators
    CScaleDivLinesMetrics& operator = (const ZS::System::Math::CScaleDivLines& i_other);
    CScaleDivLinesMetrics& operator = (const CScaleDivLinesMetrics& i_other);
public: // instance methods (setting properties)
    bool setFont(const QFont& i_fnt);
    bool setDigitsCountMax(int i_iDigitsCountMax);
    bool setUseEngineeringFormat(bool i_bUseEngineeringFormat);
    bool setDivLineLabelsMinTextExtent(const QSize& i_sizeMin);
public: // instance methods (getting properties)
    QFont font() const;
    int digitsCountMax() const;
    bool useEngineeringFormat() const;
    QSize divLineLabelsMinTextExtent() const;
public: // overridables of base class CScaleDivLines (to recalculate division lines after changing settings)
    virtual bool update() override;
public: // instance methods (returning calculated values)
    int getTrailingDigits() const;
    int getExponentDigits() const;
    QSize getDivLineLabelsMaxTextExtent() const;
    int getDivLineLabelsSpacingInPix() const;
    bool isDivLineLabelVisible(const CEnumDivLineLayer& i_eLayer, int i_idxDivLine) const;
    QString getDivLineLabelText(const CEnumDivLineLayer& i_eLayer, int i_idxDivLine) const;
    QRect getDivLineLabelBoundingRect(const CEnumDivLineLayer& i_eLayer, int i_idxDivLine) const;
    QString getScaleMinValText() const;
    QString getScaleMaxValText() const;
    QRect getScaleMinValBoundingRect() const;
    QRect getScaleMaxValBoundingRect() const;
    QString formatValue(double i_fVal) const;
protected: // overriding auxiliary instance methods of base class
    virtual void invalidateResults() override;
protected: // auxiliary instance methods
    int updateNumberFormatting();
    void updateMaxTextExtentAndSpacing(int i_iLeadingDigits);
    void updateDivLineLabelsBoundingRects();
    void updateDivLineLabelsVisibilities();
    void updateScaleMinMaxBoundingRects();
protected: // auxiliary instance methods
    bool intersectsWithDivLineLabelsRects(
        const QRect& i_rect, const CEnumDivLineLayer& i_eLayer,
        int i_idxDivLineStart, const CEnumSearchDirection& i_eSearchDirection = ESearchDirection::Descending);
protected: // instance members (config values)
    /*!< Font to be used. */
    QFont m_fnt;
    /*!< If greater than 0 defines the maximum number of digits which may be used to
         output the division line labels. If the sum of trailing and leading digits
         would exceed the maximum number of digits for the mantissa, the division lines
         will be labeled in engineering format. */
    int m_iDigitsCountMax;
    /*!< If set to true the division line labels will be output in engineering format. */
    bool m_bUseEngineeringFormat;
    /*!< Minimum text extent of the division line labels. */
    QSize m_sizeMinTextExtent;
protected: // instance members (calculated)
    /*!< Calculated number of trailing digits used to indicate the division line labels. */
    int m_iTrailingDigits;
    /*!< Calculated number of exponent digits used to indicate the division line labels. */
    int m_iExponentDigits;
    /*!< Calculated bounding rectangle of the "longest" division line label. */
    QSize m_sizeMaxTextExtent;
    /*!< Space between two successive division line labels. Calculated depending on the used font. */
    int m_iSpacing_px;
    /*!< Calculated bounding rectangles of the division line labels. */
    QVector<QVector<QRect>> m_ararrectLabels;
    /*!< Calculated texts of the division line labels. */
    QVector<QVector<QString>> m_ararstrLabels;
    /*!< Flag for each division line label to indicate whether the label should be output.
         If a successive label would overlap a preceding label the overlapping label
         is not not shown. */
    QVector<QVector<bool>> m_ararbLabelsVisible;
    /*!< Calculated texts for the minimum and maximum values of the bound scale.
         The need to be formatted according to the scale resolution. */
    QVector<QString> m_arstrScaleMinMaxVal;
    /*!< Calculated bounding rectangles of the minimum and maximum values. */
    QVector<QRect> m_arrectScaleMinMaxVal;

}; // class CScaleDivLinesMetrics

} // namespace Math

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_MathScaleDivLinesMetrics_h
