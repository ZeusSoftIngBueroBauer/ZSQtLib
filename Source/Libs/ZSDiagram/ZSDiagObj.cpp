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

#include <QtCore/qglobal.h>
#include <QtGui/qpainter.h>
#include <QtGui/qpixmap.h>

#if QT_VERSION < 0x050000
#include <QtGui/qapplication.h>
#else
#include <QtWidgets/qapplication.h>
#endif

#include "ZSDiagram/ZSDiagObj.h"
#include "ZSDiagram/ZSDiagramMsg.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMemLeakDump.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

using namespace ZS::System;
using namespace ZS::Diagram;
using namespace ZS::PhysVal;
using namespace ZS::Trace;


/*******************************************************************************
class ZSDIAGRAMDLL_API CDiagObjMoveEvent
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDiagObjMoveEvent::CDiagObjMoveEvent() :
//------------------------------------------------------------------------------
    m_type(ETypeUndefined),
    m_pt(),
    m_fVal(0.0),
    m_pUnit(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
CDiagObjMoveEvent::CDiagObjMoveEvent( EType i_type, const QPoint& i_pt ) :
//------------------------------------------------------------------------------
    m_type(i_type),
    m_pt(i_pt),
    m_fVal(i_pt.x()),
    m_pUnit(nullptr)
{
    if( m_type != ETypePointAbs && m_type != ETypePointRel )
    {
        m_type = ETypeUndefined;
    }

} // ctor

//------------------------------------------------------------------------------
CDiagObjMoveEvent::CDiagObjMoveEvent( EType i_type, double i_fVal, CUnit* i_pUnit ) :
//------------------------------------------------------------------------------
    m_type(i_type),
    m_pt(),
    m_fVal(i_fVal),
    m_pUnit(i_pUnit)
{
    if( m_type != ETypeResRel && m_type != ETypeValAbs && m_type != ETypeValRel )
    {
        m_type = ETypeUndefined;
    }

} // ctor

//------------------------------------------------------------------------------
CDiagObjMoveEvent::~CDiagObjMoveEvent()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDiagObjMoveEvent::setType( EType i_type )
//------------------------------------------------------------------------------
{
    m_type = i_type;
}

//------------------------------------------------------------------------------
CDiagObjMoveEvent::EType CDiagObjMoveEvent::type() const
//------------------------------------------------------------------------------
{
    return m_type;
}

//------------------------------------------------------------------------------
void CDiagObjMoveEvent::setPoint( const QPoint& i_pt )
//------------------------------------------------------------------------------
{
    m_pt = i_pt;
}

//------------------------------------------------------------------------------
QPoint CDiagObjMoveEvent::point() const
//------------------------------------------------------------------------------
{
    return m_pt;
}

//------------------------------------------------------------------------------
void CDiagObjMoveEvent::setVal( double i_fVal, CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    m_fVal = i_fVal;
    m_pUnit = i_pUnit;
}

//------------------------------------------------------------------------------
double CDiagObjMoveEvent::val( CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    double fVal = m_fVal;

    if( m_pUnit != nullptr && i_pUnit != nullptr )
    {
        fVal = m_pUnit->convertValue(m_fVal,i_pUnit);
    }
    return fVal;
}


/*******************************************************************************
class ZSDIAGRAMDLL_API CDiagObj
*******************************************************************************/

