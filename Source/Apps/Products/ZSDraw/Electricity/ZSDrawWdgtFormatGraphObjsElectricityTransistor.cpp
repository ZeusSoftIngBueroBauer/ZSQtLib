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

#include "Electricity/ZSDrawWdgtFormatGraphObjsElectricityTransistor.h"
#include "Electricity/ZSDrawGraphObjElectricityTransistor.h"

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
class CWdgtFormatGraphObjsTransistor : public CWdgtFormatGraphObjs
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsTransistor::CWdgtFormatGraphObjsTransistor(
    CDrawingScene* i_pDrawingScene,
    CGraphObj*     i_pGraphObj,
    QWidget*       i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtFormatGraphObjs(i_pDrawingScene,i_pGraphObj,i_pWdgtParent),
    m_pLyt(nullptr),
    m_pLblHeadLine(nullptr),
    m_pLytDopingType(nullptr),
    m_pLblDopingType(nullptr),
    m_pCmbDopingType(nullptr),
    m_pChkDopingTypeVisible(nullptr),
    // Trace
    m_pTrcAdminObj(nullptr)
{
    setObjectName("WdgtFormatGraphObjsTransistor");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Products::Draw", "CWdgtFormatGraphObjsTransistor", objectName());

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

    CGraphObjTransistor* pGraphObjTransistor = dynamic_cast<CGraphObjTransistor*>(m_pGraphObj);

    if( pGraphObjTransistor == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjTransistor == nullptr" );
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

    // <Line> Doping Type
    //===================

    m_pLytDopingType = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytDopingType);

    // <ComboBox> Doping Type
    //-----------------------

    m_pLblDopingType = new QLabel("Doping Type:");
    m_pLblDopingType->setFixedWidth(cxLblWidth);
    m_pLytDopingType->addWidget(m_pLblDopingType);
    m_pCmbDopingType = new QComboBox();
    m_pCmbDopingType->addItem( CGraphObjTransistor::DopingType2Str(CGraphObjTransistor::EDopingTypeNPN), CGraphObjTransistor::EDopingTypeNPN );
    m_pCmbDopingType->addItem( CGraphObjTransistor::DopingType2Str(CGraphObjTransistor::EDopingTypePNP), CGraphObjTransistor::EDopingTypePNP );
    idxCmb = m_pCmbDopingType->findData(pGraphObjTransistor->getDopingType());
    m_pCmbDopingType->setCurrentIndex(idxCmb);
    m_pLytDopingType->addWidget(m_pCmbDopingType);

    // <CheckBox> Show Doping Type
    //----------------------------

    m_pChkDopingTypeVisible = new QCheckBox("Visible");
    m_pChkDopingTypeVisible->setChecked( pGraphObjTransistor->isDopingTypeVisible() );
    m_pLytDopingType->addWidget(m_pChkDopingTypeVisible);

    // <Stretch> at bottom of Widget
    //==============================

    m_pLyt->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsTransistor::~CWdgtFormatGraphObjsTransistor()
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
    m_pLytDopingType = nullptr;
    m_pLblDopingType = nullptr;
    m_pCmbDopingType = nullptr;
    m_pChkDopingTypeVisible = nullptr;

    // Trace
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // must overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsTransistor::applyChanges()
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

    CGraphObjTransistor* pGraphObjTransistor = dynamic_cast<CGraphObjTransistor*>(m_pGraphObj);

    if( pGraphObjTransistor == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjTransistor == nullptr" );
    }

    if( m_pCmbDopingType != nullptr )
    {
        int idxCmb = m_pCmbDopingType->currentIndex();
        int iType  = m_pCmbDopingType->itemData(idxCmb).toInt();

        if( iType >= 0 && iType < CGraphObjTransistor::EDopingTypeCount )
        {
            CGraphObjTransistor::EDopingType type = static_cast<CGraphObjTransistor::EDopingType>(iType);
            pGraphObjTransistor->setDopingType(type);
        }
    }

    if( m_pChkDopingTypeVisible != nullptr )
    {
        if( m_pChkDopingTypeVisible->isChecked() )
        {
            if( !pGraphObjTransistor->isDopingTypeVisible() )
            {
                pGraphObjTransistor->showDopingType();
            }
        }
        else
        {
            if( pGraphObjTransistor->isDopingTypeVisible() )
            {
                pGraphObjTransistor->hideDopingType();
            }
        }
    }

} // applyChanges

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsTransistor::resetChanges()
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

    CGraphObjTransistor* pGraphObjTransistor = dynamic_cast<CGraphObjTransistor*>(m_pGraphObj);

    if( pGraphObjTransistor == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjTransistor == nullptr" );
    }

    if( m_pCmbDopingType != nullptr )
    {
        int idxCmb = m_pCmbDopingType->findData(pGraphObjTransistor->getDopingType());
        m_pCmbDopingType->setCurrentIndex(idxCmb);
    }
    if( m_pChkDopingTypeVisible != nullptr )
    {
        m_pChkDopingTypeVisible->setChecked( pGraphObjTransistor->isDopingTypeVisible() );
    }

} // resetChanges

//------------------------------------------------------------------------------
bool CWdgtFormatGraphObjsTransistor::hasChanges() const
//------------------------------------------------------------------------------
{
    bool bHasChanges = false;

    CGraphObjTransistor* pGraphObjTransistor = dynamic_cast<CGraphObjTransistor*>(m_pGraphObj);

    if( pGraphObjTransistor == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjTransistor == nullptr" );
    }

    if( m_pCmbDopingType != nullptr )
    {
        int idxCmb = m_pCmbDopingType->currentIndex();
        int iType  = m_pCmbDopingType->itemData(idxCmb).toInt();

        if( pGraphObjTransistor->getDopingType() != iType )
        {
            bHasChanges = true;
        }
    }

    if( !bHasChanges && m_pChkDopingTypeVisible != nullptr )
    {
        bHasChanges = (pGraphObjTransistor->isDopingTypeVisible() != m_pChkDopingTypeVisible->isChecked());
    }

    return bHasChanges;

} // hasChanges

/*==============================================================================
protected: // must overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsTransistor::onGraphObjChanged()
//------------------------------------------------------------------------------
{
} // onGraphObjChanged