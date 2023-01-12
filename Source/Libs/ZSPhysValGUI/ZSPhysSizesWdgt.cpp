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

#include "ZSPhysValGUI/ZSPhysSizesWdgt.h"
#include "ZSPhysValGUI/ZSPhysSizesTreeViewWdgt.h"
#include "ZSPhysValGUI/ZSPhysTreeEntriesWdgt.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qlayout.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qsplitter.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qsplitter.h>
#endif

#include <QtCore/qsettings.h>

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::PhysVal::GUI;


/*******************************************************************************
class CWdgtPhysSizes : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtPhysSizes::CWdgtPhysSizes(
    CIdxTreeUnits* i_pIdxTree,
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_pIdxTree(nullptr),
    m_pLytMain(nullptr),
    m_pSplitter(nullptr),
    m_pWdgtTreeView(nullptr),
    m_pWdgtTreeEntries(nullptr)
{
    setObjectName( i_pIdxTree == nullptr ? "IdxTreePhysSizes" : i_pIdxTree->objectName() );

    m_pLytMain = new QVBoxLayout();
    setLayout(m_pLytMain);

    // Split View with Tree View and Branch Content Widget
    //====================================================

    m_pSplitter = new QSplitter(Qt::Horizontal);
    m_pLytMain->addWidget(m_pSplitter, 1);

    // <TreeView>
    //-----------

    m_pWdgtTreeView = new CWdgtTreeViewPhysSizes(nullptr, nullptr);
    m_pWdgtTreeView->setMinimumWidth(50);
    m_pSplitter->addWidget(m_pWdgtTreeView);

    QObject::connect(
        m_pWdgtTreeView, &CWdgtTreeViewPhysSizes::currentRowChanged,
        this, &CWdgtPhysSizes::onTreeViewCurrentRowChanged );

    // <TableView>
    //------------

    m_pWdgtTreeEntries = new CWdgtTreeEntries(nullptr, nullptr);
    m_pSplitter->addWidget(m_pWdgtTreeEntries);

    if( i_pIdxTree != nullptr )
    {
        setIdxTree(i_pIdxTree);
    }

    // <Geometry>
    //===================

    QSettings settings;

    restoreGeometry( settings.value(ClassName() + "/" + objectName() + "/Geometry").toByteArray() );

    QList<int> listSizes = m_pSplitter->sizes();
    for( int idx = 0; idx < listSizes.count(); idx++ )
    {
        listSizes[idx] = settings.value( ClassName() + "/" + objectName() + "/SplitterHeight" + QString::number(idx), 50 ).toInt();
    }
    m_pSplitter->setSizes(listSizes);

} // ctor

//------------------------------------------------------------------------------
CWdgtPhysSizes::~CWdgtPhysSizes()
//------------------------------------------------------------------------------
{
    QSettings settings;

    QList<int> listSizes = m_pSplitter->sizes();
    for( int idx = 0; idx < listSizes.count(); idx++ )
    {
        settings.setValue( ClassName() + "/" + objectName() + "/SplitterHeight" + QString::number(idx), listSizes[idx] );
    }
    settings.setValue( ClassName() + "/" + objectName() + "/Geometry", saveGeometry());

    QObject::disconnect(
        m_pWdgtTreeView, &CWdgtTreeViewPhysSizes::currentRowChanged,
        this, &CWdgtPhysSizes::onTreeViewCurrentRowChanged );

    m_pIdxTree = nullptr;
    m_pLytMain = nullptr;
    m_pSplitter = nullptr;
    m_pWdgtTreeView = nullptr;
    m_pWdgtTreeEntries = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtPhysSizes::setIdxTree( CIdxTreeUnits* i_pIdxTree )
//------------------------------------------------------------------------------
{
    if( m_pIdxTree != i_pIdxTree )
    {
        if( m_pIdxTree != nullptr )
        {
            QObject::disconnect(
                m_pIdxTree, &CIdxTreeUnits::aboutToBeDestroyed,
                this, &CWdgtPhysSizes::onIdxTreeAboutToBeDestroyed);
        }

        m_pIdxTree = i_pIdxTree;

        if( m_pIdxTree != nullptr )
        {
            QObject::connect(
                m_pIdxTree, &CIdxTreeUnits::aboutToBeDestroyed,
                this, &CWdgtPhysSizes::onIdxTreeAboutToBeDestroyed);
        }

        m_pWdgtTreeView->setIdxTree(i_pIdxTree);
        m_pWdgtTreeEntries->setIdxTree(i_pIdxTree);
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtPhysSizes::onTreeViewCurrentRowChanged(
    const QModelIndex& i_modelIdxCurr,
    const QModelIndex& i_modelIdxPrev )
//------------------------------------------------------------------------------
{
    if( i_modelIdxCurr.isValid() )
    {
        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdxCurr.internalPointer());

        if( pModelTreeEntry != nullptr )
        {
            m_pWdgtTreeEntries->setKeyInTreeOfRootEntry(pModelTreeEntry->keyInTree());
        }
        else
        {
            m_pWdgtTreeEntries->setKeyInTreeOfRootEntry("");
        }
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtPhysSizes::onIdxTreeAboutToBeDestroyed()
//------------------------------------------------------------------------------
{
    m_pIdxTree = nullptr;

    if( m_pWdgtTreeView != nullptr ) {
        m_pWdgtTreeView->setIdxTree(nullptr);
    }
    if( m_pWdgtTreeEntries != nullptr ) {
        m_pWdgtTreeEntries->setIdxTree(nullptr);
    }
}
