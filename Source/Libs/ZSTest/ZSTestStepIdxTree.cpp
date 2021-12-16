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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qmutex.h>
#include <QtCore/qdir.h>
#include <QtCore/qfile.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qsettings.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include "ZSTest/ZSTestStepIdxTree.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSTest/ZSTestStepRoot.h"
#include "ZSTest/ZSTest.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Test;


/*******************************************************************************
class CTestStepIdxTree : public ZS::System::CIdxTree
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTestStepIdxTree::CTestStepIdxTree( CTest* i_pTest ) :
//------------------------------------------------------------------------------
    CIdxTree(
        /* strIdxTreeName   */ i_pTest->objectName(),
        /* pRootTreeEntry   */ new CTestStepRoot(i_pTest, i_pTest->objectName()),
        /* strNodeSeparator */ "\\",
        /* bCreateMutex     */ true,
        /* pObjParent       */ nullptr),
    m_pTest(i_pTest),
    //m_bInitializingTest(false),
    //m_iTestStepsExecutedCount(0),
    //m_iTestStepsPassedCount(0),
    //m_iTestStepsFailedCount(0),
    //m_iTestStepsIgnoredCount(0),
    //m_iUpdateGroupCallDepth(0),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_pTest->objectName());

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Test", "CTestStepIdxTree", objectName());

    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strMthArgs   */ strAddTrcInfo);

} // ctor

//------------------------------------------------------------------------------
CTestStepIdxTree::~CTestStepIdxTree()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strMthArgs   */ strAddTrcInfo);

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pTest = nullptr;
    //m_bInitializingTest = false;
    //m_iTestStepsExecutedCount = 0;
    //m_iTestStepsPassedCount = 0;
    //m_iTestStepsFailedCount = 0;
    //m_iTestStepsIgnoredCount = 0;
    m_pTrcAdminObj = nullptr;

} // dtor

#if 0

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CTestStepIdxTree::getTestStepsCount() const
//------------------------------------------------------------------------------
{
    return m_pTSGrpRoot->getTestStepCount(true);
}

#endif

/*==============================================================================
public: // instance methods find test step groups
==============================================================================*/

//------------------------------------------------------------------------------
CTestStepGroup* CTestStepIdxTree::findTestStepGroup(
    const QString& i_strParentPath,
    const QString& i_strGroupName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CTestStepGroup*>(findBranch(i_strParentPath, i_strGroupName));
}

//------------------------------------------------------------------------------
CTestStepGroup* CTestStepIdxTree::findTestStepGroup(
    CTestStepGroup* i_pTestGroupParent,
    const QString&  i_strGroupName )
//------------------------------------------------------------------------------
{
    QString strParentPath;

    if( i_pTestGroupParent != nullptr )
    {
        strParentPath = i_pTestGroupParent->path();
    }
    return findTestStepGroup(strParentPath, i_strGroupName);

} // findTestStepGroup

/*==============================================================================
public: // instance methods to add, remove and modify test step groups
==============================================================================*/

#if 0

//------------------------------------------------------------------------------
CTestStepGroup* CTestStepIdxTree::addTestStepGroup(
    const QString&  i_strGroupName,
    const QString&  i_strParentPath,
    EEnabled        i_enabled )
//------------------------------------------------------------------------------
{
    CTestStepGroup*    pTSGrp = nullptr;
    #error CAbstractIdxTreeEntry* pTreeEntry;
    QString            strPath = i_strGroupName;

    if( !i_strParentPath.isEmpty() )
    {
        strPath = buildPathStr(i_strParentPath, i_strGroupName);
    }

    pTreeEntry = getTreeEntry(
        /* strPath   */ strPath,
        /* entryType */ EIdxTreeEntryTypeNameSpace );

    if( pTreeEntry != nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, "A group \"" + i_strGroupName + "\" is already existing below \"" + i_strParentPath + "\".");
    }

    // Please note that on adding a node to the model the "nodeInserted"
    // signal is emitted but the test step object has not yet been set
    // at the models list entry. So the client of the test step object
    // pool model need to connect to the "testStepGroupInserted" signal.
    pTreeEntry = addNameSpaceNode(i_strParentPath, i_strGroupName);

    if( pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError, "Couldn't add namespace node \"" + i_strGroupName + "\" below \"" + i_strParentPath + "\".");
    }

    pTSGrp = new CTestStepGroup(this, i_strGroupName, pTreeEntry);

    if( i_enabled != EEnabled::Undefined )
    {
        pTSGrp->setEnabled(i_enabled);
    }

    // Please note that on adding a node to the model the "nodeInserted"
    // signal is emitted but the test step object has not yet been set
    // at the models list entry. So the client of the test step object
    // pool model need to connect to the "testStepInserted" signal.
    pTreeEntry->setObj(pTSGrp);

    //pTestStep->update(); // not necessary as the "testStepGroupInserted" signal is emitted:
    emit testStepGroupInserted(this, pTSGrp);

    return pTSGrp;

} // addTestStepGroup

//------------------------------------------------------------------------------
CTestStepGroup* CTestStepIdxTree::addTestStepGroup(
    const QString&  i_strGroupName,
    CTestStepGroup* i_pTestGroupParent,
    EEnabled        i_enabled )
//------------------------------------------------------------------------------
{
    QString strParentPath;

    if( i_pTestGroupParent != nullptr )
    {
        strParentPath = i_pTestGroupParent->getName(true);
    }

    return addTestStepGroup(i_strGroupName, strParentPath, i_enabled);

} // addTestStepGroup

//------------------------------------------------------------------------------
void CTestStepIdxTree::addTestStepGroup(
    CTestStepGroup* i_pTSGrp,
    CTestStepGroup* i_pTSGrpParent )
//------------------------------------------------------------------------------
{
    #error CAbstractIdxTreeEntry* pTreeEntry;

    QString strName = i_pTSGrp->getName();
    QString strParentPath;
    QString strPath = strName;

    if( i_pTSGrpParent != nullptr )
    {
        strParentPath = i_pTSGrpParent->getName(true);
        strPath = buildPathStr(strParentPath, strName);
    }

    pTreeEntry = getTreeEntry(strPath, EIdxTreeEntryTypeNameSpace);

    if( pTreeEntry != nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, strPath);
    }

    // Please note that on adding a node to the model the "nodeInserted"
    // signal is emitted but the test step object has not yet been set
    // at the models list entry. So the client of the test step object
    // pool model need to connect to the "testStepGroupInserted" signal.
    pTreeEntry = addNameSpaceNode(strParentPath, strName);

    if( pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError, "Could not add \"" + strPath + "\" to test steps pool");
    }

    // Please note that on adding a node to the model the "nodeInserted"
    // signal is emitted but the test step object has not yet been set
    // at the models list entry. So the client of the test step object
    // pool model need to connect to the "testStepInserted" signal.
    pTreeEntry->setObj(i_pTSGrp);

    i_pTSGrp->setTreeEntry(pTreeEntry);

    //pTestStep->update(); // not necessary as the "testStepGroupInserted" signal is emitted:
    emit testStepGroupInserted(this, i_pTSGrp);

} // addTestStepGroup

//------------------------------------------------------------------------------
CTestStepGroup* CTestStepIdxTree::getTestStepGroup(
    const QString&  i_strGroupName,
    const QString&  i_strParentPath,
    EEnabled        i_enabled )
//------------------------------------------------------------------------------
{
    CTestStepGroup*    pTSGrp = nullptr;
    CAbstractTestStepIdxTreeEntry* pTSAdmObj;
    #error CAbstractIdxTreeEntry* pTreeEntry;
    QString            strPath = i_strGroupName;

    if( !i_strParentPath.isEmpty() )
    {
        strPath = buildPathStr(i_strParentPath, i_strGroupName);
    }

    pTreeEntry = getTreeEntry(
        /* strPath   */ strPath,
        /* entryType */ EIdxTreeEntryTypeNameSpace );

    if( pTreeEntry == nullptr )
    {
        // Please note that on adding a node to the model the "nodeInserted"
        // signal is emitted but the test step object has not yet been set
        // at the models list entry. So the client of the test step object
        // pool model need to connect to the "testStepGroupInserted" signal.
        pTreeEntry = addNameSpaceNode(i_strParentPath,i_strGroupName);
    }

    if( pTreeEntry != nullptr )
    {
        pTSAdmObj = reinterpret_cast<CAbstractTestStepIdxTreeEntry*>(pTreeEntry->getObj());

        if( pTSAdmObj != nullptr && !pTSAdmObj->isGroup() )
        {
            throw CException(__FILE__, __LINE__, EResultInvalidObjType);
        }
        else
        {
            pTSGrp = dynamic_cast<CTestStepGroup*>(pTSAdmObj);
        }

        // If the test step group object is not yet part of my object pool ..
        if( pTSGrp == nullptr )
        {
            pTSGrp = new CTestStepGroup(this, i_strGroupName, pTreeEntry);

            if( i_enabled != EEnabled::Undefined )
            {
                pTSGrp->setEnabled(i_enabled);
            }

            // Please note that on adding a node to the model the "nodeInserted"
            // signal is emitted but the test step object has not yet been set
            // at the models list entry. So the client of the test step object
            // pool model need to connect to the "testStepInserted" signal.
            pTreeEntry->setObj(pTSGrp);

            //pTestStep->update(); // not necessary as the "testStepGroupInserted" signal is emitted:
            emit testStepGroupInserted(this, pTSGrp);
        }

        else // if( pTSGrp != nullptr )
        {
            bool bNodeChanged = false;

            if( i_enabled != EEnabled::Undefined && i_enabled != pTSGrp->getEnabled() )
            {
                pTSGrp->setEnabled(i_enabled);
                bNodeChanged = true;
            }
            if( bNodeChanged )
            {
                // Trigger the "nodeChanged" and "dataChanged" signals of the model:
                onTestStepGroupChanged(pTSGrp);
            }
        } // if( pTSGrp != nullptr )
    } // if( pTreeEntry != nullptr )

    return pTSGrp;

} // getTestStepGroup

//------------------------------------------------------------------------------
CTestStepGroup* CTestStepIdxTree::getTestStepGroup(
    const QString&  i_strGroupName,
    CTestStepGroup* i_pTestGroupParent,
    EEnabled        i_enabled )
//------------------------------------------------------------------------------
{
    QString strParentPath;

    if( i_pTestGroupParent != nullptr )
    {
        strParentPath = i_pTestGroupParent->getName(true);
    }

    return getTestStepGroup(
        /* strName            */ i_strGroupName,
        /* strParentPath */ strParentPath,
        /* enabled            */ i_enabled );

} // getTestStepGroup

//------------------------------------------------------------------------------
void CTestStepIdxTree::onTestStepGroupChanged( CTestStepGroup* i_pTSGrp )
//------------------------------------------------------------------------------
{
    if( i_pTSGrp == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pTSGrp == nullptr");
    }

    #error CAbstractIdxTreeEntry* pTreeEntry = i_pTSGrp->getTreeEntry();

    if( pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pTSGrp->m_pTreeEntry == nullptr");
    }

    // "dataChanged" signal of the AbstractItemModel to update the view:
    //QModelIndex modelIdxTopLeft = createIndex( pTreeEntry->getModelIdx().row(), EColumnMin, pTreeEntry );
    //QModelIndex modelIdxBottomRight = createIndex( pTreeEntry->getModelIdx().row(), EColumnMax, pTreeEntry );
    //emit QAbstractItemModel::dataChanged(modelIdxTopLeft,modelIdxBottomRight);

    emit nodeChanged(this,pTreeEntry);

    emit testStepGroupChanged(this,i_pTSGrp);

} // onTestStepGroupChanged

/*==============================================================================
public: // instance methods to add, remove and modify test steps
==============================================================================*/

//------------------------------------------------------------------------------
CTestStep* CTestStepIdxTree::getTestStep( int i_iObjId )
//------------------------------------------------------------------------------
{
    CTestStep*         pTestStep = nullptr;
    CAbstractTestStepIdxTreeEntry* pTSAdmObj;
    CIdxTreeListEntry* pListEntry;

    if( i_iObjId >= 0 && i_iObjId < static_cast<int>(getObjCount()) )
    {
        pListEntry = getListEntry(i_iObjId);

        if( pListEntry != nullptr )
        {
            pTSAdmObj = reinterpret_cast<CAbstractTestStepIdxTreeEntry*>(pListEntry->getObj());

            if( pTSAdmObj != nullptr && pTSAdmObj->isGroup() )
            {
                throw CException(__FILE__, __LINE__, EResultInvalidObjType);
            }
            else
            {
                pTestStep = dynamic_cast<CTestStep*>(pTSAdmObj);
            }
        }
    }
    return pTestStep;

} // getTestStep

#endif

//------------------------------------------------------------------------------
CTestStep* CTestStepIdxTree::findTestStep(
    const QString& i_strName,
    const QString& i_strParentPath )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CTestStep*>(findLeave(i_strParentPath, i_strName));
}

//------------------------------------------------------------------------------
CTestStep* CTestStepIdxTree::findTestStep(
    CTestStepGroup* i_pTestGroupParent,
    const QString&  i_strName )
//------------------------------------------------------------------------------
{
    QString strParentPath;

    if( i_pTestGroupParent != nullptr )
    {
        strParentPath = i_pTestGroupParent->path();
    }
    return findTestStep(i_strName, strParentPath);

} // findTestStep

#if 0

//------------------------------------------------------------------------------
CTestStep* CTestStepIdxTree::addTestStep(
    const QString& i_strName,
    const QString& i_strParentPath,
    EEnabled       i_enabled )
//------------------------------------------------------------------------------
{
    CTestStep*         pTestStep = nullptr;
    #error CAbstractIdxTreeEntry* pTreeEntry = nullptr;
    CIdxTreeListEntry* pListEntry = nullptr;
    QString            strPath = i_strName;

    if( !i_strParentPath.isEmpty() )
    {
        strPath = buildPathStr(i_strParentPath, i_strName);
    }

    pTreeEntry = getTreeEntry(
        /* strPath   */ strPath,
        /* entryType */ EIdxTreeEntryTypeObject );

    if( pTreeEntry != nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, "A test step \"" + i_strName + "\" is already existing below \"" + i_strParentPath + "\".");
    }

    int iObjId = addObjNode(strPath);

    pListEntry = getListEntry(iObjId);

    if( pListEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError, "No list entry created for \"" + i_strName + "\" below \"" + i_strParentPath + "\".");
    }

    pTestStep = new CTestStep(
        /* pIdxTree   */ this,
        /* strName    */ i_strName,
        /* pTreeEntry */ pTreeEntry,
        /* pListEntry */ pListEntry );

    pTestStep->setObjId(iObjId);

    if( i_enabled != EEnabled::Undefined )
    {
        pTestStep->setEnabled(i_enabled);
    }

    // Please note that on adding a node to the model the "nodeInserted"
    // signal is emitted but the test step object has not yet been set
    // at the models list entry. So the client of the test step object
    // pool model need to connect to the "testStepInserted" signal.
    pListEntry->setObj(pTestStep);
    pTreeEntry->setObj(pTestStep);

    //pTestStep->update(); // not necessary as the "testStepInserted" signal is emitted:
    emit testStepInserted(this, pTestStep);

    return pTestStep;

} // addTestStep

//------------------------------------------------------------------------------
CTestStep* CTestStepIdxTree::addTestStep(
    const QString&  i_strName,
    CTestStepGroup* i_pTestGroupParent,
    EEnabled        i_enabled )
