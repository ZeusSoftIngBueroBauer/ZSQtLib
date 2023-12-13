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

#include "ZSDiagram/ZSDiagObjMarker.h"
#include "ZSDiagram/ZSDiagramProcData.h"
#include "ZSDiagram/ZSDiagramFrameStyles.h"
#include "ZSDiagram/ZSDiagramLabelStyles.h"
#include "ZSDiagram/ZSDiagramLineStyles.h"
#include "ZSDiagram/ZSDiagramImageStyles.h"
#include "ZSDiagram/ZSDiagramToolTipStyles.h"
#include "ZSDiagram/ZSDiagTrace.h"
#include "ZSPhysVal/ZSPhysVal.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
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
class CDiagObjMarker : public CDiagObj
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDiagObjMarker::CDiagObjMarker(
    const QString& i_strObjName,
    CDiagTrace*    i_pDiagTrace ) :
//------------------------------------------------------------------------------
    CDiagObj(
        /* strClassName */ CDiagObjMarker::ClassName(),
        /* strObjName   */ i_strObjName,
        /* pDiagTrace   */ i_pDiagTrace,
        /* layoutPos    */ ELayoutPos::Center ),
    // Previous states (before data processing in "update" method)
    m_bVisiblePrev(false),
    m_statePrev(EDiagObjStateNormal),
    // Calculated cursor position
    m_bCalculateCursorPos(true),
    m_scaleAxisCursorMove(EScaleAxis::X),
    m_arphysValPrev(CEnumScaleAxis::count()),
    m_arphysVal(CEnumScaleAxis::count()),
    m_ptPosPrev(),
    m_ptPos(),
    // Graphical elements of the marker:
    //m_arbShowElement[EDiagObjStateCount][EElementCount]
    // - value indication with vertical line
    //m_arpLineStyleVer[EDiagObjStateCount]
    m_rectLineVerPrev(),
    // - value indication with horizontal line
    //m_arpLineStyleHor[EDiagObjStateCount]
    m_rectLineHorPrev(),
    // - value indication with image
    //m_arpImageStyle[EDiagObjStateCount]
    m_rectImageCurr(),
    m_rectImagePrev(),
    // - marker label
    //m_arpLabelStyle[EDiagObjStateCount]
    //m_ariLabelAligmentFlags[EDiagObjStateCount]
    //m_ariLabelOffset_px[EDiagObjStateCount]
    m_rectLabelCurr(),
    m_rectLabelPrev(),
    // - value indication with tool tip
    //m_arpToolTipStyle[EDiagObjStateCount]
    m_arpValueFormatToolTip(CEnumScaleAxis::count(), nullptr),
    m_rectToolTipPrev(),
    m_rectToolTipArrowPrev(),
    // - indication of focus and editing cursor (StateNormal not used)
    //m_arpImageStyleCursor[EDiagObjStateCount]
    m_rectImageCursorCurr(),
    m_rectImageCursorPrev()
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    m_bIsFocusable = true;
    m_bIsEditable  = true;

    for( int idxScaleAxis = 0; idxScaleAxis < CEnumScaleAxis::count(); idxScaleAxis++ )
    {
        EScaleAxis scaleAxis = static_cast<EScaleAxis>(idxScaleAxis);
        m_arphysValPrev[idxScaleAxis].setUnit( i_pDiagTrace->getValuesUnit(scaleAxis) );
        m_arphysVal[idxScaleAxis].setUnit( i_pDiagTrace->getValuesUnit(scaleAxis) );
        m_arpValueFormatToolTip[idxScaleAxis] = nullptr;
    }
    for( int idxObjState = 0; idxObjState < EDiagObjStateCount; idxObjState++ )
    {
        for( int idxElement = 0; idxElement < EElementCount; idxElement++ )
        {
            m_arbShowElement[idxObjState][idxElement] = false;
        }
        m_arpLineStyleVer[idxObjState] = nullptr;
        m_arpLineStyleHor[idxObjState] = nullptr;
        m_arpImageStyle[idxObjState] = nullptr;
        m_arpLabelStyle[idxObjState] = nullptr;
        m_ariLabelAligmentFlags[idxObjState] = Qt::AlignTop|Qt::AlignLeft;
        m_ariLabelOffset_px[idxObjState] = 0;
        m_arpToolTipStyle[idxObjState] = nullptr;
        m_arpImageStyleCursor[idxObjState] = nullptr;
    }

} // ctor

//------------------------------------------------------------------------------
CDiagObjMarker::~CDiagObjMarker()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    //lint -e1506, -e1551 ... die ganze Zeit hat lint das nicht gestoert. Jetzt auf einmal schon ....
    setLineStyleVer(EDiagObjStateCount,nullptr);
    setLineStyleHor(EDiagObjStateCount,nullptr);
    setImageStyle(EDiagObjStateCount,nullptr);
    setLabelStyle(EDiagObjStateCount,nullptr);
    setToolTipStyle(EDiagObjStateCount,nullptr);
    setImageStyleCursor(EDiagObjStateCount,nullptr);
    //lint +e1506, +e1551

    for( int idxScaleAxis = 0; idxScaleAxis < CEnumScaleAxis::count(); idxScaleAxis++ )
    {
        try
        {
            delete m_arpValueFormatToolTip[idxScaleAxis];
        }
        catch(...)
        {
        }
        m_arpValueFormatToolTip[idxScaleAxis] = nullptr;
    }

    m_bVisiblePrev = false;
    m_statePrev = static_cast<EDiagObjState>(0);
    m_bCalculateCursorPos = false;
    m_scaleAxisCursorMove = static_cast<EScaleAxis>(0);
    //m_arphysValPrev.clear();
    //m_arphysVal.clear();
    //m_ptPosPrev;
    //m_ptPos;
    memset(m_arbShowElement, 0x00, EDiagObjStateCount*EElementCount*sizeof(m_arbShowElement[0][0]));
    memset(m_arpLineStyleVer, 0x00, EDiagObjStateCount*sizeof(m_arpLineStyleVer[0]));
    //m_rectLineVerPrev;
    memset(m_arpLineStyleHor, 0x00, EDiagObjStateCount*sizeof(m_arpLineStyleHor[0]));
    //m_rectLineHorPrev;
    memset(m_arpImageStyle, 0x00, EDiagObjStateCount*sizeof(m_arpImageStyle[0]));
    //m_rectImageCurr;
    //m_rectImagePrev;
    memset(m_arpLabelStyle, 0x00, EDiagObjStateCount*sizeof(m_arpLabelStyle[0]));
    memset(m_ariLabelAligmentFlags, 0x00, EDiagObjStateCount*sizeof(m_ariLabelAligmentFlags[0]));
    memset(m_ariLabelOffset_px, 0x00, EDiagObjStateCount*sizeof(m_ariLabelOffset_px[0]));
    //m_rectLabelCurr;
    //m_rectLabelPrev;
    memset(m_arpToolTipStyle, 0x00, EDiagObjStateCount*sizeof(m_arpToolTipStyle[0]));
    //m_arpValueFormatToolTip.clear();
    //m_rectToolTipPrev;
    //m_rectToolTipArrowPrev;
    memset(m_arpImageStyleCursor, 0x00, EDiagObjStateCount*sizeof(m_arpImageStyleCursor[0]));
    //m_rectImageCursorCurr;
    //m_rectImageCursorPrev;

} // dtor

/*==============================================================================
public: // overridables to get and set the cursor position
==============================================================================*/

//------------------------------------------------------------------------------
void CDiagObjMarker::setCalculateCursorPos( bool i_bCalculate )
//------------------------------------------------------------------------------
{
    m_bCalculateCursorPos = i_bCalculate;
}

//------------------------------------------------------------------------------
bool CDiagObjMarker::getCalculateCursorPos() const
//------------------------------------------------------------------------------
{
    return m_bCalculateCursorPos;
}

//------------------------------------------------------------------------------
void CDiagObjMarker::setCursorMoveDir( const CEnumScaleAxis& i_scaleAxis )
//------------------------------------------------------------------------------
{
    m_scaleAxisCursorMove = i_scaleAxis.enumerator();
}

//------------------------------------------------------------------------------
EScaleAxis CDiagObjMarker::getCursorMoveDir() const
//------------------------------------------------------------------------------
{
    return m_scaleAxisCursorMove;
}

//------------------------------------------------------------------------------
void CDiagObjMarker::setVal( const CEnumScaleAxis& i_scaleAxis, const CPhysVal& i_physVal )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::EnterLeave))
    {
        strTrcMsg += "ScaleAxis=" + i_scaleAxis.toString() + ", Val=" + i_physVal.toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setVal",
        /* strAddInfo   */ strTrcMsg );

    double fVal = i_physVal.getVal(m_arphysVal[i_scaleAxis.enumeratorAsInt()].unit());
    setVal(i_scaleAxis, fVal);
}

//------------------------------------------------------------------------------
void CDiagObjMarker::setVal( const CEnumScaleAxis& i_scaleAxis, double i_fVal, CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::EnterLeave))
    {
        strTrcMsg += "ScaleAxis=" + i_scaleAxis.toString() + ", Val=" + QString::number(i_fVal);
        if( i_pUnit != nullptr )
        {
            strTrcMsg += " [" + i_pUnit->symbol() + "]";
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setVal",
        /* strAddInfo   */ strTrcMsg );

    double fValOld;
    double fValNew = i_fVal;

    if( i_pUnit != nullptr && m_arphysVal[i_scaleAxis.enumeratorAsInt()].unit() != *i_pUnit )
    {
        fValNew = i_pUnit->convertValue(fValNew, m_arphysVal[i_scaleAxis.enumeratorAsInt()].unit());
    }

    // Store the new value.
    fValOld = m_arphysVal[i_scaleAxis.enumeratorAsInt()].getVal();
    m_arphysVal[i_scaleAxis.enumeratorAsInt()] = fValNew;

    // If position not changed ...
    if( fValNew == fValOld ) //lint !e777
    {
        return;
    }

    // If the marker calculates its cursor position ...
    if( m_bCalculateCursorPos )
    {
        // If the markers X position is changeable by the user ..
        if( m_scaleAxisCursorMove == EScaleAxis::X && i_scaleAxis == EScaleAxis::X )
        {
            // .. the X value of the cursor has been set.
            // The resulting Y value need to be recalculated.
            m_arphysVal[static_cast<int>(EScaleAxis::Y)].setValidity(EValueValidity::Invalid);
        }
        // If the markers Y position is changeable by the user ..
        else if( m_scaleAxisCursorMove == EScaleAxis::Y && i_scaleAxis == EScaleAxis::Y )
        {
            // .. the Y value of the cursor has been set.
            // The resulting X value need to be recalculated.
            m_arphysVal[static_cast<int>(EScaleAxis::X)].setValidity(EValueValidity::Invalid);
        }
    }

    // Need to completely recalculate the graphical output.
    invalidate(EUpdateDataPixmapWidget);

} // setVal

//------------------------------------------------------------------------------
QPoint CDiagObjMarker::getPos() const
//------------------------------------------------------------------------------
{
    return m_ptPos;
}

/*==============================================================================
public: // overridables to modify the marker style
==============================================================================*/

//------------------------------------------------------------------------------
void CDiagObjMarker::showElement( EDiagObjState i_diagObjState, EElement i_element )
//------------------------------------------------------------------------------
{
    if( i_element < EElementMin || i_element > EElementMax )
    {
        return;
    }

    bool bInvalidate = false;
    int  idxDiagObjStateMin = i_diagObjState;
    int  idxDiagObjStateMax = i_diagObjState;
    int  idxDiagObjState;

    if( i_diagObjState == EDiagObjStateCount )
    {
        idxDiagObjStateMin = EDiagObjStateMin;
        idxDiagObjStateMax = EDiagObjStateMax;
    }
    for( idxDiagObjState = idxDiagObjStateMin; idxDiagObjState <= idxDiagObjStateMax; idxDiagObjState++ )
    {
        if( !m_arbShowElement[idxDiagObjState][i_element] )
        {
            m_arbShowElement[idxDiagObjState][i_element] = true;
            bInvalidate = true;
        }
    }
    if( bInvalidate )
    {
        invalidate(EUpdateDataPixmapWidget);
    }

} // showElement

