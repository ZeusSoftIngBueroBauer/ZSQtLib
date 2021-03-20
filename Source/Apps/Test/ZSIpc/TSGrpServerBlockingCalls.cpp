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
public slots: // test step methods (Group Server - Blocking Calls)
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepGroupServerBlockingCallsCreateServer( ZS::Test::CTestStep* i_pTestStep )
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
        /* strMethod    */ "doTestStepGroupServerBlockingCallsCreateServer",
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

} // doTestStepGroupServerBlockingCallsCreateServer

//------------------------------------------------------------------------------
void CTest::doTestStepGroupServerBlockingCallsInitSettings( ZS::Test::CTestStep* i_pTestStep )
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
        /* strMethod    */ "doTestStepGroupServerBlockingCallsInitSettings",
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

    if( m_pServer != nullptr )
    {
        try
        {
            m_pServer->setBlkType( new ZS::Ipc::CBlkTypeL(ZS::Ipc::EBlkLenDataTypeBinUINT32) );
            m_pServer->setHostSettings(m_serverHostSettings);

            /*CRequest* pReqChangeSettings =*/ m_pServer->changeSettings(
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

} // doTestStepGroupServerBlockingCallsInitSettings

//------------------------------------------------------------------------------
void CTest::doTestStepGroupServerBlockingCallsStartup( ZS::Test::CTestStep* i_pTestStep )
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
        /* strMethod    */ "doTestStepGroupServerBlockingCallsStartup",
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

} // doTestStepGroupServerBlockingCallsStartup

//------------------------------------------------------------------------------
void CTest::doTestStepGroupServerBlockingCallsChangePort( ZS::Test::CTestStep* i_pTestStep )
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
        /* strMethod    */ "doTestStepGroupServerBlockingCallsChangePort",
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

} // doTestStepGroupServerBlockingCallsChangePort

//------------------------------------------------------------------------------
void CTest::doTestStepGroupServerBlockingCallsShutdown( ZS::Test::CTestStep* i_pTestStep )
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
        /* strMethod    */ "doTestStepGroupServerBlockingCallsShutdown",
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

} // doTestStepGroupServerBlockingCallsShutdown

//------------------------------------------------------------------------------
void CTest::doTestStepGroupServerBlockingCallsDeleteServer( ZS::Test::CTestStep* i_pTestStep )
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
        /* strMethod    */ "doTestStepGroupServerBlockingCallsDeleteServer",
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

} // doTestStepGroupServerBlockingCallsDeleteServer
