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

#ifndef ZSSys_Msg_h
#define ZSSys_Msg_h

#include <QtCore/qcoreapplication.h>
#include <QtCore/qcoreevent.h>
#include <QtCore/qobject.h>
#include <QtCore/qstringlist.h>
#include <QtNetwork/qhostaddress.h>

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysMsgProtocol.h"
#include "ZSSys/ZSSysTrcMethod.h"


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

namespace ZS
{
namespace System
{
class CException;
class CMethodTracer;
class CMsg;
class CMsgReq;
class CMsgCon;
class CMsgInd;
class CMsgAck;
class CTrcAdminObj;

ZSSYSDLL_API QString systemMsgType2Str( int i_iMsgType, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrSymbol );
ZSSYSDLL_API MsgProtocol::TSystemMsgType str2SystemMsgType( const QString& i_str, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrSymbol );

enum EBaseMsgType {                                  // QEvent::User = 1000
    EBaseMsgTypeMin            = (QEvent::User+1),   // = 1001
    EBaseMsgTypeUndefined      = (QEvent::User+1),   // = 1001 (pure virtual base class CMsg)
    EBaseMsgTypeReq            = (QEvent::User+2),   // = 1002 (pure virtual base class CMsgReq)
    EBaseMsgTypeCon            = (QEvent::User+3),   // = 1003 (pure virtual base class CMsgCon)
    EBaseMsgTypeInd            = (QEvent::User+4),   // = 1004 (pure virtual base class CMsgInd)
    EBaseMsgTypeAck            = (QEvent::User+5),   // = 1005 (pure virtual base class CMsgAck)
    EBaseMsgTypeIndErr         = (QEvent::User+6),   // = 1006
    EBaseMsgTypeReqAddLogItem  = (QEvent::User+7),   // = 1007
    EBaseMsgTypeReqStartTimer  = (QEvent::User+8),   // = 1008
    EBaseMsgTypeConStartTimer  = (QEvent::User+9),   // = 1009
    EBaseMsgTypeReqStopTimer   = (QEvent::User+10),  // = 1010
    EBaseMsgTypeConStopTimer   = (QEvent::User+11),  // = 1011
    EBaseMsgTypeReqContinue    = (QEvent::User+12),  // = 1012
    EBaseMsgTypeReqAbort       = (QEvent::User+13),  // = 1013
    EBaseMsgTypeReqPoll        = (QEvent::User+14),  // = 1014
    EBaseMsgTypeConStartThread = (QEvent::User+15),  // = 1015
    EBaseMsgTypeMax            = (QEvent::User+15),
    EBaseMsgTypeCount          = (EBaseMsgTypeMax-EBaseMsgTypeMin+1)
};

ZSSYSDLL_API QString baseMsgType2Str( int i_iMsgType, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrSymbol );
ZSSYSDLL_API EBaseMsgType str2BaseMsgType( const QString& i_str, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrSymbol );

ZSSYSDLL_API QString command2Str( int i_iCmd, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrSymbol );
ZSSYSDLL_API MsgProtocol::TCommand str2Command( const QString& i_str, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrSymbol );

/*******************************************************************************
Global Methods
*******************************************************************************/

/*------------------------------------------------------------------------------
Sender or receiver of messages may be destroyed during runtime while confirmation
messages are temporarily stored (e.g. if a timeout occurs but while intermediate
results are pending). Messages are connected to the destroyed signal of the
QObject's and the member elements receiver and sender will be set to nullptr if the
objects are destroyed. The macro checks whether both the sender and receiver are
still alive before posting the message. Otherwise the message is deleted.
------------------------------------------------------------------------------*/
ZSSYSDLL_API void POST_OR_DELETE_MESSAGE( QEvent* i_pMsg, CMethodTracer* i_pMethodTracer = nullptr, ELogDetailLevel i_eFilterDetailLevel = ELogDetailLevel::None );

/*------------------------------------------------------------------------------
For debugging purposes (which was especially necessary to create python bindings)
the ctors and dtors of messages might be traced. But tracing messages again may
create message objects which will be traced and again and again message objects
would be created. To break this endless recursion of message ctors a static
flag "tracing" has been introduced. But this flag would have to be protected by
a mutex slowing down the programm execution. This is not desired. As tracing
message ctors and dtors is very likely not used anymore (or only in a very
special case) tracing must be explicitly enabled for the message classes by
the application.
------------------------------------------------------------------------------*/
ZSSYSDLL_API void SYS_MESSAGES_SET_TRACING_ENABLED( bool i_bEnabled = true );

/*------------------------------------------------------------------------------
For debugging purposes to detect whether all messages have been released during
runtime the applications destructor may add log entries in the trace file for
each not destroyed message object. This method may also be called if tracing
of message objects is not enabled.
------------------------------------------------------------------------------*/
ZSSYSDLL_API void TRACE_LIVING_MESSAGE_OBJECTS( CMethodTracer* i_pMethodTracer );


//******************************************************************************
class ZSSYSDLL_API CMsgSndRcvDestroyedHandler : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CMsgSndRcvDestroyedHandler( CMsg* i_pMsg );
    virtual ~CMsgSndRcvDestroyedHandler();
public: // instance methods (called by CMsg)
    void onMsgSenderChanged();
    void onMsgReceiverChanged();
protected slots:
    void onSenderDestroyed( QObject* i_pObj );
    void onReceiverDestroyed( QObject* i_pObj );
protected: // instance members
    CMsg*    m_pMsg;
    QObject* m_pObjSender;
    QObject* m_pObjReceiver;

}; // class CMsgSndRcvDestroyedHandler


//******************************************************************************
class ZSSYSDLL_API CMsg : /* public QObject, !!! not possible !!! */ public QEvent
/* Please note (double inheritance by QObject and QEvent):
   The message classes are exposed to the python world by creating bindings
   using shiboken. During runtime the types got to be cast from a python object
   to the corresponding C++ object and vice versa. This only works properly if
   the exposed classes are only inheritate one class at a time. In case of the
   message object on calling QCoreApplication::event the event object might be
   casted to QObject instead of QEvent and the application may crash. At least
   the dynamic_cast to CMsg* on receiving a QEvent* will not work properly.
   As message objects are using in request queues to store runtime data it was
   necessary to get noticed whether the sender or receiver of the message
   objects are destroyed while the message is postponed in a request queue.
   But as messages cannot be dervied from QObject a QObject signal/slot handler
   instance must be aggregated by the message instance.
*******************************************************************************/
{
public: // class methods
    static QString NameSpace() { return "ZS::System"; }
    static QString ClassName() { return "CMsg"; }
protected: // class methods
    static qint64 GetUniqueMsgId();
protected: // ctors (pure virtual base class)
    CMsg(
        MsgProtocol::TSystemMsgType i_systemMsgType,
        int                         i_iMsgType,
        QObject*                    i_pObjSender,
        QObject*                    i_pObjReceiver,
        qint64                      i_iMsgId = -1 );
    CMsg( const MsgProtocol::SMsgHeader* i_pMsgHdr );
public: // dtor
    virtual ~CMsg();
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    MsgProtocol::TSystemMsgType getSystemMsgType() const { return m_systemMsgType; }
    bool isBaseMsgType() const { return m_bIsBaseMsgType; }
public: // alias for QEvent::type()
    int getMsgType() const { return QEvent::type(); }
public: // instance methods
    void setSender( QObject* i_pObjSender );
    QObject* getSender() { return m_pObjSender; }
    QString getSenderName() const;
    void setSenderId( qint64 i_iSenderId );
    qint64 getSenderId() const { return m_iSenderId; }
public: // instance methods
    void setReceiver( QObject* i_pObjReceiver );
    QObject* getReceiver() { return m_pObjReceiver; }
    QString getReceiverName() const;
    void setReceiverId( qint64 i_iReceiverId );
    qint64 getReceiverId() const { return m_iReceiverId; }
public: // instance methods
    void setMsgId( qint64 i_iMsgId );
    qint64 getMsgId() const { return m_iMsgId; }
public: // instance methods
    void setObjectName( const QString& i_strObjName );
    QString objectName() { return m_strObjName; }
public: // must overridables
    virtual CMsg* clone( ECopyDepth i_copyDepth = ECopyDepth::FlatKeepOwnership ) = 0;
public: // overridables
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, EContentToStrFormat i_format = EContentToStrFormat::PlainText );
public: // overridables
    virtual QByteArray serialize() const;
protected: // instance members
    MsgProtocol::TSystemMsgType m_systemMsgType;
    bool                        m_bIsBaseMsgType;
    QObject*                    m_pObjSender;
    qint64                      m_iSenderId;
    QObject*                    m_pObjReceiver;
    qint64                      m_iReceiverId;
    qint64                      m_iMsgId;
    QString                     m_strObjName;
    CMsgSndRcvDestroyedHandler* m_pSndRcvDestroyedHandler;

}; // class CMsg

