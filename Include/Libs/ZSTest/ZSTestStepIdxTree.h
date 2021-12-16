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
public: // overridables
    virtual QString nameSpace() const { return CTestStepIdxTree::NameSpace(); }
    virtual QString className() const { return CTestStepIdxTree::ClassName(); }
public: // instance methods
    CTest* getTest() { return m_pTest; }
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
public: // instance methods to find test steps
    CTestStep* findTestStep( const QString& i_strParentPath, const QString& i_strName );
    CTestStep* findTestStep( CTestStepGroup* i_pTestGroupParent, const QString& i_strName );
public: // overridables
    virtual void reset(); // to reset the test result of all test steps
protected: // instance methods
    void reset( ZS::System::CIdxTreeEntry* i_pTreeEntry );
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
    //bool                     m_bInitializingTest;
    //int                      m_iTestStepsExecutedCount;
    //int                      m_iTestStepsPassedCount;
    //int                      m_iTestStepsFailedCount;
    //int                      m_iTestStepsIgnoredCount;
    //int                      m_iUpdateGroupCallDepth;
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CTestStepIdxTree

} // namespace Test

} // namespace ZS

#endif // #ifndef ZSTest_TestStepIdxTree_h
