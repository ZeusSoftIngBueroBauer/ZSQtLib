/*******************************************************************************

Copyright 2004 - 2009 by    ZeusSoft, Ing. Buero Bauer
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

#include "ZSDiagram/ZSDiagramDllMain.h"
#include "ZSDiagram/ZSDiagObj.h"

namespace ZS
{
namespace Diagram
{
//******************************************************************************
class ZSDIAGRAMDLL_API CDiagObjAxisLabel : public CDiagObj
//******************************************************************************
{
public: // type definitions and constants
    typedef enum {
        EPartMin           = 0,
        EPartDivLines      = 0,
        EPartDivLineLabels = 1,
        EPartAxisLabel     = 2,
        EPartMax           = 2,
        EPartCount,
        EPartUndefined
    }   EPart;
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
    void show( EPart i_part, EDivLineLayer i_layer );
    void hide( EPart i_part, EDivLineLayer i_layer );
    void showUnit( EPart i_part );
    void hideUnit( EPart i_part );
    void setColFg( EPart i_part, const QColor& i_col );
    QColor getColFg( EPart i_part ) const;
    void setPenStyle( EPart i_part, const Qt::PenStyle& i_penStyle );
    Qt::PenStyle getPenStyle( EPart i_part ) const;
    void setColBg( EPart i_part, const QColor& i_col );
    QColor getColBg( EPart i_part ) const;
    void setBrushStyle( EPart i_part, const Qt::BrushStyle& i_brushStyle );
    Qt::BrushStyle getBrushStyle( EPart i_part ) const;
    void setFont( EPart i_part, const QFont& i_fnt );
    QFont getFont( EPart i_part ) const;
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
    void addAxisLabelToBeConsidered( CDiagObjAxisLabel* i_pDiagObjAxisLabel );
    void removeAxisLabelToBeConsidered( const CDiagObjAxisLabel* i_pDiagObjAxisLabel );
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
        const QRect& i_rectDivLineLabel,
        int          i_iDivLineLayer,
        bool         i_bCheckAxisLabelsToBeConsidered,
        int          i_idxDivLineLabelMin = -1,
        int          i_idxDivLineLabelMax = -1 ) const;
    QString formatValue( double i_fVal, PhysVal::CUnit* i_pUnit = nullptr ) const;
private: // copy ctor not allowed
    CDiagObjAxisLabel( const CDiagObjAxisLabel& );
private: // assignment operator not allowed
    void operator=( const CDiagObjAxisLabel& );
protected:  // instance members
    CDiagScale*                 m_pDiagScale;
    PhysVal::CUnit*             m_pUnit;       // nullptr means "use best unit"
    PhysVal::CUnit*             m_pUnitLabels; // Used to indicate the values (may differ from "m_pUnit")
    QString                     m_strPhysUnitLabels;
    int                         m_iSpaceDiagPartCenter2DivLineLabels;
    int                         m_iSpaceDivLineLabels2AxisLabel;
    int                         m_iSpaceDiagBorder2AxisLabel;
    bool                        m_ararbShow[EPartCount][EDivLineLayerCount];
    bool                        m_arbShowUnit[EPartCount];
    QColor                      m_arcolFg[EPartCount];
    Qt::PenStyle                m_arpenStyle[EPartCount];
    QColor                      m_arcolBg[EPartCount];
    Qt::BrushStyle              m_arbrushStyle[EPartCount];
    QFont                       m_arfnt[EPartCount];
    int                         m_iDivLineLabelsDigitsCountMax;
    bool                        m_bUseEngineeringFormat;
    QString                     m_strAxisLabel;
    QRect                       m_rectAxisLabel;
    int                         m_iDivLineLabelsTrailingDigits;
    int                         m_iDivLineLabelsExponentDigits;
    QRect                       m_rectDivLineLabelsMaxTextExtent;
    int                         m_cxDivLineLabelsSpace;
    int                         m_ariDivLinesCount[EDivLineLayerCount];
    int*                        m_arpiDivLines[EDivLineLayerCount];
    QRect*                      m_arpRectDivLineLabels[EDivLineLayerCount];
    QString*                    m_arpStrDivLineLabels[EDivLineLayerCount];
    bool*                       m_arpbDivLineLabelsVisible[EDivLineLayerCount];
    QRect                       m_rectDivLineLabelsPhysUnit;
    bool                        m_bDivLineLabelsPhysUnitVisible;
    QString                     m_arstrScaleMinMaxVal[2];
    QRect                       m_arrectScaleMinMaxVal[2];
    bool                        m_arbScaleMinMaxValVisible[2];
    int                         m_iAxisLabelsToBeConsideredListSize;
    int                         m_iAxisLabelsToBeConsidered;
    CDiagObjAxisLabel**         m_arpAxisLabelsToBeConsidered;

}; // class CDiagObjAxisLabel

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagObjAxisLabel_h
