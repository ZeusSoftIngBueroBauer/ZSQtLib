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

#ifndef ZSRemCmd_Msg_h
#define ZSRemCmd_Msg_h

#include "ZSRemCmd/ZSRemCmdDllMain.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

namespace ZS
{
namespace RemCmd
{
enum ERequest {
    ERequestNone       = 0,
    ERequestCmdExecute = 1,
    ERequestCount,
    ERequestUndefined
};
ZSREMCMDDLL_API QString request2Str( int i_iRequest );

enum EMsgType {
    EMsgTypeMin           = (ZS::Ipc::EMsgTypeMax+1),   // = 1027+01
    EMsgTypeReqCmdExecute = (EMsgTypeMin + 0),          // = 1028
    EMsgTypeConCmdExecute = (EMsgTypeMin + 1),          // = 1029
    EMsgTypeMax           = (EMsgTypeMin + 1),
    EMsgTypeCount         = (EMsgTypeMax-EMsgTypeMin+1),
    EMsgTypeUndefined
};
ZSREMCMDDLL_API QString msgType2Str( int i_iMsgType );

//******************************************************************************
class ZSREMCMDDLL_API CMsgReqCmdExecute : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqCmdExecute(
        QObject*                    i_pObjSender,
        QObject*                    i_pObjReceiver,
        const ZS::Ipc::SSocketDscr& i_socketDscr,
        const QString&              i_strCmd,
        bool                        i_bIsQuery,
        const QByteArray&           i_byteArrArgs,
        bool                        i_bMustBeConfirmed = true,
        qint64                      i_iParentReqId = -1,
        qint64                      i_iMsgId = -1 );
    virtual ~CMsgReqCmdExecute();
public: // instance methods
    ZS::Ipc::SSocketDscr getSocketDscr() const { return m_socketDscr; }
    int getSocketId() const { return m_socketDscr.m_iSocketId; }
    QString getCommand() const { return m_strCmd; }
    bool isQuery() const { return m_bIsQuery; }
    QByteArray getArgs() const { return m_byteArrArgs; }
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
protected: // instance members
    ZS::Ipc::SSocketDscr m_socketDscr;
    QString              m_strCmd;
    bool                 m_bIsQuery;
    QByteArray           m_byteArrArgs;

}; // CMsgReqCmdExecute

//******************************************************************************
class ZSREMCMDDLL_API CMsgConCmdExecute : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConCmdExecute(
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        const ZS::Ipc::SSocketDscr&       i_socketDscr,
        const QString&                    i_strCmd,
        bool                              i_bWasQuery,
        const QByteArray&                 i_byteArrResults,
        qint64                            i_iParentReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                               i_iProgressInPerCent = 100 );
    virtual ~CMsgConCmdExecute();
public: // instance methods
    ZS::Ipc::SSocketDscr getSocketDscr() const { return m_socketDscr; }
    int getSocketId() const { return m_socketDscr.m_iSocketId; }
    QString getCommand() const { return m_strCmd; }
    bool wasQuery() const { return m_bWasQuery; }
public: // instance methods
    void setResults( const QByteArray& i_byteArrResults );
    QByteArray getResults() const { return m_byteArrResults; }
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
protected: // instance members
    ZS::Ipc::SSocketDscr m_socketDscr;
    QString              m_strCmd;
    bool                 m_bWasQuery;
    QByteArray           m_byteArrResults;

}; // CMsgConCmdExecute

} // namespace RemCmd

} // namespace ZS

#endif // ZSRemCmd_Msg_h