//------------------------------------------------------------------------------
{
    QString strParentPath;

    if( i_pTestGroupParent != nullptr )
    {
        strParentPath = i_pTestGroupParent->getName(true);
    }

    return addTestStep(
        /* strName            */ i_strName,
        /* strParentPath */ strParentPath,
        /* enabled            */ i_enabled );

} // addTestStep

//------------------------------------------------------------------------------
void CTestStepIdxTree::addTestStep(
    CTestStep*      i_pTestStep,
    CTestStepGroup* i_pTSGrpParent )
//------------------------------------------------------------------------------
{
    #error CAbstractIdxTreeEntry* pTreeEntry = nullptr;
    CIdxTreeListEntry* pListEntry = nullptr;

    QString strName = i_pTestStep->getName();
    QString strParentPath;
    QString strPath = strName;

    if( i_pTSGrpParent != nullptr )
    {
        strParentPath = i_pTSGrpParent->getName(true);
        strPath = buildPathStr(strParentPath, strName);
    }

    pTreeEntry = getTreeEntry(strPath, EIdxTreeEntryTypeObject);

    if( pTreeEntry != nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, strPath);
    }

    // Please note that on adding a node to the model the "nodeInserted"
    // signal is emitted but the test step object has not yet been set
    // at the models list entry. So the client of the test step object
    // pool model need to connect to the "testStepInserted" signal.
    int iObjId = addObjNode(strPath);

    pListEntry = getListEntry(iObjId);

    if( pListEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError, "Could not add \"" + strPath + "\" to test steps pool");
    }

    pTreeEntry = pListEntry->getTreeEntry();

    i_pTestStep->setObjId(iObjId);

    // Please note that on adding a node to the model the "nodeInserted"
    // signal is emitted but the test step object has not yet been set
    // at the models list entry. So the client of the test step object
    // pool model need to connect to the "testStepInserted" signal.
    pListEntry->setObj(i_pTestStep);
    pTreeEntry->setObj(i_pTestStep);

    i_pTestStep->setListEntry(pListEntry);
    i_pTestStep->setTreeEntry(pTreeEntry);

    //pTestStep->update(); // not necessary as the "testStepInserted" signal is emitted:
    emit testStepInserted(this, i_pTestStep);

} // addTestStep

//------------------------------------------------------------------------------
CTestStep* CTestStepIdxTree::getTestStep(
    const QString& i_strName,
    const QString& i_strParentPath,
    EEnabled       i_enabled,
    int            i_iObjId )
//------------------------------------------------------------------------------
{
    CTestStep*         pTestStep = nullptr;
    CAbstractTestStepIdxTreeEntry* pTSAdmObj;
    #error CAbstractIdxTreeEntry* pTreeEntry = nullptr;
    CIdxTreeListEntry* pListEntry = nullptr;
    int                iObjId = i_iObjId;
    QString            strPath = i_strName;

    if( !i_strParentPath.isEmpty() )
    {
        strPath = buildPathStr(i_strParentPath, i_strName);
    }

    if( iObjId >= 0 && iObjId < static_cast<int>(getObjCount()) )
    {
        pListEntry = getListEntry(iObjId);

        if( pListEntry != nullptr )
        {
            pTreeEntry = pListEntry->getTreeEntry();
        }
    }

    if( pTreeEntry == nullptr )
    {
        pTreeEntry = getTreeEntry(
            /* strPath   */ strPath,
            /* entryType */ EIdxTreeEntryTypeObject );
    }

    if( pTreeEntry == nullptr )
    {
        // Please note that on adding a node to the model the "nodeInserted"
        // signal is emitted but the test step object has not yet been set
        // at the models list entry. So the client of the test step object
        // pool model need to connect to the "testStepInserted" signal.
        if( iObjId >= 0 )
        {
            pTreeEntry = addObjNode(strPath,iObjId);
        }
        else
        {
            iObjId = addObjNode(strPath);
        }

        pListEntry = getListEntry(iObjId);

        if( pListEntry != nullptr )
        {
            pTreeEntry = pListEntry->getTreeEntry();
        }
    }
    else if( pListEntry == nullptr )
    {
        pListEntry = getListEntry(pTreeEntry->getObjId());
    }

    if( pListEntry != nullptr && pTreeEntry != nullptr )
    {
        iObjId = pListEntry->getObjId();

        pTSAdmObj = reinterpret_cast<CAbstractTestStepIdxTreeEntry*>(pListEntry->getObj());

        if( pTSAdmObj != nullptr && pTSAdmObj->isGroup() )
        {
            throw CException(__FILE__,__LINE__,EResultInvalidObjType);
        }
        else
        {
            pTestStep = dynamic_cast<CTestStep*>(pTSAdmObj);
        }

        // If the test step object is not yet part of my object pool ..
        if( pTestStep == nullptr )
        {
            pTestStep = new CTestStep(this,i_strName,pTreeEntry,pListEntry);
            pTestStep->setObjId(iObjId);

            if( i_enabled != EEnabled::Undefined )
            {
                pTestStep->setEnabled(i_enabled);
            }

            // Please note that on adding a node to the model the "nodeInserted"
            // signal is emitted but the test step object has not yet been set
            // at the models list entry. So the client of the test step object
            // pool model need to connect to the "testStepInserted" signal.
            pListEntry->setObj(pTestStep);
            pTreeEntry->setObj(pTestStep);

            //pTestStep->update(); // not necessary as the "testStepInserted" signal is emitted:
            emit testStepInserted(this,pTestStep);
        }

        else // if( pTestStep != nullptr )
        {
            bool bNodeChanged = false;

            if( i_enabled != EEnabled::Undefined && i_enabled != pTestStep->getEnabled() )
            {
                pTestStep->setEnabled(i_enabled);
                bNodeChanged = true;
            }
            if( bNodeChanged )
            {
                // Trigger the "nodeChanged" and "dataChanged" signals of the model:
                onTestStepChanged(pTestStep);
            }
        } // if( pTestStep != nullptr )
    } // if( pListEntry != nullptr && pTreeEntry != nullptr )

    return pTestStep;

} // getTestStep

//------------------------------------------------------------------------------
CTestStep* CTestStepIdxTree::getTestStep(
    const QString&  i_strName,
    CTestStepGroup* i_pTestGroupParent,
    EEnabled        i_enabled,
    int             i_iObjId )
//------------------------------------------------------------------------------
{
    QString strParentPath;

    if( i_pTestGroupParent != nullptr )
    {
        strParentPath = i_pTestGroupParent->getName(true);
    }

    return getTestStep(
        /* strName            */ i_strName,
        /* strParentPath */ strParentPath,
        /* enabled            */ i_enabled,
        /* iObjId             */ i_iObjId );

} // getTestStep

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStepIdxTree::onTestStepChanged( CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    if( i_pTestStep == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pTestStep == nullptr");
    }

    CIdxTreeListEntry* pListEntry = i_pTestStep->getListEntry();

    if( pListEntry == nullptr )
    {
        pListEntry = getListEntry(i_pTestStep->getObjId());
    }

    if( pListEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pTestStep->m_pListEntry == nullptr");
    }

    #error CAbstractIdxTreeEntry* pTreeEntry = pListEntry->getTreeEntry();

    if( pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pTestStep->m_pTreeEntry->m_pTreeEntry == nullptr");
    }

    emit nodeChanged(this, pTreeEntry);
    emit objectNodeChanged(this, pListEntry);

    emit testStepChanged(this, i_pTestStep);

} // onTestStepChanged

//------------------------------------------------------------------------------
void CTestStepIdxTree::testStepStarted( CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    if( i_pTestStep == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pTestStep == nullptr");
    }

    onTestStepChanged(i_pTestStep);

    CTestStepGroup* pTSGrpParent = i_pTestStep->getParentGroup();

    if( pTSGrpParent != nullptr )
    {
        if( pTSGrpParent->isFirstTestStep(i_pTestStep) )
        {
            pTSGrpParent->testStarted();
        }
    } // if( pTSGrpParent != nullptr )

} // testStepStarted

