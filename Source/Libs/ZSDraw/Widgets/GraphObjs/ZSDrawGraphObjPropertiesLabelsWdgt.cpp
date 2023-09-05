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

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjPropertiesLabelsWdgt.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSSysGUI/ZSSysSepLine.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysRefCountGuard.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#if QT_VERSION < 0x050000
#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#else
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
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
    m_pWdgtHeadline(nullptr),
    m_pLytWdgtHeadline(nullptr),
    m_pxmBtnDown(":/ZS/Button/ButtonArrowDown.png"),
    m_pxmBtnUp(":/ZS/Button/ButtonArrowUp.png"),
    m_pBtnCollapse(nullptr),
    m_pLblHeadlineIcon(nullptr),
    m_pLblHeadline(nullptr),
    m_pSepHeadline(nullptr),
    m_pWdgtLabels(nullptr),
    m_pLytWdgtLabels(nullptr),
    m_pLytLineName(nullptr),
    m_pLblName(nullptr),
    m_pEdtName(nullptr),
    m_pLblNameError(nullptr),
    m_pLytLineNameVisibilities(nullptr),
    m_pLblNameVisible(nullptr),
    m_pChkNameLabelVisible(nullptr),
    m_pLblNameLabelAnchorSelPt(nullptr),
    m_pCmbNameLabelAnchorSelPt(nullptr),
    m_pLblNameLabelAnchorLineVisible(nullptr),
    m_pChkNameLabelAnchorLineVisible(nullptr)
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    int cxLblWidth = 60;

    // <Widget> Headline
    //==================

    m_pWdgtHeadline = new QWidget();
    m_pLytWdgtHeadline = new QHBoxLayout();
    m_pLytWdgtHeadline->setContentsMargins(0, 0, 0, 0);
    m_pWdgtHeadline->setLayout(m_pLytWdgtHeadline);
    m_pLyt->addWidget(m_pWdgtHeadline);

    m_pBtnCollapse = new QPushButton();
    m_pBtnCollapse->setIcon(m_pxmBtnDown);
    m_pLytWdgtHeadline->addWidget(m_pBtnCollapse);

    QObject::connect(
        m_pBtnCollapse, &QPushButton::clicked,
        this, &CWdgtGraphObjPropertiesLabels::onBtnCollapseClicked);

    QPixmap pxmHeadline(":/ZS/Draw/Key16x16.png");
    m_pLblHeadlineIcon = new QLabel();
    m_pLblHeadlineIcon->setPixmap(pxmHeadline);
    m_pLytWdgtHeadline->addWidget(m_pLblHeadlineIcon);

    m_pLblHeadline = new QLabel("Labels");
    QFont fntHeadline = m_pLblHeadline->font();
    fntHeadline.setPointSize(fntHeadline.pointSize() + 2);
    m_pLblHeadline->setFont(fntHeadline);
    m_pLytWdgtHeadline->addWidget(m_pLblHeadline);

    m_pSepHeadline = new CSepLine();
    m_pLytWdgtHeadline->addWidget(m_pSepHeadline, 1);

    m_pLytWdgtHeadline->addStretch();

    // <Widget> Labels
    //================

    m_pWdgtLabels = new QWidget();
    m_pLytWdgtLabels = new QVBoxLayout();
    m_pLytWdgtLabels->setContentsMargins(0, 0, 0, 0);
    m_pWdgtLabels->setLayout(m_pLytWdgtLabels);
    m_pLyt->addWidget(m_pWdgtLabels);

    // <Line> Name
    //------------

    m_pLytLineName = new QHBoxLayout();
    m_pLytWdgtLabels->addLayout(m_pLytLineName);

    m_pLblName = new QLabel("Name:");
    m_pLblName->setFixedWidth(cxLblWidth);
    m_pLytLineName->addWidget(m_pLblName);
    m_pEdtName = new QLineEdit();
    m_pEdtName->setEnabled(false);
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

    // <Line> Name Visibilities
    //-------------------------

    m_pLytLineNameVisibilities = new QHBoxLayout();
    m_pLytWdgtLabels->addLayout(m_pLytLineNameVisibilities);

    m_pLblNameVisible = new QLabel("Show Name:");
    m_pLblNameVisible->setFixedWidth(cxLblWidth);
    m_pLytLineNameVisibilities->addWidget(m_pLblNameVisible);
    m_pChkNameLabelVisible = new QCheckBox();
    m_pChkNameLabelVisible->setEnabled(false);
    m_pLytLineNameVisibilities->addWidget(m_pChkNameLabelVisible);

    QObject::connect(
        m_pChkNameLabelVisible, &QCheckBox::stateChanged,
        this, &CWdgtGraphObjPropertiesLabels::onChkNameLabelVisibleStateChanged);

    m_pLblNameLabelAnchorSelPt = new QLabel("Anchored To:");
    m_pLytLineNameVisibilities->addWidget(m_pLblNameLabelAnchorSelPt);
    m_pCmbNameLabelAnchorSelPt = new QComboBox();
    m_pCmbNameLabelAnchorSelPt->setEnabled(false);
    m_pLytLineNameVisibilities->addWidget(m_pCmbNameLabelAnchorSelPt);

    QObject::connect(
        m_pCmbNameLabelAnchorSelPt, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &CWdgtGraphObjPropertiesLabels::onCmbNameLabelAnchorSelPtCurrentIndexChanged);

    m_pLblNameLabelAnchorLineVisible = new QLabel("Show Anchor Line:");
    m_pLytLineNameVisibilities->addWidget(m_pLblNameLabelAnchorLineVisible);
    m_pChkNameLabelAnchorLineVisible = new QCheckBox();
    m_pChkNameLabelAnchorLineVisible->setEnabled(false);
    m_pLytLineNameVisibilities->addWidget(m_pChkNameLabelAnchorLineVisible);

    QObject::connect(
        m_pChkNameLabelAnchorLineVisible, &QCheckBox::stateChanged,
        this, &CWdgtGraphObjPropertiesLabels::onChkNameLabelAnchorLineVisibleStateChanged);

    m_pWdgtLabels->hide();

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

    m_pWdgtHeadline = nullptr;
    m_pLytWdgtHeadline = nullptr;
    //m_pxmBtnDown;
    //m_pxmBtnUp;
    m_pBtnCollapse = nullptr;
    m_pLblHeadlineIcon = nullptr;
    m_pLblHeadline = nullptr;
    m_pSepHeadline = nullptr;
    m_pWdgtLabels = nullptr;
    m_pLytWdgtLabels = nullptr;
    m_pLytLineName = nullptr;
    m_pLblName = nullptr;
    m_pEdtName = nullptr;
    m_pLblNameError = nullptr;
    m_pLytLineNameVisibilities = nullptr;
    m_pLblNameVisible = nullptr;
    m_pChkNameLabelVisible = nullptr;
    m_pLblNameLabelAnchorSelPt = nullptr;
    m_pCmbNameLabelAnchorSelPt = nullptr;
    m_pLblNameLabelAnchorLineVisible = nullptr;
    m_pChkNameLabelAnchorLineVisible = nullptr;
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
        if (!bHasChanges) {
            if (m_pGraphObj->isNameLabelVisible() != (m_pChkNameLabelVisible->checkState() == Qt::Checked)) {
                bHasChanges = true;
            }
        }
        if (!bHasChanges && m_pChkNameLabelVisible->checkState() == Qt::Checked) {
            CEnumSelectionPoint eSelPtGraphObj = CEnumSelectionPoint(m_pGraphObj->nameLabelAnchorPoint());
            int idxCmb = m_pCmbNameLabelAnchorSelPt->currentIndex();
            CEnumSelectionPoint eSelPtCmb = CEnumSelectionPoint(m_pCmbNameLabelAnchorSelPt->itemData(idxCmb).toInt());
            if (eSelPtGraphObj != eSelPtCmb) {
                bHasChanges = true;
            }
            if (!bHasChanges) {
                if (m_pGraphObj->isNameLabelAnchorLineVisible() != (m_pChkNameLabelAnchorLineVisible->checkState() == Qt::Checked)) {
                    bHasChanges = true;
                }
            }
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHasChanges);
    }
    return bHasChanges;
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesLabels::onBtnCollapseClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnCollapseClicked",
        /* strAddInfo   */ "" );

    if (m_pWdgtLabels->isHidden()) {
        m_pBtnCollapse->setIcon(m_pxmBtnUp);
        m_pWdgtLabels->show();
    }
    else {
        m_pBtnCollapse->setIcon(m_pxmBtnDown);
        m_pWdgtLabels->hide();
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
    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesLabels::onChkNameLabelVisibleStateChanged(int i_iState)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qCheckState2Str(i_iState);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkNameLabelVisibleStateChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesLabels::onCmbNameLabelAnchorSelPtCurrentIndexChanged(int i_idx)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_idx);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbNameLabelAnchorSelPtCurrentIndexChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesLabels::onChkNameLabelAnchorLineVisibleStateChanged(int i_iState)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qCheckState2Str(i_iState);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkNameLabelAnchorLineVisibleStateChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        emit_contentChanged();
    }
}

