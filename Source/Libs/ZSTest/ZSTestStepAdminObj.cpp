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

#include "ZSTest/ZSTestStepAdminObj.h"
#include "ZSTest/ZSTest.h"
#include "ZSTest/ZSTestStepAdminObjPool.h"

//#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Test;


/*******************************************************************************
class CTestStepAdminObj : public QObject
*******************************************************************************/

/*==============================================================================
public: // ctor (obsolete)
==============================================================================*/

//------------------------------------------------------------------------------
CTestStepAdminObj::CTestStepAdminObj(
    CTestStepAdminObjPool* i_pObjPool,
    const QString&         i_strName,
    CObjPoolTreeEntry*     i_pTreeEntry ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pTest(nullptr),
    m_pObjPool(i_pObjPool),
    m_strName(i_strName),
    m_pTreeEntry(i_pTreeEntry),
    m_enabled(EEnabledYes),
    m_bTestRunning(false),
    m_testResult(ETestResultUndefined),
    m_fTimeTestStart_s(0.0),
    m_fTimeTestEnd_s(0.0),
    m_strToolTip()
{
} // ctor

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTestStepAdminObj::CTestStepAdminObj( CTest* i_pTest, const QString& i_strName ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pTest(i_pTest),
    m_pObjPool(nullptr),
    m_strName(i_strName),
    m_pTreeEntry(nullptr),
    m_enabled(EEnabledYes),
    m_bTestRunning(false),
    m_testResult(ETestResultUndefined),
    m_fTimeTestStart_s(0.0),
    m_fTimeTestEnd_s(0.0),
    m_strToolTip()
{
    m_pObjPool = i_pTest->getAdminObjIdxTree();

} // ctor

//------------------------------------------------------------------------------
CTestStepAdminObj::~CTestStepAdminObj()
//------------------------------------------------------------------------------
{
    m_pTest = nullptr;
    m_pObjPool = nullptr;
    //m_strName;
    m_pTreeEntry = nullptr;
    m_enabled = static_cast<EEnabled>(0);
    m_bTestRunning = false;
    m_testResult = static_cast<ETestResult>(0);
    m_fTimeTestStart_s = 0.0;
    m_fTimeTestEnd_s = 0.0;
    //m_strToolTip;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStepAdminObj::setName( const QString& i_strName )
//------------------------------------------------------------------------------
{
    if( m_strName != i_strName )
    {
        m_strName = i_strName;
        update();
    }

} // setName

//------------------------------------------------------------------------------
QString CTestStepAdminObj::getName( bool i_bIncludeGroupNameParents ) const
//------------------------------------------------------------------------------
{
    QString strName;

    if( i_bIncludeGroupNameParents )
    {
        strName = m_strName;

        if( m_pObjPool != nullptr && m_pTreeEntry != nullptr )
        {
            QString strParentNameSpace = m_pTreeEntry->getParentNameSpace(true);

            if( !strParentNameSpace.isEmpty() && !strParentNameSpace.endsWith(m_pObjPool->getNodeSeparator()) )
            {
                strParentNameSpace += m_pObjPool->getNodeSeparator();
            }
            strName = strParentNameSpace + m_strName;
        }
    }
    else
    {
        strName = m_strName;
    }
    return strName;

} // getName

//------------------------------------------------------------------------------
QString CTestStepAdminObj::getParentName( bool i_bIncludeGroupNameParents ) const
//------------------------------------------------------------------------------
{
    QString strParentName;

    if( m_pObjPool != nullptr && m_pTreeEntry != nullptr )
    {
        strParentName = m_pTreeEntry->getParentNameSpace(i_bIncludeGroupNameParents);
    }
    else
    {
        strParentName = m_strName;
    }
    return strParentName;

} // getParentName

//------------------------------------------------------------------------------
CTestStepGroup* CTestStepAdminObj::getParentGroup()
//------------------------------------------------------------------------------
{
    CTestStepGroup* pTSGrpParent = nullptr;

    if( m_pObjPool != nullptr && m_pTreeEntry != nullptr )
    {
        CObjPoolTreeEntry* pTreeEntryParent = m_pTreeEntry->getParentEntry();

        if( pTreeEntryParent != nullptr )
        {
            pTSGrpParent = reinterpret_cast<CTestStepGroup*>(pTreeEntryParent->getObj());
        }
    } // if( m_pObjPool != nullptr && m_pTreeEntry != nullptr )

    return pTSGrpParent;

} // getParentGroup

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStepAdminObj::setEnabled( EEnabled i_enabled )
//------------------------------------------------------------------------------
{
    if( m_enabled != i_enabled )
    {
        m_enabled = i_enabled;
        update();
    }

} // setEnabled

//------------------------------------------------------------------------------
bool CTestStepAdminObj::isEnabled() const
//------------------------------------------------------------------------------
{
    bool bEnabled = (m_enabled == EEnabledYes);

    if( m_enabled == EEnabledNo )
    {
        if( isGroup() || m_pObjPool->isInitializingTest() )
        {
            bEnabled = true;
        }
    }
    return bEnabled;

} // isEnabled

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStepAdminObj::reset()
//------------------------------------------------------------------------------
{
    m_bTestRunning = false;
    m_testResult = ETestResultUndefined;
    m_fTimeTestStart_s = 0.0;
    m_fTimeTestEnd_s = 0.0;

    update();

} // reset

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStepAdminObj::updateTestEndTime()
//------------------------------------------------------------------------------
{
    m_fTimeTestEnd_s = ZS::System::Time::getProcTimeInSec();
    update();

} // updateTestEndTime

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
double CTestStepAdminObj::getTestDurationInSec() const
//------------------------------------------------------------------------------
{
    return m_fTimeTestEnd_s - m_fTimeTestStart_s;
}

//------------------------------------------------------------------------------
double CTestStepAdminObj::getTestDurationInMilliSec() const
//------------------------------------------------------------------------------
{
    return (m_fTimeTestEnd_s - m_fTimeTestStart_s)*1.0e3;
}

//------------------------------------------------------------------------------
double CTestStepAdminObj::getTestDurationInMicroSec() const
//------------------------------------------------------------------------------
{
    return (m_fTimeTestEnd_s - m_fTimeTestStart_s)*1.0e6;
}

//------------------------------------------------------------------------------
double CTestStepAdminObj::getTestDurationInNanoSec() const
//------------------------------------------------------------------------------
{
    return (m_fTimeTestEnd_s - m_fTimeTestStart_s)*1.0e9;
}

//------------------------------------------------------------------------------
QString CTestStepAdminObj::testDuration2StrInBestUnit() const
//------------------------------------------------------------------------------
{
    QString strDuration;
    double  fDuration_s = getTestDurationInSec();

    if( fabs(fDuration_s) <= 1.0e-6 )
    {
        strDuration = QString::number(getTestDurationInNanoSec(),'f',3) + " ns";
    }
    else if( fabs(fDuration_s) <= 1.0e-3 )
    {
        strDuration = QString::number(getTestDurationInMicroSec(),'f',3) + " " + QString::fromLatin1("u") + "s";
    }
    else if( fabs(fDuration_s) <= 1.0 )
    {
        strDuration = QString::number(getTestDurationInMilliSec(),'f',3) + " ms";
    }
    else if( fabs(fDuration_s) <= 1.0e3 )
    {
        strDuration = QString::number(fDuration_s,'f',3) + " s";
    }
    else
    {
        strDuration = QString::number(fDuration_s,'f',3) + " s";
    }
    return strDuration;

} // testDuration2StrInBestUnit

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStepAdminObj::setToolTip( const QString& i_strToolTip )
//------------------------------------------------------------------------------
{
    if( m_strToolTip != i_strToolTip )
    {
        m_strToolTip= i_strToolTip;
        update();
    }

} // setToolTip

/*==============================================================================
public: // instance methods (experts use only)
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStepAdminObj::setTreeEntry( CObjPoolTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    m_pTreeEntry = i_pTreeEntry;
}
