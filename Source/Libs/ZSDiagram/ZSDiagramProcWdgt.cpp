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

#include "ZSDiagram/ZSDiagramProcWdgt.h"
#include "ZSDiagram/ZSDiagObj.h"
#include "ZSDiagram/ZSDiagScale.h"
#include "ZSDiagram/ZSDiagTrace.h"
#include "ZSDiagram/ZSDiagramFrameStyles.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTime.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtCore/qdir.h>
#include <QtGui/qevent.h>
#include <QtGui/qpainter.h>

#if QT_VERSION < 0x050000
#include <QtGui/qapplication.h>
#include <QtGui/qfiledialog.h>
#include <QtGui/qlabel.h>
#include <QtGui/qmenu.h>
#include <QtGui/qprinter.h>
#else
#include <QtPrintSupport/qprinter.h>
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qmenu.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Diagram;
using namespace ZS::PhysVal;

//lint -e834

/*******************************************************************************
class CWdgtDiagram : public QWidget, public CPixmapDiagram
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtDiagram::CWdgtDiagram(const QString& i_strObjName, QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    CPixmapDiagram(i_strObjName,EDiagramUpdateTypeWidget),
    m_bZoomingXScaleEnabled(false),
    m_bZoomingYScaleEnabled(false),
    m_bIsZooming(false),
    m_ptZoomStart(),
    m_ptZoomEnd(),
    m_rectZoom(),
    m_iZoomCount(0),
    m_bContextPopupMenuEnabled(false),
    m_pMenuContext(nullptr),
    m_strTipOfTheDay(),
    m_iTipOfTheDayIndentTop(20),
    m_iTipOfTheDayIndentBottom(20),
    m_iTipOfTheDayIndentLeft(20),
    m_iTipOfTheDayIndentRight(20),
    m_pLblTipOfTheDay(nullptr),
    m_strCurrentFileDir(),
    m_bMoveKeyAccelerationEnabled(true),
    m_pMoveKeyAccelerationDiagScale(nullptr),
    m_iMoveKeyAccelerationTimeout_ms(1000),
    m_iMoveKeyAccelerationTimeoutSlowDown_ms(400),
    m_iMoveKeyAccelerationTimeoutSpeedUp_ms(200),
    m_iMoveKeyAccelarationSpeedUpDelayRepCount(5),
    m_fMoveKeyAccelerationFactor(2.0),
    m_fMoveKeyAccelerationMaxStepScaleFactor(0.02),
    m_fMoveKeyEventTime_ms(0.0),
    m_fMoveKeyRepCount(0.0),
    m_iMoveKeySpeedUpDelayRepCount(0),
    m_iMoveKeyPrev(-1),
    m_pDiagObjFocused(nullptr),
    m_pDiagObjFocusedByKeyEvent(nullptr),
    m_pDiagObjFocusedByMouseEvent(nullptr),
    m_pDiagObjFocusedRecently(nullptr),
    m_pDiagObjEditing(nullptr),
    m_pDiagObjEditingByKeyEvent(nullptr),
    m_pDiagObjEditingByMouseEvent(nullptr),
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjUpdate(nullptr),
    m_pTrcAdminObjEvents(nullptr),
    m_pTrcAdminObjLayout(nullptr),
    m_pTrcAdminObjValidate(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName(), m_strObjName);
    m_pTrcAdminObjUpdate = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::Update", m_strObjName);
    m_pTrcAdminObjEvents = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::Events", m_strObjName);
    m_pTrcAdminObjLayout = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::Layout", m_strObjName);
    m_pTrcAdminObjValidate = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::Validate", m_strObjName);

    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_strObjName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    #if QT_VERSION >= 0x040100
    #else
    setWFlags( getWFlags() | Qt::WNoAutoErase | Qt::WRepaintNoErase ); //lint !e1506
    #endif

    #if QT_VERSION >= 0x040100
    #else
    m_colErase = eraseColor();
    #endif

} // ctor

//------------------------------------------------------------------------------
CWdgtDiagram::~CWdgtDiagram()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    emit_aboutToBeDestroyed(m_strObjName);

    try
    {
        delete m_pMenuContext;
    }
    catch(...)
    {
    }
    m_pMenuContext = nullptr;

    try
    {
        delete m_pLblTipOfTheDay;
    }
    catch(...)
    {
    }
    m_pLblTipOfTheDay = nullptr;

    m_pMoveKeyAccelerationDiagScale = nullptr;

    m_pDiagObjFocused = nullptr;
    m_pDiagObjFocusedByKeyEvent = nullptr;
    m_pDiagObjFocusedByMouseEvent = nullptr;
    m_pDiagObjFocusedRecently = nullptr;
    m_pDiagObjEditing = nullptr;
    m_pDiagObjEditingByKeyEvent = nullptr;
    m_pDiagObjEditingByMouseEvent = nullptr;

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

} // dtor

/*==============================================================================
public: // copy ctor not allowed but diagrams may be cloned
==============================================================================*/

//------------------------------------------------------------------------------
CDataDiagram* CWdgtDiagram::clone( EDiagramUpdateType i_diagramUpdateType ) const
//------------------------------------------------------------------------------
{
    CDataDiagram* pDiagram = nullptr;

    if( i_diagramUpdateType < EDiagramUpdateTypeWidget )
    {
        pDiagram = CPixmapDiagram::clone(i_diagramUpdateType);
    }
    else
    {
        CWdgtDiagram* pWdgtDiagram = new CWdgtDiagram(m_strObjName);

        // Data Diagram
        //-------------

        pWdgtDiagram->m_uUpdateFlags = m_uUpdateFlags;
        pWdgtDiagram->m_measState = m_measState;
        pWdgtDiagram->m_measMode = m_measMode;
        pWdgtDiagram->m_iMeasType = m_iMeasType;

        for (int idx = 0; idx < CEnumScaleDir::count(); idx++)
        {
            pWdgtDiagram->m_arSpacing[idx] = m_arSpacing[idx];
        }
        for (int idx = 0; idx < m_arpDiagScales.size(); ++idx)
        {
            m_arpDiagScales[idx]->clone(pWdgtDiagram);
        }
        for (int idx = 0; idx < m_arpDiagTraces.size(); ++idx)
        {
            m_arpDiagTraces[idx]->clone(pWdgtDiagram);
        }
        for (int idx = 0; idx < m_arpDiagObjs.size(); ++idx)
        {
            CDiagObj* pDiagObj = m_arpDiagObjs[idx]->clone(pWdgtDiagram);
            QString strDiagObjKey = m_arpDiagObjs[idx]->className() + "::" + m_arpDiagObjs[idx]->getObjName();
            pWdgtDiagram->m_hshpDiagObjs[strDiagObjKey] = pDiagObj;
        }

        // Pixmap Diagram
        //---------------

        pWdgtDiagram->m_colErase = m_colErase;
        pWdgtDiagram->m_colBgPartCenter = m_colBgPartCenter;

        if( m_pPixmap != nullptr )
        {
            pWdgtDiagram->m_pPixmap = new QPixmap(*m_pPixmap);
        }
        pWdgtDiagram->m_size = m_size;
        pWdgtDiagram->m_cyMinimumHeightPartTop = m_cyMinimumHeightPartTop;
        pWdgtDiagram->m_cyMinimumHeightPartBottom = m_cyMinimumHeightPartBottom;
        pWdgtDiagram->m_cxMinimumWidthPartLeft = m_cxMinimumWidthPartLeft;
        pWdgtDiagram->m_cxMinimumWidthPartRight = m_cxMinimumWidthPartRight;
        pWdgtDiagram->m_iMarginTop = m_iMarginTop;
        pWdgtDiagram->m_iMarginBottom = m_iMarginBottom;
        pWdgtDiagram->m_iMarginLeft = m_iMarginLeft;
        pWdgtDiagram->m_iMarginRight = m_iMarginRight;
        pWdgtDiagram->m_rectPartTop = m_rectPartTop;
        pWdgtDiagram->m_rectPartBottom = m_rectPartBottom;
        pWdgtDiagram->m_rectPartLeft = m_rectPartLeft;
        pWdgtDiagram->m_rectPartRight = m_rectPartRight;
        pWdgtDiagram->m_rectPartCenter = m_rectPartCenter;

        if( m_pFrameStyle != nullptr )
        {
            pWdgtDiagram->m_pFrameStyle = new SFrameStyle(*m_pFrameStyle);
        }
        if( m_pFrameStylePartCenter != nullptr )
        {
            pWdgtDiagram->m_pFrameStylePartCenter = new SFrameStyle(*m_pFrameStylePartCenter);
        }

        // Widget Diagram
        //---------------

        pWdgtDiagram->m_bZoomingXScaleEnabled = m_bZoomingXScaleEnabled;
        pWdgtDiagram->m_bZoomingYScaleEnabled = m_bZoomingYScaleEnabled;
        //pWdgtDiagram->m_bIsZooming;
        //pWdgtDiagram->m_ptZoomStart;
        //pWdgtDiagram->m_ptZoomEnd;
        //pWdgtDiagram->m_rectZoom;
        //pWdgtDiagram->m_iZoomCount;
        pWdgtDiagram->m_bContextPopupMenuEnabled = m_bContextPopupMenuEnabled;
        //pWdgtDiagram->m_pMenuContext;
        //pWdgtDiagram->m_pTipOfTheDay;
        //pWdgtDiagram->m_strCurrentFileDir;
        pWdgtDiagram->m_bMoveKeyAccelerationEnabled = m_bMoveKeyAccelerationEnabled;
        pWdgtDiagram->m_pMoveKeyAccelerationDiagScale = m_pMoveKeyAccelerationDiagScale;
        pWdgtDiagram->m_iMoveKeyAccelerationTimeout_ms = m_iMoveKeyAccelerationTimeout_ms;
        pWdgtDiagram->m_iMoveKeyAccelerationTimeoutSlowDown_ms = m_iMoveKeyAccelerationTimeoutSlowDown_ms;
        pWdgtDiagram->m_iMoveKeyAccelerationTimeoutSpeedUp_ms = m_iMoveKeyAccelerationTimeoutSpeedUp_ms;
        pWdgtDiagram->m_iMoveKeyAccelarationSpeedUpDelayRepCount = m_iMoveKeyAccelarationSpeedUpDelayRepCount;
        pWdgtDiagram->m_fMoveKeyAccelerationFactor = m_fMoveKeyAccelerationFactor;
        pWdgtDiagram->m_fMoveKeyAccelerationMaxStepScaleFactor = m_fMoveKeyAccelerationMaxStepScaleFactor;
        //pWdgtDiagram->m_fMoveKeyEventTime_ms;
        //pWdgtDiagram->m_fMoveKeyRepCount;
        //pWdgtDiagram->m_iMoveKeySpeedUpDelayRepCount;
        //pWdgtDiagram->m_iMoveKeyPrev;
        //pWdgtDiagram->m_pDiagObjFocused;
        //pWdgtDiagram->m_pDiagObjFocusedByKeyEvent;
        //pWdgtDiagram->m_pDiagObjFocusedByMouseEvent;
        //pWdgtDiagram->m_pDiagObjFocusedRecently;
        //pWdgtDiagram->m_pDiagObjEditing;
        //pWdgtDiagram->m_pDiagObjEditingByKeyEvent;
        //pWdgtDiagram->m_pDiagObjEditingByMouseEvent;

        pDiagram = pWdgtDiagram;
    }
    return pDiagram;

} // clone

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CDiagObj* CWdgtDiagram::getDiagObjFocused()
//------------------------------------------------------------------------------
{
    return m_pDiagObjFocused;
}

