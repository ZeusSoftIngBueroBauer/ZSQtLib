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

#include <QtCore/qtimer.h>
#include <QtGui/qevent.h>

#if QT_VERSION < 0x050000
#include <QtGui/qaction.h>
#else
#include <QtWidgets/qaction.h>
#endif

#include "Test.h"
#include "MainWindow.h"

#include "ZSIpc/ZSIpcBlkTypeL.h"
#include "ZSIpc/ZSIpcServer.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Apps::Test::Ipc;

/*******************************************************************************
class CTest : public Test::CTest
*******************************************************************************/

/*==============================================================================
public slots: // test step methods (Group Server - Request Queue)
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepGroupServerRequestQueueCreateServer( ZS::Test::CTestStep* i_pTestStep )
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
        /* strMethod    */ "doTestStepGroupServerRequestQueueCreateServer",
        /* strAddInfo   */ strMthInArgs );

    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    m_serverHostSettings = m_serverHostSettingsDefault;

    strDesiredValue  = "Idle (";
    strDesiredValue += m_serverHostSettings.m_strLocalHostName;
    strDesiredValue += ":" + QString::number(m_serverHostSettings.m_uLocalPort);
    strDesiredValue += ":" + ZS::Ipc::socketType2Str(m_serverHostSettings.m_socketType);
    strDesiredValue += ":" + ZS::Ipc::blkType2Str(ZS::Ipc::EBlkTypeL) + ")";
    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    try
    {
        m_pServer = new ZS::Ipc::CServer("IpcServer", false);

        m_pServer->setKeepReqDscrInExecTree(true);
    }
    catch( CException& exc )
    {
        strActualValue = exception2Str(exc);
        strlstActualValues.append(strActualValue);
    }
    catch(...)
    {
        strActualValue = "Critical Error: Unknown Exception Occurred";
        strlstActualValues.append(strActualValue);
    }

    // Actual Values
    //---------------

    if( m_pServer != nullptr )
    {
        ZS::Ipc::SServerHostSettings hostSettings = m_pServer->getHostSettings();

        strActualValue  = ZS::Ipc::CServer::State2Str(m_pServer->getState()) + " (";
        strActualValue += hostSettings.m_strLocalHostName;
        strActualValue += ":" + QString::number(hostSettings.m_uLocalPort);
        strActualValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
        if( m_pServer->getBlkType() != nullptr )
        {
            strActualValue += ":" + ZS::Ipc::blkType2Str(m_pServer->getBlkType()->type());
        }
        strActualValue += ")";
        strlstActualValues.append(strActualValue);
    }

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues.append("");
    }

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepGroupServerRequestQueueCreateServer

//------------------------------------------------------------------------------
void CTest::doTestStepGroupServerRequestQueueInitSettings( ZS::Test::CTestStep* i_pTestStep )
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
        /* strMethod    */ "doTestStepGroupServerRequestQueueInitSettings",
        /* strAddInfo   */ strMthInArgs );

    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    m_serverHostSettings = m_serverHostSettingsDefault;

    strDesiredValue  = "Idle (";
    strDesiredValue += m_serverHostSettings.m_strLocalHostName;
    strDesiredValue += ":" + QString::number(m_serverHostSettings.m_uLocalPort);
    strDesiredValue += ":" + ZS::Ipc::socketType2Str(m_serverHostSettings.m_socketType);
    strDesiredValue += ":" + ZS::Ipc::blkType2Str(ZS::Ipc::EBlkTypeL) + ")";
    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    CRequest* pReqChangeSettings = nullptr;

    if( m_pServer != nullptr )
    {
        try
        {
            m_pServer->setBlkType( new ZS::Ipc::CBlkTypeL(ZS::Ipc::EBlkLenDataTypeBinUINT32) );
            m_pServer->setHostSettings(m_serverHostSettings);

            pReqChangeSettings = m_pServer->changeSettings(
                /* iTimeout_ms  */ m_iReqTimeout_ms,
                /* bWait        */ true,
                /* iReqIdParent */ -1 );

            if( isAsynchronousRequest(pReqChangeSettings) )
            {
                pReqChangeSettings->setExecutionData( QString::number(pReqChangeSettings->getId()), i_pTestStep );

                m_hshServerRequestQueueReqsInProgress[pReqChangeSettings->getId()] = pReqChangeSettings;

                if( !QObject::connect(
                    /* pObjSender   */ pReqChangeSettings,
                    /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onTestStepGroupServerRequestQueueReqInProgressChanged(ZS::System::SRequestDscr)) ) )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                }
            }
        }
        catch( CException& exc )
        {
            strActualValue = exception2Str(exc);
            strlstActualValues.append(strActualValue);
        }
        catch(...)
        {
            strActualValue = "Critical Error: Unknown Exception Occurred";
            strlstActualValues.append(strActualValue);
        }
    } // if( m_pServer != nullptr )

    // Actual Values
    //---------------

    if( !isAsynchronousRequest(pReqChangeSettings) )
    {
        if( m_pServer != nullptr )
        {
            ZS::Ipc::SServerHostSettings hostSettings = m_pServer->getHostSettings();

            strActualValue  = ZS::Ipc::CServer::State2Str(m_pServer->getState()) + " (";
            strActualValue += hostSettings.m_strLocalHostName;
            strActualValue += ":" + QString::number(hostSettings.m_uLocalPort);
            strActualValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
            if( m_pServer->getBlkType() != nullptr )
            {
                strActualValue += ":" + ZS::Ipc::blkType2Str(m_pServer->getBlkType()->type());
            }
            strActualValue += ")";
            strlstActualValues.append(strActualValue);

        } // if( m_pServer != nullptr )

        // Please note that to finish a test step the list of actual values may not be empty.
        if( strlstActualValues.size() == 0 )
        {
            strlstActualValues.append("");
        }

        i_pTestStep->setActualValues(strlstActualValues);

    } // if( !isAsynchronousRequest(pReqChangeSettings) )

} // doTestStepGroupServerRequestQueueInitSettings

