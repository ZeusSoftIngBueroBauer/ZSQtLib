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
#include "ZSIpc/ZSIpcClient.h"
#include "ZSIpc/ZSIpcServer.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Apps::Test::Ipc;

#if 0

/*******************************************************************************
class CTest : public Test::CTest
*******************************************************************************/

/*==============================================================================
protected: // instance methods
==============================================================================*/

enum {
    ETestStepCreateServer     =  0,
    ETestStepStartupServer    =  1,
    ETestStepCreateClient     =  2,
    ETestStepInitSettings     =  3,
    ETestStepConnectOk        =  4,
    ETestStepChangePort       =  5,
    ETestStepReconnectTimeout =  6,
    ETestStepResetPort        =  7,
    ETestStepReconnectOk      =  8,
    ETestStepDisconnect       =  9,
    ETestStepDeleteClient     = 10,
    ETestStepShutdownServer   = 11,
    ETestStepDeleteServer     = 12,
    ETestStepCount
};

//------------------------------------------------------------------------------
void CTest::doTestStepGroupClientBlockingCalls( ZS::Test::CTestStepGroup* i_pTestStepGrpParent )
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStepGroup* pTSGrp = getTestStepGroup( QString::number(m_iTestStepGroup) + ".Client.BlockingCalls", i_pTestStepGrpParent );
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
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".Client.BlockingCalls.Server.Create", pTSGrp );
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

        else if( m_iTestStep == ETestStepStartupServer )
        {
            //----------------------------------------------------------------------
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".Client.BlockingCalls.Server.Startup", pTSGrp );
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
                    /*CRequest* pReqStartup =*/ m_pServer->startup(
                        /* iTimeout_ms  */ m_iReqTimeout_ms,
                        /* bWait        */ true,
                        /* iReqIdParent */ -1 );

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

            if( m_hshClientRequestQueueReqsInProgress.isEmpty() )
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
        } // if( m_iTestStep == ETestStepStartupServer )

        else if( m_iTestStep == ETestStepCreateClient )
        {
            //----------------------------------------------------------------------
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + "Client.BlockingCalls.Client.Create", pTSGrp );
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

                m_clientHostSettingsDefault.m_strRemoteHostName = m_serverHostSettings.m_strLocalHostName;
                m_clientHostSettingsDefault.m_uRemotePort = m_serverHostSettings.m_uLocalPort;
                m_clientHostSettingsDefault.m_socketType = m_serverHostSettings.m_socketType;

                m_clientHostSettings = m_clientHostSettingsDefault;
                m_clientWatchDogSettings = m_clientWatchDogSettingsDefault;

                strDesiredValue  = "Unconnected (";
                strDesiredValue += m_clientHostSettings.m_strRemoteHostName;
                strDesiredValue += ":" + QString::number(m_clientHostSettings.m_uRemotePort);
                strDesiredValue += ":" + ZS::Ipc::socketType2Str(m_clientHostSettings.m_socketType);
                strDesiredValue += ":" + ZS::Ipc::blkType2Str(ZS::Ipc::EBlkTypeL) + ")";
                strlstDesiredValues.append(strDesiredValue);
                strDesiredValue.clear();

                strMethod = "new Ipc::Client";
                strArgs = "";
                pTestStep->setOperation( strMethod + "( " + strArgs + " )" );
                pTestStep->setDescription( "" );
                pTestStep->setDesiredValues(strlstDesiredValues);

                pTestStep->testStarted();

                try
                {
                    m_pClient = new ZS::Ipc::CClient(
                        /* strObjName         */ "IpcClient",
                        /* bMultiThreadAccess */ false,
                        /* bTracingEnabled    */ true );

                    m_pClient->setKeepReqDscrInExecTree(true);

                    pTestStep->updateTestEndTime();

                    ZS::Ipc::SClientHostSettings hostSettings = m_pClient->getHostSettings();

                    strActualValue  = ZS::Ipc::CClient::State2Str(m_pClient->getState()) + " (";
                    strActualValue += hostSettings.m_strRemoteHostName;
                    strActualValue += ":" + QString::number(hostSettings.m_uRemotePort);
                    strActualValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
                    if( m_pClient->getBlkType() != nullptr )
                    {
                        strActualValue += ":" + ZS::Ipc::blkType2Str(m_pClient->getBlkType()->type());
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

            if( m_hshClientRequestQueueReqsInProgress.isEmpty() )
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
        } // if( m_iTestStep == ETestStepCreateClient )

        else if( m_iTestStep == ETestStepInitSettings )
        {
            //----------------------------------------------------------------------
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".Client.BlockingCalls.InitSettings", pTSGrp );
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

                m_clientHostSettingsDefault.m_strRemoteHostName = m_serverHostSettings.m_strLocalHostName;
                m_clientHostSettingsDefault.m_uRemotePort = m_serverHostSettings.m_uLocalPort;
                m_clientHostSettingsDefault.m_socketType = m_serverHostSettings.m_socketType;

                m_clientHostSettings = m_clientHostSettingsDefault;
                m_clientWatchDogSettings = m_clientWatchDogSettingsDefault;

                strDesiredValue  = "Unconnected (";
                strDesiredValue += m_clientHostSettings.m_strRemoteHostName;
                strDesiredValue += ":" + QString::number(m_clientHostSettings.m_uRemotePort);
                strDesiredValue += ":" + ZS::Ipc::socketType2Str(m_clientHostSettings.m_socketType);
                strDesiredValue += ":" + ZS::Ipc::blkType2Str(ZS::Ipc::EBlkTypeL) + ")";
                strlstDesiredValues.append(strDesiredValue);
                strDesiredValue.clear();

                strMethod = "Ipc::CClient.changeSettings";
                strArgs = "";
                pTestStep->setOperation( strMethod + "( " + strArgs + " )" );
                pTestStep->setDescription( "" );
                pTestStep->setDesiredValues(strlstDesiredValues);

                pTestStep->testStarted();

                try
                {
                    m_pClient->setBlkType( new ZS::Ipc::CBlkTypeL(ZS::Ipc::EBlkLenDataTypeBinUINT32) );
                    m_pClient->setHostSettings(m_clientHostSettings);

                    m_pClient->changeSettings(
                        /* iTimeout_ms  */ m_iReqTimeout_ms,
                        /* bWait        */ true,
                        /* iReqIdParent */ -1 );

                    pTestStep->updateTestEndTime();

                    ZS::Ipc::SClientHostSettings hostSettings = m_pClient->getHostSettings();

                    strActualValue  = ZS::Ipc::CClient::State2Str(m_pClient->getState()) + " (";
                    strActualValue += hostSettings.m_strRemoteHostName;
                    strActualValue += ":" + QString::number(hostSettings.m_uRemotePort);
                    strActualValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
                    if( m_pClient->getBlkType() != nullptr )
                    {
                        strActualValue += ":" + ZS::Ipc::blkType2Str(m_pClient->getBlkType()->type());
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

        } // if( m_iTestStep == ETestStepInitSettings )

        else if( m_iTestStep == ETestStepConnectOk )
        {
            //----------------------------------------------------------------------
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".Client.BlockingCalls.ConnectOk", pTSGrp );
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

                strDesiredValue  = "Connected (";
                strDesiredValue += m_clientHostSettings.m_strRemoteHostName;
                strDesiredValue += ":" + QString::number(m_clientHostSettings.m_uRemotePort);
                strDesiredValue += ":" + ZS::Ipc::socketType2Str(m_clientHostSettings.m_socketType);
                strDesiredValue += ":" + ZS::Ipc::blkType2Str(ZS::Ipc::EBlkTypeL) + ")";
                strlstDesiredValues.append(strDesiredValue);
                strDesiredValue.clear();

                strMethod = "Ipc::CClient.connect";
                strArgs = "";
                pTestStep->setOperation( strMethod + "( " + strArgs + " )" );
                pTestStep->setDescription( "" );
                pTestStep->setDesiredValues(strlstDesiredValues);

                pTestStep->testStarted();

                try
                {
                    m_pClient->connect_(
                        /* iTimeout_ms  */ m_iReqTimeout_ms,
                        /* bWait        */ true,
                        /* iReqIdParent */ -1 );

                    pTestStep->updateTestEndTime();

                    ZS::Ipc::SSocketDscr socketDscr = m_pClient->getSocketDscr();

                    strActualValue  = ZS::Ipc::socketState2Str(socketDscr.m_socketState) + " (";
                    strActualValue += socketDscr.m_strRemoteHostName;
                    strActualValue += ":" + QString::number(socketDscr.m_uRemotePort);
                    strActualValue += ":" + ZS::Ipc::socketType2Str(socketDscr.m_socketType);
                    if( m_pClient->getBlkType() != nullptr )
                    {
                        strActualValue += ":" + ZS::Ipc::blkType2Str(m_pClient->getBlkType()->type());
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

        } // if( m_iTestStep == ETestStepConnectOk )

        else if( m_iTestStep == ETestStepChangePort )
        {
            //----------------------------------------------------------------------
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".Client.BlockingCalls.ChangePort", pTSGrp );
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

                m_clientHostSettings.m_uRemotePort++;

                strDesiredValue  = "Unconnected (";
                strDesiredValue += m_clientHostSettings.m_strRemoteHostName;
                strDesiredValue += ":" + QString::number(m_clientHostSettings.m_uRemotePort);
                strDesiredValue += ":" + ZS::Ipc::socketType2Str(m_clientHostSettings.m_socketType);
                strDesiredValue += ":" + ZS::Ipc::blkType2Str(ZS::Ipc::EBlkTypeL) + ")";
                strlstDesiredValues.append(strDesiredValue);
                strDesiredValue.clear();

                strMethod = "Ipc::CClient.changeSettings";
                strArgs = "";
                pTestStep->setOperation( strMethod + "( " + strArgs + " )" );
                pTestStep->setDescription( "" );
                pTestStep->setDesiredValues(strlstDesiredValues);

                pTestStep->testStarted();

                try
                {
                    m_pClient->setBlkType( new ZS::Ipc::CBlkTypeL(ZS::Ipc::EBlkLenDataTypeStrBase10UINT32) );
                    m_pClient->setHostSettings(m_clientHostSettings);
                    m_pClient->setWatchDogSettings(m_clientWatchDogSettings);

                    //#pragma message(__TODO__"Reset timeout to 10000 ms")
                    m_pClient->changeSettings(
                        /* iTimeout_ms  */ m_iReqTimeout_ms,
                        /* bWait        */ true,
                        /* iReqIdParent */ -1 );

                    pTestStep->updateTestEndTime();

                    ZS::Ipc::SClientHostSettings hostSettings = m_pClient->getHostSettings();
                    ZS::Ipc::SSocketDscr socketDscr = m_pClient->getSocketDscr();

                    strActualValue  = ZS::Ipc::socketState2Str(socketDscr.m_socketState) + " (";
                    strActualValue += hostSettings.m_strRemoteHostName;
                    strActualValue += ":" + QString::number(hostSettings.m_uRemotePort);
                    strActualValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
                    if( m_pClient->getBlkType() != nullptr )
                    {
                        strActualValue += ":" + ZS::Ipc::blkType2Str(m_pClient->getBlkType()->type());
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

        } // if( m_iTestStep == ETestStepChangePort )

        else if( m_iTestStep == ETestStepReconnectTimeout )
        {
            //----------------------------------------------------------------------
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".Client.BlockingCalls.Reconnect.Timeout", pTSGrp );
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

                strDesiredValue  = "Unconnected (";
                strDesiredValue += m_clientHostSettings.m_strRemoteHostName;
                strDesiredValue += ":" + QString::number(m_clientHostSettings.m_uRemotePort);
                strDesiredValue += ":" + ZS::Ipc::socketType2Str(m_clientHostSettings.m_socketType);
                strDesiredValue += ":" + ZS::Ipc::blkType2Str(ZS::Ipc::EBlkTypeL) + ")";
                strlstDesiredValues.append(strDesiredValue);
                strDesiredValue.clear();

                strMethod = "Ipc::CClient.connect";
                strArgs = "";
                pTestStep->setOperation( strMethod + "( " + strArgs + " )" );
                pTestStep->setDescription( "" );
                pTestStep->setDesiredValues(strlstDesiredValues);

                pTestStep->testStarted();

                try
                {
                    m_pClient->connect_(
                        /* iTimeout_ms  */ m_iReqTimeout_ms,
                        /* bWait        */ true,
                        /* iReqIdParent */ -1 );

                    pTestStep->updateTestEndTime();

                    ZS::Ipc::SClientHostSettings hostSettings = m_pClient->getHostSettings();
                    ZS::Ipc::SSocketDscr socketDscr = m_pClient->getSocketDscr();

                    strActualValue  = ZS::Ipc::socketState2Str(socketDscr.m_socketState) + " (";
                    strActualValue += hostSettings.m_strRemoteHostName;
                    strActualValue += ":" + QString::number(hostSettings.m_uRemotePort);
                    strActualValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
                    if( m_pClient->getBlkType() != nullptr )
                    {
                        strActualValue += ":" + ZS::Ipc::blkType2Str(m_pClient->getBlkType()->type());
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

        } // if( m_iTestStep == ETestStepReconnectTimeout )

        else if( m_iTestStep == ETestStepResetPort )
        {
            //----------------------------------------------------------------------
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".Client.BlockingCalls.ResetPort", pTSGrp );
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

                m_clientHostSettings.m_uRemotePort--;

                strDesiredValue  = "Unconnected (";
                strDesiredValue += m_clientHostSettings.m_strRemoteHostName;
                strDesiredValue += ":" + QString::number(m_clientHostSettings.m_uRemotePort);
                strDesiredValue += ":" + ZS::Ipc::socketType2Str(m_clientHostSettings.m_socketType);
                strDesiredValue += ":" + ZS::Ipc::blkType2Str(ZS::Ipc::EBlkTypeL) + ")";
                strlstDesiredValues.append(strDesiredValue);
                strDesiredValue.clear();

                strMethod = "Ipc::CClient.changeSettings";
                strArgs = "";
                pTestStep->setOperation( strMethod + "( " + strArgs + " )" );
                pTestStep->setDescription( "" );
                pTestStep->setDesiredValues(strlstDesiredValues);

                pTestStep->testStarted();

                try
                {
                    m_pClient->setBlkType( new ZS::Ipc::CBlkTypeL(ZS::Ipc::EBlkLenDataTypeStrBase10UINT32) );
                    m_pClient->setHostSettings(m_clientHostSettings);
                    m_pClient->setWatchDogSettings(m_clientWatchDogSettings);

                    m_pClient->changeSettings(
                        /* iTimeout_ms  */ m_iReqTimeout_ms,
                        /* bWait        */ true,
                        /* iReqIdParent */ -1 );

                    pTestStep->updateTestEndTime();

                    ZS::Ipc::SClientHostSettings hostSettings = m_pClient->getHostSettings();
                    ZS::Ipc::SSocketDscr         socketDscr = m_pClient->getSocketDscr();

                    strActualValue  = ZS::Ipc::socketState2Str(socketDscr.m_socketState) + " (";
                    strActualValue += hostSettings.m_strRemoteHostName;
                    strActualValue += ":" + QString::number(hostSettings.m_uRemotePort);
                    strActualValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
                    if( m_pClient->getBlkType() != nullptr )
                    {
                        strActualValue += ":" + ZS::Ipc::blkType2Str(m_pClient->getBlkType()->type());
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

        } // if( m_iTestStep == ETestStepResetPort )

        else if( m_iTestStep == ETestStepReconnectOk )
        {
            //----------------------------------------------------------------------
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".Client.BlockingCalls.ReconnectOk", pTSGrp );
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

                strDesiredValue  = "Connected (";
                strDesiredValue += m_clientHostSettings.m_strRemoteHostName;
                strDesiredValue += ":" + QString::number(m_clientHostSettings.m_uRemotePort);
                strDesiredValue += ":" + ZS::Ipc::socketType2Str(m_clientHostSettings.m_socketType);
                strDesiredValue += ":" + ZS::Ipc::blkType2Str(ZS::Ipc::EBlkTypeL) + ")";
                strlstDesiredValues.append(strDesiredValue);
                strDesiredValue.clear();

                strMethod = "Ipc::CClient.connect";
                strArgs = "";
                pTestStep->setOperation( strMethod + "( " + strArgs + " )" );
                pTestStep->setDescription( "" );
                pTestStep->setDesiredValues(strlstDesiredValues);

                pTestStep->testStarted();

                try
                {
                    m_pClient->connect_(
                        /* iTimeout_ms  */ m_iReqTimeout_ms,
                        /* bWait        */ true,
                        /* iReqIdParent */ -1 );

                    pTestStep->updateTestEndTime();

                    ZS::Ipc::SSocketDscr socketDscr = m_pClient->getSocketDscr();

                    strActualValue  = ZS::Ipc::socketState2Str(socketDscr.m_socketState) + " (";
                    strActualValue += socketDscr.m_strRemoteHostName;
                    strActualValue += ":" + QString::number(socketDscr.m_uRemotePort);
                    strActualValue += ":" + ZS::Ipc::socketType2Str(socketDscr.m_socketType);
                    if( m_pClient->getBlkType() != nullptr )
                    {
                        strActualValue += ":" + ZS::Ipc::blkType2Str(m_pClient->getBlkType()->type());
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

        } // if( m_iTestStep == ETestStepReconnectOk )

        else if( m_iTestStep == ETestStepDisconnect )
        {
            //----------------------------------------------------------------------
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".Client.BlockingCalls.Disconnect", pTSGrp );
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

                strDesiredValue  = "Unconnected (";
                strDesiredValue += m_clientHostSettings.m_strRemoteHostName;
                strDesiredValue += ":" + QString::number(m_clientHostSettings.m_uRemotePort);
                strDesiredValue += ":" + ZS::Ipc::socketType2Str(m_clientHostSettings.m_socketType);
                strDesiredValue += ":" + ZS::Ipc::blkType2Str(ZS::Ipc::EBlkTypeL) + ")";
                strlstDesiredValues.append(strDesiredValue);
                strDesiredValue.clear();

                strMethod = "Ipc::CClient.disconnect";
                strArgs = "";
                pTestStep->setOperation( strMethod + "( " + strArgs + " )" );
                pTestStep->setDescription( "" );
                pTestStep->setDesiredValues(strlstDesiredValues);

                pTestStep->testStarted();

                try
                {
                    m_pClient->disconnect_(
                        /* iTimeout_ms  */ m_iReqTimeout_ms,
                        /* bWait        */ true,
                        /* iReqIdParent */ -1 );

                    pTestStep->updateTestEndTime();

                    ZS::Ipc::SClientHostSettings hostSettings = m_pClient->getHostSettings();
                    ZS::Ipc::SSocketDscr         socketDscr = m_pClient->getSocketDscr();

                    strActualValue  = ZS::Ipc::socketState2Str(socketDscr.m_socketState) + " (";
                    strActualValue += hostSettings.m_strRemoteHostName;
                    strActualValue += ":" + QString::number(hostSettings.m_uRemotePort);
                    strActualValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
                    if( m_pClient->getBlkType() != nullptr )
                    {
                        strActualValue += ":" + ZS::Ipc::blkType2Str(m_pClient->getBlkType()->type());
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

        } // if( m_iTestStep == ETestStepDisconnect )

        else if( m_iTestStep == ETestStepDeleteClient )
        {
            //----------------------------------------------------------------------
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + "Client.BlockingCalls.Client.Delete", pTSGrp );
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

                strDesiredValue = "Client.Name: nullptr";
                strlstDesiredValues.append(strDesiredValue);
                strDesiredValue.clear();

                strMethod = "delete Ipc::CClient";
                strArgs = "";
                pTestStep->setOperation( strMethod + "( " + strArgs + " )" );
                pTestStep->setDescription( "" );
                pTestStep->setDesiredValues(strlstDesiredValues);

                pTestStep->testStarted();

                try
                {
                    delete m_pClient;
                    m_pClient = nullptr;

                    pTestStep->updateTestEndTime();

                    if( m_pClient == nullptr )
                    {
                        strActualValue = "Client.Name: nullptr";
                    }
                    else
                    {
                        strActualValue = "Client.Name: " + m_pClient->objectName();
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

            if( m_hshClientRequestQueueReqsInProgress.isEmpty() )
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
        } // if( m_iTestStep == ETestStepDeleteClient )

        else if( m_iTestStep == ETestStepShutdownServer )
        {
            //----------------------------------------------------------------------
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".Client.BlockingCalls.Server.Shutdown", pTSGrp );
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
                    /*CRequest* pReqShutdown =*/ m_pServer->shutdown(
                        /* iTimeout_ms  */ m_iReqTimeout_ms,
                        /* bWait        */ true,
                        /* iReqIdParent */ -1 );

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

            if( m_hshClientRequestQueueReqsInProgress.isEmpty() )
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
        } // if( m_iTestStep == ETestStepShutdownServer )

        else if( m_iTestStep == ETestStepDeleteServer )
        {
            //----------------------------------------------------------------------
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".Client.BlockingCalls.Server.Delete", pTSGrp );
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

            if( m_hshClientRequestQueueReqsInProgress.isEmpty() )
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

} // doTestStepGroupClientBlockingCalls

#endif
