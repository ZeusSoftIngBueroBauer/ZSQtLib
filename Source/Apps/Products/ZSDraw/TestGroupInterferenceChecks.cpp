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

#include <QtGui/qevent.h>

#include "Test.h"
#include "MainWindow.h"

#include "ZSDraw/ZSDrawingScene.h"
#include "ZSDraw/ZSDrawingView.h"
#include "ZSDraw/ZSDrawGraphObjRect.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::Draw::Electricity;
using namespace ZS::Draw::QtWidgets;
using namespace ZS::PhysVal;
using namespace ZS::Apps::Products::Draw;
using namespace ZS::Trace;


/*==============================================================================
protected slots: // test step methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepInterferenceChecksI1L1( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_pTestStep->getName(true);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "doTestStepInterferenceChecksI1L1",
        /* strAddInfo   */ strMthInArgs );

    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------


    // Test Step
    //----------

    /* strOperation might be:

       "Rect {899.9e6, 0.21e6, 33.0, 24.028359}",
    */

    QString strOperation = i_pTestStep->getOperation();
    double  fSBarCenterFreq_Hz  = 0.0;
    double  fSBarBandwidth_Hz   = 0.0;
    double  fSBarMaxPeakPwr_dBm = 0.0;
    double  fSBarDynRange_dB    = 0.0;
    bool    bOk = false;

    if( strOperation.contains("Rect") )
    {
        QString strCoors = strOperation;

        strCoors.remove("Rect");
        strCoors.remove("{");
        strCoors.remove("}");

        QStringList strlstCoors = strCoors.split(",");

        if( strlstCoors.size() == 4 )
        {
            fSBarCenterFreq_Hz = strlstCoors[0].toDouble(&bOk);
            if( bOk ) fSBarBandwidth_Hz = strlstCoors[1].toDouble(&bOk);
            if( bOk ) fSBarMaxPeakPwr_dBm = strlstCoors[2].toDouble(&bOk);
            if( bOk ) fSBarDynRange_dB = strlstCoors[3].toDouble(&bOk);
        }
    }

    if( bOk )
    {
        QRectF rectDrawing = m_pDrawingScene->sceneRect();

        double fDrawingFreqRange_Hz   = m_fInterferenceChecksDrawingFreqMax_Hz - m_fInterferenceChecksDrawingFreqMin_Hz;
        double fDrawingLevelRange_dBm = m_fInterferenceChecksDrawingLevelMax_dBm - m_fInterferenceChecksDrawingLevelMin_dBm;

        double fWidth_px  = rectDrawing.width() * fSBarBandwidth_Hz / fDrawingFreqRange_Hz;
        double fHeight_px = rectDrawing.height() * fSBarDynRange_dB / fDrawingLevelRange_dBm;

        double fSBarMin_Hz = fSBarCenterFreq_Hz - 0.5 * fSBarBandwidth_Hz;
        double fLeft_px    = rectDrawing.width() * (fSBarMin_Hz - m_fInterferenceChecksDrawingFreqMin_Hz) / fDrawingFreqRange_Hz;

        double fSBarLevelMin_dBm = fSBarMaxPeakPwr_dBm - fSBarDynRange_dB;
        double fBottom_px        = rectDrawing.height() * (fSBarLevelMin_dBm - m_fInterferenceChecksDrawingLevelMin_dBm) / fDrawingLevelRange_dBm;

        if( (m_pDrawingScene->getEditTool() != EEditToolCreateObjects) || (m_pDrawingScene->getCurrentDrawingToolGraphObjType() != EGraphObjTypeRect) )
        {
            m_pMainWindow->setCheckedActionDrawStandardShapeRect(true);
        }

        CDrawSettings drawSettings(EGraphObjTypeRect);

        CGraphObjRect* pGraphObj = new CGraphObjRect(
            /* pDrawingScene */ m_pDrawingScene,
            /* drawSettings  */ drawSettings,
            /* strObjName    */ "I1L1",
            /* strObjId      */ "I1L1" );

        // Drawing coors are counted from top to bottom:
        fBottom_px = rectDrawing.height() - fBottom_px;
        double fTop_px = fBottom_px - fHeight_px;
        pGraphObj->setRect( QRectF( QPointF(fLeft_px, fTop_px), QSize(fWidth_px, fHeight_px) ) );

        m_pDrawingScene->addItem(pGraphObj);

        m_pDrawingScene->onGraphObjCreated(pGraphObj);

        pGraphObj->acceptCurrentAsOriginalCoors();
    }

    // Actual Values
    //--------------

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues.append("Test Step not implemented");
    }
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepInterferenceChecksI1L1
