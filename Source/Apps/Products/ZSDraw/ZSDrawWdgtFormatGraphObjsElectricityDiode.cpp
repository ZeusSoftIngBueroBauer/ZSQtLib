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

#include "ZSDrawWdgtFormatGraphObjsElectricityDiode.h"
#include "ZSDrawGraphObjElectricityDiode.h"

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
using namespace ZS::Trace;


/*******************************************************************************
class CWdgtFormatGraphObjsDiode : public CWdgtFormatGraphObjs
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsDiode::CWdgtFormatGraphObjsDiode(
    CDrawingScene* i_pDrawingScene,
    CGraphObj*     i_pGraphObj,
    QWidget*       i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtFormatGraphObjs(i_pDrawingScene,i_pGraphObj,i_pWdgtParent),
    m_pLyt(nullptr),
    m_pLblHeadLine(nullptr),
    m_pLytCutOffVoltage(nullptr),
    m_pLblCutOffVoltage(nullptr),
    m_pEdtCutOffVoltage(nullptr),
    m_pChkCutOffVoltageVisible(nullptr),
    // Trace
    m_pTrcAdminObj(nullptr)
{
    setObjectName("WdgtFormatGraphObjsDiode");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Products::Draw", "CWdgtFormatGraphObjsDiode", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    QVBoxLayout* m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    int cxLblWidth = 90;
    //int cxEdtWidth = 80;

    CGraphObjDiode* pGraphObjDiode = dynamic_cast<CGraphObjDiode*>(m_pGraphObj);

    if( pGraphObjDiode == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjDiode == nullptr" );
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

    // <Line> CutOffVoltage
    //===================

    m_pLytCutOffVoltage = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytCutOffVoltage);

    // <SpinBox> CutOffVoltage
    //---------------------

    m_pLblCutOffVoltage = new QLabel("CutOffVoltage:");
    m_pLblCutOffVoltage->setFixedWidth(cxLblWidth);
    m_pLytCutOffVoltage->addWidget(m_pLblCutOffVoltage);
    m_pEdtCutOffVoltage = new QDoubleSpinBox();
    m_pEdtCutOffVoltage->setRange(0.0,1.0e12);
    m_pEdtCutOffVoltage->setDecimals(1);
    m_pEdtCutOffVoltage->setSingleStep(1.0);
    m_pEdtCutOffVoltage->setSuffix(" V");
    m_pEdtCutOffVoltage->setValue(pGraphObjDiode->getCutOffVoltage());
    m_pLytCutOffVoltage->addWidget(m_pEdtCutOffVoltage);

    // <CheckBox> Show Object Name
    //-----------------------------

    m_pChkCutOffVoltageVisible = new QCheckBox("Visible");
    m_pChkCutOffVoltageVisible->setChecked( pGraphObjDiode->isCutOffVoltageVisible() );
    m_pLytCutOffVoltage->addWidget(m_pChkCutOffVoltageVisible);

    m_pLytCutOffVoltage->addStretch();

    // <Stretch> at bottom of Widget
    //==============================

    m_pLyt->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsDiode::~CWdgtFormatGraphObjsDiode()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pLyt = nullptr;
    m_pLblHeadLine = nullptr;
    m_pLytCutOffVoltage = nullptr;
    m_pLblCutOffVoltage = nullptr;
    m_pEdtCutOffVoltage = nullptr;
    m_pChkCutOffVoltageVisible = nullptr;

    // Trace
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // must overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsDiode::applyChanges()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "applyChanges",
        /* strAddInfo   */ strAddTrcInfo );

    CGraphObjDiode* pGraphObjDiode = dynamic_cast<CGraphObjDiode*>(m_pGraphObj);

    if( pGraphObjDiode == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjDiode == nullptr" );
    }

    if( m_pEdtCutOffVoltage != nullptr )
    {
        pGraphObjDiode->setCutOffVoltage( m_pEdtCutOffVoltage->value() );
    }

    if( m_pChkCutOffVoltageVisible != nullptr )
    {
        if( m_pChkCutOffVoltageVisible->isChecked() )
        {
            if( !pGraphObjDiode->isCutOffVoltageVisible() )
            {
                pGraphObjDiode->showCutOffVoltage();
            }
        }
        else
        {
            if( pGraphObjDiode->isCutOffVoltageVisible() )
            {
                pGraphObjDiode->hideCutOffVoltage();
            }
        }
    }

} // applyChanges

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsDiode::resetChanges()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "resetChanges",
        /* strAddInfo   */ strAddTrcInfo );

    CGraphObjDiode* pGraphObjDiode = dynamic_cast<CGraphObjDiode*>(m_pGraphObj);

    if( pGraphObjDiode == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjDiode == nullptr" );
    }

    if( m_pEdtCutOffVoltage != nullptr )
    {
        m_pEdtCutOffVoltage->setValue( pGraphObjDiode->getCutOffVoltage() );
    }
    if( m_pChkCutOffVoltageVisible != nullptr )
    {
        m_pChkCutOffVoltageVisible->setChecked( pGraphObjDiode->isCutOffVoltageVisible() );
    }

} // resetChanges

//------------------------------------------------------------------------------
bool CWdgtFormatGraphObjsDiode::hasChanges() const
//------------------------------------------------------------------------------
{
    bool bHasChanges = false;

    CGraphObjDiode* pGraphObjDiode = dynamic_cast<CGraphObjDiode*>(m_pGraphObj);

    if( pGraphObjDiode == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjDiode == nullptr" );
    }

    if( m_pEdtCutOffVoltage != nullptr )
    {
        if( pGraphObjDiode->getCutOffVoltage() != m_pEdtCutOffVoltage->value() )
        {
            bHasChanges = true;
        }
    }

    if( !bHasChanges && m_pChkCutOffVoltageVisible != nullptr )
    {
        bHasChanges = (pGraphObjDiode->isCutOffVoltageVisible() != m_pChkCutOffVoltageVisible->isChecked());
    }

    return bHasChanges;

} // hasChanges
