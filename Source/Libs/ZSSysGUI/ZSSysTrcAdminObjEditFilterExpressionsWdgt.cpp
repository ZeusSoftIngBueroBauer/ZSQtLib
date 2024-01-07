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

#include "ZSSysGUI/ZSSysTrcAdminObjEditFilterExpressionsWdgt.h"
#include "ZSSysGUI/ZSSysTrcAdminObjEditFilterExpressionsModel.h"
#include "ZSSysGUI/ZSSysCheckBoxItemDelegate.h"
#include "ZSSysGUI/ZSSysComboBoxItemDelegate.h"
#include "ZSSysGUI/ZSSysSepLine.h"
#include "ZSSysGUI/ZSSysTableView.h"
#include "ZSSys/ZSSysRefCountGuard.h"
#include "ZSSys/ZSSysTrcAdminObj.h"

#if QT_VERSION < 0x050000
//#include <QtGui/qcheckbox.h>
//#include <QtGui/qcombobox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
//#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#else
//#include <QtWidgets/qcheckbox.h>
//#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
//#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CWdgtTrcAdminObjEditFilterExpressions : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtTrcAdminObjEditFilterExpressions::CWdgtTrcAdminObjEditFilterExpressions( QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pTrcAdminObj(nullptr),
    m_eFilter(EMethodTraceFilterProperty::Undefined),
    m_pLyt(nullptr),
    m_pLytLineTrcAdminObjPath(nullptr),
    m_pLblTrcAdminObjPath(nullptr),
    m_pSepLineTrcAdminObjPath(nullptr),
    m_pWdgtFilterExpressions(nullptr),
    m_pLytWdgtFilterExpressions(nullptr),
    m_pLytLineEditButtons(nullptr),
    m_pBtnResizeRowsAndColumnsToContents(nullptr),
    m_pBtnAddLabel(nullptr),
    m_pBtnRemoveLabels(nullptr),
    m_pLblFilterProperty(nullptr),
    m_pLytTableView(nullptr),
    m_pTableView(nullptr),
    m_pModel(nullptr)
{
    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    // <Widget> Headline
    //==================

    m_pLytLineTrcAdminObjPath = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytLineTrcAdminObjPath);

    m_pLblTrcAdminObjPath = new QLabel("Trace Admin Object Path");
    QFont fntHeadline = m_pLblTrcAdminObjPath->font();
    fntHeadline.setPointSize(fntHeadline.pointSize() + 2);
    m_pLblTrcAdminObjPath->setFont(fntHeadline);
    m_pLytLineTrcAdminObjPath->addWidget(m_pLblTrcAdminObjPath);
    m_pLytLineTrcAdminObjPath->addStretch();

    m_pSepLineTrcAdminObjPath = new CSepLine();
    m_pLyt->addWidget(m_pSepLineTrcAdminObjPath, 1);

    // <Widget> Filter Expressions
    //============================

    m_pWdgtFilterExpressions = new QWidget();
    m_pLytWdgtFilterExpressions = new QVBoxLayout();
    m_pLytWdgtFilterExpressions->setContentsMargins(0, 0, 0, 0);
    m_pWdgtFilterExpressions->setLayout(m_pLytWdgtFilterExpressions);
    m_pLyt->addWidget(m_pWdgtFilterExpressions);

    // <Line> Edit Buttons
    //--------------------

    m_pLytLineEditButtons = new QHBoxLayout();
    m_pLytWdgtFilterExpressions->addLayout(m_pLytLineEditButtons);

    QPixmap pxmResizeToContents(":/ZS/TreeView/TreeViewResizeToContents.png");
    m_pBtnResizeRowsAndColumnsToContents = new QPushButton();
    m_pBtnResizeRowsAndColumnsToContents->setIcon(pxmResizeToContents);
    m_pBtnResizeRowsAndColumnsToContents->setFixedSize(QSize(24, 24));
    m_pBtnResizeRowsAndColumnsToContents->setToolTip("Press to resize the rows and columns to their contents");
    m_pLytLineEditButtons->addWidget(m_pBtnResizeRowsAndColumnsToContents);
    m_pLytLineEditButtons->addSpacing(10);
    QObject::connect(
        m_pBtnResizeRowsAndColumnsToContents, &QPushButton::clicked,
        this, &CWdgtTrcAdminObjEditFilterExpressions::onBtnResizeRowsAndColumnsToContentsClicked );

    QPixmap pxmAddExpression(":/ZS/Button/ButtonAdd24x24.png");
    m_pBtnAddLabel = new QPushButton();
    m_pBtnAddLabel->setIcon(pxmAddExpression);
    m_pBtnAddLabel->setFixedSize(QSize(24, 24));
    m_pBtnAddLabel->setToolTip("Press to add a filter expression");
    m_pLytLineEditButtons->addWidget(m_pBtnAddLabel);
    m_pLytLineEditButtons->addSpacing(10);
    QObject::connect(
        m_pBtnAddLabel, &QPushButton::clicked,
        this, &CWdgtTrcAdminObjEditFilterExpressions::onBtnAddFilterExpressionClicked );

    QPixmap pxmRemoveExpression(":/ZS/Button/ButtonDelete24x24.png");
    m_pBtnRemoveLabels = new QPushButton();
    m_pBtnRemoveLabels->setIcon(pxmRemoveExpression);
    m_pBtnRemoveLabels->setFixedSize(QSize(24, 24));
    m_pBtnRemoveLabels->setToolTip("Press to remove the selected filter expression");
    m_pBtnRemoveLabels->setEnabled(false);
    m_pLytLineEditButtons->addWidget(m_pBtnRemoveLabels);
    m_pLytLineEditButtons->addSpacing(10);
    QObject::connect(
        m_pBtnRemoveLabels, &QPushButton::clicked,
        this, &CWdgtTrcAdminObjEditFilterExpressions::onBtnRemoveFilterExpressionClicked );

    m_pLblFilterProperty = new QLabel("Filter Property");
    m_pLblFilterProperty->setFont(fntHeadline);
    m_pLytLineEditButtons->addWidget(m_pLblFilterProperty);
    m_pLytLineEditButtons->addStretch();

    // <Line> Table View
    //------------------

    m_pLytTableView = new QVBoxLayout();
    m_pLytWdgtFilterExpressions->addLayout(m_pLytTableView, 1);

    m_pModel = new CModelTrcAdminObjEditFilterExpressions(this);
    m_pTableView = new CTableView("TrcAdminObjEditFilterExpression");
    m_pTableView->setModel(m_pModel);

    m_pTableView->setItemDelegateForColumn(
        CModelTrcAdminObjEditFilterExpressions::EColumnSelected,
        new CCheckBoxItemDelegate(m_pModel, m_pTableView));
    m_pTableView->addKeyAsEditTriggerForColumn(
        CModelTrcAdminObjEditFilterExpressions::EColumnSelected,
        CTableView::SEditTriggerKey(Qt::Key_Return));

    m_pTableView->setItemDelegateForColumn(
        CModelTrcAdminObjEditFilterExpressions::EColumnFilterExpressionType,
        new CComboBoxItemDelegate(m_pModel, m_pTableView));
    m_pTableView->addKeyAsEditTriggerForColumn(
        CModelTrcAdminObjEditFilterExpressions::EColumnFilterExpressionType,
        CTableView::SEditTriggerKey(Qt::Key_Return));

    m_pTableView->setEditTriggers(
        QAbstractItemView::CurrentChanged|QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked|
        QAbstractItemView::EditKeyPressed|QAbstractItemView::AnyKeyPressed);
    m_pTableView->resizeColumnsToContents();
    m_pTableView->resizeRowsToContents();
    m_pLytTableView->addWidget(m_pTableView);

    QObject::connect(
        m_pModel, &CModelTrcAdminObjEditFilterExpressions::contentChanged,
        this, &CWdgtTrcAdminObjEditFilterExpressions::onModelEditFilterExpressionsContentChanged);

} // ctor