/*==============================================================================
protected: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesLabels::fillEditControls()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "fillEditControls",
        /* strAddInfo   */ "" );

    if (m_pGraphObj == nullptr)
    {
        m_pEdtName->setText("");
        m_pEdtName->setEnabled(false);
        m_pLblNameError->hide();
        m_pChkNameLabelVisible->setEnabled(false);
        m_pChkNameLabelVisible->setCheckState(Qt::Unchecked);
        m_pCmbNameLabelAnchorSelPt->setEnabled(false);
        m_pCmbNameLabelAnchorSelPt->clear();
        m_pChkNameLabelAnchorLineVisible->setEnabled(false);
        m_pChkNameLabelAnchorLineVisible->setCheckState(Qt::Unchecked);
    }
    else
    {
        // Depending on the type of graphical object, fill the combo box
        // with possible anchor points.
        m_pEdtName->setText(m_pGraphObj->name());
        m_pEdtName->setEnabled(true);
        m_pChkNameLabelVisible->setEnabled(true);
        m_pChkNameLabelVisible->setCheckState(
            m_pGraphObj->isNameLabelVisible() ? Qt::Checked : Qt::Unchecked);
        fillComboNameLabelAnchorSelPt();
        m_pCmbNameLabelAnchorSelPt->setEnabled(true);
        CEnumSelectionPoint selPt = m_pGraphObj->nameLabelAnchorPoint();
        int idxCmb = m_pCmbNameLabelAnchorSelPt->findData(selPt.enumeratorAsInt());
        m_pCmbNameLabelAnchorSelPt->setCurrentIndex(idxCmb); // -1 is ok showing an empty string (None)
        m_pChkNameLabelAnchorLineVisible->setEnabled(true);
        m_pChkNameLabelAnchorLineVisible->setCheckState(
            m_pGraphObj->isNameLabelAnchorLineVisible() ? Qt::Checked : Qt::Unchecked);
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesLabels::applySettings()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "applySettings",
        /* strAddInfo   */ "" );

    if (m_pGraphObj != nullptr && !hasErrors())
    {
        m_pGraphObj->rename(m_pEdtName->text());

        if (m_pChkNameLabelVisible->checkState() == Qt::Checked) {
            int idxCmb = m_pCmbNameLabelAnchorSelPt->currentIndex();
            if (idxCmb < 0) {
                idxCmb = 0; // Set to most commonly used anchor point.
                m_pCmbNameLabelAnchorSelPt->setCurrentIndex(idxCmb);
            }
            CEnumSelectionPoint eSelPtCmb = CEnumSelectionPoint(m_pCmbNameLabelAnchorSelPt->itemData(idxCmb).toInt());
            m_pGraphObj->showNameLabel(eSelPtCmb.enumerator());

            if (m_pChkNameLabelAnchorLineVisible->checkState() == Qt::Checked) {
                m_pGraphObj->showNameLabelAnchorLine();
            }
            else {
                m_pGraphObj->hideNameLabelAnchorLine();
            }
        }
        else {
            m_pGraphObj->hideNameLabel();
            m_pGraphObj->hideNameLabelAnchorLine();
        }
    }
}

