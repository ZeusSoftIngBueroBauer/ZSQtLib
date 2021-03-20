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
#include "ZSTest/ZSTestStepAdminObjPool.h"

//#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Test;


/*******************************************************************************
class CTestStep : public CTestStepAdminObj
*******************************************************************************/

/*==============================================================================
public: // ctor (obsolete)
==============================================================================*/

//------------------------------------------------------------------------------
CTestStep::CTestStep(
    CTestStepAdminObjPool* i_pObjPool,
    const QString&         i_strName,
    CObjPoolTreeEntry*     i_pTreeEntry,
    CObjPoolListEntry*     i_pListEntry ) :
//------------------------------------------------------------------------------
    CTestStepAdminObj(i_pObjPool, i_strName, i_pTreeEntry),
    m_fctDoTestStep(nullptr),
    m_iObjId(-1),
    m_pListEntry(i_pListEntry),
    m_strOperation(),
    m_strDescription(),
    m_strlstDesiredValues(),
    m_strlstActualValues(),
    m_bBreakpoint(false),
    m_bBreakpointDisabled(false)
{
} // ctor

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTestStep::CTestStep(
    CTest*          i_pTest,
    const QString&  i_strName,
    const QString&  i_strOperation,
    CTestStepGroup* i_pTSGrpParent,
    TFctDoTestStep  i_fctDoTestStep ) :
//------------------------------------------------------------------------------
    CTestStepAdminObj(i_pTest, i_strName),
    m_fctDoTestStep(i_fctDoTestStep),
    m_iObjId(-1),
    m_pListEntry(nullptr),
    m_strOperation(i_strOperation),
    m_strDescription(),
    m_strlstDesiredValues(),
    m_strlstActualValues(),
    m_bBreakpoint(false),
    m_bBreakpointDisabled(false)
{
    m_pObjPool->addTestStep(this, i_pTSGrpParent);

} // ctor

//------------------------------------------------------------------------------
CTestStep::CTestStep(
    CTest*          i_pTest,
    const QString&  i_strName,
    const QString&  i_strOperation,
    CTestStepGroup* i_pTSGrpParent,
    const char*     i_szDoTestStepSlotFct ) :
//------------------------------------------------------------------------------
    CTestStepAdminObj(i_pTest, i_strName),
    m_fctDoTestStep(nullptr),
    m_iObjId(-1),
    m_pListEntry(nullptr),
    m_strOperation(i_strOperation),
    m_strDescription(),
    m_strlstDesiredValues(),
    m_strlstActualValues(),
    m_bBreakpoint(false),
    m_bBreakpointDisabled(false)
{
    m_pObjPool->addTestStep(this, i_pTSGrpParent);

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
    m_pObjPool->invalidateObjNode(m_iObjId);

    m_fctDoTestStep = nullptr;
    m_iObjId = 0;
    m_pListEntry = nullptr;
    //m_strOperation;
    //m_strDescription;
    //m_strlstDesiredValues.clear();
    //m_strlstActualValues.clear();
    m_bBreakpoint = false;
    m_bBreakpointDisabled = false;

} // dtor

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStep::doTestStep()
//------------------------------------------------------------------------------
{
    if( m_fctDoTestStep != nullptr )
    {
        m_fctDoTestStep(m_pTest, this);
    }

    emit doTestStep(this);

} // doTestStep

/*==============================================================================
public: // overridables of base class CTestStepAdminObj
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
                CObjPoolTreeEntry* pTreeEntryParent = m_pListEntry->getTreeEntry()->getParentEntry();

                if( pTreeEntryParent != nullptr )
                {
                    CTestStepAdminObj* pTSAdmObjParent = reinterpret_cast<CTestStepAdminObj*>(pTreeEntryParent->getObj());

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
public: // must overridables of base class CTestStepAdminObj
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStep::testStarted()
//------------------------------------------------------------------------------
{
    m_bTestRunning = true;
    m_fTimeTestStart_s = ZS::System::Time::getProcTimeInSec();
    m_fTimeTestEnd_s = -1.0;
    m_pObjPool->testStepStarted(this);
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

    m_pObjPool->testStepEnded(this);

    // update(); update is used to update the model but the model is implicitly updated by the testStepEnded method

} // testEnded

//------------------------------------------------------------------------------
void CTestStep::reset()
//------------------------------------------------------------------------------
{
    CTestStepAdminObj::reset();

    m_strlstDesiredValues.clear();
    m_strlstActualValues.clear();

} // reset

/*==============================================================================
public: // must overridables of base class CTestStepAdminObj
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

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStep::setObjId( int i_iObjId )
//------------------------------------------------------------------------------
{
    if( m_iObjId != i_iObjId )
    {
        m_iObjId = i_iObjId;
        update();
    }
}

//------------------------------------------------------------------------------
int CTestStep::getObjId() const
//------------------------------------------------------------------------------
{
    return m_iObjId;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CTestStep::getOperation() const
//------------------------------------------------------------------------------
{
    return m_strOperation;
}

//------------------------------------------------------------------------------
void CTestStep::setOperation( const QString& i_strOperation )
//------------------------------------------------------------------------------
{
    if( m_strOperation != i_strOperation )
    {
        m_strOperation = i_strOperation;
        update();
    }

} // setOperation

//------------------------------------------------------------------------------
QString CTestStep::getDescription() const
//------------------------------------------------------------------------------
{
    return m_strDescription;
}

//------------------------------------------------------------------------------
void CTestStep::setDescription( const QString& i_strDescription )
//------------------------------------------------------------------------------
{
    if( m_strDescription != i_strDescription )
    {
        m_strDescription = i_strDescription;
        update();
    }

} // setDescription

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QStringList CTestStep::getDesiredValues() const
//------------------------------------------------------------------------------
{
    return m_strlstDesiredValues;
}

//------------------------------------------------------------------------------
void CTestStep::setDesiredValues( const QStringList& i_strlstDesiredValues )
//------------------------------------------------------------------------------
{
    m_strlstDesiredValues = i_strlstDesiredValues;
    update();
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

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QStringList CTestStep::getActualValues() const
//------------------------------------------------------------------------------
{
    return m_strlstActualValues;
}

//------------------------------------------------------------------------------
void CTestStep::setActualValues( const QStringList& i_strlstActualValues )
//------------------------------------------------------------------------------
{
    m_strlstActualValues = i_strlstActualValues;

    if( m_strlstActualValues.size() > 0 )
    {
        testEnded();
    }

    update();

    if( m_strlstActualValues.size() > 0 )
    {
        emit finished(this);
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
        update();
    }

} // setBreakpoint

//------------------------------------------------------------------------------
void CTestStep::removeBreakpoint()
//------------------------------------------------------------------------------
{
    if( m_bBreakpoint )
    {
        m_bBreakpoint = false;
        update();
    }

} // removeBreakpoint

//------------------------------------------------------------------------------
void CTestStep::enableBreakpoint()
//------------------------------------------------------------------------------
{
    if( m_bBreakpointDisabled )
    {
        m_bBreakpointDisabled = false;
        update();
    }

} // enableBreakpoint

//------------------------------------------------------------------------------
void CTestStep::disableBreakpoint()
//------------------------------------------------------------------------------
{
    if( !m_bBreakpointDisabled )
    {
        m_bBreakpointDisabled = true;
        update();
    }

} // disableBreakpoint

/*==============================================================================
public: // must overridables of base class CTestStepAdminObj
==============================================================================*/

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
void CTestStep::setListEntry( CObjPoolListEntry* i_pListEntry )
//------------------------------------------------------------------------------
{
    m_pListEntry = i_pListEntry;
}
