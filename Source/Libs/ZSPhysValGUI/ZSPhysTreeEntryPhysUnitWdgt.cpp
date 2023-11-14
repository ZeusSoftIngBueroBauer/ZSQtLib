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

#include "ZSPhysValGUI/ZSPhysTreeEntryPhysUnitWdgt.h"
#include "ZSPhysValGUI/ZSPhysValEditWdgt.h"
#include "ZSPhysValGUI/ZSPhysUnitFctConvertInternalModel.h"
#include "ZSPhysValGUI/ZSPhysUnitFctConvertExternalModel.h"
#include "ZSPhysVal/ZSPhysTreeEntryPhysUnit.h"
#include "ZSPhysVal/ZSPhysTreeEntryGrpPhysUnits.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"

#if QT_VERSION < 0x050000
#include <QtGui/qcombobox.h>
#include <QtGui/qheaderview.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qtableview.h>
#else
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
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;


/*******************************************************************************
class CWdgtPhysUnit : public QTableView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtPhysUnit::CWdgtPhysUnit(QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    CWdgtAbstractTreeEntry(i_pWdgtParent),
    m_szBtns(24, 24),
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
    // Conversion test
    m_pLytLinePhysValSrc(nullptr),
    m_pLblPhysValSrc(nullptr),
    m_pEdtPhysValSrc(nullptr),
    m_pLytLineValDst(nullptr),
    m_pLblValDst(nullptr),
    m_pEdtValDst(nullptr),
    m_pCmbUnitsDst(nullptr),
    // Table views with conversion functions
    m_pSplTableViewFctConverts(nullptr),
    // Internal conversion functions
    m_pWdgtTableViewFctConvertsInternal(nullptr),
    m_pLytTableViewFctConvertsInternal(nullptr),
    m_pLytHeadlineFctConvertsInternal(nullptr),
    m_pLblHeadlineFctConvertsInternal(nullptr),
    m_pBtnTableViewFctConvertsInternalResizeToContents(nullptr),
    m_pModelFctConvertsInternal(nullptr),
    m_pTableViewFctConvertsInternal(nullptr),
    // External conversion functions
    m_pWdgtTableViewFctConvertsExternal(nullptr),
    m_pLytTableViewFctConvertsExternal(nullptr),
    m_pLytHeadlineFctConvertsExternal(nullptr),
    m_pLblHeadlineFctConvertsExternal(nullptr),
    m_pBtnTableViewFctConvertsExternalResizeToContents(nullptr),
    m_pModelFctConvertsExternal(nullptr),
    m_pTableViewFctConvertsExternal(nullptr)
{
    m_pLyt = new QVBoxLayout;
    setLayout(m_pLyt);

    // Symbol
    //---------------

    m_pLytSymbol = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytSymbol);
    m_pEdtSymbol = new QLineEdit();
    m_pEdtSymbol->setFixedWidth(m_cxEdtWidth);
    m_pEdtSymbol->setReadOnly(true);
    //m_pEdtSymbol->setEnabled(false);
    m_pLblSymbol = new QLabel();
    m_pLblSymbol->setBuddy(m_pEdtSymbol);
    m_pLblSymbol->setFixedWidth(m_cxLblWidth);
    m_pLblSymbol->setText(tr("Symbol:"));
    m_pLytSymbol->addWidget(m_pLblSymbol);
    m_pLytSymbol->addWidget(m_pEdtSymbol);
    m_pLytSymbol->addStretch();

    // NextLower
    //-----------

    m_pLytNextLower = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytNextLower);
    m_pEdtNextLower = new QLineEdit();
    m_pEdtNextLower->setFixedWidth(m_cxEdtWidth);
    m_pEdtNextLower->setReadOnly(true);
    //m_pEdtNextLower->setEnabled(false);
    m_pLblNextLower = new QLabel();
    m_pLblNextLower->setBuddy(m_pEdtNextLower);
    m_pLblNextLower->setFixedWidth(m_cxLblWidth);
    m_pLblNextLower->setText(tr("NextLower:"));
    m_pLytNextLower->addWidget(m_pLblNextLower);
    m_pLytNextLower->addWidget(m_pEdtNextLower);
    m_pLytNextLower->addStretch();

    // NextHigher
    //-----------

    m_pLytNextHigher = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytNextHigher);
    m_pEdtNextHigher = new QLineEdit();
    m_pEdtNextHigher->setFixedWidth(m_cxEdtWidth);
    m_pEdtNextHigher->setReadOnly(true);
    //m_pEdtNextHigher->setEnabled(false);
    m_pLblNextHigher = new QLabel();
    m_pLblNextHigher->setBuddy(m_pEdtNextHigher);
    m_pLblNextHigher->setFixedWidth(m_cxLblWidth);
    m_pLblNextHigher->setText(tr("NextHigher:"));
    m_pLytNextHigher->addWidget(m_pLblNextHigher);
    m_pLytNextHigher->addWidget(m_pEdtNextHigher);
    m_pLytNextHigher->addStretch();

    // Content (configuration) of the physical unit
    //=============================================

    // FctConvertFromSIUnit
    //---------------------

    m_pLytFctConvertFromSIUnit = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytFctConvertFromSIUnit);
    m_pEdtFctConvertFromSIUnit = new QLineEdit();
    m_pEdtFctConvertFromSIUnit->setFixedWidth(m_cxEdtWidth);
    m_pEdtFctConvertFromSIUnit->setReadOnly(true);
    //m_pEdtFctConvertFromSIUnit->setEnabled(false);
    m_pLblFctConvertFromSIUnit = new QLabel();
    m_pLblFctConvertFromSIUnit->setBuddy(m_pEdtFctConvertFromSIUnit);
    m_pLblFctConvertFromSIUnit->setMinimumWidth(m_cxLblWidth);
    m_pLblFctConvertFromSIUnit->setText(tr("FctConvertFromSIUnit:"));
    m_pLytFctConvertFromSIUnit->addWidget(m_pLblFctConvertFromSIUnit);
    m_pLytFctConvertFromSIUnit->addWidget(m_pEdtFctConvertFromSIUnit);
    m_pLytFctConvertFromSIUnit->addStretch();

    // FctConvertIntoSIUnit
    //---------------------

    m_pLytFctConvertIntoSIUnit = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytFctConvertIntoSIUnit);
    m_pEdtFctConvertIntoSIUnit = new QLineEdit();
    m_pEdtFctConvertIntoSIUnit->setFixedWidth(m_cxEdtWidth);
    m_pEdtFctConvertIntoSIUnit->setReadOnly(true);
    //m_pEdtFctConvertIntoSIUnit->setEnabled(false);
    m_pLblFctConvertIntoSIUnit = new QLabel();
    m_pLblFctConvertIntoSIUnit->setBuddy(m_pEdtFctConvertIntoSIUnit);
    m_pLblFctConvertIntoSIUnit->setMinimumWidth(m_cxLblWidth);
    m_pLblFctConvertIntoSIUnit->setText(tr("FctConvertIntoSIUnit:"));
    m_pLytFctConvertIntoSIUnit->addWidget(m_pLblFctConvertIntoSIUnit);
    m_pLytFctConvertIntoSIUnit->addWidget(m_pEdtFctConvertIntoSIUnit);
    m_pLytFctConvertIntoSIUnit->addStretch();

    m_pLyt->addSpacing(10);

    // Conversion test
    //================

    m_pLytLinePhysValSrc = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytLinePhysValSrc);

    m_pEdtPhysValSrc = new CWdgtEditPhysVal();
    m_pEdtPhysValSrc->setFixedWidth(m_cxEdtWidth);
    m_pLblPhysValSrc = new QLabel();
    m_pLblPhysValSrc->setBuddy(m_pEdtPhysValSrc);
    m_pLblPhysValSrc->setFixedWidth(m_cxLblWidth);
    m_pLblPhysValSrc->setText(tr("Source Value:"));
    m_pLytLinePhysValSrc->addWidget(m_pLblPhysValSrc);
    m_pLytLinePhysValSrc->addWidget(m_pEdtPhysValSrc);
    m_pLytLinePhysValSrc->addStretch();

    if( !QObject::connect(
        /* pObjSender   */ m_pEdtPhysValSrc,
        /* szSignal     */ SIGNAL(editingFinished()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtPhysValSrcEditingFinished()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pLytLineValDst = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytLineValDst);

    m_pLblValDst = new QLabel();
    m_pLblValDst->setFixedWidth(m_cxLblWidth);
    m_pLblValDst->setText(tr("Converted Value:"));
    m_pLytLineValDst->addWidget(m_pLblValDst);

    QWidget* pWdgtValDst = new QWidget();
    pWdgtValDst->setFixedWidth(m_cxEdtWidth);
    QHBoxLayout* pLytWdgtValDst = new QHBoxLayout();
    pLytWdgtValDst->setContentsMargins(0,0,0,0);
    pWdgtValDst->setLayout(pLytWdgtValDst);
    m_pLytLineValDst->addWidget(pWdgtValDst);

    m_pEdtValDst = new QLineEdit();
    m_pEdtValDst->setReadOnly(true);
    pLytWdgtValDst->addWidget(m_pEdtValDst, 1);

    m_pCmbUnitsDst = new QComboBox();
    //m_pCmbUnitsDst->setFixedWidth(m_cxEdtWidth);
    pLytWdgtValDst->addWidget(m_pCmbUnitsDst);

    if( !QObject::connect(
        /* pObjSender   */ m_pCmbUnitsDst,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbUnitsDstCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pLytLineValDst->addStretch();

    // Table views with conversion functions
    //======================================

    m_pSplTableViewFctConverts = new QSplitter();
    m_pSplTableViewFctConverts->setChildrenCollapsible(false);
    m_pLyt->addWidget(m_pSplTableViewFctConverts, 1);

    // Table view with internal conversion functions
    //----------------------------------------------

    m_pWdgtTableViewFctConvertsInternal = new QWidget();
    m_pWdgtTableViewFctConvertsInternal->setMinimumWidth(m_cxLblWidth+m_cxEdtWidth);
    m_pSplTableViewFctConverts->addWidget(m_pWdgtTableViewFctConvertsInternal);

    m_pLytTableViewFctConvertsInternal = new QVBoxLayout;
    m_pLytTableViewFctConvertsInternal->setContentsMargins(2,2,2,2);
    m_pWdgtTableViewFctConvertsInternal->setLayout(m_pLytTableViewFctConvertsInternal);

    // Label (headline)
    //------------------

    m_pLytHeadlineFctConvertsInternal = new QHBoxLayout();
    m_pLytTableViewFctConvertsInternal->addLayout(m_pLytHeadlineFctConvertsInternal);

    m_pLblHeadlineFctConvertsInternal = new QLabel();
    m_pLblHeadlineFctConvertsInternal->setText(tr("Internal Conversion Routines:"));
    m_pLytHeadlineFctConvertsInternal->addWidget(m_pLblHeadlineFctConvertsInternal);

    m_pLytHeadlineFctConvertsInternal->addStretch();

    QPixmap pxmResizeToContents(":/ZS/TreeView/TreeViewResizeToContents.png");

    m_pBtnTableViewFctConvertsInternalResizeToContents = new QPushButton();
    m_pBtnTableViewFctConvertsInternalResizeToContents->setIcon(pxmResizeToContents);
    m_pBtnTableViewFctConvertsInternalResizeToContents->setFixedSize(m_szBtns);
    m_pBtnTableViewFctConvertsInternalResizeToContents->setToolTip("Press to resize the columns to their contents");
    m_pLytHeadlineFctConvertsInternal->addWidget(m_pBtnTableViewFctConvertsInternalResizeToContents);

    QObject::connect(
        m_pBtnTableViewFctConvertsInternalResizeToContents, &QPushButton::clicked,
        this, &CWdgtPhysUnit::onBtnTableViewFctConvertsInternalResizeToContentsClicked );

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
    m_pWdgtTableViewFctConvertsExternal->setMinimumWidth(m_cxLblWidth+m_cxEdtWidth);
    m_pSplTableViewFctConverts->addWidget(m_pWdgtTableViewFctConvertsExternal);

    m_pLytTableViewFctConvertsExternal = new QVBoxLayout;
    m_pLytTableViewFctConvertsExternal->setContentsMargins(2,2,2,2);
    m_pWdgtTableViewFctConvertsExternal->setLayout(m_pLytTableViewFctConvertsExternal);

    // Label (headline)
    //------------------

    m_pLytHeadlineFctConvertsExternal = new QHBoxLayout();
    m_pLytTableViewFctConvertsExternal->addLayout(m_pLytHeadlineFctConvertsExternal);

    m_pLblHeadlineFctConvertsExternal = new QLabel();
    m_pLblHeadlineFctConvertsExternal->setText(tr("External Conversion Routines:"));
    m_pLytHeadlineFctConvertsExternal->addWidget(m_pLblHeadlineFctConvertsExternal);

    m_pBtnTableViewFctConvertsExternalResizeToContents = new QPushButton();
    m_pBtnTableViewFctConvertsExternalResizeToContents->setIcon(pxmResizeToContents);
    m_pBtnTableViewFctConvertsExternalResizeToContents->setFixedSize(m_szBtns);
    m_pBtnTableViewFctConvertsExternalResizeToContents->setToolTip("Press to resize the columns to their contents");
    m_pLytHeadlineFctConvertsExternal->addWidget(m_pBtnTableViewFctConvertsExternalResizeToContents);

    QObject::connect(
        m_pBtnTableViewFctConvertsExternalResizeToContents, &QPushButton::clicked,
        this, &CWdgtPhysUnit::onBtnTableViewFctConvertsExternalResizeToContentsClicked );

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

    m_szBtns = QSize(0, 0);
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
    // Conversion test
    m_pLytLinePhysValSrc = nullptr;
    m_pLblPhysValSrc = nullptr;
    m_pEdtPhysValSrc = nullptr;
    m_pLytLineValDst = nullptr;
    m_pLblValDst = nullptr;
    m_pEdtValDst = nullptr;
    m_pCmbUnitsDst = nullptr;
    // Table views with conversion functions
    m_pSplTableViewFctConverts = nullptr;
    // Internal conversion functions
    m_pWdgtTableViewFctConvertsInternal = nullptr;
    m_pLytTableViewFctConvertsInternal = nullptr;
    m_pLytHeadlineFctConvertsExternal = nullptr;
    m_pLblHeadlineFctConvertsInternal = nullptr;
    m_pBtnTableViewFctConvertsInternalResizeToContents = nullptr;
    m_pModelFctConvertsInternal = nullptr;
    m_pTableViewFctConvertsInternal = nullptr;
    // External conversion functions
    m_pWdgtTableViewFctConvertsExternal = nullptr;
    m_pLytTableViewFctConvertsExternal = nullptr;
    m_pLytHeadlineFctConvertsExternal = nullptr;
    m_pLblHeadlineFctConvertsExternal = nullptr;
    m_pBtnTableViewFctConvertsExternalResizeToContents = nullptr;
    m_pModelFctConvertsExternal = nullptr;
    m_pTableViewFctConvertsExternal = nullptr;

} // dtor

/*==============================================================================
public: // overridables of base class CWdgtAbstractTreeEntry
==============================================================================*/

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

        QObject::disconnect(
            /* pObjSender   */ m_pEdtPhysValSrc,
            /* szSignal     */ SIGNAL(editingFinished()),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtPhysValSrcEditingFinished()) );
        QObject::disconnect(
            /* pObjSender   */ m_pCmbUnitsDst,
            /* szSignal     */ SIGNAL(currentIndexChanged(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onCmbUnitsDstCurrentIndexChanged(int)) );

        if( pPhysUnit == nullptr )
        {
            m_pEdtSymbol->setText("---");
            m_pEdtNextLower->setText("---");
            m_pEdtNextHigher->setText("---");
            m_pEdtPhysValSrc->clear();
            m_pEdtValDst->setText("---");
            m_pCmbUnitsDst->clear();
            m_pEdtFctConvertIntoSIUnit->setText("---");
            m_pEdtFctConvertFromSIUnit->setText("---");
        }
        else
        {
            CUnitsTreeEntryGrpPhysUnits* pPhysSize = nullptr;
            CIdxTreeEntry* pTreeEntry = pPhysUnit->parentBranch();

            if( pTreeEntry != nullptr && (pTreeEntry->isRoot() || pTreeEntry->isBranch()) ) {
                pPhysSize = dynamic_cast<CUnitsTreeEntryGrpPhysUnits*>(pTreeEntry);
            }

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

            m_pEdtPhysValSrc->setUnit(*pPhysUnit);
            m_pEdtValDst->setText("---");

            if( pPhysSize != nullptr ) {
                for( int idxUnit = 0; idxUnit < pPhysSize->count(); ++idxUnit ) {
                    CUnitsTreeEntryPhysUnit* pPhysUnitDst =
                        dynamic_cast<CUnitsTreeEntryPhysUnit*>(pPhysSize->at(idxUnit));
                    if( pPhysUnitDst != nullptr ) {
                        m_pCmbUnitsDst->addItem(pPhysUnitDst->symbol());
                    }
                }
                m_pCmbUnitsDst->setCurrentText(pPhysUnit->symbol());
                QString strVal = m_pEdtPhysValSrc->value().toString(pPhysUnit->symbol(), PhysValSubStr::Val);
                m_pEdtValDst->setText(strVal);
            }

            m_pEdtFctConvertIntoSIUnit->setText( pPhysUnit->getFctConvertIntoSIUnitName() );
            m_pEdtFctConvertFromSIUnit->setText( pPhysUnit->getFctConvertFromSIUnitName() );
        }

        if( !QObject::connect(
            /* pObjSender   */ m_pEdtPhysValSrc,
            /* szSignal     */ SIGNAL(editingFinished()),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtPhysValSrcEditingFinished()) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pCmbUnitsDst,
            /* szSignal     */ SIGNAL(currentIndexChanged(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onCmbUnitsDstCurrentIndexChanged(int)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        m_pModelFctConvertsInternal->setKeyInTreeOfRootEntry(i_strKeyInTree);
        m_pModelFctConvertsExternal->setKeyInTreeOfRootEntry(i_strKeyInTree);
    }
} // setKeyInTreeOfRootEntry

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtPhysUnit::onEdtPhysValSrcEditingFinished()
//------------------------------------------------------------------------------
{
    QString strSymbol = m_pCmbUnitsDst->currentText();
    QString strVal = m_pEdtPhysValSrc->value().toString(strSymbol, PhysValSubStr::Val);
    m_pEdtValDst->setText(strVal);
}

//------------------------------------------------------------------------------
void CWdgtPhysUnit::onCmbUnitsDstCurrentIndexChanged( int i_idx )
//------------------------------------------------------------------------------
{
    QString strSymbol = m_pCmbUnitsDst->currentText();
    QString strVal = m_pEdtPhysValSrc->value().toString(strSymbol, PhysValSubStr::Val);
    m_pEdtValDst->setText(strVal);
}

//------------------------------------------------------------------------------
void CWdgtPhysUnit::onBtnTableViewFctConvertsInternalResizeToContentsClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( m_pTableViewFctConvertsInternal != nullptr )
    {
        m_pTableViewFctConvertsInternal->resizeColumnsToContents();
    }
}

//------------------------------------------------------------------------------
void CWdgtPhysUnit::onBtnTableViewFctConvertsExternalResizeToContentsClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( m_pTableViewFctConvertsExternal != nullptr )
    {
        m_pTableViewFctConvertsExternal->resizeColumnsToContents();
    }
}
