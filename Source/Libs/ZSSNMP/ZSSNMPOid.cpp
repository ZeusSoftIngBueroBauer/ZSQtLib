/*******************************************************************************

Copyright 2004 - 2009 by    ZeusSoft, Ing. Buero Bauer
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

//lint -esym(766,..\ZSSys/Include/ZSSysMemLeakDump.h)
#include <QtCore/qstringlist.h>
#define HAVE_SOCKLEN_T
#include "net-snmp/net-snmp-config.h"
#include "net-snmp/net-snmp-includes.h"
#undef index
#include "ZSSNMP/Include/ZSSNMPOid.h"
#include "ZSSys/Include/ZSSysAux.h"
#include "ZSSys/Include/ZSSysEnum.h"
#include "ZSSys/Include/ZSSysErrResult.h"
#include "ZSSys/Include/ZSSysException.h"
#include "ZSSys/Include/ZSSysMemLeakDump.h"

using namespace ZS::SNMP;


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

/*==============================================================================
enum OidFormat
==============================================================================*/

//------------------------------------------------------------------------------
const ZS::SEnumEntry s_arEnumStrOidFormat[EOidFormatCount] =
//------------------------------------------------------------------------------
{
    ZS::SEnumEntry( EOidFormatSuffix,  "Suffix"  ),
    ZS::SEnumEntry( EOidFormatModule,  "Module"  ),
    ZS::SEnumEntry( EOidFormatFull,    "Full"    ),
    ZS::SEnumEntry( EOidFormatNumeric, "Numeric" ),
    ZS::SEnumEntry( EOidFormatUcd,     "Ucd"     )
};

//------------------------------------------------------------------------------
QString ZS::SNMP::oidFormat2Str( int i_iFormat )
//------------------------------------------------------------------------------
{
    return ZS::SEnumEntry::val2Str(s_arEnumStrOidFormat,EOidFormatCount,i_iFormat);
}

/*==============================================================================
enum ValueType
==============================================================================*/

//------------------------------------------------------------------------------
const ZS::SEnumEntry s_arEnumStrValueTypes[EValueTypeCount] =
//------------------------------------------------------------------------------
{
    /*  0 */ ZS::SEnumEntry( EValueTypeUndefined,      "Undefined"      ),
    /*  1 */ ZS::SEnumEntry( EValueTypeNull,           "Null"           ),
    /*  2 */ ZS::SEnumEntry( EValueTypeBoolean,        "Boolean"        ),
    /*  3 */ ZS::SEnumEntry( EValueTypeInt,            "Integer"        ),
    /*  4 */ ZS::SEnumEntry( EValueTypeUInt,           "UInt"           ),
    /*  5 */ ZS::SEnumEntry( EValueTypeTimeTicks,      "TimeTicks"      ),
    /*  6 */ ZS::SEnumEntry( EValueTypeFloat,          "Float"          ),
    /*  7 */ ZS::SEnumEntry( EValueTypeDouble,         "Double"         ),
    /*  8 */ ZS::SEnumEntry( EValueTypeString,         "String"         ),
    /*  9 */ ZS::SEnumEntry( EValueTypeBitString,      "BitString"      ),
    /*  9 */ ZS::SEnumEntry( EValueTypeOctetString,    "OctetString"    ),
    /* 10 */ ZS::SEnumEntry( EValueTypeIPAddress,      "IPAddress"      ),
    /* 12 */ ZS::SEnumEntry( EValueTypeCounter32,      "Counter32"      ),
    /* 13 */ ZS::SEnumEntry( EValueTypeCounter64,      "Counter64"      ),
    /* 14 */ ZS::SEnumEntry( EValueTypeOid,            "Oid"            ),
    /* 15 */ ZS::SEnumEntry( EValueTypeSequence,       "Sequence"       ),
    /* 16 */ ZS::SEnumEntry( EValueTypeOpaque,         "Opaque"         ),
    /* 17 */ ZS::SEnumEntry( EValueTypeNoSuchObject,   "NoSuchObject"   ),
    /* 18 */ ZS::SEnumEntry( EValueTypeNoSuchInstance, "NoSuchInstance" ),
    /* 19 */ ZS::SEnumEntry( EValueTypeEndOfMibView,   "EndOfMibView"   )
};

//------------------------------------------------------------------------------
QString ZS::SNMP::valueType2Str( int i_iValueType )
//------------------------------------------------------------------------------
{
    return ZS::SEnumEntry::val2Str(s_arEnumStrValueTypes,EValueTypeCount,i_iValueType);
}

//------------------------------------------------------------------------------
EValueType ZS::SNMP::pduDataType2ValueType( int i_iPduDataType )
//------------------------------------------------------------------------------
{
    EValueType valueType = EValueTypeUndefined;

    switch( i_iPduDataType )
    {
        case EPDUDataTypeBoolean: // 1
        {
            valueType = EValueTypeBoolean;
            break;
        }
        case EPDUDataTypeInteger: // 2
        {
            valueType = EValueTypeInt;
            break;
        }
        case EPDUDataTypeBitString: // 3
        {
            valueType = EValueTypeBitString;
            break;
        }
        case EPDUDataTypeBits: // 4
        //case EPDUDataTypeOctetString: 4
        {
            valueType = EValueTypeOctetString;
            break;
        }
        case EPDUDataTypeNull: // 5
        {
            valueType = EValueTypeNull;
            break;
        }
        case EPDUDataTypeOid: // 6
        {
            valueType = EValueTypeOid;
            break;
        }
        case EPDUDataTypeSequence: //  48
        //case EPDUDataTypeSequenceOf: 48
        //case EPDUDataTypeVarBind:    48
        {
            valueType = EValueTypeSequence;
            break;
        }
        case EPDUDataTypeIPAddress: // 64
        {
            valueType = EValueTypeIPAddress;
            break;
        }
        case EPDUDataTypeCounter32: // 65
        {
            valueType = EValueTypeCounter32;
            break;
        }
        //case EPDUDataTypeGauge32:     66
        case EPDUDataTypeUnsigned32: // 66
        {
            valueType = EValueTypeUInt;
            break;
        }
        case EPDUDataTypeTimeTicks: // 67
        {
            valueType = EValueTypeTimeTicks;
            break;
        }
        case EPDUDataTypeOpaque: // 68
        {
            valueType = EValueTypeOpaque;
            break;
        }
        case EPDUDataTypeCounter64: // 70
        {
            valueType = EValueTypeCounter64;
            break;
        }
        case EPDUDataTypeNoSuchObject: // 128
        {
            valueType = EValueTypeNoSuchObject;
            break;
        }
        case EPDUDataTypeNoSuchInstance: // 129
        {
            valueType = EValueTypeNoSuchInstance;
            break;
        }
        case EPDUDataTypeEndOfMibView: // 130
        {
            valueType = EValueTypeEndOfMibView;
            break;
        }
        default:
        {
            break;
        }
    }
    return valueType;

} // pduDataType2ValueType

//------------------------------------------------------------------------------
QString ZS::SNMP::pduDataType2Str( int i_iPduDataType )
//------------------------------------------------------------------------------
{
    QString strPDUDataType = QString::number(i_iPduDataType) + "(" + QString::number(i_iPduDataType,16) + ")";

    switch( i_iPduDataType )
    {
        case EPDUDataTypeBoolean: // 1
        {
            strPDUDataType = "Boolean";
            break;
        }
        case EPDUDataTypeInteger: // 2
        {
            strPDUDataType = "Int";
            break;
        }
        case EPDUDataTypeBitString: // 3
        {
            strPDUDataType = "BitString";
            break;
        }
        case EPDUDataTypeBits: // 4
        //case EPDUDataTypeOctetString: 4
        {
            strPDUDataType = "Bits|OctetString";
            break;
        }
        case EPDUDataTypeNull: // 5
        {
            strPDUDataType = "Null";
            break;
        }
        case EPDUDataTypeOid: // 6
        {
            strPDUDataType = "Oid";
            break;
        }
        case EPDUDataTypeSequence: //  48
        //case EPDUDataTypeSequenceOf: 48
        //case EPDUDataTypeVarBind:    48
        {
            strPDUDataType = "Sequence|SequenceOf|VarBind";
            break;
        }
        case EPDUDataTypeIPAddress: // 64
        {
            strPDUDataType = "IPAddress";
            break;
        }
        case EPDUDataTypeCounter32: // 65
        {
            strPDUDataType = "Counter32";
            break;
        }
        //case EPDUDataTypeGauge32:     66
        case EPDUDataTypeUnsigned32: // 66
        {
            strPDUDataType = "UInt|Gauge32";
            break;
        }
        case EPDUDataTypeTimeTicks: // 67
        {
            strPDUDataType = "TimeTicks";
            break;
        }
        case EPDUDataTypeOpaque: // 68
        {
            strPDUDataType = "Opaque";
            break;
        }
        case EPDUDataTypeCounter64: // 70
        {
            strPDUDataType = "Counter64";
            break;
        }
        case EPDUDataTypeNoSuchObject: // 128
        {
            strPDUDataType = "NoSuchObject";
            break;
        }
        case EPDUDataTypeNoSuchInstance: // 129
        {
            strPDUDataType = "NoSuchInstance";
            break;
        }
        case EPDUDataTypeEndOfMibView: // 130
        {
            strPDUDataType = "EndOfMibView";
            break;
        }
        default:
        {
            break;
        }
    }
    return strPDUDataType;

} // pduDataType2Str


