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

#ifdef _WINDOWS
#include <windows.h>
#endif

#include <QtCore/qglobal.h>

#include "ZSTest/ZSTestDllMain.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Test;


/*******************************************************************************
exported methods
*******************************************************************************/

/*==============================================================================
Enum ETestResult
==============================================================================*/

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<ETestResult>::s_arEnumEntries;
//------------------------------------------------------------------------------
class CInitEnumTestResult
{
public: // ctor
    CInitEnumTestResult() {
        QVector<SEnumEntry>* pVEnumEntries = const_cast<QVector<SEnumEntry>*>(&CEnum<ETestResult>::s_arEnumEntries);
                                                   // Enumerator,                              Name,        Symbol,  Text,        SCIPShort, SCPILong
        pVEnumEntries->append( /* 0 */ SEnumEntry( static_cast<int>(ETestResult::Undefined),   "Undefined", "?",     "Undefined", "UND",     "UNDefined" ),
        pVEnumEntries->append( /* 1 */ SEnumEntry( static_cast<int>(ETestResult::TestFailed),  "Failed",    "ERR",   "Failed",    "FAIL",    "FAILed"    ),
        pVEnumEntries->append( /* 2 */ SEnumEntry( static_cast<int>(ETestResult::TestPassed),  "Passed",    "OK",    "Passed",    "PASS",    "PASSed"    ),
        pVEnumEntries->append( /* 3 */ SEnumEntry( static_cast<int>(ETestResult::TestSkipped), "Skipped",    "-",    "Skipped",   "SKIP",    "SKIPped"   )
    }
};
static CInitEnumTestResult s_initEnumTestResult;
#else // #if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> CEnum<ETestResult>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( static_cast<int>(ETestResult::Undefined),   "Undefined", "?",     "Undefined", "UND",     "UNDefined" ),
    /* 1 */ SEnumEntry( static_cast<int>(ETestResult::TestFailed),  "Failed",    "ERR",   "Failed",    "FAIL",    "FAILed"    ),
    /* 2 */ SEnumEntry( static_cast<int>(ETestResult::TestPassed),  "Passed",    "OK",    "Passed",    "PASS",    "PASSed"    ),
    /* 3 */ SEnumEntry( static_cast<int>(ETestResult::TestSkipped), "Skipped",   "-",     "Skipped",   "SKIP",    "SKIPped"   )
};
#endif // #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)


/*==============================================================================
Enum ETestState
==============================================================================*/

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<ETestState>::s_arEnumEntries;
//------------------------------------------------------------------------------
class CInitEnumTestState
{
public: // ctor
    CInitEnumTestState() {
        QVector<SEnumEntry>* pVEnumEntries = const_cast<QVector<SEnumEntry>*>(&CEnum<ETestState>::s_arEnumEntries);
                                                     // Enumerator,                       Name,      Symbol,    Text,      SCIPShort, SCPILong
        pVEnumEntries->append( /* 0 */ SEnumEntry( static_cast<int>(ETestState::Idle),    "Idle",    "Idle",    "Idle",    "IDLE",    "IDLE"    );
        pVEnumEntries->append( /* 1 */ SEnumEntry( static_cast<int>(ETestState::Running), "Running", "Running", "Running", "RUNN",    "RUNNing" );
        pVEnumEntries->append( /* 2 */ SEnumEntry( static_cast<int>(ETestState::Paused),  "Paused",  "Paused",  "Paused",  "PAUS",    "PAUSed"  );
    }
};
static CInitEnumTestState s_initEnumTestState;
#else // #if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
//------------------------------------------------------------------------------
template<> const QVector<SEnumEntry> CEnum<ETestState>::s_arEnumEntries =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( static_cast<int>(ETestState::Idle),    "Idle",    "Idle",    "Idle",    "IDLE",    "IDLE"    ),
    /* 1 */ SEnumEntry( static_cast<int>(ETestState::Running), "Running", "Running", "Running", "RUNN",    "RUNNing" ),
    /* 2 */ SEnumEntry( static_cast<int>(ETestState::Paused),  "Paused",  "Paused",  "Paused",  "PAUS",    "PAUSed"  )
};
#endif // #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)


/*******************************************************************************
Entry point for the DLL application.
*******************************************************************************/

#ifdef _WINDOWS
//------------------------------------------------------------------------------
BOOL APIENTRY DllMain(
    HANDLE /*i_hModule*/,
    DWORD  i_ul_reason_for_call,
    LPVOID /*i_lpReserved*/ )
//------------------------------------------------------------------------------
{
    switch( i_ul_reason_for_call )
    {
        case DLL_PROCESS_ATTACH:
        {
            break;
        }
        case DLL_THREAD_ATTACH:
        {
            break;
        }
        case DLL_THREAD_DETACH:
        {
            break;
        }
        case DLL_PROCESS_DETACH:
        {
            break;
        }
        default:
        {
            break;
        }
    }
    return TRUE;

} // DllMain
#endif