//------------------------------------------------------------------------------
CDiagObj* CWdgtDiagram::getDiagObjEditing()
//------------------------------------------------------------------------------
{
    return m_pDiagObjEditing;
}

/*==============================================================================
public: // instance methods to set optional attributes of the diagram
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagram::enableZooming( const CEnumScaleDir& i_scaleDir )
//------------------------------------------------------------------------------
{
    switch( i_scaleDir.enumerator() )
    {
        case EScaleDir::X:
        {
            m_bZoomingXScaleEnabled = true;
            break;
        }
        case EScaleDir::Y:
        {
            m_bZoomingYScaleEnabled = true;
            break;
        }
        default:
        {
            m_bZoomingXScaleEnabled = true;
            m_bZoomingYScaleEnabled = true;
            break;
        }
    }
}

//------------------------------------------------------------------------------
void CWdgtDiagram::disableZooming( const CEnumScaleDir& i_scaleDir )
//------------------------------------------------------------------------------
{
    switch( i_scaleDir.enumerator() )
    {
        case EScaleDir::X:
        {
            m_bZoomingXScaleEnabled = false;
            break;
        }
        case EScaleDir::Y:
        {
            m_bZoomingYScaleEnabled = false;
            break;
        }
        default:
        {
            m_bZoomingXScaleEnabled = false;
            m_bZoomingYScaleEnabled = false;
            break;
        }
    }
}

//------------------------------------------------------------------------------
bool CWdgtDiagram::isZoomingEnabled( const CEnumScaleDir& i_scaleDir ) const
//------------------------------------------------------------------------------
{
    bool bZoomingEnabled = false;

    switch( i_scaleDir.enumerator() )
    {
        case EScaleDir::X:
        {
            bZoomingEnabled = m_bZoomingXScaleEnabled;
            break;
        }
        case EScaleDir::Y:
        {
            bZoomingEnabled = m_bZoomingYScaleEnabled;
            break;
        }
        default:
        {
            bZoomingEnabled = (m_bZoomingXScaleEnabled || m_bZoomingYScaleEnabled);
            break;
        }
    }
    return bZoomingEnabled;
}

//------------------------------------------------------------------------------
void CWdgtDiagram::enableContextPopupMenu()
//------------------------------------------------------------------------------
{
    m_bContextPopupMenuEnabled = true;
}

//------------------------------------------------------------------------------
void CWdgtDiagram::disableContextPopupMenu()
//------------------------------------------------------------------------------
{
    m_bContextPopupMenuEnabled = false;
}

//------------------------------------------------------------------------------
bool CWdgtDiagram::isContextPopupMenuEnabled() const
//------------------------------------------------------------------------------
{
    return m_bContextPopupMenuEnabled;
}

//------------------------------------------------------------------------------
void CWdgtDiagram::showTipOfTheDay()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "showTipOfTheDay",
        /* strAddInfo   */ "" );

    if( m_pLblTipOfTheDay == nullptr )
    {
        m_pLblTipOfTheDay = new QLabel(this);
    }
    m_pLblTipOfTheDay->clear();
    m_pLblTipOfTheDay->setText(m_strTipOfTheDay);
    m_pLblTipOfTheDay->setAlignment(Qt::AlignTop);
    m_pLblTipOfTheDay->setMargin(4);
    m_pLblTipOfTheDay->show();
    invalidate(nullptr,EUpdateWidget);
}

//------------------------------------------------------------------------------
void CWdgtDiagram::hideTipOfTheDay()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hideTipOfTheDay",
        /* strAddInfo   */ "" );

    if( m_pLblTipOfTheDay != nullptr )
    {
        delete m_pLblTipOfTheDay;
        m_pLblTipOfTheDay = nullptr;
        invalidate(nullptr,EUpdateWidget);
    }
}

//------------------------------------------------------------------------------
void CWdgtDiagram::setTipOfTheDay( const QString& i_str )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setTipOfTheDay",
        /* strAddInfo   */ i_str );

    m_strTipOfTheDay = i_str;

    if( m_pLblTipOfTheDay != nullptr )
    {
        m_pLblTipOfTheDay->clear();
        m_pLblTipOfTheDay->setText(m_strTipOfTheDay);
        invalidate(nullptr,EUpdateWidget);
    }
}

//------------------------------------------------------------------------------
void CWdgtDiagram::setTipOfTheDayIndentTop( int i_iIndent )
//------------------------------------------------------------------------------
{
    m_iTipOfTheDayIndentTop = i_iIndent;
}

//------------------------------------------------------------------------------
void CWdgtDiagram::setTipOfTheDayIndentBottom( int i_iIndent )
//------------------------------------------------------------------------------
{
    m_iTipOfTheDayIndentBottom = i_iIndent;
}

//------------------------------------------------------------------------------
void CWdgtDiagram::setTipOfTheDayIndentLeft( int i_iIndent )
//------------------------------------------------------------------------------
{
    m_iTipOfTheDayIndentLeft = i_iIndent;
}

//------------------------------------------------------------------------------
void CWdgtDiagram::setTipOfTheDayIndentRight( int i_iIndent )
//------------------------------------------------------------------------------
{
    m_iTipOfTheDayIndentRight = i_iIndent;
}

//------------------------------------------------------------------------------
void CWdgtDiagram::enableMoveKeyAcceleration(
    CDiagScale* i_pDiagScale,
    int         i_iTimeout_ms,
    int         i_iTimeoutSlowDown_ms,
    int         i_iTimeoutSpeedUp_ms,
    int         i_iSpeedUpDelayRepCount,
    double      i_fFactor,
    double      i_fMaxStepScaleFactor )
//------------------------------------------------------------------------------
{
    m_bMoveKeyAccelerationEnabled = true;
    m_pMoveKeyAccelerationDiagScale = i_pDiagScale;
    m_iMoveKeyAccelerationTimeout_ms = i_iTimeout_ms;
    m_iMoveKeyAccelerationTimeoutSlowDown_ms = i_iTimeoutSlowDown_ms;
    m_iMoveKeyAccelerationTimeoutSpeedUp_ms = i_iTimeoutSpeedUp_ms;
    m_iMoveKeyAccelarationSpeedUpDelayRepCount = i_iSpeedUpDelayRepCount;
    m_fMoveKeyAccelerationFactor = i_fFactor;
    m_fMoveKeyAccelerationMaxStepScaleFactor = i_fMaxStepScaleFactor;
}

//------------------------------------------------------------------------------
void CWdgtDiagram::disableMoveKeyAcceleration()
//------------------------------------------------------------------------------
{
    m_bMoveKeyAccelerationEnabled = false;
}

/*==============================================================================
public: // overridables of base class CPixmapDiagram
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief This method is called by the diagram objects to trigger a paint event.

    Used instead of QWidget::update(rect) to trace the calling diagram object.
*/
void CWdgtDiagram::update( CDiagObj* i_pDiagObj, const QRect& i_rect )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObjUpdate != nullptr && m_pTrcAdminObjUpdate->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave) )
    {
        strMthInArgs = QString(i_pDiagObj == nullptr ? "null" : i_pDiagObj->getObjName());
        strMthInArgs += ", " + qRect2Str(i_rect);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "update",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strAddTrcInfo = "OldUpdFlags: " + updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strAddTrcInfo);
    }

    //CPixmapDiagram::update(i_pDiagObj, i_rect);

    // This method is called by the diagram objects after they have
    // recalculated their outputs within their update methods.
    if( i_rect.isValid() )
    {
        // Trigger paint event.
        update(i_rect);
    }

    // Mark current process depth as executed (reset bit):
    validate(EUpdateWidget);

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strAddTrcInfo = "NewUpdFlags: " + updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strAddTrcInfo);
    }

} // update

/*==============================================================================
public: // reimplemented (hiding) methods of QWidget for method tracing
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief This method is called by the diagram itself to trigger a paint event.

    Used instead of QWidget::update() to trace the method.
*/
void CWdgtDiagram::update()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "update",
        /* strAddInfo   */ "" );

    // This method is called by the underlying window system but may also be called
    // by objects controlling the diagram to update the content of the diagram.

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strAddTrcInfo = "OldUpdFlags: " + updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strAddTrcInfo);
    }

    // Trigger update (repainting) content of diagram
    QWidget::update();

    // Mark current process depth as executed (reset bit):
    validate(EUpdateWidget);

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strAddTrcInfo = "NewUpdFlags: " + updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strAddTrcInfo);
    }

} // update