//------------------------------------------------------------------------------
void CTest::doTestStepGroupServerRequestQueueStartup( ZS::Test::CTestStep* i_pTestStep )
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
        /* strMethod    */ "doTestStepGroupServerRequestQueueStartup",
        /* strAddInfo   */ strMthInArgs );

    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strDesiredValue  = "Listening (";
    strDesiredValue += m_serverHostSettings.m_strLocalHostName;
    strDesiredValue += ":" + QString::number(m_serverHostSettings.m_uLocalPort);
    strDesiredValue += ":" + ZS::Ipc::socketType2Str(m_serverHostSettings.m_socketType);
    strDesiredValue += ":" + ZS::Ipc::blkType2Str(ZS::Ipc::EBlkTypeL) + ")";
    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    if( m_pServer != nullptr )
    {
        try
        {
            m_pServer->startup(
                /* iTimeout_ms  */ m_iReqTimeout_ms,
                /* bWait        */ true,
                /* iReqIdParent */ -1 );
        }
        catch( CException& exc )
        {
            strActualValue = exception2Str(exc);
            strlstActualValues.append(strActualValue);
        }
        catch(...)
        {
            strActualValue = "Critical Error: Unknown Exception Occurred";
            strlstActualValues.append(strActualValue);
        }
    } // if( m_pServer != nullptr )

    // Actual Values
    //---------------

    if( m_pServer != nullptr )
    {
        ZS::Ipc::SServerHostSettings hostSettings = m_pServer->getHostSettings();

        strActualValue  = ZS::Ipc::CServer::State2Str(m_pServer->getState()) + " (";
        strActualValue += hostSettings.m_strLocalHostName;
        strActualValue += ":" + QString::number(hostSettings.m_uLocalPort);
        strActualValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
        if( m_pServer->getBlkType() != nullptr )
        {
            strActualValue += ":" + ZS::Ipc::blkType2Str(m_pServer->getBlkType()->type());
        }
        strActualValue += ")";
        strlstActualValues.append(strActualValue);

    } // if( m_pServer != nullptr )

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues.append("");
    }

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepGroupServerRequestQueueStartup

//------------------------------------------------------------------------------
void CTest::doTestStepGroupServerRequestQueueChangePort( ZS::Test::CTestStep* i_pTestStep )
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
        /* strMethod    */ "doTestStepGroupServerRequestQueueChangePort",
        /* strAddInfo   */ strMthInArgs );

    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    m_serverHostSettings.m_uLocalPort++;

    strDesiredValue  = "Listening (";
    strDesiredValue += m_serverHostSettings.m_strLocalHostName;
    strDesiredValue += ":" + QString::number(m_serverHostSettings.m_uLocalPort);
    strDesiredValue += ":" + ZS::Ipc::socketType2Str(m_serverHostSettings.m_socketType);
    strDesiredValue += ":" + ZS::Ipc::blkType2Str(ZS::Ipc::EBlkTypeL) + ")";
    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    if( m_pServer != nullptr )
    {
        try
        {
            m_pServer->setBlkType( new ZS::Ipc::CBlkTypeL(ZS::Ipc::EBlkLenDataTypeStrBase10UINT32) );
            m_pServer->setHostSettings(m_serverHostSettings);

            m_pServer->changeSettings(
                /* iTimeout_ms  */ m_iReqTimeout_ms,
                /* bWait        */ true,
                /* iReqIdParent */ -1 );
        }
        catch( CException& exc )
        {
            strActualValue = exception2Str(exc);
            strlstActualValues.append(strActualValue);
        }
        catch(...)
        {
            strActualValue = "Critical Error: Unknown Exception Occurred";
            strlstActualValues.append(strActualValue);
        }
    } // if( m_pServer != nullptr )

    // Actual Values
    //---------------

    if( m_pServer != nullptr )
    {
        ZS::Ipc::SServerHostSettings hostSettings = m_pServer->getHostSettings();

        strActualValue  = ZS::Ipc::CServer::State2Str(m_pServer->getState()) + " (";
        strActualValue += hostSettings.m_strLocalHostName;
        strActualValue += ":" + QString::number(hostSettings.m_uLocalPort);
        strActualValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
        if( m_pServer->getBlkType() != nullptr )
        {
            strActualValue += ":" + ZS::Ipc::blkType2Str(m_pServer->getBlkType()->type());
        }
        strActualValue += ")";
        strlstActualValues.append(strActualValue);

    } // if( m_pServer != nullptr )

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues.append("");
    }

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepGroupServerRequestQueueChangePort

//------------------------------------------------------------------------------
void CTest::doTestStepGroupServerRequestQueueShutdown( ZS::Test::CTestStep* i_pTestStep )
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
        /* strMethod    */ "doTestStepGroupServerRequestQueueShutdown",
        /* strAddInfo   */ strMthInArgs );

    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strDesiredValue  = "Idle (";
    strDesiredValue += m_serverHostSettings.m_strLocalHostName;
    strDesiredValue += ":" + QString::number(m_serverHostSettings.m_uLocalPort);
    strDesiredValue += ":" + ZS::Ipc::socketType2Str(m_serverHostSettings.m_socketType);
    strDesiredValue += ":" + ZS::Ipc::blkType2Str(ZS::Ipc::EBlkTypeL) + ")";
    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    if( m_pServer != nullptr )
    {
        try
        {
            m_pServer->shutdown(
                /* iTimeout_ms  */ m_iReqTimeout_ms,
                /* bWait        */ true,
                /* iReqIdParent */ -1 );
        }
        catch( CException& exc )
        {
            strActualValue = exception2Str(exc);
            strlstActualValues.append(strActualValue);
        }
        catch(...)
        {
            strActualValue = "Critical Error: Unknown Exception Occurred";
            strlstActualValues.append(strActualValue);
        }
    } // if( m_pServer != nullptr )

    // Actual Values
    //---------------

    if( m_pServer != nullptr )
    {
        ZS::Ipc::SServerHostSettings hostSettings = m_pServer->getHostSettings();

        strActualValue = ZS::Ipc::CServer::State2Str(m_pServer->getState()) + " (";
        strActualValue += hostSettings.m_strLocalHostName;
        strActualValue += ":" + QString::number(hostSettings.m_uLocalPort);
        strActualValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
        if( m_pServer->getBlkType() != nullptr )
        {
            strActualValue += ":" + ZS::Ipc::blkType2Str(m_pServer->getBlkType()->type());
        }
        strActualValue += ")";
        strlstActualValues.append(strActualValue);

    } // if( m_pServer != nullptr )

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues.append("");
    }

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepGroupServerRequestQueueShutdown

//------------------------------------------------------------------------------
void CTest::doTestStepGroupServerRequestQueueDeleteServer( ZS::Test::CTestStep* i_pTestStep )
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
        /* strMethod    */ "doTestStepGroupServerRequestQueueDeleteServer",
        /* strAddInfo   */ strMthInArgs );

    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strDesiredValue = "Server.Name: nullptr";
    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    try
    {
        delete m_pServer;
        m_pServer = nullptr;
    }
    catch( CException& exc )
    {
        strActualValue = exception2Str(exc);
        strlstActualValues.append(strActualValue);
    }
    catch(...)
    {
        strActualValue = "Critical Error: Unknown Exception Occurred";
        strlstActualValues.append(strActualValue);
    }

    // Actual Values
    //---------------

    if( m_pServer == nullptr )
    {
        strActualValue = "Server.Name: nullptr";
        strlstActualValues.append(strActualValue);
    }
    else
    {
        strActualValue = "Server.Name: " + m_pServer->objectName();
        strlstActualValues.append(strActualValue);
    }

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues.append("");
    }

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepGroupServerRequestQueueDeleteServer

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::onTestStepGroupServerRequestQueueReqInProgressChanged( ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    if( m_hshServerRequestQueueReqsInProgress.contains(i_reqDscr.m_iId) )
    {
        CRequest* pReq = m_hshServerRequestQueueReqsInProgress[i_reqDscr.m_iId];

        SErrResultInfo errResultInfo = i_reqDscr.m_errResultInfo;

        // If the request has been finished ...
        if( errResultInfo.isErrorResult() || i_reqDscr.m_iProgress_perCent >= 100 )
        {
            if( errResultInfo.getResult() == EResultUndefined )
            {
                errResultInfo.setErrResult(ErrResultSuccess);
            }

            m_hshServerRequestQueueReqsInProgress.remove(i_reqDscr.m_iId);

            ZS::Test::CTestStepGroup* pTSGrp = getTestStepGroup( QString::number(m_iTestStepGroup) + ".Server.RequestQueue", getTestStepGroupRoot() );
            ZS::Test::CTestStep* pTestStep = reinterpret_cast<ZS::Test::CTestStep*>(pReq->takeExecutionData(QString::number(i_reqDscr.m_iId)));

            if( pTSGrp != nullptr && pTestStep != nullptr )
            {
                QString     strActualValue;
                QStringList strlstActualValues;
                QString     strToolTip;

                ZS::Ipc::SServerHostSettings hostSettings = m_pServer->getHostSettings();

                strActualValue = ZS::Ipc::CServer::State2Str(m_pServer->getState()) + " (";
                strActualValue += hostSettings.m_strLocalHostName;
                strActualValue += ":" + QString::number(hostSettings.m_uLocalPort);
                strActualValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
                if( m_pServer->getBlkType() != nullptr )
                {
                    strActualValue += ":" + ZS::Ipc::blkType2Str(m_pServer->getBlkType()->type());
                }
                strActualValue += ")";
                strlstActualValues.append(strActualValue);

                pTestStep->setActualValues(strlstActualValues);

                //if( m_hshServerRequestQueueReqsInProgress.isEmpty() )
                //{
                //    // Continue with next test step of this group.
                //    triggerDoTestStep();
                //}
            } // if( pTSGrp != nullptr && pTestStep != nullptr )
        } // if( errResultInfo.isErrorResult() || i_reqDscr.m_iProgress_perCent >= 100 )
    } // if( m_hshServerRequestQueueReqsInProgress.contains(i_reqDscr.m_iId) )

} // onTestStepGroupServerRequestQueueReqInProgressChanged