//------------------------------------------------------------------------------
void CTestStepIdxTree::testStepEnded( CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    if( i_pTestStep == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pTestStep == nullptr");
    }

    m_iTestStepsExecutedCount++;

    if( i_pTestStep->getTestResult() == ETestResultTestPassed )
    {
        m_iTestStepsPassedCount++;
    }
    else if( i_pTestStep->getTestResult() == ETestResultTestFailed )
    {
        m_iTestStepsFailedCount++;
    }
    else if( i_pTestStep->getTestResult() == ETestResultIgnore )
    {
        m_iTestStepsIgnoredCount++;
    }

    onTestStepChanged(i_pTestStep);

    CTestStepGroup* pTSGrpParent = i_pTestStep->getParentGroup();

    if( pTSGrpParent != nullptr )
    {
        if( pTSGrpParent->isLastTestStep(i_pTestStep) )
        {
            pTSGrpParent->testEnded();
        }
    } // if( pTSGrpParent != nullptr )

} // testStepEnded

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStepIdxTree::onTestGroupChanged( CTestStepGroup* i_pTSGrp )
//------------------------------------------------------------------------------
{
    if( i_pTSGrp == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pTSGrp == nullptr");
    }

    #error CAbstractIdxTreeEntry* pTreeEntry = i_pTSGrp->getTreeEntry();

    if( pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pTSGrp->m_pTreeEntry->m_pTreeEntry == nullptr");
    }

    emit nodeChanged(this, pTreeEntry);

    emit testStepGroupChanged(this, i_pTSGrp);

} // onTestGroupChanged

//------------------------------------------------------------------------------
void CTestStepIdxTree::testGroupStarted( CTestStepGroup* i_pTSGrp )
//------------------------------------------------------------------------------
{
    if( i_pTSGrp == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pTSGrp == nullptr");
    }

    onTestGroupChanged(i_pTSGrp);

    CTestStepGroup* pTSGrpParent = i_pTSGrp->getParentGroup();

    if( pTSGrpParent != nullptr )
    {
        if( pTSGrpParent->isFirstTestGroup(i_pTSGrp) )
        {
            pTSGrpParent->testStarted();
        }
    } // if( pTSGrpParent != nullptr )

} // testGroupStarted

//------------------------------------------------------------------------------
void CTestStepIdxTree::testGroupEnded( CTestStepGroup* i_pTSGrp )
//------------------------------------------------------------------------------
{
    if( i_pTSGrp == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pTSGrp == nullptr");
    }

    onTestGroupChanged(i_pTSGrp);

    CTestStepGroup* pTSGrpParent = i_pTSGrp->getParentGroup();

    if( pTSGrpParent != nullptr )
    {
        if( pTSGrpParent->isLastTestGroup(i_pTSGrp) )
        {
            pTSGrpParent->testEnded();
        }
    } // if( pTSGrpParent != nullptr )

} // testGroupEnded

/*==============================================================================
public: // instance methods to recursively modify test step objects via namespace node entries
==============================================================================*/

//------------------------------------------------------------------------------
int CTestStepIdxTree::getUpdateGroupCallDepth() const
//------------------------------------------------------------------------------
{
    return m_iUpdateGroupCallDepth;
}

//------------------------------------------------------------------------------
void CTestStepIdxTree::updateGroup(
    const QString& i_strName,
    const QString& i_strGroupNameParent,
    EEnabled       i_enabled )
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    #error CAbstractIdxTreeEntry* pTreeEntry = getTreeEntry(
        /* strParentNameSpace */ i_strGroupNameParent,
        /* strNodeName        */ i_strName,
        /* entryType          */ EIdxTreeEntryTypeNameSpace );

    if( pTreeEntry != nullptr )
    {
        updateGroup( pTreeEntry, i_enabled );
    }

} // updateGroup

//------------------------------------------------------------------------------
void CTestStepIdxTree::updateGroup( #error CAbstractIdxTreeEntry* i_pTreeEntry, EEnabled i_enabled )
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    if( i_pTreeEntry != nullptr )
    {
        m_iUpdateGroupCallDepth++;

        #error CAbstractIdxTreeEntry* pTreeEntryChild;
        CTestStep*         pTestStep;
        int                iRow;

        for( iRow = 0; iRow < static_cast<int>(i_pTreeEntry->getChildCount()); iRow++ )
        {
            pTreeEntryChild = i_pTreeEntry->getChildEntry(iRow);

            if( pTreeEntryChild->getEntryType() == EIdxTreeEntryTypeObject )
            {
                pTestStep = reinterpret_cast<CTestStep*>(pTreeEntryChild->getObj());

                if( pTestStep != nullptr )
                {
                    if( i_enabled != EEnabled::Undefined && pTestStep->getEnabled() != i_enabled )
                    {
                        pTestStep->setEnabled(i_enabled);
                    }
                    onTestStepChanged(pTestStep);
                }
            }
            else
            {
                updateGroup(pTreeEntryChild, i_enabled);
            }
        }

        m_iUpdateGroupCallDepth--;

        if( m_iUpdateGroupCallDepth == 0 )
        {
            emit nameSpaceNodeChanged(this, i_pTreeEntry, static_cast<int>(i_enabled));
        }
    }

} // updateGroup

/*==============================================================================
public: // overridables of base class CModelIdxTree
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStepIdxTree::clear()
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    int iRow;

    // Clearing is the only way to delete list or tree entries.
    // During "normal operation" neither name space nor object entries
    // may be removed. If a referenced object is destroyed only the
    // reference to that object will be invalidated within the list
    // and tree entry (set to nullptr in the list entry and in the tree
    // entry the object state will be marked as destroyed).
    emit clearing(this);

    // The root entry will not be deleted.
    if( m_pTreeObjs->getChildCount() > 0 )
    {
        #error CAbstractIdxTreeEntry* pTreeEntryChild;
        CAbstractTestStepIdxTreeEntry* pTSAdmObj;
        CTestStepGroup*    pTSGrp;
        CTestStep*         pTestStep;

        for( iRow = m_pTreeObjs->getChildCount()-1; iRow >= 0; iRow-- )
        {
            pTreeEntryChild = m_pTreeObjs->getChildEntry(iRow);
            pTSAdmObj = reinterpret_cast<CAbstractTestStepIdxTreeEntry*>(pTreeEntryChild->getObj());

            if( pTreeEntryChild->getEntryType() != EIdxTreeEntryTypeObject )
            {
                clearTreeEntry(pTreeEntryChild);
            }

            // After the childs of the child have been removed the child may be removed.
            m_pTreeObjs->removeChildEntry(iRow);

            pTSGrp = nullptr;
            pTestStep = nullptr;

            if( pTSAdmObj != nullptr )
            {
                if( pTSAdmObj->isGroup() )
                {
                    pTSGrp = dynamic_cast<CTestStepGroup*>(pTSAdmObj);
                }
                else
                {
                    pTestStep = dynamic_cast<CTestStep*>(pTSAdmObj);
                }
            }

            if( pTSGrp != nullptr )
            {
                delete pTSGrp;
                pTSGrp = nullptr;
            }
            else if( pTestStep != nullptr )
            {
                invalidateObjNode(pTestStep->getObjId());

                delete pTestStep;
                pTestStep = nullptr;
            }

            if( m_pTreeEntryCurr == pTreeEntryChild )
            {
                m_pTreeEntryCurr = nullptr;
            }

            delete pTreeEntryChild;
            pTreeEntryChild = nullptr;
        }
    }
    m_pTreeEntryCurr = m_pTreeObjs;

    if( m_arpListObjs.size() > 0 )
    {
        for( iRow = m_arpListObjs.size()-1; iRow >= 0; iRow-- )
        {
            delete m_arpListObjs[iRow];
            m_arpListObjs[iRow] = nullptr;
        }
    }
    m_arpListObjs.clear();

    m_pTreeObjs->setNodeName(m_strIdxTreeName);

} // clear

#endif

/*==============================================================================
public: // overridables of base class CModelIdxTree
==============================================================================*/

