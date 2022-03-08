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

#ifndef ZSSys_SleeperThread_h
#define ZSSys_SleeperThread_h

#include <QtCore/QThread>

#include "ZSSys/ZSSysDllMain.h"

class QSettings;
class QTimer;

namespace ZS
{
namespace System
{
//******************************************************************************
class ZSSYSDLL_API CSleeperThread : public QThread
//******************************************************************************
{
public: // class methods
    static void msleep( unsigned long i_iTime_ms );
};

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_SleeperThread_h
