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

#ifndef ZSTest_TestStepRoot_h
#define ZSTest_TestStepRoot_h

#include "ZSTest/ZSTestStepGroup.h"

namespace ZS
{
namespace Test
{
//******************************************************************************
class ZSTESTDLL_API CTestStepRoot : public CTestStepGroup
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Test"; }
    static QString ClassName() { return "CTestStepRoot"; }
public: // ctors and dtor
    CTestStepRoot( CTest* i_pTest, const QString&  i_strName );
    virtual ~CTestStepRoot();
public: // overridables
    virtual QString nameSpace() const { return CTestStepRoot::NameSpace(); }
    virtual QString className() const { return CTestStepRoot::ClassName(); }
private: // default ctor not allowed
    CTestStepRoot();
private: // copy ctor not allowed
    CTestStepRoot( const CTestStepRoot& );

}; // class CTestStepRoot

} // namespace Test

} // namespace ZS

#endif // #ifndef ZSTest_TestStepRoot_h