//------------------------------------------------------------------------------
/*! Recursively resets the test results of all test step groups and test steps.
*/
void CTestStepIdxTree::reset()
//------------------------------------------------------------------------------
{
    reset(m_pRoot);
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Helper method to recursively reset the test results of all test step groups
    and test steps below the specified node.

    @param i_pTreeEntry Tree entry to be reset together with all its children.
*/
void CTestStepIdxTree::reset( CIdxTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    // But this is a protected method called by the public reset method
    // and the mutex has already been locked.

    CIdxTreeEntry* pTreeEntry;
    int            idxEntry;

    for( idxEntry = 0; idxEntry < i_pTreeEntry->count(); ++idxEntry )
    {
        pTreeEntry = i_pTreeEntry->at(idxEntry);

        if( pTreeEntry != nullptr )
        {
            if( pTreeEntry->isLeave() )
            {
                CTestStep* pTestStep = dynamic_cast<CTestStep*>(pTreeEntry);
                pTestStep->reset();
            }
            else // if( !pTreeEntry->isLeave() )
            {
                reset(pTreeEntry);
            }
        } // if(pTreeEntry != nullptr)
    } // for( idxEntry = 0; idxEntry < i_pTreeEntry->count(); ++idxEntry )

} // reset

/*==============================================================================
public: // instance methods
==============================================================================*/

#if 0

//------------------------------------------------------------------------------
void CTestStepIdxTree::beginInitTest()
//------------------------------------------------------------------------------
{
    m_bInitializingTest = true;
}

//------------------------------------------------------------------------------
void CTestStepIdxTree::endInitTest()
//------------------------------------------------------------------------------
{
    m_bInitializingTest = false;
}

//------------------------------------------------------------------------------
void CTestStepIdxTree::testStarted()
//------------------------------------------------------------------------------
{
    m_iTestStepsExecutedCount = 0;
    m_iTestStepsPassedCount = 0;
    m_iTestStepsFailedCount = 0;
    m_iTestStepsIgnoredCount = 0;

    m_pTSGrpRoot->testStarted();

} // testStarted

//------------------------------------------------------------------------------
void CTestStepIdxTree::testEnded()
//------------------------------------------------------------------------------
{
    m_pTSGrpRoot->testEnded();
}

//------------------------------------------------------------------------------
void CTestStepIdxTree::testAborted()
//------------------------------------------------------------------------------
{
    m_pTSGrpRoot->testEnded();
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CTestStepIdxTree::deserialize( QXmlStreamReader* i_pxmlStreamReader )
//------------------------------------------------------------------------------
{
    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".deserialize";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QString strAddErrInfo;

    QXmlStreamReader::TokenType xmlStreamTokenType;
    QXmlStreamAttribute         xmlStreamAttr;

    QString strElemName;

    SErrResultInfo  errResultInfoTmp = ErrResultInfoSuccess(strMth);
    CTestStepGroup* pTSGrp = nullptr;
    CTestStep*      pTestStep = nullptr;
    QString         strName;
    QString         strVal;
    EEnabled        enabled = EEnabled::No;
    bool            bVal = false;
    bool            bConverted = false;

    while( !i_pxmlStreamReader->atEnd() )
    {
        xmlStreamTokenType = i_pxmlStreamReader->readNext();

        if( xmlStreamTokenType == QXmlStreamReader::StartElement || xmlStreamTokenType == QXmlStreamReader::EndElement )
        {
            strElemName = i_pxmlStreamReader->name().toString();

            //---------------------------------------
            if( strElemName == "TestStepGroup" )
            //---------------------------------------
            {
                if( xmlStreamTokenType == QXmlStreamReader::StartElement )
                {
                    strName = i_pxmlStreamReader->attributes().value("Name").toString();

                    if( strName.isEmpty() )
                    {
                        strAddErrInfo = "Attribute \"Name\" for \"" + strElemName + "\" not defined";
                        i_pxmlStreamReader->raiseError(strAddErrInfo);
                    }
                    else // if( !strName.isEmpty() )
                    {
                        CTestStepGroup* pTSGrpTmp = findTestStepGroup(strName, pTSGrp);

                        if( pTSGrpTmp == nullptr )
                        {
                            strAddErrInfo = strElemName + " \"" + strName + "\" is not a child of \"" + QString(pTSGrp == nullptr ? "Root" : pTSGrp->getName(true)) + "\".";
                            i_pxmlStreamReader->raiseError(strAddErrInfo);
                        }
                        else // if( pTSGrpTmp != nullptr )
                        {
                            pTSGrp = pTSGrpTmp;

                            if( i_pxmlStreamReader->attributes().hasAttribute("Enabled") )
                            {
                                strVal = i_pxmlStreamReader->attributes().value("Enabled").toString();
                                enabled = CEnumEnabled::toEnumerator(strVal);
                                if( enabled == EEnabled::Undefined )
                                {
                                    strAddErrInfo = "Attribute \"Enabled\" for " + strElemName + " \"" + pTSGrp->getName(true) + "\" is out of range.";
                                    errResultInfoTmp.setSeverity(EResultSeverityError);
                                    errResultInfoTmp.setResult(EResultValueOutOfRange);
                                    errResultInfoTmp.setAddErrInfoDscr(strAddErrInfo);
                                    if( CErrLog::GetInstance() != nullptr )
                                    {
                                        CErrLog::GetInstance()->addEntry(errResultInfoTmp);
                                    }
                                }
                                else
                                {
                                    pTSGrp->setEnabled(enabled);
                                }
                            }
                        } // if( pTSGrpTmp != nullptr )
                    } // if( !strName.isEmpty() )
                } // if( xmlStreamTokenType == QXmlStreamReader::StartElement )

                else if (xmlStreamTokenType == QXmlStreamReader::EndElement)
                {
                    if( pTSGrp != nullptr )
                    {
                        pTSGrp = pTSGrp->getParentGroup();
                    }
                } // if (xmlStreamTokenType == QXmlStreamReader::EndElement)
            } // if( strElemName == "TestStepGroup" )

            //---------------------------------------
            else if( strElemName == "TestStep" )
            //---------------------------------------
            {
                if( xmlStreamTokenType == QXmlStreamReader::StartElement )
                {
                    strName = i_pxmlStreamReader->attributes().value("Name").toString();

                    if( strName.isEmpty() )
                    {
                        strAddErrInfo = "Attribute \"Name\" for \"" + strElemName + "\" not defined";
                        i_pxmlStreamReader->raiseError(strAddErrInfo);
                    }
                    else // if( !strName.isEmpty() )
                    {
                        pTestStep = findTestStep(strName, pTSGrp);

                        if( pTestStep == nullptr )
                        {
                            strAddErrInfo = strElemName + " \"" + strName + "\" is not a child of \"" + QString(pTSGrp == nullptr ? "Root" : pTSGrp->getName(true)) + "\".";
                            i_pxmlStreamReader->raiseError(strAddErrInfo);
                        }
                        else // if( pTestStep != nullptr )
                        {
                            if( i_pxmlStreamReader->attributes().hasAttribute("Enabled") )
                            {
                                strVal = i_pxmlStreamReader->attributes().value("Enabled").toString();
                                enabled = CEnumEnabled::toEnumerator(strVal);
                                if( enabled == EEnabled::Undefined )
                                {
                                    strAddErrInfo = "Attribute \"Enabled\" for " + strElemName + " \"" + pTestStep->getName(true) + "\" is out of range.";
                                    errResultInfoTmp.setSeverity(EResultSeverityError);
                                    errResultInfoTmp.setResult(EResultValueOutOfRange);
                                    errResultInfoTmp.setAddErrInfoDscr(strAddErrInfo);
                                    if( CErrLog::GetInstance() != nullptr )
                                    {
                                        CErrLog::GetInstance()->addEntry(errResultInfoTmp);
                                    }
                                }
                                else
                                {
                                    pTestStep->setEnabled(enabled);
                                }
                            }
                            if( i_pxmlStreamReader->attributes().hasAttribute("Breakpoint") )
                            {
                                strVal = i_pxmlStreamReader->attributes().value("Breakpoint").toString();
                                bVal = str2Bool(strVal, &bConverted);
                                if( !bConverted )
                                {
                                    strAddErrInfo = "Attribute \"Breakpoint\" for " + strElemName + " \"" + pTestStep->getName(true) + "\" is out of range.";
                                    errResultInfoTmp.setSeverity(EResultSeverityError);
                                    errResultInfoTmp.setResult(EResultValueOutOfRange);
                                    errResultInfoTmp.setAddErrInfoDscr(strAddErrInfo);
                                    if( CErrLog::GetInstance() != nullptr )
                                    {
                                        CErrLog::GetInstance()->addEntry(errResultInfoTmp);
                                    }
                                }
                                else if( !bVal )
                                {
                                    pTestStep->removeBreakpoint();
                                }
                                else // if( bVal )
                                {
                                    pTestStep->setBreakpoint();

                                    if( i_pxmlStreamReader->attributes().hasAttribute("BreakpointDisabled") )
                                    {
                                        strVal = i_pxmlStreamReader->attributes().value("BreakpointDisabled").toString();
                                        bVal = str2Bool(strVal, &bConverted);
                                        if( !bConverted )
                                        {
                                            strAddErrInfo = "Attribute \"BreakpointDisabled\" for " + strElemName + " \"" + pTestStep->getName(true) + "\" is out of range.";
                                            errResultInfoTmp.setSeverity(EResultSeverityError);
                                            errResultInfoTmp.setResult(EResultValueOutOfRange);
                                            errResultInfoTmp.setAddErrInfoDscr(strAddErrInfo);
                                            if( CErrLog::GetInstance() != nullptr )
                                            {
                                                CErrLog::GetInstance()->addEntry(errResultInfoTmp);
                                            }
                                        }
                                        else if( bVal )
                                        {
                                            pTestStep->disableBreakpoint();
                                        }
                                        else
                                        {
                                            pTestStep->enableBreakpoint();
                                        }
                                    }
                                } // if( bVal )
                            } // if( i_pxmlStreamReader->attributes().hasAttribute("Breakpoint") )
                        } // if( pTestStep != nullptr )
                    } // if( !strName.isEmpty() )
                } // if( xmlStreamTokenType == QXmlStreamReader::StartElement )

                else if (xmlStreamTokenType == QXmlStreamReader::EndElement)
                {
                    pTestStep = nullptr;

                } // if (xmlStreamTokenType == QXmlStreamReader::EndElement)
            } // if( strElemName == "TestStep" )

            //---------------------------------------
            else
            //---------------------------------------
            {
                i_pxmlStreamReader->raiseError("Invalid element name \"" + strElemName + "\"");
            }
        } // if( i_pxmlStreamReader->isStartElement() || i_pxmlStreamReader->isEndElement() )

        if( errResultInfo.isErrorResult() )
        {
            break;
        }
    } // while( !i_pxmlStreamReader->atEnd() )

    if( i_pxmlStreamReader->hasError() )
    {
        strAddErrInfo = i_pxmlStreamReader->errorString();

        if( i_pxmlStreamReader->device() != nullptr )
        {
            QIODevice* pIODevice = i_pxmlStreamReader->device();
            QFile*     pFile = dynamic_cast<QFile*>(pIODevice);

            if( pFile != nullptr )
            {
                strAddErrInfo += " on parsing \"" + pFile->fileName() + "\".";
                strAddErrInfo += "Line:" + QString::number(i_pxmlStreamReader->lineNumber()) + ", ";
                strAddErrInfo += "Column:" + QString::number(i_pxmlStreamReader->columnNumber());
            }
        }

        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultFileReadContent);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
    }

    return errResultInfo;

} // deserialize

//------------------------------------------------------------------------------
SErrResultInfo CTestStepIdxTree::serialize(
    QXmlStreamWriter* i_pxmlStreamWriter,
    CTestStepGroup*   i_pTSGrpParent ) const
//------------------------------------------------------------------------------
{
    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".serialize";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QString strAddErrInfo;

    CTestStepGroup* pTSGrp = i_pTSGrpParent;
    CTestStep*      pTestStep;
    CTestStepGroup* pTSGrpChild;
    int             idxGrp;
    int             idxStep;

    if( i_pTSGrpParent == nullptr )
    {
        pTSGrp = m_pTSGrpRoot;
    }

    i_pxmlStreamWriter->writeStartElement("TestStepGroup");
    i_pxmlStreamWriter->writeAttribute( "Name", pTSGrp->getName() );
    i_pxmlStreamWriter->writeAttribute( "Enabled", CEnumEnabled::toString(pTSGrp->getEnabled()) );

    for( idxGrp = 0; idxGrp < pTSGrp->getTestStepGroupCount(); idxGrp++ )
    {
        pTSGrpChild = pTSGrp->getTestStepGroup(idxGrp);
        serialize(i_pxmlStreamWriter, pTSGrpChild);
    }

    for( idxStep = 0; idxStep < pTSGrp->getTestStepCount(); idxStep++ )
    {
        pTestStep = pTSGrp->getTestStep(idxStep);

        i_pxmlStreamWriter->writeStartElement("TestStep");
        i_pxmlStreamWriter->writeAttribute( "Name", pTestStep->getName() );
        i_pxmlStreamWriter->writeAttribute( "Enabled", CEnumEnabled::toString(pTestStep->getEnabled()) );
        i_pxmlStreamWriter->writeAttribute( "Breakpoint", bool2Str(pTestStep->isBreakpointSet()) );
        if( pTestStep->isBreakpointSet() )
        {
            i_pxmlStreamWriter->writeAttribute( "BreakpointDisabled", bool2Str(pTestStep->isBreakpointDisabled()) );
        }
        i_pxmlStreamWriter->writeEndElement(/*"TestStep"*/);
    }

    i_pxmlStreamWriter->writeEndElement(/*"TestStepGroup"*/);

    return errResultInfo;

} // serialize

/*==============================================================================
protected: // overridables of base class CModelIdxTree
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStepIdxTree::clearTreeEntry( #error CAbstractIdxTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    // This is a protected method which may only be called through the public
    // methods. The mutex to protect the list and tree has already been locked.

    // The "removeEntry" method removes the given tree entry from the model.
    // If the given tree entry has childs also all child entries will be removed
    // and deleted. But the given tree entry will not be deleted. This is the task
    // of the callee.

    // This "removeEntry" method is called:
    // - On clearing the model. In this case the entry together with all childs
    //   need to be deleted and removed from the model.

    if( i_pTreeEntry == nullptr )
    {
        return;
    }

    if( i_pTreeEntry->getChildCount() > 0 )
    {
        int                iRow;
        #error CAbstractIdxTreeEntry* pTreeEntryChild;
        CAbstractTestStepIdxTreeEntry* pTSAdmObj;
        CTestStepGroup*    pTSGrp;
        CTestStep*         pTestStep;

        // Starting from end of list to first entry so there is
        // no need to correct the row id's of the childs ..
        for( iRow = static_cast<int>(i_pTreeEntry->getChildCount()-1); iRow >= 0; iRow-- )
        {
            pTreeEntryChild = i_pTreeEntry->getChildEntry(iRow);

            if( pTreeEntryChild != nullptr && pTreeEntryChild->getEntryType() != EIdxTreeEntryTypeObject )
            {
                clearTreeEntry(pTreeEntryChild);
            }

            //beginRemoveRows( i_pTreeEntry->getModelIdx(), iRow, iRow );
            i_pTreeEntry->removeChildEntry(iRow);
            //endRemoveRows();

            pTSAdmObj = reinterpret_cast<CAbstractTestStepIdxTreeEntry*>(pTreeEntryChild->getObj());

            pTSGrp = nullptr;
            pTestStep = nullptr;

            if( pTSAdmObj != nullptr )
            {
                if( pTSAdmObj->isGroup() )
                {
                    pTSGrp = dynamic_cast<CTestStepGroup*>(pTSAdmObj);
                }
                else
                {
                    pTestStep = dynamic_cast<CTestStep*>(pTSAdmObj);
                }
            }

            if( pTSGrp != nullptr )
            {
                delete pTSGrp;
                pTSGrp = nullptr;
            }
            else if( pTestStep != nullptr )
            {
                invalidateObjNode(pTestStep->getObjId());

                delete pTestStep;
                pTestStep = nullptr;
            }

            if( m_pTreeEntryCurr == pTreeEntryChild )
            {
                m_pTreeEntryCurr = nullptr;
            }
            delete pTreeEntryChild;
            pTreeEntryChild = nullptr;
        }
    }

} // clearTreeEntry

#endif

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! Saves the settings of the test steps and test step groups in an XML file.

    The attributes (enabled, breakpoints) will be save so that they can be
    recalled on restarting the test.

    Please note that only the settings for those index tree entries may be recalled
    which again will be found on recalling the test tree.

    @param i_strAbsFilePath [in] Absolute path name of the test steps file.
*/
SErrResultInfo CTestStepIdxTree::save( const QString& i_strAbsFilePath ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

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

    QString strMth = "save";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QFile file;

    if( i_strAbsFilePath.isEmpty() )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInvalidFileName);
    }
    else
    {
        QFileInfo fileInfo(i_strAbsFilePath);
        QDir      dir = fileInfo.absoluteDir();

        if( !dir.exists() )
        {
            dir.mkpath(dir.absolutePath());
        }

        file.setFileName(i_strAbsFilePath);

        if( !file.open(QIODevice::WriteOnly) )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultFileOpenForWrite);
            errResultInfo.setAddErrInfoDscr(i_strAbsFilePath);
        }
    }

    if( !errResultInfo.isErrorResult() )
    {
        QByteArray byteArrSchema;

        QXmlStreamWriter xmlStreamWriter(&byteArrSchema);

        xmlStreamWriter.setAutoFormatting(true);

        xmlStreamWriter.writeStartDocument();
        xmlStreamWriter.writeStartElement("Test");
        xmlStreamWriter.writeAttribute( "Name", m_pRoot->name() );

        save(xmlStreamWriter, m_pRoot);

        xmlStreamWriter.writeEndElement();  // Method Trace Admin Objects
        xmlStreamWriter.writeEndDocument();

        if( !errResultInfo.isErrorResult() )
        {
            file.write(byteArrSchema);
        }
    } // if( !errResultInfo.isErrorResult() )

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

    @param i_strAbsFilePath [in] Absolute path name of the test steps file.