/*==============================================================================
protected: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
CDiagObj::CDiagObj(
    const QString& i_strObjName,
    CDiagTrace*    i_pDiagTrace,
    ELayoutPos     i_layoutPos ) :
//------------------------------------------------------------------------------
    QObject(),
    m_strObjName(i_strObjName),
    m_iObjId(-1),
    m_pDataDiagram(nullptr),
    m_pPixmapDiagram(nullptr),
    m_pWdgtDiagram(nullptr),
    m_pDiagTrace(i_pDiagTrace),
    //m_arpDiagScale[EScaleDirCount]
    m_uUpdateFlags(EUpdateAll),
    m_layoutPos(i_layoutPos),
    m_rectContent(),
    m_rectContentPrev(),
    m_bAdjustContentRect2DiagPartCenter(false),
    m_bVisible(true),
    m_state(EDiagObjStateNormal),
    m_bIsFocusable(false),
    m_bPtFocusedValid(false),
    m_ptFocused(),
    m_bIsEditable(false),
    m_bPtEditSessionValid(false),
    m_ptEditSession(),
    m_pDiagObjNext(nullptr),
    m_pDiagObjPrev(nullptr),
    m_pDiagObjPaintNext(nullptr),
    m_pDiagObjPaintPrev(nullptr),
    m_pTrcAdminObj(nullptr)
{
    m_arpDiagScale[EScaleDirX] = nullptr;
    m_arpDiagScale[EScaleDirY] = nullptr;

} // ctor

//------------------------------------------------------------------------------
CDiagObj::CDiagObj(
    const QString& i_strObjName,
    CDiagScale*    i_pDiagScaleX,
    CDiagScale*    i_pDiagScaleY,
    ELayoutPos     i_layoutPos ) :
//------------------------------------------------------------------------------
    QObject(),
    m_strObjName(i_strObjName),
    m_iObjId(-1),
    m_pDataDiagram(nullptr),
    m_pPixmapDiagram(nullptr),
    m_pWdgtDiagram(nullptr),
    m_pDiagTrace(nullptr),
    //m_arpDiagScale[EScaleDirCount]
    m_uUpdateFlags(EUpdateAll),
    m_layoutPos(i_layoutPos),
    m_rectContent(),
    m_rectContentPrev(),
    m_bAdjustContentRect2DiagPartCenter(false),
    m_bVisible(true),
    m_state(EDiagObjStateNormal),
    m_bIsFocusable(false),
    m_bPtFocusedValid(false),
    m_ptFocused(),
    m_bIsEditable(false),
    m_bPtEditSessionValid(false),
    m_ptEditSession(),
    m_pDiagObjNext(nullptr),
    m_pDiagObjPrev(nullptr),
    m_pDiagObjPaintNext(nullptr),
    m_pDiagObjPaintPrev(nullptr),
    m_pTrcAdminObj(nullptr)
{
    m_arpDiagScale[EScaleDirX] = i_pDiagScaleX;
    m_arpDiagScale[EScaleDirY] = i_pDiagScaleY;

} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDiagObj::~CDiagObj()
//------------------------------------------------------------------------------
{
    int idxScaleDir;

    m_pDataDiagram = nullptr;
    m_pPixmapDiagram = nullptr;
    m_pWdgtDiagram = nullptr;
    m_pDiagTrace = nullptr;

    for( idxScaleDir = 0; idxScaleDir < EScaleDirCount; idxScaleDir++ )
    {
        m_arpDiagScale[idxScaleDir] = nullptr;
    }

    m_pDiagObjNext = nullptr;
    m_pDiagObjPrev = nullptr;
    m_pDiagObjPaintNext = nullptr;
    m_pDiagObjPaintPrev = nullptr;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDiagObj::setObjName( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    m_strObjName = i_strObjName;
}

//------------------------------------------------------------------------------
QString CDiagObj::getObjName() const
//------------------------------------------------------------------------------
{
    return m_strObjName;
}

//------------------------------------------------------------------------------
int CDiagObj::getObjId() const
//------------------------------------------------------------------------------
{
    return m_iObjId;
}

//------------------------------------------------------------------------------
CDataDiagram* CDiagObj::getDiagram()
//------------------------------------------------------------------------------
{
    return m_pDataDiagram;
}

//------------------------------------------------------------------------------
CDiagScale* CDiagObj::getDiagScale( EScaleDir i_scaleDir )
//------------------------------------------------------------------------------
{
    CDiagScale* pDiagScale = m_arpDiagScale[i_scaleDir];

    if( pDiagScale == nullptr && m_pDiagTrace != nullptr )
    {
        pDiagScale = m_pDiagTrace->getDiagScale(i_scaleDir);
    }
    return pDiagScale;
}

//------------------------------------------------------------------------------
CDiagTrace* CDiagObj::getDiagTrace()
//------------------------------------------------------------------------------
{
    return m_pDiagTrace;
}

//------------------------------------------------------------------------------
ELayoutPos CDiagObj::getLayoutPos() const
//------------------------------------------------------------------------------
{
    return m_layoutPos;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CDiagObj::setDiagTrace( CDiagTrace* i_pDiagTrace )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "setDiagTrace",
        /* strAddInfo   */ "" );

    if( mthTracer.isActive(c_iTrcDetailLevel0) )
    {
        QString strTrcMsg;

        strTrcMsg  = "CurrUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags) + ", ";
        if( m_pDiagTrace == nullptr )
        {
            strTrcMsg += "PrevTrace=nullptr, ";
        }
        else
        {
            strTrcMsg += "PrevTrace=" + m_pDiagTrace->getObjName() + ", ";
        }
        if( i_pDiagTrace == nullptr )
        {
            strTrcMsg += "NewTrace=nullptr";
        }
        else
        {
            strTrcMsg += "NewTrace=" + i_pDiagTrace->getObjName();
        }
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevel0);
    }

    m_pDiagTrace = i_pDiagTrace;

    invalidate(EUpdateDataPixmapWidget,true);

} // setDiagTrace

