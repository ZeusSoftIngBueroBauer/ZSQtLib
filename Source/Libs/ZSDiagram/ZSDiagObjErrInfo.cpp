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

#include "ZSDiagram/ZSDiagObjErrInfo.h"
#include "ZSDiagram/ZSDiagramProcData.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtCore/qtimer.h>
#include <QtGui/qpainter.h>

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Diagram;
using namespace ZS::PhysVal;


/*******************************************************************************
class CDiagObjErrInfo : public CDiagObj
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDiagObjErrInfo::CDiagObjErrInfo(
    const QString& i_strObjName,
    ELayoutPos     i_layoutPos ) :
//------------------------------------------------------------------------------
    CDiagObj(
        /* strClassName */ CDiagObjErrInfo::ClassName(),
        /* strObjName   */ i_strObjName,
        /* pDiagTrace   */ nullptr,
        /* layoutPos    */ i_layoutPos ),
    m_errResultInfoCurr(),
    m_errResultInfoPrev(),
    m_pTimer(nullptr),
    //m_ariTimeout_ms[EResultSeverityCount]
    //m_arcolText[EResultSeverityCount]
    //m_arfntText[EResultSeverityCount]
    m_iMarginTop(0),
    m_iMarginBottom(0),
    m_iMarginLeft(0),
    m_iMarginRight(0),
    m_textOrientation(ETextOrientationLeft2Right),
    m_strText(),
    m_rectTextCurr(),
    m_rectTextPrev()
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    m_pTimer = new QTimer(this);
    m_pTimer->setSingleShot(true);

    QObject::connect(
        m_pTimer, &QTimer::timeout,
        this, &CDiagObjErrInfo::timeout);

    for( int idxSev = 0; idxSev < EResultSeverityCount; idxSev++ )
    {
        m_ariTimeout_ms[idxSev] = 0;
        m_arcolText[idxSev] = Qt::black;
        //m_arfntText[idxSev]; // with default ctor
    }
    m_ariTimeout_ms[EResultSeverityWarning] = 5000; // default: 5 seconds

} // ctor

