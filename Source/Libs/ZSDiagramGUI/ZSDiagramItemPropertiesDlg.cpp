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

#include "ZSDiagramGUI/ZSDiagramItemPropertiesDlg.h"
#include "ZSDiagramGUI/ZSDiagramObjAxisLabelPropertiesWdgt.h"
#include "ZSDiagramGUI/ZSDiagramPropertiesWdgt.h"
#include "ZSDiagramGUI/ZSDiagramScalePropertiesWdgt.h"
#include "ZSDiagramGUI/ZSDiagramTracePropertiesWdgt.h"
#include "ZSDiagram/ZSDiagObjAxisLabel.h"
#include "ZSDiagram/ZSDiagObjCurve.h"
#include "ZSDiagram/ZSDiagObjErrInfo.h"
#include "ZSDiagram/ZSDiagObjGrid.h"
#include "ZSDiagram/ZSDiagObjHistogram.h"
#include "ZSDiagram/ZSDiagObjLabel.h"
#include "ZSDiagram/ZSDiagObjLine.h"
#include "ZSDiagram/ZSDiagObjMarker.h"
#include "ZSDiagram/ZSDiagObjProgressBar.h"
#include "ZSDiagram/ZSDiagObjRect.h"
#include "ZSDiagram/ZSDiagObjTable.h"
#include "ZSDiagram/ZSDiagObjValueProvider.h"
#include "ZSDiagram/ZSDiagramProcWdgt.h"
#include "ZSDiagram/ZSDiagScale.h"
#include "ZSDiagram/ZSDiagTrace.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include <QtCore/qsettings.h>

#if QT_VERSION < 0x050000
#include <QtGui/qlayout.h>
#include <QtGui/qpushbutton.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Diagram;
using namespace ZS::Diagram::GUI;


/*******************************************************************************
class CDlgDiagramItemProperties : public CDialog
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CDlgDiagramItemProperties* CDlgDiagramItemProperties::CreateInstance(
    const QString&  i_strDlgTitle,
    CWdgtDiagram*   i_pDiagram,
    const QString&  i_strDiagItemClassName,
    const QString&  i_strDiagItemObjName,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wFlags )
//------------------------------------------------------------------------------
{
    QString strDiagItemKey = i_strDiagItemClassName + "::" + i_strDiagItemObjName;

    if( CDialog::GetInstance(NameSpace(), ClassName(), strDiagItemKey) != nullptr )
    {
        QString strKey = buildPathStr("::", NameSpace(), ClassName(), strDiagItemKey);
        throw CException(__FILE__, __LINE__, EResultSingletonClassAlreadyInstantiated, strKey);
    }

    return new CDlgDiagramItemProperties(
        /* strDlgTitle  */ i_strDlgTitle,
        /* pDiagram     */ i_pDiagram,
        /* strClassName */ i_strDiagItemClassName,
        /* strObjName   */ i_strDiagItemObjName,
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags );
}

//------------------------------------------------------------------------------
CDlgDiagramItemProperties* CDlgDiagramItemProperties::GetInstance(
    const QString& i_strDiagItemClassName,
    const QString& i_strDiagItemObjName)
