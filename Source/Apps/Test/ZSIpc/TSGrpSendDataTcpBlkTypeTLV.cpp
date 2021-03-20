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

#include "ZSIpc/ZSIpcBlkTypeTLV.h"
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
void CTest::FctDoTestStepGroupSendDataTcpBlkTypeTLV( ZS::Test::CTest* i_pThis, ZS::Test::CTestStepGroup* i_pTestStepGrpParent )
//------------------------------------------------------------------------------
{
    dynamic_cast<CTest*>(i_pThis)->doTestStepGroupSendDataTcpBlkTypeTLV(i_pTestStepGrpParent);
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
void CTest::doTestStepGroupSendDataTcpBlkTypeTLV( ZS::Test::CTestStepGroup* i_pTestStepGrpParent )
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStepGroup* pTSGrp = getTestStepGroup( QString::number(m_iTestStepGroup) + ".SendData.Tcp.BlkType.TLV", i_pTestStepGrpParent );
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
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".SendData.Tcp.BlkType.TLV.Server.Create", pTSGrp );
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
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".SendData.Tcp.BlkType.TLV.Server.Startup", pTSGrp );
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
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".SendData.Tcp.BlkType.TLV.Client.Create", pTSGrp );
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
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".SendData.Tcp.BlkType.TLV.Client.Connect", pTSGrp );
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
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".SendData.Tcp.BlkType.TLV.SetBlkType.Server", pTSGrp );
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
                strDesiredValue += ":" + ZS::Ipc::blkType2Str(ZS::Ipc::EBlkTypeTLV) + ")";
                strlstDesiredValues.append(strDesiredValue);
                strDesiredValue.clear();

                strMethod = "Ipc::CServer.setBlkType";
                strArgs = "TLV";
                pTestStep->setOperation( strMethod + "( " + strArgs + " )" );
                pTestStep->setDescription( "" );
                pTestStep->setDesiredValues(strlstDesiredValues);

                pTestStep->testStarted();

                try
                {
                    ZS::Ipc::CBlkType* pBlkType = new ZS::Ipc::CBlkTypeTLV(
                        /* uTLVTagSize         */ 2,
                        /* blkLenDataType      */ ZS::Ipc::EBlkLenDataTypeBinUINT32,
                        /* iBlkLenDataTypeSize */ -1 ); // -1 means use default (see enum EBlkLenDataType above)
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
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".SendData.Tcp.BlkType.TLV.SetBlkType.Client", pTSGrp );
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
                strDesiredValue += ":" + ZS::Ipc::blkType2Str(ZS::Ipc::EBlkTypeTLV) + ")";
                strlstDesiredValues.append(strDesiredValue);
                strDesiredValue.clear();

                strMethod = "Ipc::Client.setBlkType";
                strArgs = "TLV";
                pTestStep->setOperation( strMethod + "( " + strArgs + " )" );
                pTestStep->setDescription( "" );
                pTestStep->setDesiredValues(strlstDesiredValues);

                pTestStep->testStarted();

                try
                {
                    ZS::Ipc::CBlkType* pBlkType = new ZS::Ipc::CBlkTypeTLV(
                        /* uTLVTagSize         */ 2,
                        /* blkLenDataType      */ ZS::Ipc::EBlkLenDataTypeBinUINT32,
                        /* iBlkLenDataTypeSize */ -1 ); // -1 means use default (see enum EBlkLenDataType above)
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
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".SendData.Tcp.BlkType.TLV.SendData.CltSrv", pTSGrp );
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

                //CBlkTypeTLV(
                //    /* uTLVTagSize         */ 2,
                //    /* blkLenDataType      */ ZS::Ipc::EBlkLenDataTypeBinUINT32,
                //    /* iBlkLenDataTypeSize */ -1 ); // -1 means use default (see enum EBlkLenDataType above)

                QByteArray byteArrTag("##");
                QByteArray byteArrBlkLen(ZS::Ipc::blockLenDataType2SizeInBytes(ZS::Ipc::EBlkLenDataTypeBinUINT32),0x00);
                QByteArray byteArrValue(1024,'.');
                QByteArray byteArrData;

                byteArrValue.replace(0,12,"Hello Server");

                quint32 uBlkLen = byteArrValue.size();

                memcpy( byteArrBlkLen.data(), &uBlkLen, byteArrBlkLen.size() );

                byteArrData  = byteArrTag;
                byteArrData += byteArrBlkLen;
                byteArrData += byteArrValue;

                quint32 uBlkLenChk = 0;

                memcpy( &uBlkLenChk, &byteArrData.data()[byteArrTag.size()], sizeof(uBlkLenChk) );

                strDesiredValue = byteArrValue.left(12);
                strlstDesiredValues.append(strDesiredValue);
                strDesiredValue.clear();

                strMethod = "Ipc::Client.sendData";
                strArgs = QString(byteArrTag) + QString("%1").arg(QString::number(uBlkLen),sizeof(uBlkLen),' ') + QString(byteArrValue).left(12);
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
                        /* szSlot       */ SLOT(onTestStepGroupSendDataTcpBlkTypeTLVServerReceivedData(QObject*,int,const QByteArray&)) ) )
                    {
                        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                    }

                    m_pClient->sendData(byteArrData);
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
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".SendData.Tcp.BlkType.TLV.SendData.SrvClt", pTSGrp );
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

                //CBlkTypeTLV(
                //    /* uTLVTagSize         */ 2,
                //    /* blkLenDataType      */ ZS::Ipc::EBlkLenDataTypeBinUINT32,
                //    /* iBlkLenDataTypeSize */ -1 ); // -1 means use default (see enum EBlkLenDataType above)

                QByteArray byteArrTag("##");
                QByteArray byteArrBlkLen(ZS::Ipc::blockLenDataType2SizeInBytes(ZS::Ipc::EBlkLenDataTypeBinUINT32),0x00);
                QByteArray byteArrValue(1024,'.');
                QByteArray byteArrData;

                byteArrValue.replace(0,12,"Hello Client");

                quint32 uBlkLen = byteArrValue.size();

                memcpy( byteArrBlkLen.data(), &uBlkLen, byteArrBlkLen.size() );

                byteArrData  = byteArrTag;
                byteArrData += byteArrBlkLen;
                byteArrData += byteArrValue;

                quint32 uBlkLenChk = 0;

                memcpy( &uBlkLenChk, &byteArrData.data()[byteArrTag.size()], sizeof(uBlkLenChk) );

                strDesiredValue = byteArrValue.left(12);
                strlstDesiredValues.append(strDesiredValue);
                strDesiredValue.clear();

                strMethod = "Ipc::Server.sendData";
                strArgs = QString(byteArrTag) + QString("%1").arg(QString::number(uBlkLen),sizeof(uBlkLen),' ') + QString(byteArrValue).left(12);
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
                        /* szSlot       */ SLOT(onTestStepGroupSendDataTcpBlkTypeTLVClientReceivedData(QObject*,const QByteArray&)) ) )
                    {
                        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                    }

                    m_pServer->sendData( ZS::Ipc::ESocketIdAllSockets, byteArrData );
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
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".SendData.Tcp.BlkType.TLV.Server.Delete", pTSGrp );
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
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".SendData.Tcp.BlkType.TLV.Client.Disconnected", pTSGrp );
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
                strDesiredValue += ":" + ZS::Ipc::blkType2Str(ZS::Ipc::EBlkTypeTLV) + ")";
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
                        /* szSlot       */ SLOT(onTestStepGroupSendDataTcpBlkTypeTLVClientDisconnected(QObject*)) ) )
                    {
                        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                    }
                    if( m_pClient->getState() == ZS::Ipc::CClient::EStateUnconnected )
                    {
                        onTestStepGroupSendDataTcpBlkTypeTLVClientDisconnected(m_pClient);
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
            pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(m_iTestStep) + ".SendData.Tcp.BlkType.TLV.Client.Delete", pTSGrp );
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

} // doTestStepGroupSendDataTcpBlkTypeTLV

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::onTestStepGroupSendDataTcpBlkTypeTLVServerReceivedData(
    QObject*          /*i_pServer*/,
    int               /*i_iSocketId*/,
    const QByteArray& i_byteArr )
