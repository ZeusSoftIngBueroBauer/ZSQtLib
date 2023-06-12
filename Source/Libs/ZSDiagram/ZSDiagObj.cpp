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

#include "ZSDiagram/ZSDiagObj.h"
#include "ZSDiagram/ZSDiagScale.h"
#include "ZSDiagram/ZSDiagTrace.h"
#include "ZSDiagram/ZSDiagramProcData.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Diagram;
using namespace ZS::PhysVal;


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
class CDiagObj : public QObject
*******************************************************************************/

/*==============================================================================
protected: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
CDiagObj::CDiagObj(
    const QString& i_strClassName,
    const QString& i_strObjName,
    CDiagTrace*    i_pDiagTrace,
    ELayoutPos     i_layoutPos ) :
//------------------------------------------------------------------------------
    QObject(),
    m_strClassName(i_strClassName),
    m_strObjName(i_strObjName),
    m_pDiagram(nullptr),
    m_pDiagTrace(i_pDiagTrace),
    m_arpDiagScale(CEnumScaleDir::count(), nullptr),
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
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjUpdate(nullptr),
    m_pTrcAdminObjEvents(nullptr),
    m_pTrcAdminObjLayout(nullptr),
    m_pTrcAdminObjValidate(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace(), m_strClassName, objectName());
    m_pTrcAdminObjUpdate = CTrcServer::GetTraceAdminObj(
        NameSpace(), m_strClassName + "::Update", objectName());
    m_pTrcAdminObjEvents = CTrcServer::GetTraceAdminObj(
        NameSpace(), m_strClassName + "::Events", objectName());
    m_pTrcAdminObjLayout = CTrcServer::GetTraceAdminObj(
        NameSpace(), m_strClassName + "::Layout", objectName());
    m_pTrcAdminObjValidate = CTrcServer::GetTraceAdminObj(
        NameSpace(), m_strClassName + "::Validate", objectName());

    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_strObjName;
        strMthInArgs += ", Trace: " + QString(i_pDiagTrace == nullptr ? "nullptr" : i_pDiagTrace->objectName());
        strMthInArgs += ", LayoutPos: " + layoutPos2Str(i_layoutPos);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strMthInArgs */ strMthInArgs );

} // ctor

//------------------------------------------------------------------------------
CDiagObj::CDiagObj(
    const QString& i_strClassName,
    const QString& i_strObjName,
    CDiagScale*    i_pDiagScaleX,
    CDiagScale*    i_pDiagScaleY,
    ELayoutPos     i_layoutPos ) :
//------------------------------------------------------------------------------
    QObject(),
    m_strClassName(i_strClassName),
    m_strObjName(i_strObjName),
    m_pDiagram(nullptr),
    m_pDiagTrace(nullptr),
    m_arpDiagScale(CEnumScaleDir::count(), nullptr),
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
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjUpdate(nullptr),
    m_pTrcAdminObjEvents(nullptr),
    m_pTrcAdminObjLayout(nullptr),
    m_pTrcAdminObjValidate(nullptr)
{
    m_arpDiagScale[static_cast<int>(EScaleDir::X)] = i_pDiagScaleX;
    m_arpDiagScale[static_cast<int>(EScaleDir::Y)] = i_pDiagScaleY;

    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace(), m_strClassName, objectName());
    m_pTrcAdminObjUpdate = CTrcServer::GetTraceAdminObj(
        NameSpace(), m_strClassName + "::Update", objectName());
    m_pTrcAdminObjEvents = CTrcServer::GetTraceAdminObj(
        NameSpace(), m_strClassName + "::Events", objectName());
    m_pTrcAdminObjLayout = CTrcServer::GetTraceAdminObj(
        NameSpace(), m_strClassName + "::Layout", objectName());
    m_pTrcAdminObjValidate = CTrcServer::GetTraceAdminObj(
        NameSpace(), m_strClassName + "::Validate", objectName());

    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_strObjName;
        strMthInArgs += ", ScaleX: " + QString(i_pDiagScaleX == nullptr ? "nullptr" : i_pDiagScaleX->objectName());
        strMthInArgs += ", ScaleY: " + QString(i_pDiagScaleY == nullptr ? "nullptr" : i_pDiagScaleY->objectName());
        strMthInArgs += ", LayoutPos: " + layoutPos2Str(i_layoutPos);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strMthInArgs */ strMthInArgs );

} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDiagObj::~CDiagObj()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strMthInArgs */ "" );

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjUpdate);
    m_pTrcAdminObjUpdate = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjEvents);
    m_pTrcAdminObjEvents = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjLayout);
    m_pTrcAdminObjLayout = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjValidate);
    m_pTrcAdminObjValidate = nullptr;

    //m_strClassName;
    //m_strObjName;
    m_pDiagram = nullptr;
    m_pDiagTrace = nullptr;
    //m_arpDiagScale.clear();
    m_uUpdateFlags = 0;
    m_layoutPos = static_cast<ELayoutPos>(0);
    //m_rectContent;
    //m_rectContentPrev;
    m_bAdjustContentRect2DiagPartCenter = false;
    m_bVisible = false;
    m_state = static_cast<EDiagObjState>(0);
    m_bIsFocusable = false;
    m_bPtFocusedValid = false;
    //m_ptFocused;
    m_bIsEditable = false;
    m_bPtEditSessionValid = false;
    //m_ptEditSession;
    m_pTrcAdminObj = nullptr;
    m_pTrcAdminObjUpdate = nullptr;
    m_pTrcAdminObjEvents = nullptr;
    m_pTrcAdminObjLayout = nullptr;
    m_pTrcAdminObjValidate = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDiagObj::setObjName( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_strObjName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setObjectName",
        /* strMthInArgs */ strMthInArgs );

    if( m_strObjName != i_strObjName )
    {
        QObject::setObjectName(i_strObjName);

        m_strObjName = i_strObjName;

        if( m_pTrcAdminObj != nullptr )
        {
            m_pTrcAdminObj = CTrcServer::RenameTraceAdminObj(m_pTrcAdminObj, objectName());
        }
        if( m_pTrcAdminObjUpdate != nullptr )
        {
            m_pTrcAdminObjUpdate =
                CTrcServer::RenameTraceAdminObj(m_pTrcAdminObjUpdate, objectName());
        }
        if( m_pTrcAdminObjEvents != nullptr )
        {
            m_pTrcAdminObjEvents =
                CTrcServer::RenameTraceAdminObj(m_pTrcAdminObjEvents, objectName());
        }
        if( m_pTrcAdminObjLayout != nullptr )
        {
            m_pTrcAdminObjLayout =
                CTrcServer::RenameTraceAdminObj(m_pTrcAdminObjLayout, objectName());
        }
        if( m_pTrcAdminObjValidate != nullptr )
        {
            m_pTrcAdminObjValidate =
                CTrcServer::RenameTraceAdminObj(m_pTrcAdminObjValidate, objectName());
        }
    }
} // setObjName

//------------------------------------------------------------------------------
QString CDiagObj::getObjName() const
//------------------------------------------------------------------------------
{
    return m_strObjName;
}

//------------------------------------------------------------------------------
CDataDiagram* CDiagObj::getDiagram()
//------------------------------------------------------------------------------
{
    return m_pDiagram;
}

//------------------------------------------------------------------------------
CDiagScale* CDiagObj::getDiagScale( const CEnumScaleDir& i_scaleDir )
//------------------------------------------------------------------------------
{
    CDiagScale* pDiagScale = m_arpDiagScale[i_scaleDir.enumeratorAsInt()];

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
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = QString(i_pDiagTrace == nullptr ? "null" : i_pDiagTrace->objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setDiagTrace",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
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
        mthTracer.trace(strTrcMsg, ELogDetailLevel::Debug);
    }

    m_pDiagTrace = i_pDiagTrace;

    invalidate(EUpdateDataPixmapWidget,true);

} // setDiagTrace

//------------------------------------------------------------------------------
void CDiagObj::setAdjustContentRect2DiagPartCenter( bool i_bAdjust )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = bool2Str(i_bAdjust);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setAdjustContentRect2DiagPartCenter",
        /* strAddInfo   */ strMthInArgs );

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
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = bool2Str(i_bInformDiagram);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "show",
        /* strAddInfo   */ strMthInArgs );

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
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = bool2Str(i_bInformDiagram);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hide",
        /* strAddInfo   */ strMthInArgs );

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
double CDiagObj::getValRes( const CEnumScaleDir& i_scaleDir, const CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    double fRes = 0.0;

    if( m_pDiagTrace != nullptr )
    {
        fRes = m_pDiagTrace->getValRes(i_scaleDir, i_pUnit);
    }
    else if( m_arpDiagScale[i_scaleDir.enumeratorAsInt()] != nullptr )
    {
        fRes = m_arpDiagScale[i_scaleDir.enumeratorAsInt()]->getScaleRes(i_pUnit);
    }
    return fRes;

} // getValRes

//------------------------------------------------------------------------------
double CDiagObj::getValRes( const CEnumScaleDir& i_scaleDir, double i_fVal, const CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    double fRes = 0.0;

    if( m_pDiagTrace != nullptr )
    {
        fRes = m_pDiagTrace->getValRes(i_scaleDir,i_fVal,i_pUnit);
    }
    else if( m_arpDiagScale[i_scaleDir.enumeratorAsInt()] != nullptr )
    {
        fRes = m_arpDiagScale[i_scaleDir.enumeratorAsInt()]->getScaleRes(i_fVal, i_pUnit);
    }
    return fRes;

} // getValRes