//******************************************************************************
class ZSSYSDLL_API CMsgReq : public CMsg
/* Request messages are used e.g. if the sender object wants the receiver to
   do some action or if the sender requests some information from the receiver.
   If the flag "MustBeConfirmed" is set to true the receiver of the request
   message must confirm the receipt of the request message (before or after
   executing the request) by sending the corresponding confirmation message
   back to the sender.
*******************************************************************************/
{
public: // class methods
    static QString NameSpace() { return "ZS::System"; }
    static QString ClassName() { return "CMsgReq"; }
protected: // ctors (pure virtual base class)
    CMsgReq(
        int      i_iMsgType,
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeConfirmed,
        qint64   i_iReqId = -1,
        qint64   i_iMsgId = -1 );
    CMsgReq( const MsgProtocol::SMsgHeader* i_pMsgHdr );
public: // dtor
    virtual ~CMsgReq();
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    void setMustBeConfirmed( bool i_bMustBeConfirmed );
    bool mustBeConfirmed() const { return m_bMustBeConfirmed; }
public: // instance methods
    void setTimeoutInMs( int i_iTimeout_ms );
    int getTimeoutInMs() const { return m_iTimeout_ms; }
public: // instance methods
    void setIsBlockingRequest( bool i_bIsBlocking );
    int isBlockingRequest() const { return m_bIsBlockingRequest; }
public: // instance methods
    void setRequestId( qint64 i_iId );
    qint64 getRequestId() const { return m_iReqId; }
public: // must overridables (keeps request and message id)
    virtual CMsgCon* createConfirmationMessage( ECopyDepth i_copyDepth = ECopyDepth::FlatKeepOwnership ) = 0;
public: // overridables of base class CMsg
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, EContentToStrFormat i_format = EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;
protected: // instance members
    bool   m_bMustBeConfirmed;
    int    m_iTimeout_ms;
    bool   m_bIsBlockingRequest;
    qint64 m_iReqId;

}; // class CMsgReq

//******************************************************************************
class ZSSYSDLL_API CMsgCon : public CMsg
/* Confirmation messages are the answer of the receiver on receiving a request
   message that must be confirmed.
   Please note that (as default) for confirmation messages no new message Id will
   be created but acknowledge message Id's should be set to the message Id of the
   corresponding request message. If you want the confirmation message to create
   its own unique message Id the ctor parameter MsgIdReq need to be set to -1.
*******************************************************************************/
{
public: // class methods
    static QString NameSpace() { return "ZS::System"; }
    static QString ClassName() { return "CMsgCon"; }
protected: // ctors (pure virtual base class)
    CMsgCon(
        int                   i_iMsgType,
        QObject*              i_pObjSender,
        QObject*              i_pObjReceiver,
        qint64                i_iReqId,
        qint64                i_iMsgIdReq,
        const SErrResultInfo& i_errResultInfo,
        int                   i_iProgressInPerCent );
    CMsgCon( const MsgProtocol::SMsgHeader* i_pMsgHdr );
public: // dtor
    virtual ~CMsgCon();
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    void setRequestId( qint64 i_iId );
    qint64 getRequestId() const { return m_iReqId; }
public: // instance methods
    void setErrResultInfo( const SErrResultInfo& i_errResultInfo );
    SErrResultInfo getErrResultInfo() const { return m_errResultInfo; }
    void setErrResult( const SErrResult& i_errResult );
    SErrResult getErrResult() const { return m_errResultInfo.getErrResult(); }
    void setSeverity( EResultSeverity i_severity );
    EResultSeverity getSeverity() const { return m_errResultInfo.getSeverity(); }
    QString getSeverityStr() const { return m_errResultInfo.getSeverityStr(); }
    void setResult( EResult i_result );
    EResult getResult() const { return m_errResultInfo.getResult(); }
    QString getResultStr() const { return m_errResultInfo.getResultStr(); }
    void setErrSource( const SErrSource& i_errSource );
    SErrSource getErrSource() { return m_errResultInfo.getErrSource(); }
    void setAddErrInfoDscr( const QString& i_strAddErrInfoDscr );
    QString getAddErrInfoDscr() const { return m_errResultInfo.getAddErrInfoDscr(); }
public: // instance methods
    void setProgress( int i_iProgressInPerCent );
    int getProgress() const { return m_iProgressInPerCent; }
public: // overridables of base class CMsg
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, EContentToStrFormat i_format = EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;
protected: // instance members
    qint64         m_iReqId;
    SErrResultInfo m_errResultInfo;
    int            m_iProgressInPerCent;

}; // class CMsgCon

