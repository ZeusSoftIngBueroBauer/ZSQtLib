/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer, Germany
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

#if QT_VERSION < 0x050000
#include <QtGui/qapplication.h>
#include <QtGui/qmenubar.h>
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qmenubar.h>
#endif

#include "MainWindow.h"
#include "WidgetCentral.h"

#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Apps::Test::PhysVal;


/*******************************************************************************
class CMainWindow : public QMainWindow
*******************************************************************************/

/*==============================================================================
private: // class members
==============================================================================*/

CMainWindow*  CMainWindow::s_pThis = nullptr; // singleton class

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMainWindow::CMainWindow(
    const QString&  i_strWindowTitle,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QMainWindow(i_pWdgtParent,i_wflags),
    m_pMenuFile(nullptr),
    m_pActionFileExit(nullptr),
    m_pWdgtCentral(nullptr)
{
    if( s_pThis != nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultSingletonClassAlreadyInstantiated);
    }
    s_pThis = this;

    setObjectName("MainWindow");

    setWindowTitle(i_strWindowTitle);

    // <Menu> File
    //======================

    m_pMenuFile = menuBar()->addMenu(tr("File"));

    // <MenuItem> File::Quit
    //----------------------

    m_pActionFileExit = m_pMenuFile->addAction(tr("Exit"));
    m_pActionFileExit->setShortcut(Qt::Key_F4 + Qt::ALT);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionFileExit,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ qApp,
        /* szSlot       */ SLOT(quit()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <CentralWidget>
    //======================

    m_pWdgtCentral = new CWidgetCentral();
    setCentralWidget(m_pWdgtCentral);

    // <Geometry>
    //======================

    QSettings settings;

    restoreGeometry( settings.value("MainWindow/Geometry").toByteArray() );

} // ctor

//------------------------------------------------------------------------------
CMainWindow::~CMainWindow()
//------------------------------------------------------------------------------
{
    // <Geometry>
    //======================

    QSettings settings;

    settings.setValue( "MainWindow/Geometry", saveGeometry() );

    // <Clear>
    //======================

    m_pMenuFile = nullptr;
    m_pActionFileExit = nullptr;
    m_pWdgtCentral = nullptr;

} // dtor
