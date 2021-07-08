/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#include "ZSSysGUI/ZSSysTrcAdminObjIdxTreeDlg.h"
#include "ZSSysGUI/ZSSysTrcAdminObjIdxTreeWdgt.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObjIdxTree.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Trace::GUI;


/*******************************************************************************
class CDlgIdxTreeTrcAdminObjs : public QDialog
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CDlgIdxTreeTrcAdminObjs* CDlgIdxTreeTrcAdminObjs::CreateInstance(
    CIdxTreeTrcAdminObjs* i_pIdxTree,
    const QString&        i_strDlgTitle,
    QWidget*              i_pWdgtParent,
    Qt::WindowFlags       i_wFlags,
    int                   i_iTrcDetailLevel )
//------------------------------------------------------------------------------
{
    if( CDialog::GetInstance(NameSpace(), ClassName(), i_pIdxTree->objectName()) != nullptr )
    {
        QString strKey = buildPathStr("::", NameSpace(), ClassName(), i_pIdxTree->objectName());
        throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, strKey);
    }

    // The ctor of base class CDialog adds the instance to the hash of dialogs.
    return new CDlgIdxTreeTrcAdminObjs(
        /* pIdxTree        */ i_pIdxTree,
        /* strDlgTitle     */ i_strDlgTitle,
        /* pWdgtParent     */ i_pWdgtParent,
        /* wFlags          */ i_wFlags,
        /* iTrcDetailLevel */ i_iTrcDetailLevel );

} // CreateInstance

//------------------------------------------------------------------------------
CDlgIdxTreeTrcAdminObjs* CDlgIdxTreeTrcAdminObjs::GetInstance( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CDlgIdxTreeTrcAdminObjs*>(CDialog::GetInstance(NameSpace(), ClassName(), i_strObjName));
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgIdxTreeTrcAdminObjs::CDlgIdxTreeTrcAdminObjs(
    CIdxTreeTrcAdminObjs* i_pIdxTree,
    const QString&        i_strDlgTitle,
    QWidget*              i_pWdgtParent,
    Qt::WindowFlags       i_wFlags,
    int                   i_iTrcDetailLevel ) :
//------------------------------------------------------------------------------
    CDialog(
        /* strNameSpace */ NameSpace(),
        /* strClassName */ ClassName(),
        /* strObjName   */ i_pIdxTree->objectName(),
        /* strDlgTitle  */ i_strDlgTitle,
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags ),
    m_pIdxTree(i_pIdxTree),
    m_pLyt(nullptr),
    m_pWdgtIdxTree(nullptr),
    m_iTrcDetailLevel(i_iTrcDetailLevel)
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    m_pWdgtIdxTree = new CWdgtIdxTreeTrcAdminObjs(m_pIdxTree, nullptr, i_iTrcDetailLevel);

    m_pLyt->addWidget(m_pWdgtIdxTree);

} // ctor

//------------------------------------------------------------------------------
CDlgIdxTreeTrcAdminObjs::~CDlgIdxTreeTrcAdminObjs()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ strMthInArgs );

    m_pIdxTree = nullptr;
    m_pLyt = nullptr;
    m_pWdgtIdxTree = nullptr;
    m_iTrcDetailLevel = 0;

} // dtor
