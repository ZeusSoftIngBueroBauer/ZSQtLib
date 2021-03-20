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

#ifndef ZSTest_TestStepGroup_h
#define ZSTest_TestStepGroup_h

#include "ZSTest/ZSTestStepAdminObj.h"

namespace ZS
{
namespace Test
{
class CTestStep;

//******************************************************************************
class ZSTESTDLL_API CTestStepGroup : public CTestStepAdminObj
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Test"; }
    static QString ClassName() { return "CTestStepGroup"; }
public: // ctor (obsolete)
    CTestStepGroup(
        CTestStepAdminObjPool*         i_pObjPool,
        const QString&                 i_strName,
        ZS::System::CObjPoolTreeEntry* i_pTreeEntry );
public: // ctors and dtor
    CTestStepGroup(
        CTest*          i_pTest,
        const QString&  i_strName,
        CTestStepGroup* i_pTSGrpParent = nullptr );
    virtual ~CTestStepGroup();
public: // overridables
    virtual QString nameSpace() const { return CTestStepGroup::NameSpace(); }
    virtual QString className() const { return CTestStepGroup::ClassName(); }
public: // must overridables of base class CTestStepAdminObj
    virtual bool isGroup() const { return true; }
public: // must overridables of base class CTestStepAdminObj
    virtual void setTestResult( ETestResult i_testResult );
public: // must overridables of base class CTestStepAdminObj
    virtual void testStarted();
    virtual void testEnded( bool i_bIgnoreTestResult = false ); // Implicitly updates test end time if not already updated.
public: // instance methods
    int getTestStepGroupCount();
    CTestStepGroup* getTestStepGroup( int i_iTestStepGroupIdx );
public: // instance methods
    int getTestStepCount( bool i_bIncludeChildGroups = false);
    CTestStep* getTestStep( int i_iTestStepIdx/*, bool i_bIncludeChildGroups = false*/);
    //CTestStep* getTestStep( int& io_iTestStepCount, int i_iTestStepIdx, bool i_bIncludeChildGroups = false );
public: // instance methods
    bool isParentOf( CTestStep* i_pTestStep ) const;
    bool isFirstTestStep( CTestStep* i_pTestStep ) const;
    bool isLastTestStep( CTestStep* i_pTestStep ) const;
    bool isFirstTestGroup( CTestStepGroup* i_pTSGrp ) const;
    bool isLastTestGroup( CTestStepGroup* i_pTSGrp ) const;
public: // must overridables of base class CTestStepAdminObj
    virtual void update(); // triggers the "nodeChanged" and "dataChanged" signals of the model
private: // default ctor not allowed
    CTestStepGroup();
private: // copy ctor not allowed
    CTestStepGroup( const CTestStepGroup& );

}; // class CTestStepGroup

} // namespace Test

} // namespace ZS

#endif // #ifndef ZSTest_TestStepAdminObj_h
