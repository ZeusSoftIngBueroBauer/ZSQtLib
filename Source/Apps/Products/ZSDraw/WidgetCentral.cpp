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
    m_pWdgtDrawing(nullptr)
{
    if( s_pThis != nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultSingletonClassAlreadyInstantiated);
    }
    s_pThis = this;

    setObjectName("CentralWidget");

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    m_pWdgtDrawing = new CWdgtDrawing();
    m_pLyt->addWidget(m_pWdgtDrawing);

} // ctor

//------------------------------------------------------------------------------
CWidgetCentral::~CWidgetCentral()
//------------------------------------------------------------------------------
{
    s_pThis = nullptr;

    m_pLyt = nullptr;
    m_pWdgtDrawing = nullptr;

} // dtor

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
