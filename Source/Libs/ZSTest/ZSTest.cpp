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

#include "ZSTest/ZSTest.h"
#include "ZSTest/ZSTestStepAdminObjPool.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"

#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMsg.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Test;
using namespace ZS::Trace;


/*******************************************************************************
class CTest
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
const ZS::System::SEnumEntry s_arEnumStrStates[CTest::EStateCount] =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( CTest::EStateIdle,    "Idle"    ),
    /* 1 */ SEnumEntry( CTest::EStateInit,    "Init"    ),
    /* 2 */ SEnumEntry( CTest::EStateRunning, "Running" ),
    /* 3 */ SEnumEntry( CTest::EStatePaused,  "Paused"  )
};

//------------------------------------------------------------------------------
QString CTest::State2Str( int i_iState )
//------------------------------------------------------------------------------
{
    return ZS::System::SEnumEntry::enumerator2Str(s_arEnumStrStates,EStateCount,i_iState);
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTest::CTest(
    const QString& i_strName,
    const QString& i_strObjPoolFileName,
    const QString& i_strNodeSeparator,
    int            i_iTestStepInterval_ms ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pAdminObjPool(nullptr),
    m_pTestStepCurr(nullptr),
    m_iTestStepGroup(0),
    m_arfctsDoTestStepGroups(),
    m_iTestStepInterval_ms(i_iTestStepInterval_ms),
    m_state(EStateIdle),
    m_runMode(ERunMode::Continuous),
    m_bDoTestStepPending(false),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Test", "CTest", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    m_pAdminObjPool = new CTestStepAdminObjPool(
        /* pTest              */ this,
        /* strObjPoolFileName */ i_strObjPoolFileName,
        /* strNodeSeparator   */ i_strNodeSeparator );

    // Should be called by derived class if desired.
    //m_pAdminObjPool->recall();

} // default ctor

//------------------------------------------------------------------------------
CTest::~CTest()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    // Should be called by derived class if desired.
    //m_pAdminObjPool->save();

    try
    {
        delete m_pAdminObjPool;
    }
    catch(...)
    {
    }

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pAdminObjPool = nullptr;
    m_pTestStepCurr = nullptr;
    m_iTestStepGroup = 0;
    m_arfctsDoTestStepGroups.clear();
    m_iTestStepInterval_ms = 0;
    m_state = static_cast<EState>(0);
    m_runMode = static_cast<ERunMode>(0);
    m_bDoTestStepPending = false;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CTestStepGroup* CTest::getTestStepGroupRoot()
//------------------------------------------------------------------------------
{
    return m_pAdminObjPool->getTestStepGroupRoot();
}

//------------------------------------------------------------------------------
CTestStepGroup* CTest::getTestStepGroup( const QString& i_strGroupName, const QString& i_strGroupNameParent )
//------------------------------------------------------------------------------
{
    if( i_strGroupName.isEmpty() )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"GroupName is empty");
    }

    return m_pAdminObjPool->getTestStepGroup(
        /* strGroupName       */ i_strGroupName,
        /* strGroupPathParent */ i_strGroupNameParent,
        /* enabled            */ EEnabledUndefined );
}

//------------------------------------------------------------------------------
CTestStepGroup* CTest::getTestStepGroup( const QString& i_strGroupName, CTestStepGroup* i_pTestGroupParent )
//------------------------------------------------------------------------------
{
    if( i_strGroupName.isEmpty() )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"GroupName is empty");
    }

    return m_pAdminObjPool->getTestStepGroup(
        /* strGroupName     */ i_strGroupName,
        /* pTestGroupParent */ i_pTestGroupParent,
        /* enabled          */ EEnabledUndefined );
}

//------------------------------------------------------------------------------
CTestStep* CTest::getTestStep( const QString& i_strName, const QString& i_strGroupNameParent )
//------------------------------------------------------------------------------
{
    if( i_strName.isEmpty() )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"Name is empty");
    }

    return m_pAdminObjPool->getTestStep(
        /* strName            */ i_strName,
        /* strGroupPathParent */ i_strGroupNameParent,
        /* enabled            */ EEnabledUndefined );
}

