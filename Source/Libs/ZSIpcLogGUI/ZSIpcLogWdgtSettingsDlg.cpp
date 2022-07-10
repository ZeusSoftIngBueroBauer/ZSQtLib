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
#include <QtGui/qcheckbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qpushbutton.h>
#else
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#endif

#include "ZSIpcLogGUI/ZSIpcLogWdgtSettingsDlg.h"
#include "ZSIpcLogGUI/ZSIpcLogWdgt.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSysGUI/ZSSysSepLine.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Log;
using namespace ZS::Log::GUI;


/*******************************************************************************
class CDlgWdgtLogSettings : public ZS::System::GUI::CDialog
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CDlgWdgtLogSettings* CDlgWdgtLogSettings::CreateInstance(
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

    return new CDlgWdgtLogSettings(
        /* strDlgTitle  */ i_strDlgTitle,
        /* strObjName   */ i_strObjName,
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags );

} // CreateInstance

//------------------------------------------------------------------------------
CDlgWdgtLogSettings* CDlgWdgtLogSettings::GetInstance( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CDlgWdgtLogSettings*>(CDialog::GetInstance(NameSpace(), ClassName(), i_strObjName));
}

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgWdgtLogSettings::CDlgWdgtLogSettings(
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
    m_pWdgtLog(nullptr),
    m_pLyt(nullptr),
    m_pLytLineShowTimeInfo(nullptr),
    m_pLblShowTimeInfo(nullptr),
    m_pChkShowTimeInfo(nullptr),
    m_pLytBtns(nullptr),
    m_pBtnApply(nullptr),
    m_pBtnOk(nullptr),
    m_pBtnCancel(nullptr)
{
    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    m_pLytLineShowTimeInfo = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytLineShowTimeInfo);

    m_pLblShowTimeInfo = new QLabel("Show Time Info: ");
    m_pLytLineShowTimeInfo->addWidget(m_pLblShowTimeInfo);

    m_pChkShowTimeInfo = new QCheckBox();
    m_pChkShowTimeInfo->setEnabled(false);
    m_pLytLineShowTimeInfo->addWidget(m_pChkShowTimeInfo);
    m_pLytLineShowTimeInfo->addStretch();

    if( !QObject::connect(
        /* pObjSender   */ m_pChkShowTimeInfo,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkShowTimeInfoToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pLyt->addWidget(new CSepLine(5, this));

    m_pLytBtns = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytBtns);

    m_pBtnApply = new QPushButton("Apply");
    m_pBtnApply->setEnabled(false);
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

    if( !QObject::connect(
        /* pObjSender   */ this,
        /* szSignal     */ SIGNAL(accepted()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(hide()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ this,
        /* szSignal     */ SIGNAL(rejected()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(hide()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
} // ctor

//------------------------------------------------------------------------------
CDlgWdgtLogSettings::~CDlgWdgtLogSettings()
//------------------------------------------------------------------------------
{
    m_pWdgtLog = nullptr;
    m_pLyt = nullptr;
    m_pLytLineShowTimeInfo = nullptr;
    m_pLblShowTimeInfo = nullptr;
    m_pChkShowTimeInfo = nullptr;
    m_pLytBtns = nullptr;
    m_pBtnApply = nullptr;
    m_pBtnOk = nullptr;
    m_pBtnCancel = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgWdgtLogSettings::setLogWidget( CWdgtLog* i_pWdgtLog )
//------------------------------------------------------------------------------
{
    if( m_pWdgtLog != i_pWdgtLog )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pChkShowTimeInfo,
            /* szSignal     */ SIGNAL(toggled(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onChkShowTimeInfoToggled(bool)) );

        m_pWdgtLog = i_pWdgtLog;

        if( m_pWdgtLog != nullptr )
        {
            m_pChkShowTimeInfo->setChecked(m_pWdgtLog->getShowTimeInfo());
            m_pChkShowTimeInfo->setEnabled(true);
        }
        else
        {
            m_pChkShowTimeInfo->setChecked(false);
            m_pChkShowTimeInfo->setEnabled(false);
        }

        if( hasChanges() )
        {
            m_pBtnApply->setEnabled(true);
        }
        else
        {
            m_pBtnApply->setEnabled(false);
        }

        if( !QObject::connect(
            /* pObjSender   */ m_pChkShowTimeInfo,
            /* szSignal     */ SIGNAL(toggled(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onChkShowTimeInfoToggled(bool)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    }
} // setTraceMethodListWidget

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgWdgtLogSettings::onChkShowTimeInfoToggled( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( hasChanges() )
    {
        m_pBtnApply->setEnabled(true);
    }
    else
    {
        m_pBtnApply->setEnabled(false);
    }
}

//------------------------------------------------------------------------------
void CDlgWdgtLogSettings::onBtnApplyClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    applyChanges();
}

//------------------------------------------------------------------------------
void CDlgWdgtLogSettings::onBtnOkClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    applyChanges();

    emit accepted();
}

//------------------------------------------------------------------------------
void CDlgWdgtLogSettings::onBtnCancelClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    emit rejected();
}

/*==============================================================================
protected: // auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CDlgWdgtLogSettings::hasChanges() const
//------------------------------------------------------------------------------
{
    bool bHasChanges = false;

    if( m_pWdgtLog != nullptr )
    {
        if( m_pWdgtLog->getShowTimeInfo() != m_pChkShowTimeInfo->isChecked() )
        {
            bHasChanges = true;
        }
    }
    return bHasChanges;
}

//------------------------------------------------------------------------------
void CDlgWdgtLogSettings::applyChanges()
//------------------------------------------------------------------------------
{
    if( m_pWdgtLog != nullptr )
    {
         m_pWdgtLog->setShowTimeInfo(m_pChkShowTimeInfo->isChecked());
    }
    m_pBtnApply->setEnabled(false);
}