/*******************************************************************************
struct SOid
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
SOid::SOid() :
//------------------------------------------------------------------------------
    m_iSubIdsCount(0),
    m_iSubIdsArrLen(MAX_OID_LEN)
{
    memset( m_aruSubIds, 0x00, m_iSubIdsArrLen*sizeof(m_aruSubIds[0]) );

} // default ctor

//------------------------------------------------------------------------------
SOid::SOid( const QString& i_strOid ) :
//------------------------------------------------------------------------------
    m_iSubIdsCount(0),
    m_iSubIdsArrLen(MAX_OID_LEN)
{
    memset( m_aruSubIds, 0x00, m_iSubIdsArrLen*sizeof(m_aruSubIds[0]) );

    QString strOid = i_strOid;

    if( !strOid.startsWith(".") )
    {
        strOid.insert(0,".");
    }

    size_t iOidLen = static_cast<size_t>(m_iSubIdsArrLen);

    read_objid( strOid.toAscii(), m_aruSubIds, &iOidLen );

    m_iSubIdsCount = iOidLen;

} // ctor

/*==============================================================================
public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
QString SOid::toString( EOidFormat i_format )
//------------------------------------------------------------------------------
{
    char arcOid[1024] = { 0 };

    // Read current output format:
    int iOutputFormat = netsnmp_ds_get_int( NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_OID_OUTPUT_FORMAT );

    switch( i_format )
    {
        case EOidFormatSuffix:
        {
            netsnmp_ds_set_int( NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_OID_OUTPUT_FORMAT, NETSNMP_OID_OUTPUT_SUFFIX );
            break;
        }
        case EOidFormatModule:
        {
            netsnmp_ds_set_int( NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_OID_OUTPUT_FORMAT, NETSNMP_OID_OUTPUT_MODULE );
            break;
        }
        case EOidFormatFull:
        {
            netsnmp_ds_set_int( NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_OID_OUTPUT_FORMAT, NETSNMP_OID_OUTPUT_FULL );
            break;
        }
        case EOidFormatUcd:
        {
            netsnmp_ds_set_int( NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_OID_OUTPUT_FORMAT, NETSNMP_OID_OUTPUT_UCD );
            break;
        }
        case EOidFormatNumeric:
        default:
        {
            netsnmp_ds_set_int( NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_OID_OUTPUT_FORMAT, NETSNMP_OID_OUTPUT_NUMERIC );
            break;
        }
    }

    // Convert the unsigned int sub id array into desired format.
    // "snprint_objid" returns the length of the string or -1 on error.
    snprint_objid( arcOid, sizeof(arcOid), m_aruSubIds, m_iSubIdsCount );

    // Restore previous output format:
    netsnmp_ds_set_int( NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_OID_OUTPUT_FORMAT, iOutputFormat );

    return arcOid;

} // toString


/*******************************************************************************
class COid
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
COid::COid() :
//------------------------------------------------------------------------------
    m_puSubIds(NULL),
    m_iSubIdsCount(0),
    m_strOid()
    //m_arstrFormat[EOidFormatCount]
{
} // default ctor

//------------------------------------------------------------------------------
COid::COid( const COid& i_oid ) :
//------------------------------------------------------------------------------
    m_puSubIds(NULL),
    m_iSubIdsCount(0),
    m_strOid()
    //m_arstrFormat[EOidFormatCount]
{
    if( i_oid.m_puSubIds != NULL )
    {
        m_iSubIdsCount = i_oid.m_iSubIdsCount;
        m_puSubIds = new TSubId[m_iSubIdsCount];
        memcpy( m_puSubIds, i_oid.m_puSubIds, m_iSubIdsCount*sizeof(TSubId) );
    }
    m_strOid = i_oid.m_strOid;

    int idxFormat;

    for( idxFormat = 0; idxFormat < EOidFormatCount; idxFormat++ )
    {
        m_arstrFormat[idxFormat] = i_oid.m_arstrFormat[idxFormat];
    }

} // copy ctor

//------------------------------------------------------------------------------
COid::COid( const QString& i_strOid ) :
//------------------------------------------------------------------------------
    m_puSubIds(NULL),
    m_iSubIdsCount(0),
    m_strOid(i_strOid)
    //m_arstrFormat[EOidFormatCount]
{
    SOid* pOid = NULL;

    if( !m_strOid.isEmpty() && m_iSubIdsCount == 0 )
    {
        pOid = new SOid(m_strOid);
        createSubIdsArr(pOid);
    }
    delete pOid;
    pOid = NULL;

} // ctor

//------------------------------------------------------------------------------
COid::COid( TSubId* i_puSubIds, int i_iSubIdsCount ) :
//------------------------------------------------------------------------------
    m_puSubIds(NULL),
    m_iSubIdsCount(i_iSubIdsCount),
    m_strOid()
    //m_arstrFormat[EOidFormatCount]
{
    if( m_iSubIdsCount > 0 )
    {
        m_puSubIds = new TSubId[m_iSubIdsCount];
        memcpy( m_puSubIds, i_puSubIds, m_iSubIdsCount*sizeof(TSubId) );

        int idx;

        for( idx = 0; idx < m_iSubIdsCount; idx++ )
        {
            m_strOid.append(".");
            m_strOid.append( QString::number(m_puSubIds[idx]) );
        }
    }

} // ctor

//------------------------------------------------------------------------------
COid::~COid()
//------------------------------------------------------------------------------
{
    try
    {
        delete [] m_puSubIds;
    }
    catch(...)
    {
    }
    m_puSubIds = NULL;
    m_iSubIdsCount = 0;

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
void COid::operator = ( const COid& i_oid )
//------------------------------------------------------------------------------
{
    if( i_oid.m_puSubIds != NULL )
    {
        try
        {
            delete m_puSubIds;
        }
        catch(...)
        {
        }
        m_iSubIdsCount = i_oid.m_iSubIdsCount;
        m_puSubIds = new TSubId[m_iSubIdsCount];
        memcpy( m_puSubIds, i_oid.m_puSubIds, m_iSubIdsCount*sizeof(TSubId) );
    }
    m_strOid = i_oid.m_strOid;

    int idxFormat;

    for( idxFormat = 0; idxFormat < EOidFormatCount; idxFormat++ )
    {
        m_arstrFormat[idxFormat] = i_oid.m_arstrFormat[idxFormat];
    }

} // operator =

//------------------------------------------------------------------------------
bool COid::operator == ( COid& i_oidOther )
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    int     iSubIdsCountOfMe = getSubIdsCount();
    TSubId* pSubIdsOfMe = getSubIdsList(false);
    int     iSubIdsCountOfOther = i_oidOther.getSubIdsCount();
    TSubId* pSubIdsOfOther = i_oidOther.getSubIdsList(false);
    int     idxSubId;

    if( iSubIdsCountOfMe != iSubIdsCountOfOther )
    {
        bEqual = false;
    }
    else
    {
        // I am a child of the other oid if I contain all of the other sub ids ..
        for( idxSubId = 0; idxSubId < iSubIdsCountOfOther; idxSubId++ )
        {
            if( pSubIdsOfMe[idxSubId] != pSubIdsOfOther[idxSubId] )
            {
                bEqual = false;
                break;
            }
        }
    }
    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
bool COid::operator != ( COid& i_oidOther )
//------------------------------------------------------------------------------
{
    return !(*this == i_oidOther);

} // operator !=

//------------------------------------------------------------------------------
bool COid::operator == ( const QString& i_strOidOther )
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    COid oidOther(i_strOidOther);

    int     iSubIdsCountOfMe = getSubIdsCount();
    TSubId* pSubIdsOfMe = getSubIdsList(false);
    int     iSubIdsCountOfOther = oidOther.getSubIdsCount();
    TSubId* pSubIdsOfOther = oidOther.getSubIdsList(false);
    int     idxSubId;

    if( iSubIdsCountOfMe != iSubIdsCountOfOther )
    {
        bEqual = false;
    }
    else
    {
        // I am a child of the other oid if I contain all of the other sub ids ..
        for( idxSubId = 0; idxSubId < iSubIdsCountOfOther; idxSubId++ )
        {
            if( pSubIdsOfMe[idxSubId] != pSubIdsOfOther[idxSubId] )
            {
                bEqual = false;
                break;
            }
        }
    }
    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
bool COid::operator != ( const QString& i_strOidOther )
//------------------------------------------------------------------------------
{
    return !(*this == i_strOidOther);

} // operator !=

/*==============================================================================
public: // instance members
==============================================================================*/

//------------------------------------------------------------------------------
QString COid::toString( EOidFormat i_format )
//------------------------------------------------------------------------------
{
    SOid* pOid = NULL;

    if( !m_strOid.isEmpty() && m_iSubIdsCount == 0 )
    {
        pOid = new SOid(m_strOid);
        createSubIdsArr(pOid);
    }
    if( m_iSubIdsCount > 0 && m_arstrFormat[i_format].isEmpty() )
    {
        if( pOid == NULL )
        {
            pOid = new SOid(m_strOid);
        }
        m_arstrFormat[i_format] = pOid->toString(i_format);
    }
    delete pOid;
    pOid = NULL;

    return m_arstrFormat[i_format];

} // toString

//------------------------------------------------------------------------------
SOid COid::toOidStruct()
//------------------------------------------------------------------------------
{
    return SOid(m_strOid);

} // toOidStruct

//------------------------------------------------------------------------------
TSubId* COid::getSubIdsList( bool i_bCopyList )
//------------------------------------------------------------------------------
{
    if( !m_strOid.isEmpty() && m_iSubIdsCount == 0 )
    {
        createSubIdsArr();
    }

    TSubId* pSubIds = NULL;

    if( i_bCopyList )
    {
        pSubIds = new TSubId[m_iSubIdsCount];
        memcpy( pSubIds, m_puSubIds, m_iSubIdsCount*sizeof(TSubId) );
    }
    else
    {
        pSubIds = m_puSubIds;
    }
    return pSubIds;

} // getSubIdsList

//------------------------------------------------------------------------------
int COid::getSubIdsCount()
//------------------------------------------------------------------------------
{
    if( !m_strOid.isEmpty() && m_iSubIdsCount == 0 )
    {
        createSubIdsArr();
    }
    return m_iSubIdsCount;
}

//------------------------------------------------------------------------------
TSubId COid::getSubId( int i_idx )
//------------------------------------------------------------------------------
{
    TSubId uSubId = 0;

    if( !m_strOid.isEmpty() && m_iSubIdsCount == 0 )
    {
        createSubIdsArr();
    }
    if( i_idx < m_iSubIdsCount )
    {
        uSubId = m_puSubIds[i_idx];
    }
    return uSubId;
}

//------------------------------------------------------------------------------
bool COid::isChildOf( COid* i_pOid )
//------------------------------------------------------------------------------
{
    bool bIsChild = false;

    if( i_pOid != NULL )
    {
        int     iSubIdsCountOfMe = getSubIdsCount();
        TSubId* pSubIdsOfMe = getSubIdsList(false);
        int     iSubIdsCountOfOther = i_pOid->getSubIdsCount();
        TSubId* pSubIdsOfOther = i_pOid->getSubIdsList(false);
        int     idxSubId;

        // I can only be a child of the other oid if I have more sub ids than the other ..
        if( iSubIdsCountOfMe > iSubIdsCountOfOther )
        {
            bIsChild = true;

            // I am a child of the other oid if I contain all of the other sub ids ..
            for( idxSubId = 0; idxSubId < iSubIdsCountOfOther; idxSubId++ )
            {
                if( pSubIdsOfMe[idxSubId] != pSubIdsOfOther[idxSubId] )
                {
                    bIsChild = false;
                    break;
                }
            }
        }
    }
    return bIsChild;

} // isChildOf

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void COid::createSubIdsArr( SOid* i_pOid )
//------------------------------------------------------------------------------
{
    if( m_iSubIdsCount == 0 )
    {
        SOid* pOid = i_pOid;
        bool  bDeleteOid = false;

        if( pOid == NULL )
        {
            pOid = new SOid(m_strOid);
            bDeleteOid = true;
        }
        if( pOid->m_iSubIdsCount > 0 )
        {
            m_iSubIdsCount = pOid->m_iSubIdsCount;
            m_puSubIds = new TSubId[m_iSubIdsCount];
            memcpy( m_puSubIds, pOid->m_aruSubIds, m_iSubIdsCount*sizeof(TSubId) );
        }
        if( bDeleteOid )
        {
            delete pOid;
            pOid = NULL;
        }
    }

} // createSubIdsArr


