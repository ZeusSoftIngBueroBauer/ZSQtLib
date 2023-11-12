/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer, Germany
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
#include <QtGui/qevent.h>
#include <QtGui/qpainter.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qgroupbox.h>
#include <QtGui/qlayout.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qsplitter.h>
#else
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qsplitter.h>
#endif

#include "WidgetCentral.h"
#include "App.h"
#include "Test.h"

#include "ZSIpcTrace/ZSIpcTrcServer.h"
#include "ZSTestGUI/ZSTestStepIdxTreeWdgt.h"
#include "ZSIpcTraceGUI/ZSIpcTrcMthWdgt.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Test::GUI;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::Trace::GUI;
using namespace ZS::Apps::Test::IpcTrace;


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
    CTest*          i_pTest,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_pTest(i_pTest),
    m_pLyt(nullptr),
    m_pSplitter(nullptr),
    m_pWdgtTest(nullptr),
    m_pTabWidgetTrcMthLists(nullptr),
    m_pWdgtTrcMthList(nullptr)
{
    if( s_pThis != nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultSingletonClassAlreadyInstantiated);
    }
    s_pThis = this;

    setObjectName("CentralWidget");

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    // <Splitter> Following widgets organized in splitter
    //---------------------------------------------------

    m_pSplitter = new QSplitter(Qt::Vertical);
    m_pSplitter->setChildrenCollapsible(false);
    m_pLyt->addWidget(m_pSplitter);

    // <TestSteps>
    //----------------

    m_pWdgtTest = new CWdgtIdxTreeTestSteps( CApplication::GetInstance()->getTest() );
    m_pSplitter->addWidget(m_pWdgtTest);

    // <MethodTrace>
    //----------------

    m_pTabWidgetTrcMthLists = new QTabWidget();
    m_pSplitter->addWidget(m_pTabWidgetTrcMthLists);

    CIpcTrcClient* pTrcClient = CApplication::GetInstance()->getTrcClient();
    QString strCltName = pTrcClient->objectName();
    m_pWdgtTrcMthList = new CWdgtTrcMthList(pTrcClient);
    m_pTabWidgetTrcMthLists->addTab(m_pWdgtTrcMthList, strCltName);

    QObject::connect(
        m_pWdgtTrcMthList, &CWdgtTrcMthList::progressBarConnectDblClicked,
        this, &CWidgetCentral::onProgressBarConnectDblClicked);

    // Restore geometry of widget
    //---------------------------

    QSettings settings;

    restoreGeometry( settings.value(objectName()+"/Geometry").toByteArray() );

    if( m_pSplitter != nullptr )
    {
        QList<int> listSizes = m_pSplitter->sizes();
        for( int idx = 0; idx < listSizes.count(); idx++ )
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

    m_pTest = nullptr;
    m_pLyt = nullptr;
    m_pSplitter = nullptr;
    m_pWdgtTest = nullptr;
    m_pTabWidgetTrcMthLists = nullptr;
    m_pWdgtTrcMthList = nullptr;

    s_pThis = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtTrcMthList* CWidgetCentral::getTrcMthListWdgt()
//------------------------------------------------------------------------------
{
    return m_pWdgtTrcMthList;
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWidgetCentral::onProgressBarConnectDblClicked()
//------------------------------------------------------------------------------
{
    emit progressBarConnectDblClicked();
}
