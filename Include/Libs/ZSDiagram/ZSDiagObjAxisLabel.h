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

#include <QtGui/qcolor.h>
#include <QtGui/qfont.h>

namespace ZS
{
namespace Diagram
{
//******************************************************************************
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
public: // instance methods
    void setUnit( PhysVal::CUnit* i_pUnit ); // nullptr means "use best unit"
    PhysVal::CUnit* getUnit();
    // Methods for adjusting and reading properties of (almost) all parts
    void show( const CEnumDivLineLabelsPart& i_ePart, const ZS::System::CEnumDivLineLayer& i_eLayer );
    void hide( const CEnumDivLineLabelsPart& i_ePart, const ZS::System::CEnumDivLineLayer& i_eLayer );
    void showUnit( const CEnumDivLineLabelsPart& i_ePart );
    void hideUnit( const CEnumDivLineLabelsPart& i_ePart );
    void setColFg( const CEnumDivLineLabelsPart& i_ePart, const QColor& i_col );
    QColor getColFg( const CEnumDivLineLabelsPart& i_ePart ) const;
    void setPenStyle( const CEnumDivLineLabelsPart& i_ePart, const Qt::PenStyle& i_penStyle );
    Qt::PenStyle getPenStyle( const CEnumDivLineLabelsPart& i_ePart ) const;
    void setColBg( const CEnumDivLineLabelsPart& i_ePart, const QColor& i_col );
    QColor getColBg( const CEnumDivLineLabelsPart& i_ePart ) const;
    void setBrushStyle( const CEnumDivLineLabelsPart& i_ePart, const Qt::BrushStyle& i_brushStyle );
    Qt::BrushStyle getBrushStyle( const CEnumDivLineLabelsPart& i_ePart ) const;
    void setFont( const CEnumDivLineLabelsPart& i_ePart, const QFont& i_fnt );
    QFont getFont( const CEnumDivLineLabelsPart& i_ePart ) const;
    // Methods affecting the labels at the division lines
    void setDigitsCountMax( int i_iDigitsCountMax );
    int getDigitsCountMax() const;
    void setUseEngineeringFormat( bool i_bUseEngineeringFormat );
    bool getUseEngineeringFormat() const;
    // String to be shown at the axis label
    void setAxisLabel( const QString& i_strLabel );
    QString getAxisLabel() const;
    void setSpaceDiagPartCenter2DivLineLabels( int i_iSpace );
    int getSpaceDiagPartCenter2DivLineLabels() const;
    void setSpaceDiagPartCenter2AxisLabel( int i_iSpace );
    int getSpaceDiagPartCenter2AxisLabel() const;
    void setSpaceDivLineLabels2AxisLabel( int i_iSpace );
    int getSpaceDivLineLabels2AxisLabel() const;
    //void addAxisLabelToBeConsidered( CDiagObjAxisLabel* i_pDiagObjAxisLabel );
    //void removeAxisLabelToBeConsidered( const CDiagObjAxisLabel* i_pDiagObjAxisLabel );
public: // overridables of base class CDiagObj
    virtual void show( bool i_bInformDiagram = true );
    virtual void hide( bool i_bInformDiagram = true );
    virtual QSize sizeHint();
public: // must overridables of base class CDiagObj
    virtual CDiagObj* clone( CDataDiagram* i_pDiagramTrg ) const;
    virtual void update( unsigned int i_uUpdateFlags, QPaintDevice* i_pPaintDevice = nullptr );
protected: // instance methods
    void updateLayout();
    void updateData();
    void updatePixmap( QPaintDevice* i_pPaintDevice );
    void updateWidget();
    bool intersectsDivLineLabels(
        const QRect& i_rect,
        const ZS::System::CEnumDivLineLayer& i_eLayer = ZS::System::CEnumDivLineLayer(),
        int i_idxDivLineLabelMin = -1,
        int i_idxDivLineLabelMax = -1) const;
protected: // auxiliary instance methods
    QRect getDiagRect() const;
    PhysVal::CUnit getAxisLabelUnit() const;
    bool isAxisLabelVisible() const;
    QRect getAxisLabelBoundingRect() const;
    QVector<double> getDivLineValsSorted() const;
    bool intersectsDivLineLabelsPhysUnit(const QRect& i_rect) const;
    QString formatValue(double i_fVal, PhysVal::CUnit* i_pUnit = nullptr) const;
private: // copy ctor not allowed
    CDiagObjAxisLabel( const CDiagObjAxisLabel& );
private: // assignment operator not allowed
    void operator=( const CDiagObjAxisLabel& );
protected:  // instance members
    /*!< Axis labels may only be bound to one scale object.
         The scale object is passed with the constructor and stored here.
         Please note that the referenced scale object is also store in the
         base class CDiagObj in member "m_arpDiagScale". But keeping a reference
         also in the axis label class simplifies the code. */
    CDiagScale* m_pDiagScale;
    /*!< The unit to indicate the values may be explicitly set.
         If explicitly set the reference to the unit is stored here.
         If not explicitly set the "best unit" is used to indicate the values. */
    PhysVal::CUnit* m_pUnit;
    /*!< Calculated unit used to indicate the values. This may be either the "best unit"
         to indicate the values or the explicityl set unit from "m_pUnit" above. */
    PhysVal::CUnit m_unitLabels;
    /*!< Calculated unit string. Used to calculate the extent of the axis labels. */
    QString m_strPhysUnitLabels;
    /*!< Axis label to be output (e.g. "Frequency"). */
    QString m_strAxisLabel;
    /*!< Defines the space in pixels between the diagrams center area and the division line labels. */
    int m_iSpaceDiagPartCenter2DivLineLabels;
    /*!< Defines the space in pixels between the diagrams center area and the axis label.
         Usually equal to m_iSpaceDiagPartCenter2DivLineLabels. */
    int m_iSpaceDiagPartCenter2AxisLabel;
    /*!< Defines the space in pixels between the label at the division line nearest to
         the axis label and the axis label. */
    int m_iSpaceDivLineLabels2AxisLabel;
    /*!< Defines the visibility for each part (Lines, Labels, AxisLabel) and layer (Main/Sub).
         First dimension of the vector is EDivLineLabelsPart, second dimension is EDivLineLayer. */
    QVector<QVector<bool>> m_ararbShowPartsLayer;
    /*!< Defines in which area the unit is indicated (Lines, Labels, AxisLabel).
         Please note that axis labels are never output in the Sub-Layer. */
    QVector<bool> m_arbShowPartsUnit;
    /*!< Foreground color (e.g. used to output the text) used in the parts. */
    QVector<QColor> m_arcolPartsFg;
    /*!< Pen style used in the parts. */
    QVector<Qt::PenStyle> m_arpenStyleParts;
    /*!< Background color to be used in the parts. */
    QVector<QColor> m_arcolPartsBg;
    /*!< Brush style used in the parts. */
    QVector<Qt::BrushStyle> m_arbrushStyleParts;
    /*!< Fonts used in the parts. */
    QVector<QFont> m_arfntParts;
    /*!< If greater than 0 defines the maximum number of digits which may be used to
         output the division line labels. If the sum of trailing and leading digits
         would exceed the maximum number of digits for the mantissa the division lines
         will be labeled in engineering format. */
    int m_iDivLineLabelsDigitsCountMax;
    /*!< If set to true the division line labels will be output in engineering format. */
    bool m_bUseEngineeringFormat;
    /*!< Calculated rectangle area in which the axis label will be output. */
    QRect m_rectAxisLabel;
    /*!< Calculated number of trailing digits used to indicate the division line labels. */
    int m_iDivLineLabelsTrailingDigits;
    /*!< Calculated number of exponent digits used to indicate the division line labels. */
    int m_iDivLineLabelsExponentDigits;
    /*!< Calculated rectangle area of the "longest" division line label. */
    QRect m_rectDivLineLabelsMaxTextExtent;
    /*!< Space between two successive division line labels. Calculated depending on the used font. */
    int m_cxDivLineLabelsSpace;
    /*!< The number of division lines is calculated by the scale object bound
         to the axis label object. For fast access the numbers are stored here. */
    QVector<int> m_ariDivLinesCount;
    /*!< The pixel position of the division lines is calculated by scale object bound
         to the axis label object. For fast access the numbers are stored here. */
    QVector<QVector<int>> m_arariDivLines_px;
    /*!< Calculated rectangle areas (bounding rectangles) of the division line labels. */
    QVector<QVector<QRect>> m_ararrectDivLineLabels;
    /*!< Calculated texts of the division line labels. */
    QVector<QVector<QString>> m_ararstrDivLineLabels;
    /*!< Flag for each division line label to indicate whether the label should be output.
         If a successive label would overlap a preceding label the overlapping label
         is not not shown. */
    QVector<QVector<bool>> m_ararbDivLineLabelsVisible;
    /*!< Calculated rectangle area needed to output the unit string if the unit
         string should be output in the division line labels part. */
    QRect m_rectDivLineLabelsPhysUnit;
    /*!< Flag to indicate during runtime whether the physical unit has already been output. */
    bool m_bDivLineLabelsPhysUnitVisible;
    /*!< Calculated texts for the minimum and maximum values of the bound scale. */
    QVector<QString> m_arstrScaleMinMaxVal;
    /*!< Calculated rectangle areas (bounding rectangles) of the minimum and maximum values. */
    QVector<QRect> m_arrectScaleMinMaxVal;
    /*!< Flags to indicate whether the scales minimum and/or maximum value should be output. */
    QVector<bool> m_arbScaleMinMaxValVisible;

}; // class CDiagObjAxisLabel

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagObjAxisLabel_h
