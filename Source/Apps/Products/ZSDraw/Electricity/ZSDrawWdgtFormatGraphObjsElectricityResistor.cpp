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

#include "Electricity/ZSDrawWdgtFormatGraphObjsElectricityResistor.h"
#include "Electricity/ZSDrawGraphObjElectricityResistor.h"

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
class CWdgtFormatGraphObjsResistor : public CWdgtFormatGraphObjs
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsResistor::CWdgtFormatGraphObjsResistor(
    CDrawingScene* i_pDrawingScene,
    CGraphObj*     i_pGraphObj,
    QWidget*       i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtFormatGraphObjs(i_pDrawingScene, "CWdgtFormatGraphObjsResistor", "", i_pGraphObj, i_pWdgtParent),
    m_pLyt(nullptr),
    m_pLblHeadLine(nullptr),
    m_pLytResistance(nullptr),
    m_pLblResistance(nullptr),
    m_pEdtResistance(nullptr),
    m_pChkResistanceVisible(nullptr),
    // Trace
    m_pTrcAdminObj(nullptr)
{
    setObjectName("WdgtFormatGraphObjsResistor");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Products::Draw", "CWdgtFormatGraphObjsResistor", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    QVBoxLayout* m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    int cxLblWidth = 90;
    //int cxEdtWidth = 80;

    CGraphObjResistor* pGraphObjResistor = dynamic_cast<CGraphObjResistor*>(m_pGraphObj);

    if( pGraphObjResistor == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjResistor == nullptr" );
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

    // <Line> Resistance
    //===================

    m_pLytResistance = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytResistance);

    // <SpinBox> Resistance
    //---------------------

    m_pLblResistance = new QLabel("Resistance:");
    m_pLblResistance->setFixedWidth(cxLblWidth);
    m_pLytResistance->addWidget(m_pLblResistance);
    m_pEdtResistance = new QDoubleSpinBox();
    m_pEdtResistance->setRange(0.0,1.0e12);
    m_pEdtResistance->setDecimals(1);
    m_pEdtResistance->setSingleStep(1.0);
    m_pEdtResistance->setSuffix(" Ohm");
    m_pEdtResistance->setValue(pGraphObjResistor->getResistance());
    m_pLytResistance->addWidget(m_pEdtResistance);

    // <CheckBox> Show Object Name
    //-----------------------------

    m_pChkResistanceVisible = new QCheckBox("Visible");
    m_pChkResistanceVisible->setChecked( pGraphObjResistor->isResistanceVisible() );
    m_pLytResistance->addWidget(m_pChkResistanceVisible);

    m_pLytResistance->addStretch();

    // <Stretch> at bottom of Widget
    //==============================

    m_pLyt->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsResistor::~CWdgtFormatGraphObjsResistor()
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
    m_pLytResistance = nullptr;
    m_pLblResistance = nullptr;
    m_pEdtResistance = nullptr;
    m_pChkResistanceVisible = nullptr;

    // Trace
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // must overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsResistor::applyChanges()
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

    CGraphObjResistor* pGraphObjResistor = dynamic_cast<CGraphObjResistor*>(m_pGraphObj);

    if( pGraphObjResistor == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjResistor == nullptr" );
    }

    if( m_pEdtResistance != nullptr )
    {
        pGraphObjResistor->setResistance( m_pEdtResistance->value() );
    }

    if( m_pChkResistanceVisible != nullptr )
    {
        if( m_pChkResistanceVisible->isChecked() )
        {
            if( !pGraphObjResistor->isResistanceVisible() )
            {
                pGraphObjResistor->showResistance();
            }
        }
        else
        {
            if( pGraphObjResistor->isResistanceVisible() )
            {
                pGraphObjResistor->hideResistance();
            }
        }
    }

} // applyChanges

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsResistor::resetChanges()
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

    CGraphObjResistor* pGraphObjResistor = dynamic_cast<CGraphObjResistor*>(m_pGraphObj);

    if( pGraphObjResistor == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjResistor == nullptr" );
    }

    if( m_pEdtResistance != nullptr )
    {
        m_pEdtResistance->setValue( pGraphObjResistor->getResistance() );
    }
    if( m_pChkResistanceVisible != nullptr )
    {
        m_pChkResistanceVisible->setChecked( pGraphObjResistor->isResistanceVisible() );
    }

} // resetChanges

//------------------------------------------------------------------------------
bool CWdgtFormatGraphObjsResistor::hasChanges() const
//------------------------------------------------------------------------------
{
    bool bHasChanges = false;

    CGraphObjResistor* pGraphObjResistor = dynamic_cast<CGraphObjResistor*>(m_pGraphObj);

    if( pGraphObjResistor == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjResistor == nullptr" );
    }

    if( m_pEdtResistance != nullptr )
    {
        if( pGraphObjResistor->getResistance() != m_pEdtResistance->value() )
        {
            bHasChanges = true;
        }
    }

    if( !bHasChanges && m_pChkResistanceVisible != nullptr )
    {
        bHasChanges = (pGraphObjResistor->isResistanceVisible() != m_pChkResistanceVisible->isChecked());
    }

    return bHasChanges;

} // hasChanges

/*==============================================================================
protected: // must overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsResistor::onGraphObjChanged()
//------------------------------------------------------------------------------
{
} // onGraphObjChanged
