/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#include "ZSSys/ZSSysTime.h"
#include "ZSSys/ZSSysErrResult.h"

#ifdef _WINDOWS
#include <windows.h>
#elif defined __linux__
#include <time.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::Time;

#ifdef _WINDOWS
static double    s_fClockRate = 0.0;
static double    s_fClockFirstCall = 0.0;
#endif
static double    s_fTimeFirstCallSec = 0.0;
static QDateTime s_dateTimeFirstCall;


/*******************************************************************************
Local functions prototypes
*******************************************************************************/

namespace ZS
{
namespace System
{
namespace Time
{
#ifdef _WINDOWS
static __int64 getProcessorClock();
#endif

} // namespace Time

} // namespace System

} // namespace ZS


/******************************************************************************/
class CInitModuleSysTime
/* Please note:
   The class name should be unique for the whole system. Otherwise the compiler
   may be confused and using a CInitModule class from other modules to create
   the static InitModule instance.
*******************************************************************************/
{
public: // ctor
    CInitModuleSysTime()
    {
        #ifdef _WINDOWS

        // Read clockrate from registry
        HKEY hKey;
        LONG lResult;

        lResult = RegOpenKeyEx( HKEY_LOCAL_MACHINE, L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey );
        if( lResult == ERROR_SUCCESS )
        {
            DWORD dwValType;
            DWORD dwValSize(sizeof(DWORD));
            DWORD dwVal;

            lResult = RegQueryValueEx( hKey, L"~MHz", 0, &dwValType, (BYTE*)&dwVal, &dwValSize );
            if( lResult == ERROR_SUCCESS )
            {
                if( dwValType == REG_DWORD )
                {
                    s_fClockRate = 1.0E6 * dwVal;
                }
            }
            RegCloseKey(hKey);
        }
        if( s_fClockRate == 0.0 )
        {
            throw "Time module cannot be used as it was not possible to determine the processors clock rate";
        }

        s_fClockFirstCall = getProcessorClock();
        s_fTimeFirstCallSec = static_cast<double>(static_cast<unsigned long int>(s_fClockFirstCall/s_fClockRate));

        #elif defined __linux__

        timespec timespecClock;

        if( clock_gettime(CLOCK_REALTIME,&timespecClock) == 0 )
        {
            s_fTimeFirstCallSec = timespecClock.tv_sec + (1.0e-9)*timespecClock.tv_nsec;
        }
        #endif

        s_dateTimeFirstCall = QDateTime::currentDateTime();
    }

}; // class CInitModuleSysTime

static CInitModuleSysTime s_initModuleSysTime;


/*******************************************************************************
Global functions implementations
*******************************************************************************/

//------------------------------------------------------------------------------
double ZS::System::Time::getProcTimeInSec()
//------------------------------------------------------------------------------
{
    #ifdef _MSC_VER
    double fClock = getProcessorClock();
    double fTimeSec = fClock/s_fClockRate - s_fTimeFirstCallSec;
    #else
    double fTimeSec = 0.0;
    timespec timespecClock;
    if( clock_gettime(CLOCK_REALTIME,&timespecClock) == 0 )
    {
        fTimeSec = timespecClock.tv_sec + (1.0e-9)*timespecClock.tv_nsec;
        fTimeSec -= s_fTimeFirstCallSec;
    }
    #endif
    return fTimeSec;

} // getProcTimeInSec

//------------------------------------------------------------------------------
double ZS::System::Time::getProcTimeInMilliSec()
//------------------------------------------------------------------------------
{
    return getProcTimeInSec()*1.0e3;
}

//------------------------------------------------------------------------------
double ZS::System::Time::getProcTimeInMicroSec()
//------------------------------------------------------------------------------
{
    return getProcTimeInSec()*1.0e6;
}

//------------------------------------------------------------------------------
QTime ZS::System::Time::sysTimeInSec2QTime( double i_fSysTime_s )
//------------------------------------------------------------------------------
{
    return s_dateTimeFirstCall.addSecs(static_cast<int>(i_fSysTime_s)).time();
}

//------------------------------------------------------------------------------
QTime ZS::System::Time::sysTimeInMilliSec2QTime( double i_fSysTime_ms )
//------------------------------------------------------------------------------
{
    return s_dateTimeFirstCall.addMSecs(static_cast<qint64>(i_fSysTime_ms)).time();
}

//------------------------------------------------------------------------------
QTime ZS::System::Time::sysTimeInMicroSec2QTime( double i_fSysTime_us )
//------------------------------------------------------------------------------
{
    double fSysTime_ms = i_fSysTime_us / 1000.0;
    return s_dateTimeFirstCall.addMSecs(static_cast<qint64>(fSysTime_ms)).time();
}

//------------------------------------------------------------------------------
QDateTime ZS::System::Time::sysTimeInSec2QDateTime( double i_fSysTime_s )
//------------------------------------------------------------------------------
{
    return s_dateTimeFirstCall.addSecs(static_cast<int>(i_fSysTime_s));
}

//------------------------------------------------------------------------------
QDateTime ZS::System::Time::sysTimeInMilliSec2QDateTime( double i_fSysTime_ms )
//------------------------------------------------------------------------------
{
    return s_dateTimeFirstCall.addMSecs(static_cast<qint64>(i_fSysTime_ms));
}

//------------------------------------------------------------------------------
QDateTime ZS::System::Time::sysTimeInMicroSec2QDateTime( double i_fSysTime_us )
//------------------------------------------------------------------------------
{
    double fSysTime_ms = i_fSysTime_us / 1000.0;
    return s_dateTimeFirstCall.addMSecs(static_cast<qint64>(fSysTime_ms));
}

#ifdef _WINDOWS
//------------------------------------------------------------------------------
__int64 ZS::System::Time::getProcessorClock()
//------------------------------------------------------------------------------
{
    #ifdef _WIN64
    return __rdtsc();
    #else
    __asm { RDTSC }
    #endif
}
#endif
