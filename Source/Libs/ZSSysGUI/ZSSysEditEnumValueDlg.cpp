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
#include <QtGui/qcombobox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qpushbutton.h>
#else
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#endif

#include "ZSSysGUI/ZSSysEditEnumValueDlg.h"
#include "ZSSysGUI/ZSSysSepLine.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CDlgEditEnumValue : public QDialog
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CDlgEditEnumValue* CDlgEditEnumValue::CreateInstance(
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

    return new CDlgEditEnumValue(
        /* strDlgTitle  */ i_strDlgTitle,
        /* strObjName   */ i_strObjName,
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags );

} // CreateInstance

//------------------------------------------------------------------------------
CDlgEditEnumValue* CDlgEditEnumValue::GetInstance( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CDlgEditEnumValue*>(CDialog::GetInstance(NameSpace(), ClassName(), i_strObjName));
}

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgEditEnumValue::CDlgEditEnumValue(
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
    m_arEnumEntries(),
    m_pCmbValue(nullptr),
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

    m_pCmbValue = new QComboBox();
    m_pLytValue->addWidget(m_pCmbValue);
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
CDlgEditEnumValue::~CDlgEditEnumValue()
//------------------------------------------------------------------------------
{
    m_pLyt = nullptr;
    m_pLytValue = nullptr;
    m_pLblValue = nullptr;
    m_pCmbValue = nullptr;
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
int CDlgEditEnumValue::exec()
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
void CDlgEditEnumValue::setValueName( const QString& i_strName )
//------------------------------------------------------------------------------
{
    m_pLblValue->setText(i_strName + ":");
}

//------------------------------------------------------------------------------
QString CDlgEditEnumValue::getValueName() const
//------------------------------------------------------------------------------
{
    return m_pLblValue->text().remove(":");
}

//------------------------------------------------------------------------------
void CDlgEditEnumValue::setComboItems( const QVector<SEnumEntry>& i_arEnumEntries, int i_idxAliasToBeIndicated )
//------------------------------------------------------------------------------
{
    m_arEnumEntries = i_arEnumEntries;

    m_pCmbValue->clear();

    for( const auto& enumEntry : i_arEnumEntries )
    {
        if( enumEntry.m_val.isValid() )
        {
            m_pCmbValue->addItem(enumEntry.getName(i_idxAliasToBeIndicated), enumEntry.m_val);
        }
        else
        {
            m_pCmbValue->addItem(enumEntry.getName(i_idxAliasToBeIndicated));
        }
    }
}

//------------------------------------------------------------------------------
void CDlgEditEnumValue::setEnumerator( int i_iEnumerator )
//------------------------------------------------------------------------------
{
    m_iValOrig = i_iEnumerator;
    m_pCmbValue->setCurrentIndex(i_iEnumerator);
}

//------------------------------------------------------------------------------
void CDlgEditEnumValue::setEnumerator( const QString& i_strEnumerator )
//------------------------------------------------------------------------------
{
    int idxCmb = m_pCmbValue->findText(i_strEnumerator);
    if( idxCmb >= 0 )
    {
        m_pCmbValue->setCurrentIndex(idxCmb);
    }
}

//------------------------------------------------------------------------------
int CDlgEditEnumValue::getEnumerator() const
//------------------------------------------------------------------------------
{
    return m_pCmbValue->currentIndex();
}

//------------------------------------------------------------------------------
QString CDlgEditEnumValue::getEnumeratorName( int i_idxAlias ) const
//------------------------------------------------------------------------------
{
    int idxCmb = m_pCmbValue->currentIndex();
    const SEnumEntry& enumEntry = m_arEnumEntries[idxCmb];
    return enumEntry.getName(idxCmb);
}

//------------------------------------------------------------------------------
void CDlgEditEnumValue::setDescription( const QString& i_strDescription )
//------------------------------------------------------------------------------
{
    m_pLblDescription->setText(i_strDescription);
    m_pLblDescription->setVisible(!i_strDescription.isEmpty());
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgEditEnumValue::onBtnApplyClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    emit applied();
}

//------------------------------------------------------------------------------
void CDlgEditEnumValue::onBtnOkClicked( bool /*i_bChecked*/ )
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
void CDlgEditEnumValue::onBtnCancelClicked( bool /*i_bChecked*/ )
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
