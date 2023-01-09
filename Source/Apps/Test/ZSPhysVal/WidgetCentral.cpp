/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer, Germany
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

#include "WidgetCentral.h"
#include "App.h"
#include "PhysSizes/PhysSizes.h"
#include "Test.h"

#include "ZSPhysValGUI/ZSPhysSizesIdxTreeWdgt.h"
#include "ZSPhysVal/ZSPhysSizesIdxTree.h"
#include "ZSTestGUI/ZSTestStepIdxTreeWdgt.h"
#include "ZSSys/ZSSysErrCode.h"
#include "ZSSys/ZSSysException.h"

#include <QtCore/qsettings.h>

#if QT_VERSION < 0x050000
//#include <QtGui/qgroupbox.h>
#include <QtGui/qlayout.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qtabwidget.h>
#else
//#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qtabwidget.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
//using namespace ZS::System::GUI;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;
using namespace ZS::Test;
using namespace ZS::Test::GUI;
using namespace ZS::Apps::Test::PhysVal;


/*******************************************************************************
class CWidgetCentral : public QWidget
*******************************************************************************/

CWidgetCentral* CWidgetCentral::s_pThis = nullptr; // singleton class

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWidgetCentral::CWidgetCentral( QWidget* i_pWdgtParent, Qt::WindowFlags  i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_pLyt(nullptr),
    m_pSplitter(nullptr),
    m_pTabWdgtMain(nullptr),
    m_pWdgtPhysSizes(nullptr),
    m_pWdgtTest(nullptr)
{
    if( s_pThis != nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultSingletonClassAlreadyInstantiated);
    }
    s_pThis = this;

    setObjectName("CentralWidget");

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    m_pSplitter = new QSplitter(Qt::Vertical);
    m_pLyt->addWidget(m_pSplitter);

    // <TabWidget>
    //============

    m_pTabWdgtMain = new QTabWidget(this);
    m_pSplitter->addWidget(m_pTabWdgtMain);

    // <Tab> Units
    //-------------

    m_pWdgtPhysSizes = new CWdgtIdxTreePhysSizes(&IdxTreePhysSizes);
    m_pTabWdgtMain->addTab(m_pWdgtPhysSizes,"Units");

    // <Tab> Test
    //-------------

    m_pWdgtTest = new CWdgtIdxTreeTestSteps( CApplication::GetInstance()->getTest() );
    m_pTabWdgtMain->addTab( m_pWdgtTest, "Test" );

    // <Geometry>
    //===================

    QSettings settings;

    restoreGeometry( settings.value(objectName()+"/Geometry").toByteArray() );

    QList<int> listSizes = m_pSplitter->sizes();
    for( int idx = 0; idx < listSizes.count(); idx++ )
    {
        listSizes[idx] = settings.value( objectName() + "/SplitterHeight" + QString::number(idx), 50 ).toInt();
    }
    m_pSplitter->setSizes(listSizes);

    setMinimumWidth(600);
    setMinimumHeight(300);

} // ctor

//------------------------------------------------------------------------------
CWidgetCentral::~CWidgetCentral()
//------------------------------------------------------------------------------
{
    QSettings settings;

    QList<int> listSizes = m_pSplitter->sizes();
    for( int idx = 0; idx < listSizes.count(); idx++ )
    {
        settings.setValue( objectName() + "/SplitterHeight" + QString::number(idx), listSizes[idx] );
    }

    m_pLyt = nullptr;
    m_pSplitter = nullptr;
    m_pTabWdgtMain = nullptr;
    m_pWdgtPhysSizes = nullptr;
    m_pWdgtTest = nullptr;

} // dtor
