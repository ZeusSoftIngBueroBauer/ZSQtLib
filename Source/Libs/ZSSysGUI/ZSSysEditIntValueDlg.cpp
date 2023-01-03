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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qsettings.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qpushbutton.h>
#else
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#endif

#include "ZSSysGUI/ZSSysEditIntValueDlg.h"
#include "ZSSysGUI/ZSSysSepLine.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CDlgEditIntValue : public QDialog
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CDlgEditIntValue* CDlgEditIntValue::CreateInstance(
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

    return new CDlgEditIntValue(
        /* strDlgTitle  */ i_strDlgTitle,
        /* strObjName   */ i_strObjName,
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags );

} // CreateInstance

//------------------------------------------------------------------------------
CDlgEditIntValue* CDlgEditIntValue::GetInstance( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CDlgEditIntValue*>(CDialog::GetInstance(NameSpace(), ClassName(), i_strObjName));
}

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgEditIntValue::CDlgEditIntValue(
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
    m_iValOrig(0),
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

    m_pEdtValue = new QSpinBox();
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

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnApply,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnApplyClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pBtnOk = new QPushButton("Ok");
    m_pLytBtns->addWidget(m_pBtnOk);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnOk,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnOkClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pBtnCancel = new QPushButton("Cancel");
    m_pLytBtns->addWidget(m_pBtnCancel);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnCancel,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnCancelClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgEditIntValue::~CDlgEditIntValue()
//------------------------------------------------------------------------------
{
    m_pLyt = nullptr;
    m_pLytValue = nullptr;
    m_pLblValue = nullptr;
    m_pEdtValue = nullptr;
    m_iValOrig = 0;
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
int CDlgEditIntValue::exec()
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
void CDlgEditIntValue::setValueName( const QString& i_strName )
//------------------------------------------------------------------------------
{
    m_pLblValue->setText(i_strName + ":");
}

//------------------------------------------------------------------------------
QString CDlgEditIntValue::getValueName() const
//------------------------------------------------------------------------------
{
    return m_pLblValue->text().remove(":");
}

//------------------------------------------------------------------------------
void CDlgEditIntValue::setValue( int i_iVal )
//------------------------------------------------------------------------------
{
    m_iValOrig = i_iVal;
    m_pEdtValue->setValue(m_iValOrig);
}

//------------------------------------------------------------------------------
int CDlgEditIntValue::getValue() const
//------------------------------------------------------------------------------
{
    return m_pEdtValue->value();
}

//------------------------------------------------------------------------------
void CDlgEditIntValue::setUnit( const QString& i_strUnit )
//------------------------------------------------------------------------------
{
    m_pEdtValue->setSuffix(" " + i_strUnit);
}

//------------------------------------------------------------------------------
QString CDlgEditIntValue::getUnit() const
//------------------------------------------------------------------------------
{
    return m_pEdtValue->suffix().trimmed();
}

//------------------------------------------------------------------------------
void CDlgEditIntValue::setMinimum( int i_iMinimum )
//------------------------------------------------------------------------------
{
    m_pEdtValue->setMinimum(i_iMinimum);
}

//------------------------------------------------------------------------------
int CDlgEditIntValue::getMinimum() const
//------------------------------------------------------------------------------
{
    return m_pEdtValue->minimum();
}

//------------------------------------------------------------------------------
void CDlgEditIntValue::setMaximum( int i_iMaximum )
//------------------------------------------------------------------------------
{
    m_pEdtValue->setMaximum(i_iMaximum);
}

//------------------------------------------------------------------------------
int CDlgEditIntValue::getMaximum() const
//------------------------------------------------------------------------------
{
    return m_pEdtValue->maximum();
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
//------------------------------------------------------------------------------
void CDlgEditIntValue::setStepType( QAbstractSpinBox::StepType i_stepType )
//------------------------------------------------------------------------------
{
    m_pEdtValue->setStepType(i_stepType);
}

//------------------------------------------------------------------------------
QAbstractSpinBox::StepType CDlgEditIntValue::getStepType() const
//------------------------------------------------------------------------------
{
    return m_pEdtValue->stepType();
}
#endif // #if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)

//------------------------------------------------------------------------------
void CDlgEditIntValue::setDescription( const QString& i_strDescription )
//------------------------------------------------------------------------------
{
    m_pLblDescription->setText(i_strDescription);
    m_pLblDescription->setVisible(!i_strDescription.isEmpty());
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgEditIntValue::onBtnApplyClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    emit applied();
}

//------------------------------------------------------------------------------
void CDlgEditIntValue::onBtnOkClicked( bool /*i_bChecked*/ )
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
void CDlgEditIntValue::onBtnCancelClicked( bool /*i_bChecked*/ )
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
