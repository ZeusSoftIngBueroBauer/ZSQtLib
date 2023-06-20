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

#include "ZSDiagram/ZSDiagObjAxisLabel.h"
#include "ZSDiagram/ZSDiagramProcPixmap.h"
#include "ZSDiagram/ZSDiagScale.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtGui/qpainter.h>

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Diagram;
using namespace ZS::PhysVal;


/*******************************************************************************
class CDiagObjAxisLabel : public CDiagObj
*******************************************************************************/

#ifdef __linux__
const int c_iMaxTextExtentAddHeight = 4; // unter Linux werden sonst die Labels abgeschnitten
const int c_iMaxTextExtentAddWidth  = 0;
#else
const int c_iMaxTextExtentAddHeight = 0;
const int c_iMaxTextExtentAddWidth  = 0;
#endif

// To make the code more readable:
const int EDivLineLabelsPartLines = static_cast<int>(EDivLineLabelsPart::Lines);
const int EDivLineLabelsPartLabels = static_cast<int>(EDivLineLabelsPart::Labels);
const int EDivLineLabelsPartAxisLabel = static_cast<int>(EDivLineLabelsPart::AxisLabel);

//lint -e834

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDiagObjAxisLabel::CDiagObjAxisLabel(
    const QString& i_strObjName,
    CDiagScale*    i_pDiagScale,
    ELayoutPos     i_layoutPos,
    const QString& i_strAxisLabel ) :
//------------------------------------------------------------------------------
    CDiagObj(
        /* strClassName */ CDiagObjAxisLabel::ClassName(),
        /* strObjName   */ i_strObjName,
        /* pDiagScaleX  */ nullptr,
        /* pDiagScaleY  */ nullptr,
        /* layoutPos    */ i_layoutPos ),
    m_pDiagScale(i_pDiagScale),
    m_pUnit(nullptr), // as default: "use best unit"
    m_unitLabels(i_pDiagScale->getScale().unit()),
    m_strPhysUnitLabels(),
    m_strAxisLabel(i_strAxisLabel),
    m_iSpaceDiagPartCenter2DivLineLabels(0),
    m_iSpaceDiagPartCenter2AxisLabel(0),
    m_iSpaceDivLineLabels2AxisLabel(0),
    m_ararbShowPartsLayer(CEnumDivLineLabelsPart::count(), QVector<bool>(CEnumDivLineLayer::count(), false)),
    m_arbShowPartsUnit(CEnumDivLineLabelsPart::count(), false),
    m_arcolPartsFg(CEnumDivLineLabelsPart::count(), Qt::black),
    m_arpenStyleParts(CEnumDivLineLabelsPart::count(), Qt::SolidLine),
    m_arcolPartsBg(CEnumDivLineLabelsPart::count(), Qt::gray),
    m_arbrushStyleParts(CEnumDivLineLabelsPart::count(), Qt::NoBrush),
    m_arfntParts(CEnumDivLineLabelsPart::count()),
    m_iDivLineLabelsDigitsCountMax(-1),
    m_bUseEngineeringFormat(false),
    m_rectAxisLabel(),
    m_iDivLineLabelsTrailingDigits(1),
    m_iDivLineLabelsExponentDigits(0),
    m_rectDivLineLabelsMaxTextExtent(),
    m_cxDivLineLabelsSpace(8),
    m_ariDivLinesCount(CEnumDivLineLayer::count(), 0),
    m_arariDivLines_px(CEnumDivLineLayer::count(), QVector<int>(0)),
    m_ararrectDivLineLabels(CEnumDivLineLayer::count(), QVector<QRect>(0)),
    m_ararstrDivLineLabels(CEnumDivLineLayer::count(), QVector<QString>(0)),
    m_ararbDivLineLabelsVisible(CEnumDivLineLayer::count(), QVector<bool>(0)),
    m_rectDivLineLabelsPhysUnit(),
    m_bDivLineLabelsPhysUnitVisible(false),
    m_arstrScaleMinMaxVal(2),
    m_arrectScaleMinMaxVal(2),
    m_arbScaleMinMaxValVisible(2, false)
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    // The base class CDiagObj administers two scale objects: ScaleX and ScaleY.
    // The axis label just needs one of those scale objects. To avoid if/else
    // statements which scale object is needed by the axis label object depending
    // on the scale direction the used scale object is stored within the axis
    // label class. But the diagram needs to know to which scale object the
    // axis label object is linked to. So we need to set the member element of
    // the base class correspondingly.
    switch( m_pDiagScale->getScaleDir() )
    {
        case EScaleDir::X:
        {
            m_arpDiagScale[static_cast<int>(EScaleDir::X)] = m_pDiagScale;
            break;
        }
        case EScaleDir::Y:
        {
            m_arpDiagScale[static_cast<int>(EScaleDir::Y)] = m_pDiagScale;
            break;
        }
        default:
        {
            break;
        }
    }

    // Overwrite some default values with more useful values.
    // As default all possible parts are shown for the main layer.
    m_ararbShowPartsLayer[EDivLineLabelsPartLines][EDivLineLayerMain] = true;
    m_ararbShowPartsLayer[EDivLineLabelsPartLabels][EDivLineLayerMain] = true;
    if (!i_strAxisLabel.isEmpty()) {
        m_ararbShowPartsLayer[EDivLineLabelsPartAxisLabel][EDivLineLayerMain] = true;
    }

    // As default the unit string is output in the area of the axis label.
    // If a valid label string is passed (e.g. "Frequency") the unit will be
    // appended to this string and indicated together with the axis label.
    m_arbShowPartsUnit[EDivLineLabelsPartAxisLabel] = true;

    switch (i_layoutPos)
    {
        case ELayoutPosTop:
        case ELayoutPosBottom:
        {
            m_iSpaceDiagPartCenter2DivLineLabels = 6;
            m_iSpaceDiagPartCenter2AxisLabel = 6;
            m_iSpaceDivLineLabels2AxisLabel = 4;
            break;
        }
        case ELayoutPosLeft:
        case ELayoutPosRight:
        {
            m_iSpaceDiagPartCenter2DivLineLabels = 4;
            m_iSpaceDiagPartCenter2AxisLabel = 4;
            m_iSpaceDivLineLabels2AxisLabel = 2;
            break;
        }
        case ELayoutPosCenter:
        {
            m_iSpaceDiagPartCenter2DivLineLabels = 0;
            m_iSpaceDiagPartCenter2AxisLabel = 0;
            m_iSpaceDivLineLabels2AxisLabel = 0;
            break;
        }
        default:
        {
            break;
        }
    }
} // ctor

//------------------------------------------------------------------------------
CDiagObjAxisLabel::~CDiagObjAxisLabel()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_pDiagScale = nullptr;
    m_pUnit = nullptr;
    //m_unitLabels;
    //m_strPhysUnitLabels;
    //m_strAxisLabel;
    m_iSpaceDiagPartCenter2DivLineLabels = 0;
    m_iSpaceDiagPartCenter2AxisLabel = 0;
    m_iSpaceDivLineLabels2AxisLabel = 0;
    //m_ararbShowPartsLayer.clear();
    //m_arbShowPartsUnit.clear();
    //m_arcolPartsFg.clear();
    //m_arpenStyleParts.clear();
    //m_arcolPartsBg.clear();
    //m_arbrushStyleParts.clear();
    //m_arfntParts.clear();
    m_iDivLineLabelsDigitsCountMax = 0;
    m_bUseEngineeringFormat = false;
    //m_rectAxisLabel;
    m_iDivLineLabelsTrailingDigits = 0;
    m_iDivLineLabelsExponentDigits = 0;
    //m_rectDivLineLabelsMaxTextExtent;
    m_cxDivLineLabelsSpace = 0;
    //m_ariDivLinesCount.clear();
    //m_arariDivLines_px.clear();
    //m_ararrectDivLineLabels.clear();
    //m_ararstrDivLineLabels.clear();
    //m_ararbDivLineLabelsVisible.clear();
    //m_rectDivLineLabelsPhysUnit;
    m_bDivLineLabelsPhysUnitVisible = false;
    //m_arstrScaleMinMaxVal.clear();
    //m_arrectScaleMinMaxVal.clear();
    //m_arbScaleMinMaxValVisible.clear();

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

