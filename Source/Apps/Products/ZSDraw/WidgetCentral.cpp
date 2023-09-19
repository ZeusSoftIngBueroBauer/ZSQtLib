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

#include "WidgetCentral.h"

#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Widgets/Drawing/ZSDrawingView.h"

//#include <QtCore/qsettings.h>
//#include <QtCore/qtimer.h>

//#include <QtGui/qbitmap.h>
//#include <QtGui/qevent.h>
//#include <QtGui/qicon.h>
//#include <QtGui/qpainter.h>

#if QT_VERSION < 0x050000
//#include <QtGui/qapplication.h>
//#include <QtGui/qbuttongroup.h>
//#include <QtGui/qcombobox.h>
//#include <QtGui/qdockwidget.h>
//#include <QtGui/qfiledialog.h>
//#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
//#include <QtGui/qmenubar.h>
//#include <QtGui/qmessagebox.h>
//#include <QtGui/qpushbutton.h>
//#include <QtGui/qspinbox.h>
//#include <QtGui/qsplitter.h>
//#include <QtGui/qstatusbar.h>
//#include <QtGui/qtoolbar.h>
//#include <QtGui/qtoolbutton.h>
//#include <QtGui/qtreeview.h>
#else
//#include <QtWidgets/qapplication.h>
//#include <QtWidgets/qbuttongroup.h>
//#include <QtWidgets/qcombobox.h>
//#include <QtWidgets/qdockwidget.h>
//#include <QtWidgets/qfiledialog.h>
//#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
//#include <QtWidgets/qmenubar.h>
//#include <QtWidgets/qmessagebox.h>
//#include <QtWidgets/qpushbutton.h>
//#include <QtWidgets/qspinbox.h>
//#include <QtWidgets/qsplitter.h>
//#include <QtWidgets/qstatusbar.h>
//#include <QtWidgets/qtoolbar.h>
//#include <QtWidgets/qtoolbutton.h>
//#include <QtWidgets/qtreeview.h>
#endif

#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::Apps::Products::Draw;


/*******************************************************************************
class CWidgetCentral : public QMainWindow
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

CWidgetCentral* CWidgetCentral::s_pThis = nullptr;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CWidgetCentral* CWidgetCentral::GetInstance()
//------------------------------------------------------------------------------
{
    return s_pThis;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWidgetCentral::CWidgetCentral(
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent, i_wflags),
    m_pLyt(nullptr),
    m_pDrawingScene(nullptr),
    m_pDrawingView(nullptr)
{
    if( s_pThis != nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultSingletonClassAlreadyInstantiated);
    }
    s_pThis = this;

    setObjectName("CentralWidget");

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    // The drawing size instance created by the drawing scene accesses the
    // unit to get the screen resolution. To get the screen resolution the
    // screen resolution must be set before. Setting it in the drawing views
    // constructor is too late. It must be done by the creator of the drawing view.
    Units.Length.setPxpi(logicalDpiX());
    Units.Length.setDpi(physicalDpiX());

    m_pDrawingScene = new CDrawingScene("theInst");
    //m_pDrawingScene->setBackgroundBrush(Qt::blue);

    m_pDrawingView = new CDrawingView(m_pDrawingScene);
    m_pDrawingView->setMouseTracking(true);
    m_pLyt->addWidget(m_pDrawingView/*, 0, Qt::AlignCenter*/);

} // ctor

//------------------------------------------------------------------------------
CWidgetCentral::~CWidgetCentral()
//------------------------------------------------------------------------------
{
    s_pThis = nullptr;

    try
    {
        delete m_pDrawingScene;
    }
    catch(...)
    {
    }

    m_pLyt = nullptr;
    m_pDrawingScene = nullptr;
    m_pDrawingView = nullptr;

} // dtor

