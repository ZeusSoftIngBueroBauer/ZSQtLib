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

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Apps::Test::Ipc;

#if 0

/*******************************************************************************
class CTest : public Test::CTest
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::FctDoTestStepGroupSendDataTcpBlkTypeL( ZS::Test::CTest* i_pThis, ZS::Test::CTestStepGroup* i_pTestStepGrpParent )
//------------------------------------------------------------------------------
{
    dynamic_cast<CTest*>(i_pThis)->doTestStepGroupSendDataTcpBlkTypeL(i_pTestStepGrpParent);
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

enum {
    ETestStepCreateServer       =  0,
    ETestStepStartupServer      =  1,
    ETestStepCreateClient       =  2,
    ETestStepConnectClient      =  3,
    ETestStepSetBlkTypeServer   =  4,
    ETestStepSetBlkTypeClient   =  5,
    ETestStepSendDataCltSrv     =  6,
    ETestStepSendDataSrvClt     =  7,
    ETestStepDeleteServer       =  8,
    ETestStepClientDisconnected =  9,
    ETestStepDeleteClient       = 10,
    ETestStepCount
};

//------------------------------------------------------------------------------
void CTest::doTestStepGroupSendDataTcpBlkTypeL( ZS::Test::CTestStepGroup* i_pTestStepGrpParent )
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStepGroup* pTSGrp = getTestStepGroup( QString::number(m_iTestStepGroup) + ".SendData.Tcp.BlkType.L", i_pTestStepGrpParent );
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
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".SendData.Tcp.BlkType.L.Server.Create", pTSGrp );
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
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".SendData.Tcp.BlkType.L.Server.Startup", pTSGrp );
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
                    m_pServer->startup(
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

            // Continue with next test step of this group.
            m_iTestStep++;

        } // if( m_iTestStep == ETestStepStartupServer )

        else if( m_iTestStep == ETestStepCreateClient )
        {
            //----------------------------------------------------------------------
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".SendData.Tcp.BlkType.L.Client.Create", pTSGrp );
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

            // Continue with next test step of this group.
            m_iTestStep++;

        } // if( m_iTestStep == ETestStepCreateClient )

        else if( m_iTestStep == ETestStepConnectClient )
        {
            //----------------------------------------------------------------------
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".SendData.Tcp.BlkType.L.Client.Connect", pTSGrp );
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

        } // if( m_iTestStep == ETestStepCreateClient )

        else if( m_iTestStep == ETestStepSetBlkTypeServer )
        {
            //----------------------------------------------------------------------
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".SendData.Tcp.BlkType.L.SetBlkType.Server", pTSGrp );
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

                strMethod = "Ipc::CServer.setBlkType";
                strArgs = "L";
                pTestStep->setOperation( strMethod + "( " + strArgs + " )" );
                pTestStep->setDescription( "" );
                pTestStep->setDesiredValues(strlstDesiredValues);

                pTestStep->testStarted();

                try
                {
                    ZS::Ipc::CBlkType* pBlkType = new ZS::Ipc::CBlkTypeL(
                        /* blkLenDataType      */ ZS::Ipc::EBlkLenDataTypeBinUINT32,
                        /* bInsertBlkEnd       */ true,
                        /* bRemoveBlkEnd       */ true,
                        /* iBlkLenDataTypeSize */ -1 );
                    m_pServer->setBlkType(pBlkType);
                    pBlkType = nullptr;

                    m_pServer->changeSettings(
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

            // Continue with next test step of this group.
            m_iTestStep++;

        } // if( m_iTestStep == ETestStepSetBlkTypeServer )

        else if( m_iTestStep == ETestStepSetBlkTypeClient )
        {
            //----------------------------------------------------------------------
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".SendData.Tcp.BlkType.L.SetBlkType.Client", pTSGrp );
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

                strDesiredValue  = "Connected (";
                strDesiredValue += m_clientHostSettings.m_strRemoteHostName;
                strDesiredValue += ":" + QString::number(m_clientHostSettings.m_uRemotePort);
                strDesiredValue += ":" + ZS::Ipc::socketType2Str(m_clientHostSettings.m_socketType);
                strDesiredValue += ":" + ZS::Ipc::blkType2Str(ZS::Ipc::EBlkTypeL) + ")";
                strlstDesiredValues.append(strDesiredValue);
                strDesiredValue.clear();

                strMethod = "Ipc::Client.setBlkType";
                strArgs = "L";
                pTestStep->setOperation( strMethod + "( " + strArgs + " )" );
                pTestStep->setDescription( "" );
                pTestStep->setDesiredValues(strlstDesiredValues);

                pTestStep->testStarted();

                try
                {
                    ZS::Ipc::CBlkType* pBlkType = new ZS::Ipc::CBlkTypeL(
                        /* blkLenDataType      */ ZS::Ipc::EBlkLenDataTypeBinUINT32,
                        /* bInsertBlkEnd       */ true,
                        /* bRemoveBlkEnd       */ true,
                        /* iBlkLenDataTypeSize */ -1 );
                    m_pClient->setBlkType(pBlkType);
                    pBlkType = nullptr;

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

        } // if( m_iTestStep == ETestStepSetBlkTypeClient )

        else if( m_iTestStep == ETestStepSendDataCltSrv )
        {
            //----------------------------------------------------------------------
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".SendData.Tcp.BlkType.L.SendData.CltSrv", pTSGrp );
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

                //CBlkTypeL(
                //    /* blkLenDataType      */ ZS::Ipc::EBlkLenDataTypeBinUINT32,
                //    /* bInsertBlkEnd       */ true,
                //    /* bRemoveBlkEnd       */ true,
                //    /* iBlkLenDataTypeSize */ -1 );

                QByteArray byteArrValue(1024,'.');

                byteArrValue.replace(0,12,"Hello Server");

                strDesiredValue = byteArrValue.left(12);
                strlstDesiredValues.append(strDesiredValue);
                strDesiredValue.clear();

                strMethod = "Ipc::Client.sendData";
                strArgs = QString(byteArrValue).left(12);
                pTestStep->setOperation( strMethod + "( " + strArgs + " )" );
                pTestStep->setDescription( "" );
                pTestStep->setDesiredValues(strlstDesiredValues);

                pTestStep->testStarted();

                try
                {
                    if( !QObject::connect(
                        /* pObjSender   */ m_pServer,
                        /* szSignal     */ SIGNAL(receivedData(QObject*,int,const QByteArray&)),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT(onTestStepGroupSendDataTcpBlkTypeLServerReceivedData(QObject*,int,const QByteArray&)) ) )
                    {
                        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                    }

                    m_pClient->sendData(byteArrValue);
                }
                catch( CException& exc )
                {
                    strActualValue = exception2Str(exc);
                    m_iTestStep++;
                }
                catch(...)
                {
                    strActualValue = "Critical Error: Unknown Exception Occurred";
                    m_iTestStep++;
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
        } // if( m_iTestStep == ETestStepSendDataCltSrv )

        else if( m_iTestStep == ETestStepSendDataSrvClt )
        {
            //----------------------------------------------------------------------
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".SendData.Tcp.BlkType.L.SendData.SrvClt", pTSGrp );
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

                //CBlkTypeL(
                //    /* blkLenDataType      */ ZS::Ipc::EBlkLenDataTypeBinUINT32,
                //    /* bInsertBlkEnd       */ true,
                //    /* bRemoveBlkEnd       */ true,
                //    /* iBlkLenDataTypeSize */ -1 );

                QByteArray byteArrValue(1024,'.');

                byteArrValue.replace(0,12,"Hello Client");

                strDesiredValue = byteArrValue.left(12);
                strlstDesiredValues.append(strDesiredValue);
                strDesiredValue.clear();

                strMethod = "Ipc::Server.sendData";
                strArgs = QString(byteArrValue).left(12);
                pTestStep->setOperation( strMethod + "( " + strArgs + " )" );
                pTestStep->setDescription( "" );
                pTestStep->setDesiredValues(strlstDesiredValues);

                pTestStep->testStarted();

                try
                {
                    if( !QObject::connect(
                        /* pObjSender   */ m_pClient,
                        /* szSignal     */ SIGNAL(receivedData(QObject*,const QByteArray&)),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT(onTestStepGroupSendDataTcpBlkTypeLClientReceivedData(QObject*,const QByteArray&)) ) )
                    {
                        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                    }

                    m_pServer->sendData( ZS::Ipc::ESocketIdAllSockets, byteArrValue );
                }
                catch( CException& exc )
                {
                    strActualValue = exception2Str(exc);
                    m_iTestStep++;
                }
                catch(...)
                {
                    strActualValue = "Critical Error: Unknown Exception Occurred";
                    m_iTestStep++;
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
        } // if( m_iTestStep == ETestStepSendDataSrvClt )

        else if( m_iTestStep == ETestStepDeleteServer )
        {
            //----------------------------------------------------------------------
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".SendData.Tcp.BlkType.L.Server.Delete", pTSGrp );
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

            // Continue with next test step of this group.
            m_iTestStep++;

        } // if( m_iTestStep == ETestStepServerDelete )

        else if( m_iTestStep == ETestStepClientDisconnected )
        {
            //----------------------------------------------------------------------
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".SendData.Tcp.BlkType.L.Client.Disconnected", pTSGrp );
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

                strMethod = "Ipc::CClient::disconnected";
                strArgs = "";
                pTestStep->setOperation( strMethod + "( " + strArgs + " )" );
                pTestStep->setDescription( "" );
                pTestStep->setDesiredValues(strlstDesiredValues);

                pTestStep->testStarted();

                try
                {
                    if( !QObject::connect(
                        /* pObjSender   */ m_pClient,
                        /* szSignal     */ SIGNAL(disconnected(QObject*)),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT(onTestStepGroupSendDataTcpBlkTypeLClientDisconnected(QObject*)) ) )
                    {
                        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                    }
                    if( m_pClient->getState() == ZS::Ipc::CClient::EStateUnconnected )
                    {
                        onTestStepGroupSendDataTcpBlkTypeLClientDisconnected(m_pClient);
                    }
                }
                catch( CException& exc )
                {
                    strActualValue = exception2Str(exc);
                    m_iTestStep++;
                }
                catch(...)
                {
                    strActualValue = "Critical Error: Unknown Exception Occurred";
                    m_iTestStep++;
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
        } // if( m_iTestStep == ETestStepClientDisconnected )

        else if( m_iTestStep == ETestStepDeleteClient )
        {
            //----------------------------------------------------------------------
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".SendData.Tcp.BlkType.L.Client.Delete", pTSGrp );
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

            // Continue with next test step of this group.
            m_iTestStep++;

        } // if( m_iTestStep == ETestStepDeleteClient )

        else // should never be the case
        {
            m_iTestStep++;
        }

        if( m_iTestStep >= ETestStepCount )
        {
            pTSGrp->testEnded();

            // Continue with next test step group.
            m_iTestStep = 0;
            m_iTestStepGroup++;
        }

        if( iTestStepPrev != m_iTestStep )
        {
            triggerDoTestStep();
        }

    } // if( pTSGrp->isEnabled() )

} // doTestStepGroupSendDataTcpBlkTypeL

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::onTestStepGroupSendDataTcpBlkTypeLServerReceivedData(
    QObject*          /*i_pServer*/,
    int               /*i_iSocketId*/,
    const QByteArray& i_byteArr )
