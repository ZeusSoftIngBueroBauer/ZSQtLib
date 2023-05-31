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
    void show( const CEnumDivLineLabelsPart& i_ePart, const CEnumDivLineLayer& i_eLayer );
    void hide( const CEnumDivLineLabelsPart& i_ePart, const CEnumDivLineLayer& i_eLayer );
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
    // Space between the diagrams center part and the division line labels
    void setSpaceDiagPartCenter2DivLineLabels( int i_iSpace );
    int getSpaceDiagPartCenter2DivLineLabels() const;
    // Space between the division line labels and the axis label
    void setSpaceDivLineLabels2AxisLabel( int i_iSpace );
    int getSpaceDivLineLabels2AxisLabel() const;
    // Space between the axis label and the diagram border
    void setSpaceDiagBorder2AxisLabel( int i_iSpace );
    int getSpaceDiagBorder2AxisLabel() const;
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
        const CEnumDivLineLayer& i_eLayer,
        int i_idxDivLineLabelMin,
        int i_idxDivLineLabelMax ) const;
protected: // auxiliary instance methods
    QRect getDiagRect() const;
    PhysVal::CUnit getAxisLabelUnit() const;
    bool isAxisLabelVisible() const;
    QRect getAxisLabelBoundingRect() const;
    bool intersectsDivLineLabelsPhysUnit(const QRect& i_rect) const;
    QString formatValue( double i_fVal, PhysVal::CUnit* i_pUnit = nullptr ) const;
private: // copy ctor not allowed
    CDiagObjAxisLabel( const CDiagObjAxisLabel& );
private: // assignment operator not allowed
    void operator=( const CDiagObjAxisLabel& );
protected:  // instance members
    CDiagScale* m_pDiagScale;
    /*!< nullptr means "use best unit" */
    PhysVal::CUnit* m_pUnit;
    /*!< Used to indicate the values (may differ from "m_pUnit") */
    PhysVal::CUnit m_unitLabels;
    QString m_strPhysUnitLabels;
    int m_iSpaceDiagPartCenter2DivLineLabels;
    int m_iSpaceDivLineLabels2AxisLabel;
    int m_iSpaceDiagBorder2AxisLabel;
    QVector<QVector<bool>> m_ararbShowPartsLayer;
    QVector<bool> m_arbShowPartsUnit;
    QVector<QColor> m_arcolPartsFg;
    QVector<Qt::PenStyle> m_arpenStyleParts;
    QVector<QColor> m_arcolPartsBg;
    QVector<Qt::BrushStyle> m_arbrushStyleParts;
    QVector<QFont> m_arfntParts;
    int m_iDivLineLabelsDigitsCountMax;
    bool m_bUseEngineeringFormat;
    QString m_strAxisLabel;
    QRect m_rectAxisLabel;
    int m_iDivLineLabelsTrailingDigits;
    int m_iDivLineLabelsExponentDigits;
    QRect m_rectDivLineLabelsMaxTextExtent;
    int m_cxDivLineLabelsSpace;
    QVector<int> m_ariDivLinesCount;
    QVector<QVector<int>> m_arariDivLines_px;
    QVector<QVector<QRect>> m_ararrectDivLineLabels;
    QVector<QVector<QString>> m_ararstrDivLineLabels;
    QVector<QVector<bool>> m_ararbDivLineLabelsVisible;
    QRect m_rectDivLineLabelsPhysUnit;
    bool m_bDivLineLabelsPhysUnitVisible;
    QVector<QString> m_arstrScaleMinMaxVal;
    QVector<QRect> m_arrectScaleMinMaxVal;
    QVector<bool> m_arbScaleMinMaxValVisible;

}; // class CDiagObjAxisLabel

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagObjAxisLabel_h
