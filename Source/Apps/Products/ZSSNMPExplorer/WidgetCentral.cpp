/*******************************************************************************

Copyright 2004 - 2009 by    ZeusSoft, Ing. Buero Bauer
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

--------------------------------------------------------------------------------

$Date: 22.01.10 22:11 $

$Revision: 8 $

$Author: Christian Bauer $

$History: ZSWidgetCentral.cpp $
 * 
 * *****************  Version 8  *****************
 * User: Christian Bauer Date: 22.01.10   Time: 22:11
 * Updated in $/ZSAppSNMPExplorer/Source
 * 
 * *****************  Version 6  *****************
 * User: Christian Bauer Date: 10.01.10   Time: 19:16
 * Updated in $/ZSAppSNMPExplorer/Source

*******************************************************************************/

#include <QtCore/qsettings.h>
#include <QtCore/qthread.h>
#include <QtCore/qtimer.h>
#include <QtGui/qlayout.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qtabwidget.h>
#include "Include/ZSWidgetCentral.h"
#include "Include/ZSApp.h"
#include "ZSSNMP/Include/ZSSNMPExplorerWdgt.h"
#include "ZSSNMP/Include/ZSSNMPCltStdPCWdgt.h"
#include "ZSTrace/Include/ZSTrcServer.h"
#include "ZSTrace/Include/ZSTrcServerWdgt.h"
#include "ZSSys/Include/ZSSysErrLogModel.h"
#include "ZSSys/Include/ZSSysErrLogWdgt.h"
#include "ZSSys/Include/ZSSysErrResult.h"
#include "ZSSys/Include/ZSSysException.h"
#include "ZSSys/Include/ZSSysMemLeakDump.h"

using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::SNMP;


/*******************************************************************************
class CWidgetCentral : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWidgetCentral::CWidgetCentral(
    CExplorer*     /*i_pExplorer*/,
    CCltStdPC*     i_pCltStdPC,
    QSettings*     i_pSettingsFile,
    const QString& i_strSettingsKey,
    QWidget*       i_pWdgtParent,
    Qt::WFlags     i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_pSettingsFile(i_pSettingsFile),
    m_strSettingsKey(i_strSettingsKey),
    m_pLytMain(NULL),
    m_pSplitter(NULL),
    m_pTabWdgt(NULL),
    m_pWdgtExplorer(NULL),
    m_pWdgtCltStdPC(NULL),
    m_pWdgtTrcServer(NULL),
    m_pWdgtErrLog(NULL)
{
    setObjectName("CentralWidget");

    if( m_strSettingsKey.isEmpty() )
    {
        m_strSettingsKey = objectName();
    }

    // Create user controls
    //----------------------

    QPalette paletteWdgtCentral;
    paletteWdgtCentral.setColor(QPalette::Window,Qt::lightGray);
    setPalette(paletteWdgtCentral);
    setAutoFillBackground(true);

    m_pLytMain = new QVBoxLayout();
    setLayout(m_pLytMain);

    m_pSplitter = new QSplitter(Qt::Vertical);
    m_pSplitter->setChildrenCollapsible(false);
    m_pLytMain->addWidget(m_pSplitter);

    // Tab widget
    //-----------------

    m_pTabWdgt = new QTabWidget(this);
    m_pSplitter->addWidget(m_pTabWdgt);

    // Explorer widget
    //-----------------

    m_pWdgtExplorer = new CWdgtExplorer(
        /* pSettingsFile  */ m_pSettingsFile,
        /* strSettingsKey */ "WdgtExplorer" );
    m_pTabWdgt->addTab(m_pWdgtExplorer,"Explorer");

    // Standard PC client widget
    //--------------------------

    m_pWdgtCltStdPC = new CWdgtCltStdPC(
        /* pCltStdPC      */ i_pCltStdPC,
        /* pSettingsFile  */ m_pSettingsFile,
        /* strSettingsKey */ "WdgtCltStdPC" );
    m_pTabWdgt->addTab(m_pWdgtCltStdPC,"Standard PC Client");

    // Trace Server
    //-------------

    if( CTrcServer::getTrcServer(ETraceTypeMth) != NULL )
    {
        m_pWdgtTrcServer = new Trace::CWdgtServer(
            /* pTrcServer     */ CTrcServer::getTrcServer(ETraceTypeMth),
            /* pSettingsFile  */ m_pSettingsFile,
            /* strSettingsKey */ "WdgtTrcServer" );
        m_pTabWdgt->addTab(m_pWdgtTrcServer,"Trace Server");
    }

    // Error Table
    //------------

    m_pWdgtErrLog = new CWdgtErrLog(
        /* pSettingsFile */ m_pSettingsFile,
        /* strHeadLine   */ "Errors");
    m_pSplitter->addWidget(m_pWdgtErrLog);

    // Restore splitter position
    //--------------------------

    if( m_pSettingsFile != NULL )
    {
        QList<int> listSizes = m_pSplitter->sizes();

        for( int idx = 0; idx < listSizes.count(); idx++ )
        {
            listSizes[idx] = m_pSettingsFile->value( m_strSettingsKey + "/SplitterHeight" + QString::number(idx), 250 ).toInt();
        }
        m_pSplitter->setSizes(listSizes);
    }

} // ctor

//------------------------------------------------------------------------------
CWidgetCentral::~CWidgetCentral()
//------------------------------------------------------------------------------
{
    if( m_pSettingsFile != NULL )
    {
        QList<int> listSizes = m_pSplitter->sizes();

        for( int idx = 0; idx < listSizes.count(); idx++ )
        {
            m_pSettingsFile->setValue( m_strSettingsKey + "/SplitterHeight" + QString::number(idx), listSizes[idx] );
        }
    }

    m_pLytMain = NULL;
    m_pSplitter = NULL;
    m_pTabWdgt = NULL;
    m_pWdgtExplorer = NULL;
    m_pWdgtCltStdPC = NULL;
    m_pWdgtTrcServer = NULL;
    m_pWdgtErrLog = NULL;

} // dtor
