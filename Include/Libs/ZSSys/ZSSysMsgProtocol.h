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

#ifndef ZSSys_MsgProtocol_h
#define ZSSys_MsgProtocol_h

#include <QtCore/qdatetime.h>

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysErrResult.h"


namespace ZS
{
namespace System
{
namespace MsgProtocol
{
#pragma pack(push,1)

/*******************************************************************************

Format and content of message data blocks:
------------------------------------------

==================+=============================
| System:MsgHeader | Fixed Size Elements       |
|                  +---------------------------+
|                  | System:MsgHeader:Buffer   |
================================================
| Data Block                                   |
================================================

In general a serialized message starts with a message header.
This message header is divided into a part with fixed size elements
and an optional buffer with variable length. The length of this buffer
is defined by the element "m_blockLenBuffer" within the fixed size section
and "BuffIdx" elements point to the start of those arrays.

After a message header a data block follows. The size of the data block
is defined by the member "m_blockLenData" of the fixed size section.
Please note that this block length does not include the size of the current
header (also not including the buffer of the current message header).

The data block itself may again be assembled by headers, buffers and
further data blocks which again may be assembled by headers, buffers and
further data blocks which again may be assembled by ...

The system message header in detail:
------------------------------------

============================================================================
| System:SMsgHeader | m_protocolVersion     | quint16  2  |                |
|                   | m_protocolType        | quint8   1  |                |
|                   | m_timeStampDate       | quint32  4  |                |
|                   | m_timeStampTime       | quint32  4  |                |
|                   | m_systemMsgType       | quint8   1  |                |
|                   | m_msgType             | qint32   4  |                |
|                   | m_idSender            | qint64   8  |                |
|                   | m_buffIdxSenderName   | qint32   4  |                |
|                   | m_idReceiver          | qint64   8  |                |
|                   | m_buffIdxReceiverName | qint32   4  |                |
|                   | m_idMsg               | qint64   8  |                |
|                   | m_idRequest           | qint64   8  |                |
|                   | m_flag                | quint8   1  |                |
|                   | m_resultSeverity      | quint8   1  |                |
|                   | m_result              | qint32   4  |                |
|                   | m_iProgress_perCent   | quint8   1  |                |
|                   | m_iFillByte           | quint8   1  |                |
|                   | m_blockLenBuffer      | quint32  4  |                |
|                   | m_blockLenData        | quint32  4  |  72 bytes      |
+-------------------+-----------------------+-------------+----------------+
| MsgHeaderBuffer   | SenderName            |             |                |
|                   | ...                   |             |                |
|                   | ReceiverName          |             |                |
|                   | ...                   |             | blockLenBuffer |
============================================================================
| Data Block        | ...                   | ...         | blockLenData   |
============================================================================

*******************************************************************************/


/*******************************************************************************
public type definitions and constants
*******************************************************************************/

// !! DON'T !! change the size of the protocol data types.
// Otherwise the protocol is no longer "downward compatible".
typedef quint16  TVersion;
typedef quint8   TSystemMsgType;
typedef quint8   TCommand;
typedef qint32   TMsgType;
typedef quint8   TFlag;
typedef quint32  TTimeStampDate;
typedef quint32  TTimeStampTime;
typedef qint64   TObjId;
typedef qint64   TMsgId;
typedef qint64   TRequestId;
typedef quint8   TResultSeverity;
typedef qint32   TResult;
typedef quint8   TProgress;
typedef quint8   TDataType;
typedef quint32  TBlockLen;
typedef qint32   TBuffIdx;

// Protocol version
//------------------------------------------------------------------------------

static const TVersion EVersionMax = 0xffff;

// The VERSION is divided into two parts - the major version and the minor version number.
// The major software version number is encoded in the two higher bytes,
// The minor software version number is encoded in the two lower bytes.
// On changing either the major or minor version number the protocol is no
// longer downward compatible. This means that even if the format of the protocol
// header has not been changed additional features (e.g. additional queries that
// have to be answered by the controllers) might have been introduced and older
// versions of the controllers may not respond correspondingly to those requests.

// 0x0100 ... Message Protocol Version 1.0
static const TVersion VERSION = 0x0100;

// System Message types (conversion routine systemMsgType2String in ZSSysMsg)
//------------------------------------------------------------------------------

static const TSystemMsgType ESystemMsgTypeUndefined = 0x00;
static const TSystemMsgType ESystemMsgTypeReq       = 0x01;  // Request
static const TSystemMsgType ESystemMsgTypeCon       = 0x02;  // Confirmation to request
static const TSystemMsgType ESystemMsgTypeInd       = 0x03;  // Indication
static const TSystemMsgType ESystemMsgTypeAck       = 0x04;  // Acknowledge to indication

// Commands (conversion routine command2String in ZSSysMsg)
//------------------------------------------------------------------------------

static const TCommand ECommandUndefined = 0x00;
static const TCommand ECommandSelect    = 0x01;
static const TCommand ECommandUpdate    = 0x02;
static const TCommand ECommandInsert    = 0x03;
static const TCommand ECommandDelete    = 0x04;

// Message flags
//------------------------------------------------------------------------------

static const TFlag EFlagNone = 0x00;  // No flag set
static const TFlag EFlagAck  = 0x01;  // Answer expected (only meaningful for requests or indications)

// TimeStampDate
//------------------------------------------------------------------------------

static const quint8         ETimeStampDayBitShift    = 0;
static const TTimeStampDate ETimeStampDayBitMask     = 0x0000001f;  // ---- ---- ---- ---- ---- ---- ---1 1111
static const quint8         ETimeStampMonthBitShift  = 5;
static const TTimeStampDate ETimeStampMonthBitMask   = 0x000001e0;  // ---- ---- ---- ---- ---- ---1 111- ----
static const quint8         ETimeStampYearBitShift   = 9;
static const TTimeStampDate ETimeStampYearBitMask    = 0x001ffe00;  // ---- ---- ---1 1111 1111 111- ---- ----

// TimeStampTime
//------------------------------------------------------------------------------

static const quint8         ETimeStampMSecBitShift   = 0;
static const TTimeStampTime ETimeStampMSecBitMask    = 0x000003ff;  // ---- ---- ---- ---- ---- --11 1111 1111
static const quint8         ETimeStampSecondBitShift = 10;
static const TTimeStampTime ETimeStampSecondBitMask  = 0x0000fc00;  // ---- ---- ---- ---- 1111 11-- ---- ----
static const quint8         ETimeStampMinuteBitShift = 16;
static const TTimeStampTime ETimeStampMinuteBitMask  = 0x003f0000;  // ---- ---- --11 1111 ---- ---- ---- ----
static const quint8         ETimeStampHourBitShift   = 22;
static const TTimeStampTime ETimeStampHourBitMask    = 0x07c00000;  // ---- -111 11-- ---- ---- ---- ---- ----

// Result Severity
//------------------------------------------------------------------------------

static const TResultSeverity EResultSeverityCount = ZS::System::EResultSeverityCount;

// Result
//------------------------------------------------------------------------------

static const TResult EResultCount = ZS::System::EResultCount;

//******************************************************************************
struct ZSSYSDLL_API SMsgHeader
//******************************************************************************
{
public: // ctors and dtor
    // !! DON'T !! change the content of the header.
    // Otherwise the protocol is no longer "downward compatible".
    SMsgHeader() :
        m_protocolVersion(VERSION),
        m_timeStampDate(0),
        m_timeStampTime(0),
        m_systemMsgType(ESystemMsgTypeUndefined),
        m_command(ECommandUndefined),
        m_msgType(0),
        m_idSender(-1),
        m_buffIdxSenderName(-1),
        m_idReceiver(-1),
        m_buffIdxReceiverName(-1),
        m_idMsg(-1),
        m_idRequest(-1),
        m_flag(EFlagNone),
        m_resultSeverity(EResultSeveritySuccess),
        m_result(EResultSuccess),
        m_iProgress_perCent(0),
        m_iFillByte(0),
        m_blockLenBuffer(0),
        m_blockLenData(0)
    {
    }
public: // struct members                                 | SUM | Comment
    TVersion        m_protocolVersion;      // quint16  2 |   2 |
    TTimeStampDate  m_timeStampDate;        // quint32  4 |   6 |
    TTimeStampTime  m_timeStampTime;        // quint32  4 |  10 |
    TSystemMsgType  m_systemMsgType;        // quint8   1 |  11 |
    TCommand        m_command;              // quint8   1 |  12 |
    TMsgType        m_msgType;              // qint32   4 |  16 | Range [QEvent::User = 1000, QEvent::MaxUser = 65535]
    TObjId          m_idSender;             // qint64   8 |  24 |
    TBuffIdx        m_buffIdxSenderName;    // qint32   4 |  28 |
    TObjId          m_idReceiver;           // qint64   8 |  36 |
    TBuffIdx        m_buffIdxReceiverName;  // qint32   4 |  40 |
    TMsgId          m_idMsg;                // qint64   8 |  48 |
    TRequestId      m_idRequest;            // qint64   8 |  56 |
    TFlag           m_flag;                 // quint8   1 |  57 | May only be set to "Ack" by request and indication messages.
    TResultSeverity m_resultSeverity;       // quint8   1 |  58 | Valid only for confirmation and acknowledge messages.
    TResult         m_result;               // qint32   4 |  62 | Valid only for confirmation and acknowledge messages.
    quint8          m_iProgress_perCent;    // quint8   1 |  63 | Range [0..100]
    qint8           m_iFillByte;            // qint16   1 |  64 | To get a message size as a whole number multiple of 8.
    TBlockLen       m_blockLenBuffer;       // quint32  4 |  68 |
    TBlockLen       m_blockLenData;         // quint32  4 |  72 |

}; // struct SMsgHeader

#pragma pack(pop)

ZSSYSDLL_API SErrResultInfo checkMsgHeader( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );

} // namespace MsgProtocol


//******************************************************************************
struct ZSSYSDLL_API STimeStamp
//******************************************************************************
{
public: // ctors
    STimeStamp(); // default ctor reads current system date and time
    STimeStamp(
        int i_iYear,
        int i_iMonth,
        int i_iDay,
        int i_iHour,
        int i_iMinute,
        int i_iSecond,
        int i_iMilliSecond = 0 );
    STimeStamp( const QDateTime& i_dateTime );
    STimeStamp( const QDate& i_date );
    STimeStamp( const QTime& i_time );
    STimeStamp( MsgProtocol::TTimeStampDate i_timeStampDate, MsgProtocol::TTimeStampTime i_timeStampTime );
public: // instance methods
    MsgProtocol::TTimeStampDate toTimeStampDate() const;
    MsgProtocol::TTimeStampTime toTimeStampTime() const;
    QDateTime toDateTime() const;
public: // instance members
    int m_iYear;
    int m_iMonth;
    int m_iDay;
    int m_iHour;
    int m_iMinute;
    int m_iSecond;
    int m_iMilliSecond;

}; // struct STimeStamp

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_MsgProtocol_h
