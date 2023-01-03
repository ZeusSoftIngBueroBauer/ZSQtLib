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

#include <QtCore/qglobal.h>

#include "ZSIpc/ZSIpcBlkType.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Ipc;


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

/*==============================================================================
Block Types
==============================================================================*/

//------------------------------------------------------------------------------
const SEnumEntry s_arEnumStrBlkTypes[] =
//------------------------------------------------------------------------------
{                                        // IdxName, Symbol,  Text,                                SCPIShort, SCPILong
    /* 0 */ SEnumEntry( EBlkTypeNone,       "None",  "N",     "[Data]",                            "NONE",  "NONE"    ),
    /* 1 */ SEnumEntry( EBlkTypeL,          "L",     "L",     "Len[Data]",                         "LENG",  "LENGth"  ),
    /* 2 */ SEnumEntry( EBlkTypeE,          "E",     "E",     "[Data]End",                         "EOT",   "EOText"  ),
    /* 3 */ SEnumEntry( EBlkTypeSL,         "SL",    "SL",    "StartLen[Data]",                    "SLEN",  "SLENght" ),
    /* 4 */ SEnumEntry( EBlkTypeTLV,        "TLV",   "TLV",   "TagLenVal",                         "TLV",   "TLValue" ),
    /* 5 */ SEnumEntry( EBlkTypeUser,       "User",  "User",  "User",                              "USER",  "USER"    ),
    /* 6 */ SEnumEntry( EBlkTypeCount,      "Count", "Count", "Count",                             "COUN",  "COUNt"   ),
    /* 7 */ SEnumEntry( EBlkTypeUndefined,  "?",     "?",     "Undefined",                         "?",     "?"       )
};

//------------------------------------------------------------------------------
QString ZS::Ipc::blkType2Str( int i_iVal, ZS::System::EEnumEntryAliasStr /*i_alias*/ )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_iVal >= 0 && i_iVal < _ZSArrLen(s_arEnumStrBlkTypes) )
    {
        str = SEnumEntry::enumerator2Str(s_arEnumStrBlkTypes, _ZSArrLen(s_arEnumStrBlkTypes), i_iVal);
    }
    else
    {
        str = "? (" + QString::number(i_iVal) + ")";
    }

    return str;

} // blkType2Str

//------------------------------------------------------------------------------
EBlkType ZS::Ipc::str2BlkType( const QString& i_str, ZS::System::EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    EBlkType blkType = EBlkTypeUndefined;

    int iBlkType = SEnumEntry::str2Enumerator(s_arEnumStrBlkTypes, _ZSArrLen(s_arEnumStrBlkTypes), i_str, i_alias, Qt::CaseInsensitive);

    if( iBlkType >= 0 && iBlkType < EBlkTypeCount )
    {
        blkType = static_cast<EBlkType>(iBlkType);
    }

    return blkType;

} // str2BlkType

/*==============================================================================
Block Len Data Types
==============================================================================*/

const quint8 s_aruBlkLenDataTypeSizeInBytes[EBlkLenDataTypeCount] =
{
    /*  0 EBlkLenDataTypeBinINT8         */  1, // Range [0...................127]
    /*  1 EBlkLenDataTypeBinUINT8        */  1, // Range [0...................255]
    /*  2 EBlkLenDataTypeBinINT16        */  2, // Range [0.................32767]
    /*  3 EBlkLenDataTypeBinUINT16       */  2, // Range [0.................65535]
    /*  4 EBlkLenDataTypeBinINT32        */  4, // Range [0............2147483647]
    /*  5 EBlkLenDataTypeBinUINT32       */  4, // Range [0............4294967295]
    /*  6 EBlkLenDataTypeBinINT64        */  8, // Range [0...9223372036854775807]
    /*  7 EBlkLenDataTypeBinUINT64       */  8, // Range [0..18446744073709551616]
    /*  8 EBlkLenDataTypeStrBase10INT8   */  3, // Range [                 "000"..................."127"]
    /*  9 EBlkLenDataTypeStrBase10UINT8  */  3, // Range [                 "000"................   "255"]
    /* 10 EBlkLenDataTypeStrBase10INT16  */  5, // Range [               "00000"............     "32767"]
    /* 11 EBlkLenDataTypeStrBase10UINT16 */  5, // Range [               "00000"............     "65535"]
    /* 12 EBlkLenDataTypeStrBase10INT32  */ 10, // Range [          "2147483647".......     "2147483647"]
    /* 13 EBlkLenDataTypeStrBase10UINT32 */ 10, // Range [          "4294967295"............"4294967295"]
    /* 14 EBlkLenDataTypeStrBase10INT64  */ 19, // Range [ "9223372036854775807"..."9223372036854775807"]
    /* 15 EBlkLenDataTypeStrBase10UINT64 */ 19, // Range ["18446744073709551616".."18446744073709551616"]
    /* 16 EBlkLenDataTypeStrBase16INT8   */  2, // Range [              "00"................"FF"]
    /* 17 EBlkLenDataTypeStrBase16UINT8  */  2, // Range [              "00".............   "FF"]
    /* 18 EBlkLenDataTypeStrBase16INT16  */  4, // Range [            "0000".........     "FFFF"]
    /* 19 EBlkLenDataTypeStrBase16UINT16 */  4, // Range [            "0000".........     "FFFF"]
    /* 20 EBlkLenDataTypeStrBase16INT32  */  8, // Range [        "00000000".....     "FFFFFFFF"]
    /* 21 EBlkLenDataTypeStrBase16UINT32 */  8, // Range [        "00000000".........."FFFFFFFF"]
    /* 22 EBlkLenDataTypeStrBase16INT64  */ 16, // Range ["0000000000000000".."FFFFFFFFFFFFFFFF"]
    /* 23 EBlkLenDataTypeStrBase16UINT64 */ 16  // Range ["0000000000000000".."FFFFFFFFFFFFFFFF"]
};