*/
SErrResultInfo CTestStepIdxTree::recall( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "AbsFilePath: " + i_strAbsFilePath;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod       */ "recall",
        /* strMthInArgs    */ strMthInArgs );

    QString strMth = "recall";

    QString strAddErrInfo;

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QFile file;

    if( i_strAbsFilePath.isEmpty() )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInvalidFileName);
    }
    else
    {
        file.setFileName(i_strAbsFilePath);

        if( !file.open(QIODevice::ReadOnly) )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultFileOpenForRead);
            errResultInfo.setAddErrInfoDscr(i_strAbsFilePath);
        }
    }

    if( !errResultInfo.isErrorResult() )
    {
        QXmlStreamReader            xmlStreamReader(&file);
        QXmlStreamReader::TokenType xmlStreamTokenType;

        SErrResultInfo errResultInfoTmp = ErrResultInfoSuccess(strMth);

        CTestStepGroup* pTSGrp = nullptr;
        CTestStep*      pTestStep = nullptr;

        QString  strElemName;
        QString  strName;
        QString  strVal;
        EEnabled enabled = EEnabled::No;
        bool     bVal = false;
        bool     bConverted = false;

        xmlStreamTokenType = xmlStreamReader.readNext();

        if( xmlStreamTokenType != QXmlStreamReader::StartDocument )
        {
            xmlStreamReader.raiseError("Invalid XML document");
        }

        xmlStreamTokenType = xmlStreamReader.readNext();
        strElemName = xmlStreamReader.name().toString();

        if( xmlStreamTokenType != QXmlStreamReader::StartElement || strElemName != "Test" )
        {
            xmlStreamReader.raiseError("Invalid XML document (missing root node \"Test\")");
        }

        if( !xmlStreamReader.hasError() )
        {
            while( !xmlStreamReader.atEnd() )
            {
                xmlStreamTokenType = xmlStreamReader.readNext();

                if( xmlStreamReader.isStartElement() || xmlStreamReader.isEndElement() )
                {
                    strElemName = xmlStreamReader.name().toString();

                    // For best performance start with the most frequently used element names ..

                    //---------------------------------------
                    if( strElemName == "TestStepGroup" )
                    //---------------------------------------
                    {
                        if( xmlStreamTokenType == QXmlStreamReader::StartElement )
                        {
                            strName = xmlStreamReader.attributes().value("Name").toString();

                            if( strName.isEmpty() )
                            {
                                strAddErrInfo = "Attribute \"Name\" for \"" + strElemName + "\" not defined";
                                xmlStreamReader.raiseError(strAddErrInfo);
                            }
                            else // if( !strName.isEmpty() )
                            {
                                CTestStepGroup* pTSGrpTmp = findTestStepGroup(pTSGrp, strName);

                                if( pTSGrpTmp != nullptr )
                                {
                                    pTSGrp = pTSGrpTmp;

                                    if( xmlStreamReader.attributes().hasAttribute("Enabled") )
                                    {
                                        strVal = xmlStreamReader.attributes().value("Enabled").toString();
                                        enabled = CEnumEnabled::toEnumerator(strVal);
                                        if( enabled == EEnabled::Undefined )
                                        {
                                            strAddErrInfo = "Attribute \"Enabled\" for " + strElemName + " \"" + pTSGrp->path() + "\" is out of range.";
                                            errResultInfoTmp.setSeverity(EResultSeverityError);
                                            errResultInfoTmp.setResult(EResultValueOutOfRange);
                                            errResultInfoTmp.setAddErrInfoDscr(strAddErrInfo);
                                            if( CErrLog::GetInstance() != nullptr )
                                            {
                                                CErrLog::GetInstance()->addEntry(errResultInfoTmp);
                                            }
                                        }
                                        else
                                        {
                                            pTSGrp->setEnabled(enabled);
                                        }
                                    }
                                } // if( pTSGrpTmp != nullptr )
                            } // if( !strName.isEmpty() )
                        } // if( xmlStreamTokenType == QXmlStreamReader::StartElement )

                        else if (xmlStreamTokenType == QXmlStreamReader::EndElement)
                        {
                            if( pTSGrp != nullptr )
                            {
                                pTSGrp = pTSGrp->getParentGroup();
                            }
                        } // if (xmlStreamTokenType == QXmlStreamReader::EndElement)
                    } // if( strElemName == "TestStepGroup" )

                    //---------------------------------------
                    else if( strElemName == "TestStep" )
                    //---------------------------------------
                    {
                        if( xmlStreamTokenType == QXmlStreamReader::StartElement )
                        {
                            strName = xmlStreamReader.attributes().value("Name").toString();

                            if( strName.isEmpty() )
                            {
                                strAddErrInfo = "Attribute \"Name\" for \"" + strElemName + "\" not defined";
                                xmlStreamReader.raiseError(strAddErrInfo);
                            }
                            else // if( !strName.isEmpty() )
                            {
                                pTestStep = findTestStep(pTSGrp, strName);

                                if( pTestStep != nullptr )
                                {
                                    if( xmlStreamReader.attributes().hasAttribute("Enabled") )
                                    {
                                        strVal = xmlStreamReader.attributes().value("Enabled").toString();
                                        enabled = CEnumEnabled::toEnumerator(strVal);
                                        if( enabled == EEnabled::Undefined )
                                        {
                                            strAddErrInfo = "Attribute \"Enabled\" for " + strElemName + " \"" + pTestStep->path() + "\" is out of range.";
                                            errResultInfoTmp.setSeverity(EResultSeverityError);
                                            errResultInfoTmp.setResult(EResultValueOutOfRange);
                                            errResultInfoTmp.setAddErrInfoDscr(strAddErrInfo);
                                            if( CErrLog::GetInstance() != nullptr )
                                            {
                                                CErrLog::GetInstance()->addEntry(errResultInfoTmp);
                                            }
                                        }
                                        else
                                        {
                                            pTestStep->setEnabled(enabled);
                                        }
                                    }
                                    if( xmlStreamReader.attributes().hasAttribute("Breakpoint") )
                                    {
                                        strVal = xmlStreamReader.attributes().value("Breakpoint").toString();
                                        bVal = str2Bool(strVal, &bConverted);
                                        if( !bConverted )
                                        {
                                            strAddErrInfo = "Attribute \"Breakpoint\" for " + strElemName + " \"" + pTestStep->path() + "\" is out of range.";
                                            errResultInfoTmp.setSeverity(EResultSeverityError);
                                            errResultInfoTmp.setResult(EResultValueOutOfRange);
                                            errResultInfoTmp.setAddErrInfoDscr(strAddErrInfo);
                                            if( CErrLog::GetInstance() != nullptr )
                                            {
                                                CErrLog::GetInstance()->addEntry(errResultInfoTmp);
                                            }
                                        }
                                        else if( !bVal )
                                        {
                                            pTestStep->removeBreakpoint();
                                        }
                                        else // if( bVal )
                                        {
                                            pTestStep->setBreakpoint();

                                            if( xmlStreamReader.attributes().hasAttribute("BreakpointEnabled") )
                                            {
                                                strVal = xmlStreamReader.attributes().value("BreakpointEnabled").toString();
                                                bVal = str2Bool(strVal, &bConverted);
                                                if( !bConverted )
                                                {
                                                    strAddErrInfo = "Attribute \"BreakpointEnabled\" for " + strElemName + " \"" + pTestStep->path() + "\" is out of range.";
                                                    errResultInfoTmp.setSeverity(EResultSeverityError);
                                                    errResultInfoTmp.setResult(EResultValueOutOfRange);
                                                    errResultInfoTmp.setAddErrInfoDscr(strAddErrInfo);
                                                    if( CErrLog::GetInstance() != nullptr )
                                                    {
                                                        CErrLog::GetInstance()->addEntry(errResultInfoTmp);
                                                    }
                                                }
                                                else if( bVal )
                                                {
                                                    pTestStep->disableBreakpoint();
                                                }
                                                else
                                                {
                                                    pTestStep->enableBreakpoint();
                                                }
                                            }
                                        } // if( bVal )
                                    } // if( xmlStreamReader.attributes().hasAttribute("Breakpoint") )
                                } // if( pTestStep != nullptr )
                            } // if( !strName.isEmpty() )
                        } // if( xmlStreamTokenType == QXmlStreamReader::StartElement )

                        else if (xmlStreamTokenType == QXmlStreamReader::EndElement)
                        {
                            pTestStep = nullptr;

                        } // if (xmlStreamTokenType == QXmlStreamReader::EndElement)
                    } // if( strElemName == "TestStep" )

                    //---------------------------------------
                    else if( strElemName == "Test" )
                    //---------------------------------------
                    {
                        if( xmlStreamTokenType != QXmlStreamReader::EndElement )
                        {
                            xmlStreamReader.raiseError("Invalid element name \"" + strElemName + "\"");
                        }
                    }

                    //---------------------------------------
                    else
                    //---------------------------------------
                    {
                        xmlStreamReader.raiseError("Invalid element name \"" + strElemName + "\"");
                    }
                } // if( xmlStreamReader.isStartElement() || xmlStreamReader.isEndElement() )
            } // while( !xmlStreamReader.atEnd() )
        } // if( !xmlStreamReader.hasError() )

        if( xmlStreamReader.hasError() )
        {
            strAddErrInfo  = xmlStreamReader.errorString() + " on reading " + i_strAbsFilePath + " ";
            strAddErrInfo += "Line:" + QString::number(xmlStreamReader.lineNumber()) + ", ";
            strAddErrInfo += "Column:" + QString::number(xmlStreamReader.columnNumber());

            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultFileReadContent);
            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        }
    } // if( !errResultInfo.isErrorResult() )

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(errResultInfo);
    }

    return errResultInfo;

} // recall

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStepIdxTree::save(
    QXmlStreamWriter& i_xmlStreamWriter,
    CIdxTreeEntry*    i_pTreeEntry ) const