//------------------------------------------------------------------------------
void CDiagObjMarker::hideElement( EDiagObjState i_diagObjState, EElement i_element )
//------------------------------------------------------------------------------
{
    if( i_element < EElementMin || i_element > EElementMax )
    {
        return;
    }

    bool bInvalidate = false;
    int  idxDiagObjStateMin = i_diagObjState;
    int  idxDiagObjStateMax = i_diagObjState;
    int  idxDiagObjState;

    if( i_diagObjState == EDiagObjStateCount )
    {
        idxDiagObjStateMin = EDiagObjStateMin;
        idxDiagObjStateMax = EDiagObjStateMax;
    }
    for( idxDiagObjState = idxDiagObjStateMin; idxDiagObjState <= idxDiagObjStateMax; idxDiagObjState++ )
    {
        if( !m_arbShowElement[idxDiagObjState][i_element] )
        {
            m_arbShowElement[idxDiagObjState][i_element] = false;
            bInvalidate = true;
        }
    }
    if( bInvalidate )
    {
        invalidate(EUpdateDataPixmapWidget);
    }

} // hideElement

//------------------------------------------------------------------------------
bool CDiagObjMarker::isElementVisible( EDiagObjState i_diagObjState, EElement i_element ) const
//------------------------------------------------------------------------------
{
    if( i_diagObjState < EDiagObjStateMin || i_diagObjState > EDiagObjStateMax )
    {
        return false;
    }
    if( i_element < EElementMin || i_element > EElementMax )
    {
        return false;
    }
    return m_arbShowElement[i_diagObjState][i_element];
}

//------------------------------------------------------------------------------
void CDiagObjMarker::setLineStyleVer( EDiagObjState i_diagObjState, SLineStyle* i_pLineStyle )
//------------------------------------------------------------------------------
{
    bool bInvalidate = false;
    int  idxDiagObjState;
    int  idxDiagObjStateTmp;

    // If the styles for all object states should be set ...
    if( i_diagObjState == EDiagObjStateCount )
    {
        // .. delete all styles for all object states and take over new style.
        for( idxDiagObjState = 0; idxDiagObjState < EDiagObjStateCount; idxDiagObjState++ )
        {
            // If it is not the same (maybe just slightly modified) image style that should be set again ...
            if( m_arpLineStyleVer[idxDiagObjState] != i_pLineStyle )
            {
                // Check whether the style is also used by any other (following) object state ...
                for( idxDiagObjStateTmp = idxDiagObjState+1; idxDiagObjStateTmp < EDiagObjStateCount; idxDiagObjStateTmp++ )
                {
                    if( m_arpLineStyleVer[idxDiagObjStateTmp] == m_arpLineStyleVer[idxDiagObjState] )
                    {
                        m_arpLineStyleVer[idxDiagObjStateTmp] = nullptr;
                    }
                }
                delete m_arpLineStyleVer[idxDiagObjState];
                m_arpLineStyleVer[idxDiagObjState] = nullptr;
            }
            // Take over "new" style (the new style might be the same than the old one but slighty modified) ..
            m_arpLineStyleVer[idxDiagObjState] = i_pLineStyle;
        }
        bInvalidate = true;
    }

    // If the style for one specific object state should be set ...
    else if( i_diagObjState >= EDiagObjStateMin && i_diagObjState <= EDiagObjStateMax )
    {
        bool bStyleUsed = false;

        idxDiagObjState = i_diagObjState;

        // Check whether the style is also used by any other object state or
        // if the same (maybe just slightly modified) image style should be set ...
        for( idxDiagObjState = 0; idxDiagObjState < EDiagObjStateCount; idxDiagObjState++ )
        {
            // If it is the same (maybe just slightly modified) image style that should be set ...
            if( m_arpLineStyleVer[idxDiagObjState] == i_pLineStyle )
            {
                bStyleUsed = true;
                break;
            }
            // For other object states than the one to be modified ..
            else if( idxDiagObjState != i_diagObjState )
            {
                // If the image style to be replaced is used by other object states than the one to be modified ..
                if( m_arpLineStyleVer[idxDiagObjState] == m_arpLineStyleVer[i_diagObjState] )
                {
                    bStyleUsed = true;
                    break;
                }
            }
        }
        if( !bStyleUsed )
        {
            delete m_arpLineStyleVer[i_diagObjState];
        }
        // Take over "new" style (the new style might be the same than the old one but slighty modified) ..
        m_arpLineStyleVer[i_diagObjState] = i_pLineStyle;
        bInvalidate = true;
    }
    if( bInvalidate )
    {
        invalidate(EUpdateDataPixmapWidget);
    }

} // setLineStyleVer

//lint -e1961 ... kann eben nicht const sein, da der Aufrufer den Linientyp aendern koennen soll
//------------------------------------------------------------------------------
SLineStyle* CDiagObjMarker::getLineStyleVer( EDiagObjState i_diagObjState )
//------------------------------------------------------------------------------
{
    SLineStyle* pLineStyle = nullptr;

    if( i_diagObjState >= EDiagObjStateMin && i_diagObjState <= EDiagObjStateMax )
    {
        pLineStyle = m_arpLineStyleVer[i_diagObjState];
    }
    return pLineStyle;
}
//lint +e1962

//------------------------------------------------------------------------------
void CDiagObjMarker::setLineStyleHor( EDiagObjState i_diagObjState, SLineStyle* i_pLineStyle )
//------------------------------------------------------------------------------
{
    bool bInvalidate = false;
    int  idxDiagObjState;
    int  idxDiagObjStateTmp;

    // If the styles for all object states should be set ...
    if( i_diagObjState == EDiagObjStateCount )
    {
        // .. delete all styles for all object states and take over new style.
        for( idxDiagObjState = 0; idxDiagObjState < EDiagObjStateCount; idxDiagObjState++ )
        {
            // If it is not the same (maybe just slightly modified) image style that should be set again ...
            if( m_arpLineStyleHor[idxDiagObjState] != i_pLineStyle )
            {
                // Check whether the style is also used by any other (following) object state ...
                for( idxDiagObjStateTmp = idxDiagObjState+1; idxDiagObjStateTmp < EDiagObjStateCount; idxDiagObjStateTmp++ )
                {
                    if( m_arpLineStyleHor[idxDiagObjStateTmp] == m_arpLineStyleHor[idxDiagObjState] )
                    {
                        m_arpLineStyleHor[idxDiagObjStateTmp] = nullptr;
                    }
                }
                delete m_arpLineStyleHor[idxDiagObjState];
                m_arpLineStyleHor[idxDiagObjState] = nullptr;
            }
            // Take over "new" style (the new style might be the same than the old one but slighty modified) ..
            m_arpLineStyleHor[idxDiagObjState] = i_pLineStyle;
        }
        bInvalidate = true;
    }

    // If the style for one specific object state should be set ...
    else if( i_diagObjState >= EDiagObjStateMin && i_diagObjState <= EDiagObjStateMax )
    {
        bool bStyleUsed = false;

        idxDiagObjState = i_diagObjState;

        // Check whether the style is also used by any other object state or
        // if the same (maybe just slightly modified) image style should be set ...
        for( idxDiagObjState = 0; idxDiagObjState < EDiagObjStateCount; idxDiagObjState++ )
        {
            // If it is the same (maybe just slightly modified) image style that should be set ...
            if( m_arpLineStyleHor[idxDiagObjState] == i_pLineStyle )
            {
                bStyleUsed = true;
                break;
            }
            // For other object states than the one to be modified ..
            else if( idxDiagObjState != i_diagObjState )
            {
                // If the image style to be replaced is used by other object states than the one to be modified ..
                if( m_arpLineStyleHor[idxDiagObjState] == m_arpLineStyleHor[i_diagObjState] )
                {
                    bStyleUsed = true;
                    break;
                }
            }
        }
        if( !bStyleUsed )
        {
            delete m_arpLineStyleHor[i_diagObjState];
        }
        // Take over "new" style (the new style might be the same than the old one but slighty modified) ..
        m_arpLineStyleHor[i_diagObjState] = i_pLineStyle;
        bInvalidate = true;
    }
    if( bInvalidate )
    {
        invalidate(EUpdateDataPixmapWidget);
    }

} // setLineStyleHor

//lint -e1961 ... kann eben nicht const sein, da der Aufrufer den Linientyp aendern koennen soll
//------------------------------------------------------------------------------
SLineStyle* CDiagObjMarker::getLineStyleHor( EDiagObjState i_diagObjState )
//------------------------------------------------------------------------------
{
    SLineStyle* pLineStyle = nullptr;

    if( i_diagObjState >= EDiagObjStateMin && i_diagObjState <= EDiagObjStateMax )
    {
        pLineStyle = m_arpLineStyleHor[i_diagObjState];
    }
    return pLineStyle;
}
//lint +e1961

//------------------------------------------------------------------------------
void CDiagObjMarker::setImageStyle( EDiagObjState i_diagObjState, CImageStyle* i_pImageStyle )
//------------------------------------------------------------------------------
{
    bool bInvalidate = false;
    int  idxDiagObjState;
    int  idxDiagObjStateTmp;

    // If the styles for all object states should be set ...
    if( i_diagObjState == EDiagObjStateCount )
    {
        // .. delete all styles for all object states and take over new style.
        for( idxDiagObjState = 0; idxDiagObjState < EDiagObjStateCount; idxDiagObjState++ )
        {
            // If it is not the same (maybe just slightly modified) image style that should be set again ...
            if( m_arpImageStyle[idxDiagObjState] != i_pImageStyle )
            {
                // Check whether the style is also used by any other (following) object state ...
                for( idxDiagObjStateTmp = idxDiagObjState+1; idxDiagObjStateTmp < EDiagObjStateCount; idxDiagObjStateTmp++ )
                {
                    if( m_arpImageStyle[idxDiagObjStateTmp] == m_arpImageStyle[idxDiagObjState] )
                    {
                        m_arpImageStyle[idxDiagObjStateTmp] = nullptr;
                    }
                }
                delete m_arpImageStyle[idxDiagObjState];
                m_arpImageStyle[idxDiagObjState] = nullptr;
            }
            // Take over "new" style (the new style might be the same than the old one but slighty modified) ..
            m_arpImageStyle[idxDiagObjState] = i_pImageStyle;
        }
        bInvalidate = true;
    }

    // If the style for one specific object state should be set ...
    else if( i_diagObjState >= EDiagObjStateMin && i_diagObjState <= EDiagObjStateMax )
    {
        bool bStyleUsed = false;

        idxDiagObjState = i_diagObjState;

        // Check whether the style is also used by any other object state or
        // if the same (maybe just slightly modified) image style should be set ...
        for( idxDiagObjState = 0; idxDiagObjState < EDiagObjStateCount; idxDiagObjState++ )
        {
            // If it is the same (maybe just slightly modified) image style that should be set ...
            if( m_arpImageStyle[idxDiagObjState] == i_pImageStyle )
            {
                bStyleUsed = true;
                break;
            }
            // For other object states than the one to be modified ..
            else if( idxDiagObjState != i_diagObjState )
            {
                // If the image style to be replaced is used by other object states than the one to be modified ..
                if( m_arpImageStyle[idxDiagObjState] == m_arpImageStyle[i_diagObjState] )
                {
                    bStyleUsed = true;
                    break;
                }
            }
        }
        if( !bStyleUsed )
        {
            delete m_arpImageStyle[i_diagObjState];
        }
        // Take over "new" style (the new style might be the same than the old one but slighty modified) ..
        m_arpImageStyle[i_diagObjState] = i_pImageStyle;
        bInvalidate = true;
    }
    if( bInvalidate )
    {
        invalidate(EUpdateDataPixmapWidget);
    }

} // setImageStyle

