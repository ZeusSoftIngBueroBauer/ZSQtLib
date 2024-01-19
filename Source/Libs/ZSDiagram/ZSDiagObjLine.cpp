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

#include "ZSDiagram/ZSDiagObjLine.h"
#include "ZSDiagram/ZSDiagramProcData.h"
#include "ZSDiagram/ZSDiagramLineStyles.h"
#include "ZSDiagram/ZSDiagScale.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include <QtGui/qpainter.h>

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Diagram;
using namespace ZS::PhysVal;


/*******************************************************************************
class CDiagObjLine : public CDiagObj
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDiagObjLine::CDiagObjLine(
    const QString& i_strObjName,
    CDiagScale*    i_pDiagScaleX,
    CDiagScale*    i_pDiagScaleY,
    SLineStyle*    i_lineStyle ) :
//------------------------------------------------------------------------------
    CDiagObj(
        /* strClassName */ CDiagObjLine::ClassName(),
        /* strObjName   */ i_strObjName,
        /* pDiagScaleX  */ i_pDiagScaleX,
        /* pDiagScaleY  */ i_pDiagScaleY,
        /* layoutPos    */ ELayoutPos::Center ),
    m_pLineStyle( i_lineStyle ),
    m_pt1( -1 , -1 ),
    m_pt2( -1 , -1 ),
    m_fX1( -1 ),
    m_eX1( ECoordinatesUnitAxisScale ),
    m_fY1( -1 ),
    m_eY1( ECoordinatesUnitAxisScale ),
    m_fX2( -1 ),
    m_eX2( ECoordinatesUnitAxisScale ),
    m_fY2( -1 ),
    m_eY2( ECoordinatesUnitAxisScale )
{
} // ctor

//------------------------------------------------------------------------------
CDiagObjLine::~CDiagObjLine()
//------------------------------------------------------------------------------
{
    try
    {
        m_strObjName = "";
        m_pt1.setX( -1 );
        m_pt1.setY( -1 );
        m_pt2.setX( -1 );
        m_pt2.setY( -1 );
    }
    catch (...)
    {
    }
    delete( m_pLineStyle );
    m_pLineStyle = nullptr;
    m_fX1 = -1;
    m_eX1 = ECoordinatesUnitAxisScale;
    m_fY1 = -1;
    m_eX2 = ECoordinatesUnitAxisScale;
    m_fX2 = -1;
    m_eY1 = ECoordinatesUnitAxisScale;
    m_fY2 = -1;
    m_eY2 = ECoordinatesUnitAxisScale;

} // dtor