//------------------------------------------------------------------------------
{
    // This is a protected method which may only be called through the public
    // methods. The mutex to protect the list and tree has already been locked.

    CIdxTreeEntry* pTreeEntry;
    int            idxEntry;

    if( i_pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
    {
        CTestStep* pTestStep = dynamic_cast<CTestStep*>(i_pTreeEntry);

        i_xmlStreamWriter.writeStartElement("TestStep");
        i_xmlStreamWriter.writeAttribute( "Name", pTestStep->name() );
        i_xmlStreamWriter.writeAttribute( "Enabled", CEnumEnabled::toString(pTestStep->getEnabled()) );
        i_xmlStreamWriter.writeAttribute( "Breakpoint", bool2Str(pTestStep->isBreakpointSet()) );
        if( pTestStep->isBreakpointSet() )
        {
            i_xmlStreamWriter.writeAttribute( "BreakpointEnabled", bool2Str(pTestStep->isBreakpointEnabled()) );
        }
        i_xmlStreamWriter.writeEndElement(/*"TestStep"*/);
    }
    else if( i_pTreeEntry->entryType() == EIdxTreeEntryType::Branch )
    {
        CTestStepGroup* pTestStepGroup = dynamic_cast<CTestStepGroup*>(i_pTreeEntry);

        i_xmlStreamWriter.writeStartElement("TestStepGroup");
        i_xmlStreamWriter.writeAttribute( "Name", pTestStepGroup->name() );
        i_xmlStreamWriter.writeAttribute( "Enabled", CEnumEnabled::toString(pTestStepGroup->getEnabled()) );

        for( idxEntry = 0; idxEntry < i_pTreeEntry->count(); ++idxEntry )
        {
            pTreeEntry = i_pTreeEntry->at(idxEntry);

            if( pTreeEntry != nullptr )
            {
                save(i_xmlStreamWriter, pTreeEntry);
            }
        }
        i_xmlStreamWriter.writeEndElement(/*"TestStepGroup"*/);
    }
    else // if( i_pTreeEntry->entryType() == EIdxTreeEntryType::Root )
    {
        for( idxEntry = 0; idxEntry < i_pTreeEntry->count(); ++idxEntry )
        {
            pTreeEntry = i_pTreeEntry->at(idxEntry);

            if( pTreeEntry != nullptr )
            {
                save(i_xmlStreamWriter, pTreeEntry);
            }
        }
    }

} // save