//lint -e1961 ... kann eben nicht const sein, da der Aufrufer den ImageStyle aendern koennen soll
//------------------------------------------------------------------------------
CImageStyle* CDiagObjMarker::getImageStyle( EDiagObjState i_diagObjState )
//------------------------------------------------------------------------------
{
    CImageStyle* pImageStyle = nullptr;

    if( i_diagObjState >= EDiagObjStateMin && i_diagObjState <= EDiagObjStateMax )
    {
        pImageStyle = m_arpImageStyle[i_diagObjState];
    }
    return pImageStyle;
}
//lint +e196

//------------------------------------------------------------------------------
void CDiagObjMarker::setLabelStyle( EDiagObjState i_diagObjState, CLabelStyle* i_pLabelStyle, int i_iAlignmentFlags, int i_iOffset_px )
//------------------------------------------------------------------------------
{
    bool bInvalidate = false;
    int  idxDiagObjState;
    int  idxDiagObjStateTmp;

    // If the styles for all object states should be set ...
    if( i_diagObjState == EDiagObjStateCount )
    {
        // .. delete all styles for all object states and take over new style.
        for( idxDiagObjState = 0; idxDiagObjState < EDiagObjStateCount; idxDiagObjState++ )
        {
            // If it is not the same (maybe just slightly modified) image style that should be set again ...
            if( m_arpLabelStyle[idxDiagObjState] != i_pLabelStyle )
            {
                // Check whether the style is also used by any other (following) object state ...
                for( idxDiagObjStateTmp = idxDiagObjState+1; idxDiagObjStateTmp < EDiagObjStateCount; idxDiagObjStateTmp++ )
                {
                    if( m_arpLabelStyle[idxDiagObjStateTmp] == m_arpLabelStyle[idxDiagObjState] )
                    {
                        m_arpLabelStyle[idxDiagObjStateTmp] = nullptr;
                    }
                }
                delete m_arpLabelStyle[idxDiagObjState];
                m_arpLabelStyle[idxDiagObjState] = nullptr;
            }
            // Take over "new" style (the new style might be the same than the old one but slightly modified) ..
            m_arpLabelStyle[idxDiagObjState] = i_pLabelStyle;
            m_ariLabelAligmentFlags[idxDiagObjState] = i_iAlignmentFlags;
            m_ariLabelOffset_px[idxDiagObjState] = i_iOffset_px;
        }
        bInvalidate = true;
    }

    // If the style for one specific object state should be set ...
    else if( i_diagObjState >= EDiagObjStateMin && i_diagObjState <= EDiagObjStateMax )
    {
        bool bStyleUsed = false;

        idxDiagObjState = i_diagObjState;

        // Check whether the style is also used by any other object state or
        // if the same (maybe just slightly modified) image style should be set ...
        for( idxDiagObjState = 0; idxDiagObjState < EDiagObjStateCount; idxDiagObjState++ )
        {
            // If it is the same (maybe just slightly modified) image style that should be set ...
            if( m_arpLabelStyle[idxDiagObjState] == i_pLabelStyle )
            {
                bStyleUsed = true;
                break;
            }
            // For other object states than the one to be modified ..
            else if( idxDiagObjState != i_diagObjState )
            {
                // If the image style to be replaced is used by other object states than the one to be modified ..
                if( m_arpLabelStyle[idxDiagObjState] == m_arpLabelStyle[i_diagObjState] )
                {
                    bStyleUsed = true;
                    break;
                }
            }
        }
        if( !bStyleUsed )
        {
            delete m_arpLabelStyle[i_diagObjState];
        }
        // Take over "new" style (the new style might be the same than the old one but slighty modified) ..
        m_arpLabelStyle[i_diagObjState] = i_pLabelStyle;
        m_ariLabelAligmentFlags[i_diagObjState] = i_iAlignmentFlags;
        m_ariLabelOffset_px[i_diagObjState] = i_iOffset_px;
        bInvalidate = true;
    }
    if( bInvalidate )
    {
        invalidate(EUpdateDataPixmapWidget);
    }

} // setLabelStyle

//lint -e1961 ... kann eben nicht const sein, da der Aufrufer den Labeltyp aendern koennen soll
//------------------------------------------------------------------------------
CLabelStyle* CDiagObjMarker::getLabelStyle( EDiagObjState i_diagObjState )
//------------------------------------------------------------------------------
{
    CLabelStyle* pLabelStyle = nullptr;

    if( i_diagObjState >= EDiagObjStateMin && i_diagObjState <= EDiagObjStateMax )
    {
        pLabelStyle = m_arpLabelStyle[i_diagObjState];
    }
    return pLabelStyle;
}
//lint +e1962

//------------------------------------------------------------------------------
void CDiagObjMarker::setToolTipStyle( EDiagObjState i_diagObjState, CToolTipStyle* i_pToolTipStyle )
//------------------------------------------------------------------------------
{
    bool bInvalidate = false;
    int  idxDiagObjState;
    int  idxDiagObjStateTmp;

    // If the styles for all object states should be set ...
    if( i_diagObjState == EDiagObjStateCount )
    {
        // .. delete all styles for all object states and take over new style.
        for( idxDiagObjState = 0; idxDiagObjState < EDiagObjStateCount; idxDiagObjState++ )
        {
            // If it is not the same (maybe just slightly modified) image style that should be set again ...
            if( m_arpToolTipStyle[idxDiagObjState] != i_pToolTipStyle )
            {
                // Check whether the style is also used by any other (following) object state ...
                for( idxDiagObjStateTmp = idxDiagObjState+1; idxDiagObjStateTmp < EDiagObjStateCount; idxDiagObjStateTmp++ )
                {
                    if( m_arpToolTipStyle[idxDiagObjStateTmp] == m_arpToolTipStyle[idxDiagObjState] )
                    {
                        m_arpToolTipStyle[idxDiagObjStateTmp] = nullptr;
                    }
                }
                delete m_arpToolTipStyle[idxDiagObjState];
                m_arpToolTipStyle[idxDiagObjState] = nullptr;
            }
            // Take over "new" style (the new style might be the same than the old one but slighty modified) ..
            m_arpToolTipStyle[idxDiagObjState] = i_pToolTipStyle;
        }
        bInvalidate = true;
    }

    // If the style for one specific object state should be set ...
    else if( i_diagObjState >= EDiagObjStateMin && i_diagObjState <= EDiagObjStateMax )
    {
        bool bStyleUsed = false;

        idxDiagObjState = i_diagObjState;

        // Check whether the style is also used by any other object state or
        // if the same (maybe just slightly modified) image style should be set ...
        for( idxDiagObjState = 0; idxDiagObjState < EDiagObjStateCount; idxDiagObjState++ )
        {
            // If it is the same (maybe just slightly modified) image style that should be set ...
            if( m_arpToolTipStyle[idxDiagObjState] == i_pToolTipStyle )
            {
                bStyleUsed = true;
                break;
            }
            // For other object states than the one to be modified ..
            else if( idxDiagObjState != i_diagObjState )
            {
                // If the image style to be replaced is used by other object states than the one to be modified ..
                if( m_arpToolTipStyle[idxDiagObjState] == m_arpToolTipStyle[i_diagObjState] )
                {
                    bStyleUsed = true;
                    break;
                }
            }
        }
        if( !bStyleUsed )
        {
            delete m_arpToolTipStyle[i_diagObjState];
        }
        // Take over "new" style (the new style might be the same than the old one but slighty modified) ..
        m_arpToolTipStyle[i_diagObjState] = i_pToolTipStyle;
        bInvalidate = true;
    }
    if( bInvalidate )
    {
        invalidate(EUpdateDataPixmapWidget);
    }

} // setToolTipStyle

//lint -e1961 ... kann eben nicht const sein, da der Aufrufer den Tooltip aendern koennen soll
//------------------------------------------------------------------------------
CToolTipStyle* CDiagObjMarker::getToolTipStyle( EDiagObjState i_diagObjState )
//------------------------------------------------------------------------------
{
    CToolTipStyle* pToolTipStyle = nullptr;

    if( i_diagObjState >= EDiagObjStateMin && i_diagObjState <= EDiagObjStateMax )
    {
        pToolTipStyle = m_arpToolTipStyle[i_diagObjState];
    }
    return pToolTipStyle;
}
//lint +e1961

//------------------------------------------------------------------------------
void CDiagObjMarker::setToolTipValueFormat( const CEnumScaleAxis& i_scaleAxis, SValueFormatProvider* i_pValueFormat )
//------------------------------------------------------------------------------
{
    int iUsedCount = 0;
    int idxScaleAxis;

    for( idxScaleAxis = 0; idxScaleAxis < CEnumScaleAxis::count(); idxScaleAxis++ )
    {
        if( i_pValueFormat == m_arpValueFormatToolTip[idxScaleAxis] )
        {
            iUsedCount++;
        }
    }
    if( iUsedCount == 0 )
    {
        delete m_arpValueFormatToolTip[i_scaleAxis.enumeratorAsInt()];
    }
    m_arpValueFormatToolTip[i_scaleAxis.enumeratorAsInt()] = i_pValueFormat;

} // setToolTipValueFormat

//------------------------------------------------------------------------------
SValueFormatProvider* CDiagObjMarker::getToolTipValueFormat( const CEnumScaleAxis& i_scaleAxis )
//------------------------------------------------------------------------------
{
    return m_arpValueFormatToolTip[i_scaleAxis.enumeratorAsInt()];
}

//------------------------------------------------------------------------------
void CDiagObjMarker::setImageStyleCursor( EDiagObjState i_diagObjState, CImageStyle* i_pImageStyle )
//------------------------------------------------------------------------------
{
    bool bInvalidate = false;
    int  idxDiagObjState;
    int  idxDiagObjStateTmp;

    // If the styles for all object states should be set ...
    if( i_diagObjState == EDiagObjStateCount )
    {
        // .. delete all styles for all object states and take over new style.
        for( idxDiagObjState = 0; idxDiagObjState < EDiagObjStateCount; idxDiagObjState++ )
        {
            // If it is not the same (maybe just slightly modified) image style that should be set again ...
            if( m_arpImageStyleCursor[idxDiagObjState] != i_pImageStyle )
            {
                // Check whether the style is also used by any other (following) object state ...
                for( idxDiagObjStateTmp = idxDiagObjState+1; idxDiagObjStateTmp < EDiagObjStateCount; idxDiagObjStateTmp++ )
                {
                    if( m_arpImageStyleCursor[idxDiagObjStateTmp] == m_arpImageStyleCursor[idxDiagObjState] )
                    {
                        m_arpImageStyleCursor[idxDiagObjStateTmp] = nullptr;
                    }
                }
                delete m_arpImageStyleCursor[idxDiagObjState];
                m_arpImageStyleCursor[idxDiagObjState] = nullptr;
            }
            // Take over "new" style (the new style might be the same than the old one but slighty modified) ..
            m_arpImageStyleCursor[idxDiagObjState] = i_pImageStyle;
        }
        bInvalidate = true;
    }

    // If the style for one specific object state should be set ...
    else if( i_diagObjState >= EDiagObjStateMin && i_diagObjState <= EDiagObjStateMax )
    {
        bool bStyleUsed = false;

        idxDiagObjState = i_diagObjState;

        // Check whether the style is also used by any other object state or
        // if the same (maybe just slightly modified) image style should be set ...
        for( idxDiagObjState = 0; idxDiagObjState < EDiagObjStateCount; idxDiagObjState++ )
        {
            // If it is the same (maybe just slightly modified) image style that should be set ...
            if( m_arpImageStyleCursor[idxDiagObjState] == i_pImageStyle )
            {
                bStyleUsed = true;
                break;
            }
            // For other object states than the one to be modified ..
            else if( idxDiagObjState != i_diagObjState )
            {
                // If the image style to be replaced is used by other object states than the one to be modified ..
                if( m_arpImageStyleCursor[idxDiagObjState] == m_arpImageStyleCursor[i_diagObjState] )
                {
                    bStyleUsed = true;
                    break;
                }
            }
        }
        if( !bStyleUsed )
        {
            delete m_arpImageStyleCursor[i_diagObjState];
        }
        // Take over "new" style (the new style might be the same than the old one but slighty modified) ..
        m_arpImageStyleCursor[i_diagObjState] = i_pImageStyle;
        bInvalidate = true;
    }
    if( bInvalidate )
    {
        invalidate(EUpdateDataPixmapWidget);
    }

} // setImageStyleCursor