/*******************************************************************************
class CValue
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CValue::CValue() :
//------------------------------------------------------------------------------
    m_oid(),
    m_pduDataType(EPDUDataTypeUndefined),
    m_valueType(EValueTypeUndefined),
    m_bValue(false),
    m_iValue(0),
    m_uValue(0),
    m_fValue(0.0),
    m_byteArrValue(),
    m_counter64Value(),
    m_pOidValue(NULL)
{
} // default ctor

//------------------------------------------------------------------------------
CValue::CValue( const CValue& i_value ) :
//------------------------------------------------------------------------------
    m_oid(i_value.m_oid),
    m_pduDataType(i_value.m_pduDataType),
    m_valueType(i_value.m_valueType),
    m_bValue(i_value.m_bValue),
    m_iValue(i_value.m_iValue),
    m_uValue(i_value.m_uValue),
    m_fValue(i_value.m_fValue),
    m_byteArrValue(i_value.m_byteArrValue),
    m_counter64Value(i_value.m_counter64Value),
    m_pOidValue(NULL)
{
    if( i_value.m_pOidValue != NULL )
    {
        m_pOidValue = new COid(*i_value.m_pOidValue);
    }

} // copy ctor

//------------------------------------------------------------------------------
CValue::CValue( const COid& i_oid ) :
//------------------------------------------------------------------------------
    m_oid(i_oid),
    m_pduDataType(EPDUDataTypeUndefined),
    m_valueType(EValueTypeUndefined),
    m_bValue(false),
    m_iValue(0),
    m_uValue(0),
    m_fValue(0.0),
    m_byteArrValue(),
    m_counter64Value(),
    m_pOidValue(NULL)
{
} // ctor

//------------------------------------------------------------------------------
CValue::CValue( const QString& i_strOid ) :
//------------------------------------------------------------------------------
    m_oid(i_strOid),
    m_pduDataType(EPDUDataTypeUndefined),
    m_valueType(EValueTypeUndefined),
    m_bValue(false),
    m_iValue(0),
    m_uValue(0),
    m_fValue(0.0),
    m_byteArrValue(),
    m_counter64Value(),
    m_pOidValue(NULL)
{
} // ctor

//------------------------------------------------------------------------------
CValue::CValue( TSubId* i_puSubIds, int i_iSubIdsCount ) :
//------------------------------------------------------------------------------
    m_oid(i_puSubIds,i_iSubIdsCount),
    m_pduDataType(EPDUDataTypeUndefined),
    m_valueType(EValueTypeUndefined),
    m_bValue(false),
    m_iValue(0),
    m_uValue(0),
    m_fValue(0.0),
    m_byteArrValue(),
    m_counter64Value(),
    m_pOidValue(NULL)
{
} // ctor

//------------------------------------------------------------------------------
CValue::CValue(
    const COid&   i_oid,
    EValueType    i_valueType,
    char*         i_pcValue,
    int           i_iValLen,
    unsigned char i_pduDataType ) :
//------------------------------------------------------------------------------
    m_oid(i_oid),
    m_pduDataType(i_pduDataType),
    m_valueType(i_valueType),
    m_bValue(false),
    m_iValue(0),
    m_uValue(0),
    m_fValue(0.0),
    m_byteArrValue(),
    m_counter64Value(),
    m_pOidValue(NULL)
{
    if( i_pcValue != NULL && i_iValLen > 0 )
    {
        switch( i_valueType )
        {
            case EValueTypeNull:
            {
                break;
            }
            case EValueTypeBoolean:
            {
                bool* pVal = reinterpret_cast<bool*>(i_pcValue);
                m_bValue = *pVal;
                break;
            }
            case EValueTypeInt:
            {
                qint32* pVal = reinterpret_cast<qint32*>(i_pcValue);
                m_iValue = *pVal;
                break;
            }
            case EValueTypeUInt:
            {
                quint32* pVal = reinterpret_cast<quint32*>(i_pcValue);
                m_uValue = *pVal;
                break;
            }
            case EValueTypeTimeTicks:
            {
                quint32* pVal = reinterpret_cast<quint32*>(i_pcValue);
                m_uValue = *pVal;
                break;
            }
            case EValueTypeFloat:
            {
                float* pVal = reinterpret_cast<float*>(i_pcValue);
                m_fValue = *pVal;
                break;
            }
            case EValueTypeDouble:
            {
                double* pVal = reinterpret_cast<double*>(i_pcValue);
                m_fValue = *pVal;
                break;
            }
            case EValueTypeCounter32:
            {
                quint32* pVal = reinterpret_cast<quint32*>(i_pcValue);
                m_uValue = *pVal;
                break;
            }
            case EValueTypeCounter64:
            {
                SValueCounter64* pVal = reinterpret_cast<SValueCounter64*>(i_pcValue);
                m_counter64Value = *pVal;
                break;
            }
            case EValueTypeOid:
            {
                TSubId* pVal = reinterpret_cast<TSubId*>(i_pcValue);
                m_pOidValue = new COid( pVal, i_iValLen/sizeof(TSubId) );
                break;
            }
            case EValueTypeIPAddress:
            {
                unsigned char* pVal = reinterpret_cast<unsigned char*>(i_pcValue);
                int            idxSubId;
                QByteArray     szTmp1;

                for( idxSubId = 0; idxSubId < 4; idxSubId++ )
                {
                    if( idxSubId < i_iValLen )
                    {
                        szTmp1 = QByteArray::number(*pVal);
                    }
                    else
                    {
                        szTmp1 = "0";
                    }
                    m_byteArrValue.append(szTmp1);

                    if( idxSubId < 3 )
                    {
                        m_byteArrValue.append(".");
                    }
                }
                break;
            }
            case EValueTypeString:
            case EValueTypeBitString:
            case EValueTypeOctetString:
            default:
            {
                m_byteArrValue.resize(i_iValLen);
                memcpy( m_byteArrValue.data(), i_pcValue, i_iValLen );
                break;
            }
        }
    }

} // ctor

//------------------------------------------------------------------------------
CValue::CValue( const COid& i_oid, bool i_bValue ) :
//------------------------------------------------------------------------------
    m_oid(i_oid),
    m_pduDataType(EPDUDataTypeUndefined),
    m_valueType(EValueTypeBoolean),
    m_bValue(i_bValue),
    m_iValue(0),
    m_uValue(0),
    m_fValue(0.0),
    m_byteArrValue(),
    m_counter64Value(),
    m_pOidValue(NULL)
{
} // ctor

//------------------------------------------------------------------------------
CValue::CValue( const COid& i_oid, qint32 i_iValue ) :
//------------------------------------------------------------------------------
    m_oid(i_oid),
    m_pduDataType(EPDUDataTypeUndefined),
    m_valueType(EValueTypeInt),
    m_bValue(false),
    m_iValue(i_iValue),
    m_uValue(0),
    m_fValue(0.0),
    m_byteArrValue(),
    m_counter64Value(),
    m_pOidValue(NULL)
{
} // ctor

//------------------------------------------------------------------------------
CValue::CValue( const COid& i_oid, quint32 i_uValue, EValueType i_valueType ) :
//------------------------------------------------------------------------------
    m_oid(i_oid),
    m_pduDataType(EPDUDataTypeUndefined),
    m_valueType(i_valueType),
    m_bValue(false),
    m_iValue(0),
    m_uValue(i_uValue),
    m_fValue(0.0),
    m_byteArrValue(),
    m_counter64Value(),
    m_pOidValue(NULL)
{
    if( i_valueType != EValueTypeUInt && i_valueType != EValueTypeTimeTicks && i_valueType != EValueTypeCounter32 )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultArgOutOfRange,"valueType must be UInt, TimeTicks, or Counter32");
    }

} // ctor

//------------------------------------------------------------------------------
CValue::CValue( const COid& i_oid, double i_fValue, EValueType i_valueType ) :
//------------------------------------------------------------------------------
    m_oid(i_oid),
    m_pduDataType(EPDUDataTypeUndefined),
    m_valueType(i_valueType),
    m_bValue(false),
    m_iValue(0),
    m_uValue(0),
    m_fValue(i_fValue),
    m_byteArrValue(),
    m_counter64Value(),
    m_pOidValue(NULL)
{
    if( i_valueType != EValueTypeFloat && i_valueType != EValueTypeDouble )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultArgOutOfRange,"valueType must be Float or Double");
    }

} // ctor

//------------------------------------------------------------------------------
CValue::CValue( const COid& i_oid, QByteArray& i_byteArrValue, EValueType i_valueType ) :
//------------------------------------------------------------------------------
    m_oid(i_oid),
    m_pduDataType(EPDUDataTypeUndefined),
    m_valueType(i_valueType),
    m_bValue(false),
    m_iValue(0),
    m_uValue(0),
    m_fValue(0.0),
    m_byteArrValue(),
    m_counter64Value(),
    m_pOidValue(NULL)
{
    switch( i_valueType )
    {
        case EValueTypeNull:
        {
            break;
        }
        case EValueTypeBoolean:
        {
            bool* pVal = reinterpret_cast<bool*>(i_byteArrValue.data());
            m_bValue = *pVal;
            break;
        }
        case EValueTypeInt:
        {
            qint32* pVal = reinterpret_cast<qint32*>(i_byteArrValue.data());
            m_iValue = *pVal;
            break;
        }
        case EValueTypeUInt:
        {
            quint32* pVal = reinterpret_cast<quint32*>(i_byteArrValue.data());
            m_uValue = *pVal;
            break;
        }
        case EValueTypeTimeTicks:
        {
            quint32* pVal = reinterpret_cast<quint32*>(i_byteArrValue.data());
            m_uValue = *pVal;
            break;
        }
        case EValueTypeFloat:
        {
            float* pVal = reinterpret_cast<float*>(i_byteArrValue.data());
            m_fValue = *pVal;
            break;
        }
        case EValueTypeDouble:
        {
            double* pVal = reinterpret_cast<double*>(i_byteArrValue.data());
            m_fValue = *pVal;
            break;
        }
        case EValueTypeCounter32:
        {
            quint32* pVal = reinterpret_cast<quint32*>(i_byteArrValue.data());
            m_uValue = *pVal;
            break;
        }
        case EValueTypeCounter64:
        {
            SValueCounter64* pVal = reinterpret_cast<SValueCounter64*>(i_byteArrValue.data());
            m_counter64Value = *pVal;
            break;
        }
        case EValueTypeOid:
        {
            TSubId* pVal = reinterpret_cast<TSubId*>(i_byteArrValue.data());
            m_pOidValue = new COid( pVal, i_byteArrValue.count()/sizeof(TSubId) );
            break;
        }
        case EValueTypeString:
        case EValueTypeBitString:
        case EValueTypeOctetString:
        case EValueTypeIPAddress:
        default:
        {
            m_byteArrValue.resize(i_byteArrValue.count());
            memcpy( m_byteArrValue.data(), i_byteArrValue.data(), i_byteArrValue.count() );
            break;
        }
    }

} // ctor

//------------------------------------------------------------------------------
CValue::CValue( const COid& i_oid, const SValueCounter64& i_counter64Value ) :
//------------------------------------------------------------------------------
    m_oid(i_oid),
    m_pduDataType(EPDUDataTypeUndefined),
    m_valueType(EValueTypeCounter64),
    m_bValue(false),
    m_iValue(0),
    m_uValue(0),
    m_fValue(0.0),
    m_byteArrValue(),
    m_counter64Value(i_counter64Value),
    m_pOidValue(NULL)
{
} // ctor

//------------------------------------------------------------------------------
CValue::CValue( const COid& i_oid, const COid& i_oidValue ) :
//------------------------------------------------------------------------------
    m_oid(i_oid),
    m_pduDataType(EPDUDataTypeUndefined),
    m_valueType(EValueTypeOid),
    m_bValue(false),
    m_iValue(0),
    m_uValue(0),
    m_fValue(0.0),
    m_byteArrValue(),
    m_counter64Value(),
    m_pOidValue(NULL)
{
    m_pOidValue = new COid(i_oidValue);

} // ctor

//------------------------------------------------------------------------------
CValue::CValue(
    const QString& i_strOid,
    EValueType     i_valueType,
    char*          i_pcValue,
    int            i_iValLen,
    unsigned char  i_pduDataType ) :
//------------------------------------------------------------------------------
    m_oid(i_strOid),
    m_pduDataType(i_pduDataType),
    m_valueType(i_valueType),
    m_bValue(false),
    m_iValue(0),
    m_uValue(0),
    m_fValue(0.0),
    m_byteArrValue(),
    m_counter64Value(),
    m_pOidValue(NULL)
{
    if( i_pcValue != NULL && i_iValLen > 0 )
    {
        switch( i_valueType )
        {
            case EValueTypeNull:
            {
                break;
            }
            case EValueTypeBoolean:
            {
                bool* pVal = reinterpret_cast<bool*>(i_pcValue);
                m_iValue = *pVal;
                break;
            }
            case EValueTypeInt:
            {
                qint32* pVal = reinterpret_cast<qint32*>(i_pcValue);
                m_iValue = *pVal;
                break;
            }
            case EValueTypeUInt:
            {
                quint32* pVal = reinterpret_cast<quint32*>(i_pcValue);
                m_uValue = *pVal;
                break;
            }
            case EValueTypeTimeTicks:
            {
                quint32* pVal = reinterpret_cast<quint32*>(i_pcValue);
                m_uValue = *pVal;
                break;
            }
            case EValueTypeFloat:
            {
                float* pVal = reinterpret_cast<float*>(i_pcValue);
                m_fValue = *pVal;
                break;
            }
            case EValueTypeDouble:
            {
                double* pVal = reinterpret_cast<double*>(i_pcValue);
                m_fValue = *pVal;
                break;
            }
            case EValueTypeCounter32:
            {
                quint32* pVal = reinterpret_cast<quint32*>(i_pcValue);
                m_uValue = *pVal;
                break;
            }
            case EValueTypeCounter64:
            {
                SValueCounter64* pVal = reinterpret_cast<SValueCounter64*>(i_pcValue);
                m_counter64Value = *pVal;
                break;
            }
            case EValueTypeOid:
            {
                TSubId* pVal = reinterpret_cast<TSubId*>(i_pcValue);
                m_pOidValue = new COid( pVal, i_iValLen/sizeof(TSubId) );
                break;
            }
            case EValueTypeIPAddress:
            {
                unsigned char* pVal = reinterpret_cast<unsigned char*>(i_pcValue);
                int            idxSubId;
                QByteArray     szTmp1;

                for( idxSubId = 0; idxSubId < 4; idxSubId++ )
                {
                    if( idxSubId < i_iValLen )
                    {
                        szTmp1 = QByteArray::number(*pVal);
                    }
                    else
                    {
                        szTmp1 = "0";
                    }
                    m_byteArrValue.append(szTmp1);

                    if( idxSubId < 3 )
                    {
                        m_byteArrValue.append(".");
                    }
                }
                break;
            }
            case EValueTypeString:
            case EValueTypeBitString:
            case EValueTypeOctetString:
            default:
            {
                m_byteArrValue.resize(i_iValLen);
                memcpy( m_byteArrValue.data(), i_pcValue, i_iValLen );
                break;
            }
        }
    }

} // ctor

//------------------------------------------------------------------------------
CValue::CValue( const QString& i_strOid, bool i_bValue ) :
//------------------------------------------------------------------------------
    m_oid(i_strOid),
    m_pduDataType(EPDUDataTypeUndefined),
    m_valueType(EValueTypeBoolean),
    m_bValue(i_bValue),
    m_iValue(0),
    m_uValue(0),
    m_fValue(0.0),
    m_byteArrValue(),
    m_counter64Value(),
    m_pOidValue(NULL)
{
} // ctor

//------------------------------------------------------------------------------
CValue::CValue( const QString& i_strOid, qint32 i_iValue ) :
//------------------------------------------------------------------------------
    m_oid(i_strOid),
    m_pduDataType(EPDUDataTypeUndefined),
    m_valueType(EValueTypeInt),
    m_bValue(false),
    m_iValue(i_iValue),
    m_uValue(0),
    m_fValue(0.0),
    m_byteArrValue(),
    m_counter64Value(),
    m_pOidValue(NULL)
{
} // ctor

//------------------------------------------------------------------------------
CValue::CValue( const QString& i_strOid, quint32 i_uValue, EValueType i_valueType ) :
//------------------------------------------------------------------------------
    m_oid(i_strOid),
    m_pduDataType(EPDUDataTypeUndefined),
    m_valueType(i_valueType),
    m_bValue(false),
    m_iValue(0),
    m_uValue(i_uValue),
    m_fValue(0.0),
    m_byteArrValue(),
    m_counter64Value(),
    m_pOidValue(NULL)
{
    if( i_valueType != EValueTypeUInt && i_valueType != EValueTypeTimeTicks && i_valueType != EValueTypeCounter32 )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultArgOutOfRange,"valueType must be UInt, TimeTicks, or Counter32");
    }

} // ctor

//------------------------------------------------------------------------------
CValue::CValue( const QString& i_strOid, double i_fValue, EValueType i_valueType ) :
//------------------------------------------------------------------------------
    m_oid(i_strOid),
    m_pduDataType(EPDUDataTypeUndefined),
    m_valueType(i_valueType),
    m_bValue(false),
    m_iValue(0),
    m_uValue(0),
    m_fValue(i_fValue),
    m_byteArrValue(),
    m_counter64Value(),
    m_pOidValue(NULL)
{
    if( i_valueType != EValueTypeFloat && i_valueType != EValueTypeDouble )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultArgOutOfRange,"valueType must be Float or Double");
    }

} // ctor

//------------------------------------------------------------------------------
CValue::CValue( const QString& i_strOid, QByteArray& i_byteArrValue, EValueType i_valueType ) :
//------------------------------------------------------------------------------
    m_oid(i_strOid),
    m_pduDataType(EPDUDataTypeUndefined),
    m_valueType(i_valueType),
    m_bValue(false),
    m_iValue(0),
    m_uValue(0),
    m_fValue(0.0),
    m_byteArrValue(),
    m_counter64Value(),
    m_pOidValue(NULL)
{
    switch( i_valueType )
    {
        case EValueTypeNull:
        {
            break;
        }
        case EValueTypeBoolean:
        {
            bool* pVal = reinterpret_cast<bool*>(i_byteArrValue.data());
            m_bValue = *pVal;
            break;
        }
        case EValueTypeInt:
        {
            qint32* pVal = reinterpret_cast<qint32*>(i_byteArrValue.data());
            m_iValue = *pVal;
            break;
        }
        case EValueTypeUInt:
        {
            quint32* pVal = reinterpret_cast<quint32*>(i_byteArrValue.data());
            m_uValue = *pVal;
            break;
        }
        case EValueTypeTimeTicks:
        {
            quint32* pVal = reinterpret_cast<quint32*>(i_byteArrValue.data());
            m_uValue = *pVal;
            break;
        }
        case EValueTypeFloat:
        {
            float* pVal = reinterpret_cast<float*>(i_byteArrValue.data());
            m_fValue = *pVal;
            break;
        }
        case EValueTypeDouble:
        {
            double* pVal = reinterpret_cast<double*>(i_byteArrValue.data());
            m_fValue = *pVal;
            break;
        }
        case EValueTypeCounter32:
        {
            quint32* pVal = reinterpret_cast<quint32*>(i_byteArrValue.data());
            m_uValue = *pVal;
            break;
        }
        case EValueTypeCounter64:
        {
            SValueCounter64* pVal = reinterpret_cast<SValueCounter64*>(i_byteArrValue.data());
            m_counter64Value = *pVal;
            break;
        }
        case EValueTypeOid:
        {
            TSubId* pVal = reinterpret_cast<TSubId*>(i_byteArrValue.data());
            m_pOidValue = new COid( pVal, i_byteArrValue.count()/sizeof(TSubId) );
            break;
        }
        case EValueTypeString:
        case EValueTypeBitString:
        case EValueTypeOctetString:
        case EValueTypeIPAddress:
        default:
        {
            m_byteArrValue.resize(i_byteArrValue.count());
            memcpy( m_byteArrValue.data(), i_byteArrValue.data(), i_byteArrValue.count() );
            break;
        }
    }

} // ctor

//------------------------------------------------------------------------------
CValue::CValue( const QString& i_strOid, const SValueCounter64& i_counter64Value ) :
//------------------------------------------------------------------------------
    m_oid(i_strOid),
    m_pduDataType(EPDUDataTypeUndefined),
    m_valueType(EValueTypeCounter64),
    m_bValue(false),
    m_iValue(0),
    m_uValue(0),
    m_fValue(0.0),
    m_byteArrValue(),
    m_counter64Value(i_counter64Value),
    m_pOidValue(NULL)
{
} // ctor

//------------------------------------------------------------------------------
CValue::CValue( const QString& i_strOid, const COid& i_oidValue ) :
//------------------------------------------------------------------------------
    m_oid(i_strOid),
    m_pduDataType(EPDUDataTypeUndefined),
    m_valueType(EValueTypeOid),
    m_bValue(false),
    m_iValue(0),
    m_uValue(0),
    m_fValue(0.0),
    m_byteArrValue(),
    m_counter64Value(),
    m_pOidValue(NULL)
{
    m_pOidValue = new COid(i_oidValue);

} // ctor

//------------------------------------------------------------------------------
CValue::CValue(
    TSubId*       i_puSubIds,
    int           i_iSubIdsCount,
    EValueType    i_valueType,
    char*         i_pcValue,
    int           i_iValLen,
    unsigned char i_pduDataType ) :
//------------------------------------------------------------------------------
    m_oid(i_puSubIds,i_iSubIdsCount),
    m_pduDataType(i_pduDataType),
    m_valueType(i_valueType),
    m_bValue(false),
    m_iValue(0),
    m_uValue(0),
    m_fValue(0.0),
    m_byteArrValue(),
    m_counter64Value(),
    m_pOidValue(NULL)
{
    if( i_pcValue != NULL && i_iValLen > 0 )
    {
        switch( i_valueType )
        {
            case EValueTypeNull:
            {
                break;
            }
            case EValueTypeBoolean:
            {
                bool* pVal = reinterpret_cast<bool*>(i_pcValue);
                m_bValue = *pVal;
                break;
            }
            case EValueTypeInt:
            {
                qint32* pVal = reinterpret_cast<qint32*>(i_pcValue);
                m_iValue = *pVal;
                break;
            }
            case EValueTypeUInt:
            {
                quint32* pVal = reinterpret_cast<quint32*>(i_pcValue);
                m_uValue = *pVal;
                break;
            }
            case EValueTypeTimeTicks:
            {
                quint32* pVal = reinterpret_cast<quint32*>(i_pcValue);
                m_uValue = *pVal;
                break;
            }
            case EValueTypeFloat:
            {
                float* pVal = reinterpret_cast<float*>(i_pcValue);
                m_fValue = *pVal;
                break;
            }
            case EValueTypeDouble:
            {
                double* pVal = reinterpret_cast<double*>(i_pcValue);
                m_fValue = *pVal;
                break;
            }
            case EValueTypeCounter32:
            {
                quint32* pVal = reinterpret_cast<quint32*>(i_pcValue);
                m_uValue = *pVal;
                break;
            }
            case EValueTypeCounter64:
            {
                SValueCounter64* pVal = reinterpret_cast<SValueCounter64*>(i_pcValue);
                m_counter64Value = *pVal;
                break;
            }
            case EValueTypeOid:
            {
                TSubId* pVal = reinterpret_cast<TSubId*>(i_pcValue);
                m_pOidValue = new COid( pVal, i_iValLen/sizeof(TSubId) );
                break;
            }
            case EValueTypeIPAddress:
            {
                unsigned char* pVal = reinterpret_cast<unsigned char*>(i_pcValue);
                int            idxSubId;
                QByteArray     szTmp1;

                for( idxSubId = 0; idxSubId < 4; idxSubId++ )
                {
                    if( idxSubId < i_iValLen )
                    {
                        szTmp1 = QByteArray::number(*pVal);
                    }
                    else
                    {
                        szTmp1 = "0";
                    }
                    m_byteArrValue.append(szTmp1);

                    if( idxSubId < 3 )
                    {
                        m_byteArrValue.append(".");
                    }
                }
                break;
            }
            case EValueTypeString:
            case EValueTypeBitString:
            case EValueTypeOctetString:
            default:
            {
                m_byteArrValue.resize(i_iValLen);
                memcpy( m_byteArrValue.data(), i_pcValue, i_iValLen );
                break;
            }
        }
    }

} // ctor

//------------------------------------------------------------------------------
CValue::CValue( TSubId* i_puSubIds, int i_iSubIdsCount, bool i_bValue ) :
//------------------------------------------------------------------------------
    m_oid(i_puSubIds,i_iSubIdsCount),
    m_pduDataType(EPDUDataTypeUndefined),
    m_valueType(EValueTypeBoolean),
    m_bValue(i_bValue),
    m_iValue(0),
    m_uValue(0),
    m_fValue(0.0),
    m_byteArrValue(),
    m_counter64Value(),
    m_pOidValue(NULL)
{
} // ctor

//------------------------------------------------------------------------------
CValue::CValue( TSubId* i_puSubIds, int i_iSubIdsCount, qint32 i_iValue ) :
//------------------------------------------------------------------------------
    m_oid(i_puSubIds,i_iSubIdsCount),
    m_pduDataType(EPDUDataTypeUndefined),
    m_valueType(EValueTypeInt),
    m_bValue(false),
    m_iValue(i_iValue),
    m_uValue(0),
    m_fValue(0.0),
    m_byteArrValue(),
    m_counter64Value(),
    m_pOidValue(NULL)
{
} // ctor

//------------------------------------------------------------------------------
CValue::CValue( TSubId* i_puSubIds, int i_iSubIdsCount, quint32 i_uValue, EValueType i_valueType ) :
//------------------------------------------------------------------------------
    m_oid(i_puSubIds,i_iSubIdsCount),
    m_pduDataType(EPDUDataTypeUndefined),
    m_valueType(EValueTypeInt),
    m_bValue(false),
    m_iValue(0),
    m_uValue(i_uValue),
    m_fValue(0.0),
    m_byteArrValue(),
    m_counter64Value(),
    m_pOidValue(NULL)
{
    if( i_valueType != EValueTypeUInt && i_valueType != EValueTypeTimeTicks && i_valueType != EValueTypeCounter32 )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultArgOutOfRange,"valueType must be UInt, TimeTicks, or Counter32");
    }

} // ctor

//------------------------------------------------------------------------------
CValue::CValue( TSubId* i_puSubIds, int i_iSubIdsCount, double i_fValue, EValueType i_valueType ) :
//------------------------------------------------------------------------------
    m_oid(i_puSubIds,i_iSubIdsCount),
    m_pduDataType(EPDUDataTypeUndefined),
    m_valueType(i_valueType),
    m_bValue(false),
    m_iValue(0),
    m_uValue(0),
    m_fValue(i_fValue),
    m_byteArrValue(),
    m_counter64Value(),
    m_pOidValue(NULL)
{
    if( i_valueType != EValueTypeFloat && i_valueType != EValueTypeDouble )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultArgOutOfRange,"valueType must be Float or Double");
    }

} // ctor

//------------------------------------------------------------------------------
CValue::CValue( TSubId* i_puSubIds, int i_iSubIdsCount, QByteArray& i_byteArrValue, EValueType i_valueType ) :
//------------------------------------------------------------------------------
    m_oid(i_puSubIds,i_iSubIdsCount),
    m_pduDataType(EPDUDataTypeUndefined),
    m_valueType(i_valueType),
    m_bValue(false),
    m_iValue(0),
    m_uValue(0),
    m_fValue(0.0),
    m_byteArrValue(),
    m_counter64Value(),
    m_pOidValue(NULL)
{
    switch( i_valueType )
    {
        case EValueTypeNull:
        {
            break;
        }
        case EValueTypeBoolean:
        {
            bool* pVal = reinterpret_cast<bool*>(i_byteArrValue.data());
            m_bValue = *pVal;
            break;
        }
        case EValueTypeInt:
        {
            qint32* pVal = reinterpret_cast<qint32*>(i_byteArrValue.data());
            m_iValue = *pVal;
            break;
        }
        case EValueTypeUInt:
        {
            quint32* pVal = reinterpret_cast<quint32*>(i_byteArrValue.data());
            m_uValue = *pVal;
            break;
        }
        case EValueTypeTimeTicks:
        {
            quint32* pVal = reinterpret_cast<quint32*>(i_byteArrValue.data());
            m_uValue = *pVal;
            break;
        }
        case EValueTypeFloat:
        {
            float* pVal = reinterpret_cast<float*>(i_byteArrValue.data());
            m_fValue = *pVal;
            break;
        }
        case EValueTypeDouble:
        {
            double* pVal = reinterpret_cast<double*>(i_byteArrValue.data());
            m_fValue = *pVal;
            break;
        }
        case EValueTypeCounter32:
        {
            quint32* pVal = reinterpret_cast<quint32*>(i_byteArrValue.data());
            m_uValue = *pVal;
            break;
        }
        case EValueTypeCounter64:
        {
            SValueCounter64* pVal = reinterpret_cast<SValueCounter64*>(i_byteArrValue.data());
            m_counter64Value = *pVal;
            break;
        }
        case EValueTypeOid:
        {
            TSubId* pVal = reinterpret_cast<TSubId*>(i_byteArrValue.data());
            m_pOidValue = new COid( pVal, i_byteArrValue.count()/sizeof(TSubId) );
            break;
        }
        case EValueTypeString:
        case EValueTypeBitString:
        case EValueTypeOctetString:
        case EValueTypeIPAddress:
        default:
        {
            m_byteArrValue.resize(i_byteArrValue.count());
            memcpy( m_byteArrValue.data(), i_byteArrValue.data(), i_byteArrValue.count() );
            break;
        }
    }

} // ctor

//------------------------------------------------------------------------------
CValue::CValue( TSubId* i_puSubIds, int i_iSubIdsCount, const SValueCounter64& i_counter64Value ) :
//------------------------------------------------------------------------------
    m_oid(i_puSubIds,i_iSubIdsCount),
    m_pduDataType(EPDUDataTypeUndefined),
    m_valueType(EValueTypeCounter64),
    m_bValue(false),
    m_iValue(0),
    m_uValue(0),
    m_fValue(0.0),
    m_byteArrValue(),
    m_counter64Value(i_counter64Value),
    m_pOidValue(NULL)
{
} // ctor

//------------------------------------------------------------------------------
CValue::CValue( TSubId* i_puSubIds, int i_iSubIdsCount, const COid& i_oidValue ) :
//------------------------------------------------------------------------------
    m_oid(i_puSubIds,i_iSubIdsCount),
    m_pduDataType(EPDUDataTypeUndefined),
    m_valueType(EValueTypeOid),
    m_bValue(false),
    m_iValue(0),
    m_uValue(0),
    m_fValue(0.0),
    m_byteArrValue(),
    m_counter64Value(),
    m_pOidValue(NULL)
{
    m_pOidValue = new COid(i_oidValue);

} // ctor

//------------------------------------------------------------------------------
CValue::~CValue()
//------------------------------------------------------------------------------
{
    try
    {
        delete m_pOidValue;
    }
    catch(...)
    {
    }
    m_pOidValue = NULL;

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
void CValue::operator = ( const CValue& i_value )
//------------------------------------------------------------------------------
{
    m_oid = i_value.m_oid;
    m_pduDataType = i_value.m_pduDataType;
    m_valueType = i_value.m_valueType;
    m_bValue = i_value.m_bValue;
    m_iValue = i_value.m_iValue;
    m_uValue = i_value.m_uValue;
    m_fValue = i_value.m_fValue;
    m_byteArrValue = i_value.m_byteArrValue;
    m_counter64Value = i_value.m_counter64Value;
    m_pOidValue = i_value.m_pOidValue;

    if( i_value.m_pOidValue != NULL )
    {
        m_pOidValue = new COid(*i_value.m_pOidValue);
    }

} // operator =

/*==============================================================================
public: // instance members
==============================================================================*/

//------------------------------------------------------------------------------
void CValue::setOid( const COid& i_oid )
//------------------------------------------------------------------------------
{
    m_oid = i_oid;
}

//------------------------------------------------------------------------------
void CValue::setOid( const QString& i_strOid )
//------------------------------------------------------------------------------
{
    m_oid = i_strOid;
}

//------------------------------------------------------------------------------
void CValue::setOid( TSubId* i_puSubIds, int i_iSubIdsCount )
//------------------------------------------------------------------------------
{
    m_oid = COid(i_puSubIds,i_iSubIdsCount);
}

//------------------------------------------------------------------------------
COid* CValue::getOid()
//------------------------------------------------------------------------------
{
    return &m_oid;
}

//------------------------------------------------------------------------------
void CValue::setValueType( EValueType i_valueType )
//------------------------------------------------------------------------------
{
    if( m_valueType != EValueTypeUndefined )
    {
        switch( m_valueType )
        {
            case EValueTypeNull:
            {
                break;
            }
            case EValueTypeBoolean:
            {
                switch( i_valueType )
                {
                    case EValueTypeNull:
                    {
                        break;
                    }
                    case EValueTypeBoolean:
                    {
                        break;
                    }
                    case EValueTypeInt:
                    {
                        m_iValue = m_bValue;
                        m_bValue = false;
                        break;
                    }
                    case EValueTypeUInt:
                    {
                        m_uValue = m_bValue;
                        m_bValue = false;
                        break;
                    }
                    case EValueTypeTimeTicks:
                    {
                        m_uValue = m_bValue;
                        m_bValue = false;
                        break;
                    }
                    case EValueTypeFloat:
                    case EValueTypeDouble:
                    {
                        m_fValue = m_bValue;
                        m_bValue = false;
                        break;
                    }
                    case EValueTypeString:
                    case EValueTypeBitString:
                    case EValueTypeOctetString:
                    {
                        m_byteArrValue = QByteArray::number(m_bValue);
                        m_bValue = false;
                        break;
                    }
                    case EValueTypeCounter32:
                    {
                        m_uValue = m_bValue;
                        m_bValue = false;
                        break;
                    }
                    case EValueTypeCounter64:
                    {
                        m_counter64Value.m_uHighWord = 0;
                        m_counter64Value.m_uLowWord = m_bValue;
                        m_bValue = false;
                        break;
                    }
                    case EValueTypeIPAddress:
                    default:
                    {
                        m_bValue = false; // conversion not possible
                        break;
                    }
                }
                break;
            } // case m_valueType:EValueTypeBoolean

            case EValueTypeInt:
            {
                switch( i_valueType )
                {
                    case EValueTypeNull:
                    {
                        break;
                    }
                    case EValueTypeBoolean:
                    {
                        m_bValue = m_iValue;
                        m_iValue = 0;
                        break;
                    }
                    case EValueTypeInt:
                    {
                        break;
                    }
                    case EValueTypeUInt:
                    {
                        m_uValue = m_iValue;
                        m_iValue = 0;
                        break;
                    }
                    case EValueTypeTimeTicks:
                    {
                        m_uValue = m_iValue;
                        m_iValue = 0;
                        break;
                    }
                    case EValueTypeFloat:
                    case EValueTypeDouble:
                    {
                        m_fValue = m_iValue;
                        m_iValue = 0;
                        break;
                    }
                    case EValueTypeString:
                    case EValueTypeBitString:
                    case EValueTypeOctetString:
                    {
                        m_byteArrValue = QByteArray::number(m_iValue);
                        m_iValue = 0;
                        break;
                    }
                    case EValueTypeCounter32:
                    {
                        m_uValue = m_iValue;
                        m_iValue = 0;
                        break;
                    }
                    case EValueTypeCounter64:
                    {
                        m_counter64Value.m_uHighWord = 0;
                        m_counter64Value.m_uLowWord = m_iValue;
                        m_iValue = 0;
                        break;
                    }
                    case EValueTypeIPAddress:
                    default:
                    {
                        m_iValue = 0; // conversion not possible
                        break;
                    }
                }
                break;
            } // case m_valueType:EValueTypeInt

            case EValueTypeUInt:
            {
                switch( i_valueType )
                {
                    case EValueTypeNull:
                    {
                        break;
                    }
                    case EValueTypeBoolean:
                    {
                        m_bValue = m_uValue;
                        m_uValue = 0;
                        break;
                    }
                    case EValueTypeInt:
                    {
                        m_iValue = m_uValue;
                        m_uValue = 0;
                        break;
                    }
                    case EValueTypeUInt:
                    {
                        break;
                    }
                    case EValueTypeTimeTicks:
                    {
                        break;
                    }
                    case EValueTypeFloat:
                    case EValueTypeDouble:
                    {
                        m_fValue = m_uValue;
                        m_uValue = 0;
                        break;
                    }
                    case EValueTypeString:
                    case EValueTypeBitString:
                    case EValueTypeOctetString:
                    {
                        m_byteArrValue = QByteArray::number(m_uValue);
                        m_uValue = 0;
                        break;
                    }
                    case EValueTypeCounter32:
                    {
                        break;
                    }
                    case EValueTypeCounter64:
                    {
                        m_counter64Value.m_uHighWord = 0;
                        m_counter64Value.m_uLowWord = m_uValue;
                        m_uValue = 0;
                        break;
                    }
                    case EValueTypeIPAddress:
                    default:
                    {
                        m_uValue = 0; // conversion not possible
                        break;
                    }
                }
                break;
            } // case m_valueType:EValueTypeUInt

            case EValueTypeTimeTicks:
            {
                switch( i_valueType )
                {
                    case EValueTypeNull:
                    {
                        break;
                    }
                    case EValueTypeBoolean:
                    {
                        m_bValue = m_uValue;
                        m_uValue = 0;
                        break;
                    }
                    case EValueTypeInt:
                    {
                        m_iValue = m_uValue;
                        m_uValue = 0;
                        break;
                    }
                    case EValueTypeUInt:
                    {
                        break;
                    }
                    case EValueTypeTimeTicks:
                    {
                        break;
                    }
                    case EValueTypeFloat:
                    case EValueTypeDouble:
                    {
                        m_fValue = m_uValue;
                        m_uValue = 0;
                        break;
                    }
                    case EValueTypeString:
                    case EValueTypeBitString:
                    case EValueTypeOctetString:
                    {
                        m_byteArrValue = QByteArray::number(m_uValue);
                        m_uValue = 0;
                        break;
                    }
                    case EValueTypeIPAddress:
                    {
                        m_uValue = 0; // conversion not possible
                        break;
                    }
                    case EValueTypeCounter32:
                    {
                        break;
                    }
                    case EValueTypeCounter64:
                    {
                        m_counter64Value.m_uHighWord = 0;
                        m_counter64Value.m_uLowWord = m_uValue;
                        m_uValue = 0;
                        break;
                    }
                    default:
                    {
                        m_uValue = 0; // conversion not possible
                        break;
                    }
                }
                break;
            } // case m_valueType:EValueTypeTimeTicks

            case EValueTypeFloat:
            case EValueTypeDouble:
            {
                switch( i_valueType )
                {
                    case EValueTypeNull:
                    {
                        break;
                    }
                    case EValueTypeBoolean:
                    {
                        m_bValue = m_iValue;
                        m_fValue = 0.0;
                        break;
                    }
                    case EValueTypeInt:
                    {
                        m_iValue = static_cast<qint32>(m_fValue);
                        m_fValue = 0.0;
                        break;
                    }
                    case EValueTypeUInt:
                    {
                        m_uValue = static_cast<quint32>(m_fValue);
                        m_fValue = 0.0;
                        break;
                    }
                    case EValueTypeTimeTicks:
                    {
                        m_uValue = static_cast<quint32>(m_fValue);
                        m_fValue = 0.0;
                        break;
                    }
                    case EValueTypeFloat:
                    case EValueTypeDouble:
                    {
                        break;
                    }
                    case EValueTypeString:
                    case EValueTypeBitString:
                    case EValueTypeOctetString:
                    {
                        m_byteArrValue = QByteArray::number(m_fValue);
                        break;
                    }
                    case EValueTypeIPAddress:
                    {
                        m_fValue = 0.0; // conversion not possible
                        break;
                    }
                    case EValueTypeCounter32:
                    {
                        m_uValue = static_cast<quint32>(m_fValue);
                        m_fValue = 0.0;
                        break;
                    }
                    case EValueTypeCounter64:
                    {
                        m_counter64Value.m_uHighWord = 0;
                        m_counter64Value.m_uLowWord = static_cast<unsigned long>(m_fValue);
                        m_fValue = 0.0;
                        break;
                    }
                    default:
                    {
                        m_fValue = 0.0; // conversion not possible
                        break;
                    }
                }
                break;
            } // case m_valueType:EValueTypeFloat||m_valueType:EValueTypeDouble

            case EValueTypeString:
            case EValueTypeBitString:
            case EValueTypeOctetString:
            case EValueTypeIPAddress:
            {
                switch( i_valueType )
                {
                    case EValueTypeNull:
                    {
                        break;
                    }
                    case EValueTypeBoolean:
                    {
                        m_bValue = m_byteArrValue.toInt();
                        m_byteArrValue.clear();
                        break;
                    }
                    case EValueTypeInt:
                    {
                        m_iValue = m_byteArrValue.toInt();
                        m_byteArrValue.clear();
                        break;
                    }
                    case EValueTypeUInt:
                    {
                        m_uValue = m_byteArrValue.toUInt();
                        m_byteArrValue.clear();
                        break;
                    }
                    case EValueTypeTimeTicks:
                    {
                        m_uValue = m_byteArrValue.toUInt();
                        m_byteArrValue.clear();
                        break;
                    }
                    case EValueTypeFloat:
                    {
                        m_fValue = m_byteArrValue.toFloat();
                        m_byteArrValue.clear();
                        break;
                    }
                    case EValueTypeDouble:
                    {
                        m_fValue = m_byteArrValue.toDouble();
                        m_byteArrValue.clear();
                        break;
                    }
                    case EValueTypeString:
                    case EValueTypeBitString:
                    case EValueTypeOctetString:
                    case EValueTypeIPAddress:
                    {
                        break;
                    }
                    case EValueTypeCounter32:
                    {
                        m_uValue = m_byteArrValue.toInt();
                        m_byteArrValue.clear();
                        break;
                    }
                    case EValueTypeCounter64:
                    {
                        m_counter64Value.m_uHighWord = 0;
                        m_counter64Value.m_uLowWord = m_byteArrValue.toUInt();
                        m_byteArrValue.clear();
                        break;
                    }
                    case EValueTypeOid:
                    {
                        m_pOidValue = new COid(m_byteArrValue);
                        m_byteArrValue.clear();
                        break;
                    }
                    default:
                    {
                        m_byteArrValue.clear(); // conversion not possible
                        break;
                    }
                }
                break;
            } // case m_valueType:EValueTypeString || m_valueType:EValueTypeBitString

            case EValueTypeCounter32:
            {
                switch( i_valueType )
                {
                    case EValueTypeNull:
                    {
                        break;
                    }
                    case EValueTypeBoolean:
                    {
                        m_bValue = m_uValue;
                        m_uValue = 0;
                        break;
                    }
                    case EValueTypeInt:
                    {
                        m_iValue = m_uValue;
                        m_uValue = 0;
                        break;
                    }
                    case EValueTypeUInt:
                    {
                        break;
                    }
                    case EValueTypeTimeTicks:
                    {
                        break;
                    }
                    case EValueTypeFloat:
                    case EValueTypeDouble:
                    {
                        m_fValue = m_uValue;
                        m_uValue = 0;
                        break;
                    }
                    case EValueTypeString:
                    case EValueTypeBitString:
                    case EValueTypeOctetString:
                    {
                        m_byteArrValue = QByteArray::number(m_uValue);
                        m_uValue = 0;
                        break;
                    }
                    case EValueTypeCounter32:
                    {
                        break;
                    }
                    case EValueTypeCounter64:
                    {
                        m_counter64Value.m_uHighWord = 0;
                        m_counter64Value.m_uLowWord = m_uValue;
                        m_uValue = 0;
                        break;
                    }
                    case EValueTypeIPAddress:
                    default:
                    {
                        m_uValue = 0; // conversion not possible
                        break;
                    }
                }
                break;
            } // case m_valueType:EValueTypeCounter32

            case EValueTypeCounter64:
            {
                switch( i_valueType )
                {
                    case EValueTypeNull:
                    {
                        break;
                    }
                    case EValueTypeBoolean:
                    {
                        m_bValue = m_counter64Value.m_uLowWord;
                        m_counter64Value.m_uHighWord = 0;
                        m_counter64Value.m_uLowWord = 0;
                        break;
                    }
                    case EValueTypeInt:
                    {
                        m_iValue = m_counter64Value.m_uLowWord;
                        m_counter64Value.m_uHighWord = 0;
                        m_counter64Value.m_uLowWord = 0;
                        break;
                    }
                    case EValueTypeUInt:
                    {
                        m_uValue = m_counter64Value.m_uLowWord;
                        m_counter64Value.m_uHighWord = 0;
                        m_counter64Value.m_uLowWord = 0;
                        break;
                    }
                    case EValueTypeTimeTicks:
                    {
                        m_uValue = m_counter64Value.m_uLowWord;
                        m_counter64Value.m_uHighWord = 0;
                        m_counter64Value.m_uLowWord = 0;
                        break;
                    }
                    case EValueTypeFloat:
                    case EValueTypeDouble:
                    {
                        m_fValue = m_counter64Value.m_uLowWord;
                        m_counter64Value.m_uHighWord = 0;
                        m_counter64Value.m_uLowWord = 0;
                        break;
                    }
                    case EValueTypeString:
                    case EValueTypeBitString:
                    case EValueTypeOctetString:
                    {
                        m_byteArrValue  = QByteArray::number(static_cast<qulonglong>(m_counter64Value.m_uHighWord));
                        m_byteArrValue += QByteArray::number(static_cast<qulonglong>(m_counter64Value.m_uLowWord));
                        m_counter64Value.m_uHighWord = 0;
                        m_counter64Value.m_uLowWord = 0;
                        break;
                    }
                    case EValueTypeCounter32:
                    {
                        m_uValue = m_counter64Value.m_uLowWord;
                        m_counter64Value.m_uHighWord = 0;
                        m_counter64Value.m_uLowWord = 0;
                        break;
                    }
                    case EValueTypeCounter64:
                    {
                        break;
                    }
                    case EValueTypeIPAddress:
                    default:
                    {
                        m_counter64Value.m_uHighWord = 0; // conversion not possible
                        m_counter64Value.m_uLowWord = 0;
                        break;
                    }
                }
                break;
            } // case m_valueType:EValueTypeCounter64

            case EValueTypeOid:
            {
                switch( i_valueType )
                {
                    case EValueTypeNull:
                    case EValueTypeBoolean:
                    case EValueTypeInt:
                    case EValueTypeUInt:
                    case EValueTypeTimeTicks:
                    case EValueTypeFloat:
                    case EValueTypeDouble:
                    {
                        delete m_pOidValue; // conversion not possible
                        m_pOidValue = NULL;
                        break;
                    }
                    case EValueTypeOid:
                    {
                        break;
                    }
                    case EValueTypeString:
                    case EValueTypeBitString:
                    case EValueTypeOctetString:
                    {
                        if( m_pOidValue != NULL )
                        {
                            m_byteArrValue = m_pOidValue->toString(EOidFormatNumeric).toAscii();
                        }
                        delete m_pOidValue;
                        m_pOidValue = NULL;
                        break;
                    }
                    case EValueTypeIPAddress:
                    case EValueTypeCounter32:
                    case EValueTypeCounter64:
                    default:
                    {
                        delete m_pOidValue; // conversion not possible
                        m_pOidValue = NULL;
                        break;
                    }
                }
                break;
            } // case m_valueType:EValueTypeOid

            default:
            {
                break;
            }
        } // switch( m_valueType )

    } // if( m_valueType != EValueTypeUndefined )

    m_valueType = i_valueType;

} // setValueType

