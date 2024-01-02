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

#include "ZSSysGUI/ZSSysEditFilterExpressionsDlg.h"
#include "ZSSysGUI/ZSSysSepLine.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysException.h"

//#include <QtCore/qcoreapplication.h>
//#include <QtCore/qsettings.h>

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
class CDlgEditFilterExpressions : public QDialog
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CDlgEditFilterExpressions* CDlgEditFilterExpressions::CreateInstance(
    const QString& i_strDlgTitle,
    const QString& i_strObjName,
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wFlags )
//------------------------------------------------------------------------------
{
    if (CDialog::GetInstance(NameSpace(), ClassName(), i_strObjName) != nullptr) {
        QString strKey = buildPathStr("::", NameSpace(), ClassName(), i_strObjName);
        throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, strKey);
    }
    return new CDlgEditFilterExpressions(
        /* strDlgTitle */ i_strDlgTitle,
        /* strObjName  */ i_strObjName,
        /* pWdgtParent */ i_pWdgtParent,
        /* wFlags      */ i_wFlags );
}

//------------------------------------------------------------------------------
CDlgEditFilterExpressions* CDlgEditFilterExpressions::GetInstance( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CDlgEditFilterExpressions*>(CDialog::GetInstance(NameSpace(), ClassName(), i_strObjName));
}

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgEditFilterExpressions::CDlgEditFilterExpressions(
    const QString& i_strDlgTitle,
    const QString& i_strObjName,
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
    m_pLyt(nullptr),
    m_pLytBtns(nullptr),
    m_pBtnApply(nullptr),
    m_pBtnOk(nullptr),
    m_pBtnCancel(nullptr)
{
    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    m_pLyt->addWidget(new CSepLine(5, this));

    m_pLytBtns = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytBtns);

    m_pBtnApply = new QPushButton("Apply");
    m_pLytBtns->addWidget(m_pBtnApply);
    QObject::connect(
        m_pBtnApply, &QPushButton::clicked,
        this, &CDlgEditFilterExpressions::onBtnApplyClicked);

    m_pBtnOk = new QPushButton("Ok");
    m_pLytBtns->addWidget(m_pBtnOk);
    QObject::connect(
        m_pBtnOk, &QPushButton::clicked,
        this, &CDlgEditFilterExpressions::onBtnOkClicked);

    m_pBtnCancel = new QPushButton("Cancel");
    m_pLytBtns->addWidget(m_pBtnCancel);
    QObject::connect(
        m_pBtnCancel, &QPushButton::clicked,
        this, &CDlgEditFilterExpressions::onBtnCancelClicked);

} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgEditFilterExpressions::~CDlgEditFilterExpressions()
//------------------------------------------------------------------------------
{
    m_pLyt = nullptr;
    m_pLytBtns = nullptr;
    m_pBtnApply = nullptr;
    m_pBtnOk = nullptr;
    m_pBtnCancel = nullptr;

} // dtor

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgEditFilterExpressions::onBtnApplyClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    //emit applied();
}

//------------------------------------------------------------------------------
void CDlgEditFilterExpressions::onBtnOkClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if (isModal()) {
        accept();
    }
    else {
        emit accepted();
    }
}

//------------------------------------------------------------------------------
void CDlgEditFilterExpressions::onBtnCancelClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if (isModal()) {
        reject();
    }
    else {
        emit rejected();
    }
}
