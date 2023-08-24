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

#include "ZSPhysValGUI/ZSPhysTreeEntryUnitQuantityWdgt.h"
#include "ZSPhysVal/ZSPhysTreeEntryUnitQuantity.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"

#if QT_VERSION < 0x050000
#include <QtGui/qlayout.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlineedit.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlineedit.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;


/*******************************************************************************
class CWdgtUnitQuantity : public QTableView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtUnitQuantity::CWdgtUnitQuantity(QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    CWdgtAbstractTreeEntry(i_pWdgtParent),
    m_pLyt(nullptr),
    m_pLytSymbol(nullptr),
    m_pLblSymbol(nullptr),
    m_pEdtSymbol(nullptr),
    m_pLytFactor(nullptr),
    m_pLblFactor(nullptr),
    m_pEdtFactor(nullptr),
    m_pLytNextLower(nullptr),
    m_pLblNextLower(nullptr),
    m_pEdtNextLower(nullptr),
    m_pLytNextHigher(nullptr),
    m_pLblNextHigher(nullptr),
    m_pEdtNextHigher(nullptr)
{
    m_pLyt = new QVBoxLayout;
    setLayout(m_pLyt);

    // Symbol
    //-------

    m_pLytSymbol = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytSymbol);
    m_pEdtSymbol = new QLineEdit();
    m_pEdtSymbol->setFixedWidth(m_cxEdtWidth);
    m_pEdtSymbol->setReadOnly(true);
    //m_pEdtSymbol->setEnabled(false);
    m_pLblSymbol = new QLabel();
    m_pLblSymbol->setBuddy(m_pEdtSymbol);
    m_pLblSymbol->setMinimumWidth(m_cxLblWidth);
    m_pLblSymbol->setText(tr("Symbol:"));
    m_pLytSymbol->addWidget(m_pLblSymbol);
    m_pLytSymbol->addWidget(m_pEdtSymbol);
    m_pLytSymbol->addStretch();

    // Factor
    //--------

    m_pLytFactor = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytFactor);
    m_pEdtFactor = new QLineEdit();
    m_pEdtFactor->setFixedWidth(m_cxEdtWidth);
    m_pEdtFactor->setReadOnly(true);
    //m_pEdtFactor->setEnabled(false);
    m_pLblFactor = new QLabel();
    m_pLblFactor->setBuddy(m_pEdtFactor);
    m_pLblFactor->setMinimumWidth(m_cxLblWidth);
    m_pLblFactor->setText(tr("Factor:"));
    m_pLytFactor->addWidget(m_pLblFactor);
    m_pLytFactor->addWidget(m_pEdtFactor);
    m_pLytFactor->addStretch();

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
    m_pLblNextLower->setMinimumWidth(m_cxLblWidth);
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
    m_pLblNextHigher->setMinimumWidth(m_cxLblWidth);
    m_pLblNextHigher->setText(tr("NextHigher:"));
    m_pLytNextHigher->addWidget(m_pLblNextHigher);
    m_pLytNextHigher->addWidget(m_pEdtNextHigher);
    m_pLytNextHigher->addStretch();

    // Stretch at the bottom of the main layout
    //------------------------------------------

    m_pLyt->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWdgtUnitQuantity::~CWdgtUnitQuantity()
//------------------------------------------------------------------------------
{
    m_pLyt = nullptr;
    m_pLytSymbol = nullptr;
    m_pLblSymbol = nullptr;
    m_pEdtSymbol = nullptr;
    m_pLytFactor = nullptr;
    m_pLblFactor = nullptr;
    m_pEdtFactor = nullptr;
    m_pLytNextLower = nullptr;
    m_pLblNextLower = nullptr;
    m_pEdtNextLower = nullptr;
    m_pLytNextHigher = nullptr;
    m_pLblNextHigher = nullptr;
    m_pEdtNextHigher = nullptr;

} // dtor

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtUnitQuantity::setKeyInTreeOfRootEntry( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    if( m_strKeyInTreeOfRootEntry != i_strKeyInTree )
    {
        m_strKeyInTreeOfRootEntry = i_strKeyInTree;

        CIdxTreeEntry* pTreeEntry = m_pIdxTree->findEntry(m_strKeyInTreeOfRootEntry);

        CUnitsTreeEntryUnitQuantity* pUnit = nullptr;

        if( pTreeEntry != nullptr )
        {
            if( pTreeEntry->isLeave() )
            {
                pUnit = dynamic_cast<CUnitsTreeEntryUnitQuantity*>(pTreeEntry);
            }
        }

        if( pUnit == nullptr )
        {
            m_pEdtSymbol->setText("---");
            m_pEdtFactor->setText("---");
            m_pEdtNextLower->setText("---");
            m_pEdtNextHigher->setText("---");
        }
        else
        {
            m_pEdtSymbol->setText( pUnit->symbol() );
            m_pEdtFactor->setText( QString::number(pUnit->factor()) );

            if( pUnit->nextLowerUnit() == nullptr) 
            {
                m_pEdtNextLower->setText("---");
            }
            else
            {
                m_pEdtNextLower->setText( pUnit->nextLowerUnit()->name() );
            }
            if( pUnit->nextHigherUnit() == nullptr) 
            {
                m_pEdtNextHigher->setText("---");
            }
            else
            {
                m_pEdtNextHigher->setText( pUnit->nextHigherUnit()->name() );
            }
        }
    }
} // setKeyInTreeOfRootEntry