// Methods affecting the division lines, the labels at the division lines and the axis label
//==============================================================================

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::setUnit( CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    m_pUnit = i_pUnit;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
CUnit* CDiagObjAxisLabel::getUnit()
//------------------------------------------------------------------------------
{
    return m_pUnit;
}

// Methods for adjusting and reading properties of (almost) all parts
//==============================================================================

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::show(
    const CEnumDivLineLabelsPart& i_ePart, const CEnumDivLineLayer& i_eLayer )
//------------------------------------------------------------------------------
{
    int iPartMin = i_ePart.enumeratorAsInt();
    int iPartMax = i_ePart.enumeratorAsInt();
    int iLayerMin = i_eLayer.enumeratorAsInt();
    int iLayerMax = i_eLayer.enumeratorAsInt();

    if( i_ePart.enumeratorAsInt() == CEnumDivLineLabelsPart::count() )
    {
        iPartMin = 0;
        iPartMax = CEnumDivLineLabelsPart::count()-1;
    }
    if( i_eLayer.enumeratorAsInt() == CEnumDivLineLayer::count() )
    {
        iLayerMin = 0;
        iLayerMax = CEnumDivLineLayer::count()-1;
    }

    for( int iPart = iPartMin; iPart <= iPartMax; iPart++ )
    {
        for( int iLayer = iLayerMin; iLayer <= iLayerMax; iLayer++ )
        {
            if( !m_ararbShowPartsLayer[iPart][iLayer] )
            {
                m_ararbShowPartsLayer[iPart][iLayer] = true;
                invalidate(EUpdateLayoutDataPixmapWidget,true);
            }
        }
    }
} // show

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::hide(
    const CEnumDivLineLabelsPart& i_ePart, const CEnumDivLineLayer& i_eLayer )
//------------------------------------------------------------------------------
{
    int iPartMin = i_ePart.enumeratorAsInt();
    int iPartMax = i_ePart.enumeratorAsInt();
    int iLayerMin = i_eLayer.enumeratorAsInt();
    int iLayerMax = i_eLayer.enumeratorAsInt();

    if( i_ePart.enumeratorAsInt() == CEnumDivLineLabelsPart::count() )
    {
        iPartMin = 0;
        iPartMax = CEnumDivLineLabelsPart::count()-1;
    }
    if( i_eLayer.enumeratorAsInt() == CEnumDivLineLayer::count() )
    {
        iLayerMin = 0;
        iLayerMax = CEnumDivLineLayer::count()-1;
    }

    for( int iPart = iPartMin; iPart <= iPartMax; iPart++ )
    {
        for( int iLayer = iLayerMin; iLayer <= iLayerMax; iLayer++ )
        {
            if( m_ararbShowPartsLayer[iPart][iLayer] )
            {
                m_ararbShowPartsLayer[iPart][iLayer] = false;
                invalidate(EUpdateLayoutDataPixmapWidget,true);
            }
        }
    }
} // hide

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::showUnit( const CEnumDivLineLabelsPart& i_ePart )
//------------------------------------------------------------------------------
{
    int iPartMin = i_ePart.enumeratorAsInt();
    int iPartMax = i_ePart.enumeratorAsInt();

    if( i_ePart.enumeratorAsInt() == CEnumDivLineLabelsPart::count() )
    {
        iPartMin = 0;
        iPartMax = CEnumDivLineLabelsPart::count()-1;
    }

    for( int iPart = iPartMin; iPart <= iPartMax; iPart++ )
    {
        if( !m_arbShowPartsUnit[iPart] )
        {
            m_arbShowPartsUnit[iPart] = true;
            invalidate(EUpdateLayoutDataPixmapWidget,true);
        }
    }
} // showUnit

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::hideUnit( const CEnumDivLineLabelsPart& i_ePart )
//------------------------------------------------------------------------------
{
    int iPartMin = i_ePart.enumeratorAsInt();
    int iPartMax = i_ePart.enumeratorAsInt();

    if( i_ePart.enumeratorAsInt() == CEnumDivLineLabelsPart::count() )
    {
        iPartMin = 0;
        iPartMax = CEnumDivLineLabelsPart::count()-1;
    }

    for( int iPart = iPartMin; iPart <= iPartMax; iPart++ )
    {
        if( m_arbShowPartsUnit[iPart] )
        {
            m_arbShowPartsUnit[iPart] = false;
            invalidate(EUpdateLayoutDataPixmapWidget,true);
        }
    }
} // hideUnit

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::setColFg( const CEnumDivLineLabelsPart& i_ePart, const QColor& i_col )
//------------------------------------------------------------------------------
{
    int iPartMin = i_ePart.enumeratorAsInt();
    int iPartMax = i_ePart.enumeratorAsInt();

    if( i_ePart.enumeratorAsInt() == CEnumDivLineLabelsPart::count() )
    {
        iPartMin = 0;
        iPartMax = CEnumDivLineLabelsPart::count()-1;
    }

    for( int iPart = iPartMin; iPart <= iPartMax; iPart++ )
    {
        if( m_arcolPartsFg[iPart] != i_col )
        {
            m_arcolPartsFg[iPart] = i_col;
            invalidate(EUpdatePixmapWidget,true);
        }
    }
} // setCol

//------------------------------------------------------------------------------
QColor CDiagObjAxisLabel::getColFg( const CEnumDivLineLabelsPart& i_ePart ) const
//------------------------------------------------------------------------------
{
    return m_arcolPartsFg[i_ePart.enumeratorAsInt()];
}

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::setPenStyle( const CEnumDivLineLabelsPart& i_ePart, const Qt::PenStyle& i_penStyle )
//------------------------------------------------------------------------------
{
    int iPartMin = i_ePart.enumeratorAsInt();
    int iPartMax = i_ePart.enumeratorAsInt();

    if( i_ePart.enumeratorAsInt() == CEnumDivLineLabelsPart::count() )
    {
        iPartMin = 0;
        iPartMax = CEnumDivLineLabelsPart::count()-1;
    }

    for( int iPart = iPartMin; iPart <= iPartMax; iPart++ )
    {
        if( m_arpenStyleParts[iPart] != i_penStyle )
        {
            m_arpenStyleParts[iPart] = i_penStyle;
            invalidate(EUpdatePixmapWidget,true);
        }
    }
} // setPenStyle

//------------------------------------------------------------------------------
Qt::PenStyle CDiagObjAxisLabel::getPenStyle( const CEnumDivLineLabelsPart& i_ePart ) const
//------------------------------------------------------------------------------
{
    return m_arpenStyleParts[i_ePart.enumeratorAsInt()];
}

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::setColBg( const CEnumDivLineLabelsPart& i_ePart, const QColor& i_col )
//------------------------------------------------------------------------------
{
    int iPartMin = i_ePart.enumeratorAsInt();
    int iPartMax = i_ePart.enumeratorAsInt();

    if( i_ePart.enumeratorAsInt() == CEnumDivLineLabelsPart::count() )
    {
        iPartMin = 0;
        iPartMax = CEnumDivLineLabelsPart::count()-1;
    }

    for( int iPart = iPartMin; iPart <= iPartMax; iPart++ )
    {
        if( m_arcolPartsBg[iPart] != i_col )
        {
            m_arcolPartsBg[iPart] = i_col;
            invalidate(EUpdatePixmapWidget,true);
        }
    }
} // setColBg

//------------------------------------------------------------------------------
QColor CDiagObjAxisLabel::getColBg( const CEnumDivLineLabelsPart& i_ePart ) const
//------------------------------------------------------------------------------
{
    return m_arcolPartsBg[i_ePart.enumeratorAsInt()];
}

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::setBrushStyle( const CEnumDivLineLabelsPart& i_ePart, const Qt::BrushStyle& i_brushStyle )
//------------------------------------------------------------------------------
{
    int iPartMin = i_ePart.enumeratorAsInt();
    int iPartMax = i_ePart.enumeratorAsInt();

    if( i_ePart.enumeratorAsInt() == CEnumDivLineLabelsPart::count() )
    {
        iPartMin = 0;
        iPartMax = CEnumDivLineLabelsPart::count()-1;
    }

    for( int iPart = iPartMin; iPart <= iPartMax; iPart++ )
    {
        if( m_arbrushStyleParts[iPart] != i_brushStyle )
        {
            m_arbrushStyleParts[iPart] = i_brushStyle;
            invalidate(EUpdatePixmapWidget,true);
        }
    }
} // setBrushStyle

//------------------------------------------------------------------------------
Qt::BrushStyle CDiagObjAxisLabel::getBrushStyle( const CEnumDivLineLabelsPart& i_ePart ) const
//------------------------------------------------------------------------------
{
    return m_arbrushStyleParts[i_ePart.enumeratorAsInt()];
}

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::setFont( const CEnumDivLineLabelsPart& i_ePart, const QFont& i_fnt )
//------------------------------------------------------------------------------
{
    int iPartMin = i_ePart.enumeratorAsInt();
    int iPartMax = i_ePart.enumeratorAsInt();

    if( i_ePart.enumeratorAsInt() == CEnumDivLineLabelsPart::count() )
    {
        iPartMin = 0;
        iPartMax = CEnumDivLineLabelsPart::count()-1;
    }

    for( int iPart = iPartMin; iPart <= iPartMax; iPart++ )
    {
        m_arfntParts[iPart] = i_fnt;
        invalidate(EUpdateLayoutDataPixmapWidget,true);
    }
} // setFont

//------------------------------------------------------------------------------
QFont CDiagObjAxisLabel::getFont( const CEnumDivLineLabelsPart& i_ePart ) const
//------------------------------------------------------------------------------
{
    return m_arfntParts[i_ePart.enumeratorAsInt()];
}

// Methods affecting the labels at the division lines
//==============================================================================

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::setDigitsCountMax( int i_iDigitsCountMax )
//------------------------------------------------------------------------------
{
    m_iDivLineLabelsDigitsCountMax = i_iDigitsCountMax;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
int CDiagObjAxisLabel::getDigitsCountMax() const
//------------------------------------------------------------------------------
{
    return m_iDivLineLabelsDigitsCountMax;
}

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::setUseEngineeringFormat( bool i_bUseEngineeringFormat )
//------------------------------------------------------------------------------
{
    m_bUseEngineeringFormat = i_bUseEngineeringFormat;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
bool CDiagObjAxisLabel::getUseEngineeringFormat() const
//------------------------------------------------------------------------------
{
    return m_bUseEngineeringFormat;
}

// String to be shown at the axis label
//==============================================================================

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::setAxisLabel( const QString& i_strLabel )
//------------------------------------------------------------------------------
{
    m_strAxisLabel = i_strLabel;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
QString CDiagObjAxisLabel::getAxisLabel() const
//------------------------------------------------------------------------------
{
    return m_strAxisLabel;
}

// Space between the diagrams center part and the division line labels
//==============================================================================

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::setSpaceDiagPartCenter2DivLineLabels( int i_iSpace )
//------------------------------------------------------------------------------
{
    m_iSpaceDiagPartCenter2DivLineLabels = i_iSpace;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
int CDiagObjAxisLabel::getSpaceDiagPartCenter2DivLineLabels() const
//------------------------------------------------------------------------------
{
    return m_iSpaceDiagPartCenter2DivLineLabels;
}

// Space between the diagrams center part and the axis label
//==============================================================================

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::setSpaceDiagPartCenter2AxisLabel( int i_iSpace )
//------------------------------------------------------------------------------
{
    m_iSpaceDiagPartCenter2AxisLabel = i_iSpace;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
int CDiagObjAxisLabel::getSpaceDiagPartCenter2AxisLabel() const
//------------------------------------------------------------------------------
{
    return m_iSpaceDiagPartCenter2AxisLabel;
}

// Space between the division line labels and the axis label
//==============================================================================

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::setSpaceDivLineLabels2AxisLabel( int i_iSpace )
//------------------------------------------------------------------------------
{
    m_iSpaceDivLineLabels2AxisLabel = i_iSpace;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
int CDiagObjAxisLabel::getSpaceDivLineLabels2AxisLabel() const
//------------------------------------------------------------------------------
{
    return m_iSpaceDivLineLabels2AxisLabel;
}

////------------------------------------------------------------------------------
//void CDiagObjAxisLabel::addAxisLabelToBeConsidered( CDiagObjAxisLabel* i_pDiagObjAxisLabel )
////------------------------------------------------------------------------------
//{
//    if( m_arpAxisLabelsToBeConsidered == nullptr )
//    {
//        m_iAxisLabelsToBeConsideredListSize = 2;
//        m_iAxisLabelsToBeConsidered = 0;
//        m_arpAxisLabelsToBeConsidered = new CDiagObjAxisLabel*[m_iAxisLabelsToBeConsideredListSize];
//
//        for( int idxAxisLabel = 0; idxAxisLabel < m_iAxisLabelsToBeConsideredListSize; idxAxisLabel++ )
//        {
//            m_arpAxisLabelsToBeConsidered[idxAxisLabel] = nullptr;
//        }
//    }
//    else if( m_iAxisLabelsToBeConsidered == m_iAxisLabelsToBeConsideredListSize )
//    {
//        CDiagObjAxisLabel** arpAxisLabelsToBeConsidered = m_arpAxisLabelsToBeConsidered;
//        int iAxisLabelsToBeConsideredListSize = m_iAxisLabelsToBeConsideredListSize;
//
//        m_iAxisLabelsToBeConsideredListSize += 2;
//        m_arpAxisLabelsToBeConsidered = new CDiagObjAxisLabel*[m_iAxisLabelsToBeConsideredListSize];
//
//        int idxAxisLabel = 0;
//        for( ; idxAxisLabel < iAxisLabelsToBeConsideredListSize; idxAxisLabel++ )
//        {
//            m_arpAxisLabelsToBeConsidered[idxAxisLabel] = arpAxisLabelsToBeConsidered[idxAxisLabel];
//        }
//        for( ; idxAxisLabel < m_iAxisLabelsToBeConsideredListSize; idxAxisLabel++ )
//        {
//            m_arpAxisLabelsToBeConsidered[idxAxisLabel] = nullptr;
//        }
//        delete arpAxisLabelsToBeConsidered;
//        arpAxisLabelsToBeConsidered = nullptr;
//    }
//    m_arpAxisLabelsToBeConsidered[m_iAxisLabelsToBeConsidered] = i_pDiagObjAxisLabel;
//    m_iAxisLabelsToBeConsidered++;
//
//} // addAxisLabelToBeConsidered
//
////------------------------------------------------------------------------------
//void CDiagObjAxisLabel::removeAxisLabelToBeConsidered( const CDiagObjAxisLabel* i_pDiagObjAxisLabel )
////------------------------------------------------------------------------------
//{
//    if( m_arpAxisLabelsToBeConsidered == nullptr )
//    {
//        return;
//    }
//
//    int idxAxisLabel;
//
//    for( idxAxisLabel = 0; idxAxisLabel < m_iAxisLabelsToBeConsidered; idxAxisLabel++ )
//    {
//        if( m_arpAxisLabelsToBeConsidered[idxAxisLabel] == i_pDiagObjAxisLabel )
//        {
//            break;
//        }
//    }
//    if( idxAxisLabel < m_iAxisLabelsToBeConsidered )
//    {
//        for( ; idxAxisLabel < m_iAxisLabelsToBeConsidered-1; idxAxisLabel++ )
//        {
//            m_arpAxisLabelsToBeConsidered[idxAxisLabel] = m_arpAxisLabelsToBeConsidered[idxAxisLabel+1];
//        }
//        m_iAxisLabelsToBeConsidered--;
//    }
//
//} // removeAxisLabelToBeConsidered

/*==============================================================================
public: // overridables of base class CDiagObj
==============================================================================*/

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::show( bool i_bInformDiagram )
//------------------------------------------------------------------------------
{
    CDiagObj::show(i_bInformDiagram);
}

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::hide( bool i_bInformDiagram )
//------------------------------------------------------------------------------
{
    CDiagObj::hide(i_bInformDiagram);
}

//------------------------------------------------------------------------------
QSize CDiagObjAxisLabel::sizeHint()
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "sizeHint",
        /* strAddInfo   */ "" );

    int cxWidth  = 0;
    int cyHeight = 0;

    if( m_pDiagram == nullptr )
    {
        return QSize(0,0);
    }

    if( isVisible() )
    {
        const CPixmapDiagram* pPixmapDiagram = nullptr;

        // As a matter of fact there is no sense in adding an axis label object to
        // a diagram just designed to analyze data.
        if( m_pDiagram->getUpdateType() >= EDiagramUpdateTypePixmap )
        {
            pPixmapDiagram = dynamic_cast<const CPixmapDiagram*>(m_pDiagram);
        }
        if( pPixmapDiagram != nullptr )
        {
            if( m_pDiagScale->getScaleDir() == EScaleDir::X )
            {
                cxWidth  = pPixmapDiagram->getWidth();
                cyHeight = 0;

                if( m_ararbShowPartsLayer[EDivLineLabelsPartLabels][EDivLineLayerMain] )
                {
                    if( m_iSpaceDiagPartCenter2DivLineLabels >= 0 )
                    {
                        cyHeight += m_rectDivLineLabelsMaxTextExtent.height() + m_iSpaceDiagPartCenter2DivLineLabels /*+ c_iMaxTextExtentAddHeight*/;
                    }
                }
                if( m_ararbShowPartsLayer[EDivLineLabelsPartLabels][EDivLineLayerMain]
                    && ( m_ararbShowPartsLayer[EDivLineLabelsPartAxisLabel][EDivLineLayerMain]
                      || m_arbShowPartsUnit[EDivLineLabelsPartAxisLabel] ) )
                {
                    if( m_iSpaceDiagPartCenter2DivLineLabels >= 0 )
                    {
                        cyHeight += m_iSpaceDivLineLabels2AxisLabel;
                    }
                    else
                    {
                        cyHeight += (m_iSpaceDiagPartCenter2DivLineLabels - m_iSpaceDivLineLabels2AxisLabel);
                    }
                }
                if( m_ararbShowPartsLayer[EDivLineLabelsPartAxisLabel][EDivLineLayerMain] || m_arbShowPartsUnit[EDivLineLabelsPartAxisLabel] )
                {
                    cyHeight += (m_rectAxisLabel.height() /*+ c_iMaxTextExtentAddHeight*/ + m_iSpaceDiagPartCenter2AxisLabel);
                }
            }
            else if( m_pDiagScale->getScaleDir() == EScaleDir::Y )
            {
                cyHeight = pPixmapDiagram->getHeight();
                cxWidth  = 0;

                if( m_ararbShowPartsLayer[EDivLineLabelsPartLabels][EDivLineLayerMain] )
                {
                    if( m_iSpaceDiagPartCenter2DivLineLabels >= 0 )
                    {
                        cxWidth = m_rectDivLineLabelsMaxTextExtent.width();

                        if( m_arrectScaleMinMaxVal[0].width() > cxWidth )
                        {
                            cxWidth = m_arrectScaleMinMaxVal[0].width();
                        }
                        if( m_arrectScaleMinMaxVal[1].width() > cxWidth )
                        {
                            cxWidth = m_arrectScaleMinMaxVal[1].width();
                        }
                        if( m_rectDivLineLabelsPhysUnit.width() > cxWidth )
                        {
                            cxWidth = m_rectDivLineLabelsPhysUnit.width();
                        }
                        cxWidth += (m_iSpaceDiagPartCenter2DivLineLabels /*+ c_iMaxTextExtentAddWidth*/);
                    }
                }
                if( m_ararbShowPartsLayer[EDivLineLabelsPartLabels][EDivLineLayerMain]
                    && ( m_ararbShowPartsLayer[EDivLineLabelsPartAxisLabel][EDivLineLayerMain]
                      || m_arbShowPartsUnit[EDivLineLabelsPartAxisLabel] ) )
                {
                    if( m_iSpaceDiagPartCenter2DivLineLabels >= 0 )
                    {
                        cxWidth += m_iSpaceDivLineLabels2AxisLabel;
                    }
                    else
                    {
                        cxWidth += (m_iSpaceDiagPartCenter2DivLineLabels-m_iSpaceDivLineLabels2AxisLabel);
                    }
                }
                if( m_ararbShowPartsLayer[EDivLineLabelsPartAxisLabel][EDivLineLayerMain] || m_arbShowPartsUnit[EDivLineLabelsPartAxisLabel] )
                {
                    cxWidth += (m_rectAxisLabel.width() + m_iSpaceDiagPartCenter2AxisLabel /*+ c_iMaxTextExtentAddWidth*/);
                }
            }
        } // if( pPixmapDiagram != nullptr )
    } // if( isVisible() )

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strTrcMsg  = "Size[w,h]=";
        strTrcMsg += QString::number(cxWidth) + ",";
        strTrcMsg += QString::number(cyHeight);
        mthTracer.setMethodReturn(strTrcMsg);
    }
    return QSize(cxWidth,cyHeight);

} // sizeHint

/*==============================================================================
public: // must overridables of base class CDiagObj
==============================================================================*/

//------------------------------------------------------------------------------
CDiagObj* CDiagObjAxisLabel::clone( CDataDiagram* i_pDiagramTrg ) const
//------------------------------------------------------------------------------
{
    if( i_pDiagramTrg == nullptr || m_pDiagScale == nullptr )
    {
        return nullptr;
    }

    CDiagScale* pDiagScale = i_pDiagramTrg->findDiagScale(m_pDiagScale->getObjName());

    CDiagObjAxisLabel* pDiagObjCloned = new CDiagObjAxisLabel(
        /* strObjName   */ m_strObjName,
        /* pDiagScale   */ pDiagScale,
        /* layoutPos    */ m_layoutPos,
        /* strAxisLabel */ m_strAxisLabel );

    // Members from base class CDiagObj:
    pDiagObjCloned->m_rectContent = m_rectContent;
    pDiagObjCloned->m_bAdjustContentRect2DiagPartCenter = m_bAdjustContentRect2DiagPartCenter;
    pDiagObjCloned->m_bVisible = m_bVisible;
    pDiagObjCloned->m_state = m_state;
    pDiagObjCloned->m_bIsFocusable = m_bIsFocusable;
    pDiagObjCloned->m_bIsEditable = m_bIsEditable;

    // Members from this class:
    pDiagObjCloned->m_pUnit = m_pUnit;
    pDiagObjCloned->m_unitLabels = m_unitLabels;
    pDiagObjCloned->m_strPhysUnitLabels = m_strPhysUnitLabels;
    pDiagObjCloned->m_iSpaceDiagPartCenter2DivLineLabels = m_iSpaceDiagPartCenter2DivLineLabels;
    pDiagObjCloned->m_iSpaceDiagPartCenter2AxisLabel = m_iSpaceDiagPartCenter2AxisLabel;
    pDiagObjCloned->m_iSpaceDivLineLabels2AxisLabel = m_iSpaceDivLineLabels2AxisLabel;

    for( int idxPart = 0; idxPart < CEnumDivLineLabelsPart::count(); idxPart++ )
    {
        for( int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
        {
            pDiagObjCloned->m_ararbShowPartsLayer[idxPart][iLayer] = m_ararbShowPartsLayer[idxPart][iLayer];
        }
        pDiagObjCloned->m_arbShowPartsUnit[idxPart] = m_arbShowPartsUnit[idxPart];
        pDiagObjCloned->m_arcolPartsFg[idxPart] = m_arcolPartsFg[idxPart];
        pDiagObjCloned->m_arpenStyleParts[idxPart] = m_arpenStyleParts[idxPart];
        pDiagObjCloned->m_arcolPartsBg[idxPart] = m_arcolPartsBg[idxPart];
        pDiagObjCloned->m_arbrushStyleParts[idxPart] = m_arbrushStyleParts[idxPart];
        pDiagObjCloned->m_arfntParts[idxPart] = m_arfntParts[idxPart];
    }
    pDiagObjCloned->m_iDivLineLabelsDigitsCountMax = m_iDivLineLabelsDigitsCountMax;
    pDiagObjCloned->m_bUseEngineeringFormat = m_bUseEngineeringFormat;
    pDiagObjCloned->m_strAxisLabel = m_strAxisLabel;
    pDiagObjCloned->m_rectAxisLabel = m_rectAxisLabel;
    pDiagObjCloned->m_iDivLineLabelsTrailingDigits = m_iDivLineLabelsTrailingDigits;
    pDiagObjCloned->m_iDivLineLabelsExponentDigits = m_iDivLineLabelsExponentDigits;
    pDiagObjCloned->m_rectDivLineLabelsMaxTextExtent = m_rectDivLineLabelsMaxTextExtent;
    pDiagObjCloned->m_cxDivLineLabelsSpace = m_cxDivLineLabelsSpace;

    for( int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
    {
        pDiagObjCloned->m_ariDivLinesCount[iLayer] = m_ariDivLinesCount[iLayer];

        if( m_ariDivLinesCount[iLayer] > 0 )
        {
            pDiagObjCloned->m_arariDivLines_px[iLayer].resize(m_ariDivLinesCount[iLayer]);
            pDiagObjCloned->m_ararrectDivLineLabels[iLayer].resize(m_ariDivLinesCount[iLayer]);
            pDiagObjCloned->m_ararstrDivLineLabels[iLayer].resize(m_ariDivLinesCount[iLayer]);
            pDiagObjCloned->m_ararbDivLineLabelsVisible[iLayer].resize(m_ariDivLinesCount[iLayer]);

            for( int idxDivLine = 0; idxDivLine < pDiagObjCloned->m_ariDivLinesCount[iLayer]; idxDivLine++ )
            {
                pDiagObjCloned->m_arariDivLines_px[iLayer][idxDivLine] = m_arariDivLines_px[iLayer][idxDivLine];
                pDiagObjCloned->m_ararrectDivLineLabels[iLayer][idxDivLine] = m_ararrectDivLineLabels[iLayer][idxDivLine];
                pDiagObjCloned->m_ararstrDivLineLabels[iLayer][idxDivLine] = m_ararstrDivLineLabels[iLayer][idxDivLine];
                pDiagObjCloned->m_ararbDivLineLabelsVisible[iLayer][idxDivLine] = m_ararbDivLineLabelsVisible[iLayer][idxDivLine];
            }
        }
    }

    pDiagObjCloned->m_rectDivLineLabelsPhysUnit = m_rectDivLineLabelsPhysUnit;
    pDiagObjCloned->m_bDivLineLabelsPhysUnitVisible = m_bDivLineLabelsPhysUnitVisible;

    for( int idxDivLine = 0; idxDivLine < 2; idxDivLine++ )
    {
        pDiagObjCloned->m_arstrScaleMinMaxVal[idxDivLine] = m_arstrScaleMinMaxVal[idxDivLine];
        pDiagObjCloned->m_arrectScaleMinMaxVal[idxDivLine] = m_arrectScaleMinMaxVal[idxDivLine];
        pDiagObjCloned->m_arbScaleMinMaxValVisible[idxDivLine] = m_arbScaleMinMaxValVisible[idxDivLine];
    }

    i_pDiagramTrg->addDiagObj(pDiagObjCloned);

    return pDiagObjCloned;

} // clone

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::update( unsigned int i_uUpdateFlags, QPaintDevice* i_pPaintDevice )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObjUpdate, EMethodTraceDetailLevel::ArgsNormal))
    {
        strTrcMsg = updateFlags2Str(i_uUpdateFlags);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "update",
        /* strAddInfo   */ strTrcMsg );

    if( i_uUpdateFlags == EUpdateNone || m_pDiagram == nullptr || m_pDiagScale == nullptr || !isVisible() )
    {
        return;
    }
    // As a matter of fact there is no sense in adding an axis label object to
    // a diagram just designed to analyze data.
    if( m_pDiagram->getUpdateType() < EDiagramUpdateTypePixmap )
    {
        return;
    }
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

    // Please note that only if by invalidating the flag (setting the bit) in the
    // internal update flags and on requesting the process depth by this update
    // method the corresponding update process will be executed.

    // If the width and height for the sizeHint method need to be recalculated ..
    if( i_uUpdateFlags & EUpdateLayout && m_uUpdateFlags & EUpdateLayout )
    {
        updateLayout();
    }

    // If the internal data structures need to be updated ..
    if( i_uUpdateFlags & EUpdateData && m_uUpdateFlags & EUpdateData )
    {
        updateData();
    }

    // If recalculation of bitmap is needed ..
    if( i_uUpdateFlags & EUpdatePixmap && m_uUpdateFlags & EUpdatePixmap && i_pPaintDevice != nullptr )
    {
        updatePixmap(i_pPaintDevice);
    }

    // If the widget need to be updated ..
    if( i_uUpdateFlags & EUpdateWidget && m_uUpdateFlags & EUpdateWidget )
    {
        updateWidget();
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

} // update

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::updateLayout()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateLayout",
        /* strAddInfo   */ "" );

    if( m_pDiagram == nullptr || m_pDiagScale == nullptr || !isVisible() )
    {
        return;
    }

    // Please note that the axis label rectangle need to be calculated before
    // calculating the rectangle for the division lines as the position for
    // the division lines may depend on the size of the axis label rectangle.

    // As a matter of fact there is no sense in adding an axis label object to
    // a diagram just designed to analyze data.
    if( m_pDiagram->getUpdateType() >= EDiagramUpdateTypePixmap )
    {
        const CPixmapDiagram* pPixmapDiagram = dynamic_cast<const CPixmapDiagram*>(m_pDiagram);
        if( pPixmapDiagram == nullptr )
        {
            return;
        }
    }

    QRect rectDiag = getDiagRect();

    // Calculate string to indicate the physical unit at the maximum scale value
    //--------------------------------------------------------------------------

    m_unitLabels = getAxisLabelUnit();

    if( m_unitLabels.isValid() ) {
        m_strPhysUnitLabels = m_unitLabels.symbol();
    }
    else {
        m_strPhysUnitLabels = "";
    }

    // Please note that after layout processing only the size of the
    // axis label rectangle is up to date which is necessary for the
    // following "sizeHint" call. The label will be moved to its final
    // position during data processing.
    m_rectAxisLabel = getAxisLabelBoundingRect();

    // Calculate width and height of the area for the division line labels
    //==========================================================================

    double fScaleMinVal = m_pDiagScale->getScale().minVal().getVal(m_unitLabels);
    double fScaleMaxVal = m_pDiagScale->getScale().maxVal().getVal(m_unitLabels);

    // The scale object calculates the number of division lines and
    // the distance between the division lines for the grid.
    // The labels are linked to those calculated division lines.
    // To discover overlapping division line labels the rectangle extents of the
    // division line labels are stored in arrays.
    for( int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
    {
        m_arariDivLines_px[iLayer].clear();
        m_ararrectDivLineLabels[iLayer].clear();
        m_ararstrDivLineLabels[iLayer].clear();
        m_ararbDivLineLabelsVisible[iLayer].clear();
        m_ariDivLinesCount[iLayer] = 0;
    }

    int iDivLinesCount = 0;
    for( int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
    {
        EDivLineLayer eLayer = static_cast<EDivLineLayer>(iLayer);
        m_ariDivLinesCount[iLayer] = m_pDiagScale->getDivLineCount(eLayer);
        iDivLinesCount += m_ariDivLinesCount[iLayer];
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        QString strMthRuntimeInfo =
            "MainDivLineLabels: " + QString::number(m_ariDivLinesCount[EDivLineLayerMain]) +
            ", SubDivLineLabels: " + QString::number(m_ariDivLinesCount[EDivLineLayerSub]);
        mthTracer.trace(strMthRuntimeInfo);
    }

    // Calculate format of division line labels:
    //-------------------------------------------

    QVector<double> arfDivLineValsSorted = getDivLineValsSorted();

    // The number of leading digits is defined by the absolute maximum value
    // of the scales division lines. If the value is less than 1.0 at least
    // one leading digit will be shown.
    std::tuple<double, double> minMax = Math::getAbsMinMax(arfDivLineValsSorted);
    double fDivLineValAbsMax = std::get<1>(minMax);
    int iDivLineLabelsLeadingDigits = 1;
    if (fDivLineValAbsMax > 1.0) {
        iDivLineLabelsLeadingDigits = Math::getFirstSignificantDigit(fDivLineValAbsMax);
    }

    // The number of trailing digits is defined by the mimimum distance between two
    // division lines to indicate unique values. If the distance is greater than 1.0
    // at least one trailing digit will be shown.
    m_iDivLineLabelsTrailingDigits = 1;
    double fDivLineDistMinVal = m_pDiagScale->getDivLineDistMin(CEnumDivLineLayer(), &m_unitLabels);
    if (fDivLineDistMinVal < 1.0) {
        m_iDivLineLabelsTrailingDigits = abs(Math::getFirstSignificantDigit(fDivLineDistMinVal));
    }

    // The absolute minimum value of the division lines may be lower than
    // the distance between two division lines.
    double fDivLineValAbsMin = std::get<0>(minMax);
    if (fDivLineValAbsMin < 1.0) {
        int iDivLineLabelsTrailingDigits = abs(Math::getFirstSignificantDigit(fDivLineValAbsMin));
        if (iDivLineLabelsTrailingDigits > m_iDivLineLabelsTrailingDigits) {
            m_iDivLineLabelsTrailingDigits = iDivLineLabelsTrailingDigits;
        }
    }

    // The absolute maximum value of the division lines may not be lower than
    // the absolute minimum value. No need to take this value into account when
    // calculating the number of trailing digits.

    // Calculate how many leading and trailing digits would be necessary to
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

    m_iDivLineLabelsExponentDigits = 0;

    // If engineering format is forced ...
    if (m_bUseEngineeringFormat)
    {
        m_iDivLineLabelsExponentDigits = static_cast<int>(log10(static_cast<double>(iDivLineLabelsLeadingDigits)))+1;
        int iTmp = static_cast<int>(log10(static_cast<double>(m_iDivLineLabelsTrailingDigits)))+1;
        if( iTmp > m_iDivLineLabelsExponentDigits )
        {
            m_iDivLineLabelsExponentDigits = iTmp;
        }
        iDivLineLabelsLeadingDigits = 1;
        m_iDivLineLabelsTrailingDigits = 1;
    }
    // If the maximum number of digits is set ...
    else if (m_iDivLineLabelsDigitsCountMax > 0)
    {
        // Engineering format (x.y) has to be used if the number of digits necessary to
        // indicate the value is greater than the maximum number of digits set.
        if ((iDivLineLabelsLeadingDigits + m_iDivLineLabelsTrailingDigits) > m_iDivLineLabelsDigitsCountMax)
        {
            m_iDivLineLabelsExponentDigits = static_cast<int>(log10(static_cast<double>(iDivLineLabelsLeadingDigits)))+1;
            int iTmp = static_cast<int>(log10(static_cast<double>(m_iDivLineLabelsTrailingDigits)))+1;
            if (iTmp > m_iDivLineLabelsExponentDigits)
            {
                m_iDivLineLabelsExponentDigits = iTmp;
            }
            iDivLineLabelsLeadingDigits = 1;
            m_iDivLineLabelsTrailingDigits = 1;
        }
    }

    // Already mentioned above:
    // If engineering format is forced or if the number of leading and trailing digits
    // exceeds the maximum number of digits,
    // but also if the scale uses logarithmic spacing ...
    if (m_iDivLineLabelsExponentDigits > 0 || m_pDiagScale->getSpacing() == ESpacing::Logarithmic)
    {
        // The division lines are rounded to a whole number of a decimal power.
        // To output the axis labels it is "just" necessary to ensure that the
        // indicated values are different.
        int iPrecisionMax = 10;
        if (m_iDivLineLabelsDigitsCountMax > 0) {
            iPrecisionMax = m_iDivLineLabelsDigitsCountMax - iDivLineLabelsLeadingDigits;
        }
        m_iDivLineLabelsTrailingDigits = Math::CScaleDivLines::getPrecision2ShowUniqueNumbers(
            arfDivLineValsSorted, m_iDivLineLabelsExponentDigits,
            m_iDivLineLabelsTrailingDigits, iPrecisionMax, m_pTrcAdminObj);
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        QString strMthRuntimeInfo =
            QString("Digits needed for DivLineValAbsMin and DivLineValAbsMax: ") +
            "Leading: " + QString::number(iDivLineLabelsLeadingDigits) +
            ", Trailing: " + QString::number(m_iDivLineLabelsTrailingDigits) +
            ", Exponent: " + QString::number(m_iDivLineLabelsExponentDigits);
        mthTracer.trace(strMthRuntimeInfo);
    }

    // Calculate extent of division line labels:
    //-------------------------------------------

    // Build string with maximum count of necessary digits and calculate
    // the maximum extent of the division line labels:
    QString strDivLineLabel("+");
    for( int idx = 0; idx < iDivLineLabelsLeadingDigits; idx++ )
    {
        strDivLineLabel += "0";
    }
    if( m_iDivLineLabelsTrailingDigits > 0 )
    {
        strDivLineLabel += ".";
        for( int idx = 0; idx < m_iDivLineLabelsTrailingDigits; idx++ )
        {
            strDivLineLabel += "0";
        }
    }
    if( m_iDivLineLabelsExponentDigits > 0 )
    {
        strDivLineLabel += "e+";
        for( int idx = 0; idx < m_iDivLineLabelsExponentDigits; idx++ )
        {
            strDivLineLabel += "0";
        }
    }

    // I'm not sure if "0" is the most "wide" character ...
    strDivLineLabel += "0"; // .. so we add one additional character.
    QFontMetrics fntmtr(m_arfntParts[EDivLineLabelsPartLabels]);
    m_rectDivLineLabelsMaxTextExtent = fntmtr.boundingRect(strDivLineLabel);
    m_rectDivLineLabelsMaxTextExtent.setHeight(m_rectDivLineLabelsMaxTextExtent.height() + c_iMaxTextExtentAddHeight);
    m_rectDivLineLabelsMaxTextExtent.setWidth(m_rectDivLineLabelsMaxTextExtent.width() + c_iMaxTextExtentAddWidth);

    // There must be a distance (space) between two successive division line labels on the
    // horizontal (X) axis. One and a half of the width of the character '0' is a good choice:
    m_cxDivLineLabelsSpace  = fntmtr.boundingRect("0").width();
    m_cxDivLineLabelsSpace += m_cxDivLineLabelsSpace/2;

    // Calculate extent of string to indicate the physical unit:
    //-------------------------------------------------------------

    if( m_arbShowPartsUnit[EDivLineLabelsPartLabels] )
    {
        m_rectDivLineLabelsPhysUnit = fntmtr.boundingRect(m_strPhysUnitLabels);
        m_rectDivLineLabelsPhysUnit.setHeight(m_rectDivLineLabelsPhysUnit.height() + 2);
        m_rectDivLineLabelsPhysUnit.setWidth(m_rectDivLineLabelsPhysUnit.width() + 2);

        switch( m_layoutPos )
        {
            // X-Axis (vertical division lines)
            case ELayoutPosTop:
            case ELayoutPosBottom:
            {
                double xDivLine = m_pDiagScale->getMaxValPix();
                m_rectDivLineLabelsPhysUnit.moveLeft( xDivLine - m_rectDivLineLabelsPhysUnit.width()/2 );

                if( m_bAdjustContentRect2DiagPartCenter )
                {
                    if( m_rectDivLineLabelsPhysUnit.right() >= m_pDiagScale->getMaxValPix() )
                    {
                        int cxOffset = m_rectDivLineLabelsPhysUnit.right()-m_pDiagScale->getMaxValPix()+1;
                        m_rectDivLineLabelsPhysUnit.moveRight( m_rectDivLineLabelsPhysUnit.right()-cxOffset );
                    }
                }
                else
                {
                    if( m_rectDivLineLabelsPhysUnit.right() >= rectDiag.right() )
                    {
                        int cxOffset = m_rectDivLineLabelsPhysUnit.right()-rectDiag.right()+1;
                        m_rectDivLineLabelsPhysUnit.moveRight( m_rectDivLineLabelsPhysUnit.right()-cxOffset );
                    }
                }
                break;
            }

            // Y-Axis (horizontal division lines)
            case ELayoutPosLeft:
            case ELayoutPosRight:
            {
                // As for the Y-axis the physical unit string should be adjusted to the division line labels
                // the extent (width) of the physical unit string depends on the division line labels.
                if( m_rectDivLineLabelsPhysUnit.width() < m_rectDivLineLabelsMaxTextExtent.width() )
                {
                    m_rectDivLineLabelsPhysUnit.setWidth(m_rectDivLineLabelsMaxTextExtent.width());
                }
                else if( m_rectDivLineLabelsPhysUnit.width() > m_rectDivLineLabelsMaxTextExtent.width() )
                {
                    m_rectDivLineLabelsMaxTextExtent.setWidth(m_rectDivLineLabelsPhysUnit.width());
                }

                double yDivLine = m_pDiagScale->getMaxValPix();
                m_rectDivLineLabelsPhysUnit.moveTop( yDivLine - m_rectDivLineLabelsPhysUnit.height()/2 );

                if( m_bAdjustContentRect2DiagPartCenter )
                {
                    if( m_rectDivLineLabelsPhysUnit.top() <= m_pDiagScale->getMaxValPix() )
                    {
                        int cyOffset = m_pDiagScale->getMaxValPix()-m_rectDivLineLabelsPhysUnit.top()+1;
                        m_rectDivLineLabelsPhysUnit.moveTop( m_rectDivLineLabelsPhysUnit.top()+cyOffset );
                    }
                }
                else
                {
                    if( m_rectDivLineLabelsPhysUnit.top() <= rectDiag.top() )
                    {
                        int cyOffset = rectDiag.top()-m_rectDivLineLabelsPhysUnit.top()+1;
                        m_rectDivLineLabelsPhysUnit.moveTop( m_rectDivLineLabelsPhysUnit.top()+cyOffset );
                    }
                }
                break;
            }
            default:
            {
                break;
            }

        } // switch( m_layoutPos )

    } // if( m_arbShowPartsUnit[EDivLineLabelsPartLabels] )

    // Calculate extent of division line labels
    //-----------------------------------------

    m_bDivLineLabelsPhysUnitVisible = false;

    for( int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
    {
        if( m_ariDivLinesCount[iLayer] > 0 )
        {
            m_arariDivLines_px[iLayer] = QVector<int>(m_ariDivLinesCount[iLayer], 0);
            m_ararrectDivLineLabels[iLayer] = QVector<QRect>(m_ariDivLinesCount[iLayer]);
            m_ararstrDivLineLabels[iLayer] = QVector<QString>(m_ariDivLinesCount[iLayer]);
            m_ararbDivLineLabelsVisible[iLayer] = QVector<bool>(m_ariDivLinesCount[iLayer], false);
        }
    }
    for( int idx = 0; idx < 2; idx++ )
    {
        m_arbScaleMinMaxValVisible[idx] = false;
    }

    switch( m_layoutPos )
    {
        // X-Axis (vertical division lines)
        case ELayoutPosTop:
        case ELayoutPosBottom:
        {
            // Division lines
            //---------------

            // Please note that we need the coordinates of the division lines also if they
            // will not be shown later on during pixmap processing.
            for( int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
            {
                EDivLineLayer eLayer = static_cast<EDivLineLayer>(iLayer);
                for( int idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]; idxDivLine++ )
                {
                    int xDivLine = static_cast<int>(m_pDiagScale->getDivLinePix(eLayer, idxDivLine) + 0.5);
                    m_arariDivLines_px[iLayer][idxDivLine] = xDivLine;
                }
            }

            // Division line labels
            //---------------------

            for( int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
            {
                EDivLineLayer eLayer = static_cast<EDivLineLayer>(iLayer);
                QRect rectDivLineLabel;

                rectDivLineLabel.setHeight(m_rectDivLineLabelsMaxTextExtent.height());

                for( int idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]; idxDivLine++ )
                {
                    int xDivLine = static_cast<int>(m_pDiagScale->getDivLinePix(eLayer, idxDivLine) + 0.5);
                    strDivLineLabel = formatValue(m_pDiagScale->getDivLineVal(eLayer, idxDivLine, &m_unitLabels));

                    // At the horizontal (X) axis the labels will be output centered below the division lines
                    // and we can remove trailing zeros:
                    removeTrailingZeros(&strDivLineLabel);
                    rectDivLineLabel = fntmtr.boundingRect(strDivLineLabel);
                    rectDivLineLabel.setWidth( rectDivLineLabel.width() + m_cxDivLineLabelsSpace );
                    rectDivLineLabel.moveLeft( xDivLine - rectDivLineLabel.width()/2 );

                    // To discover overlapping division line labels the rectangle extents of the
                    // division line labels will be stored in an array.
                    m_ararrectDivLineLabels[iLayer][idxDivLine] = rectDivLineLabel;
                    m_ararstrDivLineLabels[iLayer][idxDivLine] = strDivLineLabel;
                }
            } // for( iLayer < CEnumDivLineLayer::count() )

            // Minimum and maximum scale value
            //---------------------------------

            // Additionally try to show the minimum and/or maximum scale values if
            // not enough division lines are visible ..
            for( int idx = 0; idx < 2; idx++ )
            {
                m_arstrScaleMinMaxVal[idx] = "";
                m_arrectScaleMinMaxVal[idx].setWidth(0);
                m_arrectScaleMinMaxVal[idx].setHeight(0);

                int xDivLine = 0;

                if( idx == 0 )
                {
                    TFormatResult formatResult = PhysVal::formatValue(
                        /* fValue                */ fScaleMinVal,
                        /* unitVal               */ m_pDiagScale->getScale().unit(),
                        /* iDigitsMantissaMax    */ 6,
                        /* bAccuracyLimitsMant.  */ false,
                        /* iDigitsAccuracy       */ 0,
                        /* iDigitsExponent       */ 0,
                        /* bUseEngineeringFormat */ false,
                        /* pfVal                 */ nullptr,
                        /* pstr                  */ &strDivLineLabel,
                        /* ppUnit                */ nullptr,
                        /* piDigitsLeading       */ nullptr,
                        /* piDigitsTrailing      */ nullptr );
                    if( !(formatResult & FormatResult::Error) ) //lint !e655
                    {
                        m_arstrScaleMinMaxVal[idx] = strDivLineLabel;
                    }
                    xDivLine = m_pDiagScale->getMinValPix();
                }
                else
                {
                    TFormatResult formatResult = PhysVal::formatValue(
                        /* fVal                  */ fScaleMaxVal,
                        /* unitVal               */ m_pDiagScale->getScale().unit(),
                        /* iDigitsMantissaMax    */ 6,
                        /* bAccuracyLimitsMant.  */ false,
                        /* iDigitsAccuracy       */ 0,
                        /* iDigitsExponent       */ 0,
                        /* bUseEngineeringFormat */ false,
                        /* pfVal                 */ nullptr,
                        /* pstr                  */ &strDivLineLabel,
                        /* ppUnit                */ nullptr,
                        /* piDigitsLeading       */ nullptr,
                        /* piDigitsTrailing      */ nullptr );
                    if( !(formatResult & FormatResult::Error) ) //lint !e655
                    {
                        m_arstrScaleMinMaxVal[idx] = strDivLineLabel;
                    }
                    xDivLine = m_pDiagScale->getMaxValPix();
                }
                m_arrectScaleMinMaxVal[idx] = fntmtr.boundingRect(m_arstrScaleMinMaxVal[idx]);
                m_arrectScaleMinMaxVal[idx].setWidth( m_arrectScaleMinMaxVal[idx].width() + 2 );
                m_arrectScaleMinMaxVal[idx].setHeight( m_arrectScaleMinMaxVal[idx].height() + 2 );
                m_arrectScaleMinMaxVal[idx].moveLeft( xDivLine - m_arrectScaleMinMaxVal[idx].width()/2 );

                if( m_arrectScaleMinMaxVal[idx].left() <= rectDiag.left() )
                {
                    int cxOffset = rectDiag.left()-m_arrectScaleMinMaxVal[idx].left()+1;
                    m_arrectScaleMinMaxVal[idx].moveLeft( m_arrectScaleMinMaxVal[idx].left()+cxOffset );
                }
                else if( m_arrectScaleMinMaxVal[idx].right() >= rectDiag.right() )
                {
                    int cxOffset = m_arrectScaleMinMaxVal[idx].right()-rectDiag.right()+1;
                    m_arrectScaleMinMaxVal[idx].moveRight( m_arrectScaleMinMaxVal[idx].right()-cxOffset );
                }
            }
            break;

        } // case( PosTop || PosBottom )

        // Y-Axis (horizontal division lines)
        case ELayoutPosLeft:
        case ELayoutPosRight:
        {
            // Division lines
            //---------------

            for( int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
            {
                EDivLineLayer eLayer = static_cast<EDivLineLayer>(iLayer);
                // Division lines at the main grid:
                for( int idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]; idxDivLine++ )
                {
                    int yDivLine = static_cast<int>(m_pDiagScale->getDivLinePix(eLayer, idxDivLine) + 0.5);
                    m_arariDivLines_px[iLayer][idxDivLine] = yDivLine;
                }
            }

            // Division line labels
            //---------------------

            for( int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
            {
                EDivLineLayer eLayer = static_cast<EDivLineLayer>(iLayer);
                QRect rectDivLineLabel;

                rectDivLineLabel.setWidth(m_rectDivLineLabelsMaxTextExtent.width());
                rectDivLineLabel.setHeight(m_rectDivLineLabelsMaxTextExtent.height());

                // Division lines at the main grid:
                for( int idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]; idxDivLine++ )
                {
                    int yDivLine = static_cast<int>(m_pDiagScale->getDivLinePix(static_cast<EDivLineLayer>(iLayer), idxDivLine) + 0.5);
                    strDivLineLabel = formatValue( m_pDiagScale->getDivLineVal(static_cast<EDivLineLayer>(iLayer), idxDivLine, &m_unitLabels) );
                    rectDivLineLabel.moveTop( yDivLine - m_rectDivLineLabelsMaxTextExtent.height()/2 );

                    // To discover overlapping division line labels the rectangle extents of the
                    // division line labels will be stored in an array.
                    m_ararrectDivLineLabels[iLayer][idxDivLine] = rectDivLineLabel;
                    m_ararstrDivLineLabels[iLayer][idxDivLine] = strDivLineLabel;
                }
            } // for( iLayer < CEnumDivLineLayer::count() )

            // Minimum and maximum scale value
            //---------------------------------

            // Additionally try to show the minimum and/or maximum scale values if
            // not enough division lines are visible ..
            for( int idx = 0; idx < 2; idx++ )
            {
                m_arstrScaleMinMaxVal[idx] = "";

                // Please note that the vertical (Y) axis labels are either left or right
                // adjusted. For this the text areas for all labels need to have the same width.
                // But what if the labels at the minimum and maximum need more space than
                // the already visible division line labels? In that case we would have to adjust
                // the size of the already calculated division lines labels ... We don't do that
                // as the minimum and maximum scale values will only be indicated if just one or
                // even no division line label is visible and the scaling is anyway not straight ...
                m_arrectScaleMinMaxVal[idx].setWidth(0);
                m_arrectScaleMinMaxVal[idx].setHeight(0);

                int yDivLine = 0;

                if( idx == 0 )
                {
                    TFormatResult formatResult = PhysVal::formatValue(
                        /* fVal                  */ fScaleMinVal,
                        /* unitVal               */ m_pDiagScale->getScale().unit(),
                        /* iDigitsMantissaMax    */ 6,
                        /* bAccuracyLimitsMant.  */ false,
                        /* iDigitsAccuracy       */ 0,
                        /* iDigitsExponent       */ 0,
                        /* bUseEngineeringFormat */ false,
                        /* pfVal                 */ nullptr,
                        /* pstr                  */ &strDivLineLabel,
                        /* ppUnit                */ nullptr,
                        /* piDigitsLeading       */ nullptr,
                        /* piDigitsTrailing      */ nullptr );
                    if( !(formatResult & FormatResult::Error) ) //lint !e655
                    {
                        m_arstrScaleMinMaxVal[idx] = strDivLineLabel;
                    }
                    yDivLine = m_pDiagScale->getMinValPix();
                }
                else
                {
                    TFormatResult formatResult = PhysVal::formatValue(
                        /* fVal                  */ fScaleMaxVal,
                        /* unitVal              */ m_pDiagScale->getScale().unit(),
                        /* iDigitsMantissaMax    */ 6,
                        /* bAccuracyLimitsMant.  */ false,
                        /* iDigitsAccuracy       */ 0,
                        /* iDigitsExponent       */ 0,
                        /* bUseEngineeringFormat */ false,
                        /* pfVal                 */ nullptr,
                        /* pstr                  */ &strDivLineLabel,
                        /* ppUnit                */ nullptr,
                        /* piDigitsLeading       */ nullptr,
                        /* piDigitsTrailing      */ nullptr );
                    if( !(formatResult & FormatResult::Error) ) //lint !e655
                    {
                        m_arstrScaleMinMaxVal[idx] = strDivLineLabel;
                    }
                    yDivLine = m_pDiagScale->getMaxValPix();
                }
                m_arrectScaleMinMaxVal[idx] = fntmtr.boundingRect(m_arstrScaleMinMaxVal[idx]);
                m_arrectScaleMinMaxVal[idx].setWidth( m_arrectScaleMinMaxVal[idx].width() + 2 );
                m_arrectScaleMinMaxVal[idx].setHeight( m_arrectScaleMinMaxVal[idx].height() + 2 );
                m_arrectScaleMinMaxVal[idx].moveTop( yDivLine - m_arrectScaleMinMaxVal[idx].height()/2 );

                if( m_arrectScaleMinMaxVal[idx].bottom() >= rectDiag.bottom() )
                {
                    int cyOffset = m_arrectScaleMinMaxVal[idx].bottom()-rectDiag.bottom()+1;
                    m_arrectScaleMinMaxVal[idx].moveBottom( m_arrectScaleMinMaxVal[idx].bottom()-cyOffset );
                }
                else if( m_arrectScaleMinMaxVal[idx].top() <= rectDiag.top() )
                {
                    int cyOffset = rectDiag.top()-m_arrectScaleMinMaxVal[idx].top()+1;
                    m_arrectScaleMinMaxVal[idx].moveTop( m_arrectScaleMinMaxVal[idx].top()+cyOffset );
                }
            }
            break;

        } // case( PosLeft || PosRight )

        default:
        {
            break;
        }

    } // switch( m_layoutPos )

    // Mark current process depth as executed (reset bit)
    //---------------------------------------------------

    validate(EUpdateLayout);

} // updateLayout

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::updateData()
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateData",
        /* strAddInfo   */ "" );

    QRect rectDiag(0,0,1,1);

    const CPixmapDiagram* pPixmapDiagram = nullptr;

    // As a matter of fact there is no sense in adding an axis label object to
    // a diagram just designed to analyze data.
    if( m_pDiagram->getUpdateType() >= EDiagramUpdateTypePixmap )
    {
        pPixmapDiagram = dynamic_cast<const CPixmapDiagram*>(m_pDiagram);
    }
    if( pPixmapDiagram == nullptr )
    {
        return;
    }

    rectDiag.setLeft( pPixmapDiagram->getMarginLeft() );
    rectDiag.setWidth( pPixmapDiagram->getWidth()-pPixmapDiagram->getMarginLeft()-pPixmapDiagram->getMarginRight() );
    rectDiag.setTop( pPixmapDiagram->getMarginTop() );
    rectDiag.setHeight( pPixmapDiagram->getHeight()-pPixmapDiagram->getMarginTop()-pPixmapDiagram->getMarginBottom() );

    // Please note that the axis label rectangle need to be calculated before
    // calculating the rectangle for the division lines as the position for
    // the division lines may depend on the size of the axis label rectangle.

    // Calculate position of the axis label rectangle
    //==========================================================================

    QRect rectDivLineLabel;
    QRect rectAxisLabel;

    if( m_ararbShowPartsLayer[EDivLineLabelsPartAxisLabel][EDivLineLayerMain] || m_arbShowPartsUnit[EDivLineLabelsPartAxisLabel] )
    {
        if( m_ararbShowPartsLayer[EDivLineLabelsPartLabels][EDivLineLayerMain] )
        {
            rectDivLineLabel = m_rectDivLineLabelsMaxTextExtent;
        }

        rectAxisLabel = m_rectAxisLabel;

        int cxWidth  = rectAxisLabel.width();
        int cyHeight = rectAxisLabel.height();

        switch( m_layoutPos )
        {
            case ELayoutPosTop:
            {
                if( rectDivLineLabel.isValid() && m_iSpaceDiagPartCenter2AxisLabel < rectDivLineLabel.height() )
                {
                    rectAxisLabel.moveBottom( m_rectContent.bottom() - rectDivLineLabel.height() - m_iSpaceDiagPartCenter2AxisLabel );
                }
                else
                {
                    rectAxisLabel.moveBottom( m_rectContent.bottom() - m_iSpaceDiagPartCenter2AxisLabel );
                }
                rectAxisLabel.moveLeft( m_rectContent.width()/2 - cxWidth/2 );
                break;
            }
            case ELayoutPosBottom:
            {
                if( rectDivLineLabel.isValid() && m_iSpaceDiagPartCenter2AxisLabel < rectDivLineLabel.height() )
                {
                    rectAxisLabel.moveTop( m_rectContent.top() + rectDivLineLabel.height() + m_iSpaceDiagPartCenter2AxisLabel );
                }
                else
                {
                    rectAxisLabel.moveTop( m_rectContent.top() + m_iSpaceDiagPartCenter2AxisLabel );
                }
                rectAxisLabel.moveLeft( m_rectContent.width()/2 - cxWidth/2 );
                break;
            }
            case ELayoutPosLeft:
            {
                if( rectDivLineLabel.isValid() && m_iSpaceDiagPartCenter2AxisLabel < rectDivLineLabel.width() )
                {
                    rectAxisLabel.moveRight( m_rectContent.right() - rectDivLineLabel.width() - m_iSpaceDiagPartCenter2AxisLabel );
                }
                else
                {
                    rectAxisLabel.moveRight( m_rectContent.right() - m_iSpaceDiagPartCenter2AxisLabel );
                }
                rectAxisLabel.moveTop( m_rectContent.top() + m_rectContent.height()/2 - cyHeight/2 );
                break;
            }
            case ELayoutPosRight:
            {
                if( rectDivLineLabel.isValid() && m_iSpaceDiagPartCenter2AxisLabel < rectDivLineLabel.width() )
                {
                    rectAxisLabel.moveLeft( m_rectContent.left() + rectDivLineLabel.width() + m_iSpaceDiagPartCenter2AxisLabel );
                }
                else
                {
                    rectAxisLabel.moveLeft( m_rectContent.left() + m_iSpaceDiagPartCenter2AxisLabel );
                }
                rectAxisLabel.moveTop( m_rectContent.top() + m_rectContent.height()/2 - cyHeight/2 );
                break;
            }
            default:
            {
                break;
            }
        }
    }
    m_rectAxisLabel = rectAxisLabel;

    // Calculate position of the division line labels
    //==========================================================================

    // The X and Y scales are calculating the number of division lines and
    // the distance between the division lines for the grid.
    // The labels are linked to those calculated division lines.

    int   xDivLine;
    int   yDivLine;
    int   cxOffset;
    int   cyOffset;

    // Calculate position of string to indicate the physical unit:
    //-------------------------------------------------------------

    // As for the Y-axis the physical unit string should be adjusted to the division line labels
    // the extent (width) of the physical unit string depends on the division line labels.
    if( m_arbShowPartsUnit[EDivLineLabelsPartLabels] )
    {
        switch( m_layoutPos )
        {
            // X-Axis (vertical division lines)
            case ELayoutPosTop:
            case ELayoutPosBottom:
            {
                xDivLine = m_pDiagScale->getMaxValPix();
                m_rectDivLineLabelsPhysUnit.moveLeft( xDivLine - m_rectDivLineLabelsPhysUnit.width()/2 );

                // Now we need to adjust the horizontal position of the unit label.
                // The unit should always be indicated.and the division line labels may not overlap
                // the unit string. The unit string will be moved to fit in the diagram window or
                // - if the axis label should be adjusted to the center area - to fit in the
                // diagrams center part.
                if( m_bAdjustContentRect2DiagPartCenter )
                {
                    if( m_rectDivLineLabelsPhysUnit.right() >= pPixmapDiagram->getRectPartCenter().right() )
                    {
                        cxOffset = m_rectDivLineLabelsPhysUnit.right()-pPixmapDiagram->getRectPartCenter().right()+1;
                        m_rectDivLineLabelsPhysUnit.moveRight( m_rectDivLineLabelsPhysUnit.right()-cxOffset );
                    }
                }
                else
                {
                    if( m_rectDivLineLabelsPhysUnit.right() >= rectDiag.right() )
                    {
                        cxOffset = m_rectDivLineLabelsPhysUnit.right()-rectDiag.right()+1;
                        m_rectDivLineLabelsPhysUnit.moveRight( m_rectDivLineLabelsPhysUnit.right()-cxOffset );
                    }
                }

                // Now we need to adjust the vertical position of the unit label.
                // For this we need to access the diagrams center part.
                if( m_layoutPos == ELayoutPosTop )
                {
                    // Above the diagrams center part ..
                    if( m_iSpaceDiagPartCenter2DivLineLabels >= 0 )
                    {
                        m_rectDivLineLabelsPhysUnit.moveBottom( pPixmapDiagram->getRectPartCenter().top() - m_iSpaceDiagPartCenter2DivLineLabels );
                    }
                    // Within the diagrams center part ..
                    else
                    {
                        m_rectDivLineLabelsPhysUnit.moveTop( pPixmapDiagram->getRectPartCenter().top() - m_iSpaceDiagPartCenter2DivLineLabels );
                    }
                }
                else
                {
                    // Below the diagrams center part ..
                    if( m_iSpaceDiagPartCenter2DivLineLabels >= 0 )
                    {
                        m_rectDivLineLabelsPhysUnit.moveTop( pPixmapDiagram->getRectPartCenter().bottom() + m_iSpaceDiagPartCenter2DivLineLabels );
                    }
                    // Within the diagrams center part ..
                    else
                    {
                        m_rectDivLineLabelsPhysUnit.moveBottom( pPixmapDiagram->getRectPartCenter().bottom() + m_iSpaceDiagPartCenter2DivLineLabels );
                    }
                }
                break;
            }

            // Y-Axis (horizontal division lines)
            case ELayoutPosLeft:
            case ELayoutPosRight:
            {
                yDivLine = m_pDiagScale->getMaxValPix();
                m_rectDivLineLabelsPhysUnit.moveTop( yDivLine - m_rectDivLineLabelsPhysUnit.height()/2 );

                // Now we need to adjust the vertical position of the unit label.
                // The unit should always be indicated.and the division line labels may not overlap
                // the unit string. The unit string will be moved to fit in the diagram window or
                // - if the axis label should be adjusted to the center area - to fit in the
                // diagrams center part.
                if( m_bAdjustContentRect2DiagPartCenter )
                {
                    if( m_rectDivLineLabelsPhysUnit.top() <= pPixmapDiagram->getRectPartCenter().top() )
                    {
                        cyOffset = pPixmapDiagram->getRectPartCenter().top()-m_rectDivLineLabelsPhysUnit.top()+1;
                        m_rectDivLineLabelsPhysUnit.moveTop( m_rectDivLineLabelsPhysUnit.top()+cyOffset );
                    }
                }
                else
                {
                    if( m_rectDivLineLabelsPhysUnit.top() <= rectDiag.top() )
                    {
                        cyOffset = rectDiag.top()-m_rectDivLineLabelsPhysUnit.top()+1;
                        m_rectDivLineLabelsPhysUnit.moveTop( m_rectDivLineLabelsPhysUnit.top()+cyOffset );
                    }
                }

                // Now we need to adjust the horizontal position of the unit label.
                // For this we need to access the diagrams center part.
                if( m_layoutPos == ELayoutPosLeft )
                {
                    // Left of the diagrams center part ..
                    if( m_iSpaceDiagPartCenter2DivLineLabels >= 0 )
                    {
                        m_rectDivLineLabelsPhysUnit.moveRight( pPixmapDiagram->getRectPartCenter().left() - m_iSpaceDiagPartCenter2DivLineLabels );
                    }
                    // Within the diagrams center part ..
                    else
                    {
                        m_rectDivLineLabelsPhysUnit.moveLeft( pPixmapDiagram->getRectPartCenter().left() - m_iSpaceDiagPartCenter2DivLineLabels );
                    }
                }
                else
                {
                    // Right of the diagrams center part ..
                    if( m_iSpaceDiagPartCenter2DivLineLabels >= 0 )
                    {
                        m_rectDivLineLabelsPhysUnit.moveLeft( pPixmapDiagram->getRectPartCenter().right() + m_iSpaceDiagPartCenter2DivLineLabels );
                    }
                    // Within the diagrams center part ..
                    else
                    {
                        m_rectDivLineLabelsPhysUnit.moveRight( pPixmapDiagram->getRectPartCenter().right() + m_iSpaceDiagPartCenter2DivLineLabels );
                    }
                }
                break;
            }
            default:
            {
                break;
            }

        } // switch( m_layoutPos )

    } // if( m_arbShowPartsUnit[EDivLineLabelsPartLabels] )

    // Calculate the division line labels
    //-----------------------------------

    int iLayer;
    int idxDivLine;
    int idx;

    switch( m_layoutPos )
    {
        // X-Axis (vertical division lines)
        case ELayoutPosTop:
        case ELayoutPosBottom:
        {
            // Division lines
            //---------------

            for( iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
            {
                for( idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]; idxDivLine++ )
                {
                    xDivLine = static_cast<int>(m_pDiagScale->getDivLinePix(static_cast<EDivLineLayer>(iLayer), idxDivLine) + 0.5);
                    m_arariDivLines_px[iLayer][idxDivLine] = xDivLine;
                }
            }

            // Division line labels
            //---------------------

            for( iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
            {
                // The labels on the horizontal (X) axis will be output center aligned to
                // the division lines. The rectangles of the division line labels and the
                // rectangle for the unit string don't need to be of same size.
                for( idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]; idxDivLine++ )
                {
                    rectDivLineLabel = m_ararrectDivLineLabels[iLayer][idxDivLine];
                    xDivLine = static_cast<int>(m_pDiagScale->getDivLinePix(static_cast<EDivLineLayer>(iLayer), idxDivLine) + 0.5);
                    rectDivLineLabel.moveLeft( xDivLine - rectDivLineLabel.width()/2 );

                    if( m_layoutPos == ELayoutPosTop )
                    {
                        // Above of the diagrams center part ..
                        if( m_iSpaceDiagPartCenter2DivLineLabels >= 0 )
                        {
                            rectDivLineLabel.moveBottom( pPixmapDiagram->getRectPartCenter().top() - m_iSpaceDiagPartCenter2DivLineLabels );
                        }
                        // Within the diagrams center part ..
                        else
                        {
                            rectDivLineLabel.moveTop( pPixmapDiagram->getRectPartCenter().top() - m_iSpaceDiagPartCenter2DivLineLabels );
                        }
                    }
                    else
                    {
                        // Below of the diagrams center part ..
                        if( m_iSpaceDiagPartCenter2DivLineLabels >= 0 )
                        {
                            rectDivLineLabel.moveTop( pPixmapDiagram->getRectPartCenter().bottom() + m_iSpaceDiagPartCenter2DivLineLabels );
                        }
                        // Within the diagrams center part ..
                        else
                        {
                            rectDivLineLabel.moveBottom( pPixmapDiagram->getRectPartCenter().bottom() + m_iSpaceDiagPartCenter2DivLineLabels );
                        }
                    }
                    m_ararrectDivLineLabels[iLayer][idxDivLine] = rectDivLineLabel;
                }
            } // for( iLayer < CEnumDivLineLayer::count() )

            // Minimum and maximum scale value
            //---------------------------------

            for( idx = 0; idx < 2; idx++ )
            {
                if( !m_arstrScaleMinMaxVal[idx].isEmpty() && m_arrectScaleMinMaxVal[idx].isValid() )
                {
                    rectDivLineLabel.setWidth(m_arrectScaleMinMaxVal[idx].width());
                    rectDivLineLabel.setHeight(m_arrectScaleMinMaxVal[idx].height());

                    if( m_layoutPos == ELayoutPosTop )
                    {
                        // Above of the diagrams center part ..
                        if( m_iSpaceDiagPartCenter2DivLineLabels >= 0 )
                        {
                            rectDivLineLabel.moveBottom( pPixmapDiagram->getRectPartCenter().top() - m_iSpaceDiagPartCenter2DivLineLabels );
                        }
                        // Within the diagrams center part ..
                        else
                        {
                            rectDivLineLabel.moveTop( pPixmapDiagram->getRectPartCenter().top() - m_iSpaceDiagPartCenter2DivLineLabels );
                        }
                    }
                    else
                    {
                        // Below of the diagrams center part ..
                        if( m_iSpaceDiagPartCenter2DivLineLabels >= 0 )
                        {
                            rectDivLineLabel.moveTop( pPixmapDiagram->getRectPartCenter().bottom() + m_iSpaceDiagPartCenter2DivLineLabels );
                        }
                        // Within the diagrams center part ..
                        else
                        {
                            rectDivLineLabel.moveBottom( pPixmapDiagram->getRectPartCenter().bottom() + m_iSpaceDiagPartCenter2DivLineLabels );
                        }
                    }
                    if( idx == 0 )
                    {
                        xDivLine = m_pDiagScale->getMinValPix();
                    }
                    else
                    {
                        xDivLine = m_pDiagScale->getMaxValPix();
                    }
                    rectDivLineLabel.moveLeft( xDivLine - rectDivLineLabel.width()/2 );
                    m_arrectScaleMinMaxVal[idx] = rectDivLineLabel;
                }
            }
            break;

        } // case( PosTop || PosBottom )

        // Y-Axis (horizontal division lines)
        case ELayoutPosLeft:
        case ELayoutPosRight:
        {
            // Division lines
            //---------------

            for( iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
            {
                // Division lines at the main grid:
                for( idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]; idxDivLine++ )
                {
                    yDivLine = static_cast<int>(m_pDiagScale->getDivLinePix(static_cast<EDivLineLayer>(iLayer), idxDivLine) + 0.5);
                    m_arariDivLines_px[iLayer][idxDivLine] = yDivLine;
                }
            }

            // Division line labels
            //---------------------

            for( iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
            {
                // At the vertical (Y) axis the labels are either left or right adjusted.
                // Each of the rectangles for the division lines and the rectangle for the
                // unit string must be of same width to allow the adjustment.
                rectDivLineLabel.setWidth(m_rectDivLineLabelsMaxTextExtent.width());
                rectDivLineLabel.setHeight(m_rectDivLineLabelsMaxTextExtent.height());

                if( m_layoutPos == ELayoutPosLeft )
                {
                    // Left of the diagrams center part ..
                    if( m_iSpaceDiagPartCenter2DivLineLabels >= 0 )
                    {
                        rectDivLineLabel.moveRight( pPixmapDiagram->getRectPartCenter().left() - m_iSpaceDiagPartCenter2DivLineLabels );
                    }
                    // Within the diagrams center part ..
                    else
                    {
                        rectDivLineLabel.moveLeft( pPixmapDiagram->getRectPartCenter().left() - m_iSpaceDiagPartCenter2DivLineLabels );
                    }
                }
                else
                {
                    // Right of the diagrams center part ..
                    if( m_iSpaceDiagPartCenter2DivLineLabels >= 0 )
                    {
                        rectDivLineLabel.moveLeft( pPixmapDiagram->getRectPartCenter().right() + m_iSpaceDiagPartCenter2DivLineLabels );
                    }
                    // Within the diagrams center part ..
                    else
                    {
                        rectDivLineLabel.moveRight( pPixmapDiagram->getRectPartCenter().right() + m_iSpaceDiagPartCenter2DivLineLabels );
                    }
                }
                for( idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]; idxDivLine++ )
                {
                    yDivLine = static_cast<int>(m_pDiagScale->getDivLinePix(static_cast<EDivLineLayer>(iLayer), idxDivLine) + 0.5);
                    rectDivLineLabel.moveTop( yDivLine - m_rectDivLineLabelsMaxTextExtent.height()/2 );
                    m_ararrectDivLineLabels[iLayer][idxDivLine] = rectDivLineLabel;
                }
            } // for( iLayer < CEnumDivLineLayer::count() )

            // Minimum and maximum scale value
            //---------------------------------

            for( idx = 0; idx < 2; idx++ )
            {
                if( !m_arstrScaleMinMaxVal[idx].isEmpty() && m_arrectScaleMinMaxVal[idx].isValid() )
                {
                    rectDivLineLabel.setWidth(m_arrectScaleMinMaxVal[idx].width());
                    rectDivLineLabel.setHeight(m_arrectScaleMinMaxVal[idx].height());

                    if( m_layoutPos == ELayoutPosLeft )
                    {
                        // Left of the diagrams center part ..
                        if( m_iSpaceDiagPartCenter2DivLineLabels >= 0 )
                        {
                            rectDivLineLabel.moveRight( pPixmapDiagram->getRectPartCenter().left() - m_iSpaceDiagPartCenter2DivLineLabels );
                        }
                        // Within the diagrams center part ..
                        else
                        {
                            rectDivLineLabel.moveLeft( pPixmapDiagram->getRectPartCenter().left() - m_iSpaceDiagPartCenter2DivLineLabels );
                        }
                    }
                    else
                    {
                        // Right of the diagrams center part ..
                        if( m_iSpaceDiagPartCenter2DivLineLabels >= 0 )
                        {
                            rectDivLineLabel.moveLeft( pPixmapDiagram->getRectPartCenter().right() + m_iSpaceDiagPartCenter2DivLineLabels );
                        }
                        // Within the diagrams center part ..
                        else
                        {
                            rectDivLineLabel.moveRight( pPixmapDiagram->getRectPartCenter().right() + m_iSpaceDiagPartCenter2DivLineLabels );
                        }
                    }
                    if( idx == 0 )
                    {
                        yDivLine = m_pDiagScale->getMinValPix();
                    }
                    else
                    {
                        yDivLine = m_pDiagScale->getMaxValPix();
                    }
                    rectDivLineLabel.moveTop( yDivLine - rectDivLineLabel.height()/2 );
                    m_arrectScaleMinMaxVal[idx] = rectDivLineLabel;
                }
            }
            break;

        } // case( PosLeft || PosRight )

        default:
        {
            break;
        }

    } // switch( m_layoutPos )

    // Mark current process depth as executed (reset bit)
    //---------------------------------------------------

    validate(EUpdateData);

} // updateData

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::updatePixmap( QPaintDevice* i_pPaintDevice )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updatePixmap",
        /* strAddInfo   */ "" );

    QRect rectDiag(0,0,1,1);

    const CPixmapDiagram* pPixmapDiagram = nullptr;

    // As a matter of fact there is no sense in adding an axis label object to
    // a diagram just designed to analyze data.
    if( m_pDiagram != nullptr && m_pDiagram->getUpdateType() >= EDiagramUpdateTypePixmap )
    {
        pPixmapDiagram = dynamic_cast<const CPixmapDiagram*>(m_pDiagram);
    }
    if( pPixmapDiagram == nullptr )
    {
        return;
    }

    rectDiag.setLeft( pPixmapDiagram->getMarginLeft() );
    rectDiag.setWidth( pPixmapDiagram->getWidth()-pPixmapDiagram->getMarginLeft()-pPixmapDiagram->getMarginRight() );
    rectDiag.setTop( pPixmapDiagram->getMarginTop() );
    rectDiag.setHeight( pPixmapDiagram->getHeight()-pPixmapDiagram->getMarginTop()-pPixmapDiagram->getMarginBottom() );

    QPainter      painter(i_pPaintDevice);
    int           iLayer;
    QRect         rectDivLineLabel;
    QString       strDivLineLabel;
    int           idxDivLine;
    int           idx;
    QFontMetrics  fntmtr(m_arfntParts[EDivLineLabelsPartAxisLabel]);
    QRect         rectAxisLabel;
    QString       strAxisLabel;

    if( m_ararbShowPartsLayer[EDivLineLabelsPartAxisLabel][EDivLineLayerMain] || m_arbShowPartsUnit[EDivLineLabelsPartAxisLabel] )
    {
        strAxisLabel = m_strAxisLabel;

        if( m_arbShowPartsUnit[EDivLineLabelsPartAxisLabel] )
        {
            if( !strAxisLabel.isEmpty() )
            {
                strAxisLabel += " / ";
            }
            strAxisLabel += m_strPhysUnitLabels;
        }
    }

    // Label the axis at the grid lines
    //---------------------------------

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "Labeling axis at ";
        strTrcMsg += layoutPos2Str(m_layoutPos) + ":";
        mthTracer.trace(strTrcMsg);
    }

    switch( m_layoutPos )
    {
        // X-Axis (vertical lines from diagrams center area border to axis labels)
        case ELayoutPosTop:
        case ELayoutPosBottom:
        {
            int iDivLineLabelsVisibleCount = 0;
            int idxDivLineLabelLastVisible = -1;
            int iLayerOfDivLineLabelLastVisible = -1;
            int xDivLine;
            int cxOffset;

            // Division lines
            //---------------

            for( iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
            {
                if( m_ararbShowPartsLayer[EDivLineLabelsPartLines][iLayer] )
                {
                    painter.setPen(m_arcolPartsFg[EDivLineLabelsPartLines]);

                    for( idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]; idxDivLine++ )
                    {
                        xDivLine = m_arariDivLines_px[iLayer][idxDivLine];

                        if( m_layoutPos == ELayoutPosTop )
                        {
                            painter.drawLine(xDivLine, m_rectContent.bottom()-m_iSpaceDiagPartCenter2DivLineLabels+2, xDivLine, m_rectContent.bottom());
                        }
                        else
                        {
                            painter.drawLine(xDivLine, m_rectContent.top(), xDivLine, m_rectContent.top()+m_iSpaceDiagPartCenter2DivLineLabels-2);
                        }
                    }
                } // if( m_bShowDivLines || m_bShowSubDivLines )
            } // for( iLayer < CEnumDivLineLayer::count() )

            // Physical unit at the maximum scale value
            //-----------------------------------------

            painter.setFont(m_arfntParts[EDivLineLabelsPartLabels]);
            painter.setPen(m_arcolPartsFg[EDivLineLabelsPartLabels]);

            // Please note that we don't draw the text for the physical unit yet.
            // If it is necessary to indicate the maximum scale value as only
            // one or even no division line label is visible the physical unit should
            // not be indicated at the right border of the diagrams center area but
            // the maximum scale value should be drawn there. In this case the physical
            // unit should be moved to the left. A good choice would be:
            // - behind the minimum scale value label
            // - behind the last visible division line label.
            if( m_arbShowPartsUnit[EDivLineLabelsPartLabels] )
            {
                // But the division line labels should not be placed where the unit
                // string may be indicated ... (see method "intersectsDivLineLabels").
                m_bDivLineLabelsPhysUnitVisible = true;
            }

            // Labels at the division lines
            //------------------------------

            // We start with the highest layer (the main grid lines) as they are
            // "more important" than the sub division lines.
            for( iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
            {
                if( m_ararbShowPartsLayer[EDivLineLabelsPartLabels][iLayer] )
                {
                    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
                    {
                        strTrcMsg = "Division lines:";
                        mthTracer.trace(strTrcMsg);
                    }

                    for( idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]; idxDivLine++ )
                    {
                        strDivLineLabel = m_ararstrDivLineLabels[iLayer][idxDivLine];
                        rectDivLineLabel = m_ararrectDivLineLabels[iLayer][idxDivLine];

                        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
                        {
                            strTrcMsg  = "[";
                            strTrcMsg += QString::number(idxDivLine);
                            strTrcMsg += "]: ";
                            strTrcMsg += strDivLineLabel + "; ";
                            strTrcMsg += "T=" + QString::number(rectDivLineLabel.top()) + ",";
                            strTrcMsg += "B=" + QString::number(rectDivLineLabel.bottom()) + ",";
                            strTrcMsg += "L=" + QString::number(rectDivLineLabel.left()) + ",";
                            strTrcMsg += "R=" + QString::number(rectDivLineLabel.right());
                            mthTracer.trace(strTrcMsg);
                        }

                        // To discover overlapping division line labels the rectangle extents
                        // of the division line labels have been stored in an array:
                        if( m_bAdjustContentRect2DiagPartCenter )
                        {
                            if( rectDivLineLabel.left() > pPixmapDiagram->getRectPartCenter().left()
                             && rectDivLineLabel.right() < pPixmapDiagram->getRectPartCenter().right() )
                            {
                                bool bIntersectsWithExistingDivLineLabel = false;
                                if (idxDivLine > 0) {
                                    bIntersectsWithExistingDivLineLabel =
                                        intersectsDivLineLabels(rectDivLineLabel, iLayer, 0, idxDivLine-1);
                                }
                                if (!bIntersectsWithExistingDivLineLabel)
                                {
                                    m_ararbDivLineLabelsVisible[iLayer][idxDivLine] = true;
                                    idxDivLineLabelLastVisible = idxDivLine;
                                    iLayerOfDivLineLabelLastVisible = iLayer;
                                    iDivLineLabelsVisibleCount++;

                                    if( m_arbrushStyleParts[EDivLineLabelsPartLabels] != Qt::NoBrush )
                                    {
                                        QBrush brush(m_arcolPartsBg[EDivLineLabelsPartLabels],m_arbrushStyleParts[EDivLineLabelsPartLabels]);
                                        painter.fillRect( rectDivLineLabel, brush );
                                    }
                                    painter.drawText(rectDivLineLabel,Qt::AlignHCenter|Qt::AlignVCenter,strDivLineLabel); //lint !e655
                                }
                            }
                        }
                        else
                        {
                            if( rectDivLineLabel.left() >= rectDiag.left()
                             && rectDivLineLabel.right() <= rectDiag.right()
                             && rectDivLineLabel.top() >= rectDiag.top()
                             && rectDivLineLabel.bottom() <= rectDiag.bottom() )
                            {
                                bool bIntersectsWithExistingDivLineLabel = false;
                                if (idxDivLine > 0) {
                                    bIntersectsWithExistingDivLineLabel =
                                        intersectsDivLineLabels(rectDivLineLabel, iLayer, 0, idxDivLine-1);
                                }
                                if (!bIntersectsWithExistingDivLineLabel)
                                {
                                    m_ararbDivLineLabelsVisible[iLayer][idxDivLine] = true;
                                    idxDivLineLabelLastVisible = idxDivLine;
                                    iLayerOfDivLineLabelLastVisible = iLayer;
                                    iDivLineLabelsVisibleCount++;

                                    if( m_arbrushStyleParts[EDivLineLabelsPartLabels] != Qt::NoBrush )
                                    {
                                        QBrush brush(m_arcolPartsBg[EDivLineLabelsPartLabels],m_arbrushStyleParts[EDivLineLabelsPartLabels]);
                                        painter.fillRect( rectDivLineLabel, brush );
                                    }
                                    painter.drawText(rectDivLineLabel,Qt::AlignHCenter|Qt::AlignVCenter,strDivLineLabel); //lint !e655
                                }
                            }
                        }
                    }
                } // if( m_bShowDivLineLabels || m_bShowSubDivLineLabels )
            } // for( iLayer < CEnumDivLineLayer::count() )

            // The unit string may have to be moved.
            // But the division line labels should not be placed where the unit
            // string may be indicated ... (see method "intersectsDivLineLabels").
            // Reset the flag again (has been set before drawing the division lines).
            m_bDivLineLabelsPhysUnitVisible = false;

            // Minimum and maximum scale value
            //---------------------------------

            if( iDivLineLabelsVisibleCount < 2 )
            {
                // Please see note above why the text for the physical unit has not yet been drawn.

                // First calculate the position of the minimum and maximum scale values
                // not yet taking the unit string into account.
                for( idx = 0; idx < 2; idx++ )
                {
                    if( !m_arstrScaleMinMaxVal[idx].isEmpty() && m_arrectScaleMinMaxVal[idx].isValid() )
                    {
                        strDivLineLabel = m_arstrScaleMinMaxVal[idx];
                        rectDivLineLabel = m_arrectScaleMinMaxVal[idx];

                        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
                        {
                            if( idx == 0 )
                            {
                                strTrcMsg  = "ScaleMin: ";
                            }
                            else
                            {
                                strTrcMsg  = "ScaleMax: ";
                            }
                            strTrcMsg += strDivLineLabel + "; ";
                            strTrcMsg += "T=" + QString::number(rectDivLineLabel.top()) + ",";
                            strTrcMsg += "B=" + QString::number(rectDivLineLabel.bottom()) + ",";
                            strTrcMsg += "L=" + QString::number(rectDivLineLabel.left()) + ",";
                            strTrcMsg += "R=" + QString::number(rectDivLineLabel.right());
                            mthTracer.trace(strTrcMsg);
                        }

                        // To discover overlapping division line labels the rectangle extents
                        // of the division line labels have been stored in an array:
                        if( m_bAdjustContentRect2DiagPartCenter )
                        {
                            if( idx == 0 && rectDivLineLabel.left() < pPixmapDiagram->getRectPartCenter().left() )
                            {
                                cxOffset = pPixmapDiagram->getRectPartCenter().left()-rectDivLineLabel.left()+1;
                                rectDivLineLabel.moveLeft( rectDivLineLabel.left()+cxOffset );
                            }
                            else if( idx == 1 && rectDivLineLabel.right() > pPixmapDiagram->getRectPartCenter().right() )
                            {
                                cxOffset = rectDivLineLabel.right()-pPixmapDiagram->getRectPartCenter().right()+1;
                                rectDivLineLabel.moveRight( rectDivLineLabel.right()-cxOffset );
                            }
                            if( !intersectsDivLineLabels(rectDivLineLabel) )
                            {
                                m_arbScaleMinMaxValVisible[idx] = true;
                                m_arrectScaleMinMaxVal[idx] = rectDivLineLabel;
                            }
                        }
                        else
                        {
                            if( rectDivLineLabel.left() >= rectDiag.left()
                             && rectDivLineLabel.right() <= rectDiag.right()
                             && rectDivLineLabel.top() >= rectDiag.top()
                             && rectDivLineLabel.bottom() <= rectDiag.bottom() )
                            {
                                if( !intersectsDivLineLabels(rectDivLineLabel) )
                                {
                                    m_arbScaleMinMaxValVisible[idx] = true;
                                    m_arrectScaleMinMaxVal[idx] = rectDivLineLabel;
                                }
                            }
                        } // if( !m_bAdjustContentRect2DiagPartCenter )
                    } // if( !m_arstrScaleMinMaxVal[idx].isEmpty() && m_arrectScaleMinMaxVal[idx].isValid() )
                } // for( idx < 2 )

                // The maximum scale value should be indicated at the right border of the
                // diagrams center area together with the unit string.

                int cxSpace = 2;

                if( m_arbShowPartsUnit[EDivLineLabelsPartLabels] )
                {
                    // First try to append the unit at the maximum scale value.
                    idx = 1;
                    if( m_arbScaleMinMaxValVisible[idx] )
                    {
                        rectDivLineLabel = m_arrectScaleMinMaxVal[idx];
                        rectDivLineLabel.moveLeft(rectDivLineLabel.left()-m_rectDivLineLabelsPhysUnit.width()-cxSpace);
                        rectDivLineLabel.setWidth(rectDivLineLabel.width()+m_rectDivLineLabelsPhysUnit.width()+cxSpace);

                        if( !intersectsDivLineLabels(rectDivLineLabel) )
                        {
                            rectDivLineLabel.setWidth(m_arrectScaleMinMaxVal[idx].width()+cxSpace);
                            if( m_arbrushStyleParts[EDivLineLabelsPartLabels] != Qt::NoBrush )
                            {
                                QBrush brush(m_arcolPartsBg[EDivLineLabelsPartLabels],m_arbrushStyleParts[EDivLineLabelsPartLabels]);
                                painter.fillRect( rectDivLineLabel, brush );
                            }
                            painter.drawText(rectDivLineLabel,Qt::AlignHCenter|Qt::AlignVCenter,m_arstrScaleMinMaxVal[idx]); //lint !e655

                            m_bDivLineLabelsPhysUnitVisible = true;
                            rectDivLineLabel.moveLeft(m_rectDivLineLabelsPhysUnit.left());
                            rectDivLineLabel.setWidth(m_rectDivLineLabelsPhysUnit.width());
                            if( m_arbrushStyleParts[EDivLineLabelsPartLabels] != Qt::NoBrush )
                            {
                                QBrush brush(m_arcolPartsBg[EDivLineLabelsPartLabels],m_arbrushStyleParts[EDivLineLabelsPartLabels]);
                                painter.fillRect( rectDivLineLabel, brush );
                            }
                            painter.drawText(rectDivLineLabel,Qt::AlignHCenter|Qt::AlignVCenter,m_strPhysUnitLabels); //lint !e655
                        }
                        else
                        {
                            if( m_arbrushStyleParts[EDivLineLabelsPartLabels] != Qt::NoBrush )
                            {
                                QBrush brush(m_arcolPartsBg[EDivLineLabelsPartLabels],m_arbrushStyleParts[EDivLineLabelsPartLabels]);
                                painter.fillRect( m_arrectScaleMinMaxVal[idx], brush );
                            }
                            painter.drawText(m_arrectScaleMinMaxVal[idx],Qt::AlignHCenter|Qt::AlignVCenter,m_arstrScaleMinMaxVal[idx]); //lint !e655
                        }

                    } // if( m_arbScaleMinMaxValVisible[1] )

                    // Second try to append the unit at the minimum scale value.
                    idx = 0;
                    if( m_arbScaleMinMaxValVisible[idx] && !m_bDivLineLabelsPhysUnitVisible )
                    {
                        rectDivLineLabel = m_arrectScaleMinMaxVal[idx];
                        rectDivLineLabel.setWidth(rectDivLineLabel.width()+m_rectDivLineLabelsPhysUnit.width()+cxSpace);

                        if( !intersectsDivLineLabels(rectDivLineLabel) )
                        {
                            rectDivLineLabel.setWidth(m_arrectScaleMinMaxVal[idx].width()+cxSpace);
                            if( m_arbrushStyleParts[EDivLineLabelsPartLabels] != Qt::NoBrush )
                            {
                                QBrush brush(m_arcolPartsBg[EDivLineLabelsPartLabels],m_arbrushStyleParts[EDivLineLabelsPartLabels]);
                                painter.fillRect( rectDivLineLabel, brush );
                            }
                            painter.drawText(rectDivLineLabel,Qt::AlignHCenter|Qt::AlignVCenter,m_arstrScaleMinMaxVal[idx]); //lint !e655

                            m_bDivLineLabelsPhysUnitVisible = true;
                            rectDivLineLabel.moveLeft(m_arrectScaleMinMaxVal[idx].right()+cxSpace);
                            rectDivLineLabel.setWidth(m_rectDivLineLabelsPhysUnit.width());
                            if( m_arbrushStyleParts[EDivLineLabelsPartLabels] != Qt::NoBrush )
                            {
                                QBrush brush(m_arcolPartsBg[EDivLineLabelsPartLabels],m_arbrushStyleParts[EDivLineLabelsPartLabels]);
                                painter.fillRect( rectDivLineLabel, brush );
                            }
                            painter.drawText(rectDivLineLabel,Qt::AlignHCenter|Qt::AlignVCenter,m_strPhysUnitLabels); //lint !e655
                        }
                        else
                        {
                            if( m_arbrushStyleParts[EDivLineLabelsPartLabels] != Qt::NoBrush )
                            {
                                QBrush brush(m_arcolPartsBg[EDivLineLabelsPartLabels],m_arbrushStyleParts[EDivLineLabelsPartLabels]);
                                painter.fillRect( m_arrectScaleMinMaxVal[idx], brush );
                            }
                            painter.drawText(m_arrectScaleMinMaxVal[idx],Qt::AlignHCenter|Qt::AlignVCenter,m_arstrScaleMinMaxVal[idx]); //lint !e655
                        }
                    }
                    else
                    {
                        if( m_arbrushStyleParts[EDivLineLabelsPartLabels] != Qt::NoBrush )
                        {
                            QBrush brush(m_arcolPartsBg[EDivLineLabelsPartLabels],m_arbrushStyleParts[EDivLineLabelsPartLabels]);
                            painter.fillRect( m_arrectScaleMinMaxVal[idx], brush );
                        }
                        painter.drawText(m_arrectScaleMinMaxVal[idx],Qt::AlignHCenter|Qt::AlignVCenter,m_arstrScaleMinMaxVal[idx]); //lint !e655
                    }

                    // Third try to append the unit at the last visible division line label.
                    // There should be at maximum one visible division line at this point.
                    // Otherwise there was no need to indicate the maximum scale value.
                    if( !m_bDivLineLabelsPhysUnitVisible && iLayerOfDivLineLabelLastVisible >= 0 && idxDivLineLabelLastVisible >= 0 )
                    {
                        rectDivLineLabel = m_ararrectDivLineLabels[iLayerOfDivLineLabelLastVisible][idxDivLineLabelLastVisible];
                        rectDivLineLabel.setWidth( rectDivLineLabel.width() + m_rectDivLineLabelsPhysUnit.width() );

                        m_ararbDivLineLabelsVisible[iLayerOfDivLineLabelLastVisible][idxDivLineLabelLastVisible] = false;
                        if( !intersectsDivLineLabels(rectDivLineLabel) )
                        {
                            m_bDivLineLabelsPhysUnitVisible = true;
                            rectDivLineLabel.moveLeft(m_ararrectDivLineLabels[iLayerOfDivLineLabelLastVisible][idxDivLineLabelLastVisible].right()+cxSpace);
                            rectDivLineLabel.setWidth(m_arrectScaleMinMaxVal[1].width()+cxSpace);
                            if( m_arbrushStyleParts[EDivLineLabelsPartLabels] != Qt::NoBrush )
                            {
                                QBrush brush(m_arcolPartsBg[EDivLineLabelsPartLabels],m_arbrushStyleParts[EDivLineLabelsPartLabels]);
                                painter.fillRect( rectDivLineLabel, brush );
                            }
                            painter.drawText(rectDivLineLabel,Qt::AlignHCenter|Qt::AlignVCenter,m_strPhysUnitLabels); //lint !e655
                        }
                        m_ararbDivLineLabelsVisible[iLayerOfDivLineLabelLastVisible][idxDivLineLabelLastVisible] = true;
                    }
                }
                else // if( !m_arbShowPartsUnit[EDivLineLabelsPartLabels] )
                {
                    for( idx = 0; idx < 2; idx++ )
                    {
                        if( m_arbScaleMinMaxValVisible[idx] )
                        {
                            painter.setFont(m_arfntParts[EDivLineLabelsPartLabels]);
                            painter.setPen(m_arcolPartsFg[EDivLineLabelsPartLabels]);

                            strDivLineLabel = m_arstrScaleMinMaxVal[idx];
                            rectDivLineLabel = m_arrectScaleMinMaxVal[idx];

                            if( m_arbrushStyleParts[EDivLineLabelsPartLabels] != Qt::NoBrush )
                            {
                                QBrush brush(m_arcolPartsBg[EDivLineLabelsPartLabels],m_arbrushStyleParts[EDivLineLabelsPartLabels]);
                                painter.fillRect( rectDivLineLabel, brush );
                            }
                            painter.drawText(rectDivLineLabel,Qt::AlignHCenter|Qt::AlignVCenter,strDivLineLabel); //lint !e655

                        } // if( m_arbScaleMinMaxValVisible[idx] )
                    } // for( idx < 2 )
                } // if( !m_arbShowPartsUnit[EDivLineLabelsPartLabels] )
            } // if( iDivLineLabelsVisibleCount < 2 )

            // Physical unit at the maximum scale value
            //-----------------------------------------

            // Please see note above why the text for the physical unit may not have been drawn yet.
            if( m_arbShowPartsUnit[EDivLineLabelsPartLabels] && !m_bDivLineLabelsPhysUnitVisible )
            {
                painter.setFont(m_arfntParts[EDivLineLabelsPartLabels]);
                painter.setPen(m_arcolPartsFg[EDivLineLabelsPartLabels]);

                m_bDivLineLabelsPhysUnitVisible = true;

                if( m_arbrushStyleParts[EDivLineLabelsPartLabels] != Qt::NoBrush )
                {
                    QBrush brush(m_arcolPartsBg[EDivLineLabelsPartLabels],m_arbrushStyleParts[EDivLineLabelsPartLabels]);
                    painter.fillRect( m_rectDivLineLabelsPhysUnit, brush );
                }
                painter.drawText(m_rectDivLineLabelsPhysUnit,Qt::AlignHCenter|Qt::AlignVCenter,m_strPhysUnitLabels); //lint !e655
            }

            // Axis Label
            //-----------

            if( (m_ararbShowPartsLayer[EDivLineLabelsPartAxisLabel][EDivLineLayerMain] || m_arbShowPartsUnit[EDivLineLabelsPartAxisLabel]) && !strAxisLabel.isEmpty() )
            {
                painter.setFont(m_arfntParts[EDivLineLabelsPartAxisLabel]);
                painter.setPen(m_arcolPartsFg[EDivLineLabelsPartAxisLabel]);

                if( m_arbrushStyleParts[EDivLineLabelsPartAxisLabel] != Qt::NoBrush )
                {
                    QBrush brush(m_arcolPartsBg[EDivLineLabelsPartAxisLabel],m_arbrushStyleParts[EDivLineLabelsPartAxisLabel]);
                    painter.fillRect(m_rectAxisLabel,brush);
                }
                if( m_layoutPos == ELayoutPosTop )
                {
                    painter.drawText(m_rectAxisLabel,Qt::AlignVCenter|Qt::AlignHCenter,strAxisLabel); //lint !e655
                }
                else if( m_layoutPos == ELayoutPosBottom )
                {
                    painter.drawText(m_rectAxisLabel,Qt::AlignVCenter|Qt::AlignHCenter,strAxisLabel); //lint !e655
                }
            } // if( m_ararbShowPartsLayer[EDivLineLabelsPartAxisLabel][EDivLineLayerMain] )

            break;

        } // case( PosTop || PosBottom )

        // Y-Axis (horizontal division lines from diagrams center area border to axis labels)
        case ELayoutPosLeft:
        case ELayoutPosRight:
        {
            int iDivLineLabelsVisibleCount = 0;
            int yDivLine;
            int cyOffset;

            // Division lines
            //---------------

            for( iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
            {
                if( m_ararbShowPartsLayer[EDivLineLabelsPartLines][iLayer] )
                {
                    painter.setPen(m_arcolPartsFg[EDivLineLabelsPartLines]);

                    for( idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]; idxDivLine++ )
                    {
                        yDivLine = m_arariDivLines_px[iLayer][idxDivLine];

                        if( m_layoutPos == ELayoutPosLeft )
                        {
                            painter.drawLine(m_rectContent.right()-m_iSpaceDiagPartCenter2DivLineLabels+2,yDivLine,m_rectContent.right(),yDivLine);
                        }
                        else
                        {
                            painter.drawLine(m_rectContent.left(),yDivLine,m_rectContent.left()+m_iSpaceDiagPartCenter2DivLineLabels-2,yDivLine);
                        }
                    }
                }
            } // for( iLayer < CEnumDivLineLayer::count() )

            // Physical unit at the maximum scale value
            //-----------------------------------------

            painter.setFont(m_arfntParts[EDivLineLabelsPartLabels]);
            painter.setPen(m_arcolPartsFg[EDivLineLabelsPartLabels]);

            if( m_arbShowPartsUnit[EDivLineLabelsPartLabels] )
            {
                m_bDivLineLabelsPhysUnitVisible = true;

                if( m_layoutPos == ELayoutPosLeft )
                {
                    if( m_arbrushStyleParts[EDivLineLabelsPartLabels] != Qt::NoBrush )
                    {
                        QBrush brush(m_arcolPartsBg[EDivLineLabelsPartLabels],m_arbrushStyleParts[EDivLineLabelsPartLabels]);
                        painter.fillRect( m_rectDivLineLabelsPhysUnit, brush );
                    }
                    painter.drawText(m_rectDivLineLabelsPhysUnit,Qt::AlignRight|Qt::AlignVCenter,m_strPhysUnitLabels); //lint !e655
                }
                else
                {
                    if( m_iSpaceDiagPartCenter2DivLineLabels >= 0 )
                    {
                        if( m_arbrushStyleParts[EDivLineLabelsPartLabels] != Qt::NoBrush )
                        {
                            QBrush brush(m_arcolPartsBg[EDivLineLabelsPartLabels],m_arbrushStyleParts[EDivLineLabelsPartLabels]);
                            painter.fillRect( m_rectDivLineLabelsPhysUnit, brush );
                        }
                        painter.drawText(m_rectDivLineLabelsPhysUnit,Qt::AlignLeft|Qt::AlignVCenter,m_strPhysUnitLabels); //lint !e655
                    }
                    else
                    {
                        if( m_arbrushStyleParts[EDivLineLabelsPartLabels] != Qt::NoBrush )
                        {
                            QBrush brush(m_arcolPartsBg[EDivLineLabelsPartLabels],m_arbrushStyleParts[EDivLineLabelsPartLabels]);
                            painter.fillRect( m_rectDivLineLabelsPhysUnit, brush );
                        }
                        painter.drawText(m_rectDivLineLabelsPhysUnit,Qt::AlignRight|Qt::AlignVCenter,m_strPhysUnitLabels); //lint !e655
                    }
                }
            }

            // Labels at the division lines
            //-----------------------------

            for( iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
            {
                if( m_ararbShowPartsLayer[EDivLineLabelsPartLabels][iLayer] )
                {
                    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
                    {
                        strTrcMsg = "Division lines:";
                        mthTracer.trace(strTrcMsg);
                    }

                    for( idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]; idxDivLine++ )
                    {
                        strDivLineLabel = m_ararstrDivLineLabels[iLayer][idxDivLine];
                        rectDivLineLabel = m_ararrectDivLineLabels[iLayer][idxDivLine];

                        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
                        {
                            strTrcMsg  = "[";
                            strTrcMsg += QString::number(idxDivLine);
                            strTrcMsg += "]: ";
                            strTrcMsg += strDivLineLabel + "; ";
                            strTrcMsg += "T=" + QString::number(rectDivLineLabel.top()) + ",";
                            strTrcMsg += "B=" + QString::number(rectDivLineLabel.bottom()) + ",";
                            strTrcMsg += "L=" + QString::number(rectDivLineLabel.left()) + ",";
                            strTrcMsg += "R=" + QString::number(rectDivLineLabel.right());
                            mthTracer.trace(strTrcMsg);
                        }

                        if( m_layoutPos == ELayoutPosLeft )
                        {
                            // To discover overlapping division line labels the rectangle extents
                            // of the division line labels have been stored in an array:
                            if( m_bAdjustContentRect2DiagPartCenter )
                            {
                                if( rectDivLineLabel.top() > pPixmapDiagram->getRectPartCenter().top()
                                 && rectDivLineLabel.bottom() < pPixmapDiagram->getRectPartCenter().bottom() )
                                {
                                    bool bIntersectsWithExistingDivLineLabel = false;
                                    if (idxDivLine > 0) {
                                        bIntersectsWithExistingDivLineLabel =
                                            intersectsDivLineLabels(rectDivLineLabel, iLayer, 0, idxDivLine-1);
                                    }
                                    if (!bIntersectsWithExistingDivLineLabel)
                                    {
                                        m_ararbDivLineLabelsVisible[iLayer][idxDivLine] = true;
                                        iDivLineLabelsVisibleCount++;

                                        if( m_arbrushStyleParts[EDivLineLabelsPartLabels] != Qt::NoBrush )
                                        {
                                            QBrush brush(m_arcolPartsBg[EDivLineLabelsPartLabels],m_arbrushStyleParts[EDivLineLabelsPartLabels]);
                                            painter.fillRect( rectDivLineLabel, brush );
                                        }
                                        painter.drawText(rectDivLineLabel,Qt::AlignRight|Qt::AlignVCenter,strDivLineLabel); //lint !e655
                                    }
                                }
                            }
                            else
                            {
                                if( rectDivLineLabel.left() >= rectDiag.left()
                                 && rectDivLineLabel.right() <= rectDiag.right()
                                 && rectDivLineLabel.top() >= rectDiag.top()
                                 && rectDivLineLabel.bottom() <= rectDiag.bottom() )
                                {
                                    bool bIntersectsWithExistingDivLineLabel = false;
                                    if (idxDivLine > 0) {
                                        bIntersectsWithExistingDivLineLabel = intersectsDivLineLabels(rectDivLineLabel, iLayer, 0, idxDivLine-1);
                                    }
                                    if (!bIntersectsWithExistingDivLineLabel)
                                    {
                                        m_ararbDivLineLabelsVisible[iLayer][idxDivLine] = true;
                                        iDivLineLabelsVisibleCount++;

                                        if( m_arbrushStyleParts[EDivLineLabelsPartLabels] != Qt::NoBrush )
                                        {
                                            QBrush brush(m_arcolPartsBg[EDivLineLabelsPartLabels],m_arbrushStyleParts[EDivLineLabelsPartLabels]);
                                            painter.fillRect( rectDivLineLabel, brush );
                                        }
                                        painter.drawText(rectDivLineLabel,Qt::AlignRight|Qt::AlignVCenter,strDivLineLabel); //lint !e655
                                    }
                                }
                            }
                        }
                        else // if( m_layoutPos == ELayoutPosRight )
                        {
                            // To discover overlapping division line labels the rectangle extents
                            // of the division line labels have been stored in an array:
                            if( m_bAdjustContentRect2DiagPartCenter )
                            {
                                if( rectDivLineLabel.top() > pPixmapDiagram->getRectPartCenter().top()
                                 && rectDivLineLabel.bottom() < pPixmapDiagram->getRectPartCenter().bottom() )
                                {
                                    bool bIntersectsWithExistingDivLineLabel = false;
                                    if (idxDivLine > 0) {
                                        bIntersectsWithExistingDivLineLabel =
                                            intersectsDivLineLabels(rectDivLineLabel, iLayer, 0, idxDivLine-1);
                                    }
                                    if (!bIntersectsWithExistingDivLineLabel)
                                    {
                                        m_ararbDivLineLabelsVisible[iLayer][idxDivLine] = true;
                                        iDivLineLabelsVisibleCount++;

                                        if( m_iSpaceDiagPartCenter2DivLineLabels >= 0 )
                                        {
                                            if( m_arbrushStyleParts[EDivLineLabelsPartLabels] != Qt::NoBrush )
                                            {
                                                QBrush brush(m_arcolPartsBg[EDivLineLabelsPartLabels],m_arbrushStyleParts[EDivLineLabelsPartLabels]);
                                                painter.fillRect( rectDivLineLabel, brush );
                                            }
                                            painter.drawText(rectDivLineLabel,Qt::AlignLeft|Qt::AlignVCenter,strDivLineLabel); //lint !e655
                                        }
                                        else
                                        {
                                            if( m_arbrushStyleParts[EDivLineLabelsPartLabels] != Qt::NoBrush )
                                            {
                                                QBrush brush(m_arcolPartsBg[EDivLineLabelsPartLabels],m_arbrushStyleParts[EDivLineLabelsPartLabels]);
                                                painter.fillRect( rectDivLineLabel, brush );
                                            }
                                            painter.drawText(rectDivLineLabel,Qt::AlignRight|Qt::AlignVCenter,strDivLineLabel); //lint !e655
                                        }
                                    }
                                }
                            }
                            else
                            {
                                if( rectDivLineLabel.left() >= rectDiag.left()
                                 && rectDivLineLabel.right() <= rectDiag.right()
                                 && rectDivLineLabel.top() >= rectDiag.top()
                                 && rectDivLineLabel.bottom() <= rectDiag.bottom() )
                                {
                                    bool bIntersectsWithExistingDivLineLabel = false;
                                    if (idxDivLine > 0) {
                                        bIntersectsWithExistingDivLineLabel =
                                            intersectsDivLineLabels(rectDivLineLabel, iLayer, 0, idxDivLine-1);
                                    }
                                    if (!bIntersectsWithExistingDivLineLabel)
                                    {
                                        m_ararbDivLineLabelsVisible[iLayer][idxDivLine] = true;
                                        iDivLineLabelsVisibleCount++;

                                        if( m_iSpaceDiagPartCenter2DivLineLabels >= 0 )
                                        {
                                            if( m_arbrushStyleParts[EDivLineLabelsPartLabels] != Qt::NoBrush )
                                            {
                                                QBrush brush(m_arcolPartsBg[EDivLineLabelsPartLabels],m_arbrushStyleParts[EDivLineLabelsPartLabels]);
                                                painter.fillRect( rectDivLineLabel, brush );
                                            }
                                            painter.drawText(rectDivLineLabel,Qt::AlignLeft|Qt::AlignVCenter,strDivLineLabel); //lint !e655
                                        }
                                        else
                                        {
                                            if( m_arbrushStyleParts[EDivLineLabelsPartLabels] != Qt::NoBrush )
                                            {
                                                QBrush brush(m_arcolPartsBg[EDivLineLabelsPartLabels],m_arbrushStyleParts[EDivLineLabelsPartLabels]);
                                                painter.fillRect( rectDivLineLabel, brush );
                                            }
                                            painter.drawText(rectDivLineLabel,Qt::AlignRight|Qt::AlignVCenter,strDivLineLabel); //lint !e655
                                        }
                                    }
                                }
                            } // else if( m_bAdjustContentRect2DiagPartCenter )
                        } // if( m_layoutPos == ELayoutPosRight )
                    } // for( idxDivLine < m_uDivLineLabelsCount[iLayer] )
                } // if( m_bShowDivLineLabels || m_bShowSubDivLineLabels )
            } // for( iLayer < CEnumDivLineLayer::count() )

            // Minimum and maximum scale value
            //---------------------------------

            if( iDivLineLabelsVisibleCount < 2 )
            {
                for( idx = 0; idx < 2; idx++ )
                {
                    if( !m_arstrScaleMinMaxVal[idx].isEmpty() && m_arrectScaleMinMaxVal[idx].isValid() )
                    {
                        strDivLineLabel = m_arstrScaleMinMaxVal[idx];
                        rectDivLineLabel = m_arrectScaleMinMaxVal[idx];

                        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
                        {
                            if( idx == 0 )
                            {
                                strTrcMsg  = "ScaleMin: ";
                            }
                            else
                            {
                                strTrcMsg  = "ScaleMax: ";
                            }
                            strTrcMsg += strDivLineLabel + "; ";
                            strTrcMsg += "T=" + QString::number(rectDivLineLabel.top()) + ",";
                            strTrcMsg += "B=" + QString::number(rectDivLineLabel.bottom()) + ",";
                            strTrcMsg += "L=" + QString::number(rectDivLineLabel.left()) + ",";
                            strTrcMsg += "R=" + QString::number(rectDivLineLabel.right());
                            mthTracer.trace(strTrcMsg);
                        }

                        // To discover overlapping division line labels the rectangle extents
                        // of the division line labels have been stored in an array:
                        if( m_layoutPos == ELayoutPosLeft )
                        {
                            // To discover overlapping division line labels the rectangle extents
                            // of the division line labels have been stored in an array:
                            if( m_bAdjustContentRect2DiagPartCenter )
                            {
                                if( idx == 0 && rectDivLineLabel.top() < pPixmapDiagram->getRectPartCenter().top() )
                                {
                                    cyOffset = pPixmapDiagram->getRectPartCenter().top()-rectDivLineLabel.top()+1;
                                    rectDivLineLabel.moveTop( rectDivLineLabel.top()+cyOffset );
                                }
                                else if( idx == 1 && rectDivLineLabel.bottom() > pPixmapDiagram->getRectPartCenter().bottom() )
                                {
                                    cyOffset = rectDivLineLabel.bottom()-pPixmapDiagram->getRectPartCenter().bottom()+1;
                                    rectDivLineLabel.moveBottom( rectDivLineLabel.bottom()-cyOffset );
                                }
                                if( !intersectsDivLineLabels(rectDivLineLabel) )
                                {
                                    m_arbScaleMinMaxValVisible[idx] = true;
                                    m_arrectScaleMinMaxVal[idx] = rectDivLineLabel;

                                    if( m_arbrushStyleParts[EDivLineLabelsPartLabels] != Qt::NoBrush )
                                    {
                                        QBrush brush(m_arcolPartsBg[EDivLineLabelsPartLabels],m_arbrushStyleParts[EDivLineLabelsPartLabels]);
                                        painter.fillRect( rectDivLineLabel, brush );
                                    }
                                    painter.drawText(rectDivLineLabel,Qt::AlignRight|Qt::AlignVCenter,strDivLineLabel); //lint !e655
                                }
                            }
                            else
                            {
                                if( rectDivLineLabel.left() >= rectDiag.left()
                                 && rectDivLineLabel.right() <= rectDiag.right()
                                 && rectDivLineLabel.top() >= rectDiag.top()
                                 && rectDivLineLabel.bottom() <= rectDiag.bottom() )
                                {
                                    if( !intersectsDivLineLabels(rectDivLineLabel) )
                                    {
                                        m_arbScaleMinMaxValVisible[idx] = true;
                                        m_arrectScaleMinMaxVal[idx] = rectDivLineLabel;

                                        if( m_arbrushStyleParts[EDivLineLabelsPartLabels] != Qt::NoBrush )
                                        {
                                            QBrush brush(m_arcolPartsBg[EDivLineLabelsPartLabels],m_arbrushStyleParts[EDivLineLabelsPartLabels]);
                                            painter.fillRect( rectDivLineLabel, brush );
                                        }
                                        painter.drawText(rectDivLineLabel,Qt::AlignRight|Qt::AlignVCenter,strDivLineLabel); //lint !e655
                                    }
                                }
                            }
                        }
                        else // if( m_layoutPos == ELayoutPosRight )
                        {
                            // To discover overlapping division line labels the rectangle extents
                            // of the division line labels have been stored in an array:
                            if( m_bAdjustContentRect2DiagPartCenter )
                            {
                                if( idx == 0 && rectDivLineLabel.top() < pPixmapDiagram->getRectPartCenter().top() )
                                {
                                    cyOffset = pPixmapDiagram->getRectPartCenter().top()-rectDivLineLabel.top()+1;
                                    m_rectDivLineLabelsPhysUnit.moveTop( m_rectDivLineLabelsPhysUnit.top()+cyOffset );
                                }
                                else if( idx == 1 && rectDivLineLabel.bottom() > pPixmapDiagram->getRectPartCenter().bottom() )
                                {
                                    cyOffset = rectDivLineLabel.bottom()-pPixmapDiagram->getRectPartCenter().bottom()+1;
                                    m_rectDivLineLabelsPhysUnit.moveBottom( m_rectDivLineLabelsPhysUnit.bottom()-cyOffset );
                                }
                                if( !intersectsDivLineLabels(rectDivLineLabel) )
                                {
                                    m_arbScaleMinMaxValVisible[idx] = true;
                                    m_arrectScaleMinMaxVal[idx] = rectDivLineLabel;

                                    if( m_iSpaceDiagPartCenter2DivLineLabels >= 0 )
                                    {
                                        if( m_arbrushStyleParts[EDivLineLabelsPartLabels] != Qt::NoBrush )
                                        {
                                            QBrush brush(m_arcolPartsBg[EDivLineLabelsPartLabels],m_arbrushStyleParts[EDivLineLabelsPartLabels]);
                                            painter.fillRect( rectDivLineLabel, brush );
                                        }
                                        painter.drawText(rectDivLineLabel,Qt::AlignLeft|Qt::AlignVCenter,strDivLineLabel); //lint !e655
                                    }
                                    else
                                    {
                                        if( m_arbrushStyleParts[EDivLineLabelsPartLabels] != Qt::NoBrush )
                                        {
                                            QBrush brush(m_arcolPartsBg[EDivLineLabelsPartLabels],m_arbrushStyleParts[EDivLineLabelsPartLabels]);
                                            painter.fillRect( rectDivLineLabel, brush );
                                        }
                                        painter.drawText(rectDivLineLabel,Qt::AlignRight|Qt::AlignVCenter,strDivLineLabel); //lint !e655
                                    }
                                }
                            }
                            else
                            {
                                if( rectDivLineLabel.left() >= rectDiag.left()
                                 && rectDivLineLabel.right() <= rectDiag.right()
                                 && rectDivLineLabel.top() >= rectDiag.top()
                                 && rectDivLineLabel.bottom() <= rectDiag.bottom() )
                                {
                                    if( !intersectsDivLineLabels(rectDivLineLabel) )
                                    {
                                        m_arbScaleMinMaxValVisible[idx] = true;
                                        m_arrectScaleMinMaxVal[idx] = rectDivLineLabel;

                                        if( m_iSpaceDiagPartCenter2DivLineLabels >= 0 )
                                        {
                                            if( m_arbrushStyleParts[EDivLineLabelsPartLabels] != Qt::NoBrush )
                                            {
                                                QBrush brush(m_arcolPartsBg[EDivLineLabelsPartLabels],m_arbrushStyleParts[EDivLineLabelsPartLabels]);
                                                painter.fillRect( rectDivLineLabel, brush );
                                            }
                                            painter.drawText(rectDivLineLabel,Qt::AlignLeft|Qt::AlignVCenter,strDivLineLabel); //lint !e655
                                        }
                                        else
                                        {
                                            if( m_arbrushStyleParts[EDivLineLabelsPartLabels] != Qt::NoBrush )
                                            {
                                                QBrush brush(m_arcolPartsBg[EDivLineLabelsPartLabels],m_arbrushStyleParts[EDivLineLabelsPartLabels]);
                                                painter.fillRect( rectDivLineLabel, brush );
                                            }
                                            painter.drawText(rectDivLineLabel,Qt::AlignRight|Qt::AlignVCenter,strDivLineLabel); //lint !e655
                                        }
                                    }
                                }
                            } // else if( m_bAdjustContentRect2DiagPartCenter )
                        } // if( m_layoutPos == ELayoutPosRight )
                    }
                }
            } // if( iDivLineLabelsVisibleCount < 2 )

            // Axis Label
            //-----------

            if( (m_ararbShowPartsLayer[EDivLineLabelsPartAxisLabel][EDivLineLayerMain] || m_arbShowPartsUnit[EDivLineLabelsPartAxisLabel]) && !strAxisLabel.isEmpty() )
            {
                // Calculate extent of axis label:
                rectAxisLabel = fntmtr.boundingRect(strAxisLabel);
                if( rectAxisLabel.width() > m_rectAxisLabel.width() )
                {
                    // As the label will be rotated by 90 degrees, the width will become the height:
                    m_rectAxisLabel.setHeight( rectAxisLabel.width() );
                }

                m_rectAxisLabel.moveTop( pPixmapDiagram->getRectPartCenter().center().y() - m_rectAxisLabel.height()/2 );

                #if QT_VERSION < 0x050000
                QMatrix matrix;
                #else
                QTransform matrix;
                #endif
                painter.save();
                matrix.translate( static_cast<double>(m_rectAxisLabel.center().x()), static_cast<double>(m_rectAxisLabel.center().y()) );
                matrix.rotate(-90.0);
                #if QT_VERSION < 0x050000
                painter.setWorldMatrix(matrix);
                #else
                painter.setWorldTransform(matrix);
                #endif
                //painter.setWorldTransform(matrix);
                painter.setFont(m_arfntParts[EDivLineLabelsPartAxisLabel]);
                painter.setPen(m_arcolPartsFg[EDivLineLabelsPartAxisLabel]);
                if( m_arbrushStyleParts[EDivLineLabelsPartAxisLabel] != Qt::NoBrush )
                {
                    QBrush brush(m_arcolPartsBg[EDivLineLabelsPartAxisLabel],m_arbrushStyleParts[EDivLineLabelsPartAxisLabel]);
                    painter.fillRect(m_rectAxisLabel,brush);
                }
                painter.drawText(
                    /* x   */ -m_rectAxisLabel.height()/2,
                    /* y   */ m_rectAxisLabel.width()/2,
                    /* str */ strAxisLabel );
                painter.restore();

            } // if( m_ararbShowPartsLayer[EDivLineLabelsPartAxisLabel][EDivLineLayerMain] )

            break;

        } // case( PosLeft || PosRight )

        default:
        {
            break;
        }

    } // switch( m_layoutPos )

    // Mark current process depth as executed (reset bit)
    //---------------------------------------------------

    validate(EUpdatePixmap);

} // updatePixmap

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::updateWidget()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateWidget",
        /* strAddInfo   */ "" );

    if( m_pDiagram->getUpdateType() >= EDiagramUpdateTypeWidget )
    {
        // Invalidate output region of the diagram object to update (repaint) content of diagram.
        if( m_rectContentPrev.isValid() )
        {
            m_pDiagram->update(this, m_rectContentPrev);
            m_rectContentPrev = m_rectContent;
        }
        if( m_rectContent.isValid() )
        {
            m_pDiagram->update(this, m_rectContent);
        }

    } // if( pWdgtDiagram != nullptr )

    // Mark current process depth as executed (reset bit)
    //---------------------------------------------------

    validate(EUpdateWidget);

} // updateWidget

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the rectangle part used to draw the axis taking the
           left, right, bottom and top margins into account.

    @return Rectangle within the diagram to draw axis labels.
