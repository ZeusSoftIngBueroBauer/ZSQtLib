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

#include "ZSDraw/GraphObjWdgts/ZSDrawGraphObjPropertiesAbstractWdgt.h"
#include "ZSDraw/GraphObjs/ZSDrawGraphObj.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSSys/ZSSysRefCountGuard.h"
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
using namespace ZS::Draw;


/*******************************************************************************
class CWdgtGraphObjPropertiesAbstract : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtGraphObjPropertiesAbstract::CWdgtGraphObjPropertiesAbstract(
    CDrawingScene* i_pDrawingScene,
    const QString& i_strClassName,
    const QString& i_strObjName,
    QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pDrawingScene(i_pDrawingScene),
    m_strKeyInTree(),
    m_pGraphObj(nullptr),
    m_cxLblWidthClm1(80),
    m_cxEdtWidthClm1(160),
    m_cxLblWidthClm2(80),
    m_cxEdtWidthClm2(160),
    m_cxClmSpacing(30),
    // Blocking signals counter
    m_iValueChangedSignalsBlocked(0),
    // Edit Controls
    m_pLyt(nullptr),
    // Button Line
    m_pWdgtButtons(nullptr),
    m_pLytWdgtButtons(nullptr),
    m_pBtnApply(nullptr),
    m_pBtnReset(nullptr),
    // Trace admin object for method tracing
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::GraphObjWdgts", i_strClassName, i_strObjName);

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::ctor",
        /* strAddInfo   */ "" );

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    QObject::connect(
        /* pObjSender   */ m_pDrawingScene, &CDrawingScene::graphObjChanged,
        /* pObjReceiver */ this, &CWdgtGraphObjPropertiesAbstract::onDrawingSceneGraphObjChanged );
    QObject::connect(
        /* pObjSender   */ m_pDrawingScene, &CDrawingScene::graphObjMoved,
        /* pObjReceiver */ this, &CWdgtGraphObjPropertiesAbstract::onDrawingSceneGraphObjMoved );
    QObject::connect(
        /* pObjSender   */ m_pDrawingScene, &CDrawingScene::graphObjRenamed,
        /* pObjReceiver */ this, &CWdgtGraphObjPropertiesAbstract::onDrawingSceneGraphObjRenamed );
    QObject::connect(
        /* pObjSender   */ m_pDrawingScene, &CDrawingScene::graphObjAboutToBeDestroyed,
        /* pObjReceiver */ this, &CWdgtGraphObjPropertiesAbstract::onDrawingSceneGraphObjAboutToBeDestroyed );

} // ctor

//------------------------------------------------------------------------------
CWdgtGraphObjPropertiesAbstract::~CWdgtGraphObjPropertiesAbstract()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::dtor",
        /* strAddInfo   */ "" );

    mthTracer.onAdminObjAboutToBeReleased();
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pDrawingScene = nullptr;
    //m_strKeyInTree;
    m_pGraphObj = nullptr;
    m_cxLblWidthClm1 = 0;
    m_cxEdtWidthClm1 = 0;
    m_cxLblWidthClm2 = 0;
    m_cxEdtWidthClm2 = 0;
    m_cxClmSpacing = 0;
    // Blocking signals counter
    m_iValueChangedSignalsBlocked = 0;
    // Edit Controls
    m_pLyt = nullptr;
    // Button Line
    m_pWdgtButtons = nullptr;
    m_pLytWdgtButtons = nullptr;
    m_pBtnApply = nullptr;
    m_pBtnReset = nullptr;
    // Trace admin object for method tracing
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
protected: // ctor auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
QWidget* CWdgtGraphObjPropertiesAbstract::createButtonsLineWidget()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::createButtonsLineWidget",
        /* strAddInfo   */ "" );

    m_pWdgtButtons = new QWidget();
    m_pLytWdgtButtons = new QHBoxLayout();
    m_pWdgtButtons->setLayout(m_pLytWdgtButtons);
    m_pLytWdgtButtons->setContentsMargins(0, 0, 0, 0);

    m_pBtnApply = new QPushButton("Apply");
    m_pBtnApply->setEnabled(false);
    m_pLytWdgtButtons->addWidget(m_pBtnApply);
    QObject::connect(
        m_pBtnApply, &QPushButton::clicked,
        this, &CWdgtGraphObjPropertiesAbstract::onBtnApplyClicked);

    m_pBtnReset = new QPushButton("Reset");
    m_pBtnReset->setEnabled(false);
    m_pLytWdgtButtons->addWidget(m_pBtnReset);
    QObject::connect(
        m_pBtnReset, &QPushButton::clicked,
        this, &CWdgtGraphObjPropertiesAbstract::onBtnResetClicked);

    m_pLytWdgtButtons->addStretch();

    return m_pWdgtButtons;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesAbstract::setKeyInTree( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strKeyInTree;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::setKeyInTree",
        /* strAddInfo   */ strMthInArgs );

    if (m_strKeyInTree != i_strKeyInTree)
    {
        m_strKeyInTree = i_strKeyInTree;

        if (m_strKeyInTree.isEmpty()) {
            m_pGraphObj = nullptr;
        }
        else {
            m_pGraphObj = m_pDrawingScene->findGraphObj(i_strKeyInTree);
        }
    }
}

