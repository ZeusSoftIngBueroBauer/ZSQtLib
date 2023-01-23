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

#include "WidgetCentral.h"
#include "App.h"
#include "Test.h"
#include "WdgtTestOutput.h"
#include "Units/Units.h"

#include "ZSPhysValGUI/ZSPhysUnitsWdgt.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"
#include "ZSTestGUI/ZSTestStepIdxTreeWdgt.h"

#include <QtCore/qsettings.h>
#include <QtCore/qtimer.h>
#include <QtGui/qevent.h>

#if QT_VERSION < 0x050000
#include <QtGui/qgroupbox.h>
#include <QtGui/qlayout.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qtabwidget.h>
#else
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qtabwidget.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;
using namespace ZS::Test;
using namespace ZS::Test::GUI;
using namespace ZS::Apps::Test::Diagram;


/*******************************************************************************
class CWidgetCentral : public QWidget
*******************************************************************************/

/*==============================================================================
private: // class members
==============================================================================*/

CWidgetCentral* CWidgetCentral::s_pThis = nullptr; // singleton class

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CWidgetCentral* CWidgetCentral::GetInstance()
//------------------------------------------------------------------------------
{
    return s_pThis;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWidgetCentral::CWidgetCentral(
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_pLyt(nullptr),
    m_pTabWdgtMain(nullptr),
    m_pWdgtPhysSizes(nullptr),
    m_pSplitter(nullptr),
    m_pGrpTestOutput(nullptr),
    m_pLytGrpTestOutput(nullptr),
    m_pWdgtTestOutput(nullptr)
{
    if( s_pThis != nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultSingletonClassAlreadyInstantiated);
    }
    s_pThis = this;

    setObjectName("CentralWidget");

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    // <TabWidget>
    //============

    m_pTabWdgtMain = new QTabWidget(this);
    m_pLyt->addWidget(m_pTabWdgtMain);

    // <Tab> Test
    //-------------

    // <Splitter> Following widgets organized in splitter
    //---------------------------------------------------

    m_pSplitter = new QSplitter(Qt::Vertical);
    m_pSplitter->setChildrenCollapsible(false);
    m_pTabWdgtMain->addTab(m_pSplitter, "Test");

    // <TestSteps>
    //----------------

    m_pWdgtTest = new CWdgtIdxTreeTestSteps( CApplication::GetInstance()->getTest() );
    m_pSplitter->addWidget(m_pWdgtTest);

    // <GroupBox> Test
    //---------------------------

    m_pGrpTestOutput = new QGroupBox("Test");
    m_pLytGrpTestOutput = new QVBoxLayout();
    m_pGrpTestOutput->setLayout(m_pLytGrpTestOutput);
    m_pSplitter->addWidget(m_pGrpTestOutput);

    m_pWdgtTestOutput = new CWdgtTestOutput();
    m_pLytGrpTestOutput->addWidget(m_pWdgtTestOutput);

    // <Tab> Units
    //-------------

    m_pWdgtPhysSizes = new CWdgtUnits();
    m_pTabWdgtMain->addTab(m_pWdgtPhysSizes,"Units");

    // Restore geometry of widget
    //---------------------------

    QSettings settings;

    restoreGeometry( settings.value(objectName()+"/Geometry").toByteArray() );

    if( m_pSplitter != nullptr )
    {
        QList<int> listSizes;
        int        idx;

        listSizes = m_pSplitter->sizes();

        for( idx = 0; idx < listSizes.count(); idx++ )
        {
            listSizes[idx] = settings.value( objectName() + "/SplitterHeight" + QString::number(idx), 50 ).toInt();
        }
        m_pSplitter->setSizes(listSizes);
    }

} // ctor

//------------------------------------------------------------------------------
CWidgetCentral::~CWidgetCentral()
//------------------------------------------------------------------------------
{
    QSettings settings;

    settings.setValue( objectName()+"/Geometry", saveGeometry() );

    if( m_pSplitter != nullptr )
    {
        QList<int> listSizes;
        int        idx;

        listSizes = m_pSplitter->sizes();

        for( idx = 0; idx < listSizes.count(); idx++ )
        {
            settings.setValue( objectName() + "/SplitterHeight" + QString::number(idx), listSizes[idx] );
        }
    }

    m_pLyt = nullptr;
    m_pTabWdgtMain = nullptr;
    m_pWdgtPhysSizes = nullptr;
    m_pSplitter = nullptr;
    m_pWdgtTest = nullptr;
    m_pGrpTestOutput = nullptr;
    m_pLytGrpTestOutput = nullptr;
    m_pWdgtTestOutput = nullptr;

    s_pThis = nullptr;

} // dtor
