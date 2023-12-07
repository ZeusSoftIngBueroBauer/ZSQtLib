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

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjLabelsPropertiesWdgt.h"
#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjLabelsModel.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSSysGUI/ZSSysCheckBoxItemDelegate.h"
#include "ZSSysGUI/ZSSysComboBoxItemDelegate.h"
#include "ZSSysGUI/ZSSysSepLine.h"
#include "ZSSysGUI/ZSSysTableView.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysRefCountGuard.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtGui/qstandarditemmodel.h>

#if QT_VERSION < 0x050000
#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qtableview.h>
#else
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qtableview.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Draw;


/*******************************************************************************
class CWdgtGraphObjLabelsProperties : public CWdgtGraphObjPropertiesAbstract
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtGraphObjLabelsProperties::CWdgtGraphObjLabelsProperties(
    CDrawingScene* i_pDrawingScene,
    const QString& i_strNameSpace,
    const QString& i_strGraphObjType,
    const QString& i_strObjName,
    QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    CWdgtGraphObjPropertiesAbstract(
        i_pDrawingScene,
        i_strNameSpace, i_strGraphObjType,
        ClassName(), i_strObjName, i_pWdgtParent),
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
    m_pLytLineEditButtons(nullptr),
    m_pBtnResizeRowsAndColumnsToContents(nullptr),
    m_pBtnAddLabel(nullptr),
    m_pBtnRemoveLabels(nullptr),
    m_pLytListView(nullptr),
    m_pTableView(nullptr),
    m_pModel(nullptr)
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
    if (s_bWdgtLabelsVisible) {
        m_pBtnCollapse->setIcon(m_pxmBtnUp);
    }
    else {
        m_pBtnCollapse->setIcon(m_pxmBtnDown);
    }
    m_pLytWdgtHeadline->addWidget(m_pBtnCollapse);

    QObject::connect(
        m_pBtnCollapse, &QPushButton::clicked,
        this, &CWdgtGraphObjLabelsProperties::onBtnCollapseClicked);

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

    // <Line> Edit Buttons
    //--------------------

    m_pLytLineEditButtons = new QHBoxLayout();
    m_pLytWdgtLabels->addLayout(m_pLytLineEditButtons);

    QPixmap pxmResizeToContents(":/ZS/TreeView/TreeViewResizeToContents.png");
    m_pBtnResizeRowsAndColumnsToContents = new QPushButton();
    m_pBtnResizeRowsAndColumnsToContents->setIcon(pxmResizeToContents);
    m_pBtnResizeRowsAndColumnsToContents->setFixedSize(QSize(24, 24));
    m_pBtnResizeRowsAndColumnsToContents->setToolTip("Press to resize the rows and columns to their contents");
    m_pLytLineEditButtons->addWidget(m_pBtnResizeRowsAndColumnsToContents);
    m_pLytLineEditButtons->addSpacing(10);
    QObject::connect(
        m_pBtnResizeRowsAndColumnsToContents, &QPushButton::clicked,
        this, &CWdgtGraphObjLabelsProperties::onBtnResizeRowsAndColumnsToContentsClicked );

    QPixmap pxmAddLabel(":/ZS/Button/ButtonAdd24x24.png");
    m_pBtnAddLabel = new QPushButton();
    m_pBtnAddLabel->setIcon(pxmAddLabel);
    m_pBtnAddLabel->setFixedSize(QSize(24, 24));
    m_pBtnAddLabel->setToolTip("Press to add a user defined label");
    m_pLytLineEditButtons->addWidget(m_pBtnAddLabel);
    m_pLytLineEditButtons->addSpacing(10);
    QObject::connect(
        m_pBtnAddLabel, &QPushButton::clicked,
        this, &CWdgtGraphObjLabelsProperties::onBtnAddLabelClicked );

    QPixmap pxmRemoveLabel(":/ZS/Button/ButtonDelete24x24.png");
    m_pBtnRemoveLabels = new QPushButton();
    m_pBtnRemoveLabels->setIcon(pxmRemoveLabel);
    m_pBtnRemoveLabels->setFixedSize(QSize(24, 24));
    m_pBtnRemoveLabels->setToolTip("Press to remove the selected user defined labels");
    m_pBtnRemoveLabels->setEnabled(false);
    m_pLytLineEditButtons->addWidget(m_pBtnRemoveLabels);
    m_pLytLineEditButtons->addStretch();
    QObject::connect(
        m_pBtnRemoveLabels, &QPushButton::clicked,
        this, &CWdgtGraphObjLabelsProperties::onBtnRemoveLabelClicked );

    // <Line> Table View
    //------------------

    m_pLytListView = new QVBoxLayout();
    m_pLytWdgtLabels->addLayout(m_pLytListView);

    m_pModel = new CModelGraphObjLabels(
        m_pDrawingScene, i_strNameSpace, i_strGraphObjType, i_strObjName, this);
    m_pTableView = new CTableView(i_strObjName);
    m_pTableView->setModel(m_pModel);

    m_pTableView->setItemDelegateForColumn(
        CModelGraphObjLabels::EColumnSelected, new CCheckBoxItemDelegate(m_pModel, m_pTableView));
    m_pTableView->addKeyAsEditTriggerForColumn(
        CModelGraphObjLabels::EColumnSelected, CTableView::SEditTriggerKey(Qt::Key_Return));

    m_pTableView->addKeyAsEditTriggerForColumn(
        CModelGraphObjLabels::EColumnText, CTableView::SEditTriggerKey(Qt::Key_Return));

    m_pTableView->setItemDelegateForColumn(
        CModelGraphObjLabels::EColumnShow, new CCheckBoxItemDelegate(m_pModel, m_pTableView));
    m_pTableView->addKeyAsEditTriggerForColumn(
        CModelGraphObjLabels::EColumnShow, CTableView::SEditTriggerKey(Qt::Key_Return));

    m_pTableView->setItemDelegateForColumn(
        CModelGraphObjLabels::EColumnAnchor, new CComboBoxItemDelegate(m_pModel, m_pTableView));
    m_pTableView->addKeyAsEditTriggerForColumn(
        CModelGraphObjLabels::EColumnAnchor, CTableView::SEditTriggerKey(Qt::Key_Return));

    m_pTableView->setItemDelegateForColumn(
        CModelGraphObjLabels::EColumnShowAnchorLine, new CCheckBoxItemDelegate(m_pModel, m_pTableView));
    m_pTableView->addKeyAsEditTriggerForColumn(
        CModelGraphObjLabels::EColumnShowAnchorLine, CTableView::SEditTriggerKey(Qt::Key_Return));

    m_pTableView->setEditTriggers(
        QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked|QAbstractItemView::EditKeyPressed);
    m_pTableView->resizeColumnsToContents();
    m_pTableView->resizeRowsToContents();
    m_pLytListView->addWidget(m_pTableView);

    QObject::connect(
        m_pModel, &CModelGraphObjLabels::contentChanged,
        this, &CWdgtGraphObjLabelsProperties::onModelLabelsContentChanged);

    // Restore visibility
    //-------------------

    if (!s_bWdgtLabelsVisible) {
        m_pWdgtLabels->hide();
    }

} // ctor

//------------------------------------------------------------------------------
CWdgtGraphObjLabelsProperties::~CWdgtGraphObjLabelsProperties()
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
    m_pLytLineEditButtons = nullptr;
    m_pBtnResizeRowsAndColumnsToContents = nullptr;
    m_pBtnAddLabel = nullptr;
    m_pBtnRemoveLabels = nullptr;
    m_pLytListView = nullptr;
    m_pTableView = nullptr;
    m_pModel = nullptr;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjLabelsProperties::expand(bool i_bExpand)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bExpand);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "expand",
        /* strAddInfo   */ strMthInArgs );

    if (i_bExpand && m_pWdgtLabels->isHidden()) {
        m_pBtnCollapse->setIcon(m_pxmBtnUp);
        m_pWdgtLabels->show();
        s_bWdgtLabelsVisible = true;
    }
    else if (!i_bExpand && !m_pWdgtLabels->isHidden()) {
        m_pBtnCollapse->setIcon(m_pxmBtnDown);
        m_pWdgtLabels->hide();
        s_bWdgtLabelsVisible = false;
    }
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn("Expanded: " + bool2Str(s_bWdgtLabelsVisible));
    }
}

