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

#if QT_VERSION < 0x050000
#include <QtGui/qlayout.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qtableview.h>
#include <QtGui/qtreeview.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qtableview.h>
#include <QtWidgets/qtreeview.h>
#endif

#include "WidgetCentral.h"

#include "ZSSysGUI/ZSSysErrLogWdgt.h"
#include "ZSSysGUI/ZSSysObjPoolModel.h"
#include "ZSSys/ZSSysObjPool.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Apps::Test::ObjPoolModel;


/*******************************************************************************
class CWidgetCentral : public QWidget
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

CWidgetCentral* CWidgetCentral::s_pThis = nullptr; // singleton class

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWidgetCentral::CWidgetCentral(
    CObjPool*       i_pObjPool,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_pLyt(nullptr),
    m_pSplitter(nullptr),
    m_pObjPool(i_pObjPool),
    m_pObjPoolModel(nullptr),
    m_pTreeView(nullptr),
    m_pWdgtErrLog(nullptr)
{
    if( s_pThis != nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultSingletonClassAlreadyInstantiated);
    }
    s_pThis = this;

    setObjectName("CentralWidget");

    //QPalette paletteWdgtCentral;
    //paletteWdgtCentral.setColor(QPalette::Window,Qt::lightGray);
    //setPalette(paletteWdgtCentral);
    //setAutoFillBackground(true);

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    m_pSplitter = new QSplitter(Qt::Vertical);
    m_pLyt->addWidget(m_pSplitter);

    // Tree Model
    //-----------

    m_pObjPoolModel = new CModelObjPool(m_pObjPool);

    m_pTreeView = new QTreeView(this);
    m_pSplitter->addWidget(m_pTreeView);

    m_pTreeView->setModel(m_pObjPoolModel);

    m_pTreeView->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_pTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Please note that the first column should not be hidden to allow
    // collapsing and expanding the tree view.
    //m_pTreeView->showColumn(CModelObjPool::EColumnNodeName);
    //m_pTreeView->hideColumn(CModelObjPool::EColumnObjId);
    //m_pTreeView->hideColumn(CModelObjPool::EColumnObjState);
    //m_pTreeView->hideColumn(CModelObjPool::EColumnType);

    // Err Log Widget
    //---------------

    //m_pWdgtErrLog = new CWdgtErrLog();
    //m_pSplitter->addWidget(m_pWdgtErrLog);

    // <Geometry>
    //--------------------------

    QSettings settings;

    QList<int> listSizes = m_pSplitter->sizes();

    for( int idx = 0; idx < listSizes.count(); idx++ )
    {
        listSizes[idx] = settings.value( "WidgetCentral/SplitterWidth" + QString::number(idx), 250 ).toInt();
    }
    m_pSplitter->setSizes(listSizes);

} // ctor

//------------------------------------------------------------------------------
CWidgetCentral::~CWidgetCentral()
//------------------------------------------------------------------------------
{
    // <Geometry>
    //-----------------------

    QSettings settings;

    QList<int> listSizes = m_pSplitter->sizes();

    for( int idx = 0; idx < listSizes.count(); idx++ )
    {
        settings.setValue( "WidgetCentral/SplitterWidth" + QString::number(idx), listSizes[idx] );
    }

    // <Clear>
    //-----------------------

    try
    {
        delete m_pObjPoolModel;
    }
    catch(...)
    {
    }

    m_pLyt = nullptr;
    m_pSplitter = nullptr;
    m_pObjPool = nullptr;
    m_pObjPoolModel = nullptr;
    m_pTreeView = nullptr;
    m_pWdgtErrLog = nullptr;

    s_pThis = nullptr;

} // dtor
