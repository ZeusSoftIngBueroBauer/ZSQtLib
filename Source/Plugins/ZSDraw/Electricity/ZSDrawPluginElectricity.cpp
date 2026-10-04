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

#include "ZSDrawPluginElectricity.h"
#include "Electricity/ZSDrawObjFactoryElectricityCapacitor.h"
#include "Electricity/ZSDrawObjFactoryElectricityDiode.h"
#include "Electricity/ZSDrawObjFactoryElectricityInductor.h"
#include "Electricity/ZSDrawObjFactoryElectricitySwitch.h"
#include "Electricity/ZSDrawObjFactoryElectricityResistor.h"
#include "Electricity/ZSDrawObjFactoryElectricityTransistor.h"
#include "Electricity/ZSDrawObjFactoryElectricityVoltageSource.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtGui/qbitmap.h>
#include <QtGui/qpixmap.h>

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::Draw::Plugins::Electricity;
using namespace ZS::System;

/*******************************************************************************
class CDrawPluginElectricity : public QObject, public IDrawPluginInterface
*******************************************************************************/

//const QString CMainWindow::c_strActionNameDrawElectricityResistor    = c_strMenuNameDrawElectricity + ":&Resistor";
//const QString CMainWindow::c_strActionNameDrawElectricityCapacitor   = c_strMenuNameDrawElectricity + ":&Capacitor";
//const QString CMainWindow::c_strActionNameDrawElectricityInductor    = c_strMenuNameDrawElectricity + ":&Inductor";
//const QString CMainWindow::c_strActionNameDrawElectricitySwitch      = c_strMenuNameDrawElectricity + ":&Switch";
//const QString CMainWindow::c_strActionNameDrawElectricityTransistor  = c_strMenuNameDrawElectricity + ":&Transistor";

//const QString CMainWindow::c_strMenuNameDrawElectricity    = "Draw:&Electricity";
//const QString CMainWindow::c_strObjFactoryElectricity = "Electricity";

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDrawPluginElectricity::CDrawPluginElectricity()
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
CDrawPluginElectricity::~CDrawPluginElectricity()
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
        delete m_pObjFactoryElectricityCapacitor;
    }
    catch(...) {
    }
    m_pObjFactoryElectricityCapacitor = nullptr;

    try {
        delete m_pObjFactoryElectricityDiode;
    }
    catch(...) {
    }
    m_pObjFactoryElectricityDiode = nullptr;

    try {
        delete m_pObjFactoryElectricityInductor;
    }
    catch(...) {
    }
    m_pObjFactoryElectricityInductor = nullptr;

    try {
        delete m_pObjFactoryElectricityResistor;
    }
    catch(...) {
    }
    m_pObjFactoryElectricityResistor = nullptr;

    try {
        delete m_pObjFactoryElectricitySwitch;
    }
    catch(...) {
    }
    m_pObjFactoryElectricitySwitch = nullptr;

    try {
        delete m_pObjFactoryElectricityTransistor;
    }
    catch(...) {
    }
    m_pObjFactoryElectricityTransistor = nullptr;

    try {
        delete m_pObjFactoryElectricityVoltageSource;
    }
    catch(...) {
    }
    m_pObjFactoryElectricityVoltageSource = nullptr;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;
}

/*==============================================================================
public: // interface methods of ZSDrawPluginInterface
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawPluginElectricity::createObjFactories()
//------------------------------------------------------------------------------
{
    QPixmap pxmDrawVoltageSource(":/ZS/Draw/Plugins/Electricity/VoltageSource16x16.bmp");
    pxmDrawVoltageSource.setMask(pxmDrawVoltageSource.createHeuristicMask());
    m_pObjFactoryElectricityVoltageSource = new CObjFactoryVoltageSource(pxmDrawVoltageSource);
    QPixmap pxmDrawResistor(":/ZS/Draw/Plugins/Electricity/Resistor16x16.bmp");
    pxmDrawResistor.setMask(pxmDrawResistor.createHeuristicMask());
    m_pObjFactoryElectricityResistor = new CObjFactoryResistor(pxmDrawResistor);
    QPixmap pxmDrawInductor(":/ZS/Draw/Plugins/Electricity/Inductor16x16.bmp");
    pxmDrawInductor.setMask(pxmDrawInductor.createHeuristicMask());
    m_pObjFactoryElectricityInductor = new CObjFactoryInductor(pxmDrawInductor);
    QPixmap pxmDrawCapacitor(":/ZS/Draw/Plugins/Electricity/Capacitor16x16.bmp");
    pxmDrawCapacitor.setMask(pxmDrawCapacitor.createHeuristicMask());
    m_pObjFactoryElectricityCapacitor = new CObjFactoryCapacitor(pxmDrawCapacitor);
    QPixmap pxmDrawSwitch(":/ZS/Draw/Plugins/Electricity/Switch16x16.bmp");
    pxmDrawSwitch.setMask(pxmDrawSwitch.createHeuristicMask());
    m_pObjFactoryElectricitySwitch = new CObjFactorySwitch(pxmDrawSwitch);
    QPixmap pxmDrawDiode(":/ZS/Draw/Plugins/Electricity/Diode16x16.bmp");
    pxmDrawDiode.setMask(pxmDrawDiode.createHeuristicMask());
    m_pObjFactoryElectricityDiode = new CObjFactoryDiode(pxmDrawDiode);
    QPixmap pxmDrawTransistor(":/ZS/Draw/Plugins/Electricity/Transistor16x16.bmp");
    pxmDrawTransistor.setMask(pxmDrawTransistor.createHeuristicMask());
    m_pObjFactoryElectricityTransistor = new CObjFactoryTransistor(pxmDrawTransistor);
}