*/
QRect CDiagObjAxisLabel::getDiagRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "getDiagRect",
        /* strAddInfo   */ "" );

    QRect rectDiag(0, 0, 1, 1);

    const CPixmapDiagram* pPixmapDiagram = dynamic_cast<const CPixmapDiagram*>(m_pDiagram);
    if( pPixmapDiagram != nullptr )
    {
        rectDiag.setLeft(pPixmapDiagram->getMarginLeft());
        rectDiag.setWidth(pPixmapDiagram->getWidth()
                        - pPixmapDiagram->getMarginLeft()
                        - pPixmapDiagram->getMarginRight());
        rectDiag.setTop(pPixmapDiagram->getMarginTop());
        rectDiag.setHeight(pPixmapDiagram->getHeight()
                         - pPixmapDiagram->getMarginTop()
                         - pPixmapDiagram->getMarginBottom());
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rectDiag) + "}");
    }
    return rectDiag;
}

//------------------------------------------------------------------------------
/*! @brief Returns the unit in which the labels should be indicated.

    If the unit has not been explicitly set the best unit to indicate the values
    is calculated. The best unit is the unit in which the values are displayed
    with at least one but no more than three digits before the decimal point.

    @return Unit in which the labels should be indicated.
*/
CUnit CDiagObjAxisLabel::getAxisLabelUnit() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "getAxisLabelUnit",
        /* strAddInfo   */ "" );

    PhysVal::CUnit unit;

    // If the unit, in which the division lines should be labeled, has been explicitly specified ...
    if( m_pUnit != nullptr )
    {
        unit = *m_pUnit;
    }
    // If the "best" unit to indicate the values should be used ...
    else
    {
        CUnit  unitScaleMinValTmp;
        double fScaleMinValTmp;
        int    iDigitsLeadingScaleMinVal;
        int    iDigitsTrailingScaleMinVal;

        unit = m_pDiagScale->getScale().unit();

        double fScaleMinVal = m_pDiagScale->getScale().minVal().getVal();
        double fScaleMaxVal = m_pDiagScale->getScale().maxVal().getVal();

        TFormatResult formatResult = PhysVal::formatValue(
            /* fValue                */ fScaleMinVal,
            /* unitVal               */ unit,
            /* iDigitsMantissaMax    */ 0,
            /* bAccuracyLimitsMant.  */ false,
            /* iDigitsAccuracy       */ 0,
            /* iDigitsExponent       */ 0,
            /* bUseEngineeringFormat */ false,
            /* pfVal                 */ &fScaleMinValTmp,
            /* pstr                  */ nullptr,
            /* pUnit                 */ &unitScaleMinValTmp,
            /* piDigitsLeading       */ &iDigitsLeadingScaleMinVal,
            /* piDigitsTrailing      */ &iDigitsTrailingScaleMinVal );

        if( !(formatResult & FormatResult::Error) ) //lint !e655
        {
            CUnit  unitScaleMaxValTmp;
            double fScaleMaxValTmp;
            int    iDigitsLeadingScaleMaxVal;
            int    iDigitsTrailingScaleMaxVal;

            formatResult = PhysVal::formatValue(
                /* fValue                */ fScaleMaxVal,
                /* unitVal               */ unit,
                /* iDigitsMantissaMax    */ 0,
                /* bAccuracyLimitsMant.  */ false,
                /* iDigitsAccuracy       */ 0,
                /* iDigitsExponent       */ 0,
                /* bUseEngineeringFormat */ false,
                /* pfVal                 */ &fScaleMaxValTmp,
                /* pstr                  */ nullptr,
                /* pUnit                 */ &unitScaleMaxValTmp,
                /* piDigitsLeading       */ &iDigitsLeadingScaleMaxVal,
                /* piDigitsTrailing      */ &iDigitsTrailingScaleMaxVal );

            if( !(formatResult & FormatResult::Error) ) //lint !e655
            {
                // If the best unit for the scale minimum and scale maximum value is the same ...
                if( unitScaleMinValTmp == unitScaleMaxValTmp )
                {
                    unit = unitScaleMinValTmp;
                }
                // If the best unit for the scale minimum and scale maximum value is different ...
                else
                {
                    // ... we need to decide which of both is the best unit to indicate the
                    // labels at the division lines.

                    // If the minimum scale value was 0.0 ...
                    if( fabs(fScaleMinVal) == 0.0 )
                    {
                        // .. we take the best unit for the scale minimum value.
                        unit = unitScaleMaxValTmp;
                    }
                    // If the maximum scale value was 0.0 ...
                    else if( fabs(fScaleMaxVal) == 0.0 )
                    {
                        // .. we take the best unit for the scale minimum value.
                        unit = unitScaleMinValTmp;
                    }
                    // If the absolute value of both values were greater or equal than 1.0 ...
                    else if( fabs(fScaleMinVal) >= 1.0 && fabs(fScaleMaxVal) >= 1.0 )
                    {
                        // .. we take the best unit for the absolute greater value.
                        if( fabs(fScaleMinVal) < fabs(fScaleMaxVal) )
                        {
                            unit = unitScaleMaxValTmp;
                        }
                        else
                        {
                            unit = unitScaleMinValTmp;
                        }
                    }
                    // If the absolute value of both values were less or equal than 1.0 ...
                    else if( fabs(fScaleMinVal) <= 1.0 && fabs(fScaleMaxVal) <= 1.0 )
                    {
                        // .. we take the best unit for the absolute lower value.
                        if( fabs(fScaleMinVal) < fabs(fScaleMaxVal) )
                        {
                            unit = unitScaleMinValTmp;
                        }
                        else
                        {
                            unit = unitScaleMaxValTmp;
                        }
                    }
                    // If the absolute value of the scale max value is greater or equal than 1.0 ...
                    else if( fabs(fScaleMaxVal) >= 1.0 )
                    {
                        // .. we take the best unit of the scale max value.
                        unit = unitScaleMaxValTmp;
                    }
                    // If the absolute value of the scale max value is less than 1.0 ...
                    else // if( fabs(fScaleMaxVal) < 1.0 )
                    {
                        // .. we take the best unit of the scale min value.
                        unit = unitScaleMinValTmp;
                    }
                }
            }
        }
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(unit.symbol());
    }
    return unit;
}

