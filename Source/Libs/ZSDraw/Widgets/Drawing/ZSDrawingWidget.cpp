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

#include "ZSDraw/Widgets/Drawing/ZSDrawingWidget.h"
#include "ZSDraw/Common/ZSDrawUnits.h"
//#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Widgets/Drawing/ZSDrawingView.h"
//#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactory.h"
//#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"
//#include "ZSSysGUI/ZSSysGUIAux.h"
//#include "ZSSys/ZSSysAux.h"
//#include "ZSSys/ZSSysException.h"
//#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#if QT_VERSION < 0x050000
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#else
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
//using namespace ZS::System::GUI;
using namespace ZS::Draw;
//using namespace ZS::PhysVal;


/*******************************************************************************
class CWdgtDrawing : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates the graphics view.

    @note The drawing size instance created by the drawing scene accesses the
          the unit to get the screen resolution. To get the screen resolution
          the screen resolution must be set before. The screen resolution must
          therefore be set at the draw units BEFORE creating the drawing scene.
*/
CWdgtDrawing::CWdgtDrawing(QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pLyt(nullptr),
    m_pDrawingView(nullptr),
    m_pDrawingScene(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName("theInst");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Widgets::Drawing", ClassName(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    QHBoxLayout* pLytLine1 = new QHBoxLayout();
    m_pLyt->addLayout(pLytLine1);
    QLabel* pLblHelloWorld = new QLabel("Hello World");
    pLblHelloWorld->setBackgroundRole(QPalette::ToolTipBase);
    pLytLine1->addWidget(pLblHelloWorld);
    pLytLine1->addStretch();

    // The drawing size instance created by the drawing scene accesses the
    // unit to get the screen resolution. To get the screen resolution the
    // screen resolution must be set before. Setting it in the drawing views
    // constructor is too late. It must be done by the creator of the drawing view.
    Units.Length.setPxpi(logicalDpiX());
    Units.Length.setDpi(physicalDpiX());

    m_pDrawingScene = new CDrawingScene("theInst");

    m_pDrawingView = new CDrawingView(m_pDrawingScene);
    m_pDrawingView->setMouseTracking(true);
    m_pLyt->addWidget(m_pDrawingView/*, 0, Qt::AlignCenter*/);

    m_pLyt->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWdgtDrawing::~CWdgtDrawing()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    try
    {
        delete m_pDrawingScene;
    }
    catch (...)
    {
    }

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pLyt = nullptr;
    m_pDrawingView = nullptr;
    m_pDrawingScene = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor
