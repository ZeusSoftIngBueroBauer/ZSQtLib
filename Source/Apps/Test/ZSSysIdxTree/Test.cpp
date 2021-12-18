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

#include <QtCore/qtimer.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qmimedata.h>
#include <QtGui/qevent.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qlineedit.h>
#else
#include <QtWidgets/qlineedit.h>
#endif

#include "Test.h"
#include "App.h"
#include "MainWindow.h"

#include "ZSTest/ZSTestStepIdxTreeEntry.h"
#include "ZSTest/ZSTestStepIdxTree.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSSysGUI/ZSSysGUIAux.h"
#include "ZSSysGUI/ZSSysIdxTreeModel.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSSysGUI/ZSSysIdxTreeView.h"
#include "ZSSysGUI/ZSSysIdxTreeWdgt.h"
#include "ZSSysGUI/ZSSysIdxTreeModelBranchContent.h"
#include "ZSSysGUI/ZSSysIdxTreeTableViewBranchContent.h"
#include "ZSSys/ZSSysTrcServer.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Trace;
using namespace ZS::Apps::Test::IdxTree;


namespace ZS
{
namespace Apps
{
namespace Test
{
namespace IdxTree
{
//******************************************************************************
class CTrcAdmObjRoot : public ZS::System::CIdxTreeEntry
//******************************************************************************
{
//=============================================================================
public: // ctors and dtor
//=============================================================================

//-----------------------------------------------------------------------------
CTrcAdmObjRoot() :
//-----------------------------------------------------------------------------
    CIdxTreeEntry(EIdxTreeEntryType::Root, CTest::c_strTrcAdmObjIdxTreeName)
{
} // ctor

//-----------------------------------------------------------------------------
virtual ~CTrcAdmObjRoot()
//-----------------------------------------------------------------------------
{
} // dtor

}; // class CTrcAdmObjRoot


//******************************************************************************
class CTrcAdmObjBranch : public ZS::System::CIdxTreeEntry
//******************************************************************************
{
//=============================================================================
public: // ctors and dtor
//=============================================================================

//-----------------------------------------------------------------------------
CTrcAdmObjBranch( const QString& i_strName ) :
//-----------------------------------------------------------------------------
    CIdxTreeEntry(EIdxTreeEntryType::Branch, i_strName)
{
} // ctor

//-----------------------------------------------------------------------------
virtual ~CTrcAdmObjBranch()
//-----------------------------------------------------------------------------
{
} // dtor

}; // class CTrcAdmObjBranch


//******************************************************************************
class CTrcAdmObj : public ZS::System::CIdxTreeEntry
//******************************************************************************
{
//=============================================================================
public: // ctors and dtor
//=============================================================================

//-----------------------------------------------------------------------------
CTrcAdmObj( const QString& i_strName ) :
//-----------------------------------------------------------------------------
    CIdxTreeEntry(EIdxTreeEntryType::Leave, i_strName)
{
} // ctor

//-----------------------------------------------------------------------------
virtual ~CTrcAdmObj()
//-----------------------------------------------------------------------------
{
} // dtor

};  // class CTrcAdmObj


//******************************************************************************
class CTrcAdmObjIdxTree : public ZS::System::CIdxTree
//******************************************************************************
{
//=============================================================================
public: // ctors and dtor
//=============================================================================

//-----------------------------------------------------------------------------
CTrcAdmObjIdxTree() :
//-----------------------------------------------------------------------------
    CIdxTree(
      /* strObjName       */ CTest::c_strTrcAdmObjIdxTreeName,
      /* pRootTreeEntry   */ new CTrcAdmObjRoot(),
      /* strNodeSeparator */ "::" )
{
} // ctor

//-----------------------------------------------------------------------------
virtual ~CTrcAdmObjIdxTree()
//-----------------------------------------------------------------------------
{
} // dtor

//=============================================================================
protected: // overridables of base class CIdxTree
//=============================================================================

//-----------------------------------------------------------------------------
virtual ZS::System::CIdxTreeEntry* createBranch( const QString& i_strName ) const override
//-----------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strAddTrcInfo = "Name: " + i_strName;
    }

    CMethodTracer mthTracer(
        /* pTrcAdmObj   */ CTrcServer::GetInstance(),
        /* iTraceLevel  */ m_iTrcDetailLevel,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace */ "ZS::Apps::Test::IdxTree",
        /* strClassName */ "CTrcAdmObjIdxTree",
        /* strObjName   */ objectName(),
        /* strMethod    */ "createBranch",
        /* strMthInArgs */ strAddTrcInfo );

    CIdxTreeEntry* pBranch = new CTrcAdmObjBranch(i_strName);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(pBranch == nullptr ? "null" : pBranch->name());
    }

    return pBranch;

} // createBranch

//-----------------------------------------------------------------------------
virtual ZS::System::CIdxTreeEntry* createLeave( const QString& i_strName ) const override
//-----------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strAddTrcInfo = "Name: " + i_strName;
    }

    CMethodTracer mthTracer(
        /* pTrcAdmObj   */ CTrcServer::GetInstance(),
        /* iTraceLevel  */ m_iTrcDetailLevel,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace */ "ZS::Apps::Test::IdxTree",
        /* strClassName */ "CTrcAdmObjIdxTree",
        /* strObjName   */ objectName(),
        /* strMethod    */ "createLeave",
        /* strMthInArgs */ strAddTrcInfo );

    CIdxTreeEntry* pLeave = new ZS::Apps::Test::IdxTree::CTrcAdmObj(i_strName);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(pLeave == nullptr ? "null" : pLeave->name());
    }

    return pLeave;

} // createLeave

}; // class CTrcAdmObjIdxTree


/*******************************************************************************
struct STreeEntryDscr
*******************************************************************************/

/*=============================================================================
public: // ctors and dtor
=============================================================================*/

//-----------------------------------------------------------------------------
STreeEntryDscr::STreeEntryDscr() :
//-----------------------------------------------------------------------------
    m_entryType(EIdxTreeEntryType::Undefined),
    m_strName(),
    m_strKeyInTree(),
    m_idxInTree(-1),
    m_strParentBranchPath(),
    m_strKeyInParentBranch(),
    m_idxInParentBranch(-1)
{
} // ctor

//-----------------------------------------------------------------------------
STreeEntryDscr::STreeEntryDscr( const CIdxTreeEntry* i_pTreeEntry ) :
//-----------------------------------------------------------------------------
    m_entryType(EIdxTreeEntryType::Undefined),
    m_strName(),
    m_strKeyInTree(),
    m_idxInTree(-1),
    m_strParentBranchPath(),
    m_strKeyInParentBranch(),
    m_idxInParentBranch(-1)
{
    if( i_pTreeEntry != nullptr )
    {
        m_entryType            = i_pTreeEntry->entryType();
        m_strName              = i_pTreeEntry->name();
        m_strKeyInTree         = i_pTreeEntry->keyInTree();
        m_idxInTree            = i_pTreeEntry->indexInTree();
        m_strParentBranchPath  = i_pTreeEntry->parentBranchPath();
        m_strKeyInParentBranch = i_pTreeEntry->keyInParentBranch();
        m_idxInParentBranch    = i_pTreeEntry->indexInParentBranch();
    }
} // ctor

//-----------------------------------------------------------------------------
STreeEntryDscr::STreeEntryDscr( const STreeEntryDscr& i_other ) :
//-----------------------------------------------------------------------------
    m_entryType(i_other.m_entryType),
    m_strName(i_other.m_strName),
    m_strKeyInTree(i_other.m_strKeyInTree),
    m_idxInTree(i_other.m_idxInTree),
    m_strParentBranchPath(i_other.m_strParentBranchPath),
    m_strKeyInParentBranch(i_other.m_strKeyInParentBranch),
    m_idxInParentBranch(i_other.m_idxInParentBranch)
{
} // copy ctor

/*=============================================================================
public: // operators
=============================================================================*/

//-----------------------------------------------------------------------------
bool STreeEntryDscr::operator == (const STreeEntryDscr& i_other) const
//-----------------------------------------------------------------------------
{
    bool bEqual = true;

    if (m_entryType != i_other.m_entryType)
    {
        bEqual = false;
    }
    // The index in the tree may be compared as entries will not be moved
    // within the tree and the index must remain the same.
    else if (m_idxInTree != i_other.m_idxInTree)
    {
        bEqual = false;
    }
    else if (m_strName != i_other.m_strName)
    {
        bEqual = false;
    }
    else if (m_strKeyInTree != i_other.m_strKeyInTree)
    {
        bEqual = false;
    }
    else if (m_strParentBranchPath != i_other.m_strParentBranchPath)
    {
        bEqual = false;
    }
    else if (m_strKeyInParentBranch != i_other.m_strKeyInParentBranch)
    {
        bEqual = false;
    }
    // The index in the parent branch may not be compared as on removing
    // entries the index of following siblings will be corrected.
    //else if( m_idxInParentBranch != i_other.m_idxInParentBranch )
    //{
    //}

    return bEqual;

} // operator ==

/*=============================================================================
public: // instance methods
=============================================================================*/

//-----------------------------------------------------------------------------
QString STreeEntryDscr::toString( bool i_bWithKeyInTree, bool i_bWithIdxInParentBranch, bool i_bWithKeyInParentBranch ) const
//-----------------------------------------------------------------------------
{
    QString str;

    str = idxTreeEntryType2Str(m_entryType);
    if( i_bWithKeyInTree ) str += ", " + m_strKeyInTree;
    str += ", " + QString::number(m_idxInTree);
    str += ", " + m_strParentBranchPath;
    str += ", " + m_strName;
    if( i_bWithIdxInParentBranch ) str += ", " + QString::number(m_idxInParentBranch);;
    if( i_bWithKeyInParentBranch ) str += ", " + m_strKeyInParentBranch;

    return str;

} // toString

} // namespace IdxTree

} // namespace Test

} // namespace Apps

} // namespace ZS