//------------------------------------------------------------------------------
/*! @brief This method is called by the diagram itself to trigger a paint event.

    Used instead of QWidget::update(QRect) to trace the method.
*/
void CWdgtDiagram::update( const QRect& i_rect )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjUpdate != nullptr && m_pTrcAdminObjUpdate->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave) )
    {
        strMthInArgs = qRect2Str(i_rect);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "update",
        /* strAddInfo   */ strMthInArgs );

    // This method is called by the underlying window system to force an update
    // of an invalidated region but also by the diagram objects after they have
    // recalculated their outputs within their update methods.
    if( i_rect.isValid() )
    {
        QWidget::update(i_rect);
    }

    // Mark current process depth as executed (reset bit):
    validate(EUpdateWidget);

} // update

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagram::setMouseTracking( bool i_bEnable )
//------------------------------------------------------------------------------
{
    QWidget::setMouseTracking(i_bEnable);
}

//------------------------------------------------------------------------------
void CWdgtDiagram::setEraseColor( const QColor& i_col )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setEraseColor",
        /* strAddInfo   */ "" );

    m_colErase = i_col;

    #if QT_VERSION >= 0x040100
    #else
    QWidget::setEraseColor(i_col);
    #endif

    invalidate(nullptr,EUpdatePixmapWidget);
}

/*==============================================================================
public: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagram::setFocus()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setFocus",
        /* strAddInfo   */ "" );

    QWidget::setFocus();
}

//------------------------------------------------------------------------------
void CWdgtDiagram::clearFocus()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "clearFocus",
        /* strAddInfo   */ "" );

    QWidget::clearFocus();

    // Unselect currently selected object (if any object selected) but keep the
    // focus on the selected object.
    if( m_pDiagObjEditing != nullptr )
    {
        // If the graphical output of the object changes the corresponding
        // update flags will be invalidated (the bits will be set).
        m_pDiagObjEditing->stopEditSession(true); // also inform other objects of necessary pixmap processing

        // No object is longer being edited:
        m_pDiagObjEditing = nullptr;
        m_pDiagObjEditingByKeyEvent = nullptr;
        m_pDiagObjEditingByMouseEvent = nullptr;
    }

    // Remove focus from currently focused object (if any object focused).
    if( m_pDiagObjFocused != nullptr )
    {
        // If the graphical output of the object changes the corresponding
        // update flags will be invalidated (the bits will be set).
        m_pDiagObjFocused->unsetFocus(true); // also inform other objects of necessary pixmap processing

        // No object is longer being focused:
        m_pDiagObjFocused = nullptr;
        m_pDiagObjFocusedByKeyEvent = nullptr;
        m_pDiagObjFocusedByMouseEvent = nullptr;
    }

    // Update graphical output of the diagram.
    // For the unfocused or no longer edited object at least data processing will be executed
    // and the object invalidates the "dirty" rectangle area of the widget:

    m_uUpdateFlags |= EUpdateAll;

    // Trigger paint event
    update();

} // clearFocus

