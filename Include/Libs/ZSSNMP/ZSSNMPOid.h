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

#ifndef ZSSNPMOid_h
#define ZSSNPMOid_h

#include <QtCore/qlist.h>
#include <QtCore/qvariant.h>
#include "ZSSNMP/Include/ZSSNMPDllMain.h"

namespace ZS
{
namespace SNMP
{
#ifndef MAX_OID_LEN
#define MAX_OID_LEN    128 /* max subid's in an oid */
#endif

class COid;
class CValue;

/*******************************************************************************
global type definitions and constants
*******************************************************************************/

typedef unsigned long  TSubId;
typedef QList<COid*>   TOidPtrList;
typedef QList<CValue*> TValuePtrList;

typedef enum
{
    EOidFormatSuffix  = 0,
    EOidFormatModule  = 1,
    EOidFormatFull    = 2,
    EOidFormatNumeric = 3,
    EOidFormatUcd     = 4,
    EOidFormatCount
}   EOidFormat;
ZSSNMPDLL_API QString oidFormat2Str( int i_iFormat );

typedef enum
{
    EValueTypeUndefined      =  0,
    EValueTypeNull           =  1,
    EValueTypeBoolean        =  2, // 
    EValueTypeInt            =  3, // 4 bytes
    EValueTypeUInt           =  4, // 4 bytes
    EValueTypeTimeTicks      =  5, // 4 bytes
    EValueTypeFloat          =  6,
    EValueTypeDouble         =  7,
    EValueTypeString         =  8,
    EValueTypeBitString      =  9,
    EValueTypeOctetString    = 10, // 0 .. 65535 bytes
    EValueTypeIPAddress      = 11, // 4 bytes
    EValueTypeCounter32      = 12, // 4 bytes
    EValueTypeCounter64      = 13, // 8 bytes
    EValueTypeOid            = 14,
    EValueTypeSequence       = 15,
    EValueTypeOpaque         = 16,
    EValueTypeNoSuchObject   = 17,
    EValueTypeNoSuchInstance = 18,
    EValueTypeEndOfMibView   = 19,
    EValueTypeCount
}   EValueType;
ZSSNMPDLL_API QString valueType2Str( int i_iValueType );

static const unsigned char EPDUDataTypeUndefined      =   0;
static const unsigned char EPDUDataTypeBoolean        =   1;
static const unsigned char EPDUDataTypeInteger        =   2; // 4 bytes
static const unsigned char EPDUDataTypeBitString      =   3;
static const unsigned char EPDUDataTypeBits           =   4;
static const unsigned char EPDUDataTypeOctetString    =   4; // 0 .. 65535 bytes
static const unsigned char EPDUDataTypeNull           =   5;
static const unsigned char EPDUDataTypeOid            =   6;
static const unsigned char EPDUDataTypeSequence       =  48;
static const unsigned char EPDUDataTypeSequenceOf     =  48;
static const unsigned char EPDUDataTypeVarBind        =  48;
static const unsigned char EPDUDataTypeIPAddress      =  64; // 4 bytes
static const unsigned char EPDUDataTypeCounter32      =  65; // 4 bytes
static const unsigned char EPDUDataTypeGauge32        =  66; // 4 bytes
static const unsigned char EPDUDataTypeUnsigned32     =  66; // 4 bytes
static const unsigned char EPDUDataTypeTimeTicks      =  67; // 4 bytes
static const unsigned char EPDUDataTypeOpaque         =  68;
static const unsigned char EPDUDataTypeCounter64      =  70; // 8 bytes
static const unsigned char EPDUDataTypeNoSuchObject   = 128;
static const unsigned char EPDUDataTypeNoSuchInstance = 129;
static const unsigned char EPDUDataTypeEndOfMibView   = 130;

ZSSNMPDLL_API EValueType pduDataType2ValueType( int i_iPduDataType );
ZSSNMPDLL_API QString pduDataType2Str( int i_iPduDataType );

//******************************************************************************
struct ZSSNMPDLL_API SOid
//******************************************************************************
{
public: // ctors and dtor
    SOid();
    SOid( const QString& i_strOid );
public: // struct methods
    QString toString( EOidFormat i_format = EOidFormatNumeric );
public: // struct members
    TSubId  m_aruSubIds[MAX_OID_LEN];
    int     m_iSubIdsCount;
    int     m_iSubIdsArrLen;

}; // struct SOid

//******************************************************************************
class ZSSNMPDLL_API COid
//******************************************************************************
{
public: // ctors and dtor
    COid();
    COid( const COid& i_oid );
    COid( const QString& i_strOid );
    COid( TSubId* i_puSubIds, int i_iSubIdsCount );
    ~COid();
public: // operators
    void operator = ( const COid& i_oid );
    bool operator == ( COid& i_oidOther );
    bool operator != ( COid& i_oidOther );
    bool operator == ( const QString& i_strOidOther );
    bool operator != ( const QString& i_strOidOther );
public: // instance methods
    QString toString( EOidFormat i_format = EOidFormatNumeric );
    SOid toOidStruct();
    TSubId* getSubIdsList( bool i_bCopyList = false );
    int getSubIdsCount();
    TSubId getSubId( int i_idx );
    bool isChildOf( COid* i_pOid );
protected: // instance methods
    void createSubIdsArr( SOid* i_pOid = NULL );
private: // instance members
    TSubId*  m_puSubIds;
    int      m_iSubIdsCount;
    QString  m_strOid;
    QString  m_arstrFormat[EOidFormatCount];

}; // class COid

//******************************************************************************
struct ZSSNMPDLL_API SValueCounter64
//******************************************************************************
{
    SValueCounter64() : m_uHighWord(0), m_uLowWord(0) { }
    unsigned long m_uHighWord;
    unsigned long m_uLowWord;
};

//******************************************************************************
class ZSSNMPDLL_API CValue
//******************************************************************************
{
public: // ctors and dtor
    CValue();
    CValue( const CValue& i_value );
    CValue( const COid& i_oid );
    CValue( const QString& i_strOid );
    CValue( TSubId* i_puSubIds, int i_iSubIdsCount );
    CValue( const COid& i_oid, EValueType i_valueType, char* i_pcValue, int i_iValLen, unsigned char i_pduDataType = EPDUDataTypeUndefined );
    CValue( const COid& i_oid, bool i_bValue );
    CValue( const COid& i_oid, qint32 i_iValue );
    CValue( const COid& i_oid, quint32 i_uValue, EValueType i_valueType = EValueTypeUInt ); // for UInt, TimeTicks, IPAddress, Counter32
    CValue( const COid& i_oid, double i_fValue, EValueType i_valueType = EValueTypeDouble ); // for float and double
    CValue( const COid& i_oid, QByteArray& i_byteArrValue, EValueType i_valueType = EValueTypeString ); // for String, BitString, OctetString
    CValue( const COid& i_oid, const SValueCounter64& i_counter64Value );
    CValue( const COid& i_oid, const COid& i_oidValue );
    CValue( const QString& i_strOid, EValueType i_valueType, char* i_pcValue, int i_iValLen, unsigned char i_pduDataType = EPDUDataTypeUndefined );
    CValue( const QString& i_strOid, bool i_bValue );
    CValue( const QString& i_strOid, qint32 i_iValue );
    CValue( const QString& i_strOid, quint32 i_uValue, EValueType i_valueType = EValueTypeUInt ); // for UInt, TimeTicks, IPAddress, Counter32
    CValue( const QString& i_strOid, double i_fValue, EValueType i_valueType = EValueTypeDouble ); // for float and double
    CValue( const QString& i_strOid, QByteArray& i_byteArrValue, EValueType i_valueType = EValueTypeString ); // for String, BitString, OctetString
    CValue( const QString& i_strOid, const SValueCounter64& i_counter64Value );
    CValue( const QString& i_strOid, const COid& i_oidValue );
    CValue( TSubId* i_puSubIds, int i_iSubIdsCount, EValueType i_valueType, char* i_pcValue, int i_iValLen, unsigned char i_pduDataType = EPDUDataTypeUndefined );
    CValue( TSubId* i_puSubIds, int i_iSubIdsCount, bool i_bValue );
    CValue( TSubId* i_puSubIds, int i_iSubIdsCount, qint32 i_iValue );
    CValue( TSubId* i_puSubIds, int i_iSubIdsCount, quint32 i_uValue, EValueType i_valueType = EValueTypeUInt ); // for UInt, TimeTicks, IPAddress, Counter32
    CValue( TSubId* i_puSubIds, int i_iSubIdsCount, double i_fValue, EValueType i_valueType = EValueTypeDouble ); // for float and double
    CValue( TSubId* i_puSubIds, int i_iSubIdsCount, QByteArray& i_byteArrValue, EValueType i_valueType = EValueTypeString ); // for String, BitString, OctetString
    CValue( TSubId* i_puSubIds, int i_iSubIdsCount, const SValueCounter64& i_counter64Value );
    CValue( TSubId* i_puSubIds, int i_iSubIdsCount, const COid& i_oidValue );
    ~CValue();
public: // operators
    void operator = ( const CValue& i_value );
public: // instance methods
    void setOid( const COid& i_oid );
    void setOid( const QString& i_strOid );
    void setOid( TSubId* i_puSubIds, int i_iSubIdsCount );
    COid* getOid();
    unsigned char getPDUDataType() const;
    void setValueType( EValueType i_valueType );
    EValueType getValueType() const;
    bool isInstanceNodeValueType() const; // returns true for value types that may indicated within the instance nodes model
    void setValue( EValueType i_valueType, char* i_pcValue, int i_iValLen );
    void setValue( bool i_bValue );
    void setValue( qint32 i_iValue );
    void setValue( quint32 i_uValue, EValueType i_valueType = EValueTypeUInt ); // for UInt, TimeTicks, IPAddress, Counter32
    void setValue( double i_fValue, EValueType i_valueType = EValueTypeDouble ); // for float and double
    void setValue( QByteArray& i_byteArrValue, EValueType i_valueType = EValueTypeString ); // for String, BitString, OctetString
    void setValue( const SValueCounter64& i_counter64Value );
    void setValue( const COid& i_oidValue );
    bool getValueBool() const;
    int getValueInt() const;
    int getValueUInt() const;
    float getValueFloat() const;
    double getValueDouble() const;
    QByteArray getValueString() const;
    SValueCounter64 getValueCounter64() const;
    COid* getValueOid();
private: // instance members
    COid            m_oid;
    unsigned char   m_pduDataType;
    EValueType      m_valueType;
    bool            m_bValue;
    qint32          m_iValue;
    quint32         m_uValue;
    double          m_fValue;
    QByteArray      m_byteArrValue;
    SValueCounter64 m_counter64Value;
    COid*           m_pOidValue;

}; // class CValue

} // namespace SNMP

} // namespace ZS

#endif // ZSSNPMOid_h
