/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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
#include "ZSTest/ZSTestStepIdxTree.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"

#include "ZSSys/ZSSysApp.h"
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
Type definitions and constants
*******************************************************************************/

/*******************************************************************************
class CTest
*******************************************************************************/

/*==============================================================================
public: // class methods to get default file paths
==============================================================================*/

//------------------------------------------------------------------------------
QString CTest::GetDefaultTestStepsAbsFilePath( const QString& i_strIniFileScope )
//------------------------------------------------------------------------------
{
    QString strAppConfigDir = ZS::System::getAppConfigDir(i_strIniFileScope);

    QString strAppNameNormalized = QCoreApplication::applicationName();

    // The application name may contain characters which are invalid in file names:
    strAppNameNormalized.remove(":");
    strAppNameNormalized.remove(" ");
    strAppNameNormalized.remove("\\");
    strAppNameNormalized.remove("/");
    strAppNameNormalized.remove("<");
    strAppNameNormalized.remove(">");

    QString strFileSuffix = "xml";
    QString strFileBaseName = strAppNameNormalized + "-TestSteps";

    return strAppConfigDir + "/" + strFileBaseName + "." + strFileSuffix;

} // GetDefaultAdminObjFileAbsoluteFilePath

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTest::CTest(
    const QString& i_strName,
    const QString& i_strTestStepsAbsFilePath,
    const QString& i_strNodeSeparator,
    int            i_iTestStepInterval_ms ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pIdxTree(nullptr),
    m_strTestStepsAbsFilePath(i_strTestStepsAbsFilePath),
    m_pTestStepCurr(nullptr),
    m_iTestStepInterval_ms(i_iTestStepInterval_ms),
    m_state(ETestState::Idle),
    m_runMode(ERunMode::Continuous),
    m_bDoTestStepPending(false),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Test", "CTest", objectName());

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "Name: " + i_strName;
        strMthInArgs += ", FileName: " + i_strTestStepsAbsFilePath;
        strMthInArgs += ", Interval: " + QString::number(i_iTestStepInterval_ms) + " ms";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    m_pIdxTree = new CTestStepIdxTree(this);

    if( m_strTestStepsAbsFilePath.isEmpty() )
    {
        m_strTestStepsAbsFilePath = GetDefaultTestStepsAbsFilePath();
    }

    // Should be called by derived class if desired.
    //m_pIdxTree->recall();

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
    //m_pIdxTree->save();

    try
    {
        delete m_pIdxTree;
    }
    catch(...)
    {
    }

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pIdxTree = nullptr;
    //m_strTestStepsAbsFilePath;
    m_pTestStepCurr = nullptr;
    m_iTestStepInterval_ms = 0;
    m_state = static_cast<ETestState>(0);
    m_runMode = static_cast<ERunMode>(0);
    m_bDoTestStepPending = false;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Saves the settings of the test steps and test step groups in an XML file.

    The attributes (enabled, breakpoints) will be save so that they can be
    recalled on restarting the test.

    Please note that only the settings for those index tree entries may be recalled
    which again will be found on recalling the test tree.

    @param i_strAbsFilePath [in]
        Absolute path name of the test steps file.
        If an empty string is passed the default file path is used.
        The default file path has either been provided as argument to the ctor
        or has been automatically detected when creating the test instance.
*/
SErrResultInfo CTest::save( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "AbsFilePath: " + i_strAbsFilePath;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "save",
        /* strMthInArgs */ strMthInArgs );

    QString strAbsFilePath = i_strAbsFilePath;

    if( strAbsFilePath.isEmpty() )
    {
        strAbsFilePath = m_strTestStepsAbsFilePath;
    }

    SErrResultInfo errResultInfo = m_pIdxTree->save(strAbsFilePath);

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(errResultInfo);
    }

    return errResultInfo;

} // save