/*==============================================================================
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtGraphObjLabelsProperties::setKeyInTree(const QString& i_strKeyInTree)
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
            m_pModel->setKeyInTree(i_strKeyInTree);
            m_pTableView->resizeColumnsToContents();
            m_pTableView->resizeRowsToContents();
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
bool CWdgtGraphObjLabelsProperties::hasErrors() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hasErrors",
        /* strAddInfo   */ "" );
    bool bHasErrors = m_pModel->hasErrors();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHasErrors);
    }
    return bHasErrors;
}

//------------------------------------------------------------------------------
bool CWdgtGraphObjLabelsProperties::hasChanges() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hasChanges",
        /* strAddInfo   */ "" );
    bool bHasChanges = m_pModel->hasChanges();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHasChanges);
    }
    return bHasChanges;
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLabelsProperties::acceptChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "acceptChanges",
        /* strAddInfo   */ "" );

    m_pModel->acceptChanges();
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLabelsProperties::rejectChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "rejectChanges",
        /* strAddInfo   */ "" );

    m_pModel->rejectChanges();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjLabelsProperties::onBtnCollapseClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnCollapseClicked",
        /* strAddInfo   */ "" );

    if (m_pWdgtLabels->isHidden()) {
        expand(true);
    }
    else {
        expand(false);
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLabelsProperties::onBtnResizeRowsAndColumnsToContentsClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnResizeRowsAndColumnsToContentsClicked",
        /* strAddInfo   */ "" );

    m_pTableView->resizeColumnsToContents();
    m_pTableView->resizeRowsToContents();
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLabelsProperties::onBtnAddLabelClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnAddLabelClicked",
        /* strAddInfo   */ "" );

    m_pModel->addLabel();
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLabelsProperties::onBtnRemoveLabelClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnRemoveLabelClicked",
        /* strAddInfo   */ "" );

    m_pModel->removeSelectedLabels();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjLabelsProperties::onModelLabelsContentChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onModelLabelsContentChanged",
        /* strAddInfo   */ "" );

    if (m_pModel->selectedLabelNames().size() > 0) {
        m_pBtnRemoveLabels->setEnabled(true);
    }
    else {
        m_pBtnRemoveLabels->setEnabled(false);
    }
    emit_contentChanged();
}
