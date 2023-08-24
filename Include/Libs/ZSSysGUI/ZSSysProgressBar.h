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

#ifndef ZSSysGUI_ProgressBar_h
#define ZSSysGUI_ProgressBar_h

#include <QtCore/qstring.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qprogressbar.h>
#else
#include <QtWidgets/qprogressbar.h>
#endif

#include "ZSSysGUI/ZSSysGUIDllMain.h"

class QTimer;

namespace ZS
{
namespace System
{
namespace GUI
{
//******************************************************************************
class ZSSYSGUIDLL_API CProgressBar : public QProgressBar
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CProgressBar( QWidget* i_pWdgtParent = nullptr );
    virtual ~CProgressBar();
signals:
    void durationPassed( QObject* i_pProgressBar );
public: // instance methods
    void setLabelText( const QString& i_strText );
    QString labelText() const { return m_strLabelText; }
public: // instance methods
    void setDurationInMs( int i_iDuration_ms );
    int getDurationInMs() const { return m_iDuration_ms; }
    void setIncrementInMs( int i_iIncrement_ms );
    int getIncrementInMs() const { return m_iIncrement_ms; }
    void setTimeElapsedInMs( int i_iElapsed_ms );
    int getTimeElapsedInMs() const { return m_iElapsed_ms; }
public: // instance methods
    void start();
    void stop();
    void reset();
    bool isActive() const;
public slots:
    void increment();
protected: // overridables of base class QProgressBar
    virtual void paintEvent( QPaintEvent* i_pEv );
protected: // instance members
    QTimer* m_pTimer;
    QString m_strLabelText;
    int     m_iDuration_ms;
    int     m_iIncrement_ms;
    int     m_iElapsed_ms;

}; // class CProgressBar

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_ProgressBar_h