//------------------------------------------------------------------------------
/*! Recalls the settings of the test steps and test step groups from an XML file.

    The attributes (enabled, breakpoints) will be recalled so that they can be
    reused on restarting the test.

    Please note that only the settings for those index tree entries may be recalled
    which will still be found in the test tree. Test steps and groups which will
    not be found will be ignored.

    @param i_strAbsFilePath [in]
        Absolute path name of the test steps file.
        If an empty string is passed the default file path is used.
        The default file path has either been provided as argument to the ctor
        or has been automatically detected when creating the test instance.
*/
SErrResultInfo CTest::recall( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "AbsFilePath: " + i_strAbsFilePath;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "recall",
        /* strMthInArgs */ strMthInArgs );

    QString strAbsFilePath = i_strAbsFilePath;

    if( strAbsFilePath.isEmpty() )
    {
        strAbsFilePath = m_strTestStepsAbsFilePath;
    }

    SErrResultInfo errResultInfo = m_pIdxTree->recall(strAbsFilePath);

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(errResultInfo);
    }

    return errResultInfo;

} // recall

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

    if( m_iTestStepInterval_ms != i_iTestStepInterval_ms )
    {
        m_iTestStepInterval_ms = i_iTestStepInterval_ms;

        emit testStepIntervalChanged(m_iTestStepInterval_ms);
    }

} // setTestStepInterval

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! Starts the test or continues a paused test.

*/
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
        strAddTrcInfo  = "State: " + m_state.toString();
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->path());
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_state == ETestState::Idle )
    {
        m_pIdxTree->reset();

        setCurrentTestStep(nullptr);
    }

    if( m_state == ETestState::Idle || m_state == ETestState::Paused )
    {
        setState(ETestState::Running);
        setRunMode(ERunMode::Continuous);

        triggerNextTestStep();
    }

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
        strAddTrcInfo  = "State: " + m_state.toString();
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->path());
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_state == ETestState::Idle )
    {
        m_pIdxTree->reset();

        setCurrentTestStep(nullptr);
    }

    if( m_state == ETestState::Idle || m_state == ETestState::Paused )
    {
        setState(ETestState::Running);
        setRunMode(ERunMode::SingleStep);

        triggerNextTestStep();
    }

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
        strAddTrcInfo  = "State: " + m_state.toString();
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->path());
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_state == ETestState::Running || m_state == ETestState::Paused )
    {
        setState(ETestState::Idle);
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
        strAddTrcInfo  = "State: " + m_state.toString();
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->path());
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_state == ETestState::Running || m_state == ETestState::Paused )
    {
        setState(ETestState::Idle);
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
        strAddTrcInfo  = "State: " + m_state.toString();
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->path());
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_state == ETestState::Running )
    {
        setState(ETestState::Paused);
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

    if( m_state == ETestState::Paused )
    {
        setState(ETestState::Running);
        setRunMode(ERunMode::Continuous);

        triggerNextTestStep();
    }

} // resume

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
/*! This slot method is triggered either by a timer if the test step interval
    is greater than 0 seconds or via the tests event method on receiving the
    continue request message.

    The current test steps method "doTestStep" is called. If there is no current
    test step the test has been finished.
*/
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
        strAddTrcInfo  = "State: " + m_state.toString();
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->path());
        mthTracer.trace(strAddTrcInfo);
    }

    m_bDoTestStepPending = false;

    if( m_pTestStepCurr != nullptr )
    {
        //m_pTestStepCurr->testStarted();

        m_pTestStepCurr->doTestStep();

        if( m_pTestStepCurr->isFinished() )
        {
            onCurrentTestStepFinished(m_pTestStepCurr);
        }
        else if( !m_pTestStepCurr->isFinished() )
        {
            if( !QObject::connect(
                /* pObjSender   */ m_pTestStepCurr,
                /* szSignal     */ SIGNAL(testStepFinished(ZS::Test::CTestStep*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onCurrentTestStepFinished(ZS::Test::CTestStep*)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
        }
    } // if( m_pTestStepCurr != nullptr )

    else // if( m_pTestStepCurr == nullptr )
    {
        if( m_state == ETestState::Running ) // not Paused or already Stopped
        {
            stop();
        }
    }

} // doTestStep

//------------------------------------------------------------------------------
/*! This slot method is triggered if the current test step has been finished.

    A test step is finished if the result values are set.

    @param i_pTestStep [in] Finished test step.
*/
void CTest::onCurrentTestStepFinished( CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = QString(i_pTestStep == nullptr ? "nullptr" : i_pTestStep->path());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onCurrentTestStepFinished",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "State: " + m_state.toString();
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->path());
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
        /* szSignal     */ SIGNAL(testStepFinished(ZS::Test::CTestStep*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCurrentTestStepFinished(ZS::Test::CTestStep*)) );

    if( m_state == ETestState::Running ) // not Paused or Stopped
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
void CTest::setState( const CEnumTestState& i_state )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "State: " + i_state.toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setState",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "State: " + m_state.toString();
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->path());
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_state != i_state )
    {
        CEnumTestState statePrev = m_state;
        m_state = i_state;
        emit stateChanged(m_state);
    }

} // setState

