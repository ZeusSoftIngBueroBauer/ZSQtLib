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

#include <QtCore/qsettings.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qapplication.h>
#include <QtGui/qlayout.h>
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qlayout.h>
#endif

#include "ZSTestGUI/ZSTestDlg.h"
#include "ZSTestGUI/ZSTestStepIdxTreeWdgt.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Test;
using namespace ZS::Test::GUI;


/*******************************************************************************
class CDlgTest : public QDialog
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgTest::CDlgTest(
    const QString&  i_strObjName,
    CTest*          i_pTest,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QDialog(i_pWdgtParent,i_wflags),
    m_strSettingsKey(),
    m_pLytMain(nullptr),
    m_pWdgtTest(nullptr)
{
    setObjectName(i_strObjName);

    m_strSettingsKey = "DlgTest" + i_strObjName;

    setWindowTitle( QCoreApplication::applicationName() + ": " + i_strObjName );

    m_pLytMain = new QVBoxLayout;
    setLayout(m_pLytMain);

    m_pWdgtTest = new CWdgtIdxTreeTestSteps(i_pTest);
    m_pLytMain->addWidget(m_pWdgtTest);

} // ctor

//------------------------------------------------------------------------------
CDlgTest::~CDlgTest()
//------------------------------------------------------------------------------
{
    m_pLytMain = nullptr;
    m_pWdgtTest = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgTest::show()
//------------------------------------------------------------------------------
{
    // Restore position and size of dialog
    //------------------------------------

    QSettings settings;

    restoreGeometry( settings.value(m_strSettingsKey+"/Geometry").toByteArray() );

    // Call base method to show the dialog
    //------------------------------------

    QDialog::show();

} // show

//------------------------------------------------------------------------------
void CDlgTest::hide()
//------------------------------------------------------------------------------
{
    // Save position and size of dialog
    //------------------------------------

    QSettings settings;

    settings.setValue( m_strSettingsKey+"/Geometry", saveGeometry() );

    // Call base method to hide the dialog
    //------------------------------------

    QDialog::hide();

} // hide

/*==============================================================================
protected: // overridables of base class QDialog
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgTest::closeEvent( QCloseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    // Save position and size of dialog
    //------------------------------------

    QSettings settings;

    settings.setValue( m_strSettingsKey+"/Geometry", saveGeometry() );

    // Call base method to hide the dialog
    //------------------------------------

    QDialog::closeEvent(i_pEv);

} // closeEvent