//------------------------------------------------------------------------------
CTestStep* CTest::getTestStep( const QString& i_strName, CTestStepGroup* i_pTestGroupParent )
//------------------------------------------------------------------------------
{
    if( i_strName.isEmpty() )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"Name is empty");
    }

    return m_pAdminObjPool->getTestStep(
        /* strName          */ i_strName,
        /* pTestGroupParent */ i_pTestGroupParent,
        /* enabled          */ EEnabledUndefined );
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::setTestStepInterval( int i_iTestStepInterval_ms )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Interval: " + QString::number(i_iTestStepInterval_ms);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setTestStepInterval",
        /* strAddInfo   */ strAddTrcInfo );

    m_iTestStepInterval_ms = i_iTestStepInterval_ms;

} // setTestStepInterval

//------------------------------------------------------------------------------
int CTest::getTestStepIntervalInMs() const
//------------------------------------------------------------------------------
{
    return m_iTestStepInterval_ms;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::init()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "init",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->getName(true));
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_state == EStateIdle )
    {
        //m_pAdminObjPool->reset();
        //m_pAdminObjPool->testStarted();

        setState(EStateInit);

        int iTestStepInterval_ms = m_iTestStepInterval_ms;

        m_iTestStepInterval_ms = 0;

        m_pAdminObjPool->beginInitTest();

        doTestStep();

        m_pAdminObjPool->endInitTest();

        m_iTestStepInterval_ms = iTestStepInterval_ms;

        setState(EStateIdle);

    } // if( m_state == EStateIdle )

} // init

//------------------------------------------------------------------------------
void CTest::start()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "start",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->getName(true));
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_state == EStateIdle )
    {
        m_pAdminObjPool->reset();
        m_pAdminObjPool->testStarted();

        setCurrentTestStep(nullptr);
    }

    if( m_state == EStateIdle || m_state == EStatePaused )
    {
        setState(EStateRunning);
        setRunMode(ERunMode::Continuous);

        triggerNextTestStep();

    } // if( m_state == EStateIdle )

} // start

//------------------------------------------------------------------------------
void CTest::step()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "step",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->getName(true));
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_state == EStateIdle )
    {
        m_pAdminObjPool->reset();
        m_pAdminObjPool->testStarted();

        setCurrentTestStep(nullptr);
    }

    if( m_state == EStateIdle || m_state == EStatePaused )
    {
        setState(EStateRunning);
        setRunMode(ERunMode::SingleStep);

        triggerNextTestStep();

    } // if( m_state == EStateIdle || m_state == EStatePaused )

} // step

//------------------------------------------------------------------------------
void CTest::stop()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "stop",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->getName(true));
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_state == EStateRunning || m_state == EStatePaused )
    {
        setState(EStateIdle);

        m_pAdminObjPool->testEnded();
    }

} // stop

//------------------------------------------------------------------------------
void CTest::abort()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "abort",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->getName(true));
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_state == EStateRunning || m_state == EStatePaused )
    {
        setState(EStateIdle);

        m_pAdminObjPool->testAborted();
    }

} // abort

//------------------------------------------------------------------------------
void CTest::pause()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "pause",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->getName(true));
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_state == EStateRunning )
    {
        setState(EStatePaused);
    }

} // pause

