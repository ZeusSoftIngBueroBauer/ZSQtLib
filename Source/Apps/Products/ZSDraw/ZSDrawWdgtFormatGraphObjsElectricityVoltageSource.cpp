/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#include "ZSDrawWdgtFormatGraphObjsElectricityVoltageSource.h"
#include "ZSDrawGraphObjElectricityVoltageSource.h"

#include "ZSDraw/ZSDrawingScene.h"
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
class CWdgtFormatGraphObjsVoltageSource : public CWdgtFormatGraphObjs
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsVoltageSource::CWdgtFormatGraphObjsVoltageSource(
    CDrawingScene* i_pDrawingScene,
    CGraphObj*     i_pGraphObj,
    QWidget*       i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtFormatGraphObjs(i_pDrawingScene,i_pGraphObj,i_pWdgtParent),
    m_pLyt(nullptr),
    m_pLblHeadLine(nullptr),
    m_pLytVoltage(nullptr),
    m_pLblVoltage(nullptr),
    m_pEdtVoltage(nullptr),
    m_pChkVoltageVisible(nullptr),
    // Trace
    m_pTrcAdminObj(nullptr)
{
    setObjectName("WdgtFormatGraphObjsVoltageSource");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Products::Draw", "CWdgtFormatGraphObjsVoltageSource", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    QVBoxLayout* m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    int cxLblWidth = 90;
    //int cxEdtWidth = 80;

    CGraphObjVoltageSource* pGraphObjVoltageSource = dynamic_cast<CGraphObjVoltageSource*>(m_pGraphObj);

    if( pGraphObjVoltageSource == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjVoltageSource == nullptr" );
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

    // <Line> Voltage
    //===================

    m_pLytVoltage = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytVoltage);

    // <SpinBox> Voltage
    //----------------------

    m_pLblVoltage = new QLabel("Voltage:");
    m_pLblVoltage->setFixedWidth(cxLblWidth);
    m_pLytVoltage->addWidget(m_pLblVoltage);
    m_pEdtVoltage = new QDoubleSpinBox();
    m_pEdtVoltage->setRange(0.0,1.0e12);
    m_pEdtVoltage->setDecimals(1);
    m_pEdtVoltage->setSingleStep(1.0);
    m_pEdtVoltage->setSuffix(" V");
    m_pEdtVoltage->setValue(pGraphObjVoltageSource->getVoltage());
    m_pLytVoltage->addWidget(m_pEdtVoltage);

    // <CheckBox> Show Voltage
    //-----------------------------

    m_pChkVoltageVisible = new QCheckBox("Visible");
    m_pChkVoltageVisible->setChecked( pGraphObjVoltageSource->isVoltageVisible() );
    m_pLytVoltage->addWidget(m_pChkVoltageVisible);

    // <Stretch> at bottom of Widget
    //==============================

    m_pLyt->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsVoltageSource::~CWdgtFormatGraphObjsVoltageSource()
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
    m_pLytVoltage = nullptr;
    m_pLblVoltage = nullptr;
    m_pEdtVoltage = nullptr;
    m_pChkVoltageVisible = nullptr;

    // Trace
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // must overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsVoltageSource::applyChanges()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "applyChanges",
        /* strAddInfo   */ strAddTrcInfo );

    CGraphObjVoltageSource* pGraphObjVoltageSource = dynamic_cast<CGraphObjVoltageSource*>(m_pGraphObj);

    if( pGraphObjVoltageSource == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjVoltageSource == nullptr" );
    }

    if( m_pEdtVoltage != nullptr )
    {
        pGraphObjVoltageSource->setVoltage( m_pEdtVoltage->value() );
    }

    if( m_pChkVoltageVisible != nullptr )
    {
        if( m_pChkVoltageVisible->isChecked() )
        {
            if( !pGraphObjVoltageSource->isVoltageVisible() )
            {
                pGraphObjVoltageSource->showVoltage();
            }
        }
        else
        {
            if( pGraphObjVoltageSource->isVoltageVisible() )
            {
                pGraphObjVoltageSource->hideVoltage();
            }
        }
    }

} // applyChanges

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsVoltageSource::resetChanges()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "resetChanges",
        /* strAddInfo   */ strAddTrcInfo );

    CGraphObjVoltageSource* pGraphObjVoltageSource = dynamic_cast<CGraphObjVoltageSource*>(m_pGraphObj);

    if( pGraphObjVoltageSource == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjVoltageSource == nullptr" );
    }

    if( m_pEdtVoltage != nullptr )
    {
        m_pEdtVoltage->setValue( pGraphObjVoltageSource->getVoltage() );
    }
    if( m_pChkVoltageVisible != nullptr )
    {
        m_pChkVoltageVisible->setChecked( pGraphObjVoltageSource->isVoltageVisible() );
    }

} // resetChanges

//------------------------------------------------------------------------------
bool CWdgtFormatGraphObjsVoltageSource::hasChanges() const
//------------------------------------------------------------------------------
{
    bool bHasChanges = false;

    CGraphObjVoltageSource* pGraphObjVoltageSource = dynamic_cast<CGraphObjVoltageSource*>(m_pGraphObj);

    if( pGraphObjVoltageSource == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjVoltageSource == nullptr" );
    }

    if( m_pEdtVoltage != nullptr )
    {
        if( pGraphObjVoltageSource->getVoltage() != m_pEdtVoltage->value() )
        {
            bHasChanges = true;
        }
    }

    if( !bHasChanges && m_pChkVoltageVisible != nullptr )
    {
        bHasChanges = (pGraphObjVoltageSource->isVoltageVisible() != m_pChkVoltageVisible->isChecked());
    }

    return bHasChanges;

} // hasChanges

/*==============================================================================
protected: // must overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsVoltageSource::onGraphObjChanged()
//------------------------------------------------------------------------------
{
} // onGraphObjChanged
