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

#include "ZSDraw/Widgets/Drawing/ZSDrawingWidget.h"

#if QT_VERSION < 0x050000
#include <QtGui/qlayout.h>
#else
#include <QtWidgets/qlayout.h>
#endif

#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

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
CWidgetCentral::CWidgetCentral(QWidget* i_pWdgtParent, Qt::WindowFlags i_wflags) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent, i_wflags)
{
    if (s_pThis != nullptr) {
        throw CException(__FILE__, __LINE__, EResultSingletonClassAlreadyInstantiated);
    }
    s_pThis = this;

    setObjectName("theInst");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    m_pWdgtDrawing = new CWdgtDrawing();
    m_pLyt->addWidget(m_pWdgtDrawing);

} // ctor

//------------------------------------------------------------------------------
CWidgetCentral::~CWidgetCentral()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    s_pThis = nullptr;

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtDrawing* CWidgetCentral::drawingWidget()
//------------------------------------------------------------------------------
{
    return m_pWdgtDrawing;
}

//------------------------------------------------------------------------------
CDrawingView* CWidgetCentral::drawingView()
//------------------------------------------------------------------------------
{
    return m_pWdgtDrawing->drawingView();
}

//------------------------------------------------------------------------------
CDrawingScene* CWidgetCentral::drawingScene()
//------------------------------------------------------------------------------
{
    return m_pWdgtDrawing->drawingScene();
}

/*==============================================================================
protected: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
void CWidgetCentral::closeEvent(QCloseEvent* i_pEv)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "closeEvent",
        /* strAddInfo   */ "" );

    QWidget::closeEvent(i_pEv);
}