//------------------------------------------------------------------------------
CPhysVal CDiagObj::getVal( const CEnumScaleDir& /*i_scaleDir*/ ) const
//------------------------------------------------------------------------------
{
    return CPhysVal();
}

//------------------------------------------------------------------------------
void CDiagObj::setIsFocusable( bool i_bFocusable )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = bool2Str(i_bFocusable);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setIsFocusable",
        /* strAddInfo   */ strMthInArgs );

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
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjEvents, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = bool2Str(i_bInformDiagram);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setFocus",
        /* strAddInfo   */ strMthInArgs );

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
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjEvents, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = qPoint2Str(i_pt);
        strMthInArgs += ", " + bool2Str(i_bInformDiagram);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setFocus",
        /* strAddInfo   */ strMthInArgs );

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
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjEvents, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = bool2Str(i_bInformDiagram);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "unsetFocus",
        /* strAddInfo   */ strMthInArgs );

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
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjEvents, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = bool2Str(i_bEditable);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::ArgsNormal,
        /* strMethod    */ "setIsEditable",
        /* strAddInfo   */ strMthInArgs );

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
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjEvents, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = bool2Str(i_bInformDiagram);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "startEditSession",
        /* strAddInfo   */ strMthInArgs );

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
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjEvents, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = qPoint2Str(i_pt);
        strMthInArgs += ", " + bool2Str(i_bInformDiagram);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::ArgsNormal,
        /* strMethod    */ "startEditSession",
        /* strAddInfo   */ strMthInArgs );

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
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjEvents, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = bool2Str(i_bInformDiagram);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "stopEditSession",
        /* strAddInfo   */ strMthInArgs );

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

    if (areMethodCallsActive(m_pTrcAdminObjLayout, EMethodTraceDetailLevel::ArgsNormal))
    {
        strTrcMsg  = qRect2Str(i_rect);
        strTrcMsg += ", " + bool2Str(i_bInformDiagram);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(qSize2Str(size));
    }
    return size;

} // sizeHint
//lint +e1961

//------------------------------------------------------------------------------
void CDiagObj::invalidate( unsigned int i_uUpdateFlags, bool i_bInformDiagram )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObjValidate, EMethodTraceDetailLevel::ArgsNormal))
    {
        strTrcMsg = updateFlags2Str(i_uUpdateFlags);
        strTrcMsg += ", InformDiagram: " + bool2Str(i_bInformDiagram);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjValidate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "invalidate",
        /* strAddInfo   */ strTrcMsg );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

    if( i_uUpdateFlags != EUpdateNone && m_pDiagram == nullptr )
    {
        // Widget processing is not needed.
        m_uUpdateFlags |= (i_uUpdateFlags & ~EUpdateWidget);
    }
    else if( i_uUpdateFlags != EUpdateNone && m_pDiagram != nullptr )
    {
        // The process depth to be invalidated depends on whether the object is
        // part of a widget diagram, a pixmap diagram or just a data diagram.
        // If a bit is set the corresponding update process need to be executed
        // (keeping the current update processes that still need to be executed):
        switch( m_pDiagram->getUpdateType() )
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
            m_pDiagram->invalidate(this,m_uUpdateFlags);
        }
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

} // invalidate

//------------------------------------------------------------------------------
void CDiagObj::validate( unsigned int i_uUpdateFlags )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObjValidate, EMethodTraceDetailLevel::ArgsNormal))
    {
        strTrcMsg = updateFlags2Str(i_uUpdateFlags);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjValidate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "validate",
        /* strAddInfo   */ strTrcMsg );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

    if( i_uUpdateFlags != EUpdateNone )
    {
        // If a bit is set the corresponding update process need to be executed.
        // To validate the process depth the bit need to be reset.
        m_uUpdateFlags &= ~i_uUpdateFlags;
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }
} // validate

/*==============================================================================
public: // overridables of base class QObject
==============================================================================*/

