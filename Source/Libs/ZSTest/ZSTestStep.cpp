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
#include "ZSTest/ZSTestStepRoot.h"
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
/*! @brief Creates a test step.

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
    m_hshConfigValues(),
    m_strInstruction(),
    m_strlstExpectedValues(),
    m_strlstResultValues(),
    m_fTimeTestStart_s(0.0),
    m_fTimeTestEnd_s(0.0),
    m_bBreakpoint(false),
    m_breakpointEnabled(EEnabled::Yes)
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
/*! @brief Destroys the test step instance.
*/
CTestStep::~CTestStep()
//------------------------------------------------------------------------------
{
    //m_strOperation;
    //m_strDescription;
    //m_hshConfigValues;
    //m_strInstruction;
    //m_strlstExpectedValues.clear();
    //m_strlstResultValues.clear();
    m_fTimeTestStart_s = 0.0;
    m_fTimeTestEnd_s = 0.0;
    m_bBreakpoint = false;
    m_breakpointEnabled = static_cast<EEnabled>(0);

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the operation of the test step.

    The operation should describe the method which will be tested.

    The string applied may be used by a test step slot method to retrieve further
    information like class name, instance name, method to be called and so on.

    By evaluating the content of the operation string a slot method might be reused
    by several test steps.

    A possible operation string could be:

    @code
    NameSpace::ClassName::InstName.method(args)
    @endcode

    @param i_strOperation [in] Operation of the test step.
*/
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
}

//------------------------------------------------------------------------------
/*! @brief Sets the description of the test step.

    The description should describe what will be tested.

    E.g. "Returns after timeout".

    By evaluating the content of the description string a slot method might
    be reused by several test steps.

    @param i_strDescription [in] Description of the test step.
*/
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
}

//------------------------------------------------------------------------------
/*! @brief Return the config value for the given key.

    @param i_strKey [in] Key for which the config value should be returned.

    @return Config value of the given key. If no value was set for the key an
            invalid QVariant value is returned.
*/
QStringList CTestStep::getConfigValueKeys() const
//------------------------------------------------------------------------------
{
    return m_hshConfigValues.keys();
}

//------------------------------------------------------------------------------
/*! @brief Return the config value for the given key.

    @param i_strKey [in] Key for which the config value should be returned.

    @return Config value of the given key. If no value was set for the key an
            invalid QVariant value is returned.
*/
QVariant CTestStep::getConfigValue( const QString& i_strKey ) const
//------------------------------------------------------------------------------
{
    return m_hshConfigValues.value(i_strKey, QVariant());
}

//------------------------------------------------------------------------------
/*! @brief Sets a config value of the test step.

    Config values may be used to configure a slot method of the test step so
    that the slot method might be reused by several slot methods.

    @param i_strKey [in] Key for which the config value should be set.
    @param i_val [in] Config value.
*/
void CTestStep::setConfigValue( const QString& i_strKey, const QVariant& i_val )
//------------------------------------------------------------------------------
{
    m_hshConfigValues[i_strKey] = i_val;

    if( m_pTree != nullptr )
    {
        m_pTree->onTreeEntryChanged(this);
    }
}

//------------------------------------------------------------------------------
/*! @brief Sets the instruction of the test step.

    The instruction should describe what has to be done to execute the test step.

    E.g. "Start the trace client.\n"
         "Is the client connected?"

    @param i_strInstruction [in] Instruction of the test step.
*/
void CTestStep::setInstruction( const QString& i_strInstruction )
//------------------------------------------------------------------------------
{
    if( m_strInstruction != i_strInstruction )
    {
        m_strInstruction = i_strInstruction;

        if( m_pTree != nullptr )
        {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

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
/*! Sets a breakpoint for the test step and informs the tree that the content of the
    entry has been changed and got to be updated.
*/
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
}

//------------------------------------------------------------------------------
/*! Removes the breakpoint for the test step and informs the tree that the content of the
    entry has been changed and got to be updated.
*/
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
}

//------------------------------------------------------------------------------
/*! Enables or disables the breakpoint and informs the tree that the content of the
    entry has been changed and got to be updated.

    /param i_enabled [in] Flag to enable or disable the breakpoint.
*/
void CTestStep::setBreakpointEnabled( EEnabled i_enabled )
//------------------------------------------------------------------------------
{
    if( m_breakpointEnabled != i_enabled )
    {
        m_breakpointEnabled = i_enabled;

        if( m_pTree != nullptr )
        {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Resets the test step to default values by clearing the expected and
    result values and invalidating the start and end time of the test.
    The tree will be informed that the content of the entry has been changed
    and got to be updated.
*/
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
}

//------------------------------------------------------------------------------
/*! Sets the test result of the test step and informs the tree that the content
    of the entry has been changed and got to be updated.

    If the test result has been changed and the entry has a parent group the parent
    group will also be informed that the test result has been changed.

    @param i_testResult [in]
*/
void CTestStep::setTestResult( const CEnumTestResult& i_testResult )
//------------------------------------------------------------------------------
{
    m_fTimeTestEnd_s = ZS::System::Time::getProcTimeInSec();

    CEnumTestResult testResultPrev = m_testResult;

    m_testResult = i_testResult;

    // The end time has been changed even if the result is the same.
    if( m_pTree != nullptr )
    {
        m_pTree->onTreeEntryChanged(this);
    }

    // Inform parent groups only if the test result of the
    // test step has really been changed.
    if( testResultPrev != i_testResult )
    {
        CTestStepGroup* pParentGroup = getParentGroup();

        if( pParentGroup != nullptr )
        {
            pParentGroup->onTestStepResultChanged(this, m_testResult);
        }
        else
        {
            CTestStepRoot* pRootEntry = dynamic_cast<CTestStepRoot*>(m_pTest->getTestStepIdxTree()->root());

            if( pRootEntry != nullptr )
            {
                pRootEntry->onTestStepResultChanged(this, m_testResult);
            }
        }
    } // if( testResultPrev != i_testResult )

    emit testStepFinished(this);

} // setTestResult

/*==============================================================================
public: // must overridables of base class CAbstractTestStepIdxTreeEntry
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the duration of the test step in seconds.

    @return Duration in seconds.
*/
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
    m_fTimeTestStart_s = ZS::System::Time::getProcTimeInSec();
    m_fTimeTestEnd_s = -1.0;

    if( m_pTree != nullptr )
    {
        m_pTree->onTreeEntryChanged(this);
    }

    emit doTestStep(this);
}
