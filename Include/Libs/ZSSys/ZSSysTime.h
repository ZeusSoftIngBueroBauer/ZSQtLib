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

#ifndef ZSSys_Time_h
#define ZSSys_Time_h

#include <QtCore/qdatetime.h>

#include "ZSSys/ZSSysDllMain.h"

namespace ZS
{
namespace System
{
namespace Time
{
double ZSSYSDLL_API getProcTimeInSec();
double ZSSYSDLL_API getProcTimeInMilliSec();
double ZSSYSDLL_API getProcTimeInMicroSec();

QTime ZSSYSDLL_API sysTimeInSec2QTime( double i_fSysTime_s );
QTime ZSSYSDLL_API sysTimeInMilliSec2QTime( double i_fSysTime_ms );
QTime ZSSYSDLL_API sysTimeInMicroSec2QTime( double i_fSysTime_us );

QDateTime ZSSYSDLL_API sysTimeInSec2QDateTime( double i_fSysTime_s );
QDateTime ZSSYSDLL_API sysTimeInMilliSec2QDateTime( double i_fSysTime_ms );
QDateTime ZSSYSDLL_API sysTimeInMicroSec2QDateTime( double i_fSysTime_us );

} // namespace Time

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_Time_h
