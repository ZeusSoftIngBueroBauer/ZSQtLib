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

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjGroupPropertiesWdgt.h"
#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjGroupGeometryPropertiesWdgt.h"
#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjFillStylePropertiesWdgt.h"
#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjGridStylePropertiesWdgt.h"
#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjLineStylePropertiesWdgt.h"
#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjLabelsPropertiesWdgt.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLine.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
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
class CWdgtGraphObjGroupProperties : public CWdgtGraphObjPropertiesAbstract
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
QString CWdgtGraphObjGroupProperties::widgetName(EWidget i_widget)
//------------------------------------------------------------------------------
{
    QString str;
    if (i_widget == EWidget::Labels) {
        str = "Labels";
    }
    else if (i_widget == EWidget::Geometry) {
        str = "Geometry";
    }
    else if (i_widget == EWidget::LineStyle) {
        str = "LineStyle";
    }
    else if (i_widget == EWidget::FillStyle) {
        str = "FillStyle";
    }
    else if (i_widget == EWidget::GridStyle) {
        str = "GridStyle";
    }
    return str;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtGraphObjGroupProperties::CWdgtGraphObjGroupProperties(
    CDrawingScene* i_pDrawingScene,
    const QString& i_strObjName,
    bool i_bAddApplyResetButtons,
    QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    CWdgtGraphObjPropertiesAbstract(
        i_pDrawingScene, NameSpace() + "::Widgets::GraphObjs", "StandardShapes::Group",
        ClassName(), i_strObjName, i_pWdgtParent),
    m_pWdgtLabels(nullptr),
    m_pWdgtGeometry(nullptr),
    m_pWdgtLineStyle(nullptr),
    m_pWdgtFillStyle(nullptr),
    m_pWdgtGridStyle(nullptr)
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs =
            "DrawingScene: " + QString(i_pDrawingScene == nullptr ? "null" : i_pDrawingScene->objectName()) +
            ", ObjName: " + i_strObjName +
            ", AddButtons: " + bool2Str(i_bAddApplyResetButtons);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    m_pWdgtLabels = new CWdgtGraphObjLabelsProperties(
        i_pDrawingScene, NameSpace() + "::Widgets::GraphObjs",
        "StandardShapes::Line", i_strObjName);
    m_pLyt->addWidget(m_pWdgtLabels);
    QObject::connect(
        m_pWdgtLabels, &CWdgtGraphObjLabelsProperties::contentChanged,
        this, &CWdgtGraphObjGroupProperties::onWdgtLabelsContentChanged);

    m_pWdgtGeometry = new CWdgtGraphObjGroupGeometryProperties(
        i_pDrawingScene, NameSpace() + "::Widgets::GraphObjs", i_strObjName);
    m_pLyt->addWidget(m_pWdgtGeometry);
    QObject::connect(
        m_pWdgtGeometry, &CWdgtGraphObjGroupGeometryProperties::contentChanged,
        this, &CWdgtGraphObjGroupProperties::onWdgtGeometryContentChanged);

    m_pWdgtLineStyle = new CWdgtGraphObjLineStyleProperties(
        i_pDrawingScene, NameSpace() + "::Widgets::GraphObjs",
        "StandardShapes::Group", i_strObjName);
    m_pLyt->addWidget(m_pWdgtLineStyle);
    QObject::connect(
        m_pWdgtLineStyle, &CWdgtGraphObjLineStyleProperties::contentChanged,
        this, &CWdgtGraphObjGroupProperties::onWdgtLineStyleContentChanged);

    m_pWdgtFillStyle = new CWdgtGraphObjFillStyleProperties(
        i_pDrawingScene, NameSpace() + "::Widgets::GraphObjs",
        "StandardShapes::Group", i_strObjName);
    m_pLyt->addWidget(m_pWdgtFillStyle);
    QObject::connect(
        m_pWdgtFillStyle, &CWdgtGraphObjFillStyleProperties::contentChanged,
        this, &CWdgtGraphObjGroupProperties::onWdgtFillStyleContentChanged);

    m_pWdgtGridStyle = new CWdgtGraphObjGridStyleProperties(
        i_pDrawingScene, NameSpace() + "::Widgets::GraphObjs",
        "StandardShapes::Group", i_strObjName);
    m_pLyt->addWidget(m_pWdgtGridStyle);
    QObject::connect(
        m_pWdgtGridStyle, &CWdgtGraphObjFillStyleProperties::contentChanged,
        this, &CWdgtGraphObjGroupProperties::onWdgtGridStyleContentChanged);

    // <Buttons>
    //==========

    if (i_bAddApplyResetButtons) {
        m_pLyt->addSpacing(10);
        createButtonsLineWidget();
        m_pLyt->addWidget(m_pWdgtButtons);
    }
    m_pLyt->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWdgtGraphObjGroupProperties::~CWdgtGraphObjGroupProperties()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_pWdgtLabels = nullptr;
    m_pWdgtGeometry = nullptr;
    m_pWdgtLineStyle = nullptr;
    m_pWdgtFillStyle = nullptr;
    m_pWdgtGridStyle = nullptr;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjGroupProperties::expand(EWidget i_widget, bool i_bExpand)
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

    if (i_widget == EWidget::Labels) {
        m_pWdgtLabels->expand(i_bExpand);
    }
    else if (i_widget == EWidget::Geometry) {
        m_pWdgtGeometry->expand(i_bExpand);
    }
    else if (i_widget == EWidget::LineStyle) {
        m_pWdgtLineStyle->expand(i_bExpand);
    }
    else if (i_widget == EWidget::FillStyle) {
        m_pWdgtFillStyle->expand(i_bExpand);
    }
    else if (i_widget == EWidget::GridStyle) {
        m_pWdgtGridStyle->expand(i_bExpand);
    }
}

/*==============================================================================
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtGraphObjGroupProperties::setKeyInTree(const QString& i_strKeyInTree)
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
        // Fill the content of the edit controls.
        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

            CWdgtGraphObjPropertiesAbstract::setKeyInTree(i_strKeyInTree);
            m_pWdgtLabels->setKeyInTree(i_strKeyInTree);
            m_pWdgtGeometry->setKeyInTree(i_strKeyInTree);
            m_pWdgtLineStyle->setKeyInTree(i_strKeyInTree);
            m_pWdgtFillStyle->setKeyInTree(i_strKeyInTree);
            m_pWdgtGridStyle->setKeyInTree(i_strKeyInTree);
        }
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
bool CWdgtGraphObjGroupProperties::hasErrors() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hasErrors",
        /* strAddInfo   */ "" );

    bool bHasErrors = m_pWdgtLabels->hasErrors();
    if (!bHasErrors) {
        bHasErrors = m_pWdgtGeometry->hasErrors();
    }
    if (!bHasErrors) {
        bHasErrors = m_pWdgtLineStyle->hasErrors();
    }
    if (!bHasErrors) {
        bHasErrors = m_pWdgtFillStyle->hasErrors();
    }
    if (!bHasErrors) {
        bHasErrors = m_pWdgtGridStyle->hasErrors();
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHasErrors);
    }
    return bHasErrors;
}

