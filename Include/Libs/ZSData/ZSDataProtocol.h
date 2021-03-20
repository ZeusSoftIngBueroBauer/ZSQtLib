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

#ifndef ZSData_Protocol_h
#define ZSData_Protocol_h

//#include <QtCore/qdatetime.h>

#include "ZSData/ZSDataCommon.h"
//#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysMsgProtocol.h"

#ifdef _WIN64

#elif defined _WIN32

#ifndef quint8
typedef unsigned char quint8;
#endif

#ifndef quint16
typedef unsigned short quint16;
#endif

#ifndef INT16
typedef signed short INT16;
#endif

#ifndef qint32
typedef signed int qint32;
#endif

#ifndef quint32
typedef unsigned int quint32;
#endif
#endif // #ifdef _WIN32

namespace ZS
{
namespace Data
{
namespace MsgProtocol
{
#pragma pack(push,1)

/*******************************************************************************

Format and content of message data blocks:
------------------------------------------

================================================
| System:MsgHeader | Fixed Size Elements       |
|                  +---------------------------+
|                  | System:MsgHeader:Buffer   |
================================================
| Data Block                                   |
================================================

Please see "ZSSysMsgProtocol.h" for the format and content of the System:MsgHeader.

The data block of the system message contains the data message header followed
by the data block of the data message.

The data message header in detail:
------------------------------------

============================================================================
| Data:SMsgHeader   | m_dataProtocolVersion | quint16  2  |                |
|                   | m_protocolType        | quint8   1  |                |
|                   | m_blockLenBuffer      | quint32  4  |                |
|                   | m_blockLenData        | quint32  4  |  35 Bytes      |
+-------------------+-----------------------+-------------+----------------+
| MsgHeaderBuffer   |                       |             | blockLenBuffer |
============================================================================
| Data Block        | ...                   | ...         | blockLenData   |
============================================================================

The content of the data block of a data message depends on the message type.
We can distinguish between three different types of data messages as
described below.

1. Messages for "Simple" Commands:
==================================

    EMsgTypeReqReadSchema
    EMsgTypeConReadSchema
    EMsgTypeReqSaveSchema
    EMsgTypeConSaveSchema
    EMsgTypeReqReadData
    EMsgTypeConReadData
    EMsgTypeReqSaveData
    EMsgTypeConSaveData
    EMsgTypeReqSet2Default
    EMsgTypeConSet2Default

2. Messages containing node specs
==================================

    EMsgTypeReqRegister
    EMsgTypeConRegister
    EMsgTypeReqUnregister
    EMsgTypeConUnregister
    EMsgTypeReqSelectSchema
    EMsgTypeReqSelectData

    The node specs are serialized into a XML data stream.

3. Messages containing data sets
==================================

    EMsgTypeConSelectSchema
    EMsgTypeConSelectData

    EMsgTypeReqUpdateSchema
    EMsgTypeConUpdateSchema
    EMsgTypeReqUpdateData
    EMsgTypeConUpdateData

    The data sets are serialized into a XML data stream.

//3.1 The data message header in detail:
//--------------------------------------
//
// !!!!!!!! NO LONGER VALID !!!!!!!!!!!!!!
//
// !!!!!!!! XML DATA STREAM IS USED !!!!!!
//
//The data block of the data message contains the data set which again is split
//into the data set header, the buffer for the data set header and the data
//block of the data set:
//
//--------------------+-----------------------------------+------------+----------
//SDSHeader           | m_buffIdxDSSenderName             | qint32   4 |
//                    | m_blockLenDSSenderName            | quint32  4 |
//                    | m_buffIdxDSReceiverName           | qint32   4 |
//                    | m_blockLenDSReceiverName          | quint32  4 |
//                    | m_blockLenBuffDSObjNames          | quint32  4 |
//                    | m_tableCount                      | quint32  4 |
//                    | m_blockLenData                    | quint32  4 | 28 Bytes
//--------------------+-----------------------------------+-----------------------
//
//The data block of the data set contains 1 to N data tables. Each data table
//again is split into the data table header, the buffer for the data table header,
//a block for the column descriptions, a block containing the data of the data
//rows (the data cells). For array data types like strings the data cell points
//to the data rows buffer.
//
//--------------------+-----------------------------------+-----------------------
//SDTHeader[1..N]     | m_buffIdxDTSenderName             | qint32   4 |
//                    | m_blockLenDTSenderName            | quint32  4 |
//                    | m_buffIdxDTReceiverName           | qint32   4 |
//                    | m_blockLenDTReceiverName          | quint32  4 |
//                    | m_blockLenBuffDTObjNames          | quint32  4 |
//                    | m_columnCount                     | quint32  4 |
//                    | m_blockLenDCHeaders               | quint32  4 |
//                    | m_blockLenBuffDCObjNames          | quint32  4 |
//                    | m_rowCount                        | quint32  4 |
//                    | m_blockLenDRCellHeaders           | quint32  4 |
//                    | m_blockLenBuffDRCellValues        | quint32  4 | 44 Bytes
//--------------------+-----------------------------------+-------------------------
//
//The buffers of the data tables contain the data tables object names at the
//sender and receiver side of the message:
//
//--------------------+-----------------------------------+------------+------------
//                    | SenderName                        |            |
//                    | ...                               |            |
//                    | ReceiverName                      |            |
//                    | ...                               |            | ? Bytes
//--------------------+-----------------------------------+------------+------------
//
//The block for the column headers contain 1 to N data columns.
//
//--------------------+-----------------------------------+-----------------------
//SDCHeader[1..N]     | m_buffIdxDCSenderName             | qint32   4 |
//                    | m_blockLenDCSenderName            | quint32  4 |
//                    | m_buffIdxDCReceiverName           | qint32   4 |
//                    | m_blockLenDCReceiverName          | quint32  4 |
//                    | m_dataType                        | quint8   1 | 17 Bytes
//--------------------+-----------------------------------+-----------------------
//
//The buffers of the data columns contain the data column object names at the
//sender and receiver side of the message:
//
//--------------------+-----------------------------------+------------+----------
//                    | SenderName                        |            |
//                    | ...                               |            |
//                    | ReceiverName                      |            |
//                    | ...                               |            | ? Bytes
//--------------------+-----------------------------------+------------+-----------
//
//The data block of the data table contains 0 to N data rows and is split into a
//data block with data row cell headers and a data block containing the values for
//each cell.
//
//--------------------+-----------------------------------+-----------------------
//SDRCellHeader[1..N] | m_dataType                        | quint8   1 |
//                    | m_buffIdxValue                    | qint32   4 |
//                    | m_blockLenValue                   | quint32  4 |  9 Bytes
//--------------------+-----------------------------------+------------+----------
//SDRCellValues       |                                   |            |
//                    |                                   |            |
//                    |                                   |            | ? Bytes
//--------------------+-----------------------------------+------------+----------

*******************************************************************************/


/*******************************************************************************
public type definitions and constants
*******************************************************************************/

// !! DON'T !! change the size of the protocol data types.
// Otherwise the protocol is no longer "downward compatible".
typedef quint8   TProtocolType;
//typedef quint8   TCmd;
//typedef qint32   TId;
//typedef quint8   TCmdClass;
//typedef quint8   ECmdClass;
//typedef quint8   TCmdType;
//typedef quint8   ECmdType;
//typedef qint16   TClmIdx;

// Protocol version
//------------------------------------------------------------------------------

// The VERSION is divided into two parts - the major version and the minor version number.
// The major software version number is encoded in the two higher bytes,
// The minor software version number is encoded in the two lower bytes.
// On changing either the major or minor version number the protocol is no
// longer downward compatible. This means that even if the format of the protocol
// header has not been changed additional features (e.g. additional queries that
// have to be answered by the controllers) might have been introduced and older
// versions of the controllers may not respond correspondingly to those requests.

// 0x0100 ... Data Protocol Version 1.0
static const ZS::System::MsgProtocol::TVersion VERSION = 0x0100;

// Command Type
//------------------------------------------------------------------------------

//static const TCmdType ECmdTypeUndefined       = 0x00;
//static const TCmdType ECmdTypeStandard        = 0x01;
//static const TCmdType ECmdTypeStoredProcedure = 0x02;

//struct SMsgHeader;
//struct SCmdHeader;
//struct SDSHeader;
//struct SDTHeader;
//struct SDCHeader;
//struct SDRCellHeader;

ZSDATADLL_API ZS::System::SErrResultInfo checkMsgHeader( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
ZSDATADLL_API ZS::System::SErrResultInfo checkMsgContent( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );


//******************************************************************************
struct SMsgHeader
//******************************************************************************
{
public: // ctors and dtor
    // !! DON'T !! change the content of the header.
    // Otherwise the protocol is no longer "downward compatible".
    SMsgHeader() :
        m_protocolVersion(VERSION),
        m_protocolType(EProtocolTypeUndefined),
        m_iFillByte1(0),
        m_iFillByte2(0),
        m_blockLenBuffer(0),
        m_blockLenData(0)
    {
    }
public: // struct members                                                  | SUM | Comment
    ZS::System::MsgProtocol::TVersion   m_protocolVersion;   // quint16  2 |   2 |
    TProtocolType                       m_protocolType;      // quint8   1 |   3 |
    qint8                               m_iFillByte1;        // qint8    1 |   4 | To get a message size as a whole number multiple of 8.
    qint32                              m_iFillByte2;        // qint16   4 |   8 | To get a message size as a whole number multiple of 8.
    ZS::System::MsgProtocol::TBlockLen  m_blockLenBuffer;    // quint32  4 |  12 |
    ZS::System::MsgProtocol::TBlockLen  m_blockLenData;      // quint32  4 |  16 |

}; // struct SMsgHeader


////******************************************************************************
//struct SCmdHeader
////******************************************************************************
//{
//public: // ctors and dtor
//    SCmdHeader() :
//        m_cmdClass(ECmdClassUndefined),
//        m_cmdType(ECmdTypeUndefined),
//        m_idCmd(-1),
//        m_iTimeout_ms(0),
//        m_iMaxRowCount(0),
//        m_iMaxDuration_ms(0),
//        m_iIntermediateResultsInterval_ms(0),
//        m_blockLenData(0)
//    {
//    }
//public: // struct members
//    // !! DON'T !! change the content of the header.
//    // Otherwise the protocol is no longer "downward compatible".
//    TCmdClass m_cmdClass;                        // quint8   1
//    TCmdType  m_cmdType;                         // quint8   1
//    TId       m_idCmd;                           // qint32   4
//    INT16     m_iTimeout_ms;                     // INT16    2
//    qint32    m_iMaxRowCount;                    // qint32   4
//    INT16     m_iMaxDuration_ms;                 // INT16    2
//    INT16     m_iIntermediateResultsInterval_ms; // INT16    2
//    TBlockLen m_blockLenData;                    // quint32  4
//                                                 // SUM:   20 bytes
//
//}; // struct SCmdHeader
//
//
////******************************************************************************
//struct SDSHeader
////******************************************************************************
//{
//public: // ctors and dtor
//    SDSHeader() :
//        m_buffIdxDSSenderName(-1),
//        m_blockLenDSSenderName(0),
//        m_buffIdxDSReceiverName(-1),
//        m_blockLenDSReceiverName(0),
//        m_blockLenBuffDSObjNames(0),
//        m_tableCount(0),
//        m_blockLenData(0)
//    {
//    }
//public: // struct members
//    // !! DON'T !! change the content of the header.
//    // Otherwise the protocol is no longer "downward compatible".
//    TBuffIdx  m_buffIdxDSSenderName;    // qint32   4
//    TBlockLen m_blockLenDSSenderName;   // quint32  4
//    TBuffIdx  m_buffIdxDSReceiverName;  // qint32   4
//    TBlockLen m_blockLenDSReceiverName; // quint32  4
//    TBlockLen m_blockLenBuffDSObjNames; // quint32  4
//    TBlockLen m_tableCount;             // quint32  4
//    TBlockLen m_blockLenData;           // quint32  4
//                                        // SUM:   28 bytes
//
//}; // struct SDSHeader
//
//
////******************************************************************************
//struct SDTHeader
////******************************************************************************
//{
//public: // ctors and dtor
//    SDTHeader() :
//        m_buffIdxDTSenderName(-1),
//        m_blockLenDTSenderName(0),
//        m_buffIdxDTReceiverName(-1),
//        m_blockLenDTReceiverName(0),
//        m_blockLenBuffDTObjNames(0),
//        m_columnCount(0),
//        m_blockLenDCHeaders(0),
//        m_blockLenBuffDCObjNames(0),
//        m_rowCount(0),
//        m_blockLenDRCellHeaders(0),
//        m_blockLenBuffDRCellValues(0)
//    {
//    }
//public: // struct members
//    // !! DON'T !! change the content of the header.
//    // Otherwise the protocol is no longer "downward compatible".
//    TBuffIdx  m_buffIdxDTSenderName;      // qint32   4
//    TBlockLen m_blockLenDTSenderName;     // quint32  4
//    TBuffIdx  m_buffIdxDTReceiverName;    // qint32   4
//    TBlockLen m_blockLenDTReceiverName;   // quint32  4
//    TBlockLen m_blockLenBuffDTObjNames;   // quint32  4
//    TBlockLen m_columnCount;              // quint32  4
//    TBlockLen m_blockLenDCHeaders;        // quint32  4
//    TBlockLen m_blockLenBuffDCObjNames;   // quint32  4
//    TBlockLen m_rowCount;                 // quint32  4
//    TBlockLen m_blockLenDRCellHeaders;    // quint32  4
//    TBlockLen m_blockLenBuffDRCellValues; // quint32  4
//                                          // SUM:   44 bytes
//
//}; // struct SDTHeader
//
//
////******************************************************************************
//struct SDCHeader
////******************************************************************************
//{
//public: // ctors and dtor
//    SDCHeader() :
//        m_buffIdxDCSenderName(-1),
//        m_blockLenDCSenderName(0),
//        m_buffIdxDCReceiverName(-1),
//        m_blockLenDCReceiverName(0),
//        m_dataType(EDataTypeUndefined)
//    {
//    }
//public: // struct members
//    // !! DON'T !! change the content of the header.
//    // Otherwise the protocol is no longer "downward compatible".
//    TBuffIdx  m_buffIdxDCSenderName;    // qint32   4
//    TBlockLen m_blockLenDCSenderName;   // quint32  4
//    TBuffIdx  m_buffIdxDCReceiverName;  // qint32   4
//    TBlockLen m_blockLenDCReceiverName; // quint32  4
//    TDataType m_dataType;               // quint8   1
//                                        // SUM:   17 bytes
//
//}; // struct SDCHeader
//
//
////******************************************************************************
//struct SDRCellHeader
////******************************************************************************
//{
//    // !! DON'T !! change the content of the tag.
//    // Otherwise the protocol is no longer "downward compatible".
//    SDRCellHeader() :
//        m_dataType(EDataTypeUndefined),
//        m_buffIdxValue(0),
//        m_blockLenValue(0)
//    {
//    }
//    TDataType m_dataType;        // quint8   1
//    TBuffIdx  m_buffIdxValue;    // qint32   4
//    TBlockLen m_blockLenValue;   // quint32  4
//                                 // SUM:    9 bytes
//
//}; // struct SDRCellHeader
//
#pragma pack(pop)

} // namespace MsgProtocol

} // namespace Data

} // namespace ZS

#endif // #ifndef ZSData_Protocol_h