//------------------------------------------------------------------------------
void CWdgtDiagram::keyPressEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObjEvents != nullptr && m_pTrcAdminObjEvents->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave) )
    {
        strTrcMsg = qKeyCode2Str(i_pEv->key());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "keyPressEvent",
        /* strAddInfo   */ strTrcMsg );

    // As default we ignore the event ..
    i_pEv->ignore();

    switch( i_pEv->key() )
    {
        case Qt::Key_Escape:
        {
            // Unselect currently selected object (if any object selected) but keep the
            // focus on the selected object.
            if( m_pDiagObjEditing != nullptr )
            {
                // If the graphical output of the object changes the corresponding
                // update flags will be invalidated (the bits will be set).
                m_pDiagObjEditing->stopEditSession(true); // also inform other objects of necessary pixmap processing

                // No object is longer being edited:
                m_pDiagObjEditing = nullptr;
                m_pDiagObjEditingByKeyEvent = nullptr;
                m_pDiagObjEditingByMouseEvent = nullptr;

                // Update graphical output of the diagram.
                // For the no longer edited object at least data processing will be executed
                // and the object invalidates the "dirty" rectangle area of the widget:
                m_uUpdateFlags |= EUpdateAll;

                // Trigger paint event
                update();

                // The diagram just "eats" the escape key if an object was focused or selected.
                i_pEv->accept();
            }
            // Remove focus from currently focused object (if any object focused).
            else if( m_pDiagObjFocused != nullptr )
            {
                // If the graphical output of the object changes the corresponding
                // update flags will be invalidated (the bits will be set).
                m_pDiagObjFocused->unsetFocus();

                // No object is longer being focused:
                m_pDiagObjFocused = nullptr;
                m_pDiagObjFocusedByKeyEvent = nullptr;
                m_pDiagObjFocusedByMouseEvent = nullptr;

                // Update graphical output of the diagram.
                // For the unfocused object at least data processing will be executed
                // and the object invalidates the "dirty" rectangle area of the widget:
                m_uUpdateFlags |= EUpdateAll;

                // Trigger paint event
                update();

                // The diagram just "eats" the escape key if an object was focused or selected.
                i_pEv->accept();
            }
            break;
        }
        case Qt::Key_Return:
        case Qt::Key_Enter:
        {
            // The diagram "eats" each enter key.
            i_pEv->accept();

            // If any object is already being edited ..
            if( m_pDiagObjEditing != nullptr )
            {
                // Deselect the object (but keep the focus on it).
                // If the graphical output of the object changes the corresponding
                // update flags will be invalidated (the bits will be set).
                m_pDiagObjEditing->stopEditSession(true); // also inform other objects of necessary pixmap processing

                // No object is longer being edited:
                m_pDiagObjEditing = nullptr;
                m_pDiagObjEditingByKeyEvent = nullptr;
                m_pDiagObjEditingByMouseEvent = nullptr;

                // Update graphical output of the diagram.
                // For the no longer edited object at least data processing will be executed
                // and the object invalidates the "dirty" rectangle area of the widget:
                m_uUpdateFlags |= EUpdateAll;

                // Trigger paint event
                update();
            }
            // If any object is already focused ..
            else if( m_pDiagObjFocused != nullptr )
            {
                // Select the object (and keep the focus on it).
                // If the graphical output of the object changes the corresponding
                // update flags will be invalidated (the bits will be set).
                m_pDiagObjFocused->startEditSession(true); // also inform other objects of necessary pixmap processing

                // Store the edited object:
                m_pDiagObjEditing = m_pDiagObjFocused;
                m_pDiagObjEditingByKeyEvent = m_pDiagObjFocused;
                m_pDiagObjEditingByMouseEvent = nullptr;

                // Update graphical output of the diagram.
                // For the newly edited object at least data processing will be executed
                // and the object invalidates the "dirty" rectangle area of the widget:
                m_uUpdateFlags |= EUpdateAll;

                // Trigger paint event
                update();
            }
            // If neither an object is focused nor selected ...
            else
            {
                // If an object has been recently focused ...
                if( m_pDiagObjFocusedRecently != nullptr )
                {
                    // ... again set the focus on this object.
                    m_pDiagObjFocused = m_pDiagObjFocusedRecently;
                    m_pDiagObjFocusedByKeyEvent = m_pDiagObjFocused;
                    m_pDiagObjEditingByMouseEvent = nullptr;
                }
                // If no object was recently focused ...
                else
                {
                    // Check if any object is focusable. If - set the focus on the first found object.
                    for (CDiagObj* pDiagObj : m_arpDiagObjs)
                    {
                        if( pDiagObj->isFocusable() && pDiagObj->isEditable() )
                        {
                            m_pDiagObjFocused = pDiagObj;
                            break;
                        }
                    }
                }

                // If an object has been focused ..
                if( m_pDiagObjFocused != nullptr )
                {
                    // Store the focused object:
                    m_pDiagObjFocusedByKeyEvent = m_pDiagObjFocused;
                    m_pDiagObjFocusedByMouseEvent = nullptr;
                    m_pDiagObjFocusedRecently = m_pDiagObjFocused;

                    // The focused object need to be updated at last so that no other
                    // object overwrites the graphic output of the focused object:
                    moveDiagObjInPaintList(m_pDiagObjFocused,EDiagObjMoveModeToTop);

                    // If the graphical output of the object changes the corresponding
                    // update flags will be invalidated (the bits will be set).
                    m_pDiagObjFocused->setFocus(true); // also inform other objects of necessary pixmap processing

                    // Update graphical output of the diagram.
                    // For the newly focues object at least data processing will be executed
                    // and the object invalidates the "dirty" rectangle area of the widget:
                    m_uUpdateFlags |= EUpdateAll;

                    // Trigger paint event
                    update();
                }
            }
            break;
        }
        case Qt::Key_Print:
        {
            i_pEv->accept();
            break;
        }
        case Qt::Key_Left:
        case Qt::Key_Down:
        case Qt::Key_Right:
        case Qt::Key_Up:
        {
            double fMoveKeyEventTime_ms = 0.0;
            double fMoveKeyEventTimeDelay_ms = 0.0;
            int    iMoveKeyCurr = i_pEv->key();

            // TODO: Hr. Koehler will, dass mit Up/Down der "Mouse-Cursor" nach
            //       oben und unten verschoben werden kann. Vorläufig wandeln wir
            //       aber Up in Right und Down in Left um.

            if( i_pEv->key() == Qt::Key_Down )
            {
                iMoveKeyCurr = Qt::Key_Left;
            }
            else if( i_pEv->key() == Qt::Key_Up )
            {
                iMoveKeyCurr = Qt::Key_Right;
            }
            if( m_iMoveKeyPrev == -1 )
            {
                m_iMoveKeyPrev = iMoveKeyCurr;
            }
            if( m_bMoveKeyAccelerationEnabled )
            {
                fMoveKeyEventTime_ms = Time::getProcTimeInMilliSec();
                fMoveKeyEventTimeDelay_ms = fMoveKeyEventTime_ms - m_fMoveKeyEventTime_ms;
            }

            // If no object focused or selected ...
            if( m_pDiagObjFocused == nullptr && m_pDiagObjEditing == nullptr )
            {
                // .. the move key event will be ignored by the diagram.
                i_pEv->ignore();
            }

            // If any object focused or selected ...
            else // if( m_pDiagObjFocused != nullptr || m_pDiagObjEditing != nullptr )
            {
                bool bTrcMoveKeyActive =
                    m_pTrcAdminObjEvents == nullptr ?
                        false : m_pTrcAdminObjEvents->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave);
                QString strTrcMsgMoveKey;

                // .. the move key event will be handled by the diagram.
                i_pEv->accept();

                if( m_pDiagObjEditing != nullptr )
                {
                    if( m_bMoveKeyAccelerationEnabled )
                    {
                        if( bTrcMoveKeyActive )
                        {
                            strTrcMsgMoveKey += "CurrTime=" + QString::number(fMoveKeyEventTime_ms,'f',0) + "[ms], ";
                            strTrcMsgMoveKey += "DelayTime=" + QString::number(fMoveKeyEventTimeDelay_ms,'f',0) + "[ms], ";
                        }
                        if( m_fMoveKeyRepCount < 1.0 )
                        {
                            m_fMoveKeyRepCount = 1.0;
                        }
                        if( fMoveKeyEventTimeDelay_ms >= m_iMoveKeyAccelerationTimeout_ms )
                        {
                            // Reset ..
                            m_iMoveKeySpeedUpDelayRepCount = 0;
                            m_fMoveKeyRepCount = 1.0;
                            if( bTrcMoveKeyActive )
                            {
                                strTrcMsgMoveKey += "Delay > TimeOut: Reset speed to " + QString::number(m_fMoveKeyRepCount,'f',0);
                            }
                        }
                        else if( fMoveKeyEventTimeDelay_ms >= m_iMoveKeyAccelerationTimeoutSlowDown_ms )
                        {
                            // Slow down ..
                            m_iMoveKeySpeedUpDelayRepCount = 0;
                            m_fMoveKeyRepCount /= m_fMoveKeyAccelerationFactor;
                            if( m_fMoveKeyRepCount < 1.0 )
                            {
                                m_fMoveKeyRepCount = 1.0;
                            }
                            if( bTrcMoveKeyActive )
                            {
                                strTrcMsgMoveKey += "Delay > TimeoutSlowDown: Slow down speed to " + QString::number(m_fMoveKeyRepCount,'f',0);
                            }
                        }
                        else if( fMoveKeyEventTimeDelay_ms >= m_iMoveKeyAccelerationTimeoutSpeedUp_ms )
                        {
                            m_iMoveKeySpeedUpDelayRepCount = 0;

                            // If the direction of the move events changed ...
                            if( m_iMoveKeyPrev != iMoveKeyCurr )
                            {
                                // Slow down ..
                                m_fMoveKeyRepCount /= m_fMoveKeyAccelerationFactor;
                                if( m_fMoveKeyRepCount < 1.0 )
                                {
                                    m_fMoveKeyRepCount = 1.0;
                                }
                                if( bTrcMoveKeyActive )
                                {
                                    strTrcMsgMoveKey += "Delay > TimeoutSpeedUp AND DirectionChanged: Slow down speed to " + QString::number(m_fMoveKeyRepCount,'f',0);
                                }
                            }
                            // If the direction of the move events did not change ...
                            else
                            {
                                // Keep the speed ..
                                if( bTrcMoveKeyActive )
                                {
                                    strTrcMsgMoveKey += "Delay > TimeoutSpeedUp: Keep speed by " + QString::number(m_fMoveKeyRepCount,'f',0);
                                }
                            }
                        }
                        else if( fMoveKeyEventTimeDelay_ms < m_iMoveKeyAccelerationTimeoutSpeedUp_ms )
                        {
                            // If the direction of the move events changed ...
                            if( m_iMoveKeyPrev != iMoveKeyCurr )
                            {
                                m_iMoveKeySpeedUpDelayRepCount = 0;

                                // Slow down ..
                                m_fMoveKeyRepCount /= m_fMoveKeyAccelerationFactor;
                                if( m_fMoveKeyRepCount < 1.0 )
                                {
                                    m_fMoveKeyRepCount = 1.0;
                                }
                                if( bTrcMoveKeyActive )
                                {
                                    strTrcMsgMoveKey += "Delay < TimeoutSpeedUp AND DirectionChanged: Slow down speed to " + QString::number(m_fMoveKeyRepCount,'f',0);
                                }
                            }
                            // If the direction of the move events did not change ...
                            else
                            {
                                if( m_iMoveKeySpeedUpDelayRepCount < 0 )
                                {
                                    m_iMoveKeySpeedUpDelayRepCount = 0;
                                }
                                m_iMoveKeySpeedUpDelayRepCount++;

                                // If the number of move events exceeded the configured number of events ...
                                if( m_iMoveKeySpeedUpDelayRepCount >= m_iMoveKeyAccelarationSpeedUpDelayRepCount )
                                {
                                    // Speed up ..
                                    m_iMoveKeySpeedUpDelayRepCount = 0;

                                    if( m_pMoveKeyAccelerationDiagScale != nullptr && m_pMoveKeyAccelerationDiagScale->isScaleValid() )
                                    {
                                        EScaleDir scaleDir;
                                        double    fScaleMin;
                                        double    fScaleMax;
                                        double    fScaleRange;
                                        bool      bValIsValid = false;
                                        double    fVal = 0.0;
                                        double    fRes = 0.0;

                                        scaleDir = m_pMoveKeyAccelerationDiagScale->getScaleDir();
                                        bValIsValid = m_pDiagObjEditing->getVal(scaleDir).isValid();

                                        if( bValIsValid )
                                        {
                                            fVal = m_pDiagObjEditing->getVal(scaleDir).getVal();
                                            fRes = m_pDiagObjEditing->getValRes(scaleDir,fVal);
                                        }
                                        else
                                        {
                                            fRes = m_pDiagObjEditing->getValRes(scaleDir);
                                        }
                                        fScaleMin = m_pMoveKeyAccelerationDiagScale->getScale().m_fMin;
                                        fScaleMax = m_pMoveKeyAccelerationDiagScale->getScale().m_fMax;
                                        fScaleRange = fabs(fScaleMax-fScaleMin);

                                        if( bValIsValid && m_pMoveKeyAccelerationDiagScale->getSpacing() == ESpacing::Logarithmic )
                                        {
                                            fScaleMin = log10(fScaleMin);
                                            fScaleMax = log10(fScaleMax);
                                            fScaleRange = fabs(fScaleMax-fScaleMin);
                                            fRes = log10(fVal+fRes)-log10(fVal);
                                        }

                                        m_fMoveKeyRepCount *= m_fMoveKeyAccelerationFactor;
                                        if( fRes*m_fMoveKeyRepCount > m_fMoveKeyAccelerationMaxStepScaleFactor*fScaleRange )
                                        {
                                            m_fMoveKeyRepCount = (m_fMoveKeyAccelerationMaxStepScaleFactor*fScaleRange)/fRes;
                                            if( bTrcMoveKeyActive )
                                            {
                                                strTrcMsgMoveKey += "Delay < TimeoutSpeedUp AND DelayRepCount >= SpeedUpRepCount ";
                                                strTrcMsgMoveKey += "(ScaleRange=" + QString::number(fScaleRange) + ", Res=" + QString::number(fRes) + "): Speed limit reached at " + QString::number(m_fMoveKeyRepCount,'f',0);
                                            }
                                        }
                                        else
                                        {
                                            if( bTrcMoveKeyActive )
                                            {
                                                strTrcMsgMoveKey += "Delay < TimeoutSpeedUp AND DelayRepCount >= SpeedUpRepCount ";
                                                strTrcMsgMoveKey += "(ScaleRange=" + QString::number(fScaleRange) + ", Res=" + QString::number(fRes) + "): Speed up to " + QString::number(m_fMoveKeyRepCount,'f',0);
                                            }
                                        }
                                    }
                                    else
                                    {
                                        m_fMoveKeyRepCount *= m_fMoveKeyAccelerationFactor;
                                        if( bTrcMoveKeyActive )
                                        {
                                            strTrcMsgMoveKey += "Delay < TimeoutSpeedUp AND DelayRepCount >= SpeedUpRepCount: Speed up to " + QString::number(m_fMoveKeyRepCount,'f',0);
                                        }
                                    }
                                }
                                // If the number of move events did not exceed the configured number of events ...
                                else
                                {
                                    // Keep the speed ..
                                    if( bTrcMoveKeyActive )
                                    {
                                        strTrcMsgMoveKey += "Delay < TimeoutSpeedUp AND DelayRepCount < SpeedUpRepCount: Keep speed by " + QString::number(m_fMoveKeyRepCount,'f',0);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        m_fMoveKeyRepCount = 1.0;
                    }

                    if( bTrcMoveKeyActive )
                    {
                        mthTracer.trace(strTrcMsgMoveKey);
                    }

                    processMoveKeyEvent(iMoveKeyCurr);

                    // Key events seem to have a higher priority than paint events. As we want
                    // to trace the current position of the edited object the paintEvent will
                    // be explicitly triggered.
                    #if QT_VERSION >= 0x040100
                    QPaintEvent* pPaintEv = new QPaintEvent( QRect(QPoint(0,0),m_size) );
                    #else
                    QPaintEvent* pPaintEv = new QPaintEvent( QRect(QPoint(0,0),m_size), false );
                    #endif
                    QApplication::postEvent(this, pPaintEv);
                    pPaintEv = nullptr; //lint !e423

                } // if( m_pDiagObjEdited != nullptr )

                else // if( m_pDiagObjFocused != nullptr )
                {
                    if( bTrcMoveKeyActive )
                    {
                        mthTracer.trace(strTrcMsgMoveKey);
                    }

                    // Selecting other objects don't need to be accelerated.
                    m_fMoveKeyRepCount = 1.0;
                    processMoveKeyEvent(iMoveKeyCurr);
                    m_fMoveKeyRepCount = 0.0;
                }
            } // if( m_pDiagObjFocused != nullptr || m_pDiagObjEditing != nullptr )

            if( m_bMoveKeyAccelerationEnabled )
            {
                m_fMoveKeyEventTime_ms = fMoveKeyEventTime_ms;
            }
            m_iMoveKeyPrev = iMoveKeyCurr;
            break;
        }

        default:
        {
            break;
        }

    } // switch( i_pEv->key() )

} // keyPressEvent

//lint -e1961
//------------------------------------------------------------------------------
void CWdgtDiagram::keyReleaseEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObjEvents, EMethodTraceDetailLevel::ArgsNormal))
    {
        strTrcMsg = qKeyCode2Str(i_pEv->key());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "keyReleaseEvent",
        /* strAddInfo   */ strTrcMsg );

    i_pEv->ignore();

} // keyReleaseEvent
//lint +e1961

