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

#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTest.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSTest/ZSTestStepIdxTree.h"

#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTime.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Test;


/*******************************************************************************
class CTestStep : public CAbstractTestStepIdxTreeEntry
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! Constructs a test step.

    The entry will be added to the index tree of the passed test instance.

    When the "doTestStep" method of the test step is called by the test the
    test method of the test step must be called. For this the "doTestStep" method
    emits the "doTestStep" signal wherupon the passed slot function is called.
    The slot function must belong to the test instance.
    The signature of the slot function must be:

    @code
    void doTestStep<xyz>(ZS::Test::CTestStep*)
    @endcode

    The slot function may be passed as follows:

    @code
    SLOT(doTestStep<xyz>(ZS::Test::CTestStep*))
    @endcode

    @param i_pTest [in] Reference to test instance the entry belongs to (must not be nullptr).
    @param i_strName [in] Name of the entry.
    @param i_strOperation [in] Should describe the operation which will be tested (e.g. "x + y").
    @param i_pTSGrpParent [in] Parent test group or nullptr. If nullptr is passed the tests root
                               entry will be used as the parent.
    @param i_szDoTestStepSlotFct [in] Slot function which will be called if the test instance calls "doTestStep".

*/
CTestStep::CTestStep(
    CTest*          i_pTest,
    const QString&  i_strName,
    const QString&  i_strOperation,
    CTestStepGroup* i_pTSGrpParent,
    const char*     i_szDoTestStepSlotFct ) :
//------------------------------------------------------------------------------
    CAbstractTestStepIdxTreeEntry(i_pTest, EIdxTreeEntryType::Leave, i_strName, i_pTSGrpParent),
    m_strOperation(i_strOperation),
    m_strDescription(),
    m_strlstExpectedValues(),
    m_strlstResultValues(),
    m_fTimeTestStart_s(0.0),
    m_fTimeTestEnd_s(0.0),
    m_bBreakpoint(false),
    m_bBreakpointEnabled(true)
{
    if (!QObject::connect(
        /* pObjSender   */ this,
        /* szSignal     */ SIGNAL(doTestStep(ZS::Test::CTestStep*)),
        /* pObjReceiver */ i_pTest,
        /* szSlot       */ i_szDoTestStepSlotFct ) )
    {
        throw ZS::System::CException(__FILE__, __LINE__, EResultSignalSlotConnectionFailed);
    }

} // ctor

//------------------------------------------------------------------------------
CTestStep::~CTestStep()
//------------------------------------------------------------------------------
{
    //m_strOperation;
    //m_strDescription;
    //m_strlstExpectedValues.clear();
    //m_strlstResultValues.clear();
    m_fTimeTestStart_s = 0.0;
    m_fTimeTestEnd_s = 0.0;
    m_bBreakpoint = false;
    m_bBreakpointEnabled = false;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStep::setOperation( const QString& i_strOperation )
//------------------------------------------------------------------------------
{
    if( m_strOperation != i_strOperation )
    {
        m_strOperation = i_strOperation;

        if( m_pTree != nullptr )
        {
            m_pTree->onTreeEntryChanged(this);
        }
    }

} // setOperation

//------------------------------------------------------------------------------
void CTestStep::setDescription( const QString& i_strDescription )
//------------------------------------------------------------------------------
{
    if( m_strDescription != i_strDescription )
    {
        m_strDescription = i_strDescription;

        if( m_pTree != nullptr )
        {
            m_pTree->onTreeEntryChanged(this);
        }
    }

} // setDescription

//------------------------------------------------------------------------------
/*! Sets the expected result values of the test step.

    @param i_strlstExpectedValues [in] List with strings defining the expected result
                                     values of the test step. The list will be compared
                                     with the result values.
*/
void CTestStep::setExpectedValues( const QStringList& i_strlstExpectedValues )
//------------------------------------------------------------------------------
{
    m_strlstExpectedValues = i_strlstExpectedValues;

    if( m_pTree != nullptr )
    {
        m_pTree->onTreeEntryChanged(this);
    }
}

//------------------------------------------------------------------------------
/*! Sets the expected result value of the test step.

    @param i_strlstExpectedValue [in] String defining the expected result value
                                      of the test step. The string will be compared
                                      with the result value.
*/
void CTestStep::setExpectedValue( const QString& i_strExpectedValue )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;

    if( !i_strExpectedValue.isEmpty() )
    {
        strlstExpectedValues << i_strExpectedValue;
    }
    setExpectedValues(strlstExpectedValues);
}

