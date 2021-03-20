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

#ifndef ZSApps_TestRemCmdServer_MsgTest_h
#define ZSApps_TestRemCmdServer_MsgTest_h

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
namespace RemCmdServer
{
typedef enum
{
    EMsgTypeMin                = (ZS::System::EBaseMsgTypeMax+1),
    EMsgTypeReqStartup         = (EMsgTypeMin + 0),
    EMsgTypeConStartup         = (EMsgTypeMin + 1),
    EMsgTypeReqTest            = (EMsgTypeMin + 2),
    EMsgTypeConTest            = (EMsgTypeMin + 3),
    EMsgTypeReqDbParSetVal     = (EMsgTypeMin + 4),
    EMsgTypeConDbParSetVal     = (EMsgTypeMin + 5),
    EMsgTypeReqDbParGetVal     = (EMsgTypeMin + 6),
    EMsgTypeConDbParGetVal     = (EMsgTypeMin + 7),
    EMsgTypeIndDbParValChanged = (EMsgTypeMin + 8),
    EMsgTypeMax                = (EMsgTypeMin + 8),
    EMsgTypeCount              = (EMsgTypeMax-EMsgTypeMin+1),
    EMsgTypeUndefined
}   EMsgType;

//******************************************************************************
class CMsgReqStartup : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqStartup(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeConfirmed = false,
        qint64   i_iParentReqId = -1,
        qint64   i_iMsgId = -1 );
    virtual ~CMsgReqStartup();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );

}; // CMsgReqStartup

//******************************************************************************
class CMsgConStartup : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConStartup(
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        qint64                            i_iParentReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo,
        int                               i_iProgressInPerCent );
    virtual ~CMsgConStartup();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );

}; // CMsgConStartup

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
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );

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
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );

}; // CMsgConTest

//******************************************************************************
class CMsgReqDbParSetVal : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqDbParSetVal(
        QObject*        i_pObjSender,
        QObject*        i_pObjReceiver,
        const QString&  i_strDbPar,
        const QVariant& i_val,
        bool            i_bMustBeConfirmed = false,
        qint64          i_iParentReqId = -1,
        qint64          i_iMsgId = -1 );
    virtual ~CMsgReqDbParSetVal();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // instance methods
    QString getDbPar() const { return m_strDbPar; }
    QVariant getVal() const { return m_val; }
protected: // instance members
    QString  m_strDbPar;
    QVariant m_val;

}; // CMsgReqDbParSetVal

//******************************************************************************
class CMsgConDbParSetVal : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConDbParSetVal(
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        const QString&                    i_strDbPar,
        const QVariant&                   i_val,
        qint64                            i_iParentReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo,
        int                               i_iProgressInPerCent );
    virtual ~CMsgConDbParSetVal();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // instance methods
    QString getDbPar() const { return m_strDbPar; }
    QVariant getVal() const { return m_val; }
protected: // instance members
    QString  m_strDbPar;
    QVariant m_val;

}; // CMsgConDbParSetVal

//******************************************************************************
class CMsgReqDbParGetVal : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqDbParGetVal(
        QObject*        i_pObjSender,
        QObject*        i_pObjReceiver,
        const QString&  i_strDbPar,
        qint64          i_iParentReqId = -1,
        qint64          i_iMsgId = -1 );
    virtual ~CMsgReqDbParGetVal();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // instance methods
    QString getDbPar() const { return m_strDbPar; }
protected: // instance members
    QString  m_strDbPar;

}; // CMsgReqDbParGetVal

//******************************************************************************
class CMsgConDbParGetVal : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConDbParGetVal(
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        const QString&                    i_strDbPar,
        const QVariant&                   i_val,
        qint64                            i_iParentReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo,
        int                               i_iProgressInPerCent );
    virtual ~CMsgConDbParGetVal();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // instance methods
    QString getDbPar() const { return m_strDbPar; }
    QVariant getVal() const { return m_val; }
protected: // instance members
    QString  m_strDbPar;
    QVariant m_val;

}; // CMsgConDbParGetVal

//******************************************************************************
class CMsgIndDbParValChanged : public ZS::System::CMsgInd
//******************************************************************************
{
public: // ctors and dtor
    CMsgIndDbParValChanged(
        QObject*        i_pObjSender,
        QObject*        i_pObjReceiver,
        const QString&  i_strDbPar,
        const QVariant& i_val,
        qint64          i_iMsgId = -1 );
    virtual ~CMsgIndDbParValChanged();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgAck* createAcknowledgeMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // instance methods
    QString getDbPar() const { return m_strDbPar; }
    QVariant getVal() const { return m_val; }
protected: // instance members
    QString  m_strDbPar;
    QVariant m_val;

}; // CMsgIndDbParValChanged

} // namespace RemCmdServer

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // ZSApps_TestRemCmdServer_MsgTest_h
