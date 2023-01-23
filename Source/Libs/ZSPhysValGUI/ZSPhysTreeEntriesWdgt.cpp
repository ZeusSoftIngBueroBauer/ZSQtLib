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

#include <QtCore/qglobal.h>

#include "ZSPhysValGUI/ZSPhysTreeEntriesWdgt.h"
#include "ZSPhysValGUI/ZSPhysTreeEntryGrpPhysUnitsWdgt.h"
#include "ZSPhysValGUI/ZSPhysTreeEntryPhysUnitWdgt.h"
#include "ZSPhysValGUI/ZSPhysTreeEntryUnitQuantityWdgt.h"
#include "ZSPhysValGUI/ZSPhysTreeEntryUnitRatioWdgt.h"
#include "ZSPhysValGUI/ZSPhysTreeEntryTypeUndefinedWdgt.h"
#include "ZSPhysVal/ZSPhysTreeEntryGrpBase.h"
#include "ZSPhysVal/ZSPhysTreeEntryGrpScienceField.h"
#include "ZSPhysVal/ZSPhysTreeEntryUnitBase.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"

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
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_pIdxTree(CIdxTreeUnits::GetInstance()),
    m_strKeyInTreeOfRootEntry(),
    m_pLytMain(nullptr),
    m_pLytHeadLine(nullptr),
    m_pEdtRootEntryPath(nullptr),
    m_pStackedWdgtTreeEntryContent(nullptr),
    m_arpWdgtsTreeEntryContent(ETreeEntryTypeCount, nullptr)
{
    setObjectName(m_pIdxTree->objectName());

    QObject::connect(
        m_pIdxTree, &CIdxTreeUnits::aboutToBeDestroyed,
        this, &CWdgtTreeEntries::onIdxTreeAboutToBeDestroyed);

    m_pLytMain = new QVBoxLayout();
    setLayout(m_pLytMain);

    // Headline
    //---------

    m_pLytHeadLine = new QHBoxLayout();
    m_pLytMain->addLayout(m_pLytHeadLine);

    m_pEdtRootEntryPath = new QLineEdit();
    m_pLytHeadLine->addWidget(m_pEdtRootEntryPath, 1);

    // Content of selected tree node
    //------------------------------

    m_pStackedWdgtTreeEntryContent = new QStackedWidget();
    m_pLytMain->addWidget(m_pStackedWdgtTreeEntryContent, 1);

    m_arpWdgtsTreeEntryContent[ETreeEntryTypeUndefined] =
        new CWdgtEntryTypeUndefined();
    m_arpWdgtsTreeEntryContent[ETreeEntryTypePhysSize] =
        new CWdgtPhysUnitsGrp();
    m_arpWdgtsTreeEntryContent[ETreeEntryTypeQuantity] =
        new CWdgtUnitQuantity();
    m_arpWdgtsTreeEntryContent[ETreeEntryTypePhysUnit] =
        new CWdgtPhysUnit();
    m_arpWdgtsTreeEntryContent[ETreeEntryTypeRatio] =
        new CWdgtUnitRatio();

    for( int idxEntryType = 0; idxEntryType < ETreeEntryTypeCount; idxEntryType++ )
    {
        if( m_arpWdgtsTreeEntryContent[idxEntryType] != nullptr )
        {
            m_pStackedWdgtTreeEntryContent->addWidget(m_arpWdgtsTreeEntryContent[idxEntryType]);
        }
    }
    m_pStackedWdgtTreeEntryContent->setCurrentIndex(ETreeEntryTypeUndefined);

} // ctor

//------------------------------------------------------------------------------
CWdgtTreeEntries::~CWdgtTreeEntries()
//------------------------------------------------------------------------------
{
    m_pIdxTree = nullptr;
    //m_strKeyInTreeOfRootEntry;
    m_pLytMain = nullptr;
    m_pLytHeadLine = nullptr;
    m_pEdtRootEntryPath = nullptr;
    m_pStackedWdgtTreeEntryContent = nullptr;
    //m_arpWdgtsTreeEntryContent.clear();

} // dtor

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
                    CUnitsTreeEntryGrpBase* pUnitGrp = dynamic_cast<CUnitsTreeEntryGrpBase*>(pTreeEntry);

                    if( pUnitGrp != nullptr )
                    {
                        unitClassType = pUnitGrp->classType();
                    }
                    if( unitClassType == EUnitClassType::PhysSize )
                    {
                        treeEntryType = ETreeEntryTypePhysSize;
                    }
                }
                else if( pTreeEntry->isLeave() )
                {
                    CUnitsTreeEntryUnitBase* pUnit = dynamic_cast<CUnitsTreeEntryUnitBase*>(pTreeEntry);

                    if( pUnit != nullptr )
                    {
                        unitClassType = pUnit->classType();

                        if( unitClassType == EUnitClassType::Quantity )
                        {
                            treeEntryType = ETreeEntryTypeQuantity;
                        }
                        else if( unitClassType == EUnitClassType::PhysSize )
                        {
                            treeEntryType = ETreeEntryTypePhysUnit;
                        }
                        else if( unitClassType == EUnitClassType::Ratio )
                        {
                            treeEntryType = ETreeEntryTypeRatio;
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
void CWdgtTreeEntries::onIdxTreeAboutToBeDestroyed()
//------------------------------------------------------------------------------
{
    m_pIdxTree = nullptr;
}
