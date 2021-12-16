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

#include "ZSTest/ZSTestStepRoot.h"
//#include "ZSTest/ZSTest.h"
//#include "ZSTest/ZSTestStep.h"
//#include "ZSTest/ZSTestStepIdxTree.h"
//
//#include "ZSSys/ZSSysErrResult.h"
//#include "ZSSys/ZSSysException.h"
//#include "ZSSys/ZSSysMath.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Test;


/*******************************************************************************
class CTestStepRoot : public CTestStepGroup
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! Constructs the root entry of the test step index tree.

    The entry will be added to the index tree of the passed test instance.

    @param i_pTest [in] Reference to test the entry belongs to (must not be nullptr).
    @param i_strName [in] Name of the entry.
    @param i_pTSGrpParent [in] Parent test group or nullptr, if the entry does not have a parent.
*/
CTestStepRoot::CTestStepRoot(
    CTest*          i_pTest,
    const QString&  i_strName ) :
//------------------------------------------------------------------------------
    CTestStepGroup(i_pTest, EIdxTreeEntryType::Root, i_strName)
{
} // ctor

//------------------------------------------------------------------------------
CTestStepRoot::~CTestStepRoot()
//------------------------------------------------------------------------------
{
} // dtor
