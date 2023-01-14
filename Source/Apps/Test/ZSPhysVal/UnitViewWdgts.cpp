/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer, Germany
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

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qheaderview.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qtableview.h>
#else
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qtableview.h>
#endif

#include "UnitViewWdgts.h"

#include "ZSPhysVal/ZSPhysTreeEntryGrpPhysUnits.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysIdxTree.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::Apps::Test::PhysVal;


/*******************************************************************************
class CWdgtUnitView : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtUnitView::CWdgtUnitView( QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pNode(nullptr),
    m_cxLblMinWidth(140),
    m_cxEdtMinWidth(160),
    m_cxEdtMaxWidth(240),
    m_pLyt(nullptr),
    m_pLytPath(nullptr),
    m_pLblPath(nullptr),
    m_pEdtPath(nullptr),
    m_pLytName(nullptr),
    m_pLblName(nullptr),
    m_pEdtName(nullptr),
    m_pLytKey(nullptr),
    m_pLblKey(nullptr),
    m_pEdtKey(nullptr)
{
    QFrame* pLine;

    m_pLyt = new QVBoxLayout;
    setLayout(m_pLyt);

    // Headline (path)
    //------------------

    m_pLytPath = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytPath);
    m_pEdtPath = new QLineEdit();
    m_pEdtPath->setMinimumWidth(m_cxEdtMinWidth);
    m_pEdtPath->setMaximumWidth(m_cxEdtMaxWidth);
    m_pEdtPath->setEnabled(false);
    m_pLblPath = new QLabel();
    m_pLblPath->setBuddy(m_pEdtPath);
    m_pLblPath->setMinimumWidth(m_cxLblMinWidth);
    m_pLblPath->setText(tr("Path:"));
    m_pLytPath->addWidget(m_pLblPath);
    m_pLytPath->addWidget(m_pEdtPath);
    m_pLytPath->addStretch();

    // <Separator>
    //-------------

    m_pLyt->addSpacing(5);

    pLine = new QFrame();
    pLine->setFrameShape(QFrame::HLine);
    pLine->setFrameShadow(QFrame::Sunken);
    m_pLyt->addWidget(pLine);

    m_pLyt->addSpacing(5);

    // Name
    //-------------

    m_pLytName = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytName);
    m_pEdtName = new QLineEdit();
    m_pEdtName->setMinimumWidth(m_cxEdtMinWidth);
    m_pEdtName->setMaximumWidth(m_cxEdtMaxWidth);
    m_pEdtName->setEnabled(false);
    m_pLblName = new QLabel();
    m_pLblName->setBuddy(m_pEdtName);
    m_pLblName->setMinimumWidth(m_cxLblMinWidth);
    m_pLblName->setText(tr("Name:"));
    m_pLytName->addWidget(m_pLblName);
    m_pLytName->addWidget(m_pEdtName);
    m_pLytName->addStretch();

    m_pLblName->hide();
    m_pEdtName->hide();

    // Key
    //-------------

    m_pLytKey = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytKey);
    m_pEdtKey = new QLineEdit();
    m_pEdtKey->setMinimumWidth(m_cxEdtMinWidth);
    m_pEdtKey->setMaximumWidth(m_cxEdtMaxWidth);
    m_pEdtKey->setEnabled(false);
    m_pLblKey = new QLabel();
    m_pLblKey->setBuddy(m_pEdtKey);
    m_pLblKey->setMinimumWidth(m_cxLblMinWidth);
    m_pLblKey->setText(tr("Key:"));
    m_pLytKey->addWidget(m_pLblKey);
    m_pLytKey->addWidget(m_pEdtKey);
    m_pLytKey->addStretch();

    m_pLblKey->hide();
    m_pEdtKey->hide();

} // ctor

//------------------------------------------------------------------------------
CWdgtUnitView::~CWdgtUnitView()
//------------------------------------------------------------------------------
{
    m_pNode = nullptr;
    m_pLyt = nullptr;
    m_pLytPath = nullptr;
    m_pLblPath = nullptr;
    m_pEdtPath = nullptr;
    m_pLytName = nullptr;
    m_pLblName = nullptr;
    m_pEdtName = nullptr;
    m_pLytKey = nullptr;
    m_pLblKey = nullptr;
    m_pEdtKey = nullptr;

} // dtor

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtUnitView::setUnitsModelNode( SUnitsModelNode* i_pNode )
//------------------------------------------------------------------------------
{
    m_pNode = i_pNode;

    if( m_pNode == nullptr )
    {
        m_pEdtPath->setText("---");
        m_pEdtName->setText("---");
        m_pEdtKey->setText("---");
    }
    else if( m_pNode->m_pUnit != nullptr )
    {
        m_pEdtPath->setText( m_pNode->m_pUnit->parentBranchKeyInTree() );
        m_pEdtName->setText( m_pNode->m_pUnit->name() );
        m_pEdtKey->setText( m_pNode->m_pUnit->keyInTree() );
    }
    else if( m_pNode->m_pUnitGrp != nullptr )
    {
        m_pEdtPath->setText( m_pNode->m_pUnitGrp->keyInTree() );
        m_pEdtName->setText( m_pNode->m_pUnitGrp->name() );
        m_pEdtKey->setText( m_pNode->m_pUnitGrp->keyInTree() );
    }
    else
    {
        m_pEdtPath->setText("---");
        m_pEdtName->setText("---");
        m_pEdtKey->setText("---");
    }

} // setUnitsModelNode


/*******************************************************************************
class CWdgtUnitViewUndefined : public CWdgtUnitView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtUnitViewUndefined::CWdgtUnitViewUndefined( QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtUnitView(i_pWdgtParent),
    m_pLblPxmZeusSoft(nullptr)
{
    QPixmap pxmZeusSoft(":/ZS/System/Zeus32x32.bmp");
    m_pLblPxmZeusSoft = new QLabel();
    m_pLblPxmZeusSoft->setPixmap(pxmZeusSoft);
    m_pLblPxmZeusSoft->setScaledContents(true);
    m_pLyt->addWidget(m_pLblPxmZeusSoft);

} // ctor

//------------------------------------------------------------------------------
CWdgtUnitViewUndefined::~CWdgtUnitViewUndefined()
//------------------------------------------------------------------------------
{
    m_pLblPxmZeusSoft = nullptr;

} // dtor

/*==============================================================================
public: // overridables of base class CWdgtUnitView
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtUnitViewUndefined::setUnitsModelNode( SUnitsModelNode* i_pNode )
//------------------------------------------------------------------------------
{
    CWdgtUnitView::setUnitsModelNode(i_pNode);
}


/*******************************************************************************
class CWdgtUnitViewGrp : public CWdgtUnitView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtUnitViewGrp::CWdgtUnitViewGrp( QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtUnitView(i_pWdgtParent)
{
    // Name
    //-------------

    m_pLblName->show();
    m_pEdtName->show();

    // Key
    //-------------

    m_pLblKey->show();
    m_pEdtKey->show();

} // ctor

//------------------------------------------------------------------------------
CWdgtUnitViewGrp::~CWdgtUnitViewGrp()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // overridables of base class CWdgtUnitView
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtUnitViewGrp::setUnitsModelNode( SUnitsModelNode* i_pNode )
//------------------------------------------------------------------------------
{
    CWdgtUnitView::setUnitsModelNode(i_pNode);
}


/*******************************************************************************
class CWdgtUnitViewUnt : public CWdgtUnitView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtUnitViewUnt::CWdgtUnitViewUnt( QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtUnitView(i_pWdgtParent),
    m_pLytSymbol(nullptr),
    m_pLblSymbol(nullptr),
    m_pEdtSymbol(nullptr),
    m_pLytNextLower(nullptr),
    m_pLblNextLower(nullptr),
    m_pEdtNextLower(nullptr),
    m_pLytNextHigher(nullptr),
    m_pLblNextHigher(nullptr),
    m_pEdtNextHigher(nullptr)
{
    // Name
    //-------------

    m_pLblName->show();
    m_pEdtName->show();

    // Key
    //-------------

    m_pLblKey->show();
    m_pEdtKey->show();

    // Symbol
    //---------------

    m_pLytSymbol = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytSymbol);
    m_pEdtSymbol = new QLineEdit();
    m_pEdtSymbol->setMinimumWidth(m_cxEdtMinWidth);
    m_pEdtSymbol->setMaximumWidth(m_cxEdtMaxWidth);
    m_pEdtSymbol->setEnabled(false);
    m_pLblSymbol = new QLabel();
    m_pLblSymbol->setBuddy(m_pEdtSymbol);
    m_pLblSymbol->setMinimumWidth(m_cxLblMinWidth);
    m_pLblSymbol->setText(tr("Symbol:"));
    m_pLytSymbol->addWidget(m_pLblSymbol);
    m_pLytSymbol->addWidget(m_pEdtSymbol);
    m_pLytSymbol->addStretch();

    // NextLower
    //-----------

    m_pLytNextLower = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytNextLower);
    m_pEdtNextLower = new QLineEdit();
    m_pEdtNextLower->setMinimumWidth(m_cxEdtMinWidth);
    m_pEdtNextLower->setMaximumWidth(m_cxEdtMaxWidth);
    m_pEdtNextLower->setEnabled(false);
    m_pLblNextLower = new QLabel();
    m_pLblNextLower->setBuddy(m_pEdtNextLower);
    m_pLblNextLower->setMinimumWidth(m_cxLblMinWidth);
    m_pLblNextLower->setText(tr("NextLower:"));
    m_pLytNextLower->addWidget(m_pLblNextLower);
    m_pLytNextLower->addWidget(m_pEdtNextLower);
    m_pLytNextLower->addStretch();

    // NextHigher
    //-----------

    m_pLytNextHigher = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytNextHigher);
    m_pEdtNextHigher = new QLineEdit();
    m_pEdtNextHigher->setMinimumWidth(m_cxEdtMinWidth);
    m_pEdtNextHigher->setMaximumWidth(m_cxEdtMaxWidth);
    m_pEdtNextHigher->setEnabled(false);
    m_pLblNextHigher = new QLabel();
    m_pLblNextHigher->setBuddy(m_pEdtNextHigher);
    m_pLblNextHigher->setMinimumWidth(m_cxLblMinWidth);
    m_pLblNextHigher->setText(tr("NextHigher:"));
    m_pLytNextHigher->addWidget(m_pLblNextHigher);
    m_pLytNextHigher->addWidget(m_pEdtNextHigher);
    m_pLytNextHigher->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWdgtUnitViewUnt::~CWdgtUnitViewUnt()
//------------------------------------------------------------------------------
{
    m_pLytSymbol = nullptr;
    m_pLblSymbol = nullptr;
    m_pEdtSymbol = nullptr;
    m_pLytNextLower = nullptr;
    m_pLblNextLower = nullptr;
    m_pEdtNextLower = nullptr;
    m_pLytNextHigher = nullptr;
    m_pLblNextHigher = nullptr;
    m_pEdtNextHigher = nullptr;

} // dtor

/*==============================================================================
public: // overridables of base class CWdgtUnitView
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtUnitViewUnt::setUnitsModelNode( SUnitsModelNode* i_pNode )
//------------------------------------------------------------------------------
{
    CWdgtUnitView::setUnitsModelNode(i_pNode);

    if( m_pNode == nullptr )
    {
        m_pEdtSymbol->setText("---");
        m_pEdtNextLower->setText("---");
        m_pEdtNextHigher->setText("---");
    }
    else if( m_pNode->m_pUnit != nullptr )
    {
        m_pEdtSymbol->setText( m_pNode->m_pUnit->symbol() );

        if( m_pNode->m_pUnit->nextLowerUnit() == nullptr) 
        {
            m_pEdtNextLower->setText("---");
        }
        else
        {
            m_pEdtNextLower->setText( m_pNode->m_pUnit->nextLowerUnit()->name() );
        }
        if( m_pNode->m_pUnit->nextHigherUnit() == nullptr) 
        {
            m_pEdtNextHigher->setText("---");
        }
        else
        {
            m_pEdtNextHigher->setText( m_pNode->m_pUnit->nextHigherUnit()->name() );
        }
    }
    else
    {
        m_pEdtSymbol->setText("---");
        m_pEdtNextLower->setText("---");
        m_pEdtNextHigher->setText("---");
    }

} // setUnitsModelNode


/*******************************************************************************
class CWdgtUnitViewGrpRatios : public CWdgtUnitViewGrp
*******************************************************************************/

