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

#include "ZSPhysValGUI/ZSPhysTreeEntriesWdgt.h"
#include "ZSPhysValGUI/ZSPhysTreeEntryDataQuantitiesWdgt.h"
#include "ZSPhysValGUI/ZSPhysTreeEntryDataQuantityWdgt.h"
#include "ZSPhysValGUI/ZSPhysTreeEntryRatiosWdgt.h"
#include "ZSPhysValGUI/ZSPhysTreeEntryRatioWdgt.h"
#include "ZSPhysValGUI/ZSPhysTreeEntryPhysSizeWdgt.h"
#include "ZSPhysValGUI/ZSPhysTreeEntryPhysUnitWdgt.h"
#include "ZSPhysValGUI/ZSPhysTreeEntryTypeUndefinedWdgt.h"
#include "ZSPhysValGUI/ZSPhysTreeEntryUserDefinedQuantitiesWdgt.h"
#include "ZSPhysValGUI/ZSPhysTreeEntryUserDefinedQuantityWdgt.h"
#include "ZSPhysVal/ZSPhysSizesIdxTree.h"
#include "ZSPhysVal/ZSPhysScienceField.h"
#include "ZSPhysVal/ZSPhysSize.h"
#include "ZSPhysVal/ZSPhysUnits.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qstackedwidget.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qstackedwidget.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;


