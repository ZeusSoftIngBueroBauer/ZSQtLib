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

#include "ZSPhysValGUI/ZSPhysTreeEntryAbstractWdgt.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;


/*******************************************************************************
class CWdgtAbstractTreeEntry : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtAbstractTreeEntry::CWdgtAbstractTreeEntry(
    CIdxTreeUnits* i_pIdxTree, QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pIdxTree(nullptr),
    m_strKeyInTreeOfRootEntry(),
    m_cxLblMinWidth(140),
    m_cxEdtMinWidth(160),
    m_cxEdtMaxWidth(240)
{
    setObjectName( QString(i_pIdxTree == nullptr ? "IdxTree" : i_pIdxTree->objectName()) );

    if( i_pIdxTree != nullptr )
    {
        setIdxTree(i_pIdxTree);
    }
} // ctor

//------------------------------------------------------------------------------
CWdgtAbstractTreeEntry::~CWdgtAbstractTreeEntry()
//------------------------------------------------------------------------------
{
    m_pIdxTree = nullptr;
    //m_strKeyInTreeOfRootEntry;
    m_cxLblMinWidth = 0;
    m_cxEdtMinWidth = 0;
    m_cxEdtMaxWidth = 0;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtAbstractTreeEntry::setIdxTree( CIdxTreeUnits* i_pIdxTree )
//------------------------------------------------------------------------------
{
    if( m_pIdxTree != i_pIdxTree )
    {
        if( m_pIdxTree != nullptr )
        {
            QObject::disconnect(
                m_pIdxTree, &CIdxTreeUnits::aboutToBeDestroyed,
                this, &CWdgtAbstractTreeEntry::onIdxTreeAboutToBeDestroyed);
        }

        m_pIdxTree = i_pIdxTree;
        m_strKeyInTreeOfRootEntry = "";

        if( m_pIdxTree != nullptr )
        {
            QObject::connect(
                m_pIdxTree, &CIdxTreeUnits::aboutToBeDestroyed,
                this, &CWdgtAbstractTreeEntry::onIdxTreeAboutToBeDestroyed);
        }
    }
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtAbstractTreeEntry::setKeyInTreeOfRootEntry( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    if( m_strKeyInTreeOfRootEntry != i_strKeyInTree )
    {
        m_strKeyInTreeOfRootEntry = i_strKeyInTree;
    }
}

//------------------------------------------------------------------------------
QString CWdgtAbstractTreeEntry::getKeyInTreeOfRootEntry() const
//------------------------------------------------------------------------------
{
    return m_strKeyInTreeOfRootEntry;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtAbstractTreeEntry::resizeToContents()
//------------------------------------------------------------------------------
{
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtAbstractTreeEntry::onIdxTreeAboutToBeDestroyed()
//------------------------------------------------------------------------------
{
    m_pIdxTree = nullptr;
}