//******************************************************************************
class ZSSYSDLL_API CMsgInd : public CMsg
/* Indication messages are used if the sender object wants to indicate any
   status or data change to the receiver. Usually indications are used together
   with register requests where an object registers themselves at the receiver
   object for a specified service. E.g. an output field indicating a measure
   value may register itself at the corresponding database parameter.
   If the flag "MustBeAcknowledged" is set to true the receiver of the indication
   message must acknowledge the receipt of the indication message by sending the
   corresponding acknowledge message back to the sender.
*******************************************************************************/
{
public: // class methods
    static QString NameSpace() { return "ZS::System"; }
    static QString ClassName() { return "CMsgInd"; }
protected: // ctors (pure virtual base class)
    CMsgInd(
        int      i_iMsgType,
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeAcknowledged = false,
        qint64   i_iReqId = -1,
        qint64   i_iMsgId = -1 );
    CMsgInd( const MsgProtocol::SMsgHeader* i_pMsgHdr );
public: // dtor
    virtual ~CMsgInd();
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    void setMustBeAcknowledged( bool i_bMustBeAcknowledged );
    bool mustBeAcknowledged() const { return m_bMustBeAcknowledged; }
public: // instance methods
    void setTimeoutInMs( int i_iTimeout_ms );
    int getTimeoutInMs() const { return m_iTimeout_ms; }
public: // instance methods
    void setIsBlockingRequest( bool i_bIsBlocking );
    int isBlockingRequest() const { return m_bIsBlockingRequest; }
public: // instance methods
    void setRequestId( qint64 i_iId );
    qint64 getRequestId() const { return m_iReqId; }
public: // must overridables (keeps request and message id)
    virtual CMsgAck* createAcknowledgeMessage( ECopyDepth i_copyDepth = ECopyDepth::FlatKeepOwnership ) = 0;
public: // overridables of base class CMsg
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, EContentToStrFormat i_format = EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;
protected: // instance members
    bool   m_bMustBeAcknowledged;
    int    m_iTimeout_ms;
    bool   m_bIsBlockingRequest;
    qint64 m_iReqId;

}; // class CMsgInd

