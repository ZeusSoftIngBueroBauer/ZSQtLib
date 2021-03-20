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

#include <QtCore/qsettings.h>
#include <QtGui/qevent.h>

#if QT_VERSION < 0x050000
#include <QtGui/qgroupbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qscrollarea.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qtabwidget.h>
#include <QtGui/qtextedit.h>
#include <QtGui/qtreeview.h>
#else
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qscrollarea.h>
#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qtabwidget.h>
#include <QtWidgets/qtextedit.h>
#include <QtWidgets/qtreeview.h>
#endif

#include "WidgetCentral.h"
#include "App.h"
#include "PCMemoryWdgt.h"

#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTime.h"
//#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::Apps::Products::PCResourceMonitor;


/*******************************************************************************
class CWidgetCentral : public QWidget
*******************************************************************************/

/*==============================================================================
private: // class members
==============================================================================*/

CWidgetCentral* CWidgetCentral::s_pThis = nullptr; // singleton class

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWidgetCentral::CWidgetCentral(
    CPCResourceMonitor* i_pPCResourceMonitor,
    QWidget*            i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pPCResourceMonitor(i_pPCResourceMonitor),
    m_pLyt(nullptr),
    m_pTabWdgt(nullptr),
    m_pWdgtPCSettings(nullptr),
    m_pWdgtPCSystem(nullptr),
    m_pWdgtPCProcessors(nullptr),
    m_pWdgtPCMemory(nullptr),
    m_pWdgtPCDisks(nullptr),
    m_pTrcAdminObj(nullptr)
{
    if( s_pThis != nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultSingletonClassAlreadyInstantiated);
    }
    s_pThis = this;

    setObjectName("CentralWidget");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Products::PCResourceMonitor", "CWdgtCentral", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    // Create tab widgets
    //===================

    m_pTabWdgt = new QTabWidget();
    m_pLyt->addWidget(m_pTabWdgt);

    //m_pWdgtPCSettings = new CWdgtPCSettings(m_pPCResourceMonitor);
    //m_pTabWdgt->addTab( m_pWdgtPCSettings, "Settings" );

    //m_pWdgtPCSystem = new CWdgtPCSystem(m_pPCResourceMonitor);
    //m_pTabWdgt->addTab( m_pWdgtPCSystem, "System" );

    //m_pWdgtPCProcessors = new CWdgtPCProcessors(m_pPCResourceMonitor);
    //m_pTabWdgt->addTab( m_pWdgtPCProcessors, "Processors" );

    m_pWdgtPCMemory = new CWdgtPCMemory(m_pPCResourceMonitor);
    m_pTabWdgt->addTab( m_pWdgtPCMemory, "Memory" );

    //m_pWdgtPCDisks = new CWdgtPCDisks(m_pPCResourceMonitor);
    //m_pTabWdgt->addTab( m_pWdgtPCDisks, "Disks" );

    // Geometry
    //====================

    // Restore position and size

    QSettings settings;

    restoreGeometry( settings.value(objectName()+"/Geometry").toByteArray() );

} // ctor

//------------------------------------------------------------------------------
CWidgetCentral::~CWidgetCentral()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    QSettings settings;

    settings.setValue( objectName()+"/Geometry", saveGeometry() );

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pLyt = nullptr;
    m_pTabWdgt= nullptr;
    m_pWdgtPCSettings= nullptr;
    m_pWdgtPCSystem= nullptr;
    m_pWdgtPCProcessors= nullptr;
    m_pWdgtPCMemory= nullptr;
    m_pWdgtPCDisks= nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor
