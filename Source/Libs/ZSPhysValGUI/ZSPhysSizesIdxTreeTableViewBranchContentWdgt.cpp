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

#include <QtCore/qglobal.h>

#include "ZSPhysValGUI/ZSPhysSizesIdxTreeTableViewBranchContentWdgt.h"
#include "ZSPhysValGUI/ZSPhysSizesIdxTreeTableViewBranchContent.h"
#include "ZSPhysValGUI/ZSPhysSizesIdxTreeModelBranchContent.h"
#include "ZSPhysVal/ZSPhysSizesIdxTree.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qsplitter.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qsplitter.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;


/*******************************************************************************
class CWdgtIdxTreePhysSizesTableViewBranchContent : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtIdxTreePhysSizesTableViewBranchContent::CWdgtIdxTreePhysSizesTableViewBranchContent(
    CIdxTreePhysSizes* i_pIdxTree,
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_pIdxTree(nullptr),
    m_szBtns(24, 24),
    m_pLytMain(nullptr),
    m_pLytHeadLine(nullptr),
    m_pBtnTreeViewResizeRowsAndColumnsToContents(nullptr),
    m_pTableViewBranchContent(nullptr)
{
    setObjectName( i_pIdxTree == nullptr ? "IdxTree" : i_pIdxTree->objectName() );

    m_pLytMain = new QVBoxLayout();
    setLayout(m_pLytMain);

    // Line with controls to modify optic of tree view
    //================================================

    m_pLytHeadLine = new QHBoxLayout();
    m_pLytMain->addLayout(m_pLytHeadLine);

    // <Button> Resize Columns To Contents
    //------------------------------------

    QPixmap pxmResizeToContents(":/ZS/TreeView/TreeViewResizeToContents.png");

    m_pBtnTreeViewResizeRowsAndColumnsToContents = new QPushButton();
    m_pBtnTreeViewResizeRowsAndColumnsToContents->setIcon(pxmResizeToContents);
    m_pBtnTreeViewResizeRowsAndColumnsToContents->setFixedSize(m_szBtns);
    m_pBtnTreeViewResizeRowsAndColumnsToContents->setToolTip("Press to resize the columns to their contents");
    m_pLytHeadLine->addWidget(m_pBtnTreeViewResizeRowsAndColumnsToContents);

    QObject::connect(
        m_pBtnTreeViewResizeRowsAndColumnsToContents, &QPushButton::clicked,
        this, &CWdgtIdxTreePhysSizesTableViewBranchContent::onBtnTreeViewResizeRowsAndColumnsToContentsClicked );

    m_pLytHeadLine->addSpacing(10);

    // <TableView>
    //============

    m_pTableViewBranchContent = new CTableViewIdxTreePhysSizesBranchContent(nullptr, nullptr);
    m_pLytMain->addWidget(m_pTableViewBranchContent, 1);

    if( i_pIdxTree != nullptr )
    {
        setIdxTree(i_pIdxTree);
    }
} // ctor

//------------------------------------------------------------------------------
CWdgtIdxTreePhysSizesTableViewBranchContent::~CWdgtIdxTreePhysSizesTableViewBranchContent()
//------------------------------------------------------------------------------
{
    m_pIdxTree = nullptr;
    m_szBtns = QSize(0, 0);
    m_pLytMain = nullptr;
    m_pLytHeadLine = nullptr;
    m_pBtnTreeViewResizeRowsAndColumnsToContents = nullptr;
    m_pTableViewBranchContent = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTreePhysSizesTableViewBranchContent::setIdxTree( CIdxTreePhysSizes* i_pIdxTree )
//------------------------------------------------------------------------------
{
    if( m_pIdxTree != i_pIdxTree )
    {
        if( m_pIdxTree != nullptr )
        {
            QObject::disconnect(
                m_pIdxTree, &CIdxTreePhysSizes::aboutToBeDestroyed,
                this, &CWdgtIdxTreePhysSizesTableViewBranchContent::onIdxTreeAboutToBeDestroyed);
        }

        m_pIdxTree = i_pIdxTree;

        if( m_pIdxTree != nullptr )
        {
            QObject::connect(
                m_pIdxTree, &CIdxTreePhysSizes::aboutToBeDestroyed,
                this, &CWdgtIdxTreePhysSizesTableViewBranchContent::onIdxTreeAboutToBeDestroyed);
        }

        m_pTableViewBranchContent->setIdxTree(i_pIdxTree);
    }
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTreePhysSizesTableViewBranchContent::setKeyInTreeOfRootEntry( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    m_pTableViewBranchContent->setKeyInTreeOfRootEntry(i_strKeyInTree);
}

//------------------------------------------------------------------------------
QString CWdgtIdxTreePhysSizesTableViewBranchContent::getKeyInTreeOfRootEntry() const
//------------------------------------------------------------------------------
{
    return m_pTableViewBranchContent->getKeyInTreeOfRootEntry();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTreePhysSizesTableViewBranchContent::onBtnTreeViewResizeRowsAndColumnsToContentsClicked( bool i_bChecked )
//------------------------------------------------------------------------------
{
    if( m_pTableViewBranchContent != nullptr )
    {
        for( int idxClm = 0; idxClm < CModelIdxTreePhysSizesBranchContent::EColumnCount; idxClm++ )
        {
            m_pTableViewBranchContent->resizeColumnToContents(idxClm);
        }
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTreePhysSizesTableViewBranchContent::onIdxTreeAboutToBeDestroyed()
//------------------------------------------------------------------------------
{
    m_pIdxTree = nullptr;
}