//------------------------------------------------------------------------------
{
    if( !QObject::disconnect(
        /* pObjSender   */ m_pServer,
        /* szSignal     */ SIGNAL(receivedData(QObject*,int,const QByteArray&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTestStepGroupSendDataTcpBlkTypeTLVServerReceivedData(QObject*,int,const QByteArray&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    ZS::Test::CTestStepGroup* pTSGrp = getTestStepGroup( QString::number(m_iTestStepGroup) + ".SendData.Tcp.BlkType.TLV", getTestStepGroupRoot() );
    ZS::Test::CTestStep* pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(ETestStepSendDataCltSrv) + ".SendData.Tcp.BlkType.TLV.SendData.CltSrv", pTSGrp );

    pTestStep->updateTestEndTime();

    QString     strActualValue;
    QStringList strlstActualValues;
    QString     strMethod;
    QString     strArgs;
    QString     strToolTip;

    QByteArray byteArrTag("##");
    QByteArray byteArrBlkLen(ZS::Ipc::blockLenDataType2SizeInBytes(ZS::Ipc::EBlkLenDataTypeBinUINT32),0x00);
    QByteArray byteArrValue = i_byteArr.right( i_byteArr.size() - (byteArrTag.size() + byteArrBlkLen.size()) );

    strActualValue = byteArrValue.left(12);

    strlstActualValues.append(strActualValue);
    strActualValue.clear();

    pTestStep->setActualValues(strlstActualValues);
    pTestStep->setToolTip(strToolTip);
    pTestStep->testEnded();

    // Continue with next test step of this group.
    m_iTestStep++;
    triggerDoTestStep();

} // onTestStepGroupSendDataTcpBlkTypeTLVServerReceivedData

//------------------------------------------------------------------------------
void CTest::onTestStepGroupSendDataTcpBlkTypeTLVClientReceivedData(
    QObject*          /*i_pClient*/,
    const QByteArray& i_byteArr )
//------------------------------------------------------------------------------
{
    if( !QObject::disconnect(
        /* pObjSender   */ m_pClient,
        /* szSignal     */ SIGNAL(receivedData(QObject*,const QByteArray&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTestStepGroupSendDataTcpBlkTypeTLVClientReceivedData(QObject*,const QByteArray&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    ZS::Test::CTestStepGroup* pTSGrp = getTestStepGroup( QString::number(m_iTestStepGroup) + ".SendData.Tcp.BlkType.TLV", getTestStepGroupRoot() );
    ZS::Test::CTestStep* pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(ETestStepSendDataSrvClt) + ".SendData.Tcp.BlkType.TLV.SendData.SrvClt", pTSGrp );

    pTestStep->updateTestEndTime();

    QString     strActualValue;
    QStringList strlstActualValues;
    QString     strMethod;
    QString     strArgs;
    QString     strToolTip;

    QByteArray byteArrTag("##");
    QByteArray byteArrBlkLen(ZS::Ipc::blockLenDataType2SizeInBytes(ZS::Ipc::EBlkLenDataTypeBinUINT32),0x00);
    QByteArray byteArrValue = i_byteArr.right( i_byteArr.size() - (byteArrTag.size() + byteArrBlkLen.size()) );

    strActualValue = byteArrValue.left(12);

    strlstActualValues.append(strActualValue);
    strActualValue.clear();

    pTestStep->setActualValues(strlstActualValues);
    pTestStep->setToolTip(strToolTip);
    pTestStep->testEnded();

    // Continue with next test step of this group.
    m_iTestStep++;
    triggerDoTestStep();

} // onTestStepGroupSendDataTcpBlkTypeTLVClientReceivedData

//------------------------------------------------------------------------------
void CTest::onTestStepGroupSendDataTcpBlkTypeTLVClientDisconnected( QObject* /*i_pClient*/ )
//------------------------------------------------------------------------------
{
    if( !QObject::disconnect(
        /* pObjSender   */ m_pClient,
        /* szSignal     */ SIGNAL(disconnected(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTestStepGroupSendDataTcpBlkTypeTLVClientDisconnected(QObject*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    ZS::Test::CTestStepGroup* pTSGrp = getTestStepGroup( QString::number(m_iTestStepGroup) + ".SendData.Tcp.BlkType.TLV", getTestStepGroupRoot() );
    ZS::Test::CTestStep* pTestStep = getTestStep( QString::number(m_iTestStepGroup) + "." + QString::number(ETestStepClientDisconnected) + ".SendData.Tcp.BlkType.TLV.Client.Disconnected", pTSGrp );

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

} // onTestStepGroupSendDataTcpBlkTypeTLVClientDisconnected

#endif
