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

#include "ZSDrawPluginQtWidgets.h"
#include "QtWidgets/ZSDrawObjFactoryWdgtCheckBox.h"
#include "QtWidgets/ZSDrawObjFactoryWdgtComboBox.h"
#include "QtWidgets/ZSDrawObjFactoryWdgtGroupBox.h"
#include "QtWidgets/ZSDrawObjFactoryWdgtLabel.h"
#include "QtWidgets/ZSDrawObjFactoryWdgtLineEdit.h"
#include "QtWidgets/ZSDrawObjFactoryWdgtPushButton.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtGui/qbitmap.h>
#include <QtGui/qpixmap.h>

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::Draw::Plugins::QtWidgets;
using namespace ZS::System;

/*******************************************************************************
class CDrawPluginQtWidgets : public QObject, public IDrawPluginInterface
*******************************************************************************/

//const QString CMainWindow::c_strActionNameDrawWdgtCheckBox           = c_strMenuNameDrawWidgets + ":C&heck Box";
//const QString CMainWindow::c_strActionNameDrawWdgtComboBox           = c_strMenuNameDrawWidgets + ":&Combo Box";
//const QString CMainWindow::c_strActionNameDrawWdgtGroupBox           = c_strMenuNameDrawWidgets + ":&Group Box";
//const QString CMainWindow::c_strActionNameDrawWdgtLabel              = c_strMenuNameDrawWidgets + ":&Label";
//const QString CMainWindow::c_strActionNameDrawWdgtLineEdit           = c_strMenuNameDrawWidgets + ":Line &Edit";
//const QString CMainWindow::c_strActionNameDrawWdgtPushButton         = c_strMenuNameDrawWidgets + ":&Push Button";

//const QString CMainWindow::c_strMenuNameDrawWidgets        = "Draw:&Widgets";
//const QString CMainWindow::c_strObjFactoryQtWidgets = "QtWidgets";

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDrawPluginQtWidgets::CDrawPluginQtWidgets()
//------------------------------------------------------------------------------
{
    setObjectName("theInst");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );
}

//------------------------------------------------------------------------------
CDrawPluginQtWidgets::~CDrawPluginQtWidgets()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    mthTracer.onAdminObjAboutToBeReleased();

    try {
        delete m_pObjFactoryWdgtCheckBox;
    }
    catch(...) {
    }
    m_pObjFactoryWdgtCheckBox = nullptr;

    try {
        delete m_pObjFactoryWdgtComboBox;
    }
    catch(...) {
    }
    m_pObjFactoryWdgtComboBox = nullptr;

    try {
        delete m_pObjFactoryWdgtGroupBox;
    }
    catch(...) {
    }
    m_pObjFactoryWdgtGroupBox = nullptr;

    try {
        delete m_pObjFactoryWdgtLabel;
    }
    catch(...) {
    }
    m_pObjFactoryWdgtLabel = nullptr;

    try {
        delete m_pObjFactoryWdgtLineEdit;
    }
    catch(...) {
    }
    m_pObjFactoryWdgtLineEdit = nullptr;

    try {
        delete m_pObjFactoryWdgtPushButton;
    }
    catch(...) {
    }
    m_pObjFactoryWdgtPushButton = nullptr;


    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;
}

/*==============================================================================
public: // interface methods of ZSDrawPluginInterface
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawPluginQtWidgets::createObjFactories()
//------------------------------------------------------------------------------
{
    QPixmap pxmDrawWdgtCheckBox16x16(":/ZS/Draw/Plugins/QtWidgets/CheckBox16x16.bmp");
    pxmDrawWdgtCheckBox16x16.setMask(pxmDrawWdgtCheckBox16x16.createHeuristicMask());
    m_pObjFactoryWdgtCheckBox = new CObjFactoryWdgtCheckBox(pxmDrawWdgtCheckBox16x16);
    QPixmap pxmDrawWdgtComboBox16x16(":/ZS/Draw/Plugins/QtWidgets/ComboBox16x16.bmp");
    pxmDrawWdgtComboBox16x16.setMask(pxmDrawWdgtComboBox16x16.createHeuristicMask());
    m_pObjFactoryWdgtComboBox = new CObjFactoryWdgtComboBox(pxmDrawWdgtComboBox16x16);
    QPixmap pxmDrawWdgtGroupBox16x16(":/ZS/Draw/Plugins/QtWidgets/GroupBox16x16.bmp");
    pxmDrawWdgtGroupBox16x16.setMask(pxmDrawWdgtGroupBox16x16.createHeuristicMask());
    m_pObjFactoryWdgtGroupBox = new CObjFactoryWdgtGroupBox(pxmDrawWdgtGroupBox16x16);
    QPixmap pxmDrawWdgtLabel16x16(":/ZS/Draw/Plugins/QtWidgets/Label16x16.bmp");
    pxmDrawWdgtLabel16x16.setMask(pxmDrawWdgtLabel16x16.createHeuristicMask());
    m_pObjFactoryWdgtLabel = new CObjFactoryWdgtLabel(pxmDrawWdgtLabel16x16);
    QPixmap pxmDrawWdgtLineEdit16x16(":/ZS/Draw/Plugins/QtWidgets/LineEdit16x16.bmp");
    pxmDrawWdgtLineEdit16x16.setMask(pxmDrawWdgtLineEdit16x16.createHeuristicMask());
    m_pObjFactoryWdgtLineEdit = new CObjFactoryWdgtLineEdit(pxmDrawWdgtLineEdit16x16);
    QPixmap pxmDrawWdgtPushButton16x16(":/ZS/Draw/Plugins/QtWidgets/PushButton16x16.bmp");
    pxmDrawWdgtPushButton16x16.setMask(pxmDrawWdgtPushButton16x16.createHeuristicMask());
    m_pObjFactoryWdgtPushButton = new CObjFactoryWdgtPushButton(pxmDrawWdgtPushButton16x16);
}