//------------------------------------------------------------------------------
/*! Sets the actual result values of the test step which finishes test step.
    The signal "testStepFinished" is emitted.

    @param i_strlstResultValues [in] List with strings defining the actual result
                                     values of the test step. The list will be compared
                                     with the expected values. If equal the test result is
                                     Passed, otherwise Failed.
*/
void CTestStep::setResultValues( const QStringList& i_strlstResultValues )
//------------------------------------------------------------------------------
{
    m_strlstResultValues = i_strlstResultValues;

    // Not necessary here to inform the index tree that the content of the entry
    // has been changed. Thats been done by "onTestStepFinished".

    onTestStepFinished();

    emit testStepFinished(this);

} // setResultValues

//------------------------------------------------------------------------------
/*! Sets the actual result value of the test step which finishes test step.
    The signal "testStepFinished" is emitted.

    @param i_strResultValue [in] String defining the actual result value of the test step.
                                     The value will be compared with the expected value.
                                     If equal the test result is Passed, otherwise Failed.
*/
void CTestStep::setResultValue( const QString& i_strResultValue )
//------------------------------------------------------------------------------
{
    QStringList strlstResultValues;

    if( !i_strResultValue.isEmpty() )
    {
        strlstResultValues << i_strResultValue;
    }
    setResultValues(strlstResultValues);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns true if the test step is finished and the result values has been set.

    @return true if the test step has been finished, false otherwise.
*/
bool CTestStep::isFinished() const
//------------------------------------------------------------------------------
{
    return (m_testResult != ETestResult::Undefined);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStep::setBreakpoint()
//------------------------------------------------------------------------------
{
    if( !m_bBreakpoint )
    {
        m_bBreakpoint = true;

        if( m_pTree != nullptr )
        {
            m_pTree->onTreeEntryChanged(this);
        }
    }

} // setBreakpoint

//------------------------------------------------------------------------------
void CTestStep::removeBreakpoint()
//------------------------------------------------------------------------------
{
    if( m_bBreakpoint )
    {
        m_bBreakpoint = false;

        if( m_pTree != nullptr )
        {
            m_pTree->onTreeEntryChanged(this);
        }
    }

} // removeBreakpoint

//------------------------------------------------------------------------------
void CTestStep::enableBreakpoint()
//------------------------------------------------------------------------------
{
    if( !m_bBreakpointEnabled )
    {
        m_bBreakpointEnabled = true;

        if( m_pTree != nullptr )
        {
            m_pTree->onTreeEntryChanged(this);
        }
    }

} // enableBreakpoint

//------------------------------------------------------------------------------
void CTestStep::disableBreakpoint()
//------------------------------------------------------------------------------
{
    if( m_bBreakpointEnabled )
    {
        m_bBreakpointEnabled = false;

        if( m_pTree != nullptr )
        {
            m_pTree->onTreeEntryChanged(this);
        }
    }

} // disableBreakpoint

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStep::reset()
//------------------------------------------------------------------------------
{
    m_strlstExpectedValues.clear();
    m_strlstResultValues.clear();
    m_testResult = static_cast<ETestResult>(0);
    m_fTimeTestStart_s = 0.0;
    m_fTimeTestEnd_s = 0.0;

    if( m_pTree != nullptr )
    {
        m_pTree->onTreeEntryChanged(this);
    }

} // reset

/*==============================================================================
public: // must overridables of base class CAbstractTestStepIdxTreeEntry
==============================================================================*/

//------------------------------------------------------------------------------
double CTestStep::getTestDurationInSec() const
//------------------------------------------------------------------------------
{
    return m_fTimeTestEnd_s - m_fTimeTestStart_s;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! Executes the test step by emitting the doTestStep signal.

    The slot function passed to the ctor of the test step is connected to this signal.
*/
void CTestStep::doTestStep()
//------------------------------------------------------------------------------
{
    onTestStepStarted();

    emit doTestStep(this);
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStep::onTestStepStarted()
//------------------------------------------------------------------------------
{
    m_fTimeTestStart_s = ZS::System::Time::getProcTimeInSec();
    m_fTimeTestEnd_s = -1.0;

    if( m_pTree != nullptr )
    {
        m_pTree->onTreeEntryChanged(this);
    }

} // onTestStepStarted

//------------------------------------------------------------------------------
void CTestStep::onTestStepFinished()
//------------------------------------------------------------------------------
{
    m_fTimeTestEnd_s = ZS::System::Time::getProcTimeInSec();

    CEnumTestResult result = ETestResult::TestPassed;

    if( m_strlstExpectedValues.size() != m_strlstResultValues.size() )
    {
        result = ETestResult::TestFailed;
    }
    else
    {
        for( int idxVal = 0; idxVal < m_strlstExpectedValues.size(); idxVal++ )
        {
            if( m_strlstExpectedValues[idxVal] != m_strlstResultValues[idxVal] )
            {
                result = ETestResult::TestFailed;
                break;
            }
        }
    }

    // Not necessary here to inform the index tree that the content of the entry
    // has been changed. Thats been done by "setTestResult".

    setTestResult(result);

} // onTestStepFinished
