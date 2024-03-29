/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
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

#include "ZSTest/ZSTestStepIdxTreeEntry.h"

namespace ZS
{
namespace Test
{
//******************************************************************************
class ZSTESTDLL_API CTestStepGroup : public CAbstractTestStepIdxTreeEntry
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Test"; }
    static QString ClassName() { return "CTestStepGroup"; }
public: // ctors and dtor
    CTestStepGroup(
        CTest*          i_pTest,
        const QString&  i_strName,
        CTestStepGroup* i_pTSGrpParent = nullptr );
    virtual ~CTestStepGroup();
protected: // ctor for class CTestGroupRoot
    CTestStepGroup(
        CTest* i_pTest,
        ZS::System::CIdxTreeEntry::EEntryType i_entryType,
        const QString& i_strName );
public: // instance methods
    void onTestStepResultChanged( CAbstractTestStepIdxTreeEntry* i_pTreeEntry, const CEnumTestResult& i_testResult );
public: // must overridables of base class CAbstractTestStepIdxTreeEntry
    virtual CEnumTestResult getTestResult() const override;
    virtual double getTestDurationInSec() const override;
private: // default ctor not allowed
    CTestStepGroup();
private: // copy ctor not allowed
    CTestStepGroup( const CTestStepGroup& );

}; // class CTestStepGroup

} // namespace Test

} // namespace ZS

#endif // #ifndef ZSTest_TestStepGroup_h
