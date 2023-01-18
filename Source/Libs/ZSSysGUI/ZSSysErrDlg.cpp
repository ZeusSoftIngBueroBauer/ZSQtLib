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
#include <QtCore/qdatetime.h>
#include <QtCore/qsettings.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qdialogbuttonbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qpushbutton.h>
#else
#include <QtWidgets/qdialogbuttonbox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#endif

#include "ZSSysGUI/ZSSysErrDlg.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CDlgErr : public QDialog
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgErr::CDlgErr( const QString& i_strDlgTitle, QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QDialog(i_pWdgtParent),
    m_errResultInfo(),
    m_pLyt(nullptr),
    // Upper part devided into two columns to increase the size of the severity icon.
    m_pLytUpperPart(nullptr),
    m_pLytUpperLeftPart(nullptr),
    m_pLytLineSeverity(nullptr),
    m_pLblSeverity(nullptr),
    m_pEdtSeverity(nullptr),
    m_pLytLineDate(nullptr),
    m_pLblDate(nullptr),
    m_pEdtDate(nullptr),
    m_pLytLineTime(nullptr),
    m_pLblTime(nullptr),
    m_pEdtTime(nullptr),
    m_pLytUpperRightPart(nullptr),
    m_pLblSeverityIcon(nullptr),
    // Lower part with one column.
    m_pLytLineResult(nullptr),
    m_pLblResult(nullptr),
    m_pEdtResult(nullptr),
    m_pLytLineSource(nullptr),
    m_pLblSource(nullptr),
    m_pEdtSource(nullptr),
    m_pLytLineAddInfo(nullptr),
    m_pLblAddInfo(nullptr),
    m_pEdtAddInfo(nullptr),
    // Ok button
    m_pBtnOk(nullptr),
    m_pBtnBox(nullptr)
{
    setObjectName("DlgErr");

    if( i_strDlgTitle.isEmpty() )
    {
        setWindowTitle( QCoreApplication::applicationName() + ": Error Log" );
    }
    else
    {
        setWindowTitle(i_strDlgTitle);
    }

    int cxLblWidth = 120;

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    // <Upper Part> Devided into two columns to increase the size of the severity icon.
    //---------------------------------------------------------------------------------

    m_pLytUpperPart = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytUpperPart);

    // <Left Upper Part>
    //------------------

    m_pLytUpperLeftPart = new QVBoxLayout();
    m_pLytUpperPart->addLayout(m_pLytUpperLeftPart);

    // <Line> Severity
    //----------------

    m_pLytLineSeverity = new QHBoxLayout();
    m_pLytUpperLeftPart->addLayout(m_pLytLineSeverity);
    m_pLblSeverity = new QLabel("Severity:");
    m_pLblSeverity->setFixedWidth(cxLblWidth);
    m_pLytLineSeverity->addWidget(m_pLblSeverity);
    m_pEdtSeverity = new QLabel();
    m_pEdtSeverity->setFixedWidth(cxLblWidth);
    m_pLytLineSeverity->addWidget(m_pEdtSeverity);
    m_pLytLineSeverity->addStretch();

    // <Line> Date
    //------------

    m_pLytLineDate = new QHBoxLayout();
    m_pLytUpperLeftPart->addLayout(m_pLytLineDate);
    m_pLblDate = new QLabel("Date:");
    m_pLblDate->setFixedWidth(cxLblWidth);
    m_pLytLineDate->addWidget(m_pLblDate);
    m_pEdtDate = new QLabel( QDate::currentDate().toString("yyyy-MM-dd") );
    m_pLytLineDate->addWidget(m_pEdtDate);
    m_pLytLineDate->addStretch();

    // <Line> Time
    //------------

    m_pLytLineTime = new QHBoxLayout();
    m_pLytUpperLeftPart->addLayout(m_pLytLineTime);
    m_pLblTime = new QLabel("Time:");
    m_pLblTime->setFixedWidth(cxLblWidth);
    m_pLytLineTime->addWidget(m_pLblTime);
    m_pEdtTime = new QLabel( QTime::currentTime().toString("hh:mm:ss") );
    m_pLytLineTime->addWidget(m_pEdtTime);
    m_pLytLineTime->addStretch();

    // <Right Upper Part>
    //-------------------

    m_pLytUpperRightPart = new QVBoxLayout();
    m_pLytUpperPart->addLayout(m_pLytUpperRightPart);

    // <Icon> Result Severity
    //-----------------------

    m_pLblSeverityIcon = new QLabel();
    m_pLblSeverityIcon->setMinimumHeight(32);
    m_pLblSeverityIcon->setMinimumWidth(32);
    m_pLblSeverityIcon->setScaledContents(true);
    m_pLblSeverityIcon->setPixmap( getErrPixmap(EResultSeverityInfo) );
    m_pLytUpperRightPart->addWidget(m_pLblSeverityIcon, 0, Qt::AlignVCenter);

    // <Lower Part> with one column
    //-----------------------------

    // <Line> Result
    //--------------

    m_pLytLineResult = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytLineResult);
    m_pLblResult = new QLabel("Result:");
    m_pLblResult->setFixedWidth(cxLblWidth);
    m_pLytLineResult->addWidget(m_pLblResult);
    m_pEdtResult = new QLabel();
    m_pLytLineResult->addWidget(m_pEdtResult);
    m_pLytLineResult->addStretch();

    // <Line> Source
    //--------------

    m_pLytLineSource = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytLineSource);
    m_pLblSource = new QLabel("Source:");
    m_pLblSource->setFixedWidth(cxLblWidth);
    m_pLytLineSource->addWidget(m_pLblSource);
    m_pEdtSource = new QLabel();
    m_pLytLineSource->addWidget(m_pEdtSource);
    m_pLytLineSource->addStretch();

    // <Line> AddErrInfo
    //------------------

    m_pLytLineAddInfo = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytLineAddInfo);
    m_pLblAddInfo = new QLabel("Additional Info:");
    m_pLblAddInfo->setFixedWidth(cxLblWidth);
    m_pLytLineAddInfo->addWidget(m_pLblAddInfo);
    m_pEdtAddInfo = new QLabel();
    m_pLytLineAddInfo->addWidget(m_pEdtAddInfo);
    m_pLytLineAddInfo->addStretch();

    // <DialogButtonBox>
    //------------------

    m_pLyt->addSpacing(10);

    m_pBtnBox = new QDialogButtonBox();
    m_pBtnOk = m_pBtnBox->addButton(QDialogButtonBox::Ok);
    m_pLyt->addWidget(m_pBtnBox);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnOk,
        /* szSignal     */ SIGNAL(clicked()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(accept()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CDlgErr::~CDlgErr()
//------------------------------------------------------------------------------
{
    QSettings settings;

    settings.setValue( windowTitle()+"/Geometry", saveGeometry() );

    //m_errResultInfo;
    m_pLyt = nullptr;
    // Upper part devided into two columns to increase the size of the severity icon.
    m_pLytUpperPart = nullptr;
    m_pLytUpperLeftPart = nullptr;
    m_pLytLineSeverity = nullptr;
    m_pLblSeverity = nullptr;
    m_pEdtSeverity = nullptr;
    m_pLytLineDate = nullptr;
    m_pLblDate = nullptr;
    m_pEdtDate = nullptr;
    m_pLytLineTime = nullptr;
    m_pLblTime = nullptr;
    m_pEdtTime = nullptr;
    m_pLytUpperRightPart = nullptr;
    m_pLblSeverityIcon = nullptr;
    // Lower part with one column.
    m_pLytLineResult = nullptr;
    m_pLblResult = nullptr;
    m_pEdtResult = nullptr;
    m_pLytLineSource = nullptr;
    m_pLblSource = nullptr;
    m_pEdtSource = nullptr;
    m_pLytLineAddInfo = nullptr;
    m_pLblAddInfo = nullptr;
    m_pEdtAddInfo = nullptr;
    // Ok button
    m_pBtnOk = nullptr;
    m_pBtnBox = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgErr::setErrResultInfo( const SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    m_errResultInfo = i_errResultInfo;

    m_pEdtSeverity->setText( resultSeverity2Str(i_errResultInfo.getSeverity()) );
    m_pLblSeverityIcon->setPixmap( getErrPixmap(i_errResultInfo.getSeverity()) );
    m_pEdtDate->setText( QDate::currentDate().toString("yyyy-MM-dd") );
    m_pEdtTime->setText( QTime::currentTime().toString("hh:mm:ss") );
    m_pEdtResult->setText( i_errResultInfo.getResultStr() );
    m_pEdtSource->setText( i_errResultInfo.getErrSource().toString() );
    m_pEdtAddInfo->setText( i_errResultInfo.getAddErrInfoDscr() );

} // setErrResultInfo

/*==============================================================================
protected: // overridable slots of base class QDialog
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgErr::accept()
//------------------------------------------------------------------------------
{
    setErrResultInfo(SErrResultInfo());

    QDialog::accept();

} // accept

//------------------------------------------------------------------------------
void CDlgErr::reject()
//------------------------------------------------------------------------------
{
    setErrResultInfo(SErrResultInfo());

    QDialog::reject();

} // reject