//------------------------------------------------------------------------------
void CWdgtDiagram::mousePressEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ "" );

    // Maybe also parents are interested in the mouse event ...
    i_pEv->ignore();

    delete m_pMenuContext;
    m_pMenuContext = nullptr;

    if( i_pEv->button() == Qt::LeftButton )
    {
        // Remove focus from currently focused object (if any object focused).
        if( m_pDiagObjFocused != nullptr )
        {
            // If the graphical output of the object changes the corresponding
            // update flags will be invalidated (the bits will be set).
            m_pDiagObjFocused->unsetFocus();

            // No object is longer being focused:
            m_pDiagObjFocused = nullptr;
            m_pDiagObjFocusedByKeyEvent = nullptr;
            m_pDiagObjFocusedByMouseEvent = nullptr;
        }

        // Unselect currently selected object (if any object selected).
        if( m_pDiagObjEditing != nullptr )
        {
            // If the graphical output of the object changes the corresponding
            // update flags will be invalidated (the bits will be set).
            m_pDiagObjEditing->stopEditSession();

            // No object is longer being focused:
            m_pDiagObjEditing = nullptr;
            m_pDiagObjEditingByKeyEvent = nullptr;
            m_pDiagObjEditingByMouseEvent = nullptr;
        }

        // Check if any object has been hit. If - start editing the object.
        if (m_arpDiagObjs.size() > 0)
        {
            for (int idx = m_arpDiagObjs.size()-1; idx >= 0; --idx)
            {
                CDiagObj* pDiagObj = m_arpDiagObjs[idx];
                if( pDiagObj->isFocusable() && pDiagObj->isEditable() && pDiagObj->isHit(i_pEv->pos()) )
                {
                    // Store the focused object:
                    m_pDiagObjFocused = pDiagObj;
                    m_pDiagObjFocusedByMouseEvent = pDiagObj;
                    m_pDiagObjFocusedRecently = pDiagObj;

                    // Store the edited object:
                    m_pDiagObjEditing = pDiagObj;
                    m_pDiagObjEditingByMouseEvent = pDiagObj;

                    // The focused object need to be updated at last so that no other
                    // object overwrites the graphic output of the focused object:
                    moveDiagObjInPaintList(m_pDiagObjFocused,EDiagObjMoveModeToTop);

                    // If the graphical output of the object changes the corresponding
                    // update flags will be invalidated (the bits will be set).
                    m_pDiagObjFocused->startEditSession(i_pEv->pos()); // implicitly sets the focus on this object
                    break;
                }
            }
        }

        // If none of the diagram objects has been hit and if zooming is enabled ...
        if( m_pDiagObjFocused == nullptr && m_pDiagObjEditing == nullptr && isZoomingEnabled() )
        {
            if( m_rectPartCenter.contains(i_pEv->pos()) )
            {
                m_bIsZooming = true;
                m_ptZoomStart = i_pEv->pos();
                m_ptZoomEnd = i_pEv->pos();
                m_rectZoom = Math::calcRect(m_ptZoomStart,m_ptZoomEnd);
            }
        }

        // Update graphical output of the diagram.
        // For the newly focused or edited object at least data processing will be executed
        // and the object invalidates the "dirty" rectangle area of the widget:
        m_uUpdateFlags |= EUpdateAll;

        // Trigger paint event
        update();
    }

} // mousePressEvent

//------------------------------------------------------------------------------
void CWdgtDiagram::mouseReleaseEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "mouseReleaseEvent",
        /* strAddInfo   */ "" );

    // Maybe also parents are interested in the mouse event ...
    i_pEv->ignore();

    delete m_pMenuContext;
    m_pMenuContext = nullptr;

    if( i_pEv->button() == Qt::LeftButton )
    {
        // Unselect currently selected object.
        if( m_pDiagObjEditing != nullptr )
        {
            // If the graphical output of the object changes the corresponding
            // update flags will be invalidated (the bits will be set).
            m_pDiagObjEditing->stopEditSession();

            // No object is longer being focused:
            m_pDiagObjEditing = nullptr;
            m_pDiagObjEditingByKeyEvent = nullptr;
            m_pDiagObjEditingByMouseEvent = nullptr;

            // Update graphical output of the diagram.
            // For the no longer edited object at least data processing will be executed
            // and the object invalidates the "dirty" rectangle area of the widget:
            m_uUpdateFlags |= EUpdateAll;

            // Trigger paint event
            update();
        }

        if( m_bIsZooming )
        {
            QPoint ptZoomEnd = i_pEv->pos();

            if( ptZoomEnd.x() < m_rectPartCenter.left() )
            {
                ptZoomEnd.setX(m_rectPartCenter.left());
            }
            if( ptZoomEnd.x() > m_rectPartCenter.right() )
            {
                ptZoomEnd.setX(m_rectPartCenter.right());
            }
            if( ptZoomEnd.y() < m_rectPartCenter.top() )
            {
                ptZoomEnd.setY(m_rectPartCenter.top());
            }
            if( ptZoomEnd.y() > m_rectPartCenter.bottom() )
            {
                ptZoomEnd.setY(m_rectPartCenter.bottom());
            }
            m_ptZoomEnd = ptZoomEnd;

            m_rectZoom = Math::calcRect(m_ptZoomStart,m_ptZoomEnd);

            if( !isZoomingEnabled(EScaleDir::X) )
            {
                m_rectZoom.setLeft(m_rectPartCenter.left());
                m_rectZoom.setWidth(m_rectPartCenter.width());
            }
            else if( !isZoomingEnabled(EScaleDir::Y) )
            {
                m_rectZoom.setTop(m_rectPartCenter.top());
                m_rectZoom.setHeight(m_rectPartCenter.height());
            }

            if( m_rectZoom.width() >= 2 && m_rectZoom.height() >= 2 )
            {
                for (CDiagScale* pDiagScale : m_arpDiagScales)
                {
                    switch( pDiagScale->getScaleDir() )
                    {
                        case EScaleDir::X:
                        {
                            pDiagScale->zoomIn(m_rectZoom.left(),m_rectZoom.right());
                            break;
                        }
                        case EScaleDir::Y:
                        {
                            pDiagScale->zoomIn(m_rectZoom.bottom(),m_rectZoom.top());
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                }
                m_iZoomCount++;
            }
            else
            {
                invalidate(nullptr,EUpdatePixmap);
            }
            m_bIsZooming = false;

            // Update graphical output of the diagram (here paint event by diagram itself).
            update();
        }
    }
    else if( i_pEv->button() == Qt::RightButton )
    {
        if( m_bContextPopupMenuEnabled && m_pPixmap != nullptr )
        {
            #if QT_VERSION >= 0x040100
            //m_pMenuContext = new QMenu(this,"PopupMenuContextDiagram");
            //m_pMenuContext->addAction( "Save", dynamic_cast<QWidget*>(this), SLOT(popupMenuContextItemSaveActivated()) );
            //m_pMenuContext->addAction( "Print", dynamic_cast<QWidget*>(this), SLOT(popupMenuContextItemPrintActivated()) );
            #else
            m_pMenuContext = new QPopupMenu(this,"PopupMenuContextDiagram");
            m_pMenuContext->insertItem( "Save", dynamic_cast<QWidget*>(this), SLOT(popupMenuContextItemSaveActivated()) );
            m_pMenuContext->insertItem( "Print", dynamic_cast<QWidget*>(this), SLOT(popupMenuContextItemPrintActivated()) );
            m_pMenuContext->popup(mapToGlobal(i_pEv->pos()));
            #endif
        }
    }

} // mouseReleaseEvent

//------------------------------------------------------------------------------
void CWdgtDiagram::mouseDoubleClickEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "mouseDoubleClickEvent",
        /* strAddInfo   */ "" );

    // Maybe also parents are interested in the mouse event ...
    i_pEv->ignore();

    if( m_iZoomCount > 0 )
    {
        for (CDiagScale* pDiagScale : m_arpDiagScales)
        {
            pDiagScale->zoomOut();
        }
        m_iZoomCount--;

        // Update graphical output of the diagram.
        // For the changed objects at least data processing will be executed and the
        // objects are invalidating the "dirty" rectangle area of the widget:
        m_uUpdateFlags |= EUpdateAll;

        // Trigger paint event
        update();
    }

} // mouseDoubleClickEvent

