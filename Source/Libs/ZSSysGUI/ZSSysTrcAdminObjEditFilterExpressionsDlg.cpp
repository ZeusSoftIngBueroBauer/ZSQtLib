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

#include "ZSSysGUI/ZSSysTrcAdminObjEditFilterExpressionsDlg.h"
#include "ZSSysGUI/ZSSysTrcAdminObjEditFilterExpressionsWdgt.h"
#include "ZSSysGUI/ZSSysSepLine.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysException.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qlayout.h>
#include <QtGui/qpushbutton.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CDlgTrcAdminObjEditFilterExpressions : public CDialog
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CDlgTrcAdminObjEditFilterExpressions* CDlgTrcAdminObjEditFilterExpressions::CreateInstance(
    const QString& i_strDlgTitle,
    const QString& i_strObjName,
    CIdxTreeTrcAdminObjs* i_pIdxTree,
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wFlags )
//------------------------------------------------------------------------------
{
    if (CDialog::GetInstance(NameSpace(), ClassName(), i_strObjName) != nullptr) {
        QString strKey = buildPathStr("::", NameSpace(), ClassName(), i_strObjName);
        throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, strKey);
    }
    return new CDlgTrcAdminObjEditFilterExpressions(
        /* strDlgTitle */ i_strDlgTitle,
        /* strObjName  */ i_strObjName,
        /* pIdxTree    */ i_pIdxTree,
        /* pWdgtParent */ i_pWdgtParent,
        /* wFlags      */ i_wFlags );
}

//------------------------------------------------------------------------------
CDlgTrcAdminObjEditFilterExpressions* CDlgTrcAdminObjEditFilterExpressions::GetInstance( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CDlgTrcAdminObjEditFilterExpressions*>(CDialog::GetInstance(NameSpace(), ClassName(), i_strObjName));
}

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgTrcAdminObjEditFilterExpressions::CDlgTrcAdminObjEditFilterExpressions(
    const QString& i_strDlgTitle,
    const QString& i_strObjName,
    CIdxTreeTrcAdminObjs* i_pIdxTree,
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wFlags ) :
//------------------------------------------------------------------------------
    CDialog(
        /* strDlgTitle  */ i_strDlgTitle,
        /* strNameSpace */ NameSpace(),
        /* strClassName */ ClassName(),
        /* strObjName   */ i_strObjName,
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags ),
    m_pIdxTree(i_pIdxTree),
    m_strKeyInTree(),
    m_eFilter(EMethodTraceFilterProperty::Undefined),
    m_pLyt(nullptr),
    m_pWdgtEditFilterExpressions(nullptr),
    m_pLytLineBtns(nullptr),
    m_pBtnApply(nullptr),
    m_pBtnOk(nullptr),
    m_pBtnCancel(nullptr)
{
    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    m_pWdgtEditFilterExpressions = new CWdgtTrcAdminObjEditFilterExpressions(i_pIdxTree);
    m_pLyt->addWidget(m_pWdgtEditFilterExpressions);
    QObject::connect(
        m_pWdgtEditFilterExpressions, &CWdgtTrcAdminObjEditFilterExpressions::contentChanged,
        this, &CDlgTrcAdminObjEditFilterExpressions::onWdgtEditFilterExpressionsContentChanged);

    m_pLyt->addWidget(new CSepLine(5, this));

    m_pLytLineBtns = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytLineBtns);

    m_pBtnOk = new QPushButton("Ok");
    m_pLytLineBtns->addWidget(m_pBtnOk);
    QObject::connect(
        m_pBtnOk, &QPushButton::clicked,
        this, &CDlgTrcAdminObjEditFilterExpressions::onBtnOkClicked);

    m_pBtnApply = new QPushButton("Apply");
    m_pLytLineBtns->addWidget(m_pBtnApply);
    m_pBtnApply->setEnabled(false);
    QObject::connect(
        m_pBtnApply, &QPushButton::clicked,
        this, &CDlgTrcAdminObjEditFilterExpressions::onBtnApplyClicked);

    m_pBtnReset = new QPushButton("Reset");
    m_pLytLineBtns->addWidget(m_pBtnReset);
    m_pBtnReset->setEnabled(false);
    QObject::connect(
        m_pBtnReset, &QPushButton::clicked,
        this, &CDlgTrcAdminObjEditFilterExpressions::onBtnResetClicked);

    m_pBtnCancel = new QPushButton("Cancel");
    m_pLytLineBtns->addWidget(m_pBtnCancel);
    QObject::connect(
        m_pBtnCancel, &QPushButton::clicked,
        this, &CDlgTrcAdminObjEditFilterExpressions::onBtnCancelClicked);

} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgTrcAdminObjEditFilterExpressions::~CDlgTrcAdminObjEditFilterExpressions()
//------------------------------------------------------------------------------
{
    m_pIdxTree = nullptr;
    //m_strKeyInTree;
    m_eFilter = static_cast<EMethodTraceFilterProperty>(0);
    m_pLyt = nullptr;
    m_pWdgtEditFilterExpressions = nullptr;
    m_pLytLineBtns = nullptr;
    m_pBtnApply = nullptr;
    m_pBtnOk = nullptr;
    m_pBtnCancel = nullptr;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgTrcAdminObjEditFilterExpressions::setKeyEntryToEdit(
    const QString& i_strKeyInTree, EMethodTraceFilterProperty i_filter)
//------------------------------------------------------------------------------
{
    if (m_strKeyInTree != i_strKeyInTree || m_eFilter != i_filter) {
        m_strKeyInTree = i_strKeyInTree;
        m_eFilter = i_filter;
        m_pWdgtEditFilterExpressions->setKeyEntryToEdit(i_strKeyInTree, i_filter);
        onWdgtEditFilterExpressionsContentChanged();
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgTrcAdminObjEditFilterExpressions::onBtnOkClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    m_pWdgtEditFilterExpressions->acceptChanges();
    QDialog::accept();
}

//------------------------------------------------------------------------------
void CDlgTrcAdminObjEditFilterExpressions::onBtnApplyClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    m_pWdgtEditFilterExpressions->acceptChanges();

    // After accepting changes there should be no changes anymore.
    // Code just added for the sake of clarification.
    onWdgtEditFilterExpressionsContentChanged();
}

//------------------------------------------------------------------------------
void CDlgTrcAdminObjEditFilterExpressions::onBtnResetClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    m_pWdgtEditFilterExpressions->rejectChanges();

    // After rejecting changes there should be no changes anymore.
    // Code just added for the sake of clarification.
    onWdgtEditFilterExpressionsContentChanged();
}

//------------------------------------------------------------------------------
void CDlgTrcAdminObjEditFilterExpressions::onBtnCancelClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    m_pWdgtEditFilterExpressions->rejectChanges();
    QDialog::reject();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgTrcAdminObjEditFilterExpressions::onWdgtEditFilterExpressionsContentChanged()
//------------------------------------------------------------------------------
{
    bool bHasChanges = m_pWdgtEditFilterExpressions->hasChanges();
    m_pBtnApply->setEnabled(bHasChanges);
    m_pBtnReset->setEnabled(bHasChanges);
}
