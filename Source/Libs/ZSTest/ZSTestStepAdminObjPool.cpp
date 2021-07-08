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

#include "ZSTest/ZSTestStepAdminObjPool.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSTest/ZSTest.h"
#include "ZSSys/ZSSysAux.h"
//#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Test;


/*******************************************************************************
class CTestStepAdminObjPool : public ZS::CObjPool
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTestStepAdminObjPool::CTestStepAdminObjPool(
    CTest*         i_pTest,
    const QString& i_strTestStepsFileName,
    const QString& i_strNodeSeparator ) :
//------------------------------------------------------------------------------
    CObjPool(
        /* strModelName           */ i_pTest->objectName(),
        /* strNodeSeparator       */ i_strNodeSeparator,
        /* bSortTreeEntriesByName */ false ),
    m_pTest(i_pTest),
    m_pTSGrpRoot(nullptr),
    m_bInitializingTest(false),
    m_iTestStepsExecutedCount(0),
    m_iTestStepsPassedCount(0),
    m_iTestStepsFailedCount(0),
    m_iTestStepsIgnoredCount(0),
    m_iUpdateGroupCallDepth(0)
{
    m_pTSGrpRoot = getTestStepGroup(
        /* strGroupName     */ i_pTest->objectName(),
        /* pTestGroupParent */ nullptr,
        /* enabled          */ EEnabled::Undefined );

    // Set default value for file name
    //--------------------------------

    QString strTestStepsFileName = i_strTestStepsFileName;

    if( strTestStepsFileName.isEmpty() )
    {
        #ifdef _WINDOWS
        #pragma message(__TODO__"Default file name")
        #endif
    }

    m_strFileName = strTestStepsFileName;

} // ctor

//------------------------------------------------------------------------------
CTestStepAdminObjPool::~CTestStepAdminObjPool()
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    m_pMtxObjs->lock();

    // Need to call clear here to destroy the test step group and test step
    // objects. After the model has been destroyed the virtual method
    // "clearTreeEntry" of this test model class cannot be called anymore
    // (the virtual method table is no longer valid).
    clear();

    try
    {
        delete m_pTSGrpRoot;
    }
    catch(...)
    {
    }

    m_pTest = nullptr;
    m_pTSGrpRoot = nullptr;
    m_bInitializingTest = false;
    m_iTestStepsExecutedCount = 0;
    m_iTestStepsPassedCount = 0;
    m_iTestStepsFailedCount = 0;
    m_iTestStepsIgnoredCount = 0;

    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    m_pMtxObjs->unlock();

} // dtor