const quint64 s_aruBlkLenDataTypeMaxSizeInBytes[EBlkLenDataTypeCount] =
{
    /*  0 EBlkLenDataTypeBinINT8         */ 0x7f,
    /*  1 EBlkLenDataTypeBinUINT8        */ 0xff,
    /*  2 EBlkLenDataTypeBinINT16        */ 0x7fff,
    /*  3 EBlkLenDataTypeBinUINT16       */ 0xffff,
    /*  4 EBlkLenDataTypeBinINT32        */ 0x7fffffff,
    /*  5 EBlkLenDataTypeBinUINT32       */ 0xffffffff,
    /*  6 EBlkLenDataTypeBinINT64        */ Q_UINT64_C(0x7fffffffffffffff),
    /*  7 EBlkLenDataTypeBinUINT64       */ Q_UINT64_C(0xffffffffffffffff),
    /*  8 EBlkLenDataTypeStrBase10INT8   */ 0x7f,
    /*  9 EBlkLenDataTypeStrBase10UINT8  */ 0xff,
    /* 10 EBlkLenDataTypeStrBase10INT16  */ 0x7fff,
    /* 11 EBlkLenDataTypeStrBase10UINT16 */ 0xffff,
    /* 12 EBlkLenDataTypeStrBase10INT32  */ 0x7fffffff,
    /* 13 EBlkLenDataTypeStrBase10UINT32 */ 0xffffffff,
    /* 14 EBlkLenDataTypeStrBase10INT64  */ Q_UINT64_C(0x7fffffffffffffff),
    /* 15 EBlkLenDataTypeStrBase10UINT64 */ Q_UINT64_C(0xffffffffffffffff),
    /* 16 EBlkLenDataTypeStrBase16INT8   */ 0x7f,
    /* 17 EBlkLenDataTypeStrBase16UINT8  */ 0xff,
    /* 18 EBlkLenDataTypeStrBase16INT16  */ 0x7fff,
    /* 19 EBlkLenDataTypeStrBase16UINT16 */ 0xffff,
    /* 20 EBlkLenDataTypeStrBase16INT32  */ 0x7fffffff,
    /* 21 EBlkLenDataTypeStrBase16UINT32 */ 0xffffffff,
    /* 22 EBlkLenDataTypeStrBase16INT64  */ Q_UINT64_C(0x7fffffffffffffff),
    /* 23 EBlkLenDataTypeStrBase16UINT64 */ Q_UINT64_C(0xffffffffffffffff)
};

//------------------------------------------------------------------------------
quint8 ZS::Ipc::blockLenDataType2SizeInBytes( EBlkLenDataType i_dataType )
//------------------------------------------------------------------------------
{
    quint8 uBlockLenSize = 0;

    if( i_dataType >= 0 && i_dataType < EBlkLenDataTypeCount )
    {
        uBlockLenSize = s_aruBlkLenDataTypeSizeInBytes[i_dataType];
    }
    return uBlockLenSize;

} // blockLenDataType2SizeInBytes

//------------------------------------------------------------------------------
quint64 ZS::Ipc::blockLenDataType2MaxSizeInBytes( EBlkLenDataType i_dataType )
//------------------------------------------------------------------------------
{
    quint64 uBlockLenMaxSize = 0;

    if( i_dataType >= 0 && i_dataType < EBlkLenDataTypeCount )
    {
        uBlockLenMaxSize = s_aruBlkLenDataTypeMaxSizeInBytes[i_dataType];
    }
    return uBlockLenMaxSize;

} // blockLenDataType2MaxSizeInBytes


/*******************************************************************************
class CBlkType
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

                                                            // 012345678901234567890123456789012345
const QByteArray   CBlkType::c_byteArrWatchDogDataBlock     = "CF28FFF9-F7C6-4C0B-BC6B-C7F99497A5A2";
const unsigned int CBlkType::c_uByteArrWatchDogDataBlockLen = 36; // 32 digits, 4 times "-"

/*==============================================================================
public: // default ctor
==============================================================================*/

//------------------------------------------------------------------------------
CBlkType::CBlkType() :
//------------------------------------------------------------------------------
    m_blkType(EBlkTypeNone)
{
} // default ctor

/*==============================================================================
protected: // ctor (for derived classes)
==============================================================================*/

//------------------------------------------------------------------------------
CBlkType::CBlkType( EBlkType i_blkType ) :
//------------------------------------------------------------------------------
    m_blkType(i_blkType)
{
} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CBlkType::~CBlkType()
//------------------------------------------------------------------------------
{
} // dtor