//------------------------------------------------------------------------------
/*! @brief Checks wheter the axis label is visible.

    The axis label indicates the physical size and the unit.

    @return true if the axis label should be indicated.
*/
bool CDiagObjAxisLabel::isAxisLabelVisible() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "isAxisLabelVisible",
        /* strAddInfo   */ "" );

    bool bVisible = (m_ararbShowPartsLayer[EDivLineLabelsPartAxisLabel][EDivLineLayerMain]
                  || m_arbShowPartsUnit[EDivLineLabelsPartAxisLabel]);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bVisible);
    }
    return bVisible;
}

//------------------------------------------------------------------------------
/*! @brief Returns the bounding rectangle of the axis label.

    @return Bounding rectangle of the axis label.
*/
QRect CDiagObjAxisLabel::getAxisLabelBoundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "getAxisLabelBoundingRect",
        /* strAddInfo   */ "" );

    QRect rectAxisLabel;

    if (isAxisLabelVisible())
    {
        // Calculate extent of axis label:
        QFontMetrics fntmtr(m_arfntParts[EDivLineLabelsPartAxisLabel]);
        QString strAxisLabel = m_strAxisLabel;

        if( m_arbShowPartsUnit[EDivLineLabelsPartAxisLabel] )
        {
            if( !strAxisLabel.isEmpty() )
            {
                strAxisLabel += " / ";
            }
            strAxisLabel += m_strPhysUnitLabels;
        }
        rectAxisLabel = fntmtr.boundingRect(strAxisLabel);
        rectAxisLabel.setHeight(rectAxisLabel.height() + 2);
        rectAxisLabel.setWidth(rectAxisLabel.width() + 2);

        int cxWidth  = rectAxisLabel.width();
        int cyHeight = rectAxisLabel.height();

        // Please note that we need to access the diagrams geometry as the
        // "calculateAxisLabelRect" method might have been invoked to get the
        // sizeHint for the content rectangle and the content rectangle will
        // be changed by the diagram later on.
        switch (m_layoutPos)
        {
            case ELayoutPosLeft:
            case ELayoutPosRight:
            {
                // The label will be drawn rotated by 90 degrees.
                // The width becomes the height.
                rectAxisLabel.setHeight(cxWidth);
                rectAxisLabel.setWidth(cyHeight);
                break;
            }
            case ELayoutPosTop:
            case ELayoutPosBottom:
            default:
            {
                break;
            }
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rectAxisLabel) + "}");
    }
    return rectAxisLabel;
}