#if 0

/*==============================================================================
protected: // instance methods
==============================================================================*/

enum {
    ETestStepCreateServer     = 0,
    ETestStepInitSettings     = 1,
    ETestStepStartup          = 2,
    ETestStepChangePort       = 3,
    ETestStepShutdown         = 4,
    ETestStepDeleteServer     = 5,
    ETestStepCount
};

//------------------------------------------------------------------------------
void CTest::doTestStepGroupServerRequestQueue( ZS::Test::CTestStepGroup* i_pTestStepGrpParent )
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStepGroup* pTSGrp = getTestStepGroup( QString::number(m_iTestStepGroup) + ".Server.RequestQueue", i_pTestStepGrpParent );
    ZS::Test::CTestStep*      pTestStep;

    if( !pTSGrp->isEnabled() )
    {
        // Continue with next test step group.
        m_iTestStep = 0;
        m_iTestStepGroup++;
        triggerDoTestStep();
    }
    else // if( pTSGrp->isEnabled() )
    {
        ZS::Trace::CMethodTracer methodTracerTestGroup(
            /* pAdminObj    */ m_pTrcAdminObj,
            /* iDetailLevel */ ETraceDetailLevelMethodCalls,
            /* strMethod    */ pTSGrp->getName(),
            /* strAddInfo   */ "" );

        QString     strDesiredValue;
        QStringList strlstDesiredValues;
        QString     strActualValue;
        QStringList strlstActualValues;
        QString     strMethod;
        QString     strArgs;
        QString     strToolTip;

        int iTestStepPrev = m_iTestStep;

        if( m_iTestStep == 0 )
        {
            pTSGrp->testStarted();
        }

        if( m_iTestStep == ETestStepCreateServer )
        {
            //----------------------------------------------------------------------
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".Server.RequestQueue.CreateServer", pTSGrp );
            //----------------------------------------------------------------------

            if( !pTestStep->isEnabled() )
            {
                // Continue with next test step of this group.
                m_iTestStep++;
            }
            else // if( pTestStep->isEnabled() )
            {
                ZS::Trace::CMethodTracer methodTracerTestStep(
                    /* pAdminObj    */ m_pTrcAdminObj,
                    /* iDetailLevel */ ETraceDetailLevelMethodCalls,
                    /* strMethod    */ pTestStep->getName(),
                    /* strAddInfo   */ "" );

                strDesiredValue.clear();
                strlstDesiredValues.clear();
                strActualValue.clear();
                strlstActualValues.clear();

                m_serverHostSettings = m_serverHostSettingsDefault;

                strDesiredValue  = "Idle (";
                strDesiredValue += m_serverHostSettings.m_strLocalHostName;
                strDesiredValue += ":" + QString::number(m_serverHostSettings.m_uLocalPort);
                strDesiredValue += ":" + ZS::Ipc::socketType2Str(m_serverHostSettings.m_socketType);
                strDesiredValue += ":" + ZS::Ipc::blkType2Str(ZS::Ipc::EBlkTypeL) + ")";
                strlstDesiredValues.append(strDesiredValue);
                strDesiredValue.clear();

                strMethod = "new Ipc::CServer";
                strArgs = "";
                pTestStep->setOperation( strMethod + "( " + strArgs + " )" );
                pTestStep->setDescription( "" );
                pTestStep->setDesiredValues(strlstDesiredValues);

                pTestStep->testStarted();

                try
                {
                    m_pServer = new ZS::Ipc::CServer(
                        /* strObjName         */ "IpcServer",
                        /* bMultiThreadAccess */ false,
                        /* bTracingEnabled    */ true );

                    m_pServer->setKeepReqDscrInExecTree(true);

                    pTestStep->updateTestEndTime();

                    ZS::Ipc::SServerHostSettings hostSettings = m_pServer->getHostSettings();

                    strActualValue  = ZS::Ipc::CServer::State2Str(m_pServer->getState()) + " (";
                    strActualValue += hostSettings.m_strLocalHostName;
                    strActualValue += ":" + QString::number(hostSettings.m_uLocalPort);
                    strActualValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
                    if( m_pServer->getBlkType() != nullptr )
                    {
                        strActualValue += ":" + ZS::Ipc::blkType2Str(m_pServer->getBlkType()->type());
                    }
                    strActualValue += ")";
                }
                catch( CException& exc )
                {
                    strActualValue = exception2Str(exc);
                }
                catch(...)
                {
                    strActualValue = "Critical Error: Unknown Exception Occurred";
                }

                strlstActualValues.append(strActualValue);
                strActualValue.clear();

                pTestStep->setActualValues(strlstActualValues);
                pTestStep->setToolTip(strToolTip);
                pTestStep->testEnded();

            } // if( pTestStep->isEnabled() )

            // Continue with next test step of this group.
            m_iTestStep++;

        } // if( m_iTestStep == ETestStepCreateServer )

        else if( m_iTestStep == ETestStepInitSettings )
        {
            //----------------------------------------------------------------------
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".Server.RequestQueue.InitSettings", pTSGrp );
            //----------------------------------------------------------------------

            if( !pTestStep->isEnabled() )
            {
                // Continue with next test step of this group.
                m_iTestStep++;
            }
            else // if( pTestStep->isEnabled() )
            {
                ZS::Trace::CMethodTracer methodTracerTestStep(
                    /* pAdminObj    */ m_pTrcAdminObj,
                    /* iDetailLevel */ ETraceDetailLevelMethodCalls,
                    /* strMethod    */ pTestStep->getName(),
                    /* strAddInfo   */ "" );

                strDesiredValue.clear();
                strlstDesiredValues.clear();
                strActualValue.clear();
                strlstActualValues.clear();

                m_serverHostSettings = m_serverHostSettingsDefault;

                strDesiredValue  = "Idle (";
                strDesiredValue += m_serverHostSettings.m_strLocalHostName;
                strDesiredValue += ":" + QString::number(m_serverHostSettings.m_uLocalPort);
                strDesiredValue += ":" + ZS::Ipc::socketType2Str(m_serverHostSettings.m_socketType);
                strDesiredValue += ":" + ZS::Ipc::blkType2Str(ZS::Ipc::EBlkTypeL) + ")";
                strlstDesiredValues.append(strDesiredValue);
                strDesiredValue.clear();

                strMethod = "Ipc::CServer.changeSettings";
                strArgs = "";
                pTestStep->setOperation( strMethod + "( " + strArgs + " )" );
                pTestStep->setDescription( "" );
                pTestStep->setDesiredValues(strlstDesiredValues);

                pTestStep->testStarted();

                try
                {
                    m_pServer->setBlkType( new ZS::Ipc::CBlkTypeL(ZS::Ipc::EBlkLenDataTypeBinUINT32) );
                    m_pServer->setHostSettings(m_serverHostSettings);

                    CRequest* pReqChangeSettings = m_pServer->changeSettings();

                    if( isAsynchronousRequest(pReqChangeSettings) )
                    {
                        pReqChangeSettings->setExecutionData( QString::number(pReqChangeSettings->getId()), pTestStep );

                        m_hshServerRequestQueueReqsInProgress[pReqChangeSettings->getId()] = pReqChangeSettings;

                        if( !QObject::connect(
                            /* pObjSender   */ pReqChangeSettings,
                            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                            /* pObjReceiver */ this,
                            /* szSlot       */ SLOT(onTestStepGroupServerRequestQueueReqInProgressChanged(ZS::System::SRequestDscr)) ) )
                        {
                            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                        }
                    }
                    else
                    {
                        pTestStep->updateTestEndTime();

                        ZS::Ipc::SServerHostSettings hostSettings = m_pServer->getHostSettings();

                        strActualValue  = ZS::Ipc::CServer::State2Str(m_pServer->getState()) + " (";
                        strActualValue += hostSettings.m_strLocalHostName;
                        strActualValue += ":" + QString::number(hostSettings.m_uLocalPort);
                        strActualValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
                        if( m_pServer->getBlkType() != nullptr )
                        {
                            strActualValue += ":" + ZS::Ipc::blkType2Str(m_pServer->getBlkType()->type());
                        }
                        strActualValue += ")";
                    }
                }
                catch( CException& exc )
                {
                    strActualValue = exception2Str(exc);
                }
                catch(...)
                {
                    strActualValue = "Critical Error: Unknown Exception Occurred";
                }

                if( !strActualValue.isEmpty() )
                {
                    strlstActualValues.append(strActualValue);
                    strActualValue.clear();

                    pTestStep->setActualValues(strlstActualValues);
                    pTestStep->setToolTip(strToolTip);
                    pTestStep->testEnded();
                }
            } // if( pTestStep->isEnabled() )

            if( m_hshServerRequestQueueReqsInProgress.isEmpty() )
            {
                // Continue with next test step of this group.
                m_iTestStep++;
            }
            else
            {
                // Don't continue with next test step of this group but
                // wait unit all requests have been finished.
                //m_iTestStep++;
            }
        } // if( m_iTestStep == ETestStepInitSettings )

        else if( m_iTestStep == ETestStepStartup )
        {
            //----------------------------------------------------------------------
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".Server.RequestQueue.Startup", pTSGrp );
            //----------------------------------------------------------------------

            if( !pTestStep->isEnabled() )
            {
                // Continue with next test step of this group.
                m_iTestStep++;
            }
            else // if( pTestStep->isEnabled() )
            {
                ZS::Trace::CMethodTracer methodTracerTestStep(
                    /* pAdminObj    */ m_pTrcAdminObj,
                    /* iDetailLevel */ ETraceDetailLevelMethodCalls,
                    /* strMethod    */ pTestStep->getName(),
                    /* strAddInfo   */ "" );

                strDesiredValue.clear();
                strlstDesiredValues.clear();
                strActualValue.clear();
                strlstActualValues.clear();

                strDesiredValue  = "Listening (";
                strDesiredValue += m_serverHostSettings.m_strLocalHostName;
                strDesiredValue += ":" + QString::number(m_serverHostSettings.m_uLocalPort);
                strDesiredValue += ":" + ZS::Ipc::socketType2Str(m_serverHostSettings.m_socketType);
                strDesiredValue += ":" + ZS::Ipc::blkType2Str(ZS::Ipc::EBlkTypeL) + ")";
                strlstDesiredValues.append(strDesiredValue);
                strDesiredValue.clear();

                strMethod = "Ipc::CServer.startup";
                strArgs = "";
                pTestStep->setOperation( strMethod + "( " + strArgs + " )" );
                pTestStep->setDescription( "" );
                pTestStep->setDesiredValues(strlstDesiredValues);

                pTestStep->testStarted();

                try
                {
                    CRequest* pReqStartup = m_pServer->startup();

                    if( isAsynchronousRequest(pReqStartup) )
                    {
                        pReqStartup->setExecutionData( QString::number(pReqStartup->getId()), pTestStep );

                        m_hshServerRequestQueueReqsInProgress[pReqStartup->getId()] = pReqStartup;

                        if( !QObject::connect(
                            /* pObjSender   */ pReqStartup,
                            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                            /* pObjReceiver */ this,
                            /* szSlot       */ SLOT(onTestStepGroupServerRequestQueueReqInProgressChanged(ZS::System::SRequestDscr)) ) )
                        {
                            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                        }
                    }
                    else
                    {
                        pTestStep->updateTestEndTime();

                        ZS::Ipc::SServerHostSettings hostSettings = m_pServer->getHostSettings();

                        strActualValue  = ZS::Ipc::CServer::State2Str(m_pServer->getState()) + " (";
                        strActualValue += hostSettings.m_strLocalHostName;
                        strActualValue += ":" + QString::number(hostSettings.m_uLocalPort);
                        strActualValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
                        if( m_pServer->getBlkType() != nullptr )
                        {
                            strActualValue += ":" + ZS::Ipc::blkType2Str(m_pServer->getBlkType()->type());
                        }
                        strActualValue += ")";
                    }
                }
                catch( CException& exc )
                {
                    strActualValue = exception2Str(exc);
                }
                catch(...)
                {
                    strActualValue = "Critical Error: Unknown Exception Occurred";
                }

                if( !strActualValue.isEmpty() )
                {
                    strlstActualValues.append(strActualValue);
                    strActualValue.clear();

                    pTestStep->setActualValues(strlstActualValues);
                    pTestStep->setToolTip(strToolTip);
                    pTestStep->testEnded();
                }
            } // if( pTestStep->isEnabled() )

            if( m_hshServerRequestQueueReqsInProgress.isEmpty() )
            {
                // Continue with next test step of this group.
                m_iTestStep++;
            }
            else
            {
                // Don't continue with next test step of this group but
                // wait unit all requests have been finished.
                //m_iTestStep++;
            }
        } // if( m_iTestStep == ETestStepStartup )

        else if( m_iTestStep == ETestStepChangePort )
        {
            //----------------------------------------------------------------------
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".Server.RequestQueue.ChangePort", pTSGrp );
            //----------------------------------------------------------------------

            if( !pTestStep->isEnabled() )
            {
                // Continue with next test step of this group.
                m_iTestStep++;
            }
            else // if( pTestStep->isEnabled() )
            {
                ZS::Trace::CMethodTracer methodTracerTestStep(
                    /* pAdminObj    */ m_pTrcAdminObj,
                    /* iDetailLevel */ ETraceDetailLevelMethodCalls,
                    /* strMethod    */ pTestStep->getName(),
                    /* strAddInfo   */ "" );

                strDesiredValue.clear();
                strlstDesiredValues.clear();
                strActualValue.clear();
                strlstActualValues.clear();

                m_serverHostSettings.m_uLocalPort++;

                strDesiredValue  = "Listening (";
                strDesiredValue += m_serverHostSettings.m_strLocalHostName;
                strDesiredValue += ":" + QString::number(m_serverHostSettings.m_uLocalPort);
                strDesiredValue += ":" + ZS::Ipc::socketType2Str(m_serverHostSettings.m_socketType);
                strDesiredValue += ":" + ZS::Ipc::blkType2Str(ZS::Ipc::EBlkTypeL) + ")";
                strlstDesiredValues.append(strDesiredValue);
                strDesiredValue.clear();

                strMethod = "Ipc::CServer.changeSettings";
                strArgs = "";
                pTestStep->setOperation( strMethod + "( " + strArgs + " )" );
                pTestStep->setDescription( "" );
                pTestStep->setDesiredValues(strlstDesiredValues);

                pTestStep->testStarted();

                try
                {
                    m_pServer->setBlkType( new ZS::Ipc::CBlkTypeL(ZS::Ipc::EBlkLenDataTypeStrBase10UINT32) );
                    m_pServer->setHostSettings(m_serverHostSettings);

                    CRequest* pReqChangeSettings = m_pServer->changeSettings();

                    if( isAsynchronousRequest(pReqChangeSettings) )
                    {
                        pReqChangeSettings->setExecutionData( QString::number(pReqChangeSettings->getId()), pTestStep );

                        m_hshServerRequestQueueReqsInProgress[pReqChangeSettings->getId()] = pReqChangeSettings;

                        if( !QObject::connect(
                            /* pObjSender   */ pReqChangeSettings,
                            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                            /* pObjReceiver */ this,
                            /* szSlot       */ SLOT(onTestStepGroupServerRequestQueueReqInProgressChanged(ZS::System::SRequestDscr)) ) )
                        {
                            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                        }
                    }
                    else
                    {
                        pTestStep->updateTestEndTime();

                        ZS::Ipc::SServerHostSettings hostSettings = m_pServer->getHostSettings();

                        strActualValue  = ZS::Ipc::CServer::State2Str(m_pServer->getState()) + " (";
                        strActualValue += hostSettings.m_strLocalHostName;
                        strActualValue += ":" + QString::number(hostSettings.m_uLocalPort);
                        strActualValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
                        if( m_pServer->getBlkType() != nullptr )
                        {
                            strActualValue += ":" + ZS::Ipc::blkType2Str(m_pServer->getBlkType()->type());
                        }
                        strActualValue += ")";
                    }
                }
                catch( CException& exc )
                {
                    strActualValue = exception2Str(exc);
                }
                catch(...)
                {
                    strActualValue = "Critical Error: Unknown Exception Occurred";
                }

                if( !strActualValue.isEmpty() )
                {
                    strlstActualValues.append(strActualValue);
                    strActualValue.clear();

                    pTestStep->setActualValues(strlstActualValues);
                    pTestStep->setToolTip(strToolTip);
                    pTestStep->testEnded();
                }
            } // if( pTestStep->isEnabled() )

            if( m_hshServerRequestQueueReqsInProgress.isEmpty() )
            {
                // Continue with next test step of this group.
                m_iTestStep++;
            }
            else
            {
                // Don't continue with next test step of this group but
                // wait unit all requests have been finished.
                //m_iTestStep++;
            }
        } // if( m_iTestStep == ETestStepChangePort )

        else if( m_iTestStep == ETestStepShutdown )
        {
            //----------------------------------------------------------------------
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".Server.RequestQueue.Shutdown", pTSGrp );
            //----------------------------------------------------------------------

            if( !pTestStep->isEnabled() )
            {
                // Continue with next test step of this group.
                m_iTestStep++;
            }
            else // if( pTestStep->isEnabled() )
            {
                ZS::Trace::CMethodTracer methodTracerTestStep(
                    /* pAdminObj    */ m_pTrcAdminObj,
                    /* iDetailLevel */ ETraceDetailLevelMethodCalls,
                    /* strMethod    */ pTestStep->getName(),
                    /* strAddInfo   */ "" );

                strDesiredValue.clear();
                strlstDesiredValues.clear();
                strActualValue.clear();
                strlstActualValues.clear();

                strDesiredValue  = "Idle (";
                strDesiredValue += m_serverHostSettings.m_strLocalHostName;
                strDesiredValue += ":" + QString::number(m_serverHostSettings.m_uLocalPort);
                strDesiredValue += ":" + ZS::Ipc::socketType2Str(m_serverHostSettings.m_socketType);
                strDesiredValue += ":" + ZS::Ipc::blkType2Str(ZS::Ipc::EBlkTypeL) + ")";
                strlstDesiredValues.append(strDesiredValue);
                strDesiredValue.clear();

                strMethod = "Ipc::CServer.shutdown";
                strArgs = "";
                pTestStep->setOperation( strMethod + "( " + strArgs + " )" );
                pTestStep->setDescription( "" );
                pTestStep->setDesiredValues(strlstDesiredValues);

                pTestStep->testStarted();

                try
                {
                    CRequest* pReqShutdown = m_pServer->shutdown();

                    if( isAsynchronousRequest(pReqShutdown) )
                    {
                        pReqShutdown->setExecutionData( QString::number(pReqShutdown->getId()), pTestStep );

                        m_hshServerRequestQueueReqsInProgress[pReqShutdown->getId()] = pReqShutdown;

                        if( !QObject::connect(
                            /* pObjSender   */ pReqShutdown,
                            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                            /* pObjReceiver */ this,
                            /* szSlot       */ SLOT(onTestStepGroupServerRequestQueueReqInProgressChanged(ZS::System::SRequestDscr)) ) )
                        {
                            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                        }
                    }
                    else
                    {
                        pTestStep->updateTestEndTime();

                        ZS::Ipc::SServerHostSettings hostSettings = m_pServer->getHostSettings();

                        strActualValue = ZS::Ipc::CServer::State2Str(m_pServer->getState()) + " (";
                        strActualValue += hostSettings.m_strLocalHostName;
                        strActualValue += ":" + QString::number(hostSettings.m_uLocalPort);
                        strActualValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
                        if( m_pServer->getBlkType() != nullptr )
                        {
                            strActualValue += ":" + ZS::Ipc::blkType2Str(m_pServer->getBlkType()->type());
                        }
                        strActualValue += ")";
                    }
                }
                catch( CException& exc )
                {
                    strActualValue = exception2Str(exc);
                }
                catch(...)
                {
                    strActualValue = "Critical Error: Unknown Exception Occurred";
                }

                if( !strActualValue.isEmpty() )
                {
                    strlstActualValues.append(strActualValue);
                    strActualValue.clear();

                    pTestStep->setActualValues(strlstActualValues);
                    pTestStep->setToolTip(strToolTip);
                    pTestStep->testEnded();
                }
            } // if( pTestStep->isEnabled() )

            if( m_hshServerRequestQueueReqsInProgress.isEmpty() )
            {
                // Continue with next test step of this group.
                m_iTestStep++;
            }
            else
            {
                // Don't continue with next test step of this group but
                // wait unit all requests have been finished.
                //m_iTestStep++;
            }
        } // if( m_iTestStep == ETestStepShutdown )

        else if( m_iTestStep == ETestStepDeleteServer )
        {
            //----------------------------------------------------------------------
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".Server.RequestQueue.DeleteServer", pTSGrp );
            //----------------------------------------------------------------------

            if( !pTestStep->isEnabled() )
            {
                // Continue with next test step of this group.
                m_iTestStep++;
            }
            else // if( pTestStep->isEnabled() )
            {
                ZS::Trace::CMethodTracer methodTracerTestStep(
                    /* pAdminObj    */ m_pTrcAdminObj,
                    /* iDetailLevel */ ETraceDetailLevelMethodCalls,
                    /* strMethod    */ pTestStep->getName(),
                    /* strAddInfo   */ "" );

                strDesiredValue.clear();
                strlstDesiredValues.clear();
                strActualValue.clear();
                strlstActualValues.clear();

                strDesiredValue = "Server.Name: nullptr";
                strlstDesiredValues.append(strDesiredValue);
                strDesiredValue.clear();

                strMethod = "delete Ipc::CServer";
                strArgs = "";
                pTestStep->setOperation( strMethod + "( " + strArgs + " )" );
                pTestStep->setDescription( "" );
                pTestStep->setDesiredValues(strlstDesiredValues);

                pTestStep->testStarted();

                try
                {
                    delete m_pServer;
                    m_pServer = nullptr;

                    pTestStep->updateTestEndTime();

                    if( m_pServer == nullptr )
                    {
                        strActualValue = "Server.Name: nullptr";
                    }
                    else
                    {
                        strActualValue = "Server.Name: " + m_pServer->objectName();
                    }
                }
                catch( CException& exc )
                {
                    strActualValue = exception2Str(exc);
                }
                catch(...)
                {
                    strActualValue = "Critical Error: Unknown Exception Occurred";
                }

                if( !strActualValue.isEmpty() )
                {
                    strlstActualValues.append(strActualValue);
                    strActualValue.clear();

                    pTestStep->setActualValues(strlstActualValues);
                    pTestStep->setToolTip(strToolTip);
                    pTestStep->testEnded();
                }
            } // if( pTestStep->isEnabled() )

            if( m_hshServerRequestQueueReqsInProgress.isEmpty() )
            {
                // Continue with next test step of this group.
                m_iTestStep++;
            }
            else
            {
                // Don't continue with next test step of this group but
                // wait unit all requests have been finished.
                //m_iTestStep++;
            }
        } // if( m_iTestStep == ETestStepDeleteServer )

        if( m_iTestStep == ETestStepCount )
        {
            pTSGrp->testEnded();

            // Continue with next test step group.
            m_iTestStep = 0;
            m_iTestStepGroup++;
            triggerDoTestStep();
        }
        else if( iTestStepPrev != m_iTestStep )
        {
            triggerDoTestStep();
        }

    } // if( pTSGrp->isEnabled() )

} // doTestStepGroupServerRequestQueue

#endif