//lint -e1961 ... kann eben nicht const sein, da der Aufrufer den Imagetyp aendern koennen soll (langsam nervt lint)
//------------------------------------------------------------------------------
CImageStyle* CDiagObjMarker::getImageStyleCursor( EDiagObjState i_diagObjState )
//------------------------------------------------------------------------------
{
    CImageStyle* pImageStyle = nullptr;

    if( i_diagObjState >= EDiagObjStateMin && i_diagObjState <= EDiagObjStateMax )
    {
        pImageStyle = m_arpImageStyleCursor[i_diagObjState];
    }
    return pImageStyle;
}
//lint +e1962

/*==============================================================================
public: // overridables of base class CDiagObj
==============================================================================*/

//------------------------------------------------------------------------------
CPhysVal CDiagObjMarker::getVal( const CEnumScaleAxis& i_scaleAxis ) const
//------------------------------------------------------------------------------
{
    // !! On rounding the marker to the resolution the following need to be taken into account !!
    // The X or the Y value will be rounded to the value or scale resolution. On rounding
    // the value the resulting marker position could exceed the valid scale range. E.g. if
    // the X axis would range from 9.0 kHz to 3.0 GHz with a center rectangle width of
    // 200 pixels and 100 X/Y value pairs the resolution would be 10 MHz.
    //   round2LowerDecade( (3000000000.0 Hz - 9000.0 Hz) / 200 pixels ) = 10.0 MHz
    //   round2LowerDecade( (3000000000.0 Hz - 9000.0 Hz) / 100 values ) = 10.0 MHz
    // If the marker now would be moved to the left the markers position would be
    // rounded down to 0.0 MHz. And this value would be out of range !!

    CPhysVal physVal = m_arphysVal[i_scaleAxis.enumeratorAsInt()];

    if( m_pDiagTrace != nullptr && physVal.isValid() && !physVal.hasRes() )
    {
        physVal.setRes(m_pDiagTrace->getValuesRes(i_scaleAxis));
    }
    return physVal;
}

//------------------------------------------------------------------------------
bool CDiagObjMarker::isFocusable() const
//------------------------------------------------------------------------------
{
    bool bFocusable = false;

    if( isVisible() && m_bIsFocusable )
    {
        // The value changeable by the user must be valid (somehow set to any value) ...
        if( m_arphysVal[static_cast<int>(m_scaleAxisCursorMove)].isValid() )
        {
            bFocusable = true;
        }
    }
    return bFocusable;

} // isFocusable

//------------------------------------------------------------------------------
bool CDiagObjMarker::isEditable() const
//------------------------------------------------------------------------------
{
    bool bSelectable = false;

    if( isVisible() && m_bIsFocusable && m_bIsEditable )
    {
        // The value changeable by the user must be valid (somehow set to any value) ...
        if( m_arphysVal[static_cast<int>(m_scaleAxisCursorMove)].isValid() )
        {
            bSelectable = true;
        }
    }
    return bSelectable;

} // isEditable

//------------------------------------------------------------------------------
bool CDiagObjMarker::isHit( const QPoint& i_pt ) const
//------------------------------------------------------------------------------
{
    bool bIsHit = false;

    if( isVisible() && isFocusable() )
    {
        // If the position to be tested is within the content rectangle of the
        // marker (usually the diagrams center part) ...
        if( i_pt.x() >= m_rectContent.left() && i_pt.x() <= m_rectContent.right()
         && i_pt.y() >= m_rectContent.top() && i_pt.y() <= m_rectContent.bottom() )
        {
            const SLineStyle*  pLineStyleVer = nullptr;
            const SLineStyle*  pLineStyleHor = nullptr;
            const CImageStyle* pImageStyle   = nullptr;
            const CLabelStyle* pLabelStyle   = nullptr;
            EDiagObjState      diagObjState  = EDiagObjStateNormal;

            if( isEditing() )
            {
                diagObjState = EDiagObjStateEditing;
            }
            else if( isFocused() )
            {
                diagObjState = EDiagObjStateFocused;
            }

            if( m_arbShowElement[diagObjState][EElementLineVer] && m_arpLineStyleVer[diagObjState] != nullptr )
            {
                pLineStyleVer = m_arpLineStyleVer[diagObjState];
            }
            if( m_arbShowElement[diagObjState][EElementLineHor] && m_arpLineStyleHor[diagObjState] != nullptr )
            {
                pLineStyleHor = m_arpLineStyleHor[diagObjState];
            }
            if( m_arbShowElement[diagObjState][EElementImage] && m_arpImageStyle[diagObjState] != nullptr )
            {
                pImageStyle = m_arpImageStyle[diagObjState];
            }
            if( m_arbShowElement[diagObjState][EElementLabel] && m_arpLabelStyle[diagObjState] != nullptr )
            {
                pLabelStyle = m_arpLabelStyle[diagObjState];
            }

            // Check if the vertical line has been hit ...
            if( pLineStyleVer != nullptr )
            {
                if( i_pt.x() >= m_ptPos.x()-1 && i_pt.x() <= m_ptPos.x()+1 )
                {
                    bIsHit = true;
                }
            }
            // Check if the horizontal line has been hit ...
            if( !bIsHit && pLineStyleHor != nullptr )
            {
                if( i_pt.y() >= m_ptPos.y()-1 && i_pt.y() <= m_ptPos.y()+1 )
                {
                    bIsHit = true;
                }
            }
            // Check if the image has been hit ...
            if( !bIsHit && pImageStyle != nullptr )
            {
                if( i_pt.x() >= pImageStyle->boundingRect().left()
                 && i_pt.x() <= pImageStyle->boundingRect().right()
                 && i_pt.y() >= pImageStyle->boundingRect().top()
                 && i_pt.y() <= pImageStyle->boundingRect().bottom() )
                {
                    bIsHit = true;
                }
            }
            // Check if the label has been hit ...
            if( !bIsHit && pLabelStyle != nullptr ) // TODO: label alignment flags incl. offset ist noch zu beruecksichtigen
            {
                if( i_pt.x() >= pLabelStyle->boundingRect().left()
                 && i_pt.x() <= pLabelStyle->boundingRect().right()
                 && i_pt.y() >= pLabelStyle->boundingRect().top()
                 && i_pt.y() <= pLabelStyle->boundingRect().bottom() )
                {
                    bIsHit = true;
                }
            }
        }
    }
    return bIsHit;

} // isHit

//------------------------------------------------------------------------------
void CDiagObjMarker::moveEvent( CDiagObjMoveEvent* i_pEv, bool i_bInformDiagram )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "moveEvent",
        /* strAddInfo   */ "" );

    if( m_pDiagTrace == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotDefined);
    }

    if( i_pEv == nullptr )
    {
        return;
    }

    // On moving the marker to a X/Y pixel coordinate
    // the X/Y values need to be calculated.
    bool bCalculateXVal = false;
    bool bCalculateYVal = false;

    QPoint   pt         = m_ptPos;
    CUnit    unitScale = m_pDiagTrace->getScale(m_scaleAxisCursorMove).unit();
    double   fScaleMin  = m_pDiagTrace->getScale(m_scaleAxisCursorMove).minVal().getVal();
    double   fScaleMax  = m_pDiagTrace->getScale(m_scaleAxisCursorMove).maxVal().getVal();
    CPhysVal physVal    = m_arphysVal[static_cast<int>(m_scaleAxisCursorMove)];
    double   fValue;
    double   fRes;

    if( unitScale.isValid() )
    {
        fValue = physVal.getVal(unitScale);
    }
    else
    {
        fValue = physVal.getVal();
    }
    fRes = m_pDiagTrace->getValuesRes(m_scaleAxisCursorMove).getVal();

    // !! On moving the marker by the resolution the following need to be taken into account !!
    // The X or the Y value will be rounded to the value or scale resolution. On rounding
    // the value the resulting marker position could exceed the valid scale range. E.g. if
    // the X axis would range from 9.0 kHz to 3.0 GHz with a center rectangle width of
    // 200 pixels and 100 X/Y value pairs the resolution would be 10 MHz.
    //   round2LowerDecade( (3000000000.0 Hz - 9000.0 Hz) / 200 pixels ) = 10.0 MHz
    //   round2LowerDecade( (3000000000.0 Hz - 9000.0 Hz) / 100 values ) = 10.0 MHz
    // If the marker now would be moved to the left the markers position would be
    // rounded down to 0.0 MHz. And this value would be out of range !!

    switch( i_pEv->type() )
    {
        // Absolute to given X/Y pixel coordinates
        case CDiagObjMoveEvent::ETypePointAbs:
        {
            m_ptFocused = i_pEv->point();
            m_ptEditSession = i_pEv->point();
            pt = i_pEv->point();

            if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
            {
                strTrcMsg  = "Absolute to ";
                strTrcMsg += QString::number(pt.x()) + "/" + QString::number(pt.y()) + " px";
                mthTracer.trace(strTrcMsg);
            }
            if( m_scaleAxisCursorMove == EScaleAxis::X )
            {
                bCalculateXVal = true;
            }
            else if( m_scaleAxisCursorMove == EScaleAxis::Y )
            {
                bCalculateYVal = true;
            }
            break;
        }

        // Relative with given X/Y pixel coordinates
        case CDiagObjMoveEvent::ETypePointRel:
        {
            pt += i_pEv->point();

            if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
            {
                strTrcMsg  = "Relative by ";
                strTrcMsg += QString::number(i_pEv->point().x()) + "/" + QString::number(i_pEv->point().y()) + " px ";
                strTrcMsg += "to " + QString::number(pt.x()) + "/" + QString::number(pt.y()) + "px";
                mthTracer.trace(strTrcMsg);
            }
            if( m_scaleAxisCursorMove == EScaleAxis::X )
            {
                bCalculateXVal = true;
            }
            else if( m_scaleAxisCursorMove == EScaleAxis::Y )
            {
                bCalculateYVal = true;
            }
            break;
        }

        // Relative with whole number multiple of scale resolution
        case CDiagObjMoveEvent::ETypeResRel:
        {
            double fValRel;
            fValRel = i_pEv->val() * fRes;
            fValue += fValRel;

            if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
            {
                strTrcMsg  = QString::number(i_pEv->val());
                strTrcMsg += " times by " + QString::number(fRes) + " to " + QString::number(fValue);
                mthTracer.trace(strTrcMsg);
            }
            break;
        }

        // Absolute to given value
        case CDiagObjMoveEvent::ETypeValAbs:
        {
            fValue = i_pEv->val();

            if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
            {
                strTrcMsg  = "To ";
                strTrcMsg += QString::number(fValue);
                mthTracer.trace(strTrcMsg);
            }
            break;
        }

        // Relative with given value
        case CDiagObjMoveEvent::ETypeValRel:
        {
            double fValRel = i_pEv->val();

            if( m_pDiagTrace->getSpacing(m_scaleAxisCursorMove) == ESpacing::Logarithmic )
            {
                double fXValLog = log10(fValue);
                fXValLog += fValRel;
                fValue = pow(10.0,fXValLog);
            }
            else
            {
                fValue += fValRel;
            }
            if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
            {
                strTrcMsg  = "Relative by ";
                strTrcMsg += QString::number(fValRel) + " to " + QString::number(fValue);
                mthTracer.trace(strTrcMsg);
            }
            break;
        }
        default:
        {
            break;
        }
    }

    // If the marker was moved to a X/Y pixel coordinate ...
    if( bCalculateXVal || bCalculateYVal )
    {
        // ... the X/Y values need to be calculated.

        // First verify that the pixel coordinate is within the diagrams center part:
        if( pt.x() < m_rectContent.left() )
        {
            pt.setX(m_rectContent.left());
        }
        if( pt.x() > m_rectContent.right() )
        {
            pt.setX(m_rectContent.right());
        }
        if( pt.y() < m_rectContent.top() )
        {
            pt.setY(m_rectContent.top());
        }
        if( pt.y() > m_rectContent.bottom() )
        {
            pt.setY(m_rectContent.bottom());
        }
        if( bCalculateXVal )
        {
            fValue = m_pDiagTrace->getVal(m_scaleAxisCursorMove,static_cast<double>(pt.x()));
        }
        else if( bCalculateYVal )
        {
            fValue = m_pDiagTrace->getVal(m_scaleAxisCursorMove,static_cast<double>(pt.y()));
        }
        if( fValue < fScaleMin )
        {
            fValue = fScaleMin;
        }
        else if( fValue > fScaleMax )
        {
            fValue = fScaleMax;
        }
    }

    // If the marker was moved to a X/Y value ...
    else
    {
        fValue = Math::round2Resolution(fValue,fRes);
        if( fValue < fScaleMin )
        {
            fValue = fScaleMin;
        }
        else if( fValue > fScaleMax )
        {
            fValue = fScaleMax;
        }
    }

    // Store new position:
    m_arphysVal[static_cast<int>(m_scaleAxisCursorMove)].setVal(fValue);

    // If the marker calculates his cursor position ...
    if( m_bCalculateCursorPos )
    {
        // If the markers X position is changeable by the user ..
        if( m_scaleAxisCursorMove == EScaleAxis::X )
        {
            // .. the X value of the cursor will not be changed.
            // The resulting Y value need to be recalculated.
            m_arphysVal[static_cast<int>(EScaleAxis::Y)].setValidity(EValueValidity::Invalid);
        }
        // If the markers Y position is changeable by the user ..
        else if( m_scaleAxisCursorMove == EScaleAxis::Y )
        {
            // .. the Y value of the cursor will not be changed.
            // The resulting X value need to be recalculated.
            m_arphysVal[static_cast<int>(EScaleAxis::X)].setValidity(EValueValidity::Invalid);
        }
    }

    // Need to completely recalculate the graphical output.
    invalidate(EUpdateDataPixmapWidget,i_bInformDiagram);

} // moveEvent