//------------------------------------------------------------------------------
void CTest::resume()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "resume",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_state == EStatePaused )
    {
        setState(EStateRunning);
        setRunMode(ERunMode::Continuous);

        triggerNextTestStep();
    }

} // resume

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::triggerDoTestStep( int i_iInterval_ms )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Interval: " + QString::number(i_iInterval_ms);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "triggerDoTestStep",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->getName(true));
        mthTracer.trace(strAddTrcInfo);
    }

    if( !m_bDoTestStepPending )
    {
        m_bDoTestStepPending = true;

        int iInterval_ms = i_iInterval_ms;

        if( iInterval_ms < 0.0 )
        {
            iInterval_ms = m_iTestStepInterval_ms;
        }

        if( iInterval_ms <= 0.0 )
        {
            CMsgReqContinue* pMsgReq = new CMsgReqContinue(
                /* pObjSender       */ this,
                /* pObjReceiver     */ this,
                /* bMustBeConfirmed */ false,
                /* iReqId           */ -1,
                /* iMsgId           */ -1 );
            POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer);
        }
        else
        {
            QTimer::singleShot( iInterval_ms, this, SLOT(doTestStep()) );
        }
    } // if( !m_bDoTestStepPending )

} // triggerDoTestStep

//------------------------------------------------------------------------------
void CTest::triggerNextTestStep( int i_iInterval_ms )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Interval: " + QString::number(i_iInterval_ms);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "triggerNextTestStep",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->getName(true));
        mthTracer.trace(strAddTrcInfo);
    }

    CTestStep* pTestStep = getNextTestStep(m_pTestStepCurr);

    setCurrentTestStep(pTestStep);

    if( m_pTestStepCurr != nullptr )
    {
        triggerDoTestStep(i_iInterval_ms);
    }
    else // if( m_pTestStepCurr == nullptr )
    {
        if( m_state == EStateInit || m_state == EStateRunning ) // not Paused or already Stopped
        {
            stop();
        }
    }
} // triggerNextTestStep

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::onTestStepGroupChanged( CTestStepGroup* i_pTSGrp )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelRuntimeInfo) )
    {
        strAddTrcInfo = "Group: " + QString(i_pTSGrp == nullptr ? "nullptr" : i_pTSGrp->getName(true));
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ 3,
        /* strMethod    */ "onTestStepGroupChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pAdminObjPool != nullptr )
    {
        m_pAdminObjPool->onTestStepGroupChanged(i_pTSGrp);
    }

} // onTestStepGroupChanged

//------------------------------------------------------------------------------
void CTest::onTestStepChanged( CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelRuntimeInfo) )
    {
        strAddTrcInfo = "Step: " + QString(i_pTestStep == nullptr ? "nullptr" : i_pTestStep->getName(true));
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ 3,
        /* strMethod    */ "onTestStepChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pAdminObjPool != nullptr )
    {
        m_pAdminObjPool->onTestStepChanged(i_pTestStep);
    }

} // onTestStepChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStep()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "doTestStep",
        /* strAddInfo   */ "" );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->getName(true));
        mthTracer.trace(strAddTrcInfo);
    }

    m_bDoTestStepPending = false;

    if( m_arfctsDoTestStepGroups.size() == 0 )
    {
        if( m_pTestStepCurr != nullptr )
        {
            m_pTestStepCurr->testStarted();

            m_pTestStepCurr->doTestStep();

            if( m_pTestStepCurr->isFinished() )
            {
                onCurrentTestStepFinished(m_pTestStepCurr);
            }
            else if( !m_pTestStepCurr->isFinished() )
            {
                if( !QObject::connect(
                    /* pObjSender   */ m_pTestStepCurr,
                    /* szSignal     */ SIGNAL(finished(ZS::Test::CTestStep*)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onCurrentTestStepFinished(ZS::Test::CTestStep*)) ) )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                }
            }
        } // if( m_pTestStepCurr != nullptr )

        else // if( m_pTestStepCurr == nullptr )
        {
            if( m_state == EStateInit || m_state == EStateRunning ) // not Paused or already Stopped
            {
                stop();
            }
        }
    } // if( m_arfctsDoTestStepGroups.size() == 0 )

    else // if( m_arfctsDoTestStepGroups.size() > 0 )
    {
        if( m_iTestStepGroup >= 0 && m_iTestStepGroup < m_arfctsDoTestStepGroups.size() )
        {
            CTestStepGroup* pTSGrpRoot = m_pAdminObjPool->getTestStepGroupRoot();

            if( m_state == EStateInit || m_state == EStateRunning ) // not Paused or Stopped
            {
                m_arfctsDoTestStepGroups[m_iTestStepGroup](this,pTSGrpRoot);
            }
        }
        else // if( m_iTestStepGroup m_iTestStepGroup >= m_arfctsDoTestStepGroups.size() )
        {
            if( m_state == EStateInit || m_state == EStateRunning ) // not Paused or already Stopped
            {
                stop();
            }
        }
    } // if( m_arfctsDoTestStepGroups.size() > 0 )

} // doTestStep