//******************************************************************************
class ZSSYSDLL_API CMsgAck : public CMsg
/* Acknowledge messages are the answer of the receiver on receiving an indication
   message that must be acknowledged.
   Please note that (as default) for acknowledge messages no new message Id will
   be created but acknowledge message Id's should be set to the message Id of the
   corresponding indication message. If you want the acknowledge message to create
   its own unique message Id the ctor parameter MsgIdReq need to be set to -1.
*******************************************************************************/
{
public: // class methods
    static QString NameSpace() { return "ZS::System"; }
    static QString ClassName() { return "CMsgAck"; }
protected: // ctors (pure virtual base class)
    CMsgAck(
        int                   i_iMsgType,
        QObject*              i_pObjSender,
        QObject*              i_pObjReceiver,
        qint64                i_iReqId,
        qint64                i_iMsgIdInd,
        const SErrResultInfo& i_errResultInfo,
        int                   i_iProgressInPerCent );
    CMsgAck( const MsgProtocol::SMsgHeader* i_pMsgHdr );
public: // dtor
    virtual ~CMsgAck();
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    void setRequestId( qint64 i_iId );
    qint64 getRequestId() const { return m_iReqId; }
public: // instance methods
    void setErrResultInfo( const SErrResultInfo& i_errResultInfo );
    SErrResultInfo getErrResultInfo() const { return m_errResultInfo; }
    void setErrResult( const SErrResult& i_errResult );
    SErrResult getErrResult() const { return m_errResultInfo.getErrResult(); }
    void setSeverity( EResultSeverity i_severity );
    EResultSeverity getSeverity() const { return m_errResultInfo.getSeverity(); }
    QString getSeverityStr() const { return m_errResultInfo.getSeverityStr(); }
    void setResult( EResult i_result );
    EResult getResult() const { return m_errResultInfo.getResult(); }
    QString getResultStr() const { return m_errResultInfo.getResultStr(); }
    void setErrSource( const SErrSource& i_errSource );
    SErrSource getErrSource() const { return m_errResultInfo.getErrSource(); }
    void setAddErrInfoDscr( const QString& i_strAddErrInfoDscr );
    QString getAddErrInfoDscr() const { return m_errResultInfo.getAddErrInfoDscr(); }
    void setProgress( int i_iProgressInPerCent );
    int getProgress() const { return m_iProgressInPerCent; }
public: // overridables of base class CMsg
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, EContentToStrFormat i_format = EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;
protected: // instance members
    qint64         m_iReqId;
    SErrResultInfo m_errResultInfo;
    int            m_iProgressInPerCent;

}; // class CMsgAck

//******************************************************************************
class ZSSYSDLL_API CMsgErr : public CMsgInd
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::System"; }
    static QString ClassName() { return "CMsgErr"; }
public: // ctors and dtor
    CMsgErr(
        QObject*              i_pObjSender,
        QObject*              i_pObjReceiver,
        const SErrResultInfo& i_errResultInfo,
        qint64                i_iMsgId = -1 );
    CMsgErr(
        QObject*          i_pObjSender,
        QObject*          i_pObjReceiver,
        const CException& i_exc,
        const QString&    i_strExcType,
        qint64            i_iMsgId = -1 );
    virtual ~CMsgErr();
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    bool wasException() const { return m_bExc; }
    QString getExcTypeName() const { return m_strExcType; }
public: // instance methods
    void setErrResultInfo( const SErrResultInfo& i_errResultInfo );
    SErrResultInfo getErrResultInfo() const { return m_errResultInfo; }
public: // instance methods
    void setErrResult( const SErrResult& i_errResult );
    SErrResult getErrResult() const { return m_errResultInfo.getErrResult(); }
public: // instance methods
    void setSeverity( EResultSeverity i_severity );
    EResultSeverity getSeverity() const { return m_errResultInfo.getSeverity(); }
    QString getSeverityStr() const { return m_errResultInfo.getSeverityStr(); }
public: // instance methods
    void setResult( EResult i_result );
    EResult getResult() const { return m_errResultInfo.getResult(); }
    QString getResultStr() const { return m_errResultInfo.getResultStr(); }
public: // instance methods
    void setErrSource( const SErrSource& i_strErrSource );
    SErrSource getErrSource() const { return m_errResultInfo.getErrSource(); }
public: // instance methods
    void setAddErrInfoDscr( const QString& i_strAddErrInfoDscr );
    QString getAddErrInfoDscr() const { return m_errResultInfo.getAddErrInfoDscr(); }
public: // must overridables of base class CMsg
    virtual CMsg* clone( ECopyDepth i_copyDepth = ECopyDepth::FlatKeepOwnership );
    virtual CMsgAck* createAcknowledgeMessage( ECopyDepth i_copyDepth = ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, EContentToStrFormat i_format = EContentToStrFormat::PlainText );
protected: // instance members
    bool           m_bExc;
    QString        m_strExcType;
    SErrResultInfo m_errResultInfo;

}; // class CMsgErr

