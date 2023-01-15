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
#include "ZSDiagram/ZSDiagramProcWdgt.h"
#include "ZSDiagram/ZSDiagScale.h"
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

#ifdef LINUX
const int c_iMaxTextExtentAddHeight = 4; // unter Linux werden sonst die Labels abgeschnitten
const int c_iMaxTextExtentAddWidth  = 0;
#else
const int c_iMaxTextExtentAddHeight = 0;
const int c_iMaxTextExtentAddWidth  = 0;
#endif

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
        /* strObjName  */ i_strObjName,
        /* pDiagScaleX */ nullptr,
        /* pDiagScaleY */ nullptr,
        /* layoutPos   */ i_layoutPos ),
    m_pDiagScale(i_pDiagScale),
    m_pUnit(nullptr), // as default: "use best unit"
    m_unitLabels(i_pDiagScale->getScale().m_unit),
    m_strPhysUnitLabels(),
    m_iSpaceDiagPartCenter2DivLineLabels(4),
    m_iSpaceDivLineLabels2AxisLabel(2),
    m_iSpaceDiagBorder2AxisLabel(2),
    //m_ararbShow[EPartCount][EDivLineLayerCount]
    //m_ararbShowUnit[EPartCount]
    //m_arcolFg[EPartCount]
    //m_arpenStyle[EPartCount]
    //m_arcolBg[EPartCount]
    //m_arbrushStyle[EPartCount]
    //m_arfnt[EPartCount]
    m_iDivLineLabelsDigitsCountMax(-1),
    m_bUseEngineeringFormat(false),
    m_strAxisLabel(i_strAxisLabel),
    m_rectAxisLabel(),
    m_iDivLineLabelsTrailingDigits(1),
    m_iDivLineLabelsExponentDigits(0),
    m_rectDivLineLabelsMaxTextExtent(),
    m_cxDivLineLabelsSpace(8),
    //m_ariDivLinesCount[EDivLineLayerCount],
    //m_arpiDivLines[EDivLineLayerCount],
    //m_arpRectDivLineLabels[EDivLineLayerCount],
    //m_arpStrDivLineLabels[EDivLineLayerCount],
    //m_arpbDivLineLabelsVisible[EDivLineLayerCount],
    m_rectDivLineLabelsPhysUnit(),
    m_bDivLineLabelsPhysUnitVisible(false),
    //m_arstrScaleMinMaxVal[2],
    //m_arrectScaleMinMaxVal[2],
    //m_arbScaleMinMaxValVisible[2],
    m_iAxisLabelsToBeConsideredListSize(0),
    m_iAxisLabelsToBeConsidered(0),
    m_arpAxisLabelsToBeConsidered(nullptr)
{
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Diagram", "CDiagObjAxisLabel", m_strObjName);

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
        case EScaleDirX:
        {
            m_arpDiagScale[EScaleDirX] = m_pDiagScale;
            break;
        }
        case EScaleDirY:
        {
            m_arpDiagScale[EScaleDirY] = m_pDiagScale;
            break;
        }
        default:
        {
            break;
        }
    }

    // Further initialization of instance members (arrays)
    //----------------------------------------------------

    int iPart;
    int iLayer;

    for( iPart = 0; iPart < EPartCount; iPart++ )
    {
        for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
        {
            m_ararbShow[iPart][iLayer] = false;
        }
        m_arbShowUnit[iPart] = false;
        m_arcolFg[iPart] = Qt::black;
        m_arpenStyle[iPart] = Qt::SolidLine;
        m_arcolBg[iPart] = Qt::gray;
        m_arbrushStyle[iPart] = Qt::NoBrush;
    }
    for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
    {
        m_ariDivLinesCount[iLayer] = 0;
        m_arpiDivLines[iLayer] = nullptr;
        m_arpRectDivLineLabels[iLayer] = nullptr;
        m_arpStrDivLineLabels[iLayer] = nullptr;
        m_arpbDivLineLabelsVisible[iLayer] = nullptr;
    }
    for( iPart = 0; iPart < 2; iPart++ )
    {
        m_arbScaleMinMaxValVisible[iPart] = false;
    }

    // Initialization depending on the current input values of the ctor
    //------------------------------------------------------------------

    m_ararbShow[EPartDivLines][EDivLineLayerMain] = true;
    m_ararbShow[EPartDivLineLabels][EDivLineLayerMain] = true;

    if( !i_strAxisLabel.isEmpty() )
    {
        // Please note that there is no "sub" layer for the axis label.
        m_ararbShow[EPartAxisLabel][EDivLineLayerMain] = true;
        m_arbShowUnit[EPartAxisLabel] = true;
    }
    else
    {
        m_arbShowUnit[EPartDivLineLabels] = true;
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

    int iLayer;

    m_pDiagScale = nullptr;

    for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
    {
        delete [] m_arpiDivLines[iLayer];
        m_arpiDivLines[iLayer] = nullptr;
        delete [] m_arpRectDivLineLabels[iLayer];
        m_arpRectDivLineLabels[iLayer] = nullptr;
        delete [] m_arpbDivLineLabelsVisible[iLayer];
        m_arpbDivLineLabelsVisible[iLayer] = nullptr;

        try
        {
            delete [] m_arpStrDivLineLabels[iLayer];
        }
        catch(...)
        {
        }
        m_arpStrDivLineLabels[iLayer] = nullptr;
        m_ariDivLinesCount[iLayer] = 0;
    }

    delete m_arpAxisLabelsToBeConsidered;
    m_arpAxisLabelsToBeConsidered = nullptr;
    m_iAxisLabelsToBeConsideredListSize = 0;
    m_iAxisLabelsToBeConsidered = 0;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

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
void CDiagObjAxisLabel::show( EPart i_part, EDivLineLayer i_layer )
//------------------------------------------------------------------------------
{
    int iPart;
    int iPartMin = i_part;
    int iPartMax = i_part;
    int iLayer;
    int iLayerMin = i_layer;
    int iLayerMax = i_layer;

    if( i_part != EPartCount && (i_part < EPartMin || i_part > EPartMax) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    if( i_layer != EDivLineLayerCount && (i_layer < EDivLineLayerMin || i_layer > EDivLineLayerMax) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }

    if( i_part == EPartCount )
    {
        iPartMin = 0;
        iPartMax = EPartCount-1;
    }
    if( i_layer == EDivLineLayerCount )
    {
        iLayerMin = 0;
        iLayerMax = EDivLineLayerCount-1;
    }
    for( iPart = iPartMin; iPart <= iPartMax; iPart++ )
    {
        for( iLayer = iLayerMin; iLayer <= iLayerMax; iLayer++ )
        {
            if( !m_ararbShow[iPart][iLayer] )
            {
                m_ararbShow[iPart][iLayer] = true;
                invalidate(EUpdateLayoutDataPixmapWidget,true);
            }
        }
    }

} // show

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::hide( EPart i_part, EDivLineLayer i_layer )
//------------------------------------------------------------------------------
{
    int iPart;
    int iPartMin = i_part;
    int iPartMax = i_part;
    int iLayer;
    int iLayerMin = i_layer;
    int iLayerMax = i_layer;

    if( i_part != EPartCount && (i_part < EPartMin || i_part > EPartMax) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    if( i_layer != EDivLineLayerCount && (i_layer < EDivLineLayerMin || i_layer > EDivLineLayerMax) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }

    if( i_part == EPartCount )
    {
        iPartMin = 0;
        iPartMax = EPartCount-1;
    }
    if( i_layer == EDivLineLayerCount )
    {
        iLayerMin = 0;
        iLayerMax = EDivLineLayerCount-1;
    }
    for( iPart = iPartMin; iPart <= iPartMax; iPart++ )
    {
        for( iLayer = iLayerMin; iLayer <= iLayerMax; iLayer++ )
        {
            if( m_ararbShow[iPart][iLayer] )
            {
                m_ararbShow[iPart][iLayer] = false;
                invalidate(EUpdateLayoutDataPixmapWidget,true);
            }
        }
    }

} // hide

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::showUnit( EPart i_part )
//------------------------------------------------------------------------------
{
    int iPart;
    int iPartMin = i_part;
    int iPartMax = i_part;

    if( i_part != EPartCount && (i_part < EPartMin || i_part > EPartMax) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }

    if( i_part == EPartCount )
    {
        iPartMin = 0;
        iPartMax = EPartCount-1;
    }
    for( iPart = iPartMin; iPart <= iPartMax; iPart++ )
    {
        if( !m_arbShowUnit[iPart] )
        {
            m_arbShowUnit[iPart] = true;
            invalidate(EUpdateLayoutDataPixmapWidget,true);
        }
    }

} // showUnit

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::hideUnit( EPart i_part )
//------------------------------------------------------------------------------
{
    int iPart;
    int iPartMin = i_part;
    int iPartMax = i_part;

    if( i_part != EPartCount && (i_part < EPartMin || i_part > EPartMax) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }

    if( i_part == EPartCount )
    {
        iPartMin = 0;
        iPartMax = EPartCount-1;
    }
    for( iPart = iPartMin; iPart <= iPartMax; iPart++ )
    {
        if( m_arbShowUnit[iPart] )
        {
            m_arbShowUnit[iPart] = false;
            invalidate(EUpdateLayoutDataPixmapWidget,true);
        }
    }

} // hideUnit

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::setColFg( EPart i_part, const QColor& i_col )
//------------------------------------------------------------------------------
{
    int iPart;
    int iPartMin = i_part;
    int iPartMax = i_part;

    if( i_part != EPartCount && (i_part < EPartMin || i_part > EPartMax) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }

    if( i_part == EPartCount )
    {
        iPartMin = 0;
        iPartMax = EPartCount-1;
    }
    for( iPart = iPartMin; iPart <= iPartMax; iPart++ )
    {
        if( m_arcolFg[iPart] != i_col )
        {
            m_arcolFg[iPart] = i_col;
            invalidate(EUpdatePixmapWidget,true);
        }
    }

} // setCol

//------------------------------------------------------------------------------
QColor CDiagObjAxisLabel::getColFg( EPart i_part ) const
//------------------------------------------------------------------------------
{
    if( i_part < EPartMin || i_part > EPartMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    return m_arcolFg[i_part];
}

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::setPenStyle( EPart i_part, const Qt::PenStyle& i_penStyle )
//------------------------------------------------------------------------------
{
    int iPart;
    int iPartMin = i_part;
    int iPartMax = i_part;

    if( i_part != EPartCount && (i_part < EPartMin || i_part > EPartMax) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }

    if( i_part == EPartCount )
    {
        iPartMin = 0;
        iPartMax = EPartCount-1;
    }
    for( iPart = iPartMin; iPart <= iPartMax; iPart++ )
    {
        if( m_arpenStyle[iPart] != i_penStyle )
        {
            m_arpenStyle[iPart] = i_penStyle;
            invalidate(EUpdatePixmapWidget,true);
        }
    }

} // setPenStyle

//------------------------------------------------------------------------------
Qt::PenStyle CDiagObjAxisLabel::getPenStyle( EPart i_part ) const
//------------------------------------------------------------------------------
{
    if( i_part < EPartMin || i_part > EPartMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    return m_arpenStyle[i_part];
}

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::setColBg( EPart i_part, const QColor& i_col )
//------------------------------------------------------------------------------
{
    int iPart;
    int iPartMin = i_part;
    int iPartMax = i_part;

    if( i_part != EPartCount && (i_part < EPartMin || i_part > EPartMax) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    if( i_part == EPartCount )
    {
        iPartMin = 0;
        iPartMax = EPartCount-1;
    }
    for( iPart = iPartMin; iPart <= iPartMax; iPart++ )
    {
        if( m_arcolBg[iPart] != i_col )
        {
            m_arcolBg[iPart] = i_col;
            invalidate(EUpdatePixmapWidget,true);
        }
    }

} // setColBg

//------------------------------------------------------------------------------
QColor CDiagObjAxisLabel::getColBg( EPart i_part ) const
//------------------------------------------------------------------------------
{
    if( i_part < EPartMin || i_part > EPartMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    return m_arcolBg[i_part];
}

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::setBrushStyle( EPart i_part, const Qt::BrushStyle& i_brushStyle )
//------------------------------------------------------------------------------
{
    int iPart;
    int iPartMin = i_part;
    int iPartMax = i_part;

    if( i_part != EPartCount && (i_part < EPartMin || i_part > EPartMax) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    if( i_part == EPartCount )
    {
        iPartMin = 0;
        iPartMax = EPartCount-1;
    }
    for( iPart = iPartMin; iPart <= iPartMax; iPart++ )
    {
        if( m_arbrushStyle[iPart] != i_brushStyle )
        {
            m_arbrushStyle[iPart] = i_brushStyle;
            invalidate(EUpdatePixmapWidget,true);
        }
    }

} // setBrushStyle

//------------------------------------------------------------------------------
Qt::BrushStyle CDiagObjAxisLabel::getBrushStyle( EPart i_part ) const
//------------------------------------------------------------------------------
{
    if( i_part < EPartMin || i_part > EPartMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    return m_arbrushStyle[i_part];
}

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::setFont( EPart i_part, const QFont& i_fnt )
//------------------------------------------------------------------------------
{
    int iPart;
    int iPartMin = i_part;
    int iPartMax = i_part;

    if( i_part != EPartCount && (i_part < EPartMin || i_part > EPartMax) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    if( i_part == EPartCount )
    {
        iPartMin = 0;
        iPartMax = EPartCount-1;
    }
    for( iPart = iPartMin; iPart <= iPartMax; iPart++ )
    {
        m_arfnt[iPart] = i_fnt;
        invalidate(EUpdateLayoutDataPixmapWidget,true);
    }

} // setFont

//------------------------------------------------------------------------------
QFont CDiagObjAxisLabel::getFont( EPart i_part ) const
//------------------------------------------------------------------------------
{
    if( i_part < EPartMin || i_part > EPartMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    return m_arfnt[i_part];
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

// Space between the axis label and the diagram border
//==============================================================================

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::setSpaceDiagBorder2AxisLabel( int i_iSpace )
//------------------------------------------------------------------------------
{
    m_iSpaceDiagBorder2AxisLabel = i_iSpace;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
int CDiagObjAxisLabel::getSpaceDiagBorder2AxisLabel() const
//------------------------------------------------------------------------------
{
    return m_iSpaceDiagBorder2AxisLabel;
}

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::addAxisLabelToBeConsidered( CDiagObjAxisLabel* i_pDiagObjAxisLabel )
//------------------------------------------------------------------------------
{
    int idxAxisLabel;

    if( m_arpAxisLabelsToBeConsidered == nullptr )
    {
        m_iAxisLabelsToBeConsideredListSize = 2;
        m_iAxisLabelsToBeConsidered = 0;
        m_arpAxisLabelsToBeConsidered = new CDiagObjAxisLabel*[m_iAxisLabelsToBeConsideredListSize];

        for( idxAxisLabel = 0; idxAxisLabel < m_iAxisLabelsToBeConsideredListSize; idxAxisLabel++ )
        {
            m_arpAxisLabelsToBeConsidered[idxAxisLabel] = nullptr;
        }
    }
    else if( m_iAxisLabelsToBeConsidered == m_iAxisLabelsToBeConsideredListSize )
    {
        CDiagObjAxisLabel** arpAxisLabelsToBeConsidered = m_arpAxisLabelsToBeConsidered;
        int                 iAxisLabelsToBeConsideredListSize = m_iAxisLabelsToBeConsideredListSize;

        m_iAxisLabelsToBeConsideredListSize += 2;
        m_arpAxisLabelsToBeConsidered = new CDiagObjAxisLabel*[m_iAxisLabelsToBeConsideredListSize];

        for( idxAxisLabel = 0; idxAxisLabel < iAxisLabelsToBeConsideredListSize; idxAxisLabel++ )
        {
            m_arpAxisLabelsToBeConsidered[idxAxisLabel] = arpAxisLabelsToBeConsidered[idxAxisLabel];
        }
        for( ; idxAxisLabel < m_iAxisLabelsToBeConsideredListSize; idxAxisLabel++ )
        {
            m_arpAxisLabelsToBeConsidered[idxAxisLabel] = nullptr;
        }
        delete arpAxisLabelsToBeConsidered;
        arpAxisLabelsToBeConsidered = nullptr;
    }
    m_arpAxisLabelsToBeConsidered[m_iAxisLabelsToBeConsidered] = i_pDiagObjAxisLabel;
    m_iAxisLabelsToBeConsidered++;

} // addAxisLabelToBeConsidered

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::removeAxisLabelToBeConsidered( const CDiagObjAxisLabel* i_pDiagObjAxisLabel )
//------------------------------------------------------------------------------
{
    if( m_arpAxisLabelsToBeConsidered == nullptr )
    {
        return;
    }

    int idxAxisLabel;

    for( idxAxisLabel = 0; idxAxisLabel < m_iAxisLabelsToBeConsidered; idxAxisLabel++ )
    {
        if( m_arpAxisLabelsToBeConsidered[idxAxisLabel] == i_pDiagObjAxisLabel )
        {
            break;
        }
    }
    if( idxAxisLabel < m_iAxisLabelsToBeConsidered )
    {
        for( ; idxAxisLabel < m_iAxisLabelsToBeConsidered-1; idxAxisLabel++ )
        {
            m_arpAxisLabelsToBeConsidered[idxAxisLabel] = m_arpAxisLabelsToBeConsidered[idxAxisLabel+1];
        }
        m_iAxisLabelsToBeConsidered--;
    }

} // removeAxisLabelToBeConsidered

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

    if( m_pDataDiagram == nullptr )
    {
        return QSize(0,0);
    }

    if( isVisible() )
    {
        const CPixmapDiagram* pPixmapDiagram = nullptr;

        // As a matter of fact there is no sense in adding an axis label object to
        // a diagram just designed to analyze data.
        if( m_pDataDiagram->getUpdateType() >= EDiagramUpdateTypePixmap )
        {
            pPixmapDiagram = dynamic_cast<const CPixmapDiagram*>(m_pDataDiagram);
        }
        if( pPixmapDiagram != nullptr )
        {
            if( m_pDiagScale->getScaleDir() == EScaleDirX )
            {
                cxWidth  = pPixmapDiagram->getWidth();
                cyHeight = 0;

                if( m_ararbShow[EPartDivLineLabels][EDivLineLayerMain] )
                {
                    if( m_iSpaceDiagPartCenter2DivLineLabels >= 0 )
                    {
                        cyHeight += m_rectDivLineLabelsMaxTextExtent.height() + m_iSpaceDiagPartCenter2DivLineLabels + c_iMaxTextExtentAddHeight;
                    }
                }
                if( m_ararbShow[EPartDivLineLabels][EDivLineLayerMain] && ( m_ararbShow[EPartAxisLabel][EDivLineLayerMain] || m_arbShowUnit[EPartAxisLabel] ) )
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
                if( m_ararbShow[EPartAxisLabel][EDivLineLayerMain] || m_arbShowUnit[EPartAxisLabel] )
                {
                    cyHeight += (m_rectAxisLabel.height() + c_iMaxTextExtentAddHeight + m_iSpaceDiagBorder2AxisLabel);
                }
            }
            else if( m_pDiagScale->getScaleDir() == EScaleDirY )
            {
                cyHeight = pPixmapDiagram->getHeight();
                cxWidth  = 0;

                if( m_ararbShow[EPartDivLineLabels][EDivLineLayerMain] )
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
                        cxWidth += (m_iSpaceDiagPartCenter2DivLineLabels + c_iMaxTextExtentAddWidth);
                    }
                }
                if( m_ararbShow[EPartDivLineLabels][EDivLineLayerMain] && ( m_ararbShow[EPartAxisLabel][EDivLineLayerMain] || m_arbShowUnit[EPartAxisLabel] ) )
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
                if( m_ararbShow[EPartAxisLabel][EDivLineLayerMain] || m_arbShowUnit[EPartAxisLabel] )
                {
                    cxWidth += (m_rectAxisLabel.width() + m_iSpaceDiagBorder2AxisLabel + c_iMaxTextExtentAddWidth);
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

    CDiagScale* pDiagScale = i_pDiagramTrg->getDiagScale( m_pDiagScale->getObjName() );

    CDiagObjAxisLabel* pDiagObj = new CDiagObjAxisLabel(
        /* strObjName   */ m_strObjName,
        /* pDiagScale   */ pDiagScale,
        /* layoutPos    */ m_layoutPos,
        /* strAxisLabel */ m_strAxisLabel );

    int idxPart;
    int idxDivLine;
    int idxAxisLabel;
    int iLayer;

    // Members from base class CDiagObj:
    pDiagObj->m_rectContent = m_rectContent;
    pDiagObj->m_bAdjustContentRect2DiagPartCenter = m_bAdjustContentRect2DiagPartCenter;
    pDiagObj->m_bVisible = m_bVisible;
    pDiagObj->m_state = m_state;
    pDiagObj->m_bIsFocusable = m_bIsFocusable;
    pDiagObj->m_bIsEditable = m_bIsEditable;

    // Members from this class:
    pDiagObj->m_pUnit = m_pUnit;
    pDiagObj->m_unitLabels = m_unitLabels;
    pDiagObj->m_strPhysUnitLabels = m_strPhysUnitLabels;
    pDiagObj->m_iSpaceDiagPartCenter2DivLineLabels = m_iSpaceDiagPartCenter2DivLineLabels;
    pDiagObj->m_iSpaceDivLineLabels2AxisLabel = m_iSpaceDivLineLabels2AxisLabel;
    pDiagObj->m_iSpaceDiagBorder2AxisLabel = m_iSpaceDiagBorder2AxisLabel;

    for( idxPart = 0; idxPart < EPartCount; idxPart++ )
    {
        for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
        {
            pDiagObj->m_ararbShow[idxPart][iLayer] = m_ararbShow[idxPart][iLayer];
        }
        pDiagObj->m_arbShowUnit[idxPart] = m_arbShowUnit[idxPart];
        pDiagObj->m_arcolFg[idxPart] = m_arcolFg[idxPart];
        pDiagObj->m_arpenStyle[idxPart] = m_arpenStyle[idxPart];
        pDiagObj->m_arcolBg[idxPart] = m_arcolBg[idxPart];
        pDiagObj->m_arbrushStyle[idxPart] = m_arbrushStyle[idxPart];
        pDiagObj->m_arfnt[idxPart] = m_arfnt[idxPart];
    }
    pDiagObj->m_iDivLineLabelsDigitsCountMax = m_iDivLineLabelsDigitsCountMax;
    pDiagObj->m_bUseEngineeringFormat = m_bUseEngineeringFormat;
    pDiagObj->m_strAxisLabel = m_strAxisLabel;
    pDiagObj->m_rectAxisLabel = m_rectAxisLabel;
    pDiagObj->m_iDivLineLabelsTrailingDigits = m_iDivLineLabelsTrailingDigits;
    pDiagObj->m_iDivLineLabelsExponentDigits = m_iDivLineLabelsExponentDigits;
    pDiagObj->m_rectDivLineLabelsMaxTextExtent = m_rectDivLineLabelsMaxTextExtent;
    pDiagObj->m_cxDivLineLabelsSpace = m_cxDivLineLabelsSpace;

    for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
    {
        pDiagObj->m_ariDivLinesCount[iLayer] = m_ariDivLinesCount[iLayer];

        if( pDiagObj->m_ariDivLinesCount[iLayer] > 0
         && m_arpiDivLines[iLayer] != nullptr
         && m_arpRectDivLineLabels[iLayer]
         && m_arpStrDivLineLabels[iLayer] )
        {
            pDiagObj->m_arpiDivLines[iLayer] = new int[pDiagObj->m_ariDivLinesCount[iLayer]];
            pDiagObj->m_arpRectDivLineLabels[iLayer] = new QRect[pDiagObj->m_ariDivLinesCount[iLayer]];
            pDiagObj->m_arpStrDivLineLabels[iLayer] = new QString[pDiagObj->m_ariDivLinesCount[iLayer]];
            pDiagObj->m_arpbDivLineLabelsVisible[iLayer] = new bool[pDiagObj->m_ariDivLinesCount[iLayer]];

            for( idxDivLine = 0; idxDivLine < pDiagObj->m_ariDivLinesCount[iLayer]; idxDivLine++ )
            {
                pDiagObj->m_arpiDivLines[iLayer][idxDivLine] = m_arpiDivLines[iLayer][idxDivLine];
                pDiagObj->m_arpRectDivLineLabels[iLayer][idxDivLine] = m_arpRectDivLineLabels[iLayer][idxDivLine];
                pDiagObj->m_arpStrDivLineLabels[iLayer][idxDivLine] = m_arpStrDivLineLabels[iLayer][idxDivLine];
                pDiagObj->m_arpbDivLineLabelsVisible[iLayer][idxDivLine] = m_arpbDivLineLabelsVisible[iLayer][idxDivLine];
            }
        }
    }

    pDiagObj->m_rectDivLineLabelsPhysUnit = m_rectDivLineLabelsPhysUnit;
    pDiagObj->m_bDivLineLabelsPhysUnitVisible = m_bDivLineLabelsPhysUnitVisible;

    for( idxDivLine = 0; idxDivLine < 2; idxDivLine++ )
    {
        pDiagObj->m_arstrScaleMinMaxVal[idxDivLine] = m_arstrScaleMinMaxVal[idxDivLine];
        pDiagObj->m_arrectScaleMinMaxVal[idxDivLine] = m_arrectScaleMinMaxVal[idxDivLine];
        pDiagObj->m_arbScaleMinMaxValVisible[idxDivLine] = m_arbScaleMinMaxValVisible[idxDivLine];
    }

    pDiagObj->m_iAxisLabelsToBeConsideredListSize = m_iAxisLabelsToBeConsideredListSize;
    pDiagObj->m_iAxisLabelsToBeConsidered = m_iAxisLabelsToBeConsidered;

    if( pDiagObj->m_iAxisLabelsToBeConsideredListSize > 0 && m_arpAxisLabelsToBeConsidered != nullptr )
    {
        pDiagObj->m_arpAxisLabelsToBeConsidered = new CDiagObjAxisLabel*[pDiagObj->m_iAxisLabelsToBeConsideredListSize];

        for( idxAxisLabel = 0; idxAxisLabel < pDiagObj->m_iAxisLabelsToBeConsideredListSize; idxAxisLabel++ )
        {
            if( m_arpAxisLabelsToBeConsidered[idxAxisLabel] != nullptr )
            {
                pDiagObj->m_arpAxisLabelsToBeConsidered[idxAxisLabel] = dynamic_cast<CDiagObjAxisLabel*>(i_pDiagramTrg->getDiagObj(m_arpAxisLabelsToBeConsidered[idxAxisLabel]->getObjId()));
            }
            else
            {
                pDiagObj->m_arpAxisLabelsToBeConsidered[idxAxisLabel] = nullptr;
            }
        }
    }

    i_pDiagramTrg->addDiagObj(pDiagObj);

    return pDiagObj;

} // clone

//------------------------------------------------------------------------------
void CDiagObjAxisLabel::update( unsigned int i_uUpdateFlags, QPaintDevice* i_pPaintDevice )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObjUpdate != nullptr && m_pTrcAdminObjUpdate->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strTrcMsg = updateFlags2Str(i_uUpdateFlags);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "update",
        /* strAddInfo   */ strTrcMsg );

    if( i_uUpdateFlags == EUpdateNone || m_pDataDiagram == nullptr || m_pDiagScale == nullptr || !isVisible() )
    {
        return;
    }
    // As a matter of fact there is no sense in adding an axis label object to
    // a diagram just designed to analyze data.
    if( m_pDataDiagram->getUpdateType() < EDiagramUpdateTypePixmap )
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
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateLayout",
        /* strAddInfo   */ "" );

    if( m_pDataDiagram == nullptr || m_pDiagScale == nullptr || !isVisible() )
    {
        return;
    }

    // Please note that the axis label rectangle need to be calculated before
    // calculating the rectangle for the division lines as the position for
    // the division lines may depend on the size of the axis label rectangle.

    QRect rectDiag(0,0,1,1);

    // As a matter of fact there is no sense in adding an axis label object to
    // a diagram just designed to analyze data.
    if( m_pDataDiagram->getUpdateType() >= EDiagramUpdateTypePixmap )
    {
        const CPixmapDiagram* pPixmapDiagram = dynamic_cast<const CPixmapDiagram*>(m_pDataDiagram);
        if( pPixmapDiagram == nullptr )
        {
            return;
        }
        rectDiag.setLeft( pPixmapDiagram->getMarginLeft() );
        rectDiag.setWidth( pPixmapDiagram->getWidth()-pPixmapDiagram->getMarginLeft()-pPixmapDiagram->getMarginRight() );
        rectDiag.setTop( pPixmapDiagram->getMarginTop() );
        rectDiag.setHeight( pPixmapDiagram->getHeight()-pPixmapDiagram->getMarginTop()-pPixmapDiagram->getMarginBottom() );
    }

    // Calculate width and height of the axis label
    //==========================================================================

    QRect rectAxisLabel;

    if( m_ararbShow[EPartAxisLabel][EDivLineLayerMain] || m_arbShowUnit[EPartAxisLabel] )
    {
        // Calculate extent of axis label:
        QFontMetrics fntmtr(m_arfnt[EPartAxisLabel]);
        QString      strAxisLabel = m_strAxisLabel;

        if( m_arbShowUnit[EPartAxisLabel] )
        {
            if( !strAxisLabel.isEmpty() )
            {
                strAxisLabel += " / ";
            }
            strAxisLabel += m_strPhysUnitLabels;
        }
        rectAxisLabel = fntmtr.boundingRect(strAxisLabel);
        rectAxisLabel.setHeight( rectAxisLabel.height() + c_iMaxTextExtentAddHeight );
        rectAxisLabel.setWidth( rectAxisLabel.width() + c_iMaxTextExtentAddWidth );

        int cxWidth  = rectAxisLabel.width();
        int cyHeight = rectAxisLabel.height();

        // Please note that we need to access the diagrams geometry as the
        // "calculateAxisLabelRect" method might have been invoked to get the
        // sizeHint for the content rectangle and the content rectangle will
        // be changed by the diagram later on.
        switch( m_layoutPos )
        {
            case ELayoutPosLeft:
            case ELayoutPosRight:
            {
                // The label will be drawn rotated by 90 degrees.
                // The width becomes the height.
                rectAxisLabel.setHeight( cxWidth );
                rectAxisLabel.setWidth( cyHeight );
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

    // Please note that after layout processing only the size of the
    // axis label rectangle is up to date which is necessary for the
    // following "sizeHint" call. The label will be moved to its final
    // position during data processing.
    m_rectAxisLabel = rectAxisLabel;

    // Calculate width and height of the area for the division line labels
    //==========================================================================

    int iLayer;

    // The X and Y scales are calculating the number of division lines and
    // the distance between the division lines for the grid.
    // The labels are linked to those calculated division lines.

    // To discover overlapping division line labels the rectangle extents of the
    // division line labels are stored in an array:
    for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
    {
        delete [] m_arpiDivLines[iLayer];
        m_arpiDivLines[iLayer] = nullptr;
        delete [] m_arpRectDivLineLabels[iLayer];
        m_arpRectDivLineLabels[iLayer] = nullptr;
        delete [] m_arpStrDivLineLabels[iLayer];
        m_arpStrDivLineLabels[iLayer] = nullptr;
        delete [] m_arpbDivLineLabelsVisible[iLayer];
        m_arpbDivLineLabelsVisible[iLayer] = nullptr;
        m_ariDivLinesCount[iLayer] = 0;
    }

    double        fScaleMinVal;
    double        fScaleMaxVal;
    double        fScaleRes;
    double        fDivLineValAbsMin;
    double        fDivLineValAbsMax;
    double        fDivLineVal;
    int           idxDivLine;
    int           iDivLinesCount = 0;
    int           iDivLineLabelsLeadingDigits = 1;
    int           iPrecisionMin = 1;
    int           iPrecisionMinPrev = 0;
    double        fDivLineVal1;
    double        fDivLineVal2;
    double        fDivLineVal3;
    double        fDivLineDist1;
    double        fDivLineDist2;
    int           idx;
    QFontMetrics  fntmtr(m_arfnt[EPartDivLineLabels]);
    QString       strDivLineLabel;
    QString       strDivLineLabel1;
    QString       strDivLineLabel2;
    QString       strDivLineLabel3;
    QRect         rectDivLineLabel;
    QRect         rectDivLineLabelTextExtent;
    TFormatResult formatResult;
    int           xDivLine;
    int           yDivLine;
    int           cxOffset;
    int           cyOffset;
    double        fTmp;
    int           iTmp;

    for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
    {
         m_ariDivLinesCount[iLayer] = 0;
    }

    // Calculate string to indicate the physical unit at the maximum scale value
    //--------------------------------------------------------------------------

    m_iDivLineLabelsTrailingDigits = 1;
    m_iDivLineLabelsExponentDigits = 0;

    // If the unit, in which the division lines should be labeled, has been explicitly specified ...
    if( m_pUnit != nullptr )
    {
        m_unitLabels = *m_pUnit;
    }
    // If the "best" unit to indicate the values should be used ...
    else
    {
        CUnit  unitScaleMinValTmp;
        CUnit  unitScaleMaxValTmp;
        double fScaleMinValTmp;
        double fScaleMaxValTmp;
        int    iDigitsLeadingScaleMinVal;
        int    iDigitsTrailingScaleMinVal;
        int    iDigitsLeadingScaleMaxVal;
        int    iDigitsTrailingScaleMaxVal;

        m_unitLabels = m_pDiagScale->getScale().m_unit;
        fScaleMinVal = m_pDiagScale->getScale().m_fMin;
        fScaleMaxVal = m_pDiagScale->getScale().m_fMax;

        formatResult = PhysVal::formatValue(
            /* fValue                */ fScaleMinVal,
            /* unitVal               */ m_pDiagScale->getScale().m_unit,
            /* iDigitsMantissaMax    */ 0,
            /* bAccuracyLimitsMant.  */ false,
            /* iDigitsAccuracy       */ 0,
            /* iDigitsExponent       */ 0,
            /* bUseEngineeringFormat */ false,
            /* pfVal                 */ &fScaleMinValTmp,
            /* pstr                  */ nullptr,
            /* ppUnit                */ &unitScaleMinValTmp,
            /* piDigitsLeading       */ &iDigitsLeadingScaleMinVal,
            /* piDigitsTrailing      */ &iDigitsTrailingScaleMinVal );

        if( !(formatResult & FormatResult::Error) ) //lint !e655
        {
            formatResult = PhysVal::formatValue(
                /* fValue                */ fScaleMaxVal,
                /* unitVal               */ m_pDiagScale->getScale().m_unit,
                /* iDigitsMantissaMax    */ 0,
                /* bAccuracyLimitsMant.  */ false,
                /* iDigitsAccuracy       */ 0,
                /* iDigitsExponent       */ 0,
                /* bUseEngineeringFormat */ false,
                /* pfVal                 */ &fScaleMaxValTmp,
                /* pstr                  */ nullptr,
                /* ppUnit                */ &unitScaleMaxValTmp,
                /* piDigitsLeading       */ &iDigitsLeadingScaleMaxVal,
                /* piDigitsTrailing      */ &iDigitsTrailingScaleMaxVal );

            if( !(formatResult & FormatResult::Error) ) //lint !e655
            {
                // If the best unit for the scale minimum and scale maximum value is the same ...
                if( unitScaleMinValTmp == unitScaleMaxValTmp )
                {
                    m_unitLabels = unitScaleMinValTmp;
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
                        m_unitLabels = unitScaleMaxValTmp;
                    }
                    // If the maximum scale value was 0.0 ...
                    else if( fabs(fScaleMaxVal) == 0.0 )
                    {
                        // .. we take the best unit for the scale minimum value.
                        m_unitLabels = unitScaleMinValTmp;
                    }
                    // If the absolute value of both values were greater or equal than 1.0 ...
                    else if( fabs(fScaleMinVal) >= 1.0 && fabs(fScaleMaxVal) >= 1.0 )
                    {
                        // .. we take the best unit for the absolute greater value.
                        if( fabs(fScaleMinVal) < fabs(fScaleMaxVal) )
                        {
                            m_unitLabels = unitScaleMaxValTmp;
                        }
                        else
                        {
                            m_unitLabels = unitScaleMinValTmp;
                        }
                    }
                    // If the absolute value of both values were less or equal than 1.0 ...
                    else if( fabs(fScaleMinVal) <= 1.0 && fabs(fScaleMaxVal) <= 1.0 )
                    {
                        // .. we take the best unit for the absolute lower value.
                        if( fabs(fScaleMinVal) < fabs(fScaleMaxVal) )
                        {
                            m_unitLabels = unitScaleMinValTmp;
                        }
                        else
                        {
                            m_unitLabels = unitScaleMaxValTmp;
                        }
                    }
                    // If the absolute value of the scale max value is greater or equal than 1.0 ...
                    else if( fabs(fScaleMaxVal) >= 1.0 )
                    {
                        // .. we take the best unit of the scale max value.
                        m_unitLabels = unitScaleMaxValTmp;
                    }
                    // If the absolute value of the scale max value is less than 1.0 ...
                    else // if( fabs(fScaleMaxVal) < 1.0 )
                    {
                        // .. we take the best unit of the scale min value.
                        m_unitLabels = unitScaleMinValTmp;
                    }
                }
            }
        }
    }

    if( m_unitLabels.isValid() )
    {
        m_strPhysUnitLabels = m_unitLabels.symbol();
    }
    else
    {
        m_strPhysUnitLabels = "";
    }

    // Calculate extent of division line labels:
    //-------------------------------------------

    fScaleMinVal = m_pDiagScale->getScale().m_unit.convertValue(
        m_pDiagScale->getScale().m_fMin, m_unitLabels);
    fScaleMaxVal = m_pDiagScale->getScale().m_unit.convertValue(
        m_pDiagScale->getScale().m_fMax, m_unitLabels);

    for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
    {
        m_ariDivLinesCount[iLayer] = m_pDiagScale->getDivLineCount(iLayer);
        iDivLinesCount += m_ariDivLinesCount[iLayer];
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "MainDivLineLabelsCount=";
        strTrcMsg += QString::number(m_ariDivLinesCount[EDivLineLayerMain]) + ",";
        strTrcMsg += "SubDivLineLabelsCount=";
        strTrcMsg += QString::number(m_ariDivLinesCount[EDivLineLayerSub]);
        mthTracer.trace(strTrcMsg);
    }

    // Get absolute minimum and maximum values of division line labels ...
    fDivLineValAbsMin = fabs(fScaleMinVal);
    fDivLineValAbsMax = fabs(fScaleMaxVal);
    if( fDivLineValAbsMin > fDivLineValAbsMax )
    {
        fTmp = fDivLineValAbsMin;
        fDivLineValAbsMin = fDivLineValAbsMax;
        fDivLineValAbsMax = fTmp;
    }
    if( iDivLinesCount >= 2 )
    {
        fDivLineValAbsMin = DBL_MAX;
        fDivLineValAbsMax = DBL_EPSILON;

        for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
        {
            for( idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]; idxDivLine++ )
            {
                fTmp = fabs(m_pDiagScale->getDivLineVal(iLayer,idxDivLine,&m_unitLabels));

                if( fTmp > fDivLineValAbsMax )
                {
                    fDivLineValAbsMax = fTmp;
                }
                if( fTmp < fDivLineValAbsMin && fTmp > 0.0 )
                {
                    fDivLineValAbsMin = fTmp;
                }
            }
        }
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "DivLineValAbsMin=";
        strTrcMsg += QString::number(fDivLineValAbsMin) + ",";
        strTrcMsg += "DivLineValAbsMax=";
        strTrcMsg += QString::number(fDivLineValAbsMax);
        mthTracer.trace(strTrcMsg);
    }

    // Calculate how many leading and trailing digits would be necessary to
    // indicate the absolute minimum and maximum values and decide, whether
    // an exponent is necessary to indicate the values ...
    if( fDivLineValAbsMax >= 10.0 )
    {
        double fLogInt;
        double fLog = log10(fDivLineValAbsMax);
        double fLogFrac = modf(fLog,&fLogInt);
        iDivLineLabelsLeadingDigits = static_cast<int>(fLog);
        if( fLogFrac > 0.0 )
        {
            iDivLineLabelsLeadingDigits += 1;
        }
    }
    if( fDivLineValAbsMin < 1.0 && fDivLineValAbsMin > 0.0 )
    {
        double fLogInt;
        double fLog = log10(fDivLineValAbsMin);
        double fLogFrac = modf(fLog,&fLogInt);
        m_iDivLineLabelsTrailingDigits = -static_cast<int>(fLog);
        if( fLogFrac > 0.0 )
        {
            m_iDivLineLabelsTrailingDigits += 1;
        }
    }
    if( m_iDivLineLabelsDigitsCountMax > 0 || m_bUseEngineeringFormat )
    {
        if( iDivLineLabelsLeadingDigits + m_iDivLineLabelsTrailingDigits > m_iDivLineLabelsDigitsCountMax
         || ( m_bUseEngineeringFormat && (iDivLineLabelsLeadingDigits > 1) ) )
        {
            m_iDivLineLabelsExponentDigits = static_cast<int>(log10(static_cast<double>(iDivLineLabelsLeadingDigits)))+1;
            iTmp = static_cast<int>(log10(static_cast<double>(m_iDivLineLabelsTrailingDigits)))+1;
            if( iTmp > m_iDivLineLabelsExponentDigits )
            {
                m_iDivLineLabelsExponentDigits = iTmp;
            }
            iDivLineLabelsLeadingDigits = 1;
            m_iDivLineLabelsTrailingDigits = 1;
        }
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "Digits needed for DivLineValAbsMin and DivLineValAbsMax: ";
        strTrcMsg += "LeadingDigits=" + QString::number(iDivLineLabelsLeadingDigits) + ",";
        strTrcMsg += "TrailingDigits=" + QString::number(m_iDivLineLabelsTrailingDigits) + ",";
        strTrcMsg += "ExponentDigits=" + QString::number(m_iDivLineLabelsExponentDigits);
        mthTracer.trace(strTrcMsg);
    }

    // If just one or even no division line label is available ..
    if( iDivLinesCount < 2 && m_pDiagScale->isScaleValid() && fScaleMaxVal > fScaleMinVal )
    {
        // If less than one division line is visible we try to indicate the minimum and
        // maximum scale values. This is a special case for formatting the values as the
        // minimum and maximum values might be any not rounded value (in contrary to the
        // division lines which are always rounded to a whole number of a decimal power).
        // To output the number of digits for the minimum and maximum scale values the
        // scale resolution is important. If the user enters the value 3.625362 it
        // would not be good to limit the output to less than the entered digits. But we
        // also don't want to indicate trailing zeros if the resolution exceeds the number
        // of entered digits.

        int iDivLineLabelsCountTmp;
        int iPrecisionTmp;
        int iTrailingDigitsTmp = 1;
        int iLeadingDigitsTmp = 1;
        int iTrailingZeroes = 0;
        int idxChar;
        int iLabel1ExponentDigits = 0;
        int iLabel2ExponentDigits = 0;
        int iLabel3ExponentDigits = 0;
        int iLabel1TrailingZeroesCount = 0;
        int iLabel2TrailingZeroesCount = 0;
        int iLabel3TrailingZeroesCount = 0;

        // To take logarithmic scalings into account we get the resolution at the absolute
        // minimum value that should be indicated by the axis label.
        fScaleRes = m_pDiagScale->getScaleRes(fDivLineValAbsMin, &m_unitLabels);

        // Calculate number of trailing and leading digits needed to indicate
        // the values with the calculated scale resolution.
        iPrecisionTmp = static_cast<int>(log10(fabs(fScaleRes)));
        if( iPrecisionTmp < 0 )
        {
            iTrailingDigitsTmp = -iPrecisionTmp;
        }
        else if( iPrecisionTmp > 0 )
        {
            iLeadingDigitsTmp = iPrecisionTmp;
        }

        // Calculate number of trailing and leading digits needed to indicate
        // the absolute maximum value that should be indicated by the axis label
        // and increase the number of trailing and leading digits if they exceed
        // the number of digits necessary to indicate the values according to
        // the scale resolution.
        iPrecisionTmp = static_cast<int>(log10(fabs(fDivLineValAbsMax)));
        if( iPrecisionTmp < 0 )
        {
            // If more trailing digits are needed than calculated for the scale resolution ...
            if( -iPrecisionTmp > iTrailingDigitsTmp )
            {
                iTrailingDigitsTmp = -iPrecisionTmp;
            }
        }
        else if( iPrecisionTmp > 0 )
        {
            // If more leading digits are needed than calculated for the scale resolution ...
            if( iPrecisionTmp > iLeadingDigitsTmp )
            {
                iLeadingDigitsTmp = iPrecisionTmp;
            }
        }

        // Calculate number of trailing and leading digits needed to indicate
        // the absolute minimum value that should be indicated by the axis label
        // and increase the number of trailing and leading digits if they exceed
        // the number of digits necessary to indicate the values according to
        // the scale resolution or the absolute maximum value.
        iPrecisionTmp = static_cast<int>(log10(fabs(fDivLineValAbsMin)));
        if( iPrecisionTmp < 0 )
        {
            // If more trailing digits are needed than calculated for the scale resolution
            // or the absolute maximum value ...
            if( -iPrecisionTmp > iTrailingDigitsTmp )
            {
                iTrailingDigitsTmp = -iPrecisionTmp;
            }
        }
        else if( iPrecisionTmp > 0 )
        {
            // If more leading digits are needed than calculated for the scale resolution
            // or the absolute maximum value ...
            if( iPrecisionTmp > iLeadingDigitsTmp )
            {
                iLeadingDigitsTmp = iPrecisionTmp;
            }
        }

        // Minimum number of digits needed to indicate the values according to the
        // scale resolution, the absolute maximum and absolute minimum value.
        iPrecisionMin = iLeadingDigitsTmp + iTrailingDigitsTmp;

        // Now get the values that should be indicated by the axis label.
        // If no division line is available the minimum and maximum scale values will be indicated.
        iDivLineLabelsCountTmp = 2;
        fDivLineVal1 = fScaleMinVal;
        fDivLineVal2 = fScaleMaxVal;

        for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
        {
            // If the division line is available ..
            if( m_ariDivLinesCount[iLayer] == 1 )
            {
                // .. and if the division line is not at the scale minimum or scale maximum value ..
                fDivLineVal = m_pDiagScale->getDivLineVal(iLayer,0,&m_unitLabels);
                if( fDivLineVal > fScaleMinVal && fDivLineVal < fScaleMaxVal )
                {
                    // ... also the value of the main division line may be indicated.
                    fDivLineVal3 = fDivLineVal;
                    iDivLineLabelsCountTmp = 3;
                }
            }
        }

        // Now format the values with the precision calculated according to the
        // resolution and absolute maximum and minimum values and see if we can
        // remove trailing zeros:
        if( m_iDivLineLabelsExponentDigits > 0 )
        {
            strDivLineLabel1 = QString::number(
                /* fVal       */ fDivLineVal1,
                /* chFormat   */ 'e',
                /* iPrecision */ iPrecisionMin );
            #if QT_VERSION >= 0x040100
            idxChar = strDivLineLabel1.indexOf('e',Qt::CaseInsensitive );
            #else
            idxChar = strDivLineLabel1.find('e',false);
            #endif
            if( idxChar >= 0 )
            {
                iLabel1ExponentDigits = strDivLineLabel1.length()-idxChar;
            }
            strDivLineLabel2 = QString::number(
                /* fVal       */ fDivLineVal2,
                /* chFormat   */ 'e',
                /* iPrecision */ iPrecisionMin );
            #if QT_VERSION >= 0x040100
            idxChar = strDivLineLabel2.indexOf('e',Qt::CaseInsensitive );
            #else
            idxChar = strDivLineLabel2.find('e',false);
            #endif
            if( idxChar >= 0 )
            {
                iLabel2ExponentDigits = strDivLineLabel2.length()-idxChar;
            }
            if( iDivLineLabelsCountTmp == 3 )
            {
                strDivLineLabel3 = QString::number(
                    /* fVal       */ fDivLineVal3,
                    /* chFormat   */ 'e',
                    /* iPrecision */ iPrecisionMin ); //lint !e644 .. if fDivLineVal3 would not have been initialized uDivLineCountTmp would not be equal to 3
                #if QT_VERSION >= 0x040100
                idxChar = strDivLineLabel3.indexOf('e',Qt::CaseInsensitive );
                #else
                idxChar = strDivLineLabel3.find('e',false);
                #endif
                if( idxChar >= 0 )
                {
                    iLabel3ExponentDigits = strDivLineLabel3.length()-idxChar;
                }
            }
        }
        else
        {
            strDivLineLabel1 = QString::number(
                /* fVal       */ fDivLineVal1,
                /* chFormat   */ 'f',
                /* iPrecision */ iPrecisionMin );
            strDivLineLabel2 = QString::number(
                /* fVal       */ fDivLineVal2,
                /* chFormat   */ 'f',
                /* iPrecision */ iPrecisionMin );
            if( iDivLineLabelsCountTmp == 3 )
            {
                strDivLineLabel3 = QString::number(
                    /* fVal       */ fDivLineVal3,
                    /* chFormat   */ 'f',
                    /* iPrecision */ iPrecisionMin );
            }
        }
        if( strDivLineLabel3.length() == 0 )
        {
            iLabel3TrailingZeroesCount = iPrecisionMin;
        }
        if( strDivLineLabel1.length() > 0 )
        {
            for( idxChar = strDivLineLabel1.length()-iLabel1ExponentDigits-1; idxChar >= 0; idxChar-- )
            {
                #if QT_VERSION < 0x050000
                if( strDivLineLabel1.toAscii()[idxChar] == '.' || strDivLineLabel1.toAscii()[idxChar] == ',' )
                #else
                if( strDivLineLabel1.toLatin1()[idxChar] == '.' || strDivLineLabel1.toLatin1()[idxChar] == ',' )
                #endif
                {
                    break;
                }
                #if QT_VERSION < 0x050000
                else if( strDivLineLabel1.toAscii()[idxChar] != '0' )
                #else
                else if( strDivLineLabel1.toLatin1()[idxChar] != '0' )
                #endif
                {
                    break;
                }
                iLabel1TrailingZeroesCount++;
            }
        }
        if( strDivLineLabel2.length() > 0 && iLabel1TrailingZeroesCount > 0 )
        {
            for( idxChar = strDivLineLabel2.length()-iLabel2ExponentDigits-1; idxChar >= 0; idxChar-- )
            {
                #if QT_VERSION < 0x050000
                if( strDivLineLabel2.toAscii()[idxChar] == '.' || strDivLineLabel2.toAscii()[idxChar] == ',' )
                #else
                if( strDivLineLabel2.toLatin1()[idxChar] == '.' || strDivLineLabel2.toLatin1()[idxChar] == ',' )
                #endif
                {
                    break;
                }
                #if QT_VERSION < 0x050000
                else if( strDivLineLabel2.toAscii()[idxChar] != '0' )
                #else
                else if( strDivLineLabel2.toLatin1()[idxChar] != '0' )
                #endif
                {
                    break;
                }
                iLabel2TrailingZeroesCount++;
            }
        }
        if( strDivLineLabel3.length() > 0 && iLabel2TrailingZeroesCount > 0 )
        {
            for( idxChar = strDivLineLabel3.length()-iLabel3ExponentDigits-1; idxChar >= 0; idxChar-- )
            {
                #if QT_VERSION < 0x050000
                if( strDivLineLabel3.toAscii()[idxChar] == '.' || strDivLineLabel3.toAscii()[idxChar] == ',' )
                #else
                if( strDivLineLabel3.toLatin1()[idxChar] == '.' || strDivLineLabel3.toLatin1()[idxChar] == ',' )
                #endif
                {
                    break;
                }
                #if QT_VERSION < 0x050000
                else if( strDivLineLabel3.toAscii()[idxChar] != '0' )
                #else
                else if( strDivLineLabel3.toLatin1()[idxChar] != '0' )
                #endif
                {
                    break;
                }
                iLabel3TrailingZeroesCount++;
            }
        }
        if( iLabel1TrailingZeroesCount > 0 && iLabel2TrailingZeroesCount > 0 && iLabel3TrailingZeroesCount > 0 )
        {
            iTrailingZeroes = iLabel3TrailingZeroesCount;
            if( iTrailingZeroes > iLabel2TrailingZeroesCount )
            {
                iTrailingZeroes = iLabel2TrailingZeroesCount;
            }
            if( iTrailingZeroes > iLabel1TrailingZeroesCount )
            {
                iTrailingZeroes = iLabel1TrailingZeroesCount;
            }
        }

        iPrecisionMin -= iTrailingZeroes;
        if( iPrecisionMin < 1 )
        {
            iPrecisionMin = 1;
        }
        iPrecisionMinPrev = iPrecisionMin-1;

        // At this point the values to be indicated have been formatted according to the
        // scale resolution, the absolute minimum and maximum value and trailing zeros
        // (if there have been any) have been removed. Now we need to ensure that each
        // axis label is different.
        while( iPrecisionMin != iPrecisionMinPrev )
        {
            iPrecisionMinPrev = iPrecisionMin;

            if( m_iDivLineLabelsExponentDigits > 0 )
            {
                strDivLineLabel1 = QString::number(
                    /* fVal       */ fDivLineVal1,
                    /* chFormat   */ 'e',
                    /* iPrecision */ iPrecisionMin );
                strDivLineLabel2 = QString::number(
                    /* fVal       */ fDivLineVal2,
                    /* chFormat   */ 'e',
                    /* iPrecision */ iPrecisionMin );
                if( iDivLineLabelsCountTmp == 3 )
                {
                    strDivLineLabel3 = QString::number(
                        /* fVal       */ fDivLineVal3,
                        /* chFormat   */ 'e',
                        /* iPrecision */ iPrecisionMin );
                }
            }
            else
            {
                strDivLineLabel1 = QString::number(
                    /* fVal       */ fDivLineVal1,
                    /* chFormat   */ 'f',
                    /* iPrecision */ iPrecisionMin );
                strDivLineLabel2 = QString::number(
                    /* fVal       */ fDivLineVal2,
                    /* chFormat   */ 'f',
                    /* iPrecision */ iPrecisionMin );
                if( iDivLineLabelsCountTmp == 3 )
                {
                    strDivLineLabel3 = QString::number(
                        /* fVal       */ fDivLineVal3,
                        /* chFormat   */ 'f',
                        /* iPrecision */ iPrecisionMin );
                }
            }

            // All labels must be different ...
            if( strDivLineLabel1 == strDivLineLabel2 )
            {
                iPrecisionMin++;
            }
            if( iDivLineLabelsCountTmp == 3 )
            {
                if( strDivLineLabel1 == strDivLineLabel3 || strDivLineLabel2 == strDivLineLabel3 )
                {
                    iPrecisionMin++;
                }
            }

            // Just to avoid an endless loop in any case ..
            if( iPrecisionMin > 100 )
            {
                break;
            }
        } // while( iPrecisionMin != iPrecisionMinPrev )

    } // if( uDivLineLabelsCount < 2 )

    // If at least two division lines are available ..
    else if( iDivLinesCount >= 2 && m_pDiagScale->isScaleValid() && fScaleMaxVal > fScaleMinVal )
    {
        int aridxDivLine[EDivLineLayerCount];
        int iLayerTmp;

        // The division lines are rounded to a whole number of a decimal power.
        // To output the axis labels it is "just" necessary to ensure that the
        // indicated values are different.

        iPrecisionMin = m_iDivLineLabelsTrailingDigits-1;
        iPrecisionMinPrev = iPrecisionMin-1;

        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
        {
            strTrcMsg = "Loop to ensure different div line labels: ";
            mthTracer.trace(strTrcMsg);
        }

        // As long as there are two equal successive labels ...
        while( iPrecisionMin != iPrecisionMinPrev )
        {
            if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
            {
                strTrcMsg  = "PrecisionMin=";
                strTrcMsg += QString::number(iPrecisionMin) + ",";
                strTrcMsg += "PrecisionMinPrev=";
                strTrcMsg += QString::number(iPrecisionMinPrev);
                mthTracer.trace(strTrcMsg);
            }

            iPrecisionMinPrev = iPrecisionMin;

            for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
            {
                aridxDivLine[iLayer] = 0;
            }

            // Find the division line layer with the lowest value ...
            for( fDivLineVal1 = fScaleMaxVal, iLayer = 0, iLayerTmp = 0; iLayerTmp < EDivLineLayerCount; iLayerTmp++ )
            {
                if( m_ariDivLinesCount[iLayerTmp] > 0 )
                {
                    fDivLineVal = m_pDiagScale->getDivLineVal(iLayerTmp,0,&m_unitLabels);
                    if( fDivLineVal < fDivLineVal1 )
                    {
                        fDivLineVal1 = fDivLineVal;
                        iLayer = iLayerTmp;
                    }
                }
            }
            aridxDivLine[iLayer]++;

            for( idxDivLine = 1; idxDivLine < iDivLinesCount; idxDivLine++ )
            {
                // Find the division line layer with the next value ...
                for( fDivLineVal2 = fScaleMaxVal, iLayer = 0, iLayerTmp = 0; iLayerTmp < EDivLineLayerCount; iLayerTmp++ )
                {
                    if( aridxDivLine[iLayerTmp] < m_ariDivLinesCount[iLayerTmp] )
                    {
                        fDivLineVal = m_pDiagScale->getDivLineVal(iLayerTmp,aridxDivLine[iLayerTmp],&m_unitLabels);
                        if( fDivLineVal < fDivLineVal2 )
                        {
                            fDivLineVal2 = fDivLineVal;
                            iLayer = iLayerTmp;
                        }
                    }
                }
                aridxDivLine[iLayer]++;

                if( m_iDivLineLabelsExponentDigits > 0 )
                {
                    strDivLineLabel1 = QString::number(
                        /* fVal       */ fDivLineVal1,
                        /* chFormat   */ 'e',
                        /* iPrecision */ iPrecisionMin );
                    strDivLineLabel2 = QString::number(
                        /* fVal       */ fDivLineVal2,
                        /* chFormat   */ 'e',
                        /* iPrecision */ iPrecisionMin );
                }
                else
                {
                    strDivLineLabel1 = QString::number(
                        /* fVal       */ fDivLineVal1,
                        /* chFormat   */ 'f',
                        /* iPrecision */ iPrecisionMin );
                    strDivLineLabel2 = QString::number(
                        /* fVal       */ fDivLineVal2,
                        /* chFormat   */ 'f',
                        /* iPrecision */ iPrecisionMin );
                }
                fDivLineDist1 = strDivLineLabel2.toDouble() - strDivLineLabel1.toDouble();

                // Two successive labels may not be equal ...
                if( strDivLineLabel1 == strDivLineLabel2 )
                {
                    iPrecisionMin++;
                    break; // for( idxDivLine < uDivLineCount )
                }

                if( m_iDivLineLabelsExponentDigits > 0 )
                {
                    strDivLineLabel1 = QString::number(
                        /* fVal       */ fDivLineVal1,
                        /* chFormat   */ 'e',
                        /* iPrecision */ iPrecisionMin+1 );
                    strDivLineLabel2 = QString::number(
                        /* fVal       */ fDivLineVal2,
                        /* chFormat   */ 'e',
                        /* iPrecision */ iPrecisionMin+1 );
                }
                else
                {
                    strDivLineLabel1 = QString::number(
                        /* fVal       */ fDivLineVal1,
                        /* chFormat   */ 'f',
                        /* iPrecision */ iPrecisionMin+1 );
                    strDivLineLabel2 = QString::number(
                        /* fVal       */ fDivLineVal2,
                        /* chFormat   */ 'f',
                        /* iPrecision */ iPrecisionMin+1 );
                }
                fDivLineDist2 = strDivLineLabel2.toDouble() - strDivLineLabel1.toDouble();

                // The distance between all labels must be equal ...
                if( fDivLineDist1 < fDivLineDist2 - m_pDiagScale->getScaleRes(&m_unitLabels)
                 || fDivLineDist1 > fDivLineDist2 + m_pDiagScale->getScaleRes(&m_unitLabels) )
                {
                    iPrecisionMin++;
                    break; // for( idxDivLine < uDivLineCount )
                }
                fDivLineVal1 = fDivLineVal2;
            }

            // Just to avoid an endless loop in any case (there are problems with the
            // floating point accuracy in the LINUX release version) ..
            if( iPrecisionMin > 100 )
            {
                break;
            }

        } // while( iPrecisionMin != iPrecisionMinPrev )

        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
        {
            strTrcMsg  = "Calculated precision = ";
            strTrcMsg += QString::number(iPrecisionMin);
            mthTracer.trace(strTrcMsg);
        }

    } // if( uDivLineLabelsCount >= 2 )

    if( iPrecisionMin > m_iDivLineLabelsTrailingDigits )
    {
        m_iDivLineLabelsTrailingDigits = iPrecisionMin;
    }

    // If engineering format should be used ..
    if( m_iDivLineLabelsDigitsCountMax > 0 || m_bUseEngineeringFormat )
    {
        if( iDivLineLabelsLeadingDigits + m_iDivLineLabelsTrailingDigits > m_iDivLineLabelsDigitsCountMax
         || ( m_bUseEngineeringFormat && (iDivLineLabelsLeadingDigits > 1) ) )
        {
            m_iDivLineLabelsExponentDigits = static_cast<int>(log10(static_cast<double>(iDivLineLabelsLeadingDigits)))+1;
            iTmp = static_cast<int>(log10(static_cast<double>(m_iDivLineLabelsTrailingDigits)))+1;
            if( iTmp > m_iDivLineLabelsExponentDigits )
            {
                m_iDivLineLabelsExponentDigits = iTmp;
            }
            iDivLineLabelsLeadingDigits = 1;
            m_iDivLineLabelsTrailingDigits = m_iDivLineLabelsDigitsCountMax - iDivLineLabelsLeadingDigits - m_iDivLineLabelsExponentDigits; //lint !e834
            if( m_iDivLineLabelsTrailingDigits < 1 )
            {
                m_iDivLineLabelsTrailingDigits = 1;
            }
        }
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "Digits needed for all DivLineLabels: ";
        strTrcMsg += "LeadingDigits=" + QString::number(iDivLineLabelsLeadingDigits) + ",";
        strTrcMsg += "TrailingDigits=" + QString::number(m_iDivLineLabelsTrailingDigits) + ",";
        strTrcMsg += "ExponentDigits=" + QString::number(m_iDivLineLabelsExponentDigits);
        mthTracer.trace(strTrcMsg);
    }

    // Build string with maximum count of necessary digits and calculate
    // the maximum extent of the division line labels:
    strDivLineLabel += "+";
    for( idx = 0; idx < iDivLineLabelsLeadingDigits; idx++ )
    {
        strDivLineLabel += "0";
    }
    if( m_iDivLineLabelsTrailingDigits > 0 )
    {
        strDivLineLabel += ".";
        for( idx = 0; idx < m_iDivLineLabelsTrailingDigits; idx++ )
        {
            strDivLineLabel += "0";
        }
    }
    if( m_iDivLineLabelsExponentDigits > 0 )
    {
        strDivLineLabel += "e+";
        for( idx = 0; idx < m_iDivLineLabelsExponentDigits; idx++ )
        {
            strDivLineLabel += "0";
        }
    }
    // I'm not sure if "0" is the most "wide" character ...
    strDivLineLabel += "0"; // .. so we add one additional character.
    m_rectDivLineLabelsMaxTextExtent = fntmtr.boundingRect(strDivLineLabel);
    m_rectDivLineLabelsMaxTextExtent.setHeight( m_rectDivLineLabelsMaxTextExtent.height() + c_iMaxTextExtentAddHeight );
    m_rectDivLineLabelsMaxTextExtent.setWidth( m_rectDivLineLabelsMaxTextExtent.width() + c_iMaxTextExtentAddWidth );

    // There must be a distance (space) between two successive division line labels on the
    // horizontal (X) axis. One and a half of the width of the character '0' is a good choice:
    m_cxDivLineLabelsSpace  = fntmtr.boundingRect("0").width();
    m_cxDivLineLabelsSpace += m_cxDivLineLabelsSpace/2;

    // Calculate extent of string to indicate the physical unit:
    //-------------------------------------------------------------

    if( m_arbShowUnit[EPartDivLineLabels] )
    {
        m_rectDivLineLabelsPhysUnit = fntmtr.boundingRect(m_strPhysUnitLabels);
        m_rectDivLineLabelsPhysUnit.setHeight( m_rectDivLineLabelsPhysUnit.height() + c_iMaxTextExtentAddHeight );
        m_rectDivLineLabelsPhysUnit.setWidth( m_rectDivLineLabelsPhysUnit.width() + c_iMaxTextExtentAddWidth );

        switch( m_layoutPos )
        {
            // X-Axis (vertical division lines)
            case ELayoutPosTop:
            case ELayoutPosBottom:
            {
                xDivLine = m_pDiagScale->getScaleMaxValPix();
                m_rectDivLineLabelsPhysUnit.moveLeft( xDivLine - m_rectDivLineLabelsPhysUnit.width()/2 );

                if( m_bAdjustContentRect2DiagPartCenter )
                {
                    if( m_rectDivLineLabelsPhysUnit.right() >= m_pDiagScale->getScaleMaxValPix() )
                    {
                        cxOffset = m_rectDivLineLabelsPhysUnit.right()-m_pDiagScale->getScaleMaxValPix()+1;
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

                yDivLine = m_pDiagScale->getScaleMaxValPix();
                m_rectDivLineLabelsPhysUnit.moveTop( yDivLine - m_rectDivLineLabelsPhysUnit.height()/2 );

                if( m_bAdjustContentRect2DiagPartCenter )
                {
                    if( m_rectDivLineLabelsPhysUnit.top() <= m_pDiagScale->getScaleMaxValPix() )
                    {
                        cyOffset = m_pDiagScale->getScaleMaxValPix()-m_rectDivLineLabelsPhysUnit.top()+1;
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
                break;
            }
            default:
            {
                break;
            }

        } // switch( m_layoutPos )

    } // if( m_arbShowUnit[EPartDivLineLabels] )

    // Calculate extent of division line labels
    //-----------------------------------------

    m_bDivLineLabelsPhysUnitVisible = false;

    for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
    {
        if( m_ariDivLinesCount[iLayer] > 0 )
        {
            m_arpiDivLines[iLayer] = new int[m_ariDivLinesCount[iLayer]];
            m_arpRectDivLineLabels[iLayer] = new QRect[m_ariDivLinesCount[iLayer]];
            m_arpStrDivLineLabels[iLayer] = new QString[m_ariDivLinesCount[iLayer]];
            m_arpbDivLineLabelsVisible[iLayer] = new bool[m_ariDivLinesCount[iLayer]];

            for( idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]; idxDivLine++ )
            {
                m_arpbDivLineLabelsVisible[iLayer][idxDivLine] = false;
            }
        }
    }
    for( idx = 0; idx < 2; idx++ )
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
            for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
            {
                if( m_ariDivLinesCount[iLayer] > 0 && m_arpiDivLines[iLayer] != nullptr )
                {
                    for( idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]; idxDivLine++ )
                    {
                        xDivLine = static_cast<int>(m_pDiagScale->getDivLinePix(iLayer,idxDivLine)+0.5);
                        m_arpiDivLines[iLayer][idxDivLine] = xDivLine;
                    }
                }
            }

            // Division line labels
            //---------------------

            for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
            {
                if( m_ariDivLinesCount[iLayer] > 0 && m_arpRectDivLineLabels[iLayer] != nullptr && m_arpStrDivLineLabels[iLayer] != nullptr )
                {
                    rectDivLineLabel.setHeight(m_rectDivLineLabelsMaxTextExtent.height());

                    for( idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]; idxDivLine++ )
                    {
                        xDivLine = static_cast<int>(m_pDiagScale->getDivLinePix(iLayer,idxDivLine)+0.5);
                        strDivLineLabel = formatValue( m_pDiagScale->getDivLineVal(iLayer,idxDivLine,&m_unitLabels) );

                        // At the horizontal (X) axis the labels will be output centered below the division lines
                        // and we can remove trailing zeros:
                        removeTrailingZeros(&strDivLineLabel);
                        rectDivLineLabel = fntmtr.boundingRect(strDivLineLabel);
                        rectDivLineLabel.setWidth( rectDivLineLabel.width() + m_cxDivLineLabelsSpace );
                        rectDivLineLabel.moveLeft( xDivLine - rectDivLineLabel.width()/2 );

                        // To discover overlapping division line labels the rectangle extents of the
                        // division line labels will be stored in an array.
                        m_arpRectDivLineLabels[iLayer][idxDivLine] = rectDivLineLabel;
                        m_arpStrDivLineLabels[iLayer][idxDivLine] = strDivLineLabel;
                    }
                }
            } // for( iLayer < EDivLineLayerCount )

            // Minimum and maximum scale value
            //---------------------------------

            // Additionally try to show the minimum and/or maximum scale values if
            // not enough division lines are visible ..
            for( idx = 0; idx < 2; idx++ )
            {
                m_arstrScaleMinMaxVal[idx] = "";
                m_arrectScaleMinMaxVal[idx].setWidth(0);
                m_arrectScaleMinMaxVal[idx].setHeight(0);

                if( idx == 0 )
                {
                    formatResult = PhysVal::formatValue(
                        /* fValue                */ fScaleMinVal,
                        /* unitVal               */ m_pDiagScale->getScale().m_unit,
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
                    xDivLine = m_pDiagScale->getScaleMinValPix();
                }
                else
                {
                    formatResult = PhysVal::formatValue(
                        /* fVal                  */ fScaleMaxVal,
                        /* unitVal               */ m_pDiagScale->getScale().m_unit,
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
                    xDivLine = m_pDiagScale->getScaleMaxValPix();
                }
                m_arrectScaleMinMaxVal[idx] = fntmtr.boundingRect(m_arstrScaleMinMaxVal[idx]);
                m_arrectScaleMinMaxVal[idx].setWidth( m_arrectScaleMinMaxVal[idx].width() + c_iMaxTextExtentAddWidth );
                m_arrectScaleMinMaxVal[idx].setHeight( m_arrectScaleMinMaxVal[idx].height() + c_iMaxTextExtentAddHeight );
                m_arrectScaleMinMaxVal[idx].moveLeft( xDivLine - m_arrectScaleMinMaxVal[idx].width()/2 );

                if( m_arrectScaleMinMaxVal[idx].left() <= rectDiag.left() )
                {
                    cxOffset = rectDiag.left()-m_arrectScaleMinMaxVal[idx].left()+1;
                    m_arrectScaleMinMaxVal[idx].moveLeft( m_arrectScaleMinMaxVal[idx].left()+cxOffset );
                }
                else if( m_arrectScaleMinMaxVal[idx].right() >= rectDiag.right() )
                {
                    cxOffset = m_arrectScaleMinMaxVal[idx].right()-rectDiag.right()+1;
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

            for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
            {
                if( m_ariDivLinesCount[iLayer] > 0 && m_arpiDivLines[iLayer] != nullptr )
                {
                    // Division lines at the main grid:
                    for( idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]; idxDivLine++ )
                    {
                        yDivLine = static_cast<int>(m_pDiagScale->getDivLinePix(iLayer,idxDivLine)+0.5);
                        m_arpiDivLines[iLayer][idxDivLine] = yDivLine;
                    }
                }
            }

            // Division line labels
            //---------------------

            for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
            {
                if( m_ariDivLinesCount[iLayer] > 0 && m_arpRectDivLineLabels[iLayer] != nullptr && m_arpStrDivLineLabels[iLayer] != nullptr )
                {
                    rectDivLineLabel.setWidth(m_rectDivLineLabelsMaxTextExtent.width());
                    rectDivLineLabel.setHeight(m_rectDivLineLabelsMaxTextExtent.height());

                    // Division lines at the main grid:
                    for( idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]; idxDivLine++ )
                    {
                        yDivLine = static_cast<int>(m_pDiagScale->getDivLinePix(iLayer,idxDivLine)+0.5);
                        strDivLineLabel = formatValue( m_pDiagScale->getDivLineVal(iLayer,idxDivLine,&m_unitLabels) );
                        rectDivLineLabel.moveTop( yDivLine - m_rectDivLineLabelsMaxTextExtent.height()/2 );

                        // To discover overlapping division line labels the rectangle extents of the
                        // division line labels will be stored in an array.
                        m_arpRectDivLineLabels[iLayer][idxDivLine] = rectDivLineLabel;
                        m_arpStrDivLineLabels[iLayer][idxDivLine] = strDivLineLabel;
                    }
                }
            } // for( iLayer < EDivLineLayerCount )

            // Minimum and maximum scale value
            //---------------------------------

            // Additionally try to show the minimum and/or maximum scale values if
            // not enough division lines are visible ..
            for( idx = 0; idx < 2; idx++ )
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

                if( idx == 0 )
                {
                    formatResult = PhysVal::formatValue(
                        /* fVal                  */ fScaleMinVal,
                        /* unitVal               */ m_pDiagScale->getScale().m_unit,
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
                    yDivLine = m_pDiagScale->getScaleMinValPix();
                }
                else
                {
                    formatResult = PhysVal::formatValue(
                        /* fVal                  */ fScaleMaxVal,
                        /* unitVal              */ m_pDiagScale->getScale().m_unit,
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
                    yDivLine = m_pDiagScale->getScaleMaxValPix();
                }
                m_arrectScaleMinMaxVal[idx] = fntmtr.boundingRect(m_arstrScaleMinMaxVal[idx]);
                m_arrectScaleMinMaxVal[idx].setWidth( m_arrectScaleMinMaxVal[idx].width() + c_iMaxTextExtentAddWidth );
                m_arrectScaleMinMaxVal[idx].setHeight( m_arrectScaleMinMaxVal[idx].height() + c_iMaxTextExtentAddHeight );
                m_arrectScaleMinMaxVal[idx].moveTop( yDivLine - m_arrectScaleMinMaxVal[idx].height()/2 );

                if( m_arrectScaleMinMaxVal[idx].bottom() >= rectDiag.bottom() )
                {
                    cyOffset = m_arrectScaleMinMaxVal[idx].bottom()-rectDiag.bottom()+1;
                    m_arrectScaleMinMaxVal[idx].moveBottom( m_arrectScaleMinMaxVal[idx].bottom()-cyOffset );
                }
                else if( m_arrectScaleMinMaxVal[idx].top() <= rectDiag.top() )
                {
                    cyOffset = rectDiag.top()-m_arrectScaleMinMaxVal[idx].top()+1;
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
    if( m_pDataDiagram->getUpdateType() >= EDiagramUpdateTypePixmap )
    {
        pPixmapDiagram = dynamic_cast<const CPixmapDiagram*>(m_pDataDiagram);
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

    if( m_ararbShow[EPartAxisLabel][EDivLineLayerMain] || m_arbShowUnit[EPartAxisLabel] )
    {
        if( m_ararbShow[EPartDivLineLabels][EDivLineLayerMain] )
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
                if( rectDivLineLabel.isValid() && m_iSpaceDiagBorder2AxisLabel < rectDivLineLabel.height() )
                {
                    rectAxisLabel.moveBottom( m_rectContent.bottom() - rectDivLineLabel.height() - m_iSpaceDiagBorder2AxisLabel );
                }
                else
                {
                    rectAxisLabel.moveBottom( m_rectContent.bottom() - m_iSpaceDiagBorder2AxisLabel );
                }
                rectAxisLabel.moveLeft( m_rectContent.width()/2 - cxWidth/2 );
                break;
            }
            case ELayoutPosBottom:
            {
                if( rectDivLineLabel.isValid() && m_iSpaceDiagBorder2AxisLabel < rectDivLineLabel.height() )
                {
                    rectAxisLabel.moveTop( m_rectContent.top() + rectDivLineLabel.height() + m_iSpaceDiagBorder2AxisLabel );
                }
                else
                {
                    rectAxisLabel.moveTop( m_rectContent.top() + m_iSpaceDiagBorder2AxisLabel );
                }
                rectAxisLabel.moveLeft( m_rectContent.width()/2 - cxWidth/2 );
                break;
            }
            case ELayoutPosLeft:
            {
                if( rectDivLineLabel.isValid() && m_iSpaceDiagBorder2AxisLabel < rectDivLineLabel.width() )
                {
                    rectAxisLabel.moveRight( m_rectContent.right() - rectDivLineLabel.width() - m_iSpaceDiagBorder2AxisLabel );
                }
                else
                {
                    rectAxisLabel.moveRight( m_rectContent.right() - m_iSpaceDiagBorder2AxisLabel );
                }
                rectAxisLabel.moveTop( m_rectContent.top() + m_rectContent.height()/2 - cyHeight/2 );
                break;
            }
            case ELayoutPosRight:
            {
                if( rectDivLineLabel.isValid() && m_iSpaceDiagBorder2AxisLabel < rectDivLineLabel.width() )
                {
                    rectAxisLabel.moveLeft( m_rectContent.left() + rectDivLineLabel.width() + m_iSpaceDiagBorder2AxisLabel );
                }
                else
                {
                    rectAxisLabel.moveLeft( m_rectContent.left() + m_iSpaceDiagBorder2AxisLabel );
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
    if( m_arbShowUnit[EPartDivLineLabels] )
    {
        switch( m_layoutPos )
        {
            // X-Axis (vertical division lines)
            case ELayoutPosTop:
            case ELayoutPosBottom:
            {
                xDivLine = m_pDiagScale->getScaleMaxValPix();
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
                yDivLine = m_pDiagScale->getScaleMaxValPix();
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

    } // if( m_arbShowUnit[EPartDivLineLabels] )

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

            for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
            {
                if( m_ariDivLinesCount[iLayer] > 0 && m_arpiDivLines[iLayer] != nullptr )
                {
                    for( idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]; idxDivLine++ )
                    {
                        xDivLine = static_cast<int>(m_pDiagScale->getDivLinePix(iLayer,idxDivLine)+0.5);
                        m_arpiDivLines[iLayer][idxDivLine] = xDivLine;
                    }
                }
            }

            // Division line labels
            //---------------------

            for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
            {
                if( m_ariDivLinesCount[iLayer] > 0 && m_arpRectDivLineLabels[iLayer] != nullptr && m_arpStrDivLineLabels[iLayer] != nullptr )
                {
                    // The labels on the horizontal (X) axis will be output center aligned to
                    // the division lines. The rectangles of the division line labels and the
                    // rectangle for the unit string don't need to be of same size.
                    for( idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]; idxDivLine++ )
                    {
                        rectDivLineLabel = m_arpRectDivLineLabels[iLayer][idxDivLine];
                        xDivLine = static_cast<int>(m_pDiagScale->getDivLinePix(iLayer,idxDivLine)+0.5);
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
                        m_arpRectDivLineLabels[iLayer][idxDivLine] = rectDivLineLabel;
                    }
                } // if( m_ariDivLinesCount[iLayer] > 0 )
            } // for( iLayer < EDivLineLayerCount )

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
                        xDivLine = m_pDiagScale->getScaleMinValPix();
                    }
                    else
                    {
                        xDivLine = m_pDiagScale->getScaleMaxValPix();
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

            for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
            {
                if( m_ariDivLinesCount[iLayer] > 0 && m_arpiDivLines[iLayer] != nullptr )
                {
                    // Division lines at the main grid:
                    for( idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]; idxDivLine++ )
                    {
                        yDivLine = static_cast<int>(m_pDiagScale->getDivLinePix(iLayer,idxDivLine)+0.5);
                        m_arpiDivLines[iLayer][idxDivLine] = yDivLine;
                    }
                }
            }

            // Division line labels
            //---------------------

            for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
            {
                if( m_ariDivLinesCount[iLayer] > 0 && m_arpRectDivLineLabels[iLayer] != nullptr && m_arpStrDivLineLabels[iLayer] != nullptr )
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
                        yDivLine = static_cast<int>(m_pDiagScale->getDivLinePix(iLayer,idxDivLine)+0.5);
                        rectDivLineLabel.moveTop( yDivLine - m_rectDivLineLabelsMaxTextExtent.height()/2 );
                        m_arpRectDivLineLabels[iLayer][idxDivLine] = rectDivLineLabel;
                    }

                } // if( m_ariDivLinesCount[iLayer] > 0 )
            } // for( iLayer < EDivLineLayerCount )

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
                        yDivLine = m_pDiagScale->getScaleMinValPix();
                    }
                    else
                    {
                        yDivLine = m_pDiagScale->getScaleMaxValPix();
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
    if( m_pDataDiagram != nullptr && m_pDataDiagram->getUpdateType() >= EDiagramUpdateTypePixmap )
    {
        pPixmapDiagram = dynamic_cast<const CPixmapDiagram*>(m_pDataDiagram);
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
    QFontMetrics  fntmtr(m_arfnt[EPartAxisLabel]);
    QRect         rectAxisLabel;
    QString       strAxisLabel;

    if( m_ararbShow[EPartAxisLabel][EDivLineLayerMain] || m_arbShowUnit[EPartAxisLabel] )
    {
        strAxisLabel = m_strAxisLabel;

        if( m_arbShowUnit[EPartAxisLabel] )
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

            for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
            {
                if( m_arpiDivLines[iLayer] != nullptr && m_ararbShow[EPartDivLines][iLayer] )
                {
                    painter.setPen(m_arcolFg[EPartDivLines]);

                    for( idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]; idxDivLine++ )
                    {
                        xDivLine = m_arpiDivLines[iLayer][idxDivLine];

                        if( m_layoutPos == ELayoutPosTop )
                        {
                            painter.drawLine(xDivLine,m_rectContent.bottom()-m_iSpaceDiagPartCenter2DivLineLabels+2,xDivLine,m_rectContent.bottom());
                        }
                        else
                        {
                            painter.drawLine(xDivLine,m_rectContent.top(),xDivLine,m_rectContent.top()+m_iSpaceDiagPartCenter2DivLineLabels-2);
                        }
                    }
                } // if( m_bShowDivLines || m_bShowSubDivLines )
            } // for( iLayer < EDivLineLayerCount )

            // Physical unit at the maximum scale value
            //-----------------------------------------

            painter.setFont(m_arfnt[EPartDivLineLabels]);
            painter.setPen(m_arcolFg[EPartDivLineLabels]);

            // Please note that we don't draw the text for the physical unit yet.
            // If it is necessary to indicate the maximum scale value as only
            // one or even no division line label is visible the physical unit should
            // not be indicated at the right border of the diagrams center area but
            // the maximum scale value should be drawn there. In this case the physical
            // unit should be moved to the left. A good choice would be:
            // - behind the minimum scale value label
            // - behind the last visible division line label.
            if( m_arbShowUnit[EPartDivLineLabels] )
            {
                // But the division line labels should not be placed where the unit
                // string may be indicated ... (see method "intersectsDivLineLabels").
                m_bDivLineLabelsPhysUnitVisible = true;
            }

            // Labels at the division lines
            //------------------------------

            // We start with the highest layer (the main grid lines) as they are
            // "more important" than the sub division lines.
            for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
            {
                if( m_arpStrDivLineLabels[iLayer] != nullptr && m_arpRectDivLineLabels[iLayer] != nullptr && m_ararbShow[EPartDivLineLabels][iLayer] )
                {
                    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
                    {
                        strTrcMsg = "Division lines:";
                        mthTracer.trace(strTrcMsg);
                    }

                    for( idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]; idxDivLine++ )
                    {
                        strDivLineLabel = m_arpStrDivLineLabels[iLayer][idxDivLine];
                        rectDivLineLabel = m_arpRectDivLineLabels[iLayer][idxDivLine];

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
                                if( !intersectsDivLineLabels(rectDivLineLabel,iLayer,true,0,idxDivLine-1) )
                                {
                                    m_arpbDivLineLabelsVisible[iLayer][idxDivLine] = true;
                                    idxDivLineLabelLastVisible = idxDivLine;
                                    iLayerOfDivLineLabelLastVisible = iLayer;
                                    iDivLineLabelsVisibleCount++;

                                    if( m_arbrushStyle[EPartDivLineLabels] != Qt::NoBrush )
                                    {
                                        QBrush brush(m_arcolBg[EPartDivLineLabels],m_arbrushStyle[EPartDivLineLabels]);
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
                                if( !intersectsDivLineLabels(rectDivLineLabel,iLayer,true,0,idxDivLine-1) )
                                {
                                    m_arpbDivLineLabelsVisible[iLayer][idxDivLine] = true;
                                    idxDivLineLabelLastVisible = idxDivLine;
                                    iLayerOfDivLineLabelLastVisible = iLayer;
                                    iDivLineLabelsVisibleCount++;

                                    if( m_arbrushStyle[EPartDivLineLabels] != Qt::NoBrush )
                                    {
                                        QBrush brush(m_arcolBg[EPartDivLineLabels],m_arbrushStyle[EPartDivLineLabels]);
                                        painter.fillRect( rectDivLineLabel, brush );
                                    }
                                    painter.drawText(rectDivLineLabel,Qt::AlignHCenter|Qt::AlignVCenter,strDivLineLabel); //lint !e655
                                }
                            }
                        }
                    }
                } // if( m_bShowDivLineLabels || m_bShowSubDivLineLabels )
            } // for( iLayer < EDivLineLayerCount )

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
                            if( !intersectsDivLineLabels(rectDivLineLabel,-1,true) )
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
                                if( !intersectsDivLineLabels(rectDivLineLabel,-1,true) )
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

                if( m_arbShowUnit[EPartDivLineLabels] )
                {
                    // First try to append the unit at the maximum scale value.
                    idx = 1;
                    if( m_arbScaleMinMaxValVisible[idx] )
                    {
                        rectDivLineLabel = m_arrectScaleMinMaxVal[idx];
                        rectDivLineLabel.moveLeft(rectDivLineLabel.left()-m_rectDivLineLabelsPhysUnit.width()-cxSpace);
                        rectDivLineLabel.setWidth(rectDivLineLabel.width()+m_rectDivLineLabelsPhysUnit.width()+cxSpace);

                        if( !intersectsDivLineLabels(rectDivLineLabel,-1,true) )
                        {
                            rectDivLineLabel.setWidth(m_arrectScaleMinMaxVal[idx].width()+cxSpace);
                            if( m_arbrushStyle[EPartDivLineLabels] != Qt::NoBrush )
                            {
                                QBrush brush(m_arcolBg[EPartDivLineLabels],m_arbrushStyle[EPartDivLineLabels]);
                                painter.fillRect( rectDivLineLabel, brush );
                            }
                            painter.drawText(rectDivLineLabel,Qt::AlignHCenter|Qt::AlignVCenter,m_arstrScaleMinMaxVal[idx]); //lint !e655

                            m_bDivLineLabelsPhysUnitVisible = true;
                            rectDivLineLabel.moveLeft(m_rectDivLineLabelsPhysUnit.left());
                            rectDivLineLabel.setWidth(m_rectDivLineLabelsPhysUnit.width());
                            if( m_arbrushStyle[EPartDivLineLabels] != Qt::NoBrush )
                            {
                                QBrush brush(m_arcolBg[EPartDivLineLabels],m_arbrushStyle[EPartDivLineLabels]);
                                painter.fillRect( rectDivLineLabel, brush );
                            }
                            painter.drawText(rectDivLineLabel,Qt::AlignHCenter|Qt::AlignVCenter,m_strPhysUnitLabels); //lint !e655
                        }
                        else
                        {
                            if( m_arbrushStyle[EPartDivLineLabels] != Qt::NoBrush )
                            {
                                QBrush brush(m_arcolBg[EPartDivLineLabels],m_arbrushStyle[EPartDivLineLabels]);
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

                        if( !intersectsDivLineLabels(rectDivLineLabel,-1,true) )
                        {
                            rectDivLineLabel.setWidth(m_arrectScaleMinMaxVal[idx].width()+cxSpace);
                            if( m_arbrushStyle[EPartDivLineLabels] != Qt::NoBrush )
                            {
                                QBrush brush(m_arcolBg[EPartDivLineLabels],m_arbrushStyle[EPartDivLineLabels]);
                                painter.fillRect( rectDivLineLabel, brush );
                            }
                            painter.drawText(rectDivLineLabel,Qt::AlignHCenter|Qt::AlignVCenter,m_arstrScaleMinMaxVal[idx]); //lint !e655

                            m_bDivLineLabelsPhysUnitVisible = true;
                            rectDivLineLabel.moveLeft(m_arrectScaleMinMaxVal[idx].right()+cxSpace);
                            rectDivLineLabel.setWidth(m_rectDivLineLabelsPhysUnit.width());
                            if( m_arbrushStyle[EPartDivLineLabels] != Qt::NoBrush )
                            {
                                QBrush brush(m_arcolBg[EPartDivLineLabels],m_arbrushStyle[EPartDivLineLabels]);
                                painter.fillRect( rectDivLineLabel, brush );
                            }
                            painter.drawText(rectDivLineLabel,Qt::AlignHCenter|Qt::AlignVCenter,m_strPhysUnitLabels); //lint !e655
                        }
                        else
                        {
                            if( m_arbrushStyle[EPartDivLineLabels] != Qt::NoBrush )
                            {
                                QBrush brush(m_arcolBg[EPartDivLineLabels],m_arbrushStyle[EPartDivLineLabels]);
                                painter.fillRect( m_arrectScaleMinMaxVal[idx], brush );
                            }
                            painter.drawText(m_arrectScaleMinMaxVal[idx],Qt::AlignHCenter|Qt::AlignVCenter,m_arstrScaleMinMaxVal[idx]); //lint !e655
                        }
                    }
                    else
                    {
                        if( m_arbrushStyle[EPartDivLineLabels] != Qt::NoBrush )
                        {
                            QBrush brush(m_arcolBg[EPartDivLineLabels],m_arbrushStyle[EPartDivLineLabels]);
                            painter.fillRect( m_arrectScaleMinMaxVal[idx], brush );
                        }
                        painter.drawText(m_arrectScaleMinMaxVal[idx],Qt::AlignHCenter|Qt::AlignVCenter,m_arstrScaleMinMaxVal[idx]); //lint !e655
                    }

                    // Third try to append the unit at the last visible division line label.
                    // There should be at maximum one visible division line at this point.
                    // Otherwise there was no need to indicate the maximum scale value.
                    if( !m_bDivLineLabelsPhysUnitVisible && iLayerOfDivLineLabelLastVisible >= 0 && idxDivLineLabelLastVisible >= 0 )
                    {
                        rectDivLineLabel = m_arpRectDivLineLabels[iLayerOfDivLineLabelLastVisible][idxDivLineLabelLastVisible];
                        rectDivLineLabel.setWidth( rectDivLineLabel.width() + m_rectDivLineLabelsPhysUnit.width() );

                        m_arpbDivLineLabelsVisible[iLayerOfDivLineLabelLastVisible][idxDivLineLabelLastVisible] = false;
                        if( !intersectsDivLineLabels(rectDivLineLabel,-1,true) )
                        {
                            m_bDivLineLabelsPhysUnitVisible = true;
                            rectDivLineLabel.moveLeft(m_arpRectDivLineLabels[iLayerOfDivLineLabelLastVisible][idxDivLineLabelLastVisible].right()+cxSpace);
                            rectDivLineLabel.setWidth(m_arrectScaleMinMaxVal[1].width()+cxSpace);
                            if( m_arbrushStyle[EPartDivLineLabels] != Qt::NoBrush )
                            {
                                QBrush brush(m_arcolBg[EPartDivLineLabels],m_arbrushStyle[EPartDivLineLabels]);
                                painter.fillRect( rectDivLineLabel, brush );
                            }
                            painter.drawText(rectDivLineLabel,Qt::AlignHCenter|Qt::AlignVCenter,m_strPhysUnitLabels); //lint !e655
                        }
                        m_arpbDivLineLabelsVisible[iLayerOfDivLineLabelLastVisible][idxDivLineLabelLastVisible] = true;
                    }
                }
                else // if( !m_arbShowUnit[EPartDivLineLabels] )
                {
                    for( idx = 0; idx < 2; idx++ )
                    {
                        if( m_arbScaleMinMaxValVisible[idx] )
                        {
                            painter.setFont(m_arfnt[EPartDivLineLabels]);
                            painter.setPen(m_arcolFg[EPartDivLineLabels]);

                            strDivLineLabel = m_arstrScaleMinMaxVal[idx];
                            rectDivLineLabel = m_arrectScaleMinMaxVal[idx];

                            if( m_arbrushStyle[EPartDivLineLabels] != Qt::NoBrush )
                            {
                                QBrush brush(m_arcolBg[EPartDivLineLabels],m_arbrushStyle[EPartDivLineLabels]);
                                painter.fillRect( rectDivLineLabel, brush );
                            }
                            painter.drawText(rectDivLineLabel,Qt::AlignHCenter|Qt::AlignVCenter,strDivLineLabel); //lint !e655

                        } // if( m_arbScaleMinMaxValVisible[idx] )
                    } // for( idx < 2 )
                } // if( !m_arbShowUnit[EPartDivLineLabels] )
            } // if( iDivLineLabelsVisibleCount < 2 )

            // Physical unit at the maximum scale value
            //-----------------------------------------

            // Please see note above why the text for the physical unit may not have been drawn yet.
            if( m_arbShowUnit[EPartDivLineLabels] && !m_bDivLineLabelsPhysUnitVisible )
            {
                painter.setFont(m_arfnt[EPartDivLineLabels]);
                painter.setPen(m_arcolFg[EPartDivLineLabels]);

                m_bDivLineLabelsPhysUnitVisible = true;

                if( m_arbrushStyle[EPartDivLineLabels] != Qt::NoBrush )
                {
                    QBrush brush(m_arcolBg[EPartDivLineLabels],m_arbrushStyle[EPartDivLineLabels]);
                    painter.fillRect( m_rectDivLineLabelsPhysUnit, brush );
                }
                painter.drawText(m_rectDivLineLabelsPhysUnit,Qt::AlignHCenter|Qt::AlignVCenter,m_strPhysUnitLabels); //lint !e655
            }

            // Axis Label
            //-----------

            if( (m_ararbShow[EPartAxisLabel][EDivLineLayerMain] || m_arbShowUnit[EPartAxisLabel]) && !strAxisLabel.isEmpty() )
            {
                painter.setFont(m_arfnt[EPartAxisLabel]);
                painter.setPen(m_arcolFg[EPartAxisLabel]);

                if( m_arbrushStyle[EPartAxisLabel] != Qt::NoBrush )
                {
                    QBrush brush(m_arcolBg[EPartAxisLabel],m_arbrushStyle[EPartAxisLabel]);
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
            } // if( m_ararbShow[EPartAxisLabel][EDivLineLayerMain] )

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

            for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
            {
                if( m_arpiDivLines[iLayer] != nullptr && m_ararbShow[EPartDivLines][iLayer] )
                {
                    painter.setPen(m_arcolFg[EPartDivLines]);
                    for( idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]; idxDivLine++ )
                    {
                        yDivLine = m_arpiDivLines[iLayer][idxDivLine];
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
            } // for( iLayer < EDivLineLayerCount )

            // Physical unit at the maximum scale value
            //-----------------------------------------

            painter.setFont(m_arfnt[EPartDivLineLabels]);
            painter.setPen(m_arcolFg[EPartDivLineLabels]);

            if( m_arbShowUnit[EPartDivLineLabels] )
            {
                m_bDivLineLabelsPhysUnitVisible = true;

                if( m_layoutPos == ELayoutPosLeft )
                {
                    if( m_arbrushStyle[EPartDivLineLabels] != Qt::NoBrush )
                    {
                        QBrush brush(m_arcolBg[EPartDivLineLabels],m_arbrushStyle[EPartDivLineLabels]);
                        painter.fillRect( m_rectDivLineLabelsPhysUnit, brush );
                    }
                    painter.drawText(m_rectDivLineLabelsPhysUnit,Qt::AlignRight|Qt::AlignVCenter,m_strPhysUnitLabels); //lint !e655
                }
                else
                {
                    if( m_iSpaceDiagPartCenter2DivLineLabels >= 0 )
                    {
                        if( m_arbrushStyle[EPartDivLineLabels] != Qt::NoBrush )
                        {
                            QBrush brush(m_arcolBg[EPartDivLineLabels],m_arbrushStyle[EPartDivLineLabels]);
                            painter.fillRect( m_rectDivLineLabelsPhysUnit, brush );
                        }
                        painter.drawText(m_rectDivLineLabelsPhysUnit,Qt::AlignLeft|Qt::AlignVCenter,m_strPhysUnitLabels); //lint !e655
                    }
                    else
                    {
                        if( m_arbrushStyle[EPartDivLineLabels] != Qt::NoBrush )
                        {
                            QBrush brush(m_arcolBg[EPartDivLineLabels],m_arbrushStyle[EPartDivLineLabels]);
                            painter.fillRect( m_rectDivLineLabelsPhysUnit, brush );
                        }
                        painter.drawText(m_rectDivLineLabelsPhysUnit,Qt::AlignRight|Qt::AlignVCenter,m_strPhysUnitLabels); //lint !e655
                    }
                }
            }

            // Labels at the division lines
            //-----------------------------

            for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
            {
                if( m_arpStrDivLineLabels[iLayer] != nullptr && m_arpRectDivLineLabels[iLayer] != nullptr && m_ararbShow[EPartDivLineLabels][iLayer] )
                {
                    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
                    {
                        strTrcMsg = "Division lines:";
                        mthTracer.trace(strTrcMsg);
                    }

                    for( idxDivLine = 0; idxDivLine < m_ariDivLinesCount[iLayer]; idxDivLine++ )
                    {
                        strDivLineLabel = m_arpStrDivLineLabels[iLayer][idxDivLine];
                        rectDivLineLabel = m_arpRectDivLineLabels[iLayer][idxDivLine];

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
                                    if( !intersectsDivLineLabels(rectDivLineLabel,iLayer,true,0,idxDivLine-1) )
                                    {
                                        m_arpbDivLineLabelsVisible[iLayer][idxDivLine] = true;
                                        iDivLineLabelsVisibleCount++;

                                        if( m_arbrushStyle[EPartDivLineLabels] != Qt::NoBrush )
                                        {
                                            QBrush brush(m_arcolBg[EPartDivLineLabels],m_arbrushStyle[EPartDivLineLabels]);
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
                                    if( !intersectsDivLineLabels(rectDivLineLabel,iLayer,true,0,idxDivLine-1) )
                                    {
                                        m_arpbDivLineLabelsVisible[iLayer][idxDivLine] = true;
                                        iDivLineLabelsVisibleCount++;

                                        if( m_arbrushStyle[EPartDivLineLabels] != Qt::NoBrush )
                                        {
                                            QBrush brush(m_arcolBg[EPartDivLineLabels],m_arbrushStyle[EPartDivLineLabels]);
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
                                    if( !intersectsDivLineLabels(rectDivLineLabel,iLayer,true,0,idxDivLine-1) )
                                    {
                                        m_arpbDivLineLabelsVisible[iLayer][idxDivLine] = true;
                                        iDivLineLabelsVisibleCount++;

                                        if( m_iSpaceDiagPartCenter2DivLineLabels >= 0 )
                                        {
                                            if( m_arbrushStyle[EPartDivLineLabels] != Qt::NoBrush )
                                            {
                                                QBrush brush(m_arcolBg[EPartDivLineLabels],m_arbrushStyle[EPartDivLineLabels]);
                                                painter.fillRect( rectDivLineLabel, brush );
                                            }
                                            painter.drawText(rectDivLineLabel,Qt::AlignLeft|Qt::AlignVCenter,strDivLineLabel); //lint !e655
                                        }
                                        else
                                        {
                                            if( m_arbrushStyle[EPartDivLineLabels] != Qt::NoBrush )
                                            {
                                                QBrush brush(m_arcolBg[EPartDivLineLabels],m_arbrushStyle[EPartDivLineLabels]);
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
                                    if( !intersectsDivLineLabels(rectDivLineLabel,iLayer,true,0,idxDivLine-1) )
                                    {
                                        m_arpbDivLineLabelsVisible[iLayer][idxDivLine] = true;
                                        iDivLineLabelsVisibleCount++;

                                        if( m_iSpaceDiagPartCenter2DivLineLabels >= 0 )
                                        {
                                            if( m_arbrushStyle[EPartDivLineLabels] != Qt::NoBrush )
                                            {
                                                QBrush brush(m_arcolBg[EPartDivLineLabels],m_arbrushStyle[EPartDivLineLabels]);
                                                painter.fillRect( rectDivLineLabel, brush );
                                            }
                                            painter.drawText(rectDivLineLabel,Qt::AlignLeft|Qt::AlignVCenter,strDivLineLabel); //lint !e655
                                        }
                                        else
                                        {
                                            if( m_arbrushStyle[EPartDivLineLabels] != Qt::NoBrush )
                                            {
                                                QBrush brush(m_arcolBg[EPartDivLineLabels],m_arbrushStyle[EPartDivLineLabels]);
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
            } // for( iLayer < EDivLineLayerCount )

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
                                if( !intersectsDivLineLabels(rectDivLineLabel,-1,true) )
                                {
                                    m_arbScaleMinMaxValVisible[idx] = true;
                                    m_arrectScaleMinMaxVal[idx] = rectDivLineLabel;

                                    if( m_arbrushStyle[EPartDivLineLabels] != Qt::NoBrush )
                                    {
                                        QBrush brush(m_arcolBg[EPartDivLineLabels],m_arbrushStyle[EPartDivLineLabels]);
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
                                    if( !intersectsDivLineLabels(rectDivLineLabel,-1,true) )
                                    {
                                        m_arbScaleMinMaxValVisible[idx] = true;
                                        m_arrectScaleMinMaxVal[idx] = rectDivLineLabel;

                                        if( m_arbrushStyle[EPartDivLineLabels] != Qt::NoBrush )
                                        {
                                            QBrush brush(m_arcolBg[EPartDivLineLabels],m_arbrushStyle[EPartDivLineLabels]);
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
                                if( !intersectsDivLineLabels(rectDivLineLabel,-1,true) )
                                {
                                    m_arbScaleMinMaxValVisible[idx] = true;
                                    m_arrectScaleMinMaxVal[idx] = rectDivLineLabel;

                                    if( m_iSpaceDiagPartCenter2DivLineLabels >= 0 )
                                    {
                                        if( m_arbrushStyle[EPartDivLineLabels] != Qt::NoBrush )
                                        {
                                            QBrush brush(m_arcolBg[EPartDivLineLabels],m_arbrushStyle[EPartDivLineLabels]);
                                            painter.fillRect( rectDivLineLabel, brush );
                                        }
                                        painter.drawText(rectDivLineLabel,Qt::AlignLeft|Qt::AlignVCenter,strDivLineLabel); //lint !e655
                                    }
                                    else
                                    {
                                        if( m_arbrushStyle[EPartDivLineLabels] != Qt::NoBrush )
                                        {
                                            QBrush brush(m_arcolBg[EPartDivLineLabels],m_arbrushStyle[EPartDivLineLabels]);
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
                                    if( !intersectsDivLineLabels(rectDivLineLabel,-1,true) )
                                    {
                                        m_arbScaleMinMaxValVisible[idx] = true;
                                        m_arrectScaleMinMaxVal[idx] = rectDivLineLabel;

                                        if( m_iSpaceDiagPartCenter2DivLineLabels >= 0 )
                                        {
                                            if( m_arbrushStyle[EPartDivLineLabels] != Qt::NoBrush )
                                            {
                                                QBrush brush(m_arcolBg[EPartDivLineLabels],m_arbrushStyle[EPartDivLineLabels]);
                                                painter.fillRect( rectDivLineLabel, brush );
                                            }
                                            painter.drawText(rectDivLineLabel,Qt::AlignLeft|Qt::AlignVCenter,strDivLineLabel); //lint !e655
                                        }
                                        else
                                        {
                                            if( m_arbrushStyle[EPartDivLineLabels] != Qt::NoBrush )
                                            {
                                                QBrush brush(m_arcolBg[EPartDivLineLabels],m_arbrushStyle[EPartDivLineLabels]);
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

            if( (m_ararbShow[EPartAxisLabel][EDivLineLayerMain] || m_arbShowUnit[EPartAxisLabel]) && !strAxisLabel.isEmpty() )
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
                painter.setFont(m_arfnt[EPartAxisLabel]);
                painter.setPen(m_arcolFg[EPartAxisLabel]);
                if( m_arbrushStyle[EPartAxisLabel] != Qt::NoBrush )
                {
                    QBrush brush(m_arcolBg[EPartAxisLabel],m_arbrushStyle[EPartAxisLabel]);
                    painter.fillRect(m_rectAxisLabel,brush);
                }
                painter.drawText(
                    /* x   */ -m_rectAxisLabel.height()/2,
                    /* y   */ m_rectAxisLabel.width()/2,
                    /* str */ strAxisLabel );
                painter.restore();

            } // if( m_ararbShow[EPartAxisLabel][EDivLineLayerMain] )

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

    CWdgtDiagram* pWdgtDiagram = nullptr;

    if( m_pDataDiagram->getUpdateType() >= EDiagramUpdateTypeWidget )
    {
        pWdgtDiagram = dynamic_cast<CWdgtDiagram*>(m_pDataDiagram);
    }
    if( pWdgtDiagram != nullptr )
    {
        // Invalidate output region of the diagram object to update (repaint) content of diagram.
        if( m_rectContentPrev.isValid() )
        {
            pWdgtDiagram->update(this,m_rectContentPrev);
            m_rectContentPrev = m_rectContent;
        }
        if( m_rectContent.isValid() )
        {
            pWdgtDiagram->update(this,m_rectContent);
        }

    } // if( pWdgtDiagram != nullptr )

    // Mark current process depth as executed (reset bit)
    //---------------------------------------------------

    validate(EUpdateWidget);

} // updateWidget

//------------------------------------------------------------------------------
bool CDiagObjAxisLabel::intersectsDivLineLabels(
    const QRect& i_rectDivLineLabel,
    int          i_iDivLineLayer,
    bool         i_bCheckAxisLabelsToBeConsidered,
    int          i_idxDivLineLabelMin,
    int          i_idxDivLineLabelMax ) const
//------------------------------------------------------------------------------
{
    bool bIntersect = false;
    int  iLayer;
    int  iLayerMax;
    int  idxDivLineMin;
    int  idxDivLineMax;
    int  idxDivLine;

    if( i_bCheckAxisLabelsToBeConsidered && m_arpAxisLabelsToBeConsidered != nullptr )
    {
        const CDiagObjAxisLabel* pDiagObjAxisLabelToBeConsidered;
        int                      idxAxisLabel;

        for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
        {
            for( idxAxisLabel = 0; idxAxisLabel < m_iAxisLabelsToBeConsidered && !bIntersect; idxAxisLabel++ )
            {
                pDiagObjAxisLabelToBeConsidered = m_arpAxisLabelsToBeConsidered[idxAxisLabel];
                if( pDiagObjAxisLabelToBeConsidered != nullptr )
                {
                    if( pDiagObjAxisLabelToBeConsidered->m_arbShowUnit[EPartDivLineLabels] )
                    {
                        if( i_rectDivLineLabel.intersects(pDiagObjAxisLabelToBeConsidered->m_rectDivLineLabelsPhysUnit) )
                        {
                            bIntersect = true;
                        }
                    }
                    if( !bIntersect )
                    {
                        if( pDiagObjAxisLabelToBeConsidered->m_ariDivLinesCount[iLayer] == 0 || pDiagObjAxisLabelToBeConsidered->m_arpRectDivLineLabels[iLayer] == nullptr )
                        {
                            bIntersect = false;
                        }
                        else
                        {
                            for( idxDivLine = 0; idxDivLine < pDiagObjAxisLabelToBeConsidered->m_ariDivLinesCount[iLayer]; idxDivLine++ )
                            {
                                if( pDiagObjAxisLabelToBeConsidered->m_arpbDivLineLabelsVisible[iLayer][idxDivLine] )
                                {
                                    if( i_rectDivLineLabel.intersects(pDiagObjAxisLabelToBeConsidered->m_arpRectDivLineLabels[iLayer][idxDivLine]) )
                                    {
                                        bIntersect = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } // for( iLayer < EDivLineLayerCount )
    } // if( i_bCheckAxisLabelsToBeConsidered && m_arpAxisLabelsToBeConsidered != nullptr )

    if( !bIntersect )
    {
        if( m_arbShowUnit[EPartDivLineLabels] && m_bDivLineLabelsPhysUnitVisible )
        {
            if( i_rectDivLineLabel.intersects(m_rectDivLineLabelsPhysUnit) )
            {
                bIntersect = true;
            }
        }
    } // if( !bIntersect )

    if( i_iDivLineLayer < 0 )
    {
        iLayerMax = EDivLineLayerCount-1;
    }
    else
    {
        iLayerMax = i_iDivLineLayer;
        if( iLayerMax > EDivLineLayerCount-1 )
        {
            iLayerMax = EDivLineLayerCount-1;
        }
    }

    // Check the "lower" layers ...
    if( !bIntersect && iLayerMax > 0 )
    {
        for( iLayer = 0; iLayer < iLayerMax; iLayer++ )
        {
            if( m_ariDivLinesCount[iLayer] == 0 || m_arpRectDivLineLabels[iLayer] == nullptr )
            {
                bIntersect = false;
            }
            else
            {
                idxDivLineMin = 0;
                idxDivLineMax = m_ariDivLinesCount[iLayer]-1;

                for( idxDivLine = idxDivLineMin; idxDivLine <= idxDivLineMax; idxDivLine++ )
                {
                    if( m_arpbDivLineLabelsVisible[iLayer][idxDivLine] )
                    {
                        if( i_rectDivLineLabel.intersects(m_arpRectDivLineLabels[iLayer][idxDivLine]) )
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
        iLayer = iLayerMax;

        if( m_ariDivLinesCount[iLayer] == 0 || m_arpRectDivLineLabels[iLayer] == nullptr )
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
            for( idxDivLine = idxDivLineMin; idxDivLine <= idxDivLineMax; idxDivLine++ )
            {
                if( m_arpbDivLineLabelsVisible[iLayer][idxDivLine] )
                {
                    if( i_rectDivLineLabel.intersects(m_arpRectDivLineLabels[iLayer][idxDivLine]) )
                    {
                        bIntersect = true;
                        break;
                    }
                }
            }
        }
    } // if( !bIntersect )

    return bIntersect;

} // intersectsDivLineLabels

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