//------------------------------------------------------------------------------
void CWdgtDiagram::mouseMoveEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    // Maybe also parents are interested in the mouse event ...
    i_pEv->ignore();

    // If an object has been selected by mouse click ...
    if( m_pDiagObjEditingByMouseEvent != nullptr )
    {
        if (areMethodCallsActive(m_pTrcAdminObjEvents, EMethodTraceDetailLevel::ArgsNormal))
        {
            strTrcMsg  = "DiagObjEdited: ";
            strTrcMsg += m_pDiagObjEditingByMouseEvent->getObjName();
        }

        CMethodTracer mthTracer(
            /* pAdminObj    */ m_pTrcAdminObjEvents,
            /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strMethod    */ "mouseMoveEvent",
            /* strAddInfo   */ strTrcMsg );

        // ... move this object to the new mouse position.
        CDiagObjMoveEvent* pEv = new CDiagObjMoveEvent( CDiagObjMoveEvent::ETypePointAbs, i_pEv->pos() );

        // If the graphical output of the object changes the corresponding
        // update flags will be invalidated (the bits will be set), data
        // processing will be executed by the object and the object invalidates
        // the "dirty" rectangle area of the widget:
        m_pDiagObjEditingByMouseEvent->moveEvent(pEv);
        delete pEv;
        pEv = nullptr;

        // Trigger paintEvent to redraw the diagram.
        update();
    }

    // If the diagram will be zoomed ..
    else if( m_bIsZooming )
    {
        if (areMethodCallsActive(m_pTrcAdminObjEvents, EMethodTraceDetailLevel::ArgsNormal))
        {
            strTrcMsg = "Zooming";
        }

        CMethodTracer mthTracer(
            /* pAdminObj    */ m_pTrcAdminObjEvents,
            /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strMethod    */ "mouseMoveEvent",
            /* strAddInfo   */ strTrcMsg );

        QRect rectZoomPrev = m_rectZoom;

        // Calculate new output region of zooming rectangle.
        m_ptZoomEnd = i_pEv->pos();
        m_rectZoom = Math::calcRect(m_ptZoomStart,m_ptZoomEnd);

        // The objects and the diagram need to redraw the pixmap. Widget processing
        // will be realized by invalidating the zooming rectangle border lines below.
        invalidate(nullptr,EUpdatePixmap);

        // Invalidate old and new output region of zooming rectangle
        // (corresponds to update process depth "Widget").

        // Trigger paintEvent to redraw the diagram.
        // -----------------------------------------

        // Old output region:
        update( QRect(rectZoomPrev.left(),rectZoomPrev.top(),rectZoomPrev.width(),1) );
        update( QRect(rectZoomPrev.left(),rectZoomPrev.bottom(),rectZoomPrev.width(),1) );
        update( QRect(rectZoomPrev.left(),rectZoomPrev.top(),1,rectZoomPrev.height()) );
        update( QRect(rectZoomPrev.right(),rectZoomPrev.top(),1,rectZoomPrev.height()) );

        // New output region:
        update( QRect(m_rectZoom.left(),m_rectZoom.top(),m_rectZoom.width(),1) );
        update( QRect(m_rectZoom.left(),m_rectZoom.bottom(),m_rectZoom.width(),1) );
        update( QRect(m_rectZoom.left(),m_rectZoom.top(),1,m_rectZoom.height()) );
        update( QRect(m_rectZoom.right(),m_rectZoom.top(),1,m_rectZoom.height()) );

        // Trigger paintEvent to redraw the diagram.
        //update();
    }

    else if( m_pDiagObjFocusedByKeyEvent == nullptr )
    {
        // Check if any object has been hit. If - set focus on this object.
        if( m_pDiagObjFocusedByMouseEvent != nullptr )
        {
            // If the graphical output of the object changes the corresponding
            // update flags will be invalidated (the bits will be set).
            m_pDiagObjFocusedByMouseEvent->unsetFocus();
            m_pDiagObjFocusedByMouseEvent = nullptr;
        }

        for (CDiagObj* pDiagObj : m_arpDiagObjs)
        {
            if( pDiagObj->isFocusable() && pDiagObj->isEditable() && pDiagObj->isHit(i_pEv->pos()) )
            {
                m_pDiagObjFocusedByMouseEvent = pDiagObj;
                break;
            }
        }
        if( m_pDiagObjFocusedByMouseEvent != nullptr )
        {
            // If the graphical output of the object changes the corresponding
            // update flags will be invalidated (the bits will be set).
            m_pDiagObjFocusedByMouseEvent->setFocus(i_pEv->pos());
            m_pDiagObjFocused = m_pDiagObjFocusedByMouseEvent;
        }
        else
        {
            m_pDiagObjFocused = nullptr;
        }

        // Trigger paintEvent to redraw the diagram.
        update();
    }

} // mouseMoveEvent

//------------------------------------------------------------------------------
void CWdgtDiagram::paintEvent( QPaintEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "paintEvent",
        /* strAddInfo   */ "" );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

    // If the paint event occurred while the diagram is not up to date ...
    if( m_uUpdateFlags & EUpdateLayout )
    {
        updateLayout();
    }
    if( m_uUpdateFlags & EUpdateData )
    {
        updateData();
    }
    if( m_uUpdateFlags & EUpdatePixmap )
    {
        updatePixmap();
    }

    // We are going to clear the update widget flags of the objects as the paint
    // event might have been triggered on the resize event and for better
    // performance the resize event has just executed the layout processing.
    for (CDiagObj* pDiagObj : m_arpDiagObjs)
    {
        pDiagObj->validate(EUpdateWidget);
    }

    QPainter painter(this);

    // Widget processing:
    if( m_pPixmap != nullptr )
    {
        #if QT_VERSION >= 0x040100
        //QSize sizePixmap = m_pPixmap->size();
        painter.drawPixmap(
            /* rectTarget */ i_pEv->rect(),
            /* pixmap     */ *m_pPixmap,
            /* rectSource */ i_pEv->rect() );
        #else
        bitBlt(
            /* pPaintDeviceDst  */ this,
            /* xLeftDst         */ i_pEv->rect().left(),
            /* yTopDst          */ i_pEv->rect().top(),
            /* pPaintDeviceSrc  */ m_pPixmap,
            /* xLeftSrc         */ i_pEv->rect().left(),
            /* yTopSrc          */ i_pEv->rect().top(),
            /* cxWidthSrc       */ i_pEv->rect().width(),
            /* cyHeightSrc      */ i_pEv->rect().height() );
        #endif
    }

    // Mark current process depth as executed (reset bit):
    validate(EUpdateWidget);

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

    emit painted(this);

} // paintEvent

//------------------------------------------------------------------------------
void CWdgtDiagram::resizeEvent( QResizeEvent* /*i_pEv*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "resizeEvent",
        /* strAddInfo   */ "" );

    QSize sizePrev     = m_size;
    bool  bSizeChanged = false;

    if( m_size.width() != static_cast<QWidget*>(this)->width()
     || m_size.height() != static_cast<QWidget*>(this)->height() )
    {
        if( static_cast<QWidget*>(this)->width() >= minimumWidth()
         && static_cast<QWidget*>(this)->height() >= minimumHeight() )
        {
            bSizeChanged = true;
            m_size = static_cast<QWidget*>(this)->size();
        }
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        QString strTrcMsg;
        QRect   rectGeometry = geometry();
        strTrcMsg  = "SizeChanged=";
        strTrcMsg += QString::number(bSizeChanged) + ", ";
        strTrcMsg += "Geometry[x,y,w,h]=";
        strTrcMsg += QString::number(rectGeometry.x()) + ",";
        strTrcMsg += QString::number(rectGeometry.y()) + ",";
        strTrcMsg += QString::number(rectGeometry.width()) + ",";
        strTrcMsg += QString::number(rectGeometry.height());
        mthTracer.trace(strTrcMsg);
    }
    if( bSizeChanged )
    {
        int cxWidthDist  = m_size.width() - sizePrev.width();
        int cyHeightDist = m_size.height() - sizePrev.height();

        // Before updating the layout the scale objects will be provided with their
        // (probable) new extents. Those extent may change later on if the real
        // size of the diagram areas will be calculated as some objects may change
        // their size according to the number and position of the scale division lines.
        // But setting the (probable) scale geometry in advance may save time ...
        m_rectPartCenter.setWidth( m_rectPartCenter.width() + cxWidthDist );
        m_rectPartCenter.setHeight( m_rectPartCenter.height() + cyHeightDist );

        // It is NOT sufficient to mark layout processing as necessary.
        // Even if on executing the layout process the geometry of the objects
        // might be changed (if necessary) and the necessary update flags would
        // be set anyway. To recalculate the geometry of the objects some of those
        // objects need to recalculate their internal data in advance to provide
        // the correct sizeHint for the diagram. E.g. on resizing the diagram the
        // number of division lines might change and therefore an axis label object
        // need to change the number of digits to scale the diagram.
        invalidate(nullptr,EUpdateLayoutDataPixmapWidget);

        // There is no need to trigger the paint event here as the paint
        // event will follow anyway upon a resize event.
        //update();
    }

} // resizeEvent

//------------------------------------------------------------------------------
void CWdgtDiagram::focusInEvent( QFocusEvent* i_pEv )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "focusInEvent",
        /* strAddInfo   */ "" );

    QWidget::focusInEvent(i_pEv);

    emit gotFocus(this);

} // focusInEvent