//------------------------------------------------------------------------------
EValueType CValue::getValueType() const
//------------------------------------------------------------------------------
{
    return m_valueType;
}

//------------------------------------------------------------------------------
bool CValue::isInstanceNodeValueType() const
//------------------------------------------------------------------------------
{
    bool bIsInstanceNodeValueType = true;

    switch( m_valueType )
    {
        case EValueTypeUndefined:
        case EValueTypeNull:
        case EValueTypeSequence:
        case EValueTypeOpaque:
        case EValueTypeNoSuchObject:
        case EValueTypeNoSuchInstance:
        case EValueTypeEndOfMibView:
        {
            bIsInstanceNodeValueType = false;
            break;
        }
        case EValueTypeBoolean:
        case EValueTypeInt:
        case EValueTypeUInt:
        case EValueTypeTimeTicks:
        case EValueTypeFloat:
        case EValueTypeDouble:
        case EValueTypeString:
        case EValueTypeBitString:
        case EValueTypeOctetString:
        case EValueTypeIPAddress:
        case EValueTypeCounter32:
        case EValueTypeCounter64:
        case EValueTypeOid:
        default:
        {
            break;
        }
    }
    return bIsInstanceNodeValueType;

} // isInstanceNodeValueType

//------------------------------------------------------------------------------
unsigned char CValue::getPDUDataType() const
//------------------------------------------------------------------------------
{
    return m_pduDataType;
}

