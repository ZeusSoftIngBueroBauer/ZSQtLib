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

#include <QtCore/qsettings.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qlayout.h>
#else
#include <QtWidgets/qlayout.h>
#endif

#include "ZSPhysValGUI/ZSPhysUnitsDlg.h"
#include "ZSPhysValGUI/ZSPhysUnitsWdgt.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;


/*******************************************************************************
class CDlgPhysUnits : public QDialog
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CDlgPhysUnits* CDlgPhysUnits::GetInstance()
//------------------------------------------------------------------------------
{
    QString strObjName = CIdxTreeUnits::GetInstance()->objectName();
    return dynamic_cast<CDlgPhysUnits*>(CDialog::GetInstance(NameSpace(), ClassName(), strObjName));
}

//------------------------------------------------------------------------------
CDlgPhysUnits* CDlgPhysUnits::CreateInstance(
    const QString& i_strDlgTitle,
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wFlags )
//------------------------------------------------------------------------------
{
    if( GetInstance() != nullptr )
    {
        QString strObjName = CIdxTreeUnits::GetInstance()->objectName();
        QString strKey = buildPathStr("::", NameSpace(), ClassName(), strObjName);
        throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, strKey);
    }

    // The ctor of base class CDialog adds the instance to the hash of dialogs.
    return new CDlgPhysUnits(i_strDlgTitle, i_pWdgtParent, i_wFlags);
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgPhysUnits::CDlgPhysUnits(
    const QString& i_strDlgTitle,
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wFlags ) :
//------------------------------------------------------------------------------
    CDialog(
        /* strDlgTitle  */ i_strDlgTitle,
        /* strNameSpace */ NameSpace(),
        /* strClassName */ ClassName(),
        /* strObjName   */ CIdxTreeUnits::GetInstance()->objectName(),
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags ),
    m_pLyt(nullptr),
    m_pWdgtUnits(nullptr)
{
    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    m_pWdgtUnits = new CWdgtUnits();
    m_pLyt->addWidget(m_pWdgtUnits);

} // ctor

//------------------------------------------------------------------------------
CDlgPhysUnits::~CDlgPhysUnits()
//------------------------------------------------------------------------------
{
    m_pLyt = nullptr;
    m_pWdgtUnits = nullptr;

} // dtor
