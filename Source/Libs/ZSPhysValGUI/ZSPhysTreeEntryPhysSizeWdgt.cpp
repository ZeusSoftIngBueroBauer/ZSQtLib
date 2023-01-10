/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#include "ZSPhysValGUI/ZSPhysTreeEntryPhysSizeWdgt.h"
#include "ZSPhysValGUI/ZSPhysTreeEntryPhysSizeModel.h"
#include "ZSPhysVal/ZSPhysSizesIdxTree.h"
#include "ZSPhysVal/ZSPhysSize.h"

#if QT_VERSION < 0x050000
#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qheaderview.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qtableview.h>
#else
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qtableview.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;


/*******************************************************************************
class CWdgtPhysSize : public QTableView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtPhysSize::CWdgtPhysSize(
    CIdxTreePhysSizes* i_pIdxTree, QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtAbstractTreeEntry(i_pIdxTree, i_pWdgtParent),
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
    m_pLytViewMode(nullptr),
    m_pLblViewMode(nullptr),
    m_pCmbViewMode(nullptr),
    // Table view with physical units of a physical size:
    m_pLytTableView(nullptr),
    m_pWdgtTableView(nullptr),
    m_pModel(nullptr),
    m_pTableView(nullptr),
    m_pSpacerItemStretchAtBottom(nullptr)
{
    m_pLyt = new QVBoxLayout;
    setLayout(m_pLyt);

    // SIUnit
    //-------------

    m_pLytSIUnit = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytSIUnit);
    m_pEdtSIUnit = new QLineEdit();
    m_pEdtSIUnit->setMinimumWidth(m_cxEdtMinWidth);
    m_pEdtSIUnit->setMaximumWidth(m_cxEdtMaxWidth);
    m_pEdtSIUnit->setReadOnly(true);
    //m_pEdtSIUnit->setEnabled(false);
    m_pLblSIUnit = new QLabel();
    m_pLblSIUnit->setBuddy(m_pEdtSIUnit);
    m_pLblSIUnit->setMinimumWidth(m_cxLblMinWidth);
    m_pLblSIUnit->setText(tr("SI-Unit:"));
    m_pLytSIUnit->addWidget(m_pLblSIUnit);
    m_pLytSIUnit->addWidget(m_pEdtSIUnit);
    m_pLytSIUnit->addStretch();

    // FormulaSymbol
    //--------------

    m_pLytFormulaSymbol = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytFormulaSymbol);
    m_pEdtFormulaSymbol = new QLineEdit();
    m_pEdtFormulaSymbol->setMinimumWidth(m_cxEdtMinWidth);
    m_pEdtFormulaSymbol->setMaximumWidth(m_cxEdtMaxWidth);
    m_pEdtFormulaSymbol->setReadOnly(true);
    //m_pEdtFormulaSymbol->setEnabled(false);
    m_pLblFormulaSymbol = new QLabel();
    m_pLblFormulaSymbol->setBuddy(m_pEdtFormulaSymbol);
    m_pLblFormulaSymbol->setMinimumWidth(m_cxLblMinWidth);
    m_pLblFormulaSymbol->setText(tr("Formula Symbol:"));
    m_pLytFormulaSymbol->addWidget(m_pLblFormulaSymbol);
    m_pLytFormulaSymbol->addWidget(m_pEdtFormulaSymbol);
    m_pLytFormulaSymbol->addStretch();

    // IsPowerRelated
    //---------------

    m_pLytIsPowerRelated = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytIsPowerRelated);
    m_pChkIsPowerRelated = new QCheckBox();
    m_pChkIsPowerRelated->setChecked(false);
    //m_pChkIsPowerRelated->setReadOnly(true);
    m_pChkIsPowerRelated->setEnabled(false);
    m_pLblIsPowerRelated = new QLabel();
    m_pLblIsPowerRelated->setBuddy(m_pChkIsPowerRelated);
    m_pLblIsPowerRelated->setMinimumWidth(m_cxLblMinWidth);
    m_pLblIsPowerRelated->setText(tr("Is Power Related:"));
    m_pLytIsPowerRelated->addWidget(m_pLblIsPowerRelated);
    m_pLytIsPowerRelated->addWidget(m_pChkIsPowerRelated);
    m_pLytIsPowerRelated->addStretch();

    // View Mode
    //----------

    m_pLytViewMode = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytViewMode);
    m_pCmbViewMode = new QComboBox();
    m_pCmbViewMode->addItem(
        CModelPhysSize::viewMode2Str(
            CModelPhysSize::EViewModeNormal),
        CModelPhysSize::EViewModeNormal);
    m_pCmbViewMode->addItem(
        CModelPhysSize::viewMode2Str(
            CModelPhysSize::EViewModeContingencyTable),
        CModelPhysSize::EViewModeContingencyTable);
    m_pCmbViewMode->setMinimumWidth(m_cxEdtMinWidth);
    m_pCmbViewMode->setMaximumWidth(m_cxEdtMaxWidth);
    m_pLblViewMode = new QLabel();
    m_pLblViewMode->setBuddy(m_pCmbViewMode);
    m_pLblViewMode->setMinimumWidth(m_cxLblMinWidth);
    m_pLblViewMode->setText(tr("View Mode:"));
    m_pLytViewMode->addWidget(m_pLblViewMode);
    m_pLytViewMode->addWidget(m_pCmbViewMode);
    m_pLytViewMode->addStretch();

    if( !QObject::connect(
        /* pObjSender   */ m_pCmbViewMode,
        /* szSignal     */ SIGNAL( currentIndexChanged( const QString& ) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onCmbViewModeCurrentIndexChanged( const QString& ) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // Table view with physical units of a physical size
    //==================================================

    m_pWdgtTableView = new QWidget(this);
    m_pWdgtTableView->setMinimumWidth(m_cxLblMinWidth+m_cxEdtMinWidth);
    m_pLyt->addWidget(m_pWdgtTableView);

    m_pLytTableView = new QVBoxLayout;
    m_pLytTableView->setContentsMargins(0,0,0,0);
    m_pWdgtTableView->setLayout(m_pLytTableView);

    // Table view
    //-----------

    m_pModel = new CModelPhysSize(m_pIdxTree);

    m_pTableView = new QTableView();
    m_pLytTableView->addWidget(m_pTableView);

    m_pTableView->setModel(m_pModel);
    m_pTableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_pTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pTableView->verticalHeader()->hide();
    for( int iClm = 1; iClm < CModelIdxTreeBranchContent::EColumnCount; ++iClm ) {
        m_pTableView->hideColumn(iClm);
    }
    m_pTableView->setStyleSheet(R"(
        QHeaderView::section {
            background-color:lightGray;
            border: 1px solid darkGray;
        }
    )");

} // ctor

