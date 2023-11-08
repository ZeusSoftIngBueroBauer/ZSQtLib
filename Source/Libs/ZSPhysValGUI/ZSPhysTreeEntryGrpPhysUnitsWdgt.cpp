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

#include "ZSPhysValGUI/ZSPhysTreeEntryGrpPhysUnitsWdgt.h"
#include "ZSPhysValGUI/ZSPhysTreeEntryGrpPhysUnitsModel.h"
#include "ZSPhysValGUI/ZSPhysUnitFctConvertRefValsModel.h"
#include "ZSPhysVal/ZSPhysTreeEntryGrpPhysUnits.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"

#if QT_VERSION < 0x050000
#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qheaderview.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qtableview.h>
#else
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qtableview.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;


/*******************************************************************************
class CWdgtPhysUnitsGrp : public QTableView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtPhysUnitsGrp::CWdgtPhysUnitsGrp(QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    CWdgtAbstractTreeEntry(i_pWdgtParent),
    m_szBtns(24, 24),
    m_pLyt(nullptr),
    // Content (configuration) of the physical unit:
    m_pLytSIUnit(nullptr),
    m_pLblSIUnit(nullptr),
    m_pEdtSIUnit(nullptr),
    m_pLytFormulaSymbol(nullptr),
    m_pLblFormulaSymbol(nullptr),
    m_pEdtFormulaSymbol(nullptr),
    m_pLytIsPowerRelated(nullptr),
    m_pLblIsPowerRelated(nullptr),
    m_pChkIsPowerRelated(nullptr),
    // Table views
    m_pSplTableViews(nullptr),
    // Table view with reference values
    m_pWdgtTableViewRefVals(nullptr),
    m_pLytTableViewRefVals(nullptr),
    m_pLytHeadlineRefVals(nullptr),
    m_pLblHeadlineRefVals(nullptr),
    m_pBtnTableViewRefValsResizeToContents(nullptr),
    m_pModelRefVals(nullptr),
    m_pTableViewRefVals(nullptr),
    // Table view with the units of the units group
    m_pWdgtTableViewUnitsGrp(nullptr),
    m_pLytTableViewUnitsGrp(nullptr),
    m_pLytHeadlineUnitsGrp(nullptr),
    m_pLblHeadlineUnitsGrp(nullptr),
    m_pLblViewModeUnitsGrp(nullptr),
    m_pCmbViewModeUnitsGrp(nullptr),
    m_pBtnTableViewUnitsGrpResizeToContents(nullptr),
    m_pModelUnitsGrp(nullptr),
    m_pTableViewUnitsGrp(nullptr)
{
    m_pLyt = new QVBoxLayout;
    setLayout(m_pLyt);

    // SIUnit
    //-------------

    m_pLytSIUnit = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytSIUnit);
    m_pEdtSIUnit = new QLineEdit();
    m_pEdtSIUnit->setFixedWidth(m_cxEdtWidth);
    m_pEdtSIUnit->setReadOnly(true);
    //m_pEdtSIUnit->setEnabled(false);
    m_pLblSIUnit = new QLabel();
    m_pLblSIUnit->setBuddy(m_pEdtSIUnit);
    m_pLblSIUnit->setFixedWidth(m_cxLblWidth);
    m_pLblSIUnit->setText(tr("SI-Unit:"));
    m_pLytSIUnit->addWidget(m_pLblSIUnit);
    m_pLytSIUnit->addWidget(m_pEdtSIUnit);
    m_pLytSIUnit->addStretch();

    // FormulaSymbol
    //--------------

    m_pLytFormulaSymbol = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytFormulaSymbol);
    m_pEdtFormulaSymbol = new QLineEdit();
    m_pEdtFormulaSymbol->setFixedWidth(m_cxEdtWidth);
    m_pEdtFormulaSymbol->setReadOnly(true);
    //m_pEdtFormulaSymbol->setEnabled(false);
    m_pLblFormulaSymbol = new QLabel();
    m_pLblFormulaSymbol->setBuddy(m_pEdtFormulaSymbol);
    m_pLblFormulaSymbol->setFixedWidth(m_cxLblWidth);
    m_pLblFormulaSymbol->setText(tr("Formula Symbol:"));
    m_pLytFormulaSymbol->addWidget(m_pLblFormulaSymbol);
    m_pLytFormulaSymbol->addWidget(m_pEdtFormulaSymbol);
    m_pLytFormulaSymbol->addStretch();

    // IsPowerRelated
    //---------------

    m_pLytIsPowerRelated = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytIsPowerRelated);
    m_pChkIsPowerRelated = new QCheckBox();
    m_pChkIsPowerRelated->setChecked(false);
    //m_pChkIsPowerRelated->setReadOnly(true);
    m_pChkIsPowerRelated->setEnabled(false);
    m_pLblIsPowerRelated = new QLabel();
    m_pLblIsPowerRelated->setBuddy(m_pChkIsPowerRelated);
    m_pLblIsPowerRelated->setFixedWidth(m_cxLblWidth);
    m_pLblIsPowerRelated->setText(tr("Is Power Related:"));
    m_pLytIsPowerRelated->addWidget(m_pLblIsPowerRelated);
    m_pLytIsPowerRelated->addWidget(m_pChkIsPowerRelated);
    m_pLytIsPowerRelated->addStretch();

    // Splitter with Table views for reference values and units
    //---------------------------------------------------------

    m_pSplTableViews = new QSplitter();
    m_pSplTableViews->setOrientation(Qt::Vertical);
    m_pSplTableViews->setChildrenCollapsible(false);
    m_pLyt->addWidget(m_pSplTableViews);

    // Table view with reference values
    //---------------------------------

    m_pWdgtTableViewRefVals = new QWidget();
    m_pWdgtTableViewRefVals->setMinimumWidth(m_cxLblWidth+m_cxEdtWidth);
    m_pSplTableViews->addWidget(m_pWdgtTableViewRefVals);

    m_pLytTableViewRefVals = new QVBoxLayout();
    m_pLytTableViewRefVals->setContentsMargins(0,4,0,4);
    m_pWdgtTableViewRefVals->setLayout(m_pLytTableViewRefVals);

    // Headline with Label and Resize Buttons
    //---------------------------------------

    m_pLytHeadlineRefVals = new QHBoxLayout();
    m_pLytTableViewRefVals->addLayout(m_pLytHeadlineRefVals);

    m_pLblHeadlineRefVals = new QLabel("Reference Values:");
    m_pLblHeadlineRefVals->setFixedWidth(m_cxLblWidth);
    m_pLytHeadlineRefVals->addWidget(m_pLblHeadlineRefVals);

    m_pLytHeadlineRefVals->addStretch();

    QPixmap pxmResizeToContents(":/ZS/TreeView/TreeViewResizeToContents.png");

    m_pBtnTableViewRefValsResizeToContents = new QPushButton();
    m_pBtnTableViewRefValsResizeToContents->setIcon(pxmResizeToContents);
    m_pBtnTableViewRefValsResizeToContents->setFixedSize(m_szBtns);
    m_pBtnTableViewRefValsResizeToContents->setToolTip("Press to resize the columns to their contents");
    m_pLytHeadlineRefVals->addWidget(m_pBtnTableViewRefValsResizeToContents);

    QObject::connect(
        m_pBtnTableViewRefValsResizeToContents, &QPushButton::clicked,
        this, &CWdgtPhysUnitsGrp::onBtnTableViewRefValsResizeToContentsClicked );

    // Table View
    //-----------

    m_pTableViewRefVals = new QTableView();
    m_pLytTableViewRefVals->addWidget(m_pTableViewRefVals);

    m_pModelRefVals = new CModelUnitFctConvertRefVals();
    m_pTableViewRefVals->setModel(m_pModelRefVals);
    m_pTableViewRefVals->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_pTableViewRefVals->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pTableViewRefVals->verticalHeader()->hide();
    m_pTableViewRefVals->setStyleSheet(R"(
        QHeaderView::section {
            background-color:lightGray;
            border: 1px solid darkGray;
        }
    )");

    // Table view with the units of the units group
    //---------------------------------------------

    m_pWdgtTableViewUnitsGrp = new QWidget();
    m_pWdgtTableViewUnitsGrp->setMinimumWidth(m_cxLblWidth+m_cxEdtWidth);
    m_pSplTableViews->addWidget(m_pWdgtTableViewUnitsGrp);

    m_pLytTableViewUnitsGrp = new QVBoxLayout();
    m_pLytTableViewUnitsGrp->setContentsMargins(0,4,0,4);
    m_pWdgtTableViewUnitsGrp->setLayout(m_pLytTableViewUnitsGrp);

    // Headline with Label, View Mode and Resize Buttons
    //---------------------------------------------------

    m_pLytHeadlineUnitsGrp = new QHBoxLayout();
    m_pLytTableViewUnitsGrp->addLayout(m_pLytHeadlineUnitsGrp);

    QWidget* pWdgtLblsHeadlineUnitsGrp = new QWidget();
    pWdgtLblsHeadlineUnitsGrp->setFixedWidth(m_cxLblWidth);
    QHBoxLayout* pLytLblsHeadlineUnitsGrp = new QHBoxLayout();
    pWdgtLblsHeadlineUnitsGrp->setLayout(pLytLblsHeadlineUnitsGrp);
    m_pLytHeadlineUnitsGrp->addWidget(pWdgtLblsHeadlineUnitsGrp);

    m_pLblHeadlineUnitsGrp = new QLabel("Units:");
    m_pLblHeadlineUnitsGrp->setFixedWidth(m_cxLblWidth/2);
    pLytLblsHeadlineUnitsGrp->addWidget(m_pLblHeadlineUnitsGrp);

    m_pLblViewModeUnitsGrp = new QLabel("View Mode:");
    m_pLblViewModeUnitsGrp->setFixedWidth(m_cxLblWidth/2);
    pLytLblsHeadlineUnitsGrp->addWidget(m_pLblViewModeUnitsGrp);

    m_pCmbViewModeUnitsGrp = new QComboBox();
    m_pCmbViewModeUnitsGrp->addItem(
        CModelPhysUnitsGrp::viewMode2Str(
            CModelPhysUnitsGrp::EViewModeNormal),
        CModelPhysUnitsGrp::EViewModeNormal);
    m_pCmbViewModeUnitsGrp->addItem(
        CModelPhysUnitsGrp::viewMode2Str(
            CModelPhysUnitsGrp::EViewModeContingencyTable),
        CModelPhysUnitsGrp::EViewModeContingencyTable);
    m_pCmbViewModeUnitsGrp->setFixedWidth(m_cxEdtWidth);
    m_pLytHeadlineUnitsGrp->addWidget(m_pCmbViewModeUnitsGrp);

    QObject::connect(
        m_pCmbViewModeUnitsGrp, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &CWdgtPhysUnitsGrp::onCmbViewModeCurrentIndexChanged);

    m_pLytHeadlineUnitsGrp->addStretch();

    m_pBtnTableViewUnitsGrpResizeToContents = new QPushButton();
    m_pBtnTableViewUnitsGrpResizeToContents->setIcon(pxmResizeToContents);
    m_pBtnTableViewUnitsGrpResizeToContents->setFixedSize(m_szBtns);
    m_pBtnTableViewUnitsGrpResizeToContents->setToolTip("Press to resize the columns to their contents");
    m_pLytHeadlineUnitsGrp->addWidget(m_pBtnTableViewUnitsGrpResizeToContents);

    QObject::connect(
        m_pBtnTableViewUnitsGrpResizeToContents, &QPushButton::clicked,
        this, &CWdgtPhysUnitsGrp::onBtnTableViewUnitsGrpResizeToContentsClicked );

    // Table view
    //-----------

    m_pModelUnitsGrp = new CModelPhysUnitsGrp();

    m_pTableViewUnitsGrp = new QTableView();
    m_pLytTableViewUnitsGrp->addWidget(m_pTableViewUnitsGrp);

    m_pTableViewUnitsGrp->setModel(m_pModelUnitsGrp);
    m_pTableViewUnitsGrp->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_pTableViewUnitsGrp->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pTableViewUnitsGrp->verticalHeader()->hide();
    for( int iClm = 1; iClm < CModelIdxTreeBranchContent::EColumnCount; ++iClm ) {
        m_pTableViewUnitsGrp->hideColumn(iClm);
    }
    m_pTableViewUnitsGrp->setStyleSheet(R"(
        QHeaderView::section {
            background-color:lightGray;
            border: 1px solid darkGray;
        }
    )");

} // ctor

//------------------------------------------------------------------------------
CWdgtPhysUnitsGrp::~CWdgtPhysUnitsGrp()
//------------------------------------------------------------------------------
{
    try
    {
        delete m_pModelRefVals;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pModelUnitsGrp;
    }
    catch(...)
    {
    }

    m_szBtns = QSize(0, 0);
    m_pLyt = nullptr;
    // Content (configuration) of the physical unit:
    m_pLytSIUnit = nullptr;
    m_pLblSIUnit = nullptr;
    m_pEdtSIUnit = nullptr;
    m_pLytFormulaSymbol = nullptr;
    m_pLblFormulaSymbol = nullptr;
    m_pEdtFormulaSymbol = nullptr;
    m_pLytIsPowerRelated = nullptr;
    m_pLblIsPowerRelated = nullptr;
    m_pChkIsPowerRelated = nullptr;
    // Table views
    m_pSplTableViews = nullptr;
    // Table view with reference values
    m_pWdgtTableViewRefVals = nullptr;
    m_pLytTableViewRefVals = nullptr;
    m_pLytHeadlineRefVals = nullptr;
    m_pLblHeadlineRefVals = nullptr;
    m_pBtnTableViewRefValsResizeToContents = nullptr;
    m_pModelRefVals = nullptr;
    m_pTableViewRefVals = nullptr;
    // Table view with the units of the units group
    m_pWdgtTableViewUnitsGrp = nullptr;
    m_pLytTableViewUnitsGrp = nullptr;
    m_pLytHeadlineUnitsGrp = nullptr;
    m_pLblHeadlineUnitsGrp = nullptr;
    m_pLblViewModeUnitsGrp = nullptr;
    m_pCmbViewModeUnitsGrp = nullptr;
    m_pBtnTableViewUnitsGrpResizeToContents = nullptr;
    m_pModelUnitsGrp = nullptr;
    m_pTableViewUnitsGrp = nullptr;

} // dtor

/*==============================================================================
public: // overridables of base class CWdgtAbstractTreeEntry
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtPhysUnitsGrp::setKeyInTreeOfRootEntry( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    if( m_strKeyInTreeOfRootEntry != i_strKeyInTree )
    {
        m_strKeyInTreeOfRootEntry = i_strKeyInTree;

        CIdxTreeEntry* pTreeEntry = m_pIdxTree->findEntry(m_strKeyInTreeOfRootEntry);

        CUnitsTreeEntryGrpPhysUnits* pPhysUnitsGrp = nullptr;

        if( pTreeEntry != nullptr )
        {
            if( pTreeEntry->isRoot() || pTreeEntry->isBranch() )
            {
                pPhysUnitsGrp = dynamic_cast<CUnitsTreeEntryGrpPhysUnits*>(pTreeEntry);
            }
        }

        if( pPhysUnitsGrp == nullptr )
        {
            m_pEdtSIUnit->setText("---");
            m_pEdtFormulaSymbol->setText("---");
            m_pChkIsPowerRelated->setChecked(false);

            m_pLblSIUnit->hide();
            m_pEdtSIUnit->hide();
            m_pLblFormulaSymbol->hide();
            m_pEdtFormulaSymbol->hide();
            m_pLblIsPowerRelated->hide();
            m_pChkIsPowerRelated->hide();
            m_pLblViewModeUnitsGrp->hide();
            m_pCmbViewModeUnitsGrp->hide();
            m_pWdgtTableViewUnitsGrp->hide();
            m_pTableViewUnitsGrp->hide();
        }
        else
        {
            m_pEdtSIUnit->setText( pPhysUnitsGrp->getSIUnitName() );
            m_pEdtFormulaSymbol->setText( pPhysUnitsGrp->getFormulaSymbol() );
            m_pChkIsPowerRelated->setChecked( pPhysUnitsGrp->isPowerRelated() );

            m_pLblSIUnit->show();
            m_pEdtSIUnit->show();
            m_pLblFormulaSymbol->show();
            m_pEdtFormulaSymbol->show();
            m_pLblIsPowerRelated->show();
            m_pChkIsPowerRelated->show();
            m_pLblViewModeUnitsGrp->show();
            m_pCmbViewModeUnitsGrp->show();
            m_pWdgtTableViewUnitsGrp->show();
            m_pTableViewUnitsGrp->show();
        }

        m_pModelRefVals->setKeyInTreeOfRootEntry(i_strKeyInTree);
        m_pModelUnitsGrp->setKeyInTreeOfRootEntry(i_strKeyInTree);
    }

} // setKeyInTreeOfRootEntry

//------------------------------------------------------------------------------
void CWdgtPhysUnitsGrp::resizeToContents()
//------------------------------------------------------------------------------
{
    m_pTableViewUnitsGrp->resizeColumnsToContents();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtPhysUnitsGrp::onBtnTableViewRefValsResizeToContentsClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( m_pTableViewRefVals != nullptr )
    {
        m_pTableViewRefVals->resizeColumnsToContents();
    }
}

//------------------------------------------------------------------------------
void CWdgtPhysUnitsGrp::onBtnTableViewUnitsGrpResizeToContentsClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( m_pTableViewUnitsGrp != nullptr )
    {
        m_pTableViewUnitsGrp->resizeColumnsToContents();
    }
}

//------------------------------------------------------------------------------
void CWdgtPhysUnitsGrp::onCmbViewModeCurrentIndexChanged( int /*i_idx*/ )
//------------------------------------------------------------------------------
{
    updateTableView();
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtPhysUnitsGrp::updateTableView()
//------------------------------------------------------------------------------
{
    QString strViewMode = m_pCmbViewModeUnitsGrp->currentText();
    int     iViewMode   = CModelPhysUnitsGrp::str2ViewMode(strViewMode);
    CModelPhysUnitsGrp::EViewMode viewMode = CModelPhysUnitsGrp::EViewModeNormal;

    if( iViewMode >= 0 && iViewMode < CModelPhysUnitsGrp::EViewModeCount )
    {
        viewMode = static_cast<CModelPhysUnitsGrp::EViewMode>(iViewMode);
    }

    if( viewMode != m_pModelUnitsGrp->getViewMode() )
    {
        m_pModelUnitsGrp->setViewMode(viewMode);
    }
    if( viewMode == CModelPhysUnitsGrp::EViewModeContingencyTable )
    {
        m_pTableViewUnitsGrp->verticalHeader()->show();
        for( int iClm = 0; iClm < m_pModelUnitsGrp->rowCount(); iClm++ ) {
            m_pTableViewUnitsGrp->showColumn(iClm);
        }
    }
    else
    {
        m_pTableViewUnitsGrp->verticalHeader()->hide();
        for( int iClm = 1; iClm < CModelIdxTreeBranchContent::EColumnCount; ++iClm ) {
            m_pTableViewUnitsGrp->hideColumn(iClm);
        }
    }

} // updateTableView
