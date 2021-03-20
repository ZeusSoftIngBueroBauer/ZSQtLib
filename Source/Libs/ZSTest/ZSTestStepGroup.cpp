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

#include "ZSTest/ZSTestStepGroup.h"
#include "ZSTest/ZSTest.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepAdminObjPool.h"

//#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Test;


/*******************************************************************************
class CTestStepGroup : public CTestStepAdminObj
*******************************************************************************/

/*==============================================================================
public: // ctor (obsolete)
==============================================================================*/

//------------------------------------------------------------------------------
CTestStepGroup::CTestStepGroup(
    CTestStepAdminObjPool* i_pObjPool,
    const QString&         i_strName,
    CObjPoolTreeEntry*     i_pTreeEntry ) :
//------------------------------------------------------------------------------
    CTestStepAdminObj(i_pObjPool, i_strName, i_pTreeEntry)
{
} // ctor

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTestStepGroup::CTestStepGroup(
    CTest*          i_pTest,
    const QString&  i_strName,
    CTestStepGroup* i_pTSGrpParent ) :
//------------------------------------------------------------------------------
    CTestStepAdminObj(i_pTest, i_strName)
{
    m_pObjPool->addTestStepGroup(this, i_pTSGrpParent);

} // ctor

//------------------------------------------------------------------------------
CTestStepGroup::~CTestStepGroup()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CTestStepAdminObj
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStepGroup::setTestResult( ETestResult i_testResult )
//------------------------------------------------------------------------------
{
    if( m_testResult != i_testResult )
    {
        if( m_testResult == ETestResultUndefined )
        {
            // Undefined (on reset) may be set to Passed and Failed.
            m_testResult = i_testResult;
        }
        else if( m_testResult == ETestResultTestFailed )
        {
            // Failed may be set to Undefined (on reset).
            if( i_testResult == ETestResultUndefined )
            {
                m_testResult = i_testResult;
            }
        }
        else if( m_testResult == ETestResultTestPassed )
        {
            // Passed may be set to Undefined (on reset) or Failed.
            m_testResult = i_testResult;
        }
        else if( m_testResult == ETestResultIgnore )
        {
            // Ignore may be set to Undefined (on reset).
            if( i_testResult == ETestResultUndefined )
            {
                m_testResult = i_testResult;
            }
        }

        if( m_testResult != ETestResultIgnore )
        {
            if( m_pTreeEntry != nullptr && m_pTreeEntry->getParentEntry() != nullptr )
            {
                CTestStepAdminObj* pTSAdmObjParent = reinterpret_cast<CTestStepAdminObj*>(m_pTreeEntry->getParentEntry()->getObj());

                if( pTSAdmObjParent != nullptr && pTSAdmObjParent->isGroup() )
                {
                    CTestStepGroup* pTSGrpParent = dynamic_cast<CTestStepGroup*>(pTSAdmObjParent);

                    if( pTSGrpParent != nullptr )
                    {
                        pTSGrpParent->setTestResult(i_testResult);
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
void CTestStepGroup::testStarted()
//------------------------------------------------------------------------------
{
    m_bTestRunning = true;
    m_fTimeTestStart_s = ZS::System::Time::getProcTimeInSec();
    m_fTimeTestEnd_s = -1.0;
    m_pObjPool->testGroupStarted(this);
    // update(); update is used to update the model but the model is implicitly updated by the testStepStarted method

} // testStarted

//------------------------------------------------------------------------------
void CTestStepGroup::testEnded( bool /*i_bIgnoreTestResult*/ )
//------------------------------------------------------------------------------
{
    if( m_fTimeTestEnd_s < 0.0 )
    {
        m_fTimeTestEnd_s = ZS::System::Time::getProcTimeInSec();
    }

    m_bTestRunning = false;

    m_pObjPool->testGroupEnded(this);

    // update(); update is used to update the model but the model is implicitly updated by the testStepEnded method

} // testEnded

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CTestStepGroup::getTestStepGroupCount()
//------------------------------------------------------------------------------
{
    int iTestStepGroupCount = 0;

    if( m_pTreeEntry != nullptr )
    {
        CTestStepAdminObj* pTSAdmObj;
        CObjPoolTreeEntry* pTreeEntryTmp;
        int                idxRow;

        for( idxRow = 0; idxRow < static_cast<int>(m_pTreeEntry->getChildCount()); idxRow++ )
        {
            pTreeEntryTmp = m_pTreeEntry->getChildEntry(idxRow);

            if( pTreeEntryTmp != nullptr )
            {
                pTSAdmObj = reinterpret_cast<CTestStepAdminObj*>(pTreeEntryTmp->getObj());

                if( pTSAdmObj != nullptr )
                {
                    if( pTSAdmObj->isGroup() )
                    {
                        iTestStepGroupCount++;
                    }
                } // if( pTSAdmObj != nullptr )
            } // if( pTreeEntryChild != nullptr )
        } // for( idxRow = 0; idxRow < static_cast<int>(m_pTreeEntry->getChildCount()); idxRow++ )
    } // if( m_pTreeEntry != nullptr )

    return iTestStepGroupCount;

} // getTestStepGroupCount

//------------------------------------------------------------------------------
CTestStepGroup* CTestStepGroup::getTestStepGroup( int i_iTestStepGroupIdx )
//------------------------------------------------------------------------------
{
    CTestStepGroup* pTSGrp = nullptr;

    if( m_pTreeEntry != nullptr )
    {
        CTestStepAdminObj* pTSAdmObj;
        CTestStepGroup*    pTSGrpTmp;
        CObjPoolTreeEntry* pTreeEntryTmp;
        int                iTestStepGroupCount = 0;
        int                idxRow;

        for( idxRow = 0; idxRow < static_cast<int>(m_pTreeEntry->getChildCount()); idxRow++ )
        {
            pTSGrpTmp = nullptr;

            pTreeEntryTmp = m_pTreeEntry->getChildEntry(idxRow);

            if( pTreeEntryTmp != nullptr )
            {
                pTSAdmObj = reinterpret_cast<CTestStepAdminObj*>(pTreeEntryTmp->getObj());

                if( pTSAdmObj != nullptr )
                {
                    if( pTSAdmObj->isGroup() )
                    {
                        pTSGrpTmp = dynamic_cast<CTestStepGroup*>(pTSAdmObj);

                        if( pTSGrpTmp != nullptr )
                        {
                            iTestStepGroupCount++;
                        }
                    }
                } // if( pTSAdmObj != nullptr )
            } // if( pTreeEntryTmp != nullptr )

            if( (iTestStepGroupCount-1) >= i_iTestStepGroupIdx )
            {
                pTSGrp = pTSGrpTmp;
                break;
            }
        } // for( idxRow = 0; idxRow < static_cast<int>(m_pTreeEntry->getChildCount()); idxRow++ )
    } // if( m_pTreeEntry != nullptr )

    return pTSGrp;

} // getTestStepGroup

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CTestStepGroup::getTestStepCount( bool i_bIncludeChildGroups )
//------------------------------------------------------------------------------
{
    int iTestStepCount = 0;

    if( m_pTreeEntry != nullptr )
    {
        CTestStepAdminObj* pTSAdmObj;
        CTestStepGroup*    pTSGrp;
        CTestStep*         pTestStep;
        CObjPoolTreeEntry* pTreeEntryTmp;
        int                idxRow;

        for( idxRow = 0; idxRow < static_cast<int>(m_pTreeEntry->getChildCount()); idxRow++ )
        {
            pTreeEntryTmp = m_pTreeEntry->getChildEntry(idxRow);

            if( pTreeEntryTmp != nullptr )
            {
                pTSAdmObj = reinterpret_cast<CTestStepAdminObj*>(pTreeEntryTmp->getObj());

                if( pTSAdmObj != nullptr )
                {
                    pTSGrp = nullptr;
                    pTestStep = nullptr;

                    if( pTSAdmObj->isGroup() )
                    {
                        pTSGrp = dynamic_cast<CTestStepGroup*>(pTSAdmObj);
                    }
                    else
                    {
                        pTestStep = dynamic_cast<CTestStep*>(pTSAdmObj);
                    }

                    if( i_bIncludeChildGroups && pTSGrp != nullptr )
                    {
                        iTestStepCount += pTSGrp->getTestStepCount(true);
                    }
                    else if( pTestStep != nullptr )
                    {
                        iTestStepCount++;
                    }
                } // if( pTSAdmObj != nullptr )
            } // if( pTreeEntryChild != nullptr )
        } // for( idxRow = 0; idxRow < static_cast<int>(m_pTreeEntry->getChildCount()); idxRow++ )
    } // if( m_pTreeEntry != nullptr )

    return iTestStepCount;

} // getTestStepCount

//------------------------------------------------------------------------------
CTestStep* CTestStepGroup::getTestStep( int i_iTestStepIdx/*, bool i_bIncludeChildGroups*/ )
//------------------------------------------------------------------------------
{
    CTestStep* pTestStep = nullptr;

    if( m_pTreeEntry != nullptr )
    {
        CTestStepAdminObj* pTSAdmObj;
        CTestStep*         pTestStepTmp;
        CObjPoolTreeEntry* pTreeEntryTmp;
        int                iTestStepCount = 0;
        int                idxRow;

        for( idxRow = 0; idxRow < static_cast<int>(m_pTreeEntry->getChildCount()); idxRow++ )
        {
            pTestStepTmp = nullptr;

            pTreeEntryTmp = m_pTreeEntry->getChildEntry(idxRow);

            if( pTreeEntryTmp != nullptr )
            {
                pTSAdmObj = reinterpret_cast<CTestStepAdminObj*>(pTreeEntryTmp->getObj());

                if( pTSAdmObj != nullptr )
                {
                    if( !pTSAdmObj->isGroup() )
                    {
                        pTestStepTmp = dynamic_cast<CTestStep*>(pTSAdmObj);

                        if( pTestStepTmp != nullptr )
                        {
                            iTestStepCount++;
                        }
                    }
                } // if( pTSAdmObj != nullptr )
            } // if( pTreeEntryTmp != nullptr )

            if( (iTestStepCount-1) >= i_iTestStepIdx )
            {
                pTestStep = pTestStepTmp;
                break;
            }
        } // for( idxRow = 0; idxRow < static_cast<int>(m_pTreeEntry->getChildCount()); idxRow++ )
    } // if( m_pTreeEntry != nullptr )

    return pTestStep;

} // getTestStep

////------------------------------------------------------------------------------
//CTestStep* CTestStepGroup::getTestStep(
//    int& io_iTestStepCount,
//    int  i_iTestStepIdx,
//    bool i_bIncludeChildGroups )
////------------------------------------------------------------------------------
//{
//    CTestStep* pTestStep = nullptr;
//
//    if( m_pTreeEntry != nullptr )
//    {
//        CTestStepAdminObj* pTSAdmObj;
//        CTestStepGroup*    pTSGrp;
//        CTestStep*         pTestStepTmp;
//        CObjPoolTreeEntry* pTreeEntryTmp;
//        int                idxRow;
//
//        if( i_bIncludeChildGroups )
//        {
//            for( idxRow = 0; idxRow < static_cast<int>(m_pTreeEntry->getChildCount()); idxRow++ )
//            {
//                pTreeEntryTmp = m_pTreeEntry->getChildEntry(idxRow);
//
//                if( pTreeEntryTmp != nullptr )
//                {
//                    pTSAdmObj = reinterpret_cast<CTestStepAdminObj*>(pTreeEntryTmp->getObj());
//
//                    if( pTSAdmObj != nullptr )
//                    {
//                        pTSGrp = nullptr;
//
//                        if( pTSAdmObj->isGroup() )
//                        {
//                            pTSGrp = dynamic_cast<CTestStepGroup*>(pTSAdmObj);
//                        }
//                        if( i_bIncludeChildGroups && pTSGrp != nullptr )
//                        {
//                            pTestStep = pTSGrp->getTestStep(io_iTestStepCount,i_iTestStepIdx,true);
//                        }
//                    } // if( pTSAdmObj != nullptr )
//                } // if( pTreeEntryTmp != nullptr )
//
//                if( pTestStep != nullptr )
//                {
//                    break;
//                }
//
//            } // for( idxRow = 0; idxRow < static_cast<int>(m_pTreeEntry->getChildCount()); idxRow++ )
//        } // if( i_bIncludeChildGroups )
//
//        if( pTestStep == nullptr )
//        {
//            for( idxRow = 0; idxRow < static_cast<int>(m_pTreeEntry->getChildCount()); idxRow++ )
//            {
//                pTreeEntryTmp = m_pTreeEntry->getChildEntry(idxRow);
//
//                if( pTreeEntryTmp != nullptr )
//                {
//                    pTSAdmObj = reinterpret_cast<CTestStepAdminObj*>(pTreeEntryTmp->getObj());
//
//                    if( pTSAdmObj != nullptr )
//                    {
//                        pTestStepTmp = nullptr;
//
//                        if( !pTSAdmObj->isGroup() )
//                        {
//                            pTestStepTmp = dynamic_cast<CTestStep*>(pTSAdmObj);
//                        }
//                        if( pTestStepTmp != nullptr )
//                        {
//                            io_iTestStepCount++;
//                        }
//                    } // if( pTSAdmObj != nullptr )
//                } // if( pTreeEntryTmp != nullptr )
//
//                if( (io_iTestStepCount-1) >= i_iTestStepIdx )
//                {
//                    pTestStep = pTestStepTmp;
//                    break;
//                }
//
//            } // for( idxRow = 0; idxRow < static_cast<int>(m_pTreeEntry->getChildCount()); idxRow++ )
//        } // if( pTestStep == nullptr )
//
//    } // if( m_pTreeEntry != nullptr )
//
//    return pTestStep;
//
//} // getTestStep

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CTestStepGroup::isParentOf( CTestStep* i_pTestStep ) const
//------------------------------------------------------------------------------
{
    bool bIs = false;

    if( i_pTestStep != nullptr )
    {
        CTestStepGroup* pTSGrpParent = i_pTestStep->getParentGroup();

        if( pTSGrpParent == this )
        {
            bIs = true;
        }
        else // if( pTSGrpParent == this )
        {
            pTSGrpParent = pTSGrpParent->getParentGroup();

            while( pTSGrpParent != nullptr )
            {
                if( pTSGrpParent == this )
                {
                    bIs = true;
                    break;
                }
                pTSGrpParent = pTSGrpParent->getParentGroup();
            }
        } // if( pTSGrpParent == this )
    } // if( i_pTestStep != nullptr )

    return bIs;

} // isParentOf

//------------------------------------------------------------------------------
bool CTestStepGroup::isFirstTestStep( CTestStep* i_pTestStep ) const
//------------------------------------------------------------------------------
{
    bool bIs = false;

    CObjPoolTreeEntry* pTreeEntry;
    int                idxChild;

    for( idxChild = 0; idxChild < m_pTreeEntry->getChildCount(); idxChild++ )
    {
        pTreeEntry = m_pTreeEntry->getChildEntry(idxChild);

        if( pTreeEntry->getEntryType() == EObjPoolEntryTypeObject )
        {
            if( pTreeEntry->getObj() == i_pTestStep )
            {
                bIs = true;
            }
            break;
        }
    } // for( idxChild = 0; idxChild < m_pTreeEntry->getChildCount(); idxChild++ )

    return bIs;

} // isFirstTestStep

//------------------------------------------------------------------------------
bool CTestStepGroup::isLastTestStep( CTestStep* i_pTestStep ) const
//------------------------------------------------------------------------------
{
    bool bIs = false;

    if( m_pTreeEntry->getChildCount() > 0 )
    {
        CObjPoolTreeEntry* pTreeEntry;
        int                idxChild;

        for( idxChild = m_pTreeEntry->getChildCount()-1; idxChild >= 0; idxChild-- )
        {
            pTreeEntry = m_pTreeEntry->getChildEntry(idxChild);

            if( pTreeEntry->getEntryType() == EObjPoolEntryTypeObject )
            {
                if( pTreeEntry->getObj() == i_pTestStep )
                {
                    bIs = true;
                }
                break;
            }
        } // for( idxChild = m_pTreeEntry->getChildCount()-1; idxChild >= 0; idxChild-- )
    } // if( m_pTreeEntry->getChildCount() > 0 )

    return bIs;

} // isLastTestStep

//------------------------------------------------------------------------------
bool CTestStepGroup::isFirstTestGroup( CTestStepGroup* i_pTSGrp ) const
//------------------------------------------------------------------------------
{
    bool bIs = false;

    CObjPoolTreeEntry* pTreeEntry;
    int                idxChild;

    for( idxChild = 0; idxChild < m_pTreeEntry->getChildCount(); idxChild++ )
    {
        pTreeEntry = m_pTreeEntry->getChildEntry(idxChild);

        if( pTreeEntry->getEntryType() != EObjPoolEntryTypeObject )
        {
            if( pTreeEntry->getObj() == i_pTSGrp )
            {
                bIs = true;
            }
            break;
        }
    } // for( idxChild = 0; idxChild < m_pTreeEntry->getChildCount(); idxChild++ )

    return bIs;

} // isFirstTestGroup

//------------------------------------------------------------------------------
bool CTestStepGroup::isLastTestGroup( CTestStepGroup* i_pTSGrp ) const
//------------------------------------------------------------------------------
{
    bool bIs = false;

    if( m_pTreeEntry->getChildCount() > 0 )
    {
        CObjPoolTreeEntry* pTreeEntry;
        int                idxChild;

        for( idxChild = m_pTreeEntry->getChildCount()-1; idxChild >= 0; idxChild-- )
        {
            pTreeEntry = m_pTreeEntry->getChildEntry(idxChild);

            if( pTreeEntry->getEntryType() != EObjPoolEntryTypeObject )
            {
                if( pTreeEntry->getObj() == i_pTSGrp )
                {
                    bIs = true;
                }
                break;
            }
        } // for( idxChild = m_pTreeEntry->getChildCount()-1; idxChild >= 0; idxChild-- )
    } // if( m_pTreeEntry->getChildCount() > 0 )

    return bIs;

} // isLastTestGroup

/*==============================================================================
public: // must overridables of base class CTestStepAdminObj
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStepGroup::update()
//------------------------------------------------------------------------------
{
    if( m_pTest != nullptr )
    {
        m_pTest->onTestStepGroupChanged( dynamic_cast<CTestStepGroup*>(this) );
    }

} // update

