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

#include <QtCore/qglobal.h>
#include <QtCore/qtimer.h>
#include <QtGui/qbitmap.h>
#include <QtGui/qpainter.h>

#if QT_VERSION < 0x050000
#include <QtGui/qapplication.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qspinbox.h>
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qspinbox.h>
#endif

#include "Electricity/ZSDrawWdgtFormatGraphObjsElectricityCapacitor.h"
#include "Electricity/ZSDrawGraphObjElectricityCapacitor.h"

#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSSysGUI/ZSSysPushButton.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Draw;
using namespace ZS::Draw::Electricity;


/*******************************************************************************
class CWdgtFormatGraphObjsCapacitor : public CWdgtFormatGraphObjs
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsCapacitor::CWdgtFormatGraphObjsCapacitor(
    CDrawingScene* i_pDrawingScene,
    CGraphObj* i_pGraphObj,
    QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtFormatGraphObjs(i_pDrawingScene, "CWdgtFormatGraphObjsCapacitor", "", i_pGraphObj,i_pWdgtParent),
    m_pLyt(nullptr),
    m_pLblHeadLine(nullptr),
    m_pLytCapacitance(nullptr),
    m_pLblCapacitance(nullptr),
    m_pEdtCapacitance(nullptr),
    m_pChkCapacitanceVisible(nullptr),
    // Trace
    m_pTrcAdminObj(nullptr)
{
    setObjectName("WdgtFormatGraphObjsCapacitor");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Products::Draw", "CWdgtFormatGraphObjsCapacitor", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    QVBoxLayout* m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    int cxLblWidth = 90;
    //int cxEdtWidth = 80;

    CGraphObjCapacitor* pGraphObjCapacitor = dynamic_cast<CGraphObjCapacitor*>(m_pGraphObj);

    if( pGraphObjCapacitor == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjCapacitor == nullptr" );
    }

    // Headline
    //=========

    QFont fntHeadLine;

    m_pLblHeadLine = new QLabel("Electrical Parameters");
    fntHeadLine = m_pLblHeadLine->font();
    fntHeadLine.setPointSize(fntHeadLine.pointSize()+4);
    m_pLblHeadLine->setFont(fntHeadLine);
    m_pLyt->addWidget(m_pLblHeadLine);
    m_pLyt->addSpacing(4);

    // <Line> Capacitance
    //===================

    m_pLytCapacitance = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytCapacitance);

    // <SpinBox> Capacitance
    //----------------------

    m_pLblCapacitance = new QLabel("Capacitance:");
    m_pLblCapacitance->setFixedWidth(cxLblWidth);
    m_pLytCapacitance->addWidget(m_pLblCapacitance);
    m_pEdtCapacitance = new QDoubleSpinBox();
    m_pEdtCapacitance->setRange(0.0,1.0e12);
    m_pEdtCapacitance->setDecimals(1);
    m_pEdtCapacitance->setSingleStep(1.0);
    m_pEdtCapacitance->setSuffix(" F");
    m_pEdtCapacitance->setValue(pGraphObjCapacitor->getCapacitance());
    m_pLytCapacitance->addWidget(m_pEdtCapacitance);

    // <CheckBox> Show Capacitance
    //-----------------------------

    m_pChkCapacitanceVisible = new QCheckBox("Visible");
    m_pChkCapacitanceVisible->setChecked( pGraphObjCapacitor->isCapacitanceVisible() );
    m_pLytCapacitance->addWidget(m_pChkCapacitanceVisible);

    // <Stretch> at bottom of Widget
    //==============================

    m_pLyt->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsCapacitor::~CWdgtFormatGraphObjsCapacitor()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pLyt = nullptr;
    m_pLblHeadLine = nullptr;
    m_pLytCapacitance = nullptr;
    m_pLblCapacitance = nullptr;
    m_pEdtCapacitance = nullptr;
    m_pChkCapacitanceVisible = nullptr;

    // Trace
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // must overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsCapacitor::applyChanges()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "applyChanges",
        /* strAddInfo   */ strAddTrcInfo );

    CGraphObjCapacitor* pGraphObjCapacitor = dynamic_cast<CGraphObjCapacitor*>(m_pGraphObj);

    if( pGraphObjCapacitor == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjCapacitor == nullptr" );
    }

    if( m_pEdtCapacitance != nullptr )
    {
        pGraphObjCapacitor->setCapacitance( m_pEdtCapacitance->value() );
    }

    if( m_pChkCapacitanceVisible != nullptr )
    {
        if( m_pChkCapacitanceVisible->isChecked() )
        {
            if( !pGraphObjCapacitor->isCapacitanceVisible() )
            {
                pGraphObjCapacitor->showCapacitance();
            }
        }
        else
        {
            if( pGraphObjCapacitor->isCapacitanceVisible() )
            {
                pGraphObjCapacitor->hideCapacitance();
            }
        }
    }

} // applyChanges

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsCapacitor::resetChanges()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "resetChanges",
        /* strAddInfo   */ strAddTrcInfo );

    CGraphObjCapacitor* pGraphObjCapacitor = dynamic_cast<CGraphObjCapacitor*>(m_pGraphObj);

    if( pGraphObjCapacitor == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjCapacitor == nullptr" );
    }

    if( m_pEdtCapacitance != nullptr )
    {
        m_pEdtCapacitance->setValue( pGraphObjCapacitor->getCapacitance() );
    }
    if( m_pChkCapacitanceVisible != nullptr )
    {
        m_pChkCapacitanceVisible->setChecked( pGraphObjCapacitor->isCapacitanceVisible() );
    }

} // resetChanges

//------------------------------------------------------------------------------
bool CWdgtFormatGraphObjsCapacitor::hasChanges() const
//------------------------------------------------------------------------------
{
    bool bHasChanges = false;

    CGraphObjCapacitor* pGraphObjCapacitor = dynamic_cast<CGraphObjCapacitor*>(m_pGraphObj);

    if( pGraphObjCapacitor == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjCapacitor == nullptr" );
    }

    if( m_pEdtCapacitance != nullptr )
    {
        if( pGraphObjCapacitor->getCapacitance() != m_pEdtCapacitance->value() )
        {
            bHasChanges = true;
        }
    }

    if( !bHasChanges && m_pChkCapacitanceVisible != nullptr )
    {
        bHasChanges = (pGraphObjCapacitor->isCapacitanceVisible() != m_pChkCapacitanceVisible->isChecked());
    }

    return bHasChanges;

} // hasChanges

/*==============================================================================
protected: // must overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsCapacitor::onGraphObjChanged()
//------------------------------------------------------------------------------
{
} // onGraphObjChanged