//------------------------------------------------------------------------------
QVector<double> CDiagObjAxisLabel::getDivLineValsSorted() const
//------------------------------------------------------------------------------
{
    QVector<double> arfValsMain;
    int iValCountMain = m_pDiagScale->getDivLineCount(EDivLineLayerMain);
    for (int idxVal = 0; idxVal < iValCountMain; ++idxVal) {
        double fVal = m_pDiagScale->getDivLineVal(EDivLineLayerMain, idxVal, &m_unitLabels);
        arfValsMain.append(fVal);
    }
    QVector<double> arfValsSub;
    int iValCountSub = m_pDiagScale->getDivLineCount(EDivLineLayerSub);
    for (int idxVal = 0; idxVal < iValCountSub; ++idxVal) {
        double fVal = m_pDiagScale->getDivLineVal(EDivLineLayerSub, idxVal, &m_unitLabels);
        arfValsSub.append(fVal);
    }
    return Math::merge(arfValsMain, arfValsSub);
}

//------------------------------------------------------------------------------
/*! @brief Checks whetger the given rectangle intersects any of the existing
           rectangles used to label the axis.

    @param i_rect [in]
        Rectangle to be checked whether it intersects any of the existing label rectangles.
    @param i_eDivLineLayer [in]
        Div line layer to be checked.
        Invalild enumerator to check all layers (to be checked whether it intersects any of the existing label rectangles.
    @param i_idxDivLineLabelMin [in]
        Index of first division line to be checked.
    @param i_idxDivLineLabelMax [in]
        Index of last division line to be checked.

    @return true if the passed rectangle intersects any of the existing div line labels,
            false otherwise.
*/
bool CDiagObjAxisLabel::intersectsDivLineLabels(
    const QRect& i_rect,
    const CEnumDivLineLayer& i_eDivLineLayer,
    int i_idxDivLineLabelMin,
    int i_idxDivLineLabelMax ) const