#if 0

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtUnitViewGrpRatios::CWdgtUnitViewGrpRatios( QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtUnitViewGrp(i_pWdgtParent),
    // Table view with physical units of a physical size:
    m_pLytTableView(nullptr),
    m_pWdgtTableView(nullptr),
    m_model("Ratio-Units",this),
    m_pTableView(nullptr)
{
    // Table view with physical units of a physical size
    //==================================================

    m_pWdgtTableView = new QWidget(this);
    //m_pWdgtTableView->setMinimumWidth(cxLblMinWidth+cxEdtMinWidth);
    m_pLyt->addWidget(m_pWdgtTableView);

    m_pLytTableView = new QVBoxLayout;
    m_pLytTableView->setContentsMargins(0,0,0,0);
    m_pWdgtTableView->setLayout(m_pLytTableView);

    // Table view
    //-----------

    m_pTableView = new QTableView();
    m_pLytTableView->addWidget(m_pTableView);

    m_pTableView->setModel(&m_model);
    m_pTableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_pTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pTableView->verticalHeader()->hide();
    //m_pTableView->verticalHeader()->setMinimumSectionSize(1);
    //m_pTableView->setShowGrid(false);
    //m_pTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //m_pTableView->showColumn(CModelPhysUnit::EColumnPhysSizeNameSpace);

    m_pTableView->resizeColumnsToContents();
    m_pTableView->resizeRowsToContents();

} // ctor

//------------------------------------------------------------------------------
CWdgtUnitViewGrpRatios::~CWdgtUnitViewGrpRatios()
//------------------------------------------------------------------------------
{
    // Table view with physical units of a physical size:
    m_pLytTableView = nullptr;
    m_pWdgtTableView = nullptr;
    m_pTableView = nullptr;

} // dtor

/*==============================================================================
public: // overridables of base class CWdgtUnitView
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtUnitViewGrpRatios::setUnitsModelNode( SUnitsModelNode* i_pNode )
//------------------------------------------------------------------------------
{
    CWdgtUnitViewGrp::setUnitsModelNode(i_pNode);

    if( m_pNode == nullptr )
    {
        m_model.setUnitGroup(nullptr);
    }
    else if( m_pNode->m_pUnitGrp != nullptr )
    {
        m_model.setUnitGroup(m_pNode->m_pUnitGrp);
    }
    else
    {
        m_model.setUnitGroup(nullptr);
    }

    m_pTableView->resizeColumnsToContents();
    m_pTableView->resizeRowsToContents();

} // setUnitsModelNode

#endif

/*******************************************************************************
class CWdgtUnitViewGrpDataQuantities : public CWdgtUnitViewGrp
*******************************************************************************/

#if 0

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtUnitViewGrpDataQuantities::CWdgtUnitViewGrpDataQuantities( QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtUnitViewGrp(i_pWdgtParent),
    // Table view with physical units of a physical size:
    m_pLytTableView(nullptr),
    m_pWdgtTableView(nullptr),
    m_model("DataQuantity-Units",this),
    m_pTableView(nullptr)
{
    // Table view with physical units of a physical size
    //==================================================

    m_pWdgtTableView = new QWidget(this);
    //m_pWdgtTableView->setMinimumWidth(cxLblMinWidth+cxEdtMinWidth);
    m_pLyt->addWidget(m_pWdgtTableView);

    m_pLytTableView = new QVBoxLayout;
    m_pLytTableView->setContentsMargins(0,0,0,0);
    m_pWdgtTableView->setLayout(m_pLytTableView);

    // Table view
    //-----------

    m_pTableView = new QTableView();
    m_pLytTableView->addWidget(m_pTableView);

    m_pTableView->setModel(&m_model);
    m_pTableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_pTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pTableView->verticalHeader()->hide();
    //m_pTableView->verticalHeader()->setMinimumSectionSize(1);
    //m_pTableView->setShowGrid(false);
    //m_pTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //m_pTableView->showColumn(CModelPhysUnit::EColumnPhysSizeNameSpace);

    m_pTableView->resizeColumnsToContents();
    m_pTableView->resizeRowsToContents();

} // ctor

//------------------------------------------------------------------------------
CWdgtUnitViewGrpDataQuantities::~CWdgtUnitViewGrpDataQuantities()
//------------------------------------------------------------------------------
{
    // Table view with physical units of a physical size:
    m_pLytTableView = nullptr;
    m_pWdgtTableView = nullptr;
    m_pTableView = nullptr;

} // dtor

/*==============================================================================
public: // overridables of base class CWdgtUnitView
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtUnitViewGrpDataQuantities::setUnitsModelNode( SUnitsModelNode* i_pNode )
//------------------------------------------------------------------------------
{
    CWdgtUnitViewGrp::setUnitsModelNode(i_pNode);

    if( m_pNode == nullptr )
    {
        m_model.setUnitGroup(nullptr);
    }
    else if( m_pNode->m_pUnitGrp != nullptr )
    {
        m_model.setUnitGroup(m_pNode->m_pUnitGrp);
    }
    else
    {
        m_model.setUnitGroup(nullptr);
    }

    m_pTableView->resizeColumnsToContents();
    m_pTableView->resizeRowsToContents();

} // setUnitsModelNode

#endif

/*******************************************************************************
class CWdgtUnitViewGrpUserDefinedQuantities : public CWdgtUnitViewGrp
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtUnitViewGrpUserDefinedQuantities::CWdgtUnitViewGrpUserDefinedQuantities( QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtUnitViewGrp(i_pWdgtParent)
{
} // ctor

//------------------------------------------------------------------------------
CWdgtUnitViewGrpUserDefinedQuantities::~CWdgtUnitViewGrpUserDefinedQuantities()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // overridables of base class CWdgtUnitView
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtUnitViewGrpUserDefinedQuantities::setUnitsModelNode( SUnitsModelNode* i_pNode )
//------------------------------------------------------------------------------
{
    CWdgtUnitViewGrp::setUnitsModelNode(i_pNode);

} // setUnitsModelNode


/*******************************************************************************
class CWdgtUnitViewGrpPhysScienceFields : public CWdgtUnitViewGrp
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtUnitViewGrpPhysScienceFields::CWdgtUnitViewGrpPhysScienceFields( QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtUnitViewGrp(i_pWdgtParent),
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
    m_model("PhysSize",this),
    m_pTableView(nullptr),
    m_pSpacerItemStretchAtBottom(nullptr)
{
    // SIUnit
    //-------------

    m_pLytSIUnit = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytSIUnit);
    m_pEdtSIUnit = new QLineEdit();
    m_pEdtSIUnit->setMinimumWidth(m_cxEdtMinWidth);
    m_pEdtSIUnit->setMaximumWidth(m_cxEdtMaxWidth);
    m_pEdtSIUnit->setEnabled(false);
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
    m_pEdtFormulaSymbol->setEnabled(false);
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
    m_pCmbViewMode->addItem(CModelPhysSize::viewMode2Str(CModelPhysSize::EViewModeNormal),CModelPhysSize::EViewModeNormal);
    m_pCmbViewMode->addItem(CModelPhysSize::viewMode2Str(CModelPhysSize::EViewModeContingencyTable),CModelPhysSize::EViewModeContingencyTable);
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
        /* szSlot       */ SLOT( viewModeChanged( const QString& ) ) ) )
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

    m_pTableView = new QTableView();
    m_pLytTableView->addWidget(m_pTableView);

    m_pTableView->setModel(&m_model);
    m_pTableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_pTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pTableView->verticalHeader()->hide();
    //m_pTableView->verticalHeader()->setMinimumSectionSize(1);
    //m_pTableView->setShowGrid(false);
    //m_pTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //m_pTableView->showColumn(CModelPhysSize::EColumnPhysSizeNameSpace);

    m_pTableView->resizeColumnsToContents();
    m_pTableView->resizeRowsToContents();

} // ctor

