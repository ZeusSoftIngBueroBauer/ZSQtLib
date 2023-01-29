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

#if 0

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qcombobox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#else
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#endif

#include "ZSDraw/DrawingPageSetup/ZSDrawWdgtDrawingPaperPageSetup.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;
//using namespace ZS::PhysVal;


/*******************************************************************************
class CWdgtDrawingPaperPageSetup : public CWdgtPageSetup
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtDrawingPaperPageSetup::CWdgtDrawingPaperPageSetup(
    CDrawingView* i_pDrawingView,
    QWidget*      i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(),
    m_pLyt(nullptr),
    m_pLblHeadLine(nullptr),
    // Trace
    m_pTrcAdminObj(nullptr)
{
    setObjectName("WdgtPageSetupDrawingPaper");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    QVBoxLayout* m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    //int cxLblWidth = 60;
    //int cxEdtWidth = 80;

    QFont fntHeadLine;

    m_pLblHeadLine = new QLabel("Drawing Paper");
    fntHeadLine = m_pLblHeadLine->font();
    fntHeadLine.setPointSize(fntHeadLine.pointSize()+4);
    m_pLblHeadLine->setFont(fntHeadLine);
    m_pLyt->addWidget(m_pLblHeadLine);
    m_pLyt->addSpacing(4);

    // <Stretch> at bottom of widget
    //------------------------------

    m_pLyt->addStretch();

    // Set settings at GUI controls
    //-----------------------------

    //setSettings(m_pageSetup);

} // ctor

//------------------------------------------------------------------------------
CWdgtDrawingPaperPageSetup::~CWdgtDrawingPaperPageSetup()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pLyt = nullptr;
    m_pLblHeadLine = nullptr;

    // Trace
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtDrawingPaperPageSetup::hasChanges() const
//------------------------------------------------------------------------------
{
    return true;
}

//------------------------------------------------------------------------------
void CWdgtDrawingPaperPageSetup::acceptChanges() const
//------------------------------------------------------------------------------
{
}

/*==============================================================================
public: // overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

////------------------------------------------------------------------------------
//void CWdgtDrawingPaperPageSetup::setSettings( const CPageSetup& i_pageSetup )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
//    {
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod    */ "setSettings",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    m_pageSetup = i_pageSetup;
//
//    //emit pageSetupAttributeChanged( EPageSetupAttribute.., m_pageSetup.get..() );
//
//} // setSettings

#endif