//------------------------------------------------------------------------------
{
    QString strTrcMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjUpdate, EMethodTraceDetailLevel::ArgsNormal)) {
        strTrcMthInArgs = "Rect {" + qRect2Str(i_rect) + "}"
            + ", Layer: " + QString(i_eDivLineLayer.isValid() ? i_eDivLineLayer.toString() : "Undefined")
            + ", IdxMin: " + QString::number(i_idxDivLineLabelMin)
            + ", IdxMax: " + QString::number(i_idxDivLineLabelMax);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "intersectsDivLineLabels",
        /* strAddInfo   */ strTrcMthInArgs );

    bool bIntersect = intersectsDivLineLabelsPhysUnit(i_rect);

    int iLayerMax = CEnumDivLineLayer::count()-1;

    if( i_eDivLineLayer.isValid() )
    {
        iLayerMax = i_eDivLineLayer.enumeratorAsInt();
        if( iLayerMax > CEnumDivLineLayer::count()-1 )
        {
            iLayerMax = CEnumDivLineLayer::count()-1;
        }
    }

    int idxDivLineMin = 0;
    int idxDivLineMax = 0;

    // Check the "lower" layers ...
    if( !bIntersect && iLayerMax > 0 )
    {
        for( int iLayer = 0; iLayer < iLayerMax; iLayer++ )
        {
            if( m_ariDivLinesCount[iLayer] == 0 )
            {
                bIntersect = false;
            }
            else
            {
                idxDivLineMin = 0;
                idxDivLineMax = m_ariDivLinesCount[iLayer]-1;

                for( int idxDivLine = idxDivLineMin; idxDivLine <= idxDivLineMax; idxDivLine++ )
                {
                    if( m_ararbDivLineLabelsVisible[iLayer][idxDivLine] )
                    {
                        if( i_rect.intersects(m_ararrectDivLineLabels[iLayer][idxDivLine]) )
                        {
                            bIntersect = true;
                            break;
                        }
                    }
                }
            }
            if( bIntersect )
            {
                break;
            }
        }  // for( iLayer < )
    } // if( !bIntersect )

    // Check the specified "highest" layer ...
    if( !bIntersect && i_idxDivLineLabelMin <= i_idxDivLineLabelMax )
    {
        const int iLayer = iLayerMax;

        if( m_ariDivLinesCount[iLayer] == 0 )
        {
            bIntersect = false;
        }
        else
        {
            if( i_idxDivLineLabelMin < 0 )
            {
                idxDivLineMin = 0;
            }
            else
            {
                idxDivLineMin = i_idxDivLineLabelMin;
            }
            if( i_idxDivLineLabelMax < 0 )
            {
                idxDivLineMax = m_ariDivLinesCount[iLayer]-1;
            }
            else
            {
                idxDivLineMax = i_idxDivLineLabelMax;
            }
            if( idxDivLineMin >= m_ariDivLinesCount[iLayer] )
            {
                idxDivLineMin = m_ariDivLinesCount[iLayer]-1;
            }
            if( idxDivLineMax >= m_ariDivLinesCount[iLayer] )
            {
                idxDivLineMax = m_ariDivLinesCount[iLayer]-1;
            }
            for( int idxDivLine = idxDivLineMin; idxDivLine <= idxDivLineMax; idxDivLine++ )
            {
                if( m_ararbDivLineLabelsVisible[iLayer][idxDivLine] )
                {
                    if( i_rect.intersects(m_ararrectDivLineLabels[iLayer][idxDivLine]) )
                    {
                        bIntersect = true;
                        break;
                    }
                }
            }
        }
    } // if( !bIntersect )

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bIntersect);
    }
    return bIntersect;

} // intersectsDivLineLabels