//------------------------------------------------------------------------------
QString CWdgtGraphObjPropertiesAbstract::getKeyInTree() const
//------------------------------------------------------------------------------
{
    return m_strKeyInTree;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtGraphObjPropertiesAbstract::hasErrors() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::hasErrors",
        /* strAddInfo   */ "" );

    bool bHasErrors = false;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHasErrors);
    }
    return bHasErrors;
}

//------------------------------------------------------------------------------
bool CWdgtGraphObjPropertiesAbstract::hasChanges() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::hasChanges",
        /* strAddInfo   */ "" );

    bool bHasChanges = false;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHasChanges);
    }
    return bHasChanges;
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesAbstract::acceptChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::acceptChanges",
        /* strAddInfo   */ "" );

    updateButtonsEnabled();
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesAbstract::rejectChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::rejectChanges",
        /* strAddInfo   */ "" );

    updateButtonsEnabled();
}

/*==============================================================================
protected slots: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesAbstract::onBtnApplyClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::onBtnApplyClicked",
        /* strAddInfo   */ "" );

    acceptChanges();
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesAbstract::onBtnResetClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::onBtnResetClicked",
        /* strAddInfo   */ "" );

    rejectChanges();
}

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesAbstract::updateButtonsEnabled()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::updateButtonsEnabled",
        /* strAddInfo   */ "" );

    if (m_pBtnApply != nullptr || m_pBtnReset != nullptr)
    {
        bool bEnabled = hasChanges();
        if (m_pBtnApply != nullptr) {
            if (hasErrors()) {
                m_pBtnApply->setEnabled(false);
            }
            else {
                m_pBtnApply->setEnabled(bEnabled);
            }
        }
        if (m_pBtnReset != nullptr) {
            m_pBtnReset->setEnabled(bEnabled);
        }
    }
}

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesAbstract::onGraphObjChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::onGraphObjChanged",
        /* strAddInfo   */ "" );

    if (m_iValueChangedSignalsBlocked == 0) {
        updateButtonsEnabled();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesAbstract::onGraphObjMoved()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::onGraphObjMoved",
        /* strAddInfo   */ "" );

    if (m_iValueChangedSignalsBlocked == 0) {
        updateButtonsEnabled();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesAbstract::onGraphObjRenamed()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::onGraphObjRenamed",
        /* strAddInfo   */ "" );

    if (m_iValueChangedSignalsBlocked == 0) {
        updateButtonsEnabled();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesAbstract::onGraphObjAboutToDestroyed()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::onGraphObjAboutToDestroyed",
        /* strAddInfo   */ "" );

    m_pGraphObj = nullptr;

    onGraphObjChanged();
}

/*==============================================================================
private slots:
==============================================================================*/

//------------------------------------------------------------------------------
/*! Slot connected to the graphObjChanged signal of the drawing scene.

    The signal is emitted for any changed object of the drawing scene.
    The slot checks whether the changed object is the object shown by the widget
    and invokes the pure virtual method onGraphObjChanged.
    For all other objects the slot will be ignored.

    @param i_strKeyInTree [in]
        Unique key of the changed object.
*/
void CWdgtGraphObjPropertiesAbstract::onDrawingSceneGraphObjChanged(const QString& i_strKeyInTree)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strKeyInTree;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::onDrawingSceneGraphObjChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_strKeyInTree == i_strKeyInTree)
    {
        onGraphObjChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesAbstract::onDrawingSceneGraphObjMoved(
    const QString& i_strNewKeyInTree,
    const QString& i_strOrigKeyInTree,
    const QString& i_strKeyInTreeOfTargetBranch)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "NewKey: " + i_strNewKeyInTree +
            ", OrigKey: " + i_strOrigKeyInTree +
            ", KeyInTreeOfTargetBranch: " + i_strKeyInTreeOfTargetBranch;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::onDrawingSceneGraphObjMoved",
        /* strAddInfo   */ strMthInArgs );

    if (m_strKeyInTree == i_strOrigKeyInTree)
    {
        onGraphObjMoved();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesAbstract::onDrawingSceneGraphObjRenamed(
    const QString& i_strNewKeyInTree,
    const QString& i_strOrigKeyInTree,
    const QString& i_strOrigName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "NewKey: " + i_strNewKeyInTree +
            ", OrigKey: " + i_strOrigKeyInTree +
            ", OrigName: " + i_strOrigName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::onDrawingSceneGraphObjRenamed",
        /* strAddInfo   */ strMthInArgs );

    if (m_strKeyInTree == i_strOrigKeyInTree)
    {
        onGraphObjRenamed();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesAbstract::onDrawingSceneGraphObjAboutToBeDestroyed(const QString& i_strKeyInTree)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strKeyInTree;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::onDrawingSceneGraphObjAboutToBeDestroyed",
        /* strAddInfo   */ strMthInArgs );

    if (m_strKeyInTree == i_strKeyInTree)
    {
        onGraphObjAboutToDestroyed();
    }
}

/*==============================================================================
protected: // instance methods (tracing emitting signals)
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesAbstract::emit_propertyChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::emit_propertyChanged",
        /* strAddInfo   */ "" );

    emit propertyChanged();
}