//------------------------------------------------------------------------------
void CValue::setValue( EValueType i_valueType, char* i_pcValue, int i_iValLen )
//------------------------------------------------------------------------------
{
    if( m_valueType != EValueTypeUndefined || i_pcValue == NULL || i_iValLen == 0 )
    {
        m_bValue = false;
        m_iValue = 0;
        m_uValue = 0;
        m_fValue = 0.0;
        m_byteArrValue.clear();
        m_counter64Value.m_uHighWord = 0;
        m_counter64Value.m_uLowWord = 0;
        delete m_pOidValue;
        m_pOidValue = NULL;
    }

    m_valueType = i_valueType;

    if( i_pcValue != NULL && i_iValLen > 0 )
    {
        switch( i_valueType )
        {
            case EValueTypeNull:
            {
                break;
            }
            case EValueTypeBoolean:
            {
                bool* pVal = reinterpret_cast<bool*>(i_pcValue);
                m_bValue = *pVal;
                break;
            }
            case EValueTypeInt:
            {
                qint32* pVal = reinterpret_cast<qint32*>(i_pcValue);
                m_iValue = *pVal;
                break;
            }
            case EValueTypeUInt:
            {
                quint32* pVal = reinterpret_cast<quint32*>(i_pcValue);
                m_uValue = *pVal;
                break;
            }
            case EValueTypeTimeTicks:
            {
                quint32* pVal = reinterpret_cast<quint32*>(i_pcValue);
                m_uValue = *pVal;
                break;
            }
            case EValueTypeFloat:
            {
                float* pVal = reinterpret_cast<float*>(i_pcValue);
                m_fValue = *pVal;
                break;
            }
            case EValueTypeDouble:
            {
                double* pVal = reinterpret_cast<double*>(i_pcValue);
                m_fValue = *pVal;
                break;
            }
            case EValueTypeCounter32:
            {
                quint32* pVal = reinterpret_cast<quint32*>(i_pcValue);
                m_uValue = *pVal;
                break;
            }
            case EValueTypeCounter64:
            {
                SValueCounter64* pVal = reinterpret_cast<SValueCounter64*>(i_pcValue);
                m_counter64Value = *pVal;
                break;
            }
            case EValueTypeOid:
            {
                TSubId* pVal = reinterpret_cast<TSubId*>(i_pcValue);
                m_pOidValue = new COid( pVal, i_iValLen/sizeof(TSubId) );
                break;
            }
            case EValueTypeIPAddress:
            {
                unsigned char* pVal = reinterpret_cast<unsigned char*>(i_pcValue);
                int            idxSubId;
                QByteArray     szTmp1;

                for( idxSubId = 0; idxSubId < 4; idxSubId++ )
                {
                    if( idxSubId < i_iValLen )
                    {
                        szTmp1 = QByteArray::number(*pVal);
                    }
                    else
                    {
                        szTmp1 = "0";
                    }
                    m_byteArrValue.append(szTmp1);

                    if( idxSubId < 3 )
                    {
                        m_byteArrValue.append(".");
                    }
                }
                break;
            }
            case EValueTypeString:
            case EValueTypeBitString:
            case EValueTypeOctetString:
            default:
            {
                m_byteArrValue.resize(i_iValLen);
                memcpy( m_byteArrValue.data(), i_pcValue, i_iValLen );
                break;
            }
        }
    }

} // setValue(char*,int)