//------------------------------------------------------------------------------
CWdgtPhysSize::~CWdgtPhysSize()
//------------------------------------------------------------------------------
{
    if( m_pSpacerItemStretchAtBottom != nullptr )
    {
        m_pLyt->removeItem(m_pSpacerItemStretchAtBottom);

        try
        {
            delete m_pSpacerItemStretchAtBottom;
        }
        catch(...)
        {
        }
        m_pSpacerItemStretchAtBottom = nullptr;
    }

    try
    {
        delete m_pModel;
    }
    catch(...)
    {
    }

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
    m_pLytViewMode = nullptr;
    m_pLblViewMode = nullptr;
    m_pCmbViewMode = nullptr;
    // Table view with physical units of a physical size:
    m_pLytTableView = nullptr;
    m_pWdgtTableView = nullptr;
    m_pModel = nullptr;
    m_pTableView = nullptr;
    m_pSpacerItemStretchAtBottom = nullptr;

} // dtor

/*==============================================================================
public: // overridables of base class CWdgtAbstractTreeEntry
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtPhysSize::setIdxTree( CIdxTreePhysSizes* i_pIdxTree )
//------------------------------------------------------------------------------
{
    CWdgtAbstractTreeEntry::setIdxTree(i_pIdxTree);

    if( m_pModel != nullptr )
    {
        m_pModel->setIdxTree(i_pIdxTree);

        setKeyInTreeOfRootEntry("");
    }
}

//------------------------------------------------------------------------------
void CWdgtPhysSize::setKeyInTreeOfRootEntry( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    if( m_strKeyInTreeOfRootEntry != i_strKeyInTree )
    {
        m_strKeyInTreeOfRootEntry = i_strKeyInTree;

        CIdxTreeEntry* pTreeEntry = m_pIdxTree->findEntry(m_strKeyInTreeOfRootEntry);

        CPhysSize* pPhysSize = nullptr;

        if( pTreeEntry != nullptr )
        {
            if( pTreeEntry->isRoot() || pTreeEntry->isBranch() )
            {
                pPhysSize = dynamic_cast<CPhysSize*>(pTreeEntry);
            }
        }

        if( pPhysSize == nullptr )
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
            m_pLblViewMode->hide();
            m_pCmbViewMode->hide();
            m_pWdgtTableView->hide();
            m_pTableView->hide();

            if( m_pSpacerItemStretchAtBottom == nullptr )
            {
                m_pSpacerItemStretchAtBottom = new QSpacerItem( 0, 20, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding );
            }
            m_pLyt->addSpacerItem(m_pSpacerItemStretchAtBottom);
        }
        else
        {
            m_pEdtSIUnit->setText( pPhysSize->getSIUnitName() );
            m_pEdtFormulaSymbol->setText( pPhysSize->getFormulaSymbol() );
            m_pChkIsPowerRelated->setChecked( pPhysSize->isPowerRelated() );

            m_pLblSIUnit->show();
            m_pEdtSIUnit->show();
            m_pLblFormulaSymbol->show();
            m_pEdtFormulaSymbol->show();
            m_pLblIsPowerRelated->show();
            m_pChkIsPowerRelated->show();
            m_pLblViewMode->show();
            m_pCmbViewMode->show();
            m_pWdgtTableView->show();
            m_pTableView->show();

            if( m_pSpacerItemStretchAtBottom != nullptr )
            {
                m_pLyt->removeItem(m_pSpacerItemStretchAtBottom);
                delete m_pSpacerItemStretchAtBottom;
                m_pSpacerItemStretchAtBottom = nullptr;
            }
        }

        m_pModel->setKeyInTreeOfRootEntry(i_strKeyInTree);
    }

} // setKeyInTreeOfRootEntry

//------------------------------------------------------------------------------
void CWdgtPhysSize::resizeToContents()
//------------------------------------------------------------------------------
{
    m_pTableView->resizeColumnsToContents();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtPhysSize::onCmbViewModeCurrentIndexChanged( const QString& /*i_strCurrentText*/ )
//------------------------------------------------------------------------------
{
    updateTableView();
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtPhysSize::updateTableView()
//------------------------------------------------------------------------------
{
    QString strViewMode = m_pCmbViewMode->currentText();
    int     iViewMode   = CModelPhysSize::str2ViewMode(strViewMode);
    CModelPhysSize::EViewMode viewMode = CModelPhysSize::EViewModeNormal;

    if( iViewMode >= 0 && iViewMode < CModelPhysSize::EViewModeCount )
    {
        viewMode = static_cast<CModelPhysSize::EViewMode>(iViewMode);
    }
    if( viewMode != m_pModel->getViewMode() )
    {
        if( !m_pModel->setViewMode(viewMode) )
        {
            viewMode = m_pModel->getViewMode();
        }
    }
    if( viewMode == CModelPhysSize::EViewModeContingencyTable )
    {
        m_pTableView->verticalHeader()->show();

        for( int iClm = 0; iClm < m_pModel->rowCount(); iClm++ )
        {
            m_pTableView->showColumn(iClm);
        }
    }
    else
    {
        m_pTableView->verticalHeader()->hide();
    }

} // updateTableView
