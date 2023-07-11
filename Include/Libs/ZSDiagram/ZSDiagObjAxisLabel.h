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

#ifndef ZSDiagObjAxisLabel_h
#define ZSDiagObjAxisLabel_h

#include "ZSDiagram/ZSDiagObj.h"
#include "ZSSysGUI/ZSSysGUIMathScaleDivLinesMetrics.h"

#include <QtGui/qcolor.h>
#include <QtGui/qfont.h>

namespace ZS
{
namespace Diagram
{
//******************************************************************************
/*! @brief Class for axis labels.

    Axis labels are used to label the axis scales at the division lines.

    Also the unit or a axis label may be indicated.

    The position of the division lines is calculated by the scale object
    to be always at a whole number multiple of a decimal power.
    The axis label uses the scale division line labels instance for
    formatting the division line labels.

    The axis label is divided into three different parts:

    - Lines
      Lines to divide the axis into sections.
      The Lines are used to indicate at which position the axis is divided.
      If also a grid is shown the Lines of the axis may be superfluous.
      If the scale bound to the axis label is configured to calculate main
      and sub division lines also the axis label instance may show main and
      sub division lines.
    - DivLineLabels
      The division line labels are shown close to the division lines to indicate
      the corresponding value.
      If the scale bound to the axis label is configured to calculate main
      and sub division lines also the division line labels may be output at the
      main and sub division lines.
    - AxisLabel
      The axis label is used to indicate the physical size (Frequency).
    - Unit
      The unit may be positioned in the area of the division line labels at the
      scale max value labels or may be appended to the axis label.

    Example for an axis label bound to a X-Scale:

     +---------|---------|---------|---------|---------|---------|
    0.0       10.0      20.0      30.0      40.0      50.0      MHz
                      Frequency/MHz

    Example for an axis label bound to a Y-Scale:

               50.0 +
            P       |
            o  40.0 +
            w       |
            e  30.0 +
            r       |
            /  20.0 +
            d       |
            B  10.0 +
            b       |
                0.0 +

    The division line labels for the scale minimum and maximum value may
    be omitted depending on the area provided for the axis label.
*/
class ZSDIAGRAMDLL_API CDiagObjAxisLabel : public CDiagObj
//******************************************************************************
{
public: // class methods
    static QString ClassName() { return "CDiagObjAxisLabel"; }
public: // ctors and dtor
    CDiagObjAxisLabel(
        const QString& i_strObjName,
        CDiagScale*    i_pDiagScale,
        ELayoutPos     i_layoutPos,
        const QString& i_strAxisLabel = QString() );
    virtual ~CDiagObjAxisLabel();
public: // instance methods (spacing between areas)
    void setSpacingDiagPartCenter2DivLineLabels( int i_iSpacing_px );
    int spacingDiagPartCenter2DivLineLabels() const;
    void setSpacingDiagPartCenter2AxisLabel( int i_iSpacing_px );
    int spacingDiagPartCenter2AxisLabel() const;
    void setSpacingDivLineLabels2AxisLabel( int i_iSpacing_px );
    int spacingDivLineLabels2AxisLabel() const;
public: // instance methods (division lines)
    void showDivLines(const ZS::System::CEnumDivLineLayer& i_eLayer);
    void hideDivLines(const ZS::System::CEnumDivLineLayer& i_eLayer);
    void setDivLinesForegroundColor(const QColor& i_color, const ZS::System::CEnumDivLineLayer& i_eLayer);
    QColor divLinesForegroundColor(const ZS::System::CEnumDivLineLayer& i_eLayer) const;
    void setDivLinesPenStyle(const Qt::PenStyle& i_penStyle, const ZS::System::CEnumDivLineLayer& i_eLayer);
    Qt::PenStyle divLinesPenStyle(const ZS::System::CEnumDivLineLayer& i_eLayer) const;
public: // instance methods (division line labels)
    void showDivLineLabels(const ZS::System::CEnumDivLineLayer& i_eLayer);
    void hideDivLineLabels(const ZS::System::CEnumDivLineLayer& i_eLayer);
    void setDivLineLabelsColor(const QColor& i_color, const ZS::System::CEnumDivLineLayer& i_eLayer);
    QColor divLineLabelsColor(const ZS::System::CEnumDivLineLayer& i_eLayer) const;
    void setDivLineLabelsFont(const QFont& i_fnt, const ZS::System::CEnumDivLineLayer& i_eLayer);
    QFont divLineLabelsFont(const ZS::System::CEnumDivLineLayer& i_eLayer) const;
    void setDigitsCountMax( int i_iDigitsCountMax );
    int digitsCountMax() const;
    void setUseEngineeringFormat( bool i_bUseEngineeringFormat );
    bool useEngineeringFormat() const;
public: // instance methods (axis label)
    void setAxisLabel( const QString& i_strLabel );
    QString axisLabel() const;
    void showAxisLabel();
    void hideAxisLabel();
    void setAxisLabelColor(const QColor& i_color);
    QColor axisLabelColor() const;
    void setAxisLabelFont(const QFont& i_fnt);
    QFont axisLabelFont() const;
public: // instance methods (unit)
    void setUnit( PhysVal::CUnit* i_pUnit ); // nullptr means "use best unit"
    PhysVal::CUnit* getUnit();
public: // instance methods (unit at max scale value)
    void showUnitAtDivLines();
    void hideUnitAtDivLines();
    void setUnitColorAtDivLines(const QColor& i_color);
    QColor unitColorAtDivLines() const;
    void setUnitFontAtDivLines(const QFont& i_fnt);
    QFont unitFontAtDivLines() const;
public: // instance methods (unit at axis label)
    void showUnitAtAxisLabel();
    void hideUnitAtAxisLabel();
    void setUnitColorAtAxisLabel(const QColor& i_color);
    QColor unitColorAtAxisLabel() const;
    void setUnitFontAtAxisLabel(const QFont& i_fnt);
    QFont unitFontAtAxisLabel() const;
public: // overridables of base class CDiagObj
    virtual void show( bool i_bInformDiagram = true ) override;
    virtual void hide( bool i_bInformDiagram = true ) override;
    virtual QSize sizeHint() override;
public: // must overridables of base class CDiagObj
    virtual CDiagObj* clone( CDataDiagram* i_pDiagramTrg ) const override;
    virtual void update( unsigned int i_uUpdateFlags, QPaintDevice* i_pPaintDevice = nullptr ) override;
protected: // instance methods
    void updateLayout();
    void updateData();
    void updatePixmap( QPaintDevice* i_pPaintDevice );
    void updateWidget();
protected: // auxiliary instance methods
    QRect getDiagRect() const;
    PhysVal::CUnit getAxisLabelUnit() const;
    bool isAxisLabelVisible() const;
    QSize getDivLineLabelsUnitTextExtent() const;
    QRect getDivLineLabelsUnitBoundingRect() const;
    QSize getAxisLabelTextExtent() const;
    QRect getAxisLabelBoundingRect() const;
    //void adjustDivLineLabelsMaxTextExtent();
    bool intersectsDivLineLabelsPhysUnit(const QRect& i_rect) const;
    bool intersectsDivLineLabels(
        const QRect& i_rect,
        const ZS::System::CEnumDivLineLayer& i_eLayer = ZS::System::CEnumDivLineLayer(),
        int i_idxDivLineLabelMin = -1,
        int i_idxDivLineLabelMax = -1) const;
    QString formatValue(double i_fVal, PhysVal::CUnit* i_pUnit = nullptr) const;
protected: // auxiliary instance methods
    void paintAxisLabel(QPaintDevice* i_pPaintDevice);
private: // copy ctor not allowed
    CDiagObjAxisLabel( const CDiagObjAxisLabel& );
private: // assignment operator not allowed
    void operator=( const CDiagObjAxisLabel& );
protected:  // instance members (config values)
    /*!< Axis labels may only be bound to one scale object.
         The scale object is passed with the constructor and stored here.
         Please note that the referenced scale object is also store in the
         base class CDiagObj in member "m_arpDiagScale". But keeping a reference
         also in the axis label class simplifies the code. */
    CDiagScale* m_pDiagScale;
    /*!< Defines the space in pixels between the diagrams center area and the division line labels. */
    int m_iSpacingDiagPartCenter2DivLineLabels;
    /*!< Defines the space in pixels between the diagrams center area and the axis label.
         Usually equal to m_iSpaceDiagPartCenter2DivLineLabels. */
    int m_iSpacingDiagPartCenter2AxisLabel;
    /*!< Defines the space in pixels between the label at the division line nearest to
         the axis label and the axis label. */
    int m_iSpacingDivLineLabels2AxisLabel;
protected: // Division lines
    /*!< Defines the visibility of the division lines for each layer (Main/Sub).*/
    QVector<bool> m_arbShowDivLines;
    /*!< Foreground color used to paint the division lines. */
    QVector<QColor> m_arcolDivLines;
    /*!< Pen style used to paint the division lines. */
    QVector<Qt::PenStyle> m_arpenStyleDivLines;
protected: // Division line labels
    /*!< Defines the visibility of the division line labels for each layer (Main/Sub).*/
    QVector<bool> m_arbShowDivLineLabels;
    /*!< Foreground color used to paint the division lines. */
    QVector<QColor> m_arcolDivLineLabels;
    /*!< Fonts used to output the division lines labes for each layer (Main/Sub). */
    QVector<QFont> m_arfntDivLineLabels;
protected: // Axis label
    /*!< Axis label to be output (e.g. "Frequency"). */
    QString m_strAxisLabel;
    /*!< Defines whether the axis label should be indicated. */
    bool m_bShowAxisLabel;
    /*!< Text color for the axis label. */
    QColor m_colAxisLabel;
    /*!< Font used to output the axis label. */
    QFont m_fntAxisLabel;
protected: // Unit
    /*!< The unit to indicate the values may be explicitly set.
         If explicitly set the reference to the unit is stored here.
         If not explicitly set the "best unit" is used to indicate the values. */
    PhysVal::CUnit* m_pUnit;
    /*!< Calculated unit used to indicate the values. This may be either the "best unit"
         to indicate the values or the explicityl set unit from "m_pUnit" above. */
    PhysVal::CUnit m_unitLabels;
    /*!< Calculated unit string. Used to calculate the extent of the axis labels. */
    QString m_strPhysUnitLabels;
    /*!< Defines whether the unit should be output at the maximum scale value in the area of the division line labels. */
    bool m_bShowUnitAtDivLines;
    /*!< Text color for the unit at the maximum scale value. */
    QColor m_colUnitAtDivLines;
    /*!< Font used to output the unit at the maximum scale value. */
    QFont m_fntUnitAtDivLines;
    /*!< Defines whether the unit should be output together with the axis label. */
    bool m_bShowUnitAtAxisLabel;
    /*!< Text color to output the unit together with the axis label. */
    QColor m_colUnitAtAxisLabel;
    /*!< Font used to output the unit together with the axis label. */
    QFont m_fntUnitAtAxisLabel;
protected: // Division lines calculator
    /*!< Scale division lines instance to calculate the metrics of the axis labels. */
    ZS::System::GUI::Math::CScaleDivLinesMetrics m_divLinesMetrics;
protected: // instance members (calculated)
    /*!< If the unit string should be indicated in the area of the division line labels
         or if the minimum and maximum scale values have to be shown the maximum text
         extent may have to be adjusted. */
    QSize m_sizeDivLineLabelsMaxTextExtent;
    /*!< Calculated rectangle area in which the axis label will be output. */
    QRect m_rectAxisLabel;
    /*!< Calculated rectangle area needed to output the unit string if the unit
         string should be output in the division line labels part. */
    QRect m_rectDivLineLabelsPhysUnit;
    /*!< Flag to indicate during runtime whether the physical unit has already been output. */
    bool m_bDivLineLabelsPhysUnitVisible;
    /*!< Flags to indicate whether the scales minimum and/or maximum value should be output. */
    QVector<bool> m_arbScaleMinMaxValVisible;

}; // class CDiagObjAxisLabel

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagObjAxisLabel_h
