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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qsettings.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#else
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#endif

#include "ZSSysGUI/ZSSysEditStringValueDlg.h"
#include "ZSSysGUI/ZSSysSepLine.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CDlgEditStringValue : public QDialog
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CDlgEditStringValue* CDlgEditStringValue::CreateInstance(
    const QString&  i_strDlgTitle,
    const QString&  i_strObjName,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wFlags )
//------------------------------------------------------------------------------
{
    if( CDialog::GetInstance(NameSpace(), ClassName(), i_strObjName) != nullptr )
    {
        QString strKey = buildPathStr("::", NameSpace(), ClassName(), i_strObjName);
        throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, strKey);
    }

    return new CDlgEditStringValue(
        /* strDlgTitle  */ i_strDlgTitle,
        /* strObjName   */ i_strObjName,
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags );

} // CreateInstance

//------------------------------------------------------------------------------
CDlgEditStringValue* CDlgEditStringValue::GetInstance( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CDlgEditStringValue*>(CDialog::GetInstance(NameSpace(), ClassName(), i_strObjName));
}

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgEditStringValue::CDlgEditStringValue(
    const QString&  i_strDlgTitle,
    const QString&  i_strObjName,
    QWidget*        i_pWdgtParent,
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
    m_pLytValue(nullptr),
    m_pLblValue(nullptr),
    m_pEdtValue(nullptr),
    m_strValOrig(),
    m_pLytDescription(nullptr),
    m_pLblDescription(nullptr),
    m_pLytBtns(nullptr),
    m_pBtnApply(nullptr),
    m_pBtnOk(nullptr),
    m_pBtnCancel(nullptr)
{
    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    m_pLytValue = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytValue);

    m_pLblValue = new QLabel("Value: ");
    m_pLytValue->addWidget(m_pLblValue);

    m_pEdtValue = new QLineEdit();
    m_pLytValue->addWidget(m_pEdtValue);
    m_pLytValue->addStretch();

    m_pLytDescription = new QVBoxLayout();
    m_pLyt->addLayout(m_pLytDescription);

    m_pLblDescription = new QLabel();
    m_pLblDescription->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_pLytDescription->addWidget(m_pLblDescription);
    m_pLblDescription->setVisible(false);

    m_pLyt->addWidget(new CSepLine(5, this));

    m_pLytBtns = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytBtns);

    m_pBtnApply = new QPushButton("Apply");
    m_pLytBtns->addWidget(m_pBtnApply);

    QObject::connect(
        m_pBtnApply, &QPushButton::clicked,
        this, &CDlgEditStringValue::onBtnApplyClicked);

    m_pBtnOk = new QPushButton("Ok");
    m_pLytBtns->addWidget(m_pBtnOk);

    QObject::connect(
        m_pBtnOk, &QPushButton::clicked,
        this, &CDlgEditStringValue::onBtnOkClicked);

    m_pBtnCancel = new QPushButton("Cancel");
    m_pLytBtns->addWidget(m_pBtnCancel);

    QObject::connect(
        m_pBtnCancel, &QPushButton::clicked,
        this, &CDlgEditStringValue::onBtnCancelClicked);

} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgEditStringValue::~CDlgEditStringValue()
//------------------------------------------------------------------------------
{
    m_pLyt = nullptr;
    m_pLytValue = nullptr;
    m_pLblValue = nullptr;
    m_pEdtValue = nullptr;
    //m_strValOrig;
    m_pLytDescription = nullptr;
    m_pLblDescription = nullptr;
    m_pLytBtns = nullptr;
    m_pBtnApply = nullptr;
    m_pBtnOk = nullptr;
    m_pBtnCancel = nullptr;

} // dtor

/*==============================================================================
public: // overridables of base class QDialog
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Opens the dialog as a modal dialg. The apply button will be hidden.

    When pressing either Ok or Cancel the dialog will be closed and either
    Rejected or Accepted is returned.

    @return 0: QDialog::Rejected
            1: QDialog::Accepted
*/
int CDlgEditStringValue::exec()
//------------------------------------------------------------------------------
{
    m_pBtnApply->hide();
    int iResult = QDialog::exec();
    m_pBtnApply->show();
    return iResult;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgEditStringValue::setValueName( const QString& i_strName )
//------------------------------------------------------------------------------
{
    m_pLblValue->setText(i_strName + ":");
}

//------------------------------------------------------------------------------
QString CDlgEditStringValue::getValueName() const
//------------------------------------------------------------------------------
{
    return m_pLblValue->text().remove(":");
}

//------------------------------------------------------------------------------
void CDlgEditStringValue::setValue( const QString& i_strVal )
//------------------------------------------------------------------------------
{
    m_strValOrig = i_strVal;
    m_pEdtValue->setText(m_strValOrig);
}

//------------------------------------------------------------------------------
QString CDlgEditStringValue::getValue() const
//------------------------------------------------------------------------------
{
    return m_pEdtValue->text();
}

//------------------------------------------------------------------------------
void CDlgEditStringValue::setDescription( const QString& i_strDescription )
//------------------------------------------------------------------------------
{
    m_pLblDescription->setText(i_strDescription);
    m_pLblDescription->setVisible(!i_strDescription.isEmpty());
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgEditStringValue::onBtnApplyClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    emit applied();
}

//------------------------------------------------------------------------------
void CDlgEditStringValue::onBtnOkClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( isModal() )
    {
        accept();
    }
    else
    {
        emit accepted();
    }
}

//------------------------------------------------------------------------------
void CDlgEditStringValue::onBtnCancelClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( isModal() )
    {
        reject();
    }
    else
    {
        emit rejected();
    }
}
