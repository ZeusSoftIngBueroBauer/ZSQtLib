/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer, Germany
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

#ifndef ZSRemCmd_RemCmdsSyst_h
#define ZSRemCmd_RemCmdsSyst_h

#include <QtCore/qobject.h>

#include "ZSRemCmd/ZSRemCmdDllMain.h"
#include "ZSIpc/ZSIpcDllMain.h"

namespace ZS
{
namespace System
{
class CRequest;
struct SErrResultInfo;
}
namespace Ipc
{
struct SSocketDscr;
}
namespace Trace
{
class CTrcAdminObj;
}

namespace RemCmd
{
class CRemCmdObj;
class CRemCmdSubSysNode;
class CRemCmdObjPool;
class CRemCmdParser;

//******************************************************************************
class ZSREMCMDDLL_API CRemCmdsSyst : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CRemCmdsSyst(
        CRemCmdObjPool* i_pRemCmdObjPool,
        CRemCmdParser*  i_pRemCmdParser );
    virtual ~CRemCmdsSyst();
protected slots: // Subsystem: COMMands (object pool)
    void onExecuteRemCmdCommCat( const ZS::Ipc::SSocketDscr& i_socketDscr, bool i_bIsQuery, const QByteArray& i_byteArrArgs, QByteArray& o_byteArrResults, ZS::System::CRequest* io_pReq );
protected slots: // Subsystem: 
    void onExecuteRemCmdCls( const ZS::Ipc::SSocketDscr& i_socketDscr, bool i_bIsQuery, const QByteArray& i_byteArrArgs, QByteArray& o_byteArrResults, ZS::System::CRequest* io_pReq );
protected slots: // Subsystem: SYSTem:ERRor
    void onExecuteRemCmdSystErr( const ZS::Ipc::SSocketDscr& i_socketDscr, bool i_bIsQuery, const QByteArray& i_byteArrArgs, QByteArray& o_byteArrResults, ZS::System::CRequest* io_pReq );
    void onExecuteRemCmdSystErrAll( const ZS::Ipc::SSocketDscr& i_socketDscr, bool i_bIsQuery, const QByteArray& i_byteArrArgs, QByteArray& o_byteArrResults, ZS::System::CRequest* io_pReq );
    void onExecuteRemCmdSystErrCoun( const ZS::Ipc::SSocketDscr& i_socketDscr, bool i_bIsQuery, const QByteArray& i_byteArrArgs, QByteArray& o_byteArrResults, ZS::System::CRequest* io_pReq );
    void onExecuteRemCmdSystErrCodeAll( const ZS::Ipc::SSocketDscr& i_socketDscr, bool i_bIsQuery, const QByteArray& i_byteArrArgs, QByteArray& o_byteArrResults, ZS::System::CRequest* io_pReq );
    void onExecuteRemCmdSystErrCodeNext( const ZS::Ipc::SSocketDscr& i_socketDscr, bool i_bIsQuery, const QByteArray& i_byteArrArgs, QByteArray& o_byteArrResults, ZS::System::CRequest* io_pReq );
protected: // instance members
    CRemCmdObjPool*       m_pRemCmdObjPool;
    CRemCmdParser*        m_pRemCmdParser;
    // Subsystem: COMMands (object pool)
    CRemCmdObj*           m_pRemCmdCommCat;
    // Subsystem:
    CRemCmdObj*           m_pRemCmdCls;
    // Subsystem SYSTem
    CRemCmdSubSysNode*    m_pRemCmdSubSysNodeSyst;
    // Subsystem SYSTem:ERRor
    CRemCmdObj*           m_pRemCmdSystErr;
    CRemCmdObj*           m_pRemCmdSystErrAll;
    CRemCmdObj*           m_pRemCmdSystErrNext;
    CRemCmdObj*           m_pRemCmdSystErrCoun;
    CRemCmdObj*           m_pRemCmdSystErrCodeAll;
    CRemCmdObj*           m_pRemCmdSystErrCodeNext;
    // Method Trace
    Trace::CTrcAdminObj*  m_pTrcAdminObj;

}; // class CRemCmdsSyst

} // namespace RemCmd

} // namespace ZS

#endif // #ifndef ZSRemCmd_RemCmdsSyst_h