//------------------------------------------------------------------------------
void CWdgtDiagram::focusOutEvent( QFocusEvent* i_pEv )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "focusOutEvent",
        /* strAddInfo   */ "" );

    QWidget::focusOutEvent(i_pEv);

    // Unselect currently selected object (if any object selected) but keep the
    // focus on the selected object.
    if( m_pDiagObjEditing != nullptr )
    {
        // If the graphical output of the object changes the corresponding
        // update flags will be invalidated (the bits will be set).
        m_pDiagObjEditing->stopEditSession();

        // No object is longer being edited:
        m_pDiagObjEditing = nullptr;
        m_pDiagObjEditingByKeyEvent = nullptr;
        m_pDiagObjEditingByMouseEvent = nullptr;

        // Update graphical output of the diagram.
        // For the no longer edited object at least data processing will be executed
        // and the object invalidates the "dirty" rectangle area of the widget:
        m_uUpdateFlags |= EUpdateAll;

        // Trigger paint event
        update();
    }

    // Remove focus from currently focused object (if any object focused).
    if( m_pDiagObjFocused != nullptr )
    {
        // If the graphical output of the object changes the corresponding
        // update flags will be invalidated (the bits will be set).
        m_pDiagObjFocused->unsetFocus();

        // No object is focused anymore:
        m_pDiagObjFocused = nullptr;
        m_pDiagObjFocusedByKeyEvent = nullptr;
        m_pDiagObjFocusedByMouseEvent = nullptr;

        // Update graphical output of the diagram.
        // For the unfocused object at least data processing will be executed
        // and the object invalidates the "dirty" rectangle area of the widget:
        m_uUpdateFlags |= EUpdateAll;

        // Trigger paint event
        update();
    }

    emit lostFocus(this);

} // focusOutEvent

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtDiagram::processMoveKeyEvent( int i_iKey )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObjEvents, EMethodTraceDetailLevel::ArgsNormal))
    {
        strTrcMsg = qKeyCode2Str(i_iKey);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "processMoveKeyEvent",
        /* strAddInfo   */ strTrcMsg );

    bool    bHandled = false;
    bool    bTrcMoveKeyActive = mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave);
    QString strTrcMsgMoveKey;

    switch( i_iKey )
    {
        case Qt::Key_Left:
        {
            // If any object is selected ...
            if( m_pDiagObjEditing != nullptr )
            {
                if( bTrcMoveKeyActive )
                {
                    strTrcMsgMoveKey  = "Moving selected object by ";
                    strTrcMsgMoveKey += QString::number(m_fMoveKeyRepCount,'f',0) + " steps left";
                    mthTracer.trace(strTrcMsgMoveKey);
                }

                // ... move this object to the left.
                CDiagObjMoveEvent* pEv = new CDiagObjMoveEvent(CDiagObjMoveEvent::ETypeResRel,-m_fMoveKeyRepCount,0);

                // If the graphical output of the object changes the corresponding
                // update flags will be invalidated (the bits will be set).
                m_pDiagObjEditing->moveEvent(pEv,true); // also inform other objects of necessary pixmap processing

                // Update graphical output of the diagram.
                // For the edited object at least data processing will be executed
                // and the object invalidates the "dirty" rectangle area of the widget:
                m_uUpdateFlags |= EUpdateAll;

                // Trigger paint event
                update();

                delete pEv;
                pEv = nullptr;
                bHandled = true;
            }

            // If any object is focused ...
            else if( m_pDiagObjFocused != nullptr )
            {
                if( bTrcMoveKeyActive )
                {
                    strTrcMsgMoveKey = "Set focus on previous object";
                    mthTracer.trace(strTrcMsgMoveKey);
                }

                // ... set focus on previous object.
                CDiagObj* pDiagObj = nullptr;
                int idx = m_arpDiagObjs.indexOf(m_pDiagObjFocused);
                if (idx == 0)
                {
                    pDiagObj = m_arpDiagObjs.last();
                }
                else
                {
                    pDiagObj = m_arpDiagObjs[idx-1];
                }
                while( pDiagObj != nullptr && pDiagObj != m_pDiagObjFocused )
                {
                    if( pDiagObj->isFocusable() )
                    {
                        // If the graphical output of the object changes the corresponding
                        // update flags will be invalidated (the bits will be set).
                        // Please note that on setting the focus to the new object the other
                        // objects will be informed by the new focused object of the necessary
                        // pixmap processing.
                        m_pDiagObjFocused->unsetFocus(false);

                        // Store the newly focused object
                        m_pDiagObjFocused = pDiagObj;
                        m_pDiagObjFocusedRecently = m_pDiagObjFocused;

                        // The focused object need to be updated at last so that no other
                        // object overwrites the graphic output of the focused object:
                        moveDiagObjInPaintList(m_pDiagObjFocused, EDiagObjMoveModeToTop);

                        // If the graphical output of the object changes the corresponding
                        // update flags will be invalidated (the bits will be set), data
                        // processing will be executed by the object and the object invalidates
                        // the "dirty" rectangle area of the widget:
                        m_pDiagObjFocused->setFocus(true); // also inform other objects of necessary pixmap processing

                        // Update graphical output of the diagram.
                        // For the unfocused and newly focused object at least data processing
                        // will be executed and the object invalidates the "dirty" rectangle
                        // area of the widget:
                        m_uUpdateFlags |= EUpdateAll;

                        // Trigger paint event
                        update();
                        break;
                    }
                    idx = m_arpDiagObjs.indexOf(pDiagObj);
                    if (idx == 0)
                    {
                        pDiagObj = m_arpDiagObjs.last();
                    }
                    else
                    {
                        pDiagObj = m_arpDiagObjs[idx-1];
                    }
                }
                bHandled = true;
            }
            break;
        }

        case Qt::Key_Right:
        {
            // If any object is selected ...
            if( m_pDiagObjEditing != nullptr )
            {
                if( bTrcMoveKeyActive )
                {
                    strTrcMsgMoveKey  = "Moving selected object by ";
                    strTrcMsgMoveKey += QString::number(m_fMoveKeyRepCount,'f',0) + " steps right";
                    mthTracer.trace(strTrcMsgMoveKey);
                }

                // ... move this object to the right.
                CDiagObjMoveEvent* pEv = new CDiagObjMoveEvent(CDiagObjMoveEvent::ETypeResRel,m_fMoveKeyRepCount,0);

                // If the graphical output of the object changes the corresponding
                // update flags will be invalidated (the bits will be set).
                m_pDiagObjEditing->moveEvent(pEv,true); // also inform other objects of necessary pixmap processing

                // Update graphical output of the diagram.
                // For the edited object at least data processing will be executed and
                // the object invalidates the "dirty" rectangle area of the widget:
                m_uUpdateFlags |= EUpdateAll;

                // Trigger paint event
                update();

                delete pEv;
                pEv = nullptr;
                bHandled = true;
            }

            // If any object is focused ...
            else if( m_pDiagObjFocused != nullptr )
            {
                if( bTrcMoveKeyActive )
                {
                    strTrcMsgMoveKey = "Set focus on next object";
                    mthTracer.trace(strTrcMsgMoveKey);
                }

                // ... set focus on next object.
                CDiagObj* pDiagObj = nullptr;
                int idx = m_arpDiagObjs.indexOf(m_pDiagObjFocused);
                if (idx == m_arpDiagObjs.size()-1)
                {
                    pDiagObj = m_arpDiagObjs.first();
                }
                else
                {
                    pDiagObj = m_arpDiagObjs[idx+1];
                }
                while( pDiagObj != nullptr && pDiagObj != m_pDiagObjFocused )
                {
                    if( pDiagObj->isFocusable() )
                    {
                        // If the graphical output of the object changes the corresponding
                        // update flags will be invalidated (the bits will be set).
                        // Please note that on setting the focus to the new object the other
                        // objects will be informed by the new focused object of the necessary
                        // pixmap processing.
                        m_pDiagObjFocused->unsetFocus(false);

                        // Store the newly focused object:
                        m_pDiagObjFocused = pDiagObj;
                        m_pDiagObjFocusedRecently = m_pDiagObjFocused;

                        // The focused object need to be updated at last so that no other
                        // object overwrites the graphic output of the focused object:
                        moveDiagObjInPaintList(m_pDiagObjFocused,EDiagObjMoveModeToTop);

                        // If the graphical output of the object changes the corresponding
                        // update flags will be invalidated (the bits will be set).
                        m_pDiagObjFocused->setFocus(true); // also inform other objects of necessary pixmap processing

                        // Update graphical output of the diagram.
                        // For the unfocused and newly focused object at least data processing
                        // will be executed and the object invalidates the "dirty" rectangle
                        // area of the widget:
                        m_uUpdateFlags |= EUpdateAll;

                        // Trigger paint event
                        update();
                        break;
                    }
                    idx = m_arpDiagObjs.indexOf(pDiagObj);
                    if (idx == m_arpDiagObjs.size()-1)
                    {
                        pDiagObj = m_arpDiagObjs.first();
                    }
                    else
                    {
                        pDiagObj = m_arpDiagObjs[idx+1];
                    }
                }
                bHandled = true;
            }
            break;
        }

        default:
        {
            break;
        }

    } // switch( Key )

    return bHandled;

} // processMoveKeyEvent

/*==============================================================================
protected: // overidables of base class CDataDiagram
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagram::updatePixmap()
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updatePixmap",
        /* strAddInfo   */ "" );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

    if( m_pPixmap == nullptr )
    {
        return;
    }

    // This method is called by objects controlling the diagram to update the
    // content of the diagram.

    // Let the diagram objects draw into the pixmap:
    CPixmapDiagram::updatePixmap();

    // If the diagram will be zoomed ..
    if( m_bIsZooming )
    {
        QPainter painterPixmap(m_pPixmap);
        QPen     pen;
        pen.setColor(Qt::gray);
        pen.setStyle(Qt::DashLine);
        painterPixmap.setPen(pen);
        painterPixmap.drawRect(QRect(m_ptZoomStart,m_ptZoomEnd));
    }

    // Mark current process depth as executed (reset bit):
    validate(EUpdatePixmap);

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

} // updatePixmap