//------------------------------------------------------------------------------
void CValue::setValue( bool i_bValue )
//------------------------------------------------------------------------------
{
    if( m_valueType != EValueTypeUndefined )
    {
        m_bValue = false;
        m_iValue = 0;
        m_uValue = 0;
        m_fValue = 0.0;
        m_byteArrValue.clear();
        m_counter64Value.m_uHighWord = 0;
        m_counter64Value.m_uLowWord = 0;
        delete m_pOidValue;
        m_pOidValue = NULL;
    }
    m_valueType = EValueTypeBoolean;
    m_bValue = i_bValue;

} // setValue(bool)

//------------------------------------------------------------------------------
void CValue::setValue( qint32 i_iValue )
//------------------------------------------------------------------------------
{
    if( m_valueType != EValueTypeUndefined )
    {
        m_bValue = false;
        m_iValue = 0;
        m_uValue = 0;
        m_fValue = 0.0;
        m_byteArrValue.clear();
        m_counter64Value.m_uHighWord = 0;
        m_counter64Value.m_uLowWord = 0;
        delete m_pOidValue;
        m_pOidValue = NULL;
    }
    m_valueType = EValueTypeInt;
    m_iValue = i_iValue;

} // setValue(qint32)

//------------------------------------------------------------------------------
void CValue::setValue( quint32 i_uValue, EValueType i_valueType )
//------------------------------------------------------------------------------
{
    if( i_valueType != EValueTypeUInt && i_valueType != EValueTypeTimeTicks && i_valueType != EValueTypeCounter32 )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultArgOutOfRange,"valueType must be UInt, TimeTicks, or Counter32");
    }
    if( m_valueType != EValueTypeUndefined )
    {
        m_bValue = false;
        m_iValue = 0;
        m_uValue = 0;
        m_fValue = 0.0;
        m_byteArrValue.clear();
        m_counter64Value.m_uHighWord = 0;
        m_counter64Value.m_uLowWord = 0;
        delete m_pOidValue;
        m_pOidValue = NULL;
    }
    m_valueType = i_valueType;
    m_uValue = i_uValue;

} // setValue(quint32)

//------------------------------------------------------------------------------
void CValue::setValue( double i_fValue, EValueType i_valueType )
//------------------------------------------------------------------------------
{
    if( i_valueType != EValueTypeFloat && i_valueType != EValueTypeDouble )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultArgOutOfRange,"valueType must be Float or Double");
    }
    if( m_valueType != EValueTypeUndefined )
    {
        m_bValue = false;
        m_iValue = 0;
        m_uValue = 0;
        m_fValue = 0.0;
        m_byteArrValue.clear();
        m_counter64Value.m_uHighWord = 0;
        m_counter64Value.m_uLowWord = 0;
        delete m_pOidValue;
        m_pOidValue = NULL;
    }
    m_valueType = i_valueType;
    m_fValue = i_fValue;

} // setValue(double)

