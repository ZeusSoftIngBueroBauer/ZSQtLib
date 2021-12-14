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

#include "ZSTest/ZSTestStepIdxTreeEntry.h"
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
class CAbstractTestStepIdxTreeEntry : public QObject
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! Constructs an index tree entry.

    The entry will be added to the index tree of the passed test instance.

    @param i_pTest [in] Reference to test the entry belongs to (must not be nullptr).
    @param i_entryType [in] Tree entry type.
    @param i_strName [in] Name of the entry.
    @param i_pTSGrpParent [in] Parent test group or nullptr, if the entry does not have a parent.
*/
CAbstractTestStepIdxTreeEntry::CAbstractTestStepIdxTreeEntry(
    CTest*            i_pTest,
    EIdxTreeEntryType i_entryType, 
    const QString&    i_strName,
    CTestStepGroup*   i_pTSGrpParent ) :
//------------------------------------------------------------------------------
    QObject(),
    CIdxTreeEntry(i_entryType, i_strName),
    m_pTest(i_pTest),
    m_enabled(EEnabled::Yes),
    m_bTestRunning(false),
    m_strToolTip()
{
    m_pTree = i_pTest->getTestStepIdxTree();

    m_pTree->add(this, i_pTSGrpParent);

} // ctor

//------------------------------------------------------------------------------
CAbstractTestStepIdxTreeEntry::~CAbstractTestStepIdxTreeEntry()
//------------------------------------------------------------------------------
{
    m_pTree->remove(this);

    m_pTest = nullptr;
    m_enabled = static_cast<EEnabled>(0);
    m_bTestRunning = false;
    //m_strToolTip;

} // dtor

/*==============================================================================
public: // must overridables of base class CAbstractTestStepIdxTreeEntry
==============================================================================*/

//------------------------------------------------------------------------------
bool CAbstractTestStepIdxTreeEntry::isGroup() const
//------------------------------------------------------------------------------
{
    return (m_entryType == EIdxTreeEntryType::Branch);
}

//------------------------------------------------------------------------------
CTestStepGroup* CAbstractTestStepIdxTreeEntry::getParentGroup()
//------------------------------------------------------------------------------
{
    return dynamic_cast<CTestStepGroup*>(parentBranch());
}

//------------------------------------------------------------------------------
/*! Enables or disables the entry.

    /param i_enabled [in] Flag to enable or disable the test step.
*/
void CAbstractTestStepIdxTreeEntry::setEnabled( EEnabled i_enabled )
//------------------------------------------------------------------------------
{
    if( m_enabled != i_enabled )
    {
        m_enabled = i_enabled;

        if( m_pTree != nullptr )
        {
            m_pTree->onTreeEntryChanged(this);
        }
    }

} // setEnabled

/*==============================================================================
public: // must overridables of base class CAbstractTestStepIdxTreeEntry
==============================================================================*/

//------------------------------------------------------------------------------
/*! Sets the tool tip of the entry.

    /param i_strToolTip [in]
*/
void CAbstractTestStepIdxTreeEntry::setToolTip( const QString& i_strToolTip )
//------------------------------------------------------------------------------
{
    if( m_strToolTip != i_strToolTip )
    {
        m_strToolTip = i_strToolTip;

        if( m_pTree != nullptr )
        {
            m_pTree->onTreeEntryChanged(this);
        }
    }

} // setToolTip

//------------------------------------------------------------------------------
QString CAbstractTestStepIdxTreeEntry::testDuration2StrInBestUnit() const
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