//------------------------------------------------------------------------------
void CTest::setRunMode( const CEnumRunMode& i_runMode )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "RunMode: " + i_runMode.toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setRunMode",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "State: " + m_state.toString();
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->path());
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_runMode != i_runMode )
    {
        CEnumRunMode runModePrev = m_runMode;
        m_runMode = i_runMode;
        emit runModeChanged(m_runMode);
    }

} // setRunMode

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::setCurrentTestStep( CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Step: " + QString(i_pTestStep == nullptr ? "nullptr" : i_pTestStep->path());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setCurrentTestStep",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "State: " + m_state.toString();
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->path());
        mthTracer.trace(strAddTrcInfo);
    }

    m_pTestStepCurr = i_pTestStep;

    emit currentTestStepChanged(m_pTestStepCurr);

} // setCurrentTestStep

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the next test step to be executed depending on the currently finished
    test step passed as argument.

    If nullptr is passed the the test has not been started yet and the first enabled
    test step found in the index tree is returned.

    @param i_pTestStepCurr [in] Test step which has been finished before.
                                nullptr if the test has not been started yet.
*/
CTestStep* CTest::getNextTestStep( CTestStep* i_pTestStepCurr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "TestStepCurr: " + QString(i_pTestStepCurr == nullptr ? "nullptr" : i_pTestStepCurr->path());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "getNextTestStep",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "State: " + m_state.toString();
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->path());
        mthTracer.trace(strAddTrcInfo);
    }

    CIdxTreeEntry* pTreeEntryParent   = nullptr;
    CIdxTreeEntry* pTreeEntryFinished = nullptr;

    if( i_pTestStepCurr == nullptr )
    {
        pTreeEntryParent = m_pIdxTree->root();
    }
    else // if( i_pTestStepCurr != nullptr )
    {
        pTreeEntryFinished = i_pTestStepCurr;
        pTreeEntryParent = pTreeEntryFinished->parentBranch();
    }

    return getNextTestStep(pTreeEntryParent, pTreeEntryFinished);

} // getNextTestStep

