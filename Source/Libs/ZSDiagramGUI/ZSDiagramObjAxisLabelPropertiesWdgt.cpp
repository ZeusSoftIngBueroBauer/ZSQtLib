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

#include "ZSDiagramGUI/ZSDiagramObjAxisLabelPropertiesWdgt.h"
#include "ZSDiagram/ZSDiagObjAxisLabel.h"
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
class CWdgtDiagramObjAxisLabelProperties : public CWdgtDiagramObjPropertiesBase
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtDiagramObjAxisLabelProperties::CWdgtDiagramObjAxisLabelProperties(
    CWdgtDiagram* i_pDiagram,
    EMode i_mode,
    QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    CWdgtDiagramObjPropertiesBase(ClassName(), i_pDiagram, "CDiagObjAxisLabel", i_mode, i_pWdgtParent),
    m_pDiagObj(nullptr)
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pDiagram->objectName() + ", " + CEnumMode(i_mode).toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );

    // <Controls>
    //===========


    // <Buttons>
    //==========

    m_pLyt->addStretch();

    if (i_mode == EMode::View)
    {
        addButtonsWidget();
    }

    // <Mode>
    //==========

    setMode(i_mode);

} // ctor

//------------------------------------------------------------------------------
CWdgtDiagramObjAxisLabelProperties::~CWdgtDiagramObjAxisLabelProperties()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ "" );

    m_pDiagObj = nullptr;

} // dtor

/*==============================================================================
protected: // overridable init methods of base class CWdgtDiagramItemPropertiesBase
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagramObjAxisLabelProperties::setMode(EMode i_mode)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
            if (m_pWdgtButtons != nullptr)
            {
                m_pBtnEdit->setEnabled(false);
                m_pWdgtButtons->hide();
            }
        }
        else // if (m_mode == EMode::View)
        {
            if (m_pWdgtButtons != nullptr)
            {
                m_pBtnEdit->setEnabled(m_pDiagObj != nullptr);
                m_pWdgtButtons->setVisible(m_pDiagObj != nullptr);
            }
        }
        emit modeChanged();
    }
}

/*==============================================================================
public: // overridables of base class CWdgtDiagramItemPropertiesBase
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagramObjAxisLabelProperties::setDiagItemObjName( const QString& i_strObjName )
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

        if (m_pDiagObj != nullptr)
        {
            QObject::disconnect(
                m_pDiagObj, &QObject::destroyed,
                this, &CWdgtDiagramObjAxisLabelProperties::onDiagObjDestroyed);
        }

        m_pDiagObj = dynamic_cast<CDiagObjAxisLabel*>(m_pDiagram->findDiagObj(
            m_strDiagItemClassName, m_strDiagItemObjName));

        if (m_pDiagObj == nullptr)
        {
            if (m_pWdgtButtons != nullptr)
            {
                m_pBtnEdit->setEnabled(false);
                m_pWdgtButtons->hide();
            }
        }
        else
        {
            if (m_pWdgtButtons != nullptr)
            {
                m_pBtnEdit->setEnabled(m_mode == EMode::View);
                m_pWdgtButtons->setVisible(m_mode == EMode::View);
            }

            QObject::connect(
                m_pDiagObj, &QObject::destroyed,
                this, &CWdgtDiagramObjAxisLabelProperties::onDiagObjDestroyed);
        }

        emit_diagItemObjNameChanged();
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagramObjAxisLabelProperties::onDiagObjDestroyed(QObject* i_pObj)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDiagObjDestroyed",
        /* strAddInfo   */ strMthInArgs );

    m_pDiagObj = nullptr;

    setDiagItemObjName("");

    emit diagItemDestroyed(i_pObj);
}