//------------------------------------------------------------------------------
void CDiagObj::setAdjustContentRect2DiagPartCenter( bool i_bAdjust )
//------------------------------------------------------------------------------
{
    m_bAdjustContentRect2DiagPartCenter = i_bAdjust;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
bool CDiagObj::getAdjustContentRect2DiagPartCenter() const
//------------------------------------------------------------------------------
{
    return m_bAdjustContentRect2DiagPartCenter;
}

//------------------------------------------------------------------------------
void CDiagObj::show( bool i_bInformDiagram )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "show",
        /* strAddInfo   */ "" );

    if( !m_bVisible )
    {
        // The object must be added to the diagram content. As long as the
        // object was hidden the underlying data could have been changed
        // and the data might need to be recalculated in advance.
        // On changing the visibility state of a diagram object the layout
        // of the diagram may need to be rearranged.
        // On invalidating a process depth all "higher" update functions also need to
        // be processed by the diagram (but not necessarily by all diagram objects).
        // The diagram may need to change the position and sizes of the objects.
        if( m_layoutPos == ELayoutPosCenter || m_layoutPos == ELayoutPosUndefined )
        {
            invalidate(EUpdateDataPixmapWidget,i_bInformDiagram);
        }
        else
        {
            invalidate(EUpdateLayoutDataPixmapWidget,i_bInformDiagram);
        }
    }
    m_bVisible = true;

} // show

//------------------------------------------------------------------------------
void CDiagObj::hide( bool i_bInformDiagram )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "hide",
        /* strAddInfo   */ "" );

    if( m_bVisible )
    {
        // The object must be removed from the diagram content and for this
        // (as default) the pixmap must be newly created.
        // On changing the visibility state of a diagram object the layout
        // of the diagram may need to be rearranged.
        // On invalidating a process depth all "higher" update functions also need to
        // be processed by the diagram (but not necessarily by all diagram objects).
        // The diagram may need to change the position and sizes of the objects.
        if( m_layoutPos == ELayoutPosCenter || m_layoutPos == ELayoutPosUndefined )
        {
            invalidate(EUpdateDataPixmapWidget,i_bInformDiagram);
        }
        else
        {
            invalidate(EUpdateLayoutDataPixmapWidget,i_bInformDiagram);
        }
    }
    m_bVisible = false;

} // hide

//------------------------------------------------------------------------------
bool CDiagObj::isVisible() const
//------------------------------------------------------------------------------
{
    return m_bVisible;
}

//------------------------------------------------------------------------------
double CDiagObj::getValRes( EScaleDir i_scaleDir, CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    if( i_scaleDir < EScaleDirMin || i_scaleDir > EScaleDirMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }

    double fRes = 0.0;

    if( m_pDiagTrace != nullptr )
    {
        fRes = m_pDiagTrace->getValRes(i_scaleDir,i_pUnit);
    }
    else if( m_arpDiagScale[i_scaleDir] != nullptr )
    {
        fRes = m_arpDiagScale[i_scaleDir]->getScaleRes(i_pUnit);
    }
    return fRes;

} // getValRes

//------------------------------------------------------------------------------
double CDiagObj::getValRes( EScaleDir i_scaleDir, double i_fVal, CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    if( i_scaleDir < EScaleDirMin || i_scaleDir > EScaleDirMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }

    double fRes = 0.0;

    if( m_pDiagTrace != nullptr )
    {
        fRes = m_pDiagTrace->getValRes(i_scaleDir,i_fVal,i_pUnit);
    }
    else if( m_arpDiagScale[i_scaleDir] != nullptr )
    {
        fRes = m_arpDiagScale[i_scaleDir]->getScaleRes(i_fVal,i_pUnit);
    }
    return fRes;

} // getValRes

