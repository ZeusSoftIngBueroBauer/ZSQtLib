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

#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTest.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSTest/ZSTestStepIdxTree.h"

#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Test;


/*******************************************************************************
class CTestStep : public CAbstractTestStepIdxTreeEntry, public ZS::System::#error CLeaveIdxTreeEntry
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

////------------------------------------------------------------------------------
///*! Constructs a test step.
//
//    The entry will be added to the index tree of the passed test instance.
//
//    @param i_pTest [in] Reference to test the entry belongs to (must not be nullptr).
//    @param i_strName [in] Name of the entry.
//    @param i_pTSGrpParent [in] Parent test group or nullptr, if the entry does not have a parent.
//*/
//CTestStep::CTestStep(
//    CTest*          i_pTest,
//    const QString&  i_strName,
//    const QString&  i_strOperation,
//    CTestStepGroup* i_pTSGrpParent,
//    TFctDoTestStep  i_fctDoTestStep ) :
////------------------------------------------------------------------------------
//    CAbstractTestStepIdxTreeEntry(i_pTest, EIdxTreeEntryType::Leave, i_strName, i_pTSGrpParent),
//    m_fctDoTestStep(i_fctDoTestStep),
//    m_strOperation(i_strOperation),
//    m_strDescription(),
//    m_strlstDesiredValues(),
//    m_strlstActualValues(),
//    m_testResult(ETestResultUndefined),
//    m_fTimeTestStart_s(0.0),
//    m_fTimeTestEnd_s(0.0),
//    m_bBreakpoint(false),
//    m_bBreakpointEnabled(true)
//{
//} // ctor

//------------------------------------------------------------------------------
/*! Constructs a test step.

    The entry will be added to the index tree of the passed test instance.

    @param i_pTest [in] Reference to test instance the entry belongs to (must not be nullptr).
    @param i_strName [in] Name of the entry.
    @param i_strOperation [in] Should describe the operation which will be tested (e.g. "x + y").
    @param i_pTSGrpParent [in] Parent test group or nullptr. If nullptr is passed the tests root
                               entry will be used as the parent.
    @param i_szDoTestStepSlotFct [in] Slot function which wil be called if the test instance calls "doTestStep".
                                      "doTestStep" again emits the "doTestStep" signal wherupon this passed
                                      slot function is called.
                                      The signature of the slot function must be:
                                      void doTestStep<xyz>(ZS::Test::CTestStep*) and may be passed as follows:
                                      SLOT(doTestStep<xyz>(ZS::Test::CTestStep*))

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
    m_strlstDesiredValues(),
    m_strlstActualValues(),
    m_testResult(ETestResultUndefined),
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
    //m_strlstDesiredValues.clear();
    //m_strlstActualValues.clear();
    m_testResult = static_cast<ETestResult>(0);
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
void CTestStep::setDesiredValues( const QStringList& i_strlstDesiredValues )
//------------------------------------------------------------------------------
{
    m_strlstDesiredValues = i_strlstDesiredValues;

    if( m_pTree != nullptr )
    {
        m_pTree->onTreeEntryChanged(this);
    }
}

//------------------------------------------------------------------------------
void CTestStep::setDesiredValue( const QString& i_strDesiredValue )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;

    if( !i_strDesiredValue.isEmpty() )
    {
        strlstDesiredValues << i_strDesiredValue;
    }
    setDesiredValues(strlstDesiredValues);
}

//------------------------------------------------------------------------------
void CTestStep::setActualValues( const QStringList& i_strlstActualValues )
//------------------------------------------------------------------------------
{
    m_strlstActualValues = i_strlstActualValues;

    if( m_strlstActualValues.size() > 0 )
    {
        //testEnded();
    }

    if( m_pTree != nullptr )
    {
        m_pTree->onTreeEntryChanged(this);
    }

    if( m_strlstActualValues.size() > 0 )
    {
        //emit finished(this);
    }

} // setActualValues

//------------------------------------------------------------------------------
void CTestStep::setActualValue( const QString& i_strActualValue )
//------------------------------------------------------------------------------
{
    QStringList strlstActualValues;

    if( !i_strActualValue.isEmpty() )
    {
        strlstActualValues << i_strActualValue;
    }
    setActualValues(strlstActualValues);
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
public: // must overridables of base class CAbstractTestStepIdxTreeEntry
==============================================================================*/

//------------------------------------------------------------------------------
double CTestStep::getTestDurationInSec() const
//------------------------------------------------------------------------------
{
    return m_fTimeTestEnd_s - m_fTimeTestStart_s;
}

//------------------------------------------------------------------------------
double CTestStep::getTestDurationInMilliSec() const
//------------------------------------------------------------------------------
{
    return (m_fTimeTestEnd_s - m_fTimeTestStart_s) * 1.0e3;
}