/*==============================================================================
public: // must overridables of base class CDiagObj
==============================================================================*/

//------------------------------------------------------------------------------
CDiagObj* CDiagObjMarker::clone( CDataDiagram* i_pDiagramTrg ) const
//------------------------------------------------------------------------------
{
    if( i_pDiagramTrg == nullptr || m_pDiagTrace == nullptr )
    {
        return nullptr;
    }

    CDiagTrace* pDiagTrace = i_pDiagramTrg->findDiagTrace(m_pDiagTrace->getObjName());

    if( pDiagTrace == nullptr )
    {
        return nullptr;
    }

    CDiagObjMarker* pDiagObjCloned = new CDiagObjMarker(
        /* strObjName */ m_strObjName,
        /* pDiagTrace */ pDiagTrace );

    // Members from base class CDiagObj:
    pDiagObjCloned->m_layoutPos = m_layoutPos;
    pDiagObjCloned->m_rectContent = m_rectContent;
    pDiagObjCloned->m_bAdjustContentRect2DiagPartCenter = m_bAdjustContentRect2DiagPartCenter;
    pDiagObjCloned->m_bVisible = m_bVisible;
    pDiagObjCloned->m_state = m_state;
    pDiagObjCloned->m_bIsFocusable = m_bIsFocusable;
    pDiagObjCloned->m_bIsEditable = m_bIsEditable;

    // Members from this class:

    // Previous states (before data processing in "update" method)
    //pDiagObjCloned->m_bVisiblePrev = m_bVisiblePrev;
    //pDiagObjCloned->m_statePrev = m_statePrev;

    // Calculated cursor position
    pDiagObjCloned->m_bCalculateCursorPos = m_bCalculateCursorPos;
    pDiagObjCloned->m_scaleAxisCursorMove = m_scaleAxisCursorMove;
    for( int idxScaleAxis = 0; idxScaleAxis < CEnumScaleAxis::count(); idxScaleAxis++ )
    {
        //pDiagObjCloned->m_arphysValPrev[idxScaleAxis] = m_arphysVal[idxScaleAxis];
        pDiagObjCloned->m_arphysVal[idxScaleAxis] = m_arphysVal[idxScaleAxis];
    }
    //pDiagObjCloned->m_ptPosPrev = m_ptPos;
    //pDiagObjCloned->m_ptPos = m_ptPos;

    // Graphical elements of the marker:
    for( int idxObjState = 0; idxObjState < EDiagObjStateCount; idxObjState++ )
    {
        for( int idxElement = 0; idxElement < EElementCount; idxElement++ )
        {
            pDiagObjCloned->m_arbShowElement[idxObjState][idxElement] = m_arbShowElement[idxObjState][idxElement];
        }
    }

    // - value indication with vertical line
    for( int idxObjState = 0; idxObjState < EDiagObjStateCount; idxObjState++ )
    {
        if( m_arpLineStyleVer[idxObjState] != nullptr )
        {
            pDiagObjCloned->m_arpLineStyleVer[idxObjState] = new SLineStyle(*m_arpLineStyleVer[idxObjState]);
        }
    }
    //pDiagObjCloned->m_rectLineVerPrev = m_rectLineVerPrev;

    // - value indication with horizontal line
    for( int idxObjState = 0; idxObjState < EDiagObjStateCount; idxObjState++ )
    {
        if( m_arpLineStyleHor[idxObjState] != nullptr )
        {
            pDiagObjCloned->m_arpLineStyleHor[idxObjState] = new SLineStyle(*m_arpLineStyleHor[idxObjState]);
        }
    }
    //pDiagObjCloned->m_rectLineHorPrev = m_rectLineHorPrev;

    // - value indication with image
    for( int idxObjState = 0; idxObjState < EDiagObjStateCount; idxObjState++ )
    {
        if( m_arpImageStyle[idxObjState] != nullptr )
        {
            pDiagObjCloned->m_arpImageStyle[idxObjState] = m_arpImageStyle[idxObjState]->clone();
        }
    }
    //pDiagObjCloned->m_rectImageCurr = m_rectImageCurr;
    //pDiagObjCloned->m_rectImagePrev = m_rectImagePrev;

    // - marker label
    for( int idxObjState = 0; idxObjState < EDiagObjStateCount; idxObjState++ )
    {
        if( m_arpLabelStyle[idxObjState] != nullptr )
        {
            pDiagObjCloned->m_arpLabelStyle[idxObjState] = new CLabelStyle(*m_arpLabelStyle[idxObjState]);
        }
        pDiagObjCloned->m_ariLabelAligmentFlags[idxObjState] = m_ariLabelAligmentFlags[idxObjState];
        pDiagObjCloned->m_ariLabelOffset_px[idxObjState] = m_ariLabelOffset_px[idxObjState];
    }
    //pDiagObjCloned->m_rectLabelCurr = m_rectLabelCurr;
    //pDiagObjCloned->m_rectLabelPrev = m_rectLabelPrev;

    // - value indication with tool tip
    for( int idxObjState = 0; idxObjState < EDiagObjStateCount; idxObjState++ )
    {
        if( m_arpToolTipStyle[idxObjState] != nullptr )
        {
            pDiagObjCloned->m_arpToolTipStyle[idxObjState] = new CToolTipStyle(*m_arpToolTipStyle[idxObjState]);
        }
    }
    //pDiagObjCloned->m_fXValToolTip = m_fXValToolTip;
    //pDiagObjCloned->m_fYValToolTip = m_fYValToolTip;
    //pDiagObjCloned->m_physUnitXValToolTip = m_physUnitXValToolTip;
    //pDiagObjCloned->m_physUnitYValToolTip = m_physUnitYValToolTip;
    //pDiagObjCloned->m_rectToolTipPrev = m_rectToolTipPrev;
    //pDiagObjCloned->m_rectToolTipArrowPrev = m_rectToolTipArrowPrev;

    // - indication of focus and editing cursor (StateNormal not used)
    for( int idxObjState = 0; idxObjState < EDiagObjStateCount; idxObjState++ )
    {
        if( m_arpImageStyleCursor[idxObjState] != nullptr )
        {
            pDiagObjCloned->m_arpImageStyleCursor[idxObjState] = m_arpImageStyleCursor[idxObjState]->clone();
        }
    }
    //pDiagObjCloned->m_rectImageCursorCurr = m_rectImageCursorCurr;
    //pDiagObjCloned->m_rectImageCursorPrev = m_rectImageCursorPrev;

    i_pDiagramTrg->addDiagObj(pDiagObjCloned);

    return pDiagObjCloned;

} // clone

//------------------------------------------------------------------------------
void CDiagObjMarker::update( unsigned int i_uUpdateFlags, QPaintDevice* i_pPaintDevice )
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

    if( m_pDiagTrace == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotDefined);
    }
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
void CDiagObjMarker::updateLayout()
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateLayout",
        /* strAddInfo   */ "" );

    // Markers are always drawn in the center area of the diagram
    // and there is no need to update any internal data for the
    // sizeHint method.

    // Mark current process depth as executed (reset bit)
    //---------------------------------------------------

    validate(EUpdateLayout);

} // updateLayout