//------------------------------------------------------------------------------
CPhysVal CDiagObj::getVal( EScaleDir /*i_scaleDir*/ ) const
//------------------------------------------------------------------------------
{
    return CPhysVal();
}

//------------------------------------------------------------------------------
void CDiagObj::setIsFocusable( bool i_bFocusable )
//------------------------------------------------------------------------------
{
    m_bIsFocusable = i_bFocusable;
}

//------------------------------------------------------------------------------
bool CDiagObj::isFocusable() const
//------------------------------------------------------------------------------
{
    return m_bIsFocusable;
}

//------------------------------------------------------------------------------
bool CDiagObj::isFocused() const
//------------------------------------------------------------------------------
{
    bool bIsFocused = false;

    if( m_state == EDiagObjStateFocused || m_state == EDiagObjStateEditing )
    {
        bIsFocused = true;
    }
    return bIsFocused;
}

//------------------------------------------------------------------------------
void CDiagObj::setFocus( bool i_bInformDiagram )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelEvents,
        /* strMethod    */ "setFocus",
        /* strAddInfo   */ "" );

    if( isFocusable() && !isFocused() )
    {
        // As default only data processing is needed on changing the focus
        // state to indicate the changed focus state.
        // On invalidating a process depth all "higher" update functions also need to
        // be processed by the diagram (but not necessarily by the diagram objects).
        invalidate(EUpdateDataPixmapWidget,i_bInformDiagram);

        if( m_state == EDiagObjStateNormal )
        {
            m_state = EDiagObjStateFocused;
        }
        m_bPtFocusedValid = false;
    }

} // setFocus

//------------------------------------------------------------------------------
void CDiagObj::setFocus( const QPoint& i_pt, bool i_bInformDiagram )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelEvents,
        /* strMethod    */ "setFocus",
        /* strAddInfo   */ "" );

    if( isFocusable() && !isFocused() )
    {
        // As default data processing is needed on changing the focus
        // state to indicate the changed focus state.
        // On invalidating a process depth all "higher" update functions also need to
        // be processed by the diagram (but not necessarily by the diagram objects).
        invalidate(EUpdateDataPixmapWidget,i_bInformDiagram);

        if( m_state == EDiagObjStateNormal )
        {
            m_state = EDiagObjStateFocused;
        }
        m_bPtFocusedValid = true;
        m_ptFocused = i_pt;
    }

} // setFocus

//------------------------------------------------------------------------------
void CDiagObj::unsetFocus( bool i_bInformDiagram )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelEvents,
        /* strMethod    */ "unsetFocus",
        /* strAddInfo   */ "" );

    // On removing the focus from an edited object also the edit session will
    // be terminated:
    stopEditSession(i_bInformDiagram);

    if( isFocused() )
    {
        // As default data processing is needed on changing the focus
        // state to indicate the changed focus state.
        // On invalidating a process depth all "higher" update functions also need to
        // be processed by the diagram (but not necessarily by the diagram objects).
        invalidate(EUpdateDataPixmapWidget,i_bInformDiagram);
    }

    m_state = EDiagObjStateNormal;
    m_bPtFocusedValid = false;

} // unsetFocus

//------------------------------------------------------------------------------
void CDiagObj::setIsEditable( bool i_bEditable )
//------------------------------------------------------------------------------
{
    m_bIsEditable = i_bEditable;
}

//------------------------------------------------------------------------------
bool CDiagObj::isEditable() const
//------------------------------------------------------------------------------
{
    return m_bIsFocusable && m_bIsEditable;
}

//------------------------------------------------------------------------------
bool CDiagObj::isEditing() const
//------------------------------------------------------------------------------
{
    return m_state == EDiagObjStateEditing;
}