/*==============================================================================
public: // methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDiagObjLine::setCol( const QColor&  i_col )
//------------------------------------------------------------------------------
{
    if( m_pLineStyle && m_pLineStyle->getCol() != i_col )
    {
        m_pLineStyle->setCol( i_col );
        invalidate( EUpdatePixmapWidget , true );
    }
}

//------------------------------------------------------------------------------
QColor CDiagObjLine::getCol() const
//------------------------------------------------------------------------------
{
    if( m_pLineStyle )
        return( m_pLineStyle->getCol() );
    else
        return( Qt::black );
}

//------------------------------------------------------------------------------
void CDiagObjLine::setX1( double i_X1 , ECoordinatesUnit i_pointType )
//------------------------------------------------------------------------------
{
    m_fX1 = i_X1;
    m_eX1 = i_pointType;
    invalidate( EUpdateDataPixmapWidget , true );
}

//------------------------------------------------------------------------------
double CDiagObjLine::getX1( ECoordinatesUnit i_pointType ) const
//------------------------------------------------------------------------------
{
    if( i_pointType == ECoordinatesUnitAxisScale && m_eX1 == ECoordinatesUnitAxisScale)
        return( m_fX1 );
    else if( i_pointType == ECoordinatesUnitAxisScale && m_eX1 == ECoordinatesUnitPixel && m_arpDiagScale[static_cast<int>(EScaleAxis::X)] )
        return( m_arpDiagScale[static_cast<int>(EScaleAxis::X)]->getVal( m_fX1 ) );
    else if( i_pointType == ECoordinatesUnitPixel && m_eX1 == ECoordinatesUnitAxisScale && m_arpDiagScale[static_cast<int>(EScaleAxis::X)] )
        return( m_arpDiagScale[static_cast<int>(EScaleAxis::X)]->getValPix( m_fX1 ) );
    else if( i_pointType == ECoordinatesUnitPixel && m_eX1 == ECoordinatesUnitPixel && m_arpDiagScale[static_cast<int>(EScaleAxis::X)] )
        return( m_fX1 );
    else
        return( 0.0 );
}

//------------------------------------------------------------------------------
void CDiagObjLine::setY1( double i_Y1 , ECoordinatesUnit i_pointType )
//------------------------------------------------------------------------------
{
    m_fY1 = i_Y1;
    m_eY1 = i_pointType;
    invalidate( EUpdateDataPixmapWidget , true );
}

//------------------------------------------------------------------------------
double CDiagObjLine::getY1( ECoordinatesUnit i_pointType ) const
//------------------------------------------------------------------------------
{
    if( i_pointType == ECoordinatesUnitAxisScale && m_eY1 == ECoordinatesUnitAxisScale)
        return( m_fY1 );
    else if( i_pointType == ECoordinatesUnitAxisScale && m_eY1 == ECoordinatesUnitPixel && m_arpDiagScale[static_cast<int>(EScaleAxis::Y)] )
        return( m_arpDiagScale[static_cast<int>(EScaleAxis::Y)]->getVal( m_fX1 ) );
    else if( i_pointType == ECoordinatesUnitPixel && m_eY1 == ECoordinatesUnitAxisScale && m_arpDiagScale[static_cast<int>(EScaleAxis::Y)] )
        return( m_arpDiagScale[static_cast<int>(EScaleAxis::Y)]->getValPix( m_fY1 ) );
    else if( i_pointType == ECoordinatesUnitPixel && m_eY1 == ECoordinatesUnitPixel && m_arpDiagScale[static_cast<int>(EScaleAxis::Y)] )
        return( m_fY1 );
    else
        return( 0.0 );
}

//------------------------------------------------------------------------------
void CDiagObjLine::setX2( double i_X2 , ECoordinatesUnit i_pointType )
//------------------------------------------------------------------------------
{
    m_fX2 = i_X2;
    m_eX2 = i_pointType;
    invalidate( EUpdateDataPixmapWidget , true );
}

//------------------------------------------------------------------------------
double CDiagObjLine::getX2( ECoordinatesUnit i_pointType ) const
//------------------------------------------------------------------------------
{
    if( i_pointType == ECoordinatesUnitAxisScale && m_eX2 == ECoordinatesUnitAxisScale)
        return( m_fX1 );
    else if( i_pointType == ECoordinatesUnitAxisScale && m_eX2 == ECoordinatesUnitPixel && m_arpDiagScale[static_cast<int>(EScaleAxis::X)] )
        return( m_arpDiagScale[static_cast<int>(EScaleAxis::X)]->getVal( m_fX2 ) );
    else if( i_pointType == ECoordinatesUnitPixel && m_eX2 == ECoordinatesUnitAxisScale && m_arpDiagScale[static_cast<int>(EScaleAxis::X)] )
        return( m_arpDiagScale[static_cast<int>(EScaleAxis::X)]->getValPix( m_fX2 ) );
    else if( i_pointType == ECoordinatesUnitPixel && m_eX2 == ECoordinatesUnitPixel && m_arpDiagScale[static_cast<int>(EScaleAxis::X)] )
        return( m_fX2 );
    else
        return( 0.0 );
}

//------------------------------------------------------------------------------
void CDiagObjLine::setY2( double i_Y2 , ECoordinatesUnit i_pointType )
//------------------------------------------------------------------------------
{
      m_fY2 = i_Y2;
      m_eY2 = i_pointType;
      invalidate( EUpdateDataPixmapWidget , true );
}

//------------------------------------------------------------------------------
double CDiagObjLine::getY2( ECoordinatesUnit i_pointType ) const
//------------------------------------------------------------------------------
{
    if( i_pointType == ECoordinatesUnitAxisScale && m_eY2 == ECoordinatesUnitAxisScale)
        return( m_fY2 );
    else if( i_pointType == ECoordinatesUnitAxisScale && m_eY2 == ECoordinatesUnitPixel && m_arpDiagScale[static_cast<int>(EScaleAxis::Y)] )
        return( m_arpDiagScale[static_cast<int>(EScaleAxis::Y)]->getVal( m_fX2 ) );
    else if( i_pointType == ECoordinatesUnitPixel && m_eY2 == ECoordinatesUnitAxisScale && m_arpDiagScale[static_cast<int>(EScaleAxis::Y)] )
        return( m_arpDiagScale[static_cast<int>(EScaleAxis::Y)]->getValPix( m_fY2 ) );
    else if( i_pointType == ECoordinatesUnitPixel && m_eY2 == ECoordinatesUnitPixel && m_arpDiagScale[static_cast<int>(EScaleAxis::Y)] )
        return( m_fY2 );
    else
        return( 0.0 );
}

//------------------------------------------------------------------------------
CDiagObj* CDiagObjLine::clone( CDataDiagram* i_pDiagramTrg ) const
//------------------------------------------------------------------------------
{
    if( i_pDiagramTrg == nullptr )
    {
        return nullptr;
    }
    if( m_arpDiagScale[static_cast<int>(EScaleAxis::X)] == nullptr || m_arpDiagScale[static_cast<int>(EScaleAxis::Y)] == nullptr )
    {
        return nullptr;
    }

    CDiagScale* pDiagScaleX = i_pDiagramTrg->findDiagScale(m_arpDiagScale[static_cast<int>(EScaleAxis::X)]->getObjName());
    CDiagScale* pDiagScaleY = i_pDiagramTrg->findDiagScale(m_arpDiagScale[static_cast<int>(EScaleAxis::Y)]->getObjName());

    if( pDiagScaleX == nullptr || pDiagScaleY == nullptr )
    {
        return nullptr;
    }

    CDiagObjLine* pDiagObjCloned = new CDiagObjLine(
        /* strObjName  */ m_strObjName,
        /* pDiagScaleX */ pDiagScaleX,
        /* pDiagScaleY */ pDiagScaleY,
        /* SLineStyle */  m_pLineStyle );

    // Members from base class CDiagObj:
    pDiagObjCloned->m_layoutPos = m_layoutPos;
    pDiagObjCloned->m_rectContent = m_rectContent;
    pDiagObjCloned->m_bAdjustContentRect2DiagPartCenter = m_bAdjustContentRect2DiagPartCenter;
    pDiagObjCloned->m_bVisible = m_bVisible;
    pDiagObjCloned->m_state = m_state;
    pDiagObjCloned->m_bIsFocusable = m_bIsFocusable;
    pDiagObjCloned->m_bIsEditable = m_bIsEditable;

    // Members from this class:
    pDiagObjCloned->m_strObjName = m_strObjName;
    if( m_pLineStyle ) // wg. Lint
    {
        pDiagObjCloned->m_pLineStyle = new SLineStyle( *m_pLineStyle );
    }

    i_pDiagramTrg->addDiagObj(pDiagObjCloned);

    return pDiagObjCloned;

} // clone