//******************************************************************************
class ZSSYSDLL_API CMsgReqAddLogItem : public CMsgReq
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::System"; }
    static QString ClassName() { return "CMsgReqAddLogItem"; }
public: // ctors and dtor
    CMsgReqAddLogItem(
        QObject*         i_pObjSender,
        QObject*         i_pObjReceiver,
        ETransmitDir     i_transmitDir,
        bool             i_bBold,
        const QString&   i_strMsg,
        #if QT_VERSION >= QT_VERSION_CHECK(4, 7, 0)
        const QDateTime& i_dtUtc = QDateTime::currentDateTimeUtc(),
        #else
        const QDateTime& i_dtUtc = QDateTime::currentDateTime().toUTC(),
        #endif
        double           i_fSysTime_s = Time::getProcTimeInSec() );
    virtual ~CMsgReqAddLogItem();
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    ETransmitDir getTransmitDir() const { return m_transmitDir; }
    bool isBold() const { return m_bBold; }
    QString getMsg() const { return m_strMsg; }
public: // instance methods
    void setDateTimeUtc( const QDateTime& i_dtUtc );
    QDateTime getDateTimeUtc() const { return m_dtUtc; }
    void setSystemTimeInSec( double i_fSysTime_s );
    double getSystemTimeInSec() const { return m_fSysTime_s; }
public: // must overridables of base class CMsg
    virtual CMsg* clone( ECopyDepth i_copyDepth = ECopyDepth::FlatKeepOwnership );
    virtual CMsgCon* createConfirmationMessage( ECopyDepth i_copyDepth = ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
protected: // instance members
    ETransmitDir m_transmitDir;
    bool         m_bBold;
    QString      m_strMsg;
    QDateTime    m_dtUtc;
    double       m_fSysTime_s;

}; // CMsgReqAddLogItem

//******************************************************************************
class ZSSYSDLL_API CMsgReqStartTimer : public CMsgReq
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::System"; }
    static QString ClassName() { return "CMsgReqStartTimer"; }
public: // ctors and dtor
    CMsgReqStartTimer(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        int      i_iTimerId,
        bool     i_bSingleShot,
        int      i_iInterval_ms,
        bool     i_bMustBeConfirmed = false,
        qint64   i_iReqId = -1,
        qint64   i_iMsgId = -1 );
    virtual ~CMsgReqStartTimer();
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    int getTimerId() const { return m_iTimerId; }
    bool isSingleShot() const { return m_bSingleShot; }
    int getIntervalInMs() const { return m_iInterval_ms; }
public: // must overridables of base class CMsg
    virtual CMsg* clone( ECopyDepth i_copyDepth = ECopyDepth::FlatKeepOwnership );
    virtual CMsgCon* createConfirmationMessage( ECopyDepth i_copyDepth = ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
protected: // instance members
    int  m_iTimerId;
    bool m_bSingleShot;
    int  m_iInterval_ms;

}; // CMsgReqStartTimer

//******************************************************************************
class ZSSYSDLL_API CMsgConStartTimer : public CMsgCon
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::System"; }
    static QString ClassName() { return "CMsgConStartTimer"; }
public: // ctors and dtor
    CMsgConStartTimer(
        QObject*              i_pObjSender,
        QObject*              i_pObjReceiver,
        int                   i_iTimerId,
        qint64                i_iMsgIdReq,
        const SErrResultInfo& i_errResultInfo );
    virtual ~CMsgConStartTimer();
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    int getTimerId() const { return m_iTimerId; }
public: // must overridables of base class CMsg
    virtual CMsg* clone( ECopyDepth i_copyDepth = ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
protected: // instance members
    int  m_iTimerId;

}; // CMsgConStartTimer

//******************************************************************************
class ZSSYSDLL_API CMsgReqStopTimer : public CMsgReq
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::System"; }
    static QString ClassName() { return "CMsgReqStopTimer"; }
