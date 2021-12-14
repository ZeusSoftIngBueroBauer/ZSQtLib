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

#ifndef ZSTest_TestStepIdxTree_h
#define ZSTest_TestStepIdxTree_h

#include "ZSTest/ZSTestDllMain.h"
#include "ZSSys/ZSSysIdxTree.h"

class QXmlStreamReader;
class QXmlStreamWriter;

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}

namespace Test
{
class CTest;
class CTestStepGroup;
class CTestStep;

//******************************************************************************
class ZSTESTDLL_API CTestStepIdxTree : public ZS::System::CIdxTree
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Test"; }
    static QString ClassName() { return "CTestStepIdxTree"; }
public: // ctors and dtor
    CTestStepIdxTree( CTest* i_pTest );
    virtual ~CTestStepIdxTree();
signals:
    //void testStepGroupInserted( QObject* i_pAdminIdxTree, QObject* i_pTSGrp );
    //void testStepGroupChanged( QObject* i_pAdminIdxTree, QObject* i_pTSGrp );
    //void testStepInserted( QObject* i_pAdminIdxTree, QObject* i_pTestStep );
    //void testStepChanged( QObject* i_pAdminIdxTree, QObject* i_pTestStep );
    //void nameSpaceNodeChanged( QObject* i_pAdminIdxTree, ZS::System::#error CAbstractIdxTreeEntry* i_pTreeEntry, int i_enabled );
public: // overridables
    virtual QString nameSpace() const { return CTestStepIdxTree::NameSpace(); }
    virtual QString className() const { return CTestStepIdxTree::ClassName(); }
public: // instance methods
    //int getTestStepsCount() const;
    //int getTestStepsExecutedCount() const { return m_iTestStepsExecutedCount; }
    //int getTestStepsPassedCount() const { return m_iTestStepsPassedCount; }
    //int getTestStepsFailedCount() const { return m_iTestStepsFailedCount; }
    //int getTestStepsIgnoredCount() const { return m_iTestStepsIgnoredCount; }
public: // instance methods find test step groups
    CTestStepGroup* findTestStepGroup( const QString& i_strParentPath, const QString& i_strGroupName);
    CTestStepGroup* findTestStepGroup( CTestStepGroup* i_pTestGroupParent, const QString& i_strGroupName );
public: // instance methods to add, remove and modify test step groups
    //CTestStepGroup* addTestStepGroup(
    //    const QString&       i_strGroupName,
    //    const QString&       i_strParentPath,
    //    ZS::System::EEnabled i_enabled = ZS::System::EEnabled::Undefined );
    //CTestStepGroup* addTestStepGroup(
    //    const QString&       i_strGroupName,
    //    CTestStepGroup*      i_pTestGroupParent,
    //    ZS::System::EEnabled i_enabled = ZS::System::EEnabled::Undefined );
    //void addTestStepGroup( CTestStepGroup* i_pTSGrp, CTestStepGroup* i_pTSGrpParent );
    //CTestStepGroup* getTestStepGroup( // implicitely creates and adds the object if not yet existing
    //    const QString&       i_strGroupName,
    //    const QString&       i_strParentPath,
    //    ZS::System::EEnabled i_enabled = ZS::System::EEnabled::Undefined );
    //CTestStepGroup* getTestStepGroup( // implicitely creates and adds the object if not yet existing
    //    const QString&       i_strGroupName,
    //    CTestStepGroup*      i_pTestGroupParent,
    //    ZS::System::EEnabled i_enabled = ZS::System::EEnabled::Undefined );
    //void onTestStepGroupChanged( CTestStepGroup* i_pTSGrp ); // triggers the "nodeChanged" and "dataChanged" signals
public: // instance methods to find test steps
    //CTestStep* getTestStep( int i_idxObj);
    CTestStep* findTestStep( const QString& i_strParentPath, const QString& i_strName );
    CTestStep* findTestStep( CTestStepGroup* i_pTestGroupParent, const QString& i_strName );
public: // instance methods to add, remove and modify test steps
    //CTestStep* addTestStep(
    //    const QString&       i_strName,
    //    const QString&       i_strParentPath,
    //    ZS::System::EEnabled i_enabled = ZS::System::EEnabled::Undefined );
    //CTestStep* addTestStep(
    //    CTestStepGroup*      i_pTestGroupParent,
    //    const QString&       i_strName,
    //    ZS::System::EEnabled i_enabled = ZS::System::EEnabled::Undefined );
    //void addTestStep(CTestStepGroup* i_pTSGrpParent, CTestStep* i_pTestStep );
public: // instance methods to add, remove and modify test steps
    //CTestStep* getTestStep( // implicitely creates and adds the object if not yet existing
    //    const QString&       i_strParentPath,
    //    const QString&       i_strName,
    //    ZS::System::EEnabled i_enabled = ZS::System::EEnabled::Undefined,
    //    int                  i_idxObj = -1 );
    //CTestStep* getTestStep( // implicitely creates and adds the object if not yet existing
    //    CTestStepGroup*      i_pTestGroupParent,
    //    const QString&       i_strName,
    //    ZS::System::EEnabled i_enabled = ZS::System::EEnabled::Undefined,
    //    int                  i_idxObj = -1 );
public: // instance methods
    //void onTestStepChanged( CTestStep* i_pTestStep ); // triggers the "nodeChanged" and "dataChanged" signals of the model
    //void testStepStarted( CTestStep* i_pTestStep );
    //void testStepEnded( CTestStep* i_pTestStep );
public: // instance methods
    //void onTestGroupChanged( CTestStepGroup* i_pTSGrp ); // triggers the "nodeChanged" and "dataChanged" signals of the model
    //void testGroupStarted( CTestStepGroup* i_pTSGrp );
    //void testGroupEnded( CTestStepGroup* i_pTSGrp );
public: // instance methods to recursively modify test step objects via namespace node entries
    //void updateGroup(
    //    const QString&       i_strGroupNameParent,
    //    const QString&       i_strNodeName,
    //    ZS::System::EEnabled i_enabled );
    //void updateGroup(
    //    ZS::System::#error CAbstractIdxTreeEntry* i_pTreeEntry,
    //    ZS::System::EEnabled           i_enabled );
    //int getUpdateGroupCallDepth() const;
public: // overridables
    //virtual void reset(); // to reset the test result of all test steps
protected: // instance methods
    //void reset( ZS::System::#error CAbstractIdxTreeEntry* i_pTreeEntry ); // to reset the test result of all test steps below the specified node
public: // instance methods
    //void beginInitTest();
    //void endInitTest();
    //bool isInitializingTest() const { return m_bInitializingTest; }
public: // instance methods
    //void testStarted();
    //void testEnded();
    //void testAborted();
public: // overridables
    virtual ZS::System::SErrResultInfo save( const QString& i_strAbsFilePath ) const;
    virtual ZS::System::SErrResultInfo recall( const QString& i_strAbsFilePath );
protected: // auxiliary instance methods
    virtual void save( QXmlStreamWriter& xmlStreamWriter, ZS::System::CIdxTreeEntry* i_pTreeEntry ) const;
private: // copy ctor not allowed
    CTestStepIdxTree( const CTestStepIdxTree& );
private: // assignment operator not allowed
    CTestStepIdxTree& operator = ( const CTestStepIdxTree& );
protected: // instance members
    CTest*                   m_pTest;
    bool                     m_bInitializingTest;
    int                      m_iTestStepsExecutedCount;
    int                      m_iTestStepsPassedCount;
    int                      m_iTestStepsFailedCount;
    int                      m_iTestStepsIgnoredCount;
    int                      m_iUpdateGroupCallDepth;
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CTestStepIdxTree

} // namespace Test

} // namespace ZS

#endif // #ifndef ZSTest_TestStepIdxTree_h
