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

#if QT_VERSION < 0x050000
#include <QtGui/qframe.h>
#include <QtGui/qlayout.h>
#else
#include <QtWidgets/qframe.h>
#include <QtWidgets/qlayout.h>
#endif

#include "WdgtTestOutput.h"

#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::Trace;
using namespace ZS::Apps::Test::Diagram;


/*******************************************************************************
class CWdgtTestOutput : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtTestOutput::CWdgtTestOutput( QWidget* i_pWdgtParent, Qt::WindowFlags  i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    //m_scaleX(
    //    /* fMin  */ 1.0e9,
    //    /* fMax  */ 2.0e9,
    //    /* pUnit */ Frequency()->Hertz() ),
    //m_scaleY(
    //    /* fMin      */ -200.0,
    //    /* fMax      */ 50.0,
    //    /* pPhysUnit */ Power()->dBMilliWatt() ),
    //m_fRadDrift(0.0),
    //m_arfXValues(EValCount),
    //m_arfYValuesTrace0(EValCount),
    //m_arfYValuesTrace1(EValCount),
    //m_pTimerSigGen(nullptr),
    //m_pFrameDiagram(nullptr),
    //m_pWdgtDiagram(nullptr),
    //m_pDiagScaleX(nullptr),
    //m_pDiagScaleY(nullptr),
    //m_pDiagTrace0(nullptr),
    //m_pDiagTrace1(nullptr),
    //m_pDiagObjGrid(nullptr),
    //m_pDiagObjAxisLabelX(nullptr),
    //m_pDiagObjAxisLabelY(nullptr),
    //m_pDiagObjCurve0(nullptr),
    //m_pDiagObjCurve1(nullptr),
    //m_pDiagObjMarker0(nullptr),
    //m_pDiagObjMarker1(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName("CentralWidget");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Examples::Diagram", "CWdgtCentral", objectName());

    ZS::Trace::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    QPalette paletteWdgtCentral;
    paletteWdgtCentral.setColor(QPalette::Window,Qt::lightGray);
    setPalette(paletteWdgtCentral);
    setAutoFillBackground(true);

    QVBoxLayout* pLytMain = new QVBoxLayout();
    //pLytMain->setContentsMargins(0,0,0,0); // left,tip,right,bottom
    setLayout(pLytMain);

} // ctor

//------------------------------------------------------------------------------
CWdgtTestOutput::~CWdgtTestOutput()
//------------------------------------------------------------------------------
{
    ZS::Trace::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTestOutput::resizeEvent( QResizeEvent* i_pEv )
//------------------------------------------------------------------------------
{
    ZS::Trace::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "resizeEvent",
        /* strAddInfo   */ "" );

    if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) )
    {
        QString strTrcMsg;
        QRect   rectGeometry = geometry();

        strTrcMsg  = "Geometry[x,y,w,h]=";
        strTrcMsg += QString::number(rectGeometry.x()) + ",";
        strTrcMsg += QString::number(rectGeometry.y()) + ",";
        strTrcMsg += QString::number(rectGeometry.width()) + ",";
        strTrcMsg += QString::number(rectGeometry.height());
        mthTracer.trace(strTrcMsg);

        strTrcMsg  = "OldSize[w,h]=";
        strTrcMsg += QString::number(i_pEv->oldSize().width()) + ",";
        strTrcMsg += QString::number(i_pEv->oldSize().height()) + "; ";
        strTrcMsg += "NewSize[w,h]=";
        strTrcMsg += QString::number(i_pEv->size().width()) + ",";
        strTrcMsg += QString::number(i_pEv->size().height());
        mthTracer.trace(strTrcMsg);
    }

} // resizeEvent
