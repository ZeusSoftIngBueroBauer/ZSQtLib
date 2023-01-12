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

#include "ZSPhysValGUI/ZSPhysTreeEntryPhysUnitWdgt.h"
#include "ZSPhysValGUI/ZSPhysUnitFctConvertInternalModel.h"
#include "ZSPhysValGUI/ZSPhysUnitFctConvertExternalModel.h"
#include "ZSPhysVal/ZSPhysTreeEntryPhysUnit.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"

#if QT_VERSION < 0x050000
#include <QtGui/qheaderview.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qtableview.h>
#else
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qtableview.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;


/*******************************************************************************
class CWdgtPhysUnit : public QTableView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtPhysUnit::CWdgtPhysUnit(
    CIdxTreeUnits* i_pIdxTree, QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtAbstractTreeEntry(i_pIdxTree, i_pWdgtParent),
    m_pLyt(nullptr),
    m_pLytSymbol(nullptr),
    m_pLblSymbol(nullptr),
    m_pEdtSymbol(nullptr),
    m_pLytNextLower(nullptr),
    m_pLblNextLower(nullptr),
    m_pEdtNextLower(nullptr),
    m_pLytNextHigher(nullptr),
    m_pLblNextHigher(nullptr),
    m_pEdtNextHigher(nullptr),
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
    m_pModelFctConvertsInternal(nullptr),
    m_pTableViewFctConvertsInternal(nullptr),
    m_pLytTableViewFctConvertsExternal(nullptr),
    m_pWdgtTableViewFctConvertsExternal(nullptr),
    m_pLblTableViewFctConvertsExternal(nullptr),
    m_pModelFctConvertsExternal(nullptr),
    m_pTableViewFctConvertsExternal(nullptr)
{
    setObjectName( QString(i_pIdxTree == nullptr ? "IdxTree" : i_pIdxTree->objectName()) );

    m_pLyt = new QVBoxLayout;
    setLayout(m_pLyt);

    // Symbol
    //---------------

    m_pLytSymbol = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytSymbol);
    m_pEdtSymbol = new QLineEdit();
    m_pEdtSymbol->setMinimumWidth(m_cxEdtMinWidth);
    m_pEdtSymbol->setMaximumWidth(m_cxEdtMaxWidth);
    m_pEdtSymbol->setReadOnly(true);
    //m_pEdtSymbol->setEnabled(false);
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
    m_pEdtNextLower->setReadOnly(true);
    //m_pEdtNextLower->setEnabled(false);
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
    m_pEdtNextHigher->setReadOnly(true);
    //m_pEdtNextHigher->setEnabled(false);
    m_pLblNextHigher = new QLabel();
    m_pLblNextHigher->setBuddy(m_pEdtNextHigher);
    m_pLblNextHigher->setMinimumWidth(m_cxLblMinWidth);
    m_pLblNextHigher->setText(tr("NextHigher:"));
    m_pLytNextHigher->addWidget(m_pLblNextHigher);
    m_pLytNextHigher->addWidget(m_pEdtNextHigher);
    m_pLytNextHigher->addStretch();

    // Content (configuration) of the physical unit
    //================================================================

    // FctConvertFromSIUnit
    //---------------------

    m_pLytFctConvertFromSIUnit = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytFctConvertFromSIUnit);
    m_pEdtFctConvertFromSIUnit = new QLineEdit();
    m_pEdtFctConvertFromSIUnit->setMinimumWidth(m_cxEdtMinWidth);
    m_pEdtFctConvertFromSIUnit->setMaximumWidth(m_cxEdtMaxWidth);
    m_pEdtFctConvertFromSIUnit->setReadOnly(true);
    //m_pEdtFctConvertFromSIUnit->setEnabled(false);
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
    m_pEdtFctConvertIntoSIUnit->setReadOnly(true);
    //m_pEdtFctConvertIntoSIUnit->setEnabled(false);
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

    m_pModelFctConvertsInternal = new CModelUnitFctConvertInternal(m_pIdxTree);

    m_pTableViewFctConvertsInternal = new QTableView();
    m_pLytTableViewFctConvertsInternal->addWidget(m_pTableViewFctConvertsInternal);

    m_pTableViewFctConvertsInternal->setModel(m_pModelFctConvertsInternal);
    m_pTableViewFctConvertsInternal->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_pTableViewFctConvertsInternal->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pTableViewFctConvertsInternal->verticalHeader()->hide();
    m_pTableViewFctConvertsInternal->setStyleSheet(R"(
        QHeaderView::section {
            background-color:lightGray;
            border: 1px solid darkGray;
        }
    )");

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

    m_pModelFctConvertsExternal = new CModelUnitFctConvertExternal(m_pIdxTree);

    m_pTableViewFctConvertsExternal = new QTableView();
    m_pLytTableViewFctConvertsExternal->addWidget(m_pTableViewFctConvertsExternal);

    m_pTableViewFctConvertsExternal->setModel(m_pModelFctConvertsExternal);
    m_pTableViewFctConvertsExternal->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_pTableViewFctConvertsExternal->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pTableViewFctConvertsExternal->verticalHeader()->hide();
    m_pTableViewFctConvertsExternal->setStyleSheet(R"(
        QHeaderView::section {
            background-color:lightGray;
            border: 1px solid darkGray;
        }
    )");

} // ctor

//------------------------------------------------------------------------------
CWdgtPhysUnit::~CWdgtPhysUnit()
//------------------------------------------------------------------------------
{
    try {
        delete m_pModelFctConvertsInternal;
    }
    catch(...) {
    }

    try {
        delete m_pModelFctConvertsExternal;
    }
    catch(...) {
    }

    m_pLyt = nullptr;
    m_pLytSymbol = nullptr;
    m_pLblSymbol = nullptr;
    m_pEdtSymbol = nullptr;
    m_pLytNextLower = nullptr;
    m_pLblNextLower = nullptr;
    m_pEdtNextLower = nullptr;
    m_pLytNextHigher = nullptr;
    m_pLblNextHigher = nullptr;
    m_pEdtNextHigher = nullptr;
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
    m_pModelFctConvertsInternal = nullptr;
    m_pTableViewFctConvertsInternal = nullptr;
    m_pLytTableViewFctConvertsExternal = nullptr;
    m_pWdgtTableViewFctConvertsExternal = nullptr;
    m_pLblTableViewFctConvertsExternal = nullptr;
    m_pModelFctConvertsExternal = nullptr;
    m_pTableViewFctConvertsExternal = nullptr;

} // dtor

/*==============================================================================
public: // overridables of base class CWdgtAbstractTreeEntry
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtPhysUnit::setIdxTree( CIdxTreeUnits* i_pIdxTree )
//------------------------------------------------------------------------------
{
    CWdgtAbstractTreeEntry::setIdxTree(i_pIdxTree);

    m_pModelFctConvertsInternal->setIdxTree(i_pIdxTree);
    m_pModelFctConvertsExternal->setIdxTree(i_pIdxTree);

    setKeyInTreeOfRootEntry("");
}

//------------------------------------------------------------------------------
void CWdgtPhysUnit::setKeyInTreeOfRootEntry( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    if( m_strKeyInTreeOfRootEntry != i_strKeyInTree )
    {
        m_strKeyInTreeOfRootEntry = i_strKeyInTree;

        CIdxTreeEntry* pTreeEntry = m_pIdxTree->findEntry(m_strKeyInTreeOfRootEntry);

        CUnitsTreeEntryPhysUnit* pPhysUnit = nullptr;

        if( pTreeEntry != nullptr )
        {
            if( pTreeEntry->isLeave() )
            {
                pPhysUnit = dynamic_cast<CUnitsTreeEntryPhysUnit*>(pTreeEntry);
            }
        }

        if( pPhysUnit == nullptr )
        {
            m_pEdtSymbol->setText("---");
            m_pEdtNextLower->setText("---");
            m_pEdtNextHigher->setText("---");
            m_pEdtFctConvertIntoSIUnit->setText("---");
            m_pEdtFctConvertFromSIUnit->setText("---");
        }
        else
        {
            m_pEdtSymbol->setText( pPhysUnit->symbol() );

            if( pPhysUnit->nextLowerUnit() == nullptr) 
            {
                m_pEdtNextLower->setText("---");
            }
            else
            {
                m_pEdtNextLower->setText( pPhysUnit->nextLowerUnit()->name() );
            }
            if( pPhysUnit->nextHigherUnit() == nullptr) 
            {
                m_pEdtNextHigher->setText("---");
            }
            else
            {
                m_pEdtNextHigher->setText( pPhysUnit->nextHigherUnit()->name() );
            }
            m_pEdtFctConvertIntoSIUnit->setText( pPhysUnit->getFctConvertIntoSIUnitName() );
            m_pEdtFctConvertFromSIUnit->setText( pPhysUnit->getFctConvertFromSIUnitName() );
        }

        m_pModelFctConvertsInternal->setKeyInTreeOfRootEntry(i_strKeyInTree);
        m_pModelFctConvertsExternal->setKeyInTreeOfRootEntry(i_strKeyInTree);
    }
} // setKeyInTreeOfRootEntry

//------------------------------------------------------------------------------
void CWdgtPhysUnit::resizeToContents()
//------------------------------------------------------------------------------
{
    m_pTableViewFctConvertsInternal->resizeColumnsToContents();
    m_pTableViewFctConvertsExternal->resizeColumnsToContents();
}
