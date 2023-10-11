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

#include "ZSDraw/Widgets/Drawing/ZSDrawingViewDrawSettingsWdgt.h"
#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjFillStylePropertiesWdgt.h"
#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjLineStylePropertiesWdgt.h"
#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjTextStylePropertiesWdgt.h"
#include "ZSDraw/Widgets/Drawing/ZSDrawingView.h"
#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysRefCountGuard.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#if QT_VERSION < 0x050000
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qpushbutton.h>
#else
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qpushbutton.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Draw;


/*******************************************************************************
class CWdgtDrawingViewDrawSettings : public CWdgtGraphObjPropertiesAbstract
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
QString CWdgtDrawingViewDrawSettings::widgetName(EWidget i_widget)
//------------------------------------------------------------------------------
{
    QString str;
    if (i_widget == EWidget::LineStyle) {
        str = "LineStyle";
    }
    else if (i_widget == EWidget::FillStyle) {
        str = "FillStyle";
    }
    else if (i_widget == EWidget::TextStyle) {
        str = "TextStyle";
    }
    return str;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtDrawingViewDrawSettings::CWdgtDrawingViewDrawSettings(
    CDrawingScene* i_pDrawingScene, const QString& i_strObjName, QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    CWdgtGraphObjPropertiesAbstract(
        i_pDrawingScene,
        NameSpace() + "::Widgets", "Drawing",
        ClassName(), i_strObjName, i_pWdgtParent),
    m_pWdgtLineStyle(nullptr),
    m_pWdgtFillStyle(nullptr),
    m_pWdgtTextStyle(nullptr)
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    m_pWdgtLineStyle = new CWdgtGraphObjLineStyleProperties(
        i_pDrawingScene, NameSpace() + "::Widgets", "Drawing", i_strObjName);
    m_pLyt->addWidget(m_pWdgtLineStyle);
    QObject::connect(
        m_pWdgtLineStyle, &CWdgtGraphObjLineStyleProperties::contentChanged,
        this, &CWdgtDrawingViewDrawSettings::onWdgtLineStyleContentChanged);

    m_pWdgtFillStyle = new CWdgtGraphObjFillStyleProperties(
        i_pDrawingScene, NameSpace() + "::Widgets", "Drawing", i_strObjName);
    m_pLyt->addWidget(m_pWdgtFillStyle);
    QObject::connect(
        m_pWdgtFillStyle, &CWdgtGraphObjFillStyleProperties::contentChanged,
        this, &CWdgtDrawingViewDrawSettings::onWdgtFillStyleContentChanged);

    m_pWdgtTextStyle = new CWdgtGraphObjTextStyleProperties(
        i_pDrawingScene, NameSpace() + "::Widgets", "Drawing", i_strObjName);
    m_pLyt->addWidget(m_pWdgtTextStyle);
    QObject::connect(
        m_pWdgtTextStyle, &CWdgtGraphObjTextStyleProperties::contentChanged,
        this, &CWdgtDrawingViewDrawSettings::onWdgtTextStyleContentChanged);

    m_pLyt->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWdgtDrawingViewDrawSettings::~CWdgtDrawingViewDrawSettings()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_pWdgtLineStyle = nullptr;
    m_pWdgtFillStyle = nullptr;
    m_pWdgtTextStyle = nullptr;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDrawingViewDrawSettings::expand(EWidget i_widget, bool i_bExpand)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = widgetName(i_widget) + ", " + bool2Str(i_bExpand);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "expand",
        /* strAddInfo   */ strMthInArgs );

    if (i_widget == EWidget::LineStyle) {
        m_pWdgtLineStyle->expand(i_bExpand);
    }
    else if (i_widget == EWidget::FillStyle) {
        m_pWdgtFillStyle->expand(i_bExpand);
    }
    else if (i_widget == EWidget::TextStyle) {
        m_pWdgtTextStyle->expand(i_bExpand);
    }
}

/*==============================================================================
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtDrawingViewDrawSettings::setKeyInTree(const QString& i_strKeyInTree)
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

    bool bObjectChanged = false;

    if (m_strKeyInTree != i_strKeyInTree) {
        bObjectChanged = true;
        m_pWdgtLineStyle->setKeyInTree(i_strKeyInTree);
        m_pWdgtFillStyle->setKeyInTree(i_strKeyInTree);
        m_pWdgtTextStyle->setKeyInTree(i_strKeyInTree);
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bObjectChanged);
    }
    return bObjectChanged;
}

/*==============================================================================
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtDrawingViewDrawSettings::hasErrors() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hasErrors",
        /* strAddInfo   */ "" );

    bool bHasErrors = m_pWdgtLineStyle->hasErrors();
    if (!bHasErrors) {
        bHasErrors = m_pWdgtFillStyle->hasErrors();
    }
    if (!bHasErrors) {
        bHasErrors = m_pWdgtTextStyle->hasErrors();
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHasErrors);
    }
    return bHasErrors;
}

//------------------------------------------------------------------------------
bool CWdgtDrawingViewDrawSettings::hasChanges() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hasChanges",
        /* strAddInfo   */ "" );

    // If the graphical object is about to be destroyed, "onGraphObjChanged" is called
    // to update the content of the widget. But the child property widgets may not
    // have been informed yet that the graphical object is about to be destroyed as
    // here we are in the call stack of the "onGraphObjAboutToDestroyed" of this widget.
    // The child widgets slot may be called sometimes afterwards.

    bool bHasChanges = m_pWdgtLineStyle->hasChanges();
    if (!bHasChanges) {
        bHasChanges = m_pWdgtFillStyle->hasChanges();
    }
    if (!bHasChanges) {
        bHasChanges = m_pWdgtTextStyle->hasChanges();
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHasChanges);
    }
    return bHasChanges;
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewDrawSettings::acceptChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "acceptChanges",
        /* strAddInfo   */ "" );

    if (hasErrors()) {
        // Not really necessary as the apply button should be disabled if an
        // input control has an erronous value. But kept as an example how
        // the user could be informed that he should correct the values.
        QMessageBox::critical(
            this, ZS::System::GUI::getMainWindowTitle() + ": Cannot save changes",
            "Some input is erroneous. Please correct the relevant input first.");
    }
    else if (hasChanges()) {
        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

            // When applying changes onGraphObjChanged is called.
            // If the ContentChangedSignalBlockedCounter would not be incremented
            // the abstract base class would call fillEditControls of the base class.
            // This is not a problem as the method does nothing. But the method is not
            // expected to be called so we avoid it for the sake of clarification
            // (and to have a clear method trace output where the unexpected call is not listed).

            // The first calls should only temporarily store the changes.
            // Only the last applySettings call should update the settings.
            m_pWdgtLineStyle->applySettings(false);
            m_pWdgtFillStyle->applySettings(false);
            m_pWdgtTextStyle->applySettings(true);
        }
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewDrawSettings::rejectChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "rejectChanges",
        /* strAddInfo   */ "" );

    m_pWdgtLineStyle->rejectChanges();
    m_pWdgtFillStyle->rejectChanges();
    m_pWdgtTextStyle->rejectChanges();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDrawingViewDrawSettings::onWdgtLineStyleContentChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onWdgtLineStyleContentChanged",
        /* strAddInfo   */ "" );

    emit_contentChanged();
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewDrawSettings::onWdgtFillStyleContentChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onWdgtFillStyleContentChanged",
        /* strAddInfo   */ "" );

    emit_contentChanged();
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewDrawSettings::onWdgtTextStyleContentChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onWdgtTextStyleContentChanged",
        /* strAddInfo   */ "" );

    emit_contentChanged();
}