//------------------------------------------------------------------------------
{
    QString strDiagItemKey = i_strDiagItemClassName + "::" + i_strDiagItemObjName;
    return dynamic_cast<CDlgDiagramItemProperties*>(
        CDialog::GetInstance(NameSpace(), ClassName(), strDiagItemKey));
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgDiagramItemProperties::CDlgDiagramItemProperties(
    const QString&  i_strDlgTitle,
    CWdgtDiagram*   i_pDiagram,
    const QString&  i_strDiagItemClassName,
    const QString&  i_strDiagItemObjName,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wFlags ) :
//------------------------------------------------------------------------------
    CDialog(
        /* strDlgTitle  */ i_strDlgTitle,
        /* strNameSpace */ NameSpace(),
        /* strClassName */ ClassName(),
        /* strObjName   */ i_strDiagItemClassName + "::" + i_strDiagItemObjName,
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags ),
    m_pDiagram(i_pDiagram),
    m_strDiagItemClassName(i_strDiagItemClassName),
    m_strDiagItemObjName(i_strDiagItemObjName),
    m_pLyt(nullptr),
    m_pWdgtDiagItemProperties(nullptr),
    m_pLytLineBtns(nullptr),
    m_pBtnOk(nullptr),
    m_pBtnApply(nullptr),
    m_pBtnReset(nullptr),
    m_pBtnCancel(nullptr)
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    QVBoxLayout* m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    QHBoxLayout* m_pLytSettings = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytSettings);

    m_pWdgtDiagItemProperties = createDiagItemPropertiesWdgt();

    if (m_pWdgtDiagItemProperties != nullptr)
    {
        m_pWdgtDiagItemProperties->setDiagItemObjName(i_strDiagItemObjName);

        m_pLyt->addWidget(m_pWdgtDiagItemProperties);

        QObject::connect(
            m_pWdgtDiagItemProperties, &CWdgtDiagramItemPropertiesBase::modeChanged,
            this, &CDlgDiagramItemProperties::onWdgtModeChanged);
        QObject::connect(
            m_pWdgtDiagItemProperties, &CWdgtDiagramItemPropertiesBase::diagItemDestroyed,
            this, &CDlgDiagramItemProperties::onWdgtDiagItemDestroyed);
        QObject::connect(
            m_pWdgtDiagItemProperties, &CWdgtDiagramItemPropertiesBase::diagItemObjNameChanged,
            this, &CDlgDiagramItemProperties::onWdgtDiagItemObjNameChanged);
        QObject::connect(
            m_pWdgtDiagItemProperties, &CWdgtDiagramItemPropertiesBase::diagItemPropertyChanged,
            this, &CDlgDiagramItemProperties::onWdgtDiagItemPropertyChanged);
    }

    // Dialog buttons
    //================

    m_pLytLineBtns = new QHBoxLayout();
    m_pLyt->addSpacing(5);
    m_pLyt->addLayout(m_pLytLineBtns);

    m_pBtnOk = new QPushButton("Ok");
    m_pLytLineBtns->addWidget(m_pBtnOk);
    m_pBtnOk->setDefault(true);
    QObject::connect(
        m_pBtnOk, &QPushButton::clicked,
        this, &CDlgDiagramItemProperties::onBtnOkClicked);

    m_pBtnApply = new QPushButton("Apply");
    m_pBtnApply->setEnabled(false);
    m_pLytLineBtns->addWidget(m_pBtnApply);
    QObject::connect(
        m_pBtnApply, &QPushButton::clicked,
        this, &CDlgDiagramItemProperties::onBtnApplyClicked);

    m_pBtnReset = new QPushButton("Reset");
    m_pBtnReset->setEnabled(false);
    m_pLytLineBtns->addWidget(m_pBtnReset);
    QObject::connect(
        m_pBtnReset, &QPushButton::clicked,
        this, &CDlgDiagramItemProperties::onBtnResetClicked);

    m_pBtnCancel = new QPushButton("Cancel");
    m_pLytLineBtns->addWidget(m_pBtnCancel);
    QObject::connect(
        m_pBtnCancel, &QPushButton::clicked,
        this, &CDlgDiagramItemProperties::onBtnCancelClicked);

    m_pLytLineBtns->addStretch();

    // Geometry of dialog
    //===================

    setMinimumHeight(360);
    setMinimumWidth(460);

    // Restore Geometry
    //-----------------

    QSettings settings;
    restoreGeometry( settings.value(objectName()+"/Geometry").toByteArray() );

} // ctor

