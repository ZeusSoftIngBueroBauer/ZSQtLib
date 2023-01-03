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

#ifndef ZSIpc_BlockType_h
#define ZSIpc_BlockType_h

#include <QtCore/qlist.h>
#include <QtCore/qobject.h>

#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcCommon.h"


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

namespace ZS
{
namespace System
{
class CMethodTracer;
}

namespace Ipc
{
class CBlkType;
class CIpcSocketWrapper;

enum EBlkType
{
    EBlkTypeNone = 0, // <Data>
    EBlkTypeL    = 1, // <Len>[Data]
    EBlkTypeE    = 2, // [Data]<End>
    EBlkTypeSL   = 3, // <Start><Len>[Data]
    EBlkTypeTLV  = 4, // <Tag><Len>[Value]
    EBlkTypeUser = 5,
    EBlkTypeCount,
    EBlkTypeUndefined
};
ZSIPCDLL_API QString blkType2Str( int i_iVal, ZS::System::EEnumEntryAliasStr i_alias = ZS::System::EEnumEntryAliasStrName );
ZSIPCDLL_API EBlkType str2BlkType( const QString& i_str, ZS::System::EEnumEntryAliasStr i_alias = ZS::System::EEnumEntryAliasStrName );

enum EBlkLenDataType {
    // Binary data types just need to be copied (casted) into variables of the corresponding data type.
    EBlkLenDataTypeBinMin    =  0,
    EBlkLenDataTypeBinINT8   =  0,    // Range [0...................127]: binary, 1 Byte
    EBlkLenDataTypeBinUINT8  =  1,    // Range [0...................255]: binary, 1 Byte
    EBlkLenDataTypeBinINT16  =  2,    // Range [0.................32767]: binary, 2 Bytes
    EBlkLenDataTypeBinUINT16 =  3,    // Range [0.................65535]: binary, 2 Bytes
    EBlkLenDataTypeBinINT32  =  4,    // Range [0............2147483647]: binary, 4 Bytes
    EBlkLenDataTypeBinUINT32 =  5,    // Range [0............4294967295]: binary, 4 Bytes
    EBlkLenDataTypeBinINT64  =  6,    // Range [0...9223372036854775807]: binary, 8 Bytes
    EBlkLenDataTypeBinUINT64 =  7,    // Range [0..18446744073709551616]: binary, 8 Bytes
    EBlkLenDataTypeBinMax    =  7,

    // If the block length is defined as strings the length is filled with leading '0' characters.
    // The string got to be converted into the binary value. The default for the string length depends
    // on the maximum value of the binary data type. If more characters should be used the length must
    // be explicitly defined at the block type specifier. Less than the default string length is not supported.

    EBlkLenDataTypeStrMin          =  8,

    EBlkLenDataTypeStrBase10Min    =  8,
    EBlkLenDataTypeStrBase10INT8   =  8,    // Range [                 "000"..................."127"]: string, based on 10 (decimal), default  3 Bytes
    EBlkLenDataTypeStrBase10UINT8  =  9,    // Range [                 "000"................   "255"]: string, based on 10 (decimal), default  3 Byte
    EBlkLenDataTypeStrBase10INT16  = 10,    // Range [               "00000"............     "32767"]: string, based on 10 (decimal), default  5 Bytes
    EBlkLenDataTypeStrBase10UINT16 = 11,    // Range [               "00000"............     "65535"]: string, based on 10 (decimal), default  5 Bytes
    EBlkLenDataTypeStrBase10INT32  = 12,    // Range [          "2147483647".......     "2147483647"]: string, based on 10 (decimal), default 10 Bytes
    EBlkLenDataTypeStrBase10UINT32 = 13,    // Range [          "4294967295"............"4294967295"]: string, based on 10 (decimal), default 10 Bytes
    EBlkLenDataTypeStrBase10INT64  = 14,    // Range [ "9223372036854775807"..."9223372036854775807"]: string, based on 10 (decimal), default 19 Bytes
    EBlkLenDataTypeStrBase10UINT64 = 15,    // Range ["18446744073709551616".."18446744073709551616"]: string, based on 10 (decimal), default 19 Bytes
    EBlkLenDataTypeStrBase10Max    = 15,