//------------------------------------------------------------------------------
/*! @brief Checks whether the given rectangle intersects the label showing the
           unit of the axis.

    @param i_rect [in]
        Rectangle to be checked whether it intersects any of the existing label rectangles.

    @return true if the passed rectangle intersects the unit label, false otherwise.
*/
bool CDiagObjAxisLabel::intersectsDivLineLabelsPhysUnit(const QRect& i_rect) const
//------------------------------------------------------------------------------
{
    QString strTrcMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjUpdate, EMethodTraceDetailLevel::ArgsNormal)) {
        strTrcMthInArgs = qRect2Str(i_rect);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "intersectsDivLineLabelsPhysUnit",
        /* strAddInfo   */ strTrcMthInArgs );

    bool bIntersect = false;

    if( m_arbShowPartsUnit[EDivLineLabelsPartLabels] && m_bDivLineLabelsPhysUnitVisible )
    {
        if( i_rect.intersects(m_rectDivLineLabelsPhysUnit) )
        {
            bIntersect = true;
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bIntersect);
    }
    return bIntersect;
}

//------------------------------------------------------------------------------
QString CDiagObjAxisLabel::formatValue( double i_fVal, CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    // Formats the value in the given unit into a string according to the internally
    // calculated number of leading and trailing digits and the internally calculated
    // physical unit used to indicate the labels at the division lines.

    QString strVal;
    double  fVal = i_fVal;
    CUnit   unit = m_unitLabels;

    if( i_pUnit != nullptr )
    {
        unit = *i_pUnit;
    }
    if( unit != m_unitLabels )
    {
        fVal = unit.convertValue(fVal, m_unitLabels);
    }
    if( m_iDivLineLabelsExponentDigits <= 0 )
    {
        strVal = QString::number(
            /* fVal       */ fVal,
            /* chFormat   */ 'f',
            /* iPrecision */ m_iDivLineLabelsTrailingDigits );
    }
    else
    {
        strVal = QString::number(
            /* fVal       */ fVal,
            /* chFormat   */ 'e',
            /* iPrecision */ m_iDivLineLabelsTrailingDigits );
    }
    return strVal;

} // formatValue