//------------------------------------------------------------------------------
CDlgDiagramItemProperties::~CDlgDiagramItemProperties()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    QSettings settings;
    settings.setValue( objectName()+"/Geometry", saveGeometry() );

    m_pDiagram = nullptr;
    //m_strDiagItemClassName;
    //m_strDiagItemObjName;
    m_pLyt = nullptr;
    m_pWdgtDiagItemProperties = nullptr;
    m_pLytLineBtns = nullptr;
    m_pBtnOk = nullptr;
    m_pBtnApply = nullptr;
    m_pBtnReset = nullptr;
    m_pBtnCancel = nullptr;

} // dtor

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgDiagramItemProperties::onBtnOkClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnOkClicked",
        /* strAddInfo   */ "" );

    if (m_pWdgtDiagItemProperties != nullptr)
    {
        m_pWdgtDiagItemProperties->acceptChanges();
    }

    QDialog::accept();
}

//------------------------------------------------------------------------------
void CDlgDiagramItemProperties::onBtnApplyClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnApplyClicked",
        /* strAddInfo   */ "" );

    if (m_pWdgtDiagItemProperties != nullptr)
    {
        m_pWdgtDiagItemProperties->acceptChanges();

        if( m_pWdgtDiagItemProperties->hasChanges() )
        {
            m_pBtnApply->setEnabled(true);
            m_pBtnReset->setEnabled(true);
        }
        else
        {
            m_pBtnApply->setEnabled(false);
            m_pBtnReset->setEnabled(false);
        }
    }
}

//------------------------------------------------------------------------------
void CDlgDiagramItemProperties::onBtnResetClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnResetClicked",
        /* strAddInfo   */ "" );

    if (m_pWdgtDiagItemProperties != nullptr)
    {
        m_pWdgtDiagItemProperties->rejectChanges();

        if( m_pWdgtDiagItemProperties->hasChanges() )
        {
            m_pBtnApply->setEnabled(true);
            m_pBtnReset->setEnabled(true);
        }
        else
        {
            m_pBtnApply->setEnabled(false);
            m_pBtnReset->setEnabled(false);
        }
    }
}

//------------------------------------------------------------------------------
void CDlgDiagramItemProperties::onBtnCancelClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnCancelClicked",
        /* strAddInfo   */ "" );

    if (m_pWdgtDiagItemProperties != nullptr)
    {
        m_pWdgtDiagItemProperties->rejectChanges();
    }
    QDialog::reject();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgDiagramItemProperties::onWdgtModeChanged()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onWdgtModeChanged",
        /* strAddInfo   */ strMthInArgs );

}

//------------------------------------------------------------------------------
void CDlgDiagramItemProperties::onWdgtDiagItemDestroyed(QObject* i_pObj)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onWdgtDiagItemDestroyed",
        /* strAddInfo   */ strMthInArgs );

    QDialog::reject();
}

//------------------------------------------------------------------------------
void CDlgDiagramItemProperties::onWdgtDiagItemObjNameChanged()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onWdgtDiagItemObjNameChanged",
        /* strAddInfo   */ strMthInArgs );

    if( m_pWdgtDiagItemProperties->hasChanges() )
    {
        m_pBtnApply->setEnabled(true);
        m_pBtnReset->setEnabled(true);
    }
    else
    {
        m_pBtnApply->setEnabled(false);
        m_pBtnReset->setEnabled(false);
    }
}