//------------------------------------------------------------------------------
double CTestStep::getTestDurationInMicroSec() const
//------------------------------------------------------------------------------
{
    return (m_fTimeTestEnd_s - m_fTimeTestStart_s) * 1.0e6;
}

//------------------------------------------------------------------------------
double CTestStep::getTestDurationInNanoSec() const
//------------------------------------------------------------------------------
{
    return (m_fTimeTestEnd_s - m_fTimeTestStart_s) * 1.0e9;
}

#if 0

/*==============================================================================
public: // overridables of base class CAbstractTestStepIdxTreeEntry
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStep::setTestResult( ETestResult i_testResult )
//------------------------------------------------------------------------------
{
    if( m_testResult != i_testResult )
    {
        m_testResult = i_testResult;

        if( m_testResult != ETestResultIgnore )
        {
            if( m_pListEntry != nullptr && m_pListEntry->getTreeEntry() != nullptr )
            {
                #error CAbstractIdxTreeEntry* pTreeEntryParent = m_pListEntry->getTreeEntry()->getParentEntry();

                if( pTreeEntryParent != nullptr )
                {
                    CAbstractTestStepIdxTreeEntry* pTSAdmObjParent = reinterpret_cast<CAbstractTestStepIdxTreeEntry*>(pTreeEntryParent->getObj());

                    if( pTSAdmObjParent != nullptr && pTSAdmObjParent->isGroup() )
                    {
                        CTestStepGroup* pTSGrpParent = dynamic_cast<CTestStepGroup*>(pTSAdmObjParent);

                        if( pTSGrpParent != nullptr )
                        {
                            pTSGrpParent->setTestResult(i_testResult);
                        }
                    }
                }
            }
        } // if( m_testResult != ETestResultIgnore )

        update();

    } // if( m_testResult != i_testResult )

} // setTestResult

/*==============================================================================
public: // must overridables of base class CAbstractTestStepIdxTreeEntry
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStep::testStarted()
//------------------------------------------------------------------------------
{
    m_bTestRunning = true;
    m_fTimeTestStart_s = ZS::System::Time::getProcTimeInSec();
    m_fTimeTestEnd_s = -1.0;
    m_pIdxTree->testStepStarted(this);
    // update(); update is used to update the model but the model is implicitly updated by the testStepStarted method

} // testStarted

//------------------------------------------------------------------------------
void CTestStep::testEnded( bool i_bIgnoreTestResult )
//------------------------------------------------------------------------------
{
    if( m_fTimeTestEnd_s < 0.0 )
    {
        m_fTimeTestEnd_s = ZS::System::Time::getProcTimeInSec();
    }

    m_bTestRunning = false;

    if( i_bIgnoreTestResult )
    {
        setTestResult(ETestResultIgnore);
    }
    else
    {
        ETestResult result = ETestResultTestPassed;

        if( m_strlstDesiredValues.size() != m_strlstActualValues.size() )
        {
            result = ETestResultTestFailed;
        }
        else
        {
            for( int idxVal = 0; idxVal < m_strlstDesiredValues.size(); idxVal++ )
            {
                if( m_strlstDesiredValues[idxVal] != m_strlstActualValues[idxVal] )
                {
                    result = ETestResultTestFailed;
                    break;
                }
            }
        }
        setTestResult(result);
    }

    //update(); // called by "setTestResult"

    m_pIdxTree->testStepEnded(this);

    // update(); update is used to update the model but the model is implicitly updated by the testStepEnded method

} // testEnded

//------------------------------------------------------------------------------
void CTestStep::reset()
//------------------------------------------------------------------------------
{
    CAbstractTestStepIdxTreeEntry::reset();

    m_strlstDesiredValues.clear();
    m_strlstActualValues.clear();

} // reset

/*==============================================================================
public: // must overridables of base class CAbstractTestStepIdxTreeEntry
==============================================================================*/

//------------------------------------------------------------------------------
bool CTestStep::isFinished() const
//------------------------------------------------------------------------------
{
    return (m_strlstActualValues.size() > 0);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStep::updateTestEndTime()
//------------------------------------------------------------------------------
{
    m_fTimeTestEnd_s = ZS::System::Time::getProcTimeInSec();
    update();

} // updateTestEndTime

#endif

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
    emit doTestStep(this);
}

/*==============================================================================
public: // must overridables of base class CAbstractTestStepIdxTreeEntry
==============================================================================*/

#if 0

//------------------------------------------------------------------------------
void CTestStep::update()
//------------------------------------------------------------------------------
{
    if( m_pTest != nullptr )
    {
        m_pTest->onTestStepChanged( dynamic_cast<CTestStep*>(this) );
    }

} // update

/*==============================================================================
public: // instance methods (experts use only)
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStep::setListEntry( CIdxTreeListEntry* i_pListEntry )
//------------------------------------------------------------------------------
{
    m_pListEntry = i_pListEntry;
}

#endif
