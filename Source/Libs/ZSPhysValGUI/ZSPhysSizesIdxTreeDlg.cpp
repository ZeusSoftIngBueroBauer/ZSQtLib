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

#include <QtCore/qsettings.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qlayout.h>
#else
#include <QtWidgets/qlayout.h>
#endif

#include "ZSPhysValGUI/ZSPhysSizesIdxTreeDlg.h"
#include "ZSPhysValGUI/ZSPhysSizesIdxTreeWdgt.h"
#include "ZSPhysVal/ZSPhysSizesIdxTree.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
//#include "ZSSys/ZSSysTrcMethod.h"
//#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;


/*******************************************************************************
class CDlgIdxTreePhysSizes : public QDialog
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CDlgIdxTreePhysSizes* CDlgIdxTreePhysSizes::CreateInstance(
    const QString& i_strDlgTitle,
    CIdxTreePhysSizes* i_pIdxTree,
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wFlags )
//------------------------------------------------------------------------------
{
    if( CDialog::GetInstance(NameSpace(), ClassName(), i_pIdxTree->objectName()) != nullptr )
    {
        QString strKey = buildPathStr("::", NameSpace(), ClassName(), i_pIdxTree->objectName());
        throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, strKey);
    }

    // The ctor of base class CDialog adds the instance to the hash of dialogs.
    return new CDlgIdxTreePhysSizes(
        /* strDlgTitle                 */ i_strDlgTitle,
        /* pIdxTree                    */ i_pIdxTree,
        /* pWdgtParent                 */ i_pWdgtParent,
        /* wFlags                      */ i_wFlags );

} // CreateInstance

//------------------------------------------------------------------------------
CDlgIdxTreePhysSizes* CDlgIdxTreePhysSizes::GetInstance( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CDlgIdxTreePhysSizes*>(CDialog::GetInstance(NameSpace(), ClassName(), i_strObjName));
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgIdxTreePhysSizes::CDlgIdxTreePhysSizes(
    const QString& i_strDlgTitle,
    CIdxTreePhysSizes* i_pIdxTree,
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wFlags ) :
//------------------------------------------------------------------------------
    CDialog(
        /* strDlgTitle  */ i_strDlgTitle,
        /* strNameSpace */ NameSpace(),
        /* strClassName */ ClassName(),
        /* strObjName   */ i_pIdxTree->objectName(),
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags ),
    m_pIdxTree(i_pIdxTree),
    m_pLyt(nullptr),
    m_pWdgtIdxTree(nullptr)
{
    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    m_pWdgtIdxTree = new CWdgtIdxTreePhysSizes(m_pIdxTree, nullptr);

    m_pLyt->addWidget(m_pWdgtIdxTree);

} // ctor

//------------------------------------------------------------------------------
CDlgIdxTreePhysSizes::~CDlgIdxTreePhysSizes()
//------------------------------------------------------------------------------
{
    m_pIdxTree = nullptr;
    m_pLyt = nullptr;
    m_pWdgtIdxTree = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor
