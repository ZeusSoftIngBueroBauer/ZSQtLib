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
#include <QtNetwork/qhostinfo.h>

#include "Test.h"
#include "TSGrpStartup.h"
#include "TSGrpShutdown.h"

#include "ZSData/ZSDatabase.h"
#include "ZSData/ZSDatabaseClient.h"
#include "ZSPhysVal/ZSPhysVal.h"
#include "ZSTest/ZSTestStepAdminObj.h"
#include "ZSTest/ZSTestStepAdminObjPool.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::Data;
using namespace ZS::PhysVal;
using namespace ZS::Apps::Test::Data;


/*******************************************************************************
class CTest
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

const QString CTest::c_strObjNameDb                    = "Db";
const QString CTest::c_strObjNameDbDS                  = "Db";
const QString CTest::c_strObjNameDbClt1                = "DbClt1";
const QString CTest::c_strObjNameDbClt2                = "DbClt2";
const QString CTest::c_strObjNameDbClt3                = "DbClt3";
const QString CTest::c_strObjNameDbClt1DS              = "DbClt1";
const QString CTest::c_strObjNameDbClt2DS              = "DbClt2";
const QString CTest::c_strObjNameDbClt3DS              = "DbClt3";
//const QString CTest::c_strObjNameGrpCommon           = "Common";
//const QString CTest::c_strObjNameDESamplesPerSec     = "SamplesPerSec";
//const QString CTest::c_strObjNameDPSingleEnumValue   = "SingleEnumValue";
//const QString CTest::c_strObjNameDPOneDimEnumValues  = "OneDimEnumValues";
//const QString CTest::c_strObjNameDPMoreDimEnumValues = "MoreDimEnumValues";
//const QString CTest::c_strObjNameDPSinglePhysValue   = "SinglePhysValue";
//const QString CTest::c_strObjNameDPOneDimPhysValues  = "OneDimPhysValues";
//const QString CTest::c_strObjNameDPMoreDimPhysValues = "MoreDimPhysValues";
//const QString CTest::c_strObjNameGrp1                = "1";
//const QString CTest::c_strObjNameGrp2                = "2";

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTest::CTest(
    const QString& i_strTestStepsFileName,
    const QString& i_strDbSchemaFilePath,
    const QString& i_strDbDataFilePath ) :
//------------------------------------------------------------------------------
    ZS::Test::CTest(
        /* strName              */ "ZS::Data",
        /* strTestStepsFileName */ i_strTestStepsFileName,
        /* strNodeSeparator     */ "\\",
        /* iTestStepInterval_ms */ 0 ),
    m_bKeepReqDscrInExecTree(false),
    m_iReqTimeout_ms(10000),
    m_iTestStepTimeoutSlotInterval_ms(100),
    m_iTestStepTimeoutSlotCalled(0),
    m_strlstActualValuesTmp(),
    // Database
    m_arProtocolTypes(),
    m_hsharSocketTypes(),
    m_hshHostSettingsDefault(),
    m_hshHostSettings(),
    m_strDbSchemaFilePath(i_strDbSchemaFilePath),
    m_strDbDataFilePath(i_strDbDataFilePath),
    m_pDb(nullptr),
    m_pDSDb(nullptr),
    // Database Clients
    m_hshWatchDogSettingsDefault(),
    m_hshWatchDogSettings(),
    m_pDbClt1(nullptr),
    m_socketDscrDbClt1Tmp(),
    m_pDSDbClt1(nullptr),
    m_pDbClt2(nullptr),
    m_socketDscrDbClt2Tmp(),
    m_pDSDbClt2(nullptr),
    m_pDbClt3(nullptr),
    m_socketDscrDbClt3Tmp(),
    m_pDSDbClt3(nullptr),
    m_hshClientRequestQueueReqsInProgress()
{
    // Create test input data
    //-----------------------

    m_arProtocolTypes.append(EProtocolTypeZSXML);
    m_arProtocolTypes.append(EProtocolTypeZSMsg);

    m_hsharSocketTypes[EProtocolTypeZSXML].append(ESocketTypeTcp);
    m_hsharSocketTypes[EProtocolTypeZSMsg].append(ESocketTypeInProcMsg);

    STimerSettings watchDogSettings;
    SCnctType      cnctType;
    EProtocolType  protType;
    ESocketType    sockType;
    int            idxProtType;
    int            idxSockType;

    for( idxProtType = 0; idxProtType < m_arProtocolTypes.size(); idxProtType++ )
    {
        protType = m_arProtocolTypes[idxProtType];
        cnctType.m_protocolType = protType;

        for( idxSockType = 0; idxSockType < m_hsharSocketTypes[cnctType.m_protocolType].size(); idxSockType++ )
        {
            sockType = m_hsharSocketTypes[cnctType.m_protocolType][idxSockType];
            cnctType.m_socketType = sockType;

            // Please note that the host settings of the InProcMsg socket type
            // can only be initialized after the database has been created as
            // the local object is the reference to the database and the name
            // of the "LocalHostName" is the object name of the database.
            //m_hshHostSettingsDefault[cnctType] = hostSettings;
            //m_hshHostSettings[cnctType] = hostSettings;

            if( cnctType.m_socketType == ESocketTypeTcp )
            {
                watchDogSettings.m_bEnabled = true;
                watchDogSettings.m_iInterval_ms = 5000;
                watchDogSettings.m_iTimeout_ms = 11000;
            }
            else if( cnctType.m_socketType == ESocketTypeInProcMsg )
            {
                watchDogSettings.m_bEnabled = false;
                watchDogSettings.m_iInterval_ms = 0;
                watchDogSettings.m_iTimeout_ms = 0;
            }

            m_hshWatchDogSettingsDefault[cnctType] = watchDogSettings;
            m_hshWatchDogSettings[cnctType] = watchDogSettings;

        } // for( idxSockType = 0; idxSockType < m_hsharSocketTypes[cnctType.m_protocolType].size(); idxSockType++ )
    } // for( idxProtType = 0; idxProtType < m_arProtocolTypes.size(); idxProtType++ )

    // Create test groups
    //-------------------

    new CTSGrpStartup(this);
    new CTSGrpShutdown(this);

    // Recall test admin object settings
    //----------------------------------

    m_pAdminObjPool->read_(i_strTestStepsFileName);

} // default ctor

//------------------------------------------------------------------------------
CTest::~CTest()
//------------------------------------------------------------------------------
{
    m_pAdminObjPool->save_();

    try
    {
        delete m_pDbClt1;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pDbClt2;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pDbClt3;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pDb;
    }
    catch(...)
    {
    }

    m_bKeepReqDscrInExecTree = false;
    m_iReqTimeout_ms = 0;
    m_iTestStepTimeoutSlotInterval_ms = 0;
    m_iTestStepTimeoutSlotCalled = 0;
    m_strlstActualValuesTmp.clear();
    // Database
    m_arProtocolTypes.clear();
    m_hsharSocketTypes.clear();
    m_hshHostSettingsDefault.clear();
    m_hshHostSettings.clear();
    //m_strDbSchemaFilePath.clear();
    //m_strDbDataFilePath.clear();
    m_pDb = nullptr;
    m_pDSDb = nullptr;
    // Database Clients
    m_hshWatchDogSettingsDefault.clear();
    m_hshWatchDogSettings.clear();
    m_pDbClt1 = nullptr;
    m_pDSDbClt1 = nullptr;
    m_pDbClt2 = nullptr;
    m_pDSDbClt2 = nullptr;
    m_pDbClt3 = nullptr;
    m_pDSDbClt3 = nullptr;
    m_hshClientRequestQueueReqsInProgress.clear();

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::setKeepReqDscrInExecTree( bool i_bKeep )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = bool2Str(i_bKeep);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setKeepReqDscrInExecTree",
        /* strAddInfo   */ strAddTrcInfo );

    m_bKeepReqDscrInExecTree = i_bKeep;

} // setKeepReqDscrInExecTree

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::setReqTimeoutInMs( int i_iTimeout_ms )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = QString::number(i_iTimeout_ms);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setReqTimeoutInMs",
        /* strAddInfo   */ strAddTrcInfo );

    m_iReqTimeout_ms = i_iTimeout_ms;

} // setReqTimeoutInMs