//------------------------------------------------------------------------------
{
    if( !QObject::disconnect(
        /* pObjSender   */ m_pServer,
        /* szSignal     */ SIGNAL(receivedData(QObject*,int,const QByteArray&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTestStepGroupSendDataTcpBlkTypeLServerReceivedData(QObject*,int,const QByteArray&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    ZS::Test::CTestStepGroup* pTSGrp = getTestStepGroup( QString::number(m_iTestStepGroup) + ".SendData.Tcp.BlkType.L", getTestStepGroupRoot() );
    ZS::Test::CTestStep* pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(ETestStepSendDataCltSrv) + ".SendData.Tcp.BlkType.L.SendData.CltSrv", pTSGrp );

    QString     strActualValue;
    QStringList strlstActualValues;
    QString     strMethod;
    QString     strArgs;
    QString     strToolTip;

    pTestStep->updateTestEndTime();

    strActualValue = i_byteArr.left(12);

    strlstActualValues.append(strActualValue);
    strActualValue.clear();

    pTestStep->setActualValues(strlstActualValues);
    pTestStep->setToolTip(strToolTip);
    pTestStep->testEnded();

    // Continue with next test step of this group.
    m_iTestStep++;
    triggerDoTestStep();

} // onTestStepGroupSendDataTcpBlkTypeLServerReceivedData

//------------------------------------------------------------------------------
void CTest::onTestStepGroupSendDataTcpBlkTypeLClientReceivedData(
    QObject*          /*i_pClient*/,
    const QByteArray& i_byteArr )
//------------------------------------------------------------------------------
{
    if( !QObject::disconnect(
        /* pObjSender   */ m_pClient,
        /* szSignal     */ SIGNAL(receivedData(QObject*,const QByteArray&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTestStepGroupSendDataTcpBlkTypeLClientReceivedData(QObject*,const QByteArray&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    ZS::Test::CTestStepGroup* pTSGrp = getTestStepGroup( QString::number(m_iTestStepGroup) + ".SendData.Tcp.BlkType.L", getTestStepGroupRoot() );
    ZS::Test::CTestStep* pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(ETestStepSendDataSrvClt) + ".SendData.Tcp.BlkType.L.SendData.SrvClt", pTSGrp );

    QString     strActualValue;
    QStringList strlstActualValues;
    QString     strMethod;
    QString     strArgs;
    QString     strToolTip;

    pTestStep->updateTestEndTime();

    strActualValue = i_byteArr.left(12);

    strlstActualValues.append(strActualValue);
    strActualValue.clear();

    pTestStep->setActualValues(strlstActualValues);
    pTestStep->setToolTip(strToolTip);
    pTestStep->testEnded();

    // Continue with next test step of this group.
    m_iTestStep++;
    triggerDoTestStep();

} // onTestStepGroupSendDataTcpBlkTypeLClientReceivedData

//------------------------------------------------------------------------------
void CTest::onTestStepGroupSendDataTcpBlkTypeLClientDisconnected( QObject* /*i_pClient*/ )
//------------------------------------------------------------------------------
{
    if( !QObject::disconnect(
        /* pObjSender   */ m_pClient,
        /* szSignal     */ SIGNAL(disconnected(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTestStepGroupSendDataTcpBlkTypeLClientDisconnected(QObject*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    ZS::Test::CTestStepGroup* pTSGrp = getTestStepGroup( QString::number(m_iTestStepGroup) + ".SendData.Tcp.BlkType.L", getTestStepGroupRoot() );
    ZS::Test::CTestStep* pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(ETestStepClientDisconnected) + ".SendData.Tcp.BlkType.L.Client.Disconnected", pTSGrp );

    QString     strActualValue;
    QStringList strlstActualValues;
    QString     strMethod;
    QString     strArgs;
    QString     strToolTip;

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
    strlstActualValues.append(strActualValue);
    strActualValue.clear();

    pTestStep->setActualValues(strlstActualValues);
    pTestStep->setToolTip(strToolTip);
    pTestStep->testEnded();

    // Continue with next test step of this group.
    m_iTestStep++;
    triggerDoTestStep();

} // onTestStepGroupSendDataTcpBlkTypeLClientDisconnected

#endif