//------------------------------------------------------------------------------
void CDiagObjMarker::updateData()
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateData",
        /* strAddInfo   */ "" );

    // If the marker has been hidden data processing will not be executed.
    // So we need to emit the visibilityChanged signal at this point.
    // If the marker has been shown data processing will be executed and
    // the visibilityChanged signal will be emitted after data processing
    // has been executed as the receiver of the signal may very likely
    // query the new marker values.
    if( m_bVisiblePrev && !m_bVisible )
    {
        m_bVisiblePrev = m_bVisible;
        emit visibilityChanged(this);
    }

    if( m_pDiagram == nullptr || m_pDiagTrace == nullptr || !isVisible() )
    {
        return;
    }

    CUnit unitSrc = m_pDiagTrace->getScale(m_scaleAxisCursorMove).unit();
    CPhysVal* pPhysValSrc = &m_arphysVal[static_cast<int>(m_scaleAxisCursorMove)];
    if( pPhysValSrc->unit() != unitSrc ) {
        pPhysValSrc->convertValue(unitSrc);
    }

    EScaleAxis scaleAxisDst;
    if( m_scaleAxisCursorMove == EScaleAxis::X ) {
        scaleAxisDst = EScaleAxis::Y;
    }
    else {
        scaleAxisDst = EScaleAxis::X;
    }
    CUnit unitDst = m_pDiagTrace->getScale(scaleAxisDst).unit();
    CPhysVal* pPhysValDst = &m_arphysVal[static_cast<int>(scaleAxisDst)];
    if( pPhysValDst->unit() != unitDst ) {
        pPhysValDst->convertValue(unitDst);
    }

    EDiagObjState diagObjState = EDiagObjStateNormal;
    if( isEditing() ) {
        diagObjState = EDiagObjStateEditing;
    }
    else if( isFocused() ) {
        diagObjState = EDiagObjStateFocused;
    }

    const SLineStyle* pLineStyleVer = nullptr;
    if( m_arbShowElement[diagObjState][EElementLineVer] ) {
        pLineStyleVer = m_arpLineStyleVer[diagObjState];
    }

    const SLineStyle* pLineStyleHor = nullptr;
    if( m_arbShowElement[diagObjState][EElementLineHor] ) {
        pLineStyleHor = m_arpLineStyleHor[diagObjState];
    }

    CImageStyle* pImageStyle = nullptr;
    if( m_arbShowElement[diagObjState][EElementImage] ) {
        pImageStyle = m_arpImageStyle[diagObjState];
    }

    const CLabelStyle* pLabelStyle = nullptr;
    int iLabelAlignmentFlags = 0;
    int iLabelOffset_px = 0;
    if( m_arbShowElement[diagObjState][EElementLabel] ) {
        pLabelStyle = m_arpLabelStyle[diagObjState];
        iLabelAlignmentFlags = m_ariLabelAligmentFlags[diagObjState];
        iLabelOffset_px = m_ariLabelOffset_px[diagObjState];
    }

    CToolTipStyle* pToolTipStyle = nullptr;
    if( m_arbShowElement[diagObjState][EElementToolTip] ) {
        pToolTipStyle = m_arpToolTipStyle[diagObjState];
    }

    CImageStyle* pImageStyleCursor = nullptr;
    if( m_arbShowElement[diagObjState][EElementCursor] ) {
        pImageStyleCursor = m_arpImageStyleCursor[diagObjState];
    }

    // Calculate the resulting X and Y values
    //--------------------------------------------

    // If the marker is not visible ...
    if( !isVisible() )
    {
        m_rectImageCurr.setWidth(0);
        m_rectImageCurr.setHeight(0);
        m_rectLabelCurr.setWidth(0);
        m_rectLabelCurr.setHeight(0);
        m_rectImageCursorCurr.setWidth(0);
        m_rectImageCursorCurr.setHeight(0);
    }

    // If the marker is visible ...
    else
    {
        CPhysVal physValScaleMinSrc = m_pDiagTrace->getScale(m_scaleAxisCursorMove).minVal();
        CPhysVal physValScaleMaxSrc = m_pDiagTrace->getScale(m_scaleAxisCursorMove).maxVal();

        // Starting (initial) point of the markers cursor position
        // is at the left bottom corner of the diagram.
        if( !pPhysValSrc->isValid() ) {
            *pPhysValSrc = physValScaleMinSrc;
        }
        m_ptPos.setX(m_pDiagTrace->getScaleMinValPix(EScaleAxis::X));
        m_ptPos.setY(m_pDiagTrace->getScaleMinValPix(EScaleAxis::Y));

        // If the marker calculates his cursor position ...
        if( m_bCalculateCursorPos )
        {
            // The marker should always be visible (focusable and editable by the user).
            if( *pPhysValSrc < physValScaleMinSrc ) {
                *pPhysValSrc = physValScaleMinSrc;
            }
            else if( *pPhysValSrc > physValScaleMaxSrc ) {
                *pPhysValSrc = physValScaleMaxSrc;
            }

            double fValDst = 0.0;
            CUnit unit = pPhysValSrc->unit();
            EValueValidity valueValid = m_pDiagTrace->getVal(
                m_scaleAxisCursorMove, pPhysValSrc->getVal(), &pPhysValSrc->unit(),
                scaleAxisDst, &fValDst, &unitDst, false);
            if (valueValid == EValueValidity::Valid) {
                if( unitDst.isValid() ) {
                    pPhysValDst->setVal(fValDst, unitDst);
                }
                else {
                    pPhysValDst->setVal(fValDst);
                }
            }
            else {
                pPhysValDst->setValidity(EValueValidity::Invalid);
            }
        } // if( m_bCalculateCursorPos )

        // Don't set resolution at the X/Y coordinates. Otherwise the marker is not set on the
        // the curve line as the curve does not round to the resolution on drawing the value array.

        // Calculate the resulting point in the pixmap
        //--------------------------------------------

        int xPix = m_pDiagTrace->getScaleMinValPix(EScaleAxis::X);
        int yPix = m_pDiagTrace->getScaleMinValPix(EScaleAxis::Y);

        // If the markers X position is changeable by the user ..
        if( m_scaleAxisCursorMove == EScaleAxis::X )
        {
            CUnit unit = pPhysValSrc->unit();
            xPix = m_pDiagTrace->getValPix(EScaleAxis::X, pPhysValSrc->getVal(), &unit);

            // If the marker is positioned between two valid values ...
            if( pPhysValDst->isValid() ) {
                yPix = m_pDiagTrace->getValPix(EScaleAxis::Y, pPhysValDst->getVal(), &unitDst);
            }
            if( xPix < m_rectContent.left() ) {
                xPix = m_rectContent.left();
            }
            if( xPix > m_rectContent.right() ) {
                xPix = m_rectContent.right();
            }
            if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) ) {
                strTrcMsg = "X: " + pPhysValSrc->toString() + " (" + QString::number(xPix) + "px) / " +
                            "Y: " + pPhysValDst->toString() + " (" + QString::number(yPix) + "px)";
                mthTracer.trace(strTrcMsg);
            }
        }

        // If the markers Y position is changeable by the user ..
        else if( m_scaleAxisCursorMove == EScaleAxis::Y )
        {
            CUnit unitSrc = pPhysValSrc->unit();
            yPix = m_pDiagTrace->getValPix(EScaleAxis::Y, pPhysValSrc->getVal(), &unitSrc);

            // If the marker is positioned between two valid values ...
            if( pPhysValDst->isValid() ) {
                CUnit unitDst = pPhysValDst->unit();
                xPix = m_pDiagTrace->getValPix(EScaleAxis::X, pPhysValDst->getVal(), &unitDst);
            }
            if( yPix < m_rectContent.top() ) {
                yPix = m_rectContent.top();
            }
            if( yPix > m_rectContent.bottom() ) {
                yPix = m_rectContent.bottom();
            }
            if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) ) {
                strTrcMsg  = "X: " + pPhysValDst->toString() + " (" + QString::number(xPix) + " px) / " +
                             "Y="  + pPhysValSrc->toString() + " (" + QString::number(yPix) + " px)";
                mthTracer.trace(strTrcMsg);
            }
        }

        // Store resulting cursor position.
        m_ptPos.setX(xPix);
        m_ptPos.setY(yPix);

        // Calculate image rectangle
        //--------------------------

        if( pImageStyle != nullptr )
        {
            pImageStyle->setPos(m_ptPos);

            if( pPhysValSrc->isValid() && pPhysValDst->isValid() ) {
                m_rectImageCurr = pImageStyle->boundingRect();
                m_rectImageCurr.moveLeft(m_rectImageCurr.left()-1);
                m_rectImageCurr.moveTop(m_rectImageCurr.top()-1);
                m_rectImageCurr.setWidth(m_rectImageCurr.width()+2);
                m_rectImageCurr.setHeight(m_rectImageCurr.height()+2);
            }
            else {
                m_rectImageCurr.setWidth(0);
                m_rectImageCurr.setHeight(0);
            }
        }

        // Calculate label rectangle
        //--------------------------

        if( pLabelStyle != nullptr )
        {
            m_rectLabelCurr.setWidth(0);
            m_rectLabelCurr.setHeight(0);

            // Calculate new output region of the label.
            if( !pLabelStyle->getLabel().isEmpty() )
            {
                m_rectLabelCurr = pLabelStyle->boundingRect();

                m_rectLabelCurr.setWidth( m_rectLabelCurr.width()+2 );
                m_rectLabelCurr.setHeight( m_rectLabelCurr.height()+1 );

                if( pLineStyleVer != nullptr ) {
                    m_rectLabelCurr.moveLeft( m_ptPos.x() - m_rectLabelCurr.width()/2 - 1 ); //lint !e834 ... mein Gott Walter ..
                    m_rectLabelCurr.moveTop( m_pDiagTrace->getScaleMaxValPix(EScaleAxis::Y)+iLabelOffset_px );
                }
                else if( pLineStyleHor != nullptr ) {
                    m_rectLabelCurr.moveTop( m_ptPos.y() - m_rectLabelCurr.height()/2 - 1 ); //lint !e834 ... mein Gott Walter ..
                    m_rectLabelCurr.moveLeft( m_pDiagTrace->getScaleMinValPix(EScaleAxis::X)+iLabelOffset_px );
                }
                if( iLabelAlignmentFlags & Qt::AlignTop ) {
                    m_rectLabelCurr.moveTop( m_pDiagTrace->getScaleMaxValPix(EScaleAxis::Y)+iLabelOffset_px );
                }
                else if( iLabelAlignmentFlags & Qt::AlignBottom ) {
                    m_rectLabelCurr.moveBottom( m_pDiagTrace->getScaleMaxValPix(EScaleAxis::X)+iLabelOffset_px );
                }
                else if( iLabelAlignmentFlags & Qt::AlignVCenter ) {
                    m_rectLabelCurr.moveTop( m_ptPos.y() - m_rectLabelCurr.height()/2 + iLabelOffset_px );
                }
                if( iLabelAlignmentFlags & Qt::AlignLeft ) {
                    m_rectLabelCurr.moveLeft( m_pDiagTrace->getScaleMinValPix(EScaleAxis::X)+iLabelOffset_px );
                }
                else if( iLabelAlignmentFlags & Qt::AlignRight ) {
                    m_rectLabelCurr.moveRight( m_pDiagTrace->getScaleMaxValPix(EScaleAxis::X)+iLabelOffset_px );
                }
                else if( iLabelAlignmentFlags & Qt::AlignHCenter ) {
                    m_rectLabelCurr.moveLeft( m_ptPos.x() - m_rectLabelCurr.width()/2 + iLabelOffset_px );
                }
            }
        } // if( pLabelStyle != nullptr )

        // Calculate tool tip rectangle and content of tool tip
        //-----------------------------------------------------

        if( pToolTipStyle != nullptr )
        {
            QVector<QString> arstrVal(CEnumScaleAxis::count());
            QString strToolTip;
            QRect   rectToolTip;

            pToolTipStyle->setArrowHeadPos(m_ptPos);

            for( int idxScaleAxis = 0; idxScaleAxis < CEnumScaleAxis::count(); idxScaleAxis++ )
            {
                EScaleAxis scaleAxis = static_cast<EScaleAxis>(idxScaleAxis);
                CPhysVal physVal = m_arphysVal[idxScaleAxis];
                if (physVal.isValid()) {
                    physVal.setRes(getValRes(scaleAxis));
                }
                if( m_arpValueFormatToolTip[idxScaleAxis] == nullptr )
                {
                    arstrVal[idxScaleAxis] = physVal.toString(
                        /* unitFindVal          */ EUnitFind::Best,
                        /* iValSubStrVisibility */ PhysValSubStr::Val|PhysValSubStr::UnitSymbol,
                        /* unitFindRes          */ EUnitFind::None,
                        /* iResSubStrVisibility */ PhysValSubStr::None );
                }
                else
                {
                    arstrVal[idxScaleAxis] = physVal.toString(*m_arpValueFormatToolTip[idxScaleAxis]);
                }
            }
            strToolTip += arstrVal[static_cast<int>(EScaleAxis::X)] + " / " + arstrVal[static_cast<int>(EScaleAxis::Y)];

            pToolTipStyle->setToolTip(strToolTip);
            rectToolTip = pToolTipStyle->boundingRect();

            // If the tool tip text fits into the diagrams center area ...
            if( rectToolTip.width() < m_rectContent.width() && rectToolTip.height() < m_rectContent.height() )
            {
                // If there is enough space for the tool tip right of the cursor position ...
                if( m_ptPos.x() + rectToolTip.width() <= m_rectContent.right() )
                {
                    // If there is enough space for the tool tip above the cursor position ...
                    if( m_ptPos.y() - rectToolTip.height() >= m_rectContent.top() ) //lint !e834
                    {
                        pToolTipStyle->setToolTipPos(CToolTipStyle::EToolTipPosRightTop);
                        ensureToolTipIsVisible(pToolTipStyle);
                    }
                    // If there is not enough space for the tool tip above  the cursor position but
                    // if there is enough space for the tool tip below the cursor position ...
                    else if( m_ptPos.y() + rectToolTip.height() <= m_rectContent.bottom() )
                    {
                        pToolTipStyle->setToolTipPos(CToolTipStyle::EToolTipPosRightBottom);
                        ensureToolTipIsVisible(pToolTipStyle);
                    }
                    // If there is not enough space for the tool tip above the cursor position and
                    // if there is not enough space for the tool tip below the cursor position ...
                    else
                    {
                        // .. the diagram widget seems to be very small. The tool tip will
                        // be output vertically centered in this widget.
                        pToolTipStyle->setToolTipPos(CToolTipStyle::EToolTipPosRightCenter);
                        ensureToolTipIsVisible(pToolTipStyle);
                    }
                } // tool tip right of cursor

                // If there is not enough space for the tool tip right of the cursor position but
                // if there is enough space for the tool tip left of the cursor position ...
                else if( m_ptPos.x() - rectToolTip.width() >= m_rectContent.left() ) //lint !e834
                {
                    // If there is enough space for the tool tip above the cursor position ...
                    if( m_ptPos.y() - rectToolTip.height() >= m_rectContent.top() ) //lint !e834
                    {
                        pToolTipStyle->setToolTipPos(CToolTipStyle::EToolTipPosLeftTop);
                        ensureToolTipIsVisible(pToolTipStyle);
                    }
                    // If there is not enough space for the tool tip above the cursor position but
                    // if there is enough space for the tool tip below the cursor position ...
                    else if( m_ptPos.y() + rectToolTip.height() <= m_rectContent.bottom() )
                    {
                        pToolTipStyle->setToolTipPos(CToolTipStyle::EToolTipPosLeftBottom);
                        ensureToolTipIsVisible(pToolTipStyle);
                    }
                    // If there is not enough space for the tool tip above the cursor position and
                    // if there is not enough space for the tool tip below the cursor position ...
                    else
                    {
                        // .. the diagram widget seems to be very small. The tool tip will
                        // be output vertically centered in this widget.
                        pToolTipStyle->setToolTipPos(CToolTipStyle::EToolTipPosLeftCenter);
                        ensureToolTipIsVisible(pToolTipStyle);
                    }
                } // tool tip left of cursor

                // If there is not enough space for the tool tip right of the cursor position and
                // if there is not enough space for the tool tip left of the cursor position ...
                else
                {
                    // .. the diagram widget seems to be very small. The tool tip will
                    // be output horizontally centered in this widget.

                    // If there is enough space for the tool tip above the cursor position ...
                    if( m_ptPos.y() - rectToolTip.height() >= m_rectContent.top() ) //lint !e834
                    {
                        pToolTipStyle->setToolTipPos(CToolTipStyle::EToolTipPosTopCenter);
                        ensureToolTipIsVisible(pToolTipStyle);
                    }
                    // If there is not enough space for the tool tip above the cursor position but
                    // if there is enough space for the tool tip below the cursor position ...
                    else if( m_ptPos.y() + rectToolTip.height() <= m_rectContent.bottom() )
                    {
                        pToolTipStyle->setToolTipPos(CToolTipStyle::EToolTipPosBottomCenter);
                        ensureToolTipIsVisible(pToolTipStyle);
                    }
                    // If there is not enough space for the tool tip above the cursor position and
                    // if there is not enough space for the tool tip below the cursor position ...
                    else
                    {
                        // .. the diagram widget seems to be very small. The tool tip will
                        // be output vertically centered in this widget (but we dont show the arrow).
                        pToolTipStyle->setToolTipPos(CToolTipStyle::EToolTipPosCenterCenter);
                        ensureToolTipIsVisible(pToolTipStyle);
                    }
                } // not enough space either left nor right of cursor

                pToolTipStyle->updateFrameBorderPoints();

            } // If the tool tip text fits into the diagrams center area ...
        } // if( pToolTipStyle != nullptr )

        // Calculate rectangle for cursor indicating that the marker is focused or selected
        //---------------------------------------------------------------------------------

        // Calculate new output region of the cursor.
        xPix = m_ptPos.x();
        yPix = m_ptPos.y();

        if( pImageStyleCursor != nullptr )
        {
            // If the markers X position is changeable by the user ..
            if( m_scaleAxisCursorMove == EScaleAxis::X )
            {
                xPix = m_ptPos.x();
                if( m_bPtEditSessionValid ) {
                    yPix = m_ptEditSession.y();
                }
                else {
                    yPix = m_rectContent.top()+m_rectContent.height()/2;
                }
            }
            // If the markers Y position is changeable by the user ..
            else if( m_scaleAxisCursorMove == EScaleAxis::Y )
            {
                yPix = m_ptPos.y();
                if( m_bPtEditSessionValid ) {
                    xPix = m_ptEditSession.x();
                }
                else {
                    xPix = m_rectContent.left()+m_rectContent.width()/2;
                }
            }
            pImageStyleCursor->setPos( QPoint(xPix,yPix) );
            m_rectImageCursorCurr = pImageStyleCursor->boundingRect();
            m_rectImageCursorCurr.moveLeft(m_rectImageCursorCurr.left()-1);
            m_rectImageCursorCurr.moveTop(m_rectImageCursorCurr.top()-1);
            m_rectImageCursorCurr.setWidth(m_rectImageCursorCurr.width()+2);
            m_rectImageCursorCurr.setHeight(m_rectImageCursorCurr.height()+2);
        }
        else
        {
            m_rectImageCursorCurr.setWidth(0);
            m_rectImageCursorCurr.setHeight(0);
        }

        // Emit signals that the marker position has been changed.
        //--------------------------------------------------------

        // Please note that its important to set the previous values to the current values
        // BEFORE emitting the signals. Otherwise we could easily get into an endless loop if
        // e.g. "setVal" and "update" is called again while processing the "valueChanged" event
        // as this may again lead to emitting the "valueChanged" event and so on ...

        bool bValueChanged = true;

        for( int idxScaleAxis = 0; idxScaleAxis < CEnumScaleAxis::count(); idxScaleAxis++ )
        {
            if( m_arphysVal[idxScaleAxis] != m_arphysValPrev[idxScaleAxis] )
            {
                m_arphysValPrev[idxScaleAxis] = m_arphysVal[idxScaleAxis];
                emit valueChanged(static_cast<EScaleAxis>(idxScaleAxis),this);
                if( idxScaleAxis == static_cast<int>(EScaleAxis::X) ) {
                    emit valueXChanged(this);
                }
                else if( idxScaleAxis == static_cast<int>(EScaleAxis::Y) ) {
                    emit valueYChanged(this);
                }
                bValueChanged = true;
            }
        }
        if( bValueChanged )
        {
            emit valueChanged(this);
        }

        if( m_ptPosPrev != m_ptPos )
        {
            m_ptPosPrev = m_ptPos;
            emit posChanged(this);
        }
    } // if( isVisible() )

    // If the marker has been shown data processing will be executed and
    // the visibilityChanged signal will be emitted after data processing
    // has been executed as the receiver of the signal may very likely
    // query the new marker values.
    if( !m_bVisiblePrev && m_bVisible )
    {
        m_bVisiblePrev = m_bVisible;
        emit visibilityChanged(this);
    }

    // Mark current process depth as executed (reset bit)
    //---------------------------------------------------

    validate(EUpdateData);

} // updateData