/*==============================================================================
private: // auxiliary instance methods
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

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesLabels::fillComboNameLabelAnchorSelPt()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "fillComboNameLabelAnchorSelPt",
        /* strAddInfo   */ "" );

    if (m_graphObjTypeCurr != m_graphObjTypePrev) {
        m_pCmbNameLabelAnchorSelPt->clear();
        if (m_pGraphObj != nullptr) {
            for (ESelectionPoint selPt : m_pGraphObj->getPossibleLabelAnchorPoints()) {
                if (m_pGraphObj->type() == EGraphObjTypeLine) {
                    if (selPt == ESelectionPoint::TopLeft) {
                        m_pCmbNameLabelAnchorSelPt->addItem("Line Start", static_cast<int>(selPt));
                    }
                    else if (selPt == ESelectionPoint::BottomRight) {
                        m_pCmbNameLabelAnchorSelPt->addItem("Line End", static_cast<int>(selPt));
                    }
                    else {
                        m_pCmbNameLabelAnchorSelPt->addItem(CEnumSelectionPoint(selPt).toString(), static_cast<int>(selPt));
                    }
                }
                else {
                    m_pCmbNameLabelAnchorSelPt->addItem(CEnumSelectionPoint(selPt).toString(), static_cast<int>(selPt));
                }
            }
        }
    }
}