//------------------------------------------------------------------------------
void CDiagObjLine::update( unsigned int i_uUpdateFlags , QPaintDevice* i_pPaintDevice )
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

    if( i_uUpdateFlags == EUpdateNone )
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

    // Layout processing is the task of the diagram:
    if( i_uUpdateFlags & EUpdateLayout && m_uUpdateFlags & EUpdateLayout )
    {
        validate(EUpdateLayout);
    }

    // If data processing is requested and necessary for this object ..
    if( i_uUpdateFlags & EUpdateData && m_uUpdateFlags & EUpdateData )
    {
        mthTracer.trace("Processing Data", ELogDetailLevel::Debug);

        if( m_arpDiagScale[static_cast<int>(EScaleAxis::X)] && m_arpDiagScale[static_cast<int>(EScaleAxis::Y)] )
        {
            if( m_eX1 == ECoordinatesUnitAxisScale )
                m_pt1.setX( m_arpDiagScale[static_cast<int>(EScaleAxis::X)]->getValPix( m_fX1 ) );
            else if( m_eX1 == ECoordinatesUnitPixel )
                m_pt1.setX( m_rectContent.left() + static_cast<int>(m_fX1) );
            if( m_eY1 == ECoordinatesUnitAxisScale )
                m_pt1.setY( m_arpDiagScale[static_cast<int>(EScaleAxis::Y)]->getValPix( m_fY1 ) );
            else if( m_eY1 == ECoordinatesUnitPixel )
                m_pt1.setY( m_rectContent.top() + static_cast<int>(m_fY1) );
            if( m_eX2 == ECoordinatesUnitAxisScale )
                m_pt2.setX( m_arpDiagScale[static_cast<int>(EScaleAxis::X)]->getValPix( m_fX2 ) );
            else if( m_eX2 == ECoordinatesUnitPixel )
                m_pt2.setX( m_rectContent.left() + static_cast<int>(m_fX2) );
            if( m_eY2 == ECoordinatesUnitAxisScale )
                m_pt2.setY( m_arpDiagScale[static_cast<int>(EScaleAxis::Y)]->getValPix( m_fY2 ) );
            else if( m_eY2 == ECoordinatesUnitPixel )
                m_pt2.setY( m_rectContent.top() + static_cast<int>(m_fY1) );
        }

        // Mark current process depth as executed (reset bit):
        validate(EUpdateData);
    }

    // If recalculation of bitmap is needed ..
    if( i_uUpdateFlags & EUpdatePixmap && m_uUpdateFlags & EUpdatePixmap && i_pPaintDevice != nullptr )
    {
        mthTracer.trace("Processing Pixmap", ELogDetailLevel::Debug);

        if( isVisible() )
        {
            //const CPixmapDiagram* pPixmapDiagram = nullptr;

            //// As a matter of fact there is no sense in adding a histogram object to
            //// a diagram just designed to analyze data.
            //if( m_pDataDiagram->getUpdateType() >= EDiagramUpdateTypePixmap )
            //{
            //  pPixmapDiagram = dynamic_cast<const CPixmapDiagram*>(m_pDataDiagram);
            //}
            //if( pPixmapDiagram != nullptr )
            //{
                QPainter painter( i_pPaintDevice );
                painter.setClipRect( m_rectContent );
                painter.setClipping( true );

                if( m_pLineStyle )
                {
                    m_pLineStyle->draw( &painter , m_pt1 , m_pt2 );
                }

                // Mark current process depth as executed (reset bit):
                validate(EUpdatePixmap);
            //}
        }
    }

    // If widget processing is requested and necessary for this object ..
    if( i_uUpdateFlags & EUpdateWidget && m_uUpdateFlags & EUpdateWidget )
    {
        mthTracer.trace("Processing Widget", ELogDetailLevel::Debug);

        // Invalidate output region of the diagram object to update (repaint) content of diagram.
        // The histogram is output in the center area of the diagram and the whole center area
        // need to be updated if the histogram has been changed.
        m_pDiagram->update(this, m_rectContent);

        // Mark current process depth as executed (reset bit):
        validate(EUpdateWidget);

    } // if( EUpdateWidget )

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

} // update
