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

#include "ZSDiagramGUI/ZSDiagramItemPropertiesBaseWdgt.h"
#include "ZSDiagramGUI/ZSDiagramItemPropertiesDlg.h"
#include "ZSDiagram/ZSDiagramProcWdgt.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtCore/qcoreapplication.h>

#if QT_VERSION < 0x050000
#include <QtGui/qlayout.h>
#include <QtGui/qpushbutton.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Diagram;
using namespace ZS::Diagram::GUI;


/*******************************************************************************
class CWdgtDiagramItemPropertiesBase : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtDiagramItemPropertiesBase::CWdgtDiagramItemPropertiesBase(
    const QString& i_strWdgtClassName,
    CWdgtDiagram* i_pDiagram,
    const QString& i_strDiagItemClassName,
    EMode i_mode,
    QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_strWdgtClassName(i_strWdgtClassName),
    m_pDiagram(i_pDiagram),
    m_mode(EMode::Edit),
    m_strDiagItemClassName(i_strDiagItemClassName),
    m_strDiagItemObjName(),
    m_cxLblWidthClm1(60),
    m_cxEdtWidthClm1(120),
    m_cxLblWidthClm2(60),
    m_cxEdtWidthClm2(120),
    m_cxLblWidthClm3(60),
    m_cxEdtWidthClm3(120),
    m_cxLblWidthClm4(60),
    m_cxEdtWidthClm4(120),
    m_cxClmSpacing(30),
    m_iValueChangedSignalsBlocked(0),
    // Edit Controls
    m_pLyt(nullptr),
    // Button Line
    m_pWdgtButtons(nullptr),
    m_pLytWdgtButtons(nullptr),
    m_pBtnEdit(nullptr),
    // Trace admin object for method tracing
    m_pTrcAdminObj(nullptr)
{
    if (i_pDiagram->objectName() != "Diagram") {
        setObjectName(i_pDiagram->objectName()+ "-" + CEnumMode(i_mode).toString());
    }
    else {
        setObjectName(CEnumMode(i_mode).toString());
    }


    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace(), m_strWdgtClassName, objectName());

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pDiagram->objectName() + ", " + i_strDiagItemClassName + ", " + CEnumMode(i_mode).toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    QObject::connect(
        m_pDiagram, &CWdgtDiagram::aboutToBeDestroyed,
        this, &CWdgtDiagramItemPropertiesBase::onDiagramAboutToBeDestroyed);

} // ctor

//------------------------------------------------------------------------------
CWdgtDiagramItemPropertiesBase::~CWdgtDiagramItemPropertiesBase()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ "" );

    mthTracer.onAdminObjAboutToBeReleased();
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    //m_strWdgtClassName;
    m_pDiagram = nullptr;
    m_mode = static_cast<EMode>(0);
    //m_strDiagItemClassName;
    //m_strDiagItemObjName;
    m_cxLblWidthClm1 = 0;
    m_cxEdtWidthClm1 = 0;
    m_cxLblWidthClm2 = 0;
    m_cxEdtWidthClm2 = 0;
    m_cxLblWidthClm3 = 0;
    m_cxEdtWidthClm3 = 0;
    m_cxLblWidthClm4 = 0;
    m_cxEdtWidthClm4 = 0;
    m_cxClmSpacing = 0;
    m_iValueChangedSignalsBlocked = 0;
    // Button Line
    m_pWdgtButtons = nullptr;
    m_pLytWdgtButtons = nullptr;
    m_pBtnEdit = nullptr;
    // Trace admin object for method tracing

} // dtor

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagramItemPropertiesBase::addButtonsWidget()
//------------------------------------------------------------------------------
{
    // <Buttons>
    //==========

    m_pWdgtButtons = new QWidget();
    m_pLytWdgtButtons = new QHBoxLayout();
    m_pWdgtButtons->setLayout(m_pLytWdgtButtons);
    m_pLytWdgtButtons->setContentsMargins(0, 0, 0, 0);
    m_pLyt->addWidget(m_pWdgtButtons);

    m_pBtnEdit = new QPushButton("Edit");
    m_pBtnEdit->setEnabled(false);
    m_pLytWdgtButtons->addWidget(m_pBtnEdit);
    QObject::connect(
        m_pBtnEdit, &QPushButton::clicked,
        this, &CWdgtDiagramItemPropertiesBase::onBtnEditClicked);

    m_pLytWdgtButtons->addStretch();

    m_pWdgtButtons->hide();
}

//------------------------------------------------------------------------------
void CWdgtDiagramItemPropertiesBase::setMode(EMode i_mode)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = CEnumMode(i_mode).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setMode",
        /* strAddInfo   */ strMthInArgs );

    if (m_mode != i_mode)
    {
        m_mode = i_mode;

        if (m_mode == EMode::Edit)
        {
            m_pBtnEdit->setEnabled(false);
        }
        else // if (m_mode == EMode::View)
        {
            m_pBtnEdit->setEnabled(true);
        }
    }
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagramItemPropertiesBase::setDiagItemObjName( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setDiagItemObjName",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_strDiagItemObjName != i_strObjName )
    {
        m_strDiagItemObjName = i_strObjName;

        if (m_strDiagItemClassName.isEmpty() || m_strDiagItemObjName.isEmpty()) {
            if (m_pWdgtButtons != nullptr) {
                m_pWdgtButtons->hide();
            }
        }
        else {
            if (m_pWdgtButtons != nullptr) {
                m_pWdgtButtons->show();
            }
        }
        emit_diagItemObjNameChanged();
    }
}

