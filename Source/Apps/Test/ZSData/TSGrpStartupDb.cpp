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

#include <QtNetwork/qhostinfo.h>

#include "TSGrpStartupDb.h"
#include "Test.h"

#include "ZSData/ZSDatabase.h"
#include "ZSData/ZSDataSet.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Data;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::Apps::Test::Data;


/*******************************************************************************
class CTSGrpStartupDb : public ZS::Test::CTestStepGroup
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTSGrpStartupDb::CTSGrpStartupDb( CTest* i_pTest, ZS::Test::CTestStepGroup* i_pTSGrpParent ) :
//------------------------------------------------------------------------------
    ZS::Test::CTestStepGroup(i_pTest, "Db", i_pTSGrpParent)
{
    new ZS::Test::CTestStep(
        /* pTest           */ i_pTest,
        /* strName         */ "CreateDatabase",
        /* strOperation    */ "new CDb",
        /* pTSGrpParent    */ this,
        /* szDoTestStepFct */ SLOT(doTestStepGrpStartupDbCreateDatabase(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ i_pTest,
        /* strName         */ "CreateDataSet",
        /* strOperation    */ "new CDataSet",
        /* pTSGrpParent    */ this,
        /* szDoTestStepFct */ SLOT(doTestStepGrpStartupDbCreateDataSet(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ i_pTest,
        /* strName         */ "InitSettings",
        /* strOperation    */ "Db.updateHostSettings",
        /* pTSGrpParent    */ this,
        /* szDoTestStepFct */ SLOT(doTestStepGrpStartupDbInitSettings(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ i_pTest,
        /* strName         */ "StartupDatabase",
        /* strOperation    */ "Db.startup",
        /* pTSGrpParent    */ this,
        /* szDoTestStepFct */ SLOT(doTestStepGrpStartupDbStartupDatabase(ZS::Test::CTestStep*)) );

} // ctor

//------------------------------------------------------------------------------
CTSGrpStartupDb::~CTSGrpStartupDb()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
protected: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepGrpStartupDbCreateDatabase( ZS::Test::CTestStep* i_pTestStep )
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
        /* strMethod    */ "doTestStepGrpStartupDbCreateDatabase",
        /* strAddInfo   */ strAddTrcInfo );

    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    SServerHostSettings srvHostSettings;
    SCnctType           cnctType;
    EProtocolType       protType;
    ESocketType         sockType;
    int                 idxProtType;
    int                 idxSockType;

    // Test Step
    //----------

    // Default ctor creates database with:
    // ObjectName:          "Db"
    // ProtocolTypes:       [ZS]
    // SocketTypes:         {ZS: [Tcp, InProcMsg]}
    // MultiThreadedAccess: false
    m_pDb = new CDb(c_strObjNameDb);

    //m_pDb->setReqTimeout(m_iReqTimeout_ms);
    m_pDb->setKeepReqDscrInExecTree(m_bKeepReqDscrInExecTree);

    emit dbCreated(m_pDb);

    // Desired Values
    //---------------

    // Desired values must be set after creating database as "m_pObjLocal" of ZS::InProcMsg
    // ServerHostSettings must be set to the created database object.
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

            srvHostSettings.m_socketType = cnctType.m_socketType;

            if( cnctType.m_socketType == ESocketTypeTcp )
            {
                srvHostSettings.m_pObjLocal = nullptr;
                srvHostSettings.m_strLocalHostName = QHostInfo::localHostName();
                //srvHostSettings.m_hostAddrLocal;
                srvHostSettings.m_uLocalPort = 19637;
                srvHostSettings.m_uMaxPendingConnections = 30;
                //srvHostSettings.m_uBufferSize = 0;
            }
            else if( cnctType.m_socketType == ESocketTypeInProcMsg )
            {
                srvHostSettings.m_pObjLocal = m_pDb;
                srvHostSettings.m_strLocalHostName = c_strObjNameDb;
                //srvHostSettings.m_hostAddrLocal;
                //srvHostSettings.m_uLocalPort;
                //srvHostSettings.m_uMaxPendingConnections;
                //srvHostSettings.m_uBufferSize;
            }

            m_hshHostSettingsDefault[cnctType] = srvHostSettings;
            m_hshHostSettings[cnctType] = srvHostSettings;

            strDesiredValue  = protocolType2Str(protType) + ":" + socketType2Str(sockType) + ":";
            strDesiredValue += srvHostSettings.getConnectionString();
            strlstDesiredValues.append(strDesiredValue);

        } // for( idxSockType = 0; idxSockType < m_hsharSocketTypes[cnctType.m_protocolType].size(); idxSockType++ )
    } // for( idxProtType = 0; idxProtType < m_arProtocolTypes.size(); idxProtType++ )

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Actual Values
    //---------------

    QList<EProtocolType> arProtTypes = m_pDb->getProtocolTypes();

    for( idxProtType = 0; idxProtType < arProtTypes.size(); idxProtType++ )
    {
        protType = arProtTypes[idxProtType];

        QList<ESocketType> arSockTypes = m_pDb->getSocketTypes(protType);

        for( idxSockType = 0; idxSockType < arSockTypes.size(); idxSockType++ )
        {
            sockType = arSockTypes[idxSockType];

            strActualValue  = protocolType2Str(protType) + ":" + socketType2Str(sockType) + ":";
            strActualValue += m_pDb->getConnectionString(protType, sockType);
            strlstActualValues.append(strActualValue);

        } // for( idxSockType = 0; idxSockType < arSockTypes.size(); idxSockType++ )
    } // for( idxProtType = 0; idxProtType < arProtTypes.size(); idxProtType++ )

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepCreateDatabase

//------------------------------------------------------------------------------
void CTest::doTestStepGrpStartupDbCreateDataSet( ZS::Test::CTestStep* i_pTestStep )
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
        /* strMethod    */ "doTestStepGrpStartupDbCreateDataSet",
        /* strAddInfo   */ strAddTrcInfo );

    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strDesiredValue = "DS:" + c_strObjNameDbDS;
    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    if( m_pDb != nullptr )
    {
        try
        {
            m_pDb->createDataSet();
            m_pDSDb = m_pDb->getDataSet();

            emit dbDataSetCreated(m_pDSDb);
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
    }

    // Actual Values
    //---------------

    strActualValue = QString(m_pDSDb == nullptr ? "nullptr" : m_pDSDb->getDSObjKey() );
    strlstActualValues.append(strActualValue);

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepGrpStartupDbCreateDataSet

//------------------------------------------------------------------------------
void CTest::doTestStepGrpStartupDbInitSettings( ZS::Test::CTestStep* i_pTestStep )
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
        /* strMethod    */ "doTestStepGrpStartupDbInitSettings",
        /* strAddInfo   */ strAddTrcInfo );

    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    SServerHostSettings srvHostSettings;
    SServerHostSettings srvHostSettingsDefault;
    SCnctType           cnctType;
    EProtocolType       protType;
    ESocketType         sockType;
    int                 idxProtType;
    int                 idxSockType;

    // Desired Values
    //---------------

    CRequest reqDesired(
        /* strCreatorNameSpace */ CDb::NameSpace(),
        /* strCreatorClassName      */ CDb::ClassName(),
        /* pObjCreator              */ m_pDb,
        /* request                  */ CDb::ERequestUpdateHostSettings,
        /* strRequest               */ CDb::Request2Str(CDb::ERequestUpdateHostSettings),
        /* iParentId                */ -1,
        /* bKeepReqDscrInExecTree   */ false,
        /* iTimeout_ms              */ m_iReqTimeout_ms,
        /* bIsBlocking              */ false );
    reqDesired.setProgressInPerCent(100);
    reqDesired.setErrResult(ErrResultSuccess);

    strDesiredValue = "Req {" + reqDesired.getAddTrcInfoStr() + "}";
    strlstDesiredValues.append(strDesiredValue);
    strDesiredValue = "State: " + CDb::State2Str(CDb::EStateIdle);
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

            srvHostSettings = m_hshHostSettings[cnctType];
            srvHostSettingsDefault = m_hshHostSettingsDefault[cnctType];

            strDesiredValue  = protocolType2Str(protType) + ":" + socketType2Str(sockType) + ":";
            strDesiredValue += srvHostSettings.getConnectionString();
            strDesiredValue += " (Default: " + srvHostSettingsDefault.getConnectionString() + ")";
            strlstDesiredValues.append(strDesiredValue);

        } // for( idxSockType = 0; idxSockType < m_hsharSocketTypes[cnctType.m_protocolType].size(); idxSockType++ )
    } // for( idxProtType = 0; idxProtType < m_arProtocolTypes.size(); idxProtType++ )

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    CRequest* pReq = nullptr;

    if( m_pDb != nullptr )
    {
        try
        {
            //m_pDb->setDataSaveReadEnabled(true);
            //m_pDb->setDataFileFormat(EFileFormatXML);
            //m_pDb->setDataFileName(m_strDbDataFilePath);
            //m_pDb->setSchemaSaveReadEnabled(true);
            //m_pDb->setSchemaFileFormat(EFileFormatXML);
            //m_pDb->setSchemaFileName(m_strDbSchemaFilePath);

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

                    srvHostSettings = m_hshHostSettings[cnctType];
                    srvHostSettingsDefault = m_hshHostSettingsDefault[cnctType];

                    m_pDb->setDefaultHostSettings(protType, srvHostSettingsDefault);
                    m_pDb->setHostSettings(protType, srvHostSettings);

                } // for( idxSockType = 0; idxSockType < m_hsharSocketTypes[cnctType.m_protocolType].size(); idxSockType++ )
            } // for( idxProtType = 0; idxProtType < m_arProtocolTypes.size(); idxProtType++ )

            pReq = m_pDb->updateHostSettings(
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
    } // if( m_pDb != nullptr )

    // Actual Values
    //---------------

    if( m_pDb != nullptr )
    {
        strActualValue = "Req {" + QString(pReq == nullptr ? "nullptr" : pReq->getAddTrcInfoStr()) + "}";
        strlstActualValues.append(strActualValue);
        strActualValue = "State: " + CDb::State2Str(m_pDb->getState());
        strlstActualValues.append(strActualValue);

        for( idxProtType = 0; idxProtType < m_arProtocolTypes.size(); idxProtType++ )
        {
            protType = m_arProtocolTypes[idxProtType];
            cnctType.m_protocolType = protType;

            for( idxSockType = 0; idxSockType < m_hsharSocketTypes[cnctType.m_protocolType].size(); idxSockType++ )
            {
                sockType = m_hsharSocketTypes[cnctType.m_protocolType][idxSockType];
                cnctType.m_socketType = sockType;

                strActualValue  = protocolType2Str(protType) + ":" + socketType2Str(sockType) + ":";
                strActualValue += m_pDb->getConnectionString(protType, sockType);
                strActualValue += " (Default: " + m_pDb->getDefaultConnectionString(protType, sockType) + ")";
                strlstActualValues.append(strActualValue);

            } // for( idxSockType = 0; idxSockType < m_hsharSocketTypes[cnctType.m_protocolType].size(); idxSockType++ )
        } // for( idxProtType = 0; idxProtType < m_arProtocolTypes.size(); idxProtType++ )
    } // if( m_pDb != nullptr )

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepGrpStartupDbInitSettings

//------------------------------------------------------------------------------
void CTest::doTestStepGrpStartupDbStartupDatabase( ZS::Test::CTestStep* i_pTestStep )
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
        /* strMethod    */ "doTestStepGrpStartupDbStartupDatabase",
        /* strAddInfo   */ strAddTrcInfo );

    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    CRequest reqDesired(
        /* strCreatorNameSpace */ CDb::NameSpace(),
        /* strCreatorClassName      */ CDb::ClassName(),
        /* pObjCreator              */ m_pDb,
        /* request                  */ CDb::ERequestStartup,
        /* strRequest               */ CDb::Request2Str(CDb::ERequestStartup),
        /* iParentId                */ -1,
        /* bKeepReqDscrInExecTree   */ false,
        /* iTimeout_ms              */ m_iReqTimeout_ms,
        /* bIsBlocking              */ true );
    reqDesired.setProgressInPerCent(100);
    reqDesired.setErrResult(ErrResultSuccess);

    strDesiredValue = "Req {" + reqDesired.getAddTrcInfoStr() + "}";
    strlstDesiredValues.append(strDesiredValue);
    strDesiredValue = "State: " + CDb::State2Str(CDb::EStateListening);
    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    CRequest* pReq = nullptr;

    if( m_pDb != nullptr )
    {
        try
        {
            pReq = m_pDb->startup(
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
    } // if( m_pDb != nullptr )

    // Actual Values
    //---------------

    if( m_pDb != nullptr )
    {
        strActualValue = "Req {" + QString(pReq == nullptr ? "nullptr" : pReq->getAddTrcInfoStr()) + "}";
        strlstActualValues.append(strActualValue);
        strActualValue = "State: " + CDb::State2Str(m_pDb->getState());
        strlstActualValues.append(strActualValue);

    } // if( m_pDb != nullptr )

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepGrpStartupDbStartupDatabase
