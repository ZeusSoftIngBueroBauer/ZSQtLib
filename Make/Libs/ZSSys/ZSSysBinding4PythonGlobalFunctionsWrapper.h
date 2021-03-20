/*******************************************************************************

Copyright 2004 - 2017 by    ZeusSoft, Ing. Buero Bauer
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

--------------------------------------------------------------------------------

$Date: $

$Revision: $

$Author: $

$History: $

*******************************************************************************/

#ifndef ZSSys_Binding4Python_GlobalFunctionsWrapper_h
#define ZSSys_Binding4Python_GlobalFunctionsWrapper_h

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysRequest.h"

QString app_config_dir( const QString& i_strIniFileScope )
{
    return ::ZS::System::getAppConfigDir(i_strIniFileScope);
}

QString app_log_dir( const QString& i_strIniFileScope )
{
    return ::ZS::System::getAppLogDir(i_strIniFileScope);
}

QString app_data_dir( const QString& i_strIniFileScope )
{
    return ::ZS::System::getAppDataDir(i_strIniFileScope);
}

QString create_GUID()
{
    return ::ZS::System::createGUID();
}

QString bool_to_str( bool i_bValue )
{
    return ::ZS::System::bool2Str(i_bValue);
}

bool str_to_bool( const QString& i_str )
{
    return ::ZS::System::str2Bool(i_str);
}

bool is_asynchronous_request( ZS::System::CRequest* i_pReq )
{
    return ::ZS::System::isAsynchronousRequest(i_pReq);
}

#endif // #ifndef ZSSys_Binding4Python_GlobalFunctionsWrapper_h