//------------------------------------------------------------------------------
QString CWdgtDiagramItemPropertiesBase::diagItemObjName() const
//------------------------------------------------------------------------------
{
    return m_strDiagItemObjName;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtDiagramItemPropertiesBase::hasChanges() const
//------------------------------------------------------------------------------
{
    return false;
}

//------------------------------------------------------------------------------
void CWdgtDiagramItemPropertiesBase::acceptChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "acceptChanges",
        /* strAddInfo   */ "" );
}

//------------------------------------------------------------------------------
void CWdgtDiagramItemPropertiesBase::rejectChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "rejectChanges",
        /* strAddInfo   */ "" );
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagramItemPropertiesBase::onBtnEditClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnEditClicked",
        /* strAddInfo   */ "" );

    QString strItemKey = m_strDiagItemClassName + "::" + m_strDiagItemObjName;
    QString strDlgTitle =
        QCoreApplication::applicationName() + ": Edit Properties of " + strItemKey;
    CDlgDiagramItemProperties* pDlg = CDlgDiagramItemProperties::GetInstance(
        m_strDiagItemClassName, m_strDiagItemObjName);
    if( pDlg == nullptr ) {
        pDlg = CDlgDiagramItemProperties::CreateInstance(
            strDlgTitle, m_pDiagram, m_strDiagItemClassName, m_strDiagItemObjName);
        pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
        pDlg->adjustSize();
        pDlg->show();
    }
    else {
        if( pDlg->isHidden() ) {
            pDlg->show();
        }
        pDlg->raise();
        pDlg->activateWindow();
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagramItemPropertiesBase::onDiagramAboutToBeDestroyed(const QString& /*i_strObjName*/)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onDiagramAboutToBeDestroyed",
        /* strMethodInArgs    */ strMthInArgs );

    m_pDiagram = nullptr;
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagramItemPropertiesBase::emit_modeChanged()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "emit_modeChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit modeChanged();
}

//------------------------------------------------------------------------------
void CWdgtDiagramItemPropertiesBase::emit_diagItemDestroyed(QObject* i_pObj)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "emit_diagItemDestroyed",
        /* strMethodInArgs    */ strMthInArgs );

    emit diagItemDestroyed(i_pObj);
}

//------------------------------------------------------------------------------
void CWdgtDiagramItemPropertiesBase::emit_diagItemObjNameChanged()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "emit_diagItemObjNameChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit diagItemObjNameChanged();
}

//------------------------------------------------------------------------------
void CWdgtDiagramItemPropertiesBase::emit_diagItemPropertyChanged()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "emit_diagItemPropertyChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit diagItemPropertyChanged();
}