//------------------------------------------------------------------------------
void CDiagObj::startEditSession( bool i_bInformDiagram )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelEvents,
        /* strMethod    */ "startEditSession",
        /* strAddInfo   */ "" );

    if( isEditable() )
    {
        if( !isFocused() || !isEditing() )
        {
            // As default data processing is needed on changing the editing
            // state to indicate the changed state.
            // On invalidating a process depth all "higher" update functions also need to
            // be processed by the diagram (but not necessarily by the diagram objects).
            invalidate(EUpdateDataPixmapWidget,i_bInformDiagram);
        }
        m_state = EDiagObjStateEditing;
        m_bPtFocusedValid = false;
        m_bPtEditSessionValid = false;
    }

} // startEditSession

//------------------------------------------------------------------------------
void CDiagObj::startEditSession( const QPoint& i_pt, bool i_bInformDiagram )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelEvents,
        /* strMethod    */ "startEditSession",
        /* strAddInfo   */ "" );

    if( isEditable() )
    {
        if( !isFocused() || !isEditing() )
        {
            // As default data processing is needed on changing the editing
            // state to indicate the changed state.
            // On invalidating a process depth all "higher" update functions also need to
            // be processed by the diagram (but not necessarily by the diagram objects).
            invalidate(EUpdateDataPixmapWidget,i_bInformDiagram);
        }
        m_state = EDiagObjStateEditing;
        m_bPtFocusedValid = true;
        m_ptFocused = i_pt;
        m_bPtEditSessionValid = true;
        m_ptEditSession = i_pt;
    }

} // startEditSession

//------------------------------------------------------------------------------
void CDiagObj::stopEditSession( bool i_bInformDiagram )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelEvents,
        /* strMethod    */ "stopEditSession",
        /* strAddInfo   */ "" );

    if( isEditing() )
    {
        // As default data processing is needed on changing the editing
        // state to indicate the changed state.
        // On invalidating a process depth all higher" update functions also need to
        // be processed by the diagram (but not necessarily by the diagram objects).
        invalidate(EUpdateDataPixmapWidget,i_bInformDiagram);
    }
    m_state = EDiagObjStateFocused;
    m_bPtEditSessionValid = false;

} // stopEditSession

//------------------------------------------------------------------------------
bool CDiagObj::isHit( const QPoint& /*i_pt*/ ) const
//------------------------------------------------------------------------------
{
    return false;
}

//lint -e1961
//------------------------------------------------------------------------------
void CDiagObj::moveEvent( CDiagObjMoveEvent* /*i_pEv*/, bool /*i_bInformDiagram*/ )
//------------------------------------------------------------------------------
{
}
//lint +e1961

//------------------------------------------------------------------------------
QRect CDiagObj::geometry() const
//------------------------------------------------------------------------------
{
    return m_rectContent;
}

//------------------------------------------------------------------------------
void CDiagObj::setGeometry( const QRect& i_rect, bool i_bInformDiagram )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevelLayout) )
    {
        strTrcMsg  = "Rect[x,y,w,h]=";
        strTrcMsg += QString::number(i_rect.x()) + ",";
        strTrcMsg += QString::number(i_rect.y()) + ",";
        strTrcMsg += QString::number(i_rect.width()) + ",";
        strTrcMsg += QString::number(i_rect.height()) + ", ";
        strTrcMsg += "InformDiagram=" + ZS::System::bool2Str(i_bInformDiagram);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelLayout,
        /* strMethod    */ "setGeometry",
        /* strAddInfo   */ strTrcMsg );

    QRect rectContent = i_rect;

    if( i_rect.width() <= 0 )
    {
        rectContent.setWidth(0);
    }
    if( i_rect.height() <= 0 )
    {
        rectContent.setHeight(0);
    }
    if( m_rectContent != rectContent )
    {
        m_rectContent = rectContent;

        // Need to completely recalculate the graphical output. Also the layout of
        // the diagram may need to be rearranged.
        // On invalidating a process depth all "higher" update functions also need to
        // be processed by the diagram (but not necessarily by the diagram objects).
        if( isVisible() )
        {
            invalidate(EUpdateLayoutDataPixmapWidget,i_bInformDiagram);
        }
    }

} // setGeometry

