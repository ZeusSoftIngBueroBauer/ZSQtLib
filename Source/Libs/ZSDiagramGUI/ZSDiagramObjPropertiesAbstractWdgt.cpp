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

#include "ZSDiagramGUI/ZSDiagramObjPropertiesAbstractWdgt.h"
#include "ZSDiagram/ZSDiagramProcWdgt.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

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
class CWdgtDiagramObjPropertiesAbstract : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtDiagramObjPropertiesAbstract::CWdgtDiagramObjPropertiesAbstract(
    CWdgtDiagram* i_pDiagram,
    const QString& i_strDiagObjClassName,
    EMode i_mode,
    QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pDiagram(i_pDiagram),
    m_mode(EMode::Edit),
    m_strDiagObjClassName(i_strDiagObjClassName),
    m_strDiagObjName(),
    m_cxLblWidthClm1(80),
    m_cxEdtWidthClm1(160),
    m_cxLblWidthClm2(80),
    m_cxEdtWidthClm2(160),
    m_cxClmSpacing(30),
    // Edit Controls
    m_pLyt(nullptr),
    // Button Line
    m_pWdgtButtons(nullptr),
    m_pLytWdgtButtons(nullptr),
    m_pBtnEdit(nullptr),
    // Trace admin object for method tracing
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_pDiagram->objectName() + "::" + i_strDiagObjClassName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName(), objectName());

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pDiagram->objectName() + ", " + i_strDiagObjClassName;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );

    QObject::connect(
        m_pDiagram, &CWdgtDiagram::aboutToBeDestroyed,
        this, &CWdgtDiagramObjPropertiesAbstract::onDiagramAboutToBeDestroyed);

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    // <Stretch> to buttons line
    //==========================

    m_pLyt->addStretch();

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
        this, &CWdgtDiagramObjPropertiesAbstract::onBtnEditClicked);

    m_pLytWdgtButtons->addStretch();

    m_pWdgtButtons->hide();

    // Set Mode
    //---------

    setMode(i_mode);

} // ctor

//------------------------------------------------------------------------------
CWdgtDiagramObjPropertiesAbstract::~CWdgtDiagramObjPropertiesAbstract()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ "" );

    mthTracer.onAdminObjAboutToBeReleased();
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pDiagram = nullptr;
    m_mode = static_cast<EMode>(0);
    //m_strDiagObjClassName;
    //m_strDiagObjName;
    m_cxLblWidthClm1 = 0;
    m_cxEdtWidthClm1 = 0;
    m_cxLblWidthClm2 = 0;
    m_cxEdtWidthClm2 = 0;
    m_cxClmSpacing = 0;
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
void CWdgtDiagramObjPropertiesAbstract::setDiagObjName( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setDiagObjName",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_strDiagObjName != i_strObjName )
    {
        m_strDiagObjName = i_strObjName;

        if (m_strDiagObjClassName.isEmpty() || m_strDiagObjName.isEmpty()) {
            m_pWdgtButtons->hide();
        }
        else {
            m_pWdgtButtons->show();
       }
    }
}

//------------------------------------------------------------------------------
QString CWdgtDiagramObjPropertiesAbstract::diagObjName() const
//------------------------------------------------------------------------------
{
    return m_strDiagObjName;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtDiagramObjPropertiesAbstract::hasChanges() const
//------------------------------------------------------------------------------
{
    return false;
}

//------------------------------------------------------------------------------
void CWdgtDiagramObjPropertiesAbstract::acceptChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "acceptChanges",
        /* strAddInfo   */ "" );
}

//------------------------------------------------------------------------------
void CWdgtDiagramObjPropertiesAbstract::rejectChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "rejectChanges",
        /* strAddInfo   */ "" );
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagramObjPropertiesAbstract::setMode(EMode i_mode)
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

//------------------------------------------------------------------------------
EMode CWdgtDiagramObjPropertiesAbstract::mode() const
//------------------------------------------------------------------------------
{
    return m_mode;
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagramObjPropertiesAbstract::onBtnEditClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnEditClicked",
        /* strAddInfo   */ "" );
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagramObjPropertiesAbstract::onDiagramAboutToBeDestroyed(const QString& /*i_strObjName*/)
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
