/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#include <QtCore/qtimer.h>
#include <QtGui/qevent.h>
#include <QtGui/qpainter.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qlayout.h>
#else
#include <QtWidgets/qlayout.h>
#endif

#include "ZSSysGUI/ZSSysProgressBar.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CProgressBar : public QProgressBar
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CProgressBar::CProgressBar( QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QProgressBar(i_pWdgtParent),
    m_pTimer(nullptr),
    m_strLabelText(),
    m_iDuration_ms(10000),
    m_iIncrement_ms(100),
    m_iElapsed_ms(0)
{
    m_pTimer = new QTimer(this);

    if( !QObject::connect(
        /* pObjSender   */ m_pTimer,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(increment()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CProgressBar::~CProgressBar()
//------------------------------------------------------------------------------
{
    m_pTimer = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CProgressBar::setLabelText( const QString& i_strText )
//------------------------------------------------------------------------------
{
    m_strLabelText = i_strText;

    update();

} // setLabelText

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CProgressBar::setDurationInMs( int i_iDuration_ms )
//------------------------------------------------------------------------------
{
    if( m_pTimer->isActive() )
    {
        m_pTimer->stop();
    }
    m_iDuration_ms = i_iDuration_ms;

} // setDurationInMs

//------------------------------------------------------------------------------
void CProgressBar::setIncrementInMs( int i_iIncrement_ms )
//------------------------------------------------------------------------------
{
    if( m_pTimer->isActive() )
    {
        m_pTimer->stop();
    }
    m_iIncrement_ms = i_iIncrement_ms;

} // setIncrementInMs

//------------------------------------------------------------------------------
void CProgressBar::setTimeElapsedInMs( int i_iElapsed_ms )
//------------------------------------------------------------------------------
{
    m_iElapsed_ms = i_iElapsed_ms;

    setValue(m_iElapsed_ms);

    if( m_iElapsed_ms >= m_iDuration_ms )
    {
        int iElapsedInMsBeforeSignal = m_iElapsed_ms;
        int iDurationInMsBeforeSignal = m_iDuration_ms;

        emit durationPassed(this);

        // Progress bar may have been stopped by a client receiving the "durationPassed" signal ..
        if( m_pTimer->isActive() && iElapsedInMsBeforeSignal == m_iElapsed_ms && iDurationInMsBeforeSignal == m_iDuration_ms )
        {
            m_iElapsed_ms /= 2;
        }
    }

} // setTimeElapsedInMs

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CProgressBar::start()
//------------------------------------------------------------------------------
{
    if( m_pTimer->isActive() )
    {
        m_pTimer->stop();
    }
    m_iElapsed_ms = 0;
    setRange(0,m_iDuration_ms);
    m_pTimer->start(m_iIncrement_ms);

} // start

//------------------------------------------------------------------------------
void CProgressBar::stop()
//------------------------------------------------------------------------------
{
    if( m_pTimer->isActive() )
    {
        m_pTimer->stop();
    }

} // stop

//------------------------------------------------------------------------------
void CProgressBar::reset()
//------------------------------------------------------------------------------
{
    if( m_pTimer->isActive() )
    {
        m_pTimer->stop();
    }
    m_iElapsed_ms = 0;
    setValue(0);

} // reset

//------------------------------------------------------------------------------
bool CProgressBar::isActive() const
//------------------------------------------------------------------------------
{
    return m_pTimer->isActive();
}

/*==============================================================================
public slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CProgressBar::increment()
//------------------------------------------------------------------------------
{
    m_iElapsed_ms += m_iIncrement_ms;

    setValue(m_iElapsed_ms);

    if( m_iElapsed_ms >= m_iDuration_ms )
    {
        int iElapsedInMsBeforeSignal = m_iElapsed_ms;
        int iDurationInMsBeforeSignal = m_iDuration_ms;

        emit durationPassed(this);

        // Progress bar may have been stopped by a client receiving the "durationPassed" signal ..
        if( m_pTimer->isActive() && iElapsedInMsBeforeSignal == m_iElapsed_ms && iDurationInMsBeforeSignal == m_iDuration_ms )
        {
            m_iElapsed_ms /= 2;
        }
    }

} // increment

/*==============================================================================
protected: // overridables of base class QProgressBar
==============================================================================*/

//------------------------------------------------------------------------------
void CProgressBar::paintEvent( QPaintEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QProgressBar::paintEvent(i_pEv);

    if( !m_strLabelText.isEmpty() )
    {
        QPainter painter(this);

        QRect rct = i_pEv->rect();

        QMargins mrgsContents = contentsMargins();

        rct.setLeft( rct.left() + mrgsContents.left() + 5 );
        rct.setRight( rct.right() + mrgsContents.right() );
        rct.setTop( rct.top() + mrgsContents.top() );
        rct.setBottom( rct.bottom() + mrgsContents.bottom() );

        painter.drawText( rct, Qt::AlignVCenter|Qt::AlignLeft,m_strLabelText );

    } // if( !m_strLabelText.isEmpty() )

} // paintEvent