//------------------------------------------------------------------------------
CDiagObjErrInfo::~CDiagObjErrInfo()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    //m_errResultInfoCurr;
    //m_errResultInfoPrev;
    m_pTimer = nullptr;
    memset(m_ariTimeout_ms, 0x00, EResultSeverityCount*sizeof(m_ariTimeout_ms[0]));
    memset(m_arfntText, 0x00, EResultSeverityCount*sizeof(m_arcolText[0]));
    memset(m_arcolText, 0x00, EResultSeverityCount*sizeof(m_arfntText[0]));
    m_iMarginTop = 0;
    m_iMarginBottom = 0;
    m_iMarginLeft = 0;
    m_iMarginRight = 0;
    m_textOrientation = static_cast<ETextOrientation>(0);
    //m_strText;
    //m_rectTextCurr;
    //m_rectTextPrev;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDiagObjErrInfo::setErrResultInfo( const ZS::System::SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setErrResultInfo",
        /* strAddInfo   */ "" );

    m_errResultInfoCurr = i_errResultInfo;
    invalidate(EUpdateDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
ZS::System::SErrResultInfo CDiagObjErrInfo::getErrResultInfo() const
//------------------------------------------------------------------------------
{
    return m_errResultInfoCurr;
}

//------------------------------------------------------------------------------
void CDiagObjErrInfo::setTimeOut( ZS::System::EResultSeverity i_severity, int i_iTimeout_ms )
//------------------------------------------------------------------------------
{
    int idxSevMin = i_severity;
    int idxSevMax = i_severity;
    int idxSev;

    if( i_severity == EResultSeverityCount )
    {
        idxSevMin = 0;
        idxSevMax = EResultSeverityCount-1;
    }
    for( idxSev = idxSevMin; idxSev <= idxSevMax; idxSev++ )
    {
        m_ariTimeout_ms[idxSev] = i_iTimeout_ms;
    }
}

//------------------------------------------------------------------------------
int CDiagObjErrInfo::getTimeOut( ZS::System::EResultSeverity i_severity ) const
//------------------------------------------------------------------------------
{
    return m_ariTimeout_ms[i_severity];
}

//------------------------------------------------------------------------------
void CDiagObjErrInfo::setColText( ZS::System::EResultSeverity i_severity, const QColor& i_col )
//------------------------------------------------------------------------------
{
    int idxSevMin = i_severity;
    int idxSevMax = i_severity;
    int idxSev;

    if( i_severity == EResultSeverityCount )
    {
        idxSevMin = 0;
        idxSevMax = EResultSeverityCount-1;
    }
    for( idxSev = idxSevMin; idxSev <= idxSevMax; idxSev++ )
    {
        m_arcolText[idxSev] = i_col;
    }
    invalidate(EUpdatePixmapWidget,true);
}

//------------------------------------------------------------------------------
QColor CDiagObjErrInfo::getColText( ZS::System::EResultSeverity i_severity ) const
//------------------------------------------------------------------------------
{
    return m_arcolText[i_severity];
}

//------------------------------------------------------------------------------
void CDiagObjErrInfo::setFont( ZS::System::EResultSeverity i_severity, const QFont& i_fnt )
//------------------------------------------------------------------------------
{
    int idxSevMin = i_severity;
    int idxSevMax = i_severity;
    int idxSev;

    if( i_severity == EResultSeverityCount )
    {
        idxSevMin = 0;
        idxSevMax = EResultSeverityCount-1;
    }
    for( idxSev = idxSevMin; idxSev <= idxSevMax; idxSev++ )
    {
        m_arfntText[idxSev] = i_fnt;
    }
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
QFont CDiagObjErrInfo::getFont( ZS::System::EResultSeverity i_severity ) const
//------------------------------------------------------------------------------
{
    return m_arfntText[i_severity];
}

//------------------------------------------------------------------------------
void CDiagObjErrInfo::setMarginTop( int i_iMargin )
//------------------------------------------------------------------------------
{
    m_iMarginTop = i_iMargin;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
int CDiagObjErrInfo::getMarginTop() const
//------------------------------------------------------------------------------
{
    return m_iMarginTop;
}

//------------------------------------------------------------------------------
void CDiagObjErrInfo::setMarginBottom( int i_iMargin )
//------------------------------------------------------------------------------
{
    m_iMarginBottom = i_iMargin;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
int CDiagObjErrInfo::getMarginBottom() const
//------------------------------------------------------------------------------
{
    return m_iMarginBottom;
}

//------------------------------------------------------------------------------
void CDiagObjErrInfo::setMarginLeft( int i_iMargin )
//------------------------------------------------------------------------------
{
    m_iMarginLeft = i_iMargin;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
int CDiagObjErrInfo::getMarginLeft() const
//------------------------------------------------------------------------------
{
    return m_iMarginLeft;
}

//------------------------------------------------------------------------------
void CDiagObjErrInfo::setMarginRight( int i_iMargin )
//------------------------------------------------------------------------------
{
    m_iMarginRight = i_iMargin;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
int CDiagObjErrInfo::getMarginRight() const
//------------------------------------------------------------------------------
{
    return m_iMarginRight;
}

//------------------------------------------------------------------------------
void CDiagObjErrInfo::setTextOrientation( ETextOrientation i_textOrientation )
//------------------------------------------------------------------------------
{
    m_textOrientation = i_textOrientation;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
ETextOrientation CDiagObjErrInfo::getTextOrientation() const
//------------------------------------------------------------------------------
{
    return m_textOrientation;
}

//------------------------------------------------------------------------------
QString CDiagObjErrInfo::getText() const
//------------------------------------------------------------------------------
{
    return m_strText;
}

/*==============================================================================
public: // overridables of base class CDiagObj
==============================================================================*/

//------------------------------------------------------------------------------
QSize CDiagObjErrInfo::sizeHint()
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

    if( isVisible() )
    {
        // Calculate extent of label:
        QFontMetrics fntmtr(m_arfntText[m_errResultInfoCurr.getSeverity()]);
        QString      strDummyLabel = "Oy,2";
        QRect        rectText = fntmtr.boundingRect(strDummyLabel);

        switch( m_textOrientation )
        {
            case ETextOrientationBottom2Top:
            case ETextOrientationTop2Bottom:
            {
                rectText.setWidth(cyHeight);
                rectText.setHeight(cxWidth);
                cxWidth  = rectText.width() + m_iMarginLeft + m_iMarginRight;
                cyHeight = rectText.height() + m_iMarginTop + m_iMarginBottom;
                break;
            }
            case ETextOrientationLeft2Right:
            case ETextOrientationRight2Left:
            default:
            {
                cxWidth  = rectText.width() + m_iMarginLeft + m_iMarginRight;
                cyHeight = rectText.height() + m_iMarginTop + m_iMarginBottom;
                break;
            }
        }
    } // if( isVisible() )

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
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
CDiagObj* CDiagObjErrInfo::clone( CDataDiagram* i_pDiagramTrg ) const
//------------------------------------------------------------------------------
{
    if( i_pDiagramTrg == nullptr )
    {
        return nullptr;
    }

    CDiagObjErrInfo* pDiagObjCloned = new CDiagObjErrInfo(
        /* strObjName */ m_strObjName,
        /* layoutPos  */ m_layoutPos );

    int idxSev;

    // Members from base class CDiagObj:
    pDiagObjCloned->m_rectContent = m_rectContent;
    pDiagObjCloned->m_bAdjustContentRect2DiagPartCenter = m_bAdjustContentRect2DiagPartCenter;
    pDiagObjCloned->m_bVisible = m_bVisible;
    pDiagObjCloned->m_state = m_state;
    pDiagObjCloned->m_bIsFocusable = m_bIsFocusable;
    pDiagObjCloned->m_bIsEditable = m_bIsEditable;

    // Members from this class:
    pDiagObjCloned->m_errResultInfoCurr = m_errResultInfoCurr;
    pDiagObjCloned->m_errResultInfoPrev = m_errResultInfoPrev;

    for( idxSev = 0; idxSev < EResultSeverityCount; idxSev++ )
    {
        pDiagObjCloned->m_ariTimeout_ms[idxSev] = m_ariTimeout_ms[idxSev];
        pDiagObjCloned->m_arcolText[idxSev] = m_arcolText[idxSev];
        pDiagObjCloned->m_arfntText[idxSev] = m_arfntText[idxSev];
    }
    pDiagObjCloned->m_iMarginTop = m_iMarginTop;
    pDiagObjCloned->m_iMarginBottom = m_iMarginBottom;
    pDiagObjCloned->m_iMarginLeft = m_iMarginLeft;
    pDiagObjCloned->m_iMarginRight = m_iMarginRight;
    pDiagObjCloned->m_textOrientation = m_textOrientation;
    pDiagObjCloned->m_rectTextCurr = m_rectTextCurr;
    pDiagObjCloned->m_rectTextPrev = m_rectTextPrev;

    i_pDiagramTrg->addDiagObj(pDiagObjCloned);

    return pDiagObjCloned;

} // clone

//------------------------------------------------------------------------------
void CDiagObjErrInfo::update( unsigned int i_uUpdateFlags, QPaintDevice* i_pPaintDevice )
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

    // If the internal data structures need to be updated ..
    if( i_uUpdateFlags & EUpdateData && m_uUpdateFlags & EUpdateData )
    {
        mthTracer.trace("Processing Data", ELogDetailLevel::Debug);

        // As a matter of fact there is no sense in adding a label object to
        // a diagram just designed to analyze data.
        if( isVisible() && m_pDiagram->getUpdateType() >= EDiagramUpdateTypePixmap )
        {
            if( m_errResultInfoCurr != m_errResultInfoPrev )
            {
                ZS::System::SErrResult  errResult = m_errResultInfoCurr.getErrResult();
                EResultSeverity severity = errResult.getSeverity();

                if( m_pTimer->isActive() )
                {
                    m_pTimer->stop();
                }
                if( severity == EResultSeveritySuccess )
                {
                    m_strText = QString();
                    m_rectTextCurr = QRect(0,0,0,0);
                }
                else
                {
                    QString strAddErrInfo = m_errResultInfoCurr.getAddErrInfoDscr();
                    QRect   rectText;

                    m_strText = strAddErrInfo;

                    if( strAddErrInfo.length() > 0 )
                    {
                        // Calculate extent of label:
                        QFontMetrics fntmtr(m_arfntText[severity]);

                        rectText = fntmtr.boundingRect(m_strText);

                        int cxWidth  = rectText.width();
                        int cyHeight = rectText.height();

                        rectText.setWidth( cxWidth + m_iMarginLeft + m_iMarginRight );
                        rectText.setHeight( cyHeight + m_iMarginTop + m_iMarginBottom );

                        switch( m_textOrientation )
                        {
                            case ETextOrientationBottom2Top:
                            case ETextOrientationTop2Bottom:
                            {
                                rectText.setWidth(cyHeight);
                                rectText.setHeight(cxWidth);
                                cxWidth  = rectText.width();
                                cyHeight = rectText.height();
                                break;
                            }
                            case ETextOrientationLeft2Right:
                            case ETextOrientationRight2Left:
                            default:
                            {
                                break;
                            }
                        } // switch( m_textOrientation )

                        switch( m_layoutPos )
                        {
                            case ELayoutPos::Top:
                            {
                                rectText.moveTop( m_rectContent.top() );
                                rectText.moveLeft( m_rectContent.center().x() - cxWidth/2 );
                                break;
                            }
                            case ELayoutPos::Bottom:
                            {
                                rectText.moveTop( m_rectContent.bottom() - cyHeight ); //lint !e834 .. so schwierig ist eine Subtraktion doch auch wieder nicht
                                rectText.moveLeft( m_rectContent.center().x() - cxWidth/2 );
                                break;
                            }
                            case ELayoutPos::Left:
                            {
                                rectText.moveTop( m_rectContent.center().y() - cyHeight/2 );
                                rectText.moveLeft( m_rectContent.left() );
                                break;
                            }
                            case ELayoutPos::Right:
                            {
                                rectText.moveTop( m_rectContent.center().y() - cyHeight/2 );
                                rectText.moveLeft( m_rectContent.right() - cxWidth ); //lint !e834 .. so schwierig ist eine Subtraktion doch auch wieder nicht
                                break;
                            }
                            case ELayoutPos::Center:
                            {
                                rectText.moveTop( m_rectContent.center().y() - cyHeight/2 );
                                rectText.moveLeft( m_rectContent.center().x() - cxWidth/2 );
                                break;
                            }
                            default:
                            {
                                break;
                            }
                        } // switch( m_layoutPos )
                    }
                    m_rectTextCurr = rectText;

                } // if( severity != EResultSeveritySuccess )

            } // if( m_errResultInfoCurr != m_errResultInfoPrev )

        } // if( isVisible() && m_pDataDiagram->getUpdateType() >= EDiagramUpdateTypePixmap )

        // Mark current process depth as executed (reset bit):
        validate(EUpdateData);

    } // if( EUpdateData )

    // If recalculation of bitmap is needed ..
    if( i_uUpdateFlags & EUpdatePixmap && m_uUpdateFlags & EUpdatePixmap && i_pPaintDevice != nullptr )
    {
        mthTracer.trace("Processing Pixmap", ELogDetailLevel::Debug);

        if( isVisible() && !m_strText.isEmpty() )
        {
            QPainter painter(i_pPaintDevice);

            ZS::System::SErrResult  errResult = m_errResultInfoCurr.getErrResult();
            EResultSeverity severity = errResult.getSeverity();

            painter.setFont(m_arfntText[severity]);
            painter.setPen(m_arcolText[severity]);

            switch( m_textOrientation )
            {
                case ETextOrientationLeft2Right:
                {
                    painter.drawText( m_rectTextCurr, Qt::AlignVCenter|Qt::AlignHCenter, m_strText ); //lint !e655
                    break;
                }
                case ETextOrientationRight2Left:
                {
                    #if QT_VERSION < 0x050000
                    QMatrix matrix;
                    #else
                    QTransform matrix;
                    #endif
                    painter.save();
                    matrix.translate( static_cast<double>(m_rectTextCurr.center().x()), static_cast<double>(m_rectTextCurr.center().y()) );
                    matrix.rotate(-180.0);
                    #if QT_VERSION < 0x050000
                    painter.setWorldMatrix(matrix);
                    #else
                    painter.setWorldTransform(matrix);
                    #endif
                    painter.drawText(
                        /* x   */ -m_rectTextCurr.height()/2,
                        /* y   */ m_rectTextCurr.width()/2,
                        /* str */ m_strText );
                    painter.restore();
                    break;
                }
                case ETextOrientationBottom2Top:
                {
                    #if QT_VERSION < 0x050000
                    QMatrix matrix;
                    #else
                    QTransform matrix;
                    #endif
                    painter.save();
                    matrix.translate( static_cast<double>(m_rectTextCurr.center().x()), static_cast<double>(m_rectTextCurr.center().y()) );
                    matrix.rotate(-90.0);
                    #if QT_VERSION < 0x050000
                    painter.setWorldMatrix(matrix);
                    #else
                    painter.setWorldTransform(matrix);
                    #endif
                    painter.drawText(
                        /* x   */ -m_rectTextCurr.height()/2,
                        /* y   */ m_rectTextCurr.width()/2,
                        /* str */ m_strText );
                    painter.restore();
                    break;
                }
                case ETextOrientationTop2Bottom:
                {
                    #if QT_VERSION < 0x050000
                    QMatrix matrix;
                    #else
                    QTransform matrix;
                    #endif
                    painter.save();
                    matrix.translate( static_cast<double>(m_rectTextCurr.center().x()), static_cast<double>(m_rectTextCurr.center().y()) );
                    matrix.rotate(90.0);
                    #if QT_VERSION < 0x050000
                    painter.setWorldMatrix(matrix);
                    #else
                    painter.setWorldTransform(matrix);
                    #endif
                    painter.drawText(
                        /* x   */ -m_rectTextCurr.height()/2,
                        /* y   */ m_rectTextCurr.width()/2,
                        /* str */ m_strText );
                    painter.restore();
                    break;
                }
                default:
                {
                    break;
                }

            } // switch( m_textOrientation )
        } // if( isVisible() && !m_strText.isEmpty() )

        // Mark current process depth as executed (reset bit):
        validate(EUpdatePixmap);

    } // if( EUpdatePixmap )

    // If the widget need to be updated ..
    if( i_uUpdateFlags & EUpdateWidget && m_uUpdateFlags & EUpdateWidget )
    {
        mthTracer.trace("Processing Widget", ELogDetailLevel::Debug);

        // Invalidate output region of the diagram object to update (repaint) content of diagram.
        if( m_rectTextPrev.isValid() && m_rectTextPrev != m_rectTextCurr )
        {
            m_pDiagram->update(this, m_rectTextPrev);
        }
        if( !isVisible() )
        {
            m_rectTextCurr.setWidth(0);
            m_rectTextCurr.setHeight(0);
        }
        if( m_rectTextCurr.isValid() )
        {
            m_pDiagram->update(this, m_rectTextCurr);
        }
        m_rectTextPrev = m_rectTextCurr;

        if( m_errResultInfoCurr != m_errResultInfoPrev )
        {
            ZS::System::SErrResult  errResultCurr = m_errResultInfoCurr.getErrResult();
            EResultSeverity severityCurr  = errResultCurr.getSeverity();

            if( m_pTimer->isActive() )
            {
                m_pTimer->stop();
            }
            if( m_ariTimeout_ms[severityCurr] > 0 )
            {
                m_pTimer->start(m_ariTimeout_ms[severityCurr]);
            }
            m_errResultInfoPrev = m_errResultInfoCurr;
        }

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

/*==============================================================================
protected: // overridables of base class CDiagObj
==============================================================================*/

////------------------------------------------------------------------------------
//void CDiagObjErrInfo::onReceivedError( ZS::System::CMsgErr* i_pMsgErr )
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod    */ "onReceivedError",
//        /* strAddInfo   */ "" );
//
//    if( i_pMsgErr != nullptr )
//    {
//        setErrResultInfo( i_pMsgErr->getErrResultInfo() );
//    }
//}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDiagObjErrInfo::timeout()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "timeout",
        /* strAddInfo   */ "" );

    setErrResultInfo(ErrResultSuccess);

    m_pDiagram->update(this, m_rectContent);
}