//------------------------------------------------------------------------------
CWdgtUnitViewGrpPhysScienceFields::~CWdgtUnitViewGrpPhysScienceFields()
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
    m_pTableView = nullptr;
    m_pSpacerItemStretchAtBottom = nullptr;

} // dtor

/*==============================================================================
public: // overridables of base class CWdgtUnitView
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtUnitViewGrpPhysScienceFields::setUnitsModelNode( SUnitsModelNode* i_pNode )
//------------------------------------------------------------------------------
{
    CWdgtUnitViewGrp::setUnitsModelNode(i_pNode);

    CUnitsTreeEntryGrpPhysUnits* pPhysSize = nullptr;

    if( m_pNode == nullptr )
    {
        m_model.setUnitGroup(nullptr);
    }
    else if( m_pNode->m_pUnitGrp != nullptr )
    {
        pPhysSize = dynamic_cast<CUnitsTreeEntryGrpPhysUnits*>(m_pNode->m_pUnitGrp);
        m_model.setUnitGroup(m_pNode->m_pUnitGrp);
    }
    else
    {
        m_model.setUnitGroup(nullptr);
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

    updateTableView();

} // setUnitsModelNode

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtUnitViewGrpPhysScienceFields::viewModeChanged( const QString& /*i_strCurrentText*/ )
//------------------------------------------------------------------------------
{
    updateTableView();
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtUnitViewGrpPhysScienceFields::updateTableView()
//------------------------------------------------------------------------------
{
    QString strViewMode = m_pCmbViewMode->currentText();
    int     iViewMode   = CModelPhysSize::str2ViewMode(strViewMode);
    CModelPhysSize::EViewMode viewMode = CModelPhysSize::EViewModeNormal;

    if( iViewMode >= 0 && iViewMode < CModelPhysSize::EViewModeCount )
    {
        viewMode = static_cast<CModelPhysSize::EViewMode>(iViewMode);
    }
    if( viewMode != m_model.getViewMode() )
    {
        if( !m_model.setViewMode(viewMode) )
        {
            viewMode = m_model.getViewMode();
        }
    }
    if( viewMode == CModelPhysSize::EViewModeContingencyTable )
    {
        m_pTableView->verticalHeader()->show();

        for( int iCol = 0; iCol < static_cast<int>(m_model.getUnitCount()); iCol++ )
        {
            m_pTableView->showColumn(iCol);
        }
    }
    else
    {
        m_pTableView->verticalHeader()->hide();
    }

    m_pTableView->resizeColumnsToContents();
    m_pTableView->resizeRowsToContents();

} // updateTableView


/*******************************************************************************
class CWdgtUnitViewUntRatio : public CWdgtUnitViewUnt
*******************************************************************************/

#if 0

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtUnitViewUntRatio::CWdgtUnitViewUntRatio( QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtUnitViewUnt(i_pWdgtParent),
    m_pLytFactor(nullptr),
    m_pLblFactor(nullptr),
    m_pEdtFactor(nullptr)
{
    // Factor
    //--------

    m_pLytFactor = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytFactor);
    m_pEdtFactor = new QLineEdit();
    m_pEdtFactor->setMinimumWidth(m_cxEdtMinWidth);
    m_pEdtFactor->setMaximumWidth(m_cxEdtMaxWidth);
    m_pEdtFactor->setEnabled(false);
    m_pLblFactor = new QLabel();
    m_pLblFactor->setBuddy(m_pEdtFactor);
    m_pLblFactor->setMinimumWidth(m_cxLblMinWidth);
    m_pLblFactor->setText(tr("Factor:"));
    m_pLytFactor->addWidget(m_pLblFactor);
    m_pLytFactor->addWidget(m_pEdtFactor);
    m_pLytFactor->addStretch();

    // Stretch at the bottom of the main layout
    //=============================================

    m_pLyt->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWdgtUnitViewUntRatio::~CWdgtUnitViewUntRatio()
//------------------------------------------------------------------------------
{
    m_pLytFactor = nullptr;
    m_pLblFactor = nullptr;
    m_pEdtFactor = nullptr;

} // dtor

/*==============================================================================
public: // overridables of base class CWdgtUnitView
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtUnitViewUntRatio::setUnitsModelNode( SUnitsModelNode* i_pNode )
//------------------------------------------------------------------------------
{
    CWdgtUnitViewUnt::setUnitsModelNode(i_pNode);

    CUnitRatio* pUnitRatio = nullptr;

    if( m_pNode != nullptr && m_pNode->m_pUnit != nullptr )
    {
        pUnitRatio = dynamic_cast<CUnitRatio*>(m_pNode->m_pUnit);
    }

    // Ratio
    //----------------------

    if( pUnitRatio == nullptr )
    {
        m_pEdtFactor->setText("---");
    }
    else
    {
        m_pEdtFactor->setText( QString::number(pUnitRatio->getFactor()) );
    }

} // setUnitsModelNode

#endif

/*******************************************************************************
class CWdgtUnitViewUntDataQuantity : public CWdgtUnitViewUnt
*******************************************************************************/

#if 0

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtUnitViewUntDataQuantity::CWdgtUnitViewUntDataQuantity( QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtUnitViewUnt(i_pWdgtParent),
    m_pLytFactor(nullptr),
    m_pLblFactor(nullptr),
    m_pEdtFactor(nullptr)
{
    // Factor
    //--------

    m_pLytFactor = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytFactor);
    m_pEdtFactor = new QLineEdit();
    m_pEdtFactor->setMinimumWidth(m_cxEdtMinWidth);
    m_pEdtFactor->setMaximumWidth(m_cxEdtMaxWidth);
    m_pEdtFactor->setEnabled(false);
    m_pLblFactor = new QLabel();
    m_pLblFactor->setBuddy(m_pEdtFactor);
    m_pLblFactor->setMinimumWidth(m_cxLblMinWidth);
    m_pLblFactor->setText(tr("Factor:"));
    m_pLytFactor->addWidget(m_pLblFactor);
    m_pLytFactor->addWidget(m_pEdtFactor);
    m_pLytFactor->addStretch();

    // Stretch at the bottom of the main layout
    //=============================================

    m_pLyt->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWdgtUnitViewUntDataQuantity::~CWdgtUnitViewUntDataQuantity()
//------------------------------------------------------------------------------
{
    m_pLytFactor = nullptr;
    m_pLblFactor = nullptr;
    m_pEdtFactor = nullptr;

} // dtor

/*==============================================================================
public: // overridables of base class CWdgtUnitView
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtUnitViewUntDataQuantity::setUnitsModelNode( SUnitsModelNode* i_pNode )
//------------------------------------------------------------------------------
{
    CWdgtUnitViewUnt::setUnitsModelNode(i_pNode);

    CUnitDataQuantity* pUnitDataQuantity = nullptr;

    if( m_pNode != nullptr && m_pNode->m_pUnit != nullptr )
    {
        pUnitDataQuantity = dynamic_cast<CUnitDataQuantity*>(m_pNode->m_pUnit);
    }

    // DataQuantity
    //----------------------

    if( pUnitDataQuantity == nullptr )
    {
        m_pEdtFactor->setText("---");
    }
    else
    {
        m_pEdtFactor->setText( QString::number(pUnitDataQuantity->getFactor()) );
    }

} // setUnitsModelNode

#endif


/*******************************************************************************
class CWdgtUnitViewPhysUnit : public CWdgtUnitViewUnt
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtUnitViewPhysUnit::CWdgtUnitViewPhysUnit( QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtUnitViewUnt(i_pWdgtParent),
    // PhysUnit
    m_pLytFctConvertFromSIUnit(nullptr),
    m_pLblFctConvertFromSIUnit(nullptr),
    m_pEdtFctConvertFromSIUnit(nullptr),
    m_pLytFctConvertIntoSIUnit(nullptr),
    m_pLblFctConvertIntoSIUnit(nullptr),
    m_pEdtFctConvertIntoSIUnit(nullptr),
    // Table views with conversion functions
    m_pSplTableViewFctConverts(nullptr),
    m_pLytTableViewFctConvertsInternal(nullptr),
    m_pWdgtTableViewFctConvertsInternal(nullptr),
    m_pLblTableViewFctConvertsInternal(nullptr),
    m_modelFctConvertsInternal("FctConvertsInternal",this),
    m_pTableViewFctConvertsInternal(nullptr),
    m_pLytTableViewFctConvertsExternal(nullptr),
    m_pWdgtTableViewFctConvertsExternal(nullptr),
    m_pLblTableViewFctConvertsExternal(nullptr),
    m_modelFctConvertsExternal("FctConvertsExternal",this),
    m_pTableViewFctConvertsExternal(nullptr)
{
    // Content (configuration) of the physical unit
    //================================================================

    // FctConvertFromSIUnit
    //---------------------

    m_pLytFctConvertFromSIUnit = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytFctConvertFromSIUnit);
    m_pEdtFctConvertFromSIUnit = new QLineEdit();
    m_pEdtFctConvertFromSIUnit->setMinimumWidth(m_cxEdtMinWidth);
    m_pEdtFctConvertFromSIUnit->setMaximumWidth(m_cxEdtMaxWidth);
    m_pEdtFctConvertFromSIUnit->setEnabled(false);
    m_pLblFctConvertFromSIUnit = new QLabel();
    m_pLblFctConvertFromSIUnit->setBuddy(m_pEdtFctConvertFromSIUnit);
    m_pLblFctConvertFromSIUnit->setMinimumWidth(m_cxLblMinWidth);
    m_pLblFctConvertFromSIUnit->setText(tr("FctConvertFromSIUnit:"));
    m_pLytFctConvertFromSIUnit->addWidget(m_pLblFctConvertFromSIUnit);
    m_pLytFctConvertFromSIUnit->addWidget(m_pEdtFctConvertFromSIUnit);
    m_pLytFctConvertFromSIUnit->addStretch();

    // FctConvertIntoSIUnit
    //---------------------

    m_pLytFctConvertIntoSIUnit = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytFctConvertIntoSIUnit);
    m_pEdtFctConvertIntoSIUnit = new QLineEdit();
    m_pEdtFctConvertIntoSIUnit->setMinimumWidth(m_cxEdtMinWidth);
    m_pEdtFctConvertIntoSIUnit->setMaximumWidth(m_cxEdtMaxWidth);
    m_pEdtFctConvertIntoSIUnit->setEnabled(false);
    m_pLblFctConvertIntoSIUnit = new QLabel();
    m_pLblFctConvertIntoSIUnit->setBuddy(m_pEdtFctConvertIntoSIUnit);
    m_pLblFctConvertIntoSIUnit->setMinimumWidth(m_cxLblMinWidth);
    m_pLblFctConvertIntoSIUnit->setText(tr("FctConvertIntoSIUnit:"));
    m_pLytFctConvertIntoSIUnit->addWidget(m_pLblFctConvertIntoSIUnit);
    m_pLytFctConvertIntoSIUnit->addWidget(m_pEdtFctConvertIntoSIUnit);
    m_pLytFctConvertIntoSIUnit->addStretch();

    m_pLyt->addSpacing(10);

    // Table views with conversion functions
    //================================================================

    m_pSplTableViewFctConverts = new QSplitter();
    m_pSplTableViewFctConverts->setChildrenCollapsible(false);
    m_pLyt->addWidget(m_pSplTableViewFctConverts);

    // Table view with internal conversion functions
    //----------------------------------------------

    m_pWdgtTableViewFctConvertsInternal = new QWidget();
    m_pWdgtTableViewFctConvertsInternal->setMinimumWidth(m_cxLblMinWidth+m_cxEdtMinWidth);
    m_pSplTableViewFctConverts->addWidget(m_pWdgtTableViewFctConvertsInternal);

    m_pLytTableViewFctConvertsInternal = new QVBoxLayout;
    m_pLytTableViewFctConvertsInternal->setContentsMargins(2,2,2,2);
    m_pWdgtTableViewFctConvertsInternal->setLayout(m_pLytTableViewFctConvertsInternal);

    // Label (headline)
    //------------------

    m_pLblTableViewFctConvertsInternal = new QLabel();
    m_pLblTableViewFctConvertsInternal->setText(tr("Internal Conversion Routines:"));
    m_pLytTableViewFctConvertsInternal->addWidget(m_pLblTableViewFctConvertsInternal);

    // Table view
    //-----------

    m_pTableViewFctConvertsInternal = new QTableView();
    m_pLytTableViewFctConvertsInternal->addWidget(m_pTableViewFctConvertsInternal);

    m_pTableViewFctConvertsInternal->setModel(&m_modelFctConvertsInternal);
    m_pTableViewFctConvertsInternal->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_pTableViewFctConvertsInternal->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pTableViewFctConvertsInternal->verticalHeader()->hide();
    //m_pTableViewFctConvertsInternal->verticalHeader()->setMinimumSectionSize(1);
    //m_pTableViewFctConvertsInternal->setShowGrid(false);
    //m_pTableViewFctConvertsInternal->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //m_pTableViewFctConvertsInternal->showColumn(CModelPhysUnit::EColumnPhysSizeNameSpace);

    m_pTableViewFctConvertsInternal->resizeColumnsToContents();
    m_pTableViewFctConvertsInternal->resizeRowsToContents();

    // Table view with external conversion functions
    //----------------------------------------------

    m_pWdgtTableViewFctConvertsExternal = new QWidget();
    m_pWdgtTableViewFctConvertsExternal->setMinimumWidth(m_cxLblMinWidth+m_cxEdtMinWidth);
    m_pSplTableViewFctConverts->addWidget(m_pWdgtTableViewFctConvertsExternal);

    m_pLytTableViewFctConvertsExternal = new QVBoxLayout;
    m_pLytTableViewFctConvertsExternal->setContentsMargins(2,2,2,2);
    m_pWdgtTableViewFctConvertsExternal->setLayout(m_pLytTableViewFctConvertsExternal);

    // Label (headline)
    //------------------

    m_pLblTableViewFctConvertsExternal = new QLabel();
    m_pLblTableViewFctConvertsExternal->setText(tr("External Conversion Routines:"));
    m_pLytTableViewFctConvertsExternal->addWidget(m_pLblTableViewFctConvertsExternal);

    // Table view
    //-----------

    m_pTableViewFctConvertsExternal = new QTableView();
    m_pLytTableViewFctConvertsExternal->addWidget(m_pTableViewFctConvertsExternal);

    m_pTableViewFctConvertsExternal->setModel(&m_modelFctConvertsExternal);
    m_pTableViewFctConvertsExternal->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_pTableViewFctConvertsExternal->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pTableViewFctConvertsExternal->verticalHeader()->hide();
    //m_pTableViewFctConvertsExternal->verticalHeader()->setMinimumSectionSize(1);
    //m_pTableViewFctConvertsExternal->setShowGrid(false);
    //m_pTableViewFctConvertsExternal->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //m_pTableViewFctConvertsExternal->showColumn(CModelPhysUnit::EColumnPhysSizeNameSpace);

    m_pTableViewFctConvertsExternal->resizeColumnsToContents();
    m_pTableViewFctConvertsExternal->resizeRowsToContents();

} // ctor

//------------------------------------------------------------------------------
CWdgtUnitViewPhysUnit::~CWdgtUnitViewPhysUnit()
//------------------------------------------------------------------------------
{
    // PhysUnit
    m_pLytFctConvertFromSIUnit = nullptr;
    m_pLblFctConvertFromSIUnit = nullptr;
    m_pEdtFctConvertFromSIUnit = nullptr;
    m_pLytFctConvertIntoSIUnit = nullptr;
    m_pLblFctConvertIntoSIUnit = nullptr;
    m_pEdtFctConvertIntoSIUnit = nullptr;
    m_pSplTableViewFctConverts = nullptr;
    m_pLytTableViewFctConvertsInternal = nullptr;
    m_pWdgtTableViewFctConvertsInternal = nullptr;
    m_pLblTableViewFctConvertsInternal = nullptr;
    m_pTableViewFctConvertsInternal = nullptr;
    m_pLytTableViewFctConvertsExternal = nullptr;
    m_pWdgtTableViewFctConvertsExternal = nullptr;
    m_pLblTableViewFctConvertsExternal = nullptr;
    m_pTableViewFctConvertsExternal = nullptr;

} // dtor

/*==============================================================================
public: // overridables of base class CWdgtUnitView
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtUnitViewPhysUnit::setUnitsModelNode( SUnitsModelNode* i_pNode )
//------------------------------------------------------------------------------
{
    CWdgtUnitViewUnt::setUnitsModelNode(i_pNode);

    CUnitsTreeEntryPhysUnit* pPhysUnit = nullptr;

    if( m_pNode != nullptr && m_pNode->m_pUnit != nullptr )
    {
        pPhysUnit = dynamic_cast<CUnitsTreeEntryPhysUnit*>(m_pNode->m_pUnit);
    }

    // PhysUnit
    //----------------------

    if( pPhysUnit == nullptr )
    {
        m_pEdtFctConvertIntoSIUnit->setText("---");
        m_pEdtFctConvertFromSIUnit->setText("---");
    }
    else
    {
        m_pEdtFctConvertIntoSIUnit->setText( pPhysUnit->getFctConvertIntoSIUnitName() );
        m_pEdtFctConvertFromSIUnit->setText( pPhysUnit->getFctConvertFromSIUnitName() );
    }

    m_modelFctConvertsInternal.setPhysUnit(pPhysUnit);
    m_pTableViewFctConvertsInternal->resizeColumnsToContents();
    m_pTableViewFctConvertsInternal->resizeRowsToContents();

    m_modelFctConvertsExternal.setPhysUnit(pPhysUnit);
    m_pTableViewFctConvertsExternal->resizeColumnsToContents();
    m_pTableViewFctConvertsExternal->resizeRowsToContents();

} // setUnitsModelNode


/*******************************************************************************
class CWdgtUnitViewUntUserDefinedQuantity : public CWdgtUnitViewUnt
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtUnitViewUntUserDefinedQuantity::CWdgtUnitViewUntUserDefinedQuantity( QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtUnitViewUnt(i_pWdgtParent)
{
} // ctor

//------------------------------------------------------------------------------
CWdgtUnitViewUntUserDefinedQuantity::~CWdgtUnitViewUntUserDefinedQuantity()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // overridables of base class CWdgtUnitView
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtUnitViewUntUserDefinedQuantity::setUnitsModelNode( SUnitsModelNode* i_pNode )
//------------------------------------------------------------------------------
{
    CWdgtUnitViewUnt::setUnitsModelNode(i_pNode);
}