//------------------------------------------------------------------------------
bool CWdgtGraphObjGroupProperties::hasChanges() const
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
    // here we are in the call stack of the "onGraphObjAboutToBeDestroyed" of this widget.
    // The child widgets slot may be called sometimes afterwards.

    bool bHasChanges = false;
    if (m_pGraphObj != nullptr) {
        bHasChanges = m_pWdgtLabels->hasChanges();
        if (!bHasChanges) {
            bHasChanges = m_pWdgtGeometry->hasChanges();
        }
        if (!bHasChanges) {
            bHasChanges = m_pWdgtLineStyle->hasChanges();
        }
        if (!bHasChanges) {
            bHasChanges = m_pWdgtFillStyle->hasChanges();
        }
        if (!bHasChanges) {
            bHasChanges = m_pWdgtGridStyle->hasChanges();
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHasChanges);
    }
    return bHasChanges;
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGroupProperties::acceptChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "acceptChanges",
        /* strAddInfo   */ "" );

    if (m_pGraphObj != nullptr)
    {
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
                m_pWdgtLabels->acceptChanges();
                m_pWdgtGeometry->acceptChanges();
                m_pWdgtLineStyle->acceptChanges();
                m_pWdgtFillStyle->acceptChanges();
                m_pWdgtGridStyle->acceptChanges();
            }

            // If the "contentChanged" signal is no longer blocked and the content of
            // properties widget has been changed ...
            if (m_iContentChangedSignalBlockedCounter == 0 && m_bContentChanged) {
                // .. emit the contentChanged signal and update the enabled state
                // of the Apply and Reset buttons.
                updateButtonsEnabled();
                emit_contentChanged();
            }
        }
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGroupProperties::rejectChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "rejectChanges",
        /* strAddInfo   */ "" );

    {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

        m_pWdgtLabels->rejectChanges();
        m_pWdgtGeometry->rejectChanges();
        m_pWdgtLineStyle->rejectChanges();
        m_pWdgtFillStyle->rejectChanges();
        m_pWdgtGridStyle->rejectChanges();
    }

    // If the "contentChanged" signal is no longer blocked and the content of
    // properties widget has been changed ...
    if (m_iContentChangedSignalBlockedCounter == 0 && m_bContentChanged) {
        // .. emit the contentChanged signal and update the enabled state
        // of the Apply and Reset buttons.
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjGroupProperties::onWdgtLabelsContentChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onWdgtLabelsContentChanged",
        /* strAddInfo   */ "" );

    if (m_iContentChangedSignalBlockedCounter == 0) {
        updateButtonsEnabled();
        emit_contentChanged();
    }
    else {
        m_bContentChanged = true;
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGroupProperties::onWdgtGeometryContentChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onWdgtGeometryContentChanged",
        /* strAddInfo   */ "" );

    if (m_iContentChangedSignalBlockedCounter == 0) {
        updateButtonsEnabled();
        emit_contentChanged();
    }
    else {
        m_bContentChanged = true;
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGroupProperties::onWdgtLineStyleContentChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onWdgtLineStyleContentChanged",
        /* strAddInfo   */ "" );

    if (m_iContentChangedSignalBlockedCounter == 0) {
        updateButtonsEnabled();
        emit_contentChanged();
    }
    else {
        m_bContentChanged = true;
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGroupProperties::onWdgtFillStyleContentChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onWdgtFillStyleContentChanged",
        /* strAddInfo   */ "" );

    if (m_iContentChangedSignalBlockedCounter == 0) {
        updateButtonsEnabled();
        emit_contentChanged();
    }
    else {
        m_bContentChanged = true;
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGroupProperties::onWdgtGridStyleContentChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onWdgtGridStyleContentChanged",
        /* strAddInfo   */ "" );

    if (m_iContentChangedSignalBlockedCounter == 0) {
        updateButtonsEnabled();
        emit_contentChanged();
    }
    else {
        m_bContentChanged = true;
    }
}
