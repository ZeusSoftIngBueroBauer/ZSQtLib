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

#ifndef ZSTest_DllMain_h
#define ZSTest_DllMain_h

#include <QtCore/qstring.h>

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysCommon.h"

// specific export defines
#ifdef ZSTESTDLL_EXPORTS
    #define ZSTESTDLL_API __API_EXPORT
#else
    #define ZSTESTDLL_API __API_IMPORT
#endif


/*******************************************************************************
interface description of exported methods
*******************************************************************************/

// The static arrays "CEnum<>::s_arEnumEntries" are defined in the cpp file.
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4661 )
#elif defined __linux__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
#pragma GCC diagnostic pop
#endif

namespace ZS {
namespace Test {
//==============================================================================
/*! Possible results of test steps and the overall test.
*/
enum class ETestResult {
//==============================================================================
    Undefined   = 0,
    TestFailed  = 1,
    TestPassed  = 2,
    TestSkipped = 3
};
} }

// MinGW compile error: explicit instantiation of '<namespace>::<templated class>' must occur in namespace '<namespace>'
// Workaround: close namespaces before defining template class and define it outside namespaces.
template class ZSTESTDLL_API CEnum<ZS::Test::ETestResult>;
namespace ZS {
namespace Test {
typedef CEnum<ZS::Test::ETestResult> CEnumTestResult;
} }

namespace ZS {
namespace Test {
//==============================================================================
/*! Possible states of a test.
*/
enum class ETestState
//==============================================================================
{
    Idle    = 0,  /*! Test has not been started. */
    Running = 1,  /*! Test is running. */
    Paused  = 2   /*! Test has been paused. */
};
} }

// MinGW compile error: explicit instantiation of '<namespace>::<templated class>' must occur in namespace '<namespace>'
// Workaround: close namespaces before defining template class and define it outside namespaces.
template class ZSTESTDLL_API CEnum<ZS::Test::ETestState>;
namespace ZS {
namespace Test {
typedef CEnum<ZS::Test::ETestState> CEnumTestState;
} }

#ifdef _WINDOWS
#pragma warning( pop )
#endif

#endif // #ifndef ZSTest_DllMain_h
