/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer, Germany
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
#include <QtGui/qgroupbox.h>
#include <QtGui/qlayout.h>
#include <QtGui/qsplitter.h>
#else
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qsplitter.h>
#endif

#include "WidgetCentral.h"
#include "App.h"
#include "Test.h"

#include "ZSTestGUI/ZSTestWdgt.h"
#include "ZSSysGUI/ZSSysErrLogWdgt.h"
#include "ZSSys/ZSSysErrCode.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Test;
using namespace ZS::Test::GUI;
using namespace ZS::Apps::Test::SysErrLog;


/*******************************************************************************
class CWidgetCentral : public QWidget
*******************************************************************************/

CWidgetCentral* CWidgetCentral::s_pThis = nullptr; // singleton class

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWidgetCentral::CWidgetCentral( QWidget* i_pWdgtParent, Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_strSettingsKey(),
    m_pLyt(nullptr),
    m_pSplitter(nullptr),
    m_pLytGrpTest(nullptr),
    m_pGrpTest(nullptr),
    m_pWdgtTest(nullptr),
    m_pLytGrpErrLog(nullptr),
    m_pGrpErrLog(nullptr),
    m_pWdgtErrLog(nullptr)
{
    if( s_pThis != nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultSingletonClassAlreadyInstantiated);
    }
    s_pThis = this;

    setObjectName("CentralWidget");

    m_strSettingsKey = objectName();

    //QPalette paletteWdgtCentral;
    //paletteWdgtCentral.setColor(QPalette::Window,Qt::lightGray);
    //setPalette(paletteWdgtCentral);
    //setAutoFillBackground(true);

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    m_pSplitter = new QSplitter(Qt::Vertical);
    m_pLyt->addWidget(m_pSplitter);

    // <GroupBox> Automated Test
    //-------------------------

    m_pGrpTest = new QGroupBox( "Automated Test" );
    m_pSplitter->addWidget(m_pGrpTest);
    m_pLytGrpTest = new QVBoxLayout();
    m_pGrpTest->setLayout(m_pLytGrpTest);

    m_pWdgtTest = new CWdgtIdxTreeTestSteps( CApplication::GetInstance()->getTest() );
    m_pLytGrpTest->addWidget(m_pWdgtTest);

    // <GroupBox> Error Log
    //-------------------------

    m_pGrpErrLog = new QGroupBox( "Errors" );
    m_pSplitter->addWidget(m_pGrpErrLog);
    m_pLytGrpErrLog = new QVBoxLayout();
    m_pGrpErrLog->setLayout(m_pLytGrpErrLog);

    m_pWdgtErrLog = new CWdgtErrLog();
    m_pLytGrpErrLog->addWidget(m_pWdgtErrLog);

    // <Geometry>
    //-----------------------------

    QSettings settings;

    QList<int> listSizes;
    int        idx;

    listSizes = m_pSplitter->sizes();

    for( idx = 0; idx < listSizes.count(); idx++ )
    {
        listSizes[idx] = settings.value( m_strSettingsKey + "/SplitterHeight" + QString::number(idx), 50 ).toInt();
    }
    m_pSplitter->setSizes(listSizes);

} // ctor

//------------------------------------------------------------------------------
CWidgetCentral::~CWidgetCentral()
//------------------------------------------------------------------------------
{
    QSettings settings;

    QList<int> listSizes;
    int        idx;

    listSizes = m_pSplitter->sizes();

    for( idx = 0; idx < listSizes.count(); idx++ )
    {
        settings.setValue( m_strSettingsKey + "/SplitterHeight" + QString::number(idx), listSizes[idx] );
    }

    m_pLyt = nullptr;
    m_pSplitter = nullptr;
    m_pLytGrpTest = nullptr;
    m_pGrpTest = nullptr;
    m_pWdgtTest = nullptr;
    m_pLytGrpErrLog = nullptr;
    m_pGrpErrLog = nullptr;
    m_pWdgtErrLog = nullptr;

} // dtor
