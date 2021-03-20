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

#include "TSGrpStartupDbClt3.h"
#include "Test.h"

#include "ZSData/ZSDatabaseClient.h"
#include "ZSData/ZSDatabase.h"
#include "ZSData/ZSDataSet.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepAdminObjPool.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Data;
using namespace ZS::Trace;
using namespace ZS::Apps::Test::Data;


/*******************************************************************************
class CTSGrpStartupDbClt3 : public ZS::Test::CTestStepGroup
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTSGrpStartupDbClt3::CTSGrpStartupDbClt3( CTest* i_pTest, ZS::Test::CTestStepGroup* i_pTSGrpParent ) :
//------------------------------------------------------------------------------
    ZS::Test::CTestStepGroup(i_pTest, "DbClt3", i_pTSGrpParent)
{
    new ZS::Test::CTestStep(
        /* pTest           */ i_pTest,
        /* strName         */ "CreateClient",
        /* strOperation    */ "new CDbClient",
        /* pTSGrpParent    */ this,
        /* szDoTestStepFct */ SLOT(doTestStepGrpStartupDbClt3CreateClient(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ i_pTest,
        /* strName         */ "CreateDataSet",
        /* strOperation    */ "CDbClient.setDataSet",
        /* pTSGrpParent    */ this,
        /* szDoTestStepFct */ SLOT(doTestStepGrpStartupDbClt3CreateDataSet(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ i_pTest,
        /* strName         */ "InitSettings",
        /* strOperation    */ "CDbClient.updateHostSettings",
        /* pTSGrpParent    */ this,
        /* szDoTestStepFct */ SLOT(doTestStepGrpStartupDbClt3InitSettings(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ i_pTest,
        /* strName         */ "StartupClient",
        /* strOperation    */ "CDbClient.startup",
        /* pTSGrpParent    */ this,
        /* szDoTestStepFct */ SLOT(doTestStepGrpStartupDbClt3StartupClient(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ i_pTest,
        /* strName         */ "Connect",
        /* strOperation    */ "CDbClient.connect",
        /* pTSGrpParent    */ this,
        /* szDoTestStepFct */ SLOT(doTestStepGrpStartupDbClt3Connect(ZS::Test::CTestStep*)) );

    //new ZS::Test::CTestStep(
    //    /* pTest           */ i_pTest,
    //    /* strName         */ "Register",
    //    /* strOperation    */ "CDbClient.register",
    //    /* pTSGrpParent    */ this,
    //    /* szDoTestStepFct */ SLOT(doTestStepGrpStartupDbClt3Register(ZS::Test::CTestStep*)) );

    //new ZS::Test::CTestStep(
    //    /* pTest           */ i_pTest,
    //    /* pTSGrpParent    */ this,
    //    /* strName         */ "SelectSchema",
    //    /* strOperation    */ "CDbClient.selectSchema",
    //    /* szDoTestStepFct */ SLOT(doTestStepGrpStartupDbClt3SelectSchema(ZS::Test::CTestStep*)) );

} // ctor

//------------------------------------------------------------------------------
CTSGrpStartupDbClt3::~CTSGrpStartupDbClt3()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
protected: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepGrpStartupDbClt3CreateClient( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "TestStep: " + QString(i_pTestStep == nullptr ? "nullptr" : i_pTestStep->getName(true));
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "doTestStepGrpStartupDbClt3CreateClient",
        /* strAddInfo   */ strAddTrcInfo );

    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    SServerHostSettings srvHostSettings;
    SClientHostSettings cltHostSettings;
    STimerSettings      watchDogSettings;
    SCnctType           cnctType;
    EProtocolType       protType;
    ESocketType         sockType;
    int                 idxProtType;
    int                 idxSockType;

    // Desired Values
    //---------------

    for( idxProtType = 0; idxProtType < m_arProtocolTypes.size(); idxProtType++ )
    {
        protType = m_arProtocolTypes[idxProtType];
        cnctType.m_protocolType = protType;

        for( idxSockType = 0; idxSockType < m_hsharSocketTypes[cnctType.m_protocolType].size(); idxSockType++ )
        {
            sockType = m_hsharSocketTypes[cnctType.m_protocolType][idxSockType];
            cnctType.m_socketType = sockType;

            /* struct SServerHostSettings {
                ESocketType  m_socketType;
                QObject*     m_pObjLocal;
                QString      m_strLocalHostName; // So far only used for shm servers and shm clients. Tcp servers and tcp clients determine the local host name by themselves.
                QHostAddress m_hostAddrLocal;
                quint16      m_uLocalPort;      // Port the server is listening for incoming connection requests.
                unsigned int m_uMaxPendingConnections;
                unsigned int m_uBufferSize;     // Only used by shared memory socket clients
            } */

            /* struct SClientHostSettings {
                ESocketType  m_socketType;
                QObject*     m_pObjRemote;
                QString      m_strRemoteHostName;
                quint16      m_uRemotePort; // Port the server is listening for incoming connection requests.
                int          m_iConnectTimeout_ms;
                unsigned int m_uBufferSize; // only used by shared memory socket clients
            }; */

            srvHostSettings = m_hshHostSettings[cnctType];

            cltHostSettings.m_socketType = cnctType.m_socketType;

            if( cnctType.m_socketType == ESocketTypeTcp )
            {
                cltHostSettings.m_pObjRemote = nullptr;
                cltHostSettings.m_strRemoteHostName = srvHostSettings.m_strLocalHostName;
                cltHostSettings.m_uRemotePort = srvHostSettings.m_uLocalPort;
                cltHostSettings.m_iConnectTimeout_ms = 10000;
                //cltHostSettings.m_uBufferSize = 0;
            }
            else if( cnctType.m_socketType == ESocketTypeInProcMsg )
            {
                cltHostSettings.m_pObjRemote = srvHostSettings.m_pObjLocal;
                cltHostSettings.m_strRemoteHostName = srvHostSettings.m_strLocalHostName;
                //cltHostSettings.m_uRemotePort;
                cltHostSettings.m_iConnectTimeout_ms = 1000;
                //cltHostSettings.m_uBufferSize = 0;
            }

            strDesiredValue  = protocolType2Str(protType) + ":" + socketType2Str(sockType) + ":";
            strDesiredValue += cltHostSettings.getConnectionString();
            strlstDesiredValues.append(strDesiredValue);

        } // for( idxSockType = 0; idxSockType < m_hsharSocketTypes[cnctType.m_protocolType].size(); idxSockType++ )
    } // for( idxProtType = 0; idxProtType < m_arProtocolTypes.size(); idxProtType++ )

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    // Default ctor creates database client with:
        // ProtocolTypes:       [ZS]
        // SocketTypes:         {ZS: [Tcp, InProcMsg]}
        // UseCommandThreads:   false
        // ConnectionName:      ""
        // MultiThreadedAccess: false
    m_pDbClt3 = new CDbClient(c_strObjNameDbClt3); 

    //m_pDbClt3->setReqTimeout(m_iReqTimeout_ms);
    m_pDbClt3->setKeepReqDscrInExecTree(m_bKeepReqDscrInExecTree);

    emit dbClt3Created(m_pDbClt3);

    // Actual Values
    //---------------

    QList<EProtocolType> arProtTypes = m_pDbClt3->getProtocolTypes();

    for( idxProtType = 0; idxProtType < arProtTypes.size(); idxProtType++ )
    {
        protType = arProtTypes[idxProtType];

        QList<ESocketType> arSockTypes = m_pDbClt3->getSocketTypes(protType);

        for( idxSockType = 0; idxSockType < arSockTypes.size(); idxSockType++ )
        {
            sockType = arSockTypes[idxSockType];

            strActualValue  = protocolType2Str(protType) + ":" + socketType2Str(sockType) + ":";
            strActualValue += m_pDbClt3->getConnectionString(protType, sockType);
            strlstActualValues.append(strActualValue);

        } // for( idxSockType = 0; idxSockType < arSockTypes.size(); idxSockType++ )
    } // for( idxProtType = 0; idxProtType < arProtTypes.size(); idxProtType++ )

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepGrpStartupDbClt3CreateClient

//------------------------------------------------------------------------------
void CTest::doTestStepGrpStartupDbClt3CreateDataSet( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "TestStep: " + QString(i_pTestStep == nullptr ? "nullptr" : i_pTestStep->getName(true));
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "doTestStepGrpStartupDbClt3CreateDataSet",
        /* strAddInfo   */ strAddTrcInfo );

    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strDesiredValue = "DS:" + c_strObjNameDbClt3DS;
    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    if( m_pDbClt3 != nullptr )
    {
        try
        {
            m_pDSDbClt3 = new CDataSet(c_strObjNameDbClt3DS);
            m_pDbClt3->setDataSet(m_pDSDbClt3);

            emit dbClt3DataSetCreated(m_pDSDbClt3);
        }
        catch( CException& exc )
        {
            strActualValue = exc.toString();
            strlstActualValues.append(strActualValue);
        }
        catch(...)
        {
            strActualValue = "Critical Error: Unknown Exception Occurred";
            strlstActualValues.append(strActualValue);
        }
    } // if( m_pDbClt3 != nullptr )

    // Actual Values
    //---------------

    strActualValue = QString(m_pDSDbClt3 == nullptr ? "nullptr" : m_pDSDbClt3->getDSObjKey() );
    strlstActualValues.append(strActualValue);

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepGrpStartupDbClt3CreateDataSet

//------------------------------------------------------------------------------
void CTest::doTestStepGrpStartupDbClt3InitSettings( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "TestStep: " + QString(i_pTestStep == nullptr ? "nullptr" : i_pTestStep->getName(true));
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "doTestStepGrpStartupDbClt3InitSettings",
        /* strAddInfo   */ strAddTrcInfo );

    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    SServerHostSettings srvHostSettings;
    SServerHostSettings srvHostSettingsDefault;
    SClientHostSettings cltHostSettings;
    SClientHostSettings cltHostSettingsDefault;
    STimerSettings      watchDogSettings;
    STimerSettings      watchDogSettingsDefault;
    EProtocolType       protType;
    ESocketType         sockType;
    SCnctType           cnctType;
    int                 idxProtType;
    int                 idxSockType;

    // Desired Values
    //---------------

    CRequest reqDesired(
        /* strCreatorNameSpace */ CDbClient::NameSpace(),
        /* strCreatorClassName      */ CDbClient::ClassName(),
        /* pObjCreator              */ m_pDbClt3,
        /* request                  */ CDbClient::ERequestUpdateHostSettings,
        /* strRequest               */ CDbClient::Request2Str(CDbClient::ERequestUpdateHostSettings),
        /* iParentId                */ -1,
        /* bKeepReqDscrInExecTree   */ false,
        /* iTimeout_ms              */ m_iReqTimeout_ms,
        /* bIsBlocking              */ false );
    reqDesired.setProgressInPerCent(100);
    reqDesired.setErrResult(ErrResultSuccess);

    strDesiredValue = "Req {" + reqDesired.getAddTrcInfoStr() + "}";
    strlstDesiredValues.append(strDesiredValue);
    strDesiredValue = "State: " + CDbClient::State2Str(CDbClient::EStateIdle);
    strlstDesiredValues.append(strDesiredValue);

    /*struct SServerHostSettings {
        ESocketType  m_socketType;
        QObject*     m_pObjLocal;
        QString      m_strLocalHostName; // So far only used for shm servers and shm clients. Tcp servers and tcp clients determine the local host name by themselves.
        QHostAddress m_hostAddrLocal;
        quint16      m_uLocalPort;      // Port the server is listening for incoming connection requests.
        unsigned int m_uMaxPendingConnections;
        unsigned int m_uBufferSize;     // Only used by shared memory socket clients
    } */

    /* struct SClientHostSettings {
        ESocketType  m_socketType;
        QObject*     m_pObjRemote;
        QString      m_strRemoteHostName;
        quint16      m_uRemotePort; // Port the server is listening for incoming connection requests.
        int          m_iConnectTimeout_ms;
        unsigned int m_uBufferSize; // only used by shared memory socket clients
    }; */

    cnctType.m_protocolType = EProtocolTypeZSXML;
    cnctType.m_socketType = ESocketTypeTcp;

    srvHostSettings = m_hshHostSettings[cnctType];

    cltHostSettings.m_socketType = cnctType.m_socketType;

    if( cnctType.m_socketType == ESocketTypeTcp )
    {
        cltHostSettings.m_pObjRemote = nullptr;
        cltHostSettings.m_strRemoteHostName = srvHostSettings.m_strLocalHostName;
        cltHostSettings.m_uRemotePort = srvHostSettings.m_uLocalPort;
        cltHostSettings.m_iConnectTimeout_ms = 10000;
        //cltHostSettings.m_uBufferSize = 0;
    }
    else if( cnctType.m_socketType == ESocketTypeInProcMsg )
    {
        cltHostSettings.m_pObjRemote = srvHostSettings.m_pObjLocal;
        cltHostSettings.m_strRemoteHostName = srvHostSettings.m_strLocalHostName;
        //cltHostSettings.m_uRemotePort;
        cltHostSettings.m_iConnectTimeout_ms = 1000;
        //cltHostSettings.m_uBufferSize = 0;
    }

    watchDogSettings = m_hshWatchDogSettings[cnctType];

    strDesiredValue = "Current Settings:";
    strlstDesiredValues.append(strDesiredValue);

    strDesiredValue  = "- Host: " + protocolType2Str(cnctType.m_protocolType) + ":" + socketType2Str(cnctType.m_socketType) + ":";
    strDesiredValue += cltHostSettings.getConnectionString();
    strlstDesiredValues.append(strDesiredValue);

    strDesiredValue = "- WatchDog: " + watchDogSettings.toString();
    strlstDesiredValues.append(strDesiredValue);

    strDesiredValue = "Default Settings:";
    strlstDesiredValues.append(strDesiredValue);

    for( idxProtType = 0; idxProtType < m_arProtocolTypes.size(); idxProtType++ )
    {
        protType = m_arProtocolTypes[idxProtType];
        cnctType.m_protocolType = protType;

        for( idxSockType = 0; idxSockType < m_hsharSocketTypes[cnctType.m_protocolType].size(); idxSockType++ )
        {
            sockType = m_hsharSocketTypes[cnctType.m_protocolType][idxSockType];
            cnctType.m_socketType = sockType;

            /*struct SServerHostSettings {
                ESocketType  m_socketType;
                QObject*     m_pObjLocal;
                QString      m_strLocalHostName; // So far only used for shm servers and shm clients. Tcp servers and tcp clients determine the local host name by themselves.
                QHostAddress m_hostAddrLocal;
                quint16      m_uLocalPort;      // Port the server is listening for incoming connection requests.
                unsigned int m_uMaxPendingConnections;
                unsigned int m_uBufferSize;     // Only used by shared memory socket clients
            } */

            /* struct SClientHostSettings {
                ESocketType  m_socketType;
                QObject*     m_pObjRemote;
                QString      m_strRemoteHostName;
                quint16      m_uRemotePort; // Port the server is listening for incoming connection requests.
                int          m_iConnectTimeout_ms;
                unsigned int m_uBufferSize; // only used by shared memory socket clients
            }; */

            srvHostSettingsDefault = m_hshHostSettingsDefault[cnctType];

            cltHostSettingsDefault.m_socketType = cnctType.m_socketType;

            if( cnctType.m_socketType == ESocketTypeTcp )
            {
                cltHostSettingsDefault.m_pObjRemote = nullptr;
                cltHostSettingsDefault.m_strRemoteHostName = srvHostSettingsDefault.m_strLocalHostName;
                cltHostSettingsDefault.m_uRemotePort = srvHostSettingsDefault.m_uLocalPort;
                cltHostSettingsDefault.m_iConnectTimeout_ms = 10000;
                //cltHostSettingsDefault.m_uBufferSize = 0;
            }
            else if( cnctType.m_socketType == ESocketTypeInProcMsg )
            {
                cltHostSettingsDefault.m_pObjRemote = srvHostSettingsDefault.m_pObjLocal;
                cltHostSettingsDefault.m_strRemoteHostName = srvHostSettingsDefault.m_strLocalHostName;
                //cltHostSettingsDefault.m_uRemotePort;
                cltHostSettingsDefault.m_iConnectTimeout_ms = 1000;
                //cltHostSettingsDefault.m_uBufferSize = 0;
            }

            strDesiredValue = " " + protocolType2Str(protType) + ":" + socketType2Str(sockType) + ":";
            strlstDesiredValues.append(strDesiredValue);

            strDesiredValue = " - Host: " + cltHostSettingsDefault.getConnectionString();
            strlstDesiredValues.append(strDesiredValue);

            watchDogSettingsDefault = m_hshWatchDogSettingsDefault[cnctType];
            strDesiredValue = " - WatchDog: " + watchDogSettingsDefault.toString();
            strlstDesiredValues.append(strDesiredValue);

        } // for( idxSockType = 0; idxSockType < m_hsharSocketTypes[cnctType.m_protocolType].size(); idxSockType++ )
    } // for( idxProtType = 0; idxProtType < m_arProtocolTypes.size(); idxProtType++ )

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    CRequest* pReq = nullptr;

    if( m_pDbClt3 != nullptr )
    {
        try
        {
            m_pDbClt3->setDefaultProtocolType(protType);
            m_pDbClt3->setDefaultSocketType(sockType);

            for( idxProtType = 0; idxProtType < m_arProtocolTypes.size(); idxProtType++ )
            {
                protType = m_arProtocolTypes[idxProtType];
                cnctType.m_protocolType = protType;

                for( idxSockType = 0; idxSockType < m_hsharSocketTypes[cnctType.m_protocolType].size(); idxSockType++ )
                {
                    sockType = m_hsharSocketTypes[cnctType.m_protocolType][idxSockType];
                    cnctType.m_socketType = sockType;

                    /* struct SClientHostSettings {
                        ESocketType  m_socketType;
                        QObject*     m_pObjRemote;
                        QString      m_strRemoteHostName;
                        quint16      m_uRemotePort; // Port the server is listening for incoming connection requests.
                        int          m_iConnectTimeout_ms;
                        unsigned int m_uBufferSize; // only used by shared memory socket clients
                    }; */

                    srvHostSettingsDefault = m_hshHostSettingsDefault[cnctType];

                    cltHostSettingsDefault.m_socketType = cnctType.m_socketType;

                    if( cnctType.m_socketType == ESocketTypeTcp )
                    {
                        cltHostSettingsDefault.m_pObjRemote = nullptr;
                        cltHostSettingsDefault.m_strRemoteHostName = srvHostSettingsDefault.m_strLocalHostName;
                        cltHostSettingsDefault.m_uRemotePort = srvHostSettingsDefault.m_uLocalPort;
                        cltHostSettingsDefault.m_iConnectTimeout_ms = 10000;
                        //cltHostSettingsDefault.m_uBufferSize = 0;
                    }
                    else if( cnctType.m_socketType == ESocketTypeInProcMsg )
                    {
                        cltHostSettingsDefault.m_pObjRemote = srvHostSettingsDefault.m_pObjLocal;
                        cltHostSettingsDefault.m_strRemoteHostName = srvHostSettingsDefault.m_strLocalHostName;
                        //cltHostSettingsDefault.m_uRemotePort;
                        cltHostSettingsDefault.m_iConnectTimeout_ms = 1000;
                        //cltHostSettingsDefault.m_uBufferSize = 0;
                    }

                    watchDogSettingsDefault = m_hshWatchDogSettingsDefault[cnctType];

                    m_pDbClt3->setDefaultHostSettings(cltHostSettingsDefault, protType);
                    m_pDbClt3->setDefaultWatchDogSettings(watchDogSettingsDefault, protType, sockType);

                } // for( idxSockType = 0; idxSockType < m_hsharSocketTypes[cnctType.m_protocolType].size(); idxSockType++ )
            } // for( idxProtType = 0; idxProtType < m_arProtocolTypes.size(); idxProtType++ )

            cnctType.m_protocolType = EProtocolTypeZSXML;
            cnctType.m_socketType = ESocketTypeTcp;

            m_pDbClt3->setProtocolType(cnctType.m_protocolType);
            m_pDbClt3->setSocketType(cnctType.m_socketType);

            srvHostSettings = m_hshHostSettings[cnctType];
            srvHostSettingsDefault = m_hshHostSettingsDefault[cnctType];

            cltHostSettings.m_socketType = cnctType.m_socketType;
            cltHostSettings.m_pObjRemote = srvHostSettings.m_pObjLocal;
            cltHostSettings.m_strRemoteHostName = srvHostSettings.m_strLocalHostName;
            cltHostSettings.m_uRemotePort = srvHostSettings.m_uLocalPort;
            cltHostSettings.m_iConnectTimeout_ms = 10000;
            //cltHostSettings.m_uBufferSize = 0;

            watchDogSettings = m_hshWatchDogSettings[cnctType];

            m_pDbClt3->setHostSettings(cltHostSettings);
            m_pDbClt3->setWatchDogSettings(watchDogSettings);

            pReq = m_pDbClt3->updateHostSettings(
                /* iTimeout_ms  */ m_iReqTimeout_ms,
                /* bWait        */ false,
                /* iReqIdParent */ -1 );
        }
        catch( CException& exc )
        {
            strActualValue = exc.toString();
            strlstActualValues.append(strActualValue);
        }
        catch(...)
        {
            strActualValue = "Critical Error: Unknown Exception Occurred";
            strlstActualValues.append(strActualValue);
        }
    } // if( m_pDbClt3 != nullptr )

    // Actual Values
    //---------------

    if( m_pDbClt3 != nullptr )
    {
        strActualValue = "Req {" + QString(pReq == nullptr ? "nullptr" : pReq->getAddTrcInfoStr()) + "}";
        strlstActualValues.append(strActualValue);
        strActualValue = "State: " + CDbClient::State2Str(m_pDbClt3->getState());
        strlstActualValues.append(strActualValue);

        strActualValue = "Current Settings:";
        strlstActualValues.append(strActualValue);

        strActualValue  = "- Host: " + protocolType2Str(cnctType.m_protocolType) + ":" + socketType2Str(cnctType.m_socketType) + ":";
        strActualValue += m_pDbClt3->getConnectionString();
        strlstActualValues.append(strActualValue);

        strActualValue = "- WatchDog: " + m_pDbClt3->getWatchDogSettings().toString();
        strlstActualValues.append(strActualValue);

        strActualValue = "Default Settings:";
        strlstActualValues.append(strActualValue);

        for( idxProtType = 0; idxProtType < m_arProtocolTypes.size(); idxProtType++ )
        {
            protType = m_arProtocolTypes[idxProtType];
            cnctType.m_protocolType = protType;

            for( idxSockType = 0; idxSockType < m_hsharSocketTypes[cnctType.m_protocolType].size(); idxSockType++ )
            {
                sockType = m_hsharSocketTypes[cnctType.m_protocolType][idxSockType];
                cnctType.m_socketType = sockType;

                strActualValue = " " + protocolType2Str(protType) + ":" + socketType2Str(sockType) + ":";
                strlstActualValues.append(strActualValue);

                strActualValue = " - Host: " + m_pDbClt3->getDefaultConnectionString(protType, sockType);
                strlstActualValues.append(strActualValue);

                watchDogSettingsDefault = m_hshWatchDogSettingsDefault[cnctType];
                strActualValue = " - WatchDog: " + m_pDbClt3->getDefaultWatchDogSettings(protType,sockType)->toString();
                strlstActualValues.append(strActualValue);

            } // for( idxSockType = 0; idxSockType < m_hsharSocketTypes[cnctType.m_protocolType].size(); idxSockType++ )
        } // for( idxProtType = 0; idxProtType < m_arProtocolTypes.size(); idxProtType++ )
    } // if( m_pDbClt3 != nullptr )

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepGrpStartupDbClt3InitSettings

//------------------------------------------------------------------------------
void CTest::doTestStepGrpStartupDbClt3StartupClient( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "TestStep: " + QString(i_pTestStep == nullptr ? "nullptr" : i_pTestStep->getName(true));
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "doTestStepGrpStartupDbClt3StartupClient",
        /* strAddInfo   */ strAddTrcInfo );

    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    CRequest reqDesired(
        /* strCreatorNameSpace */ CDbClient::NameSpace(),
        /* strCreatorClassName      */ CDbClient::ClassName(),
        /* pObjCreator              */ m_pDbClt3,
        /* request                  */ CDbClient::ERequestStartup,
        /* strRequest               */ CDbClient::Request2Str(CDbClient::ERequestStartup),
        /* iParentId                */ -1,
        /* bKeepReqDscrInExecTree   */ false,
        /* iTimeout_ms              */ m_iReqTimeout_ms,
        /* bIsBlocking              */ true );
    reqDesired.setProgressInPerCent(100);
    reqDesired.setErrResult(ErrResultSuccess);

    strDesiredValue = "Req {" + reqDesired.getAddTrcInfoStr() + "}";
    strlstDesiredValues.append(strDesiredValue);
    strDesiredValue = "State: " + CDbClient::State2Str(CDbClient::EStateUnconnected);
    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    CRequest* pReq = nullptr;

    if( m_pDbClt3 != nullptr )
    {
        try
        {
            pReq = m_pDbClt3->startup(
                /* iTimeout_ms  */ m_iReqTimeout_ms,
                /* bWait        */ true,
                /* iReqIdParent */ -1 );
        }
        catch( CException& exc )
        {
            strActualValue = exc.toString();
            strlstActualValues.append(strActualValue);
        }
        catch(...)
        {
            strActualValue = "Critical Error: Unknown Exception Occurred";
            strlstActualValues.append(strActualValue);
        }
    } // if( m_pDbClt3 != nullptr )

    // Actual Values
    //---------------

    if( m_pDbClt3 != nullptr )
    {
        strActualValue = "Req {" + QString(pReq == nullptr ? "nullptr" : pReq->getAddTrcInfoStr()) + "}";
        strlstActualValues.append(strActualValue);
        strActualValue = "State: " + CDbClient::State2Str(m_pDbClt3->getState());
        strlstActualValues.append(strActualValue);

    } // if( m_pDbClt3 != nullptr )

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepGrpStartupDbClt3StartupClient

//------------------------------------------------------------------------------
void CTest::doTestStepGrpStartupDbClt3Connect( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "TestStep: " + QString(i_pTestStep == nullptr ? "nullptr" : i_pTestStep->getName(true));
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "doTestStepGrpStartupDbClt3Connect",
        /* strAddInfo   */ strAddTrcInfo );

    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    CRequest reqDesired(
        /* strCreatorNameSpace */ CDbClient::NameSpace(),
        /* strCreatorClassName      */ CDbClient::ClassName(),
        /* pObjCreator              */ m_pDbClt3,
        /* request                  */ CDbClient::ERequestConnect,
        /* strRequest               */ CDbClient::Request2Str(CDbClient::ERequestConnect),
        /* iParentId                */ -1,
        /* bKeepReqDscrInExecTree   */ false,
        /* iTimeout_ms              */ m_iReqTimeout_ms,
        /* bIsBlocking              */ false );
    reqDesired.setProgressInPerCent(100);
    reqDesired.setErrResult(ErrResultSuccess);

    strDesiredValue = "Req {" + reqDesired.getAddTrcInfoStr() + "}";
    strlstDesiredValues.append(strDesiredValue);
    strDesiredValue = "State: " + CDbClient::State2Str(CDbClient::EStateConnected);
    strlstDesiredValues.append(strDesiredValue);
    strDesiredValue = m_pDbClt3->objectName() + " in DbCnctList: true";
    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    CRequest* pReq = nullptr;

    if( m_pDbClt3 != nullptr )
    {
        try
        {
            pReq = m_pDbClt3->connect_(
                /* iTimeout_ms  */ m_iReqTimeout_ms,
                /* bWait        */ false,
                /* iReqIdParent */ -1 );

            pReq->setExecutionData( QString::number(pReq->getId()), i_pTestStep );

            m_hshClientRequestQueueReqsInProgress[pReq->getId()] = pReq;

            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onTestStepGrpStartupDbClt3RequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
        }
        catch( CException& exc )
        {
            strActualValue = exc.toString();
            strlstActualValues.append(strActualValue);
        }
        catch(...)
        {
            strActualValue = "Critical Error: Unknown Exception Occurred";
            strlstActualValues.append(strActualValue);
        }
    } // if( m_pDbClt3 != nullptr )

    // Actual Values
    //---------------

    // If an exception was thrown ..
    if( !strlstActualValues.isEmpty() )
    {
        strlstActualValues.append(strActualValue);
        i_pTestStep->setActualValues(strlstActualValues);
    }

} // doTestStepGrpStartupDbClt3Connect

//------------------------------------------------------------------------------
void CTest::doTestStepGrpStartupDbClt3Register( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "TestStep: " + QString(i_pTestStep == nullptr ? "nullptr" : i_pTestStep->getName(true));
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "doTestStepGrpStartupDbClt3Register",
        /* strAddInfo   */ strAddTrcInfo );

    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    // Test Step
    //----------

    // Actual Values
    //---------------

    strActualValue = "__TODO__";
    strlstActualValues.append(strActualValue);

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepGrpStartupDbClt3Register

//------------------------------------------------------------------------------
void CTest::doTestStepGrpStartupDbClt3SelectSchema( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "TestStep: " + QString(i_pTestStep == nullptr ? "nullptr" : i_pTestStep->getName(true));
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "doTestStepGrpStartupDbClt3SelectSchema",
        /* strAddInfo   */ strAddTrcInfo );

    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    // Test Step
    //----------

    // Actual Values
    //---------------

    strActualValue = "__TODO__";
    strlstActualValues.append(strActualValue);

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepGrpStartupDbClt3SelectSchema

/*==============================================================================
protected slots: // future callbacks (GrpStartupDbClt3)
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::onTestStepGrpStartupDbClt3RequestChanged( ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "Req {" + i_reqDscr.getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onTestStepGrpStartupDbClt3RequestChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_hshClientRequestQueueReqsInProgress.contains(i_reqDscr.m_iId) )
    {
        CRequest* pReq = m_hshClientRequestQueueReqsInProgress[i_reqDscr.m_iId];

        ZS::Test::CTestStep* pTestStep = reinterpret_cast<ZS::Test::CTestStep*>(pReq->takeExecutionData(QString::number(i_reqDscr.m_iId)));

        // If the request has been finished ...
        if( i_reqDscr.m_errResultInfo.isErrorResult() || i_reqDscr.m_iProgress_perCent >= 100 )
        {
            QObject::disconnect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onTestStepGrpStartupDbClt3RequestChanged(ZS::System::SRequestDscr)) );

            m_hshClientRequestQueueReqsInProgress.remove(i_reqDscr.m_iId);

            if( pTestStep != nullptr )
            {
                QString     strActualValue;
                QStringList strlstActualValues;

                if( pTestStep->getName() == "Connect" )
                {
                    strActualValue = "Req {" + QString(i_reqDscr.getAddTrcInfoStr()) + "}";
                    strlstActualValues.append(strActualValue);
                    strActualValue = "State: " + CDbClient::State2Str(m_pDbClt3->getState());
                    strlstActualValues.append(strActualValue);

                    m_strlstActualValuesTmp = strlstActualValues;

                    // It may take a while until the server receives the connect indication message from its TCP gateway.
                    m_iTestStepTimeoutSlotInterval_ms = 100;
                    m_iTestStepTimeoutSlotCalled = 0;
                    QTimer::singleShot(m_iTestStepTimeoutSlotInterval_ms, this, SLOT(onTestStepGrpStartupDbClt3Timeout()));

                } // if( pTestStep->getName() == "Connect" )
            } // if( pTestStep != nullptr )
        } // if( i_reqDscr.m_errResultInfo.isErrorResult() || i_reqDscr.m_iProgress_perCent >= 100 )
    } // if( m_requestQueue.isRequestInProgress(i_reqDscr.m_iId) )

} // onTestStepGrpStartupDbClt3RequestChanged

//------------------------------------------------------------------------------
void CTest::onTestStepGrpStartupDbClt3Timeout()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onTestStepGrpStartupDbClt3Timeout",
        /* strAddInfo   */ strAddTrcInfo );

    m_iTestStepTimeoutSlotCalled++;

    if( m_pTestStepCurr != nullptr )
    {
        if( m_pTestStepCurr->getName() == "Connect" )
        {
            QList<SSocketDscr> arCltCnctDscrs = m_pDb->getClientCnctDscrs();

            m_socketDscrDbClt3Tmp = m_pDbClt3->getSocketDscr();

            bool bCltInDbCnctList = false;

            for( int idxCnct = 0; idxCnct < arCltCnctDscrs.size(); idxCnct++ )
            {
                if( m_socketDscrDbClt3Tmp.m_socketType == ESocketTypeInProcMsg )
                {
                    if( arCltCnctDscrs[idxCnct].m_pObjRemote == m_socketDscrDbClt3Tmp.m_pObjLocal )
                    {
                        bCltInDbCnctList = true;
                        break;
                    }
                }
                else
                {
                    if( arCltCnctDscrs[idxCnct].m_uRemotePort == m_socketDscrDbClt3Tmp.m_uLocalPort )
                    {
                        bCltInDbCnctList = true;
                        break;
                    }
                }
            }

            if( bCltInDbCnctList || m_iTestStepTimeoutSlotCalled >= 10 )
            {
                QString strActualValue = m_pDbClt3->objectName() + " in DbCnctList: " + bool2Str(bCltInDbCnctList);
                m_strlstActualValuesTmp.append(strActualValue);
                m_pTestStepCurr->setActualValues(m_strlstActualValuesTmp);
                m_strlstActualValuesTmp.clear();
            }
            else if( m_iTestStepTimeoutSlotCalled < 10 )
            {
                // It may take a while until the server receives the connect indication message from its TCP gateway.
                QTimer::singleShot(m_iTestStepTimeoutSlotInterval_ms, this, SLOT(onTestStepGrpStartupDbClt3Timeout()));
            }
        } // if( m_pTestStepCurr->getName() == "Connect" )
    } // if( m_pTestStepCurr != nullptr )

} // onTestStepGrpStartupDbClt3Timeout
