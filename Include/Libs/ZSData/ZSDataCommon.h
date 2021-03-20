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

#ifndef ZSDataCommon_h
#define ZSDataCommon_h

#include <QtCore/qstringlist.h>

#include "ZSData/ZSDataDllMain.h"
#include "ZSIpc/ZSIpcCommon.h"
#include "ZSSys/ZSSysErrResult.h"

namespace ZS
{
namespace Data
{
/*******************************************************************************
public type definitions and constants
*******************************************************************************/

//==============================================================================
enum ECloneCopyDepth
//==============================================================================
{
    ECloneCopyDepthSchemaObjIdentity = 0,   // Only the schema info which is necessary to identify the object (e.g. to delete the object).
    ECloneCopyDepthSchemaPrepareData = 1,   // Minimum schema info necessary for storing and transferring data.
    ECloneCopyDepthSchemaAll         = 2,   // Complete schema info.
    ECloneCopyDepthDataAll           = 3,   // Data (including minimum schema info necessary for storing and transferring data).
    ECloneCopyDepthAll               = 4,   // Everything (schema and data).
    ECloneCopyDepthCount,
    ECloneCopyDepthUndefined
};

ZSDATADLL_API QString cloneCopyDepth2Str( int i_iDepth, ZS::System::EEnumEntryAliasStr i_alias = ZS::System::EEnumEntryAliasStrName );
ZSDATADLL_API ECloneCopyDepth str2CloneCopyDepth( const QString& i_str, ZS::System::EEnumEntryAliasStr i_alias = ZS::System::EEnumEntryAliasStrName );

//==============================================================================
enum EDataSetObjType
//==============================================================================
{
    // Node objects within data sets (derived from CDataSetObject)
    EDataSetObjTypeDataSet        = 0,
    EDataSetObjTypeGroup          = 1,
    EDataSetObjTypeEnumeration    = 2,
    EDataSetObjTypeTable          = 3,
    EDataSetObjTypeParameter      = 4,
    // Special kind of objects (not derived from CDataSetObject)
    EDataSetObjTypeCount,
    EDataSetObjTypeUndefined
};

ZSDATADLL_API QString dataSetObjType2Str( int i_iType, ZS::System::EEnumEntryAliasStr i_alias = ZS::System::EEnumEntryAliasStrName );
ZSDATADLL_API EDataSetObjType str2DataSetObjType( const QString& i_str, ZS::System::EEnumEntryAliasStr i_alias = ZS::System::EEnumEntryAliasStrName );


//==============================================================================
enum EDataType
//==============================================================================
{
    EDataTypeString     =  0, // 2 bytes per character (unicode string). Terminated by trailing zero ('0x0000').
    EDataTypeByteArr    =  1, // 2^31-4 = 2.147.483.644 bytes maximum length if serialized. Length of array stored in first four bytes.
    EDataTypeBool       =  2, // 1 byte
    EDataTypeEnum       =  3, // 4 bytes (Int32)
    EDataTypeNumericMin =  4,
    EDataTypeIntegerMin =  4,
    EDataTypeInt8       =  4, // 1 byte
    EDataTypeUInt8      =  5, // 1 byte
    EDataTypeInt16      =  6, // 2 bytes
    EDataTypeUInt16     =  7, // 2 bytes
    EDataTypeInt32      =  8, // 4 bytes
    EDataTypeUInt32     =  9, // 4 bytes
    EDataTypeInt64      = 10, // 8 bytes
    EDataTypeUInt64     = 11, // 8 bytes
    EDataTypeIntegerMax = 11,
    EDataTypeDouble     = 12, // 8 bytes
    EDataTypeNumericMax = 12,
    EDataTypeTime       = 13, // QTime is used in data cells
    EDataTypeDate       = 14, // QDate is used in data cells
    EDataTypeDateTime   = 15, // QDateTime is used in data cells
    EDataTypePhysVal    = 16, // CPhysVal is used in data cells
    EDataTypeCount,
    EDataTypeUndefined  // = 14, QVariant is used in data cells
};

ZSDATADLL_API QString dataType2Str( int i_iDataType, ZS::System::EEnumEntryAliasStr i_alias = ZS::System::EEnumEntryAliasStrSymbol );
ZSDATADLL_API EDataType str2DataType( const QString& i_str, ZS::System::EEnumEntryAliasStr i_alias = ZS::System::EEnumEntryAliasStrSymbol );


//==============================================================================
enum EProtocolType
//==============================================================================
{
    EProtocolTypeZSXML = 0, // via TCP/IP
    EProtocolTypeZSMsg = 1, // InProcMsg
    EProtocolTypeSQL   = 2,
    EProtocolTypeCount,
    EProtocolTypeUndefined
};

ZSDATADLL_API QString protocolType2Str( int i_iType, ZS::System::EEnumEntryAliasStr i_alias = ZS::System::EEnumEntryAliasStrSymbol );
ZSDATADLL_API EProtocolType str2ProtocolType( const QString& i_str, ZS::System::EEnumEntryAliasStr i_alias = ZS::System::EEnumEntryAliasStrSymbol );


//******************************************************************************
struct ZSDATADLL_API SCnctId
//******************************************************************************
{
public: // ctors
    SCnctId( EProtocolType i_protocolType = EProtocolTypeUndefined );
    SCnctId( EProtocolType i_protocolType, ZS::Ipc::ESocketType i_socketType );
    SCnctId( EProtocolType i_protocolType, ZS::Ipc::ESocketType i_socketType, int i_iSocketId );        // EProtocolTypeZSXML, (ESocketTypeTcp || ESocketTypeShm)
    SCnctId( EProtocolType i_protocolType, ZS::Ipc::ESocketType i_socketType, QObject* i_pObjDbClt );   // EProtocolTypeZSMsg, ESocketTypeInProcMsg
    //SCnctId( const QString& i_strKeyCnct );
public: // methods
    QString toString() const;
public: // struct members
    EProtocolType        m_protocolType;
    ZS::Ipc::ESocketType m_socketType;
    int                  m_iSocketId;
    QObject*             m_pObjDbClt;

}; // struct SCnctId

//------------------------------------------------------------------------------
inline bool operator == ( const SCnctId& i_key1, const SCnctId& i_key2 )
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    if( i_key1.m_protocolType != i_key2.m_protocolType )
    {
        bEqual = false;
    }
    else if( i_key1.m_socketType != i_key2.m_socketType )
    {
        bEqual = false;
    }
    else if( i_key1.m_iSocketId != i_key2.m_iSocketId )
    {
        bEqual = false;
    }
    else if( i_key1.m_pObjDbClt != i_key2.m_pObjDbClt )
    {
        bEqual = false;
    }
    return bEqual;

} // operator == (SCnctId)

//------------------------------------------------------------------------------
inline bool operator != ( const SCnctId& i_key1, const SCnctId& i_key2 )
//------------------------------------------------------------------------------
{
    return !(i_key1 == i_key2);
}

//------------------------------------------------------------------------------
inline uint qHash( const SCnctId& i_key )
//------------------------------------------------------------------------------
{
    uint uHsh = 0;

    if( i_key.m_pObjDbClt != nullptr )
    {
        uHsh = qHash( QString::number(i_key.m_protocolType) + "." + QString::number(i_key.m_socketType) + "." + QString::number(reinterpret_cast<qint64>(i_key.m_pObjDbClt)) );
    }
    else if( i_key.m_iSocketId >= 0 )
    {
        uHsh = qHash( QString::number(i_key.m_protocolType) + "." + QString::number(i_key.m_socketType) + "." + QString::number(i_key.m_iSocketId) );
    }
    else
    {
        uHsh = qHash( QString::number(i_key.m_protocolType) + "." + QString::number(i_key.m_socketType) );
    }
    return uHsh;

} // qHash::SCnctId


//******************************************************************************
struct ZSDATADLL_API SCnctType
//******************************************************************************
{
public: // ctors
    SCnctType();
    SCnctType( EProtocolType i_protocolType, ZS::Ipc::ESocketType i_socketType );
    SCnctType( const SCnctId& i_cnctId );
    SCnctId toCnctId() const { return SCnctId(m_protocolType, m_socketType); }
public: // methods
    QString toString() const;
public: // struct members
    EProtocolType        m_protocolType;
    ZS::Ipc::ESocketType m_socketType;

}; // struct SCnctType

inline bool operator == ( const SCnctType& i_key1, const SCnctType& i_key2 )
{
    return i_key1.m_protocolType == i_key2.m_protocolType && i_key1.m_socketType == i_key2.m_socketType;
}

inline uint qHash( const SCnctType& i_key )
{
    return qHash( QString::number(i_key.m_protocolType) + "." + QString::number(i_key.m_socketType) );
}


//******************************************************************************
struct ZSDATADLL_API SDSNodeSpec
//******************************************************************************
{
public: // ctors and dtor
    SDSNodeSpec();
    SDSNodeSpec(
        EDataSetObjType i_dsObjType,
        const QString&  i_strObjKey,
        bool            i_bRecursive = false );
    SDSNodeSpec( const SDSNodeSpec& i_other );
public: // struct methods
    void clear();
public: // struct methods
    bool isValid() const;
    QString toString() const;
public: // struct methods
    QString getAddTrcInfoStr( int i_iDetailLevel = 0 ) const;
public: // struct members
    EDataSetObjType m_dsObjType;            // Type of the data set object (table, parameter).
    QString         m_strObjKey;            // Key of the data set object (table, parameter).
    QStringList     m_strlstChildObjNames;  // Usually used to specify columns of a table.
    QString         m_strWhere;             // Filter.
    bool            m_bRecursive;           // Specifies whether children of a group should be included.

}; // struct SDSNodeSpec


//******************************************************************************
struct ZSDATADLL_API SDbClientRegSpec
//******************************************************************************
{
public: // ctors
    SDbClientRegSpec();
    SDbClientRegSpec( const SCnctId& i_cnctId );
public: // operators
    SDbClientRegSpec& operator = ( const SCnctId& i_cnctId );
    SDbClientRegSpec& operator = ( const QVector<SDSNodeSpec>& i_arDSNodeSpecs );
public: // struct methods
    void clear();
public: // struct methods
    bool isValid() const;
    QString toString( int i_iDetailLevel = 0 ) const;
public: // overridables
    ZS::System::SErrResultInfo serialize( QByteArray& io_byteArr ) const;
    ZS::System::SErrResultInfo deserialize( const QByteArray& i_byteArr );
public: // struct members
    SCnctId              m_cnctId;          // Unique identifier of the connection.
    QString              m_strClientName;   // Object name of the client. For InProcMsg sockets equal to "m_cnctId.m_pObjDbClt->objectName()".
    QVector<SDSNodeSpec> m_arDSNodeSpecs;   // Registered data set nodes for change indication events.

}; // struct SDbClientRegSpec


} // namespace Data

} // namespace ZS

#endif // #ifndef ZSDataCommon_h
