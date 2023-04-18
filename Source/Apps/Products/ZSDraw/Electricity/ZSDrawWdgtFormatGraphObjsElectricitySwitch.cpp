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
#include <QtGui/qcombobox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qtextedit.h>
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qtextedit.h>
#endif

#include "Electricity/ZSDrawWdgtFormatGraphObjsElectricitySwitch.h"
#include "Electricity/ZSDrawGraphObjElectricitySwitch.h"

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
class CWdgtFormatGraphObjsSwitch : public CWdgtFormatGraphObjs
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsSwitch::CWdgtFormatGraphObjsSwitch(
    CDrawingScene* i_pDrawingScene,
    CGraphObj*     i_pGraphObj,
    QWidget*       i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtFormatGraphObjs(i_pDrawingScene,i_pGraphObj,i_pWdgtParent),
    m_pLyt(nullptr),
    m_pLblHeadLine(nullptr),
    m_pLytState(nullptr),
    m_pLblState(nullptr),
    m_pCmbState(nullptr),
    m_pChkStateVisible(nullptr),
    // Trace
    m_pTrcAdminObj(nullptr)
{
    setObjectName("WdgtFormatGraphObjsSwitch");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Products::Draw", "CWdgtFormatGraphObjsSwitch", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    QVBoxLayout* m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    int cxLblWidth = 90;
    //int cxEdtWidth = 80;
    int idxCmb;

    CGraphObjSwitch* pGraphObjSwitch = dynamic_cast<CGraphObjSwitch*>(m_pGraphObj);

    if( pGraphObjSwitch == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjSwitch == nullptr" );
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

    // <Line> State
    //=============

    m_pLytState = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytState);

    // <ComboBox> State
    //-----------------

    m_pLblState = new QLabel("State:");
    m_pLblState->setFixedWidth(cxLblWidth);
    m_pLytState->addWidget(m_pLblState);
    m_pCmbState = new QComboBox();
    m_pCmbState->addItem( CGraphObjSwitch::State2Str(CGraphObjSwitch::EStateOpened), CGraphObjSwitch::EStateOpened );
    m_pCmbState->addItem( CGraphObjSwitch::State2Str(CGraphObjSwitch::EStateClosed), CGraphObjSwitch::EStateClosed );
    idxCmb = m_pCmbState->findData(pGraphObjSwitch->getState());
    m_pCmbState->setCurrentIndex(idxCmb);
    m_pLytState->addWidget(m_pCmbState);

    // <CheckBox> Show State
    //----------------------

    m_pChkStateVisible = new QCheckBox("Visible");
    m_pChkStateVisible->setChecked( pGraphObjSwitch->isStateVisible() );
    m_pLytState->addWidget(m_pChkStateVisible);

    // <Stretch> at bottom of Widget
    //==============================

    m_pLyt->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsSwitch::~CWdgtFormatGraphObjsSwitch()
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
    m_pLytState = nullptr;
    m_pLblState = nullptr;
    m_pCmbState = nullptr;
    m_pChkStateVisible = nullptr;

    // Trace
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // must overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsSwitch::applyChanges()
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

    CGraphObjSwitch* pGraphObjSwitch = dynamic_cast<CGraphObjSwitch*>(m_pGraphObj);

    if( pGraphObjSwitch == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjSwitch == nullptr" );
    }

    if( m_pCmbState != nullptr )
    {
        int idxCmb = m_pCmbState->currentIndex();
        int iState = m_pCmbState->itemData(idxCmb).toInt();

        if( iState >= 0 && iState < CGraphObjSwitch::EStateCount )
        {
            CGraphObjSwitch::EState state = static_cast<CGraphObjSwitch::EState>(iState);
            pGraphObjSwitch->setState(state);
        }
    }

    if( m_pChkStateVisible != nullptr )
    {
        if( m_pChkStateVisible->isChecked() )
        {
            if( !pGraphObjSwitch->isStateVisible() )
            {
                pGraphObjSwitch->showState();
            }
        }
        else
        {
            if( pGraphObjSwitch->isStateVisible() )
            {
                pGraphObjSwitch->hideState();
            }
        }
    }

} // applyChanges

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsSwitch::resetChanges()
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

    CGraphObjSwitch* pGraphObjSwitch = dynamic_cast<CGraphObjSwitch*>(m_pGraphObj);

    if( pGraphObjSwitch == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjSwitch == nullptr" );
    }

    if( m_pCmbState != nullptr )
    {
        int idxCmb = m_pCmbState->findData(pGraphObjSwitch->getState());
        m_pCmbState->setCurrentIndex(idxCmb);
    }
    if( m_pChkStateVisible != nullptr )
    {
        m_pChkStateVisible->setChecked( pGraphObjSwitch->isStateVisible() );
    }

} // resetChanges

//------------------------------------------------------------------------------
bool CWdgtFormatGraphObjsSwitch::hasChanges() const
//------------------------------------------------------------------------------
{
    bool bHasChanges = false;

    CGraphObjSwitch* pGraphObjSwitch = dynamic_cast<CGraphObjSwitch*>(m_pGraphObj);

    if( pGraphObjSwitch == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjSwitch == nullptr" );
    }

    if( m_pCmbState != nullptr )
    {
        int idxCmb = m_pCmbState->currentIndex();
        int iState = m_pCmbState->itemData(idxCmb).toInt();

        if( pGraphObjSwitch->getState() != iState )
        {
            bHasChanges = true;
        }
    }

    if( !bHasChanges && m_pChkStateVisible != nullptr )
    {
        bHasChanges = (pGraphObjSwitch->isStateVisible() != m_pChkStateVisible->isChecked());
    }

    return bHasChanges;

} // hasChanges

/*==============================================================================
protected: // must overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsSwitch::onGraphObjChanged()
//------------------------------------------------------------------------------
{
} // onGraphObjChanged