//------------------------------------------------------------------------------
/*! Returns the next test step to be executed depending on the currently finished
    test step passed as argument.
    passed which has currently beeing finished.
    passed.

    If nullptr is passed the the test has not been started yet and the first test
    step found in the index tree is returned.

    @param i_pTreeEntryParent [in] Parent group in which the next test step to be
                                   executed will be searched.
    @param i_pTreeEntryFinished [in] Currently finished test step. Might be nullptr
                                     if while searching a group entry is examined.
*/
CTestStep* CTest::getNextTestStep(
    CIdxTreeEntry* i_pTreeEntryParent,
    CIdxTreeEntry* i_pTreeEntryFinished )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "TreeEntryParent: " + QString(i_pTreeEntryParent == nullptr ? "nullptr" : i_pTreeEntryParent->path());
        strAddTrcInfo += ", TreeEntryFinished: " + QString(i_pTreeEntryFinished == nullptr ? "nullptr" : i_pTreeEntryFinished->name());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "getNextTestStep",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "State: " + m_state.toString();
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->path());
        mthTracer.trace(strAddTrcInfo);
    }

    CTestStep*      pTestStep      = nullptr;
    CTestStepGroup* pTestStepGroup = nullptr;

    // No parent: no next test step.
    if( i_pTreeEntryParent != nullptr )
    {
        int iTreeEntryFinishedRowIdx = -1;

        // Initially started: no finished test step.
        if( i_pTreeEntryFinished != nullptr )
        {
            iTreeEntryFinishedRowIdx = i_pTreeEntryFinished->indexInParentBranch();
        }

        // Not last entry in parent branch ...
        if( (iTreeEntryFinishedRowIdx + 1) < i_pTreeEntryParent->count() )
        {
            CIdxTreeEntry* pTreeEntryChild;
            int            idxChild;

            for( idxChild = (iTreeEntryFinishedRowIdx + 1); idxChild < i_pTreeEntryParent->count(); idxChild++ )
            {
                pTreeEntryChild = i_pTreeEntryParent->at(idxChild);

                // Leave entry ..
                if( pTreeEntryChild->entryType() == EIdxTreeEntryType::Leave )
                {
                    // .. next test step to be executed is this leave entry if the entry is enabled.
                    pTestStep = dynamic_cast<CTestStep*>(pTreeEntryChild);

                    if( !pTestStep->isEnabled() )
                    {
                        // If not enabled continue search in current group.
                        pTestStep = nullptr;
                    }
                }
                // Group entry ..
                else
                {
                    // .. next test step to be executed is this leave entry if the entry is enabled.
                    pTestStepGroup = dynamic_cast<CTestStepGroup*>(pTreeEntryChild);

                    if( pTestStepGroup->isEnabled() )
                    {
                        // .. get first step to be executed in child group.
                        pTestStep = getNextTestStep(pTreeEntryChild, nullptr);
                    }
                }
                if( pTestStep != nullptr )
                {
                    break;
                }
            } // for( idxChild = (iTreeEntryFinishedRowIdx+1); idxChild < pTreeEntry->getChildCount(); idxChild++ )
        } // if( (iTreeEntryFinishedRowIdx+1) < i_pTreeEntryParent->count() )

        // Last entry in parent branch ...
        else // if( (iTreeEntryFinishedRowIdx + 1) >= i_pTreeEntryParent->count() )
        {
            // One level up to parent branch and search next step to be executed.
            pTestStep = getNextTestStep(i_pTreeEntryParent->parentBranch(), i_pTreeEntryParent);
        }
    } // if( i_pTreeEntryParent != nullptr )

    return pTestStep;

} // getNextTestStep

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! Triggers execution of the next step by either starting a timer or immediately
    sending a continue request message to the tests event method.

    @param i_iInterval_ms [in]
*/
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
        strAddTrcInfo  = "State: " + m_state.toString();
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->path());
        mthTracer.trace(strAddTrcInfo);
    }

    // If the next step has already been triggered ..
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
        strAddTrcInfo  = "State: " + m_state.toString();
        strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->path());
        mthTracer.trace(strAddTrcInfo);
    }

    CTestStep* pTestStep = getNextTestStep(m_pTestStepCurr);

    setCurrentTestStep(pTestStep);

    if( m_pTestStepCurr != nullptr )
    {
        triggerDoTestStep(i_iInterval_ms);
    }
    else
    {
        if( m_state == ETestState::Running ) // not Paused or already Stopped
        {
            stop();
        }
    }
} // triggerNextTestStep

/*==============================================================================
protected: // overridables of inherited class QObject (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
/*! To avoid stack overlflows the test steps will be executed by sending
    a continue request message if the time interval between test steps is 0.

    @param i_pMsg [in] Pointer to event message to be handled.
*/
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
            strAddTrcInfo  = "State: " + m_state.toString();
            strAddTrcInfo += ", TestStepCurr: " + QString(m_pTestStepCurr == nullptr ? "nullptr" : m_pTestStepCurr->path());
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