public: // ctors and dtor
    CMsgReqStopTimer(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        int      i_iTimerId,
        bool     i_bMustBeConfirmed = false,
        qint64   i_iReqId = -1,
        qint64   i_iMsgId = -1 );
    virtual ~CMsgReqStopTimer();
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    int getTimerId() const { return m_iTimerId; }
public: // must overridables of base class CMsg
    virtual CMsg* clone( ECopyDepth i_copyDepth = ECopyDepth::FlatKeepOwnership );
    virtual CMsgCon* createConfirmationMessage( ECopyDepth i_copyDepth = ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
protected: // instance members
    int  m_iTimerId;

}; // CMsgReqStopTimer

//******************************************************************************
class ZSSYSDLL_API CMsgConStopTimer : public CMsgCon
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::System"; }
    static QString ClassName() { return "CMsgConStopTimer"; }
public: // ctors and dtor
    CMsgConStopTimer(
        QObject*              i_pObjSender,
        QObject*              i_pObjReceiver,
        int                   i_iTimerId,
        qint64                i_iMsgIdReq,
        const SErrResultInfo& i_errResultInfo );
    virtual ~CMsgConStopTimer();
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    int getTimerId() const { return m_iTimerId; }
public: // must overridables of base class CMsg
    virtual CMsg* clone( ECopyDepth i_copyDepth = ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
protected: // instance members
    int  m_iTimerId;

}; // CMsgConStopTimer

//******************************************************************************
class ZSSYSDLL_API CMsgReqContinue : public CMsgReq
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::System"; }
    static QString ClassName() { return "CMsgReqContinue"; }
public: // ctors and dtor
    CMsgReqContinue(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeConfirmed = false,
        qint64   i_iReqId = -1,
        qint64   i_iMsgId = -1 );
    virtual ~CMsgReqContinue();
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // must overridables of base class CMsg
    virtual CMsg* clone( ECopyDepth i_copyDepth = ECopyDepth::FlatKeepOwnership );
    virtual CMsgCon* createConfirmationMessage( ECopyDepth i_copyDepth = ECopyDepth::FlatKeepOwnership ); // not used, throws exception.
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;

}; // CMsgReqContinue

//******************************************************************************
class ZSSYSDLL_API CMsgReqAbort : public CMsgReq
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::System"; }
    static QString ClassName() { return "CMsgReqAbort"; }
public: // ctors and dtor
    CMsgReqAbort(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeConfirmed = false,
        qint64   i_iReqId = -1,
        qint64   i_iMsgId = -1 );
    virtual ~CMsgReqAbort();
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // must overridables of base class CMsg
    virtual CMsg* clone( ECopyDepth i_copyDepth = ECopyDepth::FlatKeepOwnership );
    virtual CMsgCon* createConfirmationMessage( ECopyDepth i_copyDepth = ECopyDepth::FlatKeepOwnership ); // not used, throws exception.
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;

}; // CMsgReqAbort

//******************************************************************************
class ZSSYSDLL_API CMsgReqPoll : public CMsgReq
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::System"; }
    static QString ClassName() { return "CMsgReqPoll"; }
public: // ctors and dtor
    CMsgReqPoll(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeConfirmed = false,
        qint64   i_iReqId = -1,
        qint64   i_iMsgId = -1 );
    virtual ~CMsgReqPoll();
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // must overridables of base class CMsg
    virtual CMsg* clone( ECopyDepth i_copyDepth = ECopyDepth::FlatKeepOwnership );
    virtual CMsgCon* createConfirmationMessage( ECopyDepth i_copyDepth = ECopyDepth::FlatKeepOwnership ); // not used, throws exception.
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;

}; // CMsgReqPoll

//******************************************************************************
class ZSSYSDLL_API CMsgConStartThread : public CMsgCon
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::System"; }
    static QString ClassName() { return "CMsgConStartThread"; }
public: // ctors and dtor
    CMsgConStartThread(
        QObject*              i_pObjSender,
        QObject*              i_pObjReceiver,
        qint64                i_iReqId,
        qint64                i_iMsgIdReq,
        const SErrResultInfo& i_errResultInfo );
    virtual ~CMsgConStartThread();
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // must overridables of base class CMsg
    virtual CMsg* clone( ECopyDepth i_copyDepth = ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;

}; // CMsgConStartThread

} // namespace System

} // namespace ZS

#endif // SMSysMsg_h