//------------------------------------------------------------------------------
void CDiagObjMarker::updatePixmap( QPaintDevice* i_pPaintDevice )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updatePixmap",
        /* strAddInfo   */ "" );

    if( m_pDiagram == nullptr || m_pDiagTrace == nullptr || !isVisible() )
    {
        return;
    }

    const SLineStyle*  pLineStyleVer     = nullptr;
    const SLineStyle*  pLineStyleHor     = nullptr;
    const CLabelStyle* pLabelStyle       = nullptr;
    CImageStyle*       pImageStyle       = nullptr;
    CToolTipStyle*     pToolTipStyle     = nullptr;
    CImageStyle*       pImageStyleCursor = nullptr;
    EDiagObjState      diagObjState      = EDiagObjStateNormal;

    if( isEditing() )
    {
        diagObjState = EDiagObjStateEditing;
    }
    else if( isFocused() )
    {
        diagObjState = EDiagObjStateFocused;
    }

    if( m_arbShowElement[diagObjState][EElementLineVer] )
    {
        pLineStyleVer = m_arpLineStyleVer[diagObjState];
    }
    if( m_arbShowElement[diagObjState][EElementLineHor] )
    {
        pLineStyleHor = m_arpLineStyleHor[diagObjState];
    }
    if( m_arbShowElement[diagObjState][EElementImage] )
    {
        pImageStyle = m_arpImageStyle[diagObjState];
    }
    if( m_arbShowElement[diagObjState][EElementLabel] )
    {
        pLabelStyle = m_arpLabelStyle[diagObjState];
    }
    if( m_arbShowElement[diagObjState][EElementToolTip] )
    {
        pToolTipStyle = m_arpToolTipStyle[diagObjState];
    }
    if( m_arbShowElement[diagObjState][EElementCursor] )
    {
        pImageStyleCursor = m_arpImageStyleCursor[diagObjState];
    }

    if( isVisible() )
    {
        QPainter    painter(i_pPaintDevice);
        QPen        pen;
        QBrush      brush;

        painter.setClipRect(m_rectContent);
        painter.setClipping(true);

        // Show marker position
        //-----------------------

        if( pLineStyleVer != nullptr && m_arphysVal[static_cast<int>(EScaleAxis::X)].isValid() )
        {
            pen.setColor(pLineStyleVer->getCol());
            pen.setWidth(pLineStyleVer->getLineWidth());
            pen.setStyle(pLineStyleVer->getPenStyle());
            painter.setPen(pen);
            painter.drawLine(m_ptPos.x(),m_pDiagTrace->getScaleMinValPix(EScaleAxis::Y),m_ptPos.x(),m_pDiagTrace->getScaleMaxValPix(EScaleAxis::Y));
        }
        if( pLineStyleHor != nullptr && m_arphysVal[static_cast<int>(EScaleAxis::Y)].isValid() )
        {
            pen.setColor(pLineStyleHor->getCol());
            pen.setWidth(pLineStyleHor->getLineWidth());
            pen.setStyle(pLineStyleHor->getPenStyle());
            painter.setPen(pen);
            painter.drawLine(m_pDiagTrace->getScaleMinValPix(EScaleAxis::X),m_ptPos.y(),m_pDiagTrace->getScaleMaxValPix(EScaleAxis::X),m_ptPos.y());
        }

        // Show marker image
        //------------------

        if( pImageStyle != nullptr && m_arphysVal[static_cast<int>(EScaleAxis::X)].isValid() && m_arphysVal[static_cast<int>(EScaleAxis::Y)].isValid() )
        {
            pImageStyle->draw(&painter);
        }

        // Show marker label
        //------------------

        if( pLabelStyle != nullptr && !pLabelStyle->getLabel().isEmpty() )
        {
            pLabelStyle->draw(&painter,m_rectLabelCurr);
        }

        // Show X/Y value pair as tool tip
        //--------------------------------

        if( pToolTipStyle != nullptr )
        {
            pToolTipStyle->draw(&painter);
        }

        // Indicate that the marker is focused or selected
        //------------------------------------------------

        if( pImageStyleCursor != nullptr )
        {
            pImageStyleCursor->draw(&painter);
        }
    } // if( isVisible() )

    // Mark current process depth as executed (reset bit)
    //---------------------------------------------------

    validate(EUpdatePixmap);

} // updatePixmap