    EBlkLenDataTypeStrBase16Min    = 16,
    EBlkLenDataTypeStrBase16INT8   = 16,    // Range [              "00"................"FF"]: string, based on 16 (hexadecimal), default  2 Bytes
    EBlkLenDataTypeStrBase16UINT8  = 17,    // Range [              "00".............   "FF"]: string, based on 16 (hexadecimal), default  2 Byte
    EBlkLenDataTypeStrBase16INT16  = 18,    // Range [            "0000".........     "FFFF"]: string, based on 16 (hexadecimal), default  4 Bytes
    EBlkLenDataTypeStrBase16UINT16 = 19,    // Range [            "0000".........     "FFFF"]: string, based on 16 (hexadecimal), default  4 Bytes
    EBlkLenDataTypeStrBase16INT32  = 20,    // Range [        "00000000".....     "FFFFFFFF"]: string, based on 16 (hexadecimal), default  8 Bytes
    EBlkLenDataTypeStrBase16UINT32 = 21,    // Range [        "00000000".........."FFFFFFFF"]: string, based on 16 (hexadecimal), default  8 Bytes
    EBlkLenDataTypeStrBase16INT64  = 22,    // Range ["0000000000000000".."FFFFFFFFFFFFFFFF"]: string, based on 16 (hexadecimal), default 16 Bytes
    EBlkLenDataTypeStrBase16UINT64 = 23,    // Range ["0000000000000000".."FFFFFFFFFFFFFFFF"]: string, based on 16 (hexadecimal), default 16 Bytes
    EBlkLenDataTypeStrBase16Max    = 23,

    EBlkLenDataTypeStrMax          = 23,

    EBlkLenDataTypeCount

};

// Calculates the number of bytes used to store the length of the data block
// (= 1 byte for BinINT8 or BinUINT8, = 8 bytes for BinINT64 or BinUINT64,
//  = 3 bytes for StrBase10INT8, = 8 bytes for StrBase16INT16).
ZSIPCDLL_API quint8  blockLenDataType2SizeInBytes( EBlkLenDataType i_dataType );

// Calculates the maximum number of bytes within the data block according to
// data type of the block length (= 127 for INT8 or = 255 for UINT8).
ZSIPCDLL_API quint64 blockLenDataType2MaxSizeInBytes( EBlkLenDataType i_dataType );


//******************************************************************************
class ZSIPCDLL_API CBlkType
//******************************************************************************
{
public: // class members
    static const QByteArray   c_byteArrWatchDogDataBlock;
    static const unsigned int c_uByteArrWatchDogDataBlockLen;
public: // default ctor
    CBlkType();
protected: // ctor (for derived classes)
    CBlkType( EBlkType i_blkType );
public: // dtor
    virtual ~CBlkType();
public: // must overridables
    virtual CBlkType* clone() const = 0;
public: // instance methods
    EBlkType type() const { return m_blkType; }
public: // overridables
    //virtual bool hasConnectionGreetingBlock( ESrvCltType /*i_srvCltType*/ ) const { return false; }
    //virtual QByteArray* createConnectionGreetingBlock( ESrvCltType /*i_srvCltType*/ ) const { return nullptr; }
public: // must overridables
    virtual QByteArray* createWatchDogBlock() const = 0;
public: // must overridables
    virtual QList<QByteArray> receiveDataBlocks(
        ESrvCltType                i_srvCltType,
        QObject*                   i_pObjGtw,
        QObject*                   i_pObjSrvClt,
        CIpcSocketWrapper*         i_pSocketWrapper,
        QByteArray*                i_pByteArrWatchDog = nullptr,
        ZS::System::CMethodTracer* i_pMethodTracer = nullptr,
        const QList<QObject*>&     i_arpTrcMsgLogObjects = QList<QObject*>() ) const = 0;
    virtual bool writeDataBlock(
        ESrvCltType                i_srvCltType,
        QObject*                   i_pObjGtw,
        QObject*                   i_pObjSrvClt,
        CIpcSocketWrapper*         i_pSocketWrapper,
        const QByteArray&          i_byteArr,
        bool                       i_bIsWatchDogBlock = false,
        ZS::System::CMethodTracer* i_pMethodTracer = nullptr,
        const QList<QObject*>&     i_arpTrcMsgLogObjects = QList<QObject*>() ) const = 0;
protected: // instance members
    EBlkType m_blkType;

}; // class CBlkType

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpc_BlockType_h