////------------------------------------------------------------------------------
//void CWdgtDiagram::updateWidget()
////------------------------------------------------------------------------------
//{
//    QString strTrcMsg;
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjUpdate,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod    */ "updateWidget",
//        /* strAddInfo   */ "" );
//
//    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
//    {
//        strTrcMsg  = "OldUpdFlags=";
//        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
//        mthTracer.trace(strTrcMsg);
//    }
//
//    // This method is called by objects controlled by the diagram to update the
//    // content of the diagram.
//
//    // Please note that only if by invalidating the flag (setting the bit) in the
//    // internal update flags and on requesting the process depth by this update
//    // method the corresponding update process will be executed.
//
//    CDiagObj* pDiagObj = m_pDiagObjPaintFirst;
//    while( pDiagObj != nullptr )
//    {
//        // Please also note that also invisible objects need to mark their dirty
//        // rectangles during widget processing.
//        pDiagObj->update(EUpdateWidget);
//
//        pDiagObj = pDiagObj->m_pDiagObjPaintNext;
//    }
//
//    if( m_pFrameStylePartCenter != nullptr )
//    {
//        // Trigger paint event to update content of widget
//        //--------------------------------------------------
//
//        if( m_rectFramePartCenterTopLineInv.width() > 0 && m_rectFramePartCenterTopLineInv.height() > 0 )
//        {
//            update(m_rectFramePartCenterTopLineInv);
//        }
//        if( m_rectFramePartCenterBottomLineInv.width() > 0 && m_rectFramePartCenterBottomLineInv.height() > 0 )
//        {
//            update(m_rectFramePartCenterBottomLineInv);
//        }
//        if( m_rectFramePartCenterLeftLineInv.width() > 0 && m_rectFramePartCenterLeftLineInv.height() > 0 )
//        {
//            update(m_rectFramePartCenterLeftLineInv);
//        }
//        if( m_rectFramePartCenterRightLineInv.width() > 0 && m_rectFramePartCenterRightLineInv.height() > 0 )
//        {
//            update(m_rectFramePartCenterRightLineInv);
//        }
//
//        m_rectFramePartCenterTopLineInv.setWidth(0);
//        m_rectFramePartCenterTopLineInv.setHeight(0);
//        m_rectFramePartCenterBottomLineInv.setWidth(0);
//        m_rectFramePartCenterBottomLineInv.setHeight(0);
//        m_rectFramePartCenterLeftLineInv.setWidth(0);
//        m_rectFramePartCenterLeftLineInv.setHeight(0);
//        m_rectFramePartCenterRightLineInv.setWidth(0);
//        m_rectFramePartCenterRightLineInv.setHeight(0);
//    }
//
//    // The diagram itself draws frames (around the whole diagram widget or around
//    // the center part). If the center part changes its size also the frame need
//    // to be marked as a "dirty" rectangle.
//
//    // Mark current process depth as executed (reset bit):
//    validate(EUpdateWidget);
//
//    if( m_pLblTipOfTheDay != nullptr && m_pLblTipOfTheDay->isVisible() )
//    {
//        QRect rctTipOfTheDay;
//        rctTipOfTheDay.setLeft( m_rectPartCenter.left() + m_iTipOfTheDayIndentLeft );
//        rctTipOfTheDay.setTop( m_rectPartCenter.top() + m_iTipOfTheDayIndentTop );
//        rctTipOfTheDay.setRight( m_rectPartCenter.right() - m_iTipOfTheDayIndentRight );
//        rctTipOfTheDay.setBottom( m_rectPartCenter.bottom() - m_iTipOfTheDayIndentBottom );
//        m_pLblTipOfTheDay->setGeometry(rctTipOfTheDay);
//    }
//
//    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
//    {
//        strTrcMsg  = "NewUpdFlags=";
//        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
//        mthTracer.trace(strTrcMsg);
//    }
//
//} // updateWidget

/*==============================================================================
protected slots: // overridable instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagram::popupMenuContextItemSaveActivated( void )
//------------------------------------------------------------------------------
{
    if( m_pPixmap != nullptr )
    {
        QString      strSaveFileName;
        QString      strSelectedFilter;
        EImageFormat imageFormat = EImageFormatUndefined;
        QFileDialog* pFileDialog = nullptr;

        if( m_strCurrentFileDir.length() == 0 )
        {
            #if QT_VERSION >= 0x040100
            m_strCurrentFileDir = QDir::current().absolutePath();
            #else
            m_strCurrentFileDir = QDir::current().absPath();
            #endif
        }
        else
        {
            QDir dirCurrent = QDir(m_strCurrentFileDir);
            if( !dirCurrent.exists() )
            {
                #if QT_VERSION >= 0x040100
                m_strCurrentFileDir = QDir::current().absolutePath();
                #else
                m_strCurrentFileDir = QDir::current().absPath();
                #endif
            }
        }

        #if QT_VERSION < 0x050000

        #if QT_VERSION >= 0x040100

        pFileDialog = new QFileDialog( this, "Choose a file name to save under" );
        pFileDialog->setDirectory(m_strCurrentFileDir);
        pFileDialog->setFileMode(QFileDialog::AnyFile);
        #if QT_VERSION >= 0x040400
        pFileDialog->setFilter(QDir::Files);
        pFileDialog->setNameFilter("*.xpm *.jpeg *.jpg *.bmp");
        #else
        pFileDialog->setFilter("*.xpm *.jpeg *.jpg *.bmp");
        #endif

        if( pFileDialog->exec() == QDialog::Accepted )
        {
            if( pFileDialog->selectedFiles().count() == 1 )
            {
                strSaveFileName = pFileDialog->selectedFiles().at(0);
                QFileInfo fileInfoSaveFile(strSaveFileName);
                strSaveFileName = fileInfoSaveFile.absolutePath() + "/" + fileInfoSaveFile.completeBaseName();
                m_strCurrentFileDir = fileInfoSaveFile.absolutePath();
                strSelectedFilter = pFileDialog->selectedFilter();
                imageFormat = getImageFormat(strSelectedFilter);
            }
        }

        #else

        pFileDialog = new QFileDialog( this, "PopupMenuContextSaveDiagramFileDialog", true );
        pFileDialog->setCaption("Choose a file name to save under");
        pFileDialog->setDir(m_strCurrentFileDir);
        pFileDialog->setMode(QFileDialog::AnyFile);
        pFileDialog->addFilter("*.XPM");
        pFileDialog->addFilter("*.JPEG *.JPG");
        pFileDialog->addFilter("*.BMP");

        if( pFileDialog->exec() == QDialog::Accepted )
        {
            strSaveFileName = pFileDialog->selectedFile();
            QFileInfo fileInfoSaveFile(strSaveFileName);
            strSaveFileName = fileInfoSaveFile.dirPath() + "/" + fileInfoSaveFile.baseName(true);
            m_strCurrentFileDir = fileInfoSaveFile.dirPath();
            strSelectedFilter = pFileDialog->selectedFilter();
            imageFormat = getImageFormat(strSelectedFilter);
        }
        #endif

        #endif // #if QT_VERSION < 0x050000

        delete pFileDialog;
        pFileDialog = nullptr;

        if( strSaveFileName.length() > 0 && imageFormat != EImageFormatUndefined )
        {
            save( strSaveFileName, imageFormat );
        }

    } // if( pixmap != nullptr )


} // popupMenuContextItemSaveActivated

//------------------------------------------------------------------------------
void CWdgtDiagram::popupMenuContextItemPrintActivated( void )
//------------------------------------------------------------------------------
{
    if( m_pPrinter != nullptr )
    {
        #if QT_VERSION < 0x050000
        #if QT_VERSION >= 0x040100
        QPrintDialog printDialog(m_pPrinter,this);
        if( printDialog.exec() == QDialog::Accepted)
        {
            print(m_pPrinter);
        }
        #else
        if( m_pPrinter->setup(this) )
        {
            print(m_pPrinter);
        }
        #endif
        #endif // #if QT_VERSION < 0x050000
    }

} // popupMenuContextItemPrintActivated

/*==============================================================================
protected: // auxiliary instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagram::emit_diagItemAdded(const QString& i_strClassName, const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strClassName + "::" + i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_diagItemAdded",
        /* strAddInfo   */ strMthInArgs );

    emit diagItemAdded(i_strClassName, i_strObjName);
}

//------------------------------------------------------------------------------
void CWdgtDiagram::emit_diagItemRemoved(const QString& i_strClassName, const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strClassName + "::" + i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_diagItemRemoved",
        /* strAddInfo   */ strMthInArgs );

    emit diagItemRemoved(i_strClassName, i_strObjName);
}

/*==============================================================================
protected: // overridables of base class CDataDiagram for emitting the signals
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagram::emit_aboutToBeDestroyed(const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_aboutToBeDestroyed",
        /* strAddInfo   */ strMthInArgs );

    emit aboutToBeDestroyed(i_strObjName);
}

//------------------------------------------------------------------------------
void CWdgtDiagram::emit_diagScaleAdded(const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_diagScaleAdded",
        /* strAddInfo   */ strMthInArgs );

    emit diagScaleAdded(i_strObjName);
    emit_diagItemAdded("CDiagScale", i_strObjName);
}

//------------------------------------------------------------------------------
void CWdgtDiagram::emit_diagScaleRemoved(const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_diagScaleRemoved",
        /* strAddInfo   */ strMthInArgs );

    emit diagScaleRemoved(i_strObjName);
    emit_diagItemRemoved("CDiagScale", i_strObjName);
}

//------------------------------------------------------------------------------
void CWdgtDiagram::emit_diagTraceAdded(const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_diagTraceAdded",
        /* strAddInfo   */ strMthInArgs );

    emit diagTraceAdded(i_strObjName);
    emit_diagItemAdded("CDiagTrace", i_strObjName);
}

//------------------------------------------------------------------------------
void CWdgtDiagram::emit_diagTraceRemoved(const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_diagTraceRemoved",
        /* strAddInfo   */ strMthInArgs );

    emit diagTraceRemoved(i_strObjName);
    emit_diagItemRemoved("CDiagTrace", i_strObjName);
}

//------------------------------------------------------------------------------
void CWdgtDiagram::emit_diagObjAdded(const QString& i_strClassName, const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strClassName + "::" + i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_diagObjAdded",
        /* strAddInfo   */ strMthInArgs );

    emit diagObjAdded(i_strClassName, i_strObjName);
    emit_diagItemAdded(i_strClassName, i_strObjName);
}

//------------------------------------------------------------------------------
void CWdgtDiagram::emit_diagObjRemoved(const QString& i_strClassName, const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strClassName + "::" + i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_diagObjRemoved",
        /* strAddInfo   */ strMthInArgs );

    emit diagObjRemoved(i_strClassName, i_strObjName);
    emit_diagItemRemoved(i_strClassName, i_strObjName);
}
