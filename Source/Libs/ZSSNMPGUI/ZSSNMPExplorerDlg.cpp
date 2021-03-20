/*******************************************************************************

Copyright 2004 - 2010 by    ZeusSoft, Ing. Buero Bauer
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

$Date: $

$Revision: $

$Author: $

$History: $

*******************************************************************************/

#ifdef NO_GUI
#error Module may not be used in this configuration.
#endif

#include <QtCore/qcoreapplication.h>
#include <QtCore/qsettings.h>
#include <QtGui/qlayout.h>
#include "ZSSNMP/Include/ZSSNMPExplorerDlg.h"
#include "ZSSNMP/Include/ZSSNMPExplorerWdgt.h"
#include "ZSSys/Include/ZSSysErrResult.h"
#include "ZSSys/Include/ZSSysException.h"
#include "ZSSys/Include/ZSSysMemLeakDump.h"


using namespace ZS::SNMP;


/*******************************************************************************
class CDlgExplorer : public QDialog
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgExplorer::CDlgExplorer(
    const QString& i_strWindowTitle,
    CExplorer*     i_pExplorer,
    QSettings*     i_pSettingsFile,
    const QString& i_strSettingsKey,
    QWidget*       i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QDialog(
        /* pWdgtParent */ i_pWdgtParent,
        /* windowFlags */ Qt::CustomizeWindowHint
                         |Qt::WindowTitleHint
                         |Qt::WindowSystemMenuHint
                         |Qt::Window
        #if QT_VERSION >= 0x040700
                         |Qt::WindowCloseButtonHint
        #endif
                         |Qt::WindowMinMaxButtonsHint ),
    m_pExplorer(i_pExplorer),
    m_pSettingsFile(i_pSettingsFile),
    m_strSettingsKey(i_strSettingsKey),
    m_pLytMain(NULL),
    m_pWdgtExplorer(NULL)
{
    setWindowTitle(i_strWindowTitle);

    QPalette paletteWdgtCentral;
    paletteWdgtCentral.setColor(QPalette::Window,Qt::lightGray);
    setPalette(paletteWdgtCentral);
    setAutoFillBackground(true);

    m_pLytMain = new QVBoxLayout();
    setLayout(m_pLytMain);

    m_pWdgtExplorer = new CWdgtExplorer(
        /* pExplorer      */ m_pExplorer,
        /* pSettingsFile  */ m_pSettingsFile,
        /* strSettingsKey */ "WdgtExplorer",
        /* pWdgtParent    */ NULL );
    m_pLytMain->addWidget(m_pWdgtExplorer);

} // ctor

//------------------------------------------------------------------------------
CDlgExplorer::~CDlgExplorer()
//------------------------------------------------------------------------------
{
    m_pExplorer = NULL;
    m_pSettingsFile = NULL;
    m_pLytMain = NULL;
    m_pWdgtExplorer = NULL;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgExplorer::show()
//------------------------------------------------------------------------------
{
    // Restore position and size of dialog
    //------------------------------------

    if( m_pSettingsFile != NULL )
    {
        restoreGeometry( m_pSettingsFile->value(m_strSettingsKey+"/Geometry").toByteArray() );
    }

    // Call base method to show the dialog
    //------------------------------------

    QDialog::show();

} // show

//------------------------------------------------------------------------------
void CDlgExplorer::hide()
//------------------------------------------------------------------------------
{
    // Save position and size of dialog
    //------------------------------------

    if( m_pSettingsFile != NULL )
    {
        m_pSettingsFile->setValue(m_strSettingsKey+"/Geometry",saveGeometry());
    }

    // Call base method to hide the dialog
    //------------------------------------

    QDialog::hide();

} // hide

/*==============================================================================
protected: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgExplorer::closeEvent( QCloseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    // Save position and size of dialog
    //------------------------------------

    if( m_pSettingsFile != NULL )
    {
        m_pSettingsFile->setValue(m_strSettingsKey+"/Geometry",saveGeometry());
    }

    QDialog::closeEvent(i_pEv);

    emit closed();

} // closeEvent