/*==============================================================================
public: // overridables of base class CModelObjPool
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CTestStepAdminObjPool::read_( const QString& i_strFileName )
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".read_";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    if( m_strFileName.isEmpty() )
    {
        m_strFileName = i_strFileName;
    }

    QString strFileName = m_strFileName;

    if( !i_strFileName.isEmpty() )
    {
        strFileName = i_strFileName;
    }

    QFile file;

    if( strFileName.isEmpty() )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInvalidFileName);
    }
    else // if( !strFileName.isEmpty() )
    {
        file.setFileName(strFileName);

        if( !file.open(QIODevice::ReadOnly) )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultFileOpenForRead);
            errResultInfo.setAddErrInfoDscr(strFileName);
        }
    }

    if( !errResultInfo.isErrorResult() )
    {
        QXmlStreamReader xmlStreamReader(&file);

        errResultInfo = deserialize(&xmlStreamReader);
    }

    return errResultInfo;

} // read_

//------------------------------------------------------------------------------
SErrResultInfo CTestStepAdminObjPool::save_( const QString& i_strFileName )
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    if( !i_strFileName.isEmpty() )
    {
        m_strFileName = i_strFileName;
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".save_";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    if( m_strFileName.isEmpty() )
    {
        m_strFileName = i_strFileName;
    }

    QString strFileName = m_strFileName;

    if( !i_strFileName.isEmpty() )
    {
        strFileName = i_strFileName;
    }

    QFile file;

    if( strFileName.isEmpty() )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInvalidFileName);
    }
    else // if( !strFileName.isEmpty() )
    {
        QFileInfo fileInfo(strFileName);
        QDir      dir = fileInfo.absoluteDir();

        if( !dir.exists() )
        {
            dir.mkpath(dir.absolutePath());
        }

        file.setFileName(strFileName);

        if( !file.open(QIODevice::WriteOnly) )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultFileOpenForWrite);
            errResultInfo.setAddErrInfoDscr(strFileName);
        }
    } // if( !strFileName.isEmpty() )

    if( !errResultInfo.isErrorResult() )
    {
        QXmlStreamWriter xmlStreamWriter(&file);

        xmlStreamWriter.setAutoFormatting(true);

        errResultInfo = serialize(&xmlStreamWriter);

        // Recursively save the test step object tree
        //saveTreeEntry(domdoc, domelemBody, m_pTreeObjs);
    }

    return errResultInfo;

} // save_

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CTestStepAdminObjPool::getTestStepsCount() const
//------------------------------------------------------------------------------
{
    return m_pTSGrpRoot->getTestStepCount(true);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CTestStepAdminObjPool::buildPathStr( const QString& i_str1, const QString& i_str2 )
//------------------------------------------------------------------------------
{
    QString strPath = i_str1;

    if( !strPath.isEmpty() && !i_str2.isEmpty() )
    {
        strPath += m_strNodeSeparator;
    }
    if( !i_str2.isEmpty() )
    {
        strPath += i_str2;
    }
    return strPath;

} // buildPathStr

/*==============================================================================
public: // instance methods to add, remove and modify test step groups
==============================================================================*/

//------------------------------------------------------------------------------
CTestStepGroup* CTestStepAdminObjPool::findTestStepGroup(
    const QString&  i_strGroupName,
    const QString&  i_strGroupPathParent )
//------------------------------------------------------------------------------
{
    CTestStepGroup*    pTSGrp = nullptr;
    CTestStepAdminObj* pTSAdmObj;
    CObjPoolTreeEntry* pTreeEntry;
    QString            strPath = i_strGroupName;

    if( !i_strGroupPathParent.isEmpty() )
    {
        strPath = buildPathStr(i_strGroupPathParent, i_strGroupName);
    }

    pTreeEntry = getTreeEntry(
        /* strPath   */ strPath,
        /* entryType */ EObjPoolEntryTypeNameSpace );

    if( pTreeEntry != nullptr )
    {
        pTSAdmObj = reinterpret_cast<CTestStepAdminObj*>(pTreeEntry->getObj());

        if( pTSAdmObj != nullptr && pTSAdmObj->isGroup() )
        {
            pTSGrp = dynamic_cast<CTestStepGroup*>(pTSAdmObj);
        }
    } // if( pTreeEntry != nullptr )

    return pTSGrp;

} // findTestStepGroup

//------------------------------------------------------------------------------
CTestStepGroup* CTestStepAdminObjPool::findTestStepGroup(
    const QString&  i_strGroupName,
    CTestStepGroup* i_pTestGroupParent )
//------------------------------------------------------------------------------
{
    QString strGroupPathParent;

    if( i_pTestGroupParent != nullptr )
    {
        strGroupPathParent = i_pTestGroupParent->getName(true);
    }

    return findTestStepGroup(i_strGroupName, strGroupPathParent);

} // findTestStepGroup

//------------------------------------------------------------------------------
CTestStepGroup* CTestStepAdminObjPool::addTestStepGroup(
    const QString&  i_strGroupName,
    const QString&  i_strGroupPathParent,
    EEnabled        i_enabled )