//------------------------------------------------------------------------------
void CDiagObjMarker::updateWidget()
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateWidget",
        /* strAddInfo   */ "" );

    if( m_pDiagram == nullptr || m_pDiagTrace == nullptr )
    {
        return;
    }

    const SLineStyle*  pLineStyleVer     = nullptr;
    const SLineStyle*  pLineStyleHor     = nullptr;
    const CLabelStyle* pLabelStyle       = nullptr;
    CImageStyle*       pImageStyle       = nullptr;
    CToolTipStyle*     pToolTipStyle     = nullptr;
    CImageStyle*       pImageStyleCursor = nullptr;
    EDiagObjState      diagObjState      = EDiagObjStateNormal;

    if( isEditing() )
    {
        diagObjState = EDiagObjStateEditing;
    }
    else if( isFocused() )
    {
        diagObjState = EDiagObjStateFocused;
    }

    if( m_arbShowElement[diagObjState][EElementLineVer] )
    {
        pLineStyleVer = m_arpLineStyleVer[diagObjState];
    }
    if( m_arbShowElement[diagObjState][EElementLineHor] )
    {
        pLineStyleHor = m_arpLineStyleHor[diagObjState];
    }
    if( m_arbShowElement[diagObjState][EElementImage] )
    {
        pImageStyle = m_arpImageStyle[diagObjState];
    }
    if( m_arbShowElement[diagObjState][EElementLabel] )
    {
        pLabelStyle = m_arpLabelStyle[diagObjState];
    }
    if( m_arbShowElement[diagObjState][EElementToolTip] )
    {
        pToolTipStyle = m_arpToolTipStyle[diagObjState];
    }
    if( m_arbShowElement[diagObjState][EElementCursor] )
    {
        pImageStyleCursor = m_arpImageStyleCursor[diagObjState];
    }

    // If the marker has been hide ..
    if( !isVisible() )
    {
        // Invalidate the PREVIOUS output region of the marker lines,
        // the PREVIOUS rectangle area of the tool tip and the PREVIOUS
        // rectangle area of the cursor.

        // Invalidate old output region of vertical line
        if( m_rectLineVerPrev.isValid() )
        {
            m_pDiagram->update(this, m_rectLineVerPrev);
        }
        m_rectLineVerPrev.setWidth(0);
        m_rectLineVerPrev.setHeight(0);

        // Invalidate old output region of horizontal line
        if( m_rectLineHorPrev.isValid() )
        {
            m_pDiagram->update(this, m_rectLineHorPrev);
        }
        m_rectLineHorPrev.setWidth(0);
        m_rectLineHorPrev.setHeight(0);

        // Invalidate old output region of the image.
        if( m_rectImagePrev.isValid() )
        {
            m_pDiagram->update(this, m_rectImagePrev);
        }
        m_rectImagePrev.setWidth(0);
        m_rectImagePrev.setHeight(0);

        // Invalidate old output region of the label.
        if( m_rectLabelPrev.isValid() )
        {
            m_pDiagram->update(this, m_rectLabelPrev);
        }
        m_rectLabelPrev.setWidth(0);
        m_rectLabelPrev.setHeight(0);

        // Invalidate old output region of the tool tip rectangle.
        if( m_rectToolTipPrev.isValid() )
        {
            m_pDiagram->update(this, m_rectToolTipPrev);
        }
        m_rectToolTipPrev.setWidth(0);
        m_rectToolTipPrev.setHeight(0);

        // Invalidate old output region of the tool tip arrow.
        if( m_rectToolTipArrowPrev.isValid() )
        {
            m_pDiagram->update(this, m_rectToolTipArrowPrev);
        }
        m_rectToolTipArrowPrev.setWidth(0);
        m_rectToolTipArrowPrev.setHeight(0);

        // Invalidate old output region of the focus and edit cursor.
        if( m_rectImageCursorPrev.isValid() )
        {
            m_pDiagram->update(this, m_rectImageCursorPrev);
        }
        m_rectImageCursorPrev.setWidth(0);
        m_rectImageCursorPrev.setHeight(0);
    }

    // If the marker is visible ..
    else
    {
        // Invalidate the PREVIOUS AND the CURRENT output region of the marker lines,
        // the PREVIOUS AND the CURRENT rectangle area of the tool tip and the PREVIOUS
        // AND the CURRENT rectangle area of the cursor.

        QRect rectPosValLineHor;
        QRect rectPosValLineVer;

        if( pLineStyleVer != nullptr )
        {
            rectPosValLineVer.setLeft(m_ptPos.x()-1);
            rectPosValLineVer.setTop(m_pDiagTrace->getScaleMaxValPix(EScaleAxis::Y));
            rectPosValLineVer.setWidth(3);
            rectPosValLineVer.setHeight(m_pDiagTrace->getScaleRangePix(EScaleAxis::Y));
        }
        if( pLineStyleHor != nullptr )
        {
            rectPosValLineHor.setLeft(m_pDiagTrace->getScaleMinValPix(EScaleAxis::X));
            rectPosValLineHor.setTop(m_ptPos.y()-1);
            rectPosValLineHor.setWidth(m_pDiagTrace->getScaleRangePix(EScaleAxis::X));
            rectPosValLineHor.setHeight(3);
        }

        // Invalidate previous and new output region of vertical line (if changed)
        if( m_rectLineVerPrev.isValid() && rectPosValLineVer != m_rectLineVerPrev )
        {
            m_pDiagram->update(this, m_rectLineVerPrev);
        }
        if( rectPosValLineVer.isValid() && rectPosValLineVer != m_rectLineVerPrev )
        {
            m_pDiagram->update(this, rectPosValLineVer);
            m_rectLineVerPrev = rectPosValLineVer;
        }

        // Invalidate new output region of horizontal line (if changed)
        if( m_rectLineHorPrev.isValid() && rectPosValLineHor != m_rectLineHorPrev )
        {
            m_pDiagram->update(this, m_rectLineHorPrev);
        }
        if( rectPosValLineHor.isValid() && rectPosValLineHor != m_rectLineHorPrev )
        {
            m_pDiagram->update(this, rectPosValLineHor);
            m_rectLineHorPrev = rectPosValLineHor;
        }

        // Invalidate new output region of the image (if changed)
        if( pImageStyle != nullptr )
        {
            if( m_rectImagePrev.isValid() && m_rectImageCurr != m_rectImagePrev )
            {
                m_pDiagram->update(this, m_rectImagePrev);
            }
            if( m_rectImageCurr.isValid() && m_rectImageCurr != m_rectImagePrev )
            {
                m_pDiagram->update(this, m_rectImageCurr);
                m_rectImagePrev = m_rectImageCurr;
            }
        }
        else
        {
            if( m_rectImagePrev.isValid() )
            {
                m_pDiagram->update(this, m_rectImagePrev);
            }
            m_rectImagePrev.setWidth(0);
            m_rectImagePrev.setHeight(0);
        }

        // Invalidate new output region of the label (if changed)
        if( pLabelStyle != nullptr )
        {
            if( m_rectLabelPrev.isValid() && m_rectLabelCurr != m_rectLabelPrev )
            {
                m_pDiagram->update(this, m_rectLabelPrev);
            }
            if( m_rectLabelCurr.isValid() && m_rectLabelCurr != m_rectLabelPrev )
            {
                m_pDiagram->update(this, m_rectLabelCurr);
                m_rectLabelPrev = m_rectLabelCurr;
            }
        }
        else
        {
            if( m_rectLabelPrev.isValid() )
            {
                m_pDiagram->update(this, m_rectLabelPrev);
            }
            m_rectLabelPrev.setWidth(0);
            m_rectLabelPrev.setHeight(0);
        }

        // Invalidate new output region of the tool tip (if changed)
        if( pToolTipStyle != nullptr )
        {
            // Invalidate new output region of the tool tip rectangle.
            if( m_rectToolTipPrev.isValid() && pToolTipStyle->getRectToolTip() != m_rectToolTipPrev )
            {
                m_pDiagram->update(this, m_rectToolTipPrev);
            }
            if( pToolTipStyle->getRectToolTip().isValid() && pToolTipStyle->getRectToolTip() != m_rectToolTipPrev )
            {
                m_pDiagram->update(this, pToolTipStyle->getRectToolTip());
                m_rectToolTipPrev = pToolTipStyle->getRectToolTip();
            }

            // Invalidate new output region of the tool tip arrow.
            if( pToolTipStyle->getRectArrow().isValid() && pToolTipStyle->getRectArrow() != m_rectToolTipArrowPrev )
            {
                m_pDiagram->update(this, pToolTipStyle->getRectArrow());
                m_rectToolTipArrowPrev = pToolTipStyle->getRectArrow();
            }
            if( m_rectToolTipArrowPrev.isValid() && pToolTipStyle->getRectArrow() != m_rectToolTipArrowPrev )
            {
                m_pDiagram->update(this, m_rectToolTipArrowPrev);
            }
        }
        else
        {
            if( m_rectToolTipPrev.isValid() )
            {
                m_pDiagram->update(this, m_rectToolTipPrev);
            }
            m_rectToolTipPrev.setWidth(0);
            m_rectToolTipPrev.setHeight(0);

            if( m_rectToolTipArrowPrev.isValid() )
            {
                m_pDiagram->update(this, m_rectToolTipArrowPrev);
            }
            m_rectToolTipArrowPrev.setWidth(0);
            m_rectToolTipArrowPrev.setHeight(0);
        }

        // Invalidate new output region of the focus and edit cursor (if changed)
        if( pImageStyleCursor != nullptr )
        {
            if( m_rectImageCursorPrev.isValid() && m_rectImageCursorCurr != m_rectImageCursorPrev )
            {
                m_pDiagram->update(this, m_rectImageCursorPrev);
            }
            if( m_rectImageCursorCurr.isValid() && m_rectImageCursorCurr != m_rectImageCursorPrev )
            {
                m_pDiagram->update(this, m_rectImageCursorCurr);
                m_rectImageCursorPrev = m_rectImageCursorCurr;
            }
        }
        else
        {
            if( m_rectImageCursorPrev.isValid() )
            {
                m_pDiagram->update(this, m_rectImageCursorPrev);
            }
            m_rectImageCursorPrev.setWidth(0);
            m_rectImageCursorPrev.setHeight(0);
        }

    } // if( isVisible() )

    // Mark current process depth as executed (reset bit)
    //---------------------------------------------------

    validate(EUpdateWidget);

} // updateWidget

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CDiagObjMarker::ensureToolTipIsVisible( CToolTipStyle* i_pToolTipStyle )
//------------------------------------------------------------------------------
{
    if( i_pToolTipStyle != nullptr )
    {
        QRect rectToolTip = i_pToolTipStyle->getRectToolTip();

        // If the tool tip rectangle is outside the visible area (out of top bounds) ...
        if( rectToolTip.top() < m_rectContent.top() )
        {
            rectToolTip.moveTop(m_rectContent.top());
            i_pToolTipStyle->setRectToolTip(rectToolTip);
        }
        // If the tool tip rectangle is outside the visible area (out of bottom bounds) ...
        else if( rectToolTip.bottom() > m_rectContent.bottom() )
        {
            rectToolTip.moveBottom(m_rectContent.bottom());
            i_pToolTipStyle->setRectToolTip(rectToolTip);
        }
    }

} // ensureToolTipIsVisible