//------------------------------------------------------------------------------
CWdgtTrcAdminObjEditFilterExpressions::~CWdgtTrcAdminObjEditFilterExpressions()
//------------------------------------------------------------------------------
{
    m_pTrcAdminObj = nullptr;
    m_eFilter = static_cast<EMethodTraceFilterProperty>(0);
    m_pLyt = nullptr;
    m_pLytLineTrcAdminObjPath = nullptr;
    m_pLblTrcAdminObjPath = nullptr;
    m_pSepLineTrcAdminObjPath = nullptr;
    m_pWdgtFilterExpressions = nullptr;
    m_pLytWdgtFilterExpressions = nullptr;
    m_pLytLineEditButtons = nullptr;
    m_pBtnResizeRowsAndColumnsToContents = nullptr;
    m_pBtnAddLabel = nullptr;
    m_pBtnRemoveLabels = nullptr;
    m_pLblFilterProperty = nullptr;
    m_pLytTableView = nullptr;
    m_pTableView = nullptr;
    m_pModel = nullptr;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTrcAdminObjEditFilterExpressions::setTraceAdminObj(CTrcAdminObj* i_pTrcAdminObj)
//------------------------------------------------------------------------------
{
    if (m_pTrcAdminObj != i_pTrcAdminObj) {
        m_pTrcAdminObj = i_pTrcAdminObj;
        setHeadlineText();
        m_pModel->setTraceAdminObj(m_pTrcAdminObj);
        m_pTableView->resizeColumnsToContents();
        m_pTableView->resizeRowsToContents();
    }
}

//------------------------------------------------------------------------------
void CWdgtTrcAdminObjEditFilterExpressions::setFilterToEdit(EMethodTraceFilterProperty i_filter)
//------------------------------------------------------------------------------
{
    if (m_eFilter != i_filter) {
        m_eFilter = i_filter;
        setHeadlineText();
        m_pModel->setFilterToEdit(i_filter);
        m_pTableView->resizeColumnsToContents();
        m_pTableView->resizeRowsToContents();
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtTrcAdminObjEditFilterExpressions::hasErrors() const
//------------------------------------------------------------------------------
{
    return m_pModel->hasErrors();
}

//------------------------------------------------------------------------------
bool CWdgtTrcAdminObjEditFilterExpressions::hasChanges() const
//------------------------------------------------------------------------------
{
    return m_pModel->hasChanges();
}

//------------------------------------------------------------------------------
void CWdgtTrcAdminObjEditFilterExpressions::acceptChanges()
//------------------------------------------------------------------------------
{
    m_pModel->acceptChanges();
}

//------------------------------------------------------------------------------
void CWdgtTrcAdminObjEditFilterExpressions::rejectChanges()
//------------------------------------------------------------------------------
{
    m_pModel->rejectChanges();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTrcAdminObjEditFilterExpressions::onBtnResizeRowsAndColumnsToContentsClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    m_pTableView->resizeColumnsToContents();
    m_pTableView->resizeRowsToContents();
}

//------------------------------------------------------------------------------
void CWdgtTrcAdminObjEditFilterExpressions::onBtnAddFilterExpressionClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    m_pModel->addFilterExpression();
}

//------------------------------------------------------------------------------
void CWdgtTrcAdminObjEditFilterExpressions::onBtnRemoveFilterExpressionClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    m_pModel->removeSelectedRows();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTrcAdminObjEditFilterExpressions::onModelEditFilterExpressionsContentChanged()
//------------------------------------------------------------------------------
{
    if (m_pModel->selectedRows().size() > 0) {
        m_pBtnRemoveLabels->setEnabled(true);
    }
    else {
        m_pBtnRemoveLabels->setEnabled(false);
    }
    emit contentChanged();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTrcAdminObjEditFilterExpressions::setHeadlineText()
//------------------------------------------------------------------------------
{
    if (m_pTrcAdminObj == nullptr) {
        m_pLblTrcAdminObjPath->setText("Trace Admin Object Path");
    }
    else {
        m_pLblTrcAdminObjPath->setText(m_pTrcAdminObj->path());
    }
    if (m_eFilter == EMethodTraceFilterProperty::Undefined) {
        m_pLblFilterProperty->setText("Filter Property");
    }
    else {
        m_pLblFilterProperty->setText("Set Filter for " + m_eFilter.toString() + "s");
    }
}