/*******************************************************************************
class CWdgtTreeEntries : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtTreeEntries::CWdgtTreeEntries(
    CIdxTreePhysSizes* i_pIdxTree,
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_pIdxTree(nullptr),
    m_strKeyInTreeOfRootEntry(),
    m_szBtns(24, 24),
    m_pLytMain(nullptr),
    m_pLytHeadLine(nullptr),
    m_pBtnTreeViewResizeRowsAndColumnsToContents(nullptr),
    m_pEdtRootEntryPath(nullptr),
    m_pStackedWdgtTreeEntryContent(nullptr),
    m_arpWdgtsTreeEntryContent(ETreeEntryTypeCount, nullptr)
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
        this, &CWdgtTreeEntries::onBtnTreeViewResizeRowsAndColumnsToContentsClicked );

    m_pLytHeadLine->addSpacing(10);

    // <LineEdit> Selected tree node
    //------------------------------

    m_pEdtRootEntryPath = new QLineEdit();
    m_pLytHeadLine->addWidget(m_pEdtRootEntryPath, 1);

    // Content of selected tree node
    //==============================

    m_pStackedWdgtTreeEntryContent = new QStackedWidget();
    m_pLytMain->addWidget(m_pStackedWdgtTreeEntryContent, 1);

    m_arpWdgtsTreeEntryContent[ETreeEntryTypeUndefined] =
        new CWdgtEntryTypeUndefined(nullptr, nullptr);
    m_arpWdgtsTreeEntryContent[ETreeEntryTypeDataQuantities] =
        new CWdgtDataQuantities(nullptr, nullptr);
    m_arpWdgtsTreeEntryContent[ETreeEntryTypePhysSize] =
        new CWdgtPhysSize(nullptr, nullptr);
    m_arpWdgtsTreeEntryContent[ETreeEntryTypeRatios] =
        new CWdgtRatios(nullptr, nullptr);
    m_arpWdgtsTreeEntryContent[ETreeEntryTypeUserDefinedQuantities] =
        new CWdgtUserDefinedQuantities(nullptr, nullptr);
    m_arpWdgtsTreeEntryContent[ETreeEntryTypeDataQuantity] =
        new CWdgtDataQuantity(nullptr, nullptr);
    m_arpWdgtsTreeEntryContent[ETreeEntryTypePhysUnit] =
        new CWdgtPhysUnit(nullptr, nullptr);
    m_arpWdgtsTreeEntryContent[ETreeEntryTypeRatio] =
        new CWdgtRatio(nullptr, nullptr);
    m_arpWdgtsTreeEntryContent[ETreeEntryTypeUserDefinedQuantity] =
        new CWdgtUserDefinedQuantity(nullptr, nullptr);

    for( int idxEntryType = 0; idxEntryType < ETreeEntryTypeCount; idxEntryType++ )
    {
        if( m_arpWdgtsTreeEntryContent[idxEntryType] != nullptr )
        {
            m_pStackedWdgtTreeEntryContent->addWidget(m_arpWdgtsTreeEntryContent[idxEntryType]);
        }
    }
    m_pStackedWdgtTreeEntryContent->setCurrentIndex(ETreeEntryTypeUndefined);

    if( i_pIdxTree != nullptr )
    {
        setIdxTree(i_pIdxTree);
    }
} // ctor

//------------------------------------------------------------------------------
CWdgtTreeEntries::~CWdgtTreeEntries()
//------------------------------------------------------------------------------
{
    m_pIdxTree = nullptr;
    //m_strKeyInTreeOfRootEntry;
    m_szBtns = QSize(0, 0);
    m_pLytMain = nullptr;
    m_pLytHeadLine = nullptr;
    m_pEdtRootEntryPath = nullptr;
    m_pBtnTreeViewResizeRowsAndColumnsToContents = nullptr;
    m_pStackedWdgtTreeEntryContent = nullptr;
    //m_arpWdgtsTreeEntryContent.clear();

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTreeEntries::setIdxTree( CIdxTreePhysSizes* i_pIdxTree )
//------------------------------------------------------------------------------
{
    if( m_pIdxTree != i_pIdxTree )
    {
        if( m_pIdxTree != nullptr )
        {
            QObject::disconnect(
                m_pIdxTree, &CIdxTreePhysSizes::aboutToBeDestroyed,
                this, &CWdgtTreeEntries::onIdxTreeAboutToBeDestroyed);
        }

        m_pIdxTree = i_pIdxTree;
        m_strKeyInTreeOfRootEntry = "";
        m_pEdtRootEntryPath->setText("");

        if( m_pIdxTree != nullptr )
        {
            QObject::connect(
                m_pIdxTree, &CIdxTreePhysSizes::aboutToBeDestroyed,
                this, &CWdgtTreeEntries::onIdxTreeAboutToBeDestroyed);
        }

        for( int idxEntryType = 0; idxEntryType < ETreeEntryTypeCount; idxEntryType++ )
        {
            if( m_arpWdgtsTreeEntryContent[idxEntryType] != nullptr )
            {
                m_arpWdgtsTreeEntryContent[idxEntryType]->setIdxTree(m_pIdxTree);
                m_arpWdgtsTreeEntryContent[idxEntryType]->setKeyInTreeOfRootEntry(m_strKeyInTreeOfRootEntry);
            }
        }
    }
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTreeEntries::setKeyInTreeOfRootEntry( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    if( m_strKeyInTreeOfRootEntry != i_strKeyInTree )
    {
        m_strKeyInTreeOfRootEntry = i_strKeyInTree;

        EUnitClassType unitClassType = EUnitClassType::Undefined;
        ETreeEntryType treeEntryType = ETreeEntryTypeUndefined;
        QString        strEntryPath;

        if( m_pIdxTree != nullptr )
        {
            CIdxTreeEntry* pTreeEntry = m_pIdxTree->findEntry(i_strKeyInTree);

            if( pTreeEntry != nullptr )
            {
                if( pTreeEntry->isRoot() || pTreeEntry->isBranch() )
                {
                    CUnitGrpTreeEntry* pUnitGrp = dynamic_cast<CUnitGrpTreeEntry*>(pTreeEntry);

                    if( pUnitGrp == nullptr )
                    {
                        CPhysScienceFieldTreeEntry* pPhysScienceField = dynamic_cast<CPhysScienceFieldTreeEntry*>(pTreeEntry);
                        if( pPhysScienceField != nullptr )
                        {
                            unitClassType = EUnitClassType::PhysSize;
                        }
                    }
                    else
                    {
                        unitClassType = pUnitGrp->classType();
                    }
                    if( unitClassType == EUnitClassType::DataQuantity )
                    {
                        treeEntryType = ETreeEntryTypeDataQuantities;
                    }
                    else if( unitClassType == EUnitClassType::PhysSize )
                    {
                        treeEntryType = ETreeEntryTypePhysSize;
                    }
                    else if( unitClassType == EUnitClassType::Ratios )
                    {
                        treeEntryType = ETreeEntryTypeRatios;
                    }
                    else if( unitClassType == EUnitClassType::UserDefinedQuantities )
                    {
                        treeEntryType = ETreeEntryTypeUserDefinedQuantities;
                    }
                }
                else if( pTreeEntry->isLeave() )
                {
                    CUnitTreeEntry* pUnit = dynamic_cast<CUnitTreeEntry*>(pTreeEntry);

                    if( pUnit != nullptr )
                    {
                        unitClassType = pUnit->classType();

                        if( unitClassType == EUnitClassType::DataQuantity )
                        {
                            treeEntryType = ETreeEntryTypeDataQuantity;
                        }
                        else if( unitClassType == EUnitClassType::PhysSize )
                        {
                            treeEntryType = ETreeEntryTypePhysUnit;
                        }
                        else if( unitClassType == EUnitClassType::Ratios )
                        {
                            treeEntryType = ETreeEntryTypeRatio;
                        }
                        else if( unitClassType == EUnitClassType::UserDefinedQuantities )
                        {
                            treeEntryType = ETreeEntryTypeUserDefinedQuantity;
                        }
                    }
                }

                if( unitClassType != EUnitClassType::Undefined )
                {
                    strEntryPath += CEnumUnitClassType(unitClassType).toString() + ": ";
                }
                strEntryPath += pTreeEntry->path();

            } // if( pTreeEntry != nullptr )
        } // if( m_pIdxTree != nullptr )

        m_pEdtRootEntryPath->setText(strEntryPath);

        for( int idxEntryType = 0; idxEntryType < ETreeEntryTypeCount; idxEntryType++ )
        {
            if( m_arpWdgtsTreeEntryContent[idxEntryType] != nullptr )
            {
                m_arpWdgtsTreeEntryContent[idxEntryType]->setKeyInTreeOfRootEntry(m_strKeyInTreeOfRootEntry);
            }
        }

        m_pStackedWdgtTreeEntryContent->setCurrentIndex(treeEntryType);

    } // if( m_strKeyInTreeOfRootEntry != i_strKeyInTree )
} // setKeyInTreeOfRootEntry

//------------------------------------------------------------------------------
QString CWdgtTreeEntries::getKeyInTreeOfRootEntry() const
//------------------------------------------------------------------------------
{
    return m_strKeyInTreeOfRootEntry;
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTreeEntries::onBtnTreeViewResizeRowsAndColumnsToContentsClicked( bool i_bChecked )
//------------------------------------------------------------------------------
{
    int idxEntryType = m_pStackedWdgtTreeEntryContent->currentIndex();

    if( idxEntryType >= 0 && idxEntryType < m_arpWdgtsTreeEntryContent.size() )
    {
        if( m_arpWdgtsTreeEntryContent[idxEntryType] != nullptr )
        {
            m_arpWdgtsTreeEntryContent[idxEntryType]->resizeToContents();
        }
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTreeEntries::onIdxTreeAboutToBeDestroyed()
//------------------------------------------------------------------------------
{
    m_pIdxTree = nullptr;
}