//------------------------------------------------------------------------------
void CTest::onCurrentTestStepFinished( CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = QString(i_pTestStep == nullptr ? "nullptr" : i_pTestStep->getName(true));
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onCurrentTestStepFinished",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->getName(true));
        mthTracer.trace(strAddTrcInfo);
    }

    if( i_pTestStep == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    }
    if( i_pTestStep != m_pTestStepCurr )
    {
        throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "i_pTestStep != m_pTestStepCurr");
    }

    if( !i_pTestStep->isFinished() )
    {
        throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "!i_pTestStep->isFinished()");
    }

    QObject::disconnect(
        /* pObjSender   */ m_pTestStepCurr,
        /* szSignal     */ SIGNAL(finished(ZS::Test::CTestStep*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCurrentTestStepFinished(ZS::Test::CTestStep*)) );

    if( m_state == EStateInit || m_state == EStateRunning ) // not Paused or Stopped
    {
        if( m_runMode == ERunMode::Continuous )
        {
            CTestStep* pTestStepNext = getNextTestStep(m_pTestStepCurr);

            if( pTestStepNext != nullptr && pTestStepNext->isBreakpointSet() )
            {
                pause();
            }
            else
            {
                triggerNextTestStep();
            }
        }
        else if( m_runMode == ERunMode::SingleStep )
        {
            pause();
        }
    }

} // onCurrentTestStepFinished

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::setState( EState i_state )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "State: " + State2Str(i_state);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setState",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->getName(true));
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_state != i_state )
    {
        EState statePrev = m_state;
        m_state = i_state;
        emit stateChanged(m_state,statePrev);
    }

} // setState

//------------------------------------------------------------------------------
void CTest::setRunMode( ERunMode i_runMode )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "RunMode: " + CEnumRunMode::toString(i_runMode);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setRunMode",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->getName(true));
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_runMode != i_runMode )
    {
        //ERunMode runModePrev = m_runMode;
        m_runMode = i_runMode;
    }

} // setRunMode

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CTestStep* CTest::getNextTestStep( CTestStep* i_pTestStepCurr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "TestStepCurr: " + QString(i_pTestStepCurr == nullptr ? "nullptr" : i_pTestStepCurr->getName(true));
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "getNextTestStep",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->getName(true));
        mthTracer.trace(strAddTrcInfo);
    }

    CObjPoolTreeEntry* pTreeEntryParent   = nullptr;
    CObjPoolTreeEntry* pTreeEntryFinished = nullptr;

    if( i_pTestStepCurr == nullptr )
    {
        CTestStepGroup* pTSGrpRoot = getTestStepGroupRoot();
        pTreeEntryParent = pTSGrpRoot->getTreeEntry();
    }
    else // if( i_pTestStepCurr != nullptr )
    {
        pTreeEntryFinished = i_pTestStepCurr->getTreeEntry();
        pTreeEntryParent = pTreeEntryFinished->getParentEntry();
    }

    return getNextTestStep(pTreeEntryParent, pTreeEntryFinished);

} // getNextTestStep