//------------------------------------------------------------------------------
{
    CTestStepGroup*    pTSGrp = nullptr;
    CObjPoolTreeEntry* pTreeEntry;
    QString            strPath = i_strGroupName;

    if( !i_strGroupPathParent.isEmpty() )
    {
        strPath = buildPathStr(i_strGroupPathParent, i_strGroupName);
    }

    pTreeEntry = getTreeEntry(
        /* strPath   */ strPath,
        /* entryType */ EObjPoolEntryTypeNameSpace );

    if( pTreeEntry != nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, "A group \"" + i_strGroupName + "\" is already existing below \"" + i_strGroupPathParent + "\".");
    }

    // Please note that on adding a node to the model the "nodeInserted"
    // signal is emitted but the test step object has not yet been set
    // at the models list entry. So the client of the test step object
    // pool model need to connect to the "testStepGroupInserted" signal.
    pTreeEntry = addNameSpaceNode(i_strGroupPathParent, i_strGroupName);

    if( pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError, "Couldn't add namespace node \"" + i_strGroupName + "\" below \"" + i_strGroupPathParent + "\".");
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
CTestStepGroup* CTestStepAdminObjPool::addTestStepGroup(
    const QString&  i_strGroupName,
    CTestStepGroup* i_pTestGroupParent,
    EEnabled        i_enabled )
//------------------------------------------------------------------------------
{
    QString strGroupPathParent;

    if( i_pTestGroupParent != nullptr )
    {
        strGroupPathParent = i_pTestGroupParent->getName(true);
    }

    return addTestStepGroup(i_strGroupName, strGroupPathParent, i_enabled);

} // addTestStepGroup

//------------------------------------------------------------------------------
void CTestStepAdminObjPool::addTestStepGroup(
    CTestStepGroup* i_pTSGrp,
    CTestStepGroup* i_pTSGrpParent )
//------------------------------------------------------------------------------
{
    CObjPoolTreeEntry* pTreeEntry;

    QString strName = i_pTSGrp->getName();
    QString strGroupPathParent;
    QString strPath = strName;

    if( i_pTSGrpParent != nullptr )
    {
        strGroupPathParent = i_pTSGrpParent->getName(true);
        strPath = buildPathStr(strGroupPathParent, strName);
    }

    pTreeEntry = getTreeEntry(strPath, EObjPoolEntryTypeNameSpace);

    if( pTreeEntry != nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, strPath);
    }

    // Please note that on adding a node to the model the "nodeInserted"
    // signal is emitted but the test step object has not yet been set
    // at the models list entry. So the client of the test step object
    // pool model need to connect to the "testStepGroupInserted" signal.
    pTreeEntry = addNameSpaceNode(strGroupPathParent, strName);

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
CTestStepGroup* CTestStepAdminObjPool::getTestStepGroup(
    const QString&  i_strGroupName,
    const QString&  i_strGroupPathParent,
    EEnabled        i_enabled )