/*******************************************************************************
class CTest
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

const QString CTest::c_strTrcAdmObjIdxTreeName = "TrcAdmObjs";

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTest::CTest( const QString& i_strTestStepsFileName ) :
//------------------------------------------------------------------------------
    ZS::Test::CTest(
        /* strName              */ "ZS::System::IdxTree",
        /* strTestStepsFileName */ i_strTestStepsFileName,
        /* iTestStepInterval_ms */ 0 ),
    m_pIdxTree(nullptr)
{
    // Sub-Group: CreateIdxTree
    //=========================

    int idxSubGroup = 0;

    ZS::Test::CTestStepGroup* pGrpCreateIdxTree = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " CreateIdxTree TrcAdmObjs",
        /* pTSGrpParent    */ nullptr );

    int idxStep = 0;

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CreateIdxTree TrcAdmObjs",
        /* strOperation    */ "new CTrcAdmObjIdxTree",
        /* pTSGrpParent    */ pGrpCreateIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepCreateTree(ZS::Test::CTestStep*)) );

    // Sub-Group: ModifyIdxTree
    //=========================

    ZS::Test::CTestStepGroup* pGrpModifyIdxTree = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " ModifyIdxTree TrcAdmObjs",
        /* pTSGrpParent    */ nullptr );

    idxStep = 0;

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Branch A0",
        /* strOperation    */ "IdxTree.add(B:A0)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Leave A0::o4",
        /* strOperation    */ "IdxTree.add(L:A0::o4)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Leave A0::o3",
        /* strOperation    */ "IdxTree.add(L:A0::o3)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Rename Leave A0::o4 -> o9",
        /* strOperation    */ "IdxTree.rename(L:A0::o4, o9)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepRenameEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Leave A0::o1",
        /* strOperation    */ "IdxTree.add(L:A0::o1)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Leave A0::o2",
        /* strOperation    */ "IdxTree.add(L:A0::o2)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Branch A0::B2",
        /* strOperation    */ "IdxTree.add(B:A0::B2)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Leave A0::B2::o1",
        /* strOperation    */ "IdxTree.add(L:A0::B2::o1)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Leave A0::B2::o3",
        /* strOperation    */ "IdxTree.add(L:A0::B2::o3)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Leave A0::B2::o2",
        /* strOperation    */ "IdxTree.add(L:A0::B2::o2)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Branch A4",
        /* strOperation    */ "IdxTree.add(B:A4)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Branch A2",
        /* strOperation    */ "IdxTree.add(B:A2)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Leave A0::B2::C4::o4",
        /* strOperation    */ "IdxTree.add(L:A0::B2::C4::o4)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Leave A0::B2::C4::o2",
        /* strOperation    */ "IdxTree.add(L:A0::B2::C4::o2)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Leave A0::B2::C4::o3",
        /* strOperation    */ "IdxTree.add(L:A0::B2::C4::o3)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Delete Leave A0::B2::C4::o4",
        /* strOperation    */ "delete L:A0::B2::C4::o4",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepRemoveEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Delete Leave A0::B2::C4::o3",
        /* strOperation    */ "delete L:A0::B2::C4::o3",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepRemoveEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Leave A2::B1::o1",
        /* strOperation    */ "IdxTree.add(L:A2::B1::o1)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Leave A2::B1::C1::o1",
        /* strOperation    */ "IdxTree.add(L:A2::B1::C1::o1)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Delete Branch A2",
        /* strOperation    */ "delete B:A2",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepRemoveEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Leave A0::B2::C4::o5",
        /* strOperation    */ "IdxTree.add(L:A0::B2::C4::o5)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Leave A0::B2::C4::o8",
        /* strOperation    */ "IdxTree.add(L:A0::B2::C4::o8)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Leave A0::B2::C4::o7",
        /* strOperation    */ "IdxTree.add(L:A0::B2::C4::o7)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Leave A0::B2::C3::o1",
        /* strOperation    */ "IdxTree.add(L:A0::B2::C3::o1)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Leave A0::B2::C4::D5::o7",
        /* strOperation    */ "IdxTree.add(L:A0::B2::C4::D5::o7)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Leave A0::B2::C4::D5::o8",
        /* strOperation    */ "IdxTree.add(L:A0::B2::C4::D5::o8)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Leave A0::B2::C4::D5::o5",
        /* strOperation    */ "IdxTree.add(L:A0::B2::C4::D5::o5)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Branch A4::B1",
        /* strOperation    */ "IdxTree.add(B:A4::B1)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Move Leave A0::B2::C4::o2 -> A0::B2::C3",
        /* strOperation    */ "IdxTree.move(L:A0::B2::C4::o2, B:A0::B2::C3)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepMoveEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Move Branch A0::B2::C4 -> A4::B1",
        /* strOperation    */ "IdxTree.move(B:A0::B2::C4, B:A4::B1)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepMoveEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Rename Leave A4::B1::C4::o5 -> o9",
        /* strOperation    */ "IdxTree.rename(L:A4::B1::C4::o5, o9)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepRenameEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Rename Branch A4::B1 -> B9",
        /* strOperation    */ "IdxTree.rename(B:A4::B1, B9)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepRenameEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Leave A3::B2::C2::o5",
        /* strOperation    */ "IdxTree.add(L:A3::B2::C2::o5)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Leave A3::B1::C2::o5",
        /* strOperation    */ "IdxTree.add(L:A3::B1::C2::o5)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Copy Branch A0::B2::C3 -> A0::B2",
        /* strOperation    */ "IdxTree.copy(B:A0::B2::C3, B:A0::B2)",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepCopyEntry(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Delete Branch A0::B2::C3 Copy",
        /* strOperation    */ "delete B:A0::B2::C3 Copy",
        /* pTSGrpParent    */ pGrpModifyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepRemoveEntry(ZS::Test::CTestStep*)) );

    // Sub-Group: ChangeViews
    //=========================

    ZS::Test::CTestStepGroup* pGrpChangeViews = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " ChangeViews TrcAdmObjs",
        /* pTSGrpParent    */ nullptr );

    idxStep = 0;

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " WidgetIdxTree expandAll",
        /* strOperation    */ "WidgetIdxTree.expandAll",
        /* pTSGrpParent    */ pGrpChangeViews,
        /* szDoTestStepFct */ SLOT(doTestStepChangeViews(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " WidgetIdxTree resizeColumnsToContents",
        /* strOperation    */ "WidgetIdxTree.resizeColumnsToContents",
        /* pTSGrpParent    */ pGrpChangeViews,
        /* szDoTestStepFct */ SLOT(doTestStepChangeViews(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SortOrder Ascending",
        /* strOperation    */ "WidgetIdxTree.setSortOrder(Ascending)",
        /* pTSGrpParent    */ pGrpChangeViews,
        /* szDoTestStepFct */ SLOT(doTestStepChangeViews(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SortOrder Config",
        /* strOperation    */ "WidgetIdxTree.setSortOrder(Config)",
        /* pTSGrpParent    */ pGrpChangeViews,
        /* szDoTestStepFct */ SLOT(doTestStepChangeViews(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ViewMode NavPanelAndBranchContent",
        /* strOperation    */ "WidgetIdxTree.setViewMode(NavPanelAndBranchContent)",
        /* pTSGrpParent    */ pGrpChangeViews,
        /* szDoTestStepFct */ SLOT(doTestStepChangeViews(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Select B:A0::B2",
        /* strOperation    */ "TreeViewIdxTree.select(B:A0::B2)",
        /* pTSGrpParent    */ pGrpChangeViews,
        /* szDoTestStepFct */ SLOT(doTestStepChangeViews(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SortOrder Ascending",
        /* strOperation    */ "WidgetIdxTree.setSortOrder(Ascending)",
        /* pTSGrpParent    */ pGrpChangeViews,
        /* szDoTestStepFct */ SLOT(doTestStepChangeViews(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SortOrder Config",
        /* strOperation    */ "WidgetIdxTree.setSortOrder(Config)",
        /* pTSGrpParent    */ pGrpChangeViews,
        /* szDoTestStepFct */ SLOT(doTestStepChangeViews(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ViewMode NavPanelOnly",
        /* strOperation    */ "WidgetIdxTree.setViewMode(NavPanelOnly)",
        /* pTSGrpParent    */ pGrpChangeViews,
        /* szDoTestStepFct */ SLOT(doTestStepChangeViews(ZS::Test::CTestStep*)) );

    // Sub-Group: TreeViewKeyboardInputs
    //==================================

    ZS::Test::CTestStepGroup* pGrpTreeViewKeyboardInputs = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " TreeViewKeyboardInputs",
        /* pTSGrpParent    */ nullptr );

    int idxSubGroupTreeViewKeyboardInputs = 0;

    // Sub-Group: Rename L:A0::B2::o2 -> o4
    //-------------------------------------

    ZS::Test::CTestStepGroup* pGrpTreeViewKeyboardInputsRenameLA0B2o2toA0B2o4 = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(idxSubGroup) + "." + QString::number(++idxSubGroupTreeViewKeyboardInputs) + " TreeViewKeyboardInputs.Rename L:A0::B2::o2 -> o4",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputs );

    idxStep = 0;

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Select L:A0::B2::o2",
        /* strOperation    */ "TreeViewIdxTree.select(L:A0::B2::o2)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsRenameLA0B2o2toA0B2o4,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press F2",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press F2)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsRenameLA0B2o2toA0B2o4,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );
    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release F2",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(F2)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsRenameLA0B2o2toA0B2o4,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press o",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press o)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsRenameLA0B2o2toA0B2o4,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );
    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release o",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release o)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsRenameLA0B2o2toA0B2o4,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press 4",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press 4)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsRenameLA0B2o2toA0B2o4,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );
    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release 4",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release 4)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsRenameLA0B2o2toA0B2o4,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Enter)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsRenameLA0B2o2toA0B2o4,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );
    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Enter)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsRenameLA0B2o2toA0B2o4,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Escape",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Escape)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsRenameLA0B2o2toA0B2o4,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );
    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Escape",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Escape)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsRenameLA0B2o2toA0B2o4,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );

    // Sub-Group: Copy L:A0::B2::o4 -> B:A0::B2::C3
    //---------------------------------------------

    ZS::Test::CTestStepGroup* pGrpTreeViewKeyboardInputsCopyLA0B2o4toA0B2C3 = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(idxSubGroup) + "." + QString::number(++idxSubGroupTreeViewKeyboardInputs) + " TreeViewKeyboardInputs.Copy L:A0::B2::o4 -> B:A0::B2::C3",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputs );

    idxStep = 0;

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Up",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Up)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsCopyLA0B2o4toA0B2C3,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );
    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Up",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Up)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsCopyLA0B2o4toA0B2C3,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Ctrl-c",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Ctrl-c)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsCopyLA0B2o4toA0B2C3,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );
    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Ctrl-c",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Ctrl-c)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsCopyLA0B2o4toA0B2C3,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Down",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Down)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsCopyLA0B2o4toA0B2C3,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );
    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Down",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Down)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsCopyLA0B2o4toA0B2C3,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Ctrl-v",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Ctrl-v)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsCopyLA0B2o4toA0B2C3,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );
    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Ctrl-v",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Ctrl-v)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsCopyLA0B2o4toA0B2C3,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );

    // Sub-Group: Move L:A0::B2::o3 -> B:A0::B2::C3
    //---------------------------------------------

    ZS::Test::CTestStepGroup* pGrpTreeViewKeyboardInputsMoveLA0B2o3toA0B2C3 = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(idxSubGroup) + "." + QString::number(++idxSubGroupTreeViewKeyboardInputs) + " TreeViewKeyboardInputs.Move L:A0::B2::o3 -> B:A0::B2::C3",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputs );

    idxStep = 0;

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Up",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Up)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsMoveLA0B2o3toA0B2C3,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );
    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Up",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Up)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsMoveLA0B2o3toA0B2C3,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Up",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Up)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsMoveLA0B2o3toA0B2C3,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );
    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Up",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Up)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsMoveLA0B2o3toA0B2C3,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Ctrl-x",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Ctrl-x)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsMoveLA0B2o3toA0B2C3,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );
    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Ctrl-x",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Ctrl-x)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsMoveLA0B2o3toA0B2C3,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Down",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Down)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsMoveLA0B2o3toA0B2C3,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );
    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Down",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Down)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsMoveLA0B2o3toA0B2C3,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Down",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Down)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsMoveLA0B2o3toA0B2C3,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );
    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Down",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Down)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsMoveLA0B2o3toA0B2C3,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Ctrl-v",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Ctrl-v)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsMoveLA0B2o3toA0B2C3,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );
    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Ctrl-v",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Ctrl-v)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsMoveLA0B2o3toA0B2C3,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );

    // Sub-Group: Delete L:A0::B2::C3::o3
    //-----------------------------------

    ZS::Test::CTestStepGroup* pGrpTreeViewKeyboardInputsDeleteA0B2C3o3 = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(idxSubGroup) + "." + QString::number(++idxSubGroupTreeViewKeyboardInputs) + " TreeViewKeyboardInputs.Delete L:A0::B2::C3::o3",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputs );

    idxStep = 0;

    for( int idxKeyDown = 0; idxKeyDown < 4; ++idxKeyDown )
    {
        new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " Press Down",
            /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Down)",
            /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsDeleteA0B2C3o3,
            /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );
        new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " Release Down",
            /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Down)",
            /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsDeleteA0B2C3o3,
            /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );
    }

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Delete",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Delete)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsDeleteA0B2C3o3,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );
    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Delete",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Delete)",
        /* pTSGrpParent    */ pGrpTreeViewKeyboardInputsDeleteA0B2C3o3,
        /* szDoTestStepFct */ SLOT(doTestStepKeyboardInputs(ZS::Test::CTestStep*)) );

    // Sub-Group: TreeViewLeaveContextMenus
    //=====================================

    ZS::Test::CTestStepGroup* pGrpTreeViewLeaveContextMenu = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " TreeViewLeaveContextMenus",
        /* pTSGrpParent    */ nullptr );

    int idxSubGroupTreeViewLeaveContextMenus = 0;

    // Sub-Group: Copy L:A0::B2::C3::o1 -> o1 Copy
    //--------------------------------------------

    ZS::Test::CTestStepGroup* pGrpTreeViewLeaveContextMenusCopyLA0B2C3o1too1CopyOf = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(idxSubGroup) + "." + QString::number(++idxSubGroupTreeViewLeaveContextMenus) + " TreeViewLeaveContextMenu.Copy L:A0::B2::C3::o1 -> o1 Copy",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenu );

    idxStep = 0;

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Select L:A0::B2::C3::o1",
        /* strOperation    */ "TreeViewIdxTree.select(L:A0::B2::C3::o1)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusCopyLA0B2C3o1too1CopyOf,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Press RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusCopyLA0B2C3o1too1CopyOf,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Release RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusCopyLA0B2C3o1too1CopyOf,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    for( int idxKeyDown = 0; idxKeyDown < 4; ++idxKeyDown )
    {
        new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " Press Down",
            /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Down)",
            /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusCopyLA0B2C3o1too1CopyOf,
            /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
        new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " Release Down",
            /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Down)",
            /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusCopyLA0B2C3o1too1CopyOf,
            /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
    }

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Enter)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusCopyLA0B2C3o1too1CopyOf,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Enter)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusCopyLA0B2C3o1too1CopyOf,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Select B:A0::B2::C3",
        /* strOperation    */ "TreeViewIdxTree.select(B:A0::B2::C3)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusCopyLA0B2C3o1too1CopyOf,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Press RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusCopyLA0B2C3o1too1CopyOf,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Release RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusCopyLA0B2C3o1too1CopyOf,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Up",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Up)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusCopyLA0B2C3o1too1CopyOf,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Up",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Up)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusCopyLA0B2C3o1too1CopyOf,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Enter)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusCopyLA0B2C3o1too1CopyOf,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Enter)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusCopyLA0B2C3o1too1CopyOf,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Find L:A0::B2::C3::o1 Copy",
        /* strOperation    */ "TreeViewIdxTree.findLeave(L:A0::B2::C3::o1 Copy)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusCopyLA0B2C3o1too1CopyOf,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    // Sub-Group: Move L:A0::B2::C3::o1 Copy -> L:A0::B2::o1 Copy
    //-----------------------------------------------------------

    ZS::Test::CTestStepGroup* pGrpTreeViewLeaveContextMenusMoveLA0B2C3o1CopytoBA0B2 = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(idxSubGroup) + "." + QString::number(++idxSubGroupTreeViewLeaveContextMenus) + " TreeViewLeaveContextMenu.Move L:A0::B2::C3::o1 Copy -> B:A0::B2",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenu );

    idxStep = 0;

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Select L:A0::B2::C3::o1 Copy",
        /* strOperation    */ "TreeViewIdxTree.select(L:A0::B2::C3::o1 Copy)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusMoveLA0B2C3o1CopytoBA0B2,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Press RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusMoveLA0B2C3o1CopytoBA0B2,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Release RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusMoveLA0B2C3o1CopytoBA0B2,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    for( int idxKeyDown = 0; idxKeyDown < 3; ++idxKeyDown )
    {
        new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " Press Down",
            /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Down)",
            /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusMoveLA0B2C3o1CopytoBA0B2,
            /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
        new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " Release Down",
            /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Down)",
            /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusMoveLA0B2C3o1CopytoBA0B2,
            /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
    }

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Enter)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusMoveLA0B2C3o1CopytoBA0B2,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Enter)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusMoveLA0B2C3o1CopytoBA0B2,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Select B:A0::B2",
        /* strOperation    */ "TreeViewIdxTree.select(B:A0::B2)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusMoveLA0B2C3o1CopytoBA0B2,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Press RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusMoveLA0B2C3o1CopytoBA0B2,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Release RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusMoveLA0B2C3o1CopytoBA0B2,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Up",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Up)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusMoveLA0B2C3o1CopytoBA0B2,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Up",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Up)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusMoveLA0B2C3o1CopytoBA0B2,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Enter)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusMoveLA0B2C3o1CopytoBA0B2,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Enter)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusMoveLA0B2C3o1CopytoBA0B2,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Find L:A0::B2::o1 Copy",
        /* strOperation    */ "TreeViewIdxTree.findLeave(L:A0::B2::o1 Copy)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusMoveLA0B2C3o1CopytoBA0B2,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    // Sub-Group: Delete L:A0::B2::o1 Copy
    //------------------------------------

    ZS::Test::CTestStepGroup* pGrpTreeViewLeaveContextMenusDeleteLA0B2o1Copy = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(idxSubGroup) + "." + QString::number(++idxSubGroupTreeViewLeaveContextMenus) + " TreeViewLeaveContextMenu.Delete L:A0::B2::o1 Copy",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenu );

    idxStep = 0;

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Select L:A0::B2::o1 Copy",
        /* strOperation    */ "TreeViewIdxTree.select(L:A0::B2::o1 Copy)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusDeleteLA0B2o1Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Press RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusDeleteLA0B2o1Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Release RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusDeleteLA0B2o1Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    for( int idxKeyDown = 0; idxKeyDown < 2; ++idxKeyDown )
    {
        new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " Press Down",
            /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Down)",
            /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusDeleteLA0B2o1Copy,
            /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
        new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " Release Down",
            /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Down)",
            /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusDeleteLA0B2o1Copy,
            /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
    }

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Enter)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusDeleteLA0B2o1Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Enter)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusDeleteLA0B2o1Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Find L:A0::B2::o1 Copy",
        /* strOperation    */ "TreeViewIdxTree.findLeave(L:A0::B2::o1 Copy)",
        /* pTSGrpParent    */ pGrpTreeViewLeaveContextMenusDeleteLA0B2o1Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    // Sub-Group: TreeViewBranchContextMenus
    //======================================

    ZS::Test::CTestStepGroup* pGrpTreeViewBranchContextMenu = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " TreeViewBranchContextMenus",
        /* pTSGrpParent    */ nullptr );

    int idxSubGroupTreeViewBranchContextMenus = 0;

    // Sub-Group: Create Branch B:A0::B2::C3::New Branch
    //--------------------------------------------------

    ZS::Test::CTestStepGroup* pGrpTreeViewBranchContextMenusCreateBranchBA0B2C3NewBranch = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(idxSubGroup) + "." + QString::number(++idxSubGroupTreeViewBranchContextMenus) + " TreeViewBranchContextMenu.CreateBranch B:A0::B2::C3::New Branch",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenu );

    idxStep = 0;

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Select B:A0::B2::C3",
        /* strOperation    */ "TreeViewIdxTree.select(B:A0::B2::C3)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCreateBranchBA0B2C3NewBranch,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Press RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCreateBranchBA0B2C3NewBranch,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Release RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCreateBranchBA0B2C3NewBranch,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    for( int idxKeyDown = 0; idxKeyDown < 4; ++idxKeyDown )
    {
        new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " Press Down",
            /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Down)",
            /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCreateBranchBA0B2C3NewBranch,
            /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
        new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " Release Down",
            /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Down)",
            /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCreateBranchBA0B2C3NewBranch,
            /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
    }

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Enter)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCreateBranchBA0B2C3NewBranch,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Enter)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCreateBranchBA0B2C3NewBranch,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Find B:A0::B2::C3::New Branch",
        /* strOperation    */ "TreeViewIdxTree.findBranch(B:A0::B2::C3::New Branch)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCreateBranchBA0B2C3NewBranch,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " WidgetIdxTree resizeColumnsToContents",
        /* strOperation    */ "WidgetIdxTree.resizeColumnsToContents",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCreateBranchBA0B2C3NewBranch,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    // Sub-Group: Create Leave B:A0::B2::C3::New Branch::New Leaves
    //-------------------------------------------------------------

    ZS::Test::CTestStepGroup* pGrpTreeViewBranchContextMenusCreateLeavesBA0B2C3NewBranchNewLeaves = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(idxSubGroup) + "." + QString::number(++idxSubGroupTreeViewBranchContextMenus) + " TreeViewBranchContextMenu.CreateLeaves B:A0::B2::C3::New Branch::New Leaves",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenu );

    idxStep = 0;

    for( int idxNewLeave = 0; idxNewLeave < 2; ++idxNewLeave )
    {
        new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " Select B:A0::B2::C3::New Branch",
            /* strOperation    */ "TreeViewIdxTree.select(B:A0::B2::C3::New Branch)",
            /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCreateLeavesBA0B2C3NewBranchNewLeaves,
            /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

        new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " Press RightButton",
            /* strOperation    */ "TreeViewIdxTree.mouseEvent(Press RightButton)",
            /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCreateLeavesBA0B2C3NewBranchNewLeaves,
            /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
        new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " Release RightButton",
            /* strOperation    */ "TreeViewIdxTree.mouseEvent(Release RightButton)",
            /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCreateLeavesBA0B2C3NewBranchNewLeaves,
            /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

        for( int idxKeyDown = 0; idxKeyDown < 5; ++idxKeyDown )
        {
            new ZS::Test::CTestStep(
                /* pTest           */ this,
                /* strName         */ "Step " + QString::number(++idxStep) + " Press Down",
                /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Down)",
                /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCreateLeavesBA0B2C3NewBranchNewLeaves,
                /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
            new ZS::Test::CTestStep(
                /* pTest           */ this,
                /* strName         */ "Step " + QString::number(++idxStep) + " Release Down",
                /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Down)",
                /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCreateLeavesBA0B2C3NewBranchNewLeaves,
                /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
        }

        new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " Press Enter",
            /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Enter)",
            /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCreateLeavesBA0B2C3NewBranchNewLeaves,
            /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
        new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " Release Enter",
            /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Enter)",
            /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCreateLeavesBA0B2C3NewBranchNewLeaves,
            /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

        } // for( int idxNewLeave = 0; idxNewLeave < 2; ++idxNewLeave )

    // Expand Branch "B:A0::B2::C3::New Branch"
    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Select B:A0::B2::C3::New Branch",
        /* strOperation    */ "TreeViewIdxTree.select(B:A0::B2::C3::New Branch)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCreateLeavesBA0B2C3NewBranchNewLeaves,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Press RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCreateLeavesBA0B2C3NewBranchNewLeaves,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Release RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCreateLeavesBA0B2C3NewBranchNewLeaves,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    for( int idxKeyDown = 0; idxKeyDown < 2; ++idxKeyDown )
    {
        new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " Press Down",
            /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Down)",
            /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCreateLeavesBA0B2C3NewBranchNewLeaves,
            /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
        new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " Release Down",
            /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Down)",
            /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCreateLeavesBA0B2C3NewBranchNewLeaves,
            /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
    }

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Enter)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCreateLeavesBA0B2C3NewBranchNewLeaves,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Enter)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCreateLeavesBA0B2C3NewBranchNewLeaves,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " WidgetIdxTree resizeColumnsToContents",
        /* strOperation    */ "WidgetIdxTree.resizeColumnsToContents",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCreateLeavesBA0B2C3NewBranchNewLeaves,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Find L:A0::B2::C3::New Branch::New Leave",
        /* strOperation    */ "TreeViewIdxTree.findLeave(L:A0::B2::C3::New Branch::New Leave)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCreateLeavesBA0B2C3NewBranchNewLeaves,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Find L:A0::B2::C3::New Branch::New Leave 2",
        /* strOperation    */ "TreeViewIdxTree.findLeave(L:A0::B2::C3::New Branch::New Leave2)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCreateLeavesBA0B2C3NewBranchNewLeaves,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    // Sub-Group: Delete B:A0::B2::C3::New Branch
    //-------------------------------------------

    ZS::Test::CTestStepGroup* pGrpTreeViewBranchContextMenusDeleteBA0B2C3NewBranch = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(idxSubGroup) + "." + QString::number(++idxSubGroupTreeViewBranchContextMenus) + " TreeViewBranchContextMenu.Delete B:A0::B2::C3::New Branch",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenu );

    idxStep = 0;

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Select B:A0::B2::C3::New Branch",
        /* strOperation    */ "TreeViewIdxTree.select(B:A0::B2::C3::New Branch)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusDeleteBA0B2C3NewBranch,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Press RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusDeleteBA0B2C3NewBranch,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Release RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusDeleteBA0B2C3NewBranch,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    for( int idxKeyDown = 0; idxKeyDown < 6; ++idxKeyDown )
    {
        new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " Press Down",
            /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Down)",
            /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusDeleteBA0B2C3NewBranch,
            /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
        new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " Release Down",
            /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Down)",
            /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusDeleteBA0B2C3NewBranch,
            /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
    }

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Enter)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusDeleteBA0B2C3NewBranch,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Enter)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusDeleteBA0B2C3NewBranch,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Find B:A0::B2::C3::New Branch",
        /* strOperation    */ "TreeViewIdxTree.findBranch(B:A0::B2::C3::New Branch)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusDeleteBA0B2C3NewBranch,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    // Sub-Group: Copy B:A0::B2::C3 -> B:A0::B2::C3 Copy
    //--------------------------------------------------

    ZS::Test::CTestStepGroup* pGrpTreeViewBranchContextMenusCopyBA0B2C3toBA0B2C3Copy = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(idxSubGroup) + "." + QString::number(++idxSubGroupTreeViewLeaveContextMenus) + " TreeViewBranchContextMenu.Copy B:A0::B2::C3 -> B:A0::B2::C3 Copy",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenu );

    idxStep = 0;

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Select B:A0::B2::C3",
        /* strOperation    */ "TreeViewIdxTree.select(B:A0::B2::C3)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCopyBA0B2C3toBA0B2C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Press RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCopyBA0B2C3toBA0B2C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Release RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCopyBA0B2C3toBA0B2C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    for( int idxKeyDown = 0; idxKeyDown < 8; ++idxKeyDown )
    {
        new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " Press Down",
            /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Down)",
            /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCopyBA0B2C3toBA0B2C3Copy,
            /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
        new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " Release Down",
            /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Down)",
            /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCopyBA0B2C3toBA0B2C3Copy,
            /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
    }

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Enter)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCopyBA0B2C3toBA0B2C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Enter)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCopyBA0B2C3toBA0B2C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Select B:A0::B2::C3",
        /* strOperation    */ "TreeViewIdxTree.select(B:A0::B2::C3)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCopyBA0B2C3toBA0B2C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Press RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCopyBA0B2C3toBA0B2C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Release RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCopyBA0B2C3toBA0B2C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Select B:A0::B2",
        /* strOperation    */ "TreeViewIdxTree.select(B:A0::B2)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCopyBA0B2C3toBA0B2C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Press RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCopyBA0B2C3toBA0B2C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Release RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCopyBA0B2C3toBA0B2C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Up",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Up)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCopyBA0B2C3toBA0B2C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Up",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Up)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCopyBA0B2C3toBA0B2C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Enter)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCopyBA0B2C3toBA0B2C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Enter)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCopyBA0B2C3toBA0B2C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Find L:A0::B2::C3::o1",
        /* strOperation    */ "TreeViewIdxTree.findLeave(L:A0::B2::C3::o1)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCopyBA0B2C3toBA0B2C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Find L:A0::B2::C3::o2",
        /* strOperation    */ "TreeViewIdxTree.findLeave(L:A0::B2::C3::o4)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCopyBA0B2C3toBA0B2C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Find L:A0::B2::C3::o4",
        /* strOperation    */ "TreeViewIdxTree.findLeave(L:A0::B2::C3::o4)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCopyBA0B2C3toBA0B2C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Find B:A0::B2::C3 Copy",
        /* strOperation    */ "TreeViewIdxTree.findBranch(B:A0::B2::C3 Copy)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCopyBA0B2C3toBA0B2C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Find L:A0::B2::C3 Copy::o1",
        /* strOperation    */ "TreeViewIdxTree.findLeave(L:A0::B2::C3 Copy::o1)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCopyBA0B2C3toBA0B2C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Find L:A0::B2::C3 Copy::o2",
        /* strOperation    */ "TreeViewIdxTree.findLeave(L:A0::B2::C3 Copy::o4)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCopyBA0B2C3toBA0B2C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Find L:A0::B2::C3 Copy::o4",
        /* strOperation    */ "TreeViewIdxTree.findLeave(L:A0::B2::C3 Copy::o4)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusCopyBA0B2C3toBA0B2C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    // Sub-Group: Move B:A0::B2::C3 Copy -> B:A0::B2::C3::C3 Copy
    //-----------------------------------------------------------

    ZS::Test::CTestStepGroup* pGrpTreeViewBranchContextMenusMoveBA0B2C3CopytoBA0B2C3C3Copy = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(idxSubGroup) + "." + QString::number(++idxSubGroupTreeViewLeaveContextMenus) + " TreeViewBranchContextMenu.Move B:A0::B2::C3 Copy -> B:A0::B2::C3::C3 Copy",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenu );

    idxStep = 0;

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Select B:A0::B2::C3 Copy",
        /* strOperation    */ "TreeViewIdxTree.select(B:A0::B2::C3 Copy)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusMoveBA0B2C3CopytoBA0B2C3C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Press RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusMoveBA0B2C3CopytoBA0B2C3C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Release RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusMoveBA0B2C3CopytoBA0B2C3C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    for( int idxKeyDown = 0; idxKeyDown < 3; ++idxKeyDown )
    {
        new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " Press Up",
            /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Up)",
            /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusMoveBA0B2C3CopytoBA0B2C3C3Copy,
            /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
        new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " Release Up",
            /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Up)",
            /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusMoveBA0B2C3CopytoBA0B2C3C3Copy,
            /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
    }

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Enter)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusMoveBA0B2C3CopytoBA0B2C3C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Enter)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusMoveBA0B2C3CopytoBA0B2C3C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Select B:A0::B2::C3",
        /* strOperation    */ "TreeViewIdxTree.select(B:A0::B2::C3)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusMoveBA0B2C3CopytoBA0B2C3C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Press RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusMoveBA0B2C3CopytoBA0B2C3C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Release RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusMoveBA0B2C3CopytoBA0B2C3C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Up",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Up)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusMoveBA0B2C3CopytoBA0B2C3C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Up",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Up)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusMoveBA0B2C3CopytoBA0B2C3C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Enter)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusMoveBA0B2C3CopytoBA0B2C3C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Enter)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusMoveBA0B2C3CopytoBA0B2C3C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Find B:A0::B2::C3::C3 Copy",
        /* strOperation    */ "TreeViewIdxTree.findBranch(B:A0::B2::C3::C3 Copy)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusMoveBA0B2C3CopytoBA0B2C3C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Find L:A0::B2::C3::C3 Copy::o1",
        /* strOperation    */ "TreeViewIdxTree.findLeave(L:A0::B2::C3::C3 Copy::o1)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusMoveBA0B2C3CopytoBA0B2C3C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Find L:A0::B2::C3::C3 Copy::o2",
        /* strOperation    */ "TreeViewIdxTree.findLeave(L:A0::B2::C3::C3 Copy::o4)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusMoveBA0B2C3CopytoBA0B2C3C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Find L:A0::B2::C3::C3 Copy::o4",
        /* strOperation    */ "TreeViewIdxTree.findLeave(L:A0::B2::C3::C3 Copy::o4)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusMoveBA0B2C3CopytoBA0B2C3C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    // Sub-Group: Delete B:A0::B2::C3::C3 Copy
    //----------------------------------------

    ZS::Test::CTestStepGroup* pGrpTreeViewBranchContextMenusDeleteBA0B2C3C3Copy = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(idxSubGroup) + "." + QString::number(++idxSubGroupTreeViewLeaveContextMenus) + " TreeViewBranchContextMenu.Delete B:A0::B2::C3::C3 Copy",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenu );

    idxStep = 0;

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Select L:A0::B2::C3::C3 Copy",
        /* strOperation    */ "TreeViewIdxTree.select(B:A0::B2::C3::C3 Copy)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusDeleteBA0B2C3C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Press RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusDeleteBA0B2C3C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release RightButton",
        /* strOperation    */ "TreeViewIdxTree.mouseEvent(Release RightButton)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusDeleteBA0B2C3C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    for( int idxKeyDown = 0; idxKeyDown < 6; ++idxKeyDown )
    {
        new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " Press Down",
            /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Down)",
            /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusDeleteBA0B2C3C3Copy,
            /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
        new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " Release Down",
            /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Down)",
            /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusDeleteBA0B2C3C3Copy,
            /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
    }

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Press Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Press Enter)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusDeleteBA0B2C3C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );
    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release Enter",
        /* strOperation    */ "TreeViewIdxTree.keyEvent(Release Enter)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusDeleteBA0B2C3C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Find B:A0::B2::C3::C3 Copy",
        /* strOperation    */ "TreeViewIdxTree.findBranch(B:A0::B2::C3::C3 Copy)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusDeleteBA0B2C3C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Find L:A0::B2::C3::C3 Copy::o1",
        /* strOperation    */ "TreeViewIdxTree.findLeave(L:A0::B2::C3::C3 Copy::o1)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusDeleteBA0B2C3C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Find L:A0::B2::C3::C3 Copy::o2",
        /* strOperation    */ "TreeViewIdxTree.findLeave(L:A0::B2::C3::C3 Copy::o4)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusDeleteBA0B2C3C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Find L:A0::B2::C3::C3 Copy::o4",
        /* strOperation    */ "TreeViewIdxTree.findLeave(L:A0::B2::C3::C3 Copy::o4)",
        /* pTSGrpParent    */ pGrpTreeViewBranchContextMenusDeleteBA0B2C3C3Copy,
        /* szDoTestStepFct */ SLOT(doTestStepTreeViewContextMenus(ZS::Test::CTestStep*)) );

    // Sub-Group: TreeViewDragAndDrop
    //===============================

    ZS::Test::CTestStepGroup* pGrpTreeViewDragAndDrop = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " TreeViewDragAndDrop",
        /* pTSGrpParent    */ nullptr );

    int idxSubGroupTreeViewDragAndDrop = 0;

    // Sub-Group: Move Leave L:A4::B9::C4::D5::o5 -> B:A4::B9::C4
    //-----------------------------------------------------------

    QString strKeyInTreeSrc  = "L:A4::B9::C4::D5::o5";
    QString strKeyInTreeTrg  = "B:A4::B9::C4";
    QString strKeyInTreeFind = "L:A4::B9::C4::o5";

    ZS::Test::CTestStepGroup* pGrpTreeViewDragAndDropMoveLA4B9C4D5o5toBA4B9C4 = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(idxSubGroup) + "." + QString::number(++idxSubGroupTreeViewDragAndDrop) + " TreeViewDragAndDrop.Move " + strKeyInTreeSrc + " -> " + strKeyInTreeTrg,
        /* pTSGrpParent    */ pGrpTreeViewDragAndDrop );

    idxStep = 0;

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " WidgetIdxTree expandAll",
        /* strOperation    */ "WidgetIdxTree.expandAll",
        /* pTSGrpParent    */ pGrpTreeViewDragAndDropMoveLA4B9C4D5o5toBA4B9C4,
        /* szDoTestStepFct */ SLOT(doTestStepGrpTrcAdmObjTreeStepTreeViewDragAndDrop(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " WidgetIdxTree resizeColumnsToContents",
        /* strOperation    */ "WidgetIdxTree.resizeColumnsToContents",
        /* pTSGrpParent    */ pGrpTreeViewDragAndDropMoveLA4B9C4D5o5toBA4B9C4,
        /* szDoTestStepFct */ SLOT(doTestStepGrpTrcAdmObjTreeStepTreeViewDragAndDrop(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Select L:A4::B9::C4::D5::o5",
        /* strOperation    */ "TreeViewIdxTree.select(L:A4::B9::C4::D5::o5)",
        /* pTSGrpParent    */ pGrpTreeViewDragAndDropMoveLA4B9C4D5o5toBA4B9C4,
        /* szDoTestStepFct */ SLOT(doTestStepGrpTrcAdmObjTreeStepTreeViewDragAndDrop(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " dragEnterEvent",
        /* strOperation    */ "TreeViewIdxTree.dragEnterEvent()",
        /* pTSGrpParent    */ pGrpTreeViewDragAndDropMoveLA4B9C4D5o5toBA4B9C4,
        /* szDoTestStepFct */ SLOT(doTestStepGrpTrcAdmObjTreeStepTreeViewDragAndDrop(ZS::Test::CTestStep*)) );

    int iMouseMoveCount = 5;

    for( int idxMouseMove = 0; idxMouseMove < iMouseMoveCount; ++idxMouseMove )
    {
        new ZS::Test::CTestStep(
            /* pTest           */ this,
            /* strName         */ "Step " + QString::number(++idxStep) + " dragMoveEvent Up " + QString::number(100/iMouseMoveCount) + "%",
            /* strOperation    */ "TreeViewIdxTree.dragMoveEvent(Up " + QString::number(100/iMouseMoveCount) + "%)",
            /* pTSGrpParent    */ pGrpTreeViewDragAndDropMoveLA4B9C4D5o5toBA4B9C4,
            /* szDoTestStepFct */ SLOT(doTestStepGrpTrcAdmObjTreeStepTreeViewDragAndDrop(ZS::Test::CTestStep*)) );
    }

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " dropEvent",
        /* strOperation    */ "TreeViewIdxTree.dropEvent()",
        /* pTSGrpParent    */ pGrpTreeViewDragAndDropMoveLA4B9C4D5o5toBA4B9C4,
        /* szDoTestStepFct */ SLOT(doTestStepGrpTrcAdmObjTreeStepTreeViewDragAndDrop(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " dragLeaveEvent",
        /* strOperation    */ "TreeViewIdxTree.dragLeaveEvent()",
        /* pTSGrpParent    */ pGrpTreeViewDragAndDropMoveLA4B9C4D5o5toBA4B9C4,
        /* szDoTestStepFct */ SLOT(doTestStepGrpTrcAdmObjTreeStepTreeViewDragAndDrop(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Find " + strKeyInTreeFind,
        /* strOperation    */ "TreeViewIdxTree.findLeave(" + strKeyInTreeFind + ")",
        /* pTSGrpParent    */ pGrpTreeViewDragAndDropMoveLA4B9C4D5o5toBA4B9C4,
        /* szDoTestStepFct */ SLOT(doTestStepGrpTrcAdmObjTreeStepTreeViewDragAndDrop(ZS::Test::CTestStep*)) );

    // Sub-Group: DestroyIdxTree
    //==========================

    ZS::Test::CTestStepGroup* pGrpDestroyIdxTree = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " DestroyIdxTree TrcAdmObjs",
        /* pTSGrpParent    */ nullptr );

    idxStep = 0;

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " DestroyIdxTree TrcAdmObjs",
        /* strOperation    */ "delete TrcAdmObjs",
        /* pTSGrpParent    */ pGrpDestroyIdxTree,
        /* szDoTestStepFct */ SLOT(doTestStepDeleteTree(ZS::Test::CTestStep*)) );


    // Recall test step settings
    //--------------------------

    QFileInfo fileInfo(i_strTestStepsFileName);

    if( fileInfo.exists() )
    {
        SErrResultInfo errResultInfo = recall(i_strTestStepsFileName);

        if(errResultInfo.isErrorResult())
        {
            if(CErrLog::GetInstance() != nullptr)
            {
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
        }
    }

} // default ctor

//------------------------------------------------------------------------------
CTest::~CTest()
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo = save();

    if(errResultInfo.isErrorResult())
    {
        if(CErrLog::GetInstance() != nullptr)
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

    try
    {
        delete m_pIdxTree;
    }
    catch(...)
    {
    }

    m_pIdxTree = nullptr;

} // dtor

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::onIdxTreeDestroyed( QObject* i_pIdxTree )
//------------------------------------------------------------------------------
{
    if( m_pIdxTree == i_pIdxTree )
    {
        m_pIdxTree = nullptr;
    }

} // onIdxTreeDestroyed

//------------------------------------------------------------------------------
void CTest::doTestStepCreateTree( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strExpectedValue = "IdxTree.root() != nullptr";
    strlstExpectedValues.append(strExpectedValue);

    // Test Step
    //----------

    // If stopped somewhere in the middle the index tree may not have been destroyed yet.
    try
    {
        delete m_pIdxTree;  // slot "onIdxTreeDestroyed(m_pIdxTree)" is called
    }
    catch(...)
    {
    }
    m_pIdxTree = nullptr;

    m_pIdxTree = new CTrcAdmObjIdxTree();

    if( !QObject::connect(
        /* pObjSender   */ m_pIdxTree,
        /* szSignal     */ SIGNAL(destroyed(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onIdxTreeDestroyed(QObject*)) ) )
    {
        throw CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    emit idxTreeAdded(m_pIdxTree);

    // Result Values
    //---------------

    CIdxTreeEntry* pRoot = m_pIdxTree->root();

    // Please note that to finish a test step the list of actual values may not be empty.
    if( pRoot == nullptr )
    {
        strResultValue = "IdxTree.root() == nullptr";
        strlstResultValues.append(strResultValue);
    }
    else
    {
        strResultValue = "IdxTree.root() != nullptr";
        strlstResultValues.append(strResultValue);
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepCreateTree

//------------------------------------------------------------------------------
void CTest::doTestStepAddEntry( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    QString strOperation = i_pTestStep->getOperation();

    // Examples for strOperation:
    // - "IdxTree.add(L:ZS::Apps::Test::Trace::CTestModule1::TestModule1)"
    // - "IdxTree.add(B:A0)"
    // - "IdxTree.add(B:A0:B3)"
    // - "IdxTree.add(L:A0:B3:C4:o1)"

    if( strOperation.startsWith("IdxTree.add",Qt::CaseInsensitive) )
    {
        QString strKeyInTreeToBeAdded;

        strKeyInTreeToBeAdded = strOperation;
        strKeyInTreeToBeAdded.remove("IdxTree.add");
        strKeyInTreeToBeAdded.remove("(");
        strKeyInTreeToBeAdded.remove(")");

        strOperation = "IdxTree.add";

        QStringList strlstPathNodes;
        QStringList strlstKeysDefined;
        QStringList strlstNameSpacesDefined;
        QStringList strlstClassNamesDefined;
        QStringList strlstObjNamesDefined;

        splitKey(*m_pIdxTree, strKeyInTreeToBeAdded, strlstPathNodes, strlstKeysDefined, strlstNameSpacesDefined, strlstClassNamesDefined, strlstObjNamesDefined);

        // Get current list of tree entries and map of free indices
        //---------------------------------------------------------

        QVector<CIdxTreeEntry*> arpTreeEntriesExpected = m_pIdxTree->treeEntriesVec();

        QVector<STreeEntryDscr> arTreeEntryDscrsExpected = toTreeEntryDscrs(arpTreeEntriesExpected);

        QMap<int, int> mapFreeIdxsExpected = m_pIdxTree->freeIdxsMap();

        // Expected (desired) behaviour
        //-----------------------------

        QString strKeyInTree;
        QString strNameSpace;
        QString strClassName;
        QString strObjName;

        for( int idxPathNode = 0; idxPathNode < strlstKeysDefined.size(); ++idxPathNode )
        {
            strKeyInTree = strlstKeysDefined[idxPathNode];

            // To create an admin object the NameSpace, ClassName and ObjName must be passed to the ctor.
            strNameSpace = strlstNameSpacesDefined[idxPathNode];
            strClassName = strlstClassNamesDefined[idxPathNode];
            strObjName   = strlstObjNamesDefined[idxPathNode];

            // Adds the node only if the key in tree is not yet contained in the "arTreeEntryDscrsExpected".
            addEntry(*m_pIdxTree, strKeyInTree, strNameSpace, strClassName, strObjName, arTreeEntryDscrsExpected, mapFreeIdxsExpected);

        } // for( int idxPathNode = 0; idxPathNode < strlstKeysDefined.size(); ++idxPathNode )

        // Add entries (if not yet added)
        //-------------------------------

        CIdxTreeEntry* pTreeEntry;

        QString strPath;

        for( int idxPathNode = 0; idxPathNode < strlstKeysDefined.size(); ++idxPathNode )
        {
            strKeyInTree = strlstKeysDefined[idxPathNode];

            pTreeEntry = m_pIdxTree->findEntry(strKeyInTree);

            if( pTreeEntry == nullptr )
            {
                // To create an admin object the NameSpace, ClassName and ObjName must be passed to the ctor.
                strNameSpace = strlstNameSpacesDefined[idxPathNode];
                strClassName = strlstClassNamesDefined[idxPathNode];
                strObjName   = strlstObjNamesDefined[idxPathNode];

                // To add a branch or an AdminObj instance to the index tree the parent path must be known.
                strPath = m_pIdxTree->buildPathStr(strNameSpace, strClassName);

                if( strKeyInTree.startsWith("B:") )
                {
                    pTreeEntry = new CTrcAdmObjBranch(strObjName);
                    m_pIdxTree->add(pTreeEntry, strPath);
                }
                else if( strKeyInTreeToBeAdded.startsWith("L:") )
                {
                    pTreeEntry = new ZS::Apps::Test::IdxTree::CTrcAdmObj(strObjName);
                    m_pIdxTree->add(pTreeEntry, strPath);
                }
            } // if( pTreeEntry == nullptr )
        } // for( int idxPathNode = 0; idxPathNode < strlstKeysDefined.size(); ++idxPathNode )

        // Get new list of tree entries and map of free indices and compare with desired list and map
        //--------------------------------------------------------------------------------------------

        QVector<CIdxTreeEntry*> arpTreeEntriesResult = m_pIdxTree->treeEntriesVec();

        QVector<STreeEntryDscr> arTreeEntryDscrsResult = toTreeEntryDscrs(arpTreeEntriesResult);

        QMap<int, int> mapFreeIdxsResult = m_pIdxTree->freeIdxsMap();

        compare(arTreeEntryDscrsExpected, arTreeEntryDscrsResult, strlstExpectedValues, strlstResultValues);

        compare(mapFreeIdxsExpected, mapFreeIdxsResult, strlstExpectedValues, strlstResultValues);

    } // if( strOperation.startsWith("IdxTree.add",Qt::CaseInsensitive) )

    else // if( strOperation.startsWith("?") )
    {
        strResultValue = "Unsupported Operation " + strOperation;
        strlstResultValues.append(strResultValue);
    }

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        strlstResultValues << "";
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepAddEntry

//------------------------------------------------------------------------------
void CTest::doTestStepRemoveEntry( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    QString strOperation = i_pTestStep->getOperation();

    // Examples for strOperation:
    // - "delete B:ZS"
    // - "delete L:ZS::Data::DT::FDAC::RF1In"

    if( strOperation.startsWith("delete",Qt::CaseInsensitive) )
    {
        QString strKeyInTreeToBeRemoved;

        strKeyInTreeToBeRemoved = strOperation;
        strKeyInTreeToBeRemoved.remove("delete ");

        strOperation = "delete";

        // Get current list of tree entries and map of free indices
        //---------------------------------------------------------

        QVector<CIdxTreeEntry*> arpTreeEntriesExpected = m_pIdxTree->treeEntriesVec();

        QVector<STreeEntryDscr> arTreeEntryDscrsExpected = toTreeEntryDscrs(arpTreeEntriesExpected);

        QMap<int, int> mapFreeIdxsExpected = m_pIdxTree->freeIdxsMap();

        // Expected (desired) behaviour
        //-----------------------------

        CIdxTreeEntry* pTreeEntry = m_pIdxTree->findEntry(strKeyInTreeToBeRemoved);

        STreeEntryDscr treeEntryDscr(pTreeEntry);

        removeEntry(*m_pIdxTree, treeEntryDscr, arTreeEntryDscrsExpected, mapFreeIdxsExpected);

        // Remove the tree entry from the index tree using two different kind of methods
        //------------------------------------------------------------------------------

        if( strKeyInTreeToBeRemoved.startsWith("B:") )
        {
            CIdxTreeEntry* pBranch = m_pIdxTree->findBranch(strKeyInTreeToBeRemoved);

            CTrcAdmObjBranch* pTrcAdminObjBranch = dynamic_cast<CTrcAdmObjBranch*>(pBranch);

            // The tree entrie's dtor will remove itself from the index tree.
            delete pTrcAdminObjBranch;
            pTrcAdminObjBranch = nullptr;

        } // if( strKeyInTreeToBeRemoved.startsWith("B:") )

        else if( strKeyInTreeToBeRemoved.startsWith("L:") )
        {
            CIdxTreeEntry* pLeave = m_pIdxTree->findLeave(strKeyInTreeToBeRemoved);

            CTrcAdmObj* pTrcAdminObj = dynamic_cast<CTrcAdmObj*>(pLeave);

            delete pTrcAdminObj;
            pTrcAdminObj = nullptr;

        } // if( strKeyInTreeToBeRemoved.startsWith("L:") )

        // Get new list of tree entries and map of free indices and compare with desired list and map
        //--------------------------------------------------------------------------------------------

        QVector<CIdxTreeEntry*> arpTreeEntriesResult = m_pIdxTree->treeEntriesVec();

        QVector<STreeEntryDscr> arTreeEntryDscrsResult = toTreeEntryDscrs(arpTreeEntriesResult);

        QMap<int, int> mapFreeIdxsResult = m_pIdxTree->freeIdxsMap();

        compare(arTreeEntryDscrsExpected, arTreeEntryDscrsResult, strlstExpectedValues, strlstResultValues);

        compare(mapFreeIdxsExpected, mapFreeIdxsResult, strlstExpectedValues, strlstResultValues);

    } // if( strOperation.startsWith("delete",Qt::CaseInsensitive) )

    else // if( strOperation.startsWith("?") )
    {
        strResultValue = "Unsupported Operation " + strOperation;
        strlstResultValues.append(strResultValue);
    }

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        strlstResultValues << "";
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepRemoveEntry

//------------------------------------------------------------------------------
void CTest::doTestStepMoveEntry( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    QString strOperation = i_pTestStep->getOperation();

    // Examples for strOperation:
    // - "IdxTree.move(L:A0::B2::C4::o2, B:A0::B2::C3)"
    // - "IdxTree.move(B:A0::B2::C4, B:A4::B1)"

    if( strOperation.startsWith("IdxTree.move",Qt::CaseInsensitive) )
    {
        QString strArgs;

        strArgs = strOperation;
        strArgs.remove("IdxTree.move");
        strArgs.remove("(");
        strArgs.remove(")");

        strOperation = "IdxTree.move";

        QStringList strlstArgs = strArgs.split(",", QtSkipEmptyParts);

        if( strlstArgs.size() == 2 )
        {
            QString strKeyInTreeSrc = strlstArgs[0].trimmed();
            QString strKeyInTreeTrg = strlstArgs[1].trimmed();

            QString strBranchPathSrc;
            QString strObjNameSrc;
            QString strBranchPathTrg;
            QString strObjNameTrg;

            m_pIdxTree->splitPathStr(strKeyInTreeSrc, &strBranchPathSrc, &strObjNameSrc);
            m_pIdxTree->splitPathStr(strKeyInTreeTrg, &strBranchPathTrg, &strObjNameTrg);

            strBranchPathTrg = m_pIdxTree->buildPathStr(strBranchPathTrg, strObjNameTrg);

            // Get current list of tree entries and map of free indices
            //---------------------------------------------------------

            QVector<CIdxTreeEntry*> arpTreeEntriesExpected = m_pIdxTree->treeEntriesVec();

            QVector<STreeEntryDscr> arTreeEntryDscrsExpected = toTreeEntryDscrs(arpTreeEntriesExpected);

            QMap<int, int> mapFreeIdxsExpected = m_pIdxTree->freeIdxsMap();

            // Expected (desired) behaviour
            //-----------------------------

            CIdxTreeEntry* pTreeEntrySrc = m_pIdxTree->findEntry(strKeyInTreeSrc);
            CIdxTreeEntry* pTreeEntryTrg = m_pIdxTree->findEntry(strKeyInTreeTrg);

            STreeEntryDscr treeEntryDscrSrc(pTreeEntrySrc);
            STreeEntryDscr treeEntryDscrTrg(pTreeEntryTrg);

            moveEntry(*m_pIdxTree, treeEntryDscrTrg, treeEntryDscrSrc, arTreeEntryDscrsExpected);

            // Move Entry
            //-----------

            m_pIdxTree->move(strKeyInTreeSrc, strBranchPathTrg);

            // Get new list of tree entries and map of free indices and compare with desired list and map
            //--------------------------------------------------------------------------------------------

            QVector<CIdxTreeEntry*> arpTreeEntriesResult = m_pIdxTree->treeEntriesVec();

            QVector<STreeEntryDscr> arTreeEntryDscrsResult = toTreeEntryDscrs(arpTreeEntriesResult);

            QMap<int, int> mapFreeIdxsResult = m_pIdxTree->freeIdxsMap();

            compare(arTreeEntryDscrsExpected, arTreeEntryDscrsResult, strlstExpectedValues, strlstResultValues);

            compare(mapFreeIdxsExpected, mapFreeIdxsResult, strlstExpectedValues, strlstResultValues);

        } // if( strlstArgs.size() == 2 )

        else // if( strlstArgs.size() != 2 )
        {
            strResultValue = "Number of arguments out of range for " + strOperation;
            strlstResultValues.append(strResultValue);
        }
    } // if( strOperation.startsWith("IdxTree.move",Qt::CaseInsensitive) )

    else // if( strOperation.startsWith("?") )
    {
        strResultValue = "Unsupported Operation " + strOperation;
        strlstResultValues.append(strResultValue);
    }

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        strlstResultValues << "";
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepMoveEntry

//------------------------------------------------------------------------------
void CTest::doTestStepCopyEntry( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    QString strOperation = i_pTestStep->getOperation();

    // Examples for strOperation:
    // - "IdxTree.copy(L:A0::B2::C4::o2, B:A0::B2::C3)"
    // - "IdxTree.copy(B:A0::B2::C3, B:A0::B2)"

    if( strOperation.startsWith("IdxTree.copy",Qt::CaseInsensitive) )
    {
        QString strArgs;

        strArgs = strOperation;
        strArgs.remove("IdxTree.copy");
        strArgs.remove("(");
        strArgs.remove(")");

        strOperation = "IdxTree.copy";

        QStringList strlstArgs = strArgs.split(",", QtSkipEmptyParts);

        if( strlstArgs.size() == 2 )
        {
            QString strKeyInTreeSrc = strlstArgs[0].trimmed();
            QString strKeyInTreeTrg = strlstArgs[1].trimmed();

            QString strBranchPathSrc;
            QString strObjNameSrc;
            QString strBranchPathTrg;
            QString strObjNameTrg;

            m_pIdxTree->splitPathStr(strKeyInTreeSrc, &strBranchPathSrc, &strObjNameSrc);
            m_pIdxTree->splitPathStr(strKeyInTreeTrg, &strBranchPathTrg, &strObjNameTrg);

            strBranchPathTrg = m_pIdxTree->buildPathStr(strBranchPathTrg, strObjNameTrg);

            // Get current list of tree entries and map of free indices
            //---------------------------------------------------------

            QVector<CIdxTreeEntry*> arpTreeEntriesExpected = m_pIdxTree->treeEntriesVec();

            QVector<STreeEntryDscr> arTreeEntryDscrsExpected = toTreeEntryDscrs(arpTreeEntriesExpected);

            QMap<int, int> mapFreeIdxsExpected = m_pIdxTree->freeIdxsMap();

            // Expected (desired) behaviour
            //-----------------------------

            int idxInTreeSrc = indexOf(strKeyInTreeSrc, arTreeEntryDscrsExpected);
            int idxInTreeTrg = indexOf(strKeyInTreeTrg, arTreeEntryDscrsExpected);

            if( idxInTreeSrc >= 0 && idxInTreeTrg >= 0 )
            {
                STreeEntryDscr& treeEntryDscrSrc = arTreeEntryDscrsExpected[idxInTreeSrc];
                STreeEntryDscr& treeEntryDscrTrg = arTreeEntryDscrsExpected[idxInTreeTrg];

                copyEntry(*m_pIdxTree, treeEntryDscrTrg, treeEntryDscrSrc, arTreeEntryDscrsExpected, mapFreeIdxsExpected);
            }

            // Copy Entry
            //-----------

            m_pIdxTree->copy(strKeyInTreeSrc, strBranchPathTrg);

            // Get new list of tree entries and map of free indices and compare with desired list and map
            //--------------------------------------------------------------------------------------------

            QVector<CIdxTreeEntry*> arpTreeEntriesResult = m_pIdxTree->treeEntriesVec();

            QVector<STreeEntryDscr> arTreeEntryDscrsResult = toTreeEntryDscrs(arpTreeEntriesResult);

            QMap<int, int> mapFreeIdxsResult = m_pIdxTree->freeIdxsMap();

            compare(arTreeEntryDscrsExpected, arTreeEntryDscrsResult, strlstExpectedValues, strlstResultValues);

            compare(mapFreeIdxsExpected, mapFreeIdxsResult, strlstExpectedValues, strlstResultValues);

        } // if( strlstArgs.size() == 2 )

        else // if( strlstArgs.size() != 2 )
        {
            strResultValue = "Number of arguments out of range for " + strOperation;
            strlstResultValues.append(strResultValue);
        }
    } // if( strOperation.startsWith("IdxTree.copy",Qt::CaseInsensitive) )

    else // if( strOperation.startsWith("?") )
    {
        strResultValue = "Unsupported Operation " + strOperation;
        strlstResultValues.append(strResultValue);
    }

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        strlstResultValues << "";
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepCopyEntry

//------------------------------------------------------------------------------
void CTest::doTestStepRenameEntry( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    QString strOperation = i_pTestStep->getOperation();

    // Examples for strOperation:
    // - "IdxTree.rename(L:A0::B1::C4::o2, o9)",
    // - "IdxTree.rename(B:A0::B1, B9)",

    if( strOperation.startsWith("IdxTree.rename",Qt::CaseInsensitive) )
    {
        QString strArgs;

        strArgs = strOperation;
        strArgs.remove("IdxTree.rename");
        strArgs.remove("(");
        strArgs.remove(")");

        strOperation = "IdxTree.rename";

        QStringList strlstArgs = strArgs.split(",", QtSkipEmptyParts);

        if( strlstArgs.size() == 2 )
        {
            // Get current list of tree entries and map of free indices
            //---------------------------------------------------------

            QVector<CIdxTreeEntry*> arpTreeEntriesExpected = m_pIdxTree->treeEntriesVec();

            QVector<STreeEntryDscr> arTreeEntryDscrsExpected = toTreeEntryDscrs(arpTreeEntriesExpected);

            QMap<int, int> mapFreeIdxsExpected = m_pIdxTree->freeIdxsMap();

            // Expected (desired) behaviour
            //-----------------------------

            QString strKeyInTreeSrc = strlstArgs[0].trimmed();
            QString strObjNameTrg   = strlstArgs[1].trimmed();

            CIdxTreeEntry* pTreeEntry = m_pIdxTree->findEntry(strKeyInTreeSrc);

            // If the tree entry exists ...
            if( pTreeEntry != nullptr )
            {
                STreeEntryDscr treeEntryDscr(pTreeEntry);

                renameEntry(*m_pIdxTree, strObjNameTrg, treeEntryDscr, arTreeEntryDscrsExpected);
            }

            // Rename entry
            //-------------

            m_pIdxTree->rename(strKeyInTreeSrc, strObjNameTrg);

            // Get new list of tree entries and map of free indices and compare with desired list and map
            //--------------------------------------------------------------------------------------------

            QVector<CIdxTreeEntry*> arpTreeEntriesResult = m_pIdxTree->treeEntriesVec();

            QVector<STreeEntryDscr> arTreeEntryDscrsResult = toTreeEntryDscrs(arpTreeEntriesResult);

            QMap<int, int> mapFreeIdxsResult = m_pIdxTree->freeIdxsMap();

            compare(arTreeEntryDscrsExpected, arTreeEntryDscrsResult, strlstExpectedValues, strlstResultValues);

            compare(mapFreeIdxsExpected, mapFreeIdxsResult, strlstExpectedValues, strlstResultValues);

        } // if( strlstArgs.size() == 2 )

        else // if( strlstArgs.size() != 2 )
        {
            strResultValue = "Number of arguments out of range for " + strOperation;
            strlstResultValues.append(strResultValue);
        }
    } // if( strOperation.startsWith("IdxTree.rename",Qt::CaseInsensitive) )

    else // if( strOperation.startsWith("?") )
    {
        strResultValue = "Unsupported Operation " + strOperation;
        strlstResultValues.append(strResultValue);
    }

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        strlstResultValues << "";
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepRenameEntry

//------------------------------------------------------------------------------
void CTest::doTestStepChangeViews( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    QString strOperation = i_pTestStep->getOperation();

    // Examples for strOperation:
    // - "WidgetIdxTree.setSortOrder(Config)",
    // - "WidgetIdxTree.setSortOrder(Ascending)",
    // - "WidgetIdxTree.setViewMode(NavPanelAndBranchContent)"
    // - "TreeViewIdxTree.select(B:A0::B2::C3)"
    // - "WidgetIdxTree.setViewMode(NavPanelOnly)"

    CWdgtIdxTree::EViewMode viewMode  = CWdgtIdxTree::EViewMode::Undefined;
    EIdxTreeSortOrder       sortOrder = EIdxTreeSortOrder::Undefined;
    CModelIdxTreeEntry*     pModelTreeEntrySelected = nullptr;
    QString                 strKeyInTreeToBeSelected;

    if( strOperation.contains("WidgetIdxTree.setSortOrder") )
    {
        if( strOperation.contains("Config",Qt::CaseInsensitive) )
        {
            sortOrder = EIdxTreeSortOrder::Config;
        }
        else if( strOperation.contains("Ascending",Qt::CaseInsensitive) )
        {
            sortOrder = EIdxTreeSortOrder::Ascending;
        }
        strOperation = "WidgetIdxTree.setSortOrder";
    }
    else if( strOperation.contains("WidgetIdxTree.setViewMode") )
    {
        if( strOperation.contains("NavPanelOnly",Qt::CaseInsensitive) )
        {
            viewMode = CWdgtIdxTree::EViewMode::NavPanelOnly;
        }
        else if( strOperation.contains("NavPanelAndBranchContent",Qt::CaseInsensitive) )
        {
            viewMode = CWdgtIdxTree::EViewMode::NavPanelAndBranchContent;
        }
        strOperation = "WidgetIdxTree.setViewMode";
    }
    else if( strOperation.contains("TreeViewIdxTree.select") )
    {
        strKeyInTreeToBeSelected = strOperation;
        strKeyInTreeToBeSelected.remove("TreeViewIdxTree.select");
        strKeyInTreeToBeSelected.remove("(");
        strKeyInTreeToBeSelected.remove(")");

        strOperation = "TreeViewIdxTree.select";

    } // if( strOperation.contains("TreeViewIdxTree.select") )

    strlstExpectedValues << strExpectedValue;

    // Test Step
    //----------

    CIdxTree*                       pIdxTree                       = m_pIdxTree;
    CMainWindow*                    pMainWindow                    = CMainWindow::GetInstance();
    CWdgtIdxTree*                   pWdgtIdxTree                   = pMainWindow->idxTreeWidget();
    CTreeViewIdxTree*               pTreeViewIdxTree               = pWdgtIdxTree->treeView();
    CModelIdxTree*                  pModelIdxTree                  = dynamic_cast<CModelIdxTree*>(pTreeViewIdxTree->model());
    CTableViewIdxTreeBranchContent* pTableViewIdxTreeBranchContent = pWdgtIdxTree->tableView();
    CModelIdxTreeBranchContent*     pModelIdxTreeBranchContent     = nullptr;

    if( strOperation == "WidgetIdxTree.expandAll" )
    {
        pTreeViewIdxTree->expandAll();
    }
    else if( strOperation == "WidgetIdxTree.collapseAll" )
    {
        pTreeViewIdxTree->collapseAll();
    }
    else if( strOperation == "WidgetIdxTree.resizeColumnsToContents" )
    {
        for( int idxClm = 0; idxClm < CModelIdxTree::EColumnCount; idxClm++ )
        {
            pTreeViewIdxTree->resizeColumnToContents(idxClm);
        }
    }
    else if( strOperation == "WidgetIdxTree.setSortOrder" )
    {
        pTreeViewIdxTree->setSortOrder(sortOrder);

        if( pTableViewIdxTreeBranchContent != nullptr )
        {
            pTableViewIdxTreeBranchContent->setSortOrder(sortOrder);
        }
    }
    else if( strOperation == "WidgetIdxTree.setViewMode" )
    {
        pWdgtIdxTree->setViewMode(viewMode);
    }
    else if( strOperation == "TreeViewIdxTree.select" )
    {
        QModelIndex modelIdx = pModelIdxTree->index(strKeyInTreeToBeSelected, 0);
        pTreeViewIdxTree->setCurrentIndex(modelIdx);
        pModelTreeEntrySelected = static_cast<CModelIdxTreeEntry*>(modelIdx.internalPointer());
    }

    // Result Values
    //---------------

    // If the view mode don't has to be changed by this test step ..
    if( viewMode == CWdgtIdxTree::EViewMode::Undefined )
    {
        // .. take the current setting.
        viewMode = pWdgtIdxTree->viewMode();
    }

    // If the sort order don't has to be changed by this test step ..
    if( sortOrder == EIdxTreeSortOrder::Undefined )
    {
        // .. take the current setting.
        sortOrder = pTreeViewIdxTree->sortOrder();
    }

    // If the selected node don't has to be changed by this test step ..
    if( strKeyInTreeToBeSelected.isEmpty() )
    {
        // .. take the current setting.
        QModelIndex modelIdx = pTreeViewIdxTree->currentIndex();
        pModelTreeEntrySelected = static_cast<CModelIdxTreeEntry*>(modelIdx.internalPointer());
    }

    // Please note: At the following test conditions:
    // If a node is not at the expected position the iteration will be
    // stopped and the path of the node is added to the actual values whereupon
    // actual and desired values are not the same and the test step failed.
    CIdxTree::iterator       itIdxTree;
    CModelIdxTree::iterator  itModelIdxTree;
    CIdxTreeEntry*           pTreeEntry;
    CModelIdxTreeEntry*      pModelTreeEntry;
    CModelIdxTreeEntry*      pModelBranchParent;
    CModelIdxTreeEntry*      pModelTreeEntryPrev;
    CModelIdxTreeEntry*      pModelTreeEntryNext;
    int                      idxInParentBranch;
    int                      idxItStep;

    if( viewMode == CWdgtIdxTree::EViewMode::NavPanelOnly )
    {
        // Check content of tree view
        //---------------------------

        // The tree view must contain each entry of the index tree.
        itIdxTree = pIdxTree->begin();
        while( itIdxTree != pIdxTree->end() )
        {
            pTreeEntry = *itIdxTree;
            pModelTreeEntry = pModelIdxTree->findModelEntry(pTreeEntry);
            if( pModelTreeEntry == nullptr )
            {
                strResultValue  = "ViewMode: " + CWdgtIdxTree::viewMode2Str(viewMode);
                strResultValue += ", SortOrder: " + idxTreeSortOrder2Str(sortOrder);
                strResultValue += ", Iterator.TraversalOrder: Index";
                strResultValue += "  pModelIdxTree->findModelEntry(" + pTreeEntry->keyInTree() + ") == nullptr";
                strlstResultValues << strResultValue;
                break;
            }
            ++itIdxTree;
        } // while( itIdxTree != pIdxTree->end() )

        // Check Index iterator
        //---------------------

        // When iterating through the tree model using the Index Iterator
        // the index tree iterator and the model tree iterator must point to the
        // same index tree entry. No matter which sort order is used.
        itIdxTree      = pIdxTree->begin();
        itModelIdxTree = pModelIdxTree->begin();
        idxItStep      = 0;
        while( itIdxTree != pIdxTree->end() && itModelIdxTree != pModelIdxTree->end() )
        {
            pTreeEntry = *itIdxTree;
            pModelTreeEntry = *itModelIdxTree;
            if( pTreeEntry->keyInTree() != pModelTreeEntry->keyInTree() )
            {
                strResultValue  = "ViewMode: " + CWdgtIdxTree::viewMode2Str(viewMode);
                strResultValue += ", SortOrder: " + idxTreeSortOrder2Str(sortOrder);
                strResultValue += ", Iterator.TraversalOrder: Index";
                strResultValue += ", IteratorStep: " + QString::number(idxItStep) + ":\n";
                strResultValue += "  ItIdxTree (" + pTreeEntry->keyInTree() + ") != ItModelIdxTree (" + pModelTreeEntry->keyInTree() + ")";
                strlstResultValues << strResultValue;
                break;
            }
            ++itIdxTree;
            ++itModelIdxTree;
            ++idxItStep;
        } // while( itIdxTree != pIdxTree->end() && itModelIdxTree != pModelIdxTree->end() )

        // Check PreOrder iterator
        //------------------------

        if( sortOrder == EIdxTreeSortOrder::Config )
        {
            // If sort order is Config the model iterator and the index tree iterator must point
            // to the same index tree entry.
            itIdxTree      = pIdxTree->begin(CIdxTree::iterator::ETraversalOrder::PreOrder);
            itModelIdxTree = pModelIdxTree->begin(CModelIdxTree::iterator::ETraversalOrder::PreOrder);
            idxItStep      = 0;
            while( itIdxTree != pIdxTree->end() && itModelIdxTree != pModelIdxTree->end() )
            {
                pTreeEntry = *itIdxTree;
                pModelTreeEntry = *itModelIdxTree;
                if( pTreeEntry->keyInTree() != pModelTreeEntry->keyInTree() )
                {
                    strResultValue  = "ViewMode: " + CWdgtIdxTree::viewMode2Str(viewMode);
                    strResultValue += ", SortOrder: " + idxTreeSortOrder2Str(sortOrder);
                    strResultValue += ", Iterator.TraversalOrder: PreOrder";
                    strResultValue += ", IteratorStep: " + QString::number(idxItStep) + ":\n";
                    strResultValue += "  ItIdxTree (" + pTreeEntry->keyInTree() + ") != ItModelIdxTree (" + pModelTreeEntry->keyInTree() + ")";
                    strlstResultValues << strResultValue;
                    break;
                }
                ++itIdxTree;
                ++itModelIdxTree;
                ++idxItStep;
            } // while( itIdxTree != pIdxTree->end() && itModelIdxTree != pModelIdxTree->end() )
        } // if( sortOrder == EIdxTreeSortOrder::Config )
    } // if( viewMode == CWdgtIdxTree::EViewMode::NavPanelOnly )

    else if( viewMode == CWdgtIdxTree::EViewMode::NavPanelAndBranchContent )
    {
        pTableViewIdxTreeBranchContent = pWdgtIdxTree->tableView();
        pModelIdxTreeBranchContent     = dynamic_cast<CModelIdxTreeBranchContent*>(pTableViewIdxTreeBranchContent->model());

        // Check content of views
        //-----------------------

        // The tree view must contain each branch of the index tree.
        itIdxTree = pIdxTree->begin();
        while( itIdxTree != pIdxTree->end() )
        {
            pTreeEntry = *itIdxTree;
            if( pTreeEntry->entryType() == EIdxTreeEntryType::Root || pTreeEntry->entryType() == EIdxTreeEntryType::Branch )
            {
                pModelTreeEntry = pModelIdxTree->findModelEntry(pTreeEntry);

                if( pModelTreeEntry == nullptr )
                {
                    strResultValue  = "ViewMode: " + CWdgtIdxTree::viewMode2Str(viewMode);
                    strResultValue += ", SortOrder: " + idxTreeSortOrder2Str(sortOrder) + ":\n";
                    strResultValue += "  pModelIdxTree->findModelEntry(" + pTreeEntry->keyInTree() + ") == nullptr";
                    strlstResultValues << strResultValue;
                    break;
                }
            }
            ++itIdxTree;
        } // while( itIdxTree != pIdxTree->end() )

        // The tree view (navigation panel) may only contain branch nodes.
        itModelIdxTree = pModelIdxTree->begin();
        while( itModelIdxTree != pModelIdxTree->end() )
        {
            pModelTreeEntry = *itModelIdxTree;
            if( pModelTreeEntry->entryType() != EIdxTreeEntryType::Root && pModelTreeEntry->entryType() != EIdxTreeEntryType::Branch )
            {
                strResultValue  = "ViewMode: " + CWdgtIdxTree::viewMode2Str(viewMode);
                strResultValue += ", SortOrder: " + idxTreeSortOrder2Str(sortOrder) + ":\n";
                strResultValue += "  ItIdxTree (" + pModelTreeEntry->keyInTree() + ") != Branch";
                strlstResultValues << strResultValue;
                break;
            }
            ++itModelIdxTree;
        } // itModelIdxTree != pModelIdxTree->end() )

        // On selecting a branch the branch content table view must contain all childs of the selected branch.
        if( !strKeyInTreeToBeSelected.isEmpty() && pModelTreeEntrySelected == nullptr )
        {
            strResultValue  = "ViewMode: " + CWdgtIdxTree::viewMode2Str(viewMode);
            strResultValue += ", SortOrder: " + idxTreeSortOrder2Str(sortOrder) + ":\n";
            strResultValue += "  pModelIdxTree->currentIndex() != " + strKeyInTreeToBeSelected;
            strlstResultValues << strResultValue;
        }
        else if( pModelTreeEntrySelected != nullptr )
        {
            // Please note that the model branch entry as the root of the branch content model is a clone
            // of the model branch of idx tree model. The branch node of the index tree model may not contain
            // leaves whereas the root branch (the clone) of the content model may.
            CModelIdxTreeEntry* pModelBranchSelected = pModelIdxTreeBranchContent->modelBranch();

            if( pModelBranchSelected == nullptr )
            {
                strResultValue  = "ViewMode: " + CWdgtIdxTree::viewMode2Str(viewMode);
                strResultValue += ", SortOrder: " + idxTreeSortOrder2Str(sortOrder);
                strResultValue += ", BranchSelected: nullptr:\n";
                strResultValue += "  pModelIdxTree->currentIndex() != " + strKeyInTreeToBeSelected;
                strlstResultValues << strResultValue;
            }
            else if( pModelBranchSelected->treeEntry() != pModelIdxTreeBranchContent->branch() )
            {
                strResultValue  = "ViewMode: " + CWdgtIdxTree::viewMode2Str(viewMode);
                strResultValue += ", SortOrder: " + idxTreeSortOrder2Str(sortOrder) + ":\n";
                strResultValue += ", BranchSelected: " + strKeyInTreeToBeSelected + ":\n";
                strlstResultValues << strResultValue;
            }
            else // if( pModelBranchSelected != nullptr )
            {
                if( pModelBranchSelected->keyInTree() != strKeyInTreeToBeSelected )
                {
                    strResultValue  = "ViewMode: " + CWdgtIdxTree::viewMode2Str(viewMode);
                    strResultValue += ", SortOrder: " + idxTreeSortOrder2Str(sortOrder) + ":\n";
                    strResultValue += ", BranchSelected: " + strKeyInTreeToBeSelected + ":\n";
                    strResultValue += "  pModelIdxTree->currentIndex() != " + strKeyInTreeToBeSelected;
                    strlstResultValues << strResultValue;
                }
                else // if( pModelBranchSelected->keyInTree() == strKeyInTreeToBeSelected )
                {
                    CIdxTreeEntry* pBranchSelected = dynamic_cast<CIdxTreeEntry*>(pModelBranchSelected->treeEntry());

                    if( pBranchSelected->count() != pModelBranchSelected->count() )
                    {
                        strResultValue  = "ViewMode: " + CWdgtIdxTree::viewMode2Str(viewMode);
                        strResultValue += ", SortOrder: " + idxTreeSortOrder2Str(sortOrder);
                        strResultValue += ", BranchSelected: " + strKeyInTreeToBeSelected + ":\n";
                        strResultValue += "  BranchSelected->count(" + QString::number(pBranchSelected->count()) + ")";
                        strResultValue += "!= ModelBranchSelected->count(" + QString::number(pModelBranchSelected->count()) + ")";
                        strlstResultValues << strResultValue;
                    }
                    else // if( pBranchSelected->count() == pModelBranchSelected->count() )
                    {
                        for( int idxTreeEntry = 0; idxTreeEntry < pBranchSelected->count(); ++idxTreeEntry )
                        {
                            pTreeEntry = pBranchSelected->at(idxTreeEntry);
                            pModelTreeEntry = pModelIdxTreeBranchContent->findModelEntry(pTreeEntry);
                            if( pModelTreeEntry == nullptr )
                            {
                                strResultValue  = "ViewMode: " + CWdgtIdxTree::viewMode2Str(viewMode);
                                strResultValue += ", SortOrder: " + idxTreeSortOrder2Str(sortOrder);
                                strResultValue += ", BranchSelected: " + strKeyInTreeToBeSelected + ":\n";
                                strResultValue += "  pModelIdxTreeBranchContent->findModelEntry(" + pTreeEntry->keyInTree() + ") == nullptr";
                                strlstResultValues << strResultValue;
                                break;
                            }
                        }
                    } // if( pBranchSelected->count() == pModelBranchSelected->count() )
                } // if( pModelBranchSelected->keyInTree() == strKeyInTreeToBeSelected )
            } // if( pModelBranchSelected != nullptr )
        } // if( pModelTreeEntrySelected != nullptr )
    } // if( viewMode == CWdgtIdxTree::EViewMode::NavPanelAndBranchContent )

    // Sort order Ascending
    //---------------------

    if( sortOrder == EIdxTreeSortOrder::Ascending )
    {
        // If sort order is Ascending the entries in the index tree model must be in aplphabetical order.
        itModelIdxTree = pModelIdxTree->begin(CModelIdxTree::iterator::ETraversalOrder::PreOrder);
        idxItStep      = 0;
        while( itModelIdxTree != pModelIdxTree->end() )
        {
            pModelTreeEntry     = *itModelIdxTree;
            idxInParentBranch   = pModelTreeEntry->modelIndexInParentBranch();
            pModelBranchParent  = pModelTreeEntry->modelParentBranch();
            pModelTreeEntryPrev = nullptr;
            pModelTreeEntryNext = nullptr;
            if( pModelBranchParent != nullptr )
            {
                if( idxInParentBranch > 0 )
                {
                    pModelTreeEntryPrev = pModelBranchParent->at(idxInParentBranch-1);
                }
                if( idxInParentBranch < (pModelBranchParent->count()-1) )
                {
                    pModelTreeEntryNext = pModelBranchParent->at(idxInParentBranch+1);
                }
                if( pModelTreeEntryPrev != nullptr && pModelTreeEntryNext != nullptr )
                {
                    if( pModelTreeEntry->keyInParentBranch() < pModelTreeEntryPrev->keyInParentBranch() )
                    {
                        strResultValue  = "ViewMode: " + CWdgtIdxTree::viewMode2Str(viewMode);
                        strResultValue += ", SortOrder: " + idxTreeSortOrder2Str(sortOrder);
                        strResultValue += ", Iterator.TraversalOrder: PreOrder";
                        strResultValue += ", IteratorStep: " + QString::number(idxItStep) + ":\n";
                        strResultValue += "  ItIdxTree (" + pTreeEntry->keyInParentBranch() + ") < ItIdxTreePrev (" + pModelTreeEntryPrev->keyInParentBranch() + ")";
                        strlstResultValues << strResultValue;
                    }
                    if( pModelTreeEntry->keyInParentBranch() > pModelTreeEntryNext->keyInParentBranch() )
                    {
                        strResultValue  = "ViewMode: " + CWdgtIdxTree::viewMode2Str(viewMode);
                        strResultValue += ", SortOrder: " + idxTreeSortOrder2Str(sortOrder);
                        strResultValue += ", Iterator.TraversalOrder: PreOrder";
                        strResultValue += ", IteratorStep: " + QString::number(idxItStep) + ":\n";
                        strResultValue += "  ItIdxTree (" + pTreeEntry->keyInParentBranch() + ") > ItIdxTreeNext (" + pModelTreeEntryPrev->keyInParentBranch() + ")";
                        strlstResultValues << strResultValue;
                    }
                    if( strlstResultValues.size() > 0 )
                    {
                        break;
                    }
                }
            } // if( pModelBranchParent != nullptr )
            ++itModelIdxTree;
            ++idxItStep;
        } // while( itModelIdxTree != pModelIdxTree->end() )

        // If sort order is Ascending the entries in the branch content model must be in aplphabetical order.
        if( pModelIdxTreeBranchContent != nullptr )
        {
            // Please note that the model branch entry as the root of the branch content model is a clone
            // of the model branch of idx tree model. The branch node of the index tree model may not contain
            // leaves whereas the root branch (the clone) of the content model may.
            CModelIdxTreeEntry* pModelBranchSelected = pModelIdxTreeBranchContent->modelBranch();

            // If a branch has been assigned to the table view ...
            if( pModelBranchSelected != nullptr )
            {
                for( int idxTreeEntry = 0; idxTreeEntry < pModelBranchSelected->count(); ++idxTreeEntry )
                {
                    pModelTreeEntry     = pModelBranchSelected->at(idxTreeEntry);
                    idxInParentBranch   = pModelTreeEntry->modelIndexInParentBranch();
                    pModelTreeEntryPrev = nullptr;
                    pModelTreeEntryNext = nullptr;

                    if( idxInParentBranch > 0 )
                    {
                        pModelTreeEntryPrev = pModelBranchSelected->at(idxInParentBranch-1);
                    }
                    if( idxInParentBranch < (pModelBranchSelected->count()-1) )
                    {
                        pModelTreeEntryNext = pModelBranchSelected->at(idxInParentBranch+1);
                    }
                    if( pModelTreeEntryPrev != nullptr )
                    {
                        if( pModelTreeEntry->keyInParentBranch() < pModelTreeEntryPrev->keyInParentBranch() )
                        {
                            strResultValue  = "ViewMode: " + CWdgtIdxTree::viewMode2Str(viewMode);
                            strResultValue += ", SortOrder: " + idxTreeSortOrder2Str(sortOrder);
                            strResultValue += ", Iterator.TraversalOrder: PreOrder";
                            strResultValue += "  ItModelBranch (" + pModelTreeEntry->keyInParentBranch() + ") < ItModelBranchPrev (" + pModelTreeEntryPrev->keyInParentBranch() + ")";
                            strlstResultValues << strResultValue;
                        }
                    }
                    if( pModelTreeEntryNext != nullptr )
                    {
                        if( pModelTreeEntry->keyInParentBranch() > pModelTreeEntryNext->keyInParentBranch() )
                        {
                            strResultValue  = "ViewMode: " + CWdgtIdxTree::viewMode2Str(viewMode);
                            strResultValue += ", SortOrder: " + idxTreeSortOrder2Str(sortOrder);
                            strResultValue += ", IteratorStep: " + QString::number(idxTreeEntry) + ":\n";
                            strResultValue += "  ItModelBranch (" + pModelTreeEntry->keyInParentBranch() + ") > ItModelBranchNext (" + pModelTreeEntryNext->keyInParentBranch() + ")";
                            strlstResultValues << strResultValue;
                        }
                        if( strlstResultValues.size() > 0 )
                        {
                            break;
                        }
                    }
                } // for( int idxTreeEntry = 0; idxTreeEntry < pModelBranchSelected->count(); ++idxTreeEntry )
            } // if( pModelBranchSelected != nullptr )
        } // if( pModelIdxTreeBranchContent != nullptr )
    } // if( sortOrder == EIdxTreeSortOrder::Ascending )

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        strlstResultValues << "";
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepChangeViews

//------------------------------------------------------------------------------
void CTest::doTestStepKeyboardInputs( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    QString strOperation = i_pTestStep->getOperation();

    // Examples for strOperation:
    // - "TreeViewIdxTree.select(L:A0::B2::o2)"
    // - "TreeViewIdxTree.keyEvent(Press F2)"
    // - "TreeViewIdxTree.keyEvent(Release F2)"
    // - "TreeViewIdxTree.keyEvent(Press o)"
    // - "TreeViewIdxTree.keyEvent(Release o)"
    // - "TreeViewIdxTree.keyEvent(Press 4)"
    // - "TreeViewIdxTree.keyEvent(Release 4)"
    // - "TreeViewIdxTree.keyEvent(Press Up)"
    // - "TreeViewIdxTree.keyEvent(Release Up)"
    // - "TreeViewIdxTree.keyEvent(Press Down)"
    // - "TreeViewIdxTree.keyEvent(Release Down)"
    // - "TreeViewIdxTree.keyEvent(Press Ctrl-c)"
    // - "TreeViewIdxTree.keyEvent(Release Ctrl-c)"
    // - "TreeViewIdxTree.keyEvent(Press Ctrl-x)"
    // - "TreeViewIdxTree.keyEvent(Release Ctrl-x)"
    // - "TreeViewIdxTree.keyEvent(Press Ctrl-v)"
    // - "TreeViewIdxTree.keyEvent(Release Ctrl-v)"
    // - "TreeViewIdxTree.keyEvent(Press Delete)"
    // - "TreeViewIdxTree.keyEvent(Release Delete)"

    QString      strKeyInTreeToBeSelected;
    QEvent::Type keyEvent = QEvent::None;
    QString      strKey;

    if( strOperation.contains("TreeViewIdxTree.select") )
    {
        strKeyInTreeToBeSelected = strOperation;
        strKeyInTreeToBeSelected.remove("TreeViewIdxTree.select");
        strKeyInTreeToBeSelected.remove("(");
        strKeyInTreeToBeSelected.remove(")");

        strOperation = "TreeViewIdxTree.select";

    } // if( strOperation.contains("TreeViewIdxTree.select") )

    else if( strOperation.contains("TreeViewIdxTree.keyEvent") )
    {
        QString strTmp = strOperation;
        strTmp.remove("TreeViewIdxTree.keyEvent");
        strTmp.remove("(");
        strTmp.remove(")");

        if( strTmp.contains("Press") )
        {
            keyEvent = QEvent::KeyPress;
            strTmp.remove("Press");
        }
        else if( strTmp.contains("Release") )
        {
            keyEvent = QEvent::KeyRelease;
            strTmp.remove("Release");
        }
        strKey = strTmp.trimmed();

        strOperation = "TreeViewIdxTree.keyEvent";

    } // if( strOperation.contains("TreeViewIdxTree.keyEvent") )

    strlstExpectedValues << strExpectedValue;

    // Test Step
    //----------

    CMainWindow*      pMainWindow      = CMainWindow::GetInstance();
    CWdgtIdxTree*     pWdgtIdxTree     = pMainWindow->idxTreeWidget();
    CTreeViewIdxTree* pTreeViewIdxTree = pWdgtIdxTree->treeView();
    CModelIdxTree*    pModelIdxTree    = dynamic_cast<CModelIdxTree*>(pTreeViewIdxTree->model());

    if( strOperation == "TreeViewIdxTree.select" )
    {
        QModelIndex modelIdx = pModelIdxTree->index(strKeyInTreeToBeSelected, 0);
        pTreeViewIdxTree->setCurrentIndex(modelIdx);
    }
    else if( strOperation == "TreeViewIdxTree.keyEvent" )
    {
        QLineEdit* pEdtName      = nullptr;
        QWidget*   pWdgtReceiver = pTreeViewIdxTree;
        QKeyEvent* pKeyEvent     = nullptr;

        CDelegateIdxTree* pItemDelegateIdxTree = dynamic_cast<CDelegateIdxTree*>(pTreeViewIdxTree->itemDelegate());
        if( pItemDelegateIdxTree != nullptr && pItemDelegateIdxTree->nameColumnEditor() != nullptr )
        {
            pEdtName = pItemDelegateIdxTree->nameColumnEditor();
            pWdgtReceiver = pEdtName;
        }
        if( pWdgtReceiver != nullptr && keyEvent != QEvent::None )
        {
            pTreeViewIdxTree->setSilentlyExecuteDeleteRequests(true);
            pTreeViewIdxTree->setSilentlyIgnoreInvalidCopyRequests(true);

            if( keyEvent == QEvent::KeyPress )
            {
                pKeyEvent = str2QKeyEvent(strKey, keyEvent);
            }
            else if( keyEvent == QEvent::KeyRelease )
            {
                pKeyEvent = str2QKeyEvent(strKey, keyEvent);
            }
        }
        if( pWdgtReceiver != nullptr )
        {
            if( pKeyEvent != nullptr )
            {
                QApplication::postEvent(pWdgtReceiver, pKeyEvent);
                // delete pKeyEvent; Deleted by Qt
                pKeyEvent = nullptr;
            }
        }
    } // if( strOperation == "TreeViewIdxTree.keyEvent" )

    // Result Values
    //---------------

    CIdxTreeEntry* pTreeEntrySrc = nullptr;
    CIdxTreeEntry* pTreeEntryTrg = nullptr;

    ZS::Test::CTestStepGroup* pTSGrpParent = i_pTestStep->getParentGroup();

    QString strParentGroupName = pTSGrpParent->name();

    if( strParentGroupName.contains("Rename L:A0::B2::o2 -> o4",Qt::CaseInsensitive) )
    {
        if( strOperation == "TreeViewIdxTree.keyEvent" && keyEvent == QEvent::KeyRelease && strKey.compare("Escape",Qt::CaseInsensitive) == 0 )
        {
            pTreeEntrySrc = m_pIdxTree->findEntry("L:A0::B2::o2");
            pTreeEntryTrg = m_pIdxTree->findEntry("L:A0::B2::o4");

            if( pTreeEntrySrc != nullptr || pTreeEntryTrg == nullptr )
            {
                strResultValue = "L:A0::B2::o2 not renamed into L:A0::B2::o4";
                strlstResultValues.append(strResultValue);
            }
        }
    }
    else if( strParentGroupName.contains("Copy L:A0::B2::o4 -> B:A0::B2::C3",Qt::CaseInsensitive) )
    {
        if( strOperation == "TreeViewIdxTree.keyEvent" && keyEvent == QEvent::KeyRelease && strKey.compare("Ctrl-v",Qt::CaseInsensitive) == 0 )
        {
            pTreeEntryTrg = m_pIdxTree->findEntry("L:A0::B2::C3::o4");

            if( pTreeEntryTrg == nullptr )
            {
                strResultValue = "L:A0::B2::o4 not copied to L:A0::B2::C3";
                strlstResultValues.append(strResultValue);
            }
        }
    }
    else if( strParentGroupName.contains("Move L:A0::B2::o3 -> B:A0::B2::C3",Qt::CaseInsensitive) )
    {
        if( strOperation == "TreeViewIdxTree.keyEvent" && keyEvent == QEvent::KeyRelease && strKey.compare("Ctrl-v",Qt::CaseInsensitive) == 0 )
        {
            pTreeEntrySrc = m_pIdxTree->findEntry("L:A0::B2::o3");
            pTreeEntryTrg = m_pIdxTree->findEntry("L:A0::B2::C3::o3");

            if( pTreeEntrySrc != nullptr || pTreeEntryTrg == nullptr )
            {
                strResultValue = "L:A0::B2::o3 not moved to L:A0::B2::C3";
                strlstResultValues.append(strResultValue);
            }
        }
    }
    else if( strParentGroupName.contains("Delete L:A0::B2::C3::o3",Qt::CaseInsensitive) )
    {
        if( strOperation == "TreeViewIdxTree.keyEvent" && keyEvent == QEvent::KeyRelease && strKey.compare("Delete",Qt::CaseInsensitive) == 0 )
        {
            pTreeEntrySrc = m_pIdxTree->findEntry("L:A0::B2::C3::o3");

            if( pTreeEntrySrc != nullptr )
            {
                strResultValue = "L:A0::B2::C3::o3 not deleted";
                strlstResultValues.append(strResultValue);
            }
        }
    }
    else
    {
        strResultValue = "Unexpected parent group name \"" + strParentGroupName + "\"";
        strlstResultValues.append(strResultValue);
    }

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        strlstResultValues << "";
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepKeyboardInputs

//------------------------------------------------------------------------------
void CTest::doTestStepTreeViewContextMenus( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    QString strOperation = i_pTestStep->getOperation();

    // Examples for strOperation:
    // - "TreeViewIdxTree.select(L:A0::B2::C3::o1)"
    // - "TreeViewIdxTree.mouseEvent(Press LeftButton)"
    // - "TreeViewIdxTree.mouseEvent(Release LeftButton)"
    // - "TreeViewIdxTree.mouseEvent(Press RightButton)"
    // - "TreeViewIdxTree.mouseEvent(Release RightButton)"
    // - "TreeViewIdxTree.keyEvent(Press Down)"
    // - "TreeViewIdxTree.keyEvent(Release Down)"
    // - "TreeViewIdxTree.keyEvent(Press Enter)"
    // - "TreeViewIdxTree.keyEvent(Release Enter)"
    // - "TreeViewIdxTree.findLeave(L:A0::B2::C3::o1 copy)"
    // - "TreeViewIdxTree.findBranch(B:A0::B2::C3::D1)"

    QString         strKeyInTreeToBeSelected;
    Qt::MouseButton mouseButton = Qt::NoButton;
    QEvent::Type    mouseEvent = QEvent::None;
    QEvent::Type    keyEvent = QEvent::None;
    QString         strKey;

    if( strOperation.contains("TreeViewIdxTree.select") )
    {
        strKeyInTreeToBeSelected = strOperation;
        strKeyInTreeToBeSelected.remove("TreeViewIdxTree.select");
        strKeyInTreeToBeSelected.remove("(");
        strKeyInTreeToBeSelected.remove(")");

        strOperation = "TreeViewIdxTree.select";

    } // if( strOperation.contains("TreeViewIdxTree.select") )

    else if( strOperation.contains("TreeViewIdxTree.findLeave") )
    {
        strKeyInTreeToBeSelected = strOperation;
        strKeyInTreeToBeSelected.remove("TreeViewIdxTree.findLeave");
        strKeyInTreeToBeSelected.remove("(");
        strKeyInTreeToBeSelected.remove(")");

        strOperation = "TreeViewIdxTree.findLeave";

    } // if( strOperation.contains("TreeViewIdxTree.findLeave") )

    else if( strOperation.contains("TreeViewIdxTree.findBranch") )
    {
        strKeyInTreeToBeSelected = strOperation;
        strKeyInTreeToBeSelected.remove("TreeViewIdxTree.findBranch");
        strKeyInTreeToBeSelected.remove("(");
        strKeyInTreeToBeSelected.remove(")");

        strOperation = "TreeViewIdxTree.findBranch";

    } // if( strOperation.contains("TreeViewIdxTree.findBranch") )

    else if( strOperation.contains("TreeViewIdxTree.mouseEvent") )
    {
        QString strTmp = strOperation;
        strTmp.remove("TreeViewIdxTree.mouseEvent");
        strTmp.remove("(");
        strTmp.remove(")");

        if( strTmp.contains("Press") )
        {
            mouseEvent = QEvent::MouseButtonPress;
            strTmp.remove("Press");
        }
        else if( strTmp.contains("Release") )
        {
            mouseEvent = QEvent::MouseButtonRelease;
            strTmp.remove("Release");
        }
        if( strTmp.contains("LeftButton",Qt::CaseInsensitive) )
        {
            mouseButton = Qt::LeftButton;
        }
        else if( strTmp.contains("RightButton",Qt::CaseInsensitive) )
        {
            mouseButton = Qt::RightButton;
        }
        strOperation = "TreeViewIdxTree.mouseEvent";

    } // if( strOperation.contains("TreeViewIdxTree.mouseEvent") )

    else if( strOperation.contains("TreeViewIdxTree.keyEvent") )
    {
        QString strTmp = strOperation;
        strTmp.remove("TreeViewIdxTree.keyEvent");
        strTmp.remove("(");
        strTmp.remove(")");

        if( strTmp.contains("Press") )
        {
            keyEvent = QEvent::KeyPress;
            strTmp.remove("Press");
        }
        else if( strTmp.contains("Release") )
        {
            keyEvent = QEvent::KeyRelease;
            strTmp.remove("Release");
        }
        strKey = strTmp.trimmed();

        strOperation = "TreeViewIdxTree.keyEvent";

    } // if( strOperation.contains("TreeViewIdxTree.keyEvent") )

    strlstExpectedValues << strExpectedValue;

    // Test Step
    //----------

    CIdxTree*           pIdxTree         = m_pIdxTree;
    CMainWindow*        pMainWindow      = CMainWindow::GetInstance();
    CWdgtIdxTree*       pWdgtIdxTree     = pMainWindow->idxTreeWidget();
    CTreeViewIdxTree*   pTreeViewIdxTree = pWdgtIdxTree->treeView();
    CModelIdxTree*      pModelIdxTree    = dynamic_cast<CModelIdxTree*>(pTreeViewIdxTree->model());
    CModelIdxTreeEntry* pModelTreeEntry  = nullptr;
    CIdxTreeEntry*      pBranch          = nullptr;
    CIdxTreeEntry*      pLeave           = nullptr;

    if( strOperation == "TreeViewIdxTree.select" )
    {
        QModelIndex modelIdx = pModelIdxTree->index(strKeyInTreeToBeSelected, 0);
        pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(modelIdx.internalPointer());
        pTreeViewIdxTree->setCurrentIndex(modelIdx);
    }
    else if( strOperation == "TreeViewIdxTree.mouseEvent" )
    {
        QModelIndex modelIdx = pTreeViewIdxTree->currentIndex();
        QRect       rct = pTreeViewIdxTree->visualRect(modelIdx);
        QPoint      ptLocalPos(rct.x(), rct.y());

        QMouseEvent* pMouseEvent = new QMouseEvent(
            /* type      */ mouseEvent,
            /* ptPos     */ ptLocalPos,
            /* button    */ mouseButton,
            /* buttons   */ mouseButton,
            /* modifiers */ Qt::NoModifier );

        QApplication::postEvent(pTreeViewIdxTree->viewport(), pMouseEvent);
        // delete pMouseEvent; Deleted by Qt
        pMouseEvent = nullptr;

    } // if( strOperation == "TreeViewIdxTree.mouseEvent" )

    else if( strOperation == "TreeViewIdxTree.keyEvent" )
    {
        QWidget*   pWdgtReceiver = QApplication::activePopupWidget();
        QKeyEvent* pKeyEvent     = nullptr;

        if( pWdgtReceiver != nullptr && keyEvent != QEvent::None )
        {
            //pTreeViewIdxTree->setSilentlyExecuteDeleteRequests(true);
            //pTreeViewIdxTree->setSilentlyIgnoreInvalidCopyRequests(true);

            if( keyEvent == QEvent::KeyPress )
            {
                pKeyEvent = str2QKeyEvent(strKey, keyEvent);
            }
            else if( keyEvent == QEvent::KeyRelease )
            {
                pKeyEvent = str2QKeyEvent(strKey, keyEvent);
            }
            if( pKeyEvent != nullptr )
            {
                QApplication::postEvent(pWdgtReceiver, pKeyEvent);
                // delete pKeyEvent; Deleted by Qt
                pKeyEvent = nullptr;
            }
        } // if( pWdgtReceiver != nullptr && keyEvent != QEvent::None )
    } // if( strOperation == "TreeViewIdxTree.keyEvent" )

    else if( strOperation == "WidgetIdxTree.resizeColumnsToContents" )
    {
        for( int idxClm = 0; idxClm < CModelIdxTree::EColumnCount; idxClm++ )
        {
            pTreeViewIdxTree->resizeColumnToContents(idxClm);
        }
    }

    // Result Values
    //---------------

    ZS::Test::CTestStepGroup* pTSGrpParent = i_pTestStep->getParentGroup();

    QString strParentGroupName = pTSGrpParent->name();

    if( strParentGroupName.contains("Copy L:A0::B2::C3::o1 -> o1 copy",Qt::CaseInsensitive)
     || strParentGroupName.contains("Move L:A0::B2::C3::o1 Copy -> B:A0::B2",Qt::CaseInsensitive)
     || strParentGroupName.contains("Delete L:A0::B2::o1 Copy",Qt::CaseInsensitive)
     || strParentGroupName.contains("CreateBranch B:A0::B2::C3::New Branch",Qt::CaseInsensitive)
     || strParentGroupName.contains("CreateLeaves B:A0::B2::C3::New Branch::New Leaves",Qt::CaseInsensitive)
     || strParentGroupName.contains("Delete B:A0::B2::C3::New Branch",Qt::CaseInsensitive)
     || strParentGroupName.contains("Copy B:A0::B2::C3 -> B:A0::B2::C3 Copy",Qt::CaseInsensitive)
     || strParentGroupName.contains("Move B:A0::B2::C3 Copy -> B:A0::B2::C3::C3 Copy",Qt::CaseInsensitive)
     || strParentGroupName.contains("Delete B:A0::B2::C3::C3 Copy",Qt::CaseInsensitive) )
    {
        if( strOperation == "TreeViewIdxTree.select" )
        {
            QModelIndex modelIdx = pTreeViewIdxTree->currentIndex();
            QString     strKeyInTree;

            if( modelIdx.isValid() )
            {
                pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(modelIdx.internalPointer());
                strKeyInTree = pModelTreeEntry->keyInTree();
            }
            if( strKeyInTree != strKeyInTreeToBeSelected )
            {
                strResultValue = strKeyInTreeToBeSelected + " not selected";
                strlstResultValues.append(strResultValue);
            }
        } // if( strOperation == "TreeViewIdxTree.select" )

        else if( strOperation == "TreeViewIdxTree.mouseEvent" && mouseEvent == QEvent::MouseButtonRelease )
        {
        } // if( strOperation == "TreeViewIdxTree.mouseEvent" && mouseEvent == QEvent::MouseButtonRelease )

        else if( strOperation == "TreeViewIdxTree.keyEvent" && keyEvent == QEvent::KeyRelease && strKey == "Enter" )
        {
        } // if( strOperation == "TreeViewIdxTree.keyEvent" && keyEvent == QEvent::KeyRelease && strKey == "Enter" )

        else if( strOperation == "TreeViewIdxTree.findLeave" )
        {
            pLeave = pIdxTree->findLeave(strKeyInTreeToBeSelected);

            if( strParentGroupName.contains("Delete",Qt::CaseInsensitive) )
            {
                if( pLeave != nullptr )
                {
                    strResultValue = strKeyInTreeToBeSelected + " still exist";
                    strlstResultValues.append(strResultValue);
                }
            }
            else
            {
                if( pLeave == nullptr )
                {
                    strResultValue = strKeyInTreeToBeSelected + " not found";
                    strlstResultValues.append(strResultValue);
                }
            }
        } // if( strOperation == "TreeViewIdxTree.findLeave" )

        else if( strOperation == "TreeViewIdxTree.findBranch" )
        {
            pBranch = pIdxTree->findBranch(strKeyInTreeToBeSelected);

            if( strParentGroupName.contains("Delete",Qt::CaseInsensitive) )
            {
                if( pBranch != nullptr )
                {
                    strResultValue = strKeyInTreeToBeSelected + " still exist";
                    strlstResultValues.append(strResultValue);
                }
            }
            else
            {
                if( pBranch == nullptr )
                {
                    strResultValue = strKeyInTreeToBeSelected + " not found";
                    strlstResultValues.append(strResultValue);
                }
            }
        } // if( strOperation == "TreeViewIdxTree.findBranch" )
    } // if( strParentGroupName.contains("Copy L:A0::B2::C3::o1 -> o1 Copy",Qt::CaseInsensitive) )

    else
    {
        strResultValue = "Unexpected parent group name \"" + strParentGroupName + "\"";
        strlstResultValues.append(strResultValue);
    }

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        strlstResultValues << "";
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepTreeViewContextMenus

//------------------------------------------------------------------------------
void CTest::doTestStepGrpTrcAdmObjTreeStepTreeViewDragAndDrop( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    ZS::Test::CTestStepGroup* pTSGrpParent = i_pTestStep->getParentGroup();

    QString strParentGroupName = pTSGrpParent->name();

    // Please note !
    // -------------

    // Simulating drag and drop via mouse movements including moving the cursor via "QCursor::setPos"
    // doesn't work. If dragging is started mouse move events are no longer passed to the index tree view.
    // The operating system handles further dragging and dropping and mouse move events are passed to the
    // index tree via kind of a callback mechanism by the opertating system to Qt's event loop.
    // For this dragging and dropping is tested by directly sending Drag Events to the index tree view.

    // Example of parent group name:
    // "Group 3.1 TreeViewDragAndDrop.Move L:A4::B9::C4::D5::o5 -> B:A4::B9::C4"
    // Example of test step operation:
    // "TreeViewIdxTree.select(L:A4::B9::C4::D5::o5)"
    // "WidgetIdxTree.expandAll"
    // "WidgetIdxTree.resizeColumnsToContents"
    // "TreeViewIdxTree.select(L:A4::B9::C4::D5::o5)"
    // "TreeViewIdxTree.dragEnterEvent()",
    // "TreeViewIdxTree.dragLeaveEvent()",
    // "TreeViewIdxTree.dragMoveEvent(Up 10%)"
    // "TreeViewIdxTree.dropEvent()"

    QString strGrpOperation  = "TreeViewDragAndDrop";
    QString strStepOperation = i_pTestStep->getOperation();

    if( strParentGroupName.contains(strGrpOperation) )
    {
        QString strTmp;
        QString strKeyInTreeSrc;
        QString strKeyInTreeTrg;

        int idxStr = strParentGroupName.indexOf(strGrpOperation);
        strTmp = strParentGroupName;
        strTmp.remove(0, idxStr + strGrpOperation.length());
        strGrpOperation = strTmp;

        if( strGrpOperation.startsWith(".Move") )
        {
            QString strGrpArgs = strGrpOperation;
            strGrpArgs.remove(".Move");
            strGrpArgs.remove("(");
            strGrpArgs.remove(")");
            strGrpOperation = "Move";

            QStringList strlstGrpArgs = strGrpArgs.split("->", QtSkipEmptyParts);

            if( strlstGrpArgs.size() == 2 )
            {
                strKeyInTreeSrc = strlstGrpArgs[0].trimmed();
                strKeyInTreeTrg = strlstGrpArgs[1].trimmed();
            }
        } // if( strGrpOperation.startsWith(".Move") )

        static QPoint s_ptLocalPosMouseMoveStart;
        static QPoint s_ptLocalPosMouseMoveEnd;
        static QPoint s_ptLocalPosMouseMoveCurr;

        static QMimeData* s_pMimeData = nullptr;

        CIdxTree*         pIdxTree         = m_pIdxTree;
        CMainWindow*      pMainWindow      = CMainWindow::GetInstance();
        CWdgtIdxTree*     pWdgtIdxTree     = pMainWindow->idxTreeWidget();
        CTreeViewIdxTree* pTreeViewIdxTree = pWdgtIdxTree->treeView();
        CModelIdxTree*    pModelIdxTree    = dynamic_cast<CModelIdxTree*>(pTreeViewIdxTree->model());

        if( strStepOperation.startsWith("WidgetIdxTree.expandAll") )
        {
            pTreeViewIdxTree->expandAll();
            strlstExpectedValues << strExpectedValue;
            strlstResultValues << strResultValue;
        }
        else if( strStepOperation.startsWith("WidgetIdxTree.collapseAll") )
        {
            pTreeViewIdxTree->collapseAll();
            strlstExpectedValues << strExpectedValue;
            strlstResultValues << strResultValue;
        }
        else if( strStepOperation.startsWith("WidgetIdxTree.resizeColumnsToContents") )
        {
            for( int idxClm = 0; idxClm < CModelIdxTree::EColumnCount; idxClm++ )
            {
                pTreeViewIdxTree->resizeColumnToContents(idxClm);
            }
            strlstExpectedValues << strExpectedValue;
            strlstResultValues << strResultValue;
        }
        else if( strStepOperation.startsWith("TreeViewIdxTree.select") )
        {
            // The name of the group contains source and target entries.
            // Using the source and target entries the mouse move coordinates
            // (start and end position) got to be determined.
            // "select" must be performed before any mouse operation.

            QModelIndex modelIdxSrc = pModelIdxTree->index(strKeyInTreeSrc, 0);
            QRect rctSrc = pTreeViewIdxTree->visualRect(modelIdxSrc);

            s_ptLocalPosMouseMoveStart.setX(rctSrc.center().x());
            s_ptLocalPosMouseMoveStart.setY(rctSrc.center().y());

            s_ptLocalPosMouseMoveCurr.setX(rctSrc.center().x());
            s_ptLocalPosMouseMoveCurr.setY(rctSrc.center().y());

            QModelIndex modelIdxTrg = pModelIdxTree->index(strKeyInTreeTrg, 0);
            QRect rctTrg = pTreeViewIdxTree->visualRect(modelIdxTrg);

            s_ptLocalPosMouseMoveEnd.setX(rctTrg.center().x());
            s_ptLocalPosMouseMoveEnd.setY(rctTrg.center().y());

            pTreeViewIdxTree->setCurrentIndex(modelIdxSrc);

            QCursor::setPos(pTreeViewIdxTree->viewport()->mapToGlobal(s_ptLocalPosMouseMoveCurr));

            strExpectedValue = strKeyInTreeSrc;
            strlstExpectedValues << strExpectedValue;

            modelIdxSrc = pTreeViewIdxTree->currentIndex();

            CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(modelIdxSrc.internalPointer());

            if( pModelTreeEntry != nullptr )
            {
                strResultValue = pModelTreeEntry->keyInTree();
                strlstResultValues << strResultValue;
            }

        } // if( strStepOperation.startsWith("TreeViewIdxTree.select") )

        else if( strStepOperation.startsWith("TreeViewIdxTree.dragEnterEvent") )
        {
            strTmp = strStepOperation;
            // strTmp = "TreeViewIdxTree.dragEnterEvent()"
            strTmp.remove("TreeViewIdxTree.dragEnterEvent");
            strTmp.remove("(");
            strTmp.remove(")");
            // strTmp =  ""

            QModelIndexList indexes = pTreeViewIdxTree->selectedIndexes();

            s_pMimeData = pModelIdxTree->mimeData(indexes);

            QDragEnterEvent* pDragEvent = new QDragEnterEvent(
                /* pos       */ s_ptLocalPosMouseMoveCurr,
                /* actions   */ Qt::MoveAction,
                /* pMimeData */ s_pMimeData,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::KeyboardModifiers() );

            QApplication::postEvent(pTreeViewIdxTree->viewport(), pDragEvent);
            // delete pDragEvent; Deleted by Qt
            pDragEvent = nullptr;

            strExpectedValue = "Formats [1](application/vnd.text.list {" + strKeyInTreeSrc + "})";
            strlstExpectedValues << strExpectedValue;

            strResultValue = qMimeData2Str(s_pMimeData, 1);
            strlstResultValues << strResultValue;

        } // if( strStepOperation.startsWith("TreeViewIdxTree.dragEnterEvent") )

        else if( strStepOperation.startsWith("TreeViewIdxTree.dragMoveEvent") )
        {
            strTmp = strStepOperation;
            // strTmp = "TreeViewIdxTree.dragMoveEvent()"
            strTmp.remove("TreeViewIdxTree.dragMoveEvent");
            strTmp.remove("(");
            strTmp.remove(")");
            // strTmp =  "Up 10%"

            bool bConverted;
            int  xMoveOffs_px = 0;
            int  yMoveOffs_px = 0;

            if( strTmp.contains("Up",Qt::CaseInsensitive) )
            {
                strTmp.remove("Up");
                // strTmp =  " 10%"
                if( strTmp.contains("%",Qt::CaseInsensitive) )
                {
                    strTmp.remove("%");
                    // strTmp =  " 10"
                    int iMove_perCent = strTmp.toInt(&bConverted);
                    if( bConverted )
                    {
                        yMoveOffs_px = -(iMove_perCent * abs(s_ptLocalPosMouseMoveEnd.y() - s_ptLocalPosMouseMoveStart.y())) / 100;
                    }
                }
            }
            else if( strTmp.contains("Down",Qt::CaseInsensitive) )
            {
                strTmp.remove("Down");
                if( strTmp.contains("%",Qt::CaseInsensitive) )
                {
                    strTmp.remove("%");
                    int iMove_perCent = strTmp.toInt(&bConverted);
                    if( bConverted )
                    {
                        yMoveOffs_px = (iMove_perCent * abs(s_ptLocalPosMouseMoveEnd.y() - s_ptLocalPosMouseMoveStart.y())) / 100;
                    }
                }
            }

            s_ptLocalPosMouseMoveCurr.setX( s_ptLocalPosMouseMoveCurr.x() + xMoveOffs_px );
            s_ptLocalPosMouseMoveCurr.setY( s_ptLocalPosMouseMoveCurr.y() + yMoveOffs_px );

            QCursor::setPos(pTreeViewIdxTree->viewport()->mapToGlobal(s_ptLocalPosMouseMoveCurr));

            QDragMoveEvent* pDragEvent = new QDragMoveEvent(
                /* pos       */ s_ptLocalPosMouseMoveCurr,
                /* actions   */ Qt::MoveAction,
                /* pMimeData */ s_pMimeData,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::KeyboardModifiers() );

            QApplication::postEvent(pTreeViewIdxTree->viewport(), pDragEvent);
            // delete pDragEvent; Deleted by Qt
            pDragEvent = nullptr;

            strlstExpectedValues << strExpectedValue;
            strlstResultValues << strResultValue;

        } // if( strStepOperation.startsWith("TreeViewIdxTree.dragMoveEvent") )

        else if( strStepOperation.startsWith("TreeViewIdxTree.dropEvent") )
        {
            strTmp = strStepOperation;
            // strTmp = "TreeViewIdxTree.dropEvent()"
            strTmp.remove("TreeViewIdxTree.dropEvent");
            strTmp.remove("(");
            strTmp.remove(")");
            // strTmp =  ""

            QDropEvent* pDropEvent = new QDropEvent(
                /* pos       */ s_ptLocalPosMouseMoveCurr,
                /* actions   */ Qt::MoveAction,
                /* pMimeData */ s_pMimeData,
                /* buttons   */ Qt::LeftButton,
                /* modifiers */ Qt::KeyboardModifiers() );

            QApplication::postEvent(pTreeViewIdxTree->viewport(), pDropEvent);
            // delete pDropEvent; Deleted by Qt
            pDropEvent = nullptr;

            // Can't be deleted here as the drop event is handled later.
            // To avoid a memory leak the mime data is deleted by the drag leave event below.
            //delete s_pMimeData;
            //s_pMimeData = nullptr;

            // Please also note that to test whether the drag and drop was successfull we
            // cannot use "findLeave" here as the event will be passed to the IdxTreeView and will be
            // executed later by the IdxTreeView. We need a separate test step "findLeave" for this.

            strlstExpectedValues << strExpectedValue;
            strlstResultValues << strResultValue;

        } // if( strStepOperation.startsWith("TreeViewIdxTree.dropEvent") )

        else if( strStepOperation.startsWith("TreeViewIdxTree.dragLeaveEvent") )
        {
            strTmp = strStepOperation;
            // strTmp = "TreeViewIdxTree.dragLeaveEvent()"
            strTmp.remove("TreeViewIdxTree.dragLeaveEvent");
            strTmp.remove("(");
            strTmp.remove(")");
            // strTmp =  ""

            QDragLeaveEvent* pDragEvent = new QDragLeaveEvent();

            QApplication::postEvent(pTreeViewIdxTree->viewport(), pDragEvent);
            // delete pDragEvent; Deleted by Qt
            pDragEvent = nullptr;

            delete s_pMimeData;
            s_pMimeData = nullptr;

            strExpectedValue = "";
            strlstExpectedValues << strExpectedValue;

        } // if( strStepOperation.startsWith("TreeViewIdxTree.dragLeaveEvent") )

        else if( strStepOperation.startsWith("TreeViewIdxTree.findLeave") )
        {
            // "Group 3.1 TreeViewDragAndDrop.Move L:A4::B9::C4::D5::o5 -> B:A4::B9::C4"
            // strKeyInTreeSrc = "L:A4::B9::C4::D5::o5"
            // strKeyInTreeTrg = "B:A4::B9::C4"

            QString strBranchPathSrc;
            QString strObjNameSrc;
            QString strBranchPathTrg;
            QString strObjNameTrg;

            EIdxTreeEntryType entryTypeSrc = pIdxTree->splitPathStr(strKeyInTreeSrc, &strBranchPathSrc, &strObjNameSrc);
            pIdxTree->splitPathStr(strKeyInTreeTrg, &strBranchPathTrg, &strObjNameTrg);

            strKeyInTreeTrg = pIdxTree->buildKeyInTreeStr(entryTypeSrc, strBranchPathTrg, strObjNameTrg, strObjNameSrc);

            strExpectedValue = strKeyInTreeTrg;
            strlstExpectedValues << strExpectedValue;

            CIdxTreeEntry* pTreeEntry = pIdxTree->findLeave(strKeyInTreeTrg);

            if( pTreeEntry != nullptr )
            {
                strResultValue = pTreeEntry->keyInTree();
                strlstResultValues << strResultValue;
            }
            else
            {
                strResultValue = strKeyInTreeTrg + " not found";
                strlstResultValues << strResultValue;
            }
        } // if( strStepOperation.startsWith("TreeViewIdxTree.findLeave") )

        else if( strStepOperation.startsWith("TreeViewIdxTree.findBranch") )
        {
            // "Group 3.1 TreeViewDragAndDrop.Move L:A4::B9::C4::D5::o5 -> B:A4::B9::C4"
            // strKeyInTreeSrc = "L:A4::B9::C4::D5::o5"
            // strKeyInTreeTrg = "B:A4::B9::C4"

            QString strBranchPathSrc;
            QString strObjName;

            EIdxTreeEntryType entryTypeSrc = pIdxTree->splitPathStr(strKeyInTreeSrc, &strBranchPathSrc, &strObjName);

            strKeyInTreeTrg = pIdxTree->buildKeyInTreeStr(entryTypeSrc, strKeyInTreeTrg, strObjName);

            CIdxTreeEntry* pTreeEntry = pIdxTree->findLeave(strKeyInTreeTrg);

            if( pTreeEntry != nullptr )
            {
                strResultValue = pTreeEntry->keyInTree();
                strlstResultValues << strResultValue;
            }
            else
            {
                strResultValue = strKeyInTreeTrg + " not found";
                strlstResultValues << strResultValue;
            }
        } // if( strStepOperation.startsWith("TreeViewIdxTree.findBranch") )

        else // if( strStepOperation.startsWith("?") )
        {
            strResultValue = "Unsupported Operation " + strStepOperation;
            strlstResultValues.append(strResultValue);
        }
    } // if( strParentGroupName.contains("TreeViewDragAndDrop") )

    else
    {
        strResultValue = "Unexpected parent group name \"" + strParentGroupName + "\"";
        strlstResultValues.append(strResultValue);
    }

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        strlstResultValues << "";
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepGrpTrcAdmObjTreeStepTreeViewDragAndDrop

//------------------------------------------------------------------------------
void CTest::doTestStepDeleteTree( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strlstExpectedValues.append(strExpectedValue);

    i_pTestStep->setOperation("delete IdxTree");

    // Test Step
    //----------

    try
    {
        delete m_pIdxTree;  // slot "onIdxTreeDestroyed(m_pIdxTree)" is called
    }
    catch(...)
    {
        strResultValue = "Exception thrown";
    }

    m_pIdxTree = nullptr;

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        strlstResultValues << "";
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepDeleteTree

/*==============================================================================
public: // auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
STreeEntryDscr CTest::toTreeEntryDscr( const CIdxTreeEntry* i_pTreeEntry ) const
//------------------------------------------------------------------------------
{
    return STreeEntryDscr(i_pTreeEntry);
}

//------------------------------------------------------------------------------
QVector<STreeEntryDscr> CTest::toTreeEntryDscrs( const QVector<CIdxTreeEntry*>& i_arpTreeEntries ) const
//------------------------------------------------------------------------------
{
    QVector<STreeEntryDscr> arDscrs;

    for( auto& pTreeEntry : i_arpTreeEntries )
    {
        arDscrs.append( toTreeEntryDscr(pTreeEntry) );
    }
    return arDscrs;

} // toTreeEntryDscrs

/*==============================================================================
public: // auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
int CTest::indexOf(
    const QString&                 i_strKeyInTree,
    const QVector<STreeEntryDscr>& i_arTreeEntries ) const
//------------------------------------------------------------------------------
{
    int idxInTree = -1;

    for( int idx = 0; idx < i_arTreeEntries.size(); ++idx )
    {
        // Child of the tree entry to be removed ...
        if( i_arTreeEntries[idx].m_strKeyInTree == i_strKeyInTree )
        {
            idxInTree = idx;
            break;
        }
    }
    return idxInTree;

} // indexOf

//------------------------------------------------------------------------------
void CTest::swap(
    int                      i_idxInTree1,
    int                      i_idxInTree2,
    QVector<STreeEntryDscr>& i_arTreeEntries ) const
//------------------------------------------------------------------------------
{
    STreeEntryDscr treeEntryDscr1(i_arTreeEntries[i_idxInTree1]);
    STreeEntryDscr treeEntryDscr2(i_arTreeEntries[i_idxInTree2]);

    treeEntryDscr1.m_idxInTree = i_idxInTree2;
    treeEntryDscr2.m_idxInTree = i_idxInTree1;

    i_arTreeEntries[i_idxInTree1] = treeEntryDscr2;
    i_arTreeEntries[i_idxInTree2] = treeEntryDscr1;

} // swap

//------------------------------------------------------------------------------
int CTest::addEntry(
    const CIdxTree&          i_idxTree,
    const QString&           i_strKeyInTree,
    const QString&           i_strNameSpace,
    const QString&           i_strClassName,
    const QString&           i_strObjName,
    QVector<STreeEntryDscr>& i_arTreeEntries,
    QMap<int, int>&          i_mapFreeIdxs ) const
//------------------------------------------------------------------------------
{
    int idxInTree = -1;

    if( indexOf(i_strKeyInTree, i_arTreeEntries) < 0 )
    {
        EIdxTreeEntryType entryType = i_idxTree.splitPathStr(i_strKeyInTree, nullptr, nullptr);

        STreeEntryDscr treeEntryDscr;

        treeEntryDscr.m_entryType            = entryType;
        treeEntryDscr.m_strName              = i_strObjName;
        treeEntryDscr.m_strKeyInTree         = i_strKeyInTree;
        treeEntryDscr.m_idxInTree            = -1;
        treeEntryDscr.m_strParentBranchPath  = i_idxTree.buildPathStr(i_strNameSpace, i_strClassName);
        treeEntryDscr.m_strKeyInParentBranch = i_idxTree.buildKeyInTreeStr(entryType, i_strObjName);
        treeEntryDscr.m_idxInParentBranch    = -1;

        if( !i_strKeyInTree.isEmpty() )
        {
            // To add a branch or an AdminObj instance to the index tree the parent path must be known.
            QString strPath = i_idxTree.buildPathStr(i_strNameSpace, i_strClassName);

            // The current number of childs in the target branch must be determined.
            int idxInParentBranch = 0;
            for( auto& treeEntryDscrTmp : i_arTreeEntries )
            {
                // If the tree entry will become a sibling of the tree entry to be moved ...
                if( treeEntryDscrTmp.m_strParentBranchPath == treeEntryDscr.m_strParentBranchPath )
                {
                    idxInParentBranch++;
                }
            }
            treeEntryDscr.m_idxInParentBranch = idxInParentBranch;

            // Expected behaviour is to use free entries or to append the new entry at the end.
            if( i_mapFreeIdxs.size() > 0 )
            {
                #if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
                idxInTree = i_mapFreeIdxs.firstKey();
                #else
                idxInTree = i_mapFreeIdxs.begin().key();
                #endif
                i_mapFreeIdxs.remove(idxInTree);

                treeEntryDscr.m_idxInTree = idxInTree;

                if( idxInTree >= 0 && idxInTree < i_arTreeEntries.size() && i_arTreeEntries[idxInTree].m_entryType == EIdxTreeEntryType::Undefined )
                {
                    i_arTreeEntries[idxInTree] = treeEntryDscr;
                }
            }
            else
            {
                idxInTree = i_arTreeEntries.size();
                treeEntryDscr.m_idxInTree = idxInTree;
                i_arTreeEntries.append(treeEntryDscr);
            }
        } // if( !i_strKeyInTree.isEmpty() )
    } // if( indexOf(i_strKeyInTree, i_arTreeEntries) < 0 )

    return idxInTree;

} // addEntry

//------------------------------------------------------------------------------
void CTest::removeEntry(
    const CIdxTree&          i_idxTree,
    const STreeEntryDscr&    i_treeEntry,
    QVector<STreeEntryDscr>& i_arTreeEntries,
    QMap<int, int>&          i_mapFreeIdxs ) const
//------------------------------------------------------------------------------
{
    int idxInTree;

    if( i_treeEntry.m_entryType == EIdxTreeEntryType::Branch )
    {
        QString strParentPath = i_idxTree.buildPathStr(i_treeEntry.m_strParentBranchPath, i_treeEntry.m_strName);

        // First remove all child entries.
        for( auto& treeEntryDscrTmp : i_arTreeEntries )
        {
            // Child of the tree entry to be removed ...
            if( treeEntryDscrTmp.m_strParentBranchPath == strParentPath )
            {
                removeEntry(i_idxTree, treeEntryDscrTmp, i_arTreeEntries, i_mapFreeIdxs);
            }
        }
    } // if( i_treeEntry.m_entryType == EIdxTreeEntryType::Branch )

    // The "indexOf" function requires the value type to have an implementation of operator==()
    idxInTree = i_arTreeEntries.indexOf(i_treeEntry);

    if( idxInTree < 0 || idxInTree >= i_arTreeEntries.size() )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError, "Tree entry dscr " + i_treeEntry.toString() + " not in list");
    }
    else // if( idxInTree >= 0 && idxInTree < arTreeEntryDscrsExpected.size() )
    {
        // Expected behaviour is that the entry is removed from the index tree
        // and its parent branch. The index in the tree got to be added to the
        // map of free indices if it was not the last list entry.
        // The index in the parent branch of all following child entries got to
        // be corrected (decremented).

        // The index in the parent branch of all following child entries got to
        // be corrected (decremented).
        for( auto& treeEntryDscrTmp : i_arTreeEntries )
        {
            // Sibling of the tree entry to be removed ...
            if( treeEntryDscrTmp.m_strParentBranchPath == i_treeEntry.m_strParentBranchPath )
            {
                if( treeEntryDscrTmp.m_idxInParentBranch > i_treeEntry.m_idxInParentBranch )
                {
                    --treeEntryDscrTmp.m_idxInParentBranch;
                }
            }
        }

        // If the last entry in the index tree will be removed ..
        if( idxInTree == (i_arTreeEntries.size()-1) )
        {
            // .. the list will be resized and no entry will be added to the map of free indices.
            // Instead all free indices referencing penumltimate entries will also be removed from
            // the map of free indices (kind of garbage collection).
            i_arTreeEntries.removeLast();

            if( i_arTreeEntries.size() > 0 )
            {
                for( idxInTree = i_arTreeEntries.size()-1; idxInTree >= 0; --idxInTree )
                {
                    if( i_arTreeEntries[idxInTree].m_entryType == EIdxTreeEntryType::Undefined )
                    {
                        if( i_mapFreeIdxs.contains(idxInTree) )
                        {
                            i_mapFreeIdxs.remove(idxInTree);
                        }
                        i_arTreeEntries.removeLast();
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
        // If it is not the last entry in the index tree that will be removed ..
        else
        {
            // .. the entry in the index tree will be set to null and the index
            // will be added to the map of free entries.
            // !! Attention !! The passed tree entry descriptor might be a reference to the
            // list entry we are going to invalidate. After this call "i_treeEntry"
            // will no longer contain the original values and may become invalid.
            // For this the index in the parent branches got to be corrected before.
            i_arTreeEntries[idxInTree] = STreeEntryDscr();
            i_mapFreeIdxs.insert(idxInTree, idxInTree);
        }
    } // if( idxInTree >= 0 && idxInTree < arTreeEntryDscrsExpected.size() )

} // removeEntry

//------------------------------------------------------------------------------
void CTest::renameEntry(
    const ZS::System::CIdxTree& i_idxTree,
    const QString&              i_strNameNew,
    STreeEntryDscr&             i_treeEntry,
    QVector<STreeEntryDscr>&    i_arTreeEntries ) const
//------------------------------------------------------------------------------
{
    // The "indexOf" function requires the value type to have an implementation of operator==()
    int idxInTree = i_arTreeEntries.indexOf(i_treeEntry);

    if( idxInTree < 0 || idxInTree >= i_arTreeEntries.size() )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError, "Tree entry dscr " + i_treeEntry.toString() + " not in list");
    }
    else // if( idxInTree >= 0 && idxInTree < arTreeEntryDscrsExpected.size() )
    {
        QString strParentBranchPathPrev = i_treeEntry.m_strParentBranchPath;

        if( i_treeEntry.m_entryType == EIdxTreeEntryType::Branch )
        {
            strParentBranchPathPrev = i_idxTree.buildPathStr(i_treeEntry.m_strParentBranchPath, i_treeEntry.m_strName);
        }

        // Expected behaviour is that the entry keeps its type, its index in the tree
        // and its index in the parent branch. The name and the keys have to be renamed.
        i_treeEntry.m_strName              = i_strNameNew;
        i_treeEntry.m_strKeyInTree         = i_idxTree.buildKeyInTreeStr(i_treeEntry.m_entryType, i_treeEntry.m_strParentBranchPath, i_strNameNew);
        i_treeEntry.m_strKeyInParentBranch = i_idxTree.buildKeyInTreeStr(i_treeEntry.m_entryType, i_strNameNew);

        // !! Attention !! The passed tree entry descriptors might be references to the
        // list entry we are going to modify. After this call the passed tree entries
        // may no longer contain their original values.
        i_arTreeEntries[idxInTree] = i_treeEntry;

        // The keys and parent paths of all child entries of the renamed branch must be corrected.
        if( i_treeEntry.m_entryType == EIdxTreeEntryType::Branch )
        {
            QString strParentBranchPathNew = i_idxTree.buildPathStr(i_treeEntry.m_strParentBranchPath, i_treeEntry.m_strName);

            for( auto& treeEntryDscrTmp : i_arTreeEntries )
            {
                // If the tree entry was a child of the renamed branch ...
                if( treeEntryDscrTmp.m_strParentBranchPath == strParentBranchPathPrev )
                {
                    onParentBranchRenamed(i_idxTree, strParentBranchPathNew, strParentBranchPathPrev, treeEntryDscrTmp, i_arTreeEntries);
                }
            }
        }
    } // if( idxInTree >= 0 && idxInTree < arTreeEntryDscrsExpected.size() )

} // renameEntry

//------------------------------------------------------------------------------
void CTest::moveEntry(
    const CIdxTree&          i_idxTree,
    const STreeEntryDscr&    i_treeEntryTrg,
    STreeEntryDscr&          i_treeEntrySrc,
    QVector<STreeEntryDscr>& i_arTreeEntries ) const
//------------------------------------------------------------------------------
{
    if( i_treeEntryTrg.m_entryType == EIdxTreeEntryType::Branch )
    {
        QString strParentBranchPathPrev = i_treeEntrySrc.m_strParentBranchPath;
        QString strParentBranchPathNew  = i_idxTree.buildPathStr(i_treeEntryTrg.m_strParentBranchPath, i_treeEntryTrg.m_strName);

        if( i_treeEntrySrc.m_entryType == EIdxTreeEntryType::Branch )
        {
            strParentBranchPathPrev = i_idxTree.buildPathStr(i_treeEntrySrc.m_strParentBranchPath, i_treeEntrySrc.m_strName);
        }

        // The index in the previous parent branch of all following child entries got to be corrected (decremented).
        for( auto& treeEntryDscrTmp : i_arTreeEntries )
        {
            // Sibling of the tree entry to be moved ...
            if( treeEntryDscrTmp.m_strParentBranchPath == i_treeEntrySrc.m_strParentBranchPath )
            {
                if( treeEntryDscrTmp.m_idxInParentBranch > i_treeEntrySrc.m_idxInParentBranch )
                {
                    --treeEntryDscrTmp.m_idxInParentBranch;
                }
            }
        }

        // The current number of childs in the target branch must be determined.
        int idxInParentBranch = 0;
        for( auto& treeEntryDscrTmp : i_arTreeEntries )
        {
            // If the tree entry will become a sibling of the tree entry to be moved ...
            if( treeEntryDscrTmp.m_strParentBranchPath == strParentBranchPathNew )
            {
                idxInParentBranch++;
            }
        }

        i_treeEntrySrc.m_strKeyInTree        = i_idxTree.buildKeyInTreeStr(i_treeEntrySrc.m_entryType, strParentBranchPathNew, i_treeEntrySrc.m_strName);
        i_treeEntrySrc.m_strParentBranchPath = strParentBranchPathNew;
        i_treeEntrySrc.m_idxInParentBranch   = idxInParentBranch;

        // !! Attention !! The passed tree entry descriptors might be references to the
        // list entry we are going to modify. After this call the passed tree entries
        // may no longer contain their original values.
        i_arTreeEntries[i_treeEntrySrc.m_idxInTree] = i_treeEntrySrc;

        // The keys and parent paths of all child entries of the renamed branch must be corrected.
        if( i_treeEntrySrc.m_entryType == EIdxTreeEntryType::Branch )
        {
            strParentBranchPathNew = i_idxTree.buildPathStr(i_treeEntrySrc.m_strParentBranchPath, i_treeEntrySrc.m_strName);

            for( auto& treeEntryDscrTmp : i_arTreeEntries )
            {
                // If the tree entry was a child of the renamed branch ...
                if( treeEntryDscrTmp.m_strParentBranchPath == strParentBranchPathPrev )
                {
                    onParentBranchRenamed(i_idxTree, strParentBranchPathNew, strParentBranchPathPrev, treeEntryDscrTmp, i_arTreeEntries);
                }
            }
        }
    } // if( i_treeEntryTrg.m_entryType == EIdxTreeEntryType::Branch )

} // moveEntry

//------------------------------------------------------------------------------
void CTest::copyEntry(
    const CIdxTree&          i_idxTree,
    const STreeEntryDscr&    i_treeEntryTrg,
    STreeEntryDscr&          i_treeEntrySrc,
    QVector<STreeEntryDscr>& i_arTreeEntries,
    QMap<int, int>&          i_mapFreeIdxs ) const
//------------------------------------------------------------------------------
{
    if( i_treeEntryTrg.m_entryType == EIdxTreeEntryType::Branch )
    {
        QString strBranchPathTrg = i_idxTree.buildPathStr(i_treeEntryTrg.m_strParentBranchPath, i_treeEntryTrg.m_strName);

        // The name below the target group for each entry type must be unique.
        QString strObjNameTrg   = i_treeEntrySrc.m_strName;
        QString strKeyInTreeTrg = i_idxTree.buildKeyInTreeStr(i_treeEntrySrc.m_entryType, strBranchPathTrg, strObjNameTrg);
        int     idxInTreeTrg    = indexOf(strKeyInTreeTrg, i_arTreeEntries);

        if( idxInTreeTrg >= 0 )
        {
            int iCopies = 1;

            while( idxInTreeTrg >= 0 )
            {
                strObjNameTrg = i_treeEntrySrc.m_strName + " Copy";
                if( iCopies > 1 ) strObjNameTrg += " " + QString::number(iCopies);
                strKeyInTreeTrg = i_idxTree.buildKeyInTreeStr(i_treeEntrySrc.m_entryType, strBranchPathTrg, strObjNameTrg);
                idxInTreeTrg = indexOf(strKeyInTreeTrg, i_arTreeEntries);
                ++iCopies;
            }
        }

        // As the object name may have been corrected:
        strKeyInTreeTrg = i_idxTree.buildKeyInTreeStr(i_treeEntrySrc.m_entryType, strBranchPathTrg, strObjNameTrg);

        QString strNameSpace;
        QString strClassName;

        splitKey(i_idxTree, strKeyInTreeTrg, strNameSpace, strClassName, strObjNameTrg);

        idxInTreeTrg = addEntry(i_idxTree, strKeyInTreeTrg, strNameSpace, strClassName, strObjNameTrg, i_arTreeEntries, i_mapFreeIdxs);

        // All child entries of the source branch must be copied as well.
        if( i_treeEntrySrc.m_entryType == EIdxTreeEntryType::Branch && idxInTreeTrg >= 0 )
        {
            STreeEntryDscr& treeEntryTrg = i_arTreeEntries[idxInTreeTrg];

            QString strBranchPathSrc = i_idxTree.buildPathStr(i_treeEntrySrc.m_strParentBranchPath, i_treeEntrySrc.m_strName);

            for( auto& treeEntryDscrTmp : i_arTreeEntries )
            {
                // If the tree entry was a child of the renamed branch ...
                if( treeEntryDscrTmp.m_strParentBranchPath == strBranchPathSrc )
                {
                    copyEntry(i_idxTree, treeEntryTrg, treeEntryDscrTmp, i_arTreeEntries, i_mapFreeIdxs);
                }
            }
        } // if( idxInTree >= 0 )
    } // if( i_treeEntryTrg.m_entryType == EIdxTreeEntryType::Branch )

} // copyEntry

//------------------------------------------------------------------------------
void CTest::onParentBranchRenamed(
    const ZS::System::CIdxTree& i_idxTree,
    const QString&              i_strParentBranchPathNew,
    const QString&              i_strParentBranchPathPrev,
    STreeEntryDscr&             i_treeEntry,
    QVector<STreeEntryDscr>&    i_arTreeEntries ) const
//------------------------------------------------------------------------------
{
    // !! Attention !! The passed tree entry descriptors might be references to the
    // list entry we are going to modify. After this call the passed tree entries
    // may no longer contain their original values.
    i_treeEntry.m_strKeyInTree        = i_idxTree.buildKeyInTreeStr(i_treeEntry.m_entryType, i_strParentBranchPathNew, i_treeEntry.m_strName);
    i_treeEntry.m_strParentBranchPath = i_strParentBranchPathNew;

    i_arTreeEntries[i_treeEntry.m_idxInTree] = i_treeEntry;

    if( i_treeEntry.m_entryType == EIdxTreeEntryType::Branch )
    {
        QString strParentBranchPathPrev = i_idxTree.buildPathStr(i_strParentBranchPathPrev, i_treeEntry.m_strName);
        QString strParentBranchPathNew  = i_idxTree.buildPathStr(i_strParentBranchPathNew, i_treeEntry.m_strName);

        for( auto& treeEntryDscrTmp : i_arTreeEntries )
        {
            // If the tree entry was a child of the renamed branch ...
            if( treeEntryDscrTmp.m_strParentBranchPath == strParentBranchPathPrev )
            {
                onParentBranchRenamed(i_idxTree, strParentBranchPathNew, strParentBranchPathPrev, treeEntryDscrTmp, i_arTreeEntries);
            }
        }
    } // if( i_treeEntry.m_entryType == EIdxTreeEntryType::Branch )

} // onParentBranchRenamed

/*==============================================================================
public: // auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::splitKey(
    const CIdxTree& i_idxTree,
    const QString&  i_strKey,
    QString&        o_strNameSpace,
    QString&        o_strClassName,
    QString&        o_strObjName ) const
//------------------------------------------------------------------------------
{
    QString strKeyPath = i_strKey;

    // Remove Entry Type:
    strKeyPath.remove(0, 2);

    QStringList strlstPathNodes = strKeyPath.split(i_idxTree.nodeSeparator(), QtSkipEmptyParts);

    // For each of the path nodes separate NameSpace, ClassName and ObjName to create Admin Object Tree Entries.
    //----------------------------------------------------------------------------------------------------------

    // E.g. "B:A0:B0":
    // ---------------
    //
    //   Key                 | NameSpace   | ClassName | ObjName
    //   --------------------+-------------+-----------+--------
    //   B:A0:B0             |             | A0        | B0
    //   B:A1:B1:C1:D1       | A1:B1       | C1        | D1
    //   L:A2:B2:C2:D2:E2:o2 | A2:B2:C2:E2 | E2        | o2

    if( strlstPathNodes.size() == 0 )
    {
        o_strNameSpace = "";
        o_strClassName = "";
        o_strObjName   = "";
    }
    else if( strlstPathNodes.size() == 1 )
    {
        o_strNameSpace = "";
        o_strClassName = "";
        o_strObjName   = strlstPathNodes[0];
    }
    else if( strlstPathNodes.size() == 2 )
    {
        o_strNameSpace = "";
        o_strClassName = strlstPathNodes[0];
        o_strObjName   = strlstPathNodes[1];
    }
    else // if( strlstPathNodes.size() > 2 )
    {
        for( int idxPathNode = 0; idxPathNode < strlstPathNodes.size()-2; ++idxPathNode )
        {
            o_strNameSpace = i_idxTree.buildPathStr(o_strNameSpace, strlstPathNodes[idxPathNode]);
        }
        o_strClassName = strlstPathNodes[strlstPathNodes.size()-2];
        o_strObjName   = strlstPathNodes[strlstPathNodes.size()-1];
    }

} // splitKey

//------------------------------------------------------------------------------
void CTest::splitKey(
    const CIdxTree& i_idxTree,
    const QString&  i_strKey,
    QStringList&    o_strlstPathNodes,
    QStringList&    o_strlstKeysDefined,
    QStringList&    o_strlstNameSpacesDefined,
    QStringList&    o_strlstClassNamesDefined,
    QStringList&    o_strlstObjNamesDefined ) const
//------------------------------------------------------------------------------
{
    QString strKeyPath = i_strKey;

    // Remove Entry Type:
    strKeyPath.remove(0, 2);

    o_strlstPathNodes = strKeyPath.split(i_idxTree.nodeSeparator(), QtSkipEmptyParts);

    QString strPathNode;
    QString strPath;
    QString strKeyInTree;
    QString strNameSpace;
    QString strClassName;
    QString strObjName;

    // For each of the path nodes separate NameSpace, ClassName and ObjName to create Admin Object Tree Entries.
    //----------------------------------------------------------------------------------------------------------

    // E.g. "B:A0:B0":
    // ---------------
    //
    //   PathNodes[Idx]  | Keys    | NameSpaces | ClassNames | ObjNames
    //   ----------------+---------+------------+------------+---------
    //   A0 [0 = size-2] | B:A0    |            |            | A0
    //   B0 [1 = size-1] | B:A0:B0 |            | A0         | B0
    //
    // E.g. "B:A1:B1:C1:D1":
    // ---------------------
    //
    //  PathNodes[Idx]  | Keys          | NameSpaces | ClassNames | ObjNames
    //  ----------------+---------------+------------+------------+---------
    //  A1 [0 = size-4] | B:A1          |            |            | A1
    //  B1 [1 = size-3] | B:A1:B1       |            | A1         | B1
    //  C1 [2 = size-2] | B:A1:B1:C1    | A1         | B1         | C1
    //  D1 [3 = size-1] | B:A1:B1:C1:D1 | A1:B1      | C1         | D1
    //
    // E.g. "L:A2:B2:C2:D2:E2:o2":
    // ---------------------------
    //
    //   PathNodes[Idx]  | Keys                | NameSpaces  | ClassNames | ObjNames
    //   ----------------+---------------------+-------------+------------+---------
    //   A2 [0 = size-6] | B:A2                |             |            | A2
    //   B2 [1 = size-5] | B:A2:B2             |             | A2         | B2
    //   C2 [2 = size-4] | B:A2:B2:C2          | A2          | B2         | C2
    //   D2 [3 = size-3] | B:A2:B2:C2:D2       | A2:B2       | C2         | D2
    //   E2 [4 = size-2] | B:A2:B2:C2:D2:E2    | A2:B2:C2    | D2         | E2
    //   o2 [5 = size-1] | L:A2:B2:C2:D2:E2:o2 | A2:B2:C2:D2 | E2         | o2

    for( int idxPathNode = 0; idxPathNode < o_strlstPathNodes.size(); ++idxPathNode )
    {
        strPathNode = o_strlstPathNodes[idxPathNode];
        strPath     = i_idxTree.buildPathStr(strPath, strPathNode);

        // For all entries except the last one ...
        if( idxPathNode < (o_strlstPathNodes.size()-1) )
        {
            // ... a branch entry got to be created (if not yet existing).
            strKeyInTree = "B:" + strPath;
        }
        // For the last entry ...
        else // if( idxPathNode == (strlstPathNodes.size()-1) )
        {
            // ... either a branch or a leave entry got to be created (if not yet existing).
            if( i_strKey.startsWith("B:") )
            {
                strKeyInTree = "B:" + strPath;
            }
            else if( i_strKey.startsWith("L:") )
            {
                strKeyInTree = "L:" + strPath;
            }
        }

        // For all entries the object name is the node entry.
        strObjName = strPathNode;

        // For the first entry ...
        if( idxPathNode == 0 )
        {
            // .. ClassName and NameSpace is empty.
            strClassName = "";
            strNameSpace = "";
        }
        // For the second entry ...
        else if( idxPathNode == 1 )
        {
            // .. the class name is the previous entry and the name space is (still) empty.
            strClassName = o_strlstPathNodes[idxPathNode-1];
            strNameSpace = "";
        }
        // For all entries except the first and second one ...
        else // if( idxPathNode > 1 )
        {
            // .. the class name is the previous entry and the name space
            // got to be extended by the previous class name;
            strNameSpace = i_idxTree.buildPathStr(strNameSpace, strClassName);
            strClassName = o_strlstPathNodes[idxPathNode-1];
        }

        o_strlstKeysDefined       << strKeyInTree;
        o_strlstNameSpacesDefined << strNameSpace;
        o_strlstClassNamesDefined << strClassName;
        o_strlstObjNamesDefined   << strObjName;

    } // for( int idxPathNode = 0; idxPathNode < strlstPathNodes.size(); ++idxPathNode )

} // splitKey

//------------------------------------------------------------------------------
void CTest::compare(
    QVector<STreeEntryDscr>&       i_arTreeEntriesExpected,
    const QVector<STreeEntryDscr>& i_arTreeEntriesResult,
    QStringList&                   io_strlstExpectedValues,
    QStringList&                   io_strlstResultValues ) const
//------------------------------------------------------------------------------
{
    QString strExpectedValue;
    QString strResultValue;

    QString strKeyInTree;
    int     idxInTree;

    // Compare size of lists and add result to test step
    strExpectedValue = "TreeEntries [" + QString::number(i_arTreeEntriesExpected.size()) + "]";
    io_strlstExpectedValues.append(strExpectedValue);

    strResultValue = "TreeEntries [" + QString::number(i_arTreeEntriesResult.size()) + "]";
    io_strlstResultValues.append(strResultValue);

    // The order in which the entries are added to the index tree may be different from the
    // order the entries are added to the list of test descriptors.
    // On adding (copying) entries the test searches the list of test descriptors from start to end.
    // The index tree uses the order of the branches child entries. When deleting (removing)
    // entries free entry indices are created. On using those free entry indices a child entry
    // may be inserted in this test list before a child belonging to the same parent.
    // We need to reorganize the test list before we can compare the entries with the
    // entries in the index tree.

    bool bEntryCorrected = true;

    while( bEntryCorrected )
    {
        bEntryCorrected = false;

        for( idxInTree = 0; idxInTree < i_arTreeEntriesExpected.size(); ++idxInTree )
        {
            strKeyInTree = i_arTreeEntriesExpected[idxInTree].m_strKeyInTree;

            if( !strKeyInTree.isEmpty() )
            {
                int idxInTreeResult = indexOf(strKeyInTree, i_arTreeEntriesResult);

                if( idxInTreeResult != idxInTree )
                {
                    swap(idxInTree, idxInTreeResult, i_arTreeEntriesExpected);

                    bEntryCorrected = true;
                }
                else // if( idxInTreeResult == idxInTree )
                {
                    if( i_arTreeEntriesExpected[idxInTree].m_idxInParentBranch != i_arTreeEntriesResult[idxInTree].m_idxInParentBranch )
                    {
                        i_arTreeEntriesExpected[idxInTree].m_idxInParentBranch = i_arTreeEntriesResult[idxInTree].m_idxInParentBranch;
                        bEntryCorrected = true;
                    }
                }
            }
            if( bEntryCorrected )
            {
                break;
            }
        }
    }

    // Compare content of lists. Only the differences will be added to the test step result.
    for( idxInTree = 0; idxInTree < i_arTreeEntriesExpected.size(); ++idxInTree )
    {
        strExpectedValue = QString::number(idxInTree) + ": {";
        strExpectedValue += i_arTreeEntriesExpected[idxInTree].toString(true, true, true);
        strExpectedValue += "}";

        if( idxInTree >= i_arTreeEntriesResult.size() )
        {
            strResultValue = QString::number(idxInTree) + ": {-}";
        }
        else
        {
            strResultValue = QString::number(idxInTree) + ": {";
            strResultValue += i_arTreeEntriesResult[idxInTree].toString(true, true, true);
            strResultValue += "}";
        }

        if( strExpectedValue != strResultValue )
        {
            io_strlstExpectedValues.append(strExpectedValue);
            io_strlstResultValues.append(strResultValue);
        }
    } // for( idxInTree = 0; idxInTree < i_arTreeEntriesExpected.size(); ++idxInTree )

    for( ; idxInTree < i_arTreeEntriesResult.size(); ++idxInTree )
    {
        strExpectedValue = QString::number(idxInTree) + ": {-}";

        strResultValue = QString::number(i_arTreeEntriesResult[idxInTree].m_idxInTree) + ": {";
        strResultValue += i_arTreeEntriesResult[idxInTree].toString(true, true, true);
        strResultValue += "}";

        io_strlstExpectedValues.append(strExpectedValue);
        io_strlstResultValues.append(strResultValue);

    } // for( idxInTree = 0; idxInTree < i_arTreeEntriesResult.size(); ++idxInTree )

} // compare

//------------------------------------------------------------------------------
void CTest::compare(
    const QMap<int, int>& i_mapFreeIdxsExpected,
    const QMap<int, int>& i_mapFreeIdxsResult,
    QStringList&          io_strlstExpectedValues,
    QStringList&          io_strlstResultValues ) const
//------------------------------------------------------------------------------
{
    QString strExpectedValue;
    QString strResultValue;

    // Compare size of maps and add result to test step
    strExpectedValue = "FreeIdxs [" + QString::number(i_mapFreeIdxsExpected.size()) + "]";
    io_strlstExpectedValues.append(strExpectedValue);
    strResultValue = "FreeIdxs [" + QString::number(i_mapFreeIdxsResult.size()) + "]";
    io_strlstResultValues.append(strResultValue);

    bool bEqual = true;

    // Compare content of lists. If there are differences the content of the maps are added to the test step result.
    for( auto& idxFreeEntry : i_mapFreeIdxsExpected )
    {
        strExpectedValue = QString::number(idxFreeEntry);

        if( i_mapFreeIdxsResult.contains(idxFreeEntry) )
        {
            strResultValue = QString::number(idxFreeEntry);
        }
        else
        {
            strResultValue = "";
        }
        if( strExpectedValue != strResultValue )
        {
            bEqual = false;
            break;
        }
    } // for( auto& idxFreeEntry : i_mapFreeIdxsExpected )

    for( auto& idxFreeEntry : i_mapFreeIdxsResult )
    {
        strResultValue = QString::number(idxFreeEntry);

        if( i_mapFreeIdxsExpected.contains(idxFreeEntry) )
        {
            strExpectedValue = QString::number(idxFreeEntry);
        }
        else
        {
            strExpectedValue = "";
        }
        if( strExpectedValue != strResultValue )
        {
            bEqual = false;
            break;
        }
    } // for( auto& idxFreeEntry : i_mapFreeIdxsResult )

    if( !bEqual )
    {
        strExpectedValue = "{";
        for( auto& idxFreeEntry : i_mapFreeIdxsExpected )
        {
            if( strExpectedValue != "{" ) strExpectedValue += ", ";
            strExpectedValue += QString::number(idxFreeEntry);
        }
        strExpectedValue += "}";
        io_strlstExpectedValues.append(strExpectedValue);

        strResultValue = "{";
        for( auto& idxFreeEntry : i_mapFreeIdxsResult )
        {
            if( strResultValue != "{" ) strResultValue += ", ";
            strResultValue += QString::number(idxFreeEntry);
        }
        strResultValue += "}";
        io_strlstResultValues.append(strResultValue);

    } // if( !bEqual )

} // compare