////------------------------------------------------------------------------------
//bool CDiagObj::event( QEvent* i_pEv )
////------------------------------------------------------------------------------
//{
//    bool bEventHandled = false;
//
//    ZS::System::CMsg* pMsg = dynamic_cast<ZS::System::CMsg*>(i_pEv);
//
//    if( pMsg != nullptr )
//    {
//        ZS::System::CMsgErr* pMsgErr = dynamic_cast<ZS::System::CMsgErr*>(pMsg);
//
//        if( pMsgErr != nullptr )
//        {
//            onReceivedError(pMsgErr);
//            m_pDiagram->update(this, m_rectContent);
//        }
//        else
//        {
//            switch( pMsg->getMsgType() )
//            {
//                case EMsgTypeReqShow:
//                {
//                    CMsgReqShow* pMsgReq = dynamic_cast<CMsgReqShow*>(pMsg);
//
//                    if( pMsgReq != nullptr )
//                    {
//                        SErrResultInfo errResultInfo = onReceivedReqShow(pMsgReq);
//
//                        m_pDiagram->update(this, m_rectContent);
//
//                        if( pMsgReq->mustBeConfirmed() && pMsgReq->getSender() != nullptr )
//                        {
//                            CMsgConShow* pMsgCon = new CMsgConShow(
//                                /* pObjSender    */ this,
//                                /* pObjReceiver  */ pMsgReq->getSender(),
//                                /* iReqId        */ pMsgReq->getRequestId(),
//                                /* iMsgIdReq     */ pMsgReq->getMsgId(),
//                                /* errResultInfo */ errResultInfo );
//                            POST_OR_DELETE_MESSAGE(pMsgCon);
//                            pMsgCon = nullptr;
//                        }
//                        bEventHandled = true;
//                    }
//                    break;
//                }
//                case EMsgTypeReqHide:
//                {
//                    CMsgReqHide* pMsgReq = dynamic_cast<CMsgReqHide*>(pMsg);
//
//                    if( pMsgReq != nullptr )
//                    {
//                        SErrResultInfo errResultInfo = onReceivedReqHide(pMsgReq);
//
//                        m_pDiagram->update(this, m_rectContent);
//
//                        if( pMsgReq->mustBeConfirmed() && pMsgReq->getSender() != nullptr )
//                        {
//                            CMsgConHide* pMsgCon = new CMsgConHide(
//                                /* pObjSender    */ this,
//                                /* pObjReceiver  */ pMsgReq->getSender(),
//                                /* iReqId        */ pMsgReq->getRequestId(),
//                                /* iMsgIdReq     */ pMsgReq->getMsgId(),
//                                /* errResultInfo */ errResultInfo );
//                            POST_OR_DELETE_MESSAGE(pMsgCon);
//                            pMsgCon = nullptr;
//                        }
//                        bEventHandled = true;
//                    }
//                    break;
//                }
//                case EMsgTypeReqSetValue:
//                {
//                    CMsgReqSetValue* pMsgReq = dynamic_cast<CMsgReqSetValue*>(pMsg);
//
//                    if( pMsgReq != nullptr )
//                    {
//                        SErrResultInfo errResultInfo = onReceivedReqSetValue(pMsgReq);
//
//                        m_pDiagram->update(this, m_rectContent);
//
//                        if( pMsgReq->mustBeConfirmed() && pMsgReq->getSender() != nullptr )
//                        {
//                            CMsgConSetValue* pMsgCon = new CMsgConSetValue(
//                                /* pObjSender    */ this,
//                                /* pObjReceiver  */ pMsgReq->getSender(),
//                                /* iReqId        */ pMsgReq->getRequestId(),
//                                /* iMsgIdReq     */ pMsgReq->getMsgId(),
//                                /* errResultInfo */ errResultInfo );
//                            POST_OR_DELETE_MESSAGE(pMsgCon);
//                            pMsgCon = nullptr;
//                        }
//                        bEventHandled = true;
//                    }
//                    break;
//                }
//                default:
//                {
//                    break;
//                }
//            } // switch( pMsg->getMsgType() )
//        } // if( pMsgErr == nullptr )
//    } // if( pMsg != nullptr )
//
//    if( !bEventHandled )
//    {
//        bEventHandled = QObject::event(i_pEv);
//    }
//    return bEventHandled;
//
//} // event

/*==============================================================================
protected: // overridable event methods
==============================================================================*/

////------------------------------------------------------------------------------
//void CDiagObj::onReceivedError( ZS::System::CMsgErr* /*i_pMsgErr*/ )
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod    */ "onReceivedError",
//        /* strAddInfo   */ "" );
//}
//
////------------------------------------------------------------------------------
//ZS::System::SErrResultInfo CDiagObj::onReceivedReqShow( CMsgReqShow* /*i_pMsgReq*/ )
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod    */ "onReceivedReqShow",
//        /* strAddInfo   */ "" );
//
//    show(true);
//
//    return ErrResultSuccess;
//}
//
////------------------------------------------------------------------------------
//ZS::System::SErrResultInfo CDiagObj::onReceivedReqHide( CMsgReqHide* /*i_pMsgReq*/ )
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod    */ "onReceivedReqHide",
//        /* strAddInfo   */ "" );
//
//    hide(true);
//
//    return ErrResultSuccess;
//}
//
////------------------------------------------------------------------------------
//ZS::System::SErrResultInfo CDiagObj::onReceivedReqSetValue( CMsgReqSetValue* /*i_pMsgReq*/ )
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod    */ "onReceivedReqSetValue",
//        /* strAddInfo   */ "" );
//
//    return ErrResultSuccess;
//}