//------------------------------------------------------------------------------
{
    CTestStepGroup*    pTSGrp = nullptr;
    CTestStepAdminObj* pTSAdmObj;
    CObjPoolTreeEntry* pTreeEntry;
    QString            strPath = i_strGroupName;

    if( !i_strGroupPathParent.isEmpty() )
    {
        strPath = buildPathStr(i_strGroupPathParent, i_strGroupName);
    }

    pTreeEntry = getTreeEntry(
        /* strPath   */ strPath,
        /* entryType */ EObjPoolEntryTypeNameSpace );

    if( pTreeEntry == nullptr )
    {
        // Please note that on adding a node to the model the "nodeInserted"
        // signal is emitted but the test step object has not yet been set
        // at the models list entry. So the client of the test step object
        // pool model need to connect to the "testStepGroupInserted" signal.
        pTreeEntry = addNameSpaceNode(i_strGroupPathParent,i_strGroupName);
    }

    if( pTreeEntry != nullptr )
    {
        pTSAdmObj = reinterpret_cast<CTestStepAdminObj*>(pTreeEntry->getObj());

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
CTestStepGroup* CTestStepAdminObjPool::getTestStepGroup(
    const QString&  i_strGroupName,
    CTestStepGroup* i_pTestGroupParent,
    EEnabled        i_enabled )
//------------------------------------------------------------------------------
{
    QString strGroupPathParent;

    if( i_pTestGroupParent != nullptr )
    {
        strGroupPathParent = i_pTestGroupParent->getName(true);
    }

    return getTestStepGroup(
        /* strName            */ i_strGroupName,
        /* strGroupPathParent */ strGroupPathParent,
        /* enabled            */ i_enabled );

} // getTestStepGroup

//------------------------------------------------------------------------------
void CTestStepAdminObjPool::onTestStepGroupChanged( CTestStepGroup* i_pTSGrp )
//------------------------------------------------------------------------------
{
    if( i_pTSGrp == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pTSGrp == nullptr");
    }

    CObjPoolTreeEntry* pTreeEntry = i_pTSGrp->getTreeEntry();

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
CTestStep* CTestStepAdminObjPool::getTestStep( int i_iObjId )
//------------------------------------------------------------------------------
{
    CTestStep*         pTestStep = nullptr;
    CTestStepAdminObj* pTSAdmObj;
    CObjPoolListEntry* pListEntry;

    if( i_iObjId >= 0 && i_iObjId < static_cast<int>(getObjCount()) )
    {
        pListEntry = getListEntry(i_iObjId);

        if( pListEntry != nullptr )
        {
            pTSAdmObj = reinterpret_cast<CTestStepAdminObj*>(pListEntry->getObj());

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

//------------------------------------------------------------------------------
CTestStep* CTestStepAdminObjPool::findTestStep(
    const QString& i_strName,
    const QString& i_strGroupPathParent )
//------------------------------------------------------------------------------
{
    CTestStep*         pTestStep = nullptr;
    CTestStepAdminObj* pTSAdmObj;
    CObjPoolTreeEntry* pTreeEntry = nullptr;
    CObjPoolListEntry* pListEntry = nullptr;
    QString            strPath = i_strName;

    if( !i_strGroupPathParent.isEmpty() )
    {
        strPath = buildPathStr(i_strGroupPathParent, i_strName);
    }

    pTreeEntry = getTreeEntry(
        /* strPath   */ strPath,
        /* entryType */ EObjPoolEntryTypeObject );

    if( pTreeEntry != nullptr )
    {
        int iObjId = pTreeEntry->getObjId();

        pListEntry = getListEntry(iObjId);

        if( pListEntry == nullptr )
        {
            throw CException(__FILE__, __LINE__, EResultInternalProgramError, "TreeEntry \"" + pTreeEntry->getNodeName(true) + "\" doesn't point to a list entry.");
        }

        pTSAdmObj = reinterpret_cast<CTestStepAdminObj*>(pListEntry->getObj());

        if( pTSAdmObj != nullptr && !pTSAdmObj->isGroup() )
        {
            pTestStep = dynamic_cast<CTestStep*>(pTSAdmObj);
        }
    } // if( pTreeEntry != nullptr )

    return pTestStep;

} // findTestStep

//------------------------------------------------------------------------------
CTestStep* CTestStepAdminObjPool::findTestStep(
    const QString&  i_strName,
    CTestStepGroup* i_pTestGroupParent )
//------------------------------------------------------------------------------
{
    QString strGroupPathParent;

    if( i_pTestGroupParent != nullptr )
    {
        strGroupPathParent = i_pTestGroupParent->getName(true);
    }

    return findTestStep(i_strName, strGroupPathParent);

} // findTestStep

//------------------------------------------------------------------------------
CTestStep* CTestStepAdminObjPool::addTestStep(
    const QString& i_strName,
    const QString& i_strGroupPathParent,
    EEnabled       i_enabled )
//------------------------------------------------------------------------------
{
    CTestStep*         pTestStep = nullptr;
    CObjPoolTreeEntry* pTreeEntry = nullptr;
    CObjPoolListEntry* pListEntry = nullptr;
    QString            strPath = i_strName;

    if( !i_strGroupPathParent.isEmpty() )
    {
        strPath = buildPathStr(i_strGroupPathParent, i_strName);
    }

    pTreeEntry = getTreeEntry(
        /* strPath   */ strPath,
        /* entryType */ EObjPoolEntryTypeObject );

    if( pTreeEntry != nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, "A test step \"" + i_strName + "\" is already existing below \"" + i_strGroupPathParent + "\".");
    }

    int iObjId = addObjNode(strPath);

    pListEntry = getListEntry(iObjId);

    if( pListEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError, "No list entry created for \"" + i_strName + "\" below \"" + i_strGroupPathParent + "\".");
    }

    pTestStep = new CTestStep(
        /* pObjPool   */ this,
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
CTestStep* CTestStepAdminObjPool::addTestStep(
    const QString&  i_strName,
    CTestStepGroup* i_pTestGroupParent,
    EEnabled        i_enabled )
//------------------------------------------------------------------------------
{
    QString strGroupPathParent;

    if( i_pTestGroupParent != nullptr )
    {
        strGroupPathParent = i_pTestGroupParent->getName(true);
    }

    return addTestStep(
        /* strName            */ i_strName,
        /* strGroupPathParent */ strGroupPathParent,
        /* enabled            */ i_enabled );

} // addTestStep

//------------------------------------------------------------------------------
void CTestStepAdminObjPool::addTestStep(
    CTestStep*      i_pTestStep,
    CTestStepGroup* i_pTSGrpParent )
//------------------------------------------------------------------------------
{
    CObjPoolTreeEntry* pTreeEntry = nullptr;
    CObjPoolListEntry* pListEntry = nullptr;

    QString strName = i_pTestStep->getName();
    QString strGroupPathParent;
    QString strPath = strName;

    if( i_pTSGrpParent != nullptr )
    {
        strGroupPathParent = i_pTSGrpParent->getName(true);
        strPath = buildPathStr(strGroupPathParent, strName);
    }

    pTreeEntry = getTreeEntry(strPath, EObjPoolEntryTypeObject);

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
CTestStep* CTestStepAdminObjPool::getTestStep(
    const QString& i_strName,
    const QString& i_strGroupPathParent,
    EEnabled       i_enabled,
    int            i_iObjId )
//------------------------------------------------------------------------------
{
    CTestStep*         pTestStep = nullptr;
    CTestStepAdminObj* pTSAdmObj;
    CObjPoolTreeEntry* pTreeEntry = nullptr;
    CObjPoolListEntry* pListEntry = nullptr;
    int                iObjId = i_iObjId;
    QString            strPath = i_strName;

    if( !i_strGroupPathParent.isEmpty() )
    {
        strPath = buildPathStr(i_strGroupPathParent, i_strName);
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
            /* entryType */ EObjPoolEntryTypeObject );
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

        pTSAdmObj = reinterpret_cast<CTestStepAdminObj*>(pListEntry->getObj());

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
CTestStep* CTestStepAdminObjPool::getTestStep(
    const QString&  i_strName,
    CTestStepGroup* i_pTestGroupParent,
    EEnabled        i_enabled,
    int             i_iObjId )
//------------------------------------------------------------------------------
{
    QString strGroupPathParent;

    if( i_pTestGroupParent != nullptr )
    {
        strGroupPathParent = i_pTestGroupParent->getName(true);
    }

    return getTestStep(
        /* strName            */ i_strName,
        /* strGroupPathParent */ strGroupPathParent,
        /* enabled            */ i_enabled,
        /* iObjId             */ i_iObjId );

} // getTestStep

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStepAdminObjPool::onTestStepChanged( CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    if( i_pTestStep == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pTestStep == nullptr");
    }

    CObjPoolListEntry* pListEntry = i_pTestStep->getListEntry();

    if( pListEntry == nullptr )
    {
        pListEntry = getListEntry(i_pTestStep->getObjId());
    }

    if( pListEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pTestStep->m_pListEntry == nullptr");
    }

    CObjPoolTreeEntry* pTreeEntry = pListEntry->getTreeEntry();

    if( pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pTestStep->m_pTreeEntry->m_pTreeEntry == nullptr");
    }

    emit nodeChanged(this, pTreeEntry);
    emit objectNodeChanged(this, pListEntry);

    emit testStepChanged(this, i_pTestStep);

} // onTestStepChanged

//------------------------------------------------------------------------------
void CTestStepAdminObjPool::testStepStarted( CTestStep* i_pTestStep )
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
void CTestStepAdminObjPool::testStepEnded( CTestStep* i_pTestStep )
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
void CTestStepAdminObjPool::onTestGroupChanged( CTestStepGroup* i_pTSGrp )
//------------------------------------------------------------------------------
{
    if( i_pTSGrp == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pTSGrp == nullptr");
    }

    CObjPoolTreeEntry* pTreeEntry = i_pTSGrp->getTreeEntry();

    if( pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pTSGrp->m_pTreeEntry->m_pTreeEntry == nullptr");
    }

    emit nodeChanged(this, pTreeEntry);

    emit testStepGroupChanged(this, i_pTSGrp);

} // onTestGroupChanged

//------------------------------------------------------------------------------
void CTestStepAdminObjPool::testGroupStarted( CTestStepGroup* i_pTSGrp )
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
void CTestStepAdminObjPool::testGroupEnded( CTestStepGroup* i_pTSGrp )
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
int CTestStepAdminObjPool::getUpdateGroupCallDepth() const
//------------------------------------------------------------------------------
{
    return m_iUpdateGroupCallDepth;
}

//------------------------------------------------------------------------------
void CTestStepAdminObjPool::updateGroup(
    const QString& i_strName,
    const QString& i_strGroupNameParent,
    EEnabled       i_enabled )
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    CObjPoolTreeEntry* pTreeEntry = getTreeEntry(
        /* strParentNameSpace */ i_strGroupNameParent,
        /* strNodeName        */ i_strName,
        /* entryType          */ EObjPoolEntryTypeNameSpace );

    if( pTreeEntry != nullptr )
    {
        updateGroup( pTreeEntry, i_enabled );
    }

} // updateGroup

//------------------------------------------------------------------------------
void CTestStepAdminObjPool::updateGroup( CObjPoolTreeEntry* i_pTreeEntry, EEnabled i_enabled )
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    if( i_pTreeEntry != nullptr )
    {
        m_iUpdateGroupCallDepth++;

        CObjPoolTreeEntry* pTreeEntryChild;
        CTestStep*         pTestStep;
        int                iRow;

        for( iRow = 0; iRow < static_cast<int>(i_pTreeEntry->getChildCount()); iRow++ )
        {
            pTreeEntryChild = i_pTreeEntry->getChildEntry(iRow);

            if( pTreeEntryChild->getEntryType() == EObjPoolEntryTypeObject )
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
public: // overridables of base class CModelObjPool
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStepAdminObjPool::clear()
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
        CObjPoolTreeEntry* pTreeEntryChild;
        CTestStepAdminObj* pTSAdmObj;
        CTestStepGroup*    pTSGrp;
        CTestStep*         pTestStep;

        for( iRow = m_pTreeObjs->getChildCount()-1; iRow >= 0; iRow-- )
        {
            pTreeEntryChild = m_pTreeObjs->getChildEntry(iRow);
            pTSAdmObj = reinterpret_cast<CTestStepAdminObj*>(pTreeEntryChild->getObj());

            if( pTreeEntryChild->getEntryType() != EObjPoolEntryTypeObject )
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

    m_pTreeObjs->setNodeName(m_strObjPoolName);

} // clear

/*==============================================================================
public: // overridables of base class CModelObjPool
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStepAdminObjPool::reset()
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    CObjPoolTreeEntry* pTreeEntryChild;
    CTestStepAdminObj* pTSAdmObj;
    int                iRow;

    for( iRow = 0; iRow < static_cast<int>(m_pTreeObjs->getChildCount()); iRow++ )
    {
        pTreeEntryChild = m_pTreeObjs->getChildEntry(iRow);

        pTSAdmObj = reinterpret_cast<CTestStepAdminObj*>(pTreeEntryChild->getObj());

        if( pTSAdmObj != nullptr )
        {
            pTSAdmObj->reset();
        }

        if( pTreeEntryChild->getEntryType() != EObjPoolEntryTypeObject )
        {
            reset(pTreeEntryChild);
        }
    } // for( iRow = 0; iRow < m_pTreeObjs->getChildCount(); iRow++ )

} // reset

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStepAdminObjPool::reset( CObjPoolTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    // But this is a protected method called by the public reset method
    // and the mutex has already been locked.

    CObjPoolTreeEntry* pTreeEntryChild;
    CTestStepAdminObj* pTSAdmObj;
    int                iRow;

    for( iRow = 0; iRow < static_cast<int>(i_pTreeEntry->getChildCount()); iRow++ )
    {
        pTreeEntryChild = i_pTreeEntry->getChildEntry(iRow);

        pTSAdmObj = reinterpret_cast<CTestStepAdminObj*>(pTreeEntryChild->getObj());

        if( pTSAdmObj != nullptr )
        {
            pTSAdmObj->reset();
        }

        if( pTreeEntryChild->getEntryType() != EObjPoolEntryTypeObject )
        {
            reset(pTreeEntryChild);
        }

    } // for( iRow = 0; iRow < m_pTreeObjs->getChildCount(); iRow++ )

} // reset

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStepAdminObjPool::beginInitTest()
//------------------------------------------------------------------------------
{
    m_bInitializingTest = true;
}

//------------------------------------------------------------------------------
void CTestStepAdminObjPool::endInitTest()
//------------------------------------------------------------------------------
{
    m_bInitializingTest = false;
}

//------------------------------------------------------------------------------
void CTestStepAdminObjPool::testStarted()
//------------------------------------------------------------------------------
{
    m_iTestStepsExecutedCount = 0;
    m_iTestStepsPassedCount = 0;
    m_iTestStepsFailedCount = 0;
    m_iTestStepsIgnoredCount = 0;

    m_pTSGrpRoot->testStarted();

} // testStarted

//------------------------------------------------------------------------------
void CTestStepAdminObjPool::testEnded()
//------------------------------------------------------------------------------
{
    m_pTSGrpRoot->testEnded();
}

//------------------------------------------------------------------------------
void CTestStepAdminObjPool::testAborted()
//------------------------------------------------------------------------------
{
    m_pTSGrpRoot->testEnded();
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CTestStepAdminObjPool::deserialize( QXmlStreamReader* i_pxmlStreamReader )
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
SErrResultInfo CTestStepAdminObjPool::serialize(
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
protected: // overridables of base class CModelObjPool
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStepAdminObjPool::clearTreeEntry( CObjPoolTreeEntry* i_pTreeEntry )
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
        CObjPoolTreeEntry* pTreeEntryChild;
        CTestStepAdminObj* pTSAdmObj;
        CTestStepGroup*    pTSGrp;
        CTestStep*         pTestStep;

        // Starting from end of list to first entry so there is
        // no need to correct the row id's of the childs ..
        for( iRow = static_cast<int>(i_pTreeEntry->getChildCount()-1); iRow >= 0; iRow-- )
        {
            pTreeEntryChild = i_pTreeEntry->getChildEntry(iRow);

            if( pTreeEntryChild != nullptr && pTreeEntryChild->getEntryType() != EObjPoolEntryTypeObject )
            {
                clearTreeEntry(pTreeEntryChild);
            }

            //beginRemoveRows( i_pTreeEntry->getModelIdx(), iRow, iRow );
            i_pTreeEntry->removeChildEntry(iRow);
            //endRemoveRows();

            pTSAdmObj = reinterpret_cast<CTestStepAdminObj*>(pTreeEntryChild->getObj());

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
