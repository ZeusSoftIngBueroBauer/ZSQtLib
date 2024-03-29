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

#include "ZSSysGUI/ZSSysLoggerIdxTreeDlg.h"
#include "ZSSysGUI/ZSSysLoggerIdxTreeWdgt.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysLoggerIdxTree.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CDlgIdxTreeLoggers : public QDialog
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CDlgIdxTreeLoggers* CDlgIdxTreeLoggers::CreateInstance(
    const QString& i_strDlgTitle,
    CIdxTreeLoggers* i_pIdxTree,
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wFlags,
    EMethodTraceDetailLevel i_eTrcDetailLevel,
    EMethodTraceDetailLevel i_eTrcDetailLevelNoisyMethods )
//------------------------------------------------------------------------------
{
    if( CDialog::GetInstance(NameSpace(), ClassName(), i_pIdxTree->objectName()) != nullptr )
    {
        QString strKey = buildPathStr("::", NameSpace(), ClassName(), i_pIdxTree->objectName());
        throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, strKey);
    }

    // The ctor of base class CDialog adds the instance to the hash of dialogs.
    return new CDlgIdxTreeLoggers(
        /* strDlgTitle                 */ i_strDlgTitle,
        /* pIdxTree                    */ i_pIdxTree,
        /* pWdgtParent                 */ i_pWdgtParent,
        /* wFlags                      */ i_wFlags,
        /* eTrcDetailLevel             */ i_eTrcDetailLevel,
        /* eTrcDetailLevelNoisyMethods */ i_eTrcDetailLevelNoisyMethods );

} // CreateInstance

//------------------------------------------------------------------------------
CDlgIdxTreeLoggers* CDlgIdxTreeLoggers::GetInstance( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CDlgIdxTreeLoggers*>(CDialog::GetInstance(NameSpace(), ClassName(), i_strObjName));
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgIdxTreeLoggers::CDlgIdxTreeLoggers(
    const QString& i_strDlgTitle,
    CIdxTreeLoggers* i_pIdxTree,
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wFlags,
    EMethodTraceDetailLevel i_eTrcDetailLevel,
    EMethodTraceDetailLevel i_eTrcDetailLevelNoisyMethods ) :
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

    m_pWdgtIdxTree = new CWdgtIdxTreeLoggers(m_pIdxTree, nullptr);

    m_pLyt->addWidget(m_pWdgtIdxTree);
}

//------------------------------------------------------------------------------
CDlgIdxTreeLoggers::~CDlgIdxTreeLoggers()
//------------------------------------------------------------------------------
{
    m_pIdxTree = nullptr;
    m_pLyt = nullptr;
    m_pWdgtIdxTree = nullptr;
}
