/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#if 0

#include <QtCore/qtimer.h>

#include <QtGui/qevent.h>

#if QT_VERSION < 0x050000
#include <QtGui/qaction.h>
#else
#include <QtWidgets/qaction.h>
#endif

#include "Test.h"
#include "MainWindow.h"
#include "ZSDrawGraphObjElectricityResistor.h"
#include "ZSDrawGraphObjElectricityCapacitor.h"
#include "ZSDrawGraphObjElectricityInductor.h"
#include "ZSDrawGraphObjElectricitySwitch.h"
#include "ZSDrawGraphObjElectricityTransistor.h"

#include "ZSDraw/ZSDrawingScene.h"
#include "ZSDraw/ZSDrawObjFactory.h"
#include "ZSDraw/ZSDrawGraphObjConnectionLine.h"
#include "ZSDraw/ZSDrawGraphObjConnectionPoint.h"
#include "ZSDraw/ZSDrawGraphObjEllipse.h"
#include "ZSDraw/ZSDrawGraphObjGroup.h"
#include "ZSDraw/ZSDrawGraphObjLine.h"
#include "ZSDraw/ZSDrawGraphObjPoint.h"
#include "ZSDraw/ZSDrawGraphObjPolygon.h"
#include "ZSDraw/ZSDrawGraphObjPolyline.h"
#include "ZSDraw/ZSDrawGraphObjRect.h"
#include "ZSDraw/ZSDrawGraphObjText.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::Draw::Electricity;
using namespace ZS::Draw::QtWidgets;
using namespace ZS::PhysVal;
using namespace ZS::Apps::Products::Draw;
using namespace ZS::Trace;


/*******************************************************************************
class CTest : public Test::CTest
*******************************************************************************/

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepElectricity()
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStepGroup* pTSGrpElectricity = getTestStepGroup( "Electricity", nullptr );

    if( !pTSGrpElectricity->isEnabled() )
    {
        m_iTestStepLev1++; // Next test step
        m_iTestStepLev2 = 0;
        triggerDoTestStep();
    }
    else // if( pTSGrpElectricity->isEnabled() )
    {
        if( m_iTestStepLev2 == ETestStepLev2ElectricityResistors )
        {
            doTestStepElectricityResistors(pTSGrpElectricity);
        }
        else if( m_iTestStepLev2 == ETestStepLev2ElectricityCapacitors )
        {
            doTestStepElectricityCapacitors(pTSGrpElectricity);
        }
        else if( m_iTestStepLev2 == ETestStepLev2ElectricityInductors )
        {
            doTestStepElectricityInductors(pTSGrpElectricity);
        }
        else if( m_iTestStepLev2 == ETestStepLev2ElectricityTransistors )
        {
            doTestStepElectricityTransistors(pTSGrpElectricity);
        }
        else if( m_iTestStepLev2 == ETestStepLev2ElectricitySwitches )
        {
            doTestStepElectricitySwitches(pTSGrpElectricity);
        }
        else // start next test step
        {
            pTSGrpElectricity->testEnded();

            m_iTestStepLev1++; // Next test step
            m_iTestStepLev2 = 0;
            triggerDoTestStep();
        }
    } // if( pTSGrpElectricity->isEnabled() )

} // doTestStepElectricity

#endif // #if 0
