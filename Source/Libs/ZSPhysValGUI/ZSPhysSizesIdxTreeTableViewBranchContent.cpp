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

#include "ZSPhysValGUI/ZSPhysSizesIdxTreeTableViewBranchContent.h"
#include "ZSPhysValGUI/ZSPhysSizesIdxTreeModelBranchContent.h"
#include "ZSPhysVal/ZSPhysSizesIdxTree.h"

#include <QtGui/qevent.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qmenu.h>
#else
#include <QtWidgets/qmenu.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;


/*******************************************************************************
class CTableViewIdxTreePhysSizesBranchContent : public QTableView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTableViewIdxTreePhysSizesBranchContent::CTableViewIdxTreePhysSizesBranchContent(
    CIdxTreePhysSizes* i_pIdxTree, QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QTableView(i_pWdgtParent),
    m_pIdxTree(nullptr),
    m_pModel(nullptr),
    //m_strKeyInTreeOfRootEntry(),
    m_modelIdxSelectedOnMousePressEvent(),
    m_modelIdxSelectedOnMouseReleaseEvent()
{
    setObjectName( QString(i_pIdxTree == nullptr ? "IdxTree" : i_pIdxTree->objectName()) );

    m_pModel = new CModelIdxTreePhysSizesBranchContent(nullptr, nullptr);

    setModel(m_pModel);

    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::SingleSelection);

    if( i_pIdxTree != nullptr )
    {
        setIdxTree(i_pIdxTree);
    }
} // ctor

//------------------------------------------------------------------------------
CTableViewIdxTreePhysSizesBranchContent::~CTableViewIdxTreePhysSizesBranchContent()
//------------------------------------------------------------------------------
{
    try
    {
        delete m_pModel;
    }
    catch(...)
    {
    }

    m_pIdxTree = nullptr;
    m_pModel = nullptr;
    //m_strKeyInTreeOfRootEntry;
    //m_modelIdxSelectedOnMousePressEvent;
    //m_modelIdxSelectedOnMouseReleaseEvent;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTableViewIdxTreePhysSizesBranchContent::setIdxTree( CIdxTreePhysSizes* i_pIdxTree )
//------------------------------------------------------------------------------
{
    if( m_pIdxTree != i_pIdxTree )
    {
        if( m_pIdxTree != nullptr )
        {
            QObject::disconnect(
                m_pIdxTree, &CIdxTreePhysSizes::aboutToBeDestroyed,
                this, &CTableViewIdxTreePhysSizesBranchContent::onIdxTreeAboutToBeDestroyed);
        }

        m_pIdxTree = i_pIdxTree;

        if( m_pIdxTree != nullptr )
        {
            QObject::connect(
                m_pIdxTree, &CIdxTreePhysSizes::aboutToBeDestroyed,
                this, &CTableViewIdxTreePhysSizesBranchContent::onIdxTreeAboutToBeDestroyed);
        }

        m_pModel->setIdxTree(i_pIdxTree);
    }
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTableViewIdxTreePhysSizesBranchContent::setKeyInTreeOfRootEntry( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    m_pModel->setKeyInTreeOfRootEntry(i_strKeyInTree);
}

//------------------------------------------------------------------------------
QString CTableViewIdxTreePhysSizesBranchContent::getKeyInTreeOfRootEntry() const
//------------------------------------------------------------------------------
{
    return m_pModel->getKeyInTreeOfRootEntry();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTableViewIdxTreePhysSizesBranchContent::onIdxTreeAboutToBeDestroyed()
//------------------------------------------------------------------------------
{
    m_pIdxTree = nullptr;
}