//lint -e1961
//------------------------------------------------------------------------------
QSize CDiagObj::sizeHint()
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelLayout,
        /* strMethod    */ "sizeHint",
        /* strAddInfo   */ "" );

    QSize size(0,0);

    switch( m_layoutPos )
    {
        case ELayoutPosTop:
        case ELayoutPosBottom:
        {
            break;
        }
        case ELayoutPosLeft:
        case ELayoutPosRight:
        {
            break;
        }
        case ELayoutPosCenter:
        default:
        {
            break;
        }
    }

    if( mthTracer.isActive(c_iTrcDetailLevelLayout) )
    {
        strTrcMsg  = "Size[w,h]=";
        strTrcMsg += QString::number(size.width()) + ",";
        strTrcMsg += QString::number(size.height());
        mthTracer.setMethodReturn(strTrcMsg);
    }
    return size;

} // sizeHint
//lint +e1961

//------------------------------------------------------------------------------
void CDiagObj::invalidate( unsigned int i_uUpdateFlags, bool i_bInformDiagram )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevelValidate) )
    {
        strTrcMsg = updateFlags2Str(i_uUpdateFlags);
        strTrcMsg += ",InformDiagram=" + ZS::System::bool2Str(i_bInformDiagram);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelValidate,
        /* strMethod    */ "invalidate",
        /* strAddInfo   */ strTrcMsg );

    if( mthTracer.isActive(c_iTrcDetailLevelValidate) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelValidate);
    }

    if( i_uUpdateFlags != EUpdateNone && m_pDataDiagram == nullptr )
    {
        // Widget processing is not needed.
        m_uUpdateFlags |= (i_uUpdateFlags & ~EUpdateWidget);
    }
    else if( i_uUpdateFlags != EUpdateNone && m_pDataDiagram != nullptr )
    {
        // The process depth to be invalidated depends on whether the object is
        // part of a widget diagram, a pixmap diagram or just a data diagram.
        // If a bit is set the corresponding update process need to be executed
        // (keeping the current update processes that still need to be executed):
        switch( m_pDataDiagram->getUpdateType() )
        {
            case EDiagramUpdateTypeWidget:
            {
                // Each announced process depth need to be invalidated.
                m_uUpdateFlags |= i_uUpdateFlags;
                break;
            }
            case EDiagramUpdateTypePixmap:
            {
                // Widget processing is not needed.
                m_uUpdateFlags |= (i_uUpdateFlags & ~EUpdateWidget);
                break;
            }
            case EDiagramUpdateTypeData:
            default:
            {
                // Pixmap and widget processing is not needed.
                m_uUpdateFlags |= (i_uUpdateFlags & ~(EUpdatePixmap|EUpdateWidget));
                break;
            }
        }

        // If method was not called by the diagram itself ..
        if( i_bInformDiagram )
        {
            // .. inform the diagram about the necessary process depth.
            m_pDataDiagram->invalidate(this,m_uUpdateFlags);
        }
    }

    if( mthTracer.isActive(c_iTrcDetailLevelValidate) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelValidate);
    }

} // invalidate

//------------------------------------------------------------------------------
void CDiagObj::validate( unsigned int i_uUpdateFlags )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevelValidate) )
    {
        strTrcMsg = updateFlags2Str(i_uUpdateFlags);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelValidate,
        /* strMethod    */ "validate",
        /* strAddInfo   */ strTrcMsg );

    if( mthTracer.isActive(c_iTrcDetailLevelValidate) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelValidate);
    }

    if( i_uUpdateFlags != EUpdateNone )
    {
        // If a bit is set the corresponding update process need to be executed.
        // To validate the process depth the bit need to be reset.
        m_uUpdateFlags &= ~i_uUpdateFlags;
    }

    if( mthTracer.isActive(c_iTrcDetailLevelValidate) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelValidate);
    }

} // validate

