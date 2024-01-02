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

#include "ZSSysGUI/ZSSysEditFilterExpressionsWdgt.h"
//#include "ZSSysGUI/ZSSysComboBoxItemDelegate.h"
//#include "ZSSysGUI/ZSSysComboBoxItemDelegate.h"
//#include "ZSSysGUI/ZSSysTableView.h"


#if QT_VERSION < 0x050000
//#include <QtGui/qcheckbox.h>
//#include <QtGui/qcombobox.h>
//#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
//#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
//#include <QtGui/qtableview.h>
#else
//#include <QtWidgets/qcheckbox.h>
//#include <QtWidgets/qcombobox.h>
//#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
//#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
//#include <QtWidgets/qtableview.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CWdgtEditFilterExpressions : public CWdgtGraphObjPropertiesAbstract
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtEditFilterExpressions::CWdgtEditFilterExpressions( QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pLyt(nullptr),
    m_pWdgtFilterExpressions(nullptr),
    m_pLytWdgtFilterExpressions(nullptr),
    m_pLytLineEditButtons(nullptr),
    m_pBtnResizeRowsAndColumnsToContents(nullptr),
    m_pBtnAddLabel(nullptr),
    m_pBtnRemoveLabels(nullptr),
    m_pLytTableView(nullptr),
    m_pTableView(nullptr)
{
    int cxLblWidth = 60;

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

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
        this, &CWdgtEditFilterExpressions::onBtnResizeRowsAndColumnsToContentsClicked );

    QPixmap pxmAddExpression(":/ZS/Button/ButtonAdd24x24.png");
    m_pBtnAddLabel = new QPushButton();
    m_pBtnAddLabel->setIcon(pxmAddExpression);
    m_pBtnAddLabel->setFixedSize(QSize(24, 24));
    m_pBtnAddLabel->setToolTip("Press to add a filter expression");
    m_pLytLineEditButtons->addWidget(m_pBtnAddLabel);
    m_pLytLineEditButtons->addSpacing(10);
    QObject::connect(
        m_pBtnAddLabel, &QPushButton::clicked,
        this, &CWdgtEditFilterExpressions::onBtnAddFilterExpressionClicked );

    QPixmap pxmRemoveExpression(":/ZS/Button/ButtonDelete24x24.png");
    m_pBtnRemoveLabels = new QPushButton();
    m_pBtnRemoveLabels->setIcon(pxmRemoveExpression);
    m_pBtnRemoveLabels->setFixedSize(QSize(24, 24));
    m_pBtnRemoveLabels->setToolTip("Press to remove the selected filter expression");
    m_pBtnRemoveLabels->setEnabled(false);
    m_pLytLineEditButtons->addWidget(m_pBtnRemoveLabels);
    m_pLytLineEditButtons->addStretch();
    QObject::connect(
        m_pBtnRemoveLabels, &QPushButton::clicked,
        this, &CWdgtEditFilterExpressions::onBtnRemoveFilterExpressionClicked );

    // <Line> Table View
    //------------------

    m_pLytTableView = new QVBoxLayout();
    m_pLytWdgtFilterExpressions->addLayout(m_pLytTableView);

    //m_pTableView = new CTableView(i_strObjName);
    //m_pTableView->setModel(m_pModel);

    //m_pTableView->setItemDelegateForColumn(
    //    CModelGraphObjLabels::EColumnSelected, new CCheckBoxItemDelegate(m_pModel, m_pTableView));
    //m_pTableView->addKeyAsEditTriggerForColumn(
    //    CModelGraphObjLabels::EColumnSelected, CTableView::SEditTriggerKey(Qt::Key_Return));

    //m_pTableView->addKeyAsEditTriggerForColumn(
    //    CModelGraphObjLabels::EColumnText, CTableView::SEditTriggerKey(Qt::Key_Return));

    //m_pTableView->setItemDelegateForColumn(
    //    CModelGraphObjLabels::EColumnShow, new CCheckBoxItemDelegate(m_pModel, m_pTableView));
    //m_pTableView->addKeyAsEditTriggerForColumn(
    //    CModelGraphObjLabels::EColumnShow, CTableView::SEditTriggerKey(Qt::Key_Return));

    //m_pTableView->setItemDelegateForColumn(
    //    CModelGraphObjLabels::EColumnAnchor, new CComboBoxItemDelegate(m_pModel, m_pTableView));
    //m_pTableView->addKeyAsEditTriggerForColumn(
    //    CModelGraphObjLabels::EColumnAnchor, CTableView::SEditTriggerKey(Qt::Key_Return));

    //m_pTableView->setItemDelegateForColumn(
    //    CModelGraphObjLabels::EColumnShowAnchorLine, new CCheckBoxItemDelegate(m_pModel, m_pTableView));
    //m_pTableView->addKeyAsEditTriggerForColumn(
    //    CModelGraphObjLabels::EColumnShowAnchorLine, CTableView::SEditTriggerKey(Qt::Key_Return));

    //m_pTableView->setEditTriggers(
    //    QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked|QAbstractItemView::EditKeyPressed);
    //m_pTableView->resizeColumnsToContents();
    //m_pTableView->resizeRowsToContents();
    //m_pLytTableView->addWidget(m_pTableView);

    //QObject::connect(
    //    m_pModel, &CModelGraphObjLabels::contentChanged,
    //    this, &CWdgtEditFilterExpressions::onModelLabelsContentChanged);

} // ctor

//------------------------------------------------------------------------------
CWdgtEditFilterExpressions::~CWdgtEditFilterExpressions()
//------------------------------------------------------------------------------
{
    m_pLyt = nullptr;
    m_pWdgtFilterExpressions = nullptr;
    m_pLytWdgtFilterExpressions = nullptr;
    m_pLytLineEditButtons = nullptr;
    m_pBtnResizeRowsAndColumnsToContents = nullptr;
    m_pBtnAddLabel = nullptr;
    m_pBtnRemoveLabels = nullptr;
    m_pLytTableView = nullptr;
    m_pTableView = nullptr;
}

/*==============================================================================
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtEditFilterExpressions::hasErrors() const
//------------------------------------------------------------------------------
{
    bool bHasErrors = false;
    return bHasErrors;
}

//------------------------------------------------------------------------------
bool CWdgtEditFilterExpressions::hasChanges() const
//------------------------------------------------------------------------------
{
    bool bHasChanges = false;
    return bHasChanges;
}

//------------------------------------------------------------------------------
void CWdgtEditFilterExpressions::acceptChanges()
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
void CWdgtEditFilterExpressions::rejectChanges()
//------------------------------------------------------------------------------
{
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtEditFilterExpressions::onBtnResizeRowsAndColumnsToContentsClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    //m_pTableView->resizeColumnsToContents();
    //m_pTableView->resizeRowsToContents();
}

//------------------------------------------------------------------------------
void CWdgtEditFilterExpressions::onBtnAddFilterExpressionClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    //m_pModel->addLabel();
}

//------------------------------------------------------------------------------
void CWdgtEditFilterExpressions::onBtnRemoveFilterExpressionClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    //m_pModel->removeSelectedLabels();
}
