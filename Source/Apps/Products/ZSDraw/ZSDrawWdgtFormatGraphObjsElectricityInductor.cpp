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

#include "ZSDrawWdgtFormatGraphObjsElectricityInductor.h"
#include "ZSDrawGraphObjElectricityInductor.h"

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
class CWdgtFormatGraphObjsInductor : public CWdgtFormatGraphObjs
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsInductor::CWdgtFormatGraphObjsInductor(
    CDrawingScene* i_pDrawingScene,
    CGraphObj*     i_pGraphObj,
    QWidget*       i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtFormatGraphObjs(i_pDrawingScene,i_pGraphObj,i_pWdgtParent),
    m_pLyt(nullptr),
    m_pLblHeadLine(nullptr),
    m_pLytInductance(nullptr),
    m_pLblInductance(nullptr),
    m_pEdtInductance(nullptr),
    m_pChkInductanceVisible(nullptr),
    // Trace
    m_pTrcAdminObj(nullptr)
{
    setObjectName("WdgtFormatGraphObjsInductor");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Products::Draw", "CWdgtFormatGraphObjsInductor", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    QVBoxLayout* m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    int cxLblWidth = 90;
    //int cxEdtWidth = 80;

    CGraphObjInductor* pGraphObjInductor = dynamic_cast<CGraphObjInductor*>(m_pGraphObj);

    if( pGraphObjInductor == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjInductor == nullptr" );
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

    // <Line> Inductance
    //===================

    m_pLytInductance = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytInductance);

    // <SpinBox>
    //-----------

    m_pLblInductance = new QLabel("Inductance:");
    m_pLblInductance->setFixedWidth(cxLblWidth);
    m_pLytInductance->addWidget(m_pLblInductance);
    m_pEdtInductance = new QDoubleSpinBox();
    m_pEdtInductance->setRange(0.0,1.0e12);
    m_pEdtInductance->setDecimals(1);
    m_pEdtInductance->setSingleStep(1.0);
    m_pEdtInductance->setSuffix(" H");
    m_pEdtInductance->setValue(pGraphObjInductor->getInductance());
    m_pLytInductance->addWidget(m_pEdtInductance);

    // <CheckBox> Show Inductance
    //-----------------------------

    m_pChkInductanceVisible = new QCheckBox("Visible");
    m_pChkInductanceVisible->setChecked( pGraphObjInductor->isInductanceVisible() );
    m_pLytInductance->addWidget(m_pChkInductanceVisible);

    // <Stretch> at bottom of Widget
    //==============================

    m_pLyt->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsInductor::~CWdgtFormatGraphObjsInductor()
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
    m_pLytInductance = nullptr;
    m_pLblInductance = nullptr;
    m_pEdtInductance = nullptr;
    m_pChkInductanceVisible = nullptr;

    // Trace
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // must overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsInductor::applyChanges()
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

    CGraphObjInductor* pGraphObjInductor = dynamic_cast<CGraphObjInductor*>(m_pGraphObj);

    if( pGraphObjInductor == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjInductor == nullptr" );
    }

    if( m_pEdtInductance != nullptr )
    {
        pGraphObjInductor->setInductance( m_pEdtInductance->value() );
    }

    if( m_pChkInductanceVisible != nullptr )
    {
        if( m_pChkInductanceVisible->isChecked() )
        {
            if( !pGraphObjInductor->isInductanceVisible() )
            {
                pGraphObjInductor->showInductance();
            }
        }
        else
        {
            if( pGraphObjInductor->isInductanceVisible() )
            {
                pGraphObjInductor->hideInductance();
            }
        }
    }

} // applyChanges

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsInductor::resetChanges()
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

    CGraphObjInductor* pGraphObjInductor = dynamic_cast<CGraphObjInductor*>(m_pGraphObj);

    if( pGraphObjInductor == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjInductor == nullptr" );
    }

    if( m_pEdtInductance != nullptr )
    {
        m_pEdtInductance->setValue( pGraphObjInductor->getInductance() );
    }
    if( m_pChkInductanceVisible != nullptr )
    {
        m_pChkInductanceVisible->setChecked( pGraphObjInductor->isInductanceVisible() );
    }

} // resetChanges

//------------------------------------------------------------------------------
bool CWdgtFormatGraphObjsInductor::hasChanges() const
//------------------------------------------------------------------------------
{
    bool bHasChanges = false;

    CGraphObjInductor* pGraphObjInductor = dynamic_cast<CGraphObjInductor*>(m_pGraphObj);

    if( pGraphObjInductor == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjInductor == nullptr" );
    }

    if( m_pEdtInductance != nullptr )
    {
        if( pGraphObjInductor->getInductance() != m_pEdtInductance->value() )
        {
            bHasChanges = true;
        }
    }

    if( !bHasChanges && m_pChkInductanceVisible != nullptr )
    {
        bHasChanges = (pGraphObjInductor->isInductanceVisible() != m_pChkInductanceVisible->isChecked());
    }

    return bHasChanges;

} // hasChanges
