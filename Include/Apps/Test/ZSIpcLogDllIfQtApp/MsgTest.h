/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer, Germany
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

#ifndef ZSApps_TestIpcLogDllIfQtApp_MsgTest_h
#define ZSApps_TestIpcLogDllIfQtApp_MsgTest_h

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysMsg.h"


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

namespace ZS
{
namespace Apps
{
namespace Test
{
namespace IpcLogDllIfQtApp
{
typedef enum
{
    EMsgTypeMin     = (ZS::System::EBaseMsgTypeMax+1),
    EMsgTypeReqTest = (EMsgTypeMin + 0),
    EMsgTypeConTest = (EMsgTypeMin + 1),
    EMsgTypeMax     = (EMsgTypeMin + 1),
    EMsgTypeCount   = (EMsgTypeMax-EMsgTypeMin+1),
    EMsgTypeUndefined
}   EMsgType;

//******************************************************************************
class CMsgReqTest : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqTest(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeConfirmed = false,
        qint64   i_iParentReqId = -1,
        qint64   i_iMsgId = -1 );
    virtual ~CMsgReqTest();
public: // instance methods
    void setCommand(const QString& i_strCommand);
    QString getCommand() const;
    void setCommandArg(const QString& i_strArg);
    void setCommandArgs(const QStringList& i_strlstArgs);
    QString getCommandArg() const;
    QStringList getCommandArgs() const;
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
protected: // instance methods
    QString     m_strCommand;
    QStringList m_strlstCommandArgs;

}; // CMsgReqTest

//******************************************************************************
class CMsgConTest : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConTest(
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        qint64                            i_iParentReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo,
        int                               i_iProgressInPerCent );
    virtual ~CMsgConTest();
public: // instance methods
    void setCommand(const QString& i_strCommand);
    QString getCommand() const;
public: // overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
protected: // instance methods
    QString m_strCommand;

}; // CMsgConTest

} // namespace IpcLogDllIfQtApp

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // ZSApps_TestIpcLogDllIfQtApp_MsgTest_h