//------------------------------------------------------------------------------
void CDlgDiagramItemProperties::onWdgtDiagItemPropertyChanged()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onWdgtDiagItemPropertyChanged",
        /* strAddInfo   */ strMthInArgs );

    if( m_pWdgtDiagItemProperties->hasChanges() )
    {
        m_pBtnApply->setEnabled(true);
        m_pBtnReset->setEnabled(true);
    }
    else
    {
        m_pBtnApply->setEnabled(false);
        m_pBtnReset->setEnabled(false);
    }
}

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtDiagramItemPropertiesBase* CDlgDiagramItemProperties::createDiagItemPropertiesWdgt()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = m_strDiagItemClassName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createDiagItemPropertiesWdgt",
        /* strAddInfo   */ strMthInArgs );

    CWdgtDiagramItemPropertiesBase* pWdgt = nullptr;

    if (m_strDiagItemClassName == CWdgtDiagram::ClassName())
    {
        pWdgt = new CWdgtDiagramProperties(m_pDiagram, EMode::Edit);
    }
    else if (m_strDiagItemClassName == CDiagScale::ClassName())
    {
        pWdgt = new CWdgtDiagramScaleProperties(m_pDiagram, EMode::Edit);
    }
    else if (m_strDiagItemClassName == CDiagTrace::ClassName())
    {
        pWdgt = new CWdgtDiagramTraceProperties(m_pDiagram, EMode::Edit);
    }
    else if (m_strDiagItemClassName == CDiagObjAxisLabel::ClassName())
    {
        pWdgt = new CWdgtDiagramObjAxisLabelProperties(m_pDiagram, EMode::Edit);
    }
    #pragma message(__TODO__"Implement concrete DiagObj Properties Widget classes")
    else if (m_strDiagItemClassName == CDiagObjCurve::ClassName())
    {
        pWdgt = new CWdgtDiagramObjPropertiesBase(
            "CWdgtDiagramObjCurveProperties", m_pDiagram, CDiagObjCurve::ClassName(), EMode::Edit);
    }
    else if (m_strDiagItemClassName == CDiagObjErrInfo::ClassName())
    {
        pWdgt = new CWdgtDiagramObjPropertiesBase(
            "CWdgtDiagramObjErrInfoProperties", m_pDiagram, CDiagObjErrInfo::ClassName(), EMode::Edit);
    }
    else if (m_strDiagItemClassName == CDiagObjGrid::ClassName())
    {
        pWdgt = new CWdgtDiagramObjPropertiesBase(
            "CWdgtDiagramObjGridProperties", m_pDiagram, CDiagObjGrid::ClassName(), EMode::Edit);
    }
    else if (m_strDiagItemClassName == CDiagObjHistogram::ClassName())
    {
        pWdgt = new CWdgtDiagramObjPropertiesBase(
            "CWdgtDiagramObjHistogramProperties", m_pDiagram, CDiagObjHistogram::ClassName(), EMode::Edit);
    }
    else if (m_strDiagItemClassName == CDiagObjLabel::ClassName())
    {
        pWdgt = new CWdgtDiagramObjPropertiesBase(
            "CWdgtDiagramObjLabelProperties", m_pDiagram, CDiagObjLabel::ClassName(), EMode::Edit);
    }
    else if (m_strDiagItemClassName == CDiagObjLine::ClassName())
    {
        pWdgt = new CWdgtDiagramObjPropertiesBase(
            "CWdgtDiagramObjLineProperties", m_pDiagram, CDiagObjLine::ClassName(), EMode::Edit);
    }
    else if (m_strDiagItemClassName == CDiagObjMarker::ClassName())
    {
        pWdgt = new CWdgtDiagramObjPropertiesBase(
            "CWdgtDiagramObjMarkerProperties", m_pDiagram, CDiagObjMarker::ClassName(), EMode::Edit);
    }
    else if (m_strDiagItemClassName == CDiagObjProgressBar::ClassName())
    {
        pWdgt = new CWdgtDiagramObjPropertiesBase(
            "CWdgtDiagramObjProgressBarProperties", m_pDiagram, CDiagObjProgressBar::ClassName(), EMode::Edit);
    }
    else if (m_strDiagItemClassName == CDiagObjRect::ClassName())
    {
        pWdgt = new CWdgtDiagramObjPropertiesBase(
            "CWdgtDiagramObjRectProperties", m_pDiagram, CDiagObjRect::ClassName(), EMode::Edit);
    }
    else if (m_strDiagItemClassName == CDiagObjTable::ClassName())
    {
        pWdgt = new CWdgtDiagramObjPropertiesBase(
            "CWdgtDiagramObjTableProperties", m_pDiagram, CDiagObjTable::ClassName(), EMode::Edit);
    }
    else if (m_strDiagItemClassName == CDiagObjValueProvider::ClassName())
    {
        pWdgt = new CWdgtDiagramObjPropertiesBase(
            "CWdgtDiagramObjValueProviderProperties", m_pDiagram, CDiagObjValueProvider::ClassName(), EMode::Edit);
    }
    return pWdgt;
}