/*==============================================================================
public: // overridables of base class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CDiagObj::event( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    ZS::System::CMsg* pMsg = dynamic_cast<ZS::System::CMsg*>(i_pEv);

    if( pMsg != nullptr )
    {
        ZS::System::CMsgErr* pMsgErr = dynamic_cast<ZS::System::CMsgErr*>(pMsg);

        if( pMsgErr != nullptr )
        {
            onReceivedError(pMsgErr);
            m_pDataDiagram->updateDiagram();
        }
        else
        {
            switch( pMsg->getMsgType() )
            {
                case EMsgTypeReqShow:
                {
                    CMsgReqShow* pMsgReq = dynamic_cast<CMsgReqShow*>(pMsg);

                    if( pMsgReq != nullptr )
                    {
                        SErrResultInfo errResultInfo = onReceivedReqShow(pMsgReq);

                        m_pDataDiagram->updateDiagram();

                        if( pMsgReq->mustBeConfirmed() && pMsgReq->getSender() != nullptr )
                        {
                            CMsgConShow* pMsgCon = new CMsgConShow(
                                /* pObjSender    */ this,
                                /* pObjReceiver  */ pMsgReq->getSender(),
                                /* iReqId        */ pMsgReq->getRequestId(),
                                /* iMsgIdReq     */ pMsgReq->getMsgId(),
                                /* errResultInfo */ errResultInfo );
                            POST_OR_DELETE_MESSAGE(pMsgCon);
                            pMsgCon = nullptr;
                        }
                        bEventHandled = true;
                    }
                    break;
                }
                case EMsgTypeReqHide:
                {
                    CMsgReqHide* pMsgReq = dynamic_cast<CMsgReqHide*>(pMsg);

                    if( pMsgReq != nullptr )
                    {
                        SErrResultInfo errResultInfo = onReceivedReqHide(pMsgReq);

                        m_pDataDiagram->updateDiagram();

                        if( pMsgReq->mustBeConfirmed() && pMsgReq->getSender() != nullptr )
                        {
                            CMsgConHide* pMsgCon = new CMsgConHide(
                                /* pObjSender    */ this,
                                /* pObjReceiver  */ pMsgReq->getSender(),
                                /* iReqId        */ pMsgReq->getRequestId(),
                                /* iMsgIdReq     */ pMsgReq->getMsgId(),
                                /* errResultInfo */ errResultInfo );
                            POST_OR_DELETE_MESSAGE(pMsgCon);
                            pMsgCon = nullptr;
                        }
                        bEventHandled = true;
                    }
                    break;
                }
                case EMsgTypeReqSetValue:
                {
                    CMsgReqSetValue* pMsgReq = dynamic_cast<CMsgReqSetValue*>(pMsg);

                    if( pMsgReq != nullptr )
                    {
                        SErrResultInfo errResultInfo = onReceivedReqSetValue(pMsgReq);

                        m_pDataDiagram->updateDiagram();

                        if( pMsgReq->mustBeConfirmed() && pMsgReq->getSender() != nullptr )
                        {
                            CMsgConSetValue* pMsgCon = new CMsgConSetValue(
                                /* pObjSender    */ this,
                                /* pObjReceiver  */ pMsgReq->getSender(),
                                /* iReqId        */ pMsgReq->getRequestId(),
                                /* iMsgIdReq     */ pMsgReq->getMsgId(),
                                /* errResultInfo */ errResultInfo );
                            POST_OR_DELETE_MESSAGE(pMsgCon);
                            pMsgCon = nullptr;
                        }
                        bEventHandled = true;
                    }
                    break;
                }
                default:
                {
                    break;
                }
            } // switch( pMsg->getMsgType() )
        } // if( pMsgErr == nullptr )
    } // if( pMsg != nullptr )

    if( !bEventHandled )
    {
        bEventHandled = QObject::event(i_pEv);
    }
    return bEventHandled;

} // event

/*==============================================================================
protected: // overridable event methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDiagObj::onReceivedError( ZS::System::CMsgErr* /*i_pMsgErr*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "onReceivedError",
        /* strAddInfo   */ "" );
}

//------------------------------------------------------------------------------
ZS::System::SErrResultInfo CDiagObj::onReceivedReqShow( CMsgReqShow* /*i_pMsgReq*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "onReceivedReqShow",
        /* strAddInfo   */ "" );

    show(true);

    return ErrResultSuccess;
}

//------------------------------------------------------------------------------
ZS::System::SErrResultInfo CDiagObj::onReceivedReqHide( CMsgReqHide* /*i_pMsgReq*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "onReceivedReqHide",
        /* strAddInfo   */ "" );

    hide(true);

    return ErrResultSuccess;
}

//------------------------------------------------------------------------------
ZS::System::SErrResultInfo CDiagObj::onReceivedReqSetValue( CMsgReqSetValue* /*i_pMsgReq*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "onReceivedReqSetValue",
        /* strAddInfo   */ "" );

    return ErrResultSuccess;
}