//------------------------------------------------------------------------------
void CValue::setValue( QByteArray& i_byteArrValue, EValueType i_valueType )
//------------------------------------------------------------------------------
{
    if( m_valueType != EValueTypeUndefined )
    {
        m_bValue = false;
        m_iValue = 0;
        m_uValue = 0;
        m_fValue = 0.0;
        m_byteArrValue.clear();
        m_counter64Value.m_uHighWord = 0;
        m_counter64Value.m_uLowWord = 0;
        delete m_pOidValue;
        m_pOidValue = NULL;
    }

    m_valueType = i_valueType;

    switch( i_valueType )
    {
        case EValueTypeBoolean:
        {
            bool* pVal = reinterpret_cast<bool*>(i_byteArrValue.data());
            m_bValue = *pVal;
            break;
        }
        case EValueTypeInt:
        {
            qint32* pVal = reinterpret_cast<qint32*>(i_byteArrValue.data());
            m_iValue = *pVal;
            break;
        }
        case EValueTypeUInt:
        {
            quint32* pVal = reinterpret_cast<quint32*>(i_byteArrValue.data());
            m_uValue = *pVal;
            break;
        }
        case EValueTypeTimeTicks:
        {
            quint32* pVal = reinterpret_cast<quint32*>(i_byteArrValue.data());
            m_uValue = *pVal;
            break;
        }
        case EValueTypeFloat:
        {
            float* pVal = reinterpret_cast<float*>(i_byteArrValue.data());
            m_fValue = *pVal;
            break;
        }
        case EValueTypeDouble:
        {
            double* pVal = reinterpret_cast<double*>(i_byteArrValue.data());
            m_fValue = *pVal;
            break;
        }
        case EValueTypeCounter32:
        {
            quint32* pVal = reinterpret_cast<quint32*>(i_byteArrValue.data());
            m_uValue = *pVal;
            break;
        }
        case EValueTypeCounter64:
        {
            SValueCounter64* pVal = reinterpret_cast<SValueCounter64*>(i_byteArrValue.data());
            m_counter64Value = *pVal;
            break;
        }
        case EValueTypeOid:
        {
            TSubId* pVal = reinterpret_cast<TSubId*>(i_byteArrValue.data());
            m_pOidValue = new COid( pVal, i_byteArrValue.count()/sizeof(TSubId) );
            break;
        }
        case EValueTypeString:
        case EValueTypeBitString:
        case EValueTypeOctetString:
        case EValueTypeIPAddress:
        default:
        {
            m_byteArrValue.resize(i_byteArrValue.count());
            memcpy( m_byteArrValue.data(), i_byteArrValue.data(), i_byteArrValue.count() );
            break;
        }
    }

} // setValue(QByteArray)

//------------------------------------------------------------------------------
void CValue::setValue( const SValueCounter64& i_counter64Value )
//------------------------------------------------------------------------------
{
    if( m_valueType != EValueTypeUndefined )
    {
        m_bValue = false;
        m_iValue = 0;
        m_uValue = 0;
        m_fValue = 0.0;
        m_byteArrValue.clear();
        m_counter64Value.m_uHighWord = 0;
        m_counter64Value.m_uLowWord = 0;
        delete m_pOidValue;
        m_pOidValue = NULL;
    }
    m_counter64Value = i_counter64Value;

} // setValue(SValueCounter64)

//------------------------------------------------------------------------------
void CValue::setValue( const COid& i_oidValue )
//------------------------------------------------------------------------------
{
    if( m_valueType != EValueTypeUndefined )
    {
        m_bValue = false;
        m_iValue = 0;
        m_uValue = 0;
        m_fValue = 0.0;
        m_byteArrValue.clear();
        m_counter64Value.m_uHighWord = 0;
        m_counter64Value.m_uLowWord = 0;
        delete m_pOidValue;
        m_pOidValue = NULL;
    }

    delete m_pOidValue;
    m_pOidValue = new COid(i_oidValue);

} // setValue(COid)

//------------------------------------------------------------------------------
bool CValue::getValueBool() const
//------------------------------------------------------------------------------
{
    bool bValue = m_bValue;

    switch( m_valueType )
    {
        case EValueTypeNull:
        {
            bValue = false;
            break;
        }
        case EValueTypeBoolean:
        {
            break;
        }
        case EValueTypeInt:
        {
            bValue = static_cast<bool>(m_iValue);
            break;
        }
        case EValueTypeUInt:
        case EValueTypeTimeTicks:
        {
            bValue = static_cast<bool>(m_uValue);
            break;
        }
        case EValueTypeFloat:
        {
            bValue = static_cast<bool>(m_fValue);
            break;
        }
        case EValueTypeDouble:
        {
            bValue = static_cast<bool>(m_fValue);
            break;
        }
        case EValueTypeString:
        case EValueTypeBitString:
        case EValueTypeOctetString:
        case EValueTypeIPAddress:
        {
            bValue = m_byteArrValue.toInt();
            break;
        }
        case EValueTypeCounter32:
        {
            bValue = static_cast<bool>(m_uValue);
            break;
        }
        case EValueTypeCounter64:
        {
            bValue = m_counter64Value.m_uLowWord;
            break;
        }
        default:
        {
            break;
        }
    }
    return bValue;

} // getValueBool

//------------------------------------------------------------------------------
int CValue::getValueInt() const
//------------------------------------------------------------------------------
{
    int iValue = m_iValue;

    switch( m_valueType )
    {
        case EValueTypeNull:
        {
            break;
        }
        case EValueTypeBoolean:
        {
            iValue = static_cast<qint32>(m_bValue);
            break;
        }
        case EValueTypeInt:
        {
            break;
        }
        case EValueTypeUInt:
        case EValueTypeTimeTicks:
        {
            iValue = static_cast<qint32>(m_uValue);
            break;
        }
        case EValueTypeFloat:
        {
            iValue = static_cast<qint32>(m_fValue);
            break;
        }
        case EValueTypeDouble:
        {
            iValue = static_cast<qint32>(m_fValue);
            break;
        }
        case EValueTypeString:
        case EValueTypeBitString:
        case EValueTypeOctetString:
        case EValueTypeIPAddress:
        {
            iValue = m_byteArrValue.toLong();
            break;
        }
        case EValueTypeCounter32:
        {
            iValue = static_cast<qint32>(m_uValue);
            break;
        }
        case EValueTypeCounter64:
        {
            iValue = m_counter64Value.m_uLowWord;
            break;
        }
        default:
        {
            break;
        }
    }
    return iValue;

} // getValueInt

//------------------------------------------------------------------------------
float CValue::getValueFloat() const
//------------------------------------------------------------------------------
{
    float fValue = static_cast<float>(m_fValue);

    switch( m_valueType )
    {
        case EValueTypeNull:
        {
            break;
        }
        case EValueTypeBoolean:
        {
            fValue = m_bValue;
            break;
        }
        case EValueTypeInt:
        {
            fValue = m_iValue;
            break;
        }
        case EValueTypeUInt:
        case EValueTypeTimeTicks:
        {
            fValue = m_uValue;
            break;
        }
        case EValueTypeString:
        case EValueTypeBitString:
        case EValueTypeOctetString:
        case EValueTypeIPAddress:
        {
            fValue = m_byteArrValue.toFloat();
            break;
        }
        case EValueTypeCounter32:
        {
            fValue = m_uValue;
            break;
        }
        case EValueTypeCounter64:
        {
            fValue = m_counter64Value.m_uLowWord;
            break;
        }
        default:
        {
            break;
        }
    }
    return fValue;

} // getValueFloat

//------------------------------------------------------------------------------
double CValue::getValueDouble() const
//------------------------------------------------------------------------------
{
    double fValue = m_fValue;

    switch( m_valueType )
    {
        case EValueTypeNull:
        {
            break;
        }
        case EValueTypeBoolean:
        {
            fValue = static_cast<double>(m_bValue);
            break;
        }
        case EValueTypeInt:
        {
            fValue = m_iValue;
            break;
        }
        case EValueTypeUInt:
        case EValueTypeTimeTicks:
        {
            fValue = m_uValue;
            break;
        }
        case EValueTypeString:
        case EValueTypeBitString:
        case EValueTypeOctetString:
        case EValueTypeIPAddress:
        {
            fValue = m_byteArrValue.toDouble();
            break;
        }
        case EValueTypeCounter32:
        {
            fValue = m_uValue;
            break;
        }
        case EValueTypeCounter64:
        {
            fValue = m_counter64Value.m_uLowWord;
            break;
        }
        default:
        {
            break;
        }
    }
    return fValue;

} // getValueDouble

//------------------------------------------------------------------------------
QByteArray CValue::getValueString() const
//------------------------------------------------------------------------------
{
    QByteArray byteArrValue = m_byteArrValue;

    switch( m_valueType )
    {
        case EValueTypeNull:
        {
            break;
        }
        case EValueTypeBoolean:
        {
            byteArrValue = QByteArray::number(m_bValue);
            break;
        }
        case EValueTypeInt:
        {
            byteArrValue = QByteArray::number(m_iValue);
            break;
        }
        case EValueTypeUInt:
        case EValueTypeTimeTicks:
        {
            byteArrValue = QByteArray::number(m_uValue);
            break;
        }
        case EValueTypeFloat:
        case EValueTypeDouble:
        {
            byteArrValue = QByteArray::number(m_fValue);
            break;
        }
        case EValueTypeCounter32:
        {
            byteArrValue = QByteArray::number(m_uValue);
            break;
        }
        case EValueTypeCounter64:
        {
            byteArrValue  = QByteArray::number(static_cast<qulonglong>(m_counter64Value.m_uHighWord));
            byteArrValue += QByteArray::number(static_cast<qulonglong>(m_counter64Value.m_uLowWord));
            break;
        }
        case EValueTypeOid:
        {
            if( m_pOidValue != NULL )
            {
                byteArrValue = m_pOidValue->toString(EOidFormatNumeric).toAscii();
            }
            break;
        }
        case EValueTypeString:
        case EValueTypeBitString:
        case EValueTypeOctetString:
        case EValueTypeIPAddress:
        default:
        {
            break;
        }
    }
    byteArrValue.replace(0x00,' ');

    return byteArrValue;

} // getValueString

//------------------------------------------------------------------------------
SValueCounter64 CValue::getValueCounter64() const
//------------------------------------------------------------------------------
{
    SValueCounter64 counter64Value = m_counter64Value;

    switch( m_valueType )
    {
        case EValueTypeNull:
        {
            break;
        }
        case EValueTypeBoolean:
        {
            counter64Value.m_uHighWord = 0;
            counter64Value.m_uLowWord = m_bValue;
            break;
        }
        case EValueTypeInt:
        {
            counter64Value.m_uHighWord = 0;
            counter64Value.m_uLowWord = m_iValue;
            break;
        }
        case EValueTypeUInt:
        case EValueTypeTimeTicks:
        {
            counter64Value.m_uHighWord = 0;
            counter64Value.m_uLowWord = m_uValue;
            break;
        }
        case EValueTypeFloat:
        case EValueTypeDouble:
        {
            counter64Value.m_uHighWord = 0;
            counter64Value.m_uLowWord = static_cast<unsigned long>(m_fValue);
            break;
        }
        case EValueTypeCounter32:
        {
            counter64Value.m_uHighWord = 0;
            counter64Value.m_uLowWord = m_uValue;
            break;
        }
        case EValueTypeCounter64:
        case EValueTypeOid:
        case EValueTypeString:
        case EValueTypeBitString:
        case EValueTypeOctetString:
        case EValueTypeIPAddress:
        default:
        {
            break;
        }
    }
    return counter64Value;

} // getValueCounter64

//------------------------------------------------------------------------------
COid* CValue::getValueOid()
//------------------------------------------------------------------------------
{
    COid* pOidValue = m_pOidValue;

    switch( m_valueType )
    {
        case EValueTypeString:
        case EValueTypeBitString:
        case EValueTypeOctetString:
        {
            delete m_pOidValue;
            m_pOidValue = new COid(m_byteArrValue);
            break;
        }
        default:
        {
            break;
        }
    }
    return pOidValue;

} // getValueOid
