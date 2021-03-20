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

$Revision: 7 $

$Author: Christian Bauer $

$History: ZSMainWindow.cpp $
 * 
 * *****************  Version 7  *****************
 * User: Christian Bauer Date: 22.01.10   Time: 22:11
 * Updated in $/ZSAppSNMPExplorer/Source
 * 
 * *****************  Version 6  *****************
 * User: Christian Bauer Date: 10.01.10   Time: 23:37
 * Updated in $/ZSAppSNMPExplorer/Source

*******************************************************************************/

#include <QtCore/qsettings.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include "Include/ZSMainWindow.h"
#include "Include/ZSApp.h"
#include "Include/ZSWidgetCentral.h"
#include "ZSSys/Include/ZSSysMemLeakDump.h"

using namespace ZS::SNMP;


/*******************************************************************************
class CMainWindow : public QMainWindow
*******************************************************************************/

/*==============================================================================
private: // class members
==============================================================================*/

CMainWindow*  CMainWindow::s_pThis = NULL; // singleton class

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CMainWindow* CMainWindow::instance()
//------------------------------------------------------------------------------
{
    return s_pThis;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMainWindow::CMainWindow(
    const QString& i_strWindowTitle,
    CExplorer*     i_pExplorer,
    CCltStdPC*     i_pCltStdPC,
    QSettings*     i_pSettingsFile,
    const QString& i_strSettingsKey,
    QWidget*       i_pWdgtParent,
    Qt::WFlags     i_wflags ) :
//------------------------------------------------------------------------------
    QMainWindow(i_pWdgtParent,i_wflags),
    m_pSettingsFile(i_pSettingsFile),
    m_strSettingsKey(i_strSettingsKey),
    m_pWdgtCentral(NULL)
{
    setObjectName("MainWindow");

    if( m_strSettingsKey.isEmpty() )
    {
        m_strSettingsKey = objectName();
    }

    setWindowTitle(i_strWindowTitle);

    QPoint ptPosMainWindow(100,100);
    QSize  sizeMainWindow(980,890);

    if( m_pSettingsFile != NULL )
    {
        ptPosMainWindow = m_pSettingsFile->value( m_strSettingsKey + "/Pos", ptPosMainWindow ).toPoint();
        sizeMainWindow = m_pSettingsFile->value( m_strSettingsKey + "/Size", sizeMainWindow ).toSize();
    }

    move(ptPosMainWindow);
    resize(sizeMainWindow);

    m_pWdgtCentral = new CWidgetCentral(
        /* pExplorer      */ i_pExplorer,
        /* pCltStdPC      */ i_pCltStdPC,
        /* pSettingsFile  */ m_pSettingsFile,
        /* strSettingsKey */ "WdgtCentral",
        /* pWdgtParent    */ this );
    setCentralWidget(m_pWdgtCentral);

} // ctor

//------------------------------------------------------------------------------
CMainWindow::~CMainWindow()
//------------------------------------------------------------------------------
{
    if( m_pSettingsFile != NULL )
    {
        m_pSettingsFile->setValue( m_strSettingsKey + "/Pos", geometry().topLeft() );
        m_pSettingsFile->setValue( m_strSettingsKey + "/Size", geometry().size() );
    }

    m_pSettingsFile = NULL;
    m_pWdgtCentral = NULL;

} // dtor
