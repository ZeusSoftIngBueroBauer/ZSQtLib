/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer, Germany
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

#include <QtCore/qsettings.h>
#include <QtCore/qthread.h>
#include <QtCore/qtimer.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qlayout.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qtabwidget.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qtabwidget.h>
#endif

#include "WidgetCentral.h"
#include "App.h"

#include "ZSIpcLog/ZSIpcLogClient.h"
#include "ZSIpcLogGUI/ZSIpcLogWdgt.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Log;
using namespace ZS::Log::GUI;
using namespace ZS::Apps::Products::LogClient;


/*******************************************************************************
class CWidgetCentral : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWidgetCentral::CWidgetCentral(
    CIpcLogClient*  i_pLogClient,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_pLyt(nullptr),
    m_pWdgtLog(nullptr)
{
    setObjectName("CentralWidget");

    // Create user controls
    //----------------------

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    // Trace Method List Widget
    //-------------------------

    m_pWdgtLog = new CWdgtLog(i_pLogClient);
    m_pLyt->addWidget(m_pWdgtLog);

    if( !QObject::connect(
        /* pObjSender   */ m_pWdgtLog,
        /* szSignal     */ SIGNAL(progressBarConnectDblClicked()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onProgressBarConnectDblClicked()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CWidgetCentral::~CWidgetCentral()
//------------------------------------------------------------------------------
{
    m_pLyt = nullptr;
    m_pWdgtLog = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWidgetCentral::findText()
//------------------------------------------------------------------------------
{
    if( m_pWdgtLog != nullptr )
    {
        m_pWdgtLog->findText();
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWidgetCentral::onProgressBarConnectDblClicked()
//------------------------------------------------------------------------------
{
    emit progressBarConnectDblClicked();
}