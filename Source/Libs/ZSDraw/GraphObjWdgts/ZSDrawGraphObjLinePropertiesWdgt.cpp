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

#include "ZSDraw/GraphObjWdgts/ZSDrawGraphObjLinePropertiesWdgt.h"
#include "ZSDraw/GraphObjWdgts/ZSDrawGraphObjPropertiesLabelsWdgt.h"
#include "ZSDraw/GraphObjs/ZSDrawGraphObjLine.h"
//#include "ZSDraw/Common/ZSDrawCommon.h"
//#include "ZSDraw/Drawing/ZSDrawDlgDrawingViewSetup.h"
//#include "ZSDraw/Drawing/ZSDrawUnits.h"
#include "ZSDraw/Drawing/ZSDrawingView.h"
//#include "ZSPhysValGUI/ZSPhysValWdgtEditPhysVal.h"
//#include "ZSSysGUI/ZSSysSepLine.h"
//#include "ZSSys/ZSSysAux.h"
//#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysRefCountGuard.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

//#include <QtCore/qmetaobject.h>
//#include <QtCore/qcoreapplication.h>
//#include <QtGui/qstandarditemmodel.h>

#if QT_VERSION < 0x050000
//#include <QtGui/qcheckbox.h>
//#include <QtGui/qcolordialog.h>
//#include <QtGui/qcombobox.h>
//#include <QtGui/qfontcombobox.h>
//#include <QtGui/qgroupbox.h>
//#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
//#include <QtGui/qlineedit.h>
//#include <QtGui/qlistview.h>
#include <QtGui/qpushbutton.h>
//#include <QtGui/qspinbox.h>
#else
//#include <QtWidgets/qcheckbox.h>
//#include <QtWidgets/qcolordialog.h>
//#include <QtWidgets/qcombobox.h>
//#include <QtWidgets/qfontcombobox.h>
//#include <QtWidgets/qgroupbox.h>
//#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
//#include <QtWidgets/qlineedit.h>
//#include <QtWidgets/qlistview.h>
#include <QtWidgets/qpushbutton.h>
//#include <QtWidgets/qspinbox.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;


/*******************************************************************************
class CWdgtGraphObjLineProperties : public CWdgtGraphObjPropertiesAbstract
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtGraphObjLineProperties::CWdgtGraphObjLineProperties(
    CDrawingScene* i_pDrawingScene, const QString& i_strObjName, EMode i_mode, QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    CWdgtGraphObjPropertiesAbstract(i_pDrawingScene, "StandardShapes::" + ClassName(), i_strObjName, i_mode, i_pWdgtParent),
    m_pGraphObjLine(nullptr),
    m_pWdgtLabels(nullptr)
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumMode(i_mode).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    m_pWdgtLabels = new CWdgtGraphObjPropertiesLabels(
        i_pDrawingScene, "StandardShapes::" + ClassName(), i_strObjName, i_mode);
    m_pLyt->addWidget(m_pWdgtLabels);

    // <Buttons>
    //==========

    m_pLyt->addStretch(); // Add stretch to buttons line
    createButtonsLineWidget();
    m_pLyt->addWidget(m_pWdgtButtons);
    m_pWdgtButtons->hide();

    // Set Mode
    //---------

    setMode(i_mode);

} // ctor

//------------------------------------------------------------------------------
CWdgtGraphObjLineProperties::~CWdgtGraphObjLineProperties()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_pGraphObjLine = nullptr;
    m_pWdgtLabels = nullptr;

} // dtor

/*==============================================================================
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtGraphObjLineProperties::hasChanges() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hasChanges",
        /* strAddInfo   */ "" );

    bool bHasChanges = false;
    if (m_pWdgtLabels != nullptr) {
        bHasChanges = m_pWdgtLabels->hasChanges();
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHasChanges);
    }
    return bHasChanges;
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineProperties::acceptChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "acceptChanges",
        /* strAddInfo   */ "" );

    if (m_pWdgtLabels != nullptr) {
        m_pWdgtLabels->acceptChanges();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineProperties::rejectChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "rejectChanges",
        /* strAddInfo   */ "" );

    if (m_pWdgtLabels != nullptr) {
        m_pWdgtLabels->rejectChanges();
    }
}

/*==============================================================================
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjLineProperties::setMode(EMode i_mode)
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
        if (m_pWdgtLabels != nullptr) {
            m_pWdgtLabels->setMode(i_mode);
        }

        // Don't take over mode to member as base class method is also called and also needs
        // to check whether the mode has been changed to modify the edit button widget.
        if (i_mode == EMode::Edit)
        {
        }
        else // if (i_mode == EMode::View)
        {
        }
    }

    CWdgtGraphObjPropertiesAbstract::setMode(i_mode);
}

/*==============================================================================
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjLineProperties::setKeyInTree(const QString& i_strKeyInTree)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strKeyInTree;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setKeyInTree",
        /* strAddInfo   */ strMthInArgs );

    CWdgtGraphObjPropertiesAbstract::setKeyInTree(i_strKeyInTree);

    if (m_pGraphObj != nullptr && m_pGraphObj->type() == EGraphObjTypeLine)
    {
        m_pGraphObjLine = dynamic_cast<CGraphObjLine*>(m_pGraphObj);

        if (m_pWdgtLabels != nullptr) {
            m_pWdgtLabels->setKeyInTree(i_strKeyInTree);
        }
    }
    else
    {
        m_pGraphObjLine = nullptr;

        if (m_pWdgtLabels != nullptr) {
            m_pWdgtLabels->setKeyInTree("");
        }
    }
}

/*==============================================================================
protected: // must overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjLineProperties::onGraphObjChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onGraphObjChanged",
        /* strAddInfo   */ "" );
}
