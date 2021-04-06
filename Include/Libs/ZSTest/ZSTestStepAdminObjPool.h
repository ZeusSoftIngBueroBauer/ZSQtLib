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

#ifndef ZSTest_TestStepAdminObjPool_h
#define ZSTest_TestStepAdminObjPool_h

#include "ZSTest/ZSTestDllMain.h"
#include "ZSSys/ZSSysObjPool.h"

class QXmlStreamReader;
class QXmlStreamWriter;

namespace ZS
{
namespace Test
{
class CTest;
class CTestStepGroup;
class CTestStep;

//******************************************************************************
class ZSTESTDLL_API CTestStepAdminObjPool : public ZS::System::CObjPool
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Test"; }
    static QString ClassName() { return "CTestStepAdminObjPool"; }
public: // ctors and dtor
    CTestStepAdminObjPool(
        CTest*         i_pTest,
        const QString& i_strObjPoolFileName,
        const QString& i_strNodeSeparator = "\\" );
    virtual ~CTestStepAdminObjPool();
signals:
    void testStepGroupInserted( QObject* i_pAdminObjPool, QObject* i_pTSGrp );
    void testStepGroupChanged( QObject* i_pAdminObjPool, QObject* i_pTSGrp );
    void testStepInserted( QObject* i_pAdminObjPool, QObject* i_pTestStep );
    void testStepChanged( QObject* i_pAdminObjPool, QObject* i_pTestStep );
    void nameSpaceNodeChanged( QObject* i_pAdminObjPool, ZS::System::CObjPoolTreeEntry* i_pTreeEntry, int i_enabled );
public: // overridables
    virtual QString nameSpace() const { return CTestStepAdminObjPool::NameSpace(); }
    virtual QString className() const { return CTestStepAdminObjPool::ClassName(); }
public: // instance methods
    CTest* getTest() { return m_pTest; }
    CTestStepGroup* getTestStepGroupRoot() { return m_pTSGrpRoot; }
public: // instance methods
    ZS::System::SErrResultInfo read_( const QString& i_strFileName );
    ZS::System::SErrResultInfo save_( const QString& i_strFileName = "" );
public: // instance methods
    int getTestStepsCount() const;
    int getTestStepsExecutedCount() const { return m_iTestStepsExecutedCount; }
    int getTestStepsPassedCount() const { return m_iTestStepsPassedCount; }
    int getTestStepsFailedCount() const { return m_iTestStepsFailedCount; }
    int getTestStepsIgnoredCount() const { return m_iTestStepsIgnoredCount; }
public: // instance methods
    QString buildPathStr( const QString& i_str1, const QString& i_str2 );
public: // instance methods to add, remove and modify test step groups
    CTestStepGroup* findTestStepGroup( const QString& i_strGroupName, const QString& i_strGroupPathParent );
    CTestStepGroup* findTestStepGroup( const QString& i_strGroupName, CTestStepGroup* i_pTestGroupParent );
    CTestStepGroup* addTestStepGroup(
        const QString&       i_strGroupName,
        const QString&       i_strGroupPathParent,
        ZS::System::EEnabled i_enabled = ZS::System::EEnabledUndefined );
    CTestStepGroup* addTestStepGroup(
        const QString&       i_strGroupName,
        CTestStepGroup*      i_pTestGroupParent,
        ZS::System::EEnabled i_enabled = ZS::System::EEnabledUndefined );
    void addTestStepGroup( CTestStepGroup* i_pTSGrp, CTestStepGroup* i_pTSGrpParent );
    CTestStepGroup* getTestStepGroup( // implicitely creates and adds the object if not yet existing
        const QString&       i_strGroupName,
        const QString&       i_strGroupPathParent,
        ZS::System::EEnabled i_enabled = ZS::System::EEnabledUndefined );
    CTestStepGroup* getTestStepGroup( // implicitely creates and adds the object if not yet existing
        const QString&       i_strGroupName,
        CTestStepGroup*      i_pTestGroupParent,
        ZS::System::EEnabled i_enabled = ZS::System::EEnabledUndefined );
    void onTestStepGroupChanged( CTestStepGroup* i_pTSGrp ); // triggers the "nodeChanged" and "dataChanged" signals
public: // instance methods to add, remove and modify test steps
    CTestStep* getTestStep( int i_iObjId );
    CTestStep* findTestStep( const QString& i_strName, const QString& i_strGroupPathParent );
    CTestStep* findTestStep( const QString& i_strName, CTestStepGroup* i_pTestGroupParent );
    CTestStep* addTestStep(
        const QString&       i_strName,
        const QString&       i_strGroupPathParent,
        ZS::System::EEnabled i_enabled = ZS::System::EEnabledUndefined );
    CTestStep* addTestStep(
        const QString&       i_strName,
        CTestStepGroup*      i_pTestGroupParent,
        ZS::System::EEnabled i_enabled = ZS::System::EEnabledUndefined );
    void addTestStep( CTestStep* i_pTestStep, CTestStepGroup* i_pTSGrpParent );
    CTestStep* getTestStep( // implicitely creates and adds the object if not yet existing
        const QString&       i_strName,
        const QString&       i_strGroupPathParent,
        ZS::System::EEnabled i_enabled = ZS::System::EEnabledUndefined,
        int                  i_iObjId = -1 );
    CTestStep* getTestStep( // implicitely creates and adds the object if not yet existing
        const QString&       i_strName,
        CTestStepGroup*      i_pTestGroupParent,
        ZS::System::EEnabled i_enabled = ZS::System::EEnabledUndefined,
        int                  i_iObjId = -1 );
public: // instance methods
    void onTestStepChanged( CTestStep* i_pTestStep ); // triggers the "nodeChanged" and "dataChanged" signals of the model
    void testStepStarted( CTestStep* i_pTestStep );
    void testStepEnded( CTestStep* i_pTestStep );
public: // instance methods
    void onTestGroupChanged( CTestStepGroup* i_pTSGrp ); // triggers the "nodeChanged" and "dataChanged" signals of the model
    void testGroupStarted( CTestStepGroup* i_pTSGrp );
    void testGroupEnded( CTestStepGroup* i_pTSGrp );
public: // instance methods to recursively modify test step objects via namespace node entries
    void updateGroup(
        const QString&       i_strNodeName,
        const QString&       i_strGroupNameParent,
        ZS::System::EEnabled i_enabled );
    void updateGroup(
        ZS::System::CObjPoolTreeEntry* i_pTreeEntry,
        ZS::System::EEnabled           i_enabled );
    int getUpdateGroupCallDepth() const;
public: // overridables of base class CObjPool
    virtual void clear(); // clears the whole tree and the list and destroys the test step objects
public: // overridables
    virtual void reset(); // to reset the test result of all test steps
protected: // instance methods
    void reset( ZS::System::CObjPoolTreeEntry* i_pTreeEntry ); // to reset the test result of all test steps below the specified node
public: // instance methods
    void beginInitTest();
    void endInitTest();
    bool isInitializingTest() const { return m_bInitializingTest; }
public: // instance methods
    void testStarted();
    void testEnded();
    void testAborted();
protected: // instance methods
    ZS::System::SErrResultInfo deserialize( QXmlStreamReader* i_pxmlStreamReader );
    ZS::System::SErrResultInfo serialize( QXmlStreamWriter* i_pxmlStreamWriter, CTestStepGroup* i_pTSGrpParent = nullptr ) const;
protected: // overridables
    virtual void clearTreeEntry( ZS::System::CObjPoolTreeEntry* i_pTreeEntry ); // clears the sub tree and destroys the test step objects
private: // copy ctor not allowed
    CTestStepAdminObjPool( const CTestStepAdminObjPool& );
private: // assignment operator not allowed
    CTestStepAdminObjPool& operator = ( const CTestStepAdminObjPool& );
protected: // instance members
    CTest*          m_pTest;
    CTestStepGroup* m_pTSGrpRoot;
    bool            m_bInitializingTest;
    int             m_iTestStepsExecutedCount;
    int             m_iTestStepsPassedCount;
    int             m_iTestStepsFailedCount;
    int             m_iTestStepsIgnoredCount;
    int             m_iUpdateGroupCallDepth;

}; // class CTestStepAdminObjPool

} // namespace Test

} // namespace ZS

#endif // #ifndef ZSTest_TestStepAdminObjPool_h