//------------------------------------------------------------------------------
CTestStep* CTest::getNextTestStep(
    CObjPoolTreeEntry* i_pTreeEntryParent,
    CObjPoolTreeEntry* i_pTreeEntryFinished )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "TreeEntryParent: " + QString(i_pTreeEntryParent == nullptr ? "nullptr" : i_pTreeEntryParent->getNodeName(true));
        strAddTrcInfo += ", TreeEntryFinished: " + QString(i_pTreeEntryFinished == nullptr ? "nullptr" : i_pTreeEntryFinished->getNodeName(true));
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "getNextTestStep",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->getName(true));
        mthTracer.trace(strAddTrcInfo);
    }

    CTestStep* pTestStep = nullptr;

    // No parent: no next test step.
    if( i_pTreeEntryParent != nullptr )
    {
        int iTreeEntryFinishedRowIdx = -1;

        if( i_pTreeEntryFinished != nullptr )
        {
            iTreeEntryFinishedRowIdx = i_pTreeEntryFinished->getRowId();
        }

        if( (iTreeEntryFinishedRowIdx+1) < i_pTreeEntryParent->getChildCount() )
        {
            CObjPoolTreeEntry* pTreeEntryChild;
            int                idxChild;

            for( idxChild = (iTreeEntryFinishedRowIdx+1); idxChild < i_pTreeEntryParent->getChildCount(); idxChild++ )
            {
                pTreeEntryChild = i_pTreeEntryParent->getChildEntry(idxChild);

                if( pTreeEntryChild->getEntryType() == EObjPoolEntryTypeObject )
                {
                    pTestStep = reinterpret_cast<CTestStep*>(pTreeEntryChild->getObj());

                    if( pTestStep != nullptr && !pTestStep->isEnabled() )
                    {
                        pTestStep = nullptr;
                    }
                }
                else
                {
                    pTestStep = getNextTestStep(pTreeEntryChild, nullptr);
                }
                if( pTestStep != nullptr )
                {
                    break;
                }
            } // for( idxChild = (iTreeEntryFinishedRowIdx+1); idxChild < pTreeEntry->getChildCount(); idxChild++ )
        } // if( (iTreeEntryFinishedRowIdx+1) < i_pTreeEntryParent->getChildCount() )

        else // if( (iTreeEntryFinishedRowIdx+1) >= i_pTreeEntryParent->getChildCount() )
        {
            pTestStep = getNextTestStep(i_pTreeEntryParent->getParentEntry(), i_pTreeEntryParent);

        } // if( (iTreeEntryFinishedRowIdx+1) >= i_pTreeEntryParent->getChildCount() )
    } // if( i_pTreeEntryParent != nullptr )

    return pTestStep;

} // getNextTestStep

//------------------------------------------------------------------------------
void CTest::setCurrentTestStep( CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Step: " + QString(i_pTestStep == nullptr ? "nullptr" : i_pTestStep->getName(true));
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setCurrentTestStep",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->getName(true));
        mthTracer.trace(strAddTrcInfo);
    }

    m_pTestStepCurr = i_pTestStep;

    emit currentTestStepChanged(m_pTestStepCurr);

} // setCurrentTestStep

/*==============================================================================
protected: // overridables of inherited class QObject (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
bool CTest::event( QEvent* i_pMsg )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    CMsg* pMsg = dynamic_cast<CMsg*>(i_pMsg);

    if( pMsg != nullptr )
    {
        bEventHandled = true;

        QString strAddTrcInfo;

        if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
        {
            int iAddTrcInfoDetails = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelInternalStates ? 1 : 0;
            strAddTrcInfo = "Msg { " + pMsg->getAddTrcInfoStr(iAddTrcInfoDetails) + " }";
        }

        CMethodTracer mthTracer(
            /* pAdminObj    */ m_pTrcAdminObj,
            /* iFilterLevel */ ETraceDetailLevelMethodCalls,
            /* strMethod    */ "event",
            /* strAddInfo   */ strAddTrcInfo );

        if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
        {
            strAddTrcInfo  = "State: " + State2Str(m_state);
            strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->getName(true));
            mthTracer.trace(strAddTrcInfo);
        }

        if( pMsg->getMsgType() == EBaseMsgTypeReqContinue )
        {
            doTestStep();
        }
    } // if( pMsg != nullptr )

    if( !bEventHandled )
    {
        bEventHandled = QObject::event(i_pMsg);
    }
    return bEventHandled;

} // event
