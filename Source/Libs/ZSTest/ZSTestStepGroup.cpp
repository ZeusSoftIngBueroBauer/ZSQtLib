/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
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

#include "ZSTest/ZSTestStepGroup.h"
#include "ZSTest/ZSTest.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepRoot.h"
#include "ZSTest/ZSTestStepIdxTree.h"

#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Test;


/*******************************************************************************
class CTestStepGroup : public CAbstractTestStepIdxTreeEntry
*******************************************************************************/

int CTestStepGroup::s_iInstCount = 0;

//------------------------------------------------------------------------------
/*! @brief Returns the number of test group instances.
*/
int CTestStepGroup::testGroupCount()
//------------------------------------------------------------------------------
{
    return s_iInstCount;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! Constructs a test step group.

    The entry will be added to the index tree of the passed test instance.

    @param i_pTest [in] Reference to test the entry belongs to (must not be nullptr).
    @param i_strName [in] Name of the entry.
    @param i_pTSGrpParent [in] Parent test group or nullptr, if the entry does not have a parent.
*/
CTestStepGroup::CTestStepGroup(
    CTest*          i_pTest,
    const QString&  i_strName,
    CTestStepGroup* i_pTSGrpParent ) :
//------------------------------------------------------------------------------
    CAbstractTestStepIdxTreeEntry(
        i_pTest, CIdxTreeEntry::EEntryType::Branch, i_strName, i_pTSGrpParent)
{
    ++s_iInstCount;
}

//------------------------------------------------------------------------------
CTestStepGroup::~CTestStepGroup()
//------------------------------------------------------------------------------
{
}

/*==============================================================================
protected: // ctor for class CTestGroupRoot
==============================================================================*/

//------------------------------------------------------------------------------
/*! Constructs a test step group.

    The entry will be added to the index tree of the passed test instance.

    @param i_pTest [in] Reference to test the entry belongs to (must not be nullptr).
    @param i_strName [in] Name of the entry.
    @param i_pTSGrpParent [in] Parent test group or nullptr, if the entry does not have a parent.
*/
CTestStepGroup::CTestStepGroup(
    CTest* i_pTest,
    CIdxTreeEntry::EEntryType i_entryType,
    const QString& i_strName ) :
//------------------------------------------------------------------------------
    CAbstractTestStepIdxTreeEntry(i_pTest, i_entryType, i_strName, nullptr)
{
} // ctor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! This method is called if the test result of a child entry has been changed.

    If this entry again has a parent group and the test result has been changed
    the method will be forwarded to the parent group.

    For a group the resulting test result is

    Undefined .. if ANY of the childrens test result is Undefined
    Failed ..... if NONE of the childrens test result is Undefined but at least
                 on childrens test result is Failed
    Passed ..... if NONE of the childrens test result is either Undefined or Failed.

    @param i_pTreeEntry [in] Entry whose test result has been changed.
                             On evaluating the resulting test result of the group
                             this child entry will be ignored as the resulting test
                             result of the child entry is already passed as second argument.
    @param i_testResult [in] Resulting test result of the passed tree entry.
                             Only Failed or Passed is allowed for i_testResult.
*/
void CTestStepGroup::onTestStepResultChanged(
    CAbstractTestStepIdxTreeEntry* i_pTreeEntry,
    const CEnumTestResult&         i_testResult )
//------------------------------------------------------------------------------
{
    // Don't invoke "i_pTreeEntry->getTestResult" here as that would recursively
    // go through all child of the tree entry to calculate the resulting test result
    // if "i_pTreeEntry" is a group.

    // If the saved test result is different from the test result of the child ..
    if( m_testResult != i_testResult )
    {
        CEnumTestResult testResult = ETestResult::TestPassed;

        // If the test result of the child is Failed ..
        if( i_testResult == ETestResult::TestFailed )
        {
            // .. the resulting test result of the group is also Failed.
            testResult = ETestResult::TestFailed;
        }
        // If the test result of the child is Passed  ..
        else if( i_testResult == ETestResult::TestPassed )
        {
            // .. the resulting test result is:

            // Undefined .. if ANY of the childrens test result is Undefined
            // Failed ..... if NONE of the childrens test result is Undefined but at least
            //              on childrens test result is Failed
            // Passed ..... if NONE of the childrens test result is either Undefined or Failed.

            // The test result of all children must be queried to get the
            // groups resulting test result.
            for( const auto& pIdxTreeEntry: m_arpTreeEntries )
            {
                if( pIdxTreeEntry != nullptr )
                {
                    CAbstractTestStepIdxTreeEntry* pTestStepEntry = dynamic_cast<CAbstractTestStepIdxTreeEntry*>(pIdxTreeEntry);

                    // The passed child entry will be ignored as for this child entry
                    // the resulting test result is already passed as argument.
                    if( pTestStepEntry != i_pTreeEntry )
                    {
                        CEnumTestResult testResultTmp = pTestStepEntry->getTestResult();

                        // Set to Failed also for Undefined and Ignore because those test
                        // result should not have been passed to this method.
                        if( testResultTmp == ETestResult::Undefined )
                        {
                            // The groups 
                            testResult = ETestResult::Undefined;
                            break;
                        }
                        else if( testResultTmp == ETestResult::TestFailed )
                        {
                            testResult = ETestResult::TestFailed;
                        }
                    }
                }
            }
        } // if( i_testResult == ETestResult::TestPassed )

        if( m_testResult != i_testResult )
        {
            m_testResult = i_testResult;

            emit_changed();

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
        } // if( m_testResult != i_testResult )
    } // if( m_testResult != testResult )

} // onTestStepResultChanged

/*==============================================================================
public: // must overridables of base class CAbstractTestStepIdxTreeEntry
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the test result of the group.

    For a group the resulting test result is

    Undefined .. if ANY of the childrens test result is Undefined
    Failed ..... if NONE of the childrens test result is Undefined but at least
                 on childrens test result is Failed
    Passed ..... if NONE of the childrens test result is either Undefined or Failed.

    @return Test result of the group.
*/
CEnumTestResult CTestStepGroup::getTestResult() const
//------------------------------------------------------------------------------
{
    CEnumTestResult testResult = ETestResult::Undefined;

    const CAbstractTestStepIdxTreeEntry* pTestStepEntry;

    for( const auto& pIdxTreeEntry: m_arpTreeEntries )
    {
        pTestStepEntry = dynamic_cast<CAbstractTestStepIdxTreeEntry*>(pIdxTreeEntry);

        CEnumTestResult testResultTmp = pTestStepEntry->getTestResult();

        if( testResultTmp == ETestResult::TestFailed )
        {
            testResult = ETestResult::TestFailed;
        }
        else if( testResultTmp == ETestResult::TestPassed )
        {
            if( testResult != ETestResult::TestFailed )
            {
                testResult = ETestResult::TestPassed;
            }
        }
        else // if( testResultTmp == ETestResult::Undefined || testResultTmp == ETestResult::Skipped )
        {
            testResult = ETestResult::Undefined;
            break;
        }
    }

    return testResult;

} // getTestResult

//------------------------------------------------------------------------------
/*! Returns the overall sum of the test duration in seconds for this group by
    recursively summing up the test duration of all children.

    @return Test duration of the group in seconds.
*/
double CTestStepGroup::getTestDurationInSec() const
//------------------------------------------------------------------------------
{
    double fDuration_s = 0.0;

    const CAbstractTestStepIdxTreeEntry* pTestStepEntry;

    for( const auto& pIdxTreeEntry: m_arpTreeEntries )
    {
        pTestStepEntry = dynamic_cast<CAbstractTestStepIdxTreeEntry*>(pIdxTreeEntry);

        fDuration_s += pTestStepEntry->getTestDurationInSec();
    }

    return fDuration_s;

} // getTestDurationInSec
