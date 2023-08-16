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

#include "ZSDraw/GraphObjWdgts/ZSDrawGraphObjPropertiesLabelsWdgt.h"
#include "ZSDraw/GraphObjs/ZSDrawGraphObj.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSSys/ZSSysRefCountGuard.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#if QT_VERSION < 0x050000
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#else
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;


/*******************************************************************************
class CWdgtGraphObjPropertiesLabels : public CWdgtFormatGraphObjs
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtGraphObjPropertiesLabels::CWdgtGraphObjPropertiesLabels(
    CDrawingScene* i_pDrawingScene,
    const QString& i_strParentClassName,
    const QString& i_strObjName,
    QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    CWdgtGraphObjPropertiesAbstract(i_pDrawingScene, i_strParentClassName + "::" + ClassName(), i_strObjName, i_pWdgtParent),
    m_pLblHeadLine(nullptr),
    m_pLytLineName(nullptr),
    m_pLblName(nullptr),
    m_pEdtName(nullptr),
    m_pLblNameError(nullptr),
    m_pLytLineDescription(nullptr),
    m_pLblDescription(nullptr),
    m_pEdtDescription(nullptr),
    // Trace
    m_pTrcAdminObj(nullptr)
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    int cxLblWidth = 60;

    QFont fntHeadLine;

    m_pLblHeadLine = new QLabel("Object Name");
    fntHeadLine = m_pLblHeadLine->font();
    fntHeadLine.setPointSize(fntHeadLine.pointSize()+4);
    m_pLblHeadLine->setFont(fntHeadLine);
    m_pLyt->addWidget(m_pLblHeadLine);
    m_pLyt->addSpacing(4);

    // <Line> Name
    //============

    m_pLytLineName = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytLineName);

    // <LineEdit> Name
    //----------------

    m_pLblName = new QLabel("Name:");
    m_pLblName->setFixedWidth(cxLblWidth);
    m_pLytLineName->addWidget(m_pLblName);
    m_pEdtName = new QLineEdit();
    m_pLytLineName->addWidget(m_pEdtName, 1);
    m_pLblNameError = new QLabel();
    m_pLblNameError->setToolTip("Name is not unique.");
    QPixmap pxmErr = ZS::System::GUI::getErrPixmap(EResultSeverityError, QSize(20,20));
    m_pLblNameError->setPixmap(pxmErr);
    m_pLblNameError->hide();
    m_pLytLineName->addWidget(m_pLblNameError);

    QObject::connect(
        m_pEdtName, &QLineEdit::textChanged,
        this, &CWdgtGraphObjPropertiesLabels::onEdtNameTextChanged);

    // <Line> Description
    //===================

    m_pLytLineDescription = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytLineDescription);

    // <LineEdit> Path
    //----------------

    m_pLblDescription = new QLabel("Description:");
    m_pLblDescription->setFixedWidth(cxLblWidth);
    m_pLytLineDescription->addWidget(m_pLblDescription);
    m_pEdtDescription = new QLineEdit();
    m_pLytLineDescription->addWidget(m_pEdtDescription);

    QObject::connect(
        m_pEdtDescription, &QLineEdit::textChanged,
        this, &CWdgtGraphObjPropertiesLabels::onEdtDescriptionTextChanged);

} // ctor

//------------------------------------------------------------------------------
CWdgtGraphObjPropertiesLabels::~CWdgtGraphObjPropertiesLabels()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_pLblHeadLine = nullptr;
    m_pLytLineName = nullptr;
    m_pLblName = nullptr;
    m_pEdtName = nullptr;
    m_pLblNameError = nullptr;
    m_pLytLineDescription = nullptr;
    m_pLblDescription = nullptr;
    m_pEdtDescription = nullptr;
}

/*==============================================================================
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesLabels::setKeyInTree(const QString& i_strKeyInTree)
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

    if (m_strKeyInTree != i_strKeyInTree)
    {
        CWdgtGraphObjPropertiesAbstract::setKeyInTree(i_strKeyInTree);

        if (m_pGraphObj == nullptr)
        {
            m_pEdtName->setText("");
            m_pEdtDescription->setText("");
        }
        else
        {
            m_pEdtName->setText(m_pGraphObj->name());
            m_pEdtDescription->setText(m_pGraphObj->getDescription());
        }
    }
}

/*==============================================================================
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtGraphObjPropertiesLabels::hasErrors() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hasErrors",
        /* strAddInfo   */ "" );

    bool bHasErrors = false;
    if (!changedNameIsUnique()) {
        bHasErrors = true;
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHasErrors);
    }
    return bHasErrors;
}

//------------------------------------------------------------------------------
bool CWdgtGraphObjPropertiesLabels::hasChanges() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hasChanges",
        /* strAddInfo   */ "" );

    bool bHasChanges = false;

    if (m_pGraphObj != nullptr) {
        if (m_pGraphObj->name() != m_pEdtName->text()) {
            bHasChanges = true;
        }
        else if (m_pGraphObj->getDescription() != m_pEdtDescription->text()) {
            bHasChanges = true;
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHasChanges);
    }
    return bHasChanges;
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesLabels::acceptChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "acceptChanges",
        /* strAddInfo   */ "" );

    CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);

    if (m_pGraphObj != nullptr && !hasErrors())
    {
        m_pGraphObj->rename(m_pEdtName->text());
        m_pGraphObj->setDescription(m_pEdtDescription->text());
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesLabels::rejectChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "rejectChanges",
        /* strAddInfo   */ "" );

    if (m_pGraphObj == nullptr)
    {
        m_pEdtName->setText("");
        m_pEdtDescription->setText("");
    }
    else
    {
        m_pEdtName->setText(m_pGraphObj->name());
        m_pEdtDescription->setText(m_pGraphObj->getDescription());
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesLabels::onEdtNameTextChanged(const QString& i_strText)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strText;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtNameTextChanged",
        /* strAddInfo   */ strMthInArgs );

    if (changedNameIsUnique()) {
        m_pLblNameError->hide();
    }
    else {
        m_pLblNameError->show();
    }
    emit_propertyChanged();
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesLabels::onEdtDescriptionTextChanged(const QString& i_strText)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strText;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtDescriptionTextChanged",
        /* strAddInfo   */ strMthInArgs );

    emit_propertyChanged();
}

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtGraphObjPropertiesLabels::changedNameIsUnique() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "changedNameIsUnique",
        /* strAddInfo   */ "" );

    bool bIsUnique = true;

    if (m_pGraphObj != nullptr) {
        QString strParentBranchPath = m_pGraphObj->parentBranchPath();
        QString strNewKeyInTree = m_pDrawingScene->getGraphObjsIdxTree()->buildKeyInTreeStr(
            m_pGraphObj->entryTypeSymbol(), strParentBranchPath, m_pEdtName->text());
        if (strNewKeyInTree != m_strKeyInTree) {
            bIsUnique = (m_pDrawingScene->findGraphObj(strNewKeyInTree) == nullptr);
        }
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bIsUnique);
    }
    return bIsUnique;
}

/*==============================================================================
protected: // must overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesLabels::onGraphObjChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onGraphObjChanged",
        /* strAddInfo   */ "" );
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesLabels::onGraphObjMoved()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onGraphObjMoved",
        /* strAddInfo   */ "" );
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesLabels::onGraphObjRenamed()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onGraphObjRenamed",
        /* strAddInfo   */ "" );
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesLabels::onGraphObjAboutToDestroyed()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onGraphObjDestroyed",
        /* strAddInfo   */ "" );

    m_pGraphObj = nullptr;
}